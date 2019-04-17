# neural turing machine:神经图灵机tensorflow源码注释 - Keith - CSDN博客





2016年10月28日 20:35:26[ke1th](https://me.csdn.net/u012436149)阅读数：3624
个人分类：[tensorflow																[deeplearning](https://blog.csdn.net/u012436149/article/category/6416551)](https://blog.csdn.net/u012436149/article/category/6461700)








# Neural Turing Machine

[论文地址](https://arxiv.org/pdf/1410.5401.pdf)
[tensorflow源码地址](https://github.com/carpedm20/NTM-tensorflow)
![](https://img-blog.csdn.net/20161028203100252)
NTM结构图
[Figure from…](http://distill.pub/2016/augmented-rnns/#attentional-interfaces)
![这里写图片描述](https://img-blog.csdn.net/20161028203205729)
lstm作为控制器

图中：A为控制器（为LSTM单元）write为write head, read 为 read_head 

源码重点注释：


## ops.py

ops.py中有两个函数，分别为linear() 、 Linear() ,这两个函数有什么区别呢， 看上图， linear 

主要用于左下角， Linear用于右上角

## ntm_cell.py

build_controller():用于生成LSTM单元，用于NTM的LSTM单元和常见的不同，它由三个部分构成了输入， 

build_read_head(): 生成read head， 就是图中的read 

参考文献： 

[1] [Attention and Augmented Recurrent Neural Networks](http://distill.pub/2016/augmented-rnns/#attentional-interfaces)





