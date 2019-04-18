# Faster-rcnn总结 - qq229873466的博客 - CSDN博客

2017年05月25日 20:24:50[qq229873466](https://me.csdn.net/qq229873466)阅读数：292


# Faster-rcnn算法

## 训练

使用Image-net-pre-trained模型fine-tuning训练RPN网络

图片通过RPN网络获得RP，Image-net-pre-trained模型fine-tuning训练Fast-rcnn网络

使用Fast-rcnn网络conv层初始化RPN网络（共享conv权值），修改RPN独有的权值部分，训练得到一个新的RPN网络

图片通过新的RPN网络获得RP，保持Fast-rcnn网络的conv层权值，fine-tuning训练fc层，得到最终的Faster-rcnn网络

