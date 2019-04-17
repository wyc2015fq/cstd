# TensorFlow Object Detection API 超详细教程和踩坑过程（安装） - qq_38593211的博客 - CSDN博客





2018年09月23日 16:40:19[小屁猴多看书](https://me.csdn.net/qq_38593211)阅读数：1796








**目录**

[1.配置环境](#1.%E9%85%8D%E7%BD%AE%E7%8E%AF%E5%A2%83)

**cuda安装**

**    cudnn安装**

**    anaconda安装并创建环境**

**    tensorflow环境**



[2.API安装](#2.API%E5%AE%89%E8%A3%85)

**Tensorflow.models下载**

**    Protobuf配置与测试**

## 1.配置环境

      首先说一下我的环境：Ubuntu16.04 + nvidia384 + cuda9.0 + cudnn（for cuda9.0）+ anaconda4.5.4 +tensorflow1.9

前面两步不用说了，从cuda9.0开始第一步。

**     1.cuda安装：**这里建议安装9.0，因为目前的9.2坑太多，不支持tf1.9，[可以直接打开](https://developer.nvidia.com/cuda-release-candidate-download)，有两种方式，第一个下载runfile(local),

安装的方式很简单，下载好了：

     sudo sh cuda_9.0.103_384.59_linux.run

    第二种方法，按照官方网站所给的教程就好：
- `sudo dpkg -i cuda-repo-ubuntu1604-9-0-local-rc_9.0.103-1_amd64.deb`
- `sudo apt-key add /var/cuda-repo-<version>/7fa2af80.pub`
- `sudo apt-get update`
- `sudo apt-get install cuda`

    安装的时候顺着来就好，除了第二个选项（是否自动安装nvidia驱动）选否，其他选默认的就好，当然你也可以选择安装的位置。

![](https://img-blog.csdn.net/20180923151025260?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4NTkzMjEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

    cuda测试：

         cd /usr/local/cuda-8.0/samples/1_Utilities/deviceQuery

         sudo make

         sudo ./deviceQuery

    出现显卡信息说明安装成功：

![](https://img-blog.csdn.net/20180923152003461?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4NTkzMjEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**2.cudnn安装：**cuda的安装很简单，也没有什么坑，然后是cudnn，其实也是可以不用装的，因为在anaconda中是可以用conda命令一键安装，但是我没有试过有没有问题。[cudnn的版本只要能对应cuda9.0就行](https://developer.nvidia.com/rdp/cudnn-download)，这个先注册nvidia的帐号：

![](https://img-blog.csdn.net/20180923152617184?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4NTkzMjEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

          tar -xzvf cudnn-9.0-linux-x64-v7.3.0.29.tgz

         sudo cp cuda/include/cudnn.h /usr/local/cuda/include

         sudo cp cuda/lib64/libcudnn* /usr/local/cuda/lib64

         sudo chmod a+r /usr/local/cuda/include/cudnn.h

          sudo chmod a+r /usr/local/cuda/lib64/libcudnn*

    把lib64文件夹添加到环境变量：

         sudo vim .bashrc

    在文档最后一行加入：

        export LD_LIBRARY_PATH=/usr/local/cuda/lib64:$LD_LIBRARY_PATH

    设置立即执行：

        source .bashrc

**3.anaconda安装并创建环境**：可以去清华镜像下载[anaconda的安装包](https://mirrors.tuna.tsinghua.edu.cn/anaconda/archive/)，版本可以自己选，这是最新版本：

![](https://img-blog.csdn.net/20180923153822543?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4NTkzMjEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

    安装完了之后可以用：conda --version，查看annaconda的版本。

**4.tensorflow环境**：首先用anaconda创建一个虚拟环境，以后要用tensorflow就必须在启动这个环境才能用，当然这是有好处的，这样不会干扰电脑的原生环境，注意自己给环境取个名字，这里可以选择虚拟环境的python版本，建议选3，也可以两个都装：

        conda create -n ‘conv-name’ python=3.6

     安装之后激活：

        source activate ‘conv-name’

    退出环境：

        source deactivate ‘conv-name’

    然后再安装tensorflow：

        如果之前安装过cudnn可以直接用：pip install  tensorflow-gpu==1.9

        如果之前没有安装cudnn可以用：conda install  tensorflow-gpu==1.9，它会自动安装所需要的其他安装包。

        可以测试一下：

        python import tensorflow as tf

        hello = tf.constant(‘hello,Tensorflow!’)

       session = tf.Session()

        print(session.run(hello))

![](https://img-blog.csdn.net/20180923155513421?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4NTkzMjEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

    如果想在notebook中用tensorflow还需要一些步骤：

        conda install ipython

        conda install jupyter

    然后可以打开notebook中测试：

        jupyter notebook可以打开notebook。

    至此，环境配置就结束了，除了这个api其他深度学习算法用这个环境也是可以的，但也有可能涉及一些版本问题，可能要更新或者降低。

## 2.API安装

** 1.models下载：**首先将tensorflow的object detection api下载下来，他在models目录下，两种办法：

        第一种：git clone [https://github.com/tensorflow/models](https://github.com/tensorflow/models)，这种办法会出现下载中断的情况，你只能多尝试或者换手机热点试试，当然就算你成功clone下来也有可能出现一些其他的问题，比如一些奇奇怪怪的子模块import错误，如：

        module 'nets.mobilenet_v1' has no attribute 'MOBILENETV1_CONV_DEFS'；

        from object_detection.protos import anchor_generator_pb2

        ImportError: cannot import name 'anchor_generator_pb2'

        如果出现这种错误说明clone的文件并不全，用第二种办法吧。

       第二种：直接下载。。。。很直接。

        然后执行，把所需要的库装好：

```
sudo apt-get install protobuf-compiler python-pil python-lxml python-tk
   pip install --user Cython
   pip install --user contextlib2
   pip install --user jupyter
   pip install --user matplotlib
```



**    2.Protobuf配置与测试：**

        cd到/models/research目录下执行：

        protoc object_detection/protos/*.proto --python_out=.

        将slim路径添加到环境变量：

        export PYTHONPATH=$PYTHONPATH:`pwd`:`pwd`/slim

        最后来测试一下吧：

        python object_detection/builders/model_builder_test.py

![](https://img-blog.csdn.net/20180923163349510?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM4NTkzMjEx/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

        如果显示这个，恭喜你安装成功，如果没有，请按照之前的步骤再来一次。











