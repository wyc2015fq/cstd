
#ifndef _LIBCAFFE_CPP_
#define _LIBCAFFE_CPP_

#include "cnn_config.h"

#pragma warning(disable:4267)
#pragma warning(disable:4244)
#pragma warning(disable:4661)
//#include "proto/caffe_proto.h"
//#include "proto/caffe_proto.cc"
#include "wstd/logging.hpp"
#include "blob.hpp"
//#include "internal_thread.hpp"
//#include "data_reader.hpp"
//#include "data_transformer.hpp"
//#include "layer.hpp"
//#include "net.hpp"
#if 0
#include "parallel.cpp"
#include "solver.cpp"
#include "adadelta_solver.cpp"
#include "adagrad_solver.cpp"
#include "adam_solver.cpp"
#include "nesterov_solver.cpp"
#include "rmsprop_solver.cpp"
#include "sgd_solver.cpp"
#include "syncedmem.cpp"
#include "benchmark.cpp"
#include "blocking_queue.cpp"
#include "db.cpp"
#include "db_lmdb.cpp"
#include "hdf5.cpp"
#include "im2col.cpp"
#include "insert_splits.cpp"
#include "io.cpp"
#include "math_functions.cpp"
#include "signal_handler.cpp"
#include "upgrade_proto.hpp"

#include "interp.cpp"
#include "cudnn.cpp"
#endif

#if 0


#include "layers\base_conv_layer.cpp"

#include "layers\conv_layer.cpp"
#include "layers\DenseBlock_layer.cpp"

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
//#include "layers\image_data_layer.cpp"
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
#if 0
  template <typename T>	T const volatile* get_pointer(T const volatile* p) { return p; }
#define GET_POINTER_DEF(T) 	template <>	T* get_pointer<T>(T * p) { return p; }
  //#define GET_POINTER_DEF(T) 	T* get_pointer(T * p) { return p; }
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
#ifdef USE_PRO
  GET_POINTER_DEF(class caffe::SolverParameter const volatile);
  GET_POINTER_DEF(class caffe::LayerParameter const volatile);
#endif
#undef GET_POINTER_DEF
#endif
}


//#include "tools\caffe.cpp"

#endif // _CAFFE_INL_
