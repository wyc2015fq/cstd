int caperoad_capnum()
{
  WORD wIndex = 0;
  WORD okdeviceNum = 0;
  CCyUSBDevice* pUSBDev = new CCyUSBDevice(NULL);
  int devcnt = pUSBDev->DeviceCount();
  for (wIndex = 0; wIndex < devcnt; wIndex++) {
    if (pUSBDev->Open((UCHAR)wIndex)) {
      CCyControlEndPoint* ept = pUSBDev->ControlEndPt;
      ept->Target = TGT_DEVICE;
      ept->ReqType = REQ_VENDOR;
      ept->Direction = DIR_TO_DEVICE;
      ept->ReqCode = 0xa3;
      ept->Value = 0;
      ept->Index = 0;
      char buf[64];
      ZeroMemory(buf, 64);
      LONG bytesToSend = 64;
      ept->Read((PUCHAR)buf, bytesToSend);
      ept->Read((PUCHAR)buf, bytesToSend);
      if (0x02 == buf[11]) {
        //DEBUG_INFO1(_T("%s"), _T("our device"));
        ++okdeviceNum;
      }
    }
  }
  delete pUSBDev;
  return okdeviceNum;
}
int caperoad_getframe(capdev* s0, img_t* im, int flag)
{
  eroad_dev* s = (eroad_dev*)(s0->x);
  img_t im1[1] = {0};
  int i, len = s->height * s->width;
#if 0
  int wIndex = s->wIndex;
  //if (0 && s->pUSBDev->Open((UCHAR)wIndex))
  {
    // SumFrameCount++;
    int ccc = 0;
    while (s->pUSBDev) {
      long nTemp = s->number;
      s->pUSBDev->BulkInEndPt->XferData(s->data, nTemp);
      //printf("%d %d\n", wIndex, nTemp);
      if ((int)nTemp == (int)s->number) {
        //break;
        ++ccc;
      }
      if (ccc > 1) {
        break;
      }
    }
  }
#endif
  if (s->data) {
    uchar* buf = (uchar*)memdup(s->data, len);
    //上位视频对比度
    int upcontrast = 255;
    //设置上位对比度阀值
    int ThresholdValue = 255;
    ColorSpace outtype = T_BGR;
    IMINIT(im1, s->height, s->width, buf, s->width, 1, 1);
    for (i = 0; i < len / 2; ++i) {
      int t = buf[i];
      buf[i] = buf[len - 1 - i], buf[len - 1 - i] = t;
    }
    if (CAP_GRAY == flag) {
      outtype = T_GRAY;
    }
    imcolorcvt(im1, im, T_BayerGR, outtype);
    pfree(buf);
    return true;
  }
  return 0;
}
int caperoad_open(capdev* s0, int wIndex, int width, int height)
{
  eroad_dev* s = NULL;
  int devcnt = caperoad_capnum();
  wIndex = MAX(wIndex, 0);
  if (NULL==s0 || wIndex >= devcnt) {
    return wIndex;
  }
  s = new eroad_dev;
  s0->x = s;
  s0->getframe = caperoad_getframe;
  s0->close = caperoad_close;
  s->FlagVerticalMirror = 0;//垂直镜像标志
  s->width = width;
  s->height = height;
  s->pUSBDev = new CCyUSBDevice(NULL);
  //devcnt = s->pUSBDev->DeviceCount();
  s->wIndex = wIndex;
  if (wIndex >= devcnt) {
    delete s;
    s0->x = 0;
    return 0;
  }
  if (!s->pUSBDev->Open((UCHAR)wIndex)) {
    delete s;
    s0->x = 0;
    return 0;
  }
  CCyControlEndPoint* ept;
  ept = s->pUSBDev->ControlEndPt;
  ept->Target = TGT_DEVICE;
  ept->ReqType = REQ_VENDOR;
  ept->Direction = DIR_TO_DEVICE;
  ept->ReqCode = 0xa3;
  ept->Value = 0;
  ept->Index = 0;
  BYTE buf[64];
  char buf2[64];
  ZeroMemory(buf, 64);
  LONG bytesToSend = 64;
  ept->Read((PUCHAR)buf, bytesToSend);
  ept->Read((PUCHAR)buf, bytesToSend);
  BOOL breakOk = FALSE;
  if (0x02 != buf[11]) {
    delete s;
    s0->x = 0;
    return 0;
  }
  ept = s->pUSBDev->ControlEndPt;
  ept->Target = TGT_DEVICE;
  ept->ReqType = REQ_VENDOR;
  ept->Direction = DIR_TO_DEVICE;
  ept->ReqCode = 0xa2;
  ept->Value = 0;
  ept->Index = 0;
  ZeroMemory(buf, 64);
  bytesToSend = 64;
  buf[0] = (BYTE)0xa2;
  buf[1] = (BYTE)0x06;
  ept->Write((PUCHAR)buf, bytesToSend);
  OVERLAPPED inOvLap;
  _snprintf(buf2, 64, _T("CYUSB_IN%d"), wIndex);
  inOvLap.hEvent = CreateEvent(NULL, false, false, buf2);
  breakOk = s->AutoPlayFlag;
  SetVideoModel(s0, 0);
  SetFrameRate(s0, 15);
  OpenLight(s0, 1);
  caperoad_set_manual_saturation(s0, 1);
  SetManualcontrast(s0, 1);
  caperoad_set_sensitometry(s0, 140);
  SetManuallight(s0);
  caperoad_set_brightness(s0, 35);
  s->AutoPlayFlag = TRUE;
  s->ImageDisposal_ing = FALSE;
  s->pUSBDev->BulkInEndPt->SetXferSize(s->number);
  s->inContext = s->pUSBDev->BulkInEndPt->BeginDataXfer(s->data, s->number, &inOvLap);
  if (!s->pUSBDev->BulkInEndPt->WaitForXfer(&inOvLap, 1000)) {
    delete s;
    s0->x = 0;
    return 0;
  }
  SetVideoModel(s0, 0);
  SetFrameRate(s0, 15);
  OpenLight(s0, 1);
  caperoad_set_manual_saturation(s0, 1);
  SetManualcontrast(s0, 1);
  caperoad_set_sensitometry(s0, 140);
  caperoad_set_brightness(s0, 60);
  SetManuallight(s0);
  s->AutoPlayFlag = TRUE;
  s->ImageDisposal_ing = FALSE;
  caperoad_set_manual_saturation(s0, 1);
  caperoad_set_sensitometry(s0, 140);
#if 1
  if (NULL != s->hThread) {
    if (WaitForSingleObject(s->hThread, 1000) != WAIT_OBJECT_0) {
      TerminateThread(s->hThread, 100);
    }
    s->hThread = NULL;
  }
  unsigned int dwThreadID1;
  s->hThread = (HANDLE)_beginthreadex(NULL, 0, EroadVideoThread, s, 0, &dwThreadID1);
#endif
  return wIndex+1;
}

