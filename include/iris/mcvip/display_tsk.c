/*
 * display_tsk.c
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
#include <display_priv.h>

extern DISPLAY_LayoutCtrl gDISPLAY_layoutCtrl;
DISPLAY_Ctrl gDISPLAY_ctrl= { 0 };

int DISPLAY_tskCreate( DISPLAY_CreatePrm *prm)
{
    int status, i;

    memset( &gDISPLAY_ctrl.chInfo, 0, sizeof(gDISPLAY_ctrl.chInfo) );
    gDISPLAY_ctrl.blankDetectThres = 60;
    gDISPLAY_ctrl.pCaptureInfo = prm->captureInfo;

    if( gDISPLAY_ctrl.pCaptureInfo == NULL )
    {
        return OSA_EFAIL;
    }
    
    status = DISPLAY_v4l2Create(prm->displayMode, &gDISPLAY_ctrl.frameInfo);
    if( status != OSA_SOK)
    {
        return status;
    }
    
    DISPLAY_layoutInit();
    for(i=0; i<gDISPLAY_ctrl.pCaptureInfo->chList.numCh; i++)
    {
        gDISPLAY_ctrl.chInfo[i].curBufId = -1;
    }

    return status;
}

int DISPLAY_tskDelete( )
{
    return DISPLAY_v4l2Delete();
}

int DISPLAY_getCaptureChIds()
{
    int i , bufId;
    for( i = 0; i<gDISPLAY_ctrl.pCaptureInfo->chList.numCh; i++)
    {
        bufId = -1;
        OSA_queGet( &gDISPLAY_ctrl.pCaptureInfo->displayQue[i] , &bufId , OSA_TIMEOUT_NONE );
        if( i != gDISPLAY_layoutCtrl.curPage )
        {
            if(  -1 != bufId )
            {
                MCVIP_putBuf( gDISPLAY_ctrl.pCaptureInfo->mcvipHndl, bufId , MCVIP_FLAG_DISPLAY_DONE );
            }
            continue;
        }
        else if( -1 != bufId )
        {
            if( gDISPLAY_ctrl.chInfo[i].curBufId != -1 )
            {
                MCVIP_putBuf( gDISPLAY_ctrl.pCaptureInfo->mcvipHndl, gDISPLAY_ctrl.chInfo[i].curBufId, MCVIP_FLAG_DISPLAY_DONE );
            }

            gDISPLAY_ctrl.chInfo[i].curBufId = bufId;
            gDISPLAY_ctrl.chInfo[i].blankCount = 0;

            MCVIP_BufInfo* pBufInfo = MCVIP_getBufInfo( gDISPLAY_ctrl.pCaptureInfo->mcvipHndl, bufId );
            MCVIP_ChInfo*  pChInfo = &gDISPLAY_ctrl.pCaptureInfo->chList.info[ pBufInfo->chId ];

            if( gDISPLAY_ctrl.m_CallBack && gDISPLAY_layoutCtrl.curPage == pBufInfo->chId )
            {
                if( gDISPLAY_ctrl.m_CallBack(
                        (char*)pBufInfo->virtAddr,
                        pChInfo->offsetH,
                        pChInfo->offsetV,
                        pChInfo->offsetH*2 ,
                        gDISPLAY_ctrl.m_Param , pBufInfo->chId ) )
                {
                    bufId = -1;
                    while( OSA_SOK ==  OSA_queGet( &gDISPLAY_ctrl.pCaptureInfo->displayQue[i] , &bufId , OSA_TIMEOUT_NONE ) )
                    {
                        OSA_printf( "gDISPLAY_ctrl.m_CallBack > 0 Free  %d\r\n" ,  bufId );
                        if( bufId != -1 )
                        {
                            MCVIP_putBuf( gDISPLAY_ctrl.pCaptureInfo->mcvipHndl, bufId , MCVIP_FLAG_DISPLAY_DONE );
                            bufId = -1;
                        }
                    }
                }
            }
        }
        else
        {    
#if 1
            gDISPLAY_ctrl.chInfo[i].blankCount++;
            if( gDISPLAY_ctrl.chInfo[i].blankCount >= gDISPLAY_ctrl.blankDetectThres )
            {
                gDISPLAY_ctrl.chInfo[i].blankCount = 0;//gDISPLAY_ctrl.blankDetectThres;
                if( gDISPLAY_ctrl.chInfo[i].curBufId != -1 )
                {
                    MCVIP_putBuf( gDISPLAY_ctrl.pCaptureInfo->mcvipHndl, gDISPLAY_ctrl.chInfo[i].curBufId, MCVIP_FLAG_DISPLAY_DONE);
                    gDISPLAY_ctrl.chInfo[i].curBufId = -1;
                }
            }
#endif
        }
    }
    return OSA_SOK;
}

int DISPLAY_tskStartRun(OSA_TskHndl *pTsk, OSA_MsgHndl *pMsg, Bool *isDone, Bool *doAckMsg )
{
    Bool done = FALSE, ackMsg = TRUE,nextPage = FALSE;
    Uint16 cmd;
    DISPLAY_BufInfo displayBufInfo;
    struct timeval    tval;
    int status = DISPLAY_v4l2Start();
    *isDone  = *doAckMsg = FALSE;

    OSA_tskAckOrFreeMsg(pMsg, status);
    if( status != OSA_SOK )
    {
        return status;
    }

    OSA_printf( "Enter Display main loop\n" );
    while( !done )
    {
        DISPLAY_v4l2GetBuf( &displayBufInfo );
        DISPLAY_getCaptureChIds();
        DISPLAY_layoutRun( &displayBufInfo , nextPage );
        nextPage = FALSE;
        DISPLAY_v4l2PutBuf( &displayBufInfo );

        OSA_waitMsecs( 2 );
        status = OSA_tskCheckMsg( pTsk , &pMsg );
        if(status==OSA_SOK)
        {
            cmd = OSA_msgGetCmd( pMsg );
            switch(cmd)
            {
            case DISPLAY_CMD_STOP:
                done = TRUE;
                ackMsg = TRUE;
                break;
            case DISPLAY_CMD_DELETE:
                done = TRUE;
                *isDone = TRUE;
                *doAckMsg = TRUE;
                break;
            case DISPLAY_CMD_NEXTPAGE:
                nextPage = TRUE;
                OSA_tskAckOrFreeMsg( pMsg, OSA_SOK );
                break;
            default:
                OSA_tskAckOrFreeMsg( pMsg, OSA_SOK );
                break;
            }
        }
    }

    DISPLAY_v4l2Stop();
    if( ackMsg )
    {
        OSA_tskAckOrFreeMsg( pMsg , OSA_SOK );
    }
    OSA_printf( "DISPLAY_CMD_STOP~~~~~~~~~~~~~~~~~~~\n" );
    return status;
}

int DISPLAY_tskMain(struct OSA_TskHndl *pTsk, OSA_MsgHndl *pMsg, Uint32 curState )
{
    int status;
    Bool done = FALSE, ackMsg=FALSE;;
    Uint16 cmd = OSA_msgGetCmd(pMsg);
    DISPLAY_CreatePrm *pCreatePrm = (DISPLAY_CreatePrm*)OSA_msgGetPrm(pMsg);

    if( cmd != DISPLAY_CMD_CREATE || pCreatePrm==NULL)
    {
        OSA_tskAckOrFreeMsg(pMsg, OSA_EFAIL);
        return OSA_SOK;
    }

    status = DISPLAY_tskCreate( pCreatePrm );
    OSA_tskAckOrFreeMsg(pMsg, status);
    if(status != OSA_SOK)
    {
        return OSA_SOK;
    }

    while(!done)
    {
        OSA_printf( "DISPLAY_tskMain---OSA_tskWaitMsg\n" );
        status = OSA_tskWaitMsg(pTsk, &pMsg);
        if(status != OSA_SOK)
        {
            done = TRUE;
            break;
        }
        cmd = OSA_msgGetCmd( pMsg );
        switch(cmd)
        {
        case DISPLAY_CMD_START:
            DISPLAY_tskStartRun(pTsk, pMsg, &done, &ackMsg);
            break;
        case DISPLAY_CMD_DELETE:
            done = TRUE;
            ackMsg = TRUE;
            break;
        default:
            OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
            break;
        }
    }

    DISPLAY_tskDelete();
    if(ackMsg)
    {
        OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
    }
    return OSA_SOK;
}


int DISPLAY_sendCmd(Uint16 cmd, void *prm, Uint16 flags )
{
    return OSA_mbxSendMsg(&gDISPLAY_ctrl.tskHndl.mbxHndl, &gDISPLAY_ctrl.mbxHndl, cmd, prm, flags);
}

int DISPLAY_create(DISPLAY_CreatePrm *prm)
{
    int status;

    status = OSA_tskCreate( &gDISPLAY_ctrl.tskHndl, DISPLAY_tskMain, DISPLAY_THR_PRI, DISPLAY_STACK_SIZE, 0);

    OSA_assertSuccess(status);
    
    status = OSA_mbxCreate( &gDISPLAY_ctrl.mbxHndl);

    OSA_assertSuccess(status);

    status = DISPLAY_sendCmd(DISPLAY_CMD_CREATE, prm, OSA_MBX_WAIT_ACK );

    return status;
}

int DISPLAY_delete()
{
    int status;

    status = DISPLAY_sendCmd(DISPLAY_CMD_DELETE, NULL, OSA_MBX_WAIT_ACK );

    status = OSA_tskDelete( &gDISPLAY_ctrl.tskHndl );

    OSA_assertSuccess(status);
    
    status = OSA_mbxDelete( &gDISPLAY_ctrl.mbxHndl);

    OSA_assertSuccess(status);

    return status;
}

int DISPLAY_start()
{
    return DISPLAY_sendCmd(DISPLAY_CMD_START, NULL, OSA_MBX_WAIT_ACK );
}

int DISPLAY_stop()
{
    return DISPLAY_sendCmd(DISPLAY_CMD_STOP, NULL, OSA_MBX_WAIT_ACK );
}

int DISPLAY_nextPage()
{
    return DISPLAY_sendCmd(DISPLAY_CMD_NEXTPAGE, NULL, OSA_MBX_WAIT_ACK );
}


