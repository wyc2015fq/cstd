
#include "proto.h"
#include "solver.hpp"
#include "SGDSolver.hpp"

void Lock() {}
void Unlock() {}
void InitMutex() {}

template <typename Dtype>
void ClearParamDiffs(Blob<Dtype>** learnable_params_, int learnable_params_size)
{
  for (int i = 0; i < learnable_params_size; ++i) {
    BlobF* blob = learnable_params_[i];
    caffe_set<Dtype>(CONTEXT, blob->count(), static_cast<Dtype>(0), blob->mutable_cpu_diff());
  }
}

int test_cnn4() {
  const char* fn;
  fn = "E:/OCR_Line/model/densenet-no-blstm/densenet-no-blstm.json";
  fn = "C:/caffe_train/lenet.json";
  CJSON* root = cJSON_OpenFile(fn);
  typedef float Dtype;
  typedef Net<Dtype> NetF;
  NetF* net = new NetF;
  //caffe_copy<float>((GPUContext*)0, 0, 0, 0);
  //caffe_set<float>((GPUContext*)0, 0, 0, 0);
  if (net->FromProto(root)) {
    Solver<Dtype>* solver = new SGDSolver<Dtype>();
    solver->init(net);
    solver->Solve();
    delete solver;
    net->Free();
    //ClearParamDiffs(net->layers_->blobs_, 1);
  }
  cJSON_Delete(root);
  delete net;
  return 0;
}
