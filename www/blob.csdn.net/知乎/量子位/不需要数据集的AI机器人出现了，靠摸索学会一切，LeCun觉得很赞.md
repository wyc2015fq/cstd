# 不需要数据集的AI机器人出现了，靠摸索学会一切，LeCun觉得很赞 - 知乎
# 



> 郭一璞 发自 凹非寺 
量子位 报道 | 公众号 QbitAI

伯克利的研究人员最近利用视觉模型强化学习训练了一个不一样的机器人，能够通过自己的探索，完成各种任务：整理玩具、叠衣服、收拾餐具……

并且，这样一个多功能机器人的训练过程是无监督的，不需要投喂数据，完全是机器人自行摸索后学到的。

也就是说，这个机器人看了看你杂乱的房间，就自主的收拾了起来。

帮你把乱放的苹果放进盘子：
![](https://pic1.zhimg.com/v2-953a855b3d55a11d3efab93a236bc618_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='450'></svg>)
帮你叠起秋衣：
![](https://pic4.zhimg.com/v2-7d0419f7fa1b6271720e88aedd699efb_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='450'></svg>)
整理玩具：
![](https://pic3.zhimg.com/v2-bec31a330aee94576110c5af4d68ba3e_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='450'></svg>)
咦，二胡卵子误入。

完成这一系列工作的，都是同一套算法。

这些神奇的技能，Yann LeCun看到都惊呼：Awesome！
![](https://pic4.zhimg.com/v2-1a27de7b626d0e7c481b44613e41328b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='1004'></svg>)
## **像小朋友一样摸索世界**

我们开头说过，这个机器人不需要人为投喂数据。

那么数据从哪儿来呢？从需要它工作的现实世界来。

在一个摆放了各种物体的“房间”内，可以让机器人自由摸索，感受房间里的一切，在无监督的情况下，机器人完全可以自己玩自己的。
![](https://pic3.zhimg.com/v2-d68d21d1565980cebcb6c1bab66c5f92_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='320' height='240'></svg>)
除了玩杯子、玩具这些硬的东西，还可以玩“软体”的毛巾：
![](https://pic2.zhimg.com/v2-9ca1545b66dc5c376390b259b0b904f1_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='320' height='240'></svg>)
玩的过程中，机器人就通过各种传感器，学到了视觉能力，知道自己身边是什么东西；

学到了定位能力，知道自己在做什么；

学到了做不同的动作，知道自己发出不同命令后，手臂会做什么；

学会熟练使用自己的“手”，可以预测到手做什么动作会对环境中的物体产生怎样的影响。

整个探索过程没有分数、不设输赢，完全是机器人自己凭借“好奇心”，通过摸索这个房间里的物品，形成自己的一套“世界观”的过程。

## **派活就行，不用你教怎么干**

当机器人已经对环境中的物体了如指掌之后，我们该怎么给它安排工作呢？

用像素点。

在整个环境中，通过像素点给机器人标注任务目标，红色为起始点，绿色为结束点，也就是告诉机器人把红点位置的东西搬到绿点位置。
![](https://pic1.zhimg.com/v2-7c9cf77aaea9af36e8422a6e2a783f70_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='484' height='360'></svg>)
机器人，你来把红点位置的苹果，挪到绿点位置的盘子上去。

机器人想了想，大概只要把苹果夹起来，把手臂挪过去，把苹果放下，就可以了把？
![](https://pic2.zhimg.com/v2-49adb22fa84bda74e96b032fec2d2609_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='280' height='213'></svg>)△ 机器人内心的“草稿”
那就这么办了，召唤机械臂，是骡子是马，我们挪一个看看。
![](https://pic3.zhimg.com/v2-893f66f2b848165e6853283f7e36863a_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='450'></svg>)
Bingo！成功。

下一个任务，我们把牛仔热裤叠起来。
![](https://pic2.zhimg.com/v2-eec62677e9309a8233c4b16a81de3bed_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='482' height='362'></svg>)
想一想，揪住一个角，挪过来就好了吧？
![](https://pic3.zhimg.com/v2-83fc16d7f1af8dbcfd9dc15a36f5fc2e_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='280' height='213'></svg>)
根据这个计划试一试：
![](https://pic3.zhimg.com/v2-e27837db8a73fe5321d66d578fda9b5a_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='450'></svg>)
完美成功~

这个过程具体是怎样的呢？伯克利官方给出了一个视频，可以打开学习一下：

## **传送门**

最后，照例附上传送门。

论文：

**Visual Foresight: Model-Based Deep Reinforcement Learning for Vision-Based Robotic Control**
Frederik Ebert, Chelsea Finn, Sudeep Dasari, Annie Xie, Alex Lee, Sergey Levine
[https://drive.google.com/file/d/1scfbONOHg8H2_pJ9naRkHfk4dGSNGNWO/view](https://link.zhihu.com/?target=https%3A//drive.google.com/file/d/1scfbONOHg8H2_pJ9naRkHfk4dGSNGNWO/view)

博客：

**Visual Model-Based Reinforcement Learning as a Path towards Generalist Robots**
[https://bair.berkeley.edu/blog/2018/11/30/visual-rl/](https://link.zhihu.com/?target=https%3A//bair.berkeley.edu/blog/2018/11/30/visual-rl/)

至于开源代码嘛，官方表示coming soon，或许过一段时间能在这个页面里找到：
[https://sites.google.com/view/visualforesight](https://link.zhihu.com/?target=https%3A//sites.google.com/view/visualforesight)




— **完** —

量子位

վ'ᴗ' ի 追踪AI技术和产品新动态

欢迎大家关注我们：
[量子位​www.zhihu.com![图标](https://pic4.zhimg.com/v2-ca6e7ffc10a0d10edbae635cee82d007_ipico.jpg)](https://www.zhihu.com/org/liang-zi-wei-48)
以及订阅我们在知乎上的专栏
[量子位​zhuanlan.zhihu.com![图标](https://pic3.zhimg.com/v2-7e0d98f2a88465d438867a387543a5ca_ipico.jpg)](https://zhuanlan.zhihu.com/qbitai)
诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！

相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。


