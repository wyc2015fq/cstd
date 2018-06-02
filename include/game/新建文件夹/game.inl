//#include "../stdafx.h"
#include <mmsystem.h>
#pragma comment (lib,"winmm.lib")
#include <wingdi.h>
#pragma comment (lib,"gdi32.lib")
#pragma comment (lib,"msimg32.lib")
#include <vfw.h>
#pragma comment (lib,"vfw32.lib")
#define SU_LIANG 500
#define TIMER 150
#define WIDTH 640
#define HEIGHT 480
#define SCRWI 800
#define SCRHE 600
#define GX 40
#define GY 30
#define SCRP 12
#define SBUF 2801
#define RBUF 801
#define JBUF 100
typedef struct {
  short int x;
  short int y;
} PATHN;
typedef struct {
  short int qi;
  short int bc;
} JCDZ;
typedef struct {
  short int jisu;
  short int xix, xiy;
  short int x0, y0;
  //short int w, h;
  Image32* im;
  short int lb; // 0=ÈË 1=ÊÞ 2=¾°
  short int p;
  short int m0, m1;
  short int zs;
  short int js;
  short int fw;
  short int zd;
  PATHN ph[250];
  short int pk;
  short int fid;
  short int fx, fy;
} MAN;
JCDZ zjdz[6] = {0, 5, 40, 10, 120, 10, 200, 10, 280, 10, 360, 5};
JCDZ zjdw[6] = {0, 5, 40, 10, 120, 10, 200, 10, 280, 10, 360, 5};
unsigned short dw[7][6] = {
  //
  0, 1, 1, 3, 4, 5,
  0, 1, 2, 3, 4, 5,
  0, 1, 1, 3, 4, 5,
  0, 1, 2, 3, 4, 5,
  0, 2, 2, 2, 4, 5,
  0, 1, 1, 1, 4, 5,
  0, 1, 1, 3, 4, 5
};
class game2D : public Engine3D
{
public:
  MAN man[SU_LIANG];
  short int rs;
  short int scrx, scry;
  short int mans;
  short int sbufx[SBUF];
  short int sbufy[SBUF];
  short int rbufx[RBUF];
  short int rbufy[RBUF];
  short int find_p;
  short int sys;
  Image32* imgbak;
  BOOL Init() {
    Engine3D::Init();
    sys = getsys();
    scrx = 0;
    scry = 0;
    rs = 0;
    find_p = 0;
    imgbak = 0;
    return TRUE;
  }
  void exit() {
  }
  int getsys() {
    OSVERSIONINFO stOSVI ;
    ZeroMemory(&stOSVI , sizeof(OSVERSIONINFO)) ;
    stOSVI.dwOSVersionInfoSize = sizeof(OSVERSIONINFO) ;
    GetVersionEx(&stOSVI);
    int a = 0;
    if (stOSVI.dwPlatformId == VER_PLATFORM_WIN32_WINDOWS &&
        (stOSVI.dwMajorVersion > 4 || (stOSVI.dwMajorVersion == 4
            && stOSVI.dwMinorVersion > 0))) {
      a = 1;
    }
    if (VER_PLATFORM_WIN32_NT == stOSVI.dwPlatformId && stOSVI.dwMajorVersion >= 5) {
      a = 2;
    }
    if (VER_PLATFORM_WIN32_NT == stOSVI.dwPlatformId && stOSVI.dwMajorVersion == 4) {
      a = 3;
    }
    if (VER_PLATFORM_WIN32_NT == stOSVI.dwPlatformId) {
      a = 4;
    }
    return a;
  }
  BOOL inscreer(int i) {
    int xs = man[i].xix - scrx - man[i].im->width / 2;
    int ys = man[i].xiy - scry - man[i].im->height;
    if (xs > (-man[i].im->width + 2) && xs < WIDTH && ys > (-man[i].im->height + 10) && ys < HEIGHT) {
      return TRUE;
    }
    else {
      return FALSE;
    }
  }
  void sort() {
    MAN man0;
    for (int a = 0; a < rs - 1; a++)
      for (int b = a + 1; b < rs; b++)
        if (man[a].xiy > man[b].xiy) {
          man0 = man[a];
          man[a] = man[b];
          man[b] = man0;
        }
    mans = 0;
  }
  void setman(int q) {
    int a = 400;
    if (man[q].lb == 0) {
      man[q].m0 = man[q].js * a + zjdz[man[q].zs].qi
          + man[q].fw * zjdz[man[q].zs].bc;
      man[q].m1 = zjdz[man[q].zs].bc + man[q].m0;
      man[q].p = man[q].m0;
    }
    if (man[q].lb == 1) {
      man[q].m0 = man[q].js * a + zjdw[man[q].zs].qi
          + man[q].fw * zjdw[man[q].zs].bc;
      man[q].m1 = zjdw[man[q].zs].bc + man[q].m0;
      man[q].p = man[q].m0;
    }
    man[q].zd = 0;
  }
  void manmove(int i) {
    if (man[i].lb == 2) {
      return;
    }
    if (find_p == 1 && man[i].pk < 1) {
      return;
    }
    int stx, sty, qx, qy;
    switch (man[i].zs) {
    case 2: {
      stx = 9;
      sty = 6;
      break;
    }
    case 1: {
      stx = 4;
      sty = 2;
      break;
    }
    default: {
      stx = 2;
      sty = 1;
      break;
    }
    }
    qx = man[i].x0 - man[i].xix;
    qy = man[i].y0 - man[i].xiy;
    if (qx == 0 && qy == 0) {
      if (find_p == 1) {
        find_a(i);
      }
      return ;
    }
    int qxa = abs(qx);
    int qya = abs(qy);
    if (qxa < stx) {
      stx = qxa;
    }
    if (qya < sty) {
      sty = qya;
    }
    if (qx != 0) {
      man[i].xix += qxa / qx * stx;
    }
    if (qy != 0) {
      man[i].xiy += qya / qy * sty;
    }
  }
  void bianfw(int q) {
    int qx = man[q].x0 - man[q].xix;
    int qy = man[q].y0 - man[q].xiy;
    if (qx == 0 && qy == 0) {
      if (man[q].zd == 0) {
        man[q].zs = 0;
      }
      goto aa;
    }
    if (man[q].zd == 0) {
      int a = rand() % 2 + 1;
      if (man[q].lb == 0) {
        man[q].zs = 1;
      }
      if (man[q].lb == 1) {
        man[q].zs = dw[man[q].js][a];
      }
    }
    if (qx < 0 && qy > 0) {
      man[q].fw = 1;
      goto aa;
    }
    if (qx < 0 && qy < 0) {
      man[q].fw = 3;
      goto aa;
    }
    if (qx > 0 && qy < 0) {
      man[q].fw = 5;
      goto aa;
    }
    if (qx > 0 && qy > 0) {
      man[q].fw = 7;
      goto aa;
    }
    if (qy > 0) {
      man[q].fw = 0;
      goto aa;
    }
    if (qx < 0) {
      man[q].fw = 2;
      goto aa;
    }
    if (qy < 0) {
      man[q].fw = 4;
      goto aa;
    }
    if (qx > 0) {
      man[q].fw = 6;
      goto aa;
    }
aa:
    setman(q);
    if (man[q].zs == 0) {
      man[q].p = man[q].p + rand() % 3;
    }
  }
  void loadmap(const char* name) {
    FILE* f;
    char mapbak[256];
    char buf[256];
    //SetCurrentDirectory(appdir);
    f = fopen(name, "r");
    if (f == NULL) {
      printf("Ã»ÓÐµØÍ¼ÎÄ¼þ!!!");
      return;
    }
    fscanf(f, "%s\n", mapbak);
    fscanf(f, "%d\n", &rs);
    _snprintf(buf, 256, "./Í¼Æ¬/%s", mapbak);
    imgbak = loadbmpfile(buf, 32, 1);
    assert(imgbak);
    if (rs > SU_LIANG) {
      fclose(f);
      return;
    }
    int i;
    for (i = 0; i < rs; i++) {
      fscanf(f, "%d,", &man[i].jisu);
      fscanf(f, "%d,", &man[i].lb);
      fscanf(f, "%d,", &man[i].js);
      fscanf(f, "%d,", &man[i].p);
      fscanf(f, "%d,", &man[i].xix);
      fscanf(f, "%d,", &man[i].xiy);
      man[i].x0 = man[i].xix;
      man[i].y0 = man[i].xiy;
      man[i].fw = 1;
      man[i].zs = 0;
      man[i].pk = -1;
      man[i].fid = -1;
      man[i].zd = 0;
      if (man[i].lb != 2) {
        setman(man[i].jisu);
      }
      getpic(i);
    }
    fclose(f);
  }
  BOOL getpic(int q) {
    int lb = man[q].lb, p = man[q].p;
    char name[256];
    char* strlb[] = {"ÈË", "ÊÞ", "¾°"};
    char* cc = strlb[lb];
    _snprintf(name, 256, "./Í¼Æ¬/%s/c%05d.bmp", cc, p);
    man[q].im = loadbmpfile(name, 32, 1);
    if (!man[q].im) {
      return FALSE;
    }
    _snprintf(name, 256, "./Í¼Æ¬/%s/c%05d.txt", cc, p);
    FILE* f;
    f = fopen(name, "r");
    if (f != NULL) {
      if (lb == 0) {
        fscanf(f, "%d,%d", &rbufx[p], &rbufy[p]);
      }
      if (lb == 1) {
        fscanf(f, "%d,%d", &sbufx[p], &sbufy[p]);
      }
      fclose(f);
    }
    return TRUE;
  }
  void find_a(int i) {
    if (man[i].fid < man[i].pk) {
      man[i].x0 = man[i].ph[man[i].fid].x * GX + man[i].im->width / 2;
      man[i].y0 = man[i].ph[man[i].fid].y * GY + man[i].im->height / 2;
      man[i].fid++;
      if (man[i].x0 < GX) {
        man[i].x0 = GX;
      }
      if (man[i].y0 < GX) {
        man[i].y0 = GY;
      }
      bianfw(i);
    }
    else {
      man[i].x0 = man[i].fx;
      man[i].y0 = man[i].fy;
    }
  }
  int setobj(int q) {
    int x = man[q].xix - scrx - man[q].im->width / 2;
    int y = man[q].xiy - scry - man[q].im->height;
    if (inscreer(q)) {
      if (man[q].lb != 2) {
        int x0 = 0, y0 = 0;
        if (man[q].lb == 0) {
          x0 = rbufx[man[q].p];
          y0 = rbufy[man[q].p];
        }
        if (man[q].lb == 1) {
          x0 = sbufx[man[q].p];
          y0 = sbufy[man[q].p];
        }
        if (man[q].fw > 4) {
          x0 = man[q].im->width - x0;
        }
        x = man[q].xix - scrx - x0;
        y = man[q].xiy - scry - y0;
      }
      Draw_Image(x, y, man[q].im->width, man[q].im->height,
          (const _PIXEL*)(man[q].im + 1), man[q].im->width * 4, -1);
      //TransparentBlt2(BkDC1, x, y, w, h, MemDC, 0, 0, w, h, RGB(255, 255, 255));
      mans++;
    }
    if (man[q].lb == 2) {
      return 0;
    }
    else {
      manmove(q);
    }
    man[q].p++;
    if (man[q].p >= man[q].m1) {
      bianfw(q);
    }
    return 0;
  }
  void Memcpy0(unsigned char* mu, unsigned char* yu, int z) {
    memcpy(mu, yu, z);
    return;
    int z0 = z / 32;
    int z1 = z - z0 * 32;
    _asm {
      //pand mm0,mm4 ;//packed and
      mov esi, yu
      mov edi, mu
      mov ecx, z0
      Mcont:
      movq mm0, qword ptr [esi]
      movq qword ptr [edi], mm0
      movq mm0, qword ptr [esi][8]
      movq qword ptr [edi][8], mm0
      movq mm0, qword ptr [esi][16]
      movq qword ptr [edi][16], mm0
      movq mm0, qword ptr [esi][24]
      movq qword ptr [edi][24], mm0
      add esi, 8*4
      add edi, 8*4
      loop Mcont
      mov ecx, z1;
      repe movsb;
      emms ;//empty MMX state
    }
  }
  int Draw_Frame(HDC hDC) {
    sort();
    Draw_Image(0, 0, imgbak->width, imgbak->height, (const _PIXEL*)(imgbak + 1), imgbak->width * 4, -1);
    for (int i = 0; i < rs; i++) {
      setobj(i);
    }
    Engine3D::Draw_Frame(hDC);
    return 0;
  }
};

