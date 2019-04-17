# reshape和transpose的区别 - Snoopy_Dream - CSDN博客





2018年11月06日 13:49:15[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：182
个人分类：[Python																[代码分享](https://blog.csdn.net/e01528/article/category/7620830)](https://blog.csdn.net/e01528/article/category/7430266)

所属专栏：[faster-rcnn.pytorch](https://blog.csdn.net/column/details/25674.html)








reshape填充的顺序，从最后一个维度开始赋值。

transpose与reshape的最大区别：

reshape修改的只是维度，填充顺序不变

transpose修改的是轴，填充顺序改变

anchor_target_layer:

bbox_targets = bbox_targets.reshape((1, height, width, A * 4)).transpose(0, 3, 1, 2)：

下面的例子是在caffe版本的，pytorch的同理：

all_anchors = all_anchors.view(h*w * 9, 4)

bbox_targets.ndim  (h*w * A , 4)_>(1, height, width, A * 4),**只有相邻的hwA4你才能随意拆开然后和左右重组。**

bbox_targets = bbox_targets.reshape((1, height, width, A * 4)).transpose(0, 3, 1, 2)#1  9*4 h w



再来anchor_target_layer中np.meshgrid后ravel然后vstack再transpose得到的shifts为例。

直观的看：shifts.ndim=(H*W,4),注意我写的是H*W,而不是W*H。

因为它是逐行添加的，具体可看下面x对应w，y对应h，填充y=0第一行，所有列，然后填充第二行。
|x0|y0|x1|y1|
|----|----|----|----|
|0|0|0|0|
|1|0|1|0|
|2|0|2|0|
|...|...|...|...|
|0|1|0|1|
|1|1|1|1|
|2|1|2|1|
|...|...|...|...|

      因为填充方式的问题，先填充后维度的，所以shifts.reshape(h,w,4)可以理解为：第i行填充了所有列，然后填充i+1行所有列。

      但是，不可以shifts.reshape(w,h,4)，因为这样的理解是：第i列填充了所有行，然后填充所有列。

      你只可以shifts.reshape(h,w,4).transpose(1,0,2)，这里又涉及到**transpose**，**transpose**(1,0,2)表示原先的第1维作为现在的0维，原先的第0维作为现在的第1维，第二维不变。





