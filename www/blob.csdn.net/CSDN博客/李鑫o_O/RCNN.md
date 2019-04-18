# RCNN - 李鑫o_O - CSDN博客

2017年06月04日 14:19:39[hustlx](https://me.csdn.net/HUSTLX)阅读数：389


1、Selective Search

![](https://img-blog.csdn.net/20170604142115306?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSFVTVExY/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

Yield small starting regions[1]   Efficient graph-based image segmentation

Group similar region(color,texture,size,fill)

Rank : v=RND × i

2、Feature extraction

Wraped mean-substracted 227*227RGB image

5 convolutional layer,2 fully connection layer

CNN faster than other methods

3、Training

Supervised pre-training——use ImageNet classification dataset(120w) to pre-train a model, with 1000 way softmax outputs

Domain-specific fine-tuning——Use the pretrain model to fine-tuning a domain-specific CNN, with (N+1) way softmax outputs.(“1” for the background)

Classifiers：linear SVM per class

