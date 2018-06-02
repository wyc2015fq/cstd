/*
 * display_layout.c
 *
 * Copyright (C) 2010 Texas Instruments Incorporated - http://www.ti.com/
 * 
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation version 2.
 *
 * This program is distributed as is WITHOUT ANY WARRANTY of any
 * kind, whether express or implied; without even the implied warranty
 * of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */


#include <display_priv.h>

DISPLAY_LayoutCtrl gDISPLAY_layoutCtrl;

int DISPLAY_layoutInit()
{
    gDISPLAY_layoutCtrl.displayOffsetH = gDISPLAY_ctrl.frameInfo.lineOffsetH/2;
    gDISPLAY_layoutCtrl.displayOffsetV = gDISPLAY_ctrl.frameInfo.lineOffsetV;
    gDISPLAY_layoutCtrl.displayWidth   = gDISPLAY_ctrl.frameInfo.width;
    gDISPLAY_layoutCtrl.displayHeight  = gDISPLAY_ctrl.frameInfo.height;

    gDISPLAY_layoutCtrl.numPages = 2;
    gDISPLAY_layoutCtrl.curPage     = COLOR;

    // PAGE 0
    gDISPLAY_layoutCtrl.page[0].numChannels = 1;
    gDISPLAY_layoutCtrl.page[0].chanNum[0]  = 0;
    gDISPLAY_layoutCtrl.page[0].fbOffset[0] = 0;
    gDISPLAY_layoutCtrl.page[0].width       = gDISPLAY_ctrl.pCaptureInfo->chList.info[0].width;
    gDISPLAY_layoutCtrl.page[0].height      = gDISPLAY_ctrl.pCaptureInfo->chList.info[0].height;
    gDISPLAY_layoutCtrl.page[0].offsetH     = gDISPLAY_ctrl.pCaptureInfo->chList.info[0].offsetH;
    gDISPLAY_layoutCtrl.page[0].offsetV     = gDISPLAY_ctrl.pCaptureInfo->chList.info[0].offsetV;

    // PAGE 1
    gDISPLAY_layoutCtrl.page[1].numChannels = 1;
    gDISPLAY_layoutCtrl.page[1].chanNum[0]  = 1;
    gDISPLAY_layoutCtrl.page[1].fbOffset[0] = 0;
    gDISPLAY_layoutCtrl.page[1].width       = gDISPLAY_ctrl.pCaptureInfo->chList.info[1].width;
    gDISPLAY_layoutCtrl.page[1].height      = gDISPLAY_ctrl.pCaptureInfo->chList.info[1].height;
    gDISPLAY_layoutCtrl.page[1].offsetH     = gDISPLAY_ctrl.pCaptureInfo->chList.info[1].offsetH;
    gDISPLAY_layoutCtrl.page[1].offsetV     = gDISPLAY_ctrl.pCaptureInfo->chList.info[1].offsetV;

    //DISPLAY_layoutPrintCurPageProperties();

    return OSA_SOK;
}

int DISPLAY_layoutRun(DISPLAY_BufInfo *buf, short nextPage)
{
    int i, bufId,curPage,chNum;
    MCVIP_DmaPrm dmaPrm;
    MCVIP_BufInfo *pCaptureBufInfo;

    curPage = gDISPLAY_layoutCtrl.curPage;

    dmaPrm.copyWidth  = gDISPLAY_layoutCtrl.page[curPage].width;
    dmaPrm.copyHeight = gDISPLAY_layoutCtrl.page[curPage].height;

    dmaPrm.srcOffsetH = gDISPLAY_layoutCtrl.page[curPage].offsetH;
    dmaPrm.srcOffsetV = gDISPLAY_layoutCtrl.page[curPage].offsetV;

    dmaPrm.dstOffsetH = gDISPLAY_layoutCtrl.displayOffsetH;
    dmaPrm.dstOffsetV = gDISPLAY_layoutCtrl.displayOffsetV;

    dmaPrm.fillValueY  = DISPLAY_COLOR_NO_VIDEO_Y;
    dmaPrm.fillValueC  = DISPLAY_COLOR_NO_VIDEO_C;

    for(i = 0;i < gDISPLAY_layoutCtrl.page[curPage].numChannels;i ++)
    {
        dmaPrm.dstPhysAddr = NULL; // if NULL, the virtual to physical done inside DMA API
        dmaPrm.dstVirtAddr = buf->virtAddr + gDISPLAY_layoutCtrl.page[curPage].fbOffset[i];

        chNum = gDISPLAY_layoutCtrl.page[curPage].chanNum[i];
        bufId = gDISPLAY_ctrl.chInfo[chNum].curBufId;

        dmaPrm.srcPhysAddr = NULL; // fill dst with 'blank' data
        dmaPrm.srcVirtAddr = NULL; // fill dst with 'blank' data

        pCaptureBufInfo = MCVIP_getBufInfo(gDISPLAY_ctrl.pCaptureInfo->mcvipHndl, bufId);

        if(pCaptureBufInfo != NULL)
        {
            dmaPrm.srcPhysAddr = pCaptureBufInfo->physAddr;
            dmaPrm.srcVirtAddr = pCaptureBufInfo->virtAddr;
            MCVIP_dmaRun(&dmaPrm);
        }
    }

    return OSA_SOK;
}

int DISPLAY_layoutPrintCurPageProperties()
{
  int curPage,chanNum,i;

  curPage = gDISPLAY_layoutCtrl.curPage;

  printf("\n\n");
  printf(" PAGE %d PROPERTIES\n",curPage);
  printf(" ------------------\n");
  printf(" Num of Channels = %d\n\n",gDISPLAY_layoutCtrl.page[curPage].numChannels);
  for(i = 0;i < gDISPLAY_layoutCtrl.page[curPage].numChannels;i ++)
  { 
    chanNum = gDISPLAY_layoutCtrl.page[curPage].chanNum[i]; 
    printf(" Channel No = %d   :Width = %d ,Height = %d  , H = %d , V = %d\n",chanNum,
                                                           gDISPLAY_layoutCtrl.page[curPage].width,
                                                           gDISPLAY_layoutCtrl.page[curPage].height,
                                                           gDISPLAY_layoutCtrl.page[curPage].offsetH,
                                                           gDISPLAY_layoutCtrl.page[curPage].offsetV);
  }

  printf("\n\n");

  return OSA_SOK;
}
