
#ifndef _LIBCAFFE_CPP_
#define _LIBCAFFE_CPP_

#include "caffe_config.h"


// opencv_world310d.lib; libjpeg.lib; libpng.lib; zlib.lib; cudart.lib; cublas.lib; curand.lib; gflagsd.lib; libopenblas.dll.a; libprotobufd.lib; libprotoc.lib; leveldbd.lib; lmdbd.lib; hdf5.lib; hdf5_hl.lib; Shlwapi.lib; comctl32.lib; cudnn.lib; glogd.lib; ntdll.lib
// opencv_world310.lib;libjpeg.lib;libpng.lib;zlib.lib;cudart.lib;cublas.lib;curand.lib;gflags.lib;libopenblas.dll.a;libprotobuf.lib;libprotoc.lib;leveldb.lib;lmdbd.lib;hdf5.lib;hdf5_hl.lib;Shlwapi.lib;comctl32.lib;cudnn.lib;glog.lib;ntdll.lib

#pragma comment(lib,"cudart.lib")
#ifdef USE_CUDNN
#pragma comment(lib,"cublas.lib")
#pragma comment(lib,"curand.lib")
#pragma comment(lib,"cudnn.lib")
#endif

#pragma warning(disable:4267)
#pragma warning(disable:4244)
#pragma warning(disable:4661)
//#include "proto/caffe.pb.h"
//#include "proto/caffe.pb.cc"
#include "wstd/logging.hpp"
#include "blob.cpp"
#include "common.cpp"
#include "data_reader.cpp"
#include "data_transformer.cpp"
#include "internal_thread.cpp"
#include "layer.cpp"
#include "layer_factory.cpp"
#include "net.cpp"
#include "parallel.cpp"
#undef STRICT
#undef min
#include "proto\caffe.pb.cc"
#include "solver.cpp"
#include "solvers\adadelta_solver.cpp"
#include "solvers\adagrad_solver.cpp"
#include "solvers\adam_solver.cpp"
#include "solvers\nesterov_solver.cpp"
#include "solvers\rmsprop_solver.cpp"
#include "solvers\sgd_solver.cpp"
#include "syncedmem.cpp"
#include "util\benchmark.cpp"
#include "util\blocking_queue.cpp"
#include "util\db.cpp"
#include "util\db_lmdb.cpp"
//#include "util\hdf5.cpp"
#include "util\im2col.cpp"
#include "util\insert_splits.cpp"
#include "util\io.cpp"
#include "util\math_functions.cpp"
#include "util\signal_handler.cpp"
#include "util\upgrade_proto.cpp"

#include "util\interp.cpp"
#include "util\cudnn.cpp"

#include "layers\base_conv_layer.cpp"
#include "layers\conv_layer.cpp"
#include "layers\DenseBlock_layer.cpp"

#if 1
//#include "layers\mtcnn_layer.hpp"
#include "layers\absval_layer.cpp"
#include "layers\accuracy_layer.cpp"
#include "layers\argmax_layer.cpp"
#include "layers\base_data_layer.cpp"
#include "layers\batch_norm_layer.cpp"
#include "layers\batch_reindex_layer.cpp"
#include "layers\bias_layer.cpp"
#include "layers\bnll_layer.cpp"
#include "layers\concat_layer.cpp"
#include "layers\contrastive_loss_layer.cpp"
#include "layers\crop_layer.cpp"
#include "layers\ctcpp_entrypoint.cpp"
#include "layers\ctc_decoder_layer.cpp"
#include "layers\cudnn_conv_layer.cpp"
#include "layers\cudnn_lcn_layer.cpp"
#include "layers\cudnn_lrn_layer.cpp"
#include "layers\cudnn_pooling_layer.cpp"
#include "layers\cudnn_relu_layer.cpp"
#include "layers\cudnn_sigmoid_layer.cpp"
#include "layers\cudnn_softmax_layer.cpp"
#include "layers\cudnn_tanh_layer.cpp"
#include "layers\data_layer.cpp"
#include "layers\mnist_data_layer.cpp"
#include "layers\deconv_layer.cpp"
#include "layers\dropout_layer.cpp"
#include "layers\dummy_data_layer.cpp"
#include "layers\eltwise_layer.cpp"
#include "layers\elu_layer.cpp"
#include "layers\embed_layer.cpp"
#include "layers\euclidean_loss_layer.cpp"
#include "layers\exp_layer.cpp"
#include "layers\filter_layer.cpp"
#include "layers\flatten_layer.cpp"
//#include "layers\hdf5_data_layer.cpp"
//#include "layers\hdf5_output_layer.cpp"
#include "layers\hinge_loss_layer.cpp"
#include "layers\im2col_layer.cpp"
#include "layers\image_data_layer.cpp"
#include "layers\infogain_loss_layer.cpp"
#include "layers\inner_product_layer.cpp"
#include "layers\input_layer.cpp"
#include "layers\interp_layer.cpp"
#include "layers\log_layer.cpp"
#include "layers\loss_layer.cpp"
#include "layers\lrn_layer.cpp"
#include "layers\lstm_layer.cpp"
#include "layers\lstm_layer_Junhyuk.cpp"
#include "layers\lstm_unit_layer.cpp"
#include "layers\multinomial_logistic_loss_layer.cpp"
#include "layers\mvn_layer.cpp"
#include "layers\neuron_layer.cpp"
#include "layers\parameter_layer.cpp"
#include "layers\pooling_layer.cpp"
#include "layers\power_layer.cpp"
#include "layers\prelu_layer.cpp"
#include "layers\recurrent_layer.cpp"
#include "layers\reduction_layer.cpp"
#include "layers\relu_layer.cpp"
#include "layers\reshape_layer.cpp"
#include "layers\reverse_layer.cpp"
#include "layers\reverse_time_layer.cpp"
#include "layers\rnn_layer.cpp"
#include "layers\scale_layer.cpp"
#include "layers\sigmoid_cross_entropy_loss_layer.cpp"
#include "layers\sigmoid_layer.cpp"
#include "layers\silence_layer.cpp"
#include "layers\slice_layer.cpp"
#include "layers\softmax_layer.cpp"
#include "layers\softmax_loss_layer.cpp"
#include "layers\softmax_loss_layer_multi_label.cpp"
#include "layers\split_layer.cpp"
#include "layers\spp_layer.cpp"
#include "layers\tanh_layer.cpp"
#include "layers\threshold_layer.cpp"
#include "layers\tile_layer.cpp"
#include "layers\transpose_layer.cpp"
#include "layers\warp_ctc_loss_layer.cpp"
//#include "layers\window_data_layer.cpp"

//using namespace std;
//#include "layers\memory_data_layer.cpp"
#endif


#ifdef WITH_PYTHON_LAYER
#include "layers\python_layer.hpp"
#endif
#include "sgd_solvers.hpp"
#include "solver.hpp"
#include "solver_factory.hpp"


namespace boost
{
#if 1
  template <typename T>	T const volatile* get_pointer(T const volatile* p) { return p; }
#define GET_POINTER_DEF(T) 	template <>	T* get_pointer<T>(T * p) { return p; }
  //#define GET_POINTER_DEF(T) 	T* get_pointer(T * p) { return p; }
  GET_POINTER_DEF(class caffe::SolverParameter const volatile);
  GET_POINTER_DEF(class caffe::Net<float> const volatile);
  GET_POINTER_DEF(class caffe::Timer const volatile);
  GET_POINTER_DEF(class caffe::AdamSolver<float> const volatile);
  GET_POINTER_DEF(class caffe::AdaDeltaSolver<float> const volatile);
  GET_POINTER_DEF(class caffe::RMSPropSolver<float> const volatile);
  GET_POINTER_DEF(class caffe::NesterovSolver<float> const volatile);
  GET_POINTER_DEF(class caffe::SGDSolver<float> const volatile);
  GET_POINTER_DEF(class caffe::AdaGradSolver<float> const volatile);
  GET_POINTER_DEF(class caffe::Layer<float> const volatile);
  GET_POINTER_DEF(class caffe::Solver<float> const volatile);
  GET_POINTER_DEF(class caffe::LayerParameter const volatile);
#undef GET_POINTER_DEF
#endif
}


//#include "tools\caffe.cpp"

#endif // _CAFFE_INL_
