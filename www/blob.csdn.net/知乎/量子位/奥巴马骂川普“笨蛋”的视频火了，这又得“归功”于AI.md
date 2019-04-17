# 奥巴马骂川普“笨蛋”的视频火了，这又得“归功”于AI - 知乎
# 



> 夏乙 栗子 发自 凹非寺
量子位 出品 | 公众号 QbitAI

一段吐槽视频今天在美国火了。
![](https://pic3.zhimg.com/v2-4916de3250e55d23c481a7c90e2e5dab.png)https://www.zhihu.com/video/970013969726025728
也就半天的工夫，这段视频在Twitter上有200多万次播放，1300多条评论，被转发了2万6000多次，还收获了5万多个赞。

什么视频这么万人瞩目？主角，是前总统奥巴马；这么受关注，当然是因为——内容非常劲爆，特别是这一句：

> President Trump is total and complete dipshit.

“川普总统完全就是个笨蛋。”

不过，这些劲爆的话都不是奥巴马自己说的，视频里的声音来自刚刚在今年的奥斯卡上以《Get Out》拿下最佳原创剧本奖的导演Jordan Peele。

他和BuzzFeed CEO Jonah Peretti一起，自导自演了这么一出大戏，戏里的奥巴马，对口型能力强大到让所有假唱精英败下阵来。
![](https://pic1.zhimg.com/v2-35c12c1e1debc8ee7ac087f3145e5b04_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1200' height='429'></svg>)
搬出奥巴马，就是为了向全美国（甚至全球）人民传达视频开头的那句话：

“我们已经进入了这样一个时代，我们的敌人可以做出看起来像任何人在任何时候说任何话的东西。”

未来的假新闻可能就是这个样。

这个视频……鹅厂不让上传，有兴趣的同学可以自行搭梯子去Twitter看：

[https://twitter.com/BuzzFeed/status/986257991799222272](https://link.zhihu.com/?target=https%3A//twitter.com/BuzzFeed/status/986257991799222272)

视频中，Peele还放出了奥巴马和自己的对比环节。
![](https://pic1.zhimg.com/v2-797e2ce34c7ded5a4594cd2a57d527e0_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)
右边就是Jordan Peele，这段视频的（部分）替身演员和声优。视频制作的主力工具则是Adobe AE和FakeApp。

此次的脸部搬家工作，具体是这样的。首先，找出一段奥巴马真正的演讲视频，将Peele的嘴粗暴地粘贴到奥巴马脸上。然后，再把奥巴马的下巴，换成一个可以随着演员嘴部动作一同运动的下巴。

下一步，FakeApp登场，负责把拼贴好的视频做一些平滑和细化处理。听起来像是收尾工作，但这一步任重道远。

据BuzzFeed透露，一开始搬家的效果蠢蠢的。可能用整容失败都不足以形容。但，训练时间长了之后，Peele的嘴部动作和奥巴马的脸融合得越来越自然。

最后发出来的视频，是56小时辛苦训练、以及特效专家现场指导的结晶。

## **又见FakeApp**

是的，又见FakeApp。

去年底，一位不愿透露姓名的用户deepfakes，利用业余时间搞出了这个机器学习算法，主要的功效就是两个字：换脸。

只要给这个AI一些照片，就能出色的给视频中的主角换脸。

后来他的网名，就成了这个技术的代名词。作者本人透露，这个系统是基于像TensorFlow后端的Keras等多个开源库完成的。

这个开源系统发布之后，立刻引发了轰动。国外网友在reddit上创建了专门的讨论区，用来交流和发布研究成果。

简单实用效果好，deepfakes快速流行开来。很多人开始用这个技术，把色情片主角的脸换成自己喜欢的明星。

再后来，deepfakes进化成FaceApp。

从此换脸的流程进一步简化。FaceApp被定义为一个社区开发的桌面应用，可以运行deepfakes算法，无需安装Python、TensorFlow等，并且如果想要运行，仅需要“CUDA支持的高性能GPU”。
![](https://pic3.zhimg.com/v2-6d1a1d73661e190f74db39f179898536_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='897' height='544'></svg>)换脸的训练过程
炸裂的效果引发了巨大的争议，后来reddit上这个版块也不得不关闭了事。

虽然互联网公司几乎集体封禁了deepfake相关社群，不想让网民们再到处传播换上赫敏脸的小片片。但是，这个技术的超低门槛软件FakeApp悄然迭代着，目前已经到了2.2版。

这里，我们放一段之前的演示：将希特勒的脸“嫁接”到阿根廷总统Mauricio Macri身上，体会一下效果。

关于deepfakes的应用，量子位还在另一篇报道里有过详细的描述，传送门在此：[《不可描述，技术进步》](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247493660%26idx%3D1%26sn%3Dadfebdd9ab976ee19b17f29ff383dac4%26scene%3D21%23wechat_redirect)。

## **无需配音**

在FakeApp的帮助下，换脸这件事变得非常简单。

那声音怎么办？严丝合缝的声优在哪里？

还是让AI来吧。

上周，有Reddit小伙伴发布了，川普演讲的韩文版，是AI自动生成的。

楼下，就有韩国友人鉴定，嗯，流利得可以。
[http://v.qq.com/x/page/n0632t783mk.html​v.qq.com](https://link.zhihu.com/?target=http%3A//v.qq.com/x/page/n0632t783mk.html)
这还不算，会说韩文的群众纷纷表示，美国人说韩文，基本就是这个口音。

本周，还是那个小伙伴，又放出了带有金同志嗓音的朝式英文，AI调教成果上佳。
[我们的邻居说英语，可能是这样的_腾讯视频​v.qq.com](https://link.zhihu.com/?target=http%3A//v.qq.com/x/page/d0632jd4ocb.html)
视频那一头的神秘团队Icepick，到底对他们的AI做了什么，目前并没有相关信息公开。
![](https://pic4.zhimg.com/v2-cb27d406ac85f8b7c5f6f4f66ea89c5f_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='342'></svg>)耳朵会怀孕
不过，我们依然可以看看，AI获得语音生成技能之前，到底需要经受怎样的调教——

## **气质如何调教**
![](https://pic3.zhimg.com/80/v2-f6a831a334cf24f3513003256dc2e976_b.jpg)https://www.zhihu.com/video/969958810895482880
不管是谷歌娘，Siri，或是Alexa，说的话听上去都不太像真人。这是因为，我们说话时有一些平常不容易察觉的细节，比如字与字之间的连接、呼吸声、气音、嘴唇碰撞时发出的声音等等。

所以，把多个语音片段直接拼接合成 (Concatenative Synthesis) 一段话 (即鬼畜本畜)，或者用参数合成 (Statistical Parametric Synthesis) ，都很难产生非常接近人声的语音。

第一个用神经网络来生成人类自然语音的，就是DeepMind的WaveNet。
![](https://pic4.zhimg.com/v2-e41fd475212c6bf43009f81085eec0c3_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='570' height='289'></svg>)
从前，人们很少为原始音频的声波直接建模，因为每秒有超过16,000个采样点，让模型过于复杂，难于训练。

但当PixelRNN和PixelCNN发布之后，DeepMind便有了“借助二维图像的处理方式，来处理一维声波”的想法。
![](https://pic2.zhimg.com/v2-c30c38e855aa24273739940a3ff411a1_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='570' height='262'></svg>)
WaveNet是全卷积神经网络，卷积层里的扩张因子 (dilation factors) ，能让感受野 (receptive field) 随深度变化呈指数增长，并且覆盖数千个时间步 (timestep)。

有了延时采样机制，只要增加一层，就可以多关联一倍的时间范围，训练效果更佳。 
![](https://pic4.zhimg.com/v2-7bd1f580d50243e33221aa2388f30823_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='320' height='320'></svg>)你听得出，我不是人吗
通过层层卷积，WaveNet便可以把PS痕迹明显的机器语音，转换成更加流畅自然的语音，与人类声音之间的差异大幅降低。

## **嗓音如何调教**

去年，Lyrebird也发布了语音合成技术，基于音色、音调、音节、停顿等多种特征，来定义某个人的声音，然后借用ta的声音来说话。

据公司官方表示，通过大量的样本学习，神经网络只需要听一分钟的音频，就可以模仿里面的陌生人说话了。Lyrebird系统生成一段语音，比同一时期的WaveNet要快得多。

## **口音如何调教**

说一句话很容易，但语音就是一门复杂的学科了。同样一个“啊”字从不同的人嘴里发出来，也会因为口型大小、发音位置 (这并不是官方特征分类) 等习惯的不同，让身为听众的人类或计算机感受到差异。
![](https://pic1.zhimg.com/v2-7140dd189ff3642486c3ba8fdb44d93c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='453'></svg>)给你，销魂的伦敦腔
百度的语音合成系统DeepVoice，可以轻松训练AI合成百种英文口音。研发团队发现，AI把不同口音的演讲者，对应到嵌入空间的不同区域里。比如来自大不列颠岛和北美大陆的人，在嵌入空间里占据的区域也有明显的不同。



![](https://pic2.zhimg.com/v2-35e5bd499e4e87009d25d4ee0fac64a9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='308' height='73'></svg>)简单的原理：英男 + 均美 +均英 = 美男
如果是处理中文，什么样的声线和口音，才比较适合骗大神带你吃鸡呢？

## **随口一说**
![](https://pic4.zhimg.com/v2-8f7001aff13fd8a3f7b7b5f22092f06f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='675'></svg>)你个火·箭男
也有人不喜欢用政治噱头来包装科学研究的做法，并表达了强烈的反胃之情。

不过我倒觉得，如果有人发糖，还是要尽量分享给周围的人。下面是正确示范——川川当选之际，和希拉里的深you情du对唱 (误) ，无奈视频上传有难度，就改成了传送门。

[https://www.bilibili.com/video/av7234390/](https://link.zhihu.com/?target=https%3A//www.bilibili.com/video/av7234390/)

以后，AI说不定能帮我们，把天衣无缝的南腔北调，P给鬼畜区的任何一位主角。

那么，我心心念念的雷布斯同款仙桃普通话，感觉指日可待啊。
![](https://pic4.zhimg.com/v2-5bdb3b73b3622e91ff85a333b9f2b75b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='1020'></svg>)
## **福利**

致想看视频又没梯子的同学：文中提到的视频，我们在度娘网盘传了一份。

欢迎在量子位公众号（QbitAI）对话界面回复“**造假精英**”四个字提取。

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai) · 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


