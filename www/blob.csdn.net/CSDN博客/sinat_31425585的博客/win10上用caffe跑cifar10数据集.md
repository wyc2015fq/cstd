# win10上用caffe跑cifar10数据集 - sinat_31425585的博客 - CSDN博客
2017年03月06日 18:12:22[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：885
                
刚参考了前路坎坷不弃亦的博客，在win10上用caffe跑了一下cifar10数据集，现在做一下笔记，方便以后复习。
1.下载cifar10数据集，并解压到 D:\caffe-master\data\cifar10路径下。
2.在D:\caffe-master\data\cifar10路径下，新建一个create_cifar10.bat文件，文件内容为：
```cpp
..\..\Build\x64\Release\convert_cifar_data.exe cifar-10-batches-bin ../../examples/cifar10 leveldb
pause
```
双击运行，会在D:\caffe-master\examples\cifar10生成两个文件夹：cifar10_test_leveldb，cifar10_train_leveldb。
这个步骤主要是调用caffe的文件格式转换接口，将cifar10的数据转换成LEVELDB格式。
3.在D:\caffe-master\data\cifar10路径下，新建一个compute_mean.bat文件，文件内容为：
```
..\..\Build\x64\Release\compute_image_mean.exe -backend=leveldb ../../examples/cifar10/cifar10_train_leveldb mean.binaryproto
pause
```
这个步骤主要用于计算cifar10数据样本的均值，并将计算结果保存在当前目录中的mean.binaryproto中。
4.修改D:\caffe-master\examples\cifar10路径下的cifar10_quick_train_test.prototxt文件
![](https://img-blog.csdn.net/20170304121303681?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzE0MjU1ODU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170304121310181?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzE0MjU1ODU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
5.在D:\caffe-master路径下，新建train_test_cifar.bat文件，文件内容为：
```
.\Build\x64\Release\caffe.exe train --solver=./examples/cifar10/cifar10_quick_solver.prototxt
pause
```
双击运行，即可得到如下效果：
![](https://img-blog.csdn.net/20170304121315693?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzE0MjU1ODU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
打完收工~
            
