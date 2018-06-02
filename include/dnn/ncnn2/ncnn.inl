
#ifndef _NCNN_INL_
#define _NCNN_INL_
//#include "mat.h"
//#include "cpu.inl"
#include "mat.inl"
#include "layer.inl"

#define clarg(x)  {(sizeof(x)), (uchar*)&(x)}

static int image2Matrix(buf_t* bf, const img_t* image, pBox* outpBox)
{
  int i, j, f;
  if ((image->data == NULL) || (CC_MAT_DEPTH(image) != CC_8U)) {
    printf("image's type is wrong!!Please set CC_8U. 0x%08x %d\n", image->data, CC_MAT_DEPTH(image));
    return 0;
  }
  bf_pBoxsetsize(bf, outpBox, image->rows, image->cols, 3, image->f);
  if (NULL==outpBox->data) {
    return 0;
  }
  SETMODIFYP(outpBox, 1);
  
  for (f = 0; f < image->f; f++) {
    mydataFmt* p0 = pBox_datapf(outpBox, f);
    mydataFmt* p1 = p0 + outpBox->rows * outpBox->cols;
    mydataFmt* p2 = p0 + 2 * outpBox->rows * outpBox->cols;
    uchar* pimage0 = image->data + f * image->h * image->s;
    // [0,255]  -> [-1, 1]
    if (3==image->c) {
      for (i = 0; i < image->rows; i++) {
        uchar* pimage1 = pimage0 + i * image->step;
        j = 0;
        for (; j < image->cols-1; j+=2) {
          p0[0] = (pimage1[0] - 127.5) * 0.0078125;
          p1[0] = (pimage1[1] - 127.5) * 0.0078125;
          p2[0] = (pimage1[2] - 127.5) * 0.0078125;
          p0[1] = (pimage1[3] - 127.5) * 0.0078125;
          p1[1] = (pimage1[4] - 127.5) * 0.0078125;
          p2[1] = (pimage1[5] - 127.5) * 0.0078125;
          pimage1 += 6;
          p0 += 2;
          p1 += 2;
          p2 += 2;
        }
        for (; j < image->cols; j++) {
          *p0++ = (pimage1[0] - 127.5) * 0.0078125;
          *p1++ = (pimage1[1] - 127.5) * 0.0078125;
          *p2++ = (pimage1[2] - 127.5) * 0.0078125;
          pimage1 += 3;
        }
      }
    } else if (4==image->c) {
      for (i = 0; i < image->rows; i++) {
        uchar* pimage1 = pimage0 + i * image->step;
        j = 0;
        for (; j < image->cols-1; j+=2) {
          p0[0] = (pimage1[0] - 127.5) * 0.0078125;
          p1[0] = (pimage1[1] - 127.5) * 0.0078125;
          p2[0] = (pimage1[2] - 127.5) * 0.0078125;
          p0[1] = (pimage1[4] - 127.5) * 0.0078125;
          p1[1] = (pimage1[5] - 127.5) * 0.0078125;
          p2[1] = (pimage1[6] - 127.5) * 0.0078125;
          pimage1 += 8;
          p0 += 2;
          p1 += 2;
          p2 += 2;
        }
        for (; j < image->cols; j++) {
          *p0++ = (pimage1[0] - 127.5) * 0.0078125;
          *p1++ = (pimage1[1] - 127.5) * 0.0078125;
          *p2++ = (pimage1[2] - 127.5) * 0.0078125;
          pimage1 += 4;
        }
      }
    } else if (1==image->c) {
      for (i = 0; i < image->rows; i++) {
        uchar* pimage1 = pimage0 + i * image->step;
        j = 0;
        for (; j < image->cols-1; j+=2) {
          p2[0] = p1[0] = p0[0] = (pimage1[0] - 127.5) * 0.0078125;
          p2[1] = p1[1] = p0[1] = (pimage1[1] - 127.5) * 0.0078125;
          pimage1 += 2;
          p0 += 2;
          p1 += 2;
          p2 += 2;
        }
        for (; j < image->cols; j++) {
          double t = (*pimage1++ - 127.5) * 0.0078125;
          *p0++ = t;
          *p1++ = t;
          *p2++ = t;
        }
      }
    }
  }
  return 1;
}

static int featurePad(buf_t* bf, const pBox* pbox, pBox* outpBox, const int pad)
{
  mydataFmt* p, *pIn;
  int i, n, inch = pBox_channels(pbox);
  if (pad <= 0) {
    printf("the data needn't to pad, please check you network!\n");
    return 0;
  }

  bf_pBoxsetsize(bf, outpBox, pbox->height + 2 * pad, pbox->width + 2 * pad, inch, 1);
  if (NULL==outpBox->data) {
    return 0;
  }

  p = pBox_datap(outpBox);
  pIn = pBox_datap(pbox);
  n = pBox_channels(outpBox) * outpBox->height;

  for (i = 0; i < n; i++) {

    if ((i % outpBox->height) < pad || (i % outpBox->height > (outpBox->height - pad - 1))) {
      p += outpBox->width;
      continue;
    }

    p += pad;
    memcpy(p, pIn, pbox->width * sizeof(mydataFmt));
    p += pbox->width + pad;
    pIn += pbox->width;
  }
  return 1;
}
// im2col
static int _feature2Matrix(buf_t* bf, const pBox* pbox, pBox* outpBox, const Weight* weight)
{
  int kernelSize = weight->kernelSize;
  int stride = weight->stride;
  int w_out = (pbox->width - kernelSize) / stride + 1;//
  int h_out = (pbox->height - kernelSize) / stride + 1;
  int inch = pBox_channels(pbox);
  int i, j, f, row, col, krow, kcol, c;
  int pbox_count = pbox->height * pbox->width;
  bf_pBoxsetsize(bf, outpBox, w_out * h_out, inch * kernelSize * kernelSize, 1, pbox->f);
  if (NULL==outpBox->data) {
    return 0;
  }

#ifdef HAVE_OPENCL1

  //for (int i=0; i<pbox->height*pbox->width*inch; ++i) {    pIn[i] = i;  }  SETMODIFYP(pbox, 1);
  if (used_gpu) {
    tic;
    static const char* feature2Matrix_progstr =
        "__kernel void _feature2Matrix(__global float* pIn, "
        "        __global float* pOut,  "
        "        int n_in, int w_in, int w_out, int o_step, int kernelSize)  "
        "{"
        "    int i = get_global_id(0);"
        "    int j = get_global_id(1);"
        "    int c = j/(kernelSize*kernelSize);"
        "    int k = j%(kernelSize*kernelSize);"
        "    int y = i/w_out + k/kernelSize;"
        "    int x = i%w_out + k%kernelSize;"
        "    pOut[i*o_step+j] = pIn[c*n_in+y*w_in+x];"
        "}"
        ;
    int n_in = pbox->height * pbox->width;
    int w_in = pbox->width;
    umem_setu(pbox->data);
    outpBox->data->modifyu = 1;
    size_t _globalsize[] = {outpBox->height, outpBox->width};
    buf_t args[] = {
      clarg(pbox->data->u),
      clarg(outpBox->data->u),
      clarg(n_in),
      clarg(w_in),
      clarg(w_out),
      clarg(outpBox->width),
      clarg(kernelSize),
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
n_in 	  = pbox->height * pbox->width;

    for (f = 0; f < pbox->f; ++f) {
      const mydataFmt* pIn = pBox_datapf(pbox, f);
      mydataFmt* pOut = pBox_datapf(outpBox, f);

      for (i = 0; i < outpBox->height; ++i) {
        for (j = 0; j < outpBox->width; ++j) {
          int c = j / (kernelSize * kernelSize);
          int k = j % (kernelSize * kernelSize);
          int y = i / w_out + k / kernelSize;
          int x = i % w_out + k % kernelSize;
          pOut[i * outpBox->width + j] = pIn[c * n_in + y * pbox->width + x];
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
      const mydataFmt* pIn = pBox_datapf(pbox, f);
      mydataFmt* pOut = pBox_datapf(outpBox, f);

      for (row = 0; row < h_out; ++row) {
        for (col = 0; col < w_out; ++col) {
          for (c = 0; c < inch; ++c) {
            for (krow = 0; krow < kernelSize; ++krow) {
              for (kcol = 0; kcol < kernelSize; ++kcol) {
                int i = (row * w_out + col) * outpBox->width + c * kernelSize * kernelSize + krow * kernelSize + kcol;
                int j = c * (pbox->height * pbox->width) + (row + krow) * pbox->width + col + kcol;
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
  const mydataFmt* pIn0;
  const mydataFmt* pIn1;
  const mydataFmt* ptemp;
#if 1

  if (1 == kernelSize) {
    int countbuf[256];

    if (1 == inch) {
      for (f = 0; f < pbox->f; ++f) {
        const mydataFmt* pIn = pBox_datapf(pbox, f);
        mydataFmt* p = pBox_datapf(outpBox, f);

        for (row = 0; row < h_out; row ++) {
          pIn0 = pIn + row * stride * pbox->width;

          for (col = 0; col < w_out; col++) {
            *p++ = pIn0[col * stride];
          }
        }
      }
    }
    else if (2 == inch) {
      for (f = 0; f < pbox->f; ++f) {
        const mydataFmt* pIn = pBox_datapf(pbox, f);
        mydataFmt* p = pBox_datapf(outpBox, f);

        for (row = 0; row < h_out; row ++) {
          for (col = 0; col < w_out; col++) {
            pIn1 = pIn + row * stride * pbox->width + col * stride;
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
        const mydataFmt* pIn = pBox_datapf(pbox, f);
        mydataFmt* p = pBox_datapf(outpBox, f);

        for (row = 0; row < h_out; row ++) {
          for (col = 0; col < w_out; col++) {
            pIn1 = pIn + row * stride * pbox->width + col * stride;
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
        const mydataFmt* pIn = pBox_datapf(pbox, f);
        mydataFmt* p = pBox_datapf(outpBox, f);

        for (row = 0; row < h_out; row ++) {
          for (col = 0; col < w_out; col++) {
            pIn1 = pIn + row * stride * pbox->width + col * stride;
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
  else if (2 == kernelSize) {
    for (f = 0; f < pbox->f; ++f) {
      const mydataFmt* pIn = pBox_datapf(pbox, f);
      mydataFmt* p = pBox_datapf(outpBox, f);

      for (row = 0; row < h_out; row ++) {
        for (col = 0; col < w_out; col++) {
          pIn1 = pIn + row * stride * pbox->width + col * stride;

          for (c = 0; c < inch; c++) {
            ptemp = pIn1 + c * pbox_count;
            p[0] = ptemp[0];
            p[1] = ptemp[1];
            p[2] = ptemp[pbox->width];
            p[3] = ptemp[pbox->width + 1];
            p += 4;
          }
        }
      }
    }
  }
  else if (3 == kernelSize) {
    for (f = 0; f < pbox->f; ++f) {
      const mydataFmt* pIn = pBox_datapf(pbox, f);
      mydataFmt* p = pBox_datapf(outpBox, f);

      for (row = 0; row < h_out; row ++) {
        for (col = 0; col < w_out; col++) {
          pIn1 = pIn + row * stride * pbox->width + col * stride;

          for (c = 0; c < inch; c++) {
            const mydataFmt* ptemp = pIn1 + c * pbox_count;
            const mydataFmt* ptemp1 = ptemp + pbox->width;
            const mydataFmt* ptemp2 = ptemp + pbox->width + pbox->width;
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
      const mydataFmt* pIn = pBox_datapf(pbox, f);
      mydataFmt* p = pBox_datapf(outpBox, f);

      for (row = 0; row < h_out; row ++) {
        for (col = 0; col < w_out; col++) {
          pIn1 = pIn + row * stride * pbox->width + col * stride;

          for (c = 0; c < inch; c++) {
            ptemp = pIn1 + c * pbox_count;

            for (kernelRow = 0; kernelRow < kernelSize; kernelRow++) {
              memcpy(p, ptemp, kernelSize * sizeof(mydataFmt));
              p += kernelSize;
              ptemp += pbox->width;
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
static int convolution(buf_t* bf, const Weight* weight, const pBox* pbox, pBox* outpBox)
{
  int outh = (pbox->height - weight->kernelSize) / weight->stride + 1;
  int outw = (pbox->width - weight->kernelSize) / weight->stride + 1;
  int f, Matrix_count = outpBox->height * outpBox->width;
  bf_pBoxsetsize(bf, outpBox, outh, outw, weight->selfChannel, pbox->f);
  if (NULL==outpBox->data) {
    return 0;
  }

  if (1) {
    for (f = 0; f < pbox->f; ++f) {
      img_t pbox2 = pBox_frame(pbox, f);
      img_t outpBox2 = pBox_frame(outpBox, f);
      tic;
      convolution_forward(bf, &pbox2, &outpBox2, weight, NULL, 1);
      toc;
    }
    return 1;
  }

  for (f = 0; f < pbox->f; ++f) {
    img_t pbox2 = pBox_frame(pbox, f);
    img_t tmp_matrix[1] = {0};
    const float* tmp_matrixp = pBox_datap(tmp_matrix);
    float* outpBoxp = pBox_datapf(outpBox, f);
    _feature2Matrix(bf, &pbox2, tmp_matrix, weight);

    if (0) {
      tic;
      //matMul_block(weight->data->p, tmp_matrix->width, 1, tmp_matrixp, tmp_matrix->width, 1, outpBoxp, tmp_matrix->height, 1, weight->selfChannel, tmp_matrix->height, tmp_matrix->width);
      toc;
    }

    if (0) {
      tic;
      matMul(weight->data->p, tmp_matrix->width, 1, tmp_matrixp, tmp_matrix->width, 1, outpBoxp, tmp_matrix->height, 1, weight->selfChannel, tmp_matrix->height, tmp_matrix->width);
      toc;
    }

#ifdef HAVE_OPENCL1

    if (used_gpu) {
      tic;
      umem_setu(weight->data);
      umem_setu(tmp_matrix->data);
      outpBox->data->modifyu = 1;
      int astep = matrix->width, ai = 1, bstep = matrix->width, bi = 1, cstep = matrix->height, ci = 1, M = weight->selfChannel, N = matrix->height, K = matrix->width;
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
      cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasTrans, weight->selfChannel, tmp_matrix->height, tmp_matrix->width, 1, weight->data->p, tmp_matrix->width, tmp_matrixp, tmp_matrix->width, 0, outpBoxp, tmp_matrix->height);
    }
    else {
      ASSERT(0);
      pBox padpbox[1] = {0};
      featurePad(pbox, padpbox, weight->pad);
      //C←αAB + βC
      //                1              2            3              4     C's size    5              k     alpha     A*              A'col             B*           B'col    beta      C*           C'col
      cblas_sgemm(CblasRowMajor, CblasNoTrans, CblasTrans, weight->selfChannel, tmp_matrix->height, tmp_matrix->width, 1, weight->data->p, tmp_matrix->width, tmp_matrixp, tmp_matrix->width, 0, outpBoxp, tmp_matrix->height);
      freepBox(padpbox);
    }

    toc;
#endif
    bf_imfree(bf, tmp_matrix);
  }
  return 1;
}

static int maxPooling(buf_t* bf, const pBox* pbox, pBox* outpBox, int kernelSize, int stride)
{
  if (1) {
    int ret;
    tic;
    ret = pooling_forward(bf, pbox, outpBox, PoolMethod_MAX, kernelSize, stride, 0, 0);
    toc;
    return ret;
  }
  {
    int inch = pBox_channels(pbox);
    int outh = ceil((float)(pbox->height - kernelSize) / stride + 1);
    int outw = ceil((float)(pbox->width - kernelSize) / stride + 1);
    int i, c, row, col, kernelRow;
    int inchf = inch * pbox->f;
    bf_pBoxsetsize(bf, outpBox, outh, outw, inch, pbox->f);
  if (NULL==outpBox->data) {
    return 0;
  }
    
#ifdef HAVE_OPENCL1
    
    if (used_gpu) {
      tic;
      umem_setu(pbox->data);
      outpBox->data->modifyu = 1;
      int Matrix_count = outpBox->height * outpBox->width;
      int pbox_count = pbox->height * pbox->width;
      int ih = pbox->height;
      int iw = pbox->width;
      int ow = outpBox->width;
      size_t _globalsize[] = {inch, outpBox->height, outpBox->width};
      buf_t args[] = {
        clarg(pbox->data->u),
          clarg(outpBox->data->u),
          clarg(pbox_count),
          clarg(Matrix_count),
          clarg(ih),
          clarg(iw),
          clarg(ow),
          clarg(stride),
          clarg(kernelSize),
      };
      static const char* maxPooling_progstr =
        "__kernel void maxPooling(__global float* pIn, "
        "        __global float* pOut,  "
        "        int pbox_count, int Matrix_count, int ih, int iw, int ow, int stride, int kernelSize)  "
        "{"
        "    int c = get_global_id(0);"
        "    int row = get_global_id(1);"
        "    int col = get_global_id(2);"
        "    int off = c*pbox_count + row*stride*iw + col*stride;"
        "    float maxNum = pIn[off];"
        "    int diffh = row*stride-ih+1;"
        "    int diffw = col*stride-iw+1;"
        "    for (int kernelRow = 0; kernelRow < kernelSize; kernelRow++){"
        "      if((kernelRow+diffh)>0)break;"
        "      for(int i=0;i<kernelSize;i++){"
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
      int Matrix_count = outpBox->height * outpBox->width;
      int pbox_count = pbox->height * pbox->width;
      int ih = pbox->height;
      int iw = pbox->width;
      int ow = outpBox->width;
      const mydataFmt* pIn = pBox_datap(pbox);
      mydataFmt* pOut = pBox_datap(outpBox);
      const mydataFmt* pIn0;
      umem_getu(pbox->data);
      SETMODIFYP(outpBox, 1);
      
      tic;
      {
#pragma omp parallel for
        
        for (c = 0; c < inchf; c++) {
          pIn0 = pIn + c * pbox->height * pbox->width;
          
          for (row = 0; row < outpBox->height; row ++) {
            for (col = 0; col < outpBox->width; col++) {
              int off = c * pbox_count + row * stride * iw + col * stride;
              mydataFmt maxNum = pIn0[off];
              int diffh = row * stride - ih + 1;
              int diffw = col * stride - iw + 1;
              
              for (kernelRow = 0; kernelRow < kernelSize; kernelRow++) {
                if ((kernelRow + diffh) > 0) {
                  break;
                }
                
                for (i = 0; i < kernelSize; i++) {
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
      
      const mydataFmt* pIn = pBox_datap(pbox);
      mydataFmt* p = pBox_datap(outpBox);
      const mydataFmt* pIn0;
      const mydataFmt* pIn1;
      const mydataFmt* ptemp;
      const mydataFmt* ptemp1;
      mydataFmt maxNum = 0;
      
      if ((pbox->width - kernelSize) % stride == 0 && (pbox->height - kernelSize) % stride == 0) {
        if (2 == kernelSize) {
#pragma omp parallel for
          
          for (c = 0; c < inchf; c++) {
            pIn0 = pIn + c * pbox->height * pbox->width;
            
            for (row = 0; row < outpBox->height; row ++) {
              pIn1 = pIn0 + row * stride * pbox->width;
              
              for (col = 0; col < outpBox->width; col++) {
                ptemp = pIn1 + col * stride;
                *p++ = MAX4(ptemp[0], ptemp[1], ptemp[pbox->width], ptemp[pbox->width + 1]);
              }
            }
          }
        }
        else if (3 == kernelSize) {
          int w = pbox->width;
          int w2 = pbox->width * 2;
          
#pragma omp parallel for
          
          for (c = 0; c < inchf; c++) {
            pIn0 = pIn + c * pbox->height * pbox->width;
            
            for (row = 0; row < outpBox->height; row ++) {
              pIn1 = pIn0 + row * stride * pbox->width;
              
              for (col = 0; col < outpBox->width; col++) {
                const mydataFmt* ptemp = pIn1 + col * stride;
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
            pIn0 = pIn + c * pbox->height * pbox->width;
            
            for (row = 0; row < outpBox->height; row ++) {
              pIn1 = pIn0 + row * stride * pbox->width;
              
              for (col = 0; col < outpBox->width; col++) {
                ptemp = pIn1 + col * stride;
                maxNum = *ptemp;
                
                for (kernelRow = 0; kernelRow < kernelSize; kernelRow++) {
                  for (i = 0; i < kernelSize; i++) {
                    if (maxNum < * (ptemp + i + kernelRow * pbox->width)) {
                      maxNum = *(ptemp + i + kernelRow * pbox->width);
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
        int w = pbox->width;
        int diffh = 0, diffw = 0;
        
#pragma omp parallel for
        
        for (c = 0; c < inchf; c++) {
          float* p0 = p;
          pIn0 = pIn + c * pbox->height * pbox->width;
          for (row = 0; row < outpBox->height; row ++) {
            pIn1 = pIn0 + row * stride * pbox->width;
            diffh = MIN(pbox->height - row * stride, kernelSize);
            
            for (col = 0; col < outpBox->width; col++) {
              ptemp = pIn1 + col * stride;
              maxNum = *ptemp;
              diffw = MIN(pbox->width - col * stride, kernelSize);
              
              for (kernelRow = 0; kernelRow < diffh; kernelRow++) {
                ptemp1 = ptemp + kernelRow * pbox->width;
                
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
    long dis = pbox->width * pbox->height;
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
    mydataFmt* op;
    const mydataFmt* pb;
    int i, n, dis;
    tic;
    umem_getu(pbox->data);
    umem_getu(bias);
    SETMODIFYP(pbox, 1);
    op = pBox_datap(pbox);
    pb = bias->p;
    dis = pbox->width * pbox->height;
    n = dis * inch;

    for (i = 0; i < n; i++) {
      op[i] = op[i] + pb[i / dis];
    }

    toc;
    return 1;
  }

  {
    int i, f, c, dis = pbox->width * pbox->height;
    tic;
    umem_getu(pbox->data);
    umem_getu(bias);
    SETMODIFYP(pbox, 1);
    
    
    for (f = 0; f < pbox->f; ++f) {
      mydataFmt* pIn = pBox_datapf(pbox, f);
      
      for (c = 0; c < inch; ++c) {
        mydataFmt* op = pIn + c * dis;
        const mydataFmt b = bias->p[c];
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
  mydataFmt* pIn = pBox_datap(pbox);
  int i, c, dis = pbox->width * pbox->height;

  for (c = 0; c < inch; c++) {
    mydataFmt* op = pIn + c * dis;
    mydataFmt b = bias->p[c];

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
    long dis = pbox->width * pbox->height;
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
    mydataFmt* op;
    mydataFmt* pb;
    mydataFmt* pg;
    int n, i, dis;
    umem_getu(pbox->data);
    umem_getu(bias);
    umem_getu(prelu_gmma);
    op = pBox_datap(pbox);
    pb = bias->p;
    pg = prelu_gmma->p;
    dis = pbox->width * pbox->height;
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
    dis = pbox->width * pbox->height;
    
    for (f = 0; f < pbox->f; ++f) {
      mydataFmt* pIn = pBox_datapf(pbox, f);
      
      for (c = 0; c < inch; ++c) {
        mydataFmt b = bias->p[c];
        mydataFmt g = prelu_gmma->p[c];
        mydataFmt* op = pIn + c * dis;
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

static int fullconnect(buf_t* bf, const Weight* weight, const pBox* pbox, pBox* outpBox)
{
  const mydataFmt* pIn;
  mydataFmt* pOut;
  int inch = pBox_channels(pbox);
  int instep = pbox->h * pbox->w * inch;
  bf_pBoxsetsize(bf, outpBox, 1, 1, weight->selfChannel, pbox->f);
  if (NULL==outpBox->data) {
    return 0;
  }
  ASSERT(weight->lastChannel == pbox->h * pbox->w * inch);
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
    matMulv(weight->data->p, pIn, pOut, weight->lastChannel, 1, weight->selfChannel, weight->lastChannel);
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
      innerProduct(bf, &pbox1, &outpBox1, weight->selfChannel, weight->data->p, NULL);
    }
    
    toc;
  }
#endif
  return 1;
}
static int loadData2(const char* filename, int count, const long dataNumber[], mydataFmt* pTeam[])
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
    fread(pTeam[i], sizeof(mydataFmt), dataNumber[i], pf);
  }

  fclose(pf);
  return 1;
}
static int saveData2(const char* filename, int count, const long dataNumber[], mydataFmt* pTeam[])
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
    fwrite(pTeam[i], sizeof(mydataFmt)*dataNumber[i], 1, pf);
  }

  fclose(pf);
  return 1;
}
#if 0
#include <fstream>
static int readData1(const char* filename, int count, long dataNumber[], mydataFmt* pTeam[])
{
  char buf[256];
  _snprintf(buf, 256, "%s.txt", filename);
  ifstream in(buf);
  // countof(pointTeam),
  string line;

  if (in) {
    int i = 0;
    int count = 0;
    int pos = 0;

    while (getline(in, line)) {
      try {
        if (i < dataNumber[count]) {
          line.erase(0, 1);
          pos = line.find(']');
          line.erase(pos, 1);
          *(pTeam[count])++ = atof(line.data());
        }
        else {
          count++;
          dataNumber[count] += dataNumber[count - 1];

          line.erase(0, 1);
          pos = line.find(']');
          line.erase(pos, 1);
          *(pTeam[count])++ = atof(line.data());
        }

        i++;
      }
      catch (exception& e) {
        printf(" yichang %d\n", i);
        return;
      }
    }
  }
  else {
    printf("no such file %s\n");
  }
  return 1;
}
#endif
static int readData(int ismem, const char* filename, int count, const long dataNumber[], mydataFmt* pTeam[])
{
#if 1

  if (ismem) {
    int i;

    for (i = 0; i < count; ++i) {
      int size = dataNumber[i] * sizeof(mydataFmt);
      memcpy(pTeam[i], filename, size);
      filename += size;
    }

    return 1;
  }

  loadData2(filename, count, dataNumber, pTeam);
  return 1;
#else
  long* dataNumber1 = (long*)memdup(dataNumber, count * sizeof(*dataNumber));
  mydataFmt** pTeam1 = (mydataFmt**)memdup(pTeam, count * sizeof(mydataFmt*));
  readData1(filename, count, dataNumber1, pTeam1);
  saveData2(filename, count, dataNumber, pTeam);
  free(dataNumber1);
  free(pTeam1);
#endif
  return 1;
}
static int readData1(int ismem, const char* filename, int count, const long dataNumber[], umem_t* pMem[])
{
  mydataFmt** pTeam1 = (mydataFmt**)malloc(count * sizeof(mydataFmt*));
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
  weight->lastChannel = lchannel;
  weight->kernelSize = kersize;
  weight->stride = stride;
  weight->pad = pad;
  byteLenght = weight->selfChannel * weight->lastChannel * weight->kernelSize * weight->kernelSize;
  umem_setsize(weight->bias, schannel * sizeof(mydataFmt));
  umem_setsize(weight->data, byteLenght * sizeof(mydataFmt));
  return byteLenght;
}
static int initpRelu(struct pRelu* prelu, int width)
{
  prelu->width = width;
  umem_setsize(prelu->data, width * sizeof(mydataFmt));
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
    long mapSize = pbox->width * pbox->height;
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
    mapSize = pbox->width * pbox->height;

    for (f = 0; f < pbox->f; ++f) {
      mydataFmt* p2D = pBox_datapf(pbox, f);

      for (i = 0; i < mapSize; ++i) {
        mydataFmt eleSum = 0;

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
  mapSize = pbox->width * pbox->height;

  for (f = 0; f < pbox->f; ++f) {
    mydataFmt* p2D = pBox_datapf(pbox, f);

    for (row = 0; row < pbox->height; row++) {
      for (col = 0; col < pbox->width; col++) {
        mydataFmt eleSum = 0;

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
