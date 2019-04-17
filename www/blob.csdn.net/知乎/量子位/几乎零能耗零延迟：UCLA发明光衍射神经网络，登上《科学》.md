# 几乎零能耗零延迟：UCLA发明光衍射神经网络，登上《科学》 - 知乎
# 



> 夏乙 发自 凹非寺
量子位 报道 | 公众号 QbitAI




深度学习，现在已经成立几乎每一个图像识别、语音识别、机器翻译系统的标配组件，而它的缺点也一直在被各界人士吐槽：

不够快，太耗能，不可解释……
![](https://pic2.zhimg.com/v2-c5694f48f0cb4b05c356fe7b5eba5ccd_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='432'></svg>)
加州大学洛杉矶分校（UCLA）的一组科学家们，就要从另一个角度，来解决不够快和能耗高的问题。

UCLA电子工程系教授Aydogan Ozcan带着自己的团队，把神经网络从芯片上搬到了现实世界中，依靠光的传播，实现几乎零能耗、零延迟的深度学习。

这个解决方案叫做**D2NN：衍射深度神经网络**（Diffractive Deep Neural Network）。它是光学工具、3D打印和神经网络的结合。

他们的成果，登上了Science。
![](https://pic1.zhimg.com/v2-96e9fca61f20c9d3031b11d3c9ff6e78_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='686'></svg>)
这个系统有着传统神经网络无法匹敌的优点：一是**更快**，在D2NN里，信息传递的速度，等于光速；二是**能耗接近于0**：除了最开始要提供一个光源之外，就不再需要耗电了。
![](https://pic3.zhimg.com/v2-67e7d644e000c54120140a8b217564fe_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='786' height='978'></svg>)
D2NN由多个衍射层构成，一层上的每一个点都相当于神经网络的一个神经元。它的**训练**方式和深度学习一样，只不过得到的不是神经元的权重，而是神经元的透光/反射系数。

训练完成，得到D2NN的最终设计，就到了**制造**阶段。这些衍射层会被3D打印出来，在它学会的任务上做**推断**。

在推断过程中，在这个神经网络中传递的并不是人类可见的光，而是0.4太赫兹频率的单色光。Ozcan将D2NN比作用光来连接神经元、传递信息的实体大脑。
![](https://pic1.zhimg.com/v2-23b7b1b4eebca939178ecab069786d08_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='493'></svg>)
Ozcan团队在这项研究中，制造了不同类型的D2NN，有用来给图像分类的（上图B），有用来成像的（上图C）。

一个D2NN设计出来、打印完成后，还可以继续优化。

比如说，科学家们针对MNIST手写数字识别任务，训练了一个5层的D2NN，每一层的尺寸是8cm×8cm，达到了91.75%的准确率。

然后，他们又为这个D2NN加了两层，来优化性能。于是，这个7层网络在MNIST上的分类准确率达到了93.39%。

在比MNIST稍微复杂一些的基准数据集Fashion-MNIST上，5层的D2NN最高实现了86.33%的准确率。

当然，D2NN现在还只能算是个婴儿，和卷积神经网络现在动辄99%的准确率没法比。

接下来，Ozcan团队还打算制造尺寸更大、层数更多的D2NN。

说不定有一天，这种新型的神经网络强大起来，我们会对不需耗电就能识别人脸的摄像头习以为常。

论文在此：
[http://innovate.ee.ucla.edu/wp-content/uploads/2018/07/2018-optical-ml-neural-network.pdf​innovate.ee.ucla.edu](https://link.zhihu.com/?target=http%3A//innovate.ee.ucla.edu/wp-content/uploads/2018/07/2018-optical-ml-neural-network.pdf)
— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


