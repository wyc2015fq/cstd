# 视频换脸术又升级：头、脸、眼一起动，背后的影子也清新自然 | SIGGRAPH 2018 - 知乎
# 



> 奥巴栗 发自 麦蒿寺
量子位 报道 | 公众号 QbitAI
![](https://pic4.zhimg.com/v2-61547eebb271cd1c682b5cf0a0284c3b_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)
各位说不定还记得，之前有个导演，模仿奥巴马的**声音**吐槽了川普，还把自己的**嘴**完好地贴到了奥巴马脸上。

这样，虽然嘴部有些异样，但不盯着嘴看的话，也不易察觉吐槽视频是合成的。

不过，就算只为了那一小撮**火眼金睛**的观众，科学家们大概还是要为合成视频的逼真程度赴汤蹈火。
![](https://pic1.zhimg.com/v2-d59b2f4860c9a84ec41089f9a68dc21c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='390'></svg>)
最近，普朗克研究所 (MPI) 一群技艺精湛的研究人员，表示他们是第一个，把替身的3D头部和面部动作整体搬运到目标主角脸上，的团队。

## **三位一体**

在他们的系统里，只要输入一段替身的单人表演视频，和目标主角的一段单人视频，就可以让主角学到头部和面部的所有动作。
![](https://pic4.zhimg.com/v2-89510c538d5136eea46e485df957d2c7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='608'></svg>)
视频输入后，算法会用“**面部重构** (face reconstruction) ”的方式追踪替身和主角，得到一系列参数，用来表示**头部姿势**、**面部表情**和**眼球转动**等等动作。

这些参数向量可以直接输送到主角的脸上，下一步就是**渲染**合成的主角图像。

然后重点来了，团队建造了一个拥有**时空结构** (space-time architecture)的神经网络，向它输入渲染过的**人脸参数模型**，它就能“脑补”出目标主角的逼真视频，称为**动态肖像** (video portrait) 。
![](https://pic1.zhimg.com/v2-4e341d1b6565d673f1a08a2272315368_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='306' height='148'></svg>)
团队说，这样的真实度是靠艰苦的**对抗训练**来实现的。

成果是，只要有**几分钟**的主角视频作为训练素材，替身的表演就可以获得高质量的继承。
![](https://pic1.zhimg.com/v2-64930294102c1d72eb775d8bed5432f8_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='349' height='136'></svg>)
**头发**、**脖子**、**肩膀**还有目标背景，都会随着生成的**头部**和**面部**动作，发生自然的变化。就连背景里的阴影，也能跟着前景走。

## **想调哪就调哪**

另外，如果不想让头部跟着一起动，也可以只改表情。
![](https://pic3.zhimg.com/v2-53c95a2eab7020f6d17a5c8e3e024bde_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='500'></svg>)
还有啊，如果**不想完全照搬**替身的表演，我们还可以手动调节头的朝向，脸上的表情。

**任何参数**都可以单独调，也可以整体调。
![](https://pic1.zhimg.com/v2-63854df2ce6b18d21bd7878a09b792e0_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='406' height='138'></svg>)
这就是说，**没有替身**，直接给视频里的主角改动作也没问题。眨眨眼，撇下嘴，都可以 (图示见文底) 。

## **有对比才有伤害**

至于这研究成果到底厉害成什么样，当然还是要和其他人的算法比比看。不然，极客们怎么获得碾压同行的快感？



![](https://pic4.zhimg.com/v2-30209f611116a43d131e8e2dbfd15f0f_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='408' height='230'></svg>)
首先，他们把自家的动态肖像算法和**Thies**团队的**Face2Face**做了对比。

二者相比，动态肖像大法的表情更加到位，头部动作更吻合，生成的视频也就一气呵成。

第二个对手是**Suwajanakorn**团队基于音频的配音法术。
![](https://pic3.zhimg.com/v2-13cfc4282abc299974113ad49d2479b6_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='377' height='137'></svg>)
对方的嘴唇同步很优秀，但没有给主角任何表情控制技能，效果便略显僵硬。而己方的面部、头部和眼球搭配食用，更为自然清新。

第三场比赛，是在**头部运动**的选手之间展开。
![](https://pic1.zhimg.com/v2-64930294102c1d72eb775d8bed5432f8_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='349' height='136'></svg>)
**Averbunch-Elor**团队的算法在动作上和动态肖像相差无几，但背景明显扭曲，翻了修图大计。

对此，普朗克研究所表示，这是因为对方算法是一帧一帧单独学习的，而他们是用整段视频来学姿势。

## **谦虚地说，还有局限**

虽然，现在生成的视频已经很接近真实了，但团队说这个算法还是有自己的局限性。
![](https://pic4.zhimg.com/v2-879a909b2ebcf47ab09b1951900830ab_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='408' height='230'></svg>)
比如，人物的活跃范围，超出了训练语料库 (Training Corpus) 的表情和动作，就很难高度还原替身的表演了。

但他们说，这也是多数同行会遇到的问题。

那不就是说，“**我做到的你没做到，我没做到的你也没做到**”，么？
![](https://pic1.zhimg.com/v2-63be9f7a7661254f2299337300fc8590_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='408' height='230'></svg>)
论文摘要传送门：

[https://gvv.mpi-inf.mpg.de/projects/DeepVideoPortraits/index.html](https://link.zhihu.com/?target=https%3A//gvv.mpi-inf.mpg.de/projects/DeepVideoPortraits/index.html)

完整的视频演示：
![](https://pic3.zhimg.com/80/v2-5bd403ff9302989cb013d5940617d302_b.jpg)https://www.zhihu.com/video/981529842721906688
— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai) · 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


