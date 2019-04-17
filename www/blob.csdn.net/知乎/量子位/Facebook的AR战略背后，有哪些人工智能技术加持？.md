# Facebook的AR战略背后，有哪些人工智能技术加持？ - 知乎
# 



陈桦 李林 编译整理
量子位 报道 | 公众号 QbitAI

太平洋时间周二的Facebook开发者大会F8，时刻没离开这家公司接下来的重点：AR。

F8首日Keynote演讲，近一半的时间用来谈AR；大会刚一开始，Facebook CEO扎克伯格就发布了AR平台Camera Effects，还将AR称为“下一代计算平台”；会上发布的，还有开发者工具AR Studio和Frame Studio。

![](https://pic2.zhimg.com/v2-fc9c0f3ba2bd20122f2bdb5ec196934d_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='416'></svg>)不是微软HoloLens那种AR，是Pokemon Go那种。更确切地说，“相机公司”Snapchat那种。

# AR背后

Facebook CTO Mike Schroepfer上台详细介绍了AR平台背后的技术：AI；人工智能实验室的负责人Yann LeCun也发Twitter说，驱动增强现实的，是深度学习和计算机视觉技术：SLAM(即时定位与地图构建)、卷积神经网络、Mask R-CNN。

![](https://pic2.zhimg.com/v2-1aa227acbae1343da5d7b337c83e7099_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1152' height='214'></svg>)在过去这些年里，Facebook组建了一支强大的人工智能专家团队，负责人正是LeCun。这支团队投入了很大的精力，试图让人工智能去理解静态照片和视频中的内容。

从人工智能的发展角度来看，LeCun领导这些项目很重要，但从Facebook的产品和服务角度来看，我们此前一直不知道它们会往什么方向发展。

或者说，这些技术对Facebook有什么用？

按照Facebook的说法，用户喜欢分享照片和视频，于是这家公司一直在开发新技术，来理解和运用这些内容。

直到F8大会上，小扎身后的大屏幕上出现了这句话：
![](https://pic2.zhimg.com/v2-cba4fa845fd9349a6606dae3b7cfce81_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1334' height='889'></svg>)
> We’re making the camera the first augmented reality platform.

答案清晰了起来。小扎和Facebook一众高管纷纷表示，要将Facebook应用中的相机功能变为现实增强平台。很明显，人工智能是其中关键。

Schroepfer在会上说，Facebook从2012年开始研究神经网络技术，当时他们搭建了alexanet，这个基本的神经网络虽然效果不是很惊艳，但能从照片中识别出人、树等等物体。

而现在，他们的技术现在能以很高的准确率识别照片中的人物、宠物和产品，或是从一段视频中抹去一艘行驶的帆船。

# ![](https://pic2.zhimg.com/v2-a03185f25b1e50e0390062fad0dd2b9d_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='332'></svg>)AI与Facebook的应用们

Facebook的AI技术，当然不只用于AR。

Oculus VR也与人工智能技术关系密切。Facebook和Oculus的研究团队，试图共同解决SLAM技术上的挑战。这项技术能同时识别真实世界中某人的周围环境，以及虚拟现实中的世界和某人在其中的位置。Schroepfer自豪地宣布：“这些系统的作者全部供职于Facebook和Oculus。”

Facebook的其他资产，例如Instagram和Messenger，更依赖于Facebook统一的人工智能研究成果，而这些产品也试图将新技术转化为能覆盖大量用户的体验。

这些移动服务中所用的AI技术，最初都依赖云计算平台，这类计算密集型任务需要用到众多服务器。

而近期，智能手机已可以直接实现某些先进的人工智能功能。这一方面是因为手机硬件逐渐变得更强大，而另一方面也是因为，随着计算科学的发展，研究人员可以尽可能高效地利用计算资源。Schroepfer表示：“这并不仅仅是因为CPU在变得更快。我们也不断提出更优秀的算法。”

这个“更优秀的算法”，就是支持在移动设备上部署深度学习模型Caffe2框架，也在F8大会上正式发布。在量子位公众号（QbitAI）对话界面回复“Caffe2”查看详情。

# 从手机到更多平台

![](https://pic3.zhimg.com/v2-8f0d4fc376d346a3591dc5879013a58e_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='386'></svg>)你可能已经注意到，Facebook增强现实平台的核心是目前已经非常普及的设备：智能手机。但这并不是说Facebook未来不打算像微软推出HoloLens那样，把这项技术用到更多设备中去。

Schroepfer表示，当前的主流硬件已经能实现订制化的现实增强，而这也将帮全球用户为现实增强做好准备。

他说，在10年前iPhone面市时，尽管与以往的设备都有所不同，但看起来却令人感觉眼熟。iPhone同样可以打电话、收发电子邮件，以及浏览网页。

如果Facebook的现实增强平台能通过将数字画面与现实世界画面叠加在一起，推动有用的功能普及，那么未来用户就不会对支持现实增强的眼镜、隐形眼镜或其他设备感到太陌生，从而有助于这些新设备的普及。他表示：“当你开发一类新设备时，用户这样的固有观念将带来困难。”

Schroepfer并未提到此前遭遇失败的谷歌眼镜。在谷歌眼镜推出时，用户对现实增强还没有太深刻的概念。实际上，Schroepfer没有必要关心谷歌眼镜，因为Facebook的做法与谷歌眼镜有明显不同。

只有当用户察觉不到电子设备的存在时，现实增强才能带来“魔法”。那么，这还需要多久？Schroepfer表示，目前我们正处于“研发”活动的“研究”阶段。除此以外，他并未对未来的时间表做出预测。他表示：“在人工智能和现实增强两大领域，许多聪明人的看法都不相同。”

# 相机公司

![](https://pic2.zhimg.com/v2-1f35fa7068b51d23a072035a8e9291f9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='480'></svg>)![](https://pic3.zhimg.com/v2-16645f92a2b5c57a7c1e31f5a393f46a_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1114' height='663'></svg>)最后，量子位还是要说一说“相机”。

Facebook在谈AR的时候，不止一次地提到“相机”，如果你还记得Snap，大概也会想起它在上市时自称是一家“相机公司”。

实际上，作为一家以可视化内容为中心的公司，Snap对Facebook的相机/现实增强战略产生了重要影响。Camera Effects也处处透着Snapchat的影子。

推荐阅读量子位以前写Snap的文章（最后有彩蛋哟~）：

《[美国近年最大科技IPO：90后老板+超模老板娘，人工智能加持](https://link.zhihu.com/?target=http%3A//m.toutiao.com/i6382806225809064450/%3Fgroup_id%3D6382802563545121025%26group_flags%3D0)》

---------------------------------------------------------

**招聘**

我们正在招募编辑记者、运营等岗位，工作地点在北京中关村，期待你的到来，一起体验人工智能的风起云涌。

相关细节，请在公众号对话界面，回复：“招聘”两个字。

**One More Thing…**

今天AI界还有哪些事值得关注？在量子位公众号会话界面回复“今天”，看我们全网搜罗的AI行业和研究动态。笔芯~

另外，欢迎加量子位小助手的微信：qbitbot，如果你研究或者从事AI领域，小助手会把你带入量子位的交流群里。


