
static int featurePad(const pBox* box, pBox* outpBox, const int pad)
{
  Dtype* p, *pIn;
  int i, n, inch = pBox_channels(box);
  if (pad <= 0) {
    printf("the data needn't to pad, please check you network!\n");
    return 0;
  }

  pBoxsetsize(outpBox, box->h + 2 * pad, box->w + 2 * pad, inch, 1);
  if (NULL==outpBox->data) {
    return 0;
  }

  p = pBox_datap(outpBox);
  pIn = pBox_datap(box);
  n = pBox_channels(outpBox) * outpBox->h;

  for (i = 0; i < n; i++) {

    if ((i % outpBox->h) < pad || (i % outpBox->h > (outpBox->h - pad - 1))) {
      p += outpBox->w;
      continue;
    }

    p += pad;
    memcpy(p, pIn, box->w * sizeof(Dtype));
    p += box->w + pad;
    pIn += box->w;
  }
  return 1;
}
// im2col
static int _feature2Matrix(const pBox* box, pBox* outpBox, const Weight* weight)
{
  DataSize kernelSize = weight->size[0];
  DataSize strideSize = weight->strideSize;
  int w_out = (box->w - kernelSize.w) / strideSize.w + 1;//
  int h_out = (box->h - kernelSize.h) / strideSize.h + 1;
  int inch = pBox_channels(box);
  int i, j, f, row, col, krow, kcol, c;
  int pbox_count = box->h * box->w;
  pBoxsetsize(outpBox, w_out * h_out, inch * kernelSize.h * kernelSize.w, 1, box->size.n);
  if (NULL==outpBox->data) {
    return 0;
  }

#ifdef HAVE_OPENCL1

  //for (int i=0; i<box->h*box->w*inch; ++i) {    pIn[i] = i;  }  SETMODIFYP(box, 1);
  if (used_gpu) {
    tic;
    static const char* feature2Matrix_progstr =
        "__kernel void _feature2Matrix(__global float* pIn, "
        "        __global float* pOut,  "
        "        int n_in, int w_in, int w_out, int o_step, int kernelSize.size)  "
        "{"
        "    int i = get_global_id(0);"
        "    int j = get_global_id(1);"
        "    int c = j/(kernelSize.size*kernelSize.size);"
        "    int k = j%(kernelSize.size*kernelSize.size);"
        "    int y = i/w_out + k/kernelSize.size;"
        "    int x = i%w_out + k%kernelSize.size;"
        "    pOut[i*o_step+j] = pIn[c*n_in+y*w_in+x];"
        "}"
        ;
    int n_in = box->h * box->w;
    int w_in = box->w;
    umem_setu(box->data);
    outpBox->data->modifyu = 1;
    size_t _globalsize[] = {outpBox->h, outpBox->w};
    buf_t args[] = {
      clarg(box->data->u),
      clarg(outpBox->data->u),
      clarg(n_in),
      clarg(w_in),
      clarg(w_out),
      clarg(outpBox->w),
      clarg(kernelSize.size),
    };
    static ProgramSource  feature2Matrix_oclsrc(feature2Matrix_progstr);
    feature2Matrix_oclsrc.run("_feature2Matrix", countof(_globalsize), _globalsize, NULL, countof(args), args);
    toc;
    return ;
  }

#endif

  umem_getu(box->data);
  SETMODIFYP(outpBox, 1);

  if (0) {
    int n_in;
    tic;
n_in 	  = box->h * box->w;

    for (f = 0; f < box->size.n; ++f) {
      const Dtype* pIn = pBox_datapf(box, f);
      Dtype* pOut = pBox_datapf(outpBox, f);

      for (i = 0; i < outpBox->h; ++i) {
        for (j = 0; j < outpBox->w; ++j) {
          int c = j / (kernelSize.h * kernelSize.w);
          int k = j % (kernelSize.h * kernelSize.w);
          int y = i / w_out + k / kernelSize.w;
          int x = i % w_out + k % kernelSize.w;
          pOut[i * outpBox->w + j] = pIn[c * n_in + y * box->w + x];
        }
      }
    }

    toc;
    //double aa = umem_checku(outpBox->data);
    return 1;
  }

  if (0) {
    tic;

    for (f = 0; f < box->size.n; ++f) {
      const Dtype* pIn = pBox_datapf(box, f);
      Dtype* pOut = pBox_datapf(outpBox, f);

      for (row = 0; row < h_out; ++row) {
        for (col = 0; col < w_out; ++col) {
          for (c = 0; c < inch; ++c) {
            for (krow = 0; krow < kernelSize.h; ++krow) {
              for (kcol = 0; kcol < kernelSize.w; ++kcol) {
                int i = (row * w_out + col) * outpBox->w + c * kernelSize.h * kernelSize.w + krow * kernelSize.w + kcol;
                int j = c * (box->h * box->w) + (row + krow) * box->w + col + kcol;
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

  if (1 == kernelSize.h && 1 == kernelSize.w) {
    int countbuf[256];

    if (1 == inch) {
      for (f = 0; f < box->size.n; ++f) {
        const Dtype* pIn = pBox_datapf(box, f);
        Dtype* p = pBox_datapf(outpBox, f);

        for (row = 0; row < h_out; row ++) {
          pIn0 = pIn + row * strideSize.h * box->w;

          for (col = 0; col < w_out; col++) {
            *p++ = pIn0[col * strideSize.w];
          }
        }
      }
    }
    else if (2 == inch) {
      for (f = 0; f < box->size.n; ++f) {
        const Dtype* pIn = pBox_datapf(box, f);
        Dtype* p = pBox_datapf(outpBox, f);

        for (row = 0; row < h_out; row ++) {
          for (col = 0; col < w_out; col++) {
            pIn1 = pIn + row * strideSize.h * box->w + col * strideSize.w;
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

      for (f = 0; f < box->size.n; ++f) {
        const Dtype* pIn = pBox_datapf(box, f);
        Dtype* p = pBox_datapf(outpBox, f);

        for (row = 0; row < h_out; row ++) {
          for (col = 0; col < w_out; col++) {
            pIn1 = pIn + row * strideSize.h * box->w + col * strideSize.w;
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
      for (f = 0; f < box->size.n; ++f) {
        const Dtype* pIn = pBox_datapf(box, f);
        Dtype* p = pBox_datapf(outpBox, f);

        for (row = 0; row < h_out; row ++) {
          for (col = 0; col < w_out; col++) {
            pIn1 = pIn + row * strideSize.h * box->w + col * strideSize.w;
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
  else if (2 == kernelSize.h && 2 == kernelSize.w) {
    for (f = 0; f < box->size.n; ++f) {
      const Dtype* pIn = pBox_datapf(box, f);
      Dtype* p = pBox_datapf(outpBox, f);

      for (row = 0; row < h_out; row ++) {
        for (col = 0; col < w_out; col++) {
          pIn1 = pIn + row * strideSize.h * box->w + col * strideSize.w;

          for (c = 0; c < inch; c++) {
            ptemp = pIn1 + c * pbox_count;
            p[0] = ptemp[0];
            p[1] = ptemp[1];
            p[2] = ptemp[box->w];
            p[3] = ptemp[box->w + 1];
            p += 4;
          }
        }
      }
    }
  }
  else if (3 == kernelSize.h && 3 == kernelSize.w) {
    for (f = 0; f < box->size.n; ++f) {
      const Dtype* pIn = pBox_datapf(box, f);
      Dtype* p = pBox_datapf(outpBox, f);

      for (row = 0; row < h_out; row ++) {
        for (col = 0; col < w_out; col++) {
          pIn1 = pIn + row * strideSize.h * box->w + col * strideSize.w;

          for (c = 0; c < inch; c++) {
            const Dtype* ptemp = pIn1 + c * pbox_count;
            const Dtype* ptemp1 = ptemp + box->w;
            const Dtype* ptemp2 = ptemp + box->w + box->w;
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
    for (f = 0; f < box->size.n; ++f) {
      const Dtype* pIn = pBox_datapf(box, f);
      Dtype* p = pBox_datapf(outpBox, f);

      for (row = 0; row < h_out; row ++) {
        for (col = 0; col < w_out; col++) {
          pIn1 = pIn + row * strideSize.h * box->w + col * strideSize.w;

          for (c = 0; c < inch; c++) {
            ptemp = pIn1 + c * pbox_count;

            for (kernelRow = 0; kernelRow < kernelSize.h; kernelRow++) {
              memcpy(p, ptemp, kernelSize.w * sizeof(Dtype));
              p += kernelSize.w;
              ptemp += box->w;
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


static int addbias(pBox* box, const umem_t* bias)
{
  int inch = pBox_channels(box);

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
    umem_setu(box->data);
    box->data->modifyu = 1;
    umem_setu(bias);
    long dis = box->w * box->h;
    int n = dis * pBox_channels(box);
    size_t _globalsize[] = {n};
    buf_t args[] = {
      clarg(box->data->u),
      clarg(bias->u),
      clarg(dis),
    };
    static ProgramSource  addbias_oclsrc(addbias_progstr);
    addbias_oclsrc.run("addbias", countof(_globalsize), _globalsize, NULL, countof(args), args);
    toc;
    //umem_getu(box->data);
    return ;
  }

#endif

  if (0) {
    Dtype* op;
    const Dtype* pb;
    int i, n, dis;
    tic;
    umem_getu(box->data);
    umem_getu(bias);
    SETMODIFYP(box, 1);
    op = pBox_datap(box);
    pb = bias->p;
    dis = box->w * box->h;
    n = dis * inch;

    for (i = 0; i < n; i++) {
      op[i] = op[i] + pb[i / dis];
    }

    toc;
    return 1;
  }

  {
    int i, f, c, dis = box->w * box->h;
    tic;
    umem_getu(box->data);
    umem_getu(bias);
    SETMODIFYP(box, 1);
    
    
    for (f = 0; f < box->size.n; ++f) {
      Dtype* pIn = pBox_datapf(box, f);
      
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

static int relu(pBox* box, umem_t* bias)
{
  int inch = pBox_channels(box);
  Dtype* pIn = pBox_datap(box);
  int i, c, dis = box->w * box->h;

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

static int prelu(pBox* box, const umem_t* prelu_gmma)
{
  int inch = pBox_channels(box);
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
    umem_setu(box->data);
    box->data->modifyu = 1;
    umem_setu(bias);
    umem_setu(prelu_gmma);
    long dis = box->w * box->h;
    int n = dis * inch;
    size_t _globalsize[] = {n};
    buf_t args[] = {
      clarg(box->data->u),
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
    Dtype* pg;
    int n, i, dis;
    umem_getu(box->data);
    umem_getu(bias);
    umem_getu(prelu_gmma);
    op = pBox_datap(box);
    pg = prelu_gmma->p;
    dis = box->w * box->h;
    n = dis * inch;

    tic;
    for (i = 0; i < n; i++) {
      int j = i / dis;

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
    umem_getu(box->data);
    umem_getu(bias);
    umem_getu(prelu_gmma);
    
    tic;
    dis = box->w * box->h;
    
    for (f = 0; f < box->size.n; ++f) {
      Dtype* pIn = pBox_datapf(box, f);
      
      for (c = 0; c < inch; ++c) {
        //Dtype 0 = 0;//bias->p[c];
        Dtype g = prelu_gmma->p[c];
        Dtype* op = pIn + c * dis;
#if 0
        
        for (i = 0; i < dis; ++i) {
          op[i] = op[i] + 0;
          op[i] = (op[i] > 0) ? op[i] : (op[i] * g);
        }
        
#endif
#if 1
        i = 0;
        
        for (; i < dis - 3; i += 4) {
          op[i] = (op[i] > 0) ? (op[i]) : (op[i] * g);
          op[i + 1] = (op[i + 1] > 0) ? (op[i + 1]) : (op[i + 1] * g);
          op[i + 2] = (op[i + 2] > 0) ? (op[i + 2]) : (op[i + 2] * g);
          op[i + 3] = (op[i + 3] > 0) ? (op[i + 3]) : (op[i + 3] * g);
        }
        
        for (; i < dis; ++i) {
          op[i] = (op[i] > 0) ? (op[i]) : (op[i] * g);
        }
        
#endif
      }
    }
    
    toc;
  }
  return 1;
}

static void initpRelu(float** pw, struct Weight* prelu, int width)
{
  prelu->size[0] = dataSize(1, width, 1, 1);
  prelu->n_data = 1;
  //umem_setsize(prelu->data, width * sizeof(Dtype));
  prelu->data[0].p = *pw;
  *pw += width;
  return ;
}
