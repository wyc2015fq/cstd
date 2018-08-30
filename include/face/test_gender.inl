
#include "cstd.h"
#include "str.h"
#include "cfile.h"
#include "img/imgio.inl"
#include "facetrack.inl"
#include "face_recog.inl"
#include "facedetect/faceplusplus_json.inl"
#include "draw/imdrawaa.inl"

int libsvm_save_txt(FILE* pf, const uchar* data, int len, double lable) {
  int i;
  fprintf(pf, "%+g ", lable);
  for(i=0; i<len; ++i) {
    fprintf(pf, "%d:%d ", i+1, data[i]);
  }
  fprintf(pf, "\n");
  return 0;
}

u32 swap_u32(u32 x) {
  union {
    u32 u;
    byte b[4];
  } t1, t2;
  t2.u = x;
  t1.b[3] = t2.b[0];
  t1.b[2] = t2.b[1];
  t1.b[1] = t2.b[2];
  t1.b[0] = t2.b[3];
  return t1.u;
}

int test_gender() {
  char* path = NULL;
  vstr_t sv[1] = {0};
  int i;
  img_t im[10] = {0};
  int cnt[2] = {0};
  uchar stdface[STDIMGH*STDIMGW];
  uchar feat[5000] = {0};
  char buf[256];
  img_t* gry = im+1;
  if (0) {
    path = "D:/pub/faceplusplus/out/std1";
    sys_chdir(path);
    int cnt[2] = {0};
    int h=100, w=100;
    vstr_load("list.txt", sv);
    int gender_images_header[4] = {swap_u32(0x0803), swap_u32(sv->n), swap_u32(h), swap_u32(w)};
    int gender_labels_header[2] = {swap_u32(0x0801), swap_u32(sv->n)};
    FILE* pf_train_images = fopen("gender-images.idx3-ubyte", "wb");
    FILE* pf_train_labels = fopen("gender-labels.idx1-ubyte", "wb");
    fwrite(gender_images_header, 1, sizeof(gender_images_header), pf_train_images);
    fwrite(gender_labels_header, 1, sizeof(gender_labels_header), pf_train_labels);
    for (i=0; i<sv->n; ++i) {
      if (imread(sv->v[i].s, 1, 1, im)) {
        //imresize(im, h, w, im);
        int gender = !strstr(sv->v[i].s, "\\M\\");
        FILE* pf_images = pf_train_images;
        FILE* pf_labels = pf_train_labels;
        uchar label = gender;
        //int cntsum = cnt[0]+cnt[0];
        fwrite(im->tt.data, 1, h*w, pf_images);
        fwrite(&label, 1, 1, pf_labels);
        printf("%s\n", GetFileNameExt(sv->v[i].s));
      }
    }
    fclose(pf_train_images);
    fclose(pf_train_labels);
  }
  if (1) {
    path = "D:/pub/faceplusplus/out/";
    sys_chdir(path);
    mkdirs("./std1/M");
    mkdirs("./std1/F");
    //mkdirs("./std/C");
    vstr_load("list.txt", sv);
    for (i=0; i<sv->n; ++i) {
      char* name = GetFileNameExt(sv->v[i].s);
      int gender = !strstr(sv->v[i].s, "\\ff\\");
      _snprintf(buf, countof(buf), "./std1/%s/%s", gender ? "M":"F", name);
      //_snprintf(buf, countof(buf), "./std/C/%s", name);
      if (!file_exist(buf)) {
        if (imread(sv->v[i].s, 1, 1, im)) {
          int xywh[12] = {0};
          //GetPathFileNameExtN(sv->v[i].s, buf, countof(buf));
          int ret = face_recog(im->h, im->w, im->tt.data, im->s, im->c, "gray", "df", 0, 1, xywh, stdface, NULL, NULL, 0, 0, NULL, NULL);
          if (ret) {
            if(1) {
              //imcolorcvt(im, gry, T_BGR, T_GRAY);
              int* xywh1 = xywh+4;
              int* xywh2 = xywh+8;
              IPOINT leye = iPOINT(xywh1[0]+xywh1[2]/2, xywh1[1]+xywh1[3]/2), reye = iPOINT(xywh2[0]+xywh2[2]/2, xywh2[1]+xywh2[3]/2);
              CPM_FaceStd( im->h, im->w, im->tt.data, im->s, im->c, STDIMGH, STDIMGW, stdface, STDIMGW, 1, leye, reye, 0.35, 0.5, 0.65, 0.5 );
            }
            imwrite4(buf,  STDIMGH, STDIMGW, stdface, STDIMGW, 1);
            //imdrawaa_circle(im, NULL, leye.x, leye.y, 2, _rgb(0,1,0), 0, 0);
            //imdrawaa_circle(im, NULL, reye.x, reye.y, 2, _rgb(0,1,0), 0, 0);
            //imshow(im);cvWaitKey(-1);
            printf("%s\n", name);
          }
        }
      }
    }
  }
  if (0) {
    str_t s[1] = {0};
    faceobj_t fa[1] = {0};
    path = "D:/pub/faceplusplus/gender";
    sys_chdir(path);
    mkdirs("./out1/M");
    mkdirs("./out1/F");
    vstr_load("list.txt", sv);
    for (i=0; i<sv->n; ++i) {
      if (imread(sv->v[i].s, 3, 1, im)) {
        imcolorcvt(im, gry, T_BGR, T_GRAY);
        GetPathFileNameCopy(sv->v[i].s, buf, countof(buf));
        strcat(buf, ".txt");
        str_load(buf, s);
        if (faceplusplus_json_parse(s->s, fa)) {
          char* name = GetFileNameExt(sv->v[i].s);
          printf("%s %s\n", name, fa->gender ? "ÄÐ":"Å®");
          imdrawaa_circle(im, NULL, fa->leye.x, fa->leye.y, 2, _rgb(0,1,0), 0, 0);
          imdrawaa_circle(im, NULL, fa->reye.x, fa->reye.y, 2, _rgb(0,1,0), 0, 0);
          {
            IPOINT leye = iPOINT(fa->leye.x, fa->leye.y), reye = iPOINT(fa->reye.x, fa->reye.y);
            CPM_FaceStd( gry->h, gry->w, gry->tt.data, gry->s, gry->c, STDIMGH, STDIMGW, stdface, STDIMGW, 1,
              leye, reye, 0.4, 0.4, 0.6, 0.4 );
            _snprintf(buf, countof(buf), "./out1/%s/%s", fa->gender ? "M":"F", name);
            imwrite4(buf,  STDIMGH, STDIMGW, stdface, STDIMGW, 1);
          }
          //imshow(im);cvWaitKey(-1);
        } else {
          printf("faceplusplus_json_parse fail\n");
        }
      }
    }
    str_free(s);
  }
  if (1) {
    int n=0;
    img_t* im2 = im+2;
    FILE* pf_train=0;
    FILE* pf_test=0;
    path = "F:/dataset/CAS-PEAL/CAS-PEAL-gender";
    path = "D:/pub/faceplusplus/gender/out1";
    sys_chdir(path);
    //pf_train = fopen("CAS-PEAL-gender-train.txt", "wb");
    //pf_test = fopen("CAS-PEAL-gender-test.txt", "wb");
    vstr_load("list.txt", sv);
    for (i=0; i<sv->n; ++i) {
      if (imread(sv->v[i].s, 1, 1, im)) {
        if (im->w!=100 || im->h!=100) {
          //imresize2(im, 100, 100, im);
          //imwrite(sv->v[i].s, im);
        }
        int lable = !!strstr(sv->v[i].s, "\\M\\");
        uchar* feat1;
        int len1;
        feat1 = im->tt.data;
        len1 = im->w*im->h;
        printf("%d %d\n", i, sv->n);
        if (0) {
          //int ret1 = 3 == CPM_EyesDetect( deyecas, eyecas, eyecas, alleye, alleye, eyewg, eyewg, gry, im->w, 1, &face, eye, 100 );
          len1 = CPM_GetFeature(im->h, im->w, feat1, im->w, 1, 0, feat);
          feat1 = feat;
        }
        if (0) {
          imresize(im, 64, 64, im);
          feat1 = im->tt.data;
          len1 = im->w*im->h;
        }
        if (1) {
          if (im2->tt.data==NULL) {
            imsetsize(im2, sv->n, len1+1, 1, 1);
          }
          uchar* data = im2->tt.data + n*im2->s;
          *data++ = lable;
          memcpy(data, feat1, len1);
        }
        ++n;
        if (pf_train && pf_test) {
          if (0) {
            libsvm_save_txt(pf_train, feat1, len1, lable ? 1:-1);
            libsvm_save_txt(pf_test, feat1, len1, lable ? 1:-1);
          } else {
            FILE* pf = cnt[lable]<4000 ? pf_train:pf_test;
            libsvm_save_txt(pf, feat1, len1, lable ? 1:-1);
          }
        }
        cnt[lable]++;
        //printf("%d\n", ret1);
        //imshow(im);      cvWaitKey(-1);
      }
    }
    im2->h = n;
    file_t f[1] = {0};
    FILE* pf = fopen("rongyi.df", "wb");
    if (pf) {
      fstream_init(f, pf);
      df_saveimage(f, 0, im2);
      fclose(pf);
    }
    if (pf_train) {
      fclose(pf_train);
    }
    if (pf_test) {
      fclose(pf_test);
    }
  }
  imfrees(im, 10);
  strv_free(sv);
  return 0;
}
