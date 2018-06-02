
#define _mm_muladd_ps(a, b, c)  a = _mm_add_ps(a, _mm_mul_ps(b, c))

#include "arm/convolution_1x1.h"
#include "arm/convolution_2x2.h"
#include "arm/convolution_3x3.h"

#define CONV3X3S1_NEON_ARGS(args1, j, OPT_DEF) \
	OPT_DEF(args1, j, const img_t*, bottom_blob) \
	OPT_DEF(args1, j, img_t*, top_blob) \
	OPT_DEF(args1, j, const float*, _kernel) \
	OPT_DEF(args1, j, const float*, _bias) \

#if 0
static int conv3x3s1_neon_work(void* arg, int beg, int end) {
	int p = 0, j=0, inch;
	void** args = (void**)arg;
	CONV3X3S1_NEON_ARGS(args, j, POPARG);
  inch = pBox_channels(bottom_blob);
  for (int p=beg; p<end; ++p) {
    img_t out = pBox_channel(top_blob, p);
    const float* kernel0 = _kernel + p * inch * 9;
    const float bias0 = _bias ? _bias[p] : 0.f;
    conv3x3s1_neon(bottom_blob, &out, kernel0, &bias0);
  }
  return 0;
}

static void conv3x3s1_neon_thd(const img_t* bottom_blob, img_t* top_blob, const float* _kernel, const float* _bias) {
	int j=0;
	void* args[20];
  int outch = pBox_channels(top_blob);
	CONV3X3S1_NEON_ARGS(args, j, PUSHARG);
  parallel_for(args, 0, outch, conv3x3s1_neon_work);
}
#endif

//#include "arm/convolution_5x5.h"
static int convolution_forward(buf_t* bf, const img_t* bottom_blob, img_t* top_blob, const Weight* weight, const float* bias_data CC_DEFAULT(NULL), int dilation CC_DEFAULT(1))
{
  // convolv with NxN kernel
  // value = value + bias
  img_t im[1] = {0};
  int w = bottom_blob->w;
  int h = bottom_blob->h;
  int channels = pBox_channels(bottom_blob);
  int kernel_size = weight->kernelSize;
  int pad = weight->pad;
  int stride = weight->stride;
  int num_output = weight->selfChannel;

  //     fprintf(stderr, "Convolution input %d x %d  pad = %d  ksize=%d  stride=%d\n", w, h, pad, kernel_size, stride);
  const int kernel_extent = dilation * (kernel_size - 1) + 1;

  const img_t* bottom_blob_bordered = bottom_blob;

  if (pad > 0) {
    bottom_blob_bordered = copy_make_border(bf, bottom_blob, im, pad, pad, pad, pad, BORDER_CONSTANT, 0.f);

    w = bottom_blob_bordered->w;
    h = bottom_blob_bordered->h;
  }
  else if (pad == -233) {
    int wpad = kernel_extent + (w - 1) / stride * stride - w;
    int hpad = kernel_extent + (h - 1) / stride * stride - h;

    if (wpad > 0 || hpad > 0) {
      bottom_blob_bordered = copy_make_border(bf, bottom_blob, im, hpad / 2, hpad - hpad / 2, wpad / 2, wpad - wpad / 2, BORDER_CONSTANT, 0.f);
    }

    w = bottom_blob_bordered->w;
    h = bottom_blob_bordered->h;
  }

  {
    typedef void (*conv_func)(const img_t*, img_t*, const float * kernel, const float*);
    conv_func conv = NULL;
    int outw = (w - kernel_extent) / stride + 1;
    int outh = (h - kernel_extent) / stride + 1;
    const float* _kernel = weight->data->p;
    const float* _bias = bias_data ? bias_data : NULL;
    bf_pBoxsetsize(bf, top_blob, outh, outw, num_output, 1);
    // kernel_size x stride
    {
      static conv_func conv_func_table[8][4] = {0};
      static int inited = 0;
      
      if (!inited) {
        inited = 1;
#define CONV_DEF_SET(kernel_size, stride)    conv_func_table[kernel_size-1][stride-1] = conv ## kernel_size ## x ## kernel_size ## s ## stride ## _neon
        CONV_DEF_SET(1, 1);
        CONV_DEF_SET(2, 1);
        CONV_DEF_SET(3, 1);
        //CONV_DEF_SET(5, 1);
#undef CONV_DEF_SET
        //conv_func_table[3 - 1][1 - 1] = conv3x3s1_neon_thd;
      }
      
      conv = conv_func_table[kernel_size - 1][stride - 1];
      //printf(" kernel_size %d stride %d", kernel_size, stride);
    }
    
    if (conv) {
      conv(bottom_blob, top_blob, _kernel, _bias);
    }
    else {
      //#include "x86/convolution_5x5.h"
      const int maxk = kernel_size * kernel_size;
      
      int i, j, k, p, q;
      // kernel offsets
      int* space_ofs = NULL;
      const float* weight_data_ptr = weight->data->p;
      BF_MALLOC(space_ofs, maxk);
      {
        int p1 = 0;
        int p2 = 0;
        int gap = w * dilation - kernel_size * dilation;
        
        for (i = 0; i < kernel_size; i++) {
          for (j = 0; j < kernel_size; j++) {
            space_ofs[p1] = p2;
            p1++;
            p2 += dilation;
          }
          
          p2 += gap;
        }
      }
      
      // num_output
#pragma omp parallel for
      
      for (p = 0; p < num_output; p++) {
        float* outptr = pBox_channel_p(top_blob, p);
        
        for (i = 0; i < outh; i++) {
          for (j = 0; j < outw; j++) {
            float sum = 0.f;
            const float* kptr;
            
            if (bias_data) {
              sum = bias_data[p];
            }
            
            kptr = weight_data_ptr + maxk * channels * p;
            
            // channels
            for (q = 0; q < channels; q++) {
              const float* sptr = pBox_at(bottom_blob_bordered, q, i * stride, j * stride);
              
              for (k = 0; k < maxk; k++) { // 29.23
                float val = sptr[ space_ofs[k] ]; // 20.72
                float w = kptr[k];
                sum += val * w; // 41.45
              }
              
              kptr += maxk;
            }
            
            outptr[j] = sum;
          }
          
          outptr += outw;
        }
      }
      
      BF_FREE(space_ofs);
    }
    
  }
  return 0;
}

struct Convolution
{
  // param
  int num_output;
  int kernel_size;
  int dilation;
  int stride;
  int pad;
  int bias_term;

  int weight_data_size;

  // model
  float* weight_data;
  float* bias_data;
};

static int Convolution_forward(buf_t* bf, const Layer* layer, const img_t* bottom_blob, img_t* top_blob) {
  struct Convolution* s = (struct Convolution*)layer->x;
  Weight weight[1] = {0};
  weight->kernelSize = s->kernel_size;
  weight->pad = s->pad;
  weight->stride = s->stride;
  weight->selfChannel = s->num_output;
  weight->data->p = s->weight_data;
  convolution_forward(bf, bottom_blob, top_blob, weight, s->bias_data, s->dilation);
  return 0;
}

static int Convolution_load_param(Layer* layer, const char* param) {
  struct Convolution* s = (struct Convolution*)layer->x;
  char* p = (char*)param;
  str_getint(p, &p, &s->num_output, 0);
  str_getint(p, &p, &s->kernel_size, 0);
  str_getint(p, &p, &s->dilation, 1);
  str_getint(p, &p, &s->stride, 1);
  str_getint(p, &p, &s->pad, 0);
  str_getint(p, &p, &s->bias_term, 0);
  str_getint(p, &p, &s->weight_data_size, 0);
  return 1;
}

#if 0

int Convolution::load_model(FILE* binfp)
{
  int nread;

  union {
    struct {
      unsigned char f0;
      unsigned char f1;
      unsigned char f2;
      unsigned char f3;
    };
    unsigned int tag;
  } flag_struct;

  nread = fread(&flag_struct, sizeof(flag_struct), 1, binfp);

  if (nread != 1) {
    fprintf(stderr, "Convolution read flag_struct failed %d\n", nread);
    return -1;
  }

  unsigned int flag = flag_struct.f0 + flag_struct.f1 + flag_struct.f2 + flag_struct.f3;

  weight_data.create(weight_data_size);

  if (weight_data.empty()) {
    return -100;
  }

  if (flag_struct.tag == 0x01306B47) {
    // half-precision weight data
    int align_weight_data_size = alignSize(weight_data_size * sizeof(unsigned short), 4);
    std::vector<unsigned short> float16_weights;
    float16_weights.resize(align_weight_data_size);
    nread = fread(float16_weights.data(), align_weight_data_size, 1, binfp);

    if (nread != 1) {
      fprintf(stderr, "Convolution read float16_weights failed %d\n", nread);
      return -1;
    }

    weight_data = img_t::from_float16(float16_weights.data(), weight_data_size);

    if (weight_data.empty()) {
      return -100;
    }
  }
  else if (flag != 0) {
    // quantized weight data
    float quantization_value[256];
    nread = fread(quantization_value, 256 * sizeof(float), 1, binfp);

    if (nread != 1) {
      fprintf(stderr, "Convolution read quantization_value failed %d\n", nread);
      return -1;
    }

    int align_weight_data_size = alignSize(weight_data_size * sizeof(unsigned char), 4);
    std::vector<unsigned char> index_array;
    index_array.resize(align_weight_data_size);
    nread = fread(index_array.data(), align_weight_data_size, 1, binfp);

    if (nread != 1) {
      fprintf(stderr, "Convolution read index_array failed %d\n", nread);
      return -1;
    }

    float* weight_data_ptr = weight_data;

    for (int i = 0; i < weight_data_size; i++) {
      weight_data_ptr[i] = quantization_value[ index_array[i] ];
    }
  }
  else if (flag_struct.f0 == 0) {
    // raw weight data
    nread = fread(weight_data, weight_data_size * sizeof(float), 1, binfp);

    if (nread != 1) {
      fprintf(stderr, "Convolution read weight_data failed %d\n", nread);
      return -1;
    }
  }

  if (bias_term) {
    bias_data.create(num_output);

    if (bias_data.empty()) {
      return -100;
    }

    nread = fread(bias_data, num_output * sizeof(float), 1, binfp);

    if (nread != 1) {
      fprintf(stderr, "Convolution read bias_data failed %d\n", nread);
      return -1;
    }
  }

  return 0;
}

int Convolution::load_model(const unsigned char*& mem)
{
  union {
    struct {
      unsigned char f0;
      unsigned char f1;
      unsigned char f2;
      unsigned char f3;
    };
    unsigned int tag;
  } flag_struct;

  memcpy(&flag_struct, mem, sizeof(flag_struct));
  mem += sizeof(flag_struct);

  unsigned int flag = flag_struct.f0 + flag_struct.f1 + flag_struct.f2 + flag_struct.f3;

  if (flag_struct.tag == 0x01306B47) {
    // half-precision weight data
    weight_data = img_t::from_float16((unsigned short*)mem, weight_data_size);
    mem += alignSize(weight_data_size * sizeof(unsigned short), 4);

    if (weight_data.empty()) {
      return -100;
    }
  }
  else if (flag != 0) {
    // quantized weight data
    const float* quantization_value = (const float*)mem;
    mem += 256 * sizeof(float);

    const unsigned char* index_array = (const unsigned char*)mem;
    mem += alignSize(weight_data_size * sizeof(unsigned char), 4);

    weight_data.create(weight_data_size);

    if (weight_data.empty()) {
      return -100;
    }

    float* weight_data_ptr = weight_data;

    for (int i = 0; i < weight_data_size; i++) {
      weight_data_ptr[i] = quantization_value[ index_array[i] ];
    }
  }
  else if (flag_struct.f0 == 0) {
    // raw weight data
    weight_data = img_t(weight_data_size, (float*)mem);
    mem += weight_data_size * sizeof(float);
  }

  if (bias_term) {
    bias_data = img_t(num_output, (float*)mem);
    mem += num_output * sizeof(float);
  }

  return 0;
}

#endif
