# 【深度学习】py-faster-rcnn中修改anchor的大小和数量 - Snoopy_Dream - CSDN博客





2018年05月02日 22:59:46[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：3556
所属专栏：[深度学习caffe常用python代码分享](https://blog.csdn.net/column/details/22599.html)









本文用到的是caffe下，rbg大神写的**py-faster-rcnn**。




**针对不同的数据集**，**不同大小**的目标，修改**anchor的大小和数量**，是很有必要的，可以加快收敛的速度，提高检测精度，




原始的数据集是VOC数据集，图片基本集中在**500*375**左右。在lib文件夹下**generate_anchor.py**中设计的anchor为**128*128,256*256,512*512**（不考虑ratios的话），而这样的设计**基本符合VOC中目标大小的分布**，但是如果你做的是**小目标的检测，比如你的大部分目标为8*8-20*20的样子**，很明显这里就不再适用了。



![](https://img-blog.csdn.net/20180502200656320?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/2018050220094592?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70).




为**方便修改anchor的大小**，使得可以**直接从generate_anchor修改anchor来起到真正修改anchor的作用**，需要将**调用generate_anchor.py的代码**稍作修改.

1.在lib文件夹下，proposal_layer.pyzhon中将的修改如下图，基本含义就是调用默认的参数。




![](https://img-blog.csdn.net/20180502202153904?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




2.在**anchor_target_layer.py**中同理，作修改，**红色框表示注释掉**，**绿色框表示新增代码**。




![](https://img-blog.csdn.net/20180502202723710?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





3.在**generate_anchor.py**中修改如下【不考虑ratios的话，anchor最小生成**8*8**的anchor（**6*16*1.0/12.0**）**base_size*16*scales_min**】，总共60个anchor【5*12即**ratios*scales**】，实验证明，增加anchor的数量，训练所耗费的时间增加的并不明显~

![](https://img-blog.csdn.net/2018050815394029?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


4.在models中修改**train.protxt**还有**test.prototxt**，这里只以train做了展示，test基本一样的修改。

![](https://img-blog.csdn.net/20180502225737787?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





![](https://img-blog.csdn.net/20180502225814978?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![](https://img-blog.csdn.net/20180508153900632?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![](https://img-blog.csdn.net/2018050815390845?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




