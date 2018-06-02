
enum {showui = 0, minh = 100, minw = 100};

#if 0
enum {sig_n = 16, sig_thd = sig_n, sig_nn = sig_n * sig_n};
typedef struct sig_t {
  uchar a[sig_nn];
} sig_t;
#define imsig(H, W, A, AL, AI, B)   { uchar bbb[sig_nn]={0}; imsig_base(H, W, A, AL, AI, sig_n, bbb, B); }
//#define SIG_EQ(S1, S2)  (0==memcmp(&(S1), &(S2), sig_nn))
#define SIG_EQ(S1, S2)  (count_xor(sig_nn, (S1).a, (S2).a)<sig_thd)
#else
typedef uint64 sig_t;
#define imsig imsig_8x8
#define SIG_EQ(S1, S2)  ((S1)==(S2))
#endif

int test_findrepeatimg()
{
  img_t im[3] = {0};
  FILE* pf = 0;
  const char* flist = "list.txt";
  const char* replist = "rep.txt";
  sig_t* sig = 0;
  int* siz = 0;
  vstr_t sv[1] = {0};
  str_t str[1] = {0};
  int i, j, n, cnt = 0;
  _chdir("D:/TDDOWNLOAD/pic/jpg");
  _chdir("C:/www/www.030buy.com/jpg");
  _chdir("D:/pub/bin/iris/CASIA-IrisV3-Lamp");
  _chdir("D:/www/gif/addictedtoeveythingwomen.tumblr.com");
  _chdir("D:/TDDOWNLOAD/pic/壁纸/大唐无双美女名将");
  _chdir("D:/TDDOWNLOAD/pic/壁纸/新建文件夹");
  _chdir("D:/TDDOWNLOAD/pic/壁纸");
  _chdir("D:/TDDOWNLOAD/pic/漫画");
  _chdir("D:/pub/bin/imgproc/findrepeatimg");
  _chdir("D:/pub/pic");
  _chdir("C:/老梁故事汇/男人装");
  _chdir("D:/TDDOWNLOAD/pic");
  _chdir("C:/pic/螺旋猫");
  _chdir("C:/pic/gif");
  _chdir("C:/pic");
  _chdir("C:/pic/aaa");
  _chdir("C:/pic/新建文件夹");
  _chdir("C:/TDDOWNLOAD/www/huaban.com");
  vstr_load(flist, sv);
  n = sv->n;
  pf = fopen(replist, "wb");
  sig = MALLOC(sig_t, n);
  siz = MALLOC(int, n);
  MEMSET(sig, 0, n);

  for (i = 0; i < n; ++i) {
    //sv->v[i].s[sv->v[i].l]=0;
    if (imread(sv->v[i].s, 1, 1, im)) {
      int k = -1;

      if ((im)->h < minh || (im)->w < minw) {
        fprintf(pf, "toomin_%dx%d*%s*%s\r\n", (im)->h, (im)->w, sv->v[i].s, sv->v[i].s);
        fflush(pf);
        ++cnt;
        continue;
      }

      imsig((im)->h, (im)->w, (im)->tt.data, (im)->s, (im)->c, (sig + i));
      siz[i] = im->h * im->w * im->f;

      for (j = 0; j < i; ++j) {
        if (SIG_EQ(sig[j], sig[i])) {
          if (siz[j] < siz[i]) {
            T_SWAP(str_t, sv->v[j], sv->v[i]);
            T_SWAP(int, siz[j], siz[i]);
          }

          k = j;
        }
      }

      if (k >= 0) {
        int dodel = !showui;
        j = k;
        ++cnt;

        // 大图*小图
        if (showui) {
#ifdef _IMDRAW_INL_
          char buf[32];
          img_t* im1 = im + 1;
          char ch = 0;
          imread(sv->v[i].s, 3, 1, im);
          imread(sv->v[j].s, 3, 1, im1);
          _snprintf(buf, 32, "%d", i);
          //imdraw_text_v(im, 0, buf, im->w / 2, im->h / 2, 0, _rgb(1, 0, 1));
          _snprintf(buf, 32, "%d", j);
          //imdraw_text_v(im1, 0, buf, im1->w / 2, im1->h / 2, 0, _rgb(1, 0, 1));
          imcath(2, im, im + 2);
          imshow2("ttt", im + 2);
          printf("%s*%s\n", sv->v[j].s, sv->v[i].s);
          ch = cvWaitKey(-1);

          if ('a' == ch || 'A' == ch) {
            dodel = 1;
          }
#endif
        }

        if (dodel) {
          fprintf(pf, "repeat_%dx%d*%s*%s\r\n", (im)->h, (im)->w, sv->v[i].s, sv->v[j].s);
          fflush(pf);
        }
      }

      printf("%d %d %d\r", i, n, cnt);
    }
  }

  FREE(sig);
  FREE(siz);
  freeims(im, 3);
  str_free(str);
  strv_free(sv);
  fclose(pf);
  return 0;
}
