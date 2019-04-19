# caffe训练流程 - 数据之美的博客 - CSDN博客
2017年03月25日 13:27:36[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：653
个人分类：[caffe&tensorflow&keras																[深度学习/机器学习](https://blog.csdn.net/oppo62258801/article/category/6739087)](https://blog.csdn.net/oppo62258801/article/category/6739085)
转载自：[深度学习](http://lib.csdn.net/base/deeplearning)（六）caffe入门学习 - hjimce的专栏 - 博客频道 - CSDN[.NET](http://lib.csdn.net/base/dotnet)
http://blog.csdn[.Net](http://lib.csdn.net/base/dotnet)/hjimce/article/details/48933813
本文主要讲解caffe的整个使用流程，适用于初级入门caffe，通过学习本篇博文，理清项目训练、[测试](http://lib.csdn.net/base/softwaretest)流程。初级教程，高手请绕道。
我们知道，在caffe编译完后，在caffe目录下会生成一个build目录，在build目录下有个tools，这个里面有个可执行文件caffe，如下图所示：
![](https://img-blog.csdn.net/20151006224517966?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
有了这个可执行文件我们就可以进行模型的训练，只需要学会调用这个可执行文件就可以了，这便是最简单的caffe学习，不需要对caffe底层的东西懂太多，只需要会调参数，就可以构建自己的网络，然后调用这个可执行文件就可以进行训练，当然如果你不仅仅是调参数，而且想要更改相关的[算法](http://lib.csdn.net/base/datastructure)，那就要深入学习caffe的底层函数调用了，这个以后再讲。本篇博文仅适合于刚入门学习caffe，高手请绕道。废话不多说，回归正题：
**一、总流程**
完成一个简单的自己的网络模型训练预测，主要包含几个步骤：
**1、数据格式处理，**也就是把我们的图片.jpg,.png等图片以及标注标签,打包在一起，搞成caffe可以直接方便调用的文件。后面我将具体讲解如何打包自己的数据，让caffe进行调用。
**2、编写网络结构文件，**这个文件的后缀格式是.prototxt。就是编写你的网络有多少层，每一层有多少个特征图，输入、输出……。看个例子，看一下caffe-》example-》mnist-》lenet_train_test.prototxt。这个便是手写字体网络结构文件了，我们需要根据自己的需要学会修改这个文件：
**[cpp]**[view
 plain](http://blog.csdn.net/hjimce/article/details/48933813#)[copy](http://blog.csdn.net/hjimce/article/details/48933813#)
- <span style="font-size:18px;">name: "LeNet"
- layer {  
-   name: "mnist"
-   type: "Data"//data层
-   top: "data"
-   top: "label"
-   include {  
-     phase: TRAIN   //训练阶段
-   }  
-   transform_param {  
-     scale: 0.00390625   //对所有的图片归一化到0~1之间，也就是对输入数据全部乘以scale，0.0039= 1/255
-   }  
-   data_param {  
-     source: "examples/mnist/mnist_train_lmdb"//训练数据图片路径
-     batch_size: 64    //每次训练采用的图片64张，min-batch
-     backend: LMDB  
-   }  
- }  
- layer {  
-   name: "mnist"
-   type: "Data"
-   top: "data"
-   top: "label"
-   include {  
-     phase: TEST   //测试
-   }  
-   transform_param {  
-     scale: 0.00390625  
-   }  
-   data_param {  
-     source: "examples/mnist/mnist_test_lmdb"//测试数据图片路径
-     batch_size: 100  
-     backend: LMDB  
-   }  
- }  
- layer {  
-   name: "conv1"//卷积神经网络的第一层，卷积层
-   type: "Convolution"//这层操作为卷积
-   bottom: "data"//这一层的前一层是data层
-   top: "conv1"//
-   param {  
-     lr_mult: 1     
-   }  
-   param {  
-     lr_mult: 2  
-   }  
-   convolution_param {  
-     num_output: 20    //定义输出特征图个数
-     kernel_size: 5    //定义卷积核大小
-     stride: 1  
-     weight_filler {  
-       type: "xavier"
-     }  
-     bias_filler {  
-       type: "constant"
-     }  
-   }  
- }  
- layer {  
-   name: "pool1"
-   type: "Pooling"//池化层，这一层的操作为池化
-   bottom: "conv1"//这一层的前面一层名字为：conv1
-   top: "pool1"
-   pooling_param {  
-     pool: MAX   //最大池化
-     kernel_size: 2  
-     stride: 2  
-   }  
- }  
- layer {  
-   name: "conv2"
-   type: "Convolution"
-   bottom: "pool1"
-   top: "conv2"
-   param {  
-     lr_mult: 1  
-   }  
-   param {  
-     lr_mult: 2  
-   }  
-   convolution_param {  
-     num_output: 50  
-     kernel_size: 5  
-     stride: 1  
-     weight_filler {  
-       type: "xavier"
-     }  
-     bias_filler {  
-       type: "constant"
-     }  
-   }  
- }  
- layer {  
-   name: "pool2"
-   type: "Pooling"
-   bottom: "conv2"
-   top: "pool2"
-   pooling_param {  
-     pool: MAX  
-     kernel_size: 2  
-     stride: 2  
-   }  
- }  
- layer {  
-   name: "ip1"
-   type: "InnerProduct"
-   bottom: "pool2"
-   top: "ip1"
-   param {  
-     lr_mult: 1  
-   }  
-   param {  
-     lr_mult: 2  
-   }  
-   inner_product_param {  
-     num_output: 500  
-     weight_filler {  
-       type: "xavier"
-     }  
-     bias_filler {  
-       type: "constant"
-     }  
-   }  
- }  
- layer {  
-   name: "relu1"
-   type: "ReLU"
-   bottom: "ip1"
-   top: "ip1"
- }  
- layer {  
-   name: "ip2"
-   type: "InnerProduct"
-   bottom: "ip1"
-   top: "ip2"
-   param {  
-     lr_mult: 1  
-   }  
-   param {  
-     lr_mult: 2  
-   }  
-   inner_product_param {  
-     num_output: 10  
-     weight_filler {  
-       type: "xavier"
-     }  
-     bias_filler {  
-       type: "constant"
-     }  
-   }  
- }  
- layer {  
-   name: "accuracy"
-   type: "Accuracy"
-   bottom: "ip2"
-   bottom: "label"
-   top: "accuracy"
-   include {  
-     phase: TEST  
-   }  
- }  
- layer {  
-   name: "loss"
-   type: "SoftmaxWithLoss"
-   bottom: "ip2"
-   bottom: "label"
-   top: "loss"
- }</span>  
上面的网络结构，定义的data层，就是定义我们输入的训练数据的路径、图片变换等。
**3、网络求解文件，**这个文件我们喜欢把它取名为：solver.prototxt，这个文件的后缀格式也是.prototxt。这个文件主要包含了一些求解网络，梯度下降参数、迭代次数等参数……，看下手写字体的solver.prototxt文件：
**[cpp]**[view
 plain](http://blog.csdn.net/hjimce/article/details/48933813#)[copy](http://blog.csdn.net/hjimce/article/details/48933813#)
- <span style="font-size:18px;">net: "examples/mnist/lenet_train_test.prototxt"//定义网络结构文件，也就是我们上一步编写的文件
- 
- test_iter: 100   
- 
- test_interval: 500 //每隔500次用测试数据，做一次验证
- 
- base_lr: 0.01     //学习率
- momentum: 0.9   //动量参数
- weight_decay: 0.0005   //权重衰减系数
- 
- lr_policy: "inv"//梯度下降的相关优化策略
- gamma: 0.0001  
- power: 0.75  
- 
- display: 100  
- 
- max_iter: 10000   //最大迭代次数
- 
- snapshot: 5000    //每迭代5000次，保存一次结果
- snapshot_prefix: "examples/mnist/lenet"//保存结果路径
- 
- solver_mode: GPU   //训练硬件设备选择GPU还是CPU</span>
这个文件的输入就是我们前面一步定义的网络结构。
**4、编写网络求解文件后**，我们可以说已经完成了CNN网络的编写。接着我们需要把这个文件，作为caffe的输入参数，调用caffe可执行文件，进行训练就可以了。具体的命令如下：
**[cpp]**[view
 plain](http://blog.csdn.net/hjimce/article/details/48933813#)[copy](http://blog.csdn.net/hjimce/article/details/48933813#)
- <span style="font-size:18px;">./build/tools/caffe train --solver=examples/mnist/lenet_solver.prototxt</span>  
这样就完事了，程序就开始训练了。上面的第一个参数caffe，就是我们在编译caffe，生成的可执行文件：
![](https://img-blog.csdn.net/20151006234025519?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后solver就是我们在步骤3编写的solver文件了，只要在ubuntu终端输入上面的命令，就可以开始训练了。
回想一下文件调用过程：首先caffe可执行文件，调用了solver.prototxt文件，而这个文件又调用了网络结构文件lenet_train_test.prototxt，然后lenet_train_test.prototxt文件里面又会调用输入的训练图片数据等。因此我们如果要训练自己的模型，需要备好3个文件：数据文件lmdb(该文件包含寻数据)、网络结构lenet_train_test.prototxt、求解文件solver.prototxt，这几个文件名随便，但是文件后缀格式不要随便乱改。把这三个文件放在同一个目录下，然后在终端输入命令，调用caffe就可以开始训练了。
**二、相关细节**
**1、lmdb数据格式生成**
caffe输入训练图片数据我比较喜欢用lmdb格式，好像还有另外一种格式leveldb，这个具体没用过，这里主要讲解lmdb格式数据的制作。其实在caffe-》example-》imagenet文件夹下面的一些脚本文件可以帮助我们快速生产相关的caffe所需的数据。
![](https://img-blog.csdn.net/20151007000348386?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
create_imagenet.sh这个文件可以帮我们快速的生成lmdb的数据格式文件，因此我们只需要把这个脚本文件复制出来，稍作修改，就可以对我们的训练图片、标注文件进行打包为lmdb格式文件了。制作图片的脚本文件如下：
**[python]**[view
 plain](http://blog.csdn.net/hjimce/article/details/48933813#)[copy](http://blog.csdn.net/hjimce/article/details/48933813#)
- <span style="font-size:18px;">#!/usr/bin/env sh
- # Create the imagenet lmdb inputs
- # N.B. set the path to the imagenet train + val data dirs
- 
- EXAMPLE=.          # 生成模型训练数据文化夹
- TOOLS=../../build/tools                              # caffe的工具库，不用变
- DATA=.                  # python脚步处理后数据路径
- 
- TRAIN_DATA_ROOT=train/  #待处理的训练数据图片路径
- VAL_DATA_ROOT=val/      # 带处理的验证数据图片路径
- 
- 
- 
- # Set RESIZE=true to resize the images to 256x256. Leave as false if images have
- # already been resized using another tool.
- RESIZE=true   #图片缩放
- if $RESIZE; then  
-   RESIZE_HEIGHT=256
-   RESIZE_WIDTH=256
- else
-   RESIZE_HEIGHT=0
-   RESIZE_WIDTH=0
- fi  
- 
- if [ ! -d "$TRAIN_DATA_ROOT" ]; then  
-   echo "Error: TRAIN_DATA_ROOT is not a path to a directory: $TRAIN_DATA_ROOT"
-   echo "Set the TRAIN_DATA_ROOT variable in create_imagenet.sh to the path" \  
- "where the ImageNet training data is stored."
-   exit 1
- fi  
- 
- if [ ! -d "$VAL_DATA_ROOT" ]; then  
-   echo "Error: VAL_DATA_ROOT is not a path to a directory: $VAL_DATA_ROOT"
-   echo "Set the VAL_DATA_ROOT variable in create_imagenet.sh to the path" \  
- "where the ImageNet validation data is stored."
-   exit 1
- fi  
- 
- echo "Creating train lmdb..."
- 
- GLOG_logtostderr=1 $TOOLS/convert_imageset \  
-     --resize_height=$RESIZE_HEIGHT \  
-     --resize_width=$RESIZE_WIDTH \  
-     --shuffle \  
-     $TRAIN_DATA_ROOT \  
-     $DATA/train.txt \     #标签训练数据文件
-     $EXAMPLE/train_lmdb  
- 
- echo "Creating val lmdb..."
- 
- GLOG_logtostderr=1 $TOOLS/convert_imageset \  
-     --resize_height=$RESIZE_HEIGHT \  
-     --resize_width=$RESIZE_WIDTH \  
-     --shuffle \  
-     $VAL_DATA_ROOT \  
-     $DATA/val.txt \    #验证集标签数据
-     $EXAMPLE/val_lmdb  
- 
- echo "Done."</span>  
同时我们需要制作如下四个文件：
**1、文件夹train**，用于存放训练图片
**2、文件夹val，**用于存放验证图片
**3、文件train.txt**，里面包含这每张图片的名称，及其对应的标签。
**[python]**[view
 plain](http://blog.csdn.net/hjimce/article/details/48933813#)[copy](http://blog.csdn.net/hjimce/article/details/48933813#)
- <span style="font-size:18px;">first_batch/train_female/992.jpg1
- first_batch/train_female/993.jpg1
- first_batch/train_female/994.jpg1
- first_batch/train_female/995.jpg1
- first_batch/train_female/996.jpg1
- first_batch/train_female/997.jpg1
- first_batch/train_female/998.jpg1
- first_batch/train_female/999.jpg1
- first_batch/train_male/1000.jpg0
- first_batch/train_male/1001.jpg0
- first_batch/train_male/1002.jpg0
- first_batch/train_male/1003.jpg0
- first_batch/train_male/1004.jpg0
- first_batch/train_male/1005.jpg0
- first_batch/train_male/1006.jpg0
- first_batch/train_male/1007.jpg0
- first_batch/train_male/1008.jpg0</span>  
上面的标签编号:1，表示女。标签：0，表示男。
**4、文件val.txt**，同样这个文件也是保存图片名称及其对应的标签。
这四个文件在上面的脚本文件中，都需要调用到。制作玩后，跑一下上面的脚本文件，就ok了，跑完后，即将生成下面两个文件夹：
![](https://img-blog.csdn.net/20151119200703655?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
文件夹下面有两个对应的文件：
![](https://img-blog.csdn.net/20151119201109080?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
制作完后，要看看文件的大小，有没有问题，如果就几k，那么正常是每做好训练数据，除非你的训练图片就几张。
**二、训练**
**1、直接训练法**
**[python]**[view
 plain](http://blog.csdn.net/hjimce/article/details/48933813#)[copy](http://blog.csdn.net/hjimce/article/details/48933813#)
- <span style="font-size:18px;">#!/usr/bin/env sh
- TOOLS=../cafferead/build/tools  
- $TOOLS/caffe train --solver=gender_solver.prorotxt  -gpu all  #加入 -gpu 选项</span>
-gpu 可以选择gpu的id号，如果是 -gpu all表示启用所有的GPU进行训练。
**2、采用funing-tuning 训练法**
**[python]**[view
 plain](http://blog.csdn.net/hjimce/article/details/48933813#)[copy](http://blog.csdn.net/hjimce/article/details/48933813#)
- <span style="font-size:18px;">$TOOLS/caffe train --solver=gender_solver.prorotxt -weights gender_net.caffemodel #加入-weights</span>
加入-weights，这个功能很好用，也经常会用到，因为现在的CNN相关的文献，很多都是在已有的模型基础上，进行fine-tuning，因为我们大部分人都缺少训练数据，不像谷歌、百度这些土豪公司，有很多人专门做数据标注，对于小公司而言，往往缺少标注好的训练数据。因此我们一般使用fine-tuning的方法，在少量数据的情况下，尽可能的提高精度。我们可以使用：-weights 选项，利用已有的模型训练好的参数，作为初始值，进行继续训练。
**三、调用[Python](http://lib.csdn.net/base/python)接口**
训练完毕后，我们就可以得到caffe的训练模型了，接着我们的目标就预测，看看结果了。caffe为我们提供了方便调用的python接口函数，这些都在模块pycaffe里面。因此我们还需要知道如何使用pycaffe，进行测试，查看结果。下面是pycaffe的预测调用使用示例：
**[python]**[view
 plain](http://blog.csdn.net/hjimce/article/details/48933813#)[copy](http://blog.csdn.net/hjimce/article/details/48933813#)
- # coding=utf-8
- import os  
- import numpy as np  
- from matplotlib import pyplot as plt  
- import cv2  
- import shutil  
- import time  
- 
- #因为RGB和BGR需要调换一下才能显示
- def showimage(im):  
- if im.ndim == 3:  
-         im = im[:, :, ::-1]  
-     plt.set_cmap('jet')  
-     plt.imshow(im)  
-     plt.show()  
- 
- #特征可视化显示，padval用于调整亮度
- def vis_square(data, padsize=1, padval=0):  
-     data -= data.min()  
-     data /= data.max()  
- 
- #因为我们要把某一层的特征图都显示到一个figure上，因此需要计算每个图片占用figure多少比例，以及绘制的位置
-     n = int(np.ceil(np.sqrt(data.shape[0])))  
-     padding = ((0, n ** 2 - data.shape[0]), (0, padsize), (0, padsize)) + ((0, 0),) * (data.ndim - 3)  
-     data = np.pad(data, padding, mode='constant', constant_values=(padval, padval))  
- 
- # tile the filters into an image
-     data = data.reshape((n, n) + data.shape[1:]).transpose((0, 2, 1, 3) + tuple(range(4, data.ndim + 1)))  
-     data = data.reshape((n * data.shape[1], n * data.shape[3]) + data.shape[4:])  
- 
-     showimage(data)  
- 
- 
- #设置caffe源码所在的路径
- caffe_root = '../../../caffe/'
- import sys  
- sys.path.insert(0, caffe_root + 'python')  
- import caffe  
- 
- 
- 
- 
- #加载均值文件
- mean_filename='./imagenet_mean.binaryproto'
- proto_data = open(mean_filename, "rb").read()  
- a = caffe.io.caffe_pb2.BlobProto.FromString(proto_data)  
- mean  = caffe.io.blobproto_to_array(a)[0]  
- 
- #创建网络，并加载已经训练好的模型文件
- gender_net_pretrained='./caffenet_train_iter_1500.caffemodel'
- gender_net_model_file='./deploy_gender.prototxt'
- gender_net = caffe.Classifier(gender_net_model_file, gender_net_pretrained,mean=mean,  
-                        channel_swap=(2,1,0),#RGB通道与BGR
-                        raw_scale=255,#把图片归一化到0~1之间
-                        image_dims=(256, 256))#设置输入图片的大小
- 
- 
- #预测分类及其可特征视化
- gender_list=['Male','Female']  
- input_image = caffe.io.load_image('1.jpg')#读取图片
- 
- prediction_gender=gender_net.predict([input_image])#预测图片性别
- #打印我们训练每一层的参数形状
- print'params:'
- for k, v in gender_net.params.items():  
- print'weight:'
- print (k, v[0].data.shape)#在每一层的参数blob中，caffe用vector存储了两个blob变量，用v[0]表示weight
- print'b:'
- print (k, v[1].data.shape)#用v[1]表示偏置参数
- #conv1滤波器可视化
- filters = gender_net.params['conv1'][0].data  
- vis_square(filters.transpose(0, 2, 3, 1))  
- #conv2滤波器可视化
- '''''filters = gender_net.params['conv2'][0].data
- vis_square(filters[:48].reshape(48**2, 5, 5))'''
- #特征图
- print'feature maps:'
- for k, v in gender_net.blobs.items():  
- print (k, v.data.shape);  
-     feat = gender_net.blobs[k].data[0,0:4]#显示名字为k的网络层,第一张图片所生成的4张feature maps
-     vis_square(feat, padval=1)  
- 
- 
- 
- 
- 
- #显示原图片，以及分类预测结果
- str_gender=gender_list[prediction_gender[0].argmax()]  
- print str_gender  
- 
- plt.imshow(input_image)  
- plt.title(str_gender)  
- plt.show()  
上面的接口，同时包含了pycaffe加载训练好的模型，进行预测及其特征可视化的调用方法。
