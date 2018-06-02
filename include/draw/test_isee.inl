
#define PDIV(A, B) ((((A)%(B))+(B))%(B))
#define MAXBUFIMG 5
#define MAXROW 6
#define MAXCOL 8
#define MAXIMGL (MAXROW*MAXCOL)
#define MAXBUFIMGL (MAXBUFIMG*MAXIMGL)

#include "cfile.h"

typedef struct test_isee_t {
  int pos;
  vstr_t sl[1];
  const char* exts;
  char* path;
  img_t im[MAXBUFIMG];
  int im_indx[MAXBUFIMG];
  img_t iml[MAXBUFIMGL];
  img_t iml_info[MAXBUFIMGL];
  int im_indxl[MAXBUFIMGL];
  //finddata_t fd[1];
  int th_wait;
  int exit_now;
  int listmode;
  int hot_pos;
  char* pre_path;
  IRECT rc_back[1];
  int is_FullScreen;
  //SpecialFX_Water_t FXWater[1];
  thread_t th[2];
  lock_t cs[2];
} test_isee_t;
int setpath0(test_isee_t* s, const char* pp)
{
  char buf[256];
  if (pp && file_exist(pp)) {
    s->exit_now = 1;
    sys_lock_lock(s->cs);
    //sys_lock_lock(s->cs+1);
    strv_free(s->sl);
    freeims(s->im, MAXBUFIMG);
    freeims(s->iml, MAXBUFIMGL);
    s->th_wait = 1;
    if (strcmp(s->pre_path, pp)) {
      char* p = (char*)strchr(pp, '\r');
      s->pos = 0;
      if (p) {
        *p = 0;
      }
	  p = (char*)strchr(pp, '\n');
      if (p) {
        *p = 0;
      }
      free(s->path);
      s->path = strdup(pp);
    }
    if (s->pre_path) {
      free(s->pre_path);
    }
    s->pre_path = strdup("");
    printf("aa%08x\n", s->path);
    s->th_wait = 0;
    printf("%s\n", pp);
    //sys_lock_unlock(s->cs+1);
    sys_lock_unlock(s->cs);
    _snprintf(buf, 256, "%s", pp);
    sys_reg_save_data("test_isee", "path", buf, 256);
    sys_reg_load_data("test_isee", s->path, &s->pos, sizeof(s->pos));
  }
  return 0;
}
int setpath(test_isee_t* s, int isClip)
{
  if (isClip) {
    char pp[256];
    sys_get_clipboard_text(pp, countof(pp));
    setpath0(s, pp);
  }
  else {
    setpath0(s, s->path);
  }
  return 0;
}
int next_page(test_isee_t* s, int off)
{
  int n = s->listmode ? MAXIMGL : 1;
  s->pos = ((s->pos / n) + off) * n + s->pos % n;
  force_redraw(c);
  return 0;
}
int gcDrawImageList(gc_t* g, IRECT dst, const img_t* s, int len, int row, int col)
{
  int i, x, y, r, c;
  //int dh = d->h, dw = d->w;
  int dh = RCH(&dst), dw = RCW(&dst);
  if (col <= 0 || row <= 0 || dw < 1 || dh < 1) {
    return 0;
  }
  len = MIN(len, row * col);
  for (i = 0; i < len; ++i, ++s) {
    IRECT drc;
    int sw = s->w, sh = s->h;
    int tw = dw, th = dh;
    r = i / col, c = i % col;
    if (sh < 10 || s->w < 10 || s->s < 10) {
      continue;
    }
    if (1) {
      tw = sw * dh / row;
      th = sh * dw / col;
      x = 0, y = 0;
      if (tw > th) {
        tw = dw / col, th /= sw, y = (dh / row - th) / 2;
      }
      else {
        th = dh / row, tw /= sh, x = (dw / col - tw) / 2;
      }
    }
    drc = iRECT2( dst.l + (c * dw / col + x), dst.t + (r * dh / row + y), tw, th);
    //IMRESIZE(sh, sw, s->tt.data, s->s, 3, th, tw, dd, d->s, 3, INTER_BILINEARITY);
    gcRectImageRIM(g, drc, s, NULL);
  }
  return 0;
}
int test_isee_read_img(void* arg)
{
  test_isee_t* s = (test_isee_t*)arg;
  for (; !s->exit_now;) {
    sys_sleep(10);
    if (s->sl->n > 0) {
      int i, len;
      img_t* im0;
      int* im_indx0;
      int pos = PDIV(s->pos, s->sl->n);
      if (s->listmode) {
        len = MAXBUFIMGL;
        im0 = s->iml;
        im_indx0 = s->im_indxl;
      }
      else {
        len = MAXBUFIMG;
        im0 = s->im;
        im_indx0 = s->im_indx;
      }
      for (i = 0; i < len; ++i) {
        int t, idx2 = pos + ((i + 1) / 2) * ((i & 1) ? 1 : -1);
        int idx = PDIV(idx2, s->sl->n);
        t = PDIV(idx2, len);
        if (im_indx0[t] != idx) {
          img_t* im = im0 + t;
          //sys_lock_lock(s->cs + 1);
          freeims(im0 + t, 1);
          im_indx0[t] = idx;
          if (s->listmode) {
            img_t im2[1] = {0};
            printf("ssss %d\n", idx);
            if (imread(s->sl->v[idx].s, 3, 1, im2)) {
              imresize(im2, 200, 200, im);
              s->iml_info[t] = *im2; // 保持图像信息
              s->iml_info[t].tt.data = 0;
              //printf("list\n");
              freeims(im2, 1);
            }
            imfree(im2);
          }
          else {
            img_t im2[1] = {0}, imt = *im;
            imread(s->sl->v[idx].s, 3, 1, im2);
            printf("ssss %d w=%d\n", idx, im2->w);
            if (30 == idx) {
              int adsf = 0;
            }
            *im = *im2;
            imfree(&imt);
          }
          //sys_lock_unlock(s->cs + 1);
        }
      }
    }
  }
  return 0;
}
int next_file_from_file_list(const char* curfile, const char* filelist, char* buf, int buflen)
{
  FILE* pf = fopen(filelist, "rb");
  if (pf) {
    char* s = (char*)strrchr(curfile, '\\');
    s = s ? s + 1 : (char*)curfile;
    for (; fgets(buf, buflen, pf) > 0;) {
      if (strstr(buf, s)) {
        return (fgets(buf, buflen, pf) > 0);
      }
    }
    fclose(pf);
  }
  return 0;
}
void test_isee_filelist(const char* path, fileinfo_t* info, void* userdata) {
  test_isee_t* s = (test_isee_t*)userdata;
  strv_push_cstr(s->sl, info->name, -1);
  return ;
}
int test_isee_find_file_ThreadProc(void* arg)
{
  test_isee_t* s = (test_isee_t*)arg;
  //find_file(s->path, s->exts, __filelist2, 1, s->sl);
  //finddata_t fd[1]={0};
  s->pre_path = strdup("");
  for (; !s->exit_now;) {
    sys_lock_lock(s->cs);
    if (!s->th_wait && strcmp(s->pre_path, s->path)) {
      strv_free(s->sl);
      free(s->pre_path);
      s->pre_path = strdup(s->path);
      s->exit_now = 0;
      printf("%08x\n", s->path);
      findfile_1(s->path, s->exts, test_isee_filelist, FF_SUBDIR|FF_FULLNAME|FF_FILTERS, s);
    }
    if (s->sl->n > 0) {
      s->pos = PDIV(s->pos, s->sl->n);
    }
    sys_lock_unlock(s->cs);
    sys_sleep(50);
  }
  free(s->pre_path);
  return 0;
}
const char* exts[] = {"|jpg|", "|gif|", "gif;jpg;bmp;png"};
int test_isee(ctrl_t* c) {
  SYSIO;
  IRECT rc = c->rc;
  static test_isee_t s[1] = {0};
  static int inited=0;
  if (0) {
    printf("end\n");
    sys_lock_destroy(s->cs);
    sys_lock_destroy(s->cs + 1);
    free(s->path);
    strv_free(s->sl);
    freeims(s->im, MAXBUFIMG);
    freeims(s->iml, MAXBUFIMGL);
  }
  if (!inited) {
    char* path = ".\\";
    char buf[256] = "";
    inited = 1;
    sys_chdir("E:/pub/pic");
    s->exts = exts[0];
    s->exts = exts[1];
    s->pos = 150;
    s->pos = 800;
    s->pos = 0;
    path = "C:/新建文件夹/";
    path = "D:/TDDOWNLOAD/pic/gif/gif-face/";
    path = "D:/www/gif/animeshemales.tumblr.com/gif/25.media.tumblr.com/";
    path = "D:/www/gif/daddysdribble.tumblr.com/gif/";
    path = "E:/360Downloads/";
    path = "E:/pub/pic/";
    path = "E:/www/gif/addictedtoeveythingwomen.tumblr.com/ass/";
    sys_reg_load_data("test_isee", "path", buf, 256);
    s->path = strdup(path);
    sys_lock_init(s->cs);
    sys_lock_init(s->cs + 1);
    sys_reg_load_data("test_isee", s->path, &s->pos, sizeof(s->pos));
    memset(s->im_indx, 255, sizeof(s->im_indx));
    memset(s->im_indxl, 255, sizeof(s->im_indxl));
    printf("%s:%d\n", s->path, s->pos);
    s->th[0].run = test_isee_find_file_ThreadProc;
    s->th[0].arg = s;
    s->th[1].run = test_isee_read_img;
    s->th[1].arg = s;
    sys_thread_create(s->th);
    sys_thread_create(s->th + 1);
  }
  color_background_ctrl(rc, ColorBlack, ColorWhite, 1, true);
  if (ISHOVEREDWIN2()) {
    int hot = iPtInRect(&rc, x, y);
    switch (io->msg) {
  case MSG_LBUTTONUP:
    if (s) {
    }
    break;
  case MSG_KEYDOWN:
    if (s) {
      UINT ch = io->last_key;
      if (ch > 0) {
        printf("key:%08x\n", ch);
      }
      switch (ch) {
      case 'q':
        s->exit_now = 1;
        //sys_thread_joins(2, s->th);
        break;
      //case KEY_F5:
      case 0x0d:
        setpath(s, 0x0d == ch);
        break;
      case 'g': {
        static int extmode = 1;
        extmode = !extmode;
        s->exts = exts[extmode];
        setpath(s, 0);
      }
      break;
      case 'd': {
        FILE* pf = fopen("path.txt", "a");
        if (pf) {
          printf("del:%s\n", s->path);
          fprintf(pf, "%s\n", s->path);
          fclose(pf);
        }
        ch = ']';
      }
      break;
      case 'a': {
        FILE* pf = fopen("pathaaa.txt", "a");
        if (pf) {
          printf("add:%s\n", s->path);
          fprintf(pf, "%s\n", s->path);
          fclose(pf);
        }
      }
      break;
      case 'e': {
        char buf[1024];
        _snprintf(buf, 1024, "explorer.exe %s", s->path);
        system(buf);
      }
      break;
      case ']':
      case '[': {
        char* nextpath = ']' == ch ? GetNextFile(s->path, 0, 1) : GetPrevFile(s->path, 0, 1);
        if (NULL == nextpath) {
          nextpath = GetFirstFile(s->path, 0, 1);
        }
        if (nextpath) {
          setpath0(s, nextpath);
        }
      }
      break;
      case 'c': {
        FILE* pf = fopen("pic.txt", "a");
        char* fn = s->sl->v[s->pos].s;
        if (pf) {
          fprintf(pf, "%s\n", fn);
          fclose(pf);
        }
        sys_set_clipboard_text(fn, strlen(fn));
        printf("sys_set_clipboard_text:%s\n", fn);
      }
      break;
      case KEY_DELETE: {
        char* fn = s->sl->v[s->pos].s;
        if (sys_fileopt(FOF_DELETE, fn, NULL)) {
          printf("FOF_DELETE 失败\n");
        }
        if (!file_exist(fn)) {
          strv_del_str(s->sl, s->pos);
          freeims(s->im + PDIV(s->pos, MAXBUFIMG), 1);
          if (s->sl->n <= 0) {
            break;
          }
        }
      }
      break;
      case 'h': {
        printf("帮助:\n中键切换全屏\n回车从剪贴板读取路径");
      }
      break;
      case KEY_HOME:
        s->pos = 0;
        break;
      case 't': {
      }
      break;
      case ' ':
      case KEY_PAGEDOWN:
      case KEY_PAGEUP:
        next_page(s, KEY_PAGEUP == ch ? -1 : 1);
        break;
      case 'p':
      case 'n':
        next_page(s, 'p' == ch ? -100 : 100);
        break;
      default:
        break;
      }
    }
    break;
  case MSG_LBUTTONDBLCLK:
    if (s) {
    }
    break;
  case MSG_MOUSEMOVE:
  case MSG_LBUTTONDOWN:
    if (hot) {
      if (s->listmode) {
        int pos;
        pos = ((s->pos / (MAXCOL * MAXROW)) * (MAXCOL * MAXROW));
        pos = pos + (y * MAXROW / (RCH(&rc))) * MAXCOL + (x * MAXCOL / RCW(&rc));
        if (s->hot_pos != pos) {
          s->hot_pos = pos;
          force_redraw(c);
        }
        if (MSG_LBUTTONDOWN == io->msg) {
          if (s->pos != pos) {
            s->pos = pos;
            freeims(s->im + PDIV(pos, MAXBUFIMG), 1);
          }
        }
      }
      else {
        //SpecialFX_Water_HeightBlob( s->FXWater, x, y, 15, 150, s->FXWater->m_iHpage );
        //SpecialFX_Water_HeightBlob( s->FXWater, x, y, 50, 150, s->FXWater->m_iHpage );
      }
      if (MSG_LBUTTONDOWN == io->msg) {
        s->listmode = !(s->listmode);
        force_redraw(c);
      }
    }
    break;
  case MSG_RBUTTONDOWN:
    if (hot) {
      s->listmode = !(s->listmode);
      force_redraw(c);
    }
    break;
  case MSG_MBUTTONDOWN:
    if (hot) {
      //setpath(s);
      //s->is_FullScreen = SetFullScreen(hwnd, s->is_FullScreen, s->rc_back);
      //s->pre_pos = -1;
      force_redraw(c);
    }
    break;
  case MSG_MOUSEWHEEL:
    if (hot) {
      next_page(s, mouse->wheelDelta < 0 ? 1 : -1);
    }
    break;
  }
  }
  if (1) {
    gcSolidBrush(g, ColorBlack);
    gcFillRect2(g, rc);
    if (s->listmode) {
      int pos = ((s->pos / (MAXCOL * MAXROW)) * (MAXCOL * MAXROW)) % MAXBUFIMGL;
      sys_lock_lock(s->cs + 1);
      gcDrawImageList(g, rc, s->iml + pos, MAXBUFIMGL - pos, MAXROW, MAXCOL);
      {
        int hpos, x, y, dh = RCH(&rc) / MAXROW, dw = RCW(&rc) / MAXCOL;
        hpos = PDIV(s->hot_pos, MAXIMGL);
        x = (hpos % MAXCOL) * RCW(&rc) / MAXCOL, y = (hpos / MAXCOL) * RCH(&rc) / MAXROW;
        gcEdgeRect1(g, iRECT2(x, y, dw, dh), 0, _RGB(255, 0, 0), 2);
        hpos = PDIV(s->pos, MAXIMGL);
        x = (hpos % MAXCOL) * RCW(&rc) / MAXCOL, y = (hpos / MAXCOL) * RCH(&rc) / MAXROW;
        gcEdgeRect1(g, iRECT2(x, y, dw, dh), 0, _RGB(0, 255, 0), 2);
      }
      //cvShowImage(s->winname, im->h, im->w, im->tt.data, im->s, im->c);
      sys_lock_unlock(s->cs + 1);
    }
    else {
      img_t* im0 = s->im + PDIV(s->pos, MAXBUFIMG);
      //cvShowImage("asdf", 400, 400, img, 1200, 3);
      if (im0->f && im0->h > 0) {
        int idx = (clock() / 100) % im0->f;
        if (im0->f > 0) {
          IRECT rcImg = {0};
          img_t im1[1] = {0};
          imgetframe(im0, idx, im1);
          s->th_wait = 1;
          //ASSERT(0==idx);
          sys_lock_lock(s->cs + 1);
          iRectScaling(rc, im0->w, im0->h, &rcImg);
          gcRectImageRIM(g, rcImg, im1, NULL);
          //printf("%d\n", im1->w);
          //cvShowImage(s->winname, im->h, im->w, im->tt.data + im->h * im->s * idx, im->s, im->c);
          sys_lock_unlock(s->cs + 1);
          s->th_wait = 0;
        }
      }
    }
  }
  return 0;
}

