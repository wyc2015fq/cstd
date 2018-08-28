
#ifndef _LIBCAFFE_CPP_
#define _LIBCAFFE_CPP_

#include <boost/get_pointer.hpp>
//#define GET_POINTER_DWA20021219_HPP
namespace boost
{
  //template <typename T>	T * get_pointer(T * p) { return p; }
}


#include "caffe_config.h"


// opencv_world310d.lib; libjpeg.lib; libpng.lib; zlib.lib; cudart.lib; cublas.lib; curand.lib; gflagsd.lib; libopenblas.dll.a; libprotobufd.lib; libprotoc.lib; leveldbd.lib; lmdbd.lib; hdf5.lib; hdf5_hl.lib; Shlwapi.lib; comctl32.lib; cudnn.lib; glogd.lib; ntdll.lib
// opencv_world310.lib;libjpeg.lib;libpng.lib;zlib.lib;cudart.lib;cublas.lib;curand.lib;gflags.lib;libopenblas.dll.a;libprotobuf.lib;libprotoc.lib;leveldb.lib;lmdbd.lib;hdf5.lib;hdf5_hl.lib;Shlwapi.lib;comctl32.lib;cudnn.lib;glog.lib;ntdll.lib

#pragma comment(lib,"cudart.lib")
#ifdef USE_CUDNN
#pragma comment(lib,"cublas.lib")
#pragma comment(lib,"curand.lib")
#pragma comment(lib,"cudnn.lib")
#endif
#ifdef _DEBUG
#pragma comment(lib,"libprotobufd.lib")
#pragma comment(lib,"libprotocd.lib")
//#pragma comment(lib,"gflagsd.lib")
//#pragma comment(lib,"glogd.lib")
#pragma comment(lib,"leveldbd.lib")
#pragma comment(lib,"lmdbd.lib")
#pragma comment(lib,"hdf5.lib")
#pragma comment(lib,"hdf5_hl.lib")
#pragma comment(lib,"libopenblas.dll.a")
#pragma comment(lib,"opencv_world310d.lib")
#else
#pragma comment(lib,"libprotobuf.lib")
#pragma comment(lib,"libprotoc.lib")
//#pragma comment(lib,"gflags.lib")
//#pragma comment(lib,"glog.lib")
#pragma comment(lib,"leveldb.lib")
#pragma comment(lib,"lmdb.lib")
#pragma comment(lib,"hdf5.lib")
#pragma comment(lib,"hdf5_hl.lib")
#pragma comment(lib,"libopenblas.dll.a")
#pragma comment(lib,"opencv_world310.lib")
#endif


#pragma warning(disable:4267)
#pragma warning(disable:4244)
#pragma warning(disable:4661)
//#include "proto/caffe.pb.h"
//#include "proto/caffe.pb.cc"
#include "caffe/util/logging.hpp"
#include "caffe/util/flags.hpp"
#include "..\caffe\blob.cpp"
#include "..\caffe\common.cpp"
#include "..\caffe\data_reader.cpp"
#include "..\caffe\data_transformer.cpp"
#include "..\caffe\internal_thread.cpp"
#include "..\caffe\layer.cpp"
#include "..\caffe\layer_factory.cpp"
#include "..\caffe\net.cpp"
#include "..\caffe\parallel.cpp"
#include "..\caffe\proto\caffe.pb.cc"
#include "..\caffe\solver.cpp"
#include "..\caffe\solvers\adadelta_solver.cpp"
#include "..\caffe\solvers\adagrad_solver.cpp"
#include "..\caffe\solvers\adam_solver.cpp"
#include "..\caffe\solvers\nesterov_solver.cpp"
#include "..\caffe\solvers\rmsprop_solver.cpp"
#include "..\caffe\solvers\sgd_solver.cpp"
#include "..\caffe\syncedmem.cpp"
#include "..\caffe\util\benchmark.cpp"
#include "..\caffe\util\blocking_queue.cpp"
#include "..\caffe\util\db.cpp"
#include "..\caffe\util\db_leveldb.cpp"
#include "..\caffe\util\db_lmdb.cpp"
#include "..\caffe\util\hdf5.cpp"
#include "..\caffe\util\im2col.cpp"
#include "..\caffe\util\insert_splits.cpp"
#include "..\caffe\util\io.cpp"
#include "..\caffe\util\math_functions.cpp"
#include "..\caffe\util\signal_handler.cpp"
#include "..\caffe\util\upgrade_proto.cpp"

#include "..\caffe\util\interp.cpp"
#include "..\caffe\util\cudnn.cpp"


#include "..\caffe\layers\absval_layer.cpp"
#include "..\caffe\layers\accuracy_layer.cpp"
#include "..\caffe\layers\argmax_layer.cpp"
#include "..\caffe\layers\base_conv_layer.cpp"
#include "..\caffe\layers\base_data_layer.cpp"
#include "..\caffe\layers\batch_norm_layer.cpp"
#include "..\caffe\layers\batch_reindex_layer.cpp"
#include "..\caffe\layers\bias_layer.cpp"
#include "..\caffe\layers\bnll_layer.cpp"
#include "..\caffe\layers\concat_layer.cpp"
#include "..\caffe\layers\contrastive_loss_layer.cpp"
#include "..\caffe\layers\conv_layer.cpp"
#include "..\caffe\layers\crop_layer.cpp"
#include "..\caffe\layers\ctcpp_entrypoint.cpp"
#include "..\caffe\layers\ctc_decoder_layer.cpp"
#include "..\caffe\layers\cudnn_conv_layer.cpp"
#include "..\caffe\layers\cudnn_lcn_layer.cpp"
#include "..\caffe\layers\cudnn_lrn_layer.cpp"
#include "..\caffe\layers\cudnn_pooling_layer.cpp"
#include "..\caffe\layers\cudnn_relu_layer.cpp"
#include "..\caffe\layers\cudnn_sigmoid_layer.cpp"
#include "..\caffe\layers\cudnn_softmax_layer.cpp"
#include "..\caffe\layers\cudnn_tanh_layer.cpp"
#include "..\caffe\layers\data_layer.cpp"
#include "..\caffe\layers\deconv_layer.cpp"
#include "..\caffe\layers\DenseBlock_layer.cpp"
#include "..\caffe\layers\dropout_layer.cpp"
#include "..\caffe\layers\dummy_data_layer.cpp"
#include "..\caffe\layers\eltwise_layer.cpp"
#include "..\caffe\layers\elu_layer.cpp"
#include "..\caffe\layers\embed_layer.cpp"
#include "..\caffe\layers\euclidean_loss_layer.cpp"
#include "..\caffe\layers\exp_layer.cpp"
#include "..\caffe\layers\filter_layer.cpp"
#include "..\caffe\layers\flatten_layer.cpp"
#include "..\caffe\layers\hdf5_data_layer.cpp"
#include "..\caffe\layers\hdf5_output_layer.cpp"
#include "..\caffe\layers\hinge_loss_layer.cpp"
#include "..\caffe\layers\im2col_layer.cpp"
#include "..\caffe\layers\image_data_layer.cpp"
#include "..\caffe\layers\infogain_loss_layer.cpp"
#include "..\caffe\layers\inner_product_layer.cpp"
#include "..\caffe\layers\input_layer.cpp"
#include "..\caffe\layers\interp_layer.cpp"
#include "..\caffe\layers\log_layer.cpp"
#include "..\caffe\layers\loss_layer.cpp"
#include "..\caffe\layers\lrn_layer.cpp"
#include "..\caffe\layers\lstm_layer.cpp"
#include "..\caffe\layers\lstm_layer_Junhyuk.cpp"
#include "..\caffe\layers\lstm_unit_layer.cpp"
#include "..\caffe\layers\multinomial_logistic_loss_layer.cpp"
#include "..\caffe\layers\mvn_layer.cpp"
#include "..\caffe\layers\neuron_layer.cpp"
#include "..\caffe\layers\parameter_layer.cpp"
#include "..\caffe\layers\pooling_layer.cpp"
#include "..\caffe\layers\power_layer.cpp"
#include "..\caffe\layers\prelu_layer.cpp"
#include "..\caffe\layers\recurrent_layer.cpp"
#include "..\caffe\layers\reduction_layer.cpp"
#include "..\caffe\layers\relu_layer.cpp"
#include "..\caffe\layers\reshape_layer.cpp"
#include "..\caffe\layers\reverse_layer.cpp"
#include "..\caffe\layers\reverse_time_layer.cpp"
#include "..\caffe\layers\rnn_layer.cpp"
#include "..\caffe\layers\scale_layer.cpp"
#include "..\caffe\layers\sigmoid_cross_entropy_loss_layer.cpp"
#include "..\caffe\layers\sigmoid_layer.cpp"
#include "..\caffe\layers\silence_layer.cpp"
#include "..\caffe\layers\slice_layer.cpp"
#include "..\caffe\layers\softmax_layer.cpp"
#include "..\caffe\layers\softmax_loss_layer.cpp"
#include "..\caffe\layers\softmax_loss_layer_multi_label.cpp"
#include "..\caffe\layers\split_layer.cpp"
#include "..\caffe\layers\spp_layer.cpp"
#include "..\caffe\layers\tanh_layer.cpp"
#include "..\caffe\layers\threshold_layer.cpp"
#include "..\caffe\layers\tile_layer.cpp"
#include "..\caffe\layers\transpose_layer.cpp"
#include "..\caffe\layers\warp_ctc_loss_layer.cpp"
#include "..\caffe\layers\window_data_layer.cpp"

//using namespace std;
#include "..\caffe\layers\memory_data_layer.cpp"

#if 0
#include "..\caffe\blob.hpp"
#include "..\caffe\caffe.hpp"
#include "..\caffe\common.hpp"
#include "..\caffe\data_reader.hpp"
#include "..\caffe\data_transformer.hpp"
#include "..\caffe\filler.hpp"
#include "..\caffe\internal_thread.hpp"
#include "..\caffe\layer.hpp"
#include "..\caffe\layers\absval_layer.hpp"
#include "..\caffe\layers\accuracy_layer.hpp"
#include "..\caffe\layers\argmax_layer.hpp"
#include "..\caffe\layers\base_conv_layer.hpp"
#include "..\caffe\layers\base_data_layer.hpp"
#include "..\caffe\layers\batch_norm_layer.hpp"
#include "..\caffe\layers\batch_reindex_layer.hpp"
#include "..\caffe\layers\bias_layer.hpp"
#include "..\caffe\layers\bnll_layer.hpp"
#include "..\caffe\layers\concat_layer.hpp"
#include "..\caffe\layers\contrastive_loss_layer.hpp"
#include "..\caffe\layers\conv_layer.hpp"
#include "..\caffe\layers\crop_layer.hpp"
#include "..\caffe\layers\cudnn_conv_layer.hpp"
#include "..\caffe\layers\cudnn_lcn_layer.hpp"
#include "..\caffe\layers\cudnn_lrn_layer.hpp"
#include "..\caffe\layers\cudnn_pooling_layer.hpp"
#include "..\caffe\layers\cudnn_relu_layer.hpp"
#include "..\caffe\layers\cudnn_sigmoid_layer.hpp"
#include "..\caffe\layers\cudnn_softmax_layer.hpp"
#include "..\caffe\layers\cudnn_tanh_layer.hpp"
#include "..\caffe\layers\data_layer.hpp"
#include "..\caffe\layers\deconv_layer.hpp"
#include "..\caffe\layers\dropout_layer.hpp"
#include "..\caffe\layers\dummy_data_layer.hpp"
#include "..\caffe\layers\eltwise_layer.hpp"
#include "..\caffe\layers\elu_layer.hpp"
#include "..\caffe\layers\embed_layer.hpp"
#include "..\caffe\layers\euclidean_loss_layer.hpp"
#include "..\caffe\layers\exp_layer.hpp"
#include "..\caffe\layers\filter_layer.hpp"
#include "..\caffe\layers\flatten_layer.hpp"
#include "..\caffe\layers\group_image_data_layer.hpp"
#include "..\caffe\layers\hdf5_data_layer.hpp"
#include "..\caffe\layers\hdf5_output_layer.hpp"
#include "..\caffe\layers\hinge_loss_layer.hpp"
#include "..\caffe\layers\im2col_layer.hpp"
#include "..\caffe\layers\image_data_layer.hpp"
#include "..\caffe\layers\infogain_loss_layer.hpp"
#include "..\caffe\layers\inner_product_layer.hpp"
#include "..\caffe\layers\input_layer.hpp"
#include "..\caffe\layers\log_layer.hpp"
#include "..\caffe\layers\loss_layer.hpp"
#include "..\caffe\layers\lrn_layer.hpp"
#include "..\caffe\layers\lstm_layer.hpp"
#include "..\caffe\layers\multinomial_logistic_loss_layer.hpp"
#include "..\caffe\layers\mvn_layer.hpp"
#include "..\caffe\layers\neuron_layer.hpp"
#include "..\caffe\layers\parameter_layer.hpp"
#include "..\caffe\layers\pooling_layer.hpp"
#include "..\caffe\layers\power_layer.hpp"
#include "..\caffe\layers\prelu_layer.hpp"
#include "..\caffe\layers\recurrent_layer.hpp"
#include "..\caffe\layers\reduction_layer.hpp"
#include "..\caffe\layers\relu_layer.hpp"
#include "..\caffe\layers\reshape_layer.hpp"
#include "..\caffe\layers\rnn_layer.hpp"
#include "..\caffe\layers\scale_layer.hpp"
#include "..\caffe\layers\sigmoid_cross_entropy_loss_layer.hpp"
#include "..\caffe\layers\sigmoid_layer.hpp"
#include "..\caffe\layers\silence_layer.hpp"
#include "..\caffe\layers\slice_layer.hpp"
#include "..\caffe\layers\softmax_layer.hpp"
#include "..\caffe\layers\softmax_loss_layer.hpp"
#include "..\caffe\layers\split_layer.hpp"
#include "..\caffe\layers\spp_layer.hpp"
#include "..\caffe\layers\tanh_layer.hpp"
#include "..\caffe\layers\threshold_layer.hpp"
#include "..\caffe\layers\tile_layer.hpp"
#include "..\caffe\layers\window_data_layer.hpp"
#include "..\caffe\layer_factory.hpp"
#include "..\caffe\net.hpp"
#include "..\caffe\parallel.hpp"
#include "..\caffe\proto\caffe.pb.h"
#include "..\caffe\syncedmem.hpp"
#include "..\caffe\util\db.hpp"
#include "..\caffe\util\db_lmdb.hpp"
#include "..\caffe\util\device_alternate.hpp"
#include "..\caffe\util\math_functions.hpp"
#include "..\caffe\proto\caffe.pb.h"


//Import Project="$(VCTargetsPath)\Microsoft.Cpp.targets"
//ImportGroup Label="ExtensionTargets"
//Import Project="$(VCTargetsPath)\BuildCustomizations\CUDA 9.2.targets"

#endif

#ifdef WITH_PYTHON_LAYER
#include "..\caffe\layers\python_layer.hpp"
#endif
#include "..\caffe\sgd_solvers.hpp"
#include "..\caffe\solver.hpp"
#include "..\caffe\solver_factory.hpp"


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
#undef GET_POINTER_DEF
#endif
}


//#include "..\caffe\tools\caffe.cpp"

#endif // _CAFFE_INL_
