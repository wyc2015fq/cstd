# 在Macbook Pro上安装支持GPU的TensorFlow - 沧海一粟 —— 技术随手记 - CSDN博客





2017年06月15日 15:54:33[慢游](https://me.csdn.net/eengel)阅读数：11749
个人分类：[TensorFlow](https://blog.csdn.net/eengel/article/category/6972340)









上一篇博文《在Macbook Pro上为TensorFlow设置GPU》中，我们已经为Macbook上的NVIDIA显卡安装了各种驱动，保证各种深度学习框架能够使用GPU进行计算。这儿就总结一下在后续安装TensorFlow的过程中遇到的问题。

按照TensorFlow官网的说明，考虑到Macbook上刚新装了[Anaconda](https://www.continuum.io/downloads#macos) (Python 3.6)，因此选择用anaconda来[安装tensorflow](https://www.tensorflow.org/install/install_mac#installing_with_anaconda)。选择支持GPU且Python3.6版本的TensorFlow安装[链接](https://www.tensorflow.org/install/install_mac#the_url_of_the_tensorflow_python_package)后，安装很快就顺利完成。 

接下来就是测试TensorFlow了！当在python环境中运行import tensorflow时，错误出现了：“Library not loaded: @rpath/libcudart.8.0.dylib”。查看了很多说法，发现众说纷纭。但主要的几个方面就是：

1.环境变量要设置好。在~./bash_profile和~./bashrc中，分别要设置与CUDA相关的环境变量。

```bash
export CUDA_HOME=/usr/local/cuda
export DYLD_LIBRARY_PATH="$CUDA_HOME/lib:$CUDA_HOME/extras/CUPTI/lib"
export LD_LIBRARY_PATH=$DYLD_LIBRARY_PATH
export PATH=$DYLD_LIBRARY_PATH:$PATH
export flags="--config=cuda --config=opt"
```

``

    2.各种建link的[方法](https://github.com/tensorflow/tensorflow/issues/6729)。但很不幸，都没起作用。 

    3.禁用SIP (Security Integrity Protection)。虽然这儿说不需要禁用，但最后死马当活马医，还是按照[攻略](http://www.macworld.com/article/2986118/security/how-to-modify-system-integrity-protection-in-el-capitan.html)把Mac的SIP给禁了。 

    4.也不知道是禁用SIP起了作用，还是单纯重启了机器的作用，运行import tensorflow的时候，错误信息变了：“Library not loaded: @rpath/libcudnn.5.dylib”。这下想起来了。昨天安装CUDA的时候，只把cuDNN的zip包下载下来了，而没有做任何处理！于是，先用which nvcc查看当前CUDA安装目录为/Developer/NVIDIA/CUDA-8.0，然后将cuDNN包中的文件分别拷贝到/lib和/include中。运行import tensorflow还是没找到libcudnn.5.dylib？好吧，想起来之前设置CUDA_HOME是在/usr/local/cuda，是目录/Developer/NVIDIA/CUDA-8.0的符号链接。于是将/Developer/NVIDIA/CUDA-8.0/lib/libcudnn.5.dylib的符号链接也添加进/usr/local/cuda/lib目录下。
```bash
sudo ln -s /Developer/NVIDIA/CUDA-8.0/lib/libcudnn.5.dylib /usr/local/cuda/lib/
```

``

    5.运行以下python代码，终于让TensorFlow发现了NVIDIA的显卡，并成功看到“Hello, TensorFlow!”。

```
>>> import tensorflow as tf
>>> hello = tf.constant('Hello, TensorFlow!')
>>> sess = tf.Session()
tensorflow/core/common_runtime/gpu/gpu_device.cc:887] Found device 0 with properties: 
name: GeForce GT 750M
major: 3 minor: 0 memoryClockRate (GHz) 0.9255
pciBusID 0000:01:00.0
Total memory: 2.00GiB
Free memory: 613.28MiB
2017-06-15 15:13:36.451359: I tensorflow/core/common_runtime/gpu/gpu_device.cc:908] DMA: 0 
2017-06-15 15:13:36.451364: I tensorflow/core/common_runtime/gpu/gpu_device.cc:918] 0:   Y 
2017-06-15 15:13:36.451374: I tensorflow/core/common_runtime/gpu/gpu_device.cc:977] Creating TensorFlow device (/gpu:0) -> (device: 0, name: GeForce GT 750M, pci bus id: 0000:01:00.0)
>>> print(sess.run(hello))
b'Hello, TensorFlow!'
```




