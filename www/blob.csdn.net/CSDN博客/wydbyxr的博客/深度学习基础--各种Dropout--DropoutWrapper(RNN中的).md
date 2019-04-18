# 深度学习基础--各种Dropout--DropoutWrapper(RNN中的) - wydbyxr的博客 - CSDN博客
2018年11月12日 11:34:01[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：170
# DropoutWrapper
  dropout是一种非常efficient的regularization方法，在rnn中如何使用dropout和cnn不同	对于rnn的部分不进行dropout，也就是说从t-1时候的状态传递到t时刻进行计算时，这个中间不进行memory的dropout；仅在同一个t时刻中，多层cell之间传递信息的时候进行dropout。
## 示意图
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181112113334988.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_16,color_FFFFFF,t_70)
  图中，xt−2时刻的输入首先传入第一层cell，这个过程有dropout，但是从t−2时刻的第一层cell传到t−1,t,t+1的第一层cell这个中间都不进行dropout。
  再从t+1时候的第一层cell向同一时刻内后续的cell传递时，这之间又有dropout了。
