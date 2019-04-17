# 苹果大秀AI肌肉：机器学习框架、更好的Siri和智能音箱 - 知乎
# 



李林 若朴 假装发自 McEnery

量子位 报道 | 公众号 QbitAI

> 
“I love machine learning, especially since I’m a machine, learning. ”

—— Siri


与Google相比，今天凌晨的苹果全球开发者大会（WWDC）节奏超快。

苹果一口气连续发布了多款软硬件新产品、新服务。比方各种操作系统，包括iOS 11、macOS High Sierra、watchOS 4；新款iPad Pro；全新的iMac Pro；以及多年以来的又一款新产品：HomePod。

整场WWDC有一个因素贯穿始终，那就是机器学习。长期以来苹果都被认为在人工智能方面进展缓慢，但苹果显然不甘落后。

到底苹果秀了哪些AI肌肉？咱们一个一个说。

# CoreML

我们来念一遍WWDC的全称：全球开发者大会(Worldwide Developers Conference)。

![](https://pic2.zhimg.com/v2-b3e4a95e8a17d06e3185aaa460e7f991_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1280' height='566'></svg>)在iOS 11更好的Siri、手机上的AR背后，自然是面向开发者的新工具。Core ML就是苹果新推出的，面向开发者的机器学习框架。

苹果说，Core ML让开发者更容易把机器学习用到App里，文本分析、人脸识别等等功能，都可以依靠这个工具。苹果说，用了Core ML，iPhone上的人脸识别比Google Pixel上快6倍。


Core ML支持所有主要的神经网络：深度神经网络(DNN)、递归神经网络(RNN)、卷积神经网络(CNN)，除此之外，它还支持SVM(支持向量机)、以及线性模型和树集成。

![](https://pic2.zhimg.com/v2-a9e4c8df84da00020fddbc0473ce66cd_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='847' height='236'></svg>)在Core ML的支持下，开发者可以把训练完成的机器学习模型，封装进App之中。

苹果强调用户隐私的一贯风格，在Core ML中也有承袭。Core ML让所有的机器学习计算都在iOS设备本地进行。

![](https://pic3.zhimg.com/v2-a9b05d078db785bd889d149acfa2bb22_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='676' height='336'></svg>)除了能支持超过30层的深度学习模型之外，Core ML还支持Tree ensembles，SVM和广义线性模型。

Core ML还能无缝切换于CPU和GPU之间，以提供最强的性能和效率。目前Core ML提供视觉和自然语言两个API。

关于Core ML的更多细节，请访问：
[Core ML | Apple Developer Documentation](https://link.zhihu.com/?target=https%3A//developer.apple.com/documentation/coreml)
想要获取Core ML开发工具，可以访问如下网址：
[coremltools 0.3.0](https://link.zhihu.com/?target=https%3A//pypi.python.org/pypi/coremltools)
顺便说一句，苹果还为开发者推出了AR开发工具包ARKit。

# Siri

![](https://pic4.zhimg.com/v2-cbadf6af581bfcf88e445a4c213eb41f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='930' height='355'></svg>)在iOS 11中，Siri得到了一次大升级。

早在上个月，就有传闻称苹果要让Siri支持更多的App，从只能假装陪你聊天、用用iOS的原生功能，到可以操作你手机里安装的App。

![](https://pic3.zhimg.com/v2-7d80ac23b2e95708db93963d3f60a726_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='2681' height='963'></svg>)传闻实现了，苹果在WWDC上宣布，Siri支持一系列第三方应用，包括微信，这个功能，叫做Siri Kit。

不过，苹果怎么会被传闻完全说中。

除了支持第三方应用，Siri还有更厉害的招数：更自然的语音、实时翻译、更精准的预测。

苹果负责技术的副总裁Craig Federighi说，“我们用深度学习为Siri打造了更自然、更富有表达力的声音。”

紧接着，他播放了英文版Siri的声音，男声版就讲了本文开头的那个冷笑话。不知道母语者感觉如何，量子位的母语不是英语，听起来，的确感觉与真人差异不大。不过，在后来展示实时翻译的时候，他又让Siri说了一下中文，听起来虽然比现在有很大提升，但终究还是像个机器。

![](https://pic2.zhimg.com/v2-a92335f1d0d8e3fd812554d2261e962d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1280' height='402'></svg>)实时翻译，就是在你用英语问Siri“How do you say … in Chinese?”，Siri就会立刻把你的话翻译成汉字显示在屏幕上，还要用中文给你念一遍。

这个实时翻译功能首先支持英译汉、西、意、法、德。

另外，Siri借助机器学习，还能更了解用户的兴趣，可以据此推荐新闻、帮用户回复消息，或者根据用户在浏览器中完成的订票行为，要求帮用户在日历上添加事件。

# HomePod

对，HomePod就是传说中的苹果智能音箱。售价349美元。

美国、英国、澳大利亚三个国家首发，不过正式发布也要等到今年12月。其他国家和地区要等到明年发售。

这个新产品的发布，不仅仅是苹果全家桶又增了一项，而是再次确认了新趋势的不可阻挡。随着Apple Siri的发布，美国市值最高的三家科技巨头：苹果、Google、亚马逊，全部加入到智能音箱市场的争夺之中。

就在苹果发布会开始前几天，Google宣布Google Home智能音箱将于月底在加拿大发售，并随即启动预售，而且官方还降价促销。不过，最近几天Home也出现了问题，不少用户反馈这个智能音箱失去响应。

**基本情况**

![](https://pic3.zhimg.com/v2-014210812758a420ff35c2fad1dd386e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='247'></svg>)△ 量子位注：目前Google Home官网打折，从6月4日开始，售价下调至109美元。

HomePod高度约7英寸，底部设有7个环绕喇叭，内置低音炮，整个产品由A8芯片控制，支持实时声音建模。通过芯片，HomePod可以完成声场设置、降噪和语音识别工作，而且还能对空间进行识别调整。

**![](https://pic1.zhimg.com/v2-09fcbe03d22019a5e3403461358477a4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='263'></svg>)音质**

毕竟是音箱，怎么也得谈谈音质。

在今天的发布会上，苹果大谈HomePod音质如何如何好，不过目前还无法确切得知。因为今天的HomePod只能看，不能摸，而且现场的展品也没有发出声音。

另外两家，Home能提供更丰富的环绕音质，Echo在低音方面有缺陷，但也有人觉得更自然。最近Home升级之后，也能支持蓝牙播放了，在这点上终于追上Echo。这两个设备都支持Pandora、Spotify。

当然区别还是有一些，Echo支持Amazon Music；而Home支持Google Play Musci、YouTube Red；HomePod显然支持Apple Music。

**语音控制**

智能音箱主要使用语音交互来控制音乐播放、搜索、智能家居等。Echo的唤醒指令比较丰富：Hey Alexa、Alexa、Amazon、Echo、computer都行。而Home的唤醒指令是：OK, Google以及Hey, Google。HomePod肯定要用：Hey Siri。

这些产品都使用了远场语音识别技术，Echo和Home能在15米外识别用户的语音指令。HomePod据说距离更远。

在硬件方面，Amazon Echo使用6+1配置的环形麦克风阵列，而Google Home使用的是双麦克风方案。Echo能更好的对声源方向进行定位。而苹果发布的HomePod使用了6个麦克风进行声音采集。

**![](https://pic4.zhimg.com/v2-b755df030a293c7aa83dfafef6099f8f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='900' height='726'></svg>)虚拟助手**

每个智能音箱内部，都住着一个人工智能程序。Echo住的是Alexa，Google Home住的是Google Assistant。HomePod住着Siri。

在使用体验方面，Google Assistant能够从网上获取更多的信息，因而也能更好的回答用户提出的问题，而Alexa更擅长处理购物相关的请求。而关于Siri，目前可以在各位的苹果手机上试验一下。

Google Assistant在语音交互方面更进一步，能够根据前一个问题，继续回答用户的相关追问。毫无疑问，在问答方面Google优势明显。

![](https://pic1.zhimg.com/v2-bdd761abbd3b5dce72e0e2559dfe60b8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='630' height='470'></svg>)苹果发布的信息显示，HomePod目前可以播放音乐、新闻、天气及股票等信息，设置提醒，通报交通和附近的状况，并与HomeKit集成用于控制智能家居设备。

当然，以上所有智能音箱的语音交互都得通过英语完成。

**功能扩展**

Home目前有200个第三方技能，可以帮助用户从达美乐订购披萨，也能从Uber叫车。但是Echo已经掌握了超过11000个新技能，而且能向Google Calendar添加事件、从Gmail里提取行程细节，这些反而Home都做不到。

Echo可以帮你免费打电话或发短信给任何一个使用Echo设备或Alexa App的用户。而Google Home现在可以给手机打电话了。

Google的产品支持多用户和多房间功能，Home可以辨认最多六位用户的声音，并且能无缝切换不同用户的账号和设置。

而Amazon的用户得跟Echo说“切换账户到某某”才行。Home能在不同的房间同步音频，而Echo现在还不行。

目前HomePod这方面的表现还不得而知。但可以想见，未来正式发售时，Siri一定会有更大幅度的更新，支持这个全新的硬件。

# 竞争解析

智能音箱这个市场，亚马逊是绝对的先行者。

2014年11月6日，亚马逊面向会员和受邀用户推出了Echo。这款产品于2015年6月23日在美国正式发售。目前，亚马逊共有三款不同型号的Echo系列产品在售。价格从50美元-230美元不等，瞄准不同用户群。

Google扮演了追赶者的角色。2016年5月，Google在I/O大会上正式发布Home。2017年5月的I/O大会上，Home发布了一些新功能。不过目前Google Home仍然只有一款型号在售，但可选择的配色方案更丰富。

![](https://pic1.zhimg.com/v2-51b4724a66beda04f7b94c20bab654b0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='540' height='621'></svg>)来自eMarketer的报告显示，智能音箱市场Amazon Echo一家独大，占据70%的市场份额，Google Home的份额为23%。其余所有人分享5%。现在苹果也杀进来了，不知道这个格局未来会怎样发展。

从这个格局来看，Google对苹果的入局反应最大也是情理之中。

实际上，当年Amazon Echo推出的时候并不受到重视，然而时至今日，这款产品累积销售额据称已达10亿美元，还有市场分析机构预测，到2020年Amazon的智能音箱销售额将达100亿美元，产品销量过亿。

调查也显示，美国市场各年龄段的用户，对于语音控制设备的使用量，也呈现出了明显的增加。这也是人工智能等相关技术发展的一个结果。

智能音箱看起来好像“并不复杂”，但实际上却需要语音识别、语义分析、虚拟助手等诸多领域的人工智能技术在背后支撑。

# 其他要点

**iOS 11**

通过iCloud可将iMessage里的对话内容进行云端同步；iMessage还加入了Apple Pay，可直接在对话中进行转账。

Siri将支持微信等更多应用，并通过深度学习可以理解用户言语，结合上下文进一步知晓兴趣爱好等，甚至可以预知用户即将开启的App。

新版Camera将会支持HEVC编码格式，每张照片的压缩率将为此前的两倍。iOS 11还可以从Live视频中提取静态图片。

iOS 11还重新设计了控制中心和通知中心。iOS 11还针对中国市场加入了二维码扫描、诈骗短信识别、拼音键盘以及上海话语音识别等功能。

除此以外，macOS High Sierra提供了性能更好的Safari浏览器，新的Mail更好的支持搜索并减少存储空间。

**硬件**

新款iPad Pro发布。支持10小时的续航时间，12核苹果A10处理器，图像性能提升40%，配备1200万像素摄像头，支持光学防抖，支持4K视频的录制。新款iPad Pro有10.5和12.9英寸两种尺寸，售价649和799美元起。

全新iMac Pro发布，全线产品均配备27英寸5K屏幕、128GB ECC内存、4TB SSD硬盘，并搭配全新的工作站级显卡Radeon Vega。iMac Pro会配备8核、10核、18核的英特尔至强处理器，价格4999美元起。

【完】

One More Thing…

今天AI界还有哪些事值得关注？在量子位（QbitAI）公众号对话界面回复“今天”，看我们全网搜罗的AI行业和研究动态。笔芯~


