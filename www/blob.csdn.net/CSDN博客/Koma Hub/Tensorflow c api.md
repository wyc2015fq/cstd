# Tensorflow c api - Koma Hub - CSDN博客
2019年03月25日 21:40:50[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：50
**Table of Contents**
[编译使用tensorflow c版本动态链接库](#%E7%BC%96%E8%AF%91%E4%BD%BF%E7%94%A8tensorflow%20c%E7%89%88%E6%9C%AC%E5%8A%A8%E6%80%81%E9%93%BE%E6%8E%A5%E5%BA%93)
[TensorFlow C++ Session API reference documentation](#tensorflow-c-session-api-reference-documentation-)
[Classes](#classes-)
[Structs](#structs-)
# [编译使用tensorflow c版本动态链接库](https://www.cnblogs.com/hrlnw/p/7007648.html)
注意：如果是linux或mac os操作系统，可以参考[https://www.tensorflow.org/install/install_c](https://www.tensorflow.org/install/install_c)，直接下载对应的so库和头文件，然后跳到步骤4。如果不能使用，再从源码编译。
**1.下载tensorflow源码**
  git clone --recursive [https://github.com/tensorflow/tensorflow](https://github.com/tensorflow/tensorflow)
  假设路径为：tensorflow_repo_path
**2.安装[bazel](https://bazel.build/)**
  mac os可以直接brew install bazel，没法联网安装的可以下载sh文件安装，也很简单
**3.编译源码，生成so库**
  进入tensorflow_repo_path/tensorflow文件夹，执行：
*bazel build :libtensorflow.so*
**4.添加必要路径**
  最简单方法是直接把include中文件复制到/usr/local/include，把lib中文件复制到/usr/local/lib，或者修改对应的环境变量LD_LIBRARY_PATH,并在编译的时候指定       include目录。
**5.编译运行测试程序**
依然是[https://www.tensorflow.org/install/install_c](https://www.tensorflow.org/install/install_c)上的内容，直接把下面代码保存成helloTF.c文件
```cpp
#include <stdio.h>
#include <tensorflow/c/c_api.h>
int main() {
  printf(“Hello from TensorFlow C library version %s\n”, TF_Version());
  return 0;
}
```
 如果使用gcc编译，除了-L和-I中添加对应的库、头文件路径，还要加上-ltensorflow,如果使用xcode这类ide，也要在ide中添加相应路径、链接库
  编译成功后，把libtensorflow.so复制到可执行文件同一目录下（如果已经在/usr/local/lib或者在LD_LIBRARY_PATH中就不需要复制了），运行可执行文件，得到下面   的结果，即说明运行成功：
**6.其他**
  tensorflow目前已经支持多种语言，并且可以编译对应的库文件，具体可以参考tensorflow_repo_path/tensorflow/BUILD文件，这里面有多个编译目标，c++的目前   也已经支持，不过相关参考资料太少，只有tensorflow_repo_path/tensorflow/cc/tutorials/example_trainer.cc一个例子。
**7.参考资料**
  https://medium.com/jim-fleming/loading-tensorflow-graphs-via-host-languages-be10fd81876f
  https://medium.com/jim-fleming/loading-a-tensorflow-graph-with-the-c-api-4caaff88463f#.z4qeoyfb0
  https://www.tensorflow.org/install/install_c
# TensorFlow C++ Session API reference documentation
TensorFlow's public C++ API includes only the API for executing graphs, as of version 0.5. To control the execution of a graph from C++:
- Build the computation graph using the [Python API](http://www.tensorfly.cn/tfdoc/api_docs/python).
- Use [tf.train.write_graph()](http://www.tensorfly.cn/tfdoc/api_docs/python/train.html#write_graph) to write the graph to a file.
- 
Load the graph using the C++ Session API. For example:
```cpp
// Reads a model graph definition from disk, and creates a session object you
// can use to run it.
Status LoadGraph(string graph_file_name, Session** session) {
 GraphDef graph_def;
 TF_RETURN_IF_ERROR(
     ReadBinaryProto(Env::Default(), graph_file_name, &graph_def));
 TF_RETURN_IF_ERROR(NewSession(SessionOptions(), session));
 TF_RETURN_IF_ERROR((*session)->Create(graph_def));
 return Status::OK();
}
```
- 
Run the graph with a call to `session->Run()`
## Classes
- [tensorflow::Env](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassEnv.html)
- [tensorflow::EnvWrapper](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassEnvWrapper.html)
- [tensorflow::RandomAccessFile](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassRandomAccessFile.html)
- [tensorflow::Session](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassSession.html)
- [tensorflow::Status](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassStatus.html)
- [tensorflow::Tensor](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensor.html)
- [tensorflow::TensorBuffer](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorBuffer.html)
- [tensorflow::TensorShape](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorShape.html)
- [tensorflow::TensorShapeIter](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorShapeIter.html)
- [tensorflow::TensorShapeUtils](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassTensorShapeUtils.html)
- [tensorflow::Thread](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassThread.html)
- [tensorflow::WritableFile](http://www.tensorfly.cn/tfdoc/api_docs/cc/ClassWritableFile.html)
## Structs
- [tensorflow::SessionOptions](http://www.tensorfly.cn/tfdoc/api_docs/cc/StructSessionOptions.html)
- [tensorflow::Status::State](http://www.tensorfly.cn/tfdoc/api_docs/cc/StructState.html)
- [tensorflow::TensorShapeDim](http://www.tensorfly.cn/tfdoc/api_docs/cc/StructTensorShapeDim.html)
- [tensorflow::ThreadOptions](http://www.tensorfly.cn/tfdoc/api_docs/cc/StructThreadOptions.html)
Link：[http://www.tensorfly.cn/tfdoc/api_docs/cc/index.html](http://www.tensorfly.cn/tfdoc/api_docs/cc/index.html)
