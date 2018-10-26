
#ifndef _CAFFE_PROTO_H_
#define _CAFFE_PROTO_H_


#include "parser/base64.h"
#include "parser/cJSON.hpp"

#include <vector>
#include <string>
using namespace std;

enum SolverAction {
  NONE = 0,  // Take no special action.
  STOP = 1,  // Stop training. snapshot_after_train controls whether a
             // snapshot is created.
             SNAPSHOT = 2  // Take a snapshot, and keep training.
};

enum Phase { TRAIN, TEST, };
const char* Phase_Name[] = { "TRAIN", "TEST" };

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

void IntSerializeToString(string* value, int& i, int t) {
  value->insert(i, (char*)&t, sizeof(t));
  i += sizeof(t);
}
int IntParseFromString(const string& value, int& i) {
  int t = *(const int*)(&value[i]);
  i += sizeof(t);
  return t;
}

void BytesSerializeToString(string* value, int& i, const void* arr, int size) {
  value->append((char*)arr, size);
  i+=size;
}

void BytesParseFromString(const string& value, int i, void* arr, int size) {
  memcpy(arr, &value[i], size);
  i+=size;
}

struct BlobData {
  BlobData() {
    type = TF_S8;
    encoded = false;
  }
  std::vector<int> dim;
  // the actual image data, in bytes
  std::vector<unsigned char> data;
  enum TypeFlag type;
  // Optionally, the datum could also hold float data.
  //repeated float float_data = 4;
  // If true data contains an encoded image that need to be decoded
  bool encoded;// = 5[default = false];
  void add_dim(int x) {
    dim.push_back(x);
  }

  void set_dim(const void* ptr, int size) {
    data.assign((const unsigned char*)ptr, (const unsigned char*)ptr + size);
  }

};

void BlobDataSerializeToString(string* value, int& i, const BlobData& blob) {
  IntSerializeToString(value, i, blob.type);
  IntSerializeToString(value, i, blob.encoded);
  IntSerializeToString(value, i, blob.dim.size());
  BytesSerializeToString(value, i, blob.dim.data(), blob.dim.size() * sizeof(int));
  IntSerializeToString(value, i, blob.data.size());
  BytesSerializeToString(value, i, blob.data.data(), blob.data.size() * sizeof(char));
}
void BlobDataParseFromString(const string& value, int& i, BlobData& blob) {
  blob.type = (enum TypeFlag)IntParseFromString(value, i);
  blob.encoded = !!IntParseFromString(value, i);
  int size = IntParseFromString(value, i);
  blob.dim.resize(size);
  BytesParseFromString(value, i, blob.dim.data(), blob.dim.size() * sizeof(int));
  size = IntParseFromString(value, i);
  BytesParseFromString(value, i, blob.data.data(), blob.data.size() * sizeof(char));
}


typedef std::vector<BlobData> Datum;

void DatumSerializeToString(string* value, int& i, const Datum& datum) {
  IntSerializeToString(value, i, datum.size());
  for (int j = 0; j < datum.size(); ++j) {
    BlobDataSerializeToString(value, i, datum[j]);
  }
}
void DatumParseFromString(const string& value, int& i, Datum& datum) {
  int size = IntParseFromString(value, i);
  datum.resize(size);
  for (int j = 0; j < datum.size(); ++j) {
    BlobDataParseFromString(value, i, datum[j]);
  }
}

//struct Datum{  std::vector<BlobData> blob;};


#endif // _CAFFE_PROTO_H_
