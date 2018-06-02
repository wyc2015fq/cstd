
//#include "debug.h"
#include <math.h>
#include "iris.h"
#include "rand.h"
#include "img/color.inl"
#include "iris/Irisnormalize.inl"

int test_Irisnormalize()
{
  FILE* plisttxt;
  int fram, i, j, k, picnum, irdistlen;
  char buf[256];
  img_t im[1] = {0};
  int Classnum = 70, snuml = 10, snumr = 4, row = 30, line = 360;
  IrisDist_t* irdist;
  double* dist;

#ifdef _DEBUG
  Iris_t iris[1000];
#else
  Iris_t iris[10000];
#endif
  _chdir("F:/pub/bin/iris/Irisnormalize_pic");
  _chdir("F:/pub/bin/iris/glassReResult929/out_270");
  _chdir("F:/pub/bin/iris/glassReResult929/out");

  plisttxt = fopen("list.txt", "rb");

  for (fram = 0; fram < sizeof(iris) / sizeof(iris[0]) && fgets(buf, 256, plisttxt) > 0;) {
    int ok = 0;
    strtrim(buf, "\n\r");
    strcpy(iris[fram].name, buf);
    iris[fram].id = getclass(iris[fram].name);
    buf[8] = 'P';
    imread(buf, 1, 1, im);

    if (im) {
      ++ok;

      if (im->w == IRIS_H && im->h == IRIS_W) {
        TRANSPOSE(IRIS_W, IRIS_H, im->tt.data, IRIS_H, iris[fram].data, IRIS_W);
        //memcpy(iris[fram].data, im->data, IRIS_H*IRIS_W);
      }
      else if (im->w == IRIS_W && im->h == IRIS_H) {
        memcpy(iris[fram].data, im->tt.data, IRIS_H * IRIS_W);
      }
    }

    imfree(im);
    buf[8] = 'M';
    imread(buf, 1, 1, im);

    if (im) {
      ++ok;

      if (im->w == IRIS_H && im->h == IRIS_W) {
        TRANSPOSE(IRIS_W, IRIS_H, im->tt.data, IRIS_H, iris[fram].mask, IRIS_W);
      }
      else if (im->w == IRIS_W && im->h == IRIS_H) {
        memcpy(iris[fram].mask, im->tt.data, IRIS_H * IRIS_W);
      }
    }

    imfree(im);

    //printf("%s\n", buf);
    if (2 == ok) {
      fram++;
    }
  }

  picnum = fram;
  irdistlen = picnum * (picnum - 1) / 2;
  irdist = MALLOC(IrisDist_t, irdistlen);
  dist = MALLOC(double, irdistlen);
  FILL(irdistlen, dist, 0);

  if (1) {
    int m, bm;
    int mm[] = {5, 6, 7, 8, 9, 10, 11, 12};
    bianma_t bmf[] = {bianma4, bianma5, bianma6, bianma7, bianma8, bianma9};
    char* bmname[] = {"bianma4", "bianma5", "bianma6", "bianma7", "bianma8", "bianma9"};
    logecho(0);

    for (bm = 0; bm < 1; ++bm) {
      bianma_t bianma = bmf[bm];

      for (m = 0; m < sizeof(mm) / sizeof(mm[0]); ++m) {
        k = 0;
        g_dist = mm[m];

        for (i = 0; i < picnum; ++i) {
          bianma(iris + i);

          if (0) {
            img_t ima[1] = {0};
            IMINIT(ima, IRIS_H, IRIS_W, iris[i].Juanl, IRIS_W, 1, 1);
            imshow(ima);
            cvWaitKey(-1);
          }
        }

        printf("%s, dist=%d\r", bmname[bm], g_dist);

        for (i = 0; i < picnum && k < irdistlen; ++i) {
          printf("%d/%d\r", k, irdistlen);

          for (j = i + 1; j < picnum; ++j) {
            irdist[k].ida = iris[i].id;
            irdist[k].idb = iris[j].id;
            irdist[k].dist = HDR_iris(iris + i, iris + j);
            ++k;
          }
        }

        //logclear();
        {
          FILE* pf;
          char buf[256];
          //类内
          _snprintf(buf, 256, "%s_%s类内%d.txt", bmname[bm], HDR_name, g_dist);
          //logset(buf);
          printf("%s\n", buf);
          pf = fopen(buf, "wb");

          for (k = 0, i = 0; i < irdistlen; ++i) {
            if (irdist[i].ida == irdist[i].idb) {
              dist[k++] = irdist[i].dist;
              fprintf(pf, "%f\n", irdist[i].dist);
            }
          }

          fclose(pf);
          _snprintf(buf, 256, "%s_%s类间%d.txt", bmname[bm], HDR_name, g_dist);
          printf("%s\n", buf);
          pf = fopen(buf, "wb");

          for (k = 0, i = 0; i < irdistlen; ++i) {
            if (irdist[i].ida != irdist[i].idb) {
              dist[k++] = irdist[i].dist;
              fprintf(pf, "%f\n", irdist[i].dist);
            }
          }

          fclose(pf);
        }
      }
    }
  }
  else {
    int m, bm;
    int mm[] = {0, 1, 2, 3, 4, 5, 6};
    typedef int (*bianma_t)(Iris_t * ir);
    bianma_t bmf[] = {meansure1, meansure2};
    char* bmname[] = {"meansure1", "meansure2"};
    logecho(0);

    for (bm = 1; bm < 2; ++bm) {
      bianma_t bianma = bmf[bm];

      for (m = 0; m < sizeof(mm) / sizeof(mm[0]); ++m) {
        k = 0;
        g_dist2 = mm[m];

        for (i = 0; i < picnum; ++i) {
          bianma(iris + i);

          //FPRINT1D("code.txt", "%d\r\n", binum, iris[i].code, 1);
          if (0) {
            img_t ima[1] = {0};
            IMINIT(ima, 13, 85, iris[i].newMask, IRIS_W, 1, 1);
            imshow(im);
            cvWaitKey(-1);
          }
        }

        for (i = 0; i < picnum && k < irdistlen; ++i) {
          printf("%d/%d\r", k, irdistlen);

          for (j = i + 1; j < picnum; ++j) {
            irdist[k].ida = iris[i].id;
            irdist[k].idb = iris[j].id;
            irdist[k].dist = Hamm_iris(iris + i, iris + j);
            ++k;
          }
        }

        //logclear();
        {
          FILE* pf;
          char buf[256];
          //类内
          _snprintf(buf, 256, "%s_%s类内%d.txt", bmname[bm], Hamm_name, g_dist2);
          //logset(buf);
          printf("%s\n", buf);
          pf = fopen(buf, "wb");

          for (k = 0, i = 0; i < irdistlen; ++i) {
            if (irdist[i].ida == irdist[i].idb) {
              dist[k++] = irdist[i].dist;
              fprintf(pf, "%f\n", irdist[i].dist);
            }
          }

          fclose(pf);
          _snprintf(buf, 256, "%s_%s类间%d.txt", bmname[bm], Hamm_name, g_dist2);
          printf("%s\n", buf);
          pf = fopen(buf, "wb");

          for (k = 0, i = 0; i < irdistlen; ++i) {
            if (irdist[i].ida != irdist[i].idb) {
              dist[k++] = irdist[i].dist;
              fprintf(pf, "%f\n", irdist[i].dist);
            }
          }

          fclose(pf);
        }
      }
    }
  }

  FREE(dist);
  FREE(irdist);
  return 0;
}

