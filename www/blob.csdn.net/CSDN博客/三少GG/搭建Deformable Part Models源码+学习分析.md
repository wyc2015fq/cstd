# 搭建Deformable Part Models源码+学习分析 - 三少GG - CSDN博客
2012年11月18日 12:47:30[三少GG](https://me.csdn.net/scut1135)阅读数：5133
1. 跑通代码：
代码地址： [http://people.cs.uchicago.edu.sixxs.org/~rbg/latent/](http://people.cs.uchicago.edu.sixxs.org/~rbg/latent/)  Version 5 (Sept. 5, 2012)
**别忘 mex -setup !!!**
1)
### [在windows下运行Felzenszwalb的Discriminatively Trained Deformable Part Models matlab代码](http://blog.csdn.net/pozen/article/details/7023742)
### 2)
### [最简单的方式使用Discriminatively Trained Deformable Part Models训练自己的模型（原创）](http://hi.baidu.com/liuwucn/item/bf5d210c04e81497a2df431a)
**Pozen的工作还是主要讲怎么在windows下训练，其中怎么准备数据说的不是很清楚，我这里稍微补充一下吧。（注意：我是在linux下跑的，windows下还要参考pozen的工作）**
3)
### [How to train models of Object Detection with Discriminatively Trained Part Based Models](http://blog.csdn.net/pozen/article/details/7103412)
不错的建议：1.  训练模型都交给选来的matlab代码，在线检测的部分就把他翻译成c++。
2. “最近我把检测部分（star-cascade）翻译成了C++的, 实现了检测部分，现在正在训练自己的模型呢”实 际运用中实时是没有问题的，可以从多个方面优化。首先多线程，求特征金字塔的时候，扫描检测的时候都应该用到。另算法本身也有改进与优化的余地，这样做得好的话10+/s没得问题。 处理视频的话，可以在加一些外部优化。
Deformable Part Model学习
//////////推荐博客：
[冒泡的崔](http://bubblexc.com/)
[http://bubblexc.com/](http://bubblexc.com/)
//////////////////////
1. [http://bubblexc.com.sixxs.org/y2011/422/](http://bubblexc.com.sixxs.org/y2011/422/)
Deformable Part Model是最近两年最为流行的图像中物体检测模型，利用这个模型的方法在近几届PASCAL VOC Challenge中都取得了较好的效果。其作者，芝加哥大学的[Pedro Felzenszwalb](http://people.cs.uchicago.edu.sixxs.org/~pff/)教授，也因为这项成就获得了VOC组委会授予的终身成就奖。有人认为这个模型是目前最好的物体检测算法。
不同于bag of features和hog模板匹配，这类“object conceptually weaker”的模型，在Deformable Part Model中，通过描述每一部分和部分间的位置关系来表示物体（part+deformable configuration）。其实早在1973年，Part Model就已经在 "The representation and matching of pictorial structures" 这篇文章中被提出了。
![](http://bubblexc.com.sixxs.org/wp-content/uploads/2011/05/p1-300x209.jpg)
图1：part model
Pedro Felzenszwalb教授提出的Deformable Part Model用到了三方面的知识：1.Hog Features 2.Part Model 3. Latent SVM。
- 作者通过Hog特征模板来刻画每一部分，然后进行匹配。并且采用了金字塔，即在不同的分辨率上提取Hog特征 
- 利用上段提出的Part Model。在进行object detection时，detect window的得分等于part的匹配得分减去模型变化的花费。 
- 在训练模型时，需要训练得到每一个part的Hog模板，以及衡量part位置分布cost的参数。文章中提出了Latent SVM方法，将deformable part model的学习问题转换为一个分类问题。利用SVM学习，将part的位置分布作为latent values，模型的参数转化为SVM的分割超平面。具体实现中，作者采用了迭代计算的方法，不断地更新模型。
作者的页面上有模型的matlab实现源码，必须运行在linux或mac平台中。另外，源码中已经包含PASCAL VOC中各个类别训练好的模型，可以直接用，如果需要自己训练模型，这个过程是很耗时的。为了提高效率，作者又在2010年的“Cascade Object Detection with Deformable Part Models”这篇文章中对part model做了改进，将效率提高了20倍左右。
相关资料：
- Fischler, M.A. and Elschlager, R.A. The representation and matching of pictorial structures, 1973
- Felzenszwalb, P.F. and Huttenlocher, D.P. Pictorial structures for object recognition,2005
- [http://people.csail.mit.edu.sixxs.org/torralba/courses/6.870/slide/6870_2008-09-10_histograms_pinto_opt.pdf](http://people.csail.mit.edu.sixxs.org/torralba/courses/6.870/slides/6870_2008-09-10_histograms_pinto_opt.pdf)
2.  opencv advanture
The sample program only demonstrates how to use the latent SVM for classification. The paper describes the training part in details. Although I don't understand all of it, here is the summary:
Latent SVM is a system built to recognize object by matching both
1. the HOG models, which consists of the 'whole' object and a few of its 'parts', and 2. the position of parts. The learned positions of object-parts and the 'exact' position of the whole object are the Latent Variables. The 'exact' position is with regard
 to the annotated bounding box from the input image. As an example, a human figure could be modeled by its outline-shape (whole-body head-to-toe) together with its parts (head, upper-body, left arm, right arm, left lower lib, right lower lib, feet).
The HOG descriptor for the whole body is Root Filter and those for the body parts are Parts Filter.
The target function is the best response by scanning a window over an image. The responses consists of the outputs from the all the filters. The search for best match is done in a multi-scale image pyramid. The classifier is trained iteratively using coordinate-descent
 method by holding some components constant while training the others. The components are Model Parameters (Filters Positions, Sizes), weight coefficients and error constants. The iteration process is a bit complicated - so much to learn! One important thing
 to note is the positive samples are composed of moving the parts around an allowable distance. There is a set of latent variables for this ( size of the movable-region, center of all the movable-regions, quadratic loss function coefficients). Able to consider
 the 'movable' parts is what I think being 'deformable' means.
**Detection Code**
The code for latent SVM detector code is located at OpenCV/modules/objdetect/. It seems to be self-contained. It has all the code needed to build HOG pyramids.
The detection code extract HOG descriptors from the input image and build multi-scale pyramids. It then scan the models (root and parts) over the pyramids for the good matches. Non-max suppression is used I think to remove those proximity
 matches. A threshold is applied to the score from SVM equation to determine the classification.
**Datasets**
Some trained models in matlab file format (voc-release4.tgz and older) are available for download at the website. But how to convert the available matlab files (such as cat_final.mat) to that XML format? There is a VOCWriteXML function
 in the VOC devkit (in matlab). Wonder if that could help. [http://fwd4.me/wSG](http://fwd4.me/wSG)
**Sample (latentsvmdetector.cpp)**
- Load a pre-built model and detect the object from an input image. 
- There does not seem to be a detector builder in OpenCV. 
- By looking at cat.xml The cat model has 2 models. They are probably bilateral symmetric model. Each model has 6 parts. The root filter sizes are 7x11 and 8x10.
**Results (with cat.xml model)**
- [cat.jpg] Took 61 seconds to finish. Able to detect the cat. Two false-positives at the top-right corner.
- [lena.jpg] Took 77 seconds. It detected Lena's beautiful face (including the purple feather hat and shoulder) ! Two other detected objects: her hat and some corner at the top-left corner of the picture.
- [tennis-cats.jpg] Took 44 seconds. It detected all 3 cats. Although the middle one and left cat and treated as one. Those two are closer together.
- [295087.jpg from GrabCut collection] Took 50 seconds. Somehow classified the Tree and the Rock Landscape as a cat!
- [260058.jpg from GrabCut collection] Took 76.5 seconds. Detected two false objects: 1) an area of the desert sand (small pyramid at the top edge), 2) part of the sky with clouds nears the edges.
- Without knowing how the model is trained, hard to tell the quality of this detector.[http://tech.dir.groups.yahoo.com/group/OpenCV/message/75507](http://tech.dir.groups.yahoo.com/group/OpenCV/message/75507); It is possible that it is taken from
 the 'trained' classifier parameters from the releases from the paper author (voc*-release.tgz).
**Resources**
Latent SVM: [http://people.cs.uchicago.edu/~pff/latent/](http://people.cs.uchicago.edu/~pff/latent/)
**Readings**
A Discriminatively Trained, Multiscale, Deformable Part Model, P. Felzenszwalb, et al.
