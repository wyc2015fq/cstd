
//#include "softmax_arm.inl"
static int softmax_axis(const Dtype* prevData, Dtype* nextData, DataSize size, int axis) {
  Dtype *max = NULL;
  Dtype *sum = NULL;
  int i, j, q, nn;
  int h = size.h;
  int w = size.w;
  int outer_dim = size.n;
  int bias_dim = size.c;
  int inner_dim = size.w * size.h;
  int step = size.c * size.w * size.h;
  
  if (axis == 0) {
    int size0 = inner_dim;
    resetArr(&max, size0);
    resetArr(&sum, size0);
    for (nn=0; nn<outer_dim; ++nn) {
      blas_sset(size0, -FLT_MAX, max);
      for (q = 0; q < bias_dim; q++) {
        const Dtype* ptr = prevData + q*inner_dim;
        Dtype* maxptr = max;
        
        for (i = 0; i < inner_dim; i++) {
          maxptr[i] = MAX(maxptr[i], ptr[i]);
        }
      }
      
#pragma omp parallel for
      for (q = 0; q < bias_dim; q++) {
        const Dtype* ptr = prevData + q*inner_dim;
        Dtype* outptr = nextData + q*inner_dim;
        Dtype* maxptr = max;
        
        for (i = 0; i < inner_dim; i++) {
          outptr[i] = expf(ptr[i] - maxptr[i]);
        }
      }
      
      blas_sset(size0, 0.f, sum);
      
      for (q = 0; q < bias_dim; q++) {
        const Dtype* outptr = nextData + q*inner_dim;
        Dtype* sumptr = sum;
        
        for (i = 0; i < inner_dim; i++) {
          sumptr[i] += outptr[i];
        }
      }
      
#pragma omp parallel for
      for (q = 0; q < bias_dim; q++) {
        Dtype* outptr = nextData + q * inner_dim;
        Dtype* sumptr = sum;
        
        for (i = 0; i < inner_dim; i++) {
          outptr[i] /= sumptr[i];
        }
      }
      prevData += step;
      nextData += step;
    }
  }
  else if (axis == 1) {
    int size1 = size.h * size.c;
    resetArr(&max, size1);
    resetArr(&sum, size1);
    for (nn=0; nn<outer_dim; ++nn) {
      blas_sset(size1, -FLT_MAX, max);
      
#pragma omp parallel for
      for (q = 0; q < bias_dim; q++) {
        const Dtype* ptr = prevData + q*inner_dim;
        Dtype* maxptr = max + q * size.h;
        
        for (i = 0; i < size.h; i++) {
          Dtype max = -FLT_MAX;
          
          for (j = 0; j < size.w; j++) {
            max = MAX(max, ptr[j]);
          }
          
          maxptr[i] = max;
          ptr += size.w;
        }
      }
      
#pragma omp parallel for
      for (q = 0; q < bias_dim; q++) {
        const Dtype* ptr = prevData + q*inner_dim;
        Dtype* outptr = nextData + q*inner_dim;
        Dtype* maxptr = max + q*size.h;
        
        for (i = 0; i < size.h; i++) {
          Dtype max = maxptr[i];
          
          for (j = 0; j < size.w; j++) {
            outptr[j] = exp(ptr[j] - max);
          }
          
          ptr += size.w;
          outptr += size.w;
        }
      }
      
      blas_sset(size1, 0.f, sum);
#pragma omp parallel for
      for (q = 0; q < bias_dim; q++) {
        const Dtype* outptr = nextData + q*inner_dim;
        Dtype* sumptr = sum + q*size.h;
        
        for (i = 0; i < size.h; i++) {
          Dtype sum = 0.f;
          
          for (j = 0; j < size.w; j++) {
            sum += outptr[j];
          }
          
          sumptr[i] = sum;
          outptr += size.w;
        }
      }
      
#pragma omp parallel for
      
      for (q = 0; q < bias_dim; q++) {
        Dtype* outptr = nextData + q*inner_dim;
        Dtype* sumptr = sum + q*size.h;
        
        for (i = 0; i < size.h; i++) {
          Dtype sum = sumptr[i];
          
          for (j = 0; j < size.w; j++) {
            outptr[j] /= sum;
          }
          
          outptr += size.w;
        }
      }
      prevData += step;
      nextData += step;
    }
  }
  else if (axis == 2) {
    int size2 = w * bias_dim;
    resetArr(&max, size2);
    resetArr(&sum, size2);
    for (nn=0; nn<outer_dim; ++nn) {
      blas_sset(size2, -FLT_MAX, max);
#pragma omp parallel for
      
      for (q = 0; q < bias_dim; q++) {
        const Dtype* ptr = prevData+ q*inner_dim;
        Dtype* maxptr = max+ q+w;
        
        for (i = 0; i < size.h; i++) {
          for (j = 0; j < w; j++) {
            maxptr[j] = MAX(maxptr[j], ptr[j]);
          }
          
          ptr += w;
        }
      }
      
#pragma omp parallel for
      for (q = 0; q < bias_dim; q++) {
        const Dtype* ptr = prevData + q*inner_dim;
        Dtype* outptr = nextData + q*inner_dim;
        Dtype* maxptr = max + q*w;
        
        for (i = 0; i < h; i++) {
          for (j = 0; j < w; j++) {
            outptr[j] = exp(ptr[j] - maxptr[j]);
          }
          
          ptr += w;
          outptr += w;
        }
      }
      
      blas_sset(size2, 0.f, sum);
#pragma omp parallel for
      
      for (q = 0; q < bias_dim; q++) {
        const Dtype* outptr = nextData + q*inner_dim;
        Dtype* sumptr = sum+q*w;
        
        for (i = 0; i < h; i++) {
          for (j = 0; j < w; j++) {
            sumptr[j] += outptr[j];
          }
          
          outptr += w;
        }
      }
      
#pragma omp parallel for
      
      for (q = 0; q < bias_dim; q++) {
        Dtype* outptr = nextData+ q*inner_dim;
        Dtype* sumptr = sum+q*w;
        
        for (i = 0; i < h; i++) {
          for (j = 0; j < w; j++) {
            outptr[j] /= sumptr[j];
          }
          
          outptr += w;
        }
      }
      prevData += step;
      nextData += step;
    }
  }
  FREE(max);
  FREE(sum);
}

// value = exp( value - global max value )
// sum all value
// value = value / sum
static int softmax_forward(const Dtype* prevData, Dtype* nextData, DataSize size, int axis)
{
  int i, nn;
  int outer_num = dataCount(size, 0, axis);
  int channels = size.dim[axis];
  int inner_num = dataCount(size, axis+1, 4);
  int count = Data_total(size);
  int dim = count/outer_num;

  if (1) {
    int i, j, k;
    Dtype* scaleData = NULL;
    Dtype* sum_multiplier = NULL;
    
    MYREALLOC(scaleData, inner_num);
    MYREALLOC(sum_multiplier, channels);
    blas_sset(channels, (1), sum_multiplier);
    blas_sset(inner_num, (0), scaleData);
    
    memcpy(nextData, prevData, count * sizeof(*prevData));
    // We need to subtract the max to avoid numerical issues, compute the exp,
    // and then normalize.
    for (i = 0; i < outer_num; ++i) {
      // initialize scaleData to the first plane
      memcpy(scaleData, prevData + i * dim, inner_num * sizeof(*prevData));
      for (j = 1; j < channels; j++) {
        for (k = 0; k < inner_num; k++) {
          scaleData[k] = MAX(scaleData[k], prevData[i * dim + j * inner_num + k]);
        }
      }
      // subtraction
      blas_sgemm(CblasNoTrans, CblasNoTrans, channels, inner_num,
        1, -1., sum_multiplier, scaleData, 1., nextData);
      // exponentiation
      my_sexp(dim, nextData, nextData);
      // sum after exp
      blas_sgemv(CblasTrans, channels, inner_num, 1.,
        nextData, sum_multiplier, 0., scaleData);
      // division
      for (j = 0; j < channels; j++) {
        my_sdiv(inner_num, nextData, scaleData, nextData);
        nextData += inner_num;
      }
    }
    FREE(scaleData);
    FREE(sum_multiplier);
  }
  if (0) {
    for (nn = 0; nn < outer_num; nn++) {
      //step1 : find max value
      Dtype maxVal = prevData[0];
      Dtype sum = 0;
      for (i = 0; i < dim; i++) {
        maxVal = MAX(maxVal, prevData[i]);
      }
      //step2 : sum
      for (i = 0; i < dim; i++) {
        nextData[i] = expf(prevData[i] - maxVal);
        sum += nextData[i];
      }
      //step3 : div
      for (i = 0; i < dim; i++) {
        nextData[i] = nextData[i] / sum;
      }
      prevData += dim;
      nextData += dim;
    }
  }
  return 0;
}

static int softmax_backward(const Dtype* prevData, const Dtype* nextData, const Dtype* nextDiff, Dtype* prevDiff, DataSize size, int axis) {
  int outer_num = dataCount(size, 0, axis);
  int channels = size.dim[axis];
  int inner_num = dataCount(size, axis+1, 4);
  int count = Data_total(size);
  int dim = count/outer_num;
  int i, j, k, pn;
  if (1) {
    Dtype* scaleData = NULL;
    Dtype* sum_multiplier = NULL;
    
    MYREALLOC(scaleData, inner_num);
    MYREALLOC(sum_multiplier, channels);
    blas_sset(channels, (1), sum_multiplier);
    blas_sset(inner_num, (0), scaleData);
    
    memcpy(prevDiff, nextDiff, count * sizeof(*prevDiff));
    for (i = 0; i < outer_num; ++i) {
      // compute dot(nextDiff, nextData) and subtract them from the bottom diff
      for (k = 0; k < inner_num; ++k) {
        scaleData[k] = blas_sstrided_dot(channels, prevDiff + i * dim + k, inner_num, nextData + i * dim + k, inner_num);
        //printf("scaleData=%f\n", scaleData[k]);
      }
      // subtraction
      blas_sgemm(CblasNoTrans, CblasNoTrans, channels, inner_num, 1,
        -1., sum_multiplier, scaleData, 1., prevDiff + i * dim);
    }
    // elementwise multiplication
    //printf("%10s data_dot=%f\n", next->name, BlobDataDot(next));
    for (k = 0; k < count; ++k) {
      //printf("prevDiff=%f nextData=%f\n", prevDiff[k], nextData[k]);
    }
    blas_smul(count, prevDiff, nextData, prevDiff);
    FREE(scaleData);
    FREE(sum_multiplier);
  }
  if (0) {
    for (pn = 0; pn < outer_num; pn++) {
      for (i = 0; i < dim; i++) {
        for (j = 0; j < dim; j++) {
          if (j == i) {
            prevDiff[i] += nextData[i] * (1.0f - nextData[i]) * nextDiff[j];
          }
          else {
            prevDiff[i] -= nextData[i] * nextData[j] * nextDiff[j];
          }
        }
      }
      prevData += dim;
      nextData += dim;
      nextDiff += dim;
      prevDiff += dim;
    }
  }
  return 0;
}
static Dtype SoftmaxLayer(enum LayerOp op, struct Net* net, struct Layer* layer) {
  struct Blob* prev = IBLOB(0);
  struct Blob* next = OBLOB(0);
  int axis = IARG(axis);
  axis = MAX(1, axis);
  switch (op) {
  case Op_Forward:
    next->size = prev->size; {
    const Dtype* prevData = getData(prev);
    Dtype* nextData = setDataZero(next);
    softmax_forward(prevData, nextData, prev->size, axis);
  }
  break;
  case Op_Backward: {
    const Dtype* prevData = getData(prev);
    const Dtype* nextData = getData(next);
    const Dtype* nextDiff = getDiff(next);
    Dtype* prevDiff = setDiffZero(prev);
    easyAssert(Data_4DEqu(prev->size, next->size), "data size must be equal!");
    softmax_backward(prevData, nextData, nextDiff, prevDiff, prev->size, axis);
    //printf("%10s diff_dot=%f\n", prev->name, Blob_diffDot(prev));
    //printf("%10s diff_dot=%f\n", next->name, Blob_diffDot(next));
  }
  break;
  default:
    break;
  }
  return 0;
}
#define net_softmax(net, input) net_null_1_1(net, input, SoftmaxLayer, "softmax")
static pBox* softmax(pBox* box)
{
  int inch = pBox_channels(box);
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
    umem_setu(box->data);
    box->data->modifyu = 1;
    long mapSize = box->w * box->h;
    size_t _globalsize[] = {mapSize};
    buf_t args[] = {
      clarg(box->data->u),
      clarg(mapSize),
      clarg(inch),
    };
    static ProgramSource  softmax_oclsrc(softmax_progstr);
    softmax_oclsrc.run("softmax", countof(_globalsize), _globalsize, NULL, countof(args), args);
    toc;
    return box;
  }

#endif

  if (1) {
    int i, c, f, mapSize;
    tic;
    umem_getu(box->data);
    SETMODIFYP(box, 1);
    mapSize = box->w * box->h;

    for (f = 0; f < box->size.n; ++f) {
      Dtype* p2D = pBox_datapf(box, f);

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
    return box;
  }

  {
    int c, f, mapSize, row, col;
    tic;
    umem_getu(box->data);
    SETMODIFYP(box, 1);
    mapSize = box->w * box->h;
    
    for (f = 0; f < box->size.n; ++f) {
      Dtype* p2D = pBox_datapf(box, f);
      
      for (row = 0; row < box->h; row++) {
        for (col = 0; col < box->w; col++) {
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
  return box;
}
