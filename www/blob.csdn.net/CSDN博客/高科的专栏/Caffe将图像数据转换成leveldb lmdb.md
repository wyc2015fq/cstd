
# Caffe将图像数据转换成leveldb-lmdb - 高科的专栏 - CSDN博客

2018年10月08日 16:45:47[高科](https://me.csdn.net/pbymw8iwm)阅读数：81


Caffe中convert_imageset projrct将图像数据转换成Caffe能读取的数据格式leveldb/lmdb
`-gray=true                    //whether read gray image
-shuffle=true                 //whether mix order
-resize_height=28 
-resize_width=28 
-backend=lmdb                    //leveldb/lmdb
 -check_size=false 
 -encoded=true 
 -encode_type=jpg 
 E:/MyCode/DL/caffe-master/examples/images/ 
 E:/MyCode/DL/caffe-master/examples/convert_imageset/images_list.txt  
//该文件表示的是图像//对应的标签，格式如下：
//cat gray.jpg 1
//cat.jpg 1
//cat_gray.jpg 1
//fish-bike.jpg 2
 E:/MyCode/DL/caffe-master/examples/convert_imageset/img_convert/ 
//该路径下为生成的文件，若之前已经生成过，再次执行代码时要确保没有重复文件，否则会报错`因此我执行的时候使用的是./.build_release/tools/convert_imageset -gray=true  -shuffle=true     -resize_height=28 -resize_width=28  -backend=lmdb  -check_size=false  -encoded=true  -encode_type=jpg  ~/caffe/netImage/ ~/caffe/netImage/list.txt ~/caffe/netImage1
这样最终生成的mdb文件在netImage1里存放：
![](https://img-blog.csdn.net/2018100816454277?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3BieW13OGl3bQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

