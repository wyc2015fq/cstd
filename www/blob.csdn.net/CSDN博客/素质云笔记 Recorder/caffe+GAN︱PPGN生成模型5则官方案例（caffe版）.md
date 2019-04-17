# caffe+GAN︱PPGN生成模型5则官方案例（caffe版） - 素质云笔记/Recorder... - CSDN博客





2017年01月11日 10:47:05[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：4781








# 一、效果与架构

PPGN 整合了对抗训练、cnn特征匹配、降噪自编码、Langevin采样；在NIPS2016得到了Ian Goodfellow的介绍.

PPGN生成的图像同类差异化大，可根据指定生成不同类别的图像、多类化，生成的图像清楚分辨率高。

PPGN可使用imagenet1000类分类网络生成特定类别的图像。

效果： 
![这里写图片描述](https://img-blog.csdn.net/20170111101413654?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
架构图： 
![这里写图片描述](https://img-blog.csdn.net/20170111101427061?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
详细解读：zdx3578（[微信公众号](http://mp.weixin.qq.com/s?__biz=MzA5MDMwMTIyNQ==&mid=2649289111&idx=1&sn=42e565c3b77259742ae5504eed446486&chksm=8811e7d1bf666ec7f22987f2ee9160d67fab557114801f9d67c04041b6d39d94235ad16939ab#rd)）

# 二、案例

所需环境：caffe/python/GPU可以加速 

确保你是在./caffe/python下运行settings.py 

Install ImageMagick command-line interface on your system.
## 1、案例一：1_class_conditional_sampling.sh

1_class_conditional_sampling.sh: Sampling conditioning on the class “junco” (output unit #13 of the CaffeNet DNNtrained on ImageNet dataset). This script produces a sampling chain for a single given class.

Running ./1_class_conditional_sampling.sh 13 produces this result:

![这里写图片描述](https://img-blog.csdn.net/20170111101856551?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 2、2_class_conditional_sampling_many.sh
`Running ./2_class_conditional_sampling_many.sh <epsilon1> with different epsilon1 (multiplier for the image prior component) produces a chain with different styles of samples:`
![这里写图片描述](https://img-blog.csdn.net/20170111104428465?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 3、3_hidden_conditional_sampling.sh

Instead of conditioning on a class, it is possible to condition on a hidden neuron i.e. performing Multifaceted Feature Visualization or synthesizing a set of inputs that highly activate a given neuron to understand what features it has learned to detect.
`Running ./3_hidden_conditional_sampling.sh 196 produces a set of images for a conv5 neuron #196 previously identified as a "face detector" in DeepVis toolbox:`
![这里写图片描述](https://img-blog.csdn.net/20170111104436023?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 4、4_hidden_conditional_sampling_placesCNN.sh

One can repeat the example above but with an arbitrary neuron in a different condition network. Here, we visualize the conv5 neuron #182 in the AlexNet DNN trained on MIT Places205 dataset. This neuron has been previously identified as a “food detector” in Zhou et al [2].

```
Running ./4_hidden_conditional_sampling_placesCNN.sh 182 produces this result:
```

![这里写图片描述](https://img-blog.csdn.net/20170111104534322?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 5、5_caption_conditional_sampling.sh

We can also replace the image classifier network in previous examples with a pre-trained image captioning network to form a text-to-image model without even re-training anything. The image captioning model in this example is the LRCN model in Donahue et al (2015) [1].

You would need to use the Caffe provided here and update the path to Caffe accordingly in settings.py 

The list of words supported are here
`Running ./5_caption_conditional_sampling.sh a_church_steeple_that_has_a_clock_on_it produces this result:`
![这里写图片描述](https://img-blog.csdn.net/20170111104542166?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)






