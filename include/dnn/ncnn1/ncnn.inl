
#ifndef _NCNN_INL_
#define _NCNN_INL_
//#include "mat.h"
//#include "cpu.inl"
#include "blob.inl"


#include "layer/absval.inl"
#include "layer/ArgMax.inl"
#include "layer/prelu.inl"
#include "layer/softmax.inl"
#include "layer/Dropout.inl"
#include "layer/conv_layer.inl"
#include "layer/relu_layer.inl"
#include "layer/pooling_layer.inl"
#include "layer/inner_product_layer.inl"
#include "layer/euclidean_loss_layer.inl"
#include "layer/softmax_layer.inl"
#include "layer/sigmoid_layer.inl"
#include "layer/tanh_layer.inl"

#if 0
#include "layer/Concat.inl"
#include "layer/split.inl"
LAYER_REGISTRY(Split)
LAYER_REGISTRY(Concat)
#include "layer/BatchNorm.inl"
#endif

#define LAYER_DEFDEF(DEF) \
LAYER_REGISTRY(AbsVal)  \
LAYER_REGISTRY(ArgMax)  \
LAYER_REGISTRY(Convolution)  \
LAYER_REGISTRY(InnerProduct)  \
LAYER_REGISTRY(Dropout)  \
LAYER_REGISTRY(Pooling)  \
LAYER_REGISTRY(ReLU)  \
LAYER_REGISTRY(PReLU)  \
LAYER_REGISTRY(Softmax)  \
LAYER_REGISTRY(Dropout)  \


static int layer_init(Layer* layer, const char* type)
{
  static LayerVTable layer_vtable[] = {
#define LAYER_REGISTRY(NAME)  {#NAME , NAME},
    LAYER_DEFDEF(LAYER_REGISTRY)
#undef LAYER_REGISTRY
  };

  enum { layer_registry_entry_count = countof(layer_vtable) };

  for (int i = 0; i < layer_registry_entry_count; i++) {
    const char* layer_name = layer_vtable[i].name;
    if (type[0]==layer_name[0] && strcmp(type, layer_vtable[i].name) == 0) {
      layer->type = layer_vtable[i].name;
      layer->run = layer_vtable[i].run;
      //layer->name = layer_name[i];
      return i;
    }
  }

  return -1;
}

#define clarg(x)  {(sizeof(x)), (uchar*)&(x)}

// y = x * a + b
static img_t* image2Matrix(const img_t* image, pBox* outpBox, int ch, float a DEFAULT(1./127.5), float b DEFAULT(-1), int* perm DEFAULT(NULL), int batch DEFAULT(0))
{
  int i, j, f;
  if ((image->data == NULL) || (CC_MAT_DEPTH(image) != CC_8U)) {
    printf("image's type is wrong!!Please set CC_8U. 0x%08x %d\n", image->data, CC_MAT_DEPTH(image));
    return 0;
  }
  pBoxsetsize(outpBox, image->rows, image->cols, ch, perm ? batch : image->f);
  if (NULL==outpBox->data) {
    ASSERT(0);
    return 0;
  }
  SETMODIFYP(outpBox, 1);
  
  // [0,255]  -> [-1, 1]
  for (f = 0; f < outpBox->f; f++) {
    Dtype* p0 = pBox_datapf(outpBox, f);
    const uchar* pimage0 = image->data + (perm ? perm[f] : f) * image->h * image->s;
    if (3==ch) {
      Dtype* p1 = p0 + outpBox->rows * outpBox->cols;
      Dtype* p2 = p0 + 2 * outpBox->rows * outpBox->cols;
      if (3==image->c) {
        for (i = 0; i < image->rows; i++) {
          const uchar* pimage1 = pimage0 + i * image->step;
          j = 0;
          for (; j < image->cols-1; j+=2) {
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
          for (; j < image->cols; j++) {
            *p0++ = (pimage1[0]) * a + b;
            *p1++ = (pimage1[1]) * a + b;
            *p2++ = (pimage1[2]) * a + b;
            pimage1 += 3;
          }
        }
      } else if (4==image->c) {
        for (i = 0; i < image->rows; i++) {
          const uchar* pimage1 = pimage0 + i * image->step;
          j = 0;
          for (; j < image->cols-1; j+=2) {
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
          for (; j < image->cols; j++) {
            *p0++ = (pimage1[0]) * a + b;
            *p1++ = (pimage1[1]) * a + b;
            *p2++ = (pimage1[2]) * a + b;
            pimage1 += 4;
          }
        }
      } else if (1==image->c) {
        for (i = 0; i < image->rows; i++) {
          const uchar* pimage1 = pimage0 + i * image->step;
          j = 0;
          for (; j < image->cols-1; j+=2) {
            p2[0] = p1[0] = p0[0] = (pimage1[0]) * a + b;
            p2[1] = p1[1] = p0[1] = (pimage1[1]) * a + b;
            pimage1 += 2;
            p0 += 2;
            p1 += 2;
            p2 += 2;
          }
          for (; j < image->cols; j++) {
            double t = (*pimage1++) * a + b;
            *p0++ = t;
            *p1++ = t;
            *p2++ = t;
          }
        }
      }
    } else if (1==ch) {
      if (3==image->c) {
        for (i = 0; i < image->rows; i++) {
          const uchar* pimage1 = pimage0 + i * image->step;
          j = 0;
          for (; j < image->cols-1; j+=2) {
            p0[0] = (((pimage1[0]+pimage1[2]+(pimage1[1]<<1))>>2)) * a + b;
            p0[1] = (((pimage1[3]+pimage1[5]+(pimage1[4]<<1))>>2)) * a + b;
            pimage1 += 6;
            p0 += 2;
          }
          for (; j < image->cols; j++) {
            *p0++ = (((pimage1[0]+pimage1[2]+(pimage1[1]<<1))>>2)) * a + b;
            pimage1 += 3;
          }
        }
      } else if (4==image->c) {
        for (i = 0; i < image->rows; i++) {
          const uchar* pimage1 = pimage0 + i * image->step;
          j = 0;
          for (; j < image->cols-1; j+=2) {
            p0[0] = (((pimage1[0]+pimage1[2]+(pimage1[1]<<1))>>2)) * a + b;
            p0[1] = (((pimage1[4]+pimage1[6]+(pimage1[5]<<1))>>2)) * a + b;
            pimage1 += 8;
            p0 += 2;
          }
          for (; j < image->cols; j++) {
            *p0++ = (((pimage1[0]+pimage1[2]+(pimage1[1]<<1))>>2)) * a + b;
            pimage1 += 4;
          }
        }
      } else if (1==image->c) {
        for (i = 0; i < image->rows; i++) {
          const uchar* pimage1 = pimage0 + i * image->step;
          j = 0;
          for (; j < image->cols-1; j+=2) {
            p0[0] = (pimage1[0]) * a + b;
            p0[1] = (pimage1[1]) * a + b;
            pimage1 += 2;
            p0 += 2;
          }
          for (; j < image->cols; j++) {
            *p0++ = (*pimage1++) * a + b;
          }
        }
      }
    } else if (ch==image->c) {

    }
  }
  return outpBox;
}

static int featurePad(const pBox* pbox, pBox* outpBox, const int pad)
{
  Dtype* p, *pIn;
  int i, n, inch = pBox_channels(pbox);
  if (pad <= 0) {
    printf("the data needn't to pad, please check you network!\n");
    return 0;
  }

  pBoxsetsize(outpBox, pbox->h + 2 * pad, pbox->w + 2 * pad, inch, 1);
  if (NULL==outpBox->data) {
    return 0;
  }

  p = pBox_datap(outpBox);
  pIn = pBox_datap(pbox);
  n = pBox_channels(outpBox) * outpBox->h;

  for (i = 0; i < n; i++) {

    if ((i % outpBox->h) < pad || (i % outpBox->h > (outpBox->h - pad - 1))) {
      p += outpBox->w;
      continue;
    }

    p += pad;
    memcpy(p, pIn, pbox->w * sizeof(Dtype));
    p += pbox->w + pad;
    pIn += pbox->w;
  }
  return 1;
}
// im2col
static int _feature2Matrix(const pBox* pbox, pBox* outpBox, const Weight* weight)
{
  int kernel_h = weight->kernel_h;
  int kernel_w = weight->kernel_w;
  int stride_h = weight->stride_h;
  int stride_w = weight->stride_w;
  int w_out = (pbox->w - kernel_w) / stride_w + 1;//
  int h_out = (pbox->h - kernel_h) / stride_h + 1;
  int inch = pBox_channels(pbox);
  int i, j, f, row, col, krow, kcol, c;
  int pbox_count = pbox->h * pbox->w;
  pBoxsetsize(outpBox, w_out * h_out, inch * kernel_h * kernel_w, 1, pbox->f);
  if (NULL==outpBox->data) {
    return 0;
  }

#ifdef HAVE_OPENCL1

  //for (int i=0; i<pbox->h*pbox->w*inch; ++i) {    pIn[i] = i;  }  SETMODIFYP(pbox, 1);
  if (used_gpu) {
    tic;
    static const char* feature2Matrix_progstr =
        "__kernel void _feature2Matrix(__global float* pIn, "
        "        __global float* pOut,  "
        "        int n_in, int w_in, int w_out, int o_step, int kernel_size)  "
        "{"
        "    int i = get_global_id(0);"
        "    int j = get_global_id(1);"
        "    int c = j/(kernel_size*kernel_size);"
        "    int k = j%(kernel_size*kernel_size);"
        "    int y = i/w_out + k/kernel_size;"
        "    int x = i%w_out + k%kernel_size;"
        "    pOut[i*o_step+j] = pIn[c*n_in+y*w_in+x];"
        "}"
        ;
    int n_in = pbox->h * pbox->w;
    int w_in = pbox->w;
    umem_setu(pbox->data);
    outpBox->data->modifyu = 1;
    size_t _globalsize[] = {outpBox->h, outpBox->w};
    buf_t args[] = {
      clarg(pbox->data->u),
      clarg(outpBox->data->u),
      clarg(n_in),
      clarg(w_in),
      clarg(w_out),
      clarg(outpBox->w),
      clarg(kernel_size),
    };
    static ProgramSource  feature2Matrix_oclsrc(feature2Matrix_progstr);
    feature2Matrix_oclsrc.run("_feature2Matrix", countof(_globalsize), _globalsize, NULL, countof(args), args);
    toc;
    return ;
  }

#endif

  umem_getu(pbox->data);
  SETMODIFYP(outpBox, 1);

  if (0) {
    int n_in;
    tic;
n_in 	  = pbox->h * pbox->w;

    for (f = 0; f < pbox->f; ++f) {
      const Dtype* pIn = pBox_datapf(pbox, f);
      Dtype* pOut = pBox_datapf(outpBox, f);

      for (i = 0; i < outpBox->h; ++i) {
        for (j = 0; j < outpBox->w; ++j) {
          int c = j / (kernel_h * kernel_w);
          int k = j % (kernel_h * kernel_w);
          int y = i / w_out + k / kernel_w;
          int x = i % w_out + k % kernel_w;
          pOut[i * outpBox->w + j] = pIn[c * n_in + y * pbox->w + x];
        }
      }
    }

    toc;
    //double aa = umem_checku(outpBox->data);
    return 1;
  }

  if (0) {
    tic;

    for (f = 0; f < pbox->f; ++f) {
      const Dtype* pIn = pBox_datapf(pbox, f);
      Dtype* pOut = pBox_datapf(outpBox, f);

      for (row = 0; row < h_out; ++row) {
        for (col = 0; col < w_out; ++col) {
          for (c = 0; c < inch; ++c) {
            for (krow = 0; krow < kernel_h; ++krow) {
              for (kcol = 0; kcol < kernel_w; ++kcol) {
                int i = (row * w_out + col) * outpBox->w + c * kernel_h * kernel_w + krow * kernel_w + kcol;
                int j = c * (pbox->h * pbox->w) + (row + krow) * pbox->w + col + kcol;
                pOut[i] = pIn[j];
              }
            }
          }
        }
      }
    }

    toc;
    return 1;
  }

  tic;
  {
  const Dtype* pIn0;
  const Dtype* pIn1;
  const Dtype* ptemp;
#if 1

  if (1 == kernel_h && 1 == kernel_w) {
    int countbuf[256];

    if (1 == inch) {
      for (f = 0; f < pbox->f; ++f) {
        const Dtype* pIn = pBox_datapf(pbox, f);
        Dtype* p = pBox_datapf(outpBox, f);

        for (row = 0; row < h_out; row ++) {
          pIn0 = pIn + row * stride_h * pbox->w;

          for (col = 0; col < w_out; col++) {
            *p++ = pIn0[col * stride_w];
          }
        }
      }
    }
    else if (2 == inch) {
      for (f = 0; f < pbox->f; ++f) {
        const Dtype* pIn = pBox_datapf(pbox, f);
        Dtype* p = pBox_datapf(outpBox, f);

        for (row = 0; row < h_out; row ++) {
          for (col = 0; col < w_out; col++) {
            pIn1 = pIn + row * stride_h * pbox->w + col * stride_w;
            *p++ = pIn1[0];
            *p++ = pIn1[pbox_count];
          }
        }
      }
    }
    else if (inch < countof(countbuf)) {
      for (c = 0; c < inch; c++) {
        countbuf[c] = c * pbox_count;
      }

      for (f = 0; f < pbox->f; ++f) {
        const Dtype* pIn = pBox_datapf(pbox, f);
        Dtype* p = pBox_datapf(outpBox, f);

        for (row = 0; row < h_out; row ++) {
          for (col = 0; col < w_out; col++) {
            pIn1 = pIn + row * stride_h * pbox->w + col * stride_w;
            c = 0;

            for (; c < inch - 3; c += 4) {
              p[0] = pIn1[countbuf[c + 0]];
              p[1] = pIn1[countbuf[c + 1]];
              p[2] = pIn1[countbuf[c + 2]];
              p[3] = pIn1[countbuf[c + 3]];
              p += 4;
            }

            for (; c < inch; c++) {
              *p++ = pIn1[countbuf[c]];
            }
          }
        }
      }
    }
    else {
      for (f = 0; f < pbox->f; ++f) {
        const Dtype* pIn = pBox_datapf(pbox, f);
        Dtype* p = pBox_datapf(outpBox, f);

        for (row = 0; row < h_out; row ++) {
          for (col = 0; col < w_out; col++) {
            pIn1 = pIn + row * stride_h * pbox->w + col * stride_w;
            c = 0;

            for (; c < inch - 3; c += 4) {
              p[0] = pIn1[(c + 0) * pbox_count];
              p[1] = pIn1[(c + 1) * pbox_count];
              p[2] = pIn1[(c + 2) * pbox_count];
              p[3] = pIn1[(c + 3) * pbox_count];
              p += 4;
            }

            for (; c < inch; c++) {
              *p++ = pIn1[c * pbox_count];
            }
          }
        }
      }
    }
  }
  else if (2 == kernel_h && 2 == kernel_w) {
    for (f = 0; f < pbox->f; ++f) {
      const Dtype* pIn = pBox_datapf(pbox, f);
      Dtype* p = pBox_datapf(outpBox, f);

      for (row = 0; row < h_out; row ++) {
        for (col = 0; col < w_out; col++) {
          pIn1 = pIn + row * stride_h * pbox->w + col * stride_w;

          for (c = 0; c < inch; c++) {
            ptemp = pIn1 + c * pbox_count;
            p[0] = ptemp[0];
            p[1] = ptemp[1];
            p[2] = ptemp[pbox->w];
            p[3] = ptemp[pbox->w + 1];
            p += 4;
          }
        }
      }
    }
  }
  else if (3 == kernel_h && 3 == kernel_w) {
    for (f = 0; f < pbox->f; ++f) {
      const Dtype* pIn = pBox_datapf(pbox, f);
      Dtype* p = pBox_datapf(outpBox, f);

      for (row = 0; row < h_out; row ++) {
        for (col = 0; col < w_out; col++) {
          pIn1 = pIn + row * stride_h * pbox->w + col * stride_w;

          for (c = 0; c < inch; c++) {
            const Dtype* ptemp = pIn1 + c * pbox_count;
            const Dtype* ptemp1 = ptemp + pbox->w;
            const Dtype* ptemp2 = ptemp + pbox->w + pbox->w;
            p[0] = ptemp[0];
            p[1] = ptemp[1];
            p[2] = ptemp[2];
            p[3] = ptemp1[0];
            p[4] = ptemp1[1];
            p[5] = ptemp1[2];
            p[6] = ptemp2[0];
            p[7] = ptemp2[1];
            p[8] = ptemp2[2];
            p += 9;
          }
        }
      }
    }
  }
  else
#endif
  {
    int kernelRow;
    for (f = 0; f < pbox->f; ++f) {
      const Dtype* pIn = pBox_datapf(pbox, f);
      Dtype* p = pBox_datapf(outpBox, f);

      for (row = 0; row < h_out; row ++) {
        for (col = 0; col < w_out; col++) {
          pIn1 = pIn + row * stride_h * pbox->w + col * stride_w;

          for (c = 0; c < inch; c++) {
            ptemp = pIn1 + c * pbox_count;

            for (kernelRow = 0; kernelRow < kernel_h; kernelRow++) {
              memcpy(p, ptemp, kernel_w * sizeof(Dtype));
              p += kernel_w;
              ptemp += pbox->w;
            }
          }
        }
      }
    }
  }

  }
  toc;
  return 1;
}
static void matMul(const float* A, int al, int ai, const float* B, int bl, int bi, float* C, int cl, int ci, int M, int N, int K)
{
  int i, j, k;

  for (i = 0; i < M; ++i) {
    for (j = 0; j < N; ++j) {
      const float* a = A + i * al;
      const float* b = B + j * bl;
      float s = 0;

      for (k = 0; k < K; ++k) {
        s += a[k * ai] * b[k * bi];
      }

      C[i * cl + j * ci] = s;
    }
  }
}

#if 0
//#include "blas/matmul/matmulx.txt"
void copy_block(const float* A, int al, int ai, float* B, int bl, int bi, int M, int N)
{
  int i, j;

  for (i = 0; i < M; ++i) {
    const float* a = A + i * al;
    float* b = B + i * bl;
    j = 0;

    for (; j < N - 3; j += 4) {
      b[(j)*bi] = a[(j) * ai];
      b[(j + 1)*bi] = a[(j + 1) * ai];
      b[(j + 2)*bi] = a[(j + 2) * ai];
      b[(j + 3)*bi] = a[(j + 3) * ai];
    }

    for (; j < N; ++j) {
      b[j * bi] = a[j * ai];
    }
  }
}

void matMul_block(const float* A, int al, int ai, const float* B, int bl, int bi, float* C, int cl, int ci, int M, int N, int K)
{
  int i, j, k;
  enum {blocksize = 8};

  for (i = 0; i < M; i += blocksize) {
    int mc = MIN(M - i, blocksize);

    for (j = 0; j < N; j += blocksize) {
      int nc = MIN(N - j, blocksize);
      float C_[blocksize * blocksize] = {0};

      for (k = 0; k < K; k += blocksize) {
        int kc = MIN(K - k, blocksize);
        float A_[blocksize * blocksize] = {0};
        float B_[blocksize * blocksize] = {0};
        copy_block(A + i * al + k * ai, al, ai, A_, blocksize, 1, mc, kc);
        copy_block(B + j * bl + k * ai, bl, bi, B_, blocksize, 1, nc, kc);
        matmul_8x8(A_, B_, C_);
        //matmul_16x16(A_, B_, C_);
      }

      copy_block(C_, blocksize, 1, C + i * cl + j * ci, cl, ci, mc, nc);
    }
  }
}
#endif

//#include "blas/gemm/gemm.inl"
static int convolution(const Weight* weight, const pBox* pbox, pBox* outpBox)
{
  int outh = (pbox->h - weight->kernel_h) / weight->stride_h + 1;
  int outw = (pbox->w - weight->kernel_w) / weight->stride_w + 1;
  int f, Matrix_count = outpBox->h * outpBox->w;
  pBoxsetsize(outpBox, outh, outw, weight->selfChannel, pbox->f);
  if (NULL==outpBox->data) {
    return 0;
  }

  if (1) {
    for (f = 0; f < pbox->f; ++f) {
      img_t pbox2 = pBox_frame(pbox, f);
      img_t outpBox2 = pBox_frame(outpBox, f);
      tic;
      convolution_forward(&pbox2, &outpBox2, weight, NULL, 1);
      toc;
    }
    return 1;
  }

  for (f = 0; f < pbox->f; ++f) {
    img_t pbox2 = pBox_frame(pbox, f);
    img_t tmp_matrix[1] = {0};
    const float* tmp_matrixp = pBox_datap(tmp_matrix);
    float* outpBoxp = pBox_datapf(outpBox, f);
    _feature2Matrix(&pbox2, tmp_matrix, weight);

    if (0) {
      tic;
      //matMul_block(weight->data->p, tmp_matrix->w, 1, tmp_matrixp, tmp_matrix->w, 1, outpBoxp, tmp_matrix->h, 1, weight->selfChannel, tmp_matrix->h, tmp_matrix->w);
      toc;
    }

    if (0) {
      tic;
      matMul(weight->data->p, tmp_matrix->w, 1, tmp_matrixp, tmp_matrix->w, 1, outpBoxp, tmp_matrix->h, 1, weight->selfChannel, tmp_matrix->h, tmp_matrix->w);
      toc;
    }

#ifdef HAVE_OPENCL1

    if (used_gpu) {
      tic;
      umem_setu(weight->data);
      umem_setu(tmp_matrix->data);
      outpBox->data->modifyu = 1;
      int astep = matrix->w, ai = 1, bstep = matrix->w, bi = 1, cstep = matrix->h, ci = 1, M = weight->selfChannel, N = matrix->h, K = matrix->w;
      size_t _globalsize[] = {M, N};
      buf_t args[] = {
        clarg(weight->data->u),
        clarg(matrix->data->u),
        clarg(outpBox->data->u),
        clarg(astep),
        clarg(ai),
        clarg(bstep),
        clarg(bi),
        clarg(cstep),
        clarg(ci),
        clarg(M),
        clarg(N),
        clarg(K),
      };
      static const char* matMul_progstr =
          "__kernel void matMul(__global float* A,"
          "        __global float* B,  "
          "        __global float* C,  "
          "        int astep, int ai, "
          "        int bstep, int bi, "
          "        int cstep, int ci, "
          "        int M, int N, int K)  "
          "{\n"
          "    int i = get_global_id(0);\n"
          "    int j = get_global_id(1), k;\n"
          "    float s = 0;\n"
          "    int aoff = i*astep;\n"
          "    int boff = j*bstep;\n"
          "    for (k=0; k<K; ++k) {\n"
          "      s += A[aoff+k*ai] * B[boff+k*bi];\n"
          "    }\n"
          "    C[i*cstep+j*ci] = s;\n"
          "}"
          ;

      static ProgramSource matMul_oclsrc(matMul_progstr);
      matMul_oclsrc.run("matMul", countof(_globalsize), _globalsize, NULL, countof(args), args);
      //umem_getu(outpBox->data);
      toc;
    }

#endif

#ifdef USE_OPENBLAS
    tic;

    if (weight->pad == 0) {
      //C←αAB + βC
      //                1              2            3              4     C's size    5              k     alpha     A*              A'col             B*           B'col    beta      C*           C'col
      cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasTrans, weight->selfChannel, tmp_matrix->h, tmp_matrix->w, 1, weight->data->p, tmp_matrix->w, tmp_matrixp, tmp_matrix->w, 0, outpBoxp, tmp_matrix->h);
    }
    else {
      ASSERT(0);
      pBox padpbox[1] = {0};
      featurePad(pbox, padpbox, weight->pad);
      //C←αAB + βC
      //                1              2            3              4     C's size    5              k     alpha     A*              A'col             B*           B'col    beta      C*           C'col
      cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasTrans, weight->selfChannel, tmp_matrix->h, tmp_matrix->w, 1, weight->data->p, tmp_matrix->w, tmp_matrixp, tmp_matrix->w, 0, outpBoxp, tmp_matrix->h);
      freepBox(padpbox);
    }

    toc;
#endif
    imfree(tmp_matrix);
  }
  return 1;
}

static int maxPooling(const pBox* pbox, pBox* outpBox, int kernel, int stride)
{
  int kernel_h = kernel;
  int kernel_w = kernel;
  int stride_h = stride;
  int stride_w = stride;
  if (1) {
    int ret;
    tic;
    ret = pooling_forward(pbox, outpBox, Method_MAX, kernel_h, kernel_w, stride_h, stride_w, 0, 0);
    toc;
    return ret;
  }
  {
    int inch = pBox_channels(pbox);
    int outh = ceil((float)(pbox->h - kernel_h) / stride_h + 1);
    int outw = ceil((float)(pbox->w - kernel_w) / stride_w + 1);
    int i, c, row, col, kernelRow;
    int inchf = inch * pbox->f;
    pBoxsetsize(outpBox, outh, outw, inch, pbox->f);
    if (NULL==outpBox->data) {
      return 0;
    }
    
#ifdef HAVE_OPENCL1
    
    if (used_gpu) {
      tic;
      umem_setu(pbox->data);
      outpBox->data->modifyu = 1;
      int Matrix_count = outpBox->h * outpBox->w;
      int pbox_count = pbox->h * pbox->w;
      int ih = pbox->h;
      int iw = pbox->w;
      int ow = outpBox->w;
      size_t _globalsize[] = {inch, outpBox->h, outpBox->w};
      buf_t args[] = {
        clarg(pbox->data->u),
          clarg(outpBox->data->u),
          clarg(pbox_count),
          clarg(Matrix_count),
          clarg(ih),
          clarg(iw),
          clarg(ow),
          clarg(stride),
          clarg(kernel_size),
      };
      static const char* maxPooling_progstr =
        "__kernel void maxPooling(__global float* pIn, "
        "        __global float* pOut,  "
        "        int pbox_count, int Matrix_count, int ih, int iw, int ow, int stride, int kernel_size)  "
        "{"
        "    int c = get_global_id(0);"
        "    int row = get_global_id(1);"
        "    int col = get_global_id(2);"
        "    int off = c*pbox_count + row*stride*iw + col*stride;"
        "    float maxNum = pIn[off];"
        "    int diffh = row*stride-ih+1;"
        "    int diffw = col*stride-iw+1;"
        "    for (int kernelRow = 0; kernelRow < kernel_size; kernelRow++){"
        "      if((kernelRow+diffh)>0)break;"
        "      for(int i=0;i<kernel_size;i++){"
        "        if((i+diffw)>0)break;"
        "        if(maxNum<pIn[off+i+kernelRow*iw])maxNum=pIn[off+i+kernelRow*iw];"
        "      }"
        "    }"
        "    pOut[c*Matrix_count+row*ow+col] = maxNum;"
        "}"
        ;
      static ProgramSource  maxPooling_oclsrc(maxPooling_progstr);
      maxPooling_oclsrc.run("maxPooling", countof(_globalsize), _globalsize, NULL, countof(args), args);
      //umem_getu(outpBox->data);
      toc;
      return ;
    }
    
#endif
    
    if (0) {
      int Matrix_count = outpBox->h * outpBox->w;
      int pbox_count = pbox->h * pbox->w;
      int ih = pbox->h;
      int iw = pbox->w;
      int ow = outpBox->w;
      const Dtype* pIn = pBox_datap(pbox);
      Dtype* pOut = pBox_datap(outpBox);
      const Dtype* pIn0;
      umem_getu(pbox->data);
      SETMODIFYP(outpBox, 1);
      
      tic;
      {
#pragma omp parallel for
        
        for (c = 0; c < inchf; c++) {
          pIn0 = pIn + c * pbox->h * pbox->w;
          
          for (row = 0; row < outpBox->h; row ++) {
            for (col = 0; col < outpBox->w; col++) {
              int off = c * pbox_count + row * stride_h * iw + col * stride_w;
              Dtype maxNum = pIn0[off];
              int diffh = row * stride_h - ih + 1;
              int diffw = col * stride_w - iw + 1;
              
              for (kernelRow = 0; kernelRow < kernel_h; kernelRow++) {
                if ((kernelRow + diffh) > 0) {
                  break;
                }
                
                for (i = 0; i < kernel_w; i++) {
                  if ((i + diffw) > 0) {
                    break;
                  }
                  
                  if (maxNum < pIn0[off + i + kernelRow * iw]) {
                    maxNum = pIn0[off + i + kernelRow * iw];
                  }
                }
              }
              
              *pOut++ = maxNum;
            }
          }
        }
      }
      toc;
      return 1;
    }
    
    tic;
    umem_getu(pbox->data);
    SETMODIFYP(outpBox, 1);
    {
      
      const Dtype* pIn = pBox_datap(pbox);
      Dtype* p = pBox_datap(outpBox);
      const Dtype* pIn0;
      const Dtype* pIn1;
      const Dtype* ptemp;
      const Dtype* ptemp1;
      Dtype maxNum = 0;
      
      if ((pbox->w - kernel_w) % stride_w == 0 && (pbox->h - kernel_h) % stride_h == 0) {
        if (2 == kernel_h && 2 == kernel_w) {
#pragma omp parallel for
          
          for (c = 0; c < inchf; c++) {
            pIn0 = pIn + c * pbox->h * pbox->w;
            
            for (row = 0; row < outpBox->h; row ++) {
              pIn1 = pIn0 + row * stride_h * pbox->w;
              
              for (col = 0; col < outpBox->w; col++) {
                ptemp = pIn1 + col * stride_w;
                *p++ = MAX4(ptemp[0], ptemp[1], ptemp[pbox->w], ptemp[pbox->w + 1]);
              }
            }
          }
        }
        else if (3 == kernel_h && 3 == kernel_w) {
          int w = pbox->w;
          int w2 = pbox->w * 2;
          
#pragma omp parallel for
          
          for (c = 0; c < inchf; c++) {
            pIn0 = pIn + c * pbox->h * pbox->w;
            
            for (row = 0; row < outpBox->h; row ++) {
              pIn1 = pIn0 + row * stride_h * pbox->w;
              
              for (col = 0; col < outpBox->w; col++) {
                const Dtype* ptemp = pIn1 + col * stride_w;
                float t0 = MAX3(ptemp[0], ptemp[1], ptemp[2]);
                float t1 = MAX3(ptemp[w], ptemp[w + 1], ptemp[w + 2]);
                float t2 = MAX3(ptemp[w2], ptemp[w2 + 1], ptemp[w2 + 2]);
                *p++ = MAX3(t0, t1, t2);
              }
            }
          }
        }
        else {
#pragma omp parallel for
          
          for (c = 0; c < inchf; c++) {
            pIn0 = pIn + c * pbox->h * pbox->w;
            
            for (row = 0; row < outpBox->h; row ++) {
              pIn1 = pIn0 + row * stride_h * pbox->w;
              
              for (col = 0; col < outpBox->w; col++) {
                ptemp = pIn1 + col * stride_w;
                maxNum = *ptemp;
                
                for (kernelRow = 0; kernelRow < kernel_h; kernelRow++) {
                  for (i = 0; i < kernel_w; i++) {
                    if (maxNum < * (ptemp + i + kernelRow * pbox->w)) {
                      maxNum = *(ptemp + i + kernelRow * pbox->w);
                    }
                  }
                }
                
                *p++ = maxNum;
              }
            }
          }
        }
      }
      else {
        int w = pbox->w;
        int diffh = 0, diffw = 0;
        
#pragma omp parallel for
        
        for (c = 0; c < inchf; c++) {
          float* p0 = p;
          pIn0 = pIn + c * pbox->h * pbox->w;
          for (row = 0; row < outpBox->h; row ++) {
            pIn1 = pIn0 + row * stride_h * pbox->w;
            diffh = MIN(pbox->h - row * stride_h, kernel_h);
            
            for (col = 0; col < outpBox->w; col++) {
              ptemp = pIn1 + col * stride_w;
              maxNum = *ptemp;
              diffw = MIN(pbox->w - col * stride_w, kernel_w);
              
              for (kernelRow = 0; kernelRow < diffh; kernelRow++) {
                ptemp1 = ptemp + kernelRow * pbox->w;
                
                for (i = 0; i < diffw; i++) {
                  if (maxNum < ptemp1[i]) {
                    maxNum = ptemp1[i];
                  }
                }
              }
              
              if (p == p0 && 0 == maxNum) {
                int asdf = 0;
              }
              
              *p++ = maxNum;
            }
          }
          
          if (0 == *p0) {
            int asdf = 0;
          }
        }
        
        ASSERT(p == (float*)(outpBox->tt.data + outpBox->h * outpBox->w * outpBox->c * outpBox->f));
      }
      
      toc;
    }
  }
}

static int addbias(pBox* pbox, const umem_t* bias)
{
  int inch = pBox_channels(pbox);

#ifdef HAVE_OPENCL1

  if (used_gpu) {
    tic;
    static const char* addbias_progstr =
        "__kernel void addbias(__global float* op, "
        "        __global float* pb,  "
        "        int dis)  "
        "{"
        "    int i = get_global_id(0);"
        "    op[i] = op[i] + pb[i/dis];"
        "}"
        ;
    umem_setu(pbox->data);
    pbox->data->modifyu = 1;
    umem_setu(bias);
    long dis = pbox->w * pbox->h;
    int n = dis * pBox_channels(pbox);
    size_t _globalsize[] = {n};
    buf_t args[] = {
      clarg(pbox->data->u),
      clarg(bias->u),
      clarg(dis),
    };
    static ProgramSource  addbias_oclsrc(addbias_progstr);
    addbias_oclsrc.run("addbias", countof(_globalsize), _globalsize, NULL, countof(args), args);
    toc;
    //umem_getu(pbox->data);
    return ;
  }

#endif

  if (0) {
    Dtype* op;
    const Dtype* pb;
    int i, n, dis;
    tic;
    umem_getu(pbox->data);
    umem_getu(bias);
    SETMODIFYP(pbox, 1);
    op = pBox_datap(pbox);
    pb = bias->p;
    dis = pbox->w * pbox->h;
    n = dis * inch;

    for (i = 0; i < n; i++) {
      op[i] = op[i] + pb[i / dis];
    }

    toc;
    return 1;
  }

  {
    int i, f, c, dis = pbox->w * pbox->h;
    tic;
    umem_getu(pbox->data);
    umem_getu(bias);
    SETMODIFYP(pbox, 1);
    
    
    for (f = 0; f < pbox->f; ++f) {
      Dtype* pIn = pBox_datapf(pbox, f);
      
      for (c = 0; c < inch; ++c) {
        Dtype* op = pIn + c * dis;
        const Dtype b = bias->p[c];
        i = 0;
        
        for (; i < dis - 3; i += 4) {
          op[i] += b;
          op[i + 1] += b;
          op[i + 2] += b;
          op[i + 3] += b;
        }
        
        for (; i < dis; ++i) {
          op[i] += b;
        }
      }
    }
    toc;
  }
  return 1;
}

static int relu(pBox* pbox, umem_t* bias)
{
  int inch = pBox_channels(pbox);
  Dtype* pIn = pBox_datap(pbox);
  int i, c, dis = pbox->w * pbox->h;

  for (c = 0; c < inch; c++) {
    Dtype* op = pIn + c * dis;
    Dtype b = bias->p[c];

    for (i = 0; i < dis; i++) {
      op[i] += b;

      if (op[i] < 0) {
        op[i] = 0;
      }
    }
  }
}
static int prelu(pBox* pbox, const umem_t* bias, const umem_t* prelu_gmma)
{
  int inch = pBox_channels(pbox);
#ifdef HAVE_OPENCL1

  if (used_gpu) {
    tic;
    static const char* prelu_progstr =
        "__kernel void prelu(__global float* op, "
        "        __global float* pb,  "
        "        __global float* pg, int dis)  "
        "{"
        "    int i = get_global_id(0);"
        "    int j = i/dis;"
        "    op[i] = op[i] + pb[j];"
        "    if (op[i]<0) {"
        "      op[i]=op[i]*pg[j];"
        "    }"
        "}"
        ;
    umem_setu(pbox->data);
    pbox->data->modifyu = 1;
    umem_setu(bias);
    umem_setu(prelu_gmma);
    long dis = pbox->w * pbox->h;
    int n = dis * inch;
    size_t _globalsize[] = {n};
    buf_t args[] = {
      clarg(pbox->data->u),
      clarg(bias->u),
      clarg(prelu_gmma->u),
      clarg(dis),
    };
    static ProgramSource  prelu_oclsrc(prelu_progstr);
    prelu_oclsrc.run("prelu", countof(_globalsize), _globalsize, NULL, countof(args), args);
    toc;
    return ;
  }

#endif

  if (0) {
    Dtype* op;
    Dtype* pb;
    Dtype* pg;
    int n, i, dis;
    umem_getu(pbox->data);
    umem_getu(bias);
    umem_getu(prelu_gmma);
    op = pBox_datap(pbox);
    pb = bias->p;
    pg = prelu_gmma->p;
    dis = pbox->w * pbox->h;
    n = dis * inch;

    tic;
    for (i = 0; i < n; i++) {
      int j = i / dis;
      op[i] = op[i] + pb[j];

      //op[i] = (op[i]>0)?op[i]:(op[i]*pg[j]);
      if (op[i] < 0) {
        op[i] = op[i] * pg[j];
      }
    }

    toc;
    return 1;
  }

  {
    int i, c, f, dis;
    umem_getu(pbox->data);
    umem_getu(bias);
    umem_getu(prelu_gmma);
    
    tic;
    dis = pbox->w * pbox->h;
    
    for (f = 0; f < pbox->f; ++f) {
      Dtype* pIn = pBox_datapf(pbox, f);
      
      for (c = 0; c < inch; ++c) {
        Dtype b = bias->p[c];
        Dtype g = prelu_gmma->p[c];
        Dtype* op = pIn + c * dis;
#if 0
        
        for (i = 0; i < dis; ++i) {
          op[i] = op[i] + b;
          op[i] = (op[i] > 0) ? op[i] : (op[i] * g);
        }
        
#endif
#if 1
        b = -b;
        i = 0;
        
        for (; i < dis - 3; i += 4) {
          op[i] = (op[i] > b) ? (op[i] - b) : (op[i] * g);
          op[i + 1] = (op[i + 1] > b) ? (op[i + 1] - b) : (op[i + 1] * g);
          op[i + 2] = (op[i + 2] > b) ? (op[i + 2] - b) : (op[i + 2] * g);
          op[i + 3] = (op[i + 3] > b) ? (op[i + 3] - b) : (op[i + 3] * g);
        }
        
        for (; i < dis; ++i) {
          op[i] = (op[i] > b) ? (op[i] - b) : (op[i] * g);
        }
        
#endif
      }
    }
    
    toc;
  }
  return 1;
}

static void matMulv(const float* A, const float* B, float* C, int astep, int ai, int M, int N)
{
  int i, j;

  for (i = 0; i < M; ++i) {
    float s = 0;

    for (j = 0; j < N; ++j) {
      s += A[i * astep + j * ai] * B[j];
    }

    C[i] = s;
  }
}

static int fullconnect(const Weight* weight, const pBox* pbox, pBox* outpBox)
{
  const Dtype* pIn;
  Dtype* pOut;
  int inch = pBox_channels(pbox);
  int instep = pbox->h * pbox->w * inch;
  int lastChannel = pbox->h * pbox->w * inch;
  pBoxsetsize(outpBox, 1, 1, weight->selfChannel, pbox->f);
  if (NULL==outpBox->data) {
    return 0;
  }
#ifdef HAVE_OPENCL1

  if (used_gpu) {
    tic;
    static const char* matMulv_progstr =
        "__kernel void matMulv(__global float* A, "
        "        __global float* B,  "
        "        __global float* C, int astep, int ai, int M, int N)  "
        "{"
        "    int i = get_global_id(0);"
        "    int j = 0;"
        "    float s = 0;"
        "    for (j=0; j<N; ++j) {"
        "      s += A[i*astep+j*ai]*B[j];"
        "    }"
        "    C[i] = s;"
        "}"
        ;
    umem_setu(pbox->data);
    umem_setu(weight->data);
    outpBox->data->modifyu = 1;
    int astep = weight->lastChannel, ai = 1, M = weight->selfChannel, N = weight->lastChannel;
    size_t _globalsize[] = {weight->selfChannel};
    buf_t args[] = {
      clarg(weight->data->u),
      clarg(pbox->data->u),
      clarg(outpBox->data->u),
      clarg(astep),
      clarg(ai),
      clarg(M),
      clarg(N),
    };
    static ProgramSource  matMulv_oclsrc(matMulv_progstr);
    matMulv_oclsrc.run("matMulv", countof(_globalsize), _globalsize, NULL, countof(args), args);
    toc;
    return ;
  }

#endif

  pIn = pBox_datap(pbox);
  pOut = pBox_datap(outpBox);

  if (0) {
    umem_getu(weight->data);
    umem_getu(pbox->data);
    SETMODIFYP(outpBox, 1);
    ASSERT(pbox->f == 1);
    matMulv(weight->data->p, pIn, pOut, lastChannel, 1, weight->selfChannel, lastChannel);
    return 1;
  }

#ifdef USE_OPENBLAS
  umem_getu(weight->data);
  umem_getu(pbox->data);
  SETMODIFYP(outpBox, 1);
  ASSERT(pbox->f == 1);
  tic;
  //Y←αAX + βY    β must be 0(zero)
  //               row         no trans         A's row               A'col
  cblas_sgemv(CblasRowMajor, CblasNoTrans, weight->selfChannel, weight->lastChannel, 1, weight->data->p, weight->lastChannel, pIn, 1, 0, pOut, 1);
  toc;
#else
  {
    int f;
    tic;
    
    for (f = 0; f < pbox->f; ++f) {
      img_t pbox1 = pBox_frame(pbox, f);
      img_t outpBox1 = pBox_frame(outpBox, f);
      innerProduct(&pbox1, &outpBox1, weight->selfChannel, weight->data->p, NULL);
    }
    
    toc;
  }
#endif
  return 1;
}
static int loadData2(const char* filename, int count, const long dataNumber[], Dtype* pTeam[])
{
  int i;
  FILE* pf;
  char buf[256];
  _snprintf(buf, 256, "%s.dat", filename);
  pf = fopen(buf, "rb");

  if (NULL == pf) {
    printf("%s\n", buf);
    return 0;
  }

  for (i = 0; i < count; ++i) {
    fread(pTeam[i], sizeof(Dtype), dataNumber[i], pf);
  }

  fclose(pf);
  return 1;
}
static int saveData2(const char* filename, int count, const long dataNumber[], Dtype* pTeam[])
{
  char buf[256];
  int i;
  FILE* pf;
  _snprintf(buf, 256, "%s.dat", filename);
  pf = fopen(buf, "wb");

  if (NULL == pf) {
    printf("%s\n", buf);
    return 0;
  }

  for (i = 0; i < count; ++i) {
    fwrite(pTeam[i], sizeof(Dtype)*dataNumber[i], 1, pf);
  }

  fclose(pf);
  return 1;
}
static int readData(int ismem, const char* filename, int count, const long dataNumber[], Dtype* pTeam[])
{
#if 1

  if (ismem) {
    int i;

    for (i = 0; i < count; ++i) {
      int size = dataNumber[i] * sizeof(Dtype);
      memcpy(pTeam[i], filename, size);
      filename += size;
    }

    return 1;
  }

  loadData2(filename, count, dataNumber, pTeam);
  return 1;
#else
  long* dataNumber1 = (long*)memdup(dataNumber, count * sizeof(*dataNumber));
  Dtype** pTeam1 = (Dtype**)memdup(pTeam, count * sizeof(Dtype*));
  readData1(filename, count, dataNumber1, pTeam1);
  saveData2(filename, count, dataNumber, pTeam);
  free(dataNumber1);
  free(pTeam1);
#endif
  return 1;
}
static int readData1(int ismem, const char* filename, int count, const long dataNumber[], umem_t* pMem[])
{
  Dtype** pTeam1 = (Dtype**)malloc(count * sizeof(Dtype*));
  int i;

  for (i = 0; i < count; ++i) {
    pTeam1[i] = pMem[i]->p;
    pMem[i]->modifyp = true;
  }

  readData(ismem, filename, count, dataNumber, pTeam1);

  for (i = 0; i < count; ++i) {
    umem_setu0(pMem[i]);
  }

  free(pTeam1);
  return 1;
}
static long initConvAndFc(struct Weight* weight, int schannel, int lchannel, int kersize, int stride, int pad)
{
  int byteLenght;
  weight->selfChannel = schannel;
  weight->kernel_h = kersize;
  weight->kernel_w = kersize;
  weight->stride_h = stride;
  weight->stride_w = stride;
  weight->pad = pad;
  byteLenght = weight->selfChannel * lchannel * weight->kernel_h * weight->kernel_w;
  umem_setsize(weight->bias, schannel * sizeof(Dtype));
  umem_setsize(weight->data, byteLenght * sizeof(Dtype));
  return byteLenght;
}
static int initpRelu(struct pRelu* prelu, int width)
{
  prelu->w = width;
  umem_setsize(prelu->data, width * sizeof(Dtype));
  return 0;
}
static int softmax(pBox* pbox)
{
  int inch = pBox_channels(pbox);
#ifdef HAVE_OPENCL1

  if (used_gpu) {
    tic;
    static const char* softmax_progstr =
        "__kernel void softmax(__global float* p2D, "
        "        int mapSize, int channel)  "
        "{"
        "    int i = get_global_id(0);"
        "    float eleSum = 0;"
        "    int c;"
        "    for(c=0;c<channel;c++){"
        "      int j = i+c*mapSize;"
        "      p2D[j] = exp(p2D[j]);"
        "      eleSum += p2D[j];"
        "    }"
        "    for(c=0;c<channel;c++){"
        "      int j = i+c*mapSize;"
        "      p2D[j] = p2D[j]/eleSum;"
        "    }"
        "}"
        ;
    umem_setu(pbox->data);
    pbox->data->modifyu = 1;
    long mapSize = pbox->w * pbox->h;
    size_t _globalsize[] = {mapSize};
    buf_t args[] = {
      clarg(pbox->data->u),
      clarg(mapSize),
      clarg(inch),
    };
    static ProgramSource  softmax_oclsrc(softmax_progstr);
    softmax_oclsrc.run("softmax", countof(_globalsize), _globalsize, NULL, countof(args), args);
    toc;
    return ;
  }

#endif

  if (1) {
    int i, c, f, mapSize;
    tic;
    umem_getu(pbox->data);
    SETMODIFYP(pbox, 1);
    mapSize = pbox->w * pbox->h;

    for (f = 0; f < pbox->f; ++f) {
      Dtype* p2D = pBox_datapf(pbox, f);

      for (i = 0; i < mapSize; ++i) {
        Dtype eleSum = 0;

        for (c = 0; c < inch; c++) {
          int j = i + c * mapSize;
          p2D[j] = exp(p2D[j]);
          eleSum += p2D[j];
        }

        for (c = 0; c < inch; c++) {
          int j = i + c * mapSize;
          p2D[j] = p2D[j] / eleSum;
        }
      }
    }

    toc;
    return 1;
  }

  {
    int c, f, mapSize, row, col;
    tic;
    umem_getu(pbox->data);
    SETMODIFYP(pbox, 1);
    mapSize = pbox->w * pbox->h;
    
    for (f = 0; f < pbox->f; ++f) {
      Dtype* p2D = pBox_datapf(pbox, f);
      
      for (row = 0; row < pbox->h; row++) {
        for (col = 0; col < pbox->w; col++) {
          Dtype eleSum = 0;
          
          for (c = 0; c < inch; c++) {
            int j = c * mapSize;
            p2D[j] = exp(p2D[j]);
            eleSum += p2D[j];
          }
          
          for (c = 0; c < inch; c++) {
            int j = c * mapSize;
            p2D[j] = p2D[j] / eleSum;
          }
          
          p2D++;
        }
      }
    }
    
    toc;
  }
}

#endif // _NCNN_INL_
