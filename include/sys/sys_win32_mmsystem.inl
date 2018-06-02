////////////////////////////////////////////////////////////////////////////////
#include <mmsystem.h>
#ifdef _MSC_VER
#pragma comment(lib, "Winmm.lib")
#endif // _MSC_VER

typedef struct waveout_t waveout_t;
typedef int (*wavegetdata_f)(waveout_t* w, short* out, int len);

#define MAXWAVEBUFFER 4096

typedef struct waveout_t {
  char pBuffer1[2*MAXWAVEBUFFER];
  WAVEHDR pWaveHdr1[2];
  HWAVEOUT hWaveOut;
  HANDLE hCallBackEvent;
  const void* data;
  int len;
  int bit;
  int fs;
  int pos;
  //int stop;
  wavegetdata_f getdata;
} waveout_t;

static int _wav_getdata(waveout_t* w, short* out) {
  if (w) {
    int i, len = MAXWAVEBUFFER/2;
    if ((len + w->pos) > w->len) {
      len = w->len - w->pos;
    }
    if (w->getdata) {
      len = w->getdata(w, out, len);
    } else if (w->data) {
      if (16==w->bit) {
        memcpy(out, ((short*)w->data) + w->pos, len*sizeof(short));
      } else {
        const unsigned char* pc = ((const unsigned char*)w->data) + w->pos;
        for (i=0; i<len; ++i) {
          out[i] = (pc[i] * 256) - 32768;
        }
      }
    }
    w->pos += len;
    printf("pos = %d\n", w->pos);
    if (w->pos>=w->len) {
      //w->stop = 1;
      SetEvent(w->hCallBackEvent);
    }
  }
  return 0;
}

static int CALLBACK waveOutProc(HWAVEOUT hWaveOut,UINT uMsg, DWORD_PTR dwInstance,DWORD dwParam1,DWORD dwParam2)
{
  LPWAVEHDR pWaveHeader = (LPWAVEHDR)dwParam1;
  waveout_t* w = (waveout_t*)dwInstance;
  switch (uMsg) {
  case MM_WOM_OPEN:
    /* FillBuffer (pBuffer1, iFreq);
    waveOutWrite (hWaveOut, pWaveHdr1, sizeof (WAVEHDR));
    FillBuffer (pBuffer2, iFreq);
    waveOutWrite (hWaveOut, pWaveHdr2, sizeof (WAVEHDR));*/
    return TRUE;
  case MM_WOM_DONE:
    waveOutUnprepareHeader(hWaveOut, pWaveHeader, sizeof(WAVEHDR));
    _wav_getdata(w, (short*)pWaveHeader->lpData);
    waveOutPrepareHeader(hWaveOut, pWaveHeader, sizeof(WAVEHDR));
    waveOutWrite(hWaveOut, pWaveHeader, sizeof(WAVEHDR));
    return TRUE;
  case MM_WOM_CLOSE:
    waveOutUnprepareHeader(hWaveOut, w->pWaveHdr1, sizeof(WAVEHDR));
    waveOutUnprepareHeader(hWaveOut, w->pWaveHdr1+1, sizeof(WAVEHDR));
    hWaveOut = NULL;
    return TRUE;
  default:
    return TRUE;
  }
}
static int wavplay(const void* data, int len, int fs, int bit, wavegetdata_f getdata) {
  waveout_t w[1] = {0};
  WAVEFORMATEX waveformat[1];
  int i, ret, nChannels = 1;
  int outbit = 16;
  w->hCallBackEvent = CreateEventA( NULL, TRUE, FALSE, "callBackOut" );
  if (fs<1) {
    fs = 11025;
  }
  if (bit<1) {
    bit = 16;
  }
  w->data = data;
  w->len = len;
  w->fs = fs;
  w->bit = bit;
  w->getdata = getdata;
  waveformat->wFormatTag = WAVE_FORMAT_PCM;
  waveformat->nChannels = nChannels;
  waveformat->nSamplesPerSec = fs;
  waveformat->wBitsPerSample = outbit;
  waveformat->nBlockAlign = (outbit*nChannels)>>3;
  waveformat->nAvgBytesPerSec = waveformat->nBlockAlign * fs;
  waveformat->cbSize = 0;

  ret = waveOutOpen(&w->hWaveOut, WAVE_MAPPER, waveformat, (DWORD)waveOutProc, (DWORD)w, CALLBACK_FUNCTION);
  if (ret != MMSYSERR_NOERROR) {
    return 0;
  }
  for (i=0;i<2;i++) {
    WAVEHDR* waveHdr = w->pWaveHdr1+i;
    waveHdr->lpData = (char*)w->pBuffer1 + i*MAXWAVEBUFFER;
    waveHdr->dwBufferLength = MAXWAVEBUFFER;
    waveHdr->dwBytesRecorded = 0;
    waveHdr->dwUser = 0;
    waveHdr->dwFlags = 0;
    waveHdr->dwLoops = 1;
    waveHdr->lpNext = 0;//i==0 ? NULL : (w->pWaveHdr1+i-1);
    waveHdr->reserved = 0;
    waveOutPrepareHeader(w->hWaveOut, waveHdr, sizeof(WAVEHDR));
  }
  for (i=0;i<2;i++) {
    _wav_getdata(w, (short*)(w->pBuffer1 + i*MAXWAVEBUFFER));
    waveOutWrite(w->hWaveOut, w->pWaveHdr1+i, sizeof(WAVEHDR));
  }
  //getchar();
  WaitForSingleObject(w->hCallBackEvent, INFINITE);
  ReleaseMutex(w->hCallBackEvent);
  //while(!w->stop) {    Sleep(10);  }
  waveOutClose(w->hWaveOut);
  return 0;
}

typedef struct wavein_t {
  MMRESULT mmError;
  HWAVEIN hWaveIn;
  HANDLE hCallBackEvent;
  WAVEFORMATEX waveFmt[1]; /* Pointer to PCMWAVEFORMAT */
  char pBuffer1[2*MAXWAVEBUFFER];
  WAVEHDR pWaveHdr1[2];
  short* data;
  int pos;
  int len;
} wavein_t;

static int _wav_setdata(wavein_t* w, const void* inwav) {
  int len = MAXWAVEBUFFER/2;
  short* data = w->data;
  len = min(len, w->len-w->pos);
  memcpy(data + w->pos, inwav, len*2);
  w->pos += len;
  if (w->pos>=w->len) {
    //w->stop = 1;
    SetEvent(w->hCallBackEvent);
  }
  return 0;
}

int CALLBACK callBackIn(HWAVEIN hWaveIn, UINT msg, DWORD_PTR magic, LPARAM dwParam1, LPARAM dwParam2)
{
  wavein_t* w = (wavein_t*)magic;
  LPWAVEHDR curHdr = (LPWAVEHDR)dwParam1;
  MMRESULT result;
  
  if (w==NULL) return 0;
  
  switch(msg) {
  case MM_WIM_OPEN:
    return TRUE;
  case MM_WIM_DATA:
    //EnterCriticalSection(&w->c);
    //录音设备返回数据时，触发该事件
    if(!waveInUnprepareHeader(hWaveIn, curHdr,sizeof(WAVEHDR))) {//可以释放内存。有的情况虽然返回数据，但不能释放内存，
      //如使用同一个WAVEHDR增加多个内存块时，会出现上述情况
      //memcpy(buffer, curHdr->lpData, MAXWAVEBUFFER);//复制返回数据
      //buf_len = curHdr->dwBytesRecorded;//记录录音数据长度
      _wav_setdata(w, curHdr->lpData);
      curHdr->dwUser=0;//恢复设置
      curHdr->dwBufferLength=MAXWAVEBUFFER;
      curHdr->dwFlags=0;
      //if(m_Opened)
      result=waveInPrepareHeader(hWaveIn,curHdr,sizeof(WAVEHDR));
      result=waveInAddBuffer(hWaveIn,curHdr,sizeof(WAVEHDR));
      (void)result;
    }
    //LeaveCriticalSection(&w->c);
    return TRUE;
  case MM_WIM_CLOSE:
    //SetEvent(w->callBackEvent);
    return TRUE;
  default:
    break;
  }
  return TRUE;
}

static int wavrecord(void* data, int len, int fs, int bit, wavegetdata_f getdata) {
  int i=0, nChannels = 1;
  int nBlockAlign = (bit*nChannels)>>3;
  wavein_t w[1] = {0};
  /* Set up required format */
  w->waveFmt->wFormatTag = WAVE_FORMAT_PCM;
  w->waveFmt->nChannels = nChannels;
  w->waveFmt->nSamplesPerSec = fs;
  w->waveFmt->nBlockAlign = nBlockAlign;
  w->waveFmt->wBitsPerSample = bit;
  w->waveFmt->nAvgBytesPerSec = nBlockAlign * fs;
  w->waveFmt->cbSize = 0;
  w->len = len;
  w->data = (short*)data;
  
  /* Set up position query */
  //w->wavePos->wType = TIME_SAMPLES;
  
  //InitializeCriticalSection( &(w->c) );
  w->hCallBackEvent = CreateEventA( NULL, TRUE, FALSE, "callBackIn" );
  
  //ret = waveOutOpen(&wo->hWaveOut, WAVE_MAPPER, waveformat, (DWORD)waveOutProc, (DWORD)wo, CALLBACK_FUNCTION);
  if ((w->mmError=waveInOpen(NULL, WAVE_MAPPER, w->waveFmt, 0, 0, WAVE_FORMAT_QUERY))!=MMSYSERR_NOERROR) {
    printf("Requested data format is not supported [ERR=%d]",w->mmError);
  }
  if ((w->mmError=waveInOpen(&w->hWaveIn, WAVE_MAPPER, w->waveFmt, (DWORD_PTR)callBackIn, (DWORD_PTR)w,CALLBACK_FUNCTION))!=MMSYSERR_NOERROR) {
    printf("Cannot open MMAPI audio input [ERR=%d]",w->mmError);
  }
  for (i=0;i<2;i++) {
    WAVEHDR* waveHdr = w->pWaveHdr1+i;
    /* Set up header */
    waveHdr->lpData = w->pBuffer1 + i*MAXWAVEBUFFER;
    waveHdr->dwBufferLength = MAXWAVEBUFFER;
    waveHdr->dwBytesRecorded = 0;
    waveHdr->dwUser = 0;
    waveHdr->dwFlags = 0;
    waveHdr->dwLoops = 1;
    waveHdr->lpNext = i==0 ? NULL : (w->pWaveHdr1+i-1);
    if ((w->mmError=waveInPrepareHeader(w->hWaveIn, waveHdr, sizeof(WAVEHDR)))!=MMSYSERR_NOERROR) {
      printf("Header preparation failed");
    }
    
    if ((w->mmError=waveInAddBuffer(w->hWaveIn, waveHdr, sizeof(WAVEHDR)))!=MMSYSERR_NOERROR) {
      printf("Cannot add input buffer %d",w->mmError);
    }
  }
  
  if ((w->mmError=waveInStart( w->hWaveIn ))!=MMSYSERR_NOERROR) {
    printf("Cannot start MMAPI input audio port [ERR=%d]",w->mmError);
  }
  WaitForSingleObject(w->hCallBackEvent, INFINITE);
  ReleaseMutex(w->hCallBackEvent);
  waveInStop(w->hWaveIn);
  waveInClose(w->hWaveIn);
  return 0;
}