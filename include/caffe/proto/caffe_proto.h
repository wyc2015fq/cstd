
#ifndef _CAFFE_PROTO_H_
#define _CAFFE_PROTO_H_

#include "caffe/caffe_config.h"

#ifdef USE_PRO
#include "caffe.pb.h"
#endif

#ifdef USE_JSON
#include "parser/base64.h"
#include "parser/cJSON.hpp"

#include <vector>

namespace caffe {

  enum Phase {
    TRAIN = 0,
    TEST = 1,
  };

  enum TypeFlag //枚举消息类型
  {
    TF_S8 = 0, //proto3版本中，首成员必须为0，成员不应有相同的值
    TF_U8 = 1,
    TF_S16 = 2,
    TF_U16 = 3,
    TF_S32 = 4,
    TF_U32 = 5,
    TF_F32 = 6,
    TF_F64 = 7,
  };

  struct BlobData {
    std::vector<int> dim;
    // the actual image data, in bytes
    std::vector<unsigned char> data;
    enum TypeFlag type;
    // Optionally, the datum could also hold float data.
    //repeated float float_data = 4;
    // If true data contains an encoded image that need to be decoded
    bool encoded;// = 5[default = false];
  };

  typedef std::vector<BlobData> Datum;

  //struct Datum{  std::vector<BlobData> blob;};

};
#endif

#endif // _CAFFE_PROTO_H_
