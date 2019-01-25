
#include "opencv2/imgcodecs.hpp"
//#include "opencv2/core/src/matrix.cpp"
#include "opencv2/libcore.cpp"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image.inl"
#include "stb/stb_image_write.inl"

namespace cv {

  int get_comp(int flags) {
    int req_comp = 0;
    switch (flags)
    {
    case IMREAD_UNCHANGED:
    case IMREAD_ANYDEPTH:
    case IMREAD_ANYCOLOR:
      req_comp = 0;
      break;
    case IMREAD_GRAYSCALE:
      req_comp = 1;
      break;
    case IMREAD_COLOR:
      req_comp = 3;
      break;
    default:
    case IMREAD_LOAD_GDAL:
    case IMREAD_REDUCED_GRAYSCALE_2:
    case IMREAD_REDUCED_COLOR_2:
    case IMREAD_REDUCED_GRAYSCALE_4:
    case IMREAD_REDUCED_COLOR_4:
    case IMREAD_REDUCED_GRAYSCALE_8:
    case IMREAD_REDUCED_COLOR_8:
    case IMREAD_IGNORE_ORIENTATION:
      assert(0);
      break;
    }
    return req_comp;
  }
  //typedef unsigned char uchar;
  static void rgb2bgr(uchar* dst0, int dststep, int dstinc, const uchar* src0, int srcstep, int srcinc, int h, int w) {
    int i, j;
    for (j = 0; j < h; ++j) {
      uchar* dst = dst0 + j*dststep;
      const uchar* src = src0 + j*srcstep;
      for (i = 0; i < w; ++i) {
        char a = src[0], b = src[1], c = src[2];
        dst[0] = c;
        dst[1] = b;
        dst[2] = a;
        src += srcinc;
        dst += dstinc;
      }
    }
  }
  static void gray2bgr(uchar* dst0, int dststep, int dstinc, const uchar* src0, int srcstep, int srcinc, int h, int w) {
    int i, j;
    for (j = 0; j < h; ++j) {
      uchar* dst = dst0 + j*dststep;
      const uchar* src = src0 + j*srcstep;
      for (i = 0; i < w; ++i) {
        char a = src[0];
        dst[0] = a;
        dst[1] = a;
        dst[2] = a;
        src += srcinc;
        dst += dstinc;
      }
    }
  }

  static void* memcpy2d_(void* dst, int dl, const void* src, int sl, int h, int w)
  {
    char* d = (char*)(dst);
    const char* s = (const char*)(src);
    if (dl == sl && dl == w) {
      return memcpy(dst, src, h * w);
    }
    for (; h--; s += sl, d += dl) {
      memcpy(d, s, w);
    }
    return dst;
  }
  bool imencode(const String& ext, InputArray _image,
    std::vector<uchar>& buf, const std::vector<int>& params) {
    assert(0);
    return false;
  }
  Mat imdecode(InputArray _buf, int flags) {
    Mat buf = _buf.getMat(), img;
    // stbi_load_from_memory
    int w = 0, h = 0, n = 0, req_comp = get_comp(flags);
    int size = buf.rows*buf.cols*buf.channels();
    unsigned char *data = stbi_load_from_memory(buf.data, size, &w, &h, &n, req_comp);
    Mat mat(h, w, CV_MAKETYPE(CV_8U, n));
    //memcpy2d_(mat.data, mat.step, data, w*n, h, w*n);
    rgb2bgr(mat.data, mat.step, 3, data, w*n, 3, h, w);
    stbi_image_free(data);
    return mat;
  }
  Mat imread(const String& filename, int flags) {
    int w=0, h=0, n=0, req_comp = get_comp(flags);
    unsigned char *data = stbi_load(filename.c_str(), &w, &h, &n, req_comp);
    int nn = n * 10 + req_comp;
    if (!data) { return Mat(); }
    Mat mat(h, w, CV_MAKETYPE(CV_8U, req_comp));
    //memcpy2d_(mat.data, mat.step, data, w*n, h, w*n);
    switch(nn) {
    case 14:
    case 13:
      gray2bgr(mat.data, mat.step, req_comp, data, w*n, n, h, w);
      break;
    case 34:
    case 44:
    case 43:
    case 33:
      rgb2bgr(mat.data, mat.step, req_comp, data, w*n, n, h, w);
      break;
    default:
      assert(0);
      break;
    }
    stbi_image_free(data);
    return mat;
  }
  bool imwrite(const String& filename, InputArray img, const std::vector<int>& params) {
#if 1
    std::vector<Mat> img_vec;
    if (img.isMatVector() || img.isUMatVector())
      img.getMatVector(img_vec);
    else
      img_vec.push_back(img.getMat());

    if (img_vec.size() > 0) {
      Mat mat = img_vec[0];
      //memcpy2d_(mat.data, mat.step, data, w*n, h, w*n);

      int w = mat.cols;
      int h = mat.rows;
      int n = mat.channels();
      int step = mat.step;
      uchar* data = mat.data;
      Mat mat2(h, w, CV_MAKETYPE(CV_8U, n));
      rgb2bgr(mat2.data, mat2.step, 3, data, w*n, 3, h, w);
      step = mat2.step;
      data = mat2.data;
      size_t pos = filename.rfind('.');
      if (pos == String::npos) { return false; }
      String ext = filename.substr(pos+1).toLowerCase();
#if 0
      int stbi_write_png(char const *filename, int w, int h, int comp, const void *data, int stride_in_bytes);
      int stbi_write_bmp(char const *filename, int w, int h, int comp, const void *data);
      int stbi_write_tga(char const *filename, int w, int h, int comp, const void *data);
      int stbi_write_hdr(char const *filename, int w, int h, int comp, const void *data);
#endif
      stbi_write_png(filename.c_str(), w, h, n, data, step);
      return true;
    }
#endif
    return false;
  }
} // namespace cv

