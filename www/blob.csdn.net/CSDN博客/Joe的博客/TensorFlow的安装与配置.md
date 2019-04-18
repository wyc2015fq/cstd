# TensorFlow的安装与配置 - Joe的博客 - CSDN博客





2016年09月08日 23:58:11[Joe-Han](https://me.csdn.net/u010089444)阅读数：3153标签：[TensorFlow																[安装																[配置](https://so.csdn.net/so/search/s.do?q=配置&t=blog)
个人分类：[TensorFlow](https://blog.csdn.net/u010089444/article/category/6409141)





TensorFlow 官方安装教程：[https://www.tensorflow.org/versions/r0.10/get_started/os_setup.html](https://www.tensorflow.org/versions/r0.10/get_started/os_setup.html)

TensorFlow 官方tutorial： [https://www.tensorflow.org/versions/r0.10/tutorials/mnist/beginners/index.html#the-mnist-data](https://www.tensorflow.org/versions/r0.10/tutorials/mnist/beginners/index.html#the-mnist-data)

TensorFlow 官方文档中文版 ： [http://wiki.jikexueyuan.com/project/tensorflow-zh/](http://wiki.jikexueyuan.com/project/tensorflow-zh/)
## 安装需求：
- Python 2.7 或Python 3.3+.
- Ubuntu系统（使用Docker可以在windos下安装TensorFlow，但相对麻烦，推荐在linux环境下使用）
- 若使用GPU加速，需安装Cuda Toolkit 7.0以上版本 ->[Cuda安装指南](http://docs.nvidia.com/cuda/cuda-installation-guide-linux/index.html#axzz4JiThmTbU%20%E5%AE%89%E8%A3%85%E6%8C%87%E5%8D%97)。硬件要求：Compute Capability在3.0以上的NVidia GPU [Check NVIDIA Compute Capability](https://developer.nvidia.com/cuda-gpus)

## 安装python-pip和python-dev

```
# Ubuntu/Linux 64-bit
$ sudo apt-get install python-pip python-dev
```

![图片名称](https://img-blog.csdn.net/20160909093115000)

## 安装Tensorflow

**选择合适的Tensorflow版本**

注：“CPU only”表示只通过CPU进行计算；“GPU enabled”表示使用GPU进行加速

```python
# Ubuntu/Linux 64-bit, CPU only, Python 2.7
$ export TF_BINARY_URL=https://storage.googleapis.com/tensorflow/linux/cpu/tensorflow-0.10.0-cp27-none-linux_x86_64.whl

# Ubuntu/Linux 64-bit, GPU enabled, Python 2.7
# Requires CUDA toolkit 7.5 and CuDNN v4. For other versions, see "Install from sources" below.
$ export TF_BINARY_URL=https://storage.googleapis.com/tensorflow/linux/gpu/tensorflow-0.10.0-cp27-none-linux_x8

# Ubuntu/Linux 64-bit, CPU only, Python 3.4
$ export TF_BINARY_URL=https://storage.googleapis.com/tensorflow/linux/cpu/tensorflow-0.10.0-cp34-cp34m-linux_x86_64.whl

# Ubuntu/Linux 64-bit, GPU enabled, Python 3.4
# Requires CUDA toolkit 7.5 and CuDNN v4. For other versions, see "Install from sources" below.
$ export TF_BINARY_URL=https://storage.googleapis.com/tensorflow/linux/gpu/tensorflow-0.10.0-cp34-cp34m-linux_x86_64.whl


# Ubuntu/Linux 64-bit, CPU only, Python 3.5
$ export TF_BINARY_URL=https://storage.googleapis.com/tensorflow/linux/cpu/tensorflow-0.10.0-cp35-cp35m-linux_x86_64.whl

# Ubuntu/Linux 64-bit, GPU enabled, Python 3.5
# Requires CUDA toolkit 7.5 and CuDNN v4. For other versions, see "Install from sources" below.
$ export TF_BINARY_URL=https://storage.googleapis.com/tensorflow/linux/gpu/tensorflow-0.10.0-cp35-cp35m-linux_x86_64.whl
```

**安装Tensorflow：**

```python
# Python 2
$ sudo pip install --upgrade $TF_BINARY_URL

# Python 3
$ sudo pip3 install --upgrade $TF_BINARY_URL
```](https://so.csdn.net/so/search/s.do?q=安装&t=blog)](https://so.csdn.net/so/search/s.do?q=TensorFlow&t=blog)




