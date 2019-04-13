
# caffe︱Places365-CNNs For Scene Recognition - 素质云笔记-Recorder... - CSDN博客

2017年03月03日 18:04:35[悟乙己](https://me.csdn.net/sinat_26917383)阅读数：5573



> ILSVRC2016中有一个Places Scene Classification和Scene Parsing项目的内容。

> 这次Places Scene Classification(Places2: A Large-Scale Database for

> Scene Understanding)是第二次作为ILSVRC的比赛项目，而Scene Parsing Challenge(MIT

> Scene Parsing Challenge

> 2016)是第一次纳入ILSVRC比赛，两者都是ILSVRC比较新的项目。因为考虑以往参与provided data

> track人数远远大于external data track，今年这两个项目这次都只设provided data track。

> .

> 一、场景分类数据库 Places2

> 官网：
> [http://places2.csail.mit.edu/](http://places2.csail.mit.edu/)

> github地址：

> [https://github.com/metalbubble/places365](https://github.com/metalbubble/places365)

> 数据介绍：Places2 contains more than 10 million images comprising 400+ unique scene categories. The dataset features 5000 to 30,000 training images per class, consistent with real-world frequencies of occurrence.

> .

> .

> 二、开源的Places365-CNNs

> 1、Places365 模型介绍

> Places365 is the latest subset of Places2 Database.
> There are two versions of Places365: Places365-Standard and Places365-Challenge
> .

> The train set of Places365-Standard
> has ~1.8 million images from 365 scene categories, where there are at most 5000 images per

> category. We have trained various baseline CNNs on the

> Places365-Standard and released them as below.

> * the train set of Places365-Challenge*
> has extra 6.2 million images along with all the images of Places365-Standard (so

> totally ~8 million images), where there are at most 40,000 images per

> category. Places365-Challenge will be used for the Places2 Challenge

> 2016 to be held in conjunction with the ILSVRC and COCO joint

> workshop at ECCV 2016.

> 2、Places365效能对比Places205
![这里写图片描述](https://img-blog.csdn.net/20170303175803138?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20170303175803138?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 可以看到ResNet的TOP5已经85.08%，VGG表现出色啊！同期来看看places205:
![这里写图片描述](https://img-blog.csdn.net/20170303175942128?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> [ ](https://img-blog.csdn.net/20170303175942128?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

> 两者的联合对比：
![这里写图片描述](https://img-blog.csdn.net/20170303180025253?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 来看看最终的 VGG16-Places365结果：
![这里写图片描述](https://img-blog.csdn.net/20170303180051485?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMjY5MTczODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
> 另外参考：

> 1、
> [为什么现在的CNN模型都是在GoogleNet、VGGNet或者AlexNet上调整的？](https://www.zhihu.com/question/43370067)

> 2、
> [如何评价ILSVRC2016的比赛结果？](https://www.zhihu.com/question/51024166)


