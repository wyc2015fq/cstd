# 碉堡了！基于 Linux 的可自动瞄准的智能步枪 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [sphynx](http://www.jobbole.com/members/sphynx) 翻译，[至秦](http://www.jobbole.com/members/applevip) 校稿。未经许可，禁止转载！
英文出处：[Lee Hutchinson](http://arstechnica.com/gadgets/2013/01/17000-linux-powered-rifle-brings-auto-aim-to-the-real-world/)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
***【伯乐在线注】：**此文并不是 TrackingPoint 产品的最新报道，而是基于 arstechnica  2013 年的一篇报道。*
CES *（此处指 2013 年的 CES）*真是包罗科技万象；当我们忙于报道摄影器材、电视和 CPU 时，却忽略了大量存在于我们视线之外的科技产品。总部位于 Austin 的初创公司 [TrackingPoint](http://tracking-point.com/) 并不属于我们传统的关注范围，但就凭它运用科技做到完美一击这一点，足以使我驻足把玩它的产品。
![](http://ww2.sinaimg.cn/large/7cc829d3gw1f05q5fo0i5j20hs0at0ul.jpg)
TrackingPoint 制造的“精确制导枪支”，也可称为“PGF”，这一系列有三支高度定制的猎枪，从 22 英寸枪管的点 300 Winchester Magnum，到 27 英寸枪管的点 338 Lapua Magnum， 它们都配备了类似在《终结者》中的高度数字化的瞄准镜。实际上把 PGF 与这部电影做比较是贴切的，这把枪的瞄准镜上能看到（与电影中相仿的）许多数据点和数字，所有设计都是为让子弹精准地从 A 点飞到 B 点。
![](http://ww3.sinaimg.cn/mw690/7cc829d3gw1f05q5ij3i3j20h90d0jsw.jpg)
*TrackingPoint **的数字化瞄准镜视角*
PGF 并不仅仅是步枪加上豪华瞄准镜那么简单。PGF 的组成包含一支枪，一个经过改良和阻力可调的扳机机构，一个数字化的瞄准镜，以及手动加载的定制弹夹 （只在 TrackingPoint 有卖）。这一概念有点类似卖剃刀和刀片，但制造子弹要求相当严格，因为计算机控制下的精确制导需要子弹本身也要符合一定的标准。
![](http://ww3.sinaimg.cn/mw690/7cc829d3gw1f05q5g95pnj212h0ejwg7.jpg)
*TrackingPoint XS1**， 装备 27 英寸 Krieger 枪管和 300 发定制子弹的 .338 Lapua Magnum*
瞄准镜上显示的图像并非实景，而是由瞄准镜上的物镜所拍摄的视频图片。基于 Linux 的瞄准镜生成的画面，和你在战斗机驾驶舱内从头盔显示器看到的类似，上面显示了武器的罗盘方位，斜面和倾角。要射击目标，首先用扳机旁的一个按钮来“标记”目标。标记完的目标会由内置的激光设备照亮，从瞄准镜的显示上能看到这个目标上加了标记。当目标被标记后，瞄准镜采集目标范围，环境温湿度，枪管年份以及许多其它参数并开始计算。计算完毕后显示器迅速重新定向，屏幕中央的十字标记准确地显示子弹将要去的地方。
图像识别程序使瞄准镜画面上的点标记一直处于目标上，此时扣动扳机。枪支这时并不会开火；而是在保持扳机扣动状态下，把十字标记移动至目标上的点标记位置，使十字标记由蓝变红。一旦两个标记重合，就会开火。
![](http://ww4.sinaimg.cn/large/7cc829d3gw1f05q5jppx5j20z80b9wl4.jpg)
*标记目标，扣动扳机，使十字标记和目标标记重合。*
TrackingPoint 很快强调，步枪并不是“自动”开火，事实是扳机的阻力在两个标记点重合之前，被动态地提到很高，而阻力在标记重合时恢复初始值。这样的话，射手始终保持对射击时机的控制，在标记重合开火前的任意时刻都可以手动释放扳机。TrackingPoint 公司展台上，模拟实物上的操作也是一样的 —— 我按下了扳机，使两个标记重叠起来，然后蓝色塑料玩具枪就开火了。
![](http://ww1.sinaimg.cn/large/7cc829d3gw1f05q5i0wkoj20ku0i9416.jpg)
*数字化的瞄准镜*
当标记重合时开火，而不是对扳机动作作出即时响应，这样消除了射击中大量的不确定因素。即便是最有经验的射手都有可能在扣动扳机时失去目标，要克服条件反射的肌肉抽搐，或者考虑到枪的后坐力的提前移动，都是相当得有难度。而让计算机决定射击的精确时机，射击准确度则大大地提升了。
TrackingPoint PGF 系统除了提高射击的准确度，还提供了其它的功能。数字化瞄准镜配有非易失性存储器，就像飞机的黑匣子一样，可以持续记录瞄准镜捕捉到的影像。此外瞄准镜还装备了一个微型 Wi-Fi 服务器，可以通过TrackingPoint 提供的 iOS App， 使用 ad-hoc Wi-Fi 网络与之相连并实时接收所摄影像，这样可以把 iPad 和 iPhone 变成捕猎的显示器。TrackingPoint 发现对于打猎的新手来说，把瞄准镜的影像扩展到一个额外的显示器上，可以让有经验的猎手更容易和新手进行教学互动。
![](http://ww3.sinaimg.cn/mw690/7cc829d3gw1f05q5estmrj21hc0u0dsm.jpg)
*iPad app **扩展了瞄准镜的显示，使有经验的猎手更容易帮助新手捕猎。*
连社交媒体的功能也没有落下 —— 瞄准镜的影像能上传到 YouTube 这样的分享站点。与其在小伙伴们面前夸耀上周如何从一千码外命中猎物，逼格高的做法是 —— 把头盔显示器中环境和射程的数据直接共享给他们。
TrackingPoint 在媒体室展示了一支真枪，以及一些在瞄准器位置配备 iPhone 的模拟枪。iPhone 上面运行的是TrackingPoint 瞄准镜上配套软件的仿真版本，让试玩者对准动画鹿和动画猪射击，更像流行的打猎游戏。上手却感觉有点像在玩游戏中的“简易模式”并打开了作弊码，因为即使从相当远的射程开火也不会失手。TrackingPoint 打算把演示软件作为一款独立的打猎 app 销售，以我简单的体验来看，由于标记开火方法神一般的存在，这个游戏并没有多大挑战。
这套软件兴许不会成为什么抓人眼球的电子游戏，但绝对造就了一个精确的武器制导系统。根据TrackingPoint 的说法，“首次射击成功率”—— 也就是射手第一发命中目标的成功率 —— 显著地提升了。在我抗议非得在 TrackingPoint 购买 (更贵) 的弹药，而无法使用自己买的弹药时，TrackingPoint 代表特别提到这一点。TrackingPoint 辩解说，大幅提高的命中率意味捕获相同数量猎物所需的弹药更少了，所以最终还是省钱了。
![](http://ww1.sinaimg.cn/large/7cc829d3gw1f05q5gy42wj20zk09zdie.jpg)
我很自然地问了这玩意儿潜在的军事用途，但 TrackingPoint 立马撇清他们与国防部的关系。PGF 系统的“对接射击”目标其实和美军时断时续的「陆上勇士」计划不谋而合。然而政府合同和采购的特点决定了所有军事目的的研发技术，必然得经历冗长而又曲折得可怕的开发流程，期间还不得不去迎合那些变化无常，甚至过时的设计目标。TrackingPoint 说他们的目标是先发展技术，等到技术成熟时再考虑市场和应用 —— 这就是他们已经做到的事情。
这家公司也意识到了当今关于关于武器和武器生产，弥漫在公众中的潜在负面情绪，尤其最近在 [Sandy Hook](https://en.wikipedia.org/wiki/Sandy_Hook_Elementary_School_shooting) 和 [Aurora](https://en.wikipedia.org/wiki/2012_Aurora_shooting) 等地发生大规模的枪击事件之后。然而 PGF 系列的三支枪型都是栓动步枪，对近距离射击来说过于笨重；而瞄准系统本身需要耐心和细心来操作开火，所以大规模枪击案的枪手不会选择这样的武器。况且 TrackingPoint 表示他们目前没有为栓动步枪之外的枪型加装 PGF 系统的计划。
狩猎是一项充满争议的消遣活动，但不可否认也是相当流行的活动，TrackingPoint 在这个目标市场干得很不错。价格相对来说昂贵 ——步枪的起步价是 17,000 美元 （包含一台预装 TrackingPoint app 的 iPad）， 但是对一把猎枪加一套精密光学系统而言，花费也不算太多。
### 图片汇总
伯小乐童鞋从 Arstechnica 其他几篇有关 TrackingPoint 的报道中，收集了一些图片和视频。
![](http://ww3.sinaimg.cn/large/7cc829d3gw1f05q5kdrfjj20hs0buabu.jpg)
（1000 码之外的测试）
![](http://ww4.sinaimg.cn/mw690/7cc829d3gw1f05r05m3u9j21900u0k38.jpg)
![](http://ww4.sinaimg.cn/mw690/7cc829d3gw1f05qxnpkj3j21900u0k5u.jpg)
![](http://ww1.sinaimg.cn/mw690/7cc829d3gw1f05r04iytvj21900u0ng8.jpg)
### 试射视频
（.338 Lapua Magnum XS1 步枪试射 1008 码（922米）之外的目标，试射员是第一次用枪）
（.300 WinMag XS2 步枪试射 750 码（685 米）之外的目标）
### 【更新】：智能步枪实战打猎
> 
「极客范」聚焦极客主题，分享各种新、奇、酷的东西。包括：前沿与新兴科技应用，树莓派、Arduino等微型电脑 DIY 应用。
搜索微信号：**iGeekFan  **或扫描下面的二维码关注极客范
![](http://ww3.sinaimg.cn/mw690/bfdcef89gw1eqch9eef42j203w03v74e.jpg)
