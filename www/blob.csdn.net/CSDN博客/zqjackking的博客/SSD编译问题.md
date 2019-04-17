# SSD编译问题 - zqjackking的博客 - CSDN博客





2017年09月04日 23:07:05[zqjackking](https://me.csdn.net/zqjackking)阅读数：362








编译SSD的时候，make all -j16 

出现了以下错误：

```
/usr/include/boost/property_tree/detail/json_parser_read.hpp:257:264: error: ‘type name’ declared as function returning an array
make: *** [.build_release/cuda/src/caffe/layers/detection_output_layer.o] Error 1
```

解决方案：

```
easy workaround (sudo not required!):

in <caffe_root>/src/caffe/layers/detection_output_layer.cu and .cpp: search for write_json and comment it out

in <caffe_root>/include/caffe/layers/detection_output_layer.hpp:
//#include <boost/property_tree/json_parser.hpp>

and then
make clean
make -j8

as far as I know, write_json is only used to write results in VOC format ...
```



