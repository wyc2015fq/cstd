#include <mmsystem.h>
#pragma comment(lib,"WINMM.LIB")//多媒体支持
#define WAV_BUFSZ (1<<11)
#define WAV_NBUF 4
typedef int (*wav_callback_t)(void* user, WAVEFORMATEX* waveform, WAVEHDR* hdr);
typedef struct wavbuf {
  WAVEHDR hdr;
  char buf[WAV_BUFSZ];
} wavbuf;
typedef struct wavin {
  wavbuf wb[WAV_NBUF];
  WAVEFORMATEX waveform[1];
  wav_callback_t func;
  void* user;
  HWAVEIN hWaveIn;
} wavin;
typedef struct wavout {
  wavbuf wb[WAV_NBUF];
  WAVEFORMATEX waveform[1];
  wav_callback_t func;
  void* user;
  HWAVEOUT hWaveOut;
} wavout;
void CALLBACK wavein_callback(HDRVR hdrvr, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
  wavin* s = (wavin*)dwUser;
  HWAVEIN hWaveIn = (HWAVEIN)hdrvr;
  PWAVEHDR hdr = (PWAVEHDR) dw1;
  switch (uMsg) {
  case MM_WIM_CLOSE:
    printf("MM_WIM_CLOSE\n");
    break;
  case MM_WIM_OPEN:
    printf("MM_WIM_OPEN\n");
    break;
  case MM_WIM_DATA:
    if (s->func) {
      s->func(s->user, s->waveform, hdr);
    }
    waveInAddBuffer(hWaveIn, (PWAVEHDR)dw1, sizeof(WAVEHDR));
    printf("MM_WIM_DATA %d byte\n", hdr->dwBytesRecorded);
    break;
  }
  return ;
}
void CALLBACK waveout_callback(HDRVR hdrvr, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
  wavout* s = (wavout*)dwUser;
  HWAVEOUT hWaveOut = (HWAVEOUT)hdrvr;
  WAVEHDR* hdr = &s->wb->hdr;
  switch (uMsg) {
  case MM_WOM_OPEN:
    printf("MM_WOM_OPEN\n");
    break;
  case MM_WOM_CLOSE:
    printf("MM_WOM_CLOSE\n");
    break;
  case MM_WOM_DONE:
    ASSERT(hdr == (WAVEHDR*)dw1);
    printf("MM_WOM_DONE\n");
    waveOutUnprepareHeader(s->hWaveOut, hdr, sizeof(WAVEHDR));
    if (s->func) {
      s->func(s->user, s->waveform, hdr);
      if (hdr->dwBytesRecorded > 0) {
        waveOutPrepareHeader(s->hWaveOut, hdr, sizeof(WAVEHDR));
        waveOutWrite(s->hWaveOut, hdr, sizeof(WAVEHDR));
      }
    }
    break;
  }
  return ;
}
int wavout_play(wavout* s, wav_callback_t func, void* user)
{
  wavbuf* wb = s->wb;
  WAVEHDR* hdr = &s->wb->hdr;
  if (s->hWaveOut) {
    waveOutClose(s->hWaveOut);
    s->hWaveOut = 0;
    return 0;
  }
  if (func) {
    memset(wb, 0, sizeof(wavbuf));
    hdr->lpData = wb->buf;
    hdr->dwBufferLength = WAV_BUFSZ;
    hdr->dwFlags = WHDR_BEGINLOOP | WHDR_ENDLOOP;
    hdr->dwLoops = 0;
    s->user = user;
    s->func = func;
    s->func(s->user, s->waveform, hdr);
    if (0 == hdr->dwBytesRecorded) {
      return 0;
    }
    if (waveOutOpen(&s->hWaveOut, WAVE_MAPPER, s->waveform, (DWORD)waveout_callback, (DWORD)s, CALLBACK_FUNCTION)) {
      MessageBeep(MB_ICONEXCLAMATION);
      printf("Audio output error\n");
      return 0;
    }
    waveOutPrepareHeader(s->hWaveOut, hdr, sizeof(WAVEHDR));
    waveOutWrite(s->hWaveOut, hdr, sizeof(WAVEHDR));
  }
  return 0;
}
int wavin_set(wavin* s, wav_callback_t func, void* user)
{
  int i;
  if (s->hWaveIn) {
    //waveInStop(s->hWaveIn);
    for (i = 0; i < WAV_NBUF; ++i) {
      waveInUnprepareHeader(s->hWaveIn, &s->wb[i].hdr, sizeof(WAVEHDR));
    }
    //waveInReset(s->hWaveIn);
    waveInClose(s->hWaveIn);
    s->hWaveIn = 0;
    memset(s, 0, sizeof(wavin));
    return 0;
  }
  if (func) {
    s->waveform->wFormatTag = WAVE_FORMAT_PCM;
    s->waveform->nChannels = 1;
    s->waveform->nSamplesPerSec = 11025;
    s->waveform->nAvgBytesPerSec = 11025;
    s->waveform->nBlockAlign = 1;
    s->waveform->wBitsPerSample = 8;
    s->waveform->cbSize = 0;
    s->func = func;
    s->user = user;
    if (waveInOpen(&s->hWaveIn, WAVE_MAPPER, s->waveform, (DWORD)wavein_callback, (DWORD)s, CALLBACK_FUNCTION)) {
      printf("Audio can not be open!");
    }
    if (waveInReset(s->hWaveIn)) {
      return 0;
    }
    for (i = 0; i < WAV_NBUF; ++i) {
      wavbuf* wb = s->wb + i;
      wb->hdr.lpData = wb->buf;
      wb->hdr.dwBufferLength = WAV_BUFSZ;
      wb->hdr.dwLoops = 1;
      waveInPrepareHeader(s->hWaveIn, &wb->hdr, sizeof(WAVEHDR));
      waveInAddBuffer(s->hWaveIn, &wb->hdr, sizeof(WAVEHDR));
    }
    waveInStart(s->hWaveIn);
  }
  return 0;
}
#define big
#define little
typedef struct wave_file {
  DWORD big ChunkID; // 4 文件头标识，一般就是" RIFF" 四个字母 0x52494646
  DWORD little ChunkSize; // 4 整个数据文件的大小，不包括上面ID和Size本身
  DWORD big Format; // 4 一般就是" WAVE" 四个字母 0x57415645
  DWORD big SubChunk1ID; // 4 格式说明块，本字段一般就是"fmt " 0x666d7420
  DWORD little SubChunk1Size; // 4 本数据块的大小，不包括ID和Size字段本身
  WORD little AudioFormat; // 2 音频的格式说明
  WORD little NumChannels; // 2 声道数
  DWORD little SampleRate; // 4 采样率
  DWORD little ByteRate; // 4 比特率，每秒所需要的字节数
  WORD little BlockAlign; // 2 数据块对齐单元
  WORD little BitsPerSample; // 2 采样时模数转换的分辨率
  DWORD big SubChunk2ID; // 4 真正的声音数据块，本字段一般是"data" 0x64617461
  DWORD little SubChunk2Size; // 4 本数据块的大小，不包括ID和Size字段本身
} wave_file;
#undef big
#undef little
int wav_save(const char* fn, const WAVEFORMATEX* waveform, const unsigned char* wavedata, DWORD wavedatalen, int alllen)
{
  wave_file wf = {0};
  FILE* pf = fopen(fn, "rb+");
  if (NULL == pf) {
    pf = fopen(fn, "wb");
  }
  if (NULL == pf) {
    return 0;
  }
  wf.ChunkID = *(DWORD*)("RIFF");
  wf.Format = *(DWORD*)("WAVE");
  wf.SubChunk1ID = *(DWORD*)("fmt ");
  wf.SubChunk2ID = *(DWORD*)("data");
  wf.SubChunk1Size = 16;
  wf.SubChunk2Size = alllen + wavedatalen;
  wf.ChunkSize = 4 + (8 + wf.SubChunk1Size) + (8 + wf.SubChunk2Size);
  printf("%d\n", wf.ChunkSize);
  wf.AudioFormat = waveform->wFormatTag;
  wf.NumChannels = waveform->nChannels;
  wf.SampleRate = waveform->nSamplesPerSec;
  wf.ByteRate = waveform->nAvgBytesPerSec;
  wf.BlockAlign = waveform->nBlockAlign;
  wf.BitsPerSample = waveform->wBitsPerSample;
  wf.BlockAlign = wf.NumChannels * wf.BitsPerSample / 8;
  fseek(pf, 0, SEEK_SET);
  fwrite(&wf, sizeof(wf), 1, pf);
  fseek(pf, sizeof(wf) + alllen, SEEK_SET);
  fwrite(wavedata, wavedatalen, 1, pf);
  fclose(pf);
  return alllen + wavedatalen;
}
int wav_load(const char* fn, WAVEFORMATEX* waveform, unsigned char* wavedata, DWORD wavedatalen, int alllen)
{
  wave_file wf = {0};
  FILE* pf = fopen(fn, "rb");
  int readlen = 0;
  if (NULL == pf) {
    return 0;
  }
  fseek(pf, 0, SEEK_SET);
  fread(&wf, sizeof(wf), 1, pf);
  fseek(pf, sizeof(wf) + alllen, SEEK_SET);
  readlen = fread(wavedata, 1, wavedatalen, pf);
  waveform->wFormatTag = wf.AudioFormat;
  waveform->nChannels = wf.NumChannels;
  waveform->nSamplesPerSec = wf.SampleRate;
  waveform->nAvgBytesPerSec = wf.ByteRate;
  waveform->nBlockAlign = wf.BlockAlign;
  waveform->wBitsPerSample = wf.BitsPerSample;
  fclose(pf);
  return readlen;
}

