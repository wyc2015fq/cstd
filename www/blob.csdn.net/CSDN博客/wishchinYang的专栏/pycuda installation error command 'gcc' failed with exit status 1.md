# pycuda installation error: command 'gcc' failed with exit status 1 - wishchinYang的专栏 - CSDN博客
2017年12月04日 10:49:24[wishchin](https://me.csdn.net/wishchin)阅读数：1636
      原文：[python采坑之路](https://www.cnblogs.com/gerrydeng/p/7159021.html)
## Setup script exited with error: command 'gcc' failed with exit status 1
伴随出现“cuda.h” cannot be find 
由于没有正确安装Python开发环境导致。
### Debin/Ubuntu
- Python2
`sudo apt-get install python-dev`
- 
Python3
`sudo apt-get install python3-dev`
可能需要libevent库
`sudo apt-get install libevent-dev`
最后更新下开发环境
`sudo apt-get groupinstall 'development tools'`
### Centos/Fedora
`sudo yum install python-devel`
`sudo yum install libevent-devel`
`easy_install gevent`
或者
`pip install gevent`
把环境更新下
`sudo yum install groupinstall 'development tools'`
注意事项：
`sudo apt-get groupinstall 'development tools'`
不能使用
出错的地方在于没有添加cuda path，因此添加CUDApath
 sudo get edit ~./bashrc
添加
export PATH=/usr/local/cuda/bin:$PATH 
export LD_LIBRARY_PATH=/usr/local/cuda/lib64:$LD_LIBRARY_PATH 
export CUDA_ROOT=/usr/local/cuda
此后Pycuda安装成功。
