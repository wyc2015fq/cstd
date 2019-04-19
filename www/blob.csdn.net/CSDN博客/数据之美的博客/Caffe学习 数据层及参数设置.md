# Caffe学习 数据层及参数设置 - 数据之美的博客 - CSDN博客
2017年03月25日 18:18:12[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：1110
caffe的各种数据层在caffe.proto文件中有定义。通过对定义的caffe.proto文件进行编译，产生支持各种层操作的c++代码。后面将会详细解读caffe.proto文件（在caffe里就是当做一个自动代码生成工具来用）。
本文主要介绍caffe可以读入数据的各种格式，方便后面采用caffe训练自己的数据
数据的来源主要有以下几种：
（1）高效的[数据库](http://lib.csdn.net/base/mysql)（LevelDB或者LMDB）
（2）内存
（3）硬盘文件，HDF5格式或者图片格式的文件。此种方式效率较差，目前一般是先将原生数据转换为（1）中的数据库格式，然后再来处理
**一.不同种类数据层的共同属性**
绝大部分数据层在设置时，都可以先对数据进行一定的预处理，包括归一化scale，去中心化（减去平均值），水平镜像flip，随机裁剪crop等四种预处理方式。
该四种预处理方式可以靠该Layer的transform_params属性（HDF5 Layer没有该属性。。。）来指定。指定方式如下：
**[plain]**[view
 plain](http://blog.csdn.net/u012177034/article/details/52134205#)[copy](http://blog.csdn.net/u012177034/article/details/52134205#)
- transform_param {  
-   # randomly horizontally mirror the image  
-   mirror: 1  
-   # crop a `crop_size` x `crop_size` patch:  
-   # - at random during training  
-   # - from the center during testing  
-   crop_size: 227  
-   # substract mean value（RGB three channel）: these mean_values can equivalently be replaced with a mean.binaryproto file as  
-   # mean_file: name_of_mean_file.binaryproto  
-   mean_value: 104  
-   mean_value: 117  
-   mean_value: 123  
- }  
数据预处理只是数据层transfor_params属性的一部分，其余transform_params属性可以在caffe.proto文件里的Message类型：TransformationParameter下查找。
**二.数据来源于数据库（LevelDB或者LMDB）**
LMDB：Lightning MemoryMapped Databases，由于Caffe的文件读取方式使得该格式的数据输入最适用于1-K分类问题
层参数设置实例：
**[plain]**[view
 plain](http://blog.csdn.net/u012177034/article/details/52134205#)[copy](http://blog.csdn.net/u012177034/article/details/52134205#)
- layer {  
-   name: "data"  
-   type: "Data"  
-   top: "data"  
-   top: "label"  
-   include {  
-     phase: TRAIN  
-   }  
-   transform_param {  
-     mirror: 1  
-     crop_size: 227  
-     mean_value: 104  
-     mean_value: 117  
-     mean_value: 123  
-   }  
-   data_param {  
-     source: "examples/imagenet/ilsvrc12_train_lmdb"  
-     batch_size: 32  
-     backend: LMDB  
-   }  
- }  
data_param里必须设置的参数有
（1）source：数据库文件名
（2）batch_size：每次处理的样本数目
可选参数：
（1）backend：数据库类型，默认为LMDB，可选LevelDB
（2）rand_skip：在开始的时候跳过rand_skip个输入数据，这个对异步SGD有效
减去均值的方法可以由另一种方法代替，由（一）中的代码示意可看出。
其中.binaryproto文件可由下述方式得到，由LMDB数据库计算得到均值文件
**[plain]**[view
 plain](http://blog.csdn.net/u012177034/article/details/52134205#)[copy](http://blog.csdn.net/u012177034/article/details/52134205#)
- cd ~/caffe  
- build/tools/compute_image_mean examples/imagenet/ilsvr12_train_lmdb   
- data/ilsvrc12/imagenet_mean.binaryproto  
**三.数据来源于原生图像文件**
直接从文本文件读入所有要处理的图像文件的路径与label标签
层参数设置实例：
**[plain]**[view
 plain](http://blog.csdn.net/u012177034/article/details/52134205#)[copy](http://blog.csdn.net/u012177034/article/details/52134205#)
- layer {  
-   name: "data"  
-   type: "ImageData"  
-   top: "data"  
-   top: "label"  
-   include {  
-     phase: TRAIN  
-   }  
-   transform_param {  
-     mirror: true  
-     crop_size: 227  
-     mean_value: 104  
-     mean_value: 117  
-     mean_value: 123  
-   }  
-   image_data_param {  
-     source: "/path/to/file/train.txt"  
-     batch_size: 32  
-     <span style="color:#3333FF;">shuffle: 1</span>  
-   }  
- }  
层的类型为ImageData，图像和标签文件来源为txt文本，文本内容如下所示，其中最后一列为该图像的标签
**[plain]**[view
 plain](http://blog.csdn.net/u012177034/article/details/52134205#)[copy](http://blog.csdn.net/u012177034/article/details/52134205#)
- /path/to/images/img3423.jpg 2  
- /path/to/images/img3424.jpg 13  
- /path/to/images/img3425.jpg 8  
- ...  
image_data_params参数必须设置的有：
（1）source：txt文件名
可选参数：
（1）batch_size：每次输入处理的图像个数，默认为1
（2）new_height:图像resize之后的height，默认为0，表示忽略
（3）new_width：图像resize之后的width，默认为0，表示忽略
（4）shuffle：是否随机打乱数据，默认为0，表示忽略
（5）rand_skip：同数据库层设置
**四.图像来源于内存**
可以很快的从内存中直接读取数据。
使用该方法来读入数据时，可以靠调用`MemoryDataLayer::Reset` (from C++) or`Net.set_input_arrays` (from [Python](http://lib.csdn.net/base/python))来指定一个具体的数据地址，如通常的存放所有输入数据的四维数组的首地址，这样就可以每次从改地址内读取batch_size大小的数据。
该方法也有可能会比较慢，如果使用之前先要将数据复制到内存中的话。
目前还不太清楚这种功能最适合在什么场合。。。
层参数设置实例：
**[plain]**[view
 plain](http://blog.csdn.net/u012177034/article/details/52134205#)[copy](http://blog.csdn.net/u012177034/article/details/52134205#)
- layer {  
-   top: "data"  
-   top: "label"  
-   name: "memory_data"  
-   type: "MemoryData"  
-   memory_data_param{  
-     batch_size: 2  
-     height: 100  
-     width: 100  
-     channels: 1  
-   }  
-   transform_param {  
-     scale: 0.0078125  
-     mean_file: "mean.proto"  
-     mirror: false  
-   }  
- }  
batch_size,height,width,channels都是使用该层时所必须要设置的参数，这样才能正确的从内存中读入数据
**五.数据来源于HDF5格式**
不太适合图像，因为HDF5格式数据采用FP32格式的数据，而图像采用的是uint8，因此若将文件转为该格式则会很大。与其他层不同，该层没有transform_params属性。一般不要使用～～～
层设置实例：
**[plain]**[view
 plain](http://blog.csdn.net/u012177034/article/details/52134205#)[copy](http://blog.csdn.net/u012177034/article/details/52134205#)
- layer {  
-   name: "data"  
-   type: "HDF5_DATA"  
-   top: "data"  
-   top: "label"  
-   include {  
-     phase: TRAIN  
-   }  
-   hdf5_data_param {  
-     source: "examples/hdf5_classification/data/train.txt"  
-     batch_size: 32  
-   }  
- }  
hdf5_data_params必须设置的参数：
（1）batch_size
(2)source
可选设置
（1）shuffle：默认为0，表示忽略
**六.数据来源于原始图片的窗口Windows**
最适合目标检测任务，因为目标检测的训练样本都是标注好的窗口，而不是整张图像。这也是我目前最需要的～～～
训练时是针对每一个窗口进行训练，而不是一个图像。
层设置实例：
**[plain]**[view
 plain](http://blog.csdn.net/u012177034/article/details/52134205#)[copy](http://blog.csdn.net/u012177034/article/details/52134205#)
- layers {  
-   name: "data"  
-   type: "WindowsData"  
-   top: "data"  
-   top: "label"  
-   window_data_param {  
-     source: "/path/to/file/window_train.txt"  
- 
-     mean_file: "data/ilsvrc12/imagenet_mean.binaryproto"  
-     batch_size: 128  
-     mirror: true  
-     crop_size: 227  
-     fg_threshold: 0.5  
-     bg_threshold: 0.5  
-     fg_fraction: 0.25  
-     context_pad: 16  
-   }  
- }  
其中window_train.txt文件实例如下：
**[plain]**[view
 plain](http://blog.csdn.net/u012177034/article/details/52134205#)[copy](http://blog.csdn.net/u012177034/article/details/52134205#)
- # 0  
- /home/xxx/0001.jpg  
- 641  
- 7  
- 1 1.0 353 356 393 396  
- 1 0.5 338 344 379 384  
- 2 0.7 339 336 379 376  
- 5 0 334 330 374 370  
- 4 1.0 330 324 370 364  
- 4 1.0 335 319 375 359  
- 4 1.0 341 313 381 353  
- # 1  
- /home/xxx/0002.jpg  
- 600  
- 3  
- 2 1.0 353 356 393 396  
- 2 0.5 338 344 379 384  
- 3 0.7 339 336 379 376  
其中第一行是图片的index, 从0开始, 接下来三行依此是图片的channel, height, width, 接下来一行表示 bounding box 数量. 再接下来的每一行都是一个bounding box, 第一个数字表示label, 第二个数字表示与真实goundtruth 的overlap, 接下来的四个数字表示x1, y1, x2, y2。负样本的标签可以随意指定。
该层必须要设置的参数：
（1）source：包含原始图像，窗口位置大小，窗口类别的文本文件
（2）mean_file：整张图像的的mean_file
（3）batch_szie
可选参数：
（1）mirror
（2）crop_size：裁剪的窗口的大小
（3）crop_mode：裁剪方式，“warps”代表将窗口固定大小crop_size，“square”表示能够包围窗口的最小正方形；默认“warp”
（4）fg_threshold：foreground overlap threshold ,默认0.5，代表只有BoundingBox与GroundTruth的重合面积比大于0.5时才认为是正样本
（5）bg_threshold：background overlap threshold，默认0.5，代表只有BoundingBox与GroundTruth的重合比例小于0.5时才认为是负样本
（6）fg_fraction：默认0.25，一个batch里正样本窗口的比例
（7）context_pad：默认10个像素点，代表输入窗口数据的时候会自动在窗口周围数据补充10个像素点，像素值填充0.
如下图所示，最外围的一圈即为context填充，此时context_pad为1：
![](https://img-blog.csdn.net/20160806130507545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**七.其他数据Layer**
“Input”type：常用来[测试](http://lib.csdn.net/base/softwaretest)网络的前向和后向传递时间
“DummyData”type：常用来debug，也可以用来测试网络传递时间
DummyData实例
**[plain]**[view
 plain](http://blog.csdn.net/u012177034/article/details/52134205#)[copy](http://blog.csdn.net/u012177034/article/details/52134205#)
- layer {  
-   name: "data"  
-   type: "DummyData"  
-   top: "data"  
-   include {  
-     phase: TRAIN  
-   }  
-   dummy_data_param {  
-     data_filler {  
-       type: "constant"  
-       value: 0.01  
-     }  
-     shape {  
-       dim: 32  
-       dim: 3  
-       dim: 227  
-       dim: 227  
-     }  
-   }  
- }  
- layer {  
-   name: "data"  
-   type: "DummyData"  
-   top: "label"  
-   include {  
-     phase: TRAIN  
-   }  
-   dummy_data_param {  
-     data_filler {  
-       type: "constant"  
-     }  
-     shape {  
-       dim: 32  
-     }  
-   }  
- }  
在这个例子中，有两个数据层，一个blob一个层，data一个，label一个。在HDF5，Data数据库，ImageData，都是data和label放在一个层里。这样方便调试。
最后放一张前面的各种数据读入的类的继承关系～
![](https://img-blog.csdn.net/20160806135605864?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
