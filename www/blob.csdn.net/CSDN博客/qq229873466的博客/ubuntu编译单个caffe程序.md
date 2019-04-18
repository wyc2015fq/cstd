# ubuntu编译单个caffe程序 - qq229873466的博客 - CSDN博客

2016年12月01日 20:54:23[qq229873466](https://me.csdn.net/qq229873466)阅读数：682


                
1.创建CMakeLists.txt：

```
cmake_minimum_required(VERSION 2.8)

project( cf_mnist )
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb")
#SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
find_package( Caffe )
find_package( OpenCV REQUIRED )

include_directories(${Caffe_INCLUDE_DIRS})
add_definitions(${Caffe_DEFINITIONS})
add_executable( cf_mnist cf_mnist.cpp )

target_link_libraries( cf_mnist ${OpenCV_LIBS} ${Caffe_LIBRARIES})
```
2.进入./caffe/built运行cmake ..，生成CaffeConfig.cmake和caffe-config.cmkae等文件。

3.运行make,等待一段时间。

4.回到./caffe目录，运行下面命令创建caffe.pd.h文件：

protoc src/caffe/proto/caffe.proto --cpp_out=.  

mkdir include/caffe/proto  

mv src/caffe/proto/caffe.pb.h include/caffe/proto 

（proto相关参考[http://www.cnblogs.com/stephen-liu74/archive/2013/01/02/2841485.html](http://www.cnblogs.com/stephen-liu74/archive/2013/01/02/2841485.html)）

5.回到自己项目的目录，运行cmake . -DCMAKE_BUILD_TYPE=Debug/Release, make, 完成。

其他问题：

1.找不到<caffe/caffe.hpp>，删除CMakeCache.txt，重新cmake。

2.找不到libcaffe.so.x.x，进入/etc/ld.so.conf.d文件夹，新建文件caffe.conf，输入/build/lib（libcaffe.so.x.x所在路径）。

