

#include <stdio.h>
#include <limits.h>
#include <direct.h>
#include <math.h>
#include <fstream>
#include <limits>

#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/io/zero_copy_stream_impl.h>
#include <google/protobuf/text_format.h>
#include <google/protobuf/message.h>

#include "caffe/proto/caffe.pb.h"
#include "caffe/proto/caffe.pb.cc"
#include "parser/cjson.hpp"
#define CJSON_ADDNUMBERARRAYTOOBJECT(json, name, arr)  cjson_AddNumberArrayToObject(json, name, arr.begin(), arr.size())
#define CJSON_ADDSTRINGARRAYTOOBJECT(json, name, arr)  cjson_AddStringArrayToObject(json, name, tostdvecstr(arr.begin(), arr.size()))

#ifdef _DEBUG
#pragma comment(lib,"libprotobufd.lib")
#else
#pragma comment(lib,"libprotobuf.lib")
#endif

static inline size_t alignSize(size_t sz, int n)
{
  return (sz + n - 1) & -n;
}

// convert float to half precision floating point
static unsigned short float2half(float value)
{
  // 1 : 8 : 23
  union
  {
    unsigned int u;
    float f;
  } tmp;

  tmp.f = value;

  // 1 : 8 : 23
  unsigned short sign = (tmp.u & 0x80000000) >> 31;
  unsigned short exponent = (tmp.u & 0x7F800000) >> 23;
  unsigned int significand = tmp.u & 0x7FFFFF;

  //     fprintf(stderr, "%d %d %d\n", sign, exponent, significand);

      // 1 : 5 : 10
  unsigned short fp16;
  if (exponent == 0)
  {
    // zero or denormal, always underflow
    fp16 = (sign << 15) | (0x00 << 10) | 0x00;
  }
  else if (exponent == 0xFF)
  {
    // infinity or NaN
    fp16 = (sign << 15) | (0x1F << 10) | (significand ? 0x200 : 0x00);
  }
  else
  {
    // normalized
    short newexp = exponent + (-127 + 15);
    if (newexp >= 31)
    {
      // overflow, return infinity
      fp16 = (sign << 15) | (0x1F << 10) | 0x00;
    }
    else if (newexp <= 0)
    {
      // underflow
      if (newexp >= -10)
      {
        // denormal half-precision
        unsigned short sig = (significand | 0x800000) >> (14 - newexp);
        fp16 = (sign << 15) | (0x00 << 10) | sig;
      }
      else
      {
        // underflow
        fp16 = (sign << 15) | (0x00 << 10) | 0x00;
      }
    }
    else
    {
      fp16 = (sign << 15) | (newexp << 10) | (significand >> 13);
    }
  }

  return fp16;
}

static int quantize_weight(float *data, size_t data_length, std::vector<unsigned short>& float16_weights)
{
  float16_weights.resize(data_length);

  for (size_t i = 0; i < data_length; i++)
  {
    float f = data[i];

    unsigned short fp16 = float2half(f);

    float16_weights[i] = fp16;
  }

  // magic tag for half-precision floating point
  return 0x01306B47;
}

static bool quantize_weight(float *data, size_t data_length, int quantize_level, std::vector<float> &quantize_table, std::vector<unsigned char> &quantize_index) {

  assert(quantize_level != 0);
  assert(data != NULL);
  assert(data_length > 0);

  if (data_length < static_cast<size_t>(quantize_level)) {
    fprintf(stderr, "No need quantize,because: data_length < quantize_level");
    return false;
  }

  quantize_table.reserve(quantize_level);
  quantize_index.reserve(data_length);

  // 1. Find min and max value
  float max_value = std::numeric_limits<float>::min();
  float min_value = std::numeric_limits<float>::max();

  for (size_t i = 0; i < data_length; ++i)
  {
    if (max_value < data[i]) max_value = data[i];
    if (min_value > data[i]) min_value = data[i];
  }
  float strides = (max_value - min_value) / quantize_level;

  // 2. Generate quantize table
  for (int i = 0; i < quantize_level; ++i)
  {
    quantize_table.push_back(min_value + i * strides);
  }

  // 3. Align data to the quantized value
  for (size_t i = 0; i < data_length; ++i)
  {
    size_t table_index = int((data[i] - min_value) / strides);
    table_index = std::min<float>(table_index, quantize_level - 1);

    float low_value = quantize_table[table_index];
    float high_value = low_value + strides;

    // find a nearest value between low and high value.
    float targetValue = data[i] - low_value < high_value - data[i] ? low_value : high_value;

    table_index = int((targetValue - min_value) / strides);
    table_index = std::min<float>(table_index, quantize_level - 1);
    quantize_index.push_back(table_index);
  }

  return true;
}

static bool read_proto_from_text(const char* filepath, google::protobuf::Message* message)
{
  std::ifstream fs(filepath, std::ifstream::in);
  if (!fs.is_open())
  {
    fprintf(stderr, "open failed %s\n", filepath);
    return false;
  }

  google::protobuf::io::IstreamInputStream input(&fs);
  bool success = google::protobuf::TextFormat::Parse(&input, message);

  fs.close();

  return success;
}

static bool read_proto_from_binary(const char* filepath, google::protobuf::Message* message)
{
  std::ifstream fs(filepath, std::ifstream::in | std::ifstream::binary);
  if (!fs.is_open())
  {
    fprintf(stderr, "open failed %s\n", filepath);
    return false;
  }

  google::protobuf::io::IstreamInputStream input(&fs);
  google::protobuf::io::CodedInputStream codedstr(&input);

  codedstr.SetTotalBytesLimit(INT_MAX, INT_MAX / 2);

  bool success = message->ParseFromCodedStream(&codedstr);

  fs.close();

  return success;
}

#define DefBool(name)  if (param.has_##name ()) { cjson_AddBoolToObject(json_layer, #name , param.name()); }
#define DefNumber(name)  if (param.has_##name ()) { cjson_AddNumberToObject(json_layer, #name , param.name()); }
#define DefString(name)  if (param.has_##name ()) { cjson_AddStringToObject(json_layer, #name , param.name().c_str()); }
#define DefEnum(name, TYPE)  if (param.has_##name ()) { cjson_AddStringToObject(json_layer, #name , param.TYPE ## _Name(param.name()).c_str()); }
#define DefEnum2(name, TYPE, _Name)  if (param.has_##name ()) { cjson_AddStringToObject(json_layer, #name , _Name(param.name()).c_str()); }
#define DefNumberArray(name)  { CJSON_ADDNUMBERARRAYTOOBJECT(json_layer, #name , param.name()); }
#define DefStringArray(name)  { CJSON_ADDSTRINGARRAYTOOBJECT(json_layer, #name , param.name()); }


template <typename F>
vector<string> tostdvecstr(F f, int size) {
  vector<string> out;
  out.resize(size);
  int i;
  for (i = 0; i < size; ++i) {
    out[i] = *f++;
  }
  return out;
}

void FillerParameter(cjson* root, const char* name, caffe::FillerParameter param) {
  //cjson_AddStringToObject(json_layer, "filter_filler", param.filter_filler().type().c_str());
  cjson * json_layer = cjson_AddItemToObject(root, name, cjson_CreateObject());
  DefString(type);
  DefNumber(value);
  DefNumber(min);
  DefNumber(max);
  DefNumber(mean);
  DefNumber(std);
  DefNumber(sparse);
  DefEnum(variance_norm, VarianceNorm);
}
void ParamSpec(cjson* json_layer, caffe::ParamSpec param) {
  DefString(name);
  DefNumber(lr_mult);
  DefNumber(decay_mult);
}

int caffe2json(int argc, char** argv)
{
  if (!(argc == 3 || argc == 5 || argc == 6))
  {
    fprintf(stderr, "Usage: %s [caffeproto] [caffemodel] [solverproto] [jsonoutfile] [quantizelevel]\n", argv[0]);
    return -1;
  }

  const char* caffeproto = argv[1];
  const char* caffemodel = argv[2];
  const char* solverproto = argv[3];
  const char* jsonoutfile = argc >= 5 ? argv[4] : "out.json";
  const char* quantize_param = argc >= 6 ? argv[5] : "0";
  int quantize_level = atoi(quantize_param);

  if (quantize_level != 0 && quantize_level != 256 && quantize_level != 65536) {
    fprintf(stderr, "%s: only support quantize level = 0, 256, or 65536", argv[0]);
    return -1;
  }

  caffe::NetParameter proto;
  caffe::NetParameter net_;
  caffe::NetParameter* net = NULL;
  
  // load
  bool s0 = read_proto_from_text(caffeproto, &proto);
  if (!s0)
  {
    fprintf(stderr, "read_proto_from_text failed\n");
    return -1;
  }

  if (caffemodel) {
    bool s1 = read_proto_from_binary(caffemodel, &net_);
    if (!s1) {
      fprintf(stderr, "read_proto_from_binary failed\n");
      //return -1;
    }
    else {
      net = &net_;
    }
  }
  cjson * root = cjson_CreateObject();
  int layer_count = proto.layer_size();
  cjson * json_root = cjson_CreateObject();
  cjson * json_layers = cjson_CreateArray();

  if (1) {
    caffe::SolverParameter param;
    cjson * json_layer = cjson_gcObjectItem(json_root, "solver");
    bool s0 = read_proto_from_text(solverproto, &param);
    if (!s0)
    {
      fprintf(stderr, "read_proto_from_text failed\n");
      return -1;
    }
    DefNumber(momentum);// 0.9
    DefNumber(weight_decay);// 0.00005
    DefString(lr_policy);// "step"
    DefNumber(gamma);// 0.1
    DefNumber(stepsize);// : 20000
    // Display every 20 iterations
    DefNumber(display);// 20
    //# The maximum number of iterations
    DefNumber(max_iter);// : 1000
    //# snapshot intermediate results
    DefNumber(snapshot);// : 1000
    DefString(snapshot_prefix);// : "c3d_ucf101"
    DefEnum(solver_mode, SolverMode);// : GPU
    DefNumber(momentum);
    //DefNumberArray(kernel_size);
  }
  int input_size = proto.input_size();
  int input_dim_size = proto.input_dim_size();
  if (input_size>0) {
    cjson * json_layer = cjson_pushArrayItem(json_layers);
    cjson_AddStringToObject(json_layer, "name", "input");
    cjson_AddStringToObject(json_layer, "type", "Input");
    cjson * top_json = cjson_gcObjectItem(json_layer, "top", cjson_Array);
    for (int i = 0; i < input_size; ++i) {
      cjson_AddItemToArray(top_json, cjson_CreateString(proto.input(i).c_str()));
    }
    cjson * shape_json = cjson_gcObjectItem(json_layer, "shape", cjson_Array);
    for (int i = 0; i < input_dim_size; ++i) {
      cjson_AddItemToArray(shape_json, cjson_CreateNumber(proto.input_dim(i)));
    }
  }

  for (int layer_i = 0; layer_i < layer_count; layer_i++) {
    const caffe::LayerParameter& layer = proto.layer(layer_i);
    string layer_type = layer.type();
    cjson * json_layer = cjson_pushArrayItem(json_layers);
    // find blob binary by layer name
    cjson_AddStringToObject(json_layer, "name", layer.name().c_str());
    cjson_AddStringToObject(json_layer, "type", layer_type.c_str());
    CJSON_ADDSTRINGARRAYTOOBJECT(json_layer, "top", layer.top());
    CJSON_ADDSTRINGARRAYTOOBJECT(json_layer, "bottom", layer.bottom());
    int netidx;
    //printf("%d\n", layer_i);    if (layer_i == 22) {      int asdf = 0;    }
    if (net) {
      for (netidx = 0; netidx < net->layer_size(); netidx++)
      {
        if (net->layer(netidx).name() == layer.name())
        {
          break;
        }
      }
      if (netidx == net->layer_size()) {
        continue;
      }
    }
    {
      const caffe::LayerParameter& param = layer;
      DefEnum2(phase, Phase, Phase_Name);
    }
    if (layer.include_size()>0) {
      const caffe::NetStateRule& param = layer.include(0);
      DefEnum2(phase, Phase, Phase_Name);
      //::caffe::Phase phase = param.phase();
      DefNumber(min_level);
      DefNumber(max_level);
    }
    if (layer.has_transform_param()) {
      const caffe::TransformationParameter& param = layer.transform_param();
      cjson * json_transform = cjson_gcObjectItem(json_layer, "transform");
      {
        cjson * json_layer = json_transform;
        DefNumber(scale);
        DefBool(mirror);
        DefNumber(crop_size);
        DefString(mean_file);
        DefNumberArray(mean_value);
        DefBool(force_color);
        DefBool(force_gray);
        DefBool(add_noise);
        DefNumber(noise_ratio);
        DefNumberArray(scale_factors);
        DefNumber(crop_width);
        DefNumber(crop_height);
      }
    }

    // layer specific params
    if (layer_type == "Data") {
      const caffe::DataParameter& param = layer.data_param();
      DefString(source);
      DefNumber(batch_size);
      DefEnum(backend, DB);
    }
    else if (layer_type == "BatchNorm")
    {
      const caffe::BatchNormParameter& param = layer.batch_norm_param();
      //const caffe::BlobProto& mean_blob = binlayer.blobs(0);
      //const caffe::BlobProto& var_blob = binlayer.blobs(1);
      //cjson_AddNumberToObject(json_layer, " %d", (int)mean_blob.data_size());

      //const caffe::BatchNormParameter& batch_norm_param = layer.batch_norm_param();
      //float eps = batch_norm_param.eps();

      //std::vector<float> ones(mean_blob.data_size(), 1.f);
      //fwrite(ones.data(), sizeof(float), ones.size(), bp);// slope
    }
    else if (layer_type == "Concat")
    {
      const caffe::ConcatParameter& param = layer.concat_param();
      int dim = param.axis() - 1;
      //cjson_AddNumberToObject(json_layer, " axis=%d", dim);
    }
    else if (layer_type == "Convolution")
    {
      const caffe::ConvolutionParameter& param = layer.convolution_param();
      DefNumber(num_output);
      DefNumber(axis);
      DefNumberArray(kernel_size);
      DefNumberArray(dilation);
      DefNumberArray(stride);
      DefNumberArray(pad);
      DefBool(bias_term);
      DefBool(group);
      DefBool(force_nd_im2col);
      FillerParameter(json_layer, "weight_filler", param.weight_filler());
      FillerParameter(json_layer, "bias_filler", param.bias_filler());
    }
    else if (layer_type == "Crop")
    {
      const caffe::CropParameter& param = layer.crop_param();
      DefNumber(axis);
      DefNumberArray(offset);
    }
    else if (layer_type == "Deconvolution")
    {
      const caffe::ConvolutionParameter& param = layer.convolution_param();
      DefNumber(num_output);
      DefNumberArray(kernel_size);
      DefNumberArray(dilation);
      DefNumberArray(stride);
      DefNumberArray(pad);
      DefBool(bias_term);
    }
    else if (layer_type == "DenseBlock") {
      const caffe::DenseBlockParameter& param = layer.denseblock_param();
      DefNumber(numtransition);
      DefNumber(initchannel);
      DefNumber(growthrate);
      DefBool(use_dropout);
      DefNumber(dropout_amount);
      DefNumber(pad_h);
      DefNumber(pad_w);
      DefNumber(conv_verticalstride);
      DefNumber(conv_horizentalstride);
      DefNumber(moving_average_fraction);
      DefNumber(filter_h);
      DefNumber(filter_w);
      DefNumber(workspace_mb);
      DefBool(use_dropout);
      DefBool(use_bc);
      DefBool(bc_ultra_space_efficient);
      FillerParameter(json_layer, "Filter_Filler", param.filter_filler());
      FillerParameter(json_layer, "BN_Scaler_Filler", param.bn_scaler_filler());
      FillerParameter(json_layer, "BN_Bias_Filler", param.bn_bias_filler());
    }
    else if (layer_type == "Dropout")
    {
      const caffe::DropoutParameter& param = layer.dropout_param();
      DefNumber(dropout_ratio);
    }
    else if (layer_type == "InnerProduct")
    {
      const caffe::InnerProductParameter& param = layer.inner_product_param();
      DefNumber(axis);
      DefNumber(num_output);
      DefBool(transpose);
      DefBool(bias_term);
    }
    else if (layer_type == "Pooling")
    {
      //char* poolname[] = { "MAX","AVE", "STOCHASTIC","DEF", "DEF_ALL", "DEF_ALL2", "DEF_ALL3", "DEF_ALL4", "LOWRES" };
      const caffe::PoolingParameter& param = layer.pooling_param();
      //std::string pool_name = param.PoolMethod_Name(param.pool());
      DefEnum(pool, PoolMethod);
      //cjson_AddStringToObject(json_layer, "pool", pool_name.c_str());
      DefNumber(kernel_size);
      DefNumber(kernel_h);
      DefNumber(kernel_w);
      DefNumber(stride);
      DefNumber(stride_h);
      DefNumber(stride_w);
      DefNumber(pad);
      DefNumber(pad_h);
      DefNumber(pad_w);
      DefNumber(pad);
      DefBool(global_pooling);
    }
    else if (layer_type == "Power")
    {
      const caffe::PowerParameter& param = layer.power_param();
      DefNumber(power);
      DefNumber(scale);
      DefNumber(shift);
    }
    else if (layer_type == "PReLU")
    {
      const caffe::PReLUParameter& param = layer.prelu_param();
      //fwrite(slope_blob.data().data(), sizeof(float), slope_blob.data_size(), bp);
      DefBool(channel_shared);
    }
    else if (layer_type == "ReLU")
    {
      const caffe::ReLUParameter& param = layer.relu_param();
      DefNumber(negative_slope);
    }
    else if (layer_type == "Reshape")
    {
      const caffe::ReshapeParameter& param = layer.reshape_param();
      const caffe::BlobShape& bs = param.shape();
      if (param.has_shape()) {
        CJSON_ADDNUMBERARRAYTOOBJECT(json_layer, "shape", bs.dim());
      }
      DefNumber(axis);
      DefNumber(num_axes);
    }
    else if (layer_type == "Scale")
    {
      const caffe::ScaleParameter& param = layer.scale_param();
      DefNumber(axis);
      DefBool(bias_term);
    }
    else if (layer_type == "Slice")
    {
      const caffe::SliceParameter& param = layer.slice_param();
      DefNumber(axis);
      DefNumber(slice_dim);
    }
    else if (layer_type == "Softmax")
    {
      const caffe::SoftmaxParameter& param = layer.softmax_param();
      DefNumber(axis);
    }
    else if (layer_type == "Threshold")
    {
      const caffe::ThresholdParameter& param = layer.threshold_param();
      DefNumber(threshold);
    }
    else if (layer_type == "Transpose")
    {
      const caffe::TransposeParameter& param = layer.transpose_param();
      DefNumberArray(dim);
    }
    else {
      printf("%s %s\n", layer_type.c_str(), layer.name().c_str());
    }

    if (layer.param_size()>0) {
      cjson * json_blobs = cjson_gcObjectItem(json_layer, "blobs", cjson_Array);
      for (int i = 0; i<layer.param_size(); ++i) {
        cjson * json_blob = cjson_gcArrayItem(json_blobs, i);
        ParamSpec(json_blob, layer.param(i));
      }
    }
    if (net) {
      const caffe::LayerParameter& binlayer = net->layer(netidx);
      if (binlayer.blobs_size() > 0) {
        char* buf = 0;
        int len = 0;
        cjson * json_blobs = cjson_gcObjectItem(json_layer, "blobs", cjson_Array);
        for (int i = 0; i < binlayer.blobs_size(); ++i) {
          cjson * json_blob = cjson_gcArrayItem(json_blobs, i);
          const caffe::BlobProto& blob = binlayer.blobs(i);
          CJSON_ADDNUMBERARRAYTOOBJECT(json_blob, "shape", blob.shape().dim());
          if (0) {
            int size = blob.data_size() * sizeof(float);
            buf = (char*)realloc(buf, size * 3);
            len = 0;
            base64_encode((char*)blob.data().data(), size, buf, &len);
            buf[len] = 0;
            cjson_AddStringToObject(json_blob, "data", buf);
          }
          if (1) {
            cjson_AddBinaryDataToObject(json_blob, "data", blob.data().data(), blob.data_size() * sizeof(float));
          }
          if (0) {
            cjson_AddNumberArrayToObject(json_blob, "data", blob.data().data(), blob.data_size());
          }
          //fwrite(blob.data().data(), sizeof(float), blob.data_size(), bp);
          //cjson_AddItemToArray(json_blobs, json_blob);
        }
        free(buf);
      }
    }
  }
  cjson_AddItemToObject(json_root, "layers", json_layers);
  //printf("%s\n", cjson_Print(json_root));
  cjson_SaveFile(jsonoutfile, json_root);
  //fclose(bp);
  cjson_Delete(root);
  return 0;
}

#undef DefBool
#undef DefNumber
#undef DefString
#undef DefEnum
#undef DefEnum2
#undef DefNumberArray
#undef DefStringArray


#define TESTCALL(fun, args)    fun(countof(args), args)
int test_caffe2json() {
  if (0) {
    _chdir("C:/caffe_train");
    char* test[] = { "",
      "lenet_train_test.prototxt","lenet_iter_0_.caffemodel","lenet_solver.prototxt","mnist/lenet->json", "0"
    };
    TESTCALL(caffe2json, test);
  }
  if (0) {
    _chdir("E:/OCR_Line/model/densenet-no-blstm/");
    char* test[] = { "",
      "deploy.prototxt","model.caffemodel","solver.prototxt","model.json", "0"
    };
    TESTCALL(caffe2json, test);
  }
  if (0) {
    _chdir("E:/OCR_Line/lines/densenet-no-blstm");
    char* test[] = { "",
      "train-val.prototxt", NULL, "solver.prototxt","train-val.json", "0"
    };
    TESTCALL(caffe2json, test);
  }
  if (1) {
    _chdir("D:/OCR_Line/lines/han200w/densenet-no-blstm_caffe5");
    char* test[] = { "",
      "train.prototxt", "model.caffemodel", "solver.prototxt","train.json", "0"
    };
    TESTCALL(caffe2json, test);
  }
  return 0;
}
#undef TESTCALL


