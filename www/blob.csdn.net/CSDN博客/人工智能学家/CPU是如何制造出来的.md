# CPU是如何制造出来的 - 人工智能学家 - CSDN博客
2018年05月11日 08:10:07[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：502
![640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1&retryload=1](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBU10N14iajPFGRGxWMn7iaznkeZCyxgUrqohwgVxib80QvZsgaCnUVUzQKfx0yC5TnBUv02IwmXbxyicw/640?wx_fmt=jpeg&wxfrom=5&wx_lazy=1&retryload=1)
来源：EDN电子技术设计
摘要：Intelx86架构已经经历了二十多个年头，而x86架构的CPU对我们大多数人的工作、生活影响颇为深远。
CPU是现代计算机的核心部件，又称为“微处理器”。对于PC而言，CPU的规格与频率常常被用来作为衡量一台电脑性能强弱重要指标。
Intelx86架构已经经历了二十多个年头，而x86架构的CPU对我们大多数人的工作、生活影响颇为深远。
许多对电脑知识略知一二的朋友大多会知道CPU里面最重要的东西就是晶体管了，提高CPU的速度，最重要的一点说白了就是如何在相同的CPU面积里面放进去更加多的晶体管，由于CPU实在太小，太精密，里面组成了数目相当多的晶体管，所以人手是绝对不可能完成的，只能够通过光刻工艺来进行加工的。
这就是为什么一块CPU里面为什么可以数量如此之多的晶体管。晶体管其实就是一个双位的开关：即开和关。如果您回忆起基本计算的时代，那就是一台计算机需要进行工作的全部。两种选择，开和关，对于机器来说即0和1。那么您将如何制作一个CPU呢？在今天的文章中，我们将一步一步的为您讲述中央处理器从一堆沙子到一个功能强大的集成电路芯片的全过程。
**制造CPU的基本原料**
如果问及CPU的原料是什么，大家都会轻而易举的给出答案—是硅。这是不假，但硅又来自哪里呢？其实就是那些最不起眼的沙子。难以想象吧，价格昂贵，结构复杂，功能强大，充满着神秘感的CPU竟然来自那根本一文不值的沙子。当然这中间必然要经历一个复杂的制造过程才行。不过不是随便抓一把沙子就可以做原料的，一定要精挑细选，从中提取出最最纯净的硅原料才行。试想一下，如果用那最最廉价而又储量充足的原料做成CPU，那么成品的质量会怎样，你还能用上像现在这样高性能的处理器吗？
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBU10N14iajPFGRGxWMn7iaznkXWIgC28rVuvcdqWpDxDAhlibtBMrnKAbP8II6majZ6RDhc9sHHq8orA/640?wx_fmt=png)
除去硅之外，制造CPU还需要一种重要的材料就是金属。目前为止，铝已经成为制作处理器内部配件的主要金属材料，而铜则逐渐被淘汰，这是有一些原因的，在目前的CPU工作电压下，铝的电迁移特性要明显好于铜。所谓电迁移问题，就是指当大量电子流过一段导体时，导体物质原子受电子撞击而离开原有位置，留下空位，空位过多则会导致导体连线断开，而离开原位的原子停留在其它位置，会造成其它地方的短路从而影响芯片的逻辑功能，进而导致芯片无法使用。
除了这两样主要的材料之外，在芯片的设计过程中还需要一些种类的化学原料，它们起着不同的作用，这里不再赘述。
**CPU制造的准备阶段**
在必备原材料的采集工作完毕之后，这些原材料中的一部分需要进行一些预处理工作。而作为最主要的原料，硅的处理工作至关重要。首先，硅原料要进行化学提纯，这一步骤使其达到可供半导体工业使用的原料级别。而为了使这些硅原料能够满足集成电路制造的加工需要，还必须将其整形，这一步是通过溶化硅原料，然后将液态硅注入大型高温石英容器而完成的。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBU10N14iajPFGRGxWMn7iaznknFubTWZqqYwTLPUCKV8nMnmjI1Moqx7u7tHCeyY47WM9g5fsUiawFkQ/640?wx_fmt=png)
而后，将原料进行高温溶化。中学化学课上我们学到过，许多固体内部原子是晶体结构，硅也是如此。为了达到高性能处理器的要求，整块硅原料必须高度纯净，及单晶硅。然后从高温容器中采用旋转拉伸的方式将硅原料取出，此时一个圆柱体的硅锭就产生了。从目前所使用的工艺来看，硅锭圆形横截面的直径为200毫米。
不过现在intel和其它一些公司已经开始使用300毫米直径的硅锭了。在保留硅锭的各种特性不变的情况下增加横截面的面积是具有相当的难度的，不过只要企业肯投入大批资金来研究，还是可以实现的。intel为研制和生产300毫米硅锭而建立的工厂耗费了大约35亿美元，新技术的成功使得intel可以制造复杂程度更高，功能更强大的集成电路芯片。而200毫米硅锭的工厂也耗费了15亿美元。
在制成硅锭并确保其是一个绝对的圆柱体之后，下一个步骤就是将这个圆柱体硅锭切片，切片越薄，用料越省，自然可以生产的处理器芯片就更多。切片还要镜面精加工的处理来确保表面绝对光滑，之后检查是否有扭曲或其它问题。这一步的质量检验尤为重要，它直接决定了成品CPU的质量。
新的切片中要掺入一些物质而使之成为真正的半导体材料，而后在其上刻划代表着各种逻辑功能的晶体管电路。掺入的物质原子进入硅原子之间的空隙，彼此之间发生原子力的作用，从而使得硅原料具有半导体的特性。今天的半导体制造多选择CMOS工艺（互补型金属氧化物半导体）。
其中互补一词表示半导体中N型MOS管和P型MOS管之间的交互作用。而N和P在电子工艺中分别代表负极和正极。多数情况下，切片被掺入化学物质而形成P型衬底，在其上刻划的逻辑电路要遵循nMOS电路的特性来设计，这种类型的晶体管空间利用率更高也更加节能。同时在多数情况下，必须尽量限制pMOS型晶体管的出现，因为在制造过程的后期，需要将N型材料植入P型衬底当中，而这一过程会导致pMOS管的形成。
在掺入化学物质的工作完成之后，标准的切片就完成了。然后将每一个切片放入高温炉中加热，通过控制加温时间而使得切片表面生成一层二氧化硅膜。通过密切监测温度，空气成分和加温时间，该二氧化硅层的厚度是可以控制的。
在intel的90纳米制造工艺中，门氧化物的宽度小到了惊人的5个原子厚度。这一层门电路也是晶体管门电路的一部分，晶体管门电路的作用是控制其间电子的流动，通过对门电压的控制，电子的流动被严格控制，而不论输入输出端口电压的大小。
准备工作的最后一道工序是在二氧化硅层上覆盖一个感光层。这一层物质用于同一层中的其它控制应用。这层物质在干燥时具有很好的感光效果，而且在光刻蚀过程结束之后，能够通过化学方法将其溶解并除去。
**光刻蚀**
这是目前的CPU制造过程当中工艺非常复杂的一个步骤，为什么这么说呢？光刻蚀过程就是使用一定波长的光在感光层中刻出相应的刻痕， 由此改变该处材料的化学特性。这项技术对于所用光的波长要求极为严格，需要使用短波长的紫外线和大曲率的透镜。刻蚀过程还会受到晶圆上的污点的影响。每一步刻蚀都是一个复杂而精细的过程。
设计每一步过程的所需要的数据量都可以用10GB的单位来计量，而且制造每块处理器所需要的刻蚀步骤都超过20步（每一步进行一层刻蚀）。而且每一层刻蚀的图纸如果放大许多倍的话，可以和整个纽约市外加郊区范围的地图相比，甚至还要复杂，试想一下，把整个纽约地图缩小到实际面积大小只有100个平方毫米的芯片上，那么这个芯片的结构有多么复杂，可想而知了吧。
当这些刻蚀工作全部完成之后，晶圆被翻转过来。短波长光线透过石英模板上镂空的刻痕照射到晶圆的感光层上，然后撤掉光线和模板。通过化学方法除去暴露在外边的感光层物质，而二氧化硅马上在陋空位置的下方生成。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBU10N14iajPFGRGxWMn7iaznk19fhy7rVT4c0YDw78ia6Rphr0XvqTWPMacSlAwPeMoAkrmz7ZZiaheXg/640?wx_fmt=png)
**掺杂**
在残留的感光层物质被去除之后，剩下的就是充满的沟壑的二氧化硅层以及暴露出来的在该层下方的硅层。这一步之后，另一个二氧化硅层制作完成。然后，加入另一个带有感光层的多晶硅层。多晶硅是门电路的另一种类型。由于此处使用到了金属原料（因此称作金属氧化物半导体），多晶硅允许在晶体管队列端口电压起作用之前建立门电路。感光层同时还要被短波长光线透过掩模刻蚀。再经过一部刻蚀，所需的全部门电路就已经基本成型了。然后，要对暴露在外的硅层通过化学方式进行离子轰击，此处的目的是生成N沟道或P沟道。这个掺杂过程创建了全部的晶体管及彼此间的电路连接，没个晶体管都有输入端和输出端，两端之间被称作端口。
**重复这一过程**
从这一步起，你将持续添加层级，加入一个二氧化硅层，然后光刻一次。重复这些步骤，然后就出现了一个多层立体架构，这就是你目前使用的处理器的萌芽状态了。在每层之间采用金属涂膜的技术进行层间的导电连接。今天的P4处理器采用了7层金属连接，而Athlon64使用了9层，所使用的层数取决于最初的版图设计，并不直接代表着最终产品的性能差异。
**测试 封装测试过程**
接下来的几个星期就需要对晶圆进行一关接一关的测试，包括检测晶圆的电学特性，看是否有逻辑错误，如果有，是在哪一层出现的等等。而后，晶圆上每一个出现问题的芯片单元将被单独测试来确定该芯片有否特殊加工需要。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBU10N14iajPFGRGxWMn7iaznkS2BfABr239qSr7u2BqZjIY3JibnEHlGdDEtiarIGuedqZibTuCSqNgIVw/640?wx_fmt=png)
而后，整片的晶圆被切割成一个个独立的处理器芯片单元。在最初测试中，那些检测不合格的单元将被遗弃。这些被切割下来的芯片单元将被采用某种方式进行封装，这样它就可以顺利的插入某种接口规格的主板了。大多数intel和AMD的处理器都会被覆盖一个散热层。
在处理器成品完成之后，还要进行全方位的芯片功能检测。这一部会产生不同等级的产品，一些芯片的运行频率相对较高，于是打上高频率产品的名称和编号，而那些运行频率相对较低的芯片则加以改造，打上其它的低频率型号。这就是不同市场定位的处理器。而还有一些处理器可能在芯片功能上有一些不足之处。比如它在缓存功能上有缺陷（这种缺陷足以导致绝大多数的CPU瘫痪），那么它们就会被屏蔽掉一些缓存容量，降低了性能，当然也就降低了产品的售价，这就是Celeron和Sempron的由来。
当CPU被放进包装盒之前，一般还要进行最后一次测试，以确保之前的工作准确无误。根据前面确定的最高运行频率不同，它们被放进不同的包装，销 往世界各地。
读完这些，相信你已经对CPU的制造流程有了一些比较深入的认识。CPU的制造，可以说是集多方面尖端科学技术之大成，CPU本身也就那么点 大，如果 把里面的材料分开拿出来卖，恐怕卖不了几个钱。然而CPU的制造成本是非常惊人的，从这里或许我们可以理解，为什么这东西卖这么贵了。
在测试这个环节很重要，比如你的处理器是6300还是6400就会在这个环节被划分，而 6300天生并不是6300，而是在测试之后，发现处理器不能稳定的在6400标准下工作，只能在6300标准下稳定工作，于是对处理器定义，锁频，定义 ID，封装，印上6300。
我们用AMD的来举例：同样核心的处理器都是一个生产线下来的，如果稳定工作在2.8GHz，1M*2的缓 存下，就被定义为5600+，如果缓存有瑕疵，切割有问题的那一半，成为5400+，如果缓存没问题而频率只能在2.6G通过测试，那么就是5200+， 如果缓存有瑕疵，就切割成为5000+…………一直把它测到3800+，如果还不稳定，要么想办法变成速龙64单核或者单核闪龙，或者就是出现过的ES版 的双核闪龙，如果出现批量不能工作在3800+条件下，而工作在3600+条件下，那么3600+就上市了，如果出现批量能工作在3G，1M*2条件下， 那么6000+就上市了，这就是为什么处理器总是中等型号的先上市，高端和底端的后上市，当然后期工厂可能会节约成本专门开出底端的流水线，专门生产底端 处理器，赛扬，闪龙的各种型号就相继上市，而高端的流水线因为个别处理器不稳定转变为底端处理器，例如将速龙64缓存切割就变为闪龙64。
**intel Core i7生产全过程图解**
沙子：硅是地壳内第二丰富的元素，而脱氧后的沙子(尤其是石英)最多包含25％的硅元素，以二氧化硅(SiO2)的形式存在，这也是半导体制造产业的基础。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBU10N14iajPFGRGxWMn7iaznk7V8I3icpsMyxnEXg6BesPIfVod7iczrrfdGPaalcfCwia5gc892TicTcicQ/640?wx_fmt=png)
硅熔炼：12英寸/300毫米晶圆级，下同。通过多步净化得到可用于半导体制造质量的硅，学名电子级硅(EGS)，平均每一百万个硅原子中最多只有一个杂 质原子。此图展示了是如何通过硅净化熔炼得到大晶体的，最后得到的就是硅锭(Ingot)。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBU10N14iajPFGRGxWMn7iaznkwSDagS0zM97ooO2icDzmSViaIgodFKEr8y7n0TwLZ4RicPJAQGm4PDicQg/640?wx_fmt=png)
单晶硅锭：整体基本呈圆柱形，重约100千克，硅纯度 99.9999％。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBU10N14iajPFGRGxWMn7iaznkjWicuFL6TasAQQCevw4CvhhibA0oR6UHVicsLdQruOVjzibsNKT5kl7UFg/640?wx_fmt=png)
硅锭切割：横向切割成圆形的单个硅片，也就是我们常说的晶圆 (Wafer)。顺便说，这下知道为什么晶圆都是圆形的了吧？
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBU10N14iajPFGRGxWMn7iaznkIN7MffaNzzdJgDezwZErPKVeiaqJ1lK5c2pdRFye3hUcn0HkT6uFTdw/640?wx_fmt=png)
晶圆：切割出的晶圆经过抛光后变得几乎完美无瑕，表面甚至可以当镜子。事实上，Intel自己并不生产这种晶圆，而是从第三方半导体企业那里直接购买成 品，然后利用自己的生产线进一步加工，比如现在主流的45nm HKMG(高K金属栅极)。值得一提的是，Intel公司创立之初使用的晶圆尺寸只有2英寸/50毫米
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBU10N14iajPFGRGxWMn7iaznkxc6noJy3TAiczF8I30UvCuleF77eEVTvEnFCXyt9e73vvjCxIbiac5Jw/640?wx_fmt=png)
光刻胶(Photo Resist)：图中蓝色部分就是在晶圆旋转过程中浇上去的光刻胶液体，类似制作传统胶片的那种。晶圆旋转可以让光刻胶铺的非常薄、非常平。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBU10N14iajPFGRGxWMn7iaznkuAiaoQSmVSsy2HnsCLCstS1GtBgWkuoEc9vqeK8eQ3LS86JV6HoK13A/640?wx_fmt=png)
光刻：光刻胶层随后透过掩模(Mask)被曝光在紫外线(UV)之下，变得可溶，期间发生的化学反应类似按下机械相机快门那一刻胶片的变化。掩模上印着预 先设计好的电路图案，紫外线透过它照在光刻胶层上，就会形成微处理器的每一层电路图案。一般来说，在晶圆上得到的电路图案是掩模上图案的四分之一。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBU10N14iajPFGRGxWMn7iaznk9A5TDXRIIIbmDmndP2YQibog4GkicRUoYOaEycYF0P7Vky59qPhBibSGQ/640?wx_fmt=png)
光刻：由此进入50-200纳米尺寸的晶体管级别。一块晶圆上可以切割出数百个处理器，不过从这里开始把视野缩小到其中一个上，展示如何制作晶体管等部 件。晶体管相当于开关，控制着电流的方向。现在的晶体管已经如此之小，一个针头上就能放下大约3000万个。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBU10N14iajPFGRGxWMn7iaznkfvVecU6ratAqR1zEyK2XNicUZW1wBvmv9AJDW0SJP7WXRALpC7kZjibA/640?wx_fmt=png)
溶解光刻胶：光刻过程中曝光在紫外线下的光刻胶被溶解掉，清除后留下的图案和掩模上的一致
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBU10N14iajPFGRGxWMn7iaznkRKQcjKHfD6fkLopG3Ar8ibTpK2KApn3RxgALMNDvh1QwicazX3pKsIbw/640?wx_fmt=png)
蚀刻：使用化学物质溶解掉暴露出来的晶圆部分，而剩下的光刻胶保护着不应该蚀刻的部分。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBU10N14iajPFGRGxWMn7iaznkG1micWguE3kvk3tHY0kWzfmMjqhGicqxibjm3N2EicicVFshQeEtwcaAY9A/640?wx_fmt=png)
清除光刻胶：蚀刻完成后，光刻胶的使命宣告完成，全部清除后就可以看到设计好的电路图案。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBU10N14iajPFGRGxWMn7iaznkTn8ggafcjvzAMkpwiaicE8Qgb2MBibR1iagf2yoMgiclz04kT42UuJp0F7g/640?wx_fmt=png)
光刻胶：再次浇上光刻胶(蓝色部分)，然后光刻，并洗掉曝光的部分，剩下的光刻胶还是用来保护不会离子注入的那部分材料。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBU10N14iajPFGRGxWMn7iaznkibYbHgs6VXK9QpMknqUXmFgIuxm4PsibFVNPkxBfNGL1NPfNP1OdHkfA/640?wx_fmt=png)
离子注入(Ion Implantation)：在真空系统中，用经过加速的、要掺杂的原子的离子照射(注入)固体材料，从而在被注入的区域形成特殊的注入层，并改变这些区 域的硅的导电性。经过电场加速后，注入的离子流的速度可以超过30万千米每小时。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBU10N14iajPFGRGxWMn7iaznkN6Ih7GtqIdph7CXf268BjuSXBAu5OdG5FDvDwzCyGica1eW8mBVNQBw/640?wx_fmt=png)
清除光刻胶：离子注入完成后，光刻胶也被清除，而注入区域(绿色部分)也已掺杂，注入了不同的原子。注意这时候的绿色和之前已经有所不同。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBU10N14iajPFGRGxWMn7iaznk8IBFYOsakFNsErJTLFx5WMx0xo46IMKF6dt6cI0pnUDiavHLZ9om8RA/640?wx_fmt=png)
晶体管就绪：至此，晶体管已经基本完成。在绝缘材(品红色)上蚀刻出三个孔洞，并填充铜，以便和其它晶体管互连。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBU10N14iajPFGRGxWMn7iaznkPnribjiahOkxh8JaNbLJc9UTEns5EWJyRKwxTaQjIjXWEdvtbiaIOts4g/640?wx_fmt=png)
电镀：在晶圆上电镀一层硫酸铜，将铜离子沉淀到晶体管上。铜离子会从正极(阳极)走向负极(阴极)。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBU10N14iajPFGRGxWMn7iaznkVzBljzm6Orfp9iaWVic30z6bkesOib51o4VR3xJm1ZeP0avo2BP9KWBVA/640?wx_fmt=png)
铜层：电镀完成后，铜离子沉积在晶圆表面，形成一个薄薄的铜层。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBU10N14iajPFGRGxWMn7iaznkGT9nlo1EeCeeBrPicbaJX8fpSxR6YgunBNGG6EnpQiciaDKncgdHPAgyw/640?wx_fmt=png)
抛光：将多余的铜抛光掉，也就是磨光晶圆表面。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBU10N14iajPFGRGxWMn7iaznk83yMY79wt6QiczwqDDhnTY0OB29Piat48qpibxOb4zhLZa06RDQXTNLIw/640?wx_fmt=png)
金属层：晶体管级别，六个晶体管的组合，大约500纳米。在不同晶体管之间形成复合互连金属层，具体布局取决于相应处理器所需要的不同功能性。芯片表面看 起来异常平滑，但事实上可能包含20多层复杂的电路，放大之后可以看到极其复杂的电路网络，形如未来派的多层高速公路系统。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBU10N14iajPFGRGxWMn7iaznkdlyTNJHP5K9kBNoAxA39D6Ph6lRdmsnF9lYrvQhj81QWlM4fvmdTTA/640?wx_fmt=png)
晶圆测试：内核级别，大约10毫米/0.5英寸。图中是晶圆的局部，正在接受第一次功能性测试，使用参考电路图案和每一块芯片进行对比。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBU10N14iajPFGRGxWMn7iaznkicBWl5DfE1Ojo4MS52X24mdNDC4u1JFQvMbY6R66tGoDROpljvqhrZw/640?wx_fmt=png)
晶圆切片(Slicing)：晶圆级别，300毫米/12英寸。将晶圆切割成块，每一块就是一个处理器的内核(Die)。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBU10N14iajPFGRGxWMn7iaznk6sibDCD7qnen36ibKhoibdstOQ0AkBXde9O0GvFcw6Pq81unc0FE7Rp8w/640?wx_fmt=png)
丢弃瑕疵内核：晶圆级别。测试过程中发现的有瑕疵的内核被抛弃，留下完好的准备进入下一步。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBU10N14iajPFGRGxWMn7iaznkxZn33Fic6E4icWDV0xQYprAU8gUBTriamOYDkwvXb28uUWybteelFVibLA/640?wx_fmt=png)
单个内核：内核级别。从晶圆上切割下来的单个内核，这里展示的是Core i7的核心。
封装：封装级别，20毫米/1英寸。衬底(基片)、内核、散热片堆叠在一起，就形成了我们看到的处理器的样子。衬底(绿色)相当于一个底座，并为处理器内 核提供电气与机械界面，便于与PC系统的其它部分交互。散热片(银色)就是负责内核散热的了。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBU10N14iajPFGRGxWMn7iaznkJ4O9sBwl8icIPd8RESiaw9mQ6Z1dibY0Kc4vE8S3HsDgP9femFaj1NU3g/640?wx_fmt=png)
等级测试：最后一次测试，可以鉴别出每一颗处理器的关键特性，比如最高频率、功耗、发热量等，并决定处理器的等级，比如适合做成最高端的Core i7-975 Extreme，还是低端型号Core i7-920。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBU10N14iajPFGRGxWMn7iaznkMVibyWNMjibicswYQueymQQ2TZw4DBkH32RxDXNr7hAxYDC3o42ia2w2LA/640?wx_fmt=png)
装箱：根据等级测试结果将同样级别的处理器放在一起装运。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBU10N14iajPFGRGxWMn7iaznkYMMjhQdC2RH3F1Lc0ra0fMuIr0umQZEeZWUVWK3V1sUSLvcOb3y4jA/640?wx_fmt=png)
零售包装：制造、测试完毕的处理器要么批量交付给OEM厂商，要么放在包装盒里进入零售市场。
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)
