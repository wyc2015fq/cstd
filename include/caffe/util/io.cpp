#include <fcntl.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/text_format.h>
#ifdef USE_OPENCV
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/highgui/highgui_c.h>
#include <opencv2/imgproc/imgproc.hpp>
#endif  // USE_OPENCV
#include <stdint.h>

#include <algorithm>
#include <fstream>  // NOLINT(readability/streams)
#include <string>
#include <vector>

#include "caffe/common.hpp"
#include "caffe/proto/caffe_proto.h"
#include "caffe/util/io.hpp"
//#include "glog/port.h"
#include <io.h>

const int kProtoReadBytesLimit = INT_MAX;  // Max size of 2 GB minus 1 byte.

namespace caffe
{

  using google::protobuf::io::FileInputStream;
  using google::protobuf::io::FileOutputStream;
  using google::protobuf::io::ZeroCopyInputStream;
  using google::protobuf::io::CodedInputStream;
  using google::protobuf::io::ZeroCopyOutputStream;
  using google::protobuf::io::CodedOutputStream;
  using google::protobuf::Message;

  bool ReadProtoFromTextFile(const char* filename, Message* proto)
  {
    int fd = _open(filename, O_RDONLY);
    CHECK_NE(fd, -1) << "File not found: " << filename;
    FileInputStream* input = new FileInputStream(fd);
    bool success = google::protobuf::TextFormat::Parse(input, proto);
    delete input;
    _close(fd);
    return success;
  }

  void WriteProtoToTextFile(const Message & proto, const char* filename)
  {
    int fd = _open(filename, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    FileOutputStream* output = new FileOutputStream(fd);
    CHECK(google::protobuf::TextFormat::Print(proto, output));
    delete output;
    _close(fd);
  }

  bool ReadProtoFromBinaryFile(const char* filename, Message* proto)
  {
    int fd = _open(filename, O_RDONLY | O_BINARY);
    CHECK_NE(fd, -1) << "File not found: " << filename;
    ZeroCopyInputStream* raw_input = new FileInputStream(fd);
    CodedInputStream* coded_input = new CodedInputStream(raw_input);
    coded_input->SetTotalBytesLimit(kProtoReadBytesLimit, 536870912);
    bool success = proto->ParseFromCodedStream(coded_input);
    delete coded_input;
    delete raw_input;
    _close(fd);
    return success;
  }

  void WriteProtoToBinaryFile(const Message & proto, const char* filename)
  {
    fstream output(filename, ios::out | ios::trunc | ios::binary);
    CHECK(proto.SerializeToOstream(&output));
  }

#ifdef USE_OPENCV
  cv::Mat ReadImageToCVMat(const string & filename,
                           const int height, const int width, const bool is_color)
  {
    cv::Mat cv_img;
    int cv_read_flag = (is_color ? CV_LOAD_IMAGE_COLOR :
                        CV_LOAD_IMAGE_GRAYSCALE);
    cv::Mat cv_img_origin = cv::imread(filename, cv_read_flag);
    if (!cv_img_origin.data) {
      LOG(ERROR) << "Could not open or find file " << filename;
      return cv_img_origin;
    }
    if (height > 0 && width > 0) {
      cv::resize(cv_img_origin, cv_img, cv::Size(width, height));
    } else if (height > 0 && width == -1) { //高度统一，宽度按宽高比计算
      int w0 = cv_img_origin.cols, h0 = cv_img_origin.rows;
      int h1 = height;
      int  w1 = h1 * w0 / h0;
      cv::resize(cv_img_origin, cv_img, cv::Size(w1, h1));
    } else if (height > 0 || width > 0) {
      int stdsize = std::max(width, height);
      int w0 = cv_img_origin.cols, h0 = cv_img_origin.rows;
      int w1 = w0, h1 = h0;
      if (w0 >= h0) {
        h1 = stdsize;
        w1 = h1 * w0 / h0;
      } else {
        w1 = stdsize;
        h1 = w1 * h0 / w0;
      }
      cv::resize(cv_img_origin, cv_img, cv::Size(w1, h1));
    } else {
      cv_img = cv_img_origin;
    }
    return cv_img;
  }

  cv::Mat ReadImageToCVMat(const string & filename,
                           const int height, const int width)
  {
    return ReadImageToCVMat(filename, height, width, true);
  }

  cv::Mat ReadImageToCVMat(const string & filename,
                           const bool is_color)
  {
    return ReadImageToCVMat(filename, 0, 0, is_color);
  }

  cv::Mat ReadImageToCVMat(const string & filename)
  {
    return ReadImageToCVMat(filename, 0, 0, true);
  }

// Do the file extension and encoding match?
  static bool matchExt(const std::string & fn,
                       std::string en)
  {
    size_t p = fn.rfind('.');
    std::string ext = p != fn.npos ? fn.substr(p) : fn;
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    std::transform(en.begin(), en.end(), en.begin(), ::tolower);
    if ( ext == en ) {
      return true;
    }
    if ( en == "jpg" && ext == "jpeg" ) {
      return true;
    }
    return false;
  }


  bool ReadImageToBlob(const string & filename, const int height, const int width, const bool is_color,
                        const std::string & encoding, BlobData* blob_img)
  {
    cv::Mat cv_img = ReadImageToCVMat(filename, height, width, is_color);
    if (cv_img.data) {
      if (encoding.size()) {
        if ((cv_img.channels() == 3) == is_color && !height && !width &&
            matchExt(filename, encoding)) {
          return ReadFileToBlob(filename, blob_img);
        }
        std::vector<uchar> buf;
        cv::imencode("." + encoding, cv_img, buf);
        blob_img->set_data(std::string(reinterpret_cast<char*>(&buf[0]),
                                    buf.size()));
        blob_img->set_encoded(true);
      } else {
        CVMatToBlob(cv_img, blob_img);
      }
      return true;
    } else {
      return false;
    }
  }
#endif  // USE_OPENCV

  bool ReadFileToBlob(const string & filename, BlobData* blob_img)
  {
    std::streampos size;
    fstream file(filename.c_str(), ios::in | ios::binary | ios::ate);
    if (file.is_open()) {
      size = file.tellg();
      std::string buffer(size, ' ');
      file.seekg(0, ios::beg);
      file.read(&buffer[0], size);
      file.close();
      Blob_NCHW(blob_img, true, buffer.data(), (int)buffer.size());
      return true;
    } else {
      return false;
    }
  }

#ifdef USE_OPENCV
  cv::Mat DecodeDatumToCVMatNative(const BlobData & blob)
  {
    cv::Mat cv_img;
    CHECK(blob.encoded()) << "BlobData not encoded";
    const string & data = blob.data();
    std::vector<char> vec_data(data.c_str(), data.c_str() + data.size());
    cv_img = cv::imdecode(vec_data, -1);
    if (!cv_img.data) {
      LOG(ERROR) << "Could not decode blob ";
    }
    return cv_img;
  }
  cv::Mat DecodeDatumToCVMat(const BlobData & blob, bool is_color)
  {
    cv::Mat cv_img;
    CHECK(blob.encoded()) << "BlobData not encoded";
    const string & data = blob.data();
    std::vector<char> vec_data(data.c_str(), data.c_str() + data.size());
    int cv_read_flag = (is_color ? CV_LOAD_IMAGE_COLOR :
                        CV_LOAD_IMAGE_GRAYSCALE);
    cv_img = cv::imdecode(vec_data, cv_read_flag);
    if (!cv_img.data) {
      LOG(ERROR) << "Could not decode blob ";
    }
    return cv_img;
  }

// If BlobData is encoded will decoded using DecodeDatumToCVMat and CVMatToBlob
// If BlobData is not encoded will do nothing
  bool DecodeDatumNative(BlobData* blob)
  {
    if (blob->encoded()) {
      cv::Mat cv_img = DecodeDatumToCVMatNative((*blob));
      CVMatToBlob(cv_img, blob);
      return true;
    } else {
      return false;
    }
  }
  bool DecodeDatum(BlobData* blob, bool is_color)
  {
    if (blob->encoded()) {
      cv::Mat cv_img = DecodeDatumToCVMat((*blob), is_color);
      CVMatToBlob(cv_img, blob);
      return true;
    } else {
      return false;
    }
  }

  void CVMatToBlob(const cv::Mat & cv_img, BlobData* blob)
  {
    CHECK(cv_img.depth() == CV_8U) << "Image data type must be unsigned byte";
    int datum_c = cv_img.channels();
    int datum_h = cv_img.rows;
    int datum_w = cv_img.cols;
    int datum_size = datum_c * datum_h * datum_w;
    std::string buffer(datum_size, ' ');
    for (int h = 0; h < datum_h; ++h) {
      const uchar* ptr = cv_img.ptr<uchar>(h);
      int img_index = 0;
      for (int w = 0; w < datum_w; ++w) {
        for (int c = 0; c < datum_c; ++c) {
          int datum_index = (c * datum_h + h) * datum_w + w;
          buffer[datum_index] = static_cast<char>(ptr[img_index++]);
        }
      }
    }
    Blob_NCHW(blob, false, (unsigned char*)buffer.data(), datum_w, datum_h, datum_c);
  }
#endif  // USE_OPENCV
}  // namespace caffe
