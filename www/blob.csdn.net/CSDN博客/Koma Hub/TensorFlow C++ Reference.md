# TensorFlow C++ Reference - Koma Hub - CSDN博客
2019年03月27日 22:23:44[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：56
**Table of Contents**
[array_ops](#array_ops%C2%A0)
[Members](#Members)
[candidate_sampling_ops](#candidate_sampling_ops%C2%A0)
[Members](#Members)
[control_flow_ops](#control_flow_ops%C2%A0)
[Members](#Members)
[core](#core%C2%A0)
[Members](#Members)
[data_flow_ops](#data_flow_ops%C2%A0)
[Members](#Members)
[image_ops](#image_ops%C2%A0)
[Members](#Members)
[io_ops](#io_ops%C2%A0)
[Members](#Members)
[logging_ops](#logging_ops%C2%A0)
[Members](#Members)
[math_ops](#math_ops%C2%A0)
[Members](#Members)
[nn_ops](#nn_ops%C2%A0)
[Members](#Members)
[no_op](#no_op%C2%A0)
[Members](#Members)
[parsing_ops](#parsing_ops%C2%A0)
[Members](#Members)
[random_ops](#random_ops%C2%A0)
[Members](#Members)
[sparse_ops](#sparse_ops%C2%A0)
[Members](#Members)
[state_ops](#state_ops%C2%A0)
[Members](#Members)
[string_ops](#string_ops%C2%A0)
[Members](#Members)
[training_ops](#training_ops%C2%A0)
[Members](#Members)
[user_ops](#user_ops%C2%A0)
[Members](#Members)
## [array_ops](https://tensorflow.google.cn/api_docs/cc/group/array-ops)
[tensorflow::ops::ZerosLike](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/ops/zeros-like.html)Returns a tensor of zeros with the same shape and type as x.
## [candidate_sampling_ops](https://tensorflow.google.cn/api_docs/cc/group/candidate-sampling-ops)
[tensorflow::ops::UniformCandidateSampler](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/ops/uniform-candidate-sampler.html)Generates labels for candidate sampling with a uniform distribution.
## [control_flow_ops](https://tensorflow.google.cn/api_docs/cc/group/control-flow-ops)
[tensorflow::ops::Switch](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/ops/switch.html)Forwards `data` to the output port determined by `pred`.
## [core](https://tensorflow.google.cn/api_docs/cc/group/core)
[tensorflow::Tensor](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/tensor.html)Represents an n-dimensional array of values.
## [data_flow_ops](https://tensorflow.google.cn/api_docs/cc/group/data-flow-ops)
[tensorflow::ops::Unstage](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/ops/unstage.html)Op is similar to a lightweight Dequeue.
## [image_ops](https://tensorflow.google.cn/api_docs/cc/group/image-ops)
[tensorflow::ops::SampleDistortedBoundingBoxV2](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/ops/sample-distorted-bounding-box-v2.html)Generate a single randomly distorted bounding box for an image.
## [io_ops](https://tensorflow.google.cn/api_docs/cc/group/io-ops)
[tensorflow::ops::WriteFile](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/ops/write-file.html)Writes contents to the file at input filename.
## [logging_ops](https://tensorflow.google.cn/api_docs/cc/group/logging-ops)
[tensorflow::ops::Timestamp](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/ops/timestamp.html)Provides the time since epoch in seconds.
## [math_ops](https://tensorflow.google.cn/api_docs/cc/group/math-ops)
[tensorflow::ops::Zeta](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/ops/zeta.html)Compute the Hurwitz zeta function \((x, q)\).
## [nn_ops](https://tensorflow.google.cn/api_docs/cc/group/nn-ops)
[tensorflow::ops::TopK](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/ops/top-k.html)Finds values and indices of the `k` largest elements for the last dimension.
## [no_op](https://tensorflow.google.cn/api_docs/cc/group/no-op)
[tensorflow::ops::NoOp](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/ops/no-op.html)Does nothing.
## [parsing_ops](https://tensorflow.google.cn/api_docs/cc/group/parsing-ops)
[tensorflow::ops::StringToNumber](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/ops/string-to-number.html)Converts each string in the input [Tensor](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/tensor.html#classtensorflow_1_1_tensor) to the specified numeric type.
## [random_ops](https://tensorflow.google.cn/api_docs/cc/group/random-ops)
[tensorflow::ops::TruncatedNormal](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/ops/truncated-normal.html)Outputs random values from a truncated normal distribution.
## [sparse_ops](https://tensorflow.google.cn/api_docs/cc/group/sparse-ops)
[tensorflow::ops::TakeManySparseFromTensorsMap](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/ops/take-many-sparse-from-tensors-map.html)Converts a sparse representation into a dense tensor.
## [state_ops](https://tensorflow.google.cn/api_docs/cc/group/state-ops)
[tensorflow::ops::Variable](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/ops/variable.html)Holds state in the form of a tensor that persists across steps.
## [string_ops](https://tensorflow.google.cn/api_docs/cc/group/string-ops)
[tensorflow::ops::UnicodeScript](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/ops/unicode-script.html)Determine the script codes of a given tensor of Unicode integer code points.
## [training_ops](https://tensorflow.google.cn/api_docs/cc/group/training-ops)
[tensorflow::ops::SparseApplyRMSProp](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/ops/sparse-apply-r-m-s-prop.html)Update '*var' according to the RMSProp algorithm.
## [user_ops](https://tensorflow.google.cn/api_docs/cc/group/user-ops)
[tensorflow::ops::Fact](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/ops/fact.html)[Output](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/output.html#classtensorflow_1_1_output) a fact about factorials.
> 
[https://tensorflow.google.cn/api_docs/cc/group/user-ops](https://tensorflow.google.cn/api_docs/cc/group/user-ops)
