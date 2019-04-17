# 为了治好你的脸盲，这家表情包网站开源了一个明星识别器丨GitHub - 知乎
# 



> 郭一璞 发自 凹非寺
量子位 报道 | 公众号 QbitAI

小鲜肉太多，让人分不清，需要AI替我识别，这又是谁家的爱豆。

现在，有一家公司来拯救脸盲了，不是你熟悉的硅谷/后厂村大公司，而是一家动图表情包网站：**GIPHY**。

GIPHY**开源**了一个表情包识别器，可以分清楚超过**2300**个名人，而且它识别的还是动图，准确率超过**98%**。

## **这效果，可以的**

良心的是，GIPHY不仅把这个工具开源了，而且还直接用在了自己的网站上，不是Demo，已经是成品。
![](https://pic1.zhimg.com/v2-45ec98a329d9a31c361c97e5914ff634_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='370'></svg>)
那我们来测验一下这个功能好不好用~
![](https://pic4.zhimg.com/v2-9d9e1bbef646f7890d5a05509ed5771f_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='498' height='308'></svg>)
第一张，泰勒·斯威夫特，传上去试试效果
![](https://pic3.zhimg.com/v2-d274002bd7d6ed9afb30e618195d4e56_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='248'></svg>)
认出来了，匹配度84.92%。
![](https://pic3.zhimg.com/v2-7e9956cc0952a2a818d8031cdaf1d06a_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='500' height='231'></svg>)
换个人，安妮·海瑟薇~
![](https://pic3.zhimg.com/v2-f62043c7ac797086f82d34fa3d1d069a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='249'></svg>)
这次认还是认出来了，就是AI心里不太有谱，它的把握不到6成，甚至还觉得可能是另一个妹子。

我们加大难度，看艾玛·沃森这张魔性的动图能不能认出来。
![](https://pic1.zhimg.com/v2-394cd9d554580a4e355dc3f063773698_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='500' height='232'></svg>)△ Emma觉得不行
笑成这样，估计识别起来有点难了吧？
![](https://pic2.zhimg.com/v2-e84b62c7e50a2ad7a0270f7fd626373d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='484'></svg>)
还真是，AI虽然认出来这是Emma，但是，它觉得还有4个妹子笑得跟Emma一样魔性

另外，这个AI还对一位明星表现出了迷之钟爱：
![](https://pic4.zhimg.com/v2-a37f877f4119092b829dd075cfef9883_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='500' height='357'></svg>)
赫本的匹配度，高达100%
![](https://pic2.zhimg.com/v2-745499653fccc275dff730b2fb51fa09_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='273'></svg>)
这让人怀疑AI的心理年龄……

## **模型原理**

模（mei）型（zi）测评就先到这里了，我们来看看这个模型是怎么训练出来的。

摆在GIPHY团队里的第一个问题，就是识别谁，以及用什么数据集。

作为一个表情包网站，秉持着自己动手丰衣足食的精神，先把自己家的热搜TOP 50000盘点了一下，找到了里面的所有人名，接着又用同样的方法去Facebook、Twitter等网站搜了一遍，凑齐了一个2300多名人的人脸数据库。

识别的过程则包括两部分，第一部分是人脸检测，使用**MTCNN**（arxiv: 1604.02878）预训练模型来找动图中所有帧里的人脸；第二部分是人脸识别，用前面的那个DIY的数据集在基于Resnet-50的深度卷积神经网络上训练，以识别MTCNN模型发现的每一张人脸。

最后，还需要对人脸进行聚类，给出预测结果。

模型训练完成后，在Labeled Faces in the Wild数据集上的准确度达到了96.8%。之后GIPHY还做了一个众包验证集，在这个验证集上的准确度达到了98%，覆盖度达到75%。

最后，这个模型已经在GitHub上开源了，如果想自己用的话，需要Python 3.6及以上的版本，用Linux的话则需要安装好libsm，libxext，libxrender。

进入下面传送门体验一下吧~

## **传送门**

在线使用

[https://celebrity-detection.giphy.com/](https://link.zhihu.com/?target=https%3A//celebrity-detection.giphy.com/)

开源地址

[https://github.com/Giphy/celeb-detection-oss](https://link.zhihu.com/?target=https%3A//github.com/Giphy/celeb-detection-oss)

— **完** —

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态

戳右上角「+关注」获取最新资讯↗↗

如果喜欢，请分享or点赞吧~比心❤


