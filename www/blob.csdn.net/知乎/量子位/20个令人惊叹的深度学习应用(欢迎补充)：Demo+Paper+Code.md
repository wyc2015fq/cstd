# 20个令人惊叹的深度学习应用(欢迎补充)：Demo+Paper+Code - 知乎
# 



唐旭 发自 RUC
量子位 报道 | 公众号 QbitAI

从计算机视觉到自然语言处理，在过去的几年里，深度学习技术被应用到了数以百计的实际问题中。诸多案例也已经证明，深度学习能让工作比之前做得更好。

今天，量子位为大家收集了20个深度学习方面的优秀应用——当然，这份榜单可能并不详尽，但相信看过之后，你对这项技术在某些领域的潜力会有更清晰的认识。

针对每个应用，我们还尽量收集了相关的Demo、Paper和Code等信息。

# 1、Face2Face：扮演特朗普

![](https://pic3.zhimg.com/v2-52d5fddee106325a31533edaea3ea8e6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1280' height='363'></svg>)斯坦福大学的一个小组做了一款名为Face2Face的应用，这套系统能够利用人脸捕捉，让你在视频里实时扮演另一个人，简单来讲，就是可以把你的面部表情实时移植到视频里正在发表演讲的美国总统身上。

同样的原理也可以用于对视频里场景的3D重建、电影特效也可以这么干。

这个应用的实际效果是这样的：

![](https://pic4.zhimg.com/v2-84241b47381c5ec79d8e2e0dc0154bc3_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='480' height='360'></svg>)Paper：

[http://www.graphics.stanford.edu/~niessner/papers/2016/1facetoface/thies2016face.pdf](https://link.zhihu.com/?target=http%3A//www.graphics.stanford.edu/~niessner/papers/2016/1facetoface/thies2016face.pdf)

# 2、Let there be color！：给黑白照片/视频自动上色

![](https://pic2.zhimg.com/v2-843492b089ee6f46f611c3da952f8591_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='927' height='600'></svg>)左图是拍摄于1937年的一幅矿工的照片，右图是用Let there be color！重新自动上色后的效果。

![](https://pic2.zhimg.com/v2-20cd8e10f06f89b703051bcf5b4fef85_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='360'></svg>)上图是拍摄于1909年的一张保龄球馆的照片，下图是用Let there be color！重新上色后的效果。

![](https://pic3.zhimg.com/v2-d8946fe3c3ab6ce7f6e9ca8b73b935ae_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='360'></svg>)实际上，这是运用深度学习网络学习自然存在于照片中的某些模式——比如，天通常是蓝的，云是白的或者灰的，草是绿的。通过这类规则，Let there be color！不需要人类的介入就能对照片进行重新上色。虽然有时它也会犯错，但这种错误很难被发现。

同样地，Let there be color！也可以把黑白视频变成彩色的。

Demo：

[Automatic Image Colorization・白黒画像の自動色付け](https://link.zhihu.com/?target=http%3A//hi.cs.waseda.ac.jp%3A8082/)

Paper：

[http://hi.cs.waseda.ac.jp/~iizuka/projects/colorization/data/colorization_sig2016.pdf](https://link.zhihu.com/?target=http%3A//hi.cs.waseda.ac.jp/~iizuka/projects/colorization/data/colorization_sig2016.pdf)

Code：

[satoshiiizuka/siggraph2016_colorization](https://link.zhihu.com/?target=https%3A//github.com/satoshiiizuka/siggraph2016_colorization)

# 3、Pixel Recursive Super Resolution：告别马赛克

我为什么不看美剧CSI？原因很简单：太假了。里面的主角动不动就把一段视频拉近、放大——然后他们就得到了一幅分辨率远超真实情况的影像。这根本不现实……

直到深度学习出现。今年早些时候，谷歌大脑的研究者们训练了一个深度学习网络，他们让后者根据一些分辨率极低的人脸图像来预测这些面孔真实的样子。如下图：

![](https://pic4.zhimg.com/v2-aaef9a5c5118154e989b15ce27a9e163_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='375' height='400'></svg>)最左边是输入的8×8像素的原始影像，最右一列是被拍摄的人脸在照片中的真实效果，中间则是电脑的猜测。

可以看到，虽然并不完美，但电脑预估出的结果已经与实际情况十分接近。Google的研究者将这种方法命名为 Pixel Recursive Super Resolution（像素递归超分辨率），用这种方法能显著提升图像的质量。

Paper：

[[1702.00783] Pixel Recursive Super Resolution](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1702.00783)

# 4、实时多人动作预估

深度学习网络已经能在预估动作方面帮动画师很大的忙，如今，我们甚至能做到实时预估。康奈尔大学Zhe Cao等人教会了一个神经网络估算人类骨架位置的变化。

在如下的gif中，你可以看到一群人在街头跳舞，而通过这种神经网络，我们能知道他们在哪、如何做动作。

![](https://puui.qpic.cn/qqvideo_ori/0/r0393pmzsgn_228_128/0)深度学习实时预估多人动作_腾讯视频https://v.qq.com/x/page/r0393pmzsgn.html


Paper：

[https://arxiv.org/abs/1611.08050](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1611.08050)

# 5、Neural Talk：描述照片

![](https://pic3.zhimg.com/v2-44eee2c41800164d15ee8ec94e777bfa_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1140' height='297'></svg>)用电脑来对照片进行自动分类已经很常见了。比如，Facebook能在分享的照片里标记出你的好友，Google可以为你的照片打上标签以便更有效率地进行搜索。

而如今，深度学习已经可以对照片中的各种元素进行描述。在Andrej Karpathy和李飞飞所做的一项工作中，他们训练了一个能对照片中不同区域内元素进行识别，并用一句话来描述照片的深度学习系统。

Demo：

[Image Annotation Viewer](https://link.zhihu.com/?target=http%3A//cs.stanford.edu/people/karpathy/deepimagesent/generationdemo/)

Paper：

[http://www.cv-foundation.org/openaccess/content_cvpr_2015/papers/Karpathy_Deep_Visual-Semantic_Alignments_2015_CVPR_paper.pdf](https://link.zhihu.com/?target=http%3A//www.cv-foundation.org/openaccess/content_cvpr_2015/papers/Karpathy_Deep_Visual-Semantic_Alignments_2015_CVPR_paper.pdf)

# 6、DeepWarp：你瞅啥？再瞅把你做成表情包！

![](https://pic1.zhimg.com/v2-fcb9b3ce061de05cdd16cddf423ab828_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='600'></svg>)这个应用有点恶搞的意思。现在，不管你手里拿到了谁的照片，用Ganin等人开发的这个深度学习网络过一下，你就可以让他的眼睛动起来：你可以让他上下看、左右看，甚至绕着圈看……好吧，除了生产表情包，其实也不知道它还能干什么用……

这是一些例子，里面有伊丽莎白女王、奥巴马、莎拉波娃……等等：

[DeepWarp Project Page](https://link.zhihu.com/?target=http%3A//sites.skoltech.ru/compvision/projects/deepwarp/)

Demo：

[DeepWarp Demo Page](https://link.zhihu.com/?target=http%3A//163.172.78.19/)

Paper：

[http://sites.skoltech.ru/compvision/projects/deepwarp/files/deepwarp_eccv2016.pdf](https://link.zhihu.com/?target=http%3A//sites.skoltech.ru/compvision/projects/deepwarp/files/deepwarp_eccv2016.pdf)

# 7、生成新照片

来自怀俄明州立大学的Anh Nguyen以及其他几位研究者开发了一套能用迭代的方式从已有照片中合成全新照片的深度学习网络。以下是一些电脑合成的样片，结果令人惊艳：

![](https://pic2.zhimg.com/v2-53701c4e061e602a30151f18cfef691d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='404' height='430'></svg>)Paper：

[https://arxiv.org/pdf/1612.00005.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1612.00005.pdf)

# 8、用卷积神经网络来拯救鲸鱼

众所周知，卷积神经网络在图像识别分类领域有着非常好的前景。利用深度学习，我们可以将抓拍到的珍稀动物（比如鲸鱼）照片进行分类，从而更好地估算某种动物的存活数量。

![](https://pic1.zhimg.com/v2-a0f2aadb6259dee3a58a28513a33c09c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1339' height='753'></svg>)其他的例子包括浮游生物、植物等等。

分类浮游生物的介绍：

[Classifying plankton with deep neural networks](https://link.zhihu.com/?target=http%3A//benanne.github.io/2015/03/17/plankton.html)

Code：

[benanne/kaggle-ndsb](https://link.zhihu.com/?target=https%3A//github.com/benanne/kaggle-ndsb)

# 9、Pix2Pix：灵魂画师

![](https://pic3.zhimg.com/v2-4eb7c3ea7c272568854aab22a4678536_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='990' height='370'></svg>)这个点子与之前提到的Let there be color！有点类似，不过要更有创造力一点。你可以往电脑中输入一幅简笔画，甚至一些色块，然后让电脑发挥自己的创造力输出新的图片。类似地，你也可以把衣服航拍照片变成一幅地图，将白天的场景变成夜晚。

前段时间特别火的信手画猫(传送门：[人工智能拯救渣画手，我分分钟画只猫给你看](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s/HcdYqdit-3oP0dDK85WkAw))，也是基于这个技术。

Paper：

[https://arxiv.org/pdf/1611.07004v1.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1611.07004v1.pdf)

Code：

[phillipi/pix2pix](https://link.zhihu.com/?target=https%3A//github.com/phillipi/pix2pix)

# 10、Reading text in the Wild：用图片找文字

牛津视觉几何小组运用深度学习技术开发了一项应用，它可以读出视频中的文字，输入文字，就可以直接搜出图像中包含这些文字的BBC新闻视频。比如，这是对“London”的一部分搜索结果：

![](https://pic3.zhimg.com/v2-7d063c0620703dbe15ef6b004c50a95a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='994' height='416'></svg>)Demo：

[Visual Geometry Group Home Page](https://link.zhihu.com/?target=http%3A//www.robots.ox.ac.uk/~vgg/research/text/%23sec-demo)

Paper：

[[1412.1842] Reading Text in the Wild with Convolutional Neural Networks](https://link.zhihu.com/?target=http%3A//arxiv.org/abs/1412.1842)

# 11、Google Sunroof：看你家房顶能收到多少太阳能

![](https://pic3.zhimg.com/v2-36f2888cea4461dd26e2fdfc63e7864e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='299'></svg>)Google Sunroof首先会根据Google地球的航拍地图为你家的屋顶创建一个3D模型，然后再用深度学习将屋顶和周围的数目区分开，接着根据太阳运行轨迹以及天气状况，就可以估算出安装了太阳能电池板的房顶能收集到多少太阳能。

项目首页：

[https://www.google.com/get/sunroof#p=0](https://link.zhihu.com/?target=https%3A//www.google.com/get/sunroof%23p%3D0)

# 12、AI制霸打砖块

Google的Deepmind团队用深度强化技术教会了AI玩《打砖块》（Breakout，基本大家都玩过，具体是什么请看下图）。

测试过程中，电脑并没有依据任何游戏规则被进行特殊的编成，他们只是将键盘的控制权交给AI，然后对它进行不断地训练。起初，AI玩得十分糟糕；但经过两个小时的训练之后，画风就变成了这个样子的：

![](https://pic4.zhimg.com/v2-166fde08b9eb7671a29d9c5467970a4f_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='292' height='360'></svg>)Paper：

[https://www.cs.toronto.edu/~vmnih/docs/dqn.pdf](https://link.zhihu.com/?target=https%3A//www.cs.toronto.edu/~vmnih/docs/dqn.pdf)

Code：

[https://sites.google.com/a/deepmind.com/dqn/](https://link.zhihu.com/?target=https%3A//sites.google.com/a/deepmind.com/dqn/)

# 13、制霸全场~

![](https://pic2.zhimg.com/v2-9df880f201d577f0b9ced93716cdd271_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1036' height='1278'></svg>)打砖块还不够？深度学习技术还被用来在更多游戏领域训练AI，包括：《小蜜蜂》、《毁灭公爵》、《乒乓》等几十种游戏。在大多数的游戏里，深度学习网络已经玩的比有经验的玩家更好，同样所有的玩法都是AI自己摸索的。

Paper：

[http://www.davidqiu.com:8888/research/nature14236.pdf](https://link.zhihu.com/?target=http%3A//www.davidqiu.com%3A8888/research/nature14236.pdf)

# 14、LipNet：让AI读唇语

![](https://puui.qpic.cn/qqvideo_ori/0/k039309pb7y_228_128/0)AI识别唇语_腾讯视频https://v.qq.com/x/page/k039309pb7y.html


来自牛津大学和Deepmind的科学家共同完成了这一项目。LipNet在读唇的准确率方面达到了惊人93%，远超人类读唇者52%的平均水平。

Paper：

[https://arxiv.org/pdf/1611.01599.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1611.01599.pdf)

Code：

[bshillingford/LipNet](https://link.zhihu.com/?target=https%3A//github.com/bshillingford/LipNet)

# 15、真·灵魂画师

如今，运用深度学习技术，可以让AI对某一幅画的风格、色彩、明暗等元素进行学习，然后将这幅画上的风格移植到另一幅上，而且效果非常不错。

这么说可能不够直观，来看几个例子：

下面从左到右，依次是毕加索、梵·高和莫奈风格的蒙娜丽莎。

![](https://pic3.zhimg.com/v2-1610d78142866c559d6db5648af71156_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1029' height='512'></svg>)而这是某位老哥用AI创作的谷歌地图风格的蒙娜丽莎……

![](https://pic1.zhimg.com/v2-dfe1a24347e3cae10e92d6892397e438_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='342' height='512'></svg>)Demo：

[https://deepart.io/](https://link.zhihu.com/?target=https%3A//deepart.io/)

Paper：

[http://www.cv-foundation.org/openaccess/content_cvpr_2016/papers/Gatys_Image_Style_Transfer_CVPR_2016_paper.pdf](https://link.zhihu.com/?target=http%3A//www.cv-foundation.org/openaccess/content_cvpr_2016/papers/Gatys_Image_Style_Transfer_CVPR_2016_paper.pdf)

# 16、AI写字

![](https://pic2.zhimg.com/v2-837d17ad88d00849c5f9555cc98ca67d_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1136' height='75'></svg>)现在电脑不光能用来打字，还能用来写字了：多伦多大学的Alex Graves教会了电脑用多种不同的风格来书写。

Demo：

[http://www.cs.toronto.edu/~graves/handwriting.html](https://link.zhihu.com/?target=http%3A//www.cs.toronto.edu/~graves/handwriting.html)

Paper：

[[1308.0850] Generating Sequences With Recurrent Neural Networks](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1308.0850)

# 17、深度学习预测城市热人口和选举结果

![](https://pic1.zhimg.com/v2-241033808189e46f73c09e8d32fc2684_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1152' height='1372'></svg>)Gebru等人收集了5000万张Google街景照片，然后用深度学习技术加以挖掘看看能发现什么。结果，深度学习不仅识别出了2200万辆汽车的材质、车型、年份等，还从中发现了一些关联，比如可以通过对车型的识别来分析某片区域的人口状况。

更有趣的是，AI发现，如果在一段15分钟长的驾驶路程上遇到的轿车比皮卡更多，那么这个城市在下届的总统选举中有88%的几率将选票投给民主党；否则，则有82%的几率投给共和党。

Paper：

[https://arxiv.org/pdf/1702.06683.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1702.06683.pdf)

# 18、Deep Dreaming：AI也会白日做梦

![](https://puui.qpic.cn/qqvideo_ori/0/l0393pta6st_228_128/0)Deep Dream_腾讯视频https://v.qq.com/x/page/l0393pta6st.html


2015年，Google的研究者找到了一种用深度学习来增强图像特征的方法，随后，他们就开发了Deep Dreaming——它能够在图片中生成一些不可思议的幻象。因为有些和梦境十分相似，因此取名Deep Dreaming。具体生成什么样的幻想与这个深度学习系统最近被暴露在什么样的环境下有关，有时它甚至能生成吓人的噩梦。

项目首页：

[https://research.googleblog.com/2015/06/inceptionism-going-deeper-into-neural.html](https://link.zhihu.com/?target=https%3A//research.googleblog.com/2015/06/inceptionism-going-deeper-into-neural.html)

影集：

[Google Deep Dream: 19 of the best images from mesmerising photo software](https://link.zhihu.com/?target=http%3A//www.telegraph.co.uk/technology/google/11730050/deep-dream-best-images.html%3Fframe%3D3370674)

Code：

[google/deepdream](https://link.zhihu.com/?target=https%3A//github.com/google/deepdream)

# 19、反复攻破和修补自己的防火墙

![](https://pic1.zhimg.com/v2-5b8b67e3d4a107ac0dde6dd324e08fa8_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='618' height='211'></svg>)Google大脑的研究团队创建了两个深度学习网络用于安全工具开发，他们让其中一个不断创造自己的加密算法，然后让另一个网络去尽力攻破它。在两套系统的反复缠斗后，第一个系统已经能生成非常优秀的安全加密算法。

然而，第二套算法也总是表现得比它更好……

Paper：

[https://arxiv.org/pdf/1610.06918v1.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1610.06918v1.pdf)

# 20、预测地震

![](https://pic4.zhimg.com/v2-17807d544fe73260bf2bc43dbc5ef627_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1266' height='832'></svg>)哈佛的科学家们运用深度学习技术让电脑学会“粘弹性计算”——一种被用于预测地震的计算方法。结果，计算的效果在AI上大大被增强了。

然而，计算机同时将计算速度提高了50000%。在地震中，时间就是生命。因此，目前用AI来预测地震还为时尚早。

Paper：

[https://arxiv.org/pdf/1701.08884v1.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1701.08884v1.pdf)

**好啦，先介绍到这里。如果漏掉了“好厉害!”的深度学习应用，欢迎留言补充~**

## One More Thing…

我们的公众号也叫量子位（QbitAI），关注并回复“今天”，看更多AI领域大新闻。

另外，欢迎加量子位小助手的微信：qbitbot，如果你研究或者从事AI领域，小助手会把你带入量子位的交流群里。


