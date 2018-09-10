
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

  Mat imread(const String& filename, int flags) {
    int w, h, n, req_comp = get_comp(flags);
    unsigned char *data = stbi_load("rgba.png", &w, &h, &n, req_comp);
    Mat mat(h, w, CV_MAKETYPE(CV_8U, n), data);
    stbi_image_free(data);
    return mat;
  }


  bool imwrite(const String& filename, InputArray img, const std::vector<int>& params) {
#if 0
    std::vector<Mat> img_vec;
    if (img.isMatVector() || img.isUMatVector())
      img.getMatVector(img_vec);
    else
      img_vec.push_back(img.getMat());

    if (img_vec.size() > 0) {
      stbi_write_png(filename.c_str(), w, h, n, data, w * 4);
#if 0
      int stbi_write_png(char const *filename, int w, int h, int comp, const void *data, int stride_in_bytes);
      int stbi_write_bmp(char const *filename, int w, int h, int comp, const void *data);
      int stbi_write_tga(char const *filename, int w, int h, int comp, const void *data);
      int stbi_write_hdr(char const *filename, int w, int h, int comp, const void *data);
#endif
      return true;
    }
#endif
    return false;
  }
}

