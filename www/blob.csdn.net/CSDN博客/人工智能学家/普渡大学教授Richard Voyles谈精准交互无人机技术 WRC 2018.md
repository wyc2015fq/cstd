# 普渡大学教授Richard Voyles谈精准交互无人机技术|WRC 2018 - 人工智能学家 - CSDN博客
2018年08月18日 23:03:25[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：65
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBVl83cGEeVozNV0v31xU0xCyhEY75TJry4y5eqZiamFUX6KneOz5syphPaGMSGrU10fjYwVGTlQxNg/640?wx_fmt=jpeg)
来源：雷锋网
摘要：在2018WRC主论坛上，Richard Voyles教授就精准无人机的技术关键作了详尽阐述。
Richard Voyles, 美国普渡大学（Purdue University）机器人学院创建人及领头人，普渡大学特聘杰出学者，工程技术学院、计算机信息技术学院名誉教授。曾任职于奥巴马政府白宫科学技术发展政策制定办公室，和弗吉尼亚阿灵顿美国国家科学基金会，现任职于美国国家自然基金委员会，担任机器人和物联网项目审批负责人。参建国家机器人计划、美国联邦航天局准则、智能城市、创新和劳动力开发以及知识产权等领域。
**以下为Richard Voyles在2018世界机器人大会上主题为《精准无人机在机器人和物联网领域的应用》的演讲内容：**
如何理解精准交互无人机？从控制角度来讲，也就是空中的精准控制，其中**包括高度控制、力量控制和无线传感网络。**
### **IoT“前生”：无线传感网络**
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBVl83cGEeVozNV0v31xU0xCcDodt3bjjKS6amZFpBxjue76XCZzu4A9HLmKicI8ERImjsQgTbdhd8w/640?wx_fmt=jpeg)
无线传感网络是无线通信方式形成的一个多跳自组织网络。**机器人在其中发挥的不是一个网络系统的作用，而是一个桥梁的作用，它的传感器连接了网络世界和物理世界。从模拟世界到数字世界，机器人可以形成一个闭环。**我在实验室所做的很多工作和传统所说的精确或者精准不太一样，我会强调是怎样实现这样一个精准的，例如精准无人机可以通过六个自由度实现更高的精准度。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBVl83cGEeVozNV0v31xU0xCzpj1KVPuPw1vZRWjgNaObgZ8CPfiaKgsbVKoXEgbR9NDZicK2OgyT4bw/640?wx_fmt=jpeg)
**无线传感网络其实是物联网的前身，关注的是核心技术和低功、耗长时间的传感。**八九十年代主要是关注低功耗，以及如何从一个临时网络中输入信息，也就是早期“云”的概念。很多人关注自组织网络的形成和传感器数据的多跳路由。现在无线传感网络已有诸多应用实例，例如在海洋监测中的应用，我们协调了多个传感器，有对盐度监测的传感器，有对水活动监测的传感器，以小时或天作为采样周期，但是这个系统没有反馈环节，现在该系统已经被用于气候变化的监测。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBVl83cGEeVozNV0v31xU0xCZG2Kzaa8f9V4Pfuia6grD2R0UsyNkMnlnYaOfeWc2ibvJHjT5MibrXMOA/640?wx_fmt=jpeg)
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBVl83cGEeVozNV0v31xU0xCgyYIXxyxOsINBQz7Nz1ibXxYibunHfUgJmHkmcIR9IBecxthRibntYoVg/640?wx_fmt=gif)
在智慧家庭概念中，要考虑的不仅仅是评估环境，还有对环境产生的影响。我们经常谈到的应用领域叫作网络物理系统，例如我们在实验室花数年时间做的“智能特百惠”。这是我们与特百惠公司合作打造的智能厨房，实际上可以把这个厨房看作智能机器人。它主要通过传感网络实现低功耗、长生命周期，以及食品类别和数量的传感。我们做的不是全部人类的活动，而是将重点放在厨房中食品的准备上。
### **微节点和变形总线带来网络迭代**
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBVl83cGEeVozNV0v31xU0xCyuLEcNTWvg5ecqVgL8PxhibOpbqobjufjIHgqEhQ7IickHZSks7THj9A/640?wx_fmt=jpeg)
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBVl83cGEeVozNV0v31xU0xCgyYIXxyxOsINBQz7Nz1ibXxYibunHfUgJmHkmcIR9IBecxthRibntYoVg/640?wx_fmt=gif)智能厨房需要人们帮助规划食品和午餐，我们会不断地考虑怎样把用户包括在内，尤其是用户体验包含在这个环节中。机器人只做传感和思考，通过人类和智能冰箱的互动，由智能冰箱跟其它设备相联系，然后给人们提供冰箱当中储存食品的信息，这样我们开发的一些项目就要做一个以控制为核心的架构。**我们知道无线传感器有很多节点，我们和宾夕法尼亚大学合作做了微节点，这些微节点和其它的宏节点结合，创造出无线传感网络，可以实现闭环控制。**
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBVl83cGEeVozNV0v31xU0xCmsCNxy6Feicxyj3DtNt9FdzAz83ILticPxJ2Yf7SIl43j42RLFlc2A9g/640?wx_fmt=jpeg)
**![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBVl83cGEeVozNV0v31xU0xCgyYIXxyxOsINBQz7Nz1ibXxYibunHfUgJmHkmcIR9IBecxthRibntYoVg/640?wx_fmt=gif)我们开发的另外一个伴生的产品叫做变形总线，主要是为了适应各种传感器和驱动器。**我们把变形总线作为一个标准产品推出，和常规总线不同，常规总线收集传感数据时候是这样一个流程：首先希望传感器和CPU相沟通，然后把其它的内容放进去，利用总线的沟通实现传感功能。这种常规总线结构当中，我们希望传感器能够对信息进行一些转换，也希望CPU能够做一些思考，所以**在变形总线当中我们的转型器会在整个总线当中移动，利用这些基于设备的信号更加有效地对传感器进行变形。**其中，总线的拓扑结构并不是完全崭新的，但是却是一个多极并联的总线结构。
**高性能的节点产出了宏节点和微节点，具有重新编程的功能，而且是自适应的软件，我们利用总线当中的架构帮助这些系统。**例如无人机和机器人要能够适应改变的环境，而为了使得软件工程师的工作更简单，可以设计一些能够适应变化的环境的系统。这些元素怎么和刚才我们提到的网络相连接？我们的网络当中并不只有计算节点，还有传感节点、思考节点和驱动节点，例如精准无人机是物理世界和网络世界之间连接的桥梁，驱动器控制网络和传感器相连，驱动现实世界中的智慧家庭，驱动实际的医疗机器人系统。这就涉及到物联网的技术了，所以和传统上讲的无线传感网络还是有些不同的。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBVl83cGEeVozNV0v31xU0xC7x9U4qn3r1VqMtvYib46wZQ6WVYHTibV5XY2RHFDMXbRTNuWvAhfzFbg/640?wx_fmt=jpeg)
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBVl83cGEeVozNV0v31xU0xCgyYIXxyxOsINBQz7Nz1ibXxYibunHfUgJmHkmcIR9IBecxthRibntYoVg/640?wx_fmt=gif)刚才谈到很多带有节点的传感器，我们借此研发了履带机器人，主要应用于一些救援场景。我们的爬行机器人非常小，可以进行搜救，可以在废墟中寻找被掩埋的幸存者。我们还开发出了MOTHERSHIP机器人，我们的实验室除了基础研究和系统集成外，还关注如何进行传感和思考，并且进行功能整合，这样才能真正做成实用的机器人。
### **高精准交互无人机的研究**
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBVl83cGEeVozNV0v31xU0xCE6Y7L7kmn8U3Pv9xRK2CaibrQeELvAxqIj6PnI8neUic1RvfpQZ8icnlw/640?wx_fmt=jpeg)
高精准交互无人机到底应该是什么样的？这是一个很老的例子，主要实现空中操作移动的系统，能够移动重物。这里重点要关注的是这一群人，上图中的无人机是一个载人飞机，实际上是在做一个重力补偿，也在做非常精准的调校和组装。当然，考虑的不仅仅是X轴和Y轴，必须要考虑多维操控。最近大家都在研究六旋翼无人机，也就是实现六个自由度的无人机，六旋翼无人机有更精准的力量控制。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBVl83cGEeVozNV0v31xU0xCicia7RDByOJyn6OZXicyyQVwdVMF0x14ML4aISWOsEHtfERXbYWVoiawvQ/640?wx_fmt=jpeg)
这是之前我们在无人机方面做的一些早期的工作，大家可以看到，由于可以转向不同的自由度，同样也可以在不同的方向施加力。因为和周围环境的精准互动是非常重要的，也就是将虚拟世界的信息转化为实际的行动，这些旋翼通过在空中使用，其精准度肯定不如地面上那么精准。蓝色曲线是理想的曲线，用来衡量扭矩，需要考虑力量的大小和高度，红色曲线是具体使用的效率。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBVl83cGEeVozNV0v31xU0xCrHCcalvIScZvnLI9ia1fCcibDPEiamDicEhr5YVOG2vCRsNR0VpaAibWTPA/640?wx_fmt=jpeg)
我们通过这种方式和物理世界进行互动可以应用到各领域。在具体应用中，具体的配置也要进一步优化，让其在不同的高度实现不同的运转精度，这对于多旋翼无人机来说很重要。要优化无人机，无人机本身就要做得更精准。之前我们和农业部合作，过去几十年在做开发核电站用无人机的相关工作，有些技术应用在军事领域，有些设施已经寿终正寝，现在需要摧毁，为减少对人的伤害，可以用无人机代替人类进行一些人无法接近或者接触的核污染等地方。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBVl83cGEeVozNV0v31xU0xCrePnucYicWic8gjHic3QJYL1eSmkq4yljHXLSia0TBgL9UgBfibj54uyIxw/640?wx_fmt=jpeg)
我们希望通过实验能够实现在不同的配置下看到不同的效果，从而对无人机性能进行调校，达到更好性能，所以这张图实际上就是我们测试的结果，看一看如何能够对它的配置进行进一步的优化。我们也和合作伙伴一起做了一些工作，研究如何更好地做出无人机产品原型，能够实现这些无人机更精准的控制，比如消费产品领域当中如何实现更好的效果。实际上优化以后表现确实是更好了，无论是从哪方面来看，都比以前更好一些。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBVl83cGEeVozNV0v31xU0xCB7mcnVicXIpA8wFyxMgVcJs15Jhh4XIZbAPZQjU1FRRUD6aUiazJvvwA/640?wx_fmt=jpeg)
**这样会带来一些新的问题，有些技术细节，例如想要无人机的旋翼旋转的话，一方面需要旋臂垂直旋转，另外也要考虑平行向内旋转。这和固定翼无人机非常相似，有些机翼是平行翼，有些机翼是倾斜翼，这种倾斜翼会对飞行有特殊影响，这里我们通过对其进行优化，在力上实现最优效果。**无论是从哪个方向旋转，我们都要对其实现最好的调校，考虑到空气动力学的影响，稍做调整就可以移动得更快。实际上这种上反角效应即使是四旋翼无人机，可能在实际的飞行速度上都会有20%的误差。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBVl83cGEeVozNV0v31xU0xCoZ3wdomXkpbic6XhaL7oFdCxibeQurJJ6icgicd1AQXz4xSBnhC7nQQ63A/640?wx_fmt=jpeg)
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBVl83cGEeVozNV0v31xU0xCgyYIXxyxOsINBQz7Nz1ibXxYibunHfUgJmHkmcIR9IBecxthRibntYoVg/640?wx_fmt=gif)我们为什么要把传感网络技术应用在这些领域当中，比如核废物的分离，这样人就不用自己去做了。美国能源部有一个废物隔离实验场，里面有很多核废物，也有一个非常深的井，大概有600多米深，还有普利茅斯的气体扩散场，把地下的污染气体引出来。当然，也有一些低密度的气体，希望将这些气体扩散限制在有限的范围内。这种扩散场大概有1英里的长度，但我们无法用一个无人机覆盖所有的地方，因为可能要收集一些残留物的样本，之后只有将前期的工作做好才能正式拆除。
### **采访精选：机器人行业技术发展及市场转型**
雷锋网编辑在大会上采访了Richard Voyles教授，就机器人技术发展及市场转型关键问题，Richard Voyles给出了自己的见解。
- 
**您怎么看待机器人行业及其关键技术的发展，以及对中国的影响？**
Richard Voyles：机器人技术的发展，在过去是更加关注硬件方面、设备方面。**现在，我们迈入一个崭新的机器人技术的时代，以软件和信息为驱动的机器人时代。**这也将是新时代机器人技术的一大基础，在这样一个转变当中，中国和其他的国家也都受到相同的影响。
中国的机器人行业，包括中国大多数机器人公司，也正在进行这样的一种转型，**从硬件到软件和信息以及AI为中心的一个大的趋势转变，这也将会成为中国机器人行业发展的一个主流趋势。**
这样一个转变也使得企业之间的合作开始变的愈加紧迫，也促进了越来越多的企业之间的合作，从20世纪八九十年代，企业之间合作趋势愈多，也帮助中国内部实现了行业的范式转移，这个转移是从过去比较低层的，比如说教育，发展到更高层的生态系统。这也包括其他行业，例如软件、信息技术行业的发展，也在逐渐渗透。
通过这样的一个软件信息技术的逐渐渗透又反过来推动机器人学更进一步的发展，所以这是一个双向反馈螺旋上升的过程。
- 
**在未来机器人发展中，它受软件的影响较大还是受材料影响较大，未来机器人的发展是什么样子的？**
Richard Voyles：目前关于软件对于机器人行业的影响确实也是大家经常看到的一个话题，毕竟我们过去处于工业时代，但是我们的时代在不断的演进，现在已逐渐进入信息时代，未来还要进入软件时代。
在材料方面，我们也发现**材料也变的越来越智能**，我相信它们也将成为未来信息收集时代非常重要的一个有机组成部分，比如现在经常热论的多功能材料，或者我们也把它叫做机器人材料，这样的材料可以实现自我的感知、思考和行动。那在生物系统当中，比如：在人体当中我们的信息和数据它实际上和我们的所谓材料和硬件没有一个非常明确的界限，我们的大脑中神经原在大脑还有全身的细胞中分布，他们都是可以收集信息的，我们通过这一感知所得到的这些信息，也是与我们的人体和各个组织是密切相连不能分开的，我们就把它称作一个信息处理过程或者是软件，总的来说在生物系统当中，软件和硬件之间的区分是比较模糊的。
在我看来，实际上人类能够利用客观存在的物理世界中的工具，也是人类最终能够作为一个主流的物种繁衍壮大，最后统领整个地球的根本原因。信息的采集我相信在新的时代也将会起到越来越重要的作用。过去我们会非常重视硬件在机器人技术发展方面所起到的作用，这也是我们的技术竞争中经常关注的一个要素，实际上我认为我们真正应该最为重视的因素还是信息本身，而工具或之前提到一些硬件他们只能说是一种工具，帮助我们去解析信息。
举一个例子，在我自己的实验室当中，我们有一台最新款的3D打印机，我们利用这个3D打印机不仅去打印一些结构物质，也要让它实现遥感计算，还有自动等方面的功能，我们希望硬件和软件可以有机结合起来，这样的有机结合，也将会成为未来3D打印机技术发展的主流。而传统的3D打印机只重视硬件。
- 
**现在的消费类机器人的技术瓶颈在哪里？现在应用型机器人都往消费型机器人转移，您怎么看待这一个问题？**
Richard Voyles：我们现正在进行转型，而未来的方向就是人机界面。在机器人1.0时代，我们曾经热议的就是一个关灯工厂的概念，在这车间工厂当中机器人负责所有的工作，人不在现场而只是负责运维。
在机器人1.0时代，实际上我们关注的这一些领域发展最好的就是在汽车总装这样比较小的领域。关灯工厂其实是只在汽车总装等小的领域发展很好。大多数比如说汽车的零部件，它的制造本身也不是由机器人所执行，所以它只是在非常小的市场当中是比较活跃的。所以我认为我们要解决刚才所提到这样一个技术瓶颈，最重要的是让人去做他最擅长的事，让机器也做它自己最擅长的事。
充分利用这两者的优势进行互补，充分实现人机协作。我相信这样才是最有利于未来机器人行业的发展的。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBVl83cGEeVozNV0v31xU0xCgA4BLQmPYUfUsnVFmDAiby9gicYJ5XSE2iagz3FePCTMYZGtef8k4icrJg/640?wx_fmt=jpeg)
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)
