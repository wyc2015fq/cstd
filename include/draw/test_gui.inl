
#include <time.h>
#include <stdio.h>
#include <assert.h>
#include "debug.h"
#include "imresize.h"
#include "gui.h"
#include "cstd.h"
#include "cstd.h"
#include "cstd.h"
#include "cstd.h"

#define H 600 // 高
#define W 800

// 全局
//IMGSEQ* seq = 0;
capcam* cap = 0;

int gui_init(int h, int w);
int gui_mainloop();
gwin* gui_add_win(char* title, void* param);
gwin* gui_add_btn(char* wintitle, char* title, int x, int y, int cx, int cy,
    int quick_key, fun_t fun, void* fun_param, void* param);
int gui_free_allwin();
int gui_set_active(char* title);
int gui_set_title(void* win0, const char* title);
int gui_set_on_show(void* win0, fun_t on_show);
int gui_set_on_key(void* win0, fun_t on_show);
int gui_set_on_click(void* win0, fun_t on_click, void* click_param);
void draw_digi(void* win0);
void draw_text(void* win0, int r, int g, int b);
int on_show_bmp(void* win0, void* b);
int gui_set_style(void* win0, int add0, int del0);
gwin* gui_add_obj(char* wintitle, char* title, int x, int y, int cx, int cy,
    int quick_key, fun_t fun, void* fun_param, void* param);
int gui_add_softkey(char* wintitle, int x, int y, int cx, int cy);
gwin* gui_find_win(gwin* w, char* title, gwin** prev);
gwin* find_win(char* title, gwin** prev);
gwin* gui_find_win_withkey(gwin* w, int qkey);
int gui_set_active_key(gwin* w, int qkey);

#define STDIMGH (100)
#define STDIMGW (100)

#if 0
extern unsigned char stdface[ STDIMGH* STDIMGW ];
//
int faceRecogInit();
int faceRecog(int h, int w, const unsigned char* gry, int gl,
    XRECT* facepos, int input_id, int* ouput_id, double* pscore);

int savedata();
int loaddata();
int id_feat_free();
int id_feat_del(int delid);
int faceRecogUnInit();

#else
#define savedata() {}
#define faceRecogInit() {}
#define loaddata() {}
#define id_feat_free() {}
#define id_feat_del() {}
#define faceRecogUnInit() {}
#endif

int Main_ViewVideo(void* aa, void* bb)
{
  gui_set_active("视频浏览窗口");
  return 0;
}

int Main_AudioMessage(void* aa, void* bb)
{
  gui_set_active("系统留言窗口");
  return 0;
}

int Main_RecvAudio(void* aa, void* bb)
{
  gui_set_active("听取留言窗口");
  return 0;
}

int Main_PropertiesSet(void* aa, void* bb)
{
  gui_set_active("高级设置窗口");
  return 0;
}

int Main_RecordQuery(void* aa, void* bb)
{
  gui_set_active("记录查询窗口");
  return 0;
}

int PropertiesSet_Gprs(void* aa, void* bb)
{
  gui_set_active("GPRS设置窗口");
  return 0;
}

int CollectFace(void* aa, void* bb)
{
  gui_set_active("人脸采集");
  return 0;
}

int ManagerAuthor(void* aa, void* bb)
{
  gui_set_active("管理员设置");
  return 0;
}

int on_click_edit(void* aa, void* bb);

int retmain(void* aa, void* bb)
{
  char* wintitle = (char*) bb;
  gwin* btn = ((gwin*) aa);

  for (; btn; btn = btn->next) {
    if (btn->on_click == on_click_edit) {
      strcpy(btn->title, "");
    }
  }

  gui_set_active(wintitle);
  return 0;
}

int btnfun2(void* aa, void* bb)
{
  printf("高级设置\n");
  return 0;
}

static char _buffer[ 100 ];

#define STRDATE()  _strdate(_buffer)
#define STRTIME()  _strtime(_buffer)
#define STRRAND()  dtoa(rand()/100., _buffer, 9)
char* dtoa(double d, char* str, int len)
{
  _snprintf(str, len, "%21.20f", d);
  return str;
}

enum {
  DO_NON,
  DO_CAP,
  DO_REG,
};

int cap_or_reg = DO_REG;
int cap_num = 0;
int currid = -1;

#ifdef WIN32

int on_show_digi_time(void* win, void* b)
{
  gui_set_title(win, STRTIME());
  on_show_bmp(win, b);
  draw_digi(win);
  return 1;
}

int on_show_digi_date(void* win, void* b)
{
  time_t ltime;
  struct tm* today;
  _tzset();
  time(&ltime);
  today = localtime(&ltime);
  strftime(_buffer, 100, "%Y-%m-%d", today);
  gui_set_title(win, _buffer);
  on_show_bmp(win, b);
  draw_digi(win);
  return 1;
}

// 记住：通过一次有5秒钟无敌时间（就是不用验证）
int on_show_video(void* win, void* b)
{
  if (cap_or_reg == DO_CAP && currid < 0) {
    return 0;
  }

  if (cap && cam_GrabFrame(cap)) {
    unsigned char* I = cam_begin(cap);
    int h = cam_height(cap), w = cam_width(cap), al = cam_step(cap), ai = cam_channel(cap);
    gwin* win0 = (gwin*) win;
    unsigned char* img = win0->img;
    int cx = win0->w, cy = win0->h;
    CvFont font;
    XRECT rects[ 4 ];
    cvInitFont(&font, CC_FONT_HERSHEY_SIMPLEX, 1.0f, 1.0f, 0, 3, 0);

    if (2 == ai) {
      char input[ 256 ] = "";
      char output[ 256 ] = "";
      unsigned char* rgb = NULL;
      unsigned char* gry = NULL;
      int w3 = w * 3;
      int gl = w;
      rgb = MALLOC(unsigned char, h * w * 3);
      gry = MALLOC(unsigned char, h * w);

      //icvYUV4222BGR_8u_C2C3R(h, w, img, al, rgb, w3, 3);
      colorcvt(h, w, I, al, 2, rgb, w3, 3, T_YUYV, T_BGR);
      colorcvt(h, w, rgb, w3, 3, gry, w, 1, T_BGR, T_GRAY);
      //imwrite("aaa.bmp", h, w, rgb, w3, 3);
      img = rgb;
      al = w3;
      ai = 3;
#if 0

      if (cap_or_reg == DO_CAP || cap_or_reg == DO_REG) {
        int j;
        int face_cnt, ouput_id;
        double score = 1000000000.f;

        if (cap_or_reg == DO_CAP) {
          face_cnt = faceRecog(h, w, gry, gl, rects, currid, 0, 0);
        }
        else {
          face_cnt = faceRecog(h, w, gry, gl, rects, -1, &ouput_id, &score);
        }

        if (face_cnt) {
          XRECT* re = rects;
          SKIP_COPY(STDIMGH, STDIMGW, stdface, STDIMGW, 1, img, al, ai);
          SKIP_COPY(STDIMGH, STDIMGW, stdface, STDIMGW, 1, img + 1, al, ai);
          SKIP_COPY(STDIMGH, STDIMGW, stdface, STDIMGW, 1, img + 2, al, ai);
          DrawRectangle2(h, w, img, al, ai, re[ 0 ], CC_RGB(255, 0, 0), 3);

          for (j = 1; j < 3; ++j) {
            DrawRectangle2(h, w, img, al, ai, re[ j ], CC_RGB(255, 0, 0), 1);
          }

          {
            IPOINT center1 = {re[ 1 ].x + re[ 1 ].w / 2, re[ 1 ].y + re[ 1 ].h / 2};
            IPOINT center2 = {re[ 2 ].x + re[ 2 ].w / 2, re[ 2 ].y + re[ 2 ].h / 2};
            DrawCircle(h, w, img, al, ai, center1, 3, CC_RGB(255, 255, 0), -1, 0, 0);
            DrawCircle(h, w, img, al, ai, center2, 3, CC_RGB(255, 255, 0), -1, 0, 0);

            if (center1.x && center2.x) {
              DrawLine(h, w, img, al, ai, center1, center2, CC_RGB(255, 255, 0), 3, 0, 0);
            }
          }

          if (cap_or_reg == DO_CAP) {
#define PIC_NUM_PER_MAN (20)
            gwin* btn;
            char str[ 10 ] = "FACE0";
            str[ 4 ] = '0' + (cap_num % 5);
            ++cap_num;
            btn = gui_find_win(win, str, 0);
            IMRESIZE(STDIMGH, STDIMGW, img, al, 3,
                btn->h, btn->w, btn->img, btn->w * 3, 3, INTER_BILINEARITY);

            if (cap_num >= PIC_NUM_PER_MAN) {
              int i = 0;

              for (i = 0; i < 5; ++i) {
                str[ 4 ] = '0' + (i);
                btn = gui_find_win(win, str, 0);
                MEMSET(btn->img, 0, btn->h * btn->w * 3);
              }

              cap_num = 0;
              //cap_or_reg=DO_NON;
              cap_or_reg = DO_REG;
              savedata();
            }
          }
          else {
            int minid = ouput_id;
            double mindist = score;

            if (minid >= 0) {
              mindist /= 1e5;

              //mindist/=2000.f*(1<<10);
              //mindist*=mindist;
              //mindist*=mindist;
              //mindist = 1.f/(1.f+mindist);
              if (mindist > 1.5) {
                _snprintf(output, 256, "Reg ID = *%d, score = %1.5f", minid, mindist);
              }
              else {
                _snprintf(output, 256, "Reg ID = %d, score = %1.5f", minid, mindist);
              }

              if (output[ 0 ]) {
                IPOINT pos = {110, 30};
                cvPutText(h, w, img, al, ai, output, pos, &font, CC_RGB(255, 0, 0));
              }
            }
          }
        }
      }

#endif
      img = win0->img;
      IMRESIZE(h, w, rgb, w3, 3, cy, cx, img, cx * 3, 3, INTER_BILINEARITY);
      SAFEFREE(rgb);
      SAFEFREE(gry);
    }
    else if (1 == ai) {
      //assert(ai==3);
      IMRESIZE(h, w, I, al, ai, cy, cx, img, cx * 3, 3, INTER_BILINEARITY);
      IMRESIZE(h, w, I, al, ai, cy, cx, img + 1, cx * 3, 3, INTER_BILINEARITY);
      IMRESIZE(h, w, I, al, ai, cy, cx, img + 2, cx * 3, 3, INTER_BILINEARITY);
    }
  }

  on_show_bmp(win, b);
  return 1;
}
#else
int on_show_video(void* win, void* b)
{
  if (cap_or_reg == DO_CAP && currid < 0) {
    return 0;
  }

  if (1) {
    gwin* win0 = (gwin*) win;
    //   unsigned char* img = win0->img;
    //  int cx = win0->w, cy = win0->h;

    CamLoadImage(cap, 480, 640, SHOW_IMG_BUF, 640 * 3, 3);
    YCBCR_resize(H, W, SHOW_IMG_BUF, SHOW_IMG_BUF + H * W, SHOW_IMG_BUF + H * W * 3 / 2, \
        big_length, big_width, buf_resize, \
        buf_resize + big_length * big_width, buf_resize + big_length * big_width * 3 / 2);
    DspOsdVedioImage(big_length, big_width, buf_resize, 0, 0, win0->h, win0->w, win0->img);
#undef big_length
#undef big_width
#undef big_cn

  }

  on_show_bmp(win, b);

  return 1;
}
#endif

gwin* gui_find_win(gwin* w, char* title, gwin** prev);

int on_key_edit(void* win0, void* b)
{
  gwin* btn = (gwin*) win0;
  char ch = (char) b;

  if (ch > 0) {
    int len = strlen(btn->title);

    if (8 == ch) {
      if (len > 0) {
        btn->title[ len - 1 ] = '\0';
      }
    }
    else {
      if (len < MAXTITLELEN - 1) {
        btn->title[ len ] = ch;
        btn->title[ len + 1 ] = '\0';
      }
    }
  }

  return 1;
}

char* number_pad_title[] = {
  "#1", "#2", "#3", "#4", "#5", "#6", "#7", "#8", "#9", "#0", "#<--", "#关闭"
};
int number_pad_title_cnt = sizeof(number_pad_title) / sizeof(number_pad_title[ 0 ]);

int on_click_num(void* win0, void* b)
{
  gwin* btn = (gwin*) win0;
  char* title = btn->title + 1;
  printf("%s\n", btn->title);

  if (('0' <= *title && '9' >= *title)) {
    on_key_edit(b, (void*)(int)(*title));
  }
  else if ('<' == *title) {
    on_key_edit(b, (void*)(int) 8);
  }
  else {
    int i;
    gwin* parent = btn->parent;
    gwin* win2 = parent->btn;

    for (i = 0; i < number_pad_title_cnt;) {
      win2 = gui_find_win(win2, number_pad_title[ i ], 0);
      //if (win2->w==win2->h)
      {
        gui_set_style(win2, ST_HIDE, 0);
        ++i;
      }
    }
  }

  return 1;
}

char* qkey = "0123456789BE";
int gui_add_softkey(char* wintitle, int x, int y, int cx, int cy)
{
  int i;
  gwin* win = NULL;
  gwin* btn = NULL;
  //int x=490, y=70, cx=100, cy=100;
  char** str = number_pad_title;
  win = find_win(wintitle, 0);

  for (i = 0; i < number_pad_title_cnt; ++i) {
    btn = gui_add_btn(wintitle, str[ i ], x + (i % 3) * cx, y + (i / 3) * cy, cx, cy, qkey[ i ], on_click_num, win, BUTTONBK);
    gui_set_style(btn, ST_HIDE, 0);
  }

  return 0;
}

int on_show_edit(void* win0, void* b)
{
  on_show_bmp(win0, b);
  draw_text(win0, 255, 255, 255);
  return 1;
}

int on_click_edit(void* win0, void* b)
{
  int i;
  gwin* btn = ((gwin*) b) ->btn;

  for (i = 0; i < number_pad_title_cnt; ++i) {
    btn = gui_find_win(btn, number_pad_title[ i ], 0);

    if (btn->on_click == on_click_num) {
      gui_set_style(btn, 0, ST_HIDE);
      gui_set_on_click(btn, on_click_num, win0);
    }
  }

  return 0;
}

// 人脸采集
int capface(void* aa, void* bb)
{
  gwin* btn = ((gwin*) aa);

  for (; btn; btn = btn->next) {
    if (btn->on_click == on_click_edit) {
      //strcpy(btn->title, "");
      currid = atol(btn->title);
      break;
    }
  }

  cap_or_reg = DO_CAP;
  //strcpy(btn->title, "");
  //on_click_edit(aa, bb);
  return 0;
}

int test_gui()
{
  gwin* win = NULL;
  gwin* btn = NULL;
  //seq = cam_start("F:\\public\\bin\\MILTrack\\测试序列\\avi%04d.bmp");
#if WIN32

  cap = 0; //cam_start( 0 );
#endif
  //seq = cam_start("F:\\public\\bin\\MILTrack\\new\\img%05d.bmp");
  gui_init(H, W);
  faceRecogInit();
  {
    char* wintitle = "主窗口";
    gui_add_win(wintitle, MAIN_WINDOWBK);
    gui_add_btn(wintitle, "视频浏览A", 240, 75, 173, 45, 'A', Main_ViewVideo, 0, BUTTONBK);
    gui_add_btn(wintitle, "系统留言B", 240, 148, 173, 45, 'B', Main_AudioMessage, 0, BUTTONBK);
    gui_add_btn(wintitle, "听取留言C", 240, 221, 173, 45, 'C', Main_RecvAudio, 0, BUTTONBK);
    gui_add_btn(wintitle, "高级设置D", 240, 294, 173, 45, 'D', Main_PropertiesSet, 0, BUTTONBK);
    gui_add_btn(wintitle, "记录查询E", 240, 367, 173, 45, 'E', Main_RecordQuery, 0, BUTTONBK);
  }
  {
    char* wintitle = "视频浏览窗口";
    gui_add_win(wintitle, WINDOWBK);
    gui_add_btn(wintitle, "返回F", 420, 380, 220, 50, 'F', retmain, "主窗口", BUTTONBK);
    //**
    btn = gui_add_obj(wintitle, "Indentify_Video", 20, 50, 320, 240, 0, 0, 0, BUTTONBK);
    gui_set_on_show(btn, on_show_video);
    //***
    btn = gui_add_obj(wintitle, "Indentify_Face", 420, 110, 220, 140, 0, 0, 0, INDENTIFY_FACE);

#if WIN32

    btn = gui_add_btn(wintitle, "time", 500, 300, 250, 50, 0, 0, 0, 0);
    gui_set_on_show(btn, on_show_digi_time);
    btn = gui_add_btn(wintitle, "date", 500, 350, 250, 40, 0, 0, 0, 0);
    gui_set_on_show(btn, on_show_digi_date);
#endif

  }
  {
    char* wintitle = "系统留言窗口";
    gui_add_win(wintitle, WINDOWBK);
    gui_add_btn(wintitle, "开始留言A", 60, 400, 220, 50, 'A', retmain, 0, BUTTONBK);
    gui_add_btn(wintitle, "留言试听B", 300, 400, 220, 50, 'B', retmain, 0, BUTTONBK);
    gui_add_btn(wintitle, "返回F", 400, 400, 220, 50, 'F', retmain, "主窗口", BUTTONBK);
    gui_add_btn(wintitle, "", 60, 150, 50, 80, 'C', 0, 0, VERIFYPASSWORD_LEFT);
    gui_add_btn(wintitle, "", 500, 150, 50, 80, 'D', 0, 0, VERIFYPASSWORD_RIGHT);
    {
      int i;
      int y = 120, x = 120, cx = 140, cy = 140;

      for (i = 0; i < 3; ++i) {
        char str[ 2 ] = {0};
        str[ 0 ] = '0' + i;
        btn = gui_add_obj(wintitle, str, x + i * (cx + 4), y, cx, cy, 'x', 0, 0, VERIFYPASSWORD_NORMAL);
      }
    }
  }
  {
    char* wintitle = "听取留言窗口";
    win = gui_add_win(wintitle, WINDOWBK);
    gui_add_btn(wintitle, "返回F", 420, 430, 220, 50, 'F', retmain, "主窗口", BUTTONBK);
    gui_add_btn(wintitle, "请输入验证密码", 40, 300, 260, 60, 'A', 0, 0, BUTTONBK);
    {
      int i;
      int y = 120, x = 120, cx = 140, cy = 140;

      for (i = 0; i < 3; ++i) {
        char str[ 20 ] = {0};
        _snprintf(str, 20, "face%d", i);
        btn = gui_add_obj(wintitle, str, x + i * (cx + 4), y, cx, cy, 'x', 0, 0, VERIFYPASSWORD_NORMAL);
      }
    }
    btn = gui_add_obj(wintitle, "", 40, 380, 450, 60, '1', on_click_edit, win, EDITBK);
    gui_set_on_show(btn, on_show_edit);
    gui_set_on_key(btn, on_key_edit);
    gui_set_style(btn, ST_PASSWORD, 0);
    gui_add_softkey(wintitle, 490, 70, 100, 100);
  }
  {
    char* wintitle = "高级设置窗口";
    gui_add_win(wintitle, WINDOWBK);
    gui_add_btn(wintitle, "人脸采集A", 400, 100, 220, 50, 'A', CollectFace, 0, BUTTONBK);
    gui_add_btn(wintitle, "管理员设置B", 400, 180, 220, 50, 'B', ManagerAuthor, 0, BUTTONBK);
    gui_add_btn(wintitle, "系统语音库C", 400, 260, 220, 50, 'C', retmain, 0, BUTTONBK);
    gui_add_btn(wintitle, "GPRS设置D", 400, 340, 220, 50, 'D', PropertiesSet_Gprs, 0, BUTTONBK);
    gui_add_btn(wintitle, "返回F", 400, 420, 220, 50, 'F', retmain, "主窗口", BUTTONBK);
  }
  {
    char* wintitle = "记录查询窗口";
    win = gui_add_win(wintitle, QUERYRECORDBK);
    gui_add_btn(wintitle, "返回F", 400, 320, 220, 50, 'F', retmain, "主窗口", BUTTONBK);
  }


  {
    char* wintitle = "人脸采集";
    win = gui_add_win(wintitle, WINDOWBK);
    btn = gui_add_obj(wintitle, "Indentify_Video", 50, 70, 320, 240, 0, 0, 0, INDENTIFY_VIDEO);
    gui_set_on_show(btn, on_show_video);
    btn = gui_add_obj(wintitle, "Indentify_Face", 400, 110, 220, 140, 0, 0, 0, INDENTIFY_FACE);
    gui_add_btn(wintitle, "采集A", 400, 250, 220, 50, 'A', capface, win, BUTTONBK);
    gui_add_btn(wintitle, "返回F", 400, 300, 220, 50, 'F', retmain, "高级设置窗口", BUTTONBK);
    gui_add_btn(wintitle, "", 60, 400, 50, 80, 'B', 0, 0, VERIFYPASSWORD_LEFT);
    gui_add_btn(wintitle, "", 500, 400, 50, 80, 'C', 0, 0, VERIFYPASSWORD_RIGHT);
    btn = gui_add_obj(wintitle, "", 100, 320, 250, 60, 'D', on_click_edit, win, EDITBK);
    //gui_set_active_key(btn->parent, 'D');
    win->active = btn;
    gui_set_on_show(btn, on_show_edit);
    gui_set_on_key(btn, on_key_edit);
    {
      int i;
      int y = 375, x = 120, cx = 100, cy = 100;

      for (i = 0; i < 5; ++i) {
        char str[ 10 ] = "FACE0";
        str[ 4 ] = '0' + i;
        btn = gui_add_obj(wintitle, str, x + i * (cx + 4), y, cx, cy, 'x', 0, 0, COLLECTFACE_FACEBK);
      }
    }
    gui_add_softkey(wintitle, 490, 70, 100, 100);
  }
  {
    char* wintitle = "管理员设置";
    gui_add_win(wintitle, WINDOWBK);
    gui_add_btn(wintitle, "管理员设置A", 300, 380, 220, 50, 'A', retmain, 0, BUTTONBK);
    gui_add_btn(wintitle, "删除成员信息B", 300, 400, 220, 50, 'B', retmain, 0, BUTTONBK);
    gui_add_btn(wintitle, "初始化密码C", 400, 380, 220, 50, 'C', retmain, 0, BUTTONBK);
    gui_add_btn(wintitle, "返回F", 400, 400, 220, 50, 'F', retmain, "高级设置窗口", BUTTONBK);
    gui_add_btn(wintitle, "", 60, 150, 50, 80, 'D', 0, 0, VERIFYPASSWORD_LEFT);
    gui_add_btn(wintitle, "", 550, 150, 50, 80, 'E', 0, 0, VERIFYPASSWORD_RIGHT);
    {
      int i;
      int y = 120, x = 120, cx = 140, cy = 140;

      for (i = 0; i < 3; ++i) {
        char str[ 2 ] = {0};
        str[ 0 ] = '0' + i;
        btn = gui_add_obj(wintitle, str, x + i * (cx + 4), y, cx, cy, 'x', 0, 0, VERIFYPASSWORD_NORMAL);
      }
    }
  }
  {
    char* wintitle = "GPRS设置窗口";
    win = gui_add_win(wintitle, WINDOWBK);
    gui_add_btn(wintitle, "返回F", 400, 400, 220, 50, 'F', retmain, "高级设置窗口", BUTTONBK);
    btn = gui_add_obj(wintitle, "", 160, 90, 250, 40, 0, on_click_edit, win, EDITBK);
    gui_set_on_show(btn, on_show_edit);
    gui_set_on_key(btn, on_key_edit);
    btn = gui_add_obj(wintitle, "", 160, 130, 250, 40, 0, on_click_edit, win, EDITBK);
    gui_set_on_show(btn, on_show_edit);
    gui_set_on_key(btn, on_key_edit);
    gui_set_style(btn, ST_PASSWORD, 0);
    gui_add_softkey(wintitle, 490, 70, 100, 100);
  }


  //gui_set_active( "主窗口" );
  // gui_set_active( "视频浏览窗口" );
  gui_set_active("人脸采集");
  //gui_set_active("GPRS设置窗口");
  gui_mainloop();
  gui_free_allwin();
  //cam_stop(seq);
  cam_stop(cap);

  faceRecogUnInit();
  return 0;
}

int main()
{
  MEM_CHECK_BEGIN();
  test_gui();
  MEM_CHECK_END();
  return 0;
}

