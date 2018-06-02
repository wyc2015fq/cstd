#include "wav.inl"
#include "ui/window.inl"
static int FillBuffer_sin(waveout_t* wo, short* buffer, int len)
{
  static double fAngle;
  static int iFreq = 400;
  static int ss = 10;
  int i;
  for (i = 0; i < len; i++) {
    buffer[i] = (short)((127 * sin(fAngle)) * 256);
    fAngle += 2 * PI * iFreq / wo->fs;
    if (fAngle > 2 * PI) {
      fAngle -= 2 * PI;
    }
  }
  if (1) {
    iFreq += ss;
    if (iFreq > 500 || iFreq < 300) {
      ss *= -1;
    }
  }
  return len;
}
int test_sin_wav_play()
{
  // 5s
  wavplay(0, 11025 * 5, 11025, 8, FillBuffer_sin);
  return 0;
}
int test_wav()
{
  short* y = 0;
  int fs, bit, n;
  char* fn = 0;
  fn = "HORN1.wav";
  fn = "ball.wav";
  fn = "traffic69.wav";
  fn = "beijing.wav";
  fn = "sa1.wav";
  _chdir("E:/pub/bin/audio");
  n = wavread_filename(fn, &y, &fs, &bit);
  wavplay(y, n, fs, 16, 0);
  //cvShowMat("adsf", "%I16d", n, 1, y, 2, 2, -1); cvWaitKey(-1);
  free(y);
  return 0;
}

