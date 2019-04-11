# 使用CuDNN进行卷积运算

2018年03月26日 16:12:50 [ffiirree](https://me.csdn.net/ice__snow) 阅读数：2630



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/ice__snow/article/details/79699388

## CuDNN

NVIDIA ® cuDNN is a GPU-accelerated library of primitives for deep neural networks. It provides highly tuned implementations of routines arising frequently in DNN applications:

- Convolution forward and backward, including cross-correlation

- Pooling forward and backward

- Softmax forward and backward

- Neuron activations forward and backward:

   

  - Rectified linear (ReLU)
  - Sigmoid
  - Hyperbolic tangent (TANH)

- Tensor transformation functions

- LRN, LCN and batch normalization forward and backward

## ENV

- Ubuntu 16.04
- CUDA 9.0
- CuDNN 7.0
- GCC 5.4.0
- [Alchemy](https://github.com/ffiirree/Alchemy) / OpenCV
- CLion 2018.1 RC2

## cudnnConvolutionForward()

CuDNN中计算卷积操作的由`cudnnConvolutionForward()`来实现，其原型为

```cpp
cudnnStatus_t CUDNNWINAPI cudnnConvolutionForward(
    cudnnHandle_t                       handle,
    const void                         *alpha,
    const cudnnTensorDescriptor_t       xDesc,
    const void                         *x,
    const cudnnFilterDescriptor_t       wDesc,
    const void                         *w,
    const cudnnConvolutionDescriptor_t  convDesc,
    cudnnConvolutionFwdAlgo_t           algo,
    void                               *workSpace,
    size_t                              workSpaceSizeInBytes,
    const void                         *beta,
    const cudnnTensorDescriptor_t       yDesc,
    void                               *y );
```

其中:

- `x`为输入数据的地址，`w`为卷积核的地址，`y`为输出数据的地址，对应的`xDesc`、`wDesc`和`yDesc`为描述这三个数据的描述子，比如记录了数据的batch size、channels、height和width等。
- `alpha`对卷积结果`x*w`进行缩放，`beta`对输出`y`进行缩放，其表达式为：

```cpp
dstValue = alpha[0]*computedValue + beta[0]*priorDstValue1
```

- `workspace`是指向进行卷积操作时需要的GPU空间的指针
- `workSpaceSizeInBytes`为该空间的大小
- `algo`用来指定使用什么算法来进行卷积运算
- `handle`是创建的`library context`的句柄，使用CuDNN库必须用`cudaCreate()`来初始化。

## 实例

对图片进行拉普拉斯变换。

### 1. 准备图像

```cpp
    auto image = imread("red.png");
    auto image_float = Matrix32f(image);12
```

### 2. 准备使用CUDNN的环境

```C++
    //handle
    cudnnHandle_t handle;
    cudnnCreate(&handle);123
```

### 3. 准备输入数据

```cpp
    // input
    Tensor<float> input({ 1, image.channels(), image.rows, image.cols });
    Memory::copy(image_float.count() * sizeof(float), input.gptr(), image_float.ptr());

    cudnnTensorDescriptor_t input_descriptor;
    cudnnCreateTensorDescriptor(&input_descriptor);
    cudnnSetTensor4dDescriptor(input_descriptor,
                               CUDNN_TENSOR_NHWC,
                               CUDNN_DATA_FLOAT,
                               input.shape(0), input.shape(1), input.shape(2), input.shape(3));
```

`input_descriptor`保存了输入的信息，其中

- ```cpp
  CUDNN_TENSOR_NHWC
  ```

  是数据数据的结构，这表示，input为一个四维的张量，四个维度分别为

   

  - N: Number这里作为输入的图片个数，我们例子中只有一张图片，为1
  - H: Height，即图片的高
  - W: Width，即图片的宽
  - C: Channels，即通道数，这里是图片的通道数，如果输入为RGB三通道则为3，灰度图则为1，依次类推

- `CUDNN_DATA_FLOAT`为计算的数据类型

### 4. 准备输出数据

```cpp
// output
Tensor<float> output(input.shape());
vector_set_gpu(output.count(), 0.0f, output.gptr());

cudnnTensorDescriptor_t output_descriptor;
cudnnCreateTensorDescriptor(&output_descriptor);
cudnnSetTensor4dDescriptor(output_descriptor,
    CUDNN_TENSOR_NHWC,
    CUDNN_DATA_FLOAT,
    output.shape(0), output.shape(1), output.shape(2), output.shape(3));
```

### 5. 准备卷积核

```cpp
// kernel
    Tensor<float> kernel({ output.shape(1), input.shape(1), 3, 3 });
    auto kernel_size = kernel.count(2, 4);
    float kernel_[kernel_size] = { 0, 1, 0, 1, -4, 1, 0, 1, 0 };
    for(auto i = 0; i < kernel.count(0, 2); ++i) {
        memcpy(kernel.cptr() + i * kernel_size, kernel_, kernel_size * sizeof(float));
    }

    cudnnFilterDescriptor_t kernel_descriptor;
    cudnnCreateFilterDescriptor(&kernel_descriptor);
    cudnnSetFilter4dDescriptor(kernel_descriptor,
                               CUDNN_DATA_FLOAT,
                               CUDNN_TENSOR_NCHW,
                               kernel.shape(0), kernel.shape(1), kernel.shape(2), kernel.shape(3));
```

### 6. 卷积的描述子

```cpp
    // convolution descriptor
    cudnnConvolutionDescriptor_t conv_descriptor;
    cudnnCreateConvolutionDescriptor(&conv_descriptor);
    cudnnSetConvolution2dDescriptor(conv_descriptor,
                                    1, 1, // zero-padding
                                    1, 1, // stride
                                    1, 1,
                                    CUDNN_CROSS_CORRELATION, CUDNN_DATA_FLOAT);
```

### 7. 选择算法

```
// algorithm
cudnnConvolutionFwdAlgo_t algo;
cudnnGetConvolutionForwardAlgorithm(handle,
    input_descriptor,
    kernel_descriptor,
    conv_descriptor,
    output_descriptor,
    CUDNN_CONVOLUTION_FWD_PREFER_FASTEST,
    0,
    &algo);
```

告诉CuDNN优先使用计算速度快的算法，并且没有内存限制

### 8. 准备计算所用的空间

```cpp
// workspace size && allocate memory
size_t workspace_size = 0;
cudnnGetConvolutionForwardWorkspaceSize(handle,
    input_descriptor,
    kernel_descriptor,
    conv_descriptor,
    output_descriptor,
    algo,
    &workspace_size);

void * workspace = nullptr;
cudaMalloc(&workspace, workspace_size);123456789101112
```

### 9. Convolution

```cpp
// convolution
auto alpha = 1.0f, beta = 0.0f;
cudnnConvolutionForward(handle,
    &alpha, input_descriptor, input.gptr(),
    kernel_descriptor, kernel.gptr(),
    conv_descriptor, algo,
    workspace, workspace_size,
    &beta, output_descriptor, output.gptr());12345678
```

### 10. 销毁

```
// destroy
cudaFree(workspace);

cudnnDestroyTensorDescriptor(input_descriptor);
cudnnDestroyTensorDescriptor(output_descriptor);
cudnnDestroyConvolutionDescriptor(conv_descriptor);
cudnnDestroyFilterDescriptor(kernel_descriptor);

cudnnDestroy(handle);123456789
```

## 效果

![这里写图片描述](https://img-blog.csdn.net/20180326161134805?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaWNlX19zbm93/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 源代码

```cpp
#include <cudnn_v7.h>
#include "alchemy.h"

using namespace std;
using namespace alchemy;

int main()
{
    // image
    auto image = imread("red.png");
    auto image_float = Matrix32f(image);

    //handle
    cudnnHandle_t handle;
    cudnnCreate(&handle);

    // input
    Tensor<float> input({ 1, image.channels(), image.rows, image.cols });
    Memory::copy(image_float.count() * sizeof(float), input.gptr(), image_float.ptr());

    cudnnTensorDescriptor_t input_descriptor;
    cudnnCreateTensorDescriptor(&input_descriptor);
    cudnnSetTensor4dDescriptor(input_descriptor,
                               CUDNN_TENSOR_NHWC,
                               CUDNN_DATA_FLOAT,
                               input.shape(0), input.shape(1), input.shape(2), input.shape(3));

    // output
    Tensor<float> output(input.shape());
    vector_set_gpu(output.count(), 0.0f, output.gptr());

    cudnnTensorDescriptor_t output_descriptor;
    cudnnCreateTensorDescriptor(&output_descriptor);
    cudnnSetTensor4dDescriptor(output_descriptor,
                               CUDNN_TENSOR_NHWC,
                               CUDNN_DATA_FLOAT,
                               output.shape(0), output.shape(1), output.shape(2), output.shape(3));

    // kernel
    Tensor<float> kernel({ output.shape(1), input.shape(1), 3, 3 });
    auto kernel_size = kernel.count(2, 4);
    float kernel_[kernel_size] = { 0, 1, 0, 1, -4, 1, 0, 1, 0 };
    for(auto i = 0; i < kernel.count(0, 2); ++i) {
        memcpy(kernel.cptr() + i * kernel_size, kernel_, kernel_size * sizeof(float));
    }

    cudnnFilterDescriptor_t kernel_descriptor;
    cudnnCreateFilterDescriptor(&kernel_descriptor);
    cudnnSetFilter4dDescriptor(kernel_descriptor,
                               CUDNN_DATA_FLOAT,
                               CUDNN_TENSOR_NCHW,
                               kernel.shape(0), kernel.shape(1), kernel.shape(2), kernel.shape(3));
    // convolution descriptor
    cudnnConvolutionDescriptor_t conv_descriptor;
    cudnnCreateConvolutionDescriptor(&conv_descriptor);
    cudnnSetConvolution2dDescriptor(conv_descriptor,
                                    1, 1, // zero-padding
                                    1, 1, // stride
                                    1, 1,
                                    CUDNN_CROSS_CORRELATION, CUDNN_DATA_FLOAT);

    // algorithm
    cudnnConvolutionFwdAlgo_t algo;
    cudnnGetConvolutionForwardAlgorithm(handle,
                                        input_descriptor,
                                        kernel_descriptor,
                                        conv_descriptor,
                                        output_descriptor,
                                        CUDNN_CONVOLUTION_FWD_PREFER_FASTEST,
                                        0,
                                        &algo);

    // workspace size && allocate memory
    size_t workspace_size = 0;
    cudnnGetConvolutionForwardWorkspaceSize(handle,
                                            input_descriptor,
                                            kernel_descriptor,
                                            conv_descriptor,
                                            output_descriptor,
                                            algo,
                                            &workspace_size);

    void * workspace = nullptr;
    cudaMalloc(&workspace, workspace_size);

    // convolution
    auto alpha = 1.0f, beta = 0.0f;
    cudnnConvolutionForward(handle,
                            &alpha, input_descriptor, input.gptr(),
                            kernel_descriptor, kernel.gptr(),
                            conv_descriptor, algo,
                            workspace, workspace_size,
                            &beta, output_descriptor, output.gptr());

    Matrix32f output_image(image.shape());
    cudaMemcpy(output_image.ptr(), output.gptr(), image.count() * sizeof(float), cudaMemcpyDeviceToHost);

    // destroy
    cudaFree(workspace);

    cudnnDestroyTensorDescriptor(input_descriptor);
    cudnnDestroyTensorDescriptor(output_descriptor);
    cudnnDestroyConvolutionDescriptor(conv_descriptor);
    cudnnDestroyFilterDescriptor(kernel_descriptor);

    cudnnDestroy(handle);


    // show
    imshow("original", image);
    imshow("output", Matrix(output_image/3.0));

    waitKey(0);
    return 0;
}
```