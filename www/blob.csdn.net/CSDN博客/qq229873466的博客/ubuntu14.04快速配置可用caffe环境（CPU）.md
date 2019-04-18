# ubuntu14.04快速配置可用caffe环境（CPU） - qq229873466的博客 - CSDN博客

2017年03月06日 10:59:02[qq229873466](https://me.csdn.net/qq229873466)阅读数：287
个人分类：[caffe](https://blog.csdn.net/qq229873466/article/category/6551642)



系统：ubuntu-14.04-desktop-amd64.iso

# **1. 安装开发所需的依赖包**

```cpp
sudo apt-get update
sudo apt-get install build-essential # 注1
sudo apt-get install libprotobuf-dev libleveldb-dev libsnappy-dev libopencv-dev libboost-all-dev libhdf5-serial-dev libgflags-dev libgoogle-glog-dev liblmdb-dev protobuf-compiler #
 注2
sudo apt-get install libatlas-base-dev
```

# **2 编译安装caffe**

从[https://github.com/BVLC/caffe](https://github.com/BVLC/caffe)下载caffe并解压，放在~目录下。

```cpp
cd ~/caffe-master
cp Makefile.config.example Makefile.config
```

编辑Makefile.config，将CPU_ONLY:=1前面的#去掉。

然后编译

```
make all -j4  
make test  
make runtest
```

说明：这种快速配置是不能调试代码的，要调试caffe需要在Makefile.config，将DEBUG:=1前面的#去掉；要调试opencv需要手动编译。

注意：

1.当出现下面的问题时，

“build-essential : 依赖: g++ (>= 4:4.4.3) 但是它将不会被安装

                   依赖: dpkg-dev (>= 1.13.5) 但是它将不会被安装

E: 无法修正错误，因为您要求某些软件包保持现状，就是它们破坏了软件包间的依赖关系”

执行

```cpp
sudo apt-get install aptitude
sudo  aptitude install build-essential #第一次选n，第二次y，这样就好了
```

2.若出现类似1的问题，处理同1,将apt-get改成aptitude。

参考：[2015.08.17 Ubuntu 14.04+cuda 7.5+caffe安装配置](http://blog.csdn.net/ubunfans/article/details/47724341)


