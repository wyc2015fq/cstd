#include "wav.inl"
#include "cap\capvfw.inl"
#include "udpsocket.inl"
#include "color.inl"
int draw_wave(HDDC hDC, RECT rc, const BYTE* wave, int wavelen, int wBitsPerSample)
{
  BOOL bTalk = FALSE;
  int x, y;
  COLORREF _clr = _RGB(0, 255, 0);
  COLORREF _clrbk = _RGB(0, 0, 0);
  int size = wBitsPerSample == 16 ? wavelen / 2 : wavelen;
  int yc = (rc.top + rc.bottom) / 2;
  int i, h = (rc.bottom - rc.top) / 2;
  int _x0, _y0;
  short sample;
  draw_fillrectR(hDC, &rc, _clrbk);
  TMoveTo(hDC, 0, yc);
  for (i = 0 ; i < size; i++) {
    //to draw first channel
    sample = wBitsPerSample == 16 ? ((*((short*)wave + i)) * h) / (65535 / 2) : ((*(wave + i) - 128) * h) / 128;
    x = rc.left + (RCW(&rc) * i / size);
    y = yc - sample;
    _LineTo(hDC, x, y);
  }
  return 0;
}
typedef struct test_wav_t {
  UI_engine ui[1];
  layout_ctrl m_lay[5];
  button_ctrl m_btn_rec[1];
  button_ctrl m_btn_play[1];
  button_ctrl m_btn_cap[1];
  wavin m_wavin[1];
  wavout m_wavout[1];
  char wavebuf[WAV_BUFSZ];
  int wavebuflen;
  uictrl ctrlwav[1];
  uictrl ctrlcap[1];
  int wavopened;
  int alllen;
  int poslen;
  img_t m_imcap[1];
  capvfw m_cap[1];
} test_wav_t;
int wavin_func(test_wav_t* g, WAVEFORMATEX* waveform, WAVEHDR* hdr)
{
  //alllen = wav_save("aa.wav", waveform, wavedata, wavedatalen, alllen);
  memcpy(g->wavebuf, hdr->lpData, hdr->dwBytesRecorded);
  g->wavebuflen = hdr->dwBytesRecorded;
  force_redraw2(g->ui);
  return 0;
}
int wavout_func(test_wav_t* g, WAVEFORMATEX* waveform, WAVEHDR* hdr)
{
  int len;
  printf("wavout_func\n");
  hdr->dwBytesRecorded = 0;
  if (0) {
#define TEST_WAV_FILE(x) "D:\\code\\c\\ÒôÆµ´¦Àí\\Playing Wave Resources\\"
    len = wav_load(TEST_WAV_FILE("kick03.wav"), waveform, hdr->lpData, hdr->dwBufferLength, g->poslen);
  }
  else {
    *waveform = g->m_wavin->waveform[0];
    memcpy(hdr->lpData, g->wavebuf, g->wavebuflen);
    len = g->wavebuflen;
  }
  g->poslen += len;
  hdr->dwBytesRecorded = len;
  return 0;
}
int cap_func(test_wav_t* g, img_t* im, int pix_fmt)
{
  imcolorcvt(im, g->m_imcap, pix_fmt, T_BGRA);
  imsetalpha(g->m_imcap, 255);
  force_redraw2(g->ui);
  return 0;
}
int test_wav_event_proc(test_wav_t* g, event* e)
{
  if (e->msg == EVENT_LCLICK) {
    if (e->sender == g->m_btn_rec) {
      g->alllen = 0;
      wavin_set(g->m_wavin, g->wavopened ? 0 : wavin_func, g);
      g->wavopened = !g->wavopened;
      printf("m_btn_rec EVENT_LCLICK\n");
    }
    if (e->sender == g->m_btn_play) {
      g->poslen = 0;
      wavout_play(g->m_wavout, wavout_func, g);
      printf("m_btn_play EVENT_LCLICK\n");
    }
    if (e->sender == g->m_btn_cap) {
      printf("m_btn_cap EVENT_LCLICK\n");
      if (!g->m_cap->m_hWndCap) {
        capvfw_open(g->m_cap, 0);
      }
      else {
        capvfw_close(g->m_cap);
      }
    }
  }
  return 0;
}
int test_wav_paint_proc(test_wav_t* g, HDDC hDC)
{
  UIGetWorkArea(g->ui, &g->m_lay->base.rect);
  draw_clear(hDC, _GetSysColor(COLOR_BTNFACE));
  draw_layout_ctrl(hDC, g->m_lay);
  draw_wave(hDC, g->ctrlwav->rect, g->wavebuf, g->wavebuflen, g->m_wavin->waveform->wBitsPerSample);
  if (g->m_cap->m_hWndCap) {
    draw_imageR(hDC, &g->ctrlcap->rect, g->m_imcap, 0, 0, 0);
  }
  return 0;
}
int test_wav_frame_proc(test_wav_t* g, event* e)
{
  layout_ctrl_event(g->m_lay, e);
  return 0;
}
int test_wav()
{
  test_wav_t g[1] = {0};
  layitem_t li[] = {
    0, 0, 0, g->ctrlcap,
    't', 40, 0, 0,
    'l', 120, 1, &g->m_btn_rec->base,
    'l', 120, 1, &g->m_btn_play->base,
    'l', 120, 1, &g->m_btn_cap->base,
    't', 80, 0, g->ctrlwav,
  };
  layout_ctrl_set(g->m_lay, 4, countof(li), li);
  button_ctrl_set(g->m_btn_play, "play", 0, 0);
  button_ctrl_set(g->m_btn_rec, "rec", 0, 0);
  button_ctrl_set(g->m_btn_cap, "cap", 0, 0);
  UI_set(g->ui, "test_wav", g, test_wav_paint_proc, test_wav_frame_proc, test_wav_event_proc, 0);
  waitkey(g->ui, -1);
  FreeMemDC();
  return 0;
}

