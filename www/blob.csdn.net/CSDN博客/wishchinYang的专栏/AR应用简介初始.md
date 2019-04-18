# AR应用简介初始 - wishchinYang的专栏 - CSDN博客
2013年11月25日 21:55:30[wishchin](https://me.csdn.net/wishchin)阅读数：1921
        原文出处：[kent.zhu'sBlog](http://www.ikent.me/blog/3849)
         一个AR系统需要有显示技术、跟踪和定位技术、界面和可视化技术、标定技术构成。
        跟踪和定位技术与标定技术共同完成对位置与方位的检测，并将数据报告给AR系统，实现被跟踪对象在真实世界里的坐标与虚拟世界中的坐标统一，达到让虚拟物体与用户环境无缝结合的目标。为了生成准确定位，AR系统需要进行大量的标定，测量值包括摄像机参数、视域范围、传感器的偏移、对象定位以及变形等。
        相对与智能手机而言，AR就是根据当前位置（GPS），和视野朝向（指南针）及手机朝向（方向传感器/陀螺仪），在实景中（摄像头）投射出相关信息并在显示设备（屏幕）里展示。其实现的重点在于投影矩阵的获取。
        当然，在实际开发的时候其实android系统已经将投影矩阵封装的比较好了，可以通过接口直接获取投影矩阵，然后将相关的坐标转换算成相应的坐标就可以了。
        移动增强现实系统应实时跟踪手机在真实场景中的位置及姿态，并根据这些信息计算出虚拟物体在摄像机中的坐标，实现虚拟物体画面与真实场景画面精准匹配，所以，registration（即手机的空间位置和姿态）的性能是增强现实的关键。移动AR的运作原理可以以下面这个图示简单来说明。
![](http://cms.csdnimg.cn/articlev1/uploads/allimg/110831/104_110831172050_1.jpg)
        OK，科普的事情做完了，下面再来说说AR到底有些啥搞头
**1.找到想去的地方**
**        1.1找到我想去的那家店**
![](http://cms.csdnimg.cn/articlev1/uploads/allimg/110831/104_110831172225_1.jpg)
[Yelp Monocle](http://itunes.apple.com/us/app/yelp/id284910350?mt=8)利用iPhone的摄像头和数字罗盘把Yelp评分和实时的街景结合起来，这样您就可以找出五星级酒吧而不是误入那些自以为酷的小酒馆了。
**        1.2找到我的车**
![](http://cms.csdnimg.cn/articlev1/uploads/allimg/110831/104_110831172340_1.jpg)
        当你把车停在路边，并掏出手机启动Car Finder时，软件会记录下你的GPS信息。之后，当你想在茫茫车海中寻找它时，[Car Finder](http://itunes.apple.com/us/app/find-your-car-ar-augmented/id370836023?mt=8)会利用摄像头，GPS，指南针，陀螺仪等多个传感器把你引导至正确的位置。
**        1.3指引路径**
![](http://cms.csdnimg.cn/articlev1/uploads/allimg/110831/104_110831172420_1.jpg)
[Wikitude Drive](https://market.android.com/details?id=com.wikitude)增强现实导航应用，用户看到的不是地图，而是前方街道的实时视图，以及叠加在视频上方的导航数据。现在已经在欧洲、澳大利亚、北美市场得到了应用。
**        2.“碰”到虚拟的东西**
![](http://cms.csdnimg.cn/articlev1/uploads/allimg/110831/104_110831172534_1.jpg)
[AR SOCCER](http://itunes.apple.com/us/app/arsoccer-augmented-reality/id381035151?mt=8)虚拟颠球，就是你把屏幕对着一块干净的地板，然后屏幕上会出现一个足球，现在你可以用脚去踹它。
**3.令现实按我的想法变化**
**        3.1实景翻译**
![](http://cms.csdnimg.cn/articlev1/uploads/allimg/110831/104_110831172650_1.jpg)
[Word Lens](http://itunes.apple.com/us/app/word-lens/id383463868?mt=8)是一个实景翻译的应用，不过目前只提供英语和西班牙语的相互翻译。
**        3.2虚拟试衣**
![](http://cms.csdnimg.cn/articlev1/uploads/allimg/110831/104_110831172823_1.jpg)
        使用[Zugara](http://zugara.com/)的虚拟试衣间非常简单。你需要一台带摄像头的电脑和一点空间，后退到离摄像头4-5英尺的地方挥一挥手，你选中的衣物会自动“穿”到你身上。如果你觉得没有“穿好”，你还可以通过微调衣物的位置使其看起来与你更贴合。
**        3.3拍照**
![](http://cms.csdnimg.cn/articlev1/uploads/allimg/110831/104_110831172923_1.jpg)
        在使用“[Farrago AR](http://itunes.apple.com/gb/app/farrago-ar-3d-augmented-reality/id449709362?mt=8)”时，用户可以轻松通过移动设备的触摸屏对图片内出现的物体进行旋转、调整大小、修正等精细操作。而且，“Farrago AR”友好的用户界面使得用户可以轻松创造出2D或者3D的图片外物体。
**4.把虚拟叠加到现实上**
![](http://cms.csdnimg.cn/articlev1/uploads/allimg/110831/104_110831173044_1.jpg)
        伦敦博物馆出了一个增强现实的Apps，称其为“[时光机器](http://itunes.apple.com/gb/app/museum-london-streetmuseum/id369684330?mt=8)”，把手机对准当前所在的位置，那么系统会帮你匹配当前位置几十年前的样子。
![](http://cms.csdnimg.cn/articlev1/uploads/allimg/110831/104_110831173222_1.jpg)
[Layar Reality Browser](http://www.layar.com/browser/)把摄像头对准周遭的景物，Layar会把各种数据找出来——巴士站、滑板公园以及房地产价格等等。
![](http://cms.csdnimg.cn/articlev1/uploads/allimg/110831/104_110831173428_1.jpg)
[Wikitude](http://www.wikitude.com/)被称为“世界浏览器”，它可以帮你探索周围的环境，查找地标的资料。只要你举起手机并打开摄像头，屏幕上就会出现一些标记——包括维基百科词条，带GEO标签的 Tweet，ATM的位置。
![](http://cms.csdnimg.cn/articlev1/uploads/allimg/110831/104_110831173536_1.jpg)
[Star Walk](http://itunes.apple.com/us/app/star-walk-5-stars-astronomy/id295430577?mt=8)，增强现实的“天文互动指南”，利用GPS，指南针和陀螺仪教你辨别星座。还有很多类似的游戏，比如星球大战AR版、AR Invaders（打飞碟）、AR Missile（导弹毁人）、ARBasketball（实景篮球）
…..最后，说一下几个基于AR做平台的
**        1.高通**
![](http://cms.csdnimg.cn/articlev1/uploads/allimg/110831/104_110831173659_1.jpg)
        高通放出了android平台和ios平台上上的[augmented reality develop kit](https://ar.qualcomm.at/qdevnet/sdk)。基于这个SDK开发人员更容易的运用智能设施中的摄像头，开端出实在内容与虚构内容联结的软件利用。
**        2.Layar**
        Layar旨在打造的一个开放的增强现实的平台，任何第三方都可以通过Layar的开发接口来打造基于Layar的自己的增强现实应用。目前其官方网站上列出的应用有2029个，其应用类型包括教育，游戏，建筑，艺术，交通，游戏等等；你也可以把Layar看做专门为AR应用搭建的App Store，因为里面的应用有免费的，也有收费的。
        最后，说点扯淡的事情
        移动硬件设备的发展使得人们在移动设备上的交互有了突破性的进展，以NFC、AR、裸眼3D等为代表的一大批应用形态的涌现给这个领域带来了最够的新奇与动力，创造了全新的用户体验。而在移动产品的设计上，如何利用手机的硬件性能来创造突破性的产品将是未来决定移动产品设计师能力以及移动产品成败的关键了。
[](http://www.ikent.me/blog/3849)
