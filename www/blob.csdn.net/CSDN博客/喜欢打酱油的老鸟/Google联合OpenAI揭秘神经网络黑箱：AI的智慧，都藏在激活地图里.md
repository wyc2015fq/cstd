
# Google联合OpenAI揭秘神经网络黑箱：AI的智慧，都藏在激活地图里 - 喜欢打酱油的老鸟 - CSDN博客


2019年03月17日 17:46:51[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：26标签：[谷歌																](https://so.csdn.net/so/search/s.do?q=谷歌&t=blog)[OpenAI																](https://so.csdn.net/so/search/s.do?q=OpenAI&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=谷歌&t=blog)个人分类：[人工智能																](https://blog.csdn.net/weixin_42137700/article/category/7820233)


[https://www.toutiao.com/a6665887885922664964/](https://www.toutiao.com/a6665887885922664964/)
神经网络内部，究竟藏着什么？
这是无数机器学习研究者内心的终极疑问。
现在，Google和OpenAI的一场合作，以图像分类神经网络为例，我们揭开了神经网络黑箱的一角。
他们说，图像分类神经网络里面长这样：
![Google联合OpenAI揭秘神经网络黑箱：AI的智慧，都藏在激活地图里](http://p1.pstatp.com/large/pgc-image/48afa2056fae4fc78736704eb9050437)
这不是狗头，是一系列**激活地图（Activation Atlas）**中的一张，通俗的说，就是神经网络在学习图像分类之后，在内心把所有学过的物品画了一幅思维地图，把长得像的东西放在了一起，以此作为依据，判断之后所有的它看到的图片是什么。
这个惊人的发现，让许多人振奋：
> “恭喜OpenAI，太有趣了！”

> “做的真好，他们每次都能推出厉害的项目，解答我心中的疑问。”

> “这是全新的概念啊！”

> “虽然没看懂，但是好酷啊！”

# 机器学习的字母表
激活地图可以帮助我们了解神经网络在给一张图片进行图像分类时“看”到了什么，有层次的展示出神经网络中的具体内容。
重点是，这种展示方式我们人类也可以理解，类似于机器学习的“字母表”，由简单的元素，像原子一样重组出复杂的图像。
![Google联合OpenAI揭秘神经网络黑箱：AI的智慧，都藏在激活地图里](http://p1.pstatp.com/large/pgc-image/3bc64b2ff07e4178bbdbbd5612f3d25e)
**△**教神经网络识物，是不是跟带娃差不多
示例中的激活地图是根据在ImageNet数据集上训练的卷积图像分类网络**Inceptionv1**构建的。
网络总共10层，每层有数百个神经元节点，就好像一栋一共10层的楼，每层都有数百个房间，每个房间里都住着一个神经元节点。
每个神经元节点都有自己的“喜好”，不同的神经元节点在不同类型的图像上的激活程度不同。比如一层中的一个神经元节点喜欢狗，对狗耳朵的图像有积极的反应；另一层的另一个神经元节点是个猫奴，对猫爪的图像有积极的反应。
![Google联合OpenAI揭秘神经网络黑箱：AI的智慧，都藏在激活地图里](http://p1.pstatp.com/large/pgc-image/fda7fb59ae1640569f0ca76bc56dc3d2)
**△**神经元节点：我要这个，给我买这个
把它们“喜欢”的图像列出来，根据每个神经元节点的“房间号”分布，画在“大楼1楼平面图”上，就可以组合在一起，组成单张**激活地图**，类似我们开头看到的那张像狗头一样的拼图的样子。
![Google联合OpenAI揭秘神经网络黑箱：AI的智慧，都藏在激活地图里](http://p1.pstatp.com/large/pgc-image/1fb1df8c666c4adaaddd0b44bdd90a74)
**△**神经元节点：我要这个，给我买这个
之后，把“大楼1楼平面图”、“大楼2楼平面图”……“大楼10楼平面图”集合在一起，就成为了**激活地图**。
下面就是其中两层的例子：
![Google联合OpenAI揭秘神经网络黑箱：AI的智慧，都藏在激活地图里](http://p9.pstatp.com/large/pgc-image/5cb0a66e94584058870da81dfdc45a8a)
**△**激活地图第六层MIXED4D
![Google联合OpenAI揭秘神经网络黑箱：AI的智慧，都藏在激活地图里](http://p1.pstatp.com/large/pgc-image/f05766b3bebb4eb0b2dc45375e56d10a)
**△**激活地图第七层MIXED4E
# 窥探神经网络内心世界
仔细看就能发现，每一层里类似的颜色都在临近的位置，甚至不同层的“楼上楼下”都看起来很相似。
因为神经网络是个强迫症，为了方便自己学习记忆，它把有类似喜好的神经元节点都安排在了相近的房间里，强迫他们当邻居。
所以，现在让我们走到大楼的不同区域，参观一下神经元节点们的“兴趣小组”吧~
每个方块图像，都是一个神经元节点喜欢的图片种类。“狗头图”的“左耳”上，是各种各样动物的脸：
![Google联合OpenAI揭秘神经网络黑箱：AI的智慧，都藏在激活地图里](http://p1.pstatp.com/large/pgc-image/beae540b1c1c4a8fb1243758ea7b02b2)
“额头”的部位，则是各种水果的图像：
![Google联合OpenAI揭秘神经网络黑箱：AI的智慧，都藏在激活地图里](http://p3.pstatp.com/large/pgc-image/da93fdb00034423fb6b937b6ad77e172)
“嘴巴”的部位，全是水，各种小水坑小池塘：
![Google联合OpenAI揭秘神经网络黑箱：AI的智慧，都藏在激活地图里](http://p1.pstatp.com/large/pgc-image/1cf37fcba14f402da8d7470b40eec37a)
“右脸”边缘处，就都是一些房子了。
![Google联合OpenAI揭秘神经网络黑箱：AI的智慧，都藏在激活地图里](http://p1.pstatp.com/large/pgc-image/e456cbd15c764be8b4d93639458ee13d)
所有的小图，图像挨着图像，形成渐变，进而联结在一起，构成神经网络对图片分类的认知体系。
就像这样，泰迪渐变成二哈，变成北极熊，变成狐狸：
![Google联合OpenAI揭秘神经网络黑箱：AI的智慧，都藏在激活地图里](http://p1.pstatp.com/large/pgc-image/779c145f200141569fa6684480c18cb5)
鸵鸟也可以渐变成各种不同的鸟类：
![Google联合OpenAI揭秘神经网络黑箱：AI的智慧，都藏在激活地图里](http://p1.pstatp.com/large/pgc-image/e13bea30cd3b45a6ba156e9667dac4a8)

# 利用激活地图调戏神经网络
最后，你知道了在神经网络的大楼里，哪些不同的物体是邻居，也就知道神经网络会把这些东西弄混。
看下面的激活地图，左边更像**灰鲸**，右边更像**大白鲨**。
![Google联合OpenAI揭秘神经网络黑箱：AI的智慧，都藏在激活地图里](http://p1.pstatp.com/large/pgc-image/a7147859fc97413398d3c1d4ca819853)
现在，拿一张**灰鲸**的照片，神经网络可以认出来这是灰鲸。
![Google联合OpenAI揭秘神经网络黑箱：AI的智慧，都藏在激活地图里](http://p3.pstatp.com/large/pgc-image/b799afc677c044468e5526ba0f21bbb1)
你在图片一角放一张棒球的照片，棒球的红色缝线让神经网络觉得这不是灰鲸，就朝激活图右侧偏离了一下，认为这是大白鲨。
![Google联合OpenAI揭秘神经网络黑箱：AI的智慧，都藏在激活地图里](http://p1.pstatp.com/large/pgc-image/7f5691c8f5dd416d84603d904b097dea)
现在，明白神经网络为什么会认错东西了吧？
# 传送门
Demo可玩，有高清大图，能把神经网络翻个底朝天
https://distill.pub/2019/activation-atlas/app.html
Google博客
https://ai.googleblog.com/2019/03/exploring-neural-networks.html
OpenAI博客
https://blog.openai.com/introducing-activation-atlases/
论文
https://distill.pub/2019/activation-atlas/
GitHub
https://github.com/tensorflow/lucid\#activation-atlas-notebooks
—**完**—

