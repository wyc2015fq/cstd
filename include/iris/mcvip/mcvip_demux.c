/*
 * mcvip_demux.h
 *
 * TVP5158 decoder previlege mode defined.
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

#include <mcvip_priv.h>
#include <capture_priv.h>
#include <display_priv.h>
#include <osa_cmem.h>

extern CAPTURE_Ctrl gCAPTURE_ctrl;
extern DISPLAY_LayoutCtrl gDISPLAY_layoutCtrl;

#define MCVIP_DEMUX_DEBUG

//#define MCVIP_DEBUG_HANG

#define MCVIP_XY_F0_BLANK 0xAB
#define MCVIP_XY_F0_VALID   0x80
#define MCVIP_XY_F1_BLANK 0xEC
#define MCVIP_XY_F1_VALID   0xC7

#define MCVIP_XY_INVALID      0xFF

#define MCVIP_TVP5158_METADATA_LENGTH   (4*2)

#define TVP5158_BOP       (1<<6)
#define TVP5158_EOP       (1<<5)

#define TVP5158_CH_VALID  ((1<<7)|(1<<31))
#define TVP5158_VDET      (1<<12)
#define TVP5158_EOL       (1<<13)
#define TVP5158_BOL       (1<<14)

#define TVP5158_F_BIT		  (1<<30)
#define TVP5158_DUMMY		  (0x01010101)

#define MCVIP_SKIP_COUNT  (0) //(12)
#define MCVIP_START_LINE_NUM  (0)

int MCVIP_demuxInit(MCVIP_Hndl *hndl)
{
    int i, status;
    MCVIP_BufInfo *pBufInfo;
    
    for(i=0; i<hndl->chList.numCh; i++)
    {
        hndl->chDemuxInfo[i].errorFrame=0;
        hndl->chDemuxInfo[i].errorLine=0;
        hndl->chDemuxInfo[i].curField = 0;
        hndl->chDemuxInfo[i].curLine[0] = 0;
        hndl->chDemuxInfo[i].curLine[1] = 0;
        hndl->chDemuxInfo[i].isFirstFrame = 1;
        hndl->chDemuxInfo[i].skipCount = MCVIP_SKIP_COUNT;

        hndl->chDemuxInfo[i].prevXY  = MCVIP_XY_INVALID;
        hndl->chDemuxInfo[i].prevLineNum  = MCVIP_START_LINE_NUM-1;

        status = OSA_queGet(&hndl->emptyQue[i], &hndl->chDemuxInfo[i].curBufId, OSA_TIMEOUT_FOREVER);
        OSA_assertSuccess(status);

        pBufInfo = MCVIP_getBufInfo((void*)hndl, hndl->chDemuxInfo[i].curBufId);
        pBufInfo->chId = i;

        OSA_assert(pBufInfo!=NULL);

        hndl->chDemuxInfo[i].curDstPhysAddr = pBufInfo->physAddr;
        hndl->chDemuxInfo[i].curDstVirtAddr = pBufInfo->virtAddr;
    }

    for(i=0; i<MCVIP_BUF_MAX; i++)
    {
        hndl->tmpDemuxQue[i] = -1;
    }

    hndl->dmaDemux.srcChromaOffset = 0; // since in DM355 UV data is interleaved with Y
    hndl->dmaDemux.dstChromaOffset = 0; // since in DM355 UV data is interleaved with Y
    hndl->dmaDemux.copyWidth    = hndl->chList.info[0].width * 2; // since in DM355 UV data is interleaved with Y
    hndl->dmaDemux.srcPhysAddrList = hndl->dmaDemuxSrcPhysAddr;
    hndl->dmaDemux.dstPhysAddrList = hndl->dmaDemuxDstPhysAddr;

    hndl->chBase = 0;
    if( (hndl->createPrm.videoDecoderMode == MCVIP_VIDEO_DECODER_MODE_2CH_D1) ||
        (hndl->createPrm.videoDecoderMode == MCVIP_VIDEO_DECODER_MODE_2CH_D1_CROP) )
    {
        hndl->chBase = 2;
    }

    hndl->initTimeInMsecs = OSA_getCurTimeInMsec();

    return OSA_SOK;
}

int MCVIP_demuxRun(MCVIP_Hndl *hndl, MCVIP_V4l2Buf *buf)
{
    int tmpDemuxQueIdx=0, i, numCh, eol, extraCh;
    volatile Uint8 *curAddr;
    Uint16 chId, lineNum=0;
    Uint8 *destAddr;
    Uint8 *srcAddr;
    Bool isDone;
    MCVIP_ChDemuxInfo *pChInfo;
    MCVIP_BufInfo *pBufInfo;
    Uint32 destAddrOffset, srcAddrOffset, dstLineOffsetH, curTime;
    volatile Uint32 metaData, validFlag;
    Uint8 curXY;

    hndl->dmaDemux.numLines = 0;
    dstLineOffsetH = hndl->chList.info[0].offsetH;
    numCh  = hndl->chList.numCh;
    curAddr = buf->virtAddr;

    ///////////modified by zpq 2011.03.27/////////////////
    //validFlag = TVP5158_CH_VALID|TVP5158_VDET|TVP5158_EOL|TVP5158_BOL;
    validFlag = TVP5158_CH_VALID|TVP5158_EOL|TVP5158_BOL;
    //////////////////////////////////////////////////////

    curTime = (OSA_getCurTimeInMsec() - hndl->initTimeInMsecs)*1000; // in usecs
    //    for(i=0; i<numCh; i++)
    //    {
    //        pChInfo = &hndl->chDemuxInfo[i];
    //        if(pChInfo->isFirstFrame)
    //        {
    //            if(pChInfo->skipCount)
    //            {
    //                pChInfo->skipCount--;
    //            }
    //        }
    //        else
    //        {
    //            pChInfo->skipCount = 0;
    //        }
    //    }

    for( i=0; i<hndl->v4l2FrameInfo.height; i++)
    {
        extraCh =  eol = 0;
        chId       = 0xFF;
        metaData =  ((Uint32)curAddr[1] << 0 ) | ((Uint32)curAddr[3] << 8 ) | ((Uint32)curAddr[5] << 16 ) | ((Uint32)curAddr[7] << 24 );
        ///////////modified by zpq 2011.03.27/////////////////
        //        if( ( (metaData & (1<<7))>>7 ) == 0 )
        //        {
        //            OSA_printf("TVP5158_CH_VALID1 = %d\n", (metaData & (1<<7))>>7);
        //            OSA_printf("TVP5158_CH_VALID2 = %d\n", (metaData & (1<<31))>>31);
        //            OSA_printf("TVP5158_VDET = %d\n", (metaData & (1<<12))>>12);
        //            OSA_printf("TVP5158_EOL = %d\n", (metaData & (1<<13))>>13);
        //            OSA_printf("TVP5158_BOL = %d\n", (metaData & (1<<14))>>14);
        //            OSA_printf("lineNum = %d\n", ((metaData & 0x007F0000) >> 16)|((metaData & 0x00000300) >> 1));
        //        }
        //////////////////////////////////////////////////////
        if( (metaData & TVP5158_CH_VALID) == TVP5158_CH_VALID && (metaData & TVP5158_VDET)==0 )
        {
            chId = metaData & 0x07;
            pChInfo = &hndl->chDemuxInfo[chId];
            pChInfo->curLine[0]=0;
            pChInfo->curLine[1]=0;
            pChInfo->isFirstFrame = 1;
            pChInfo->errorFrame = 0;
            pChInfo->errorLine = 0;
            pChInfo->prevLineNum = MCVIP_START_LINE_NUM-1;
            pChInfo->skipCount = MCVIP_SKIP_COUNT;
            chId = 0xFF;
        }
        if( (metaData & validFlag) == validFlag )
        {
            chId = metaData & 0x07;
            if( hndl->chBase && chId >= hndl->chBase )
            {
                chId  -= hndl->chBase;
            }
        }
        else if( (metaData & (TVP5158_CH_VALID|TVP5158_VDET)) == (TVP5158_CH_VALID|TVP5158_VDET))
        { // extra D1 channel
            chId = numCh-1;
            extraCh = 1;
            if(metaData & TVP5158_EOL)
            {
                eol = 1;
            }
        }
        if( chId < numCh )
        {
            lineNum = ((metaData & 0x007F0000) >> 16)|((metaData & 0x00000300) >> 1);
            if( lineNum>(hndl->chList.info[chId].height/2 - 1) )
            {
                chId = 0xFF; //skip line
            }
        }        
        if( chId < numCh )
        {
            pChInfo = &hndl->chDemuxInfo[chId];
            curXY = metaData >> 24;
            if(curXY == MCVIP_XY_F0_VALID || curXY == MCVIP_XY_F0_BLANK)
            {
                pChInfo->curField = 0;
            }
            else if(curXY == MCVIP_XY_F1_VALID || curXY == MCVIP_XY_F1_BLANK)
            {
                pChInfo->curField = 1;
            }
            else
            {
                chId = 0xFF;
            }
        }
        if( chId < numCh )
        {
            if( pChInfo->skipCount )
            {
                chId = 0xFF;
            }
            if(chId < numCh)
            {
                if( pChInfo->isFirstFrame && pChInfo->curField == 1 )
                {
                    // sync to even field
                    chId = 0xFF;
                    //OSA_printf(" MCVIP: Syncing to even field\n");
                }
                else
                {
                    pChInfo->isFirstFrame = 0;
                }
            }
        }
        if( chId < numCh )
        {
            if( pChInfo->curBufId == 0xFE )
            {
                if( OSA_EFAIL == OSA_queGet( &hndl->emptyQue[chId], &pChInfo->curBufId , OSA_TIMEOUT_NONE ) )
                {
                    pChInfo->errorFrame = 1;
                    pChInfo->errorLine++;
                }
                else
                {
                    //OSA_printf("                                                                              Demux   Ch %d: New %d\n", chId, pChInfo->curBufId);
                    pBufInfo = MCVIP_getBufInfo((void*)hndl, pChInfo->curBufId);
                    OSA_assert(pBufInfo!=NULL);
                    pBufInfo->chId = chId;
                    pChInfo->curDstVirtAddr = pBufInfo->virtAddr;
                    pChInfo->curDstPhysAddr = pBufInfo->physAddr;
                }
            }
            if( pChInfo->curBufId != 0xFE )
            {
                pChInfo->curLine[pChInfo->curField] = lineNum;
                if( (pChInfo->prevLineNum+1) != lineNum)
                {
                    pChInfo->errorFrame = 1;
                    pChInfo->errorLine++;
                    //OSA_printf("ch=%d L=(%d, %d) i=%d\n", chId,lineNum, pChInfo->prevLineNum, i);
                }
            }
            if( (!extraCh) || (extraCh && eol) )
            {
                pChInfo->prevLineNum  = lineNum;
            }
            if( pChInfo->curBufId != 0xFE && pChInfo->errorLine < 3 )
            {
                destAddrOffset =  2 * pChInfo->curLine[pChInfo->curField] * dstLineOffsetH
                                  +  pChInfo->curField * dstLineOffsetH
                                  + eol*hndl->chList.info[0].width;

                destAddrOffset *= 2;

                srcAddrOffset = (curAddr-buf->virtAddr)+MCVIP_TVP5158_METADATA_LENGTH;

                destAddr = pChInfo->curDstPhysAddr + destAddrOffset;
                srcAddr  = buf->physAddr + srcAddrOffset;

                hndl->dmaDemuxSrcPhysAddr[hndl->dmaDemux.numLines] = (unsigned long)srcAddr;
                hndl->dmaDemuxDstPhysAddr[hndl->dmaDemux.numLines] = (unsigned long)destAddr;
                hndl->dmaDemux.numLines++;
            }
            if( lineNum==(hndl->chList.info[chId].height/2 - 1) )
            {
                pChInfo->prevLineNum = MCVIP_START_LINE_NUM-1;
            }
            isDone = FALSE;
            if(pChInfo->curLine[0]== (hndl->chList.info[chId].height/2 - 1) &&
               pChInfo->curLine[1]== (hndl->chList.info[chId].height/2 - 1) )
            {// valid frame, all lines received
                if(extraCh)
                {
                    if(eol)
                    {
                        isDone = TRUE;
                    }
                }
                else
                {
                    isDone = TRUE;
                }
            }
            if( isDone )
            {
                pChInfo->curLine[0] = 0;
                pChInfo->curLine[1] = 0;

                pChInfo->isFirstFrame = 1;
                pChInfo->skipCount = 0;
                pChInfo->prevLineNum = MCVIP_START_LINE_NUM-1;

                //OSA_printf( "OSA_queGet chid = %d , bufid = %d\n" , chId , pChInfo->curBufId );
                if( !pChInfo->errorFrame || (pChInfo->errorFrame && pChInfo->errorLine < 3) )
                {
                    hndl->tmpDemuxQue[tmpDemuxQueIdx] = pChInfo->curBufId;
                    tmpDemuxQueIdx++;
                    pBufInfo = MCVIP_getBufInfo((void*)hndl, pChInfo->curBufId);
                    pBufInfo->timestamp = curTime;

                    //OSA_printf(" Demux   Ch %d: Buf %d\n", chId, pChInfo->curBufId);
                    if( OSA_EFAIL == OSA_queGet( &hndl->emptyQue[chId] , &pChInfo->curBufId, OSA_TIMEOUT_NONE ) )
                    {
                        pChInfo->curBufId = 0xFE;
                    }

                    if( pChInfo->curBufId == 0xFF )
                    {
                        //OSA_printf( "#########################OSA_EFAIL\n" );
                        return OSA_EFAIL;
                    }
                    //OSA_printf(" Demux   Ch %d: New %d\n", chId, pChInfo->curBufId);

                    if( pChInfo->curBufId !=  0xFE )
                    {
                        pBufInfo = MCVIP_getBufInfo((void*)hndl, pChInfo->curBufId);
                        OSA_assert(pBufInfo!=NULL);
                        pBufInfo->chId = chId;
                        pChInfo->curDstVirtAddr = pBufInfo->virtAddr;
                        pChInfo->curDstPhysAddr = pBufInfo->physAddr;
                    }
                }
                // if( pChInfo->errorFrame )
                // {
                //     OSA_printf(" Frame %5d: Missing lines %d ch=%d i=%d\n", hndl->frameCount, pChInfo->errorLine,chId,i);
                // }
                pChInfo->errorFrame = 0;
                pChInfo->errorLine = 0;
            }
        }
        curAddr += hndl->v4l2FrameInfo.offsetH * 2;
    }

    DRV_dmaDemux( &hndl->dmaDemuxHndl, &hndl->dmaDemux );

    for( i=0; i<tmpDemuxQueIdx; i++ )
    {
        if( hndl->tmpDemuxQue[i] >= 0 )
        {
            MCVIP_BufInfo* pTemp = MCVIP_getBufInfo( (void*)hndl , hndl->tmpDemuxQue[i] );
            OSA_assert( pTemp != NULL );
            OSA_assert( 0 == pTemp->flags );
            if( pTemp->chId == INFRARED )
            {
                if( OSA_EFAIL == OSA_quePut( &hndl->fullQue, hndl->tmpDemuxQue[i] , OSA_TIMEOUT_NONE ) )
                {
                    pTemp->flags |= MCVIP_FLAG_ENCODER_DONE;
                }   
            }
            else
            {
                pTemp->flags |= MCVIP_FLAG_ENCODER_DONE;
            }
            if( pTemp->chId == gDISPLAY_layoutCtrl.curPage )
            {
                if( OSA_EFAIL == OSA_quePut( &gCAPTURE_ctrl.info.displayQue[ pTemp->chId ] , hndl->tmpDemuxQue[i] , OSA_TIMEOUT_NONE ) )
                {
                    pTemp->flags |= MCVIP_FLAG_DISPLAY_DONE;
                }
            }
            else
            {
                pTemp->flags |= MCVIP_FLAG_DISPLAY_DONE;
            }
            if( (pTemp->flags & MCVIP_FLAG_ALL_DONE ) == MCVIP_FLAG_ALL_DONE  )
            {
                MCVIP_putBuf(  hndl , hndl->tmpDemuxQue[i] , pTemp->flags );
            }
            hndl->tmpDemuxQue[i] = -1;
        }
    }
    return OSA_SOK;
}

