# Google惊人研究：一组图片，就能强迫神经网络执行其他任务 - 知乎
# 



> 夏乙 发自 凹非寺
量子位 出品 | 公众号 QbitAI

还记得那些把熊猫认成猩猩、把乌龟认成枪、把枪认成直升机的算法吗？
![](https://pic4.zhimg.com/v2-fa9bf8e4fc95cb212b89e55f5da76797_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='285' height='161'></svg>)
它们遭遇的，是一个名为“**对抗攻击**(adversarial attacks)”的敌人。这个敌人每次出现，都能让图像识别算法不知所措。

现在，更丧心病狂的来了。

谷歌大脑三位研究员Gamaleldin F. Elsayed、Ian Goodfellow、Jascha Sohl-Dickstein的最新论文展示了一种新型对抗攻击手段，AI前所未遇的强大敌人。

他们说，对抗攻击不仅能让图像识别模型认错图，还能对被攻击模型进行重新编程，让它们抛弃本职任务，去干一些**由攻击者指定的，别的事情**。他们将这种偷天换日指派的事情称为“**对抗任务**”。

就算是模型根本没有这种技能，也没关系。所需要的，仅仅是在测试图像上加入一些对抗扰动信息。

比如说，让ImageNet分类器改行去**数方块**。

实现的过程并不复杂，总共分三步。
![](https://pic1.zhimg.com/v2-2d17fa59f3a47eb9c345c750c582a5d8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='906'></svg>)
首先，要在ImageNet标签和对抗任务标签之间建立映射。在这个例子里，就是将ImageNet的类别，**映射**到方块的数量，鲤鱼是1个方块，金鱼是2个方块，白鲨是3个方块……

映射建立好之后，就要把表示对抗任务的图片嵌入到一个对抗程序图片的正中间，得到用来攻击神经网络的**对抗图片**。

接下来，就该让对抗图片和目标模型见面了。

二者见面之后，目标模型就放弃了原本的图像识别任务，只会数图上究竟有几个方块。

重新分配的任务也可以比数方块复杂一点，比如说，让ImageNet分类器以为自己是个只会识别手写数字的**MNIST分类器**。
![](https://pic2.zhimg.com/v2-a1c05b95186d9505ecf14345fb0dd35d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='621'></svg>)
用来实现这个任务的对抗图片，就长成上图的样子。
![](https://pic3.zhimg.com/v2-d5fe525b60b551272951a3cecc0dc15a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='408'></svg>)
同样的方法，还可以让ImageNet分类器变成**CIFAR-10分类器**。

惨遭他们毒手的，有六个ImageNet图像识别模型，包括三种Inception变体和3种Resnet变体。

还好还好，没让神经网络去干什么惊世骇俗的事儿。

但是要知道，这项研究才刚刚起步，以后会发展到什么程度，谁也不好说……毕竟，以前并没有人给神经网络挖过这样的陷阱。

Elsayed等人在论文中也说，这些结果首次展示了这类攻击的可能性。

这一研究的三个作者，全部来自Google Brain团队。

其中第一作者**Gamaleldin F. Elsayed**，去年从哥伦比亚大学获得博士学位。目前，他其实是一位Google AI Residency成员，也就是相当于实习或者访学的身份。

第二作者**Ian Goodfellow**，大名鼎鼎。通常也被称作生成对抗网络（GANs）之父，人工智能领域的大牛。早年间，他致力于教神经网络造假骗人；现在，他的大部分研究集中在对抗攻击领域，专注于欺负神经网络。

第三作者**Jascha Sohl-Dickstein**，2012年在伯克利获得博士学位。加入Google之前曾在斯坦福做访问学者。

Jascha把他们的这个研究发到了Twitter上，立刻引发了大量用户的转载。不过留言评论的目前只有一个人。

那个人问：**你们什么时候发布源代码？谢。**

……

## **论文**
![](https://pic4.zhimg.com/v2-4c320dccf4c90f4a9bcee8f69a49d81f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='254'></svg>)
更多详情，请看他们的论文：

Adversarial Reprogramming of Neural Networks
Gamaleldin F. Elsayed, Ian Goodfellow, Jascha Sohl-Dickstein
[https://arxiv.org/abs/1806.11146](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1806.11146)

当然，如果你想直接下载pdf版本，也可以在量子位公众号（QbitAI）对话界面，回复：“**攻击**”两个汉字即可。

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai) · 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


