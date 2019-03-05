#ifndef CAFFE_UTIL_IO_H_
#define CAFFE_UTIL_IO_H_

#include "wstd/filesystem.hpp"
#include <iomanip>
#include <iostream>  // NOLINT(readability/streams)
#include <string>

#include "google/protobuf/message.h"

#include "caffe/common.hpp"
//#include "caffe/proto/caffe.pb.h"
#include "wstd/string.hpp"
#include "wstd/logging.hpp"

#ifndef CAFFE_TMP_DIR_RETRIES
#define CAFFE_TMP_DIR_RETRIES 100
#endif
using namespace std;
using namespace std::experimental;
using namespace std::experimental::filesystem;


namespace caffe
{
  using ::google::protobuf::Message;

  inline void MakeTempDir(string* temp_dirname)
  {
    temp_dirname->clear();
    const path & model =
      filesystem::temp_directory_path() / "caffe_test.%%%%-%%%%";
    for ( int i = 0; i < CAFFE_TMP_DIR_RETRIES; i++ ) {
      const path & dir = wstd::filesystem::unique_path(model).string();
      bool done = filesystem::create_directory(dir);
      if ( done ) {
        *temp_dirname = dir.string();
        return;
      }
    }
    LOG(FATAL) << "Failed to create a temporary directory.";
  }

  inline void MakeTempFilename(string* temp_filename)
  {
    static path temp_files_subpath;
    static uint64_t next_temp_file = 0;
    temp_filename->clear();
    if ( temp_files_subpath.empty() ) {
      string path_string = "";
      MakeTempDir(&path_string);
      temp_files_subpath = path_string;
    }
    *temp_filename = (temp_files_subpath / wstd::format("%09u", next_temp_file++)).string();
  }

  bool ReadProtoFromTextFile(const char* filename, Message* proto);

  inline bool ReadProtoFromTextFile(const string & filename, Message* proto)
  {
    return ReadProtoFromTextFile(filename.c_str(), proto);
  }

  inline void ReadProtoFromTextFileOrDie(const char* filename, Message* proto)
  {
    CHECK(ReadProtoFromTextFile(filename, proto));
  }

  inline void ReadProtoFromTextFileOrDie(const string & filename, Message* proto)
  {
    ReadProtoFromTextFileOrDie(filename.c_str(), proto);
  }

  void WriteProtoToTextFile(const Message & proto, const char* filename);
  inline void WriteProtoToTextFile(const Message & proto, const string & filename)
  {
    WriteProtoToTextFile(proto, filename.c_str());
  }

  bool ReadProtoFromBinaryFile(const char* filename, Message* proto);

  inline bool ReadProtoFromBinaryFile(const string & filename, Message* proto)
  {
    return ReadProtoFromBinaryFile(filename.c_str(), proto);
  }

  inline void ReadProtoFromBinaryFileOrDie(const char* filename, Message* proto)
  {
    CHECK(ReadProtoFromBinaryFile(filename, proto));
  }

  inline void ReadProtoFromBinaryFileOrDie(const string & filename,
      Message* proto)
  {
    ReadProtoFromBinaryFileOrDie(filename.c_str(), proto);
  }


  void WriteProtoToBinaryFile(const Message & proto, const char* filename);
  inline void WriteProtoToBinaryFile(
    const Message & proto, const string & filename)
  {
    WriteProtoToBinaryFile(proto, filename.c_str());
  }

  bool ReadFileToBlob(const string & filename, BlobData* blob);

  bool ReadImageToBlob(const string & filename, const int dim_h, const int dim_w, const bool is_color, const std::string & encoding, BlobData* blob);

  inline bool ReadImageToBlob(const string & filename, const int dim_h, const int dim_w, const bool is_color, BlobData* blob)
  {
    return ReadImageToBlob(filename, dim_h, dim_w, is_color, "", blob);
  }

  inline bool ReadImageToBlob(const string & filename, const int dim_h, const int dim_w, BlobData* blob)
  {
    return ReadImageToBlob(filename, dim_h, dim_w, true, blob);
  }

  inline bool ReadImageToBlob(const string & filename, const bool is_color, BlobData* blob)
  {
    return ReadImageToBlob(filename, 0, 0, is_color, blob);
  }
  
  inline bool ReadImageToBlob(const string & filename, BlobData* blob)
  {
    return ReadImageToBlob(filename, 0, 0, true, blob);
  }

  inline bool ReadImageToBlob(const string & filename, const std::string & encoding, BlobData* blob)
  {
    return ReadImageToBlob(filename, 0, 0, true, encoding, blob);
  }

  bool DecodeDatumNative(BlobData* blob);
  bool DecodeDatum(BlobData* blob, bool is_color);

#ifdef USE_OPENCV
  cv::Mat ReadImageToCVMat(const string & filename,
                           const int dim_h, const int dim_w, const bool is_color);

  cv::Mat ReadImageToCVMat(const string & filename,
                           const int dim_h, const int dim_w);

  cv::Mat ReadImageToCVMat(const string & filename,
                           const bool is_color);

  cv::Mat ReadImageToCVMat(const string & filename);

  cv::Mat DecodeDatumToCVMatNative(const BlobData & blob);
  cv::Mat DecodeDatumToCVMat(const BlobData & blob, bool is_color);

  void CVMatToBlob(const cv::Mat & cv_img, BlobData* blob);
#endif  // USE_OPENCV

  template <typename T>
  struct TypeFlag_TF { enum { flag = 0 }; };

#define TYPEFLAG_DEF(T, F)   template<> struct TypeFlag_TF<T> { enum { flag = F }; };
  template<> struct TypeFlag_TF<int> {
    enum { flag = TF_S32 };
  };
  //TYPEFLAG_DEF(int, TF_S32)
  TYPEFLAG_DEF(unsigned char, TF_U8)
    TYPEFLAG_DEF(float, TF_F32)
    TYPEFLAG_DEF(double, TF_F64)
#undef TYPEFLAG_DEF

  inline int blob_channels(const BlobData & blob) { return  blob.dim(2); }
  inline int blob_height(const BlobData & blob) { return   blob.dim(1); }
  inline int blob_width(const BlobData & blob) { return blob.dim(0); }
  inline int blob_size(const BlobData & blob) { return blob.dim(0)*blob.dim(1)*blob.dim(2)*blob.dim(3); }
  inline double blob_getValue(const BlobData & blob, int i) {
    void* data = (void*)blob.data().data();
    switch (blob.type())
    {
    case TF_U8:
      return ((unsigned char*)data)[i];
      break;
    case TF_S32:
      return ((int*)data)[i];
      break;
    case TF_F32:
      return ((float*)data)[i];
      break;
    case TF_F64:
      return ((double*)data)[i];
      break;
    default:
      assert(0);
      break;
    }
    }

    template <typename T> static
  bool Blob_NCHW(BlobData* blob, bool encoded, const T* data, int dim_w, int dim_h = 1, int dim_c = 1, int dim_n = 1) {
    int size = dim_w*dim_h*dim_c*dim_n;
    blob->clear_dim();
    blob->add_dim(dim_w);
    blob->add_dim(dim_h);
    blob->add_dim(dim_c);
    blob->add_dim(dim_n);
    blob->set_data(data, size * sizeof(T));
    blob->set_type((TypeFlag)TypeFlag_TF<T>::flag);
    blob->set_encoded(encoded);
    return true;
  }

  template <typename T> static
  bool Blob_NHWC(BlobData* blob, bool encoded, const T* data, int dim_c, int dim_w = 1, int dim_h = 1, int dim_n = 1, bool isNHWC = false) {
    int size = dim_w*dim_h*dim_c*dim_n;
    blob->clear_dim();
    T* buffer = (T*)malloc(size * sizeof(T));
    int img_index = 0;
    for (int i = 0; i < dim_n; ++i) {
      for (int h = 0; h < dim_h; ++h) {
        for (int w = 0; w < dim_w; ++w) {
          for (int c = 0; c < dim_c; ++c) {
            int datum_index = ((i*dim_c + c) * dim_h + h) * dim_w + w;
            buffer[datum_index] = (data[img_index++]);
          }
        }
      }
    }
    blob->set_data(buffer, size * sizeof(T));
    Blob_NCHW(blob, encoded, buffer, dim2, dim3, dim1, dim4);
    free(buffer);
    return true;
  }

  enum {
    NHWC,
    NCHW,
  };

  template <typename Stype, typename Dtype> inline
    bool blob_data_transform_T(int dim_n, int dim_c, int dim_h, int dim_w, Dtype* transformed_data, const Stype* data, int h_off, int w_off, bool do_mirror, Dtype scale, const Dtype* mean, const Dtype* mean_values_) {
    bool has_mean_file = mean != NULL;
    for (int n = 0; n < dim_n; ++n) {
      for (int c = 0; c < dim_c; ++c) {
        int c1 = n * dim_c + c;
        for (int h = 0; h < dim_h; ++h) {
          for (int w = 0; w < dim_w; ++w) {
            int top_index = 0, data_index = (c1 * dim_h + h_off + h) * dim_w + w_off + w;
            if (do_mirror) {
              top_index = (c1 * dim_h + h) * dim_w + (dim_w - 1 - w);
            }
            else {
              top_index = (c1 * dim_h + h) * dim_w + w;
            }
            Dtype datum_element = static_cast<Dtype>(data[data_index]);
            if (mean) {
              transformed_data[top_index] = (datum_element - mean[data_index]) * scale;
            }
            else {
              if (mean_values_) {
                transformed_data[top_index] = (datum_element - mean_values_[c]) * scale;
              }
              else {
                transformed_data[top_index] = datum_element * scale;
              }
            }
          }
        }
      }
    }
    return 0;
  }

  template <typename Dtype> inline
    bool blob_data_transform(int dim_n, int dim_c, int dim_h, int dim_w, Dtype* transformed_data, const void* data, TypeFlag flag, int h_off, int w_off, bool do_mirror, Dtype scale, const Dtype* mean, const Dtype* mean_values_) {
    switch (flag)
    {
    case TF_U8:
      blob_data_transform_T(dim_n, dim_c, dim_h, dim_w, transformed_data, (unsigned char*)data, h_off, w_off, do_mirror, scale, mean , mean_values_);
      break;
    case TF_S32:
      blob_data_transform_T(dim_n, dim_c, dim_h, dim_w, transformed_data, (int*)data, h_off, w_off, do_mirror, scale, mean, mean_values_);
      break;
    case TF_F32:
      blob_data_transform_T(dim_n, dim_c, dim_h, dim_w, transformed_data, (float*)data, h_off, w_off, do_mirror, scale, mean, mean_values_);
      break;
    case TF_F64:
      blob_data_transform_T(dim_n, dim_c, dim_h, dim_w, transformed_data, (double*)data, h_off, w_off, do_mirror, scale, mean, mean_values_);
      break;
    default:
      assert(0);
      break;
    }
    return 0;
  }

  template <typename Stype, typename Dtype> inline
    bool blob_data_copy_T(int dim_n, int dim_c, int dim_h, int dim_w, Dtype* transformed_data, const Stype* data) {
    
    for (int n = 0; n < dim_n; ++n) {
      for (int c = 0; c < dim_c; ++c) {
        int c1 = n * dim_c + c;
        for (int h = 0; h < dim_h; ++h) {
          for (int w = 0; w < dim_w; ++w) {
            int top_index = 0, data_index = (c1 * dim_h + h) * dim_w + w;
            top_index = (c1 * dim_h + h) * dim_w + w;
            Dtype datum_element = static_cast<Dtype>(data[data_index]);
            transformed_data[top_index] = datum_element;
          }
        }
      }
    }
    return 0;
  }

  template <typename Dtype> inline
    bool blob_data_copy(int dim_n, int dim_c, int dim_h, int dim_w, Dtype* transformed_data, const void* data, TypeFlag flag) {
    switch (flag)
    {
    case TF_U8:
      blob_data_copy_T(dim_n, dim_c, dim_h, dim_w, transformed_data, (unsigned char*)data);
      break;
    case TF_S32:
      blob_data_copy_T(dim_n, dim_c, dim_h, dim_w, transformed_data, (int*)data);
      break;
    case TF_F32:
      blob_data_copy_T(dim_n, dim_c, dim_h, dim_w, transformed_data, (float*)data);
      break;
    case TF_F64:
      blob_data_copy_T(dim_n, dim_c, dim_h, dim_w, transformed_data, (double*)data);
      break;
    default:
      assert(0);
      break;
    }
    return 0;
  }

  template <typename Dtype> inline
    bool blob_data_copy(Dtype* transformed_data, const BlobData& blob) {
    int dim_n = blob.dim(3);
    int dim_c = blob.dim(2);
    int dim_h = blob.dim(1);
    int dim_w = blob.dim(0);
    return blob_data_copy(dim_n, dim_c, dim_h, dim_w, transformed_data, blob.data().data(), blob.type());
  }

  inline bool LabelsToBlob(const std::vector<int> & labels, BlobData* blob_labels) {
    return Blob_NCHW(blob_labels, false, labels.data(), (int)labels.size());
  }

  inline bool LabelToBlob(int label, BlobData* blob_labels) {
    return Blob_NCHW(blob_labels, false, &label, 1);
  }

}  // namespace caffe

#endif   // CAFFE_UTIL_IO_H_
