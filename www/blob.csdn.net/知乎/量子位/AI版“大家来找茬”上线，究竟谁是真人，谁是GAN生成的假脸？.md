# AI版“大家来找茬”上线，究竟谁是真人，谁是GAN生成的假脸？ - 知乎
# 



> 郭一璞 发自 凹非寺 
量子位 报道 | 公众号 QbitAI

一直以来，都是人类用各种奇奇怪怪的问题刁难AI：

识别人脸、生成画作、理解语义、分辨文字……

现在，人类终于也轮到被AI刁难了。

MIT的学生就开发了一个AI版的“大家来找茬”：

把真人照片和GAN生成的人脸混在一起，加上时间限制和条件限制，让火眼金睛的你识别出来。
![](https://pic3.zhimg.com/v2-9c2a549a2d80ce340d26416bc3ceaae2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='379'></svg>)
这些照片真假难辨，你需要一步步得分闯关，越玩越难。

还可以拿这个游戏欺负脸盲的量子位同事。

这张脸，是天生的，还是GAN生的，这是个问题。

## **由易入难的闯关体验**

游戏规则嘛，很简单。

给你一张照片，请你判断这是真脸还是假脸。

照片一共10组，每组6张依次出现，每两张照片之间间隔3秒，看完照片之后选择**Real** or **Fake**，作出判断，最后依据判断正确的数量算出得分。
![](https://pic2.zhimg.com/v2-64db37530068df62b519345e3c0ab645_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='435' height='398'></svg>)
10组游戏各不相同，第一组每张照片可以看**5秒钟**，能够仔细观察，寻找bug；

第二组和第三组的时间就减少了，分别只能看**2秒**和**1秒**；

到第四组和第五组，每张人脸照片一眨眼就过去了，分别只有**0.5秒**和**0.25秒**，根本来不及找茬。
![](https://pic4.zhimg.com/v2-45bc4cdb4b725042e4723b9b1350db0f_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='360' height='331'></svg>)
后面五组则是把所有人脸的眼睛遮住，然后按照展示5秒、2秒、1秒、0.5秒、0.25秒的顺序再来一遍。

量子位同事们体验了一下，发现每组总分6分，一开始得个5、6分还是不难的，后面速度快了就基本靠直觉了。

## **游戏攻略**

虽然游戏中出现的人脸都非常精致，让人难以区分，不过，这也不妨碍万能的网友们从别处入手，发现一些得高分的捷径。

想挑战自己的同学，可以先复制下面链接裸考一下：

[http://nikola.mit.edu/](https://link.zhihu.com/?target=http%3A//nikola.mit.edu/)

体验之后，我们再来看“小抄”：

**攻略1：头发边缘**

头发这种一根一根的东西，对许多图像生成/处理程序来说都是一大难题，因此，头发的问题是最容易暴露的。

比如这种：
![](https://pic2.zhimg.com/v2-336f2cc8e50b32405f38473b50b7f041_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='511' height='506'></svg>)
左眼眼角的头发边缘十分奇怪，右侧的头发也很毛躁，像没梳头就出门一样。

这种：
![](https://pic4.zhimg.com/v2-83380c4c8b305d58b6a2158b4a74165b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='511' height='506'></svg>)
小哥，你左边的头发是沾水了么？

还有这种：
![](https://pic2.zhimg.com/v2-90c315ead76097387bbb27b2559c35ad_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='511' height='506'></svg>)
金发下面长了一个时空隧道？这是什么高端发型师的作品？

头发外部的轮廓，经常会和背景化为一体，出现各种抠图问题。

当然，你也可以凭直觉判断：都是明星风格的照片，那些头发凌乱的，如此不注意个人形象，一看就不是真明星。

**攻略2：观察背景**

真实场景下，人物的背景要么是真实而清晰的，要么是非常自然的背景虚化。

而GAN生成背景就很业余了，会出现下面这种类似的问题：
![](https://pic2.zhimg.com/v2-c145b8423d7931634c5892663d1985b1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='511' height='506'></svg>)
背景魔幻而迷离，但是丝毫不美观，浓浓的一股GAN玩坏了的气息。

**攻略3：耳朵和耳环**

GAN可能意识到了人的耳朵都是对称的，但是它并不知道人的耳环也应该是对称的。
![](https://pic1.zhimg.com/v2-ddd505ebb8e5c44bc80fafa050a45a6c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='511' height='506'></svg>)
比如这个妹子，左右两只大大的耳坠明显不对称，而且这耳坠看起来十分模糊，完全不像正常珠宝那样有清晰、闪光的轮廓，不符合世界上的任何一种正常的审美。

明星当然不会戴这种鬼东西出门啦，一定是假的。

还有下面这位老太太，耳朵只有下边缘，没有上边缘，完全不像人类，自然也是GAN生成的。
![](https://pic2.zhimg.com/v2-3d4bc397515db7b92f9360a81314daed_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='511' height='506'></svg>)
除了看头发、背景和耳朵之外，还有一些网友发现，在后面五关遮住眼睛的情况下，自己的分数明显高于前面五关。

对此他们得出结论：**不要看眼睛**，眼睛是心灵的窗户，一旦被眼睛“勾了魂”，你就注意不到其他地方的bug了。

当然，这些技巧也不一定都有用，毕竟样本几乎都是欧美人，我们大部分中国人很难在日常生活中“学习”到足够的数据，判断欧美人五官什么的，还是有点难度的。

## **made by NVIDIA**

看了这么多真假难辨的脸，其中那些假脸是哪儿来的呢？

这要感谢英伟达在今年初推出的Progressive GAN，一个“明星脸”生成器，从4×4低分辨率开始，逐渐为生成器和鉴别器增加新的层，经过18天的训练，生成1024×1024大小的、以假乱真的人脸照片。
![](https://pic1.zhimg.com/v2-47e90894801fb62d49856694c2c01d40_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='633'></svg>)
## **传送门**

游戏地址：

[http://nikola.mit.edu/](https://link.zhihu.com/?target=http%3A//nikola.mit.edu/)

英伟达Progressive GAN：

[https://research.nvidia.com/publication/2017-10_Progressive-Growing-of](https://link.zhihu.com/?target=https%3A//research.nvidia.com/publication/2017-10_Progressive-Growing-of)

亲测手机端可玩，欢迎在评论区秀出你们的分数~




— **完** —

[量子位](https://www.zhihu.com/org/liang-zi-wei-48) · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态

欢迎大家关注我们，以及订阅[我们的知乎专栏](https://zhuanlan.zhihu.com/qbitai)

**诚挚招聘**

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！

相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。


