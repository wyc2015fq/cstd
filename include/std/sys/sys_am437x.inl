
#ifndef SYSUI_DEFED
#define SYSUI_DEFED

static img_t dc_im[1] = {0};
window_native_t* sys_create_window(int flag, const char* name, int x, int y, int w, int h, window_host_t* host) {
  if (host->user) {
    img_t* im = (img_t*)(host->user);
    *dc_im = *im;
    host->flip_y = 0;
  }
  return (window_native_t*)(dc_im);
}
int sys_get_window_rect(window_native_t* w, IRECT* pwindow, IRECT* pclient) {
  if (pwindow) {
    *pwindow = iRECT2(0, 0, dc_im->w, dc_im->h);
  }
  if (pclient) {
    *pclient = iRECT2(0, 0, dc_im->w, dc_im->h);
  }
  return 0;
}
int sys_set_window_rect(window_native_t* w, IRECT* pwindow, IRECT* pclient) {
  if (pwindow) {
    dc_im->w = RCW(pwindow);
    dc_im->h = RCH(pwindow);
  } else if (pclient) {
    dc_im->w = RCW(pclient);
    dc_im->h = RCH(pclient);
  }
  return 0;
}
int sys_set_window_text(window_native_t* w, const char* text) {
  return 0;
}
int sys_del_dc(window_dc_t* dc) {
  memset(dc_im, 0, sizeof(*dc_im));
  return 0;
}
int sys_window_bitblt(window_native_t* dst_win, int x, int y, int w, int h, window_dc_t* src_dc, int src_x, int src_y) {
  return 0;
}
int sys_dc_set_size(window_dc_t** pdc, int h, int w, int cn) {
  dc_im->h = h;
  dc_im->w = w;
  dc_im->s = w*cn;
  dc_im->c = cn;
  dc_im->f = 1;
  return 0;
}
int sys_dc_get_image(window_dc_t* pdc, img_t* im) {
  *im = *dc_im;
  return 0;
}

int mouse_event(WindowMsg msg, int x, int y, double wheel) {
  sysio_t* io = sys_getio();
  sysmouse_t* mouse = io->mouse;
  io->msg = msg;
  mouse->pos->x = x;
  mouse->pos->y = y;
  switch(msg) {
  case MSG_LBUTTONDOWN:
    mouse->code = MOUSE_LBUTTON;
    mouse->down[0] = true;
    mouse->pos[1] = mouse->pos[0];
    io->msg = MSG_LBUTTONDOWN;
    //mouse->double_clicked[0] = sys_utime(&mouse->clicked_time[0]) < mouse->dbclick_time;
    return 1;
  case MSG_LBUTTONUP:
    mouse->code = MOUSE_LBUTTON;
    mouse->double_clicked[MOUSE_LBUTTON] = false;
    mouse->down[MOUSE_LBUTTON] = false;
    io->msg = MSG_LBUTTONUP;
    return 1;
  case MSG_RBUTTONDOWN:
    mouse->code = MOUSE_RBUTTON;
    mouse->down[1] = true;
    io->msg = MSG_RBUTTONDOWN;
    return 1;
  case MSG_RBUTTONUP:
    mouse->code = MOUSE_RBUTTON;
    mouse->down[1] = false;
    io->msg = MSG_RBUTTONUP;
    return 1;
  case MSG_MBUTTONDOWN:
    mouse->code = MOUSE_MBUTTON;
    mouse->down[2] = true;
    io->msg = MSG_MBUTTONDOWN;
    return 1;
  case MSG_MBUTTONUP:
    mouse->code = MOUSE_MBUTTON;
    mouse->down[2] = false;
    io->msg = MSG_MBUTTONUP;
    return 1;
  case MSG_MOUSEWHEEL:
    io->msg = MSG_MOUSEWHEEL;
    mouse->wheel = wheel;
    return 1;
  case MSG_MOUSEMOVE:
    io->msg = MSG_MOUSEMOVE;
    mouse->pos->x = x;
    mouse->pos->y = y;
    return 1;
  }
  return 0;
}
int msg_list[256];
int msg_list_len = 0;//[0-msg_end]

int pop_msg() {
  int msg = MSG_NOP;
  if (msg_list_len>0) {
    msg = msg_list[0];
    memcpy(msg_list, msg_list+1, msg_list_len-1);
    --msg_list_len;
  }
  return msg;
}
int push_msg(WindowMsg msg) {
  if (msg_list_len<countof(msg_list)) {
    msg_list[msg_list_len++] = msg;
  }
  return 0;
}

int sys_touch_event(int touchdown, int x, int y) {
  static int pre_touchdown = 0;
  static WindowMsg msg = MSG_NOP;
  if (pre_touchdown!=touchdown) {
    msg = touchdown ? MSG_LBUTTONDOWN : MSG_LBUTTONUP;
    pre_touchdown = touchdown;
    push_msg(msg);
  }
  mouse_event(msg, x, y, 0);
  return 0;
}

#ifdef _WIN32
int sys_waitio(int delay) {
  SYSIO;
  io->msg = MSG_NOP;
  io->time = sys_utime_counter();
  if (!(mouse->down[0] || mouse->down[1] || mouse->down[2])) {
    io->hitid = NULL;
  }

  imshow2("dc_im", dc_im);
  cvWaitKey(delay);
  return 1;
}
#else
#endif

#endif // SYSUI_DEFED
