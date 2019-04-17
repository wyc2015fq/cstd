# 纸片人活了！海报上的梅西、漫画里的悟空，都被AI唤醒 - 知乎
# 



> 安妮 郭一璞 发自 凹非寺
量子位 报道 | 公众号 QbitAI

今天要介绍的新技术，可能会让不少人有个大胆的想法。

因为2D形象变成3D“活物”，不再是魔法。

你可能还记得《哈利·波特》里的神奇世界，报纸上的照片会动，墙壁上的名人画像能说话，甚至开门入户都要与“门神”胖夫人暗号问答一番。
![](https://pic2.zhimg.com/v2-00893eeae7ff48a2126f243d5c88cb09_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='500' height='209'></svg>)
但，是，现，在，AI就是我辈麻瓜的神奇魔法。

无论你想“复活”的是梅西.jpg、库里.png，还是詹姆斯.jpeg，抑或希望新垣结衣.tiff、波多野结衣.Raw“动起来”……

这项名为**照片唤醒（Photo Wake-Up）**的新技术，都能让你得偿所望。

不信？先牛刀小试亮亮相~

它可以让你最爱的体育明星，詹姆斯、梅西、库里从照片里跑出来：
![](https://pic2.zhimg.com/v2-89a5bb84c44775bc232377104b376d1d_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='630'></svg>)


![](https://pic4.zhimg.com/v2-629e92c3c43e66d4cb1c8edf23f1e4ef_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='630'></svg>)


![](https://pic2.zhimg.com/v2-7ad0fa740af289ff967f5424cf7e498d_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='584' height='749'></svg>)
相信我，走出来的库里真的是3D的：
![](https://pic4.zhimg.com/v2-89c7019db5c5db5528db21f8f33e5707_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='448'></svg>)
它还能赋予二次元角色以力量：
![](https://pic4.zhimg.com/v2-73596d2624792ea0d33451212e4bcfbb_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='531'></svg>)
把钢铁侠带到你身边：
![](https://pic1.zhimg.com/v2-8defe7056e4065d13fd14f08a4518a34_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='497'></svg>)
甚至抽象画都能搞的定，毕加索《玛雅和玩具船》中的主角可以走出来，像人类那样活动四肢：
![](https://pic1.zhimg.com/v2-97302fe6bebc9eb76a26a03b433ba290_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='599' height='535'></svg>)
你还可以借此把角色们AR/VR化，只要动动手指，它们就能出现在你身边的世界：
![](https://pic4.zhimg.com/v2-8024227bccc02c76cf2e6c1703ff76ff_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='425'></svg>)


![](https://pic4.zhimg.com/v2-96cb9f6087b42eb8b006f8c7e99c22b7_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='391'></svg>)
不管是照片、卡通还是抽象角色，但凡有个人样，照片唤醒技术都能唤醒它们体内沉睡的灵魂，让它们从二维化世界中苏醒过来。

有了这项技术，或许游戏、动画制作行业的设计师们可能要开心坏了，只要画个2D的角色，自动就能变成3D。

看到这些效果展示，有网友表示可能这就是和女神距离最近的一次了。

恭喜恭喜~

## **原理揭秘**

这些心水的动漫人物和体育明星，是怎么一下子从画里走出来，而且还会动、变立体了？

在论文Photo Wake-Up: 3D Character Animation from a Single Photo中，研究人员来了一次完完整整的**大、揭、秘**。

研究人员表示，让主角从平面静态图像里走出来，最难的地方就是要在系统中重建一个“虚拟人”。虚拟人必须能透过厚重的衣物，精确对应到画中人物的身体轮廓。

头对头，肩并肩，手腕对手腕……当虚拟人的骨骼架构完全匹配到画中人，也就意味着你的男神/女神离动起来就不远了。

整个操作过程，其实只用了三步：
![](https://pic2.zhimg.com/v2-843f04f99905c312cc66023312b99fd5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='245'></svg>)
我们一步一步看，研究人员的**第一步**就是：分割纸片人。

研究人员利用现成的图像分割算法，将画中人的躯干、四肢分割，再用人体姿态估计，提取画中人的2D骨骼关键点，忽略厚重的衣服，拟合出一种半裸的可塑3D模型，称为**SMPL模型**（Skinned Multi-Person Linear Model）。
![](https://pic2.zhimg.com/v2-964e038d22af5f42631bdd214a5a96c9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='495'></svg>)
此时，无论是男神还是女神，现在都已经简化成看不出年龄和样貌的“石膏人”了。重要的第二波操作马上开始。

**第二步**：细化纸片人3D模型

纸片人跑不跑，全看3D模型好不好。这一步，研究人员开始细化纸片人的3D模型了。

若纸片人运动，身体各部分间难免产生轮廓的重合。完美的模型能够匹配到纸片人的身体轮廓，还能识别这种自我遮蔽（self-occlusion）现象的出现。
![](https://pic2.zhimg.com/v2-feaa14b06a96c82b95f9ad58d09a5c11_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='640'></svg>)△ 比如这样
怎样在自我遮蔽出现时保证模型的准确性？研究人员想了个主意：先将SMPL模型**映射到2D世界**对准轮廓，调好再次转换成3D模型确保精确度。

在映射到2D世界时，研究人员将SMPL模型转换成了两种不同的映射形式，一种是肌肉隐约可见的**普通图**（Normal Map），一种是五颜六色区分了身体不同部位的**皮肤图**（Skinning Map）。

二者合一，转换成3D模型后，姿态可动的3D人来了！

**最后**，研究人员用CMU的Mo-cap人体动作数据集喂给模型，数据集中的人物动作，现在可以迁移到你的纸片人身上了。
![](https://pic2.zhimg.com/v2-5d2bfb647c779866e07605aa2b61ba01_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='500'></svg>)△ 6幅画结果
此时，如果有一个AR眼镜，戴上，手指比个心，心爱的“纸片人”就被点活，从画中出走了。
![](https://pic3.zhimg.com/v2-44f507500c530c434afae91e19bc87ee_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='297'></svg>)
随后，研究人员对103名志愿者进行了调查，86%的被调查志愿者认为Photo Wake-Up的效果超越了前人所做。
![](https://pic4.zhimg.com/v2-09004270768a50186583554f9739db87_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='465'></svg>)
## **艾伦学院三人组**

这项研究的三位作者，都来自华盛顿大学保罗·艾伦计算机科学与工程学院。
![](https://pic1.zhimg.com/v2-e35b62cca9f0d24b7623bd772212e788_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='164' height='220'></svg>)
一作翁仲毅，一位台湾小哥哥，现在是华盛顿大学保罗·艾伦计算机科学与工程学院图形图像实验室（GRAIL）的博士，他本硕毕业于台大计算机与信息工程系，读博士前曾在台湾讯连科技做技术主管，最近还在Facebook实习。
![](https://pic1.zhimg.com/v2-6018ec4138b55c70fe43dde373e84e4c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='632' height='442'></svg>)
二作Brian Curless，华盛顿大学保罗·艾伦计算机科学与工程学院教授，斯坦福电气工程博士。
![](https://pic2.zhimg.com/v2-a2f1187019ce8aea415f9f667656e0ed_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='500' height='639'></svg>)
三作Ira Kemelmacher-Shlizerman小姐姐，以色列魏茨曼科学研究所计算机科学与应用数学博士，华盛顿大学保罗·艾伦计算机科学与工程学院助理教授。

同时，因为自己创立的公司Dreambit LLC被Facebook收购了，所以自从2016年起她还是Facebook的AR/VR研究科学家，并且长期担任CVPR和SIGGRAPH的区域主席和技术委员会成员。

这个研究团队，从一作到三作，那是一个比一个厉害。

量子位此前介绍过的桌面足球，就是二三作两位教授的作品；
![](https://pic1.zhimg.com/v2-ccbf940207f6b0b3ace1e197b23ea258_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='360'></svg>)
还有假装拉小提琴的Facebook AI，也是三作小姐姐参与的研究。
![](https://pic2.zhimg.com/v2-365ec4685da012ab9106c7ce51425241_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='360'></svg>)
感谢作者们的研究，从此地球人再也不怕被《三体》中的二向箔攻击了，就算被压成纸片片，我们也可以把自己复原回3D。

## **传送门**

项目网页

[https://grail.cs.washington.edu/projects/wakeup/](https://link.zhihu.com/?target=https%3A//grail.cs.washington.edu/projects/wakeup/)

论文

[https://arxiv.org/abs/1812.02246](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1812.02246)




— **完** —

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态
[a​www.zhihu.com![图标](https://pic4.zhimg.com/v2-ca6e7ffc10a0d10edbae635cee82d007_ipico.jpg)](https://www.zhihu.com/org/liang-zi-wei-48)
欢迎大家关注我们，以及订阅[我们的知乎专栏](https://zhuanlan.zhihu.com/qbitai)

**诚挚招聘**

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！

相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。


