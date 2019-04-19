# 目标检测ssd等one stage算法中关于小目标识别问题的解决方法-朝闻道-51CTO博客
考察很多算法 包括 yolo ssd等这一系列 onestage的方法 都不能解决检测时目标较小的问题，经过思考认为 问题出在小目标的特征图和 大目标不同上。
有篇论文解决了这个问题，就是RSSD算法 R 是彩虹的意思 表示将多个特征图相融合 来作为分类标准。
论文：Enhancement of SSD by concatenating feature maps for object detection 
论文链接：[https://arxiv.org/abs/1705.09587](https://arxiv.org/abs/1705.09587)
![目标检测ssd等one stage算法中关于小目标识别问题的解决方法](https://s1.51cto.com/images/blog/201804/09/02f867113a31b5e12c45ae5ccef5e883.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
![目标检测ssd等one stage算法中关于小目标识别问题的解决方法](https://s1.51cto.com/images/blog/201804/09/cb256f79d75a9e3201821407a99f37ac.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
![目标检测ssd等one stage算法中关于小目标识别问题的解决方法](https://s1.51cto.com/images/blog/201804/09/8d128551e33e56e329c55003c6089fe8.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
