/*
 * mcvip_tsk.h
 *
 * Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation version 2.
 *

 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <unistd.h>
#include <mcvip_priv.h>
#include <osa_file.h>

int MCVIP_tskCreate( MCVIP_Hndl *hndl)
{
    int status, i, chId, numBufPerCh;

    if(hndl->createPrm.numBuf > MCVIP_BUF_MAX)
    {
        return OSA_EFAIL;
    }

    hndl->chList.numCh = MCVIP_getNumCh(hndl->createPrm.videoDecoderMode);

    if(hndl->createPrm.numBuf < hndl->chList.numCh * MCVIP_BUF_PER_CH_MIN)
    {
        OSA_ERROR("Insufficient number of buffers allocated by user, need at least %d buffers\n", hndl->chList.numCh*3);
        return OSA_EFAIL;
    }
    
    hndl->saveFrame = FALSE;
    hndl->saveFileIndex = 0;

    for(i=0; i<MCVIP_TVP5158_MAX_CASCADE; i++)
    {
        if(gMCVIP_ctrl.i2cHndl[hndl->createPrm.videoInputPort][i].fd >=0 )
        {
            hndl->pI2cHndl[i] = &gMCVIP_ctrl.i2cHndl[hndl->createPrm.videoInputPort][i];
        }
        else
        {
            hndl->pI2cHndl[i] = NULL;
        }
    }
    
    status = MCVIP_v4l2Create(hndl);
    if(status!=OSA_SOK)
    {
        return status;
    }

    status = DRV_dmaOpen(&hndl->dmaDemuxHndl, DRV_DMA_MODE_DEMUX, DRV_DMA_MAX_DEMUX_TRANSFERS);
    if(status!=OSA_SOK)
    {
        return status;
    }

    for(i=0; i<hndl->chList.numCh; i++)
    {
        MCVIP_getChInfo(hndl->createPrm.videoDecoderMode,
                        hndl->createPrm.videoSystem,
                        &hndl->chList.info[i]);
    }

    if(hndl->createPrm.videoDecoderMode==MCVIP_VIDEO_DECODER_MODE_4CH_HALF_D1_PLUS_D1 ||
       hndl->createPrm.videoDecoderMode==MCVIP_VIDEO_DECODER_MODE_4CH_CIF_PLUS_D1     ||
       hndl->createPrm.videoDecoderMode==MCVIP_VIDEO_DECODER_MODE_8CH_CIF_PLUS_D1 )
    {
        hndl->chList.info[hndl->chList.numCh-1].width = 720;

        if(hndl->createPrm.videoSystem==MCVIP_VIDEO_SYSTEM_NTSC)
            hndl->chList.info[hndl->chList.numCh-1].height = 480;
        else
            hndl->chList.info[hndl->chList.numCh-1].height = 576;
    }
    else if(hndl->createPrm.videoDecoderMode==MCVIP_VIDEO_DECODER_MODE_4CH_HALF_D1_PLUS_D1_CROP ||
            hndl->createPrm.videoDecoderMode==MCVIP_VIDEO_DECODER_MODE_4CH_CIF_PLUS_D1_CROP     ||
            hndl->createPrm.videoDecoderMode==MCVIP_VIDEO_DECODER_MODE_8CH_CIF_PLUS_D1_CROP)
    { //MAT - Should 1-Ch D1 be cropped?
        hndl->chList.info[hndl->chList.numCh-1].width = 720; //704?

        if(hndl->createPrm.videoSystem==MCVIP_VIDEO_SYSTEM_NTSC)
            hndl->chList.info[hndl->chList.numCh-1].height = 480;
        else
            hndl->chList.info[hndl->chList.numCh-1].height = 576;
    }

    status = OSA_queCreate(&hndl->fullQue, hndl->createPrm.numBuf);
    OSA_assertSuccess(status);

    numBufPerCh = hndl->createPrm.numBuf/hndl->chList.numCh;

    for(chId=0; chId<hndl->chList.numCh; chId++)
    {
        status = OSA_queCreate(&hndl->emptyQue[chId], numBufPerCh);
        OSA_assertSuccess(status);
    }

    status = OSA_mutexCreate(&hndl->putBufLock);
    OSA_assertSuccess(status);

    chId = 0;
    for(i=0; i<hndl->createPrm.numBuf; i++)
    {
        hndl->bufInfo[i].flags = 0;
        hndl->bufInfo[i].chId = -1;
        hndl->bufInfo[i].physAddr = hndl->createPrm.bufPhysAddr[i];
        hndl->bufInfo[i].virtAddr = hndl->createPrm.bufVirtAddr[i];
        hndl->bufInfo[i].timestamp = 0;

        if( i>0 && (i%numBufPerCh)==0  )
        {
            chId++;
        }
        
        status = OSA_quePut(&hndl->emptyQue[chId], i, OSA_TIMEOUT_FOREVER);
        OSA_assertSuccess(status);

#ifdef MCVIP_CLEAR_BUF_DURING_INIT
        {
            MCVIP_DmaPrm dmaPrm = { 0 };
            // clear buffer with blank data
            dmaPrm.srcPhysAddr = NULL;
            dmaPrm.srcVirtAddr = NULL;
            dmaPrm.dstPhysAddr = hndl->createPrm.bufPhysAddr[i];
            dmaPrm.dstVirtAddr = hndl->createPrm.bufVirtAddr[i];
            dmaPrm.srcOffsetH  = 0;
            dmaPrm.srcOffsetV  = 0;
            dmaPrm.dstOffsetH  = hndl->chList.info[chId].offsetH;
            dmaPrm.dstOffsetV  = hndl->chList.info[chId].offsetV;
            dmaPrm.copyWidth   = hndl->chList.info[chId].width  ;
            dmaPrm.copyHeight  = hndl->chList.info[chId].height ;
            dmaPrm.fillValueY  = 0x00800080;
            dmaPrm.fillValueC  = 0x00000000;

            MCVIP_dmaRun( &dmaPrm );
        }
#endif
    }

    MCVIP_demuxInit(hndl);

    return OSA_SOK;
}

int MCVIP_tskDelete(MCVIP_Hndl *hndl )
{
    int status, i;

    status = MCVIP_v4l2Delete(hndl);
    OSA_assertSuccess(status);

    status = OSA_queDelete(&hndl->fullQue);
    OSA_assertSuccess(status);
    
    for(i=0; i<hndl->chList.numCh; i++)
    {
        status = OSA_queDelete(&hndl->emptyQue[i]);
        OSA_assertSuccess(status);
    }
    
    status = OSA_mutexDelete(&hndl->putBufLock);
    OSA_assertSuccess(status);

    DRV_dmaClose(&hndl->dmaDemuxHndl);

    return status;
}

int MCVIP_tskStart(MCVIP_Hndl *hndl)
{
    return MCVIP_v4l2Start(hndl);
}

int MCVIP_tskStop(MCVIP_Hndl *hndl)
{
    return MCVIP_v4l2Stop( hndl );
}

int MCVIP_tskSaveFrame( MCVIP_Hndl *hndl, MCVIP_V4l2Buf *pBufInfo)
{
    char filename[20] = { 0 };
    Uint32 fileSize = 0;
    int status;

    if( !hndl->saveFrame )
    {
        return 0;
    }

    hndl->saveFrame = FALSE;
    sprintf(filename, "MCV_%04d.BIN", hndl->saveFileIndex);
    fileSize = hndl->v4l2FrameInfo.offsetH * hndl->v4l2FrameInfo.offsetV * 2;
    OSA_printf("MCVIP: Saving frame of size %dx%d\n", hndl->v4l2FrameInfo.offsetH, hndl->v4l2FrameInfo.offsetV);
    status = OSA_fileWriteFile(filename, pBufInfo->virtAddr, fileSize);
    hndl->saveFileIndex++;

    return status;
}

int MCVIP_tskStartRun(MCVIP_Hndl *hndl, OSA_TskHndl *pTsk, OSA_MsgHndl *pMsg, Bool *isDone, Bool *doAckMsg )
{
    int status;
    Bool done = FALSE, ackMsg = FALSE;
    Uint16 cmd;
    MCVIP_V4l2Buf v4l2Buf = { 0 };
    *doAckMsg = *isDone = FALSE;
    status = MCVIP_tskStart( hndl );
    
    OSA_tskAckOrFreeMsg( pMsg , status );
    if( status != OSA_SOK )
    {
        return status;
    }
    while( !done )
    {
        MCVIP_v4l2GetBuf( hndl, &v4l2Buf );
        status = MCVIP_demuxRun(hndl, &v4l2Buf);
        MCVIP_v4l2PutBuf(hndl, &v4l2Buf);
        if( status != OSA_SOK )
        {
            break;
        }
        status = OSA_tskCheckMsg(pTsk, &pMsg);
        if(status==OSA_SOK)
        {
            cmd = OSA_msgGetCmd( pMsg );
            switch(cmd)
            {
            case MCVIP_CMD_STOP:
                done = TRUE;
                ackMsg = TRUE;
                OSA_printf( "MCVIP_CMD_STOP OSA_tskCheckMsg %x\n" , cmd );
                break;

            case MCVIP_CMD_DELETE:
                done = TRUE;
                *isDone = TRUE;
                *doAckMsg = TRUE;
                OSA_printf( "MCVIP_CMD_DELETE OSA_tskCheckMsg %x\n" , cmd );
                break;

            default:
                OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
                break;
            }
        }
    }
    MCVIP_tskStop( hndl );
    OSA_quePut( &hndl->fullQue , 0xFF , OSA_TIMEOUT_FOREVER );
    if( ackMsg )
    {
        OSA_tskAckOrFreeMsg( pMsg, OSA_SOK );
    }
    OSA_printf( "MCVIP_CMD_STOP  exit %x\n" , cmd );
    return status;
}

int MCVIP_tskMain(struct OSA_TskHndl *pTsk, OSA_MsgHndl *pMsg, Uint32 curState )
{
    int status;
    Bool done = FALSE, ackMsg=FALSE;
    Uint16 cmd = OSA_msgGetCmd(pMsg);
    MCVIP_Hndl *pHndl = (MCVIP_Hndl*)OSA_msgGetPrm(pMsg);

    if( cmd != MCVIP_CMD_CREATE || pHndl==NULL)
    {
        OSA_tskAckOrFreeMsg(pMsg, OSA_EFAIL);
        return OSA_SOK;
    }

    status = MCVIP_tskCreate( pHndl );

    OSA_tskAckOrFreeMsg(pMsg, status);

    if(status != OSA_SOK)
    {
        return OSA_SOK;
    }

    while(!done)
    {
        OSA_printf( "MCVIP_tskMain-OSA_tskWaitMsg 1\n" );
        status = OSA_tskWaitMsg(pTsk, &pMsg);
        OSA_printf( "MCVIP_tskMain-OSA_tskWaitMsg 2\n" );
        if(status != OSA_SOK)
        {
            done = TRUE;
            break;
        }

        cmd = OSA_msgGetCmd(pMsg);

        switch(cmd)
        {
        case MCVIP_CMD_START:
            MCVIP_tskStartRun(pHndl, pTsk, pMsg, &done, &ackMsg);
            break;

        case MCVIP_CMD_DELETE:
            done = TRUE;
            ackMsg = TRUE;
            OSA_printf( "MCVIP_CMD_DELETE \n" );
            break;

        default:
            OSA_printf( "MCVIP_tskMain-default \n" );
            OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
            break;
        }
    }
    OSA_printf( "MCVIP_tskDelete 1 \n" );
    MCVIP_tskDelete(pHndl);
    if(ackMsg)
    {
        OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
    }
    OSA_printf( "MCVIP_tskDelete 2 \n" );
    return OSA_SOK;
}

