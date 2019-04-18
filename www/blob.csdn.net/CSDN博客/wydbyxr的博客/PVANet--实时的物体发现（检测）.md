# PVANet--实时的物体发现（检测） - wydbyxr的博客 - CSDN博客
2017年03月31日 20:28:43[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：1770
所属专栏：[深度学习--最新论文](https://blog.csdn.net/column/details/23683.html)
#2017.4.10再次更新
#一、论文总结：
##abstract：
降低计算成本是同样重要的提高精度。本文提出了一种新的网络结构，这是一个数量级轻于其他先进的网络同时保持精度。
基于多层少通道的基本原理，这种新的深度神经网络最小化其冗余，采用最近新发现包括c.relu和初生结构（Inception）。
它84.9% and 84.2%mAP on VOC2007 and VOC2012，但计算量是ResNet-101的10%。
##introduction：
CNN的使用，使目标检测可商用了，但在计算速度上依赖于高昂的计算成本。目前，压缩和量化（compression and quantization）是节省成本的主要手段。而对卷积核的压缩、分解暗示着网络结构的冗余。我们使用直接在网络设计阶段节省成本的方法。它能保证精度并实时。
基于“少输出多层次”，我们改进c.relu 在初始层和Inception结构在网络的后半部分。		
采用多尺度特征级联也适用于最大化目标检测任务的多尺度性。
我们的这个框架也能用训练批次标准化（BN，batch normalization），残差连接和基于平稳检测（plateau detection）的自主学习速率调度，来训练。
主要分为两个部分：特征提取网络和目标检测网络
##2.1特征提取网络
- 
改性c.relu部分：本来c.relu可以双倍数量输出通道输出，且共享偏置；我们添加一个分离的偏置层，使两相关的过滤器可以具有不同的偏置值。
- 
初始结构部分：它是在输入图像中最具效益的部分。大的图像应该有更大的感受野，而小图像不需要这么多、大的卷积核。初生结构中的1x1卷积，防止了在其他路径上的感受野的生长，这可以减少这些冗余。
- 
深层网络的训练：一般认为越深越好。解决越深越麻烦，我们使用有预激活和批次规定化的残差结构；也使用了基于“平稳检测”的动态学习率
on-plateau，指量出平均误差后，最小误差不会随迭代次数改变。这时，学习率会被一个常数改变。
- 
总体设计：一个5x5卷积被有两个连续的3x3的卷积层，见两个图。
##2.2目标检测网络
- 
高度的特征连接：
超特征级联多尺度表示及其组合被证明是有效在许多最近的深度学习任务。但由于直接级联所有抽象层可能产生冗余信息且有更高的计算要求，我们需要设计不同的抽象层，同时小心设计抽象层层数。见图3.
- 
区域推荐网络（RPN）不需要以完全投入很深的全连接分类，只需要128个特征图，将这些特征图通过3*3*384的卷积层，42个锚的6个尺度（32，48，80，144，256，512）和7个纵横比.
- 
分类网络：1.使用全部512个特征图，对每一个ROI使用6*6*512的张量（即ROI pooling）；2.全连接（fully-connected）4096-4096-（21+84）的输出节点ROI
##4实验
使用imagenet预训练；评价指标包括top-1 err.、top-2 err.和cost；它的精度比Googlenet还好。
在VOC2007上，用Recall和mAP评价不同帧数下的情况。表格表明超过200的Proposals就不能明显提高性能了。在FC层由“4096- 4096”变成“512 - 4096 - 512 - 4096”只有0.5%的精度下降。
召回率(Recall): 也称为 True Positive Rate:R = TP/(TP+FN) ; 反映了被正确判定的正例占总的正例的比重
在VOC2012上，我们的是无敌的。
##5结论
在本文中，我们表明，目前的网络是高度冗余的，我们可以设计一个具有复杂视觉任务的轻便网络。精心挑选与组合最近DL领域的技术创新，能使我们有可能设计出一个网络最大化计算效率。
我们相信我们的设计原则是广泛适用于其他任务，例如面对识别与语义分析。
我们的网络设计是完全独立的网络压缩和量化。各种最近的压缩和量化技术是适用于我们的网络，以及进一步增加实际应用中的实际性能。
计算量主要在卷积层，而参数个数大多在FC层。
二、使用说明
##1.Git clone一下Faster R-CNN的工程
git clone --recursive [https://github.com/sanghoon/pva-faster-rcnn.git](https://github.com/sanghoon/pva-faster-rcnn.git)
##2.编译工程
cd $FRCN_ROOT/lib
make
##3.Build Caffe and pycaffe
cd $FRCN_ROOT/caffe-fast-rcnn
注意：进行APVnet时，需要改写Makefile.config文件中，去掉注释`WITH_PYTHON_LAYER := 1`
cp Makefile.config.example Makefile.config
make -j8 && make pycaffe
##4.下载 PVANet detection model
我们不翻墙就下载不了该model。
##5.下载 PASCAL VOC 2007 and 2012
[https://github.com/rbgirshick/py-faster-rcnn#beyond-the-demo-installation-for-training-and-testing-models中已经十分详细的介绍了步骤。](https://github.com/rbgirshick/py-faster-rcnn#beyond-the-demo-installation-for-training-and-testing-models%E4%B8%AD%E5%B7%B2%E7%BB%8F%E5%8D%81%E5%88%86%E8%AF%A6%E7%BB%86%E7%9A%84%E4%BB%8B%E7%BB%8D%E4%BA%86%E6%AD%A5%E9%AA%A4%E3%80%82)
下面以下载VOC2007为例，具体介绍：
###a.Download the training, validation, test data and VOCdevkit
wget [http://host.robots.ox.ac.uk/pascal/VOC/voc2007/VOCtrainval_06-Nov-2007.tar](http://host.robots.ox.ac.uk/pascal/VOC/voc2007/VOCtrainval_06-Nov-2007.tar)
wget [http://host.robots.ox.ac.uk/pascal/VOC/voc2007/VOCtest_06-Nov-2007.tar](http://host.robots.ox.ac.uk/pascal/VOC/voc2007/VOCtest_06-Nov-2007.tar)
wget [http://host.robots.ox.ac.uk/pascal/VOC/voc2007/VOCdevkit_08-Jun-2007.tar](http://host.robots.ox.ac.uk/pascal/VOC/voc2007/VOCdevkit_08-Jun-2007.tar)
###b.Extract all of these tars into one directory named VOCdevkit
tar xvf VOCtrainval_06-Nov-2007.tar
tar xvf VOCtest_06-Nov-2007.tar
tar xvf VOCdevkit_08-Jun-2007.tar
###[c.It](http://c.It) should have this basic structure
$VOCdevkit/                           # development kit
$VOCdevkit/VOCcode/                   # VOC utility code
$VOCdevkit/VOC2007                    # image sets, annotations, etc.
# … and several other directories …
###d.Create symlinks for the PASCAL VOC dataset
cd $FRCN_ROOT/data
ln -s $VOCdevkit VOCdevkit2007	     #也可以使用cp命令，将VOCdevkit命名为VOCdevkit2007
##6.现在就可以运行了
###a.PVANet on PASCAL VOC 2007
cd $FRCN_ROOT
./tools/test_net.py --net models/pvanet/pva9.1/PVA9.1_ImgNet_COCO_VOC0712.caffemodel --def models/pvanet/pva9.1/faster_rcnn_train_test_21cls.pt --cfg models/pvanet/cfgs/submit_1019.yml --gpu 0
###b.PVANet (compressed)
cd $FRCN_ROOT
./tools/test_net.py --net models/pvanet/pva9.1/PVA9.1_ImgNet_COCO_VOC0712plus_compressed.caffemodel --def models/pvanet/pva9.1/faster_rcnn_train_test_ft_rcnn_only_plus_comp.pt --cfg models/pvanet/cfgs/submit_1019.yml --gpu 0
###c.这里简单介绍一下上面所用到的某些文件
.py,即负责执行model。
.prototxt文件，这是一个文本文件，详细描述了网络结构和数据集信息，即–def [XXX.pt](http://XXX.pt)。
.caffemodel，在cifar10模型下面调用已经训练好的模型，测试。然后在指定模型的具体的权重。
-gpu: 可选参数。该参数用来指定用哪一块gpu运行，根据gpu的id进行选择，如果设置为’-gpu all’则使用所有的gpu运行。如使用第二块gpu运行，可以改为 -gpu 1
# ./build/tools/caffe test -model examples/mnist/lenet_train_test.prototxt -weights examples/mnist/lenet_iter_10000.caffemodel -gpu 0 -iterations 100-model, -weights, -gpu和-iteration四个参数。意思是利用训练好了的权重（-weight)，输入到测试模型中(-model)，用编号为0的gpu(-gpu)测试100次(-iteration)
#三、部分结果
reason:
Saving cached annotations to /home/ydwu/work/pva-faster-rcnn/data/VOCdevkit2007/annotations_cache/annots.pkl
AP for aeroplane = 0.8871
AP for bicycle = 0.8827
AP for bird = 0.8579
AP for boat = 0.8105
AP for bottle = 0.7451
AP for bus = 0.8995
AP for car = 0.8927
AP for cat = 0.8969
AP for chair = 0.7448
AP for cow = 0.8870
AP for diningtable = 0.8322
AP for dog = 0.8851
AP for horse = 0.8811
AP for motorbike = 0.8934
AP for person = 0.8737
AP for pottedplant = 0.6381
AP for sheep = 0.8739
AP for sofa = 0.8608
AP for train = 0.8935
AP for tvmonitor = 0.8418
## Mean AP = 0.8489
```
Results:
0.887
0.883
0.858
0.811
0.745
0.899
0.893
0.897
0.745
0.887
0.832
0.885
0.881
0.893
0.874
0.638
0.874
0.861
0.894
0.842
0.849
```
## Results computed with the **unofficial** Python eval code.Results should be very close to the official MATLAB eval code.Recompute with `./tools/reval.py --matlab ...` for your paper.Thanks, The Management
