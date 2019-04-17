# 解决cannot find -lopencv_dep_cudart问题 - DumpDoctorWang的博客 - CSDN博客





2017年11月24日 18:28:05[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：884








# 一、问题来源

在使用OpenCV+CUDA编程的时候,在CmakeLists.txt，在语句
`find_package(CUDA REQUIRED)`
之前加上如下代码
`set(CUDA_USE_STATIC_CUDA_RUNTIME OFF)`
如下

```
set(CUDA_USE_STATIC_CUDA_RUNTIME OFF) 		#这一句解决 cannot find -lopencv_dep_cudart
find_package(CUDA REQUIRED)
```

**二  问题的解决**

在这个链接https://github.com/opencv/opencv/issues/6542中发现了关于这个问题的讨论, 发现其中有人说opencv_dep_cudart这个库实际上并没有用到,有人用一个假的库欺骗过了链接程序.



另外一个人,采用了下面的方法

sudo ln -s /usr/local/cuda/lib64/libcudart.so /usr/lib/libopencv_dep_cudart.so

也就是添加一个libcudart.so的软链接.确实能解决问题,但是我觉得这种方法并不靠谱,还是建议用在CmakeLists.txt中添加set(CUDA_USE_STATIC_CUDA_RUNTIME OFF)这个语句来解决问题.



