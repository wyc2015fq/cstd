# AI技术宅：女神说什么，听我的！ - 知乎
# 



> 千平 发自 凹非寺
量子位 出品 | 公众号 QbitAI

“清晨，我推开房门，一个洁白的世界映入我眼帘……”

电影《爱情麻辣烫》里有段情节：高圆圆朗读课文的声音，还有日常对话的声音，被男生想方设法录了下来，最后剪辑成一句：“真的喜欢呀”。

这是20年前的老片子了。呐，就是这段：
![](https://pic4.zhimg.com/80/v2-5d67a708a06382e9dc24d827bfa8dcef_b.jpg)《爱情麻辣烫》高圆圆扮演荷玲片段_腾讯视频https://v.qq.com/x/cover/m0183cug7ps/m0183cug7ps.html
*还可以去B站看弹幕版：[http://www.bilibili.com/video/av1867834/](https://link.zhihu.com/?target=http%3A//www.bilibili.com/video/av1867834/)*

现在，想让女神开口说一句甜言蜜语，已经不需要这么原始的方式了。

万年小学生柯南，有一个神奇的领结变声器，能够把柯南的声音，转换成任何人的声音。当然量子位今天不是要讲柯南，而是想说在人工智能的加持下，这个技术正在从科幻的动画片中，走入现实。

两个韩国AI研究员，最近就启动了一个研究项目。他们搭建的AI系统，能让“女神”模仿你说的话。也就是说，你想听什么，告诉“女神”，她就听你的话复述出来~

或可称为“技术宅”。

不信？看看下面这段Demo。男声说一句话，就能让女星凯特·温斯莱特（Kate Winslet）跟着“说”一句同样的话。
![](https://pic2.zhimg.com/80/v2-95b4b22a9b6e7ee54760b3234b106ca9_b.jpg)变声效果演示_腾讯视频https://v.qq.com/x/cover/j0568jxmihk/j0568jxmihk.html
插播：温斯莱特，就是当年《泰坦尼克号》的女主角~
![](https://pic4.zhimg.com/v2-1e504b7979b769fd2f13237c9206bfe7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='475'></svg>)
## 怎么做到的？

其实这背后的技术，名叫：非并行数据（Non-Parallel Data）语音转换。

两位作者是来自韩国Kakao Brain的AI研究员Dabi Ahn和Kyubyong Park。

交待一句，Kakao是韩国最大的移动社交公司，旗下产品KakaoTalk被称为韩国版微信。当然KakaoTalk比微信早上线约一年。现在腾讯也是Kakao的第二大股东。

当初之所以要搞语音风格迁移，他们的目标是想将某人的声音，转换成特定目对象的声音。也就是随便一个什么人，也能模仿出名人或者歌手的声音。
![](https://pic1.zhimg.com/v2-78c4c5e718486725664cf0a019e90d18_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='379'></svg>)
开头我们也看到了，他们开始选的模仿对象，是女演员温斯莱特。

为了实现这个目标，作者搭建了一个深度神经网络，并使用了温斯莱特两个多小时的音频素材作为数据集进行训练。

模型架构

这是一个“多对一”的语音转换系统。这项研究的主要意义在于，可以生成用目标对象声音说出的话语，而不需要平行数据的支持，只需要目标对象的声音波形。
![](https://pic4.zhimg.com/v2-1ced260b62d87e2faaffbc22f5573bfb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='251'></svg>)
从架构上来说，这个模型由两个模块组成：

**Net1**

这是一个音素分类器。
- 过程：声波->谱图->mfccs->音素区。
- Net1会将每个时间步长上的声谱图分类为音素，以对数幅度的声谱图为输入，以对应的音素为输出。
- Net1的目标函数是交叉熵损失。
- 所用的数据集是TIMIT，。
- 测试精度超过70%。音素与讲话者无关，而波形与讲话者有关。

**Net2**

这是一个语音合成器，其中包含一个Net1作为子网。

过程：Net1->谱图->声波。
- 这里使用了Tacotron中提到的CBHG模块，即：1-D卷积组+高速网络+双向GRU。CBHG对于顺序数据的特征捕捉很有用。

Tacotron的论文：

[https://arxiv.org/abs/1703.10135](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1703.10135)
- 损失是输入和输出之间的重建误差。
- 从谱图恢复时使用了Griffin-Lim重建。
- 这里用了两个数据集：
- Target1（匿名女性）：Arctic数据集。地址在此：//[http://www.festvox.org/cmu_arctic/](https://link.zhihu.com/?target=http%3A//www.festvox.org/cmu_arctic/)
- Target2（温斯莱特）：这是一个非公开数据集，包含超过两个小时温斯莱特的读书音频。

实现

**设置**
- sample rate：16,000Hz
- window length：25ms
- hop length：5ms

**程序**

Net1和Net2应该按顺序进行训练。

训练的细节，可以参考两位作者已经在GitHub上公开的代码。地址在：

[https://github.com/andabi/deep-voice-conversion](https://link.zhihu.com/?target=https%3A//github.com/andabi/deep-voice-conversion)
![](https://pic1.zhimg.com/v2-653ab0783054dbc32364cc57c32050c0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='281'></svg>)
关于这个项目未来的发展，包括几个目标：
- 使用对抗性训练
- 产生更清晰和纯净的声音
- 跨语言工作
- 多对多语音转换系统

两位作者还在进行一个有趣的尝试，他们准备弃用大型的目标声音数据集，转而使用小型数据集，完成训练。换句话说，只要让AI听一分钟目标对象的声音，就能实现很棒的语音转换！

听起来还有点小激动，感觉离柯南的领结更近了~

## OMT

柯南的领结变声器，网上已经有“玩具版”在出售了，按捺不住的同学，可以趁“双11”自行采购。

严正声明，这不是广告~
![](https://pic4.zhimg.com/v2-1b4d6ab640d7f22bcea9cc0915ad6193_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='414'></svg>)
— 完 —

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

量子位 QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态


