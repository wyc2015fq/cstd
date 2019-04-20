# 盘点《头号玩家》里的 VR 技术，现在就能造个 Oasis 出来 - 人工智能学家 - CSDN博客
2018年04月02日 00:00:00[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：498
![640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/YjhmbbkdV6uJOx1CkS2DsmgicxicmGkQrPIXKE5lmU2LWmmeiciciacpW77wxhJmiawib24ickDRAEQyMSJtsHnFafgDtg/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1)
来源：沉浸感丨公众号   作者： 刘芳平
由史蒂文·斯皮尔伯格导演的科幻电影《头号玩家》（Ready One Player）于 3 月 30 日在中国大陆上映。首个周末便收获广泛好评，登上豆瓣电影本周口碑榜第一名，截止 31 日晚积累了 7 万多个评价，评分高达 9.2 分，不出意外它将成为一部科幻经典。
许多人可能觉得作为一部故事发生在 2045 年的科幻电影，里面的科技应该先进得不得了，肯定不是现在能比的。但其实并非如此，不用等到 2045 年，电影里展示的大部分技术，现在就有了。
下面就让笔者一步步来解读，实现影片中的效果要用到哪些产品和技术。
## **一、VR 头盔**
![640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_jpg/YjhmbbkdV6uJOx1CkS2DsmgicxicmGkQrPQ6WPicT0QxyuVqgcNWOqpt5gg2p0o86dsc5q3bic5XvHeXzNAIMO833w/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1)
影片中主角用的 VR 头盔
这个不用多说，多数人一看到电影海报就会想到 VR 头盔。VR 头盔的作用是给玩家提供一个沉浸式的全息画面，让玩家沉浸在虚拟世界里。
现在市面上有着相当多的 VR 头盔可供选择，但要实现电影里面的效果，HTC Vive、PlayStation VR、Oculus Rift 只是基础的，更沉浸式的产品，就得用到小派 8K 或是 StarVR 的 5K VR 头盔，它们都拥有高达 200 度的视场角。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/YjhmbbkdV6uJOx1CkS2DsmgicxicmGkQrPwkQ7Uw8wicPic4l6wLS1ztsRykKaSrRYP6Vic3pM6JicB5TiaNtibEmcvXvA/640?wx_fmt=jpeg)
Vive 无线升级套件，采用英特尔 WiGig 技术
另外，视频里角色们用的都是无线的头盔，而上述产品默认都是有线的。这个时候就轮到 TPCast 或是 Vive 无线 VR 套件上场了。无线套件一端连接 VR 头盔，一端连接主机，可以将主机渲染的画面传送到头盔里显示。
另外，电影里还有许多人在街上用 VR 头盔，这种情况下，适合使用带 inside-out 追踪的 VR 一体机，比如 Vive Focus，或是英特尔的 Project Alloy VR 一体机。
## **二、空间定位及动作捕捉**
光有 VR 头盔是不够的，电影里面玩家在现实世界的动作是实时在虚拟世界里同步的。实现这一点需要用到空间定位及动作捕捉技术。
从电影里面的效果来看，全身动捕是少不了的了，要做到这点现在也有一些技术方案可供选择。
或者，也可以选择提供影视动捕解决方案的诺亦腾，他们推出的 Project Alice VR 解决方案，使用光学空间定位系统，结合惯性动捕来进行追踪，只要给用户身上装上足够的追踪点，全身追踪是可以实现的。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/YjhmbbkdV6uJOx1CkS2DsmgicxicmGkQrP7epbWAxBx5KY5Yic7d9OB132s3aeiat4kd0dOqSJZN76icp7IRS9bysog/640?wx_fmt=jpeg)
另外值得一提的是，许多影视里的动画制作，就是通过真人戴上惯性动捕套装来实现的，上图为诺亦腾推出的一套方案。《玩家一号》里很有可能就用了这种技术。
## **三、VR 跑步机**
有了头盔和动作捕捉，就能实现用户在虚拟世界里的一个全身 Avatar 了，不过还有一个问题需要解决，就是电影里的玩家都是可以在虚拟世界里自由走动的，但他们在现实世界里却是待在一个地方，这要怎么实现呢？
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/YjhmbbkdV6uJOx1CkS2DsmgicxicmGkQrP6WbKBLQXibA7WzYTs2kxr1ygFfMiapGH3RvpldnibhPssKA8t0xc5LQmw/640?wx_fmt=jpeg)
男主戴着头盔，脚下是跑步机
电影里其实展示了解决方案，就是 VR 跑步机，影片一开头就有提到一个 VR 跑步机品牌——Virtual Omni。
上图中，男主脚下也有个跑步机，和 Omni 的产品不太一样，但实现的效果是一样的——玩家在现实世界的跑步机上原地踏步，在虚拟世界里则是前进的。PS. 上图男主的这款有个 Bug，它看起来是只能前后跑的，而他在虚拟世界里可以向各个方向移动。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/YjhmbbkdV6uJOx1CkS2DsmgicxicmGkQrPaSoKLfP9GiaZl7JSMKettqtmZWic1QnLW9vlG9ib34NnhJr2V88hYgvNQ/640?wx_fmt=jpeg)
影片里 IOI 公司的另一种 VR 跑步机
影片里还展示了另一种 VR 跑步机， 它和 Omni 及 KATVR 的产品类似，甚至后者还要更炫酷一些，见下图。这  类跑步机的实现方法是：在你的脚下放一口“锅”，鞋底再放个轮子，这样你不管怎么走，都会滑回“锅”里。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBUI8OlMkQ5g4WfQGOAMqGjPBaM7FqdLw2vWeOASHJ7fxquznnZh132iauPibibhiaZ89w4Ha8HaqekCXQ/640?wx_fmt=png)
KAT Space VR 跑步机
## **四、眼球及表情追踪**
国内的七鑫易维就是一家专门做眼球追踪技术的公司，他们推出的 aGlass 眼球追踪模组可以装在头盔里，追踪到的眼动可以转化为虚拟形象的眼动。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBUI8OlMkQ5g4WfQGOAMqGjP7te1uatibTFib7IodyibueAia1cpHW0FAzP5vicbyicq1PyToRh7fMnv07Hg/640?wx_fmt=png)
aGlass 眼球追踪模组
表情追踪方面，早前南加州大学的研究人员和 Oculus 合作，研究出了一种可以跟踪虚拟现实设备佩戴者脸部表情的方式，并将佩戴者的面部表情转换成一个虚拟人物。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/YjhmbbkdV6uJOx1CkS2DsmgicxicmGkQrPt6wu4z8Px1vKRWWMf8QEEZ2jIzhNE5ld42ofuVVSBVcib6Xricw6uibTw/640?wx_fmt=jpeg)
另一种大家更为熟悉的表情追踪技术是苹果 iPhone X 的 True-Depth 前置摄像头，它的一个应用是 Animoji。如果要应用到 VR 里的话，可以摄像头对准用户的脸部，不过，由于用户戴着头盔，因此还要结合头盔内的传感器生成的眼球追踪数据，一起生成用户的整个面部表情。
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/YjhmbbkdV6uJOx1CkS2DsmgicxicmGkQrPM0vrlbvUeV4tj3XSFCRrkDCrFNhPA3KtOgLdamS3JoiaWLBtibJs2ibVg/640?wx_fmt=gif)
Animoji
## **五、手势追踪、力反馈手套**
手势追踪是动作捕捉的延伸，因为玩家在游戏里的主要操作都是用手来完成的，所以有必要对手部进行更细的追踪，铺以相应的反馈让你不用真握着抢，也能感觉到自己在虚拟世界里握着的抢。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/YjhmbbkdV6uJOx1CkS2DsmgicxicmGkQrPPPpiciboB9rvVXJ4tH3oiaicSU7DDVZSgm5rrc2K8gqtibBoLicMCK932cqQ/640?wx_fmt=jpeg)
影片里的力反馈动捕手套
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/YjhmbbkdV6uJOx1CkS2DsmgicxicmGkQrPxY6eLBibGvbvZ8vVMiaMLdpdKLUbY3NHGaDHJmUS3cbBPjlcDgYrufjQ/640?wx_fmt=jpeg)
诺亦腾推出的 Hi5 VR 手套
而如果你想要更厉害的产品，Dexta Robotics 推出的 Dexmo 在动作追踪的同时，可以提供更好的力反馈效果。
它通过把用户的手部模型 Map 到虚拟人物的手上，进行物体交互的碰撞检测和物理状态判断。计算出实际上应该生成的力的矢量方向和大小后，再把这些数据发送回 Dexmo，通过母版分别传递到 5 个力反馈模块上，进行力的模拟。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/YjhmbbkdV6uJOx1CkS2DsmgicxicmGkQrPqm5rcpHFM9Wc6RtGvuwRH0wia6jrMOuyMsajnA8IAujj7Cic1PSboBng/640?wx_fmt=jpeg)
这里其实也可以看出影片里的 Bug，如果没有什么机械装置的控制或是实体物品的反馈，当玩家伸手去拿虚拟物品时，手是会穿过影像的，因为你实际上抓的是空气。相比之下，Dexmo 做的可谓更进一步。
## **六、力反馈背心**
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/YjhmbbkdV6uJOx1CkS2DsmgicxicmGkQrPAGstMiakN1pjyJJicc56iaNgBUtAzKOPGqPo0rYxpve8LCAeraQb3wYMg/640?wx_fmt=jpeg)
在虚拟世界里被打了，现实的身体如何感受到？答案是，要么真的有人打你一下，要么就得有一个装置来模拟。
这在影片里用的是力反馈背心，玩家们都穿着它，好体验在游戏里被打的感觉。
此类 VR 产品现在也有，比如数娱科技推出的这款 INAVR 力反馈背心，见下图。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/YjhmbbkdV6uJOx1CkS2DsmgicxicmGkQrPzs8Kn2ggTxPOZfiahtxNWOfE7MlvEvLOeVfn5Z9xut3CtBZmteQm3icg/640?wx_fmt=jpeg)
## **七、蛋椅**
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/YjhmbbkdV6uJOx1CkS2DsmgicxicmGkQrPFtCiaIlvYfMoRREv5AEcxehMooWgozHdre2KYF3vZiaHXptw6L1pzZXA/640?wx_fmt=jpeg)
上图为影片中反派大 BOSS 的专属宝座，笔者刚看到时就觉得：咦，这不就是前些年大火的 VR 蛋椅么。而且是个高级版的。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/YjhmbbkdV6uJOx1CkS2DsmgicxicmGkQrPZyn5ceo0KlvtsaO3oIHjFw0mu3VJ9s5aWISQ2GzudrztQRxutb1LdA/640?wx_fmt=jpeg)
VR 蛋椅可以配合游戏里的画面模拟出一些运动，让坐在上面玩 VR 的人感受更真实。不过蛋椅只是低阶的版本，还有这种更厉害的全感官体验舱，不仅可以模拟运动，还能模拟气味、触觉等，下图为日本游戏公司 Koei Tecmo 推出的 VR Sense 体验舱。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBUI8OlMkQ5g4WfQGOAMqGjPACCzFg0Tw4CsDL7IgIBMBIdv0RHq1H26eB6KN7lwx07hNJJGqyzphg/640?wx_fmt=png)
## **最后**
有了以上这些装备，还得开发出一个画面足以以假乱真的 3D 虚拟世界，做几个彩蛋进去让用户找。再加上随时随地地超高速、低延迟的互联网接入，就可以开玩了。
但是话又说回来，除了上面这些，你还得有个铁胃才行！
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)
