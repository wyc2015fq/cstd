# 非极大值抑制(NMS,Non-maximum suppression) - wydbyxr的博客 - CSDN博客
2018年09月29日 10:40:10[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：42
个人分类：[机器学习具体算法](https://blog.csdn.net/wydbyxr/article/category/7945743)
所属专栏：[经典机器学习算法](https://blog.csdn.net/column/details/28812.html)
# 非极大值抑制(NMS,Non-maximum suppression)
  在object dection中应用非常广泛，简单地说，他就是对一些region proposals（物体的候选框）进行筛选，得到最佳的物体检测位置。 顾名思义就是把非极大值过滤掉（抑制）。
## 使用
   1）非最大抑制是回答这样一个问题：“当前的梯度值在梯度方向上是一个局部最大值吗？” 所以,要把当前位置的梯度值与梯度方向上两侧的梯度值进行比较；
   2）梯度方向垂直于边缘方向。
  完成非极大值抑制后，会得到一个二值图像，非边缘的点灰度值均为0，可能为边缘的局部灰度极大值点可设置其灰度为128。根据下文的具体测试图像可以看出，这样一个检测结果还是包含了很多由噪声及其他原因造成的假边缘。因此还需要进一步的处理。Canny算法中减少假边缘数量的方法是采用双阈值法。
  在Canny算法中，非极大值抑制是进行边缘检测的重要步骤，通俗意义上是指寻找像素点局部最大值，将非极大值点所对应的灰度值置为0，这样可以剔除掉一大部分非边缘的点（这是本人的理解）。
## 具体计算过程
  Non-Maximum Suppression就是需要根据score矩阵和region的坐标信息，从中找到置信度比较高的bounding box。
  1）首先，NMS计算出每一个bounding box的面积，然后根据score进行排序，把score最大的bounding box作为队列中。
  2）接下来，计算其余bounding box与当前最大score与box的IoU，去除IoU大于设定的阈值的bounding box。
  3）然后重复上面的过程，直至候选bounding box为空。
  4）最终，检测了bounding box的过程中有两个阈值，一个就是IoU，另一个是在过程之后，从候选的bounding box中剔除score小于阈值的bounding box。
  需要注意的是：Non-Maximum Suppression一次处理一个类别，如果有N个类别，Non-Maximum Suppression就需要执行N次。
## 两种非极大值抑制的改进
  现在有两种主流的方式：软非最大值抑制（Soft NMS）和硬非最大值抑制（Hard NMS）。
  1）软非最大值抑制是对所有的候选框去做重新的估分，对比较差的框保留但会抑制分数。
  2）硬非最大值抑制就是将IOU超过阈值之后的分数比较差的那些框过滤掉。
  所以两种方式最终实现的结果是Hard NMS保留的框比较少，而Soft NMS对框的分数做了一个修改，保留了较高的召回率（recall）。
