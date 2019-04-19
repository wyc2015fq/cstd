# TensorFlow C++ Reference Namespaces - Koma Hub - CSDN博客
2019年03月27日 22:26:05[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：40
个人分类：[C/C++																[Tensorflow](https://blog.csdn.net/Rong_Toa/article/category/8794377)](https://blog.csdn.net/Rong_Toa/article/category/7156199)
**Table of Contents**
[tensorflow](#tensorflow)
[Summary](#Summary)
[Typedefs](#Typedefs)
[Functions](#Functions)
[Classes](#Classes)
[Structs](#Structs)
[Namespaces](#Namespaces)
[Typedefs](#Typedefs)
[OutputList](#OutputList)
[StatusCallback](#StatusCallback)
[Functions](#Functions)
[TfCheckOpHelper](#TfCheckOpHelper)
[TfCheckOpHelperOutOfLine](#TfCheckOpHelperOutOfLine)
[operator<<](#operator%3C%3C)
[tensorflow::batch_util](#tensorflow%3A%3Abatch_util)
[Summary](#Summary)
[Functions](#Functions)
[Functions](#Functions)
[CopyElementToSlice](#CopyElementToSlice)
[MaybeMoveSliceToElement](#MaybeMoveSliceToElement)
[tensorflow::ops](#tensorflow%3A%3Aops)
[Summary](#Summary)
[Typedefs](#Typedefs)
[Functions](#Functions)
[Classes](#Classes)
[Typedefs](#Typedefs)
[Mul](#Mul)
[Neg](#Neg)
[ReduceAll](#ReduceAll)
[ReduceAny](#ReduceAny)
[ReduceMax](#ReduceMax)
[ReduceMean](#ReduceMean)
[ReduceMin](#ReduceMin)
[ReduceProd](#ReduceProd)
[ReduceSum](#ReduceSum)
[Sub](#Sub)
[Functions](#Functions)
[AsNodeOut](#AsNodeOut)
[AsNodeOutList](#AsNodeOutList)
[AudioSummary](#AudioSummary)
[AudioSummary](#AudioSummary)
[BadColor](#BadColor)
[Const](#Const)
[Const](#Const)
[Const](#Const)
[ConstFromProto](#ConstFromProto)
[ImageSummary](#ImageSummary)
[ImageSummary](#ImageSummary)
[MaxImages](#MaxImages)
[MaxOutputs](#MaxOutputs)
[node](#node)
[range](#range)
# tensorflow
## Summary
`[StatusCallback](https://tensorflow.google.cn/api_docs/cc/namespace/tensorflow#namespacetensorflow_1a2fd9a161775bcd2d94ae8d2278eb3c46)`typedef
			
`std::function< void(const [Status](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/status.html#classtensorflow_1_1_status) &)>`
`[operator<<](https://tensorflow.google.cn/api_docs/cc/namespace/tensorflow#group__core_1gab11ba3b5186118ae745aa50ee43a759f)(std::ostream & os, const [Status](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/status.html#classtensorflow_1_1_status) & x)`
`std::ostream &`
[tensorflow::Tensor](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/tensor)
Represents an n-dimensional array of values.
[tensorflow::OutputHash](https://tensorflow.google.cn/api_docs/cc/struct/tensorflow/output-hash)
Hash class that can be used for e.g. storing Outputs in an unordered_map.
[tensorflow::ops](https://tensorflow.google.cn/api_docs/cc/namespace/tensorflow/ops)
## Typedefs
### OutputList
std::vector< [Output](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/output.html#classtensorflow_1_1_output) > OutputList
A type for representing the output of ops that produce more than one output, or a list of tensors.
### StatusCallback
std::function< void(const [Status](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/status.html#classtensorflow_1_1_status) &)> StatusCallback
## Functions
### TfCheckOpHelper
tensorflow::string * TfCheckOpHelper(
  ::[tensorflow::Status](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/status.html#classtensorflow_1_1_status) v,
  const char *msg
)
### TfCheckOpHelperOutOfLine
tensorflow::string * TfCheckOpHelperOutOfLine(
  const ::[tensorflow::Status](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/status.html#classtensorflow_1_1_status) & v,
  const char *msg
)
### operator<<
std::ostream & operator<<(
  std::ostream & os,
  const [Status](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/status.html#classtensorflow_1_1_status) & x
)
# tensorflow::batch_util
## Summary
`[MaybeMoveSliceToElement](https://tensorflow.google.cn/api_docs/cc/namespace/tensorflow/batch-util#namespacetensorflow_1_1batch__util_1a1fc899e13a3ae252236afe5ed753b8a4)([Tensor](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/tensor.html#classtensorflow_1_1_tensor) *parent, [Tensor](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/tensor.html#classtensorflow_1_1_tensor) *element, int64 index)`
`[Status](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/status.html#classtensorflow_1_1_status)`
## Functions
### CopyElementToSlice
[Status](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/status.html#classtensorflow_1_1_status) CopyElementToSlice(
  [Tensor](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/tensor.html#classtensorflow_1_1_tensor) element,
  [Tensor](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/tensor.html#classtensorflow_1_1_tensor) *parent,
  int64 index
)
### MaybeMoveSliceToElement
[Status](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/status.html#classtensorflow_1_1_status) MaybeMoveSliceToElement(
  [Tensor](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/tensor.html#classtensorflow_1_1_tensor) *parent,
  [Tensor](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/tensor.html#classtensorflow_1_1_tensor) *element,
  int64 index
)
# tensorflow::ops
## Summary
`[Sub](https://tensorflow.google.cn/api_docs/cc/namespace/tensorflow/ops#group__math__ops_1ga8872d71afb365f8e9c3547011fb33183)`typedef
			
`[Subtract](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/ops/subtract.html#classtensorflow_1_1ops_1_1_subtract)`
`[range](https://tensorflow.google.cn/api_docs/cc/namespace/tensorflow/ops#group__logging__ops_1gaef72c3599d7080731b33b33540d88b94)(It represents the value of a *pixel in the output image).Non-finite values in the input tensor are *replaced by this tensor in the output image.The default value is the color *red.**Arguments`
`image **If max_images is greater the summary value tags are *generated sequentially as *tag *tag etc **The bad_color argument is the color to use in the generated images for *non finite input values It is a uint8 D tensor of length channels *Each element must be in the`
[Max](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/ops/max.html#classtensorflow_1_1ops_1_1_max) number of batch elements to generate images for.
[tensorflow::ops::Zeta](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/ops/zeta)
Compute the Hurwitz zeta function \((x, q)\).
## Typedefs
### Mul
[Multiply](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/ops/multiply.html#classtensorflow_1_1ops_1_1_multiply) Mul
### Neg
[Negate](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/ops/negate.html#classtensorflow_1_1ops_1_1_negate) Neg
### ReduceAll
[All](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/ops/all.html#classtensorflow_1_1ops_1_1_all) ReduceAll
### ReduceAny
[Any](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/ops/any.html#classtensorflow_1_1ops_1_1_any) ReduceAny
### ReduceMax
[Max](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/ops/max.html#classtensorflow_1_1ops_1_1_max) ReduceMax
### ReduceMean
[Mean](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/ops/mean.html#classtensorflow_1_1ops_1_1_mean) ReduceMean
### ReduceMin
[Min](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/ops/min.html#classtensorflow_1_1ops_1_1_min) ReduceMin
### ReduceProd
[Prod](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/ops/prod.html#classtensorflow_1_1ops_1_1_prod) ReduceProd
### ReduceSum
[Sum](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/ops/sum.html#classtensorflow_1_1ops_1_1_sum) ReduceSum
### Sub
[Subtract](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/ops/subtract.html#classtensorflow_1_1ops_1_1_subtract) Sub
## Functions
### AsNodeOut
NodeBuilder::NodeOut AsNodeOut(
  const [Scope](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/scope.html#classtensorflow_1_1_scope) & scope,
  const [Input](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/input.html#classtensorflow_1_1_input) & inp
)
### AsNodeOutList
std::vector< NodeBuilder::NodeOut > AsNodeOutList(
  const [Scope](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/scope.html#classtensorflow_1_1_scope) & scope,
  const [InputList](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/input-list.html#classtensorflow_1_1_input_list) & inp
)
### AudioSummary
 AudioSummary(
  const ::[tensorflow::Scope](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/scope.html#classtensorflow_1_1_scope) & scope,
  ::[tensorflow::Input](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/input.html#classtensorflow_1_1_input) tag,
  ::[tensorflow::Input](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/input.html#classtensorflow_1_1_input) tensor,
  ::[tensorflow::Input](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/input.html#classtensorflow_1_1_input) sample_rate
)
### AudioSummary
 AudioSummary(
  const ::[tensorflow::Scope](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/scope.html#classtensorflow_1_1_scope) & scope,
  ::[tensorflow::Input](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/input.html#classtensorflow_1_1_input) tag,
  ::[tensorflow::Input](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/input.html#classtensorflow_1_1_input) tensor,
  ::[tensorflow::Input](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/input.html#classtensorflow_1_1_input) sample_rate,
  const AudioSummary::Attrs & attrs
)
### BadColor
TF_MUST_USE_RESULT Attrs BadColor(
  const TensorProto & x
)
Color to use for pixels with non-finite values.
Defaults to Tensor
### Const
[Output](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/output.html#classtensorflow_1_1_output) Const(
  const [Scope](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/scope.html#classtensorflow_1_1_scope) & scope,
  const [Input::Initializer](https://tensorflow.google.cn/api_docs/cc/struct/tensorflow/input/initializer.html#structtensorflow_1_1_input_1_1_initializer) & val
)
### Const
[Output](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/output.html#classtensorflow_1_1_output) Const(
  const [Scope](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/scope.html#classtensorflow_1_1_scope) & scope,
  const T & v,
  const TensorShape shape
)
### Const
[Output](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/output.html#classtensorflow_1_1_output) Const(
  const [Scope](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/scope.html#classtensorflow_1_1_scope) & scope,
  const std::initializer_list< T > & v,
  const TensorShape shape
)
### ConstFromProto
[Output](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/output.html#classtensorflow_1_1_output) ConstFromProto(
  const [Scope](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/scope.html#classtensorflow_1_1_scope) & scope,
  const TensorProto & proto
)
### ImageSummary
 ImageSummary(
  const ::[tensorflow::Scope](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/scope.html#classtensorflow_1_1_scope) & scope,
  ::[tensorflow::Input](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/input.html#classtensorflow_1_1_input) tag,
  ::[tensorflow::Input](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/input.html#classtensorflow_1_1_input) tensor
)
### ImageSummary
 ImageSummary(
  const ::[tensorflow::Scope](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/scope.html#classtensorflow_1_1_scope) & scope,
  ::[tensorflow::Input](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/input.html#classtensorflow_1_1_input) tag,
  ::[tensorflow::Input](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/input.html#classtensorflow_1_1_input) tensor,
  const ImageSummary::Attrs & attrs
)
### MaxImages
Attrs MaxImages(
  int64 x
)
### MaxOutputs
Attrs MaxOutputs(
  int64 x
)
### node
::tensorflow::Node * node() const 
### range
image **If max_images is greater the summary value tags are *generated sequentially as *tag *tag etc **The bad_color argument is the color to use in the generated images for *non finite input values It is a uint8 D tensor of length channels *Each element must be in the range(
  It represents the value of a *pixel in the output image
).Non-finite values in the input tensor are *replaced by this tensor in the output image.The default value is the color *red.**Arguments
[Max](https://tensorflow.google.cn/api_docs/cc/class/tensorflow/ops/max.html#classtensorflow_1_1ops_1_1_max) number of batch elements to generate images for.
Defaults to 3
> 
[https://tensorflow.google.cn/api_docs/cc/namespace/tensorflow/ops](https://tensorflow.google.cn/api_docs/cc/namespace/tensorflow/ops)
