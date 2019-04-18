# HDF5文件读写 - qq_32515081的博客 - CSDN博客
2018年03月15日 22:03:17[TengAdventure](https://me.csdn.net/qq_32515081)阅读数：461
做实验需要跑数据，caffe库要求数据格式为HDF5.
HDF5数据的import与export可以通过matlab简单完成。
HDFView可以简单查看HDF5数据。
1.h5disp命令可以查看HDF5数据格式
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 >> h5disp('test.h5')
 2 HDF5 test.h5 
 3 Group '/' 
 4     Dataset 'data' 
 5         Size:  4x2500
 6         MaxSize:  4x2500
 7         Datatype:   H5T_IEEE_F64LE (double)
 8         ChunkSize:  2x625
 9         Filters:  deflate(1)
10     Dataset 'label' 
11         Size:  2500
12         MaxSize:  2500
13         Datatype:   H5T_IEEE_F32LE (single)
14         ChunkSize:  2500
15         Filters:  deflate(1)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
2.h5read命令可以导出HDF5数据
```
1 >> testdata = h5read('test.h5','/data');
2 >> testlabel = h5read('test.h5','/label');
```
3.h5create和h5write命令可以创建和写入HDF5数据
```
1 >> testdata = uint8(magic(5));
2 >> h5create('my_example.h5','/dataset1',size(testdata));
3 >> h5write('my_example.h5','/dataset1',testdata)
```
注意：HDFView与MATLAB的数据显示方法不完全一样，两者数组互为转置。
