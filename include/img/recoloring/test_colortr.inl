
#include "cstd.h"
#include "img/imgio.inl"

#include "rbfcolor.inl"
#include "colortr.inl"

int palette2img(int n, const uchar* input_palette, int size, img_t* im) {
  int maxcol = 16;
  int row = (n+maxcol-1)/maxcol;
  int col = n%maxcol;
  imsetsize(im, row*=size, col*=size, 4, 1);
  int i, sz;
  uint32* data = (uint32*)im->tt.data;
  const uint32* palette = (const uint32*)(input_palette);
  sz = row*col;
  for (i=0; i<sz; ++i) {
    int j = (i/col/size)*maxcol + (i%col)/size;
    j%=n;
    data[i] = palette[j];
  }
  return 0;
}

#include "cjson.inl"

#include "img/imgioex.inl"

int json_loadimages(const char* fn, img_t* im, int maxn) {
  int ret=0;
  cJSON* js = cJSON_OpenFile(fn);
  if (js) {
    cJSON* js_imageset = cJSON_GetObjectItem(js, "imageset");
    if (js_imageset && js_imageset->type==cJSON_Array) {
      int i, n = cJSON_GetArraySize(js_imageset);
      for (i=0; i<n && ret<maxn; ++i) {
        cJSON* js_data = cJSON_GetArrayItem(js_imageset, i);
        if (js_data && js_data->valuestring) {
          const char* base64 = strstr(js_data->valuestring, "base64,");
          if (base64) {
            base64+=7;
            if (imread_base64(base64, 3, im+ret) ) {
              ++ret;
              //imshow(im); cvWaitKey(-1);
            }
          }
        }
      }
    }
    cJSON_Delete(js);
  }
  return ret;
}

int test_colortr() {
  img_t im[10] = {0};
  sys_chdir("D:/pub/bin/cap");
  sys_chdir("E:/code/c/Image/recolor");
	const int palette_size = 5;
  uchar input_palette[palette_size*4] = {0};
  uchar output_palette[palette_size*4] = {0};
  int i = json_loadimages("E:/code/cstd/include/img/recoloring/test/imageset.json", im, 4);
  img_t* palette = im+i++;
  img_t* im2 = im+i++;
  img_t* im1 = im+0;
  //imread("cap_22_35_38_097.jpg", 3, 1, im);
  //imread("river.png", 3, 1, im);
  imwrite("river.bmp", im);
  imsetsize(im2, im1->h, im1->w, 3, 1);

  gridacc_kmeans(im1->h, im1->w, im1->tt.data, im1->s, im1->c, 2, palette_size, input_palette);
  palette2img(palette_size, input_palette, 40, palette);
  memcpy(output_palette, input_palette, sizeof(output_palette));
  (output_palette)[1] += 30;
  drawRes(input_palette, output_palette, palette_size, 2, im1->h, im1->w, im1->tt.data, im1->s, im2->tt.data, im2->s, im->c, NULL, 0, 1);
  imshow(palette);
  imshow(im);
  imshow(im2);
  cvWaitKey(-1);
  imfrees(im, 10);
  return 0;
}
