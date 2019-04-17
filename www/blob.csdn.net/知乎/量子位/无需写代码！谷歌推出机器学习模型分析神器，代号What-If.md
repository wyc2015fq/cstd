# 无需写代码！谷歌推出机器学习模型分析神器，代号What-If - 知乎
# 



> 铜灵 编译整理
量子位 出品 | 公众号 QbitAI

今天，谷歌推出了已开源的TensorFlow可视化工具TensorBoard中一项新功能：What-If Tool，用户可在不编写程序代码的情况下分析机器学习（ML）模型。

不用写代码？

没错，只需为TensorFlow模型和数据集提供指针，What-If Tool就能给出一个可用来探索模型结果的可交互的视觉界面。
![](https://pic2.zhimg.com/v2-a0039c469543fdba3f76dbf0e11b3671_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='361'></svg>)△ 250张人脸和在模型中检测微笑后的结果
What-If Tool里功能很多，包括自动用Facets将数据集可视化，也有从数据集中手动编辑示例并查看更改效果的功能，还能自动生成部分关系图，显示模型预测随着单个特征的改变而改变的趋势。

不过，这还不是What-If Tool的全部实力。

## **7大功能**

What-If Tool主要有七大功能，不知道有没有你需要的那一款：

**功能一：可视化推断结果**

根据推断结果的不同，你的示例会被分成不同的颜色，之后可用混淆矩阵和其他自定义形式进行处理，从不同特征的角度显示推断结果。
![](https://pic1.zhimg.com/v2-cb2edcce5d53b2b754850f15e70a83b8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='469'></svg>)
**功能二：编辑一个数据点，看模型表现如何**

你可以编辑、添加或删除任何选定数据点的特性或特性值，然后运行推断来测试模型性能，也可上传全新示例。
![](https://pic2.zhimg.com/v2-3778abf802580cc695a88e4ed1343775_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='492'></svg>)
**功能三：发掘单个特征的效果**

可以用来探索为选中数据点中的单个特征自动生成的图，显示特征有效值不同时推断结果的变化。
![](https://pic4.zhimg.com/v2-267030ca1b1cefb49a8fbc8b9d76a7e7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='481'></svg>)
**功能四：探索反事实示例**

轻轻一点，你就能比较数据点与模型预测出不同结果的最相似点。我们将这些点称为“反事实”（Counterfactuals），可以反映出模型的决策边界。
![](https://pic4.zhimg.com/v2-c52adcc1af853037228e4caa9a55f697_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='479'></svg>)
**功能五：按相似度排列示例**

用L1或L2距离从选定的数据点创建距离特性，并将其可视化进行进一步分析。
![](https://pic3.zhimg.com/v2-6cc580aefe72712a51510f94c108c112_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='484'></svg>)
**功能六：查看混淆矩阵和ROC曲线**

对于包含描述真实标签特性的二分类模型和示例，使用阈值、ROC曲线、数值混淆矩阵和成本比交互式地探索模型性能。
![](https://pic3.zhimg.com/v2-9d0505ae55d0de3923712bb194519516_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='492'></svg>)
**功能七：测试算法公平性限制**

对于二分类模型来说，这个工具可以将你的数据集分成子数据集，继而探索不同算法公平性约束（fairness constraints）的影响。
![](https://pic4.zhimg.com/v2-e59ae95cd69c3fb407ae5b62f3dede83_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='488'></svg>)
**传送门**

在谷歌官方博客上，研究人员还发布了用预训练模型进行的一组演示，比如检测错误分类的原因，评估二元分类模型的公平性和调查不同子数据集中模型的表现等。可以移步官方博客查看更多，博客地址：
[https://ai.googleblog.com/2018/09/the-what-if-tool-code-free-probing-of.html​ai.googleblog.com](https://link.zhihu.com/?target=https%3A//ai.googleblog.com/2018/09/the-what-if-tool-code-free-probing-of.html)
What-If Tool介绍主页：
[What-If Tool​pair-code.github.io](https://link.zhihu.com/?target=https%3A//pair-code.github.io/what-if-tool/)
— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


