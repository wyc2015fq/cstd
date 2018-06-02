#include "cstd.h"
#include "BRender.inl"
//#include "test_DrawAxes.inl"
int test_BRender()
{
#if 0
  //第一阶段, 建立场景，设置光源和材质属性，完成光照计算
  //取景器
  Object3d pObj[10];
  int oblen = 0;
  RenderDC rDC[1];
  static int dwRop = 5;

  //delObject3d(pSphere, 3);
  initRenderDC(rDC, hDC);

  //光源
  //设置两个光源

  //设置第一盏灯(泛光灯)
  Lightff(&rDC->lights[0],  G3D_AMBIENT, 0.2f, 0.2f, 0.2f, 1.0f);
  Lightff(&rDC->lights[0],  G3D_DIFFUSE, 1.0f, 1.0f, 1.0f, 1.0f);
  Lightff(&rDC->lights[0], G3D_SPECULAR, 1.0f, 1.0f, 1.0f, 1.0f);
  Lightff(&rDC->lights[0], G3D_POSITION, 8.0f, 8.0f, 8.0f, 1.0f);
  //Lightf(&pLights[0], G3D_CONSTANT_ATTENUATION, 2.5f);
  //Lightb(&pLights[0], G3D_LIGHT_ON_OFF, FALSE);
  //设置第二盏灯(聚光灯)
  Lightff(&rDC->lights[1],        G3D_AMBIENT, 0.5f, 0.5f, 0.5f, 1.0f);
  Lightff(&rDC->lights[1],        G3D_DIFFUSE, 1.0f, 1.0f, 1.0f, 1.0f);
  Lightff(&rDC->lights[1],       G3D_SPECULAR, 1.0f, 1.0f, 1.0f, 1.0f);
  Lightff(&rDC->lights[1],       G3D_POSITION, 8.0f, 8.0f, 8.0f, 1.0f);
  Lightff(&rDC->lights[1], G3D_SPOT_DIRECTION, 0.0f, -1.0f, -1.0f, 1.0f);

  Lightf(&rDC->lights[1], G3D_SPOT_CUTOFF, 90.3f);
  Lightf(&rDC->lights[1], G3D_SPOT_EXPONENT, 1.0f);
  rDC->nNumLight = 2;

  //pLights[1].Lightf(G3D_CONSTANT_ATTENUATION, 2.5f);
  //pLights[1].Lightb(G3D_LIGHT_ON_OFF, FALSE);
  if (1) {
    // 定义材质
    Material Materials[] = {
      {{1.f, 1.f, 1.f, 1.0f}, {1.f, 1.f, 1.f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.f, 0.f, 0.f, 1.0f}, 50.0f, G3D_FRONT}, //黄色材质
      {{0.2f, 0.2f, 0.2f, 1.0f}, {0.8f, 0.8f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, 50.0f, G3D_FRONT}, //黄色材质
      {{0.2f, 0.2f, 0.2f, 1.0f}, {0.8f, 0.0f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, 50.0f, G3D_FRONT}, //红色材质
      {{0.2f, 0.2f, 0.2f, 1.0f}, {0.0f, 0.8f, 0.0f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, 50.0f, G3D_FRONT}, //绿色材质
      {{0.2f, 0.2f, 0.2f, 1.0f}, {0.0f, 0.0f, 0.8f, 1.0f}, {1.0f, 1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, 50.0f, G3D_FRONT}, //蓝色材质
      {{0.247f, 0.200f, 0.075f, 1.0f}, {0.752f, 0.606f, 0.226f, 1.0f}, {0.628f, 0.556f, 0.336f, 1.0f}, {0.0f, 0.0f, 0.0f, 1.0f}, 51.2f, G3D_FRONT}, //金
    };
    static int mid = 0;
    int pp = 10;
    int maxmid = countof(Materials) * pp;
    *rDC->material = Materials[mid / pp];
    //if (++mid>=maxmid) mid=0;
  }

#define CENTRALIZATION(pO)  Centralization((pO)->vListLen, (pO)->vList, .5)

  //cvWaitKey(-1);
  //球
  if (0) {
    FRGBA clr;
    float rrr = 0.5f;
    FRGBASET(clr, 1.f, 0, 0, 1.f);
    initSphere(pObj + oblen, 1, rrr, 32, 32, clr);
    CENTRALIZATION(pObj + oblen);
    oblen++;
    FRGBASET(clr, 0, 1.f, 0, 1.f);
    initCylinder(pObj + oblen, 1, 0.0f, 0.5f, 0.5f, 32, 32, clr);
    SwapXYZ(pObj + oblen, 0, 2, 1);
    CENTRALIZATION(pObj + oblen);
    oblen++;
  }

  //正方体
  if (0) {
    FRGBA clr;
    char* plgf = "D:/code/c/游戏引擎/code/Triangle Renderer Project/Cube.plg";
    char* plgf2 = "D:/code/c/游戏引擎/code/Triangle Renderer Project/Triangle.plg";
    loadObject_PLG(pObj + oblen, 1, plgf);
    CENTRALIZATION(pObj + oblen);
    FRGBASET(clr, 1.f, 0.f, 0.f, 1.f);
    SetObjColor(pObj + oblen, clr);
    oblen += 1;
  }

  if (0) {
    char* plgf3 = "D:/TDDOWNLOAD/code/3D/off_model_lib/m110.off";
    loadObject_OFF(pObj + oblen, 1, plgf3);
    CENTRALIZATION(pObj + oblen);
    oblen += 1;
  }

  if (0) {
    char* plgf4 = "D:/TDDOWNLOAD/code/3D/3d模型浏览器tm格式/manhead.tm";
    loadObject_TM(pObj + oblen, 1, plgf4);
    CENTRALIZATION(pObj + oblen);
    oblen += 1;
  }

  if (0) {
    static int fram = 0;
    _MD2HEADER* pmd2;
    char* md2tex = "D:/code/c/游戏引擎/code/Triangle Renderer Project/solo_anh.bmp";
    char* md2f = "D:/code/c/游戏引擎/code/Triangle Renderer Project/hansolo.md2";
    pmd2 = (_MD2HEADER*)loadfile(md2f, 0);
    extractFrame_MD2(pObj + oblen, 1, pmd2, fram++);
    CENTRALIZATION(pObj + oblen);
    loadbmpfile(md2tex, 32, 1, pObj[oblen].tex);
    oblen += 1;

    if (fram >= pmd2->num_frames) {
      fram = 0;
    }

    free(pmd2);
  }

  //组织场景, 进行几何变换
  //变换矩阵
  {
#define ROTATE_RATE   5.0f
    //方位矩阵
    static MATRIX3D mm;
    static int initedmm = 0;
    float x;
    int i;
    int dwRops[] = {
      G3D_RENDER_VERTICES,
      G3D_RENDER_WIRE,
      G3D_RENDER_WIRE_LIGHTED,
      G3D_RENDER_FLAT,
      G3D_RENDER_FLAT_TEX,
      G3D_RENDER_GOURAUD_SMOOTH_TEX,
      G3D_RENDER_GOURAUD_SMOOTH,
    };
#define KEYLEFT   0x250000
#define KEYUP     0x260000
#define KEYRIGHT  0x270000
#define KEYDOWN   0x280000

    if (ch > 0) {
      printf("%x\n", ch);
    }

    if (KEYUP == ch) {
      M3TRANSLATE(mm, 0, 0.0f, 1.0f);
    }

    if (KEYDOWN == ch) {
      M3TRANSLATE(mm, 0, 0.0f, -1.0f);
    }

    if (KEYLEFT == ch) {
      M3TRANSLATE(mm, .5f, 0.0f, 0.0f);
    }

    if (KEYRIGHT == ch) {
      M3TRANSLATE(mm, -.5f, 0.0f, 0.0f);
    }

    if (0 == initedmm) {
      initedmm = 1;
      M3IDENTITY(mm);
    }

    if (ch > 0) {
      printf("%d\n", ch);
    }

    if (ch >= '1' && ch <= '9') {
      dwRop = ch - '1';
    }

    if ('0' == ch) {
      M3IDENTITY(mm);
    }

    if ('x' == ch) {
      M3RotateX(&mm, ROTATE_RATE);
    }

    if ('X' == ch) {
      M3RotateX(&mm, -ROTATE_RATE);
    }

    if ('c' == ch) {
      M3RotateY(&mm, ROTATE_RATE);
    }

    if ('C' == ch) {
      M3RotateY(&mm, -ROTATE_RATE);
    }

    if ('z' == ch) {
      M3RotateZ(&mm, ROTATE_RATE);
    }

    if ('Z' == ch) {
      M3RotateZ(&mm, -ROTATE_RATE);
    }

    *rDC->m = mm;
    //test_DrawAxes(rDC);

    dwRop = (dwRop + countof(dwRops)) % countof(dwRops);
    //matrixStack.Push(mGeometry);
    //上一层球
    x = -hDC->w / (30 * rDC->pVF->xScale);
    M3TRANSLATE(*rDC->m, x, 0.0f, 0.0f);
    x = 1.f;
    rDC->dwRop = dwRops[dwRop];

    if (0) {
      int len;
      static t3DModel pModel[1];
      static char* buf = 0;

      if (0 == buf) {
        char* plgf, *plgfp;
        plgf = "D:/TDDOWNLOAD/code/3D/3DSLoader/model1.3ds";
        plgfp = "D:/TDDOWNLOAD/code/3D/3DSLoader/";
        plgf = "D:/TDDOWNLOAD/3D/new/12/Woman  N230708/Woman  N230708.3ds";
        plgfp = "D:/TDDOWNLOAD/3D/new/12/Woman  N230708/";
        buf = (char*)loadfile(plgf, &len);
        load3DS(pModel, buf);
        load3DSTex(pModel, plgfp);
      }

      M3TRANSLATE(*rDC->m, x, 0.0f, 0.0f);

      if (1) {
        utime_start(_start_time);
        Render3DS(rDC, pModel);
        printf("%f\t", utime_elapsed(_start_time));
      }

      //load3DSTex(pModel, 0);
      //free(buf);
    }

    if (0) {
      utime_start(_start_time);

      for (i = 0; i < oblen; ++i) {
        M3TRANSLATE(*rDC->m, x, 0.0f, 0.0f);
        rDC->tex = pObj[i].tex;
        RenderObject3d(rDC, pObj + i);
      }

      printf("%f\t", utime_elapsed(_start_time));
    }
  }
  delRenderDC(rDC);
  delObject3d(pObj, oblen);
#endif
  return 0;
}
