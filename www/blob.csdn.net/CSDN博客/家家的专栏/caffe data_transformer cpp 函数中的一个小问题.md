# caffe data_transformer.cpp 函数中的一个小问题 - 家家的专栏 - CSDN博客





2015年03月05日 15:40:56[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：3642








在训练情况下，测试的时候，因为phase变量没有及时更新。

导致在test的情况下出错。。。

直接更新下  this->phase_ = Caffe::phase();就好了。





//  LOG(INFO) << "MODEL:" << Caffe::phase();

//  LOG(INFO) << "phase:" << phase_;

  this->phase_ = Caffe::phase();

  if (crop_size) {

    CHECK(data.size()) << "Image cropping only support uint8 data";

    int h_off, w_off;

    // We only do random crop when we do training.

    //LOG(INFO) << "MODEL:" << phase_;

    if (phase_ == Caffe::TRAIN) {

//    if (Caffe::phase() == 0) {

//      LOG(INFO) <<"TrainING MODEL!";

      h_off = Rand() % (height - crop_size);

      w_off = Rand() % (width - crop_size);

    } else {

//      LOG(INFO) <<"TESTING MODEL!";

      h_off = (height - crop_size) / 2;

      w_off = (width - crop_size) / 2;

    }







请教了高人。可能这么修改不太合适。。

来个正确的。。

在solver.cpp中，填加了两句来着，修改如下：

void Solver<Dtype>::Init(const SolverParameter& param) {

 32   LOG(INFO) << "Initializing solver from parameters: " << std::endl

 33             << param.DebugString();

 34   param_ = param;

 35   if (param_.random_seed() >= 0) {

 36     Caffe::set_random_seed(param_.random_seed());

 37   }

 38   // Scaffolding code
 39   Caffe::set_phase(Caffe::TRAIN);
 40   InitTrainNet();
 41   Caffe::set_phase(Caffe::TEST);
 42   InitTestNets();

 43   LOG(INFO) << "Solver scaffolding done.";

 44 }




