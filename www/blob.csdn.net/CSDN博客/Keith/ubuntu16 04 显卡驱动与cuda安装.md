# ubuntu16.04 显卡驱动与cuda安装 - Keith - CSDN博客





2017年07月03日 09:16:48[ke1th](https://me.csdn.net/u012436149)阅读数：2928








> 
几乎所有的深度学习框架都可以感受到GPU加速的乐趣


## 准备工作
- 配置 apt-get 的源

```
第一种方法. 系统设置-> 软件与更新 ->ubuntu 软件 -> Download from -> 把这里改成阿里的源或其它高校的源即可
第二种方法：只需将你想使用的源 添加到 /etc/apt/sources.list 文件头部即可
```

ubuntu16.04 阿里源如下所示

```
deb http://mirrors.aliyun.com/ubuntu/ xenial main restricted

deb http://mirrors.aliyun.com/ubuntu/ xenial-updates main restricted
deb http://mirrors.aliyun.com/ubuntu/ xenial universe
deb http://mirrors.aliyun.com/ubuntu/ xenial-updates universe

deb http://mirrors.aliyun.com/ubuntu/ xenial multiverse
deb http://mirrors.aliyun.com/ubuntu/ xenial-updates multiverse

deb http://mirrors.aliyun.com/ubuntu/ xenial-backports main restricted universe multiverse

deb http://mirrors.aliyun.com/ubuntu/ xenial-security main restricted
deb http://mirrors.aliyun.com/ubuntu/ xenial-security universe
deb http://mirrors.aliyun.com/ubuntu/ xenial-security multiverse
```

## 安装依赖

```
sudo apt-get install python-pip
sudo apt-get install python-numpy swig python-dev python-wheel
sudo apt-get install git vim
sudo apt-get install freeglut3-dev build-essential libx11-dev libxmu-dev libxi-dev libgl1-mesa-glx libglu1-mesa libglu1-mesa-dev
```

## 配置 pip 源

```
cd $HOME
mkdir .pip
cd .pip
sudo vim pip.conf

在里面添加
[global]
index-url = http://mirrors.aliyun.com/pypi/simple
[install]
trusted-host = mirrors.aliyun.com
disable-pip-version-check = true
timeout = 6000
保存即可
```

## 安装 nvidia 驱动

```bash
sudo vim /etc/modprobe.d/blacklist.conf
将 blacklist nouveau加到文件最后

#重启电脑
#按Ctrl+Alt+F1进入命令行界面
# 提示，以下命令可以写成一个 .sh 文件
sudo service lightdm stop
sudo rm /tmp/.X0-lock  (删除此文件，如果本来就没有的话，就会报错)
sudo apt-get remove --purge nvidia-*  

sudo rm /etc/X11/xorg.conf
sudo apt-add-repository ppa:graphics-drivers/ppa -y

sudo apt-get update
sudo apt-get install nvidia-375 nvidia-settings nvidia-prime 
sudo apt-get install mesa-common-dev 
sudo apt-get install freeglut3-dev 
sudo ldconfig -n 
sudo update-initramfs -u

#重启电脑，就可以进入界面了， 如果进不去，进入BIOS启动页面，在Boot（或Security）中找到Security BOOT ，将其disable就可以了
#查看驱动
nvidia-smi 会输出显卡信息
```

## 安装 cuda8.0

```bash
# 官网下载cuda.run 文件
chmod +x cuda8*  
sudo ./cuda8*
# 询问 是否安装 graphic driver的时候 要选择 no，因为之前已经安装好显卡驱动了 

# 安装完之后配置 CUDA 环境
sudo vim /etc/profile
# 文件尾加入
export PATH=/usr/local/cuda/bin：$PATH  
export LD_LIBRARY_PATH=/usr/local/cuda/lib64:$LD_LIBRARY_PATH
# 保存退出，重启电脑
```

## 测试 cuda8.0是否安装成功

```bash
cd /usr/local/cuda/samples/1_Utilities/deviceQuery
sudo make
sudo ./deviceQuery

# 再测试试一下nobody：
cd ../../5_Simulations/nbody/
sudo make

# 执行：
./nbody -benchmark -numbodies=256000 -device=0
```

如果都可以成功运行，说明CUDA安装没有问题

## 安装 cudnn5.1

官网下载 cudnn

```
# 解压
tar -zxvf ...
# 安装，其实也就是复制而已啦
sudo cp cuda/include/cudnn.h /usr/local/cuda/include/  
sudo cp cuda/lib64/libcudnn* /usr/local/cuda/lib64/  
sudo chmod a+r /usr/local/cuda/include/cudnn.h  
sudo chmod a+r /usr/local/cuda/lib64/libcudnn*
```

到此，GPU环境已经配置完成，可以用你喜欢的框架了

## 其它

**gcc 版本降级**

```
sudo apt-get install gcc-4.7 g++-4.7  
sudo update-alternatives --install  /usr/bin/gcc gcc /usr/bin/gcc-4.7 10  
sudo update-alternatives --install  /usr/bin/g++ g++ /usr/bin/g++-4.7 10 
# 10 代表优先级， 高的优先
```

**java8 安装**

```
sudo add-apt-repository ppa:webupd8team/java  
sudo apt-get update  
sudo apt-get install oracle-java8-installer
```

**安装bazel**

```
echo "deb [arch=amd64] http://storage.googleapis.com/bazel-apt stable jdk1.8" | sudo tee /etc/apt/sources.list.d/bazel.list  
curl https://storage.googleapis.com/bazel-apt/doc/apt-key.pub.gpg | sudo apt-key add -
sudo apt-get update && sudo apt-get install bazel
```

**可能碰到的错误：**

```
# 如果更新显卡驱动之前可以正常运行，但是更新显卡驱动之后出现了问题，可以尝试下面命令
sudo apt-get install nvidia-modprobe
```



