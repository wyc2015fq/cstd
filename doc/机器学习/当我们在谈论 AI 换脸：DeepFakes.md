# 当我们在谈论 AI 换脸：DeepFakes

[![余文毅](https://pic3.zhimg.com/a3e2422ec_xs.jpg)](https://www.zhihu.com/people/wyyualan)

[余文毅](https://www.zhihu.com/people/wyyualan)

不自由职业者

988 人赞同了该文章

## **引言**

站在2019年回看，“视频换脸”事实上并没不是什么大噱头：早在2016年，大家就见过利用 DCGAN 来“装修”房屋，随后各种 GAN 如雨后春笋：换脸的、换物种的，造人头等等脑洞大开的玩法层出不穷，时不时就刷一下存在感。但是 GAN 难以训练、效果不稳定、参数难调等等问题一直没有被完全克服，导致其在科研界虽然饱受追捧，实际工业应用的例子却不多。而在2017年末，却有款名为DeepFakes的程序以其优秀的换脸效果、及大幅降低的训练成本，蹿红整个互联网，并引发了长达数年的关于伦理的大讨论。

所以，这里我们首先抱着吃瓜的心态回顾一下这起黑天鹅事件。

![img](https://pic2.zhimg.com/80/v2-9dd7f13d5deed3a72f0e3e4d8b4ccf71_hd.jpg)GAN “装修”房间

## **DeepFakes 事件回顾**

事件起源于2017年12月，一个网名为 deepfakes 的程序员将一个“名人AV”上传到 Reddit：该AV视频女主角竟是神奇女侠主演Gal Gadot。当然这并不是 Gal Gadot 下海，而是它利用 AI 技术将 Gal Gadot 的脸替换到某个成人视频制作而成，不过该视频真实程度令人惊讶。

![img](https://pic2.zhimg.com/v2-5de2a96932c3967bf20433964112202d_b.jpg)

神奇女侠扮演者 Gal Gadot 的虚假AV

![img](https://pic3.zhimg.com/80/v2-c0cec8646066d627dd499a7aa3005982_hd.jpg)神奇女侠扮演者 Gal Gadot

接下来，随着一些媒体对 deepfakes 制作换脸视频的报道，越来越多人涌入 Reddit 来观摩，而 deepfakes 也乐此不疲地继续制作新视频供大家娱乐。其中一个名为 deepfakeapp 的用户甚至制作了一个视频换脸应用 FakeApp，帮助不会编程的同学制作自己的换脸视频，于是 Reddit 竟涌现出越来越多高质量作品。

![img](https://pic4.zhimg.com/v2-76e7994e919b67865c42ef6c96a3946b_b.jpg)

神奇女侠扮演者 Gal Gadot 的虚假AV 2

![img](https://pic1.zhimg.com/v2-87a797bf9d57649e9869748eb8471070_b.jpg)

漫威黑寡妇扮演者Scarlett Johansson 的虚假AV



漫威黑寡妇扮演者 Scarlett Johansson

![img](https://pic3.zhimg.com/v2-ad2f5e453b9fa6d4b98ba37a3fdc716a_b.jpg)

哈利波特赫敏扮演者 Emma Watson 的虚假AV



哈利波特赫敏扮演者 Emma Watson

随着视频制作门槛降低，毫无意外的，制作和观摩“名人AV”的人在2018年1月开始爆发式增长，并引起了主流媒体和 Reddit 的注意。



deepfakes 关键词的 Google Trends

这种未经当事人同意，将其肖像用于成人视频的做法是严重违背伦理的，大量的指责声随之而来。终于，[Reddit 在2018年2月7日对 deepfakes、FakeApp 等几个相关板块禁止访问](http://link.zhihu.com/?target=https%3A//www.theverge.com/2018/2/7/16982046/reddit-deepfakes-ai-celebrity-face-swap-porn-community-ban)，如 [deepfakes板块](http://link.zhihu.com/?target=https%3A//www.reddit.com/r/deepfakes)。在巨大的舆论压力下，[一些成人视频网站甚至也开始表态抵制 deepfakes 技术制作的视频](http://link.zhihu.com/?target=https%3A//variety.com/2018/digital/news/deepfakes-porn-adult-industry-1202705749)。随着全网抵制，deepfakes 的热度骤降，但是引发的关于伪造AV的讨论持续了几乎一整年。



deepfakes 被禁止访问的页面

在我撰写此文时， Hacker News 中刚好有同学爆料出：[以未登录的方式访问 deepfakes 的 GitHub 仓库将被限制访问](http://link.zhihu.com/?target=https%3A//news.ycombinator.com/item%3Fid%3D19182956)。但 GitHub 一直以来的规则都是：无论用户是否处于登录状态，只要是公开的仓库，任何人都可以进行访问并下载源码。deepfakes 自然一直都是一个公开仓库，而非私有仓库。

![img](https://pic4.zhimg.com/80/v2-11a1fa1a694bae9b01198ad71607841f_hd.jpg)被限制访问的 deepfakes Github 页面

对了，我在回顾 deepfakes 事件的过程中也有个有趣的发现：对这个技术最热衷的地区居然并不是中国内地，而是韩国，让我小小惊讶了一下。。。

![img](https://pic2.zhimg.com/80/v2-84718383448ba7ca13bc611bfffd2e55_hd.jpg)deepfakes 热力图

## **模型结构**

事实上，原始的 deepfakes 不论是流程，还是模型都非常符合直觉，逻辑上并不怎么复杂。相信通过下面的简单介绍，你会更理解上面这句话。

这里先一句话介绍视频换脸的整体流程：对视频换脸，其实就是对每一帧的人物图像进行面部替换，再将转换后的图像与背景音效进行合成即可。这个流程非常符合直觉，所以本质任务就是对一张图像中的人物进行面部替换。

然后再简单介绍下图像面部替换的整体流程：找出图像中的人脸区域，生成需要转换的人脸，将转换后的人脸贴到原图上（是不是想起了那个“怎么把大象关进冰箱”的问题？）。当然思路简单并不表示这些步骤没有技术含量。其中第一步人脸检测已经是非常成熟的技术，有不少开源工具能够实现，如：dlib、MTCNN 。而最后一步人脸合成也很重要，主要影响的是替换后的真实程度，这块在后文会有所体现。此文我们主要关注的是人脸转换的过程，因此我们仅介绍 deepfakes 中的人脸转换模型。

在“[当我们在谈论 Deep Learning：AutoEncoder 及其相关模型](https://zhuanlan.zhihu.com/p/27865705)”我们介绍过 AutoEncoder，其中有提到利用 AutoEncoder 对图像进行压缩，这其实就是现在流行的 embedding 应用的雏形。在这里，用到的也正是 AutoEncoder 的技术。与“[当我们在谈论 Deep Learning：AutoEncoder 及其相关模型](https://zhuanlan.zhihu.com/p/27865705)”中一样，我们利用 encoder 来提取 Input 图像的主要特征，并用 decoder 试图恢复原始 Input 图像，即

![ \begin{align} &OutputFace=Decoder(Encoder(InputFace)) \\ &Loss=Dissimilarity(InputFace,OutputFace)  \end{align} ](https://www.zhihu.com/equation?tex=+%5Cbegin%7Balign%7D+%26OutputFace%3DDecoder%28Encoder%28InputFace%29%29+%5C%5C+%26Loss%3DDissimilarity%28InputFace%2COutputFace%29++%5Cend%7Balign%7D+)

当我们用大量人物 A 的脸部图像去训练时，本质就是在对 A 的脸部提取主要信息，且这些信息足以恢复出原始脸部图像。[这个博客的示意图](http://link.zhihu.com/?target=https%3A//www.alanzucconi.com/2018/03/14/understanding-the-technology-behind-deepfakes/)可以说很形象了。



接下来的想法非常有意思：我们对两个人A、B的脸部图像分别学习上述 AutoEncoder 网络，**两个网络共享 encoder，但分别拥有自己的 decoder**。也就是说，encoder 同时能对A、B的脸部特征进行抽取（**这点代表抽取出来的A、B隐特征类型是一致的**），再通过不同的 decoder 分别将抽取的信息恢复成人脸。



最后是模型的核心：由于对 A、B 人脸抽取出的隐特征类型一致（比如器官位置、表情信息等），所以当我们将抽取的A、B 的隐特征放到对方的 decoder 时，毫无疑问恢复出来的应该仍然是人脸。但由于将 A 的脸部隐特征传入了 B 的 decoder 网络，因此还原出来了 B 的样貌，但却拥有 A 的表情，如下图。



仅通过上述 embemdding 方式恢复的人脸可能边缘会很多噪音，且在贴合回原图脸部时可能有明显的边缘。所以后来也有一种简单的改进：即在学习人脸的同时，也学习脸部边界 mask 信息，用于更好地将新的脸部贴合回旧的图片上。公式如下，原理比较简单，这里就不再详细解释了。

![\begin{align} &OutputFace=Decoder_{face}(Encoder_{face}(InputFace)) \\ &OutputMask=Decoder_{mask}(Encoder_{mask}(InputMask)) \\ &Loss=Dissimilarity(InputFace \& InputMask, OutputFace \& OutputMask)  \end{align}](https://www.zhihu.com/equation?tex=%5Cbegin%7Balign%7D+%26OutputFace%3DDecoder_%7Bface%7D%28Encoder_%7Bface%7D%28InputFace%29%29+%5C%5C+%26OutputMask%3DDecoder_%7Bmask%7D%28Encoder_%7Bmask%7D%28InputMask%29%29+%5C%5C+%26Loss%3DDissimilarity%28InputFace+%5C%26+InputMask%2C+OutputFace+%5C%26+OutputMask%29++%5Cend%7Balign%7D)

## **效果展示**

这里我们用李兰迪的头像来做试验（《你好，旧时光》的里形象还是非常可爱的），原始视频则是主播阿冷弹唱《十年》的视频，目标是用李兰迪的头像替换阿冷视频中的头像。
下面是人脸图像重建与转换的结果：第一列为李兰迪的表情，第二列是网络重建的李兰迪人脸；第三列为阿冷的头像，第四列为网络重建的阿冷头像，第五列是头像换换结果（即李兰迪的 decoder 接在阿冷 encoder 的后面）。可以看出，对脸部部分的重建与转换效果都会很不错。

![img](https://pic4.zhimg.com/80/v2-bf04165743069e0dc1c0346c10e308db_hd.jpg)人脸重建与转换效果

具体的视频转换结果如下，比较可惜的是虽然上面人脸转换效果很好，但是由于贴合到原图的逻辑还不非常完善，在脸部存在一些阴影影响最终效果，这里姑且当个示例展示出来。

<iframe frameborder="0" allowfullscreen="" src="https://www.zhihu.com/video/1083501470330048512?autoplay=false&amp;useMSE=" style="display: block; width: 688px; height: 387px;"></iframe>

原始视频：主播阿冷弹唱



<iframe frameborder="0" allowfullscreen="" src="https://www.zhihu.com/video/1083501506698960896?autoplay=false&amp;useMSE=" style="display: block; width: 688px; height: 387px;"></iframe>

转换视频：主播“李兰迪”弹唱



## **总结**

本文纯粹是出于对 deepfakes 事件的八卦心态而作，不过依然可以看出其令人惊讶的效果：如上文所展示，未精调的demo效果已经能达到比较符合预期的效果，如果花点心思收集训练数据并对结果精修，毫无疑问普通人是根本无法分辨视频真伪的——所以说社会上对这项技术的忧虑还是有原因的。

在八卦过程中，我也看到不少同学尝试对 deepfakes 的模型进行优化与改机，比如[faceswap-GAN](http://link.zhihu.com/?target=https%3A//github.com/shaoanlu/faceswap-GAN)、[DeepFaceLab](http://link.zhihu.com/?target=https%3A//github.com/iperov/DeepFaceLab)等，有兴趣的同学可以持续跟进。

## **尾巴**

按照原始 deepfakes 发帖者的说法，他本身就是个工程师而非研究员，绝大部分工作都是利用的开源工具实现。这再一次印证了在 AI 应用这方面，或许我们最缺乏的其实是想象力，而非仅仅模型连连看、或参数炼丹的能力。

## **Reference：**

[神奇女侠「下海」拍片？别兴奋，下一个可能就是你「老婆」！](http://link.zhihu.com/?target=https%3A//www.ifanr.com/977830)

[DeepFaceLab](http://link.zhihu.com/?target=https%3A//github.com/iperov/DeepFaceLab)

[faceswap](http://link.zhihu.com/?target=https%3A//github.com/deepfakes/faceswap)

## **本系列其他文章：**

[专栏总目录（新）](https://zhuanlan.zhihu.com/p/36508745)