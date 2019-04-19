# Install TensorFlow for C - Koma Hub - CSDN博客
2019年03月27日 22:30:00[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：19
个人分类：[C/C++																[Tensorflow](https://blog.csdn.net/Rong_Toa/article/category/8794377)](https://blog.csdn.net/Rong_Toa/article/category/7156199)
**Table of Contents**
[Install TensorFlow for C](#Install%20TensorFlow%20for%20C)
[Supported Platforms](#Supported%20Platforms)
[Setup](#Setup)
[Download](#Download)
[Extract](#Extract)
[Linker](#Linker)
[Build](#Build)
[Example program](#Example%20program)
[Compile](#Compile)
[Build from source](#Build%20from%20source)
# Install TensorFlow for C
TensorFlow provides a C API that can be used to build [bindings for other languages](https://tensorflow.google.cn/extend/language_bindings). The API is defined in [`c_api.h`](https://github.com/tensorflow/tensorflow/blob/master/tensorflow/c/c_api.h)and designed for simplicity and uniformity rather than convenience.
## Supported Platforms
TensorFlow for C is supported on the following systems:
- Linux, 64-bit, x86
- macOS X, Version 10.12.6 (Sierra) or higher
- Windows, 64-bit x86
## Setup
### Download
|TensorFlow C library|URL|
|----|----|
|Linux| |
|Linux CPU only|[https://storage.googleapis.com/tensorflow/libtensorflow/libtensorflow-cpu-linux-x86_64-1.12.0.tar.gz](https://storage.googleapis.com/tensorflow/libtensorflow/libtensorflow-cpu-linux-x86_64-1.12.0.tar.gz)|
|Linux GPU support|[https://storage.googleapis.com/tensorflow/libtensorflow/libtensorflow-gpu-linux-x86_64-1.12.0.tar.gz](https://storage.googleapis.com/tensorflow/libtensorflow/libtensorflow-gpu-linux-x86_64-1.12.0.tar.gz)|
|macOS| |
|macOS CPU only|[https://storage.googleapis.com/tensorflow/libtensorflow/libtensorflow-cpu-darwin-x86_64-1.12.0.tar.gz](https://storage.googleapis.com/tensorflow/libtensorflow/libtensorflow-cpu-darwin-x86_64-1.12.0.tar.gz)|
|Windows| |
|Windows CPU only|[https://storage.googleapis.com/tensorflow/libtensorflow/libtensorflow-cpu-windows-x86_64-1.12.0.zip](https://storage.googleapis.com/tensorflow/libtensorflow/libtensorflow-cpu-windows-x86_64-1.12.0.zip)|
|Windows GPU only|[https://storage.googleapis.com/tensorflow/libtensorflow/libtensorflow-gpu-windows-x86_64-1.12.0.zip](https://storage.googleapis.com/tensorflow/libtensorflow/libtensorflow-gpu-windows-x86_64-1.12.0.zip)|
### Extract
Extract the downloaded archive, which contains the header files to include in your C program and the shared libraries to link against.
On Linux and macOS, you may want to extract to `/usr/local/lib`:
sudo tar -C /usr/local -xzf (downloaded file)
### Linker
On Linux/macOS, if you extract the TensorFlow C library to a system directory, such as `/usr/local`, configure the linker with `ldconfig`:
sudo ldconfig
If you extract the TensorFlow C library to a non-system directory, such as `~/mydir`, then configure the linker environmental variables:
[Linux](https://tensorflow.google.cn/install/lang_c#linux)[mac OS](https://tensorflow.google.cn/install/lang_c#mac-os)
export LIBRARY_PATH=$LIBRARY_PATH:~/mydir/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:~/mydir/lib
## Build
### Example program
With the TensorFlow C library installed, create an example program with the following source code (`hello_tf.c`):
```
#include <stdio.h>
#include <tensorflow/c/c_api.h>
int main() {
  printf("Hello from TensorFlow C library version %s\n", TF_Version());
  return 0;
}
```
### Compile
Compile the example program to create an executable, then run:
`gcc hello_tf.c -ltensorflow -o hello_tf``./hello_tf`
The command outputs: `Hello from TensorFlow C library version *number*`
**Success:** The TensorFlow C library is configured.
If the program doesn't build, make sure that `gcc` can access the TensorFlow C library. If extracted to `/usr/local`, explicitly pass the library location to the compiler:
gcc -I/usr/local/include -L/usr/local/lib hello_tf.c -ltensorflow -o hello_tf
## Build from source
TensorFlow is open source. Read [the instructions](https://github.com/tensorflow/tensorflow/blob/master/tensorflow/tools/lib_package/README.md) to build TensorFlow's C library from source code.
[https://tensorflow.google.cn/install/lang_c](https://tensorflow.google.cn/install/lang_c)
