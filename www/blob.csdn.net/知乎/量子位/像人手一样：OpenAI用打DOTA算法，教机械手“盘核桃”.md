# 像人手一样：OpenAI用打DOTA算法，教机械手“盘核桃” - 知乎
# 



> 栗子 夏乙 发自 凹非寺
量子位 报道 | 公众号 QbitAI




五根手指、24个关节，这只机械手从结构到大小，方方面面都像极了人类的手。

在OpenAI的实验室里，它缓缓 盘着核桃 转动着一个木块。没有人告诉它该动哪根指头，驱使它的，是在模拟器里训练出来的强化学习算法。

这套灵巧的系统，叫做**Dactyl**。
![](https://pic3.zhimg.com/v2-52991345cd85e62e643d0263b792717e_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)
它可以把一个正方体，转到指定的方向。

没有人指挥它，哪根手指往哪个方向用力，收到的指令只是**木块的朝向** (I在前，E在左，N在上) 而已。

当然，动作并不像人类盘核桃那样一气呵成，机器人要一步一步来。
![](https://pic1.zhimg.com/v2-7c1336575ed03021d98522769f7d7f24_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='186' height='160'></svg>)
﻿神经网络训练的过程中，自学了许多不同的分解动作。比如上图，用两个手指轻轻夹住方块，再用其他手指拨一拨，方块就转起来了。
![](https://pic3.zhimg.com/v2-b1a8f9d991cf9daf4d705b58afac3412_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='186' height='160'></svg>)
﻿再比如，推着方块在掌心滑动 (Sliding) 。还有，底面用不离开手心，但方块依然水平转动 (Finger Gaiting) ，堪称盘核桃的核心技巧。
![](https://pic2.zhimg.com/v2-b961feb1dd469f0349f7b754fc5c256d_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='186' height='160'></svg>)
这些技能，Dactyl都是在模拟器里学会的。不过，它能顺利地把技能迁到现实世界，就算物体不是方块，也能随机应变。
![](https://pic4.zhimg.com/v2-a003549d947eb121b3eea92ee18a5e7b_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='270'></svg>)
## **从二指夹具到五指机械手**

如果你关注机器人技能的进展，大概会发现这样的机械手，远不如两根指头的夹子常见。

各大门派都在用二指夹具，通过各种方法学习着抓取、放置、横扫等动作。

比如OpenAI自己，就展示过二指夹具的抓东西扔东西技能：
![](https://pic2.zhimg.com/v2-238eff626bb34ee110160d3eaaafa991_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='420' height='316'></svg>)
Google，曾经出动了大量的二指夹具来学习抓取：
![](https://pic3.zhimg.com/v2-35834ff794d489623d3b3ca45025c6ba_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='620' height='370'></svg>)
在李飞飞夫妇实验室[学习抓锤子](https://zhuanlan.zhihu.com/p/39062397)的机器人，也同样是两根手指：
![](https://pic2.zhimg.com/v2-45c16c05751b081e69285327f24619b5_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='448' height='249'></svg>)
控制相对简单，成本相对低廉，对于那些靠抓住、放开就能解决的问题来说，机械臂的确是个好选择。

但面对文章开头展示的那些复杂动作，两根指头就先天不足了。

更何况，人类对于和自己相似的物体，有着迷之热情的追求。

像人手一样的机械手发展得如何呢？虽说已经出现了几十年，但它们操纵物体的能力一直很有限。

OpenAI说，要靠传统的机器人学方法来解决灵巧操控问题，太慢了。

《纽约时报》说，按传统的方法，机械手只能做大量工程师编了程的事情。

现在，OpenAI展示了机械手自己学习更复杂的操作的能力。
![](https://pic1.zhimg.com/v2-63772cbd812c42e60e4e2c49ea1ac998_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='780' height='570'></svg>)
他们所用的这只结构上高度仿真的手，来自英国Shadow Robot公司。

在这只手上，OpenAI花了大价钱。Shadow官方网站没有明码标价，而根据机器人网站Android World显示，买这样一只机械手要花掉119700美元，差一点点就到了12万。

不让它多掌握点技能，哪对得起这12万刀。OpenAI要教这只机械手来转动各种各样的物体，每一种形状都重新编程当然不行。

另外，这只手有24个自由度，要控制的维度就比传统7自由度机械臂多了两倍。

OpenAI还想让它在真正的现实世界中工作，于是，传感器获取的信息又嘈杂又有延迟，有时候，当一个指尖的传感器被其他指头挡住，算法还得靠不完整的信息来运转。

面对种种困难和5根“手指”，他们祭出了前不久训练AI打DotA2所用的算法。

OpenAI Five，跨界了。

## **盘核桃技能习得之路**

Dactyl模型里，有两个神经网络各司其职：一是**视觉**网络，二是**控制**网络。

简单来说，要了解物体的情况，再把它转到对的方向。
![](https://pic4.zhimg.com/v2-29e289d1cffe88ccc6a5e61588f70283_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='663'></svg>)
两个网络，都是为了**随机应变**而生，用的方法叫做“域随机化 (Domain Randomization) ”。

## **控制网络**

先来说**控制网络**，这个网络是在MoJoCo物理引擎里面，搭了个模拟器来训练的。

不过，研究人员并不希望AI过度依赖物理原理。因为，传感器的测量数据可能嘈杂，也可能有延时，且随着时间的推移，机器人会受伤会老化，物理性质也会发生变化。

而要把技能推广到真实环境，就更加需要应付**多变的环境**。
![](https://pic3.zhimg.com/v2-35eaf100627eb3a478e39655389f0076_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)
﻿所以，这个模拟器只是对物理世界的一个“粗略近似 (Coarse Approximation) ”。

除此之外，为了培养AI的应变能力，模拟器提供了各种各样的场景，物体的物理性质和外观都可以发生非常丰富的变化。
![](https://pic3.zhimg.com/v2-331c7f55a179850896f856ffabc890b2_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='270'></svg>)
如果，一个强化学习策略，在所有的模拟场景里，都能完成任务，那么它到了真实世界，也更容易来者不拒。

这也不是全部，团队希望机器人可以在不同环境下，选择不同的动作来执行任务，于是用了**LSTM**来赋予AI一些记忆。没有这些记忆的话，机械手需要**两倍**的转动次数，才能把物体捧在正确的方向。
![](https://pic3.zhimg.com/v2-0b8d7ca2ed5a9924aca6c4e8b9e28a56_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='541'></svg>)
﻿成就了刀塔AI战队的Rapid，是一个**规模化**的近端策略优化系统。在这里，Dactyl用6144个CPU核加上8个GPU，训练了50小时，相当于尘世的一百年。

## **视觉网络**

训练之初，Dactyl就是朝着“**转动随机物体**”的目标去的，因为世界也不是只有正方体那样单调。
![](https://pic3.zhimg.com/v2-e1ade1cc9e622fa1c0313c2b978b528a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='608'></svg>)
所以，视觉的部分，用了很普通的RGB摄像头，数据用来估计物体的位置和朝向，不需要太精确。

三枚摄像头围在机器人身边，解决了模糊和遮挡的问题，足矣。﻿
![](https://pic4.zhimg.com/v2-cdea1d4836becddf115743e74f8500fb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='611'></svg>)
一个**卷积神经网络** (CNN) ，会把摄像头拍到的视频吃下去，估算物体的位置和方向，再用这些信息来指挥**控制网络**选取合适的操作。

这个网络的训练，不是在MUJOCO里，而是Unity游戏引擎，后者的视觉场景更为丰富，给了Dactyl千锤百炼的机会。
![](https://pic2.zhimg.com/v2-eaca3f6bce76986ad0f2044b8e8fb74d_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='270'></svg>)
两个网络是分开训练的，但合体之后便能直接走进真实世界，微调都不需要。

## **OpenAI踩过的坑**

除了展示成果，OpenAI的科学家们还热情地公开了采过的坑：这些方法我们试了，没用。

坑有两个：

一是降低反应时间。OpenAI目前设置的反应时间是80毫秒，比人类的150-250毫秒要低，但高于神经网络25毫秒的计算时间。他们也试过把反应时间降低到40毫秒，结果发现，消耗的训练时间更长了，性能没什么明显提升。

二是用真实数据来训练视觉策略。从尝试的结果来看，用真实数据和模型数据混合训练，与只用模拟数据相比成绩相当，于是，他们训练最终的模型时就只用了模拟数据。

除此之外，在实验过程中，OpenAI科学家们的认知还被颠覆了几次：

操纵真实世界物体不是非得用触觉传感不可。他们发现，用多种多样的传感器数据，如果模拟器不能有效地对这些数据建模，还不如用少量好建模的传感器。

为一个物体设置的随机化，会泛化到与它性质类似的其他物体。他们在教会系统操控方块之后，又弄了个八棱柱用来训练新算法，在这个过程中他们发现，为方块设计的随机化用起来完全没问题。但是，换成一个球体来训练，就不太行了。
![](https://pic4.zhimg.com/v2-8beb60bcdf7aa8056a30dbde7096d7ff_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='270'></svg>)
对于实体机器人来说，系统工程和算法同样重要。他们发现，运行的都是一样的策略，有一个工程师成绩总是比别人好……后来，经过对这位别人家孩子的仔细检查，发现他的电脑比大家都快，掩盖了一个bug。

## **传送门**

读Paper：

[https://d4mucfpksywv.cloudfront.net/research-covers/learning-dexterity/learning-dexterity-paper.pdf​d4mucfpksywv.cloudfront.net](https://link.zhihu.com/?target=https%3A//d4mucfpksywv.cloudfront.net/research-covers/learning-dexterity/learning-dexterity-paper.pdf)
博客原文：

[Learning Dexterity​blog.openai.com](https://link.zhihu.com/?target=https%3A//blog.openai.com/learning-dexterity/%23results)
— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


