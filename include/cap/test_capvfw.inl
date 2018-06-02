

typedef unsigned char uchar;

// 图片旋转函数支持90的倍数
// 输入图片数据img  高h 宽w 每行像素占的字节数al 通道数ai
// 输出图片数据rgb2 高ph 宽pw 每行像素占的字节数pal 通道数与输入图片相同
// istrans=1 旋转90度 istrans=2 旋转180度 istrans=3 旋转270度
static int imtrans(int h, int w, const uchar* img, int al, int ai, int* ph, int* pw, uchar* rgb2, int* pal, int istrans, int isflop)
{
  int i, j;

  for (i = 0; i < h; ++i) {
    for (j = 0; j < w; ++j) {
      const uchar* sss;
      uchar* ddd;
      int i1 = i, j1 = j, i2 = i, j2 = j, ll = w;

      if (1 == istrans) {
        i1 = i, j1 = j, i2 = j, j2 = h - i - 1, ll = h;
        sss = img + (i * w + j) * ai;
        ddd = rgb2 + (j * h + (h - i - 1)) * ai;
      }
      else if (2 == istrans) {
        i1 = i, j1 = j, i2 = h - i - 1, j2 = j, ll = w;
        sss = img + (i * w + j) * ai;
        ddd = rgb2 + ((h - i - 1) * w + j) * ai;
      }
      else if (3 == istrans) {
        i1 = i, j1 = w - j - 1, i2 = j, j2 = i, ll = h;
        sss = img + (i * w + w - j - 1) * ai;
        ddd = rgb2 + (j * h + i) * ai;
      }

      if (isflop) {
        j2 = ll - 1 - j2;
      }

      sss = img + (i1 * w + j1) * ai;
      ddd = rgb2 + (i2 * ll + j2) * ai;
      *ddd++ = *sss++;
      *ddd++ = *sss++;
      *ddd++ = *sss++;
    }
  }

  img = rgb2;

  if (istrans % 2) {
    *pw = h, *ph = w, *pal = h * ai;
  }
  else {
    *pw = w, *ph = h, *pal = w * ai;
  }

  return 0;
}


int test_capvfw()
{
  char buf[256];
  static const camera_param_t cp = { 720, 576, 15};//视频参数 720×576 15帧每秒
  capcam* cap = cam_start(0, &cp);    //启动视频
  int trans_flag = 1;//旋转参数
  char ch;

  if (cap) {
    int h, w, al, ai, bl = 0, bi = 0;
    unsigned char* a;
    unsigned char* img = 0;

    for (;;) {
      char buf[256];
      cam_QueryFrame(cap, h, w, a, al, ai);  //从视频流中获取一帧a

      if (NULL == img) {   //分配空间img
        img = MALLOC(unsigned char, h * w * 3);
      }

      // 旋转图片
      imtrans(h, w, a, al, ai, &h, &w, img, &al, trans_flag, 0);
      cvShowImage("cam", h, w, img, al, ai);  // 显示图片img
      ch = cvWaitKey(10);   //获取用户按键并延迟10ms

      if ('q' == ch) {
        break;
      }

      if ('t' == ch) {
        trans_flag++;
        trans_flag %= 4;
      }
    }

    SAFEFREE(img);   //释放图片内存
    cam_stop(cap);   //停止视频
  }

  return 0;
}
