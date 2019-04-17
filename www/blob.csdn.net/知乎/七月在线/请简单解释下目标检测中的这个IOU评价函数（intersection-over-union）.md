# 请简单解释下目标检测中的这个IOU评价函数（intersection-over-union） - 知乎
# 



●今日面试题分享●

请简单解释下目标检测中的这个IOU评价函数（intersection-over-union）




解析：




在目标检测的评价体系中，有一个参数叫做 IoU ，简单来讲就是模型产生的目标窗口和原来标记窗口的交叠率。




具体我们可以简单的理解为：即检测结果DetectionResult与真实值Ground Truth的交集比上它们的并集，即为检测的准确率 IoU :






![](https://pic4.zhimg.com/v2-a0ff3bc21bb134707ea8541f13e8c143_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='326' height='56'></svg>)






举个例子，下面是一张原图






![](https://pic4.zhimg.com/v2-9d58ab56001f138f1964550ee756a38b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='501' height='378'></svg>)






然后我们对其做下目标检测，其DR = DetectionResult，GT = GroundTruth。






![](https://pic1.zhimg.com/v2-bf210ddb6bcbb8d669b58a39e3d09bc4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='819' height='598'></svg>)






黄色边框框起来的是：




DR⋂GT




绿色框框起来的是：

DR⋃GT




不难看出，最理想的情况就是DR与GT完全重合，即IoU = 1。







**题目来源**：七月在线官网（[http://www.julyedu.com](https://link.zhihu.com/?target=http%3A//www.julyedu.com)）——面试题库——面试大题——CV





搜索VX公号并关注：**七月在线实验室 **




后台回复：**100**   免费领取**【机器学习面试100题】**

后台回复：**干货 **免费领取**【全体系人工智能学习资料】**

**后台回复： 领资料****免费领取全套【NLP工程师必备干货资料】**


