/*
 * audio_tsk.c
 *
 * Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed “as is” WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include <audio_priv.h>
//#include <setparam.h>
//#include <fdefine.h>
//#include <define.h>

#define PROFILE_FRAME_COUNT 200

//#define TEST_TDM
//#define PRINT_TIME

AUDIO_Ctrl gAUDIO_ctrl;

int AUDIO_tskCreate(AUDIO_CreatePrm *prm)
{
  int status=0;

  status = OSA_mutexCreate(&gAUDIO_ctrl.mutexLock); 

  if(status!=OSA_SOK) {
    OSA_ERROR("OSA_mutexCreate()\n");
    return status;
  }

  gAUDIO_ctrl.audPlayCh  = prm->audPlayCh;
  gAUDIO_ctrl.pMcvipHndl = prm->pMcvipHndl;
  gAUDIO_ctrl.pCreatePrm = prm->pCreatePrm;

  return status;
}

int AUDIO_tskDelete( )
{
  int status=0;

  OSA_mutexDelete(&gAUDIO_ctrl.mutexLock);
  //OSA_printf(" MCASP: AUDIO_tskDelete\n"); // AYK - 0728
  return status;
}

int prevTimeRec,totalTimeRec = 0;

int AUDIO_tskStartRun(OSA_TskHndl *pTsk, OSA_MsgHndl *pMsg, Bool *isDone, Bool *doAckMsg )
{
    int status,i = 0;
    Bool done = FALSE, ackMsg = TRUE;
    Uint16 cmd;

#ifdef TEST_TDM
    int tdmSlot = 0;
#endif

    *isDone = FALSE;
    *doAckMsg = FALSE;

    status = AUDIO_Init();

		if(status != OSA_SOK)
    {
        return status;
    }

    OSA_tskAckOrFreeMsg(pMsg, status);

#ifdef TEST_TDM
    OSA_printf(" AUD: Current TDM slot = %d\n",tdmSlot);
#endif

    while(!done)
    {
		prevTimeRec = OSA_getCurTimeInMsec();

		status = AUDIO_Record();
		totalTimeRec  += OSA_getCurTimeInMsec() - prevTimeRec;

	  if(status != OSA_SOK)
	  {
			OSA_printf(" MCVIP: Audio Record failed\n");
	  }

		if(gAUDIO_ctrl.pCreatePrm->enablePlayback == 1)
		{
		    // Demultiplex the TDM samples
			for(i = 0;i < AUDIO_SAMPLES; i += 16)
			{
			  //Always record 2 TDM channels
				//Select Ch1,Ch2 or Ch3,Ch4, or ... Ch15,Ch16
				gAUDIO_ctrl.playBuf[i>>3] = gAUDIO_ctrl.recBuf[i + gAUDIO_ctrl.audPlayCh];
				gAUDIO_ctrl.playBuf[(i>>3)+1] = gAUDIO_ctrl.recBuf[i + gAUDIO_ctrl.audPlayCh + 8];
			}

			// Play the audio samples
			if(gAUDIO_ctrl.enableMute==0)        

				status = AUDIO_Play();         
						
			if(status != OSA_SOK)
			    OSA_printf(" MCVIP: Audio Play failed.\n");
		}

#ifdef PRINT_TIME

	    frameCount ++;

	    if(frameCount == PROFILE_FRAME_COUNT)
	    {
			OSA_printf(" AUD:record sample rate = %f KHz\n",(float)AUDIO_SAMPLES_DEMUX * PROFILE_FRAME_COUNT/totalTimeRec);
			OSA_printf(" AUD:Audio Channel[%d]\n",gAUDIO_ctrl.audPlayCh+1);
			frameCount   = 0;
			totalTimeRec = 0;

#ifdef TEST_TDM
			tdmSlot ++;
			if(tdmSlot == 16)
			{
			    tdmSlot = 0;
			}
			OSA_printf(" AUD: Current TDM slot = %d\n",tdmSlot);
#endif

		}
#endif

        status = OSA_tskCheckMsg(pTsk, &pMsg);
        if(status == OSA_SOK)
        {
            cmd = OSA_msgGetCmd(pMsg);

            switch(cmd)
            {
                case AUDIO_CMD_STOP:
                    done = TRUE;
                    ackMsg = TRUE;
                    break;

                case AUDIO_CMD_DELETE:
                    done = TRUE;
                    *isDone = TRUE;
                    *doAckMsg = TRUE;
                    break;

				        case AUDIO_CMD_INCVOL:
                    AUDIO_incVol();
                    OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
                    break;

				        case AUDIO_CMD_DECVOL:
                    AUDIO_decVol();
                    OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
                    break;

				        case AUDIO_CMD_NEXTPLAYCHAN:
                    AUDIO_nextPlayChan();
                    OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
                    break;

                default:
                    OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
                    break;
            } //switch(cmd)
        } //if(status == OSA_SOK)
    } //while(!done)

	AUDIO_Exit();

	if(ackMsg)
    	OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);

	return status;
}

int AUDIO_tskMain(struct OSA_TskHndl *pTsk, OSA_MsgHndl *pMsg, Uint32 curState )
{
  int status;
  Bool done = FALSE, ackMsg=FALSE;;
  Uint16 cmd = OSA_msgGetCmd(pMsg);
  AUDIO_CreatePrm *pCreatePrm = (AUDIO_CreatePrm*)OSA_msgGetPrm(pMsg);

  if( cmd != AUDIO_CMD_CREATE || pCreatePrm==NULL) {
    OSA_tskAckOrFreeMsg(pMsg, OSA_EFAIL);
    return OSA_SOK;
  }

  status = AUDIO_tskCreate( pCreatePrm );

  OSA_tskAckOrFreeMsg(pMsg, status);

  if(status != OSA_SOK)
    return OSA_SOK;

  while(!done)
  {
    status = OSA_tskWaitMsg(pTsk, &pMsg);
    if(status != OSA_SOK) {
      done = TRUE;
      break;
    }

    cmd = OSA_msgGetCmd(pMsg);

    switch(cmd) {
      case AUDIO_CMD_START:
        AUDIO_tskStartRun(pTsk, pMsg, &done, &ackMsg);
        break;

      case AUDIO_CMD_DELETE:
        done = TRUE;
        ackMsg = TRUE;
        break;

      default:
        OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);
        break;
    }
  }

  AUDIO_tskDelete();

  if(ackMsg)
    OSA_tskAckOrFreeMsg(pMsg, OSA_SOK);

  return OSA_SOK;
}
int AUDIO_sendCmd(Uint16 cmd, void *prm, Uint16 flags )
{
  return OSA_mbxSendMsg(&gAUDIO_ctrl.tskHndl.mbxHndl, &gAUDIO_ctrl.mbxHndl, cmd, prm, flags);
}

int AUDIO_create(AUDIO_CreatePrm *prm)
{
  int status;
  status = OSA_tskCreate( &gAUDIO_ctrl.tskHndl, AUDIO_tskMain, AUDIO_THR_PRI, AUDIO_STACK_SIZE, 0);

  OSA_assertSuccess(status);

  status = OSA_mbxCreate( &gAUDIO_ctrl.mbxHndl);

  OSA_assertSuccess(status);

  status = AUDIO_sendCmd(AUDIO_CMD_CREATE, prm, OSA_MBX_WAIT_ACK );

  return status;
}

int AUDIO_delete()
{
  int status;

  status = AUDIO_sendCmd(AUDIO_CMD_DELETE, NULL, OSA_MBX_WAIT_ACK );

  status = OSA_tskDelete( &gAUDIO_ctrl.tskHndl );

  OSA_assertSuccess(status);

  status = OSA_mbxDelete( &gAUDIO_ctrl.mbxHndl);

  OSA_assertSuccess(status);

  return status;
}

int AUDIO_start()
{
  return AUDIO_sendCmd(AUDIO_CMD_START, NULL, OSA_MBX_WAIT_ACK );
}

int AUDIO_stop()
{
  return AUDIO_sendCmd(AUDIO_CMD_STOP, NULL, OSA_MBX_WAIT_ACK );
}

int AUDIO_increaseVolume()
{
  return AUDIO_sendCmd(AUDIO_CMD_INCVOL, NULL, OSA_MBX_WAIT_ACK );
}

int AUDIO_decreaseVolume()
{
  return AUDIO_sendCmd(AUDIO_CMD_DECVOL, NULL, OSA_MBX_WAIT_ACK );
}

int AUDIO_nextPlayChannels()
{
  return AUDIO_sendCmd(AUDIO_CMD_NEXTPLAYCHAN, NULL, OSA_MBX_WAIT_ACK );
}

int AUDIO_playback(short *PlayBuf, int inPlayBufSize)
{
    int ret = 0 ;

    //ret = MCVIP_audioPlayback(gAUDIO_ctrl.pMcvipHndl,(char *)PlayBuf,AUDIO_BUF_SIZE_DEMUX);
    ret = MCVIP_audioPlayback(gAUDIO_ctrl.pMcvipHndl,(char *)PlayBuf, inPlayBufSize*2); // cause short => char

    if(ret < 0)
    {
        return OSA_EFAIL;
    }
    return OSA_SOK;
}

