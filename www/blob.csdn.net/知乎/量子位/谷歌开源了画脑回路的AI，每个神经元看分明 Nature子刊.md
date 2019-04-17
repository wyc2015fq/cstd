# 谷歌开源了画脑回路的AI，每个神经元看分明|Nature子刊 - 知乎
# 



> 方栗子 发自 凹非寺
量子位 出品 | 公众号 QbitAI

作为一只AI，该如何观察碳基生物的**脑回路**？

这个问题，是21世纪“人脑逆向工程”，通往超级AI的路上，需要解决的要事。
![](https://pic1.zhimg.com/v2-83b96d935a83e8bf6d1e6f8a062d34f8_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='270'></svg>)
谷歌AI要给动物的大脑 (或者局部) ，画出一张3D脑神经地图，来研究它们思考时，走过的神经路线。

比如，给小鸟的大脑画张地图，就能观察到它是怎么唱歌的了。

然而，这项任务并不简单，需要以**纳米级**的分辨率，给脑组织做**3D成像**。然后，算法要分析图像里的数据，找到神经元产生的突起，才能发现细胞与细胞之间的突触连接。 
![](https://pic1.zhimg.com/v2-0b507f81e3a1c5472a690a2e1258dd30_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='515'></svg>)
由于对图像质量要求很高，即便是**毫米级**的脑组织，也会产生1000TB以上的数据。

画好地图，主要的难点就是解释这些数据。于是，谷歌决定跟马普所 (MPI) 并肩战斗，就像当初英特尔和麻省理工 (MIT) 联手那样。

不过，团队表示自家的成果，比起今年3月英特尔发布的算法，**准确度**提升了一个数量级。 
![](https://pic1.zhimg.com/v2-9a712a37d22c89fe3cf79290cea712d4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='236'></svg>)
于是，谷歌的脑神经地图AI，在Nature Methods期刊上发表了。

那就一起来看一下，这地图是怎么画的吧。

## **Flood Filling三维图像分割**

首先，在庞大的显微镜数据之中，追踪**神经突起**，是个不小的**图像分割**工程。

传统算法通常分**两步**：

先是要找到突起和突起之间的**分界线**，这里会用到**边缘检测器**，或者是机器学习**分类器**。

然后，把每个像素分配到它该在的组别，没有被分界线隔开的话，就分到一起。可以用分水岭 (Watershed) 算法，Graph Cut算法也行。
![](https://pic3.zhimg.com/v2-dac66447fc59f4b1423921354d7c7c32_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='358' height='446'></svg>)
不过，谷歌团队尝试用一个基于RNN的算法，将**两步并作一步**。

把算法**种在**某个像素点上，让它在那里生根发芽——用RCNN预测哪些像素，和**开始那一点**属于同一个物体，把它们填上一样的颜色就好了。
![](https://pic1.zhimg.com/v2-6e28f7b516ad6c1d868062212ea5efbc_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='270'></svg>)
各种不同的颜色，都是Flood Filling网络自动填的。

## **ERL测量准确度**

**预期运行长度** (Expected Run Length) 这个指标，是谷歌和马普所一起设计出来的。

在一张3D大脑图像上，给定某个神经元上的某个随机点，从那里开始追踪。那么，走出**多远**，算法跟踪就会出错？这就是ERL。

其实，和**平均故障间隔** (MTBF) 道理一样，只是这里测量的是**距离**，不是时间。 
![](https://pic2.zhimg.com/v2-41c85387c558b57f387f0b2eb7037c69_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='470'></svg>)
蓝线表示，预期追踪距离 (ERL) ，可以看出随着时间的推移，进展可观。

红线表示，错把两个神经元当成一个，的频率。错误率越来越低，也令人欣慰。

## **小鸟小鸟，给我唱首歌**

大概你还记得，文章开头挖了坑的，小鸟唱歌。
![](https://pic2.zhimg.com/v2-0aefbc11c1bd51502803408bfa3c5c7d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='850' height='764'></svg>)△ 斑胸草雀，胸前有高贵的斑马纹，也叫珍珠鸟，会唱歌
谷歌团队真的给一只珍珠鸟的 (一部分) 脑回路 ，做了三维地图。
![](https://pic1.zhimg.com/v2-631d5f1e489e92a060455821678e1874_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='362'></svg>)
用参考标准 (Ground Truth) 数据集验证了一下，ERL结果表明，神经元分割的准确度，比前辈们都要好。

金黄色的小球，是两个神经元之间，突触连接的部分。
![](https://pic2.zhimg.com/v2-0f5ccbb1afc8ca2a1d278c625991cd99_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='270'></svg>)
算法出错不多，只花了很少的人力去修改。现在，马普所的研究人员已经可以用这份地图，来研究鸟鸣时的大脑活动了。

一般生物表示，天籁和音痴的脑回路，大概会很不一样吧。
![](https://pic2.zhimg.com/v2-4c0d97b0ab4d017d096e40bf548acd45_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='942' height='427'></svg>)△ 别开腔，自己人
## 开源是件幸福的事

论文发表的同时，谷歌AI不忘普天同乐，在GitHub上提供了所用模型的TensorFlow代码。

另外，还有做**可视化**用的WebGL 3D软件。

大家随意领取。

TensorFlow代码传送门：
[https://github.com/google/ffn/](https://link.zhihu.com/?target=https%3A//github.com/google/ffn/)

WebGL 3D传送门：
[https://github.com/google/neuroglancer](https://link.zhihu.com/?target=https%3A//github.com/google/neuroglancer)

(付费) Nature Methods论文传送门：
[https://www.nature.com/articles/s41592-018-0049-4](https://link.zhihu.com/?target=https%3A//www.nature.com/articles/s41592-018-0049-4)

(不付费) 旧版论文传送门：
[https://www.biorxiv.org/content/biorxiv/early/2017/10/09/200675.full.pdf](https://link.zhihu.com/?target=https%3A//www.biorxiv.org/content/biorxiv/early/2017/10/09/200675.full.pdf)

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


