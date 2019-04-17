# Error: libcaffe2.so: cannot open shared object file: No such file or directory - Snoopy_Dream - CSDN博客





2018年03月24日 11:53:41[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：1888








## 按照caffe2官方安装教程，在执行python2 test_spatial_narrow_as_op.py出现问题：

**WARNING:root:This caffe2 python run does not have GPU support. Will run in CPU only mode. 

 WARNING:root:Debug message: libcaffe2_gpu.so: cannot open shared object file: No such file or directory  

CRITICAL:root:Cannot load caffe2.python. Error: libcaffe2.so: cannot open shared object file: No such file or directory**
## 解决方法:


```
cd /usr/local/lib 

 sudo ldconfig
```


## 问题分析：

显示.so文件的问题，表示不是程序文件本身的问题，而是动态链接出了问题。 

1.需要`cd /usr/local/lib`，看一看你的这个文件夹里有没有warning中提示有错的那个.so文件。如果没有，在你的安装路径查找，在lib文件夹下面，把这个拷贝到/usr/local/lib下。 

2.然后`sudo ldconfig`
importError: libopencv_core.so.3.1: cannot open shared object file: No such file or directory 

sudo ldconfig   就好

**题外话：可以具体自己再查看下ldconfig命令的作用，学习学习～～**





