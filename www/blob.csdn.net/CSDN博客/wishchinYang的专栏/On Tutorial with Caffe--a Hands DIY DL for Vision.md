# On Tutorial with Caffe--a Hands DIY DL for Vision - wishchinYang的专栏 - CSDN博客
2015年04月09日 17:51:00[wishchin](https://me.csdn.net/wishchin)阅读数：1424
个人分类：[ANN/DNN/纤维丛																[人脸识别](https://blog.csdn.net/wishchin/article/category/1508467)](https://blog.csdn.net/wishchin/article/category/3109825)
原文链接：[http://blog.sciencenet.cn/blog-1583812-844177.html](http://blog.sciencenet.cn/blog-1583812-844177.html)
       Caffe作为DL的一个学习框架，Caffe is a deep learning framework made with expression, speed, and modularity in mind.It is developed by the Berkeley Vision and Learning Center ([BVLC](http://bvlc.eecs.berkeley.edu)) and
 by community contributors.[Yangqing Jia](http://daggerfs.com) created the project during his PhD at UC Berkeley.Caffe is released under the[BSD
 2-Clause license](https://github.com/BVLC/caffe/blob/master/LICENSE).
       为什么是深度学习？............
![](http://image.sciencenet.cn/album/201411/17/2253106r0jvohb51zs56yr.png)
现有的DL框架：
![](http://image.sciencenet.cn/album/201411/17/225304hu1lolxxkwwwpli2.png)
Caffe的不同之处：使用纯C++作为底层库，开放Python、Matlab接口，基于CUDA
![](http://image.sciencenet.cn/album/201411/17/2253056lze6515472526yn.png)
Caffe的网络结构：一个块作为数据结构
![](http://image.sciencenet.cn/album/201411/17/225307h9k1dd45k9dhkvpp.png)
Caffe单层网络定义（数据结构）：
![](http://image.sciencenet.cn/album/201411/17/225309yt01tx10ih5kst51.png)
Caffe的基本Blob结构：
![](http://image.sciencenet.cn/album/201411/17/2253396gu32r6r3i262xgx.png)
如何训练Caffe网络：参数配置在solver.prototxt中
![](http://image.sciencenet.cn/album/201411/17/225341oj1x0zoktgtc0a4k.png)
生成Caffe的数据结构：
![](http://image.sciencenet.cn/album/201411/17/225341kpu5hrcrr9opavr5.png)

**Examples:**
**Logistic Regression (in Python):**[http://nbviewer.ipython.org/github/BVLC/caffe/blob/dev/examples/hdf5_classification.ipynb](http://nbviewer.ipython.org/github/BVLC/caffe/blob/dev/examples/hdf5_classification.ipynb)
**Learn LeNet on MNIST:**[http://caffe.berkeleyvision.org/gathered/examples/mnist.html](http://caffe.berkeleyvision.org/gathered/examples/mnist.html)
参数调整 ： --将一个训练好的模型 参数调整 到一个新任务中......
![](http://image.sciencenet.cn/album/201411/17/22534388lwyheyek212ub1.png)
方法：使用ImageNet训练的模型到任务，只需在定义里有一小点改变......
![](http://image.sciencenet.cn/album/201411/17/225345zgm6nwannragbg0j.png)
  #设定好模型路径和想要分类的图片
方法一：直接初始化....
```python
Model_File = caffe_root + "examples/baby/deploy.prototxt"
PreTrained = caffe_root + "examples/baby/caffenet_train_iter_6001.caffemodel"
net = caffe.Classifier(Model_File,PreTrained);
```
方法二：New一下就可以了....
```python
Net = new Caffe::Net(
      "style_solver.prototxt"
                        )
Net.CopyTrainedNetFrom(
     pretrained_model
     );
solver.Solve(net);
```
![](http://image.sciencenet.cn/album/201411/17/225404hhffbo1jhc1jiit1.png)
调整参数 使 CaffeNet 
Fine-tuning CaffeNet for Style Recognition on “Flickr Style” Data
[http://tutorial.caffe.berkeleyvision.org/gathered/examples/finetune_flickr_style.html](http://tutorial.caffe.berkeleyvision.org/gathered/examples/finetune_flickr_style.html)
![](http://image.sciencenet.cn/album/201411/17/225405dgmrqvdbmbgvjg5l.png)
SoftMax 函数损失层：
![](http://image.sciencenet.cn/album/201411/17/225406ckxg1stq1ypgtzzz.png)
SigMoid 交叉熵 损失函数：
![](http://image.sciencenet.cn/album/201411/17/2254081f1u1fxptfpxfsuv.png)
欧式损失：
![](http://image.sciencenet.cn/album/201411/17/225409yez2mmeqjexn8jqx.png)
多重 损失：
![](http://image.sciencenet.cn/album/201411/17/225454bbyzud9otxeubuo5.png)
![](http://image.sciencenet.cn/album/201411/17/225456vp69gavdv46jmv9i.png)
![](http://image.sciencenet.cn/album/201411/17/225458an6gdi8i8eify84i.png)
![](http://image.sciencenet.cn/album/201411/17/225500esz2agnqslty1ga7.png)
![](http://image.sciencenet.cn/album/201411/17/2255045d5nidxg5nk8dldp.png)
![](http://image.sciencenet.cn/album/201411/17/225522obg2d7fla8fkxxd0.png)
![](http://image.sciencenet.cn/album/201411/17/2255253tsuvv8k78hknhs0.png)
![](http://image.sciencenet.cn/album/201411/17/2255276vyzbn0ov3vn80yn.png)
Caffe Demo :[http://demo.caffe.berkeleyvision.org/](http://demo.caffe.berkeleyvision.org/)
Feature Visualization :[http://nbviewer.ipython.org/github/BVLC/caffe/blob/dev/examples/filter_visualization.ipynb](http://nbviewer.ipython.org/github/BVLC/caffe/blob/dev/examples/filter_visualization.ipynb)
![](http://image.sciencenet.cn/album/201411/17/225529zxqx5vrmmgy0yr0x.png)
How to transform models in Caffe:
[http://nbviewer.ipython.org/github/BVLC/caffe/blob/dev/examples/net_surgery.ipynb](http://nbviewer.ipython.org/github/BVLC/caffe/blob/dev/examples/net_surgery.ipynb)
**Related projects:**
R-CNN: Regions with CNN
Ross Girshick et al.*Rich feature hierarchies for accurate object detection and semantic**segmentation*. CVPR14.
[http://nbviewer.ipython.org/github/BVLC/caffe/blob/dev/examples/detection.ipynb](http://nbviewer.ipython.org/github/BVLC/caffe/blob/dev/examples/detection.ipynb)
Full scripts:
[https://github.com/rbgirshick/rcnn](https://github.com/rbgirshick/rcnn)
Visual Style Recognition:
Karayev et al.*Recognizing Image Style*. BMVC14. Caffe fine-tuning example
Demo:[http://demo.vislab.berkeleyvision.org/](http://demo.vislab.berkeleyvision.org/)
**Latest Roast:**
Model Zoo: [https://github.com/BVLC/caffe/wiki/Model-Zoo](https://github.com/BVLC/caffe/wiki/Model-Zoo)
- BVLC reference models
- VGG Devil + ILSVRC14 modelsin the zoo
- Network-in-Network / CCCP modelin the zoo
Caffe + cuDNN
Parallel / distributed training across CPUs, GPUs, and cluster nodes
[https://github.com/BVLC/caffe/pull/1148](https://github.com/BVLC/caffe/pull/1148)
参考：DIY Deep Learning for Vision with Caffe slides
[https://docs.google.com/presentation/d/1UeKXVgRvvxg9OUdh_UiC5G71UMscNPlvArsWER41PsU/preview?sle=true#slide=id.p](https://docs.google.com/presentation/d/1UeKXVgRvvxg9OUdh_UiC5G71UMscNPlvArsWER41PsU/preview?sle=true#slide=id.p)
