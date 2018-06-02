/*
 * capture_tsk.c
 *
 * Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation version 2.
 *
 *
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <unistd.h>
#include <capture_priv.h>
#include <osa_cmem.h>
#include <osa_file.h>
#include <display.h>

CAPTURE_Ctrl   gCAPTURE_ctrl;

void CAPTURE_tskErrorExit()
{
    if( gCAPTURE_ctrl.memVirtAddr )
    {
        OSA_cmemFree( gCAPTURE_ctrl.memVirtAddr );
    }
}

int CAPTURE_tskCreate( CAPTURE_CreatePrm *prm )
{
    Uint32 allocMemSize;
    int i, status, curOffset;

    gCAPTURE_ctrl.saveFrame = FALSE;
    gCAPTURE_ctrl.saveFileIndex = 0;

    gCAPTURE_ctrl.memVirtAddr = NULL;
    gCAPTURE_ctrl.memPhysAddr = NULL;
    
    memset(&gCAPTURE_ctrl.info, 0, sizeof(gCAPTURE_ctrl.info));

    gCAPTURE_ctrl.info.createPrm.videoInputPort   = MCVIP_VIDEO_INPUT_PORT_0;
    gCAPTURE_ctrl.info.createPrm.captureThrPri    = (CAPTURE_THR_PRI + 1);
    gCAPTURE_ctrl.info.createPrm.videoDecoderId   = MCVIP_VIDEO_DECODER_ID_TVP5158;
    gCAPTURE_ctrl.info.createPrm.videoDecoderMode = prm->videoDecoderMode;
    gCAPTURE_ctrl.info.createPrm.videoIfMode      = prm->videoIfMode;
    gCAPTURE_ctrl.info.createPrm.videoSystem      = prm->videoSystem;
    gCAPTURE_ctrl.info.createPrm.numBuf           = MCVIP_getNumCh(prm->videoDecoderMode)*MCVIP_BUF_PER_CH_MIN;
    gCAPTURE_ctrl.info.createPrm.bufSize          = MCVIP_getBufSize(prm->videoDecoderMode, prm->videoSystem);

    allocMemSize = gCAPTURE_ctrl.info.createPrm.numBuf * gCAPTURE_ctrl.info.createPrm.bufSize;

    gCAPTURE_ctrl.memVirtAddr = OSA_cmemAlloc(allocMemSize, 32);
    if(gCAPTURE_ctrl.memVirtAddr==NULL)
    {
        OSA_ERROR("OSA_cmemAlloc()\r\n");
        return OSA_EFAIL;
    }

    atexit( CAPTURE_tskErrorExit );
    
    gCAPTURE_ctrl.memPhysAddr = OSA_cmemGetPhysAddr( gCAPTURE_ctrl.memVirtAddr );
    if(gCAPTURE_ctrl.memPhysAddr==NULL)
    {
        OSA_ERROR("OSA_cmemGetPhysAddr()\r\n");
        OSA_cmemFree(gCAPTURE_ctrl.memVirtAddr);
        return OSA_EFAIL;
    }
#if 0
    //MAT - Debug
    OSA_printf( "\nVideoPort:%d"
                "\nThrPri:%d"
                "\nVidecID:%d"
                "\nVidecMode:%d"
                "\nVidIfMode:%d"
                "\nVidSys:%d"
                "\nNumBuf:%d"
                "\nBufSize:%d"
                "\nAllocMem:%d",
                gCAPTURE_ctrl.info.createPrm.videoInputPort,
                gCAPTURE_ctrl.info.createPrm.captureThrPri,
                gCAPTURE_ctrl.info.createPrm.videoDecoderId,
                gCAPTURE_ctrl.info.createPrm.videoDecoderMode,
                gCAPTURE_ctrl.info.createPrm.videoIfMode,
                gCAPTURE_ctrl.info.createPrm.videoSystem,
                gCAPTURE_ctrl.info.createPrm.numBuf,
                gCAPTURE_ctrl.info.createPrm.bufSize,
                allocMemSize);
#endif

    curOffset = 0;
    for(i=0; i<gCAPTURE_ctrl.info.createPrm.numBuf; i++)
    {
        gCAPTURE_ctrl.info.createPrm.bufVirtAddr[i] = gCAPTURE_ctrl.memVirtAddr + curOffset;
        gCAPTURE_ctrl.info.createPrm.bufPhysAddr[i] = gCAPTURE_ctrl.memPhysAddr + curOffset;
        curOffset += gCAPTURE_ctrl.info.createPrm.bufSize;
        //OSA_printf(" PHYS = %x, VIRT = %x\n", (Uint32)gCAPTURE_ctrl.info.createPrm.bufPhysAddr[i], (Uint32)gCAPTURE_ctrl.info.createPrm.bufVirtAddr[i]);
    }

    gCAPTURE_ctrl.info.mcvipHndl = MCVIP_create(&gCAPTURE_ctrl.info.createPrm);

    if(gCAPTURE_ctrl.info.mcvipHndl==NULL)
    {
        OSA_ERROR("MCVIP_create()\r\n");
        OSA_cmemFree(gCAPTURE_ctrl.memVirtAddr);
        return OSA_EFAIL;
    }

    status = MCVIP_getChList(gCAPTURE_ctrl.info.mcvipHndl, &gCAPTURE_ctrl.info.chList);
    OSA_assertSuccess(status);

    OSA_assert(gCAPTURE_ctrl.info.chList.numCh < MCVIP_CHANNELS_MAX);

    for( i=0; i<gCAPTURE_ctrl.info.chList.numCh; i++ )
    {
        status = OSA_queCreate(&gCAPTURE_ctrl.info.displayQue[i], MCVIP_BUF_PER_CH_MIN);
        OSA_assertSuccess(status);
    }
    return OSA_SOK;
}

int CAPTURE_tskDelete( )
{
    int i, status=OSA_SOK;

    if(gCAPTURE_ctrl.info.mcvipHndl!=NULL)
    {
        status = MCVIP_delete(gCAPTURE_ctrl.info.mcvipHndl);
    }

    if(gCAPTURE_ctrl.memVirtAddr != NULL)
    {
        status |= OSA_cmemFree(gCAPTURE_ctrl.memVirtAddr);
        gCAPTURE_ctrl.memVirtAddr = NULL;
    }
    
    for(i=0; i<gCAPTURE_ctrl.info.chList.numCh; i++)
    {
        status |= OSA_queDelete(&gCAPTURE_ctrl.info.displayQue[i]);
    }

    return status;
}

int CAPTURE_tskStart( )
{
    return MCVIP_start(gCAPTURE_ctrl.info.mcvipHndl);
}

int CAPTURE_tskStop( )
{
    return 0;
}

int CAPTURE_tskSaveFrame( MCVIP_BufInfo *pBufInfo)
{
    static int count = 0;
    int status;
    if( count++ % 20 == 0 )
    {
        char filename[20];
        MCVIP_ChInfo *pChInfo;
        Uint32 fileSize;


        sprintf(filename, "IMG_%d_%04d.YUV", pBufInfo->chId , count /*gCAPTURE_ctrl.saveFileIndex*/);

        pChInfo = &gCAPTURE_ctrl.info.chList.info[pBufInfo->chId];
        fileSize = pChInfo->offsetH * pChInfo->offsetV * 2;

        gCAPTURE_ctrl.saveFileIndex++;

        OSA_printf("Saving frame of size %dx%d  %s\n", pChInfo->offsetH, pChInfo->offsetV , filename);

        status = OSA_fileWriteFile(filename, pBufInfo->virtAddr, fileSize);
    }
    return status;
}

int CAPTURE_tskStartRun(OSA_TskHndl *pTsk, OSA_MsgHndl *pMsg, Bool *isDone, Bool *doAckMsg )
{
    int status, bufId;
    struct timeval    tval;
    Bool done = FALSE, ackMsg = TRUE;
    Uint16 cmd;
    MCVIP_BufInfo *pBufInfo;

    *isDone = FALSE;
    *doAckMsg = FALSE;
    status = CAPTURE_tskStart();

    OSA_printf("CAPTURE task started\n");
    OSA_tskAckOrFreeMsg( pMsg , status );
    if(status != OSA_SOK)
    {
        return status;
    }
    OSA_printf("CAPTURE task enter main loop\n");
    while( !done )
    {
        Uint32 start = OSA_getCurTimeInMsec();
        bufId = -1; pBufInfo = NULL;
        status = MCVIP_getBuf( gCAPTURE_ctrl.info.mcvipHndl, &bufId, OSA_TIMEOUT_NONE );
        if( OSA_SOK == status )
        {
            OSA_assert( bufId != -1 );
            pBufInfo = MCVIP_getBufInfo( gCAPTURE_ctrl.info.mcvipHndl, bufId );
            OSA_assert( pBufInfo && INFRARED == pBufInfo->chId );
            if( gCAPTURE_ctrl.m_CallBack )
            {
                MCVIP_ChInfo*  pChInfo = &gCAPTURE_ctrl.info.chList.info[ pBufInfo->chId ];
                gCAPTURE_ctrl.m_CallBack(
                        (char*)pBufInfo->virtAddr ,
                        pChInfo->offsetH ,
                        pChInfo->offsetV ,
                        pChInfo->offsetH*2 ,
                        gCAPTURE_ctrl.m_Param , pBufInfo->chId );
            }
            MCVIP_putBuf( gCAPTURE_ctrl.info.mcvipHndl , bufId , MCVIP_FLAG_ENCODER_DONE );
        }
        else if( gCAPTURE_ctrl.m_CallBack )
        {
            gCAPTURE_ctrl.m_CallBack( 0 , 0 , 0 , 0 , gCAPTURE_ctrl.m_Param , 0 );
        }
        if( OSA_getCurTimeInMsec() - start > 60 )
        {
            bufId = -1;
            while( OSA_SOK ==  MCVIP_getBuf( gCAPTURE_ctrl.info.mcvipHndl, &bufId, OSA_TIMEOUT_NONE ) )
            {
                OSA_assert( bufId != -1 );
                MCVIP_putBuf( gCAPTURE_ctrl.info.mcvipHndl , bufId , MCVIP_FLAG_ENCODER_DONE );
                bufId = -1;
            }
        }

        OSA_waitMsecs( 40 );

        status = OSA_tskCheckMsg( pTsk , &pMsg );
        if( status == OSA_SOK )
        {
            cmd = OSA_msgGetCmd( pMsg );
            switch(cmd)
            {
            case CAPTURE_CMD_STOP:
                OSA_printf( "CAPTURE_CMD_STOP0\n" );
                done = TRUE;
                ackMsg = TRUE;
                break;
            case CAPTURE_CMD_DELETE:
                done = TRUE;
                *isDone = TRUE;
                *doAckMsg = TRUE;
                break;
            default:
                OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
                break;
            }
        }
    }

    CAPTURE_tskStop();
    OSA_printf( "CAPTURE_CMD_STOP1\n" );
    if(ackMsg)
    {
        OSA_printf( "CAPTURE_CMD_STOP2\n" );
        OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
    }
    OSA_printf( "CAPTURE_CMD_STOP3\n" );
    return status;
}

int CAPTURE_tskMain(struct OSA_TskHndl *pTsk, OSA_MsgHndl *pMsg, Uint32 curState )
{
    int status;
    Bool done = FALSE, ackMsg = FALSE;
    Uint16 cmd = OSA_msgGetCmd( pMsg );
    CAPTURE_CreatePrm *pCreatePrm = (CAPTURE_CreatePrm*)OSA_msgGetPrm(pMsg);

    if( cmd != CAPTURE_CMD_CREATE || pCreatePrm==NULL )
    {
        OSA_tskAckOrFreeMsg( pMsg , OSA_EFAIL );
        return OSA_SOK;
    }

    status = CAPTURE_tskCreate( pCreatePrm );

    OSA_tskAckOrFreeMsg(pMsg, status);

    if(status != OSA_SOK)
    {
        return OSA_SOK;
    }

    while(!done)
    {
        OSA_printf( "CAPTURE_tskMain---OSA_tskWaitMsg\n" );
        status = OSA_tskWaitMsg(pTsk, &pMsg);
        if( status != OSA_SOK )
        {
            done = TRUE;
            break;
        }
        cmd = OSA_msgGetCmd(pMsg);
        switch(cmd)
        {
        case CAPTURE_CMD_START:
            OSA_printf( "CAPTURE_CMD_START  \n" );
            CAPTURE_tskStartRun(pTsk, pMsg, &done, &ackMsg);
            break;

        case CAPTURE_CMD_DELETE:
            OSA_printf( "CAPTURE_CMD_DELETE  \n" );
            done = TRUE;
            ackMsg = TRUE;
            break;

        default:
            OSA_printf( "CAPTURE_tskMain default::OSA_tskAckOrFreeMsg  \n" );
            OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
            break;
        }
    }

    OSA_printf( "CAPTURE_tskDelete 1  \n" );
    CAPTURE_tskDelete();
    if( ackMsg )
    {
        OSA_tskAckOrFreeMsg( pMsg , OSA_SOK );
    }
    OSA_printf( "CAPTURE_tskDelete 2  \n" );
    return OSA_SOK;
}


int CAPTURE_sendCmd(Uint16 cmd, void *prm, Uint16 flags )
{
    return OSA_mbxSendMsg( &gCAPTURE_ctrl.tskHndl.mbxHndl, &gCAPTURE_ctrl.mbxHndl, cmd, prm, flags );
}

int CAPTURE_create(CAPTURE_CreatePrm *prm)
{
    int status;

    status = OSA_tskCreate( &gCAPTURE_ctrl.tskHndl, CAPTURE_tskMain, CAPTURE_THR_PRI, CAPTURE_STACK_SIZE, 0);

    OSA_assertSuccess(status);

    status = OSA_mbxCreate( &gCAPTURE_ctrl.mbxHndl);

    OSA_assertSuccess(status);


    status = CAPTURE_sendCmd(CAPTURE_CMD_CREATE, prm, OSA_MBX_WAIT_ACK );

    return status;
}

int CAPTURE_delete()
{
    int status;

    status = CAPTURE_sendCmd(CAPTURE_CMD_DELETE, NULL, OSA_MBX_WAIT_ACK );

    status = OSA_tskDelete( &gCAPTURE_ctrl.tskHndl );

    OSA_assertSuccess(status);
    
    status = OSA_mbxDelete( &gCAPTURE_ctrl.mbxHndl);

    OSA_assertSuccess(status);

    return status;
}

int CAPTURE_start()
{
    return CAPTURE_sendCmd(CAPTURE_CMD_START, NULL, OSA_MBX_WAIT_ACK );
}

int CAPTURE_stop()
{
    OSA_printf( "CAPTURE_stop~~~~~~~~~~~~~1\n" );
    MCVIP_stop( gCAPTURE_ctrl.info.mcvipHndl );
    OSA_printf( "CAPTURE_stop~~~~~~~~~~~~~2\n" );
    int kk = CAPTURE_sendCmd(CAPTURE_CMD_STOP, NULL, OSA_MBX_WAIT_ACK );
    OSA_printf( "CAPTURE_stop~~~~~~~~~~~~~3\n" );
    return kk;
}

int CAPTURE_saveFrame()
{
    gCAPTURE_ctrl.saveFrame = TRUE;

    MCVIP_saveFrame(gCAPTURE_ctrl.info.mcvipHndl);

    return OSA_SOK;
}

CAPTURE_Info *CAPTURE_getInfo()
{
    return &gCAPTURE_ctrl.info;
}

