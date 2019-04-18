# Windows下使用Caffe-Resnet - wishchinYang的专栏 - CSDN博客
2017年09月24日 16:14:27[wishchin](https://me.csdn.net/wishchin)阅读数：1090
参考文章：
        编译历程参考：[CNN：Windows下编译使用Caffe和Caffe2](http://blog.csdn.net/wishchin/article/details/77863889)
        caffe的VS版本源代码直接保留了sample里面的shell命令，当然这些shell命令在Windows平台下是不能运行的，需要稍微修改一下，转换为CMD可以理解的脚本代码。
**一、使用cifar数据集合。**
     1.获取cifar10数据集get_cifar10数据集：
`echo "Downloading..." wget --no-check-certificate http://www.cs.toronto.edu/~kriz/cifar-10-binary.tar.gz echo "Unzipping..." tar -xf cifar-10-binary.tar.gz && rm -f cifar-10-binary.tar.gz mv cifar-10-batches-bin/* . && rm -rf cifar-10-batches-bin`
       还是直接下载划算。
      2. 生成标准格式的数据集合create_cifar10.sh：
`set -e EXAMPLE=examples/cifar10 DATA=data/cifar10 DBTYPE=lmdb echo "Creating $DBTYPE..." rm -rf $EXAMPLE/cifar10_train_$DBTYPE $EXAMPLE/cifar10_test_$DBTYPE ./build/examples/cifar10/convert_cifar_data.bin $DATA $EXAMPLE $DBTYPE echo "Computing image mean..." ./build/tools/compute_image_mean -backend=$DBTYPE \ $EXAMPLE/cifar10_train_$DBTYPE $EXAMPLE/mean.binaryproto echo "Done."`
      cifar10数据集合使用了二进制的bin文件，对于我们来说几乎没有任何意义，只是作为一个测试流程使用。
      修改为cmd命令：
`convert_cifar_data.exe E:/DataSet/cifar10 E:/DataSet/cifar10 lmdb`
      把e盘cifar10文件夹下面的文件转换为lmdb格式，到同目录下。
      分别生成train和test文件夹，每个文件夹下面存在两个lock.lmdb和data.lmdb文件。
      至于为何使用lmdb格式的文件，请到知乎搜索....................
       然后修改为cmd命令：    
```
compute_image_mean.exe -backend=lmdb E:/DataSet/cifar10/cifar10_train_lmdb E:/DataSet/cifar10/mean.binaryprot
```
       依次生成lmdb文件，mean.binaryproto文件
**二、开始使用Caffe网络**
       修改Caffe的train_quick命令，修改为：
caffe.exe train --solver=E:/DataSet/cifar10/net/cifar10_quick_solver.prototxt
       可以从
cifar10_quick_solver.prototxt 
      启动简单的caffe网络，开始训练过程
      prototxt配置文件里面初始参数是这样的：
```
name: "CIFAR10_quick" layer { 
 name: "cifar" type: "Data" top: "data" top: "label" include { phase: TRAIN 
} 
transform_param { 
mean_file: "E:/DataSet/cifar10/mean.binaryproto"
 } 
data_param{ 
source: "E:/DataSet/cifar10/cifar10_train_lmdb" batch_size: 100 backend: LMDB 
} 
} 
layer{ 
name: "cifar" type: "Data" top: "data" top: "label" include { phase: TEST } 
transform_param { 
mean_file: "E:/DataSet/cifar10/mean.binaryproto"
 } 
data_param { source: "E:/DataSet/cifar10/cifar10_test_lmdb" batch_size: 100 backend: LMDB 
} 
}
```
![](https://img-blog.csdn.net/20170924224804807?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
     训练完成后，生成.model文件。留作使用。
~~ 三、注意事项~~
~~      1. 在使用初始化的过程中，使用protobuf解析网络配置文件，出现默认初始化问题。    ~~
~~       在solver_param.mutable_train_state()->set_level(FLAGS_level);~~
~~           CHECK(ReadProtoFromTextFile(param_file, param))；~~
~~           ReadProtoFromTextFile(filename.c_str(), proto)；~~
~~           bool success = google::protobuf::TextFormat::Parse(input, proto);/~~
~~     调用栈之中，解析文件目录为  filename = 0x0000029ed8a371a0 "E:/DataSet/cifar10/net/cifar10_quick_solver.prototxt"~~
~~     而 默认初始化 net 目录和+        snapshot_prefix_  为  0x0000029ee0ead6b0 "examples/cifar10/cifar10_quick"   ，是相当悲剧的行为。~~
~~     下载源码，编译后，调试进入，~~
         载回头来看这些繁琐的东西，真是蛋疼。
**三、开始使用残差网络**
       修改E:/DataSet/cifar10/net/cifar10_quick_solver.prototxt 文件，修改为：
#net: "E:/DataSet/cifar10/Net/cifar10_quick_train_test.prototxt"
net: "E:/DataSet/Net/ResNet-50-deploy.prototxt"
       可以直接使用caffe的ResNet50网络
![](https://img-blog.csdn.net/20170927232150271?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2lzaGNoaW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
