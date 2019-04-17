# 从没见过干净图片，英伟达AI就学会了去噪大法| ICML论文 - 知乎
# 



> 润滑栗 发自 凹非寺
量子位 出品 | 公众号 QbitAI
![](https://pic2.zhimg.com/v2-217ee2d9eb5398a224d4bb262c4eb071_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='400' height='225'></svg>)△ Noise2Noise：我有特别的降噪技巧
如今，会**去噪**的神经网络，早已算不上稀有物种。

不过，英伟达的**Noise2Noise**，和普通的降噪AI还是有些不一样。

一般训练去噪技能，就需要给神经网络，喂食**成双成对**的图像。
![](https://pic3.zhimg.com/v2-5316607a0499fc004acc4c4275bf9ea2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='445'></svg>)
一张清晰，一张噪点满满。AI会在大量的**对比**中，习得去除噪音的方法。

但Noise2Noise的食谱里，**没有**清晰的图，只有孤单的满是噪音的图像。

即便如此，训练完成的AI依然能够了解，怎样的图像才是**干净**的，并以**毫秒级**的速度去噪。
![](https://pic1.zhimg.com/v2-55bcdc49d8fbcdcf9823baaa4e4e02c4_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='372' height='214'></svg>)
这样的学习能力，被**ICML 2018**选中了。

## **脑补清晰的信号**

Noise2Noise，是英伟达和阿尔托大学，以及麻省理工 (MIT) 共同的作品。
![](https://pic2.zhimg.com/v2-6a58a460f0168eeb793e40511cd9736d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='519'></svg>)
既然，没有**清亮**与**浑浊**相互对照，神经网络就要学习，**直接**把自己观察到的、充满噪点的景象，和素未谋面的、清晰的信号，建立联系 (mapping) 。

听上去可能有些匪夷所思，不过训练好的AI，只要观察图像**两次**，便可以轻松处理**各种各样**的噪音。
![](https://pic2.zhimg.com/v2-c2f5826502e156855eae6b27f1455a45_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='270'></svg>)
文章开头有**泊松噪音**的栗子，上图则是**高斯噪音**。

还有一种叫做**脉冲噪音**的怪兽，看上去很厉害，但瞬间就被脱了皮——
![](https://pic1.zhimg.com/v2-16d28a89c1de2f967589cae0675b03bc_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='270'></svg>)
另外，**清除弹幕**虽然不像个有用的功能，但要还原被文字挡住的画面，也并不容易。

而Noise2Noise的疗效依然显著。
![](https://pic1.zhimg.com/v2-f5bf86d66c3d004f26e3c833d1cd811c_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='270'></svg>)
连白色建筑物的纹理，都不太看得出修饰过的痕迹。
![](https://pic2.zhimg.com/v2-f29b0af9e43302ba2495612950a43251_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='487' height='449'></svg>)
这些都是，用ImageNet数据集里的50,000幅图像，训练的结果。

## **有关键用途 · 传送门**

如果，清弹幕的工作，不必劳动神经网络的大驾，那么处理医学影像，应该算得上重要的应用场景了。
![](https://pic2.zhimg.com/v2-33fa4c29222aa52bc2fdaca89acc5461_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='500' height='281'></svg>)
头部核磁检查的去噪结果，或许可以帮助医学工作者，做出更有效的诊断。

感觉有用的各位，请前往以下地址查看论文：

[https://arxiv.org/pdf/1803.04189.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1803.04189.pdf)

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


