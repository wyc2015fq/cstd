
static int Blob_init_uniform_int(Blob* blob, int low_value, int high_deviation)
{
  uniform_distribution_init_int((int*)setData(blob), Data_total(blob->size), low_value, high_deviation);
  return 0;
}
static int Blob_init_uniform(Blob* blob, const float low_value, const float high_deviation)
{
  uniform_distribution_init(setData(blob), Data_total(blob->size), low_value, high_deviation);
  return 0;
}
#define Blob_init_gaussian Blob_init_normal
static int Blob_init_normal(Blob* blob, const float mean_value, const float standard_deviation)
{
  normal_distribution_init(setData(blob), Data_total(blob->size), mean_value, standard_deviation);
  return 0;
}
static int Blob_init_xavier(Blob* blob)
{
  fill_Xavier(setData(blob), blob->size, FillMethod_FAN_IN);
  return 0;
}
static int Blob_init_const(Blob* blob, float const_value)
{
  const_distribution_init(setData(blob), Data_total(blob->size), const_value);
  return 0;
}
static int onehot_f32(int n, float* arr, int id) {
  int i = 0;
  for (; i<n; ++i) {
    arr[i] = 0.f;
  }
  arr[id] = 1.f;
  return 0;
}

static int Blob_init_label_u8(Blob* blob, int n, int classes, const uchar* label_u8, bool onehot) {
  int i;
  if (onehot) {
    float* labelData = setDataSize(blob, dataSize(n, classes, 1, 1));
    for (i = 0; i < n; ++i) {
      //label data
      onehot_f32(classes, labelData, label_u8[i]);
      labelData += classes;
    }
  } else {
    int* labelData = (int*)setDataSize(blob, dataSize(n, 1, 1, 1));
    for (i = 0; i < n; ++i) {
      labelData[i] = label_u8[i];
    }
  }
  return 0;
}
static int image2float(Dtype* data, DataSize outSize, const int* perm, const uchar* im_data, int im_h, int im_w, int im_s, int im_c, float a, float b) {
  int i, j, f;
  int ch = outSize.c;
  int pixnum = Data_2DSize(outSize);
  if (im_w*im_c==im_s) {
    im_w*=im_h;
    im_s*=im_h;
    im_h = 1;
  }
  // [0,255]  -> [-1, 1]
  for (f = 0; f < outSize.n; f++) {
    Dtype* p0 = data + Data_4DIndex1(outSize, f);
    const uchar* pimage0 = im_data + (perm ? perm[f] : f) * im_h * im_s;
    if (3==ch) {
      Dtype* p1 = p0 + pixnum;
      Dtype* p2 = p0 + 2 * pixnum;
      if (3==im_c) {
        for (i = 0; i < im_h; i++) {
          const uchar* pimage1 = pimage0 + i * im_s;
          j = 0;
          for (; j < im_w-1; j+=2) {
            p0[0] = (pimage1[0]) * a + b;
            p1[0] = (pimage1[1]) * a + b;
            p2[0] = (pimage1[2]) * a + b;
            p0[1] = (pimage1[3]) * a + b;
            p1[1] = (pimage1[4]) * a + b;
            p2[1] = (pimage1[5]) * a + b;
            pimage1 += 6;
            p0 += 2;
            p1 += 2;
            p2 += 2;
          }
          for (; j < im_w; j++) {
            *p0++ = (pimage1[0]) * a + b;
            *p1++ = (pimage1[1]) * a + b;
            *p2++ = (pimage1[2]) * a + b;
            pimage1 += 3;
          }
        }
      } else if (4==im_c) {
        for (i = 0; i < im_h; i++) {
          const uchar* pimage1 = pimage0 + i * im_s;
          j = 0;
          for (; j < im_w-1; j+=2) {
            p0[0] = (pimage1[0]) * a + b;
            p1[0] = (pimage1[1]) * a + b;
            p2[0] = (pimage1[2]) * a + b;
            p0[1] = (pimage1[4]) * a + b;
            p1[1] = (pimage1[5]) * a + b;
            p2[1] = (pimage1[6]) * a + b;
            pimage1 += 8;
            p0 += 2;
            p1 += 2;
            p2 += 2;
          }
          for (; j < im_w; j++) {
            *p0++ = (pimage1[0]) * a + b;
            *p1++ = (pimage1[1]) * a + b;
            *p2++ = (pimage1[2]) * a + b;
            pimage1 += 4;
          }
        }
      } else if (1==im_c) {
        for (i = 0; i < im_h; i++) {
          const uchar* pimage1 = pimage0 + i * im_s;
          j = 0;
          for (; j < im_w-1; j+=2) {
            p2[0] = p1[0] = p0[0] = (pimage1[0]) * a + b;
            p2[1] = p1[1] = p0[1] = (pimage1[1]) * a + b;
            pimage1 += 2;
            p0 += 2;
            p1 += 2;
            p2 += 2;
          }
          for (; j < im_w; j++) {
            double t = (*pimage1++) * a + b;
            *p0++ = t;
            *p1++ = t;
            *p2++ = t;
          }
        }
      }
    } else if (1==ch) {
      if (3==im_c) {
        for (i = 0; i < im_h; i++) {
          const uchar* pimage1 = pimage0 + i * im_s;
          j = 0;
          for (; j < im_w-1; j+=2) {
            p0[0] = (((pimage1[0]+pimage1[2]+(pimage1[1]<<1))>>2)) * a + b;
            p0[1] = (((pimage1[3]+pimage1[5]+(pimage1[4]<<1))>>2)) * a + b;
            pimage1 += 6;
            p0 += 2;
          }
          for (; j < im_w; j++) {
            *p0++ = (((pimage1[0]+pimage1[2]+(pimage1[1]<<1))>>2)) * a + b;
            pimage1 += 3;
          }
        }
      } else if (4==im_c) {
        for (i = 0; i < im_h; i++) {
          const uchar* pimage1 = pimage0 + i * im_s;
          j = 0;
          for (; j < im_w-1; j+=2) {
            p0[0] = (((pimage1[0]+pimage1[2]+(pimage1[1]<<1))>>2)) * a + b;
            p0[1] = (((pimage1[4]+pimage1[6]+(pimage1[5]<<1))>>2)) * a + b;
            pimage1 += 8;
            p0 += 2;
          }
          for (; j < im_w; j++) {
            *p0++ = (((pimage1[0]+pimage1[2]+(pimage1[1]<<1))>>2)) * a + b;
            pimage1 += 4;
          }
        }
      } else if (1==im_c) {
        for (i = 0; i < im_h; i++) {
          const uchar* pimage1 = pimage0 + i * im_s;
          j = 0;
          for (; j < im_w-1; j+=2) {
            p0[0] = (pimage1[0]) * a + b;
            p0[1] = (pimage1[1]) * a + b;
            pimage1 += 2;
            p0 += 2;
          }
          for (; j < im_w; j++) {
            *p0++ = (*pimage1++) * a + b;
          }
        }
      }
    } else if (ch==im_c) {

    }
  }
  return 0;
}

// y = x * a + b
static pBox* image2Matrix(const img_t* im, pBox* outpBox, int ch, float a DEFAULT(1./127.5), float b DEFAULT(-1), int* perm DEFAULT(NULL), int batch DEFAULT(0))
{
  if ((im->data == NULL) || (CC_MAT_DEPTH(im) != CC_8U)) {
    printf("image's type is wrong!!Please set CC_8U. 0x%08p %d\n", im->data, CC_MAT_DEPTH(im));
    return 0;
  }
  pBoxsetsize(outpBox, im->rows, im->cols, ch, perm ? batch : im->f);
  if (NULL==outpBox->data) {
    ASSERT(0);
    return 0;
  }
  SETMODIFYP(outpBox, 1);
  image2float(outpBox->data, outpBox->size, perm, im->data, im->h, im->w, im->s, im->c, a, b);
  return outpBox;
}
