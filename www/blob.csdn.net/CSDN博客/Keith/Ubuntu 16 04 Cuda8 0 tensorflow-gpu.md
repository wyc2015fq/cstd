# Ubuntu 16.04 Cuda8.0 tensorflow-gpu - Keith - CSDN博客





2016年09月16日 11:34:21[ke1th](https://me.csdn.net/u012436149)阅读数：8009








**[此教程 年代久远， 请移步] [ubuntu16.04 安装 CUDA](http://blog.csdn.net/u012436149/article/details/74171047)**



**[此教程 年代久远， 请移步] [ubuntu16.04 安装 CUDA](http://blog.csdn.net/u012436149/article/details/74171047)**




**[此教程 年代久远， 请移步] [ubuntu16.04 安装 CUDA](http://blog.csdn.net/u012436149/article/details/74171047)**




**[此教程 年代久远， 请移步] [ubuntu16.04 安装 CUDA](http://blog.csdn.net/u012436149/article/details/74171047)**




**[此教程 年代久远， 请移步] [ubuntu16.04 安装 CUDA](http://blog.csdn.net/u012436149/article/details/74171047)**














**[11.09.2016 更新, 更新源之后,要进行upgrade]**

**[11.07.2016 更新  可能出现的错误]**

**[10.16.2016  更新]**

ubuntu 16.04  python 2.7  cuda7.5/Cuda8.0 tensorflow-gpu

最近在配深度学习的框架，配了一个星期，终于配好了，中间走了n条弯路。 





下面开始介绍安装：

1-> : 准备工作：
- 安装 sshserver : ubuntu默认是没有sshserver的，只有sshclient
```php
sudo apt-get install openssh-server openssh-client
```


- 配置源，在国内最好使用国内源，因为下载速度快。[国内源介绍](http://blog.csdn.net/bleachswh/article/details/51423585)
- 只需将你想使用的源 添加到 /etc/apt/sources.list 文件头部即可
- 然后sudo apt-get update
- sudo apt-get upgrade
- 安装依赖
```php
sudo apt-get install python-pip  
sudo apt-get install python-numpy swig python-dev python-wheel
```


```php
sudo apt-get install git vim
sudo apt-get install freeglut3-dev build-essential libx11-dev libxmu-dev libxi-dev libgl1-mesa-glx libglu1-mesa libglu1-mesa-dev
```


- 这里我碰到了一个问题，使用国内源的话，安装的pip无论怎么更新，都是pip8.1.1，不是pip8.1.2,（使用pip-8.1.1的话，在之后安装tensorflow的时候会报错，所以需要升级到8.1.2）[如何升级到pip8.1.2](https://pip.pypa.io/en/stable/installing/)
- 如果你用pip install软件的时候感觉速度很慢，你也可以设置一下pip源，（豆瓣源挺快的）
```php
cd $HOME
mkdir .pip
```

- 
```php
cd .pip
```

- 
```php
sudo vim pip.conf

在里面添加
[global]
index-url = http://mirrors.aliyun.com/pypi/simple
[install]
```

- 
```php
trusted-host = mirrors.aliyun.com
```

- 
```php
disable-pip-version-check = true
```

- 
```php
timeout = 6000
保存即可
```


2->:安装nvidia驱动程序：

在这里我花了大概有3.4天的时间，被坑的不要不要的，主要一个问题就是安装完驱动之后 登陆时候一直卡在登陆界面进不去。到最后看到一个人提到了关闭Secury Boot，然后我就将其关了，才进去的。
- 将nouveau加入黑名单
```php
sudo vim /etc/modprobe.d/blacklist.conf

将 blacklist nouveau加到文件最后
```


- 重启电脑


- 按Ctrl+Alt+F1进入命令行界面
```php
sudo service lightdm stop
sudo rm /tmp/.X0-lock  (删除此文件，如果本来就没有的话，就会报错)
```


- 安装驱动
```php
sudo apt-get remove --purge nvidia-*  

sudo rm /etc/X11/xorg.cong 
sudo apt-add-repository ppa:graphics-drivers/ppa -y

sudo apt-get update
sudo apt-get install nvidia-367 nvidia-settings nvidia-prime 
sudo apt-get install mesa-common-dev 
sudo apt-get install freeglut3-dev 
sudo ldconfig -n 
sudo update-initramfs -u
```



- 重启电脑，就可以进入界面了， 如果进不去，进入BIOS启动页面，在Boot（或Security）中找到Security BOOT ，将其disable就可以了
- 查看驱动
```php
nvidia-smi 会输出显卡信息
```




3-> 安装CUDA8.0
- 官网下载 CUDA8.0 .run 文件
- cuda7.5需要gcc-4.7 g++-4.7进行编译， 而Ubuntu默认是5.4版本，所以需要降版本， cuda8.0的话不需要降级
```php
sudo apt-get install gcc-4.7 g++-4.7  
sudo update-alternatives --install  /usr/bin/gcc gcc /usr/bin/gcc-4.7 10  
sudo update-alternatives --install  /usr/bin/g++ g++ /usr/bin/g++-4.7 10 

10 代表优先级， 高的优先
```


- 安装CUDA7.5，安装时会提示 unsorported configuration ,不用理会， 还会询问你要不要装显卡驱动选择n就可以了。 

```php
chmod +x cuda7*  
sudo ./cuda7*
```


- 配置CUDA环境
```python
~$ sudo vim .bashrc
文件尾加入
export PATH=/usr/local/cuda/bin：$PATH  
export LD_LIBRARY_PATH=/usr/local/cuda/lib64:$LD_LIBRARY_PATH
保存退出
然后执行:
source .bashrc
```

- 

- 测试

- 
```php
cd /usr/local/cuda-7.5/samples/1_Utilities/deviceQuery
make
sudo ./deviceQuery

再测试试一下nobody：
cd ../../5_Simulations/nbody/
make

执行：
./nbody -benchmark -numbodies=256000 -device=0
```


4->~~安装cudnn4.0~~ 安装cudnn5.1（官方tensorflow0.10.0 是用cuda7.5 和cudnn5.1编译的）
- 官网下载 [点击打开链接](https://developer.nvidia.com/rdp/cudnn-download)
- 使用tar -zxvf [filename] 解压
- 配置cudnn
```php
sudo cp cuda/include/cudnn.h /usr/local/cuda/include/  
sudo cp cuda/lib64/libcudnn* /usr/local/cuda/lib64/  
sudo chmod a+r /usr/local/cuda/include/cudnn.h  
sudo chmod a+r /usr/local/cuda/lib64/libcudnn*
```



5->安装bazel（如果你不打算编译源码安装tensorflow的话，是不需要安装bazel的。我们这里不使用源码安装tensorflow。）
- 安装bazel需要先安装java8
```php
$ sudo add-apt-repository ppa:webupd8team/java  
$ sudo apt-get update  
$ sudo apt-get install oracle-java8-installer
```


- 安装bazel
```php
$ echo "deb [arch=amd64] http://storage.googleapis.com/bazel-apt stable jdk1.8" | sudo tee /etc/apt/sources.list.d/bazel.list  
$ curl https://storage.googleapis.com/bazel-apt/doc/apt-key.pub.gpg | sudo apt-key add -  
$ sudo apt-get update && sudo apt-get install bazel  
$ sudo apt-get upgrade bazel
```


- bazel命令需要工作在workspace文件夹或其子文件夹下

6->安装tensorflow
- 
```php
# Ubuntu/Linux 64-bit, CPU only, Python 2.7:
$ sudo pip install --upgrade https://storage.googleapis.com/tensorflow/linux/cpu/tensorflow-0.10.0-cp27-none-linux_x86_64.whl

# Ubuntu/Linux 64-bit, GPU enabled, Python 2.7. Requires CUDA toolkit 7.5 and CuDNN v5.1.
# For other versions, see "Install from sources" below.
$ sudo pip install --upgrade https://storage.googleapis.com/tensorflow/linux/gpu/tensorflow-0.10.0-cp27-none-linux_x86_64.whl
```


- 
```php
或者去github上下载相应版本， 用pip install 文件名 就可以了
```

- 然后就可以import tensorflow 测试一下啦


7->可能出现的错误:


- 如果更新了 nvidia驱动之后报错(更新之前运行正常),可以尝试安装 
- sudo apt-get install nvidia-modprobe
- 如果使用pycharm的话，进入pycharm根目录的bin/，打开pycharm.sh，然后在第180行左右插入
```
LD_LIBRARY_PATH="/usr/local/cuda/lib64" #需要增加
LD_LIBRARY_PATH="$IDE_BIN_HOME:$LD_LIBRARY_PATH" "$JAVA_BIN"
```


报错信息:


# failed call to cuInit: CUDA_ERROR_UNKNOWN in python programs using Ubuntu bumblebee








