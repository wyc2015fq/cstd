# 刷新三项COCO纪录！姿态估计模型HRNet开源了，中科大微软出品 | CVPR 2019 - 知乎
# 



> 圆栗子 发自 凹非寺 
量子位 报道 | 公众号 QbitAI
![](https://pic3.zhimg.com/v2-050c0c0baa0931bfd90b8097f4493822_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='772'></svg>)
中科大和微软亚洲研究院，发布了新的人体**姿态估计模型**，刷新了三项COCO纪录，还中选了**CVPR 2019**。

这个名叫**HRNet**的神经网络，拥有与众不同的并联结构，可以随时**保持高分辨率表征**，不只靠从低分辨率表征里，恢复高分辨率表征。

如此一来，姿势识别的效果明显提升：

在COCO数据集的**关键点检测**、**姿态估计**、**多人姿态估计**这三项任务里，HRNet都超越了所有前辈。
![](https://pic2.zhimg.com/v2-fb8012c326a6ebd85754f4f5b0e40399_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='304'></svg>)
更加优秀的是，团队已经把模型**开源**了。

## **结构不一样**

HRNet，是高分辨率网络 (High-Resolution Net) 的缩写。

团队希望，在**表征学习** (Representative Learning) 的整个过程中，都能保持高分辨率表征。

所以，他们为模型设计了**并联结构**，把不同分辨率的子网络，用新的方式连在一起：
![](https://pic1.zhimg.com/v2-b38e21ac3223439121fcaca9d1e19b18_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='532'></svg>)△ 并联
对比一下，前辈们连接各种分辨率，常常是用简单的**串联**，不论由高到低，还是由低到高：
![](https://pic4.zhimg.com/v2-c2eb7c1715b0a0b6de4cc7b9b6c8930f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1070' height='858'></svg>)△ 串联
那么，仔细观察一下HRNet的并联网络。

它从一个**高分辨率**的子网络开始，慢慢加入分辨率**由高到低**的子网络。

特别之处在于，它不是依赖一个单独的、由低到高的**上采样** (Upsampling) 步骤，粗暴地把低层、高层表征聚合到一起；

而是在整个过程中，**不停地融合** (Fusion) 各种不同尺度的表征。
![](https://pic2.zhimg.com/v2-fdf563e73f17576894a4faa140ccbea5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='338'></svg>)△ 交换单元
这里，团队用了交换单元 (**Exchange Units**) ，穿梭在不同的子网络之间：让每一个子网络，都能从其他子网络生产的表征里，获得信息。

这样不断进行下去，就能得到丰富的高分辨率表征了。

## **横扫各大数据集**
![](https://pic3.zhimg.com/v2-1e559427117e14790abf7b571c250d42_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='721'></svg>)
团队先在COCO数据集的**val2017**验证集上，对比了HRNet和一众前辈的**关键点检测**表现。

结果是，在两种输入分辨率上，大模型**HRNet-W48**和小模型HRNet-W32，都刷新了COCO纪录。

其中，大模型在384 x 288的输入分辨率上，拿到了**76.3**的AP分。
![](https://pic2.zhimg.com/v2-7651d52ea8f90bf64c54e813112f61bd_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1534' height='576'></svg>)
然后，团队又在COCO的**test-dev2017**测试集上，为HRNet和其他选手举办了**姿态估计**比赛。

结果，大模型和小模型，也都刷新了COCO纪录。大模型的AP分达到了**77.0**。
![](https://pic3.zhimg.com/v2-b61e53f06cda10e36ddfccb4732e1e1a_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1522' height='808'></svg>)
另外，在**多人姿态估计**任务上，HRNet又超越了前辈们在COCO数据集上的成绩。

那么，其他数据集能难倒它么？

在**MPII验证集、PoseTrack**、**ImageNet验证集**上，HRNet的表现都好过所有同台的对手。
![](https://pic1.zhimg.com/v2-f7b44c2f1eadc958b314a3c667acfc08_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='818' height='752'></svg>)
详细的成绩表，请从文底传送门前往。

## **开源啦**

刷榜活动圆满结束。

团队把这个振奋人心的模型开了源，是用**PyTorch**实现的。

除了估计姿势，这个方法也可以做语义分割，人脸对齐，物体检测，等等等等。
![](https://pic1.zhimg.com/v2-522d96cd1e552381c6d013f269959068_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='259'></svg>)
所以，你也去试一试吧。

论文传送门：
[https://arxiv.org/abs/1902.09212](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1902.09212)

代码传送门：
[https://github.com/leoxiaobin/deep-high-resolution-net.pytorch](https://link.zhihu.com/?target=https%3A//github.com/leoxiaobin/deep-high-resolution-net.pytorch)

— **完** —

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态

戳右上角「+关注」获取最新资讯↗↗

如果喜欢，请分享or点赞吧~比心❤


