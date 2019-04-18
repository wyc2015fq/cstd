# 深度学习基础--CNN的可视化技术 - wydbyxr的博客 - CSDN博客
2018年11月12日 11:21:28[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：99
# CNN的可视化技术
  可以很好地“理解”中间的特征层和最后的分类器层。通过类似诊断（可视化+“消除”研究ablation study）的方式。
  参考资料：
[https://blog.csdn.net/whiteinblue/article/details/43312059](https://blog.csdn.net/whiteinblue/article/details/43312059)
[https://blog.csdn.net/tina_ttl/article/details/52048765](https://blog.csdn.net/tina_ttl/article/details/52048765)
## 论文中以alexnet举例
  这种可视化技术帮助我们找到了超越Alex-net的结构，本文还通过在ImageNet上训练，然后在其他数据集上finetuning获得了很好的结果。
  1）利用Deconvnet来映射特这激活值返回到初始的像素层。
  2）unpooling：需要记录在执行max-pooling操作时，每个pooing局域内最大值的位置。
  顾名思义就是反pooling过程，由于pooling是不可逆的，所以unpooling只是正常pooling的一种近似；通过记录正常pooling时的位置，把传进来的特征按照记录的方式重新“摆放”，来近似pooling前的卷基层特征。
## 惊人的发现
  通过可视化Alex-net网络的第1,2层，发现了各种问题。
  1）问题：第一层filter是非常高频和低频的信息，中间频率的filter很少覆盖。
  答：减小第一层的filter的尺寸从11*11到7*7
  2）问题：第二层的可视化比较混淆，由于第一层比较大的间隔。
  答：缩小间隔，从4变为2。
