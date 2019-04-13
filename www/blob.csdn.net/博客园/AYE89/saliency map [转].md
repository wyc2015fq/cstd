
# saliency map [转] - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[saliency map [转]](https://www.cnblogs.com/eniac1946/p/8600422.html)
|Posted on|2018-03-19 10:40|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=8600422)|[收藏](#)
|[基于Keras实现的代码文档](https://raghakot.github.io/keras-vis/visualizations/saliency/)|（图+说明）
|["Deep Inside Convolutional Networks: Visualising Image Classification Models and Saliency Maps"](https://arxiv.org/abs/1312.6034)
|saliency map即特征图，可以告诉我们图像中的像素点对图像分类结果的影响。
|计算它的时候首先要计算与图像像素对应的正确分类中的标准化分数的梯度(这是一个标量)。如果图像的形状是(3, H, W)，这个梯度的形状也是(3, H, W)；对于图像中的每个像素点，这个梯度告诉我们当像素点发生轻微改变时，正确分类分数变化的幅度。
|计算saliency map的时候，需要计算出梯度的绝对值，然后再取三个颜色通道的最大值；因此最后的saliency map的形状是(H, W)为一个通道的灰度图。
|下图即为例子：
![这里写图片描述](http://img.blog.csdn.net/20171129165552842?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSUFNb2xkcGFu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
|上图为图像，下图为特征图，可以看到下图中亮色部分为神经网络感兴趣的部分。
|理论依据
![这里写图片描述](http://img.blog.csdn.net/20171129173955165?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSUFNb2xkcGFu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](http://img.blog.csdn.net/20171129174004492?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSUFNb2xkcGFu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
|需要注意一下：
![这里写图片描述](http://img.blog.csdn.net/20171130143015503?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSUFNb2xkcGFu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)







|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
