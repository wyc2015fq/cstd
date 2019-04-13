
# Ubuntu16.04 安装 CUDA8.0 + cudnn5.1 + TensorFlow(GPU) 详细过程 - lyc_yongcai的博客 - CSDN博客


2017年06月14日 16:08:42[刷街兜风](https://me.csdn.net/lyc_yongcai)阅读数：1784


下面介绍如何在Ubuntu16.04 中安装GPU驱动，我电脑配置的显卡是 TITAN X 。所以以下安装将以 TITAN X 为例介绍。
（1）首先要安装ubuntu16.04TLS系统在电脑中，我的电脑安装的是双系统，我没有试过在虚拟机中安装，不知道以下方法能不能在虚拟机中安装。
（2）安装显卡驱动，在系统设置-->软件和更新-->附加驱动-->选择需要的驱动         (手动设置)
（3）安装 cuda-8.0  两种方法：
a ：下载安装包：cuda-repo-ubuntu1604-8-0-local_8.0.44-1_amd64.deb
在下载路径中执行如下命令安装：
sudo dpkg -i cuda-repo-ubuntu1604-8-0-local_8.0.44-1_amd64.deb
sudo apt-get update
sudo apt-get install cuda
（建议使用）
b ： 下载安装包：cuda_8.0.44_linux.run
在下载路径中执行如下命令安装：
chmod u+x cuda_8.0.44_linux.run
sudo ./cuda_8.0.44_linux.run
注意：安装过程中不需要再安装显卡驱动

（4）安装cudnn--> cuda
下载包：cudnn-8.0-linux-x64-v5.1.tgz 解压后得到 cuda 文件夹
在cuda 目录下执行如下命令：
sudo cp cuda/include/cudnn.h /usr/local/cuda-8.0/include/
sudo cp cuda/lib64/libcudnn* /usr/local/cuda-8.0/lib64/

（5）配置环境变量：vi .bashrc
添加如下路径：
export LD_LIBRARY_PATH=/usr/local/cuda-8.0/lib64:/usr/local/cuda-8.0/extras/CUPTI/lib64:$LD_LIBRARY_PATH
export CUDA_HOME=/usr/local/cuda-8.0
export PATH=/usr/local/cuda-8.0/bin:$PATH
保存退出
执行：source .bashrc  使配置文件生效
（6）安装 tensorflow(GPU)
sudo pip install tensorflow-gpu  将会安装tensorflow最新的版本

安装完成后会安装一些其他的一些依赖库。

（7）验证 tensorflow -gpu 是否安装成功。







