
# Ubuntu16.10+GTX1060+Cuda8.0+Cudnn7+Tensorflow1.3+Python3.6 - OraYang的博客 - CSDN博客

2017年09月06日 10:14:49[OraYang](https://me.csdn.net/u010665216)阅读数：5272所属专栏：[机器学习](https://blog.csdn.net/column/details/16605.html)



最近为了跑个Skipgram模型，准备装个带GPU的tensorflow,感觉网上教程不是很详细，就自己总结了下，有问题，欢迎私信~：
## 目录
[目录](#目录)
[安装Anaconda](#安装anaconda)
[建立一个tensorflow的运行环境](#建立一个tensorflow的运行环境)
[安装 CUDA 8.0](#安装-cuda-80)[设置环境变量](#设置环境变量)
[安装安装 cudnn7](#安装安装-cudnn7)
[安装 tensorflow r1.3 GPU 版本](#安装-tensorflow-r13-gpu-版本)
[测试及可能出现的错误](#测试及可能出现的错误)


---在具体实施前，请去BIOS中将显卡切换成独立显卡（本次实验环境，组装台式机，主板z270，显卡GTX1060，显存6G)

---安装Anaconda

---从anaconda官网（

---[https://www.continuum.io/downloads](https://www.continuum.io/downloads)

---）上下载linux版本的安装文件（截至目前为止为python3.6），运行sh完成安装。

---建立一个tensorflow的运行环境

---\# Python 2.7

---$

---conda create -n tensorflow python=

---2.7

---\# Python 3.5

---$

---conda create -n tensorflow python=

---3.5

---\# Python 3.6

---$

---conda create -n tensorflow python=

---3.6

---安装 CUDA 8.0

---用的 deb(network) 的安装方式

---官网的下载地址:

---[Cuda](https://developer.nvidia.com/cuda-downloads)

---$

---wget

---http:

---/

---/developer.download.nvidia.com/compute

---/cuda/repos

---/ubuntu1604/x

---86_64/cuda-repo-ubuntu1604_8.

---0

---.

---61

----

---1_

---amd64.deb

---$

---sudo dpkg -i cuda-repo-ubuntu1604_8.

---0

---.

---61

----

---1_

---amd64.deb

---$

---sudo apt-get update

---$

---sudo apt-get install cuda

---等待安装

---设置环境变量

---在 ~/.bashrc 的最后添加

---export

---PATH

---=

---/usr/local

---/cuda/bin

---${

---PATH

---:+

---:

---${

---PATH

---}}
export

---LD_LIBRARY_PATH

---=

---/usr/local

---/cuda/lib

---64

---${

---LD_LIBRARY_PATH

---:+

---:

---${

---LD_LIBRARY_PATH

---}}
export

---CUDA_HOME

---=

---/usr/local

---/cuda

---此时重启电脑

---检查显卡驱动安装结果

---nvidia

----smi

---安装安装 cudnn7

---官网下载地址：

---[Cudnn7.0 for Cuda8.0](https://developer.nvidia.com/rdp/cudnn-download)

---下载lib库，后缀是tgz

---$

---tar xvzf cudnn-

---8.0

----linux-x64-v5.

---1

---.tgz

---$

---sudo cp cuda/

---include

---/cudnn.h /usr/local/cuda/

---include

---$

---sudo cp cuda/lib64/libcudnn*

---/usr/local

---/cuda/lib

---64

---$

---sudo chmod a+r /usr/local/cuda/

---include

---/cudnn.h /usr/local/cuda/lib64/libcudnn*

---安装 tensorflow r1.3 GPU 版本

---$

---source activate tensorflow

---$

---pip install --ignore-installed --upgrade

---https:

---/

---/storage.googleapis.com/tensorflow

---/linux/gpu

---/tensorflow-1.3.0-cp36-cp36m-linux_x86_64.whl

---如果出现timeout错误，就重新输入一便即可，或者增加timeout机制

---测试及可能出现的错误

---\# Python

---import

---tensorflow

---as

---tf
hello = tf.constant(

---'Hello, TensorFlow!'

---)
sess = tf.Session()
print(sess.run(hello))

---如果有正常打印，则恭喜你，安装成功啦，如果出现：

---ImportError: libcudnn.so.6: cannot open shared object file: No such file or directory

---libcudnn.so.6，遇到这个问题不用担心，是有套路的，在此我就教教大家

---首先检查是否存在libcundnn.so.*

---$

---find / -name libcudnn.so.*

---找到文件就下一步，没找到，检查下cudnn的依赖库，就是前面的环境变量做对了没


---建立硬连接

---$ ln -s

---<

---path

--->

---libcudnn.so.7.*

---<

---path

--->

---libcudnn.so.6

---然后就大功告成了！



