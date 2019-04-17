# 手把手教你训练一个超越科比的投篮AI，不服来练｜附开源代码 - 知乎
# 



> 原作：Abe Haskins
安妮 编译整理
量子位 出品 | 公众号 QbitAI

在这篇教程中，谷歌工程师Abe Haskins用简洁易懂的语言，教你用Unity3D和TensorFlow生产一只会投篮的AI。

不少人表示，文章中的这些gif图能看一天！教你用AI秒杀NBA球员的校车马上就要发车了，还不快准备上车？
![](https://pic4.zhimg.com/v2-a8919f3532a0940d6d049d37b81c427b_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='580' height='325'></svg>)
## **准备阶段**

在开始正式训练前，先检查下自己是不是准备好了下面这些工具：
- 模拟篮球和物理环境Unity
- 用于训练模型的Node.js和TensorFlow.js
- 通过ML-agent asset包将模型嵌入Unity的TensorFlowSharp
- 将TensorFlow.js转换成Unity中可用图的tsjs-converter
- 将线性回归简单可视化的Google Sheets

所需工具就是这些了，接下来我将手把手教你解释清楚这些工具是怎样应用的。我建议大家先去将开源代码下载下来，方便后续的学习和理解。

**代码地址：**

[https://github.com/abehaskins](https://link.zhihu.com/?target=https%3A//github.com/abehaskins)
![](https://pic3.zhimg.com/v2-2a85fb037b591acbc00bc8ee98cfbe62_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='427'></svg>)
## **明确目标**

把目标想得简单些更有利于完成目标，我们可以把这个任务想象成：如果投球手距离篮筐的距离为x，用y程度的力度投球则会进球的简单问题。

这样一想是不是觉得模型都简单了好几度？如果你对如何在Unity中制作更复杂的AI感兴趣，你可以从Unity中查看完整的ML-Agents项目。

ML-Agents项目地址：

[https://github.com/Unity-Technologies/ml-agents](https://link.zhihu.com/?target=https%3A//github.com/Unity-Technologies/ml-agents)

## **篮筐和球**

在这个游戏中，我们只需要投篮就好了。首先需要用到的就是Unity，你平时打游戏时看到的那个立方体的logo就是Unity的标志。它是一个游戏引擎，可以为所有平台构建2D和3D游戏。
![](https://pic2.zhimg.com/v2-155d5e1d411f48de9045b6c3873ed515_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='286'></svg>)
Unity内置物理世界系统，还包括基础3D建模和脚本运行时（Mono），支持用C#编程。

把球场的大环境拼接起来并不复杂：
![](https://pic4.zhimg.com/v2-8e08050b7e9446d302edb73117c32fe3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='359'></svg>)
没错，图中的红色块就代表玩家。模拟篮筐上设置有隐形触发器，帮助检测小球通过篮筐的具体时间。

触发器使用教程：

[https://unity3d.com/cn/learn/tutorials/topics/physics/colliders-triggers](https://link.zhihu.com/?target=https%3A//unity3d.com/cn/learn/tutorials/topics/physics/colliders-triggers)
![](https://pic1.zhimg.com/v2-2229cc8d0ae60c8c56aa9d9563c69998_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='311'></svg>)
如上图所示，在Unity编辑器中，肉眼可见绿色的隐形触发器。可观察到触发器分为上下两个，这样就能计算出球从篮筐顶部到底部落下消耗的时间。这两个触发器可在/Assets/BallController.cs中的OnTriggerEnter中查看。
![](https://pic3.zhimg.com/v2-71f8563fabdfb33a65b1451895803266_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='327'></svg>)
上下都有触发器的另一个好处就是，可以据此判断小球的材料，帮助掌握distance和force.y两个核心变量。

## **投篮**

打开/Assets/BallSpawnerController.cs，这是一个负责生产篮球并试图投中篮筐的脚本。你也可以在Shoot()结尾处查看这段代码。
![](https://pic4.zhimg.com/v2-01e5f24e9a24d1af2ef86a88b134570b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='348'></svg>)
不知道你有没有注意到，Instantiates代码处表示了一个全新的实例，可以在此处设置投篮的力度和距离篮板的距离。

如果你是在/Assets/BallController.cs处打开的，可以查看Start()方法，之后在加入新篮球时会在此用到这段代码。
![](https://pic2.zhimg.com/v2-1f8010146f62f41ece9fa936141933b1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='276'></svg>)
换句话说，我们创建一个小球，然后给它一定大小的力，设置30秒后球将被自动销毁，来确保我们能处理源源不断新出现的球。

设置好一切后，我们看看自己的这个全明星投篮手是如何投篮的。你可以点击Unity编辑器中的播放按钮，我们会看到……
![](https://pic3.zhimg.com/v2-e48447181592bedbfe91f0c1fec57a22_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='780' height='441'></svg>)
百发百不中！

为什么会出现这样的情况？答案就在于Assets/BallController.cs代码中float force = 0.2f这一行。这条线让所有球的行动轨迹保持一致，所以出现了一发都没进的惨烈结局。

当然~这不是我们想要的，所以我进行了下一步的与探索。

## **随机投篮，收集数据**

我们可以通过简单将力量变为随机的方式，引入一些随机噪音。

这样就能推测出，能够投中的力度是什么样的。当然这需要一些时间~
![](https://pic2.zhimg.com/v2-16295cfe341b6167726126837aa8b949_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='660' height='373'></svg>)
如图所示，红色代表没有投中的球，绿色代表已经入篮的球。此时，任何力度的投篮都是我们可以使用的数据点，下面也会谈到这些。

同时，我们需要考虑到，我们希望红色的小球能从任何位置成功投篮，因此我们需要的不仅仅是只从一个地方投篮。在Assets/BallSpawnController.cs位置中，查找这些行并去掉MoveToRandomDistance()即可解决这个问题。

这样，如果我们再次运行，就可以看到红球在每次投篮后都能在球场上热情地跳跃。
![](https://pic4.zhimg.com/v2-8fb924090f7f8b32ffa598a9678fbddb_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='620' height='350'></svg>)
随机运动和随机力量的结合创造了一种非常奇妙的东西：数据。如果此时查看Unity中的控制台，你能看到每次投篮时记录的数据。
![](https://pic1.zhimg.com/v2-376fcb96592997637727ce5dd220d96c_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='452'></svg>)
每次成功投进都会被记录，并且到目前为击球的数量，距离篮筐的距离以及击球所需的力量都可查看。这些数据提升的速度比较慢，我们可以将MoveToRandomDistance()中的参数0.3f（每次投篮的延迟为300毫秒）改成0.05f（延迟50毫秒）。
![](https://pic3.zhimg.com/v2-89a53cb4e90b6bd4e841f1f30f3358d2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='147'></svg>)
现在，看看我们参数调试后的结果：
![](https://pic2.zhimg.com/v2-a0bdb09a4ce0b7590edfce5d605533d5_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='357'></svg>)
有没有发现，这其实是个不错的训练方式，系统显示成功率为6.4%了。下一步，我们准备将这些数据从Unity中提取出来，并构建一个模型预测所需要的力量。

## **预测、模型和回归**

在用到TensorFlow之前，我们先让Unity环境中的红球完成大约50次命中。

此时如果查看Unity项目的根目录，则应该能够看到一个名为successful_shots.csv的新文件。这是来自Unity的原始转储，我们把这份数据导出来，以便在excel中分析它。

这个.csv文件只有三行，index，distance和force。我在谷歌表格中导入这个文件，创建了带趋势线的散点图，这样就能了解数据的分布情况了。
![](https://pic1.zhimg.com/v2-097c5bfe592e1abc2540cd2fa1459e80_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='279'></svg>)
这些点组合起来可以发现一些规律。y轴为力度、x轴为距离。可以看到，所需的力与投篮距离之间有明显相关性，也有一些例外情况。

是时候让TensorFlow登场了，帮助我们用类似的代码创建更复杂的模型。比如，在一个完整的游戏中，我们可以涵盖进一些新功能。

现在需要创建我们的TensorFlow.js模型了。

在你喜欢的编辑器中打开tsjs/index.js文件，这是一个基于数据训练模型的脚本successful_shots.csv。训练和保存模型的方法如下：
![](https://pic4.zhimg.com/v2-2e8760e55450bf688d9bada71ecb35cf_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='583'></svg>)
我们从.csv文件中加载数据创建一些列x和y坐标点，让模型学会服从这些数据，之后，保存！

遗憾的是，TensorFlowSharp并不接受Tensorflow.js可以保存成的模型格式，需要我们做些翻译工作才能将模型引入Unity。

一般来说可以将TensorFlow.js Format转化为Keras Format，创建一个检查点；我们也可以将它们与Protobuf Graph Definition合并，创建检查点。

现在，我们看看在Unity里Assets/BallSpawnController.cs下的GetForceFromTensorFlow()中的模型是什么样子的。
![](https://pic2.zhimg.com/v2-a45b87d8b9fe351cbdfbcd0f74061a19_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='330'></svg>)
## **比赛日**

利用上面已经完成的系统，我们在模型上创建了一写变体，在训练500次时，模拟篮球的运行轨迹是这样的：
![](https://pic2.zhimg.com/v2-decda92b484494edf196cf941932464d_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='357'></svg>)
正确率提升了10倍！

这还只是训练500次的结果，剩下的交给你了~别忘了回头看看我最后生成的结果，这是多么可爱的游戏啊~

## **传送门**

博客原文地址：

[https://medium.com/tensorflow/tf-jam-shooting-hoops-with-machine-learning-7a96e1236c32?linkId=54634097](https://link.zhihu.com/?target=https%3A//medium.com/tensorflow/tf-jam-shooting-hoops-with-machine-learning-7a96e1236c32%3FlinkId%3D54634097)

开源代码在此：

[https://github.com/abehaskins](https://link.zhihu.com/?target=https%3A//github.com/abehaskins)

祝你好运~

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


