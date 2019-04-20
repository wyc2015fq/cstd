# 从换脸到换姿势，AI 在图像处理的道路上越走越魔幻 - 人工智能学家 - CSDN博客
2018年08月31日 22:54:59[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：975
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw3rxjfOCDGiaM61G7PHyl81yx7FIznMibRwr1cn07y8ZkiahMf8BN6DODViatvxITL3a4PShJNMk6SQJw/640?wx_fmt=png)
来源：专知
加州大学伯克利分校的研究人员，近日在著名预印本网站 arXive 上，发布了最新的图像迁移成果：人体姿势和舞蹈动作迁移。旨在把专业舞蹈演员的动作迁移到不会跳舞的人身上，算法输出流畅，还原度极高。小编在感叹黑科技真厉害的同时，不禁为该算法的用途捏一把汗。因为就在不久前的 DeepFakes 深度换脸算法，还被人拿来将明星的脸换到成人影片上。
这篇文章的名字叫做：Everybody Dance Now， 是加州大学伯克利分校的研究人员：Caroline Chan, Shiry Ginosar, TingHui Zhou, Alexei A. Efros 在8月22日提交到 arXiv上的。文章中提出了一个简单动作迁移的方法：首先选择一支单人跳舞视频作为源视频，将其中的动作转换到目标视频中。 只需要几分钟，在另一个目标视频上的目标人员（完全不会跳舞的人）就会呈现同样的动作。
文章将动作姿态迁移看作是每一帧上图像到图像的转换，同时保证时间和空间的流畅。用动作探测器作为源视频和目标视频中间的表示，学习了一组从舞者动作画面到目标物体的映射，并且对这些数据进行调整，让它与视频完美融合，同时还加上了真实的人脸合成。通过这一框架，他们让很多未经训练的人跳出了芭蕾和街舞。
文章设计了能体现动作的中间表现形式：左图的火柴人。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw3rxjfOCDGiaM61G7PHyl81yJ7wyxySAiaukrcoYKUGSfpNBs3DRZpBxibeiaoGEsUfaoCAP9rBO8RQ2A/640?wx_fmt=png)
从目标视频中，作者用动作识别器为每一帧制作了(火柴人, 目标人物图像)的组合。有了这样相关的数据，作者就能用监督方法学习火柴人和目标人物之间图像到图像的转换模型了。之后，为了将源视频的动作迁移到目标视频中，作者把火柴人输入到经过训练的模型后，得到和源视频中人物相同的目标动作。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw3rxjfOCDGiaM61G7PHyl81yicUOBTnmfBzfIGYL33IAniaAqCEozuw6swHND7icCdEfic6V8vvL3yRU1w/640?wx_fmt=png)
另外，为了提高生成的质量，作者添加了两个元素：
1. 为了使生成的模型更连贯，作者会根据上一帧对目前的帧进行预测； 
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw3rxjfOCDGiaM61G7PHyl81yX7uYar4llM4mktw0kWKfiabpD8vibICUbKnSEyjD2jib6L8lPDkjNicTUA/640?wx_fmt=png)
2. 为了提高生成人脸的真实性，作者加入了经过训练的GAN来生成目标人物的脸部。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/AefvpgiaIPw3rxjfOCDGiaM61G7PHyl81y2Kooaeet1Q1wxdAibYB51CftpELibZ5Yz7ymZia20XxCxJebgCs3BlTibg/640?wx_fmt=png)
最后，让我们来欣赏一下生成的视频：

未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)
