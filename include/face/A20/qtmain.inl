
#if 1
#include "cstd.h"
#include "img/imgio.inl"
#include "cap/capimg.inl"

#define USE_SERIAL 1


#ifdef _WIN32
#include "sys/sys_win32.inl"
//#include "sys/sys_null.inl"
#include "sys/sys_win32_serial_port.inl"
#elif defined __linux__
//#include "sys/sys_linux.inl"
#include "sys/sys_null.inl"
#include "sys/sys_linux_serial_port.inl"
#else
//#error unkown system
#include "sys/sys_null.inl"
#endif

uchar lena_128x128[] = {
#include "pic/lena_128x128.txt"
};

uchar gry_360x288[] = {
#include "pic/gry_360x288.txt"
};

#include "face/face_recog.inl"
#include "draw/HZK16.inl"
//#include "draw/imdrawaa.inl"

static cap_t cap[2] = {0};
static facerecogapp_t app[1] = {0};
static font_t fo[1] = {0};
#endif

#ifndef _WIN32
#include "face/A20/cap.inl"
#endif

//#include "str/inifile.inl"

img_t g_imv[1] = {0};
img_t g_imrgb[1] = {0};

sys_serial_port_t g_ss[1] = {0};

void WorkerThread::run()
{
    img_t* im = g_imv;
    int video_trans = 0;
    int screen_flip = 0;
    int screen_flop = 0;
    const char* inifile = "mainwindow.ini";
#ifdef _WIN32
    inifile = "C://mainwindow.ini";
#endif
    while (1) {
        int trans = 0;
    #if 0
        a20cap_getframe(im);
        QImage* image = new QImage(im->tt.data, im->w, im->h, QImage::Format_RGB888);
        center->setPixmap(QPixmap::fromImage(*image));
        center->setAlignment(Qt::AlignCenter);
        //center->setScaledContents(true);
        delete image;
    #endif

    #if 1
    #ifdef _A20_CAP_INL_
        a20cap_getframe(im);
        trans = 3;
    #else
        if (cap->x && cap_getframe(cap, im, T_BGR)) {
            //imtrans2(im, im, 1, 0);
            //trans = 1;
        } else {
            img_t im1[1] = {0};
            IMINIT(im1, 288, 360, gry_360x288, 360, 1, 1);
            imcolorcvt(im1, im, T_GRAY, T_RGB);
            //imtrans(im, im, 1, 0, 0);
            imresize(im, im->h*2, im->w*2, im);
        }
    #endif
        imtrans2(im, im, trans, 1);
        if (im->tt.data) {
            img_t* imrgb = im;
            const char* s_pixtype = "RGB";
            double ss = 1;
            app->trans = 0;
            facerecogapp_frame(app, im->h, im->w, im->tt.data, im->s, im->c, s_pixtype, imrgb->tt.data, imrgb->s);

            imresize(im, im->h/ss, im->w/ss, im);
            if (app->ret) {
                int i;
                for (i=0; i<3; ++i) {
                    const int* xywh = app->xywh + i*4;
                    //DRECT drc = dRECT2(xywh[0]/2, xywh[1]/2, xywh[2]/2, xywh[3]/2);
                    //imdrawaa_rect_stroke(im, 0, NULL, drc, _rgb(0, 0, 1), 2);
                    imput_rect(im, xywh[0]/ss, xywh[1]/ss, xywh[2]/ss, xywh[3]/ss, 0, _rgb(0, 0, 1), 2);
                }
                const int* xywh = app->xywh;
                COLOR clr = _rgb(1,1,1);
                char buf[256] = {0};
                switch (app->mode) {
                case CapFace:
                    snprintf(buf, 256, "²É¼¯%d/%dÕÅ", app->cur_pic_count, app->max_cap_pic);
                    clr =_rgb(0, 0, 1);
                    break;
                case Recog11:
                case Recog1N:
                    snprintf(buf, 256, "id=%s", app->curid.m_ID);
                    if (app->rec_ok) {
                        clr = _rgb(0, 1, 0);
                    }
                    break;
                default:
                    break;
                }
                imput_str(im, xywh[0]/ss, xywh[1]/ss, buf, -1, clr);
                snprintf(buf, 256, "men=%d", app->men_num);
                imput_str(im, 200, 0, buf, -1, _rgb(1,1,1));
            }
            //imtrans2(im, im, 2, 0);
            show_cnsun_logo(im);
            imtrans2(im, im, 3, 0);
            imsetsize(g_imrgb, im->h, im->w, im->c, 1);
            imcopy(im, g_imrgb);
            //imflip_lr(im);
            emit notify(0);
            QThread::msleep(5);
        }
    #endif
    }
}

int test_facerecog_msg_default(const facerecogapp_t* app, const char* buf) {
  static int i = 0;
  if (g_ss->x && g_ss->send) {
    int len = strlen(buf);
    {
      char shortstr[50];
      int len2 = cstr_shortstr(buf, len, shortstr, 40);
      shortstr[len2] = 0;
      printf("send(%d) : %s", len, shortstr);
    }
    sys_serial_port_send(g_ss, buf, len);
  } else {
    sys_serial_port_open(g_ss, 7, 115200, 'N', 8, 1);
    printf("sys_serial_port_send fail %x %x\n", g_ss->x, g_ss->send);
  }
  ++i;
  return 0;
}

int my_recv_message_2(sys_serial_port_t* ss, char* buf, int len, UINT Msg) {
  if (len>1) {
    buf[len] = 0;
    {
      char shortstr[50];
      int len2 = cstr_shortstr(buf, len, shortstr, 40);
      shortstr[len2] = 0;
      printf("recv(%d):%s", len, shortstr);
    }
    facerecogapp_cmd(app, buf);
  }
  return 0;
}

int my_recv_message(sys_serial_port_t* ss, char* buf, int len, UINT Msg) {
  int i;
  if ((ss->pos+len)>=ss->bf->len) {
    int newlen = next_power(ss->pos+len);
    bfsetsize(ss->bf, newlen);
  }
  //printf("%d\n", bf->pos);
  for (i=0; i<len; ++i) {
    uchar t = buf[i];
    ss->bf->data[ss->pos++] = t;
    if ('\n'==t) {
      ss->bf->data[ss->pos] = 0;
      my_recv_message_2(ss, (char*)ss->bf->data, ss->pos, Msg);
      ss->pos = 0;
    }
  }
  return 0;
}

WorkerThread *thread = 0;

MainWindow::MainWindow(const QMap<QString, QSize> &customSizeHints,
                       QWidget *parent, Qt::WindowFlags flags)
                       : QMainWindow(parent, flags)
{
  setObjectName("MainWindow");
  setWindowTitle("Qt Main Window Demo");
  
  //center = new QLabel(this);
  //center->setReadOnly(true);
  //center->setMinimumSize(400, 205);
  center=new QLabel(this);
#if 1
  QImage *image=NULL;
  
  if (0) {
    img_t im[1] = {0};
    imread("C:/ËÕAQ5B65.jpg", 3, 1, im);
    image = new QImage(im->tt.data, im->w,im->h,QImage::Format_RGB888);
    imfree(im);
  } else {
    image = new QImage(lena_128x128, 128, 128, QImage::Format_Indexed8);
  }
  center->setGeometry(0,0,50,50);
  center->setPixmap(QPixmap::fromImage(*image));
#endif
  //center->setPixmap(QPixmap("E:/Qt_Program/Chrysanthemum.jpg"));
  center->show();
  
  setCentralWidget(center);
  
  //setupToolBar();
  //setupMenuBar();
  //setupDockWidgets(customSizeHints);
  
  //statusBar()->showMessage(tr("Status Bar"));
#if 1
  facerecogapp_cmd(app, NULL);
  facerecogapp_cmd(app, "IDENT_ON=1");
  
  //app->save = test_facerecog_save;
  //app->load = test_facerecog_load;
  app->msg = test_facerecog_msg_default;
  
  facerecogapp_loadall(app);
  sys_serial_port_open(g_ss, 7, 115200, 'N', 8, 1);
  if (NULL==g_ss->x) {
    printf("sys_serial_port_open fail\n");
  }
  g_ss->recv_message = my_recv_message;
  g_ss->user = app;
  
  HZK16_font_init(fo);
#endif
#ifdef _A20_CAP_INL_
  a20cap_start();
#else
  capimgseq_open(cap, "list.txt");
#endif
  //startTimer(200);
  WorkerThread* workthread;
  workthread = new WorkerThread(this);
  connect(workthread,SIGNAL(notify(int)),this,SLOT(OnNotify(int)));
  workthread->start();
}