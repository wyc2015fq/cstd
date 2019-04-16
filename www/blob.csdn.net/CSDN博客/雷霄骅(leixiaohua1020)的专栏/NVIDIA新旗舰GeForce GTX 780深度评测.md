# NVIDIA新旗舰GeForce GTX 780深度评测 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年09月25日 12:24:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：32
个人分类：[硬件](https://blog.csdn.net/leixiaohua1020/article/category/1646633)










2013年5月18日，NVIDIA在2013年游戏群英会上出人意料地以不公开产品型号的方式向大众展示了一款神秘显卡。尽管只是惊鸿一瞥，但是大家都相信这就是即将到来的GeForce GTX 780显卡。果不其然，在稍后的5月24日，NVIDIA正式发布了这款产品。GeForce GTX 780，NVIDIA新一代GTX 700系列的单核心旗舰显卡，其性能究竟如何呢？

[![NVIDIA新旗舰GeForce GTX 780深度评测](http://www.mcplive.cn/images/201307/20130726150444_67636.jpg)](http://www.mcplive.cn/index.php/article/index/id/index.php?controller=article&action=showpic&id=13218&pic=/images/201307/b_20130726150444_67636.jpg)

#### GTX 780规格解读

或许NVIDIA认为GTX 780性能已经很出色，对保密不甚上心，因此其规格早在发布之前就已经被爆料得差不多了。本刊也早在4月下《GeForce GTX 780？ GK110神秘显卡抢先曝光》一文中曝光了该显卡。

下面，我们来看看GTX 780的详细规格。它和GTX Titan一样，都使用了基于“big Kepler”的GK110核心，这颗核心在本刊3月下评测GTX Titan的时候已经有详细介绍。完整版GK110的晶体管数量有71亿个，具备15组SMX、2880个CUDA core、384bit显存位宽、48个ROP单元。不过GT X Titan屏蔽了一组SMX，因此只有2688个CUDA Core。

GTX 780则屏蔽了三组SMX，具备12组SMX，流处理器数量降低到了2304个—这也是目前看到的第4款采用GK110核心的产品。之前已经分别有GTX Titan和针对行业用户的Tesla K20X、TeslaK 20。GTX 780的核心代号为“GK110-300-A1”，相比之下GTX Titan的核心代号为“GK110-400-A1”。核心频率方面，GTX 780由于削减了流处理器数量，因此需要提高频率加强性能。最终NVIDIA将它的核心频率设定为863MHz，高于GTX Titan的837MHz，GPU Boost加速频率也达到了902MHz，高于GTX Titan的876MHz。

显存规格方面，GeForce GTX 780位宽为384bit，显存搭配6GHz GDDR5颗粒，显存频率、显存带宽分别为6008MHz和288.4GB/s，和GTX Titan保持一致，远远超过了GTX 680的192.3GB/s，和Radeon HD 7970 GHz显存位宽基本相当。不过在显存容量方面，GTX 780搭配的3GB容量则远逊色于GTX Titan的6GB。显存容量方面的差异，堪称GTX 780和GTX Titan在规格上的最大不同。

#### GTX 780的双精度能力、价格、定位

很多用户难以区分GTX 780和GTX Titan的定位，事实上，GTX 780才是NVIDIA认定的GTX 700系列的首款显卡。虽然从核心上来看，这两款产品都使用了GK110核心，但是它们的市场意义是完全不同的。GTX Titan作为NVIDIA显卡产品档次中的顶级产品系列，不属于任何产品线，它以高昂的价格和强大的性能、功能专门为顶级玩家设计，因此NVIDIA将它独立出来作为单独的一个顶级型号。GTX 780则不同，这款再次阉割的GK110核心（实际上普通玩家完全不需要关注阉割与否）的目的是引领全新的GTX 700系列显卡，为NVIDIA在2013年带来市场的全新活力。

由于GeForce GTX 780的核心依旧采用GK110，因此没有太多新的技术内容。不过由于GeForce GTX 780面向游戏玩家，因此双精度计算功能不再有用，“惨遭”屏蔽，GeForce GTX 780已经无法像GTX Titan那样提供强悍的双精度性能了。其余的一些技术诸如Dynamic Parallelism、Hyper-Q等技术，则继续保留。

GeForce GTX 780的另一个亮点在于GPU Boost 2.0，虽然这个技术在之前的GTX Titan上已经开始使用，不过相比之下，GeForce GT X 780的频率会提升得更高，在部分游戏中Boost频率往往会运行在1006MHz的高位上。本刊5月上《GPU Boost 2.0解密 GeForce GTX Titan另类超频测试》一文也详细对GPU Boost 2.0进行了测试。

在价格方面，GeForce GTX 780的售价相比GTX Titan显得颇为亲民，4799元的官方报价相比GTX Titan的7999元只有后者的约6 0%，但是性能达到了后者的80%以上。当然，这里的价格亲民也是相对的，其接近5000元的售价虽然和旗舰级身份相符合，但注定和绝大多数玩家无缘。

[![GTX 780的GPU-Z信息](http://www.mcplive.cn/images/201307/20130726150617_25206.jpg)](http://www.mcplive.cn/index.php/article/index/id/index.php?controller=article&action=showpic&id=13218&pic=/images/201307/b_20130726150617_25206.jpg)
 GTX 780的GPU-Z信息

[![GTX 780规格一览](http://www.mcplive.cn/images/201307/20130726150712_58268.png)](http://www.mcplive.cn/index.php/article/index/id/index.php?controller=article&action=showpic&id=13218&pic=/images/201307/b_20130726150712_58268.png)

#### GTX 780 拆解示意图

GTX 780无论是外观还是PCB等设计都和GTX Titan完全相同，PCB长度都为26.67cm，PCB层数为10层，工艺上使用了2倍铜来降低大电流下的电阻，同时也可以降低温度。其散热器设计和上一代GTX 690的“理念”是完全相同的。材质上采用表面电镀的镁铝合金材质，并附之以透明的聚碳酸酯。在靠近显卡挡板的部分，以印字方式刻印了GTX 780产品型号。另外，显卡侧面有绿色的“GEFOCE GTX”灯，显卡在运行时会亮起，非常好看。

[![PWM芯片方面采用了安森美的NCP4206，也是使用了独立PCB的形式，然后焊接在显卡PCB上。](http://www.mcplive.cn/images/201307/20130726151612_28347.jpg)](http://www.mcplive.cn/index.php/article/index/id/index.php?controller=article&action=showpic&id=13218&pic=/images/201307/b_20130726151612_28347.jpg)
 PWM芯片方面采用了安森美的NCP4206，也是使用了独立PCB的形式，然后焊接在显卡PCB上。

[![■ 采用6相核心供电、2相显存供电设计](http://www.mcplive.cn/images/201307/20130726151735_79699.jpg)](http://www.mcplive.cn/index.php/article/index/id/index.php?controller=article&action=showpic&id=13218&pic=/images/201307/b_20130726151735_79699.jpg)
 采用6相核心供电、2相显存供电设计

[![外接供电电源接口采用了8Pin和6Pin的组合](http://www.mcplive.cn/images/201307/20130726151837_90418.jpg)](http://www.mcplive.cn/index.php/article/index/id/index.php?controller=article&action=showpic&id=13218&pic=/images/201307/b_20130726151837_90418.jpg)
 外接供电电源接口采用了8Pin和6Pin的组合

[![散热器使用了热均腔板技术](http://www.mcplive.cn/images/201307/20130726151927_89834.jpg)](http://www.mcplive.cn/index.php/article/index/id/index.php?controller=article&action=showpic&id=13218&pic=/images/201307/b_20130726151927_89834.jpg)
 散热器使用了热均腔板技术

[![■ GTX ■ 接口为双DVI、HDMI、DisplayPort 780的核心编号为GK110-300-A1](http://www.mcplive.cn/images/201307/20130726152051_55070.jpg)](http://www.mcplive.cn/index.php/article/index/id/index.php?controller=article&action=showpic&id=13218&pic=/images/201307/b_20130726152051_55070.jpg)

■ GTX ■ 接口为双DVI、HDMI、DisplayPort 780的核心编号为GK110-300-A1

[![接口为双DVI、HDMI、DisplayPort](http://www.mcplive.cn/images/201307/20130726152146_32792.jpg)](http://www.mcplive.cn/index.php/article/index/id/index.php?controller=article&action=showpic&id=13218&pic=/images/201307/b_20130726152146_32792.jpg)
 接口为双DVI、HDMI、DisplayPort

[![NVIDIA新旗舰GeForce GTX 780深度评测](http://www.mcplive.cn/images/201307/20130726152309_83137.jpg)](http://www.mcplive.cn/index.php/article/index/id/index.php?controller=article&action=showpic&id=13218&pic=/images/201307/b_20130726152309_83137.jpg)
 NVIDIA新旗舰GeForce GTX 780深度评测



#### GTX 780全面测试

和GTX Titan同出一脉的GTX 780，在实际游戏中会有怎样的表现呢？我们梳理了和GTX 780有关的竞品，列出了几个用户最为关注的GTX 780的测试看点:

GTX 780 Vs. GTX Titan，缩减了核心规格的GTX 780的性能和GTX Titan有多大的性能差距？高频版的GTX 780能否比肩GTX Titan？

GTX 780 Vs.GTX 680，同为单核心旗舰产品，新一代GTX 780的性能能够领先上一代的GTX 680多少？

GTX 780 SLI的性能怎样？相对于GTX 780有多少性能提升？

GTX 780的发热量、温度高吗？

接下来我们将在基于英特尔Core i7 3960X处理器的顶级平台上对GTX 780进行测试。从GTX 780的定位考虑，我们除了在传统的1920×1080全高清分辨率对其测试以外，还会加入更高的2560×1440分辨率，以及4倍或8倍MSAA抗锯齿测试。

我们选取了6款游戏大作对GTX 780进行测试，这其中既有当下号称“显卡杀手”之称的《孤岛危机3》、《孤岛惊魂3》，也有深受玩家喜爱的《使命召唤:黑色行动2》（这款游戏虽然不支持DirectX 11，但目前相当火爆，因此加入测试），还有同样能深度压榨显卡性能的老牌DirectX 11游戏《失落的星球2》。此外，我们还会使用3DMark系列测试软件、Unigine HeavenBenchmark 4.0来考察GTX 780的理论图形性能。

GTX 780 Vs. GTX Titan : 落后幅度在15%左右

GTX 780相比GTX Titan最大的劣势在于流处理器数量、纹理单元数量稍微偏少，以及显存容量更低。但是双方在光栅单元数量、显存位宽、显存带宽等影响显卡的规格参数上又保持了一致，且GTX 780的核心频率又稍高于GTX Titan，在一定程度上弥补了规格的不足。规格上的差异，导致双方的性能差距达到了15%左右。

在理论测试中，GTX Titan在3DMark 11 Extreme、新3DMark Fire Strike Extreme、Unigine Heaven Benchmark4.0中的性能分别领先GTX 780 13%、10%和16%左右。在实际游戏测试中，GTX Titan的领先优势更为明显一点，例如在《孤岛危机3》的1920×1080、最高画质设置以及2560×1440、最高画质设置下，GTX Titan分别领先GTX 780 22%和23%左右。当然，在《孤岛惊魂3》这种发布不久的游戏中，NVIDIA针对它做了大量的优化，GTX 780在这款游戏中的性能与GTX Titan已经非常接近了。

此外，我们加入了一块高频版GTX 780（七彩虹GTX 780OC-3GD5）的测试，其核心频率达到了967MHz，大大超过了公版产品。其综合性能领先公版GTX 780大约7%，例如在《孤岛惊魂3》、《孤岛危机3》的1920×1080、最高画质设置下，高频版GTX 780分别领先公版7%和9%。而和GTX Titan相比，高频版GTX 780的性能差距已经缩减到8%左右，甚至在新3DMark FireStrike Extreme和《孤岛惊魂3》的测试中，双方已经不相上下了。

GTX 780 Vs. GTX 680:领先幅度并不大

双方同为单核心旗舰定位，GTX 780领衔GTX 700系列，GTX 680则是GTX 600系列的大哥。GTX 680是第一代Kepler核心，核心规格明显不如采用第二代Kepler核心的GTX 780。以流处理器数量为例，GTX 780具备了2304个，而GTX 680只有1536个，GTX 780仅在流处理器一项规格上就领先GTX 680高达50%。至于其他的显存容量、显存带宽、纹理单元数量等方面GTX 780也远超过GTX 680。图形理论测试软件反映出双方在规格上的差距，GTX 780分别在3DMark 11 Extreme、新3DMark Fire Strike Extreme、Unigine Heaven Benchmark4.0中领先GTX 680 30%、36%、43%。

但从实际游戏测试结果来看，双方的性能差距显然不如理论测试结果那样高，GTX 780只领先GTX 680 15%左右。例如在《失落的星球2》、《孤岛惊魂3》、《孤岛危机3》1920×1080、最高画质设置下，GTX 780分别只领先GTX 6807%、19%、9%左右。只是在2560×1440、最高画质设置下，GTX 780的高显存位宽、大容量显存的优势开始显现，领先GTX 680的幅度才分别达到了14%、23%、15%左右。

我们推测，GTX 780较GTX 680优势不明显的主要原因还是因为现阶段的驱动程序优化不到位，没有将GTX 780的最大性能发挥出来。GTX 780的游戏性能改善只能寄希望于后续驱动程序的更新了。

GTX 780 SLI:中规中矩

在理论测试中，GTX 780 SLI充分挥发了它的性能，在几款理论图形测试软件中都发挥出它的实力，性能较单卡有差不多100%的性能提升。不过在实际游戏中，GTX 780 SLI的优势开始发生变化。首先是在1920×1080、最高画质设置下，GT X780 SLI较单卡的优势开始缩小，例如在《失落的星球2》、《孤岛惊魂3》、《孤岛危机3》中，GTX 780 SLI分别只领先GTX 78012%、16%、50%。而在2560×1440、4MSAA、最高画质设置下，GTX 780 SLI的优势则分别达到了40%、87%、96%，大容量显存的优势开始凸显。

虽然GTX 780 SLI较GTX 780单卡有较为明显的优势，但正如前文所说，现阶段的驱动程序对GTX 780优化并不到位，这导致GTX 780 SLI相比上一代定位相同、同为SLI系统的GTX 690HD 7990优势很小，总体保持在10%以内，甚至在《战地3》中的性能还不如HD 7990。总体而言，目前GTX 780 SLI的性能表现只能说一般，其最佳设置是2560×1440（4MSAA）、最高画质。

GTX 780温度、功耗:主流水准（二级标题）

借助22nm工艺，GTX 780的功耗、发热量还是控制得不错，其待机温度、满载温度分别为36℃、84℃。这样的温度表现和GTX Titan相仿。其所在系统的待机系统功耗、满载系统功耗分别为83W、385W，有趣的是GTX Titan的待机系统功耗、满载系统功耗分别为83W、345W，即GTX 780的满载系统功耗比定位更高的GTX Titan高出40W。出现这样的结果，最大的原因很可能是GTX 780的GPU Boost功耗控制不如GTX Titan所导致的。

[![GTX 780全面测试](http://www.mcplive.cn/images/201307/20130726152518_67942.jpg)](http://www.mcplive.cn/index.php/article/index/id/13218/page/index.php?controller=article&action=showpic&id=13218&pic=/images/201307/b_20130726152518_67942.jpg)

##### 主要测试平台

处理器 英特尔Core i7 3960X

主板 华擎X79

内存 DDR3 1600 4GB×4

电源 X7-1200W

操作系统 Windows7 Ultimate 64bit

[![图1：高频GTX 780的性能大大超过公版GTX 780，而GTX 780 SLI在理论图形测试中也取得了很好的成绩。](http://www.mcplive.cn/images/201307/20130726152627_15569.jpg)](http://www.mcplive.cn/index.php/article/index/id/13218/page/index.php?controller=article&action=showpic&id=13218&pic=/images/201307/b_20130726152627_15569.jpg)

[![图1：高频GTX 780的性能大大超过公版GTX 780，而GTX 780 SLI在理论图形测试中也取得了很好的成绩。](http://www.mcplive.cn/images/201307/20130726152712_57318.jpg)](http://www.mcplive.cn/index.php/article/index/id/13218/page/index.php?controller=article&action=showpic&id=13218&pic=/images/201307/b_20130726152712_57318.jpg)
 图1：高频GTX 780的性能大大超过公版GTX 780，而GTX 780 SLI在理论图形测试中也取得了很好的成绩。

[![图2：GTX 780的满载温度为84℃](http://www.mcplive.cn/images/201307/20130726152900_40358.jpg)](http://www.mcplive.cn/index.php/article/index/id/13218/page/index.php?controller=article&action=showpic&id=13218&pic=/images/201307/b_20130726152900_40358.jpg)
 图2：GTX 780的满载温度为84℃

#### GTX 780，距离王者尚有距离

GTX 780和GTX Titan都采用相同的顶级GK110核心，拥有GTX Titan 85%左右的性能，但价格只有GTX Titan的59%。同时，它的温度、功耗控制也还不错。从横向来看，GTX 780无疑是很棒的，为整个GTX 700系列开了一个好头。但从纵向来看，GTX 780的优势立刻变得不明显起来。它拥有远超上一代定位相同的GTX 680的规格，但最终的游戏性能却只领先后者15%左右。不管造成这种结果的原因是因为驱动优化不够好还是其他因素，都难以掩盖现阶段GTX 780的问题:效率不够高，距离真正的王者尚有距离。GTX 780 SLI的测试结果也很好地说明了这个问题，它较上一代的GTX 690、HD 7990并没有表现出绝对的领先水准，甚至在一些游戏中还不如后者。

[![NVIDIA新旗舰GeForce GTX 780深度评测](http://www.mcplive.cn/images/201307/20130726145931_70878.png)](http://www.mcplive.cn/index.php/article/index/id/13218/page/index.php?controller=article&action=showpic&id=13218&pic=/images/201307/b_20130726145931_70878.png)










