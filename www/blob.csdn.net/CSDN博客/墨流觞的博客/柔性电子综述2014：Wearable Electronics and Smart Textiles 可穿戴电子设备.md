# 柔性电子综述2014：Wearable Electronics and Smart Textiles 可穿戴电子设备 - 墨流觞的博客 - CSDN博客





2018年11月11日 12:38:48[墨氲](https://me.csdn.net/dss_dssssd)阅读数：786








## Wearable Electronics and Smart Textiles: A Critical Review

Matteo Stoppa and Alessandro Chiolerio

sensors

ISSN 1424-8220
[www.mdpi.com/journal/sensors](http://www.mdpi.com/journal/sensors)
原论文我放在这了：[https://download.csdn.net/download/dss_dssssd/10778273](https://download.csdn.net/download/dss_dssssd/10778273)

摘要：

电子纺织品(Electronic Textiles , e-textiles)将电子产品连接到纺织品中，具有现有其他电子制造技术所无法达到的物理柔性和尺寸。对于一些需要计算和感知快速改变的特殊应用，电子纺织品能更容易的适应, 这是未来电源管理(power management)和情景感知(context

awareness)的一个重要特性.希望通过发展可穿戴计算技术，使得在未来，电子系统将成为我们日常生活中不可或缺的一部分。当然，这种电子系统必须具有高耐磨性。可穿戴系统的特点是能够自动识别用户的活动和行为状态以及周围环境，并利用这些信息来调整系统的配置和功能。**本文着重介绍了近年来在智能纺织品（Smart Textiles）领域的研究进展，并且特别注重材料及其制造工艺（materials and their manufacturing process）**。每种技术都有优缺点，我们的目标是在灵活性（flexibility）、人机工程学（flexibility）、低功耗（low power consumption）、集成（integration）和最终的自主性（autonomy.）之间进行权衡
### 1. Introduction

Smart Textiles 代表一个非常广泛的研究领域和产品，能扩展传统纺织品的用途和功能性。纺织品和电子产品(e-textiles)的结合与智能材料的开发有关，这些智能材料能够实现广泛的功能，在当今的刚性和非柔性电子产品中都可以找到。smart textiles可分为三个种类 ：
- Passive smart textiles： 仅仅依靠传感器来感知环境
- Active smart textiles： 对来自环境的刺激能做出反应，集成了执行器和传感器
- Very smart textiles：能感知， 反应并使行为适应特定的环境

在生物医疗和安全领域，基于纺织品的传感设备一直是很大的研究领域。这种传感器可用于**感知脑电图，肌电图**；含有**热电偶**（thermocouple）的纺织品可以用来感知温度；将**发光原件**集成在纺织品中可用于感知生物光子；对**形状敏感**的纺织物可以感知运动，并可以与肌电感知相结合来获得肌肉的适应性；集成在织物中的**碳电极**可用于检测特定的环境或生物医学特征，如氧、盐度、湿度或污染物。

active functionality 包括power generation or storge (电的产生和存储)， human interface elements（供人类使用的接口），radio frequency (RF， 具有远距离传输能力的电磁波)functionality（射频功能）,或者assistive technology（辅助技术）。所有的电子设备都需要电，这是Smart Fabrics所面临的巨大挑战。**发电可以通过从运动或光伏元件中获取能量的压电元件来实现**[15]，供人类使用的接口大致分为两部分：输入设备或警告或显示设备（input devices and annunciation or display devices.）输入设备可以包括电容贴片，功能类似于按钮，或对形状敏感的织物[18]，可以记录运动或弯曲，压力，拉伸或压缩。报警器和显示设备可能包括扬声器[17],电发光的线[19],或含有有机发光二极管阵列的线[20]。织物还可以提供生物反馈(bio-feedback)或进行简单振动.基于织物的天线是一种相对简单的智能织物应用,织物天线仅仅是一些缝合进非导电纺织物里边的特定长度的导电纱线。

研究intelligent textiles的第一阶段往往是智能材料的研究。第二阶段是考虑如何将智能材料整合到纺织品中，当时这需要不同的技术方案，如图1：

包括：- a  embroidering 刺绣
- b sewing  缝纫
- c weaving 编织
- d non-woven textile 无纺布的纺织品
- e  knitting 针织
- f spinning 纺纱
- g braiding 交错编织；编结
- h coating/laminating 表面涂层或层压，
- i printing 印刷
- j Chemical Treatments 化学处理
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181111092358896.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
这些材料的无数种组合产生了一系列纺织品，但是可商业化的只有包含传统电线，微型电子和特殊连接设备的衣服。由于人们更喜欢穿舒适的纺织品，而不是硬硬的盒子，首先是努力将纺织品本身用于电子功能。

Smart Textiles在医疗、体育、艺术、军事和航天等领域都面临挑战。

本综述特别关注用于开发智能纺织品的材料和方法，每一种方法都是建立在公司，大学或科研机构做了大量的实验的基础上的。

### 2. Fabrication Techniques 制造技术

在过去的几十年，有很多技术和方法被用来实现 *smart textiles*。

#### 2.1 conductive fibers 导电纤维

最初，导电线主要用于科技领域：超净间服装，军用服装，医疗应用和电子制造业；具有导电性能或具有电子或计算功能的纺织品结构称为电子纺织品（electro-textiles），他们拥有大量的良好的性能，比如抗静电（antistatic applications），电磁屏蔽(electromagnetic interference shielding， EMI)，电子应用， 红外吸收(infrared absorption)或爆炸区域的防护服(protective clothing in explosive areas)

传统的金属纤维生产工艺是拉丝，是一种机械生产工艺.特点是有多种拉丝步骤， 分别是: coarse, medium, fine, and carding train.

图2
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181111095552589.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
用于拉丝的拉丝模由陶瓷，碳化物或钻石制成的铁芯构成的钢架(The drawing die, used to draw the fiber, consists of a steel mount with a core out of ceramics,

carbide or diamond ? )   金属线的初始直径随材料的不同而变化，例如，铜通常是8毫米，而铁是5毫米。拉丝之后,钢丝退火温度介于600和900°C,随后，它们将淬火。然后将细金属丝包裹在旋转拉丝筒上。

**相关的工作**

瑞士公司Elektrisola Feindraht AG 生产的金属单丝可以与各种纤维混合，也可以直接用于编织和编织。重要的是，根据使用的材料，有不同的电气性能。
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018111110053376.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
公司Swiss-Shield®(瑞士Flums)专门研究如何将金属单丝纳入棉、聚酯、聚酰胺和芳纶等纱线中。金属单丝由铜、黄铜、青铜、银、金、铝制成。下图显示了一种典型的带有基体纤维的导电纱线，以及在它们周围缠绕的金属单丝。
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018111110083314.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
#### 2.2  Treated Conductive Fibers

**直接在纤维表面做金属涂层，来具有导电性能**

不同于将电子器件附连接到纺织品基片上，可以使用电子将纺织品功能化。

导电纤维也可以通过在纤维上涂上金属、电性物质或金属盐而产生。涂层可以应用于纤维、纱线甚至织物的表面，以产生导电纺织品。**常见的纺织涂布工艺包括化学镀、蒸发沉积、溅射、用导电聚合物涂布**

在[43]提出了一种制造拥有不同材料层和材料结构的纤维制造方法。

该工艺以常规的预成型纤维加工为基础，在加工过程中容易产生数公里长的功能纤维

另一个相关的工作是使用纺织中的交叉纱线来制造晶体管
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018111110201160.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

由此产生的晶体管开关电流比(on-off

current ratio )超过1000，工作时时栅电压为1.5 V。
**相关工作**：

Textile Research Institute of Thuringia-Vogtland (TITV, Greiz, Germany)通过在传统纱线上涂上金属层，成功地生产出了导电线， 称为ELITEX。电导约为$1.2 \times S \cdot cm^{-1}$,因而电阻约为$8.34 O \cdot mm^2 / m$，电阻太高而不能导电。

#### 2.3  Conductive Fabrics 导电织物

有很多方法制造导电纤维，一种方式将导电纱线通过编制等方法整合到纺织结构中。然而，导电纱线在结构中的整合是一个复杂的，没有一个统一的过程，因为它需要确保导电织物的穿着舒适，柔软，而不是很硬。可以只用不同的方法制造具有导电性能的织物：
- a 金属线缠绕在聚合纱线上
- b  金属线涂覆在聚合纱线上
- c 金属丝线构成导电纱线
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181111103440796.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
编织织物结构可以提供一个复杂的网络结构，它可以作为精细的电路使用，该电路中有许多导电和非导电成分。同时该结构具有多层结构和空间用来容纳电子器件。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181111104459751.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

ETH的研究人员引入了一种新的制造方法。目的是实现定制纺织电路的可能性
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181111104717514.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

电路元件之间的接线结构是通过嵌入铜导线的纤维实现的。为了避免铜线之间的短路，必须在布线的特定位置进行切割，实现流程如下：- 在特定的十字路口用激光烧蚀去除铜丝上的涂层 coating removal on copper wires at defined intersections with laser ablation
- 用激光切断导线以避免信号泄漏cutting the wires avoiding the signal leakage with laser
- 将导电胶滴在十字连接处，连接两根线 creating the interconnection with a drop of conductive adhesive
- 使用环氧树脂沉积来实现机电保护adding mechanical and electrical protection with an epoxy resin deposition

**相关的工作：**

另一种实现导电织物的可能性是使用刺绣技术将导电结构附着在结构表面上
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181111105448468.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
#### 2.4 Conductive Inks 导电油墨

交互式电子纺织品也可以用**导电油墨生产**，首先，导电油墨必须包括包含导电金属，比如Ag，Cu，Au 颗粒（and a

carrier vehicle ？？？） 。大多数油墨都是基于水的，水是油墨的主要成分。这些特殊的油墨可以印在各种材料上，包括纺织品，来创造具有导电性的图案

丝网印刷技术使得平面电子的集成要比导电纱线系统的集成简单的多。
有几种技术可以用来在不同的基板上喷印带电材料，Sheet-based inkjet and screen printing

适用于低容量，高精度的工作。

#### inkjet

喷墨技术适用于像有机半导体一样的低粘度，可溶性材料。而对于高粘度材料，如有机介质，和分散颗粒，如无机金属油墨，会造成喷嘴的堵塞。油墨是通过液滴沉积的，所以厚度和分散并不是非常均匀。

使用许多喷嘴可以提高提高生产率，使用预结构的基片可以提高分辨率。

对于喷墨打印，油墨需要满足一下要求：
- high electrical conductivity 高的导电性
- resistance to oxidation 抗氧化
- dry out without clogging the nozzle during printing 在打印时，干燥，不堵塞喷嘴
- good adhesion to the substrate 对基底好良好的附着力
- lower particle aggregation 较低的粒子聚合度
- suitable viscosity and surface tension  合适的粘度和表面张力

油墨也可能含有添加剂，来调优油墨性能或添加特殊性能，从而提高其性能。

打印完含有金属颗粒的油墨后，为了获得导电的图案，颗粒必须烧结在一起建立连接，从而获得导电能力。**烧结 sintering**是指在低于相应的大块金属熔点的温度下将颗粒焊接在一起的过程,涉及到固体和液体之间的表面扩散现象。例，基于金颗粒（直径1.5 nm）的油墨，熔化温度低于$380^{。}C$；而基于银颗粒（直径 15~20 nm）的油墨， 熔化温度为$180^。C$。

##### screen printing

用于**粘性材料的图案印刷**

screen printing适合于制造电器和电子产品，因为它能从似膏体的材料中产生有图案的，很厚的层。screen printing，是一种模版印刷工艺，先通过有图案的织物丝网印刷**粘性浆料**，之后是干燥过程。该方法可应用于平面或圆柱形基片。根据基材材料和印刷结构的要求，用高的温度来实现致密化是必要的(organic substrate T < 150 °C. Glass, ceramic and metal substrate T > 500 °C)。

该方法不仅能在无机材料中上打印电导线(比如电路板和天线)，也可以制作绝缘层和钝化层。通常来说，在低于100um的分辨率下，可以实现$50m^2/h$的产量，通过优化手段，分辨率可以达到30um

这种相对简单的方法主要用来制造导电层和介电层。除此之外有机半导体也可以作为基板材料，比如OPVCs and OFETs。

最近，南安普敦大学，发现利用创新的screen print技术在纺织品上打印电极和导电线网络结构，来用于医疗应用。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181111122953493.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
一种聚氨酯浆料被印刷在纺织织物上，创造一个光滑的，高表面能量的界面层，然后将银浆料印刷在这个界面层的上，来提供导电轨道。银粉被印在无纺布上，用于制造可穿戴健康监测设备。

研究人员使用干电极和导电油墨研发了不同的生物电位传感系统，来进行信号跟踪，以证明这项技术可以用于生物医学应用。具体应用试验为:心电图、面部肌电图、前臂肌电图
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181111123334364.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
**相关工作：**

The National Textile Center of the North Carolina State University (Raleigh, NC, USA)

“在无纺布上印刷电路”来制造一种检测生理指标的服装，比如测量心电图，心率，呼吸和温度等。
#### 2.5 Conductive Materials as Sensors 导电材料作为传感器

由于环境变化而导致电气性能改变的导电纺织品可以用作传感器，一些典型的例子是对变形敏感的纺织品，可用作压力传感器，stretch sensors and breathing sensors。另一方面，使用智能纺织品，有可能制造bio-potential sensors。

##### 2.5.1 stretch sensors

Stretch sensors主要用来检测和感知身体参数，这是因为纺织品和身体会有很大的接触面积。同时意味着检测可以在身体的几个地方同时进行。例如，这些传感器可以用来测量心跳速率，呼吸，运动和血压[64]。纺织传感器的一种特殊结构是将具有压阻特性的纤维整合在一起，使其成为应变或变形传感器。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181111184355638.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
手套，通过连接到手机，可以采集手势信息。手套中的传感器可以检测穿戴社的手部移动。每根手指使用四根电线来构成一个电路，电压随着手指的位置发生改变。

**相关的工作**
- Intellitex  ：套服，检测儿童的心率
- Redhill, UK，将拉伸传感器集成到衣服中，可以实现 音乐音量和音轨的控制

##### 2.5.2  pressure sensors 压力传感器

常用作电子设备的开关或用户接口，也用于检测用户的生命体征

有一些方法来制造平面压力传感器（plane pressure sensors）。其工作原理是压电谐振频率随着施加电压和电容的改变而改变，这是由覆盖着传导线矩阵的弹性泡沫引起的。对于电容传感器来说，寄生电容和电阻的变化可以通过电子器件来补偿，因此布线对感知信号的影响是微乎其微的。

The Wearable Computing Lab of ETH Zurich已经发明了：一种带有多个电容式压力传感器的矩阵，用来集成到衣服中，如下图：![在这里插入图片描述](https://img-blog.csdnimg.cn/20181111190417767.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

通过这种方法，能够测量人体的压力，和检测上臂的肌肉活动。使用在人体不同的地方，可以为运动跟踪或几日物理状态的检测提供更多的细节。
**相关工作：**
- 
商业化一款产品

- 
KeyCaseTM keyboard， pda 个人助理

- 
高性能多元件压力和触觉传感系统 Tactarray and ConTacts 下图a
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181111190935751.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)- 
如上图b， 两层导电织物组成，中间十一层不带电网片。当织物受压时，一个导电层与另一个导电层接触，从而产生电。

- 
The team of the Center for Micro-Bio Robotics of Istituto Italiano di Tecnologia 生产了一种完全基于商用导电织物的复合电容**三轴传感器**，证明了其在操纵下的高依从性和稳定性


#### 2.5.3 Electrochemical Sensors  电化学传感器

最近对新型制造方法和电化学技术的深入研究使化学传感器能够增强传统物理测量，比如心率，脑电图，心电图(heart rate, EEG, ECG)等，

基于柔性和纺织物的丝网印刷的电化学传感器可能是非侵入式检测的替代品，但是不能很容易的附着在身体，尤其是皮肤。由于其腐蚀性的化学成分。
**相关的工作：**

The researchers of National Centre for Sensor Research, Dublin City University (Ireland),制造出了可穿戴及化学传感器，检测穿戴者移动以及身处的环境。该化学传感器能够实时测量和分析身体上的汗液。他们也开发了一种微晶片版的平台，用来测量汗液pH值的变化。在芯片两边放置贴面封装(SMT)LED和光电二极管模块，注意要与PH敏感纤维对其，来检测PH敏感纤维的颜色变化。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181111192801348.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
#### 2.5.4   Textile Energy Harvesting and Portable Power Supply System 能源采集和便携式供电系统

**电源技术仍然是可穿戴设备面临的重大挑战**，虽然可穿戴电子产品已经取得了相当大的进展，但锂电池作为可穿戴设备的动力源，由于其脆弱的机械稳定性，并没有跟上这一进展的步伐，导致它们仍然是整个技术的限制元素。
**开发一种可穿戴系统，能积累身体消耗的能量。**

其他研究团队利用用户在日常活动中通过腿部运动和身体热量提供的能量。例如Infineon尝试利用身体运动产生的能量，使用喷墨打印技术将MP3整合到夹克中。[78]

在英国，博尔顿大学已经开发出一种新的技术，将压电聚合物基体和光伏涂层系统集成在一起，创造出一种薄膜或纤维结构，能够从自然界中获取能量，包括太阳、雨、风、浪和潮汐.[79]

Engineering and Physical Sciences Research Council (EPSRC) 发起一个项目，采用快速印刷工艺和活性印刷油墨发明了一种可嵌入到纺织品中的能量收集膜[80,81].

Zhong Lin Wang领导的Georgia Tech researchers，发明了利用氧化锌纳米线包裹的柔性纤维来讲机械能转化为电能。研究人员说纤维能捕捉任意一种振动或移动来产生电流。镀金氧化锌纳米线，每一个大约3.5um高，生长在一层柔性聚合物纤维上，这些纳米线与未处理的纳米线摩擦，来产生电流[82]。

通过新一代的太阳能电池来手机太阳能的衣服[83,84]，将柔性太阳能电池集成到服装中可以为便携式电子设备提供电力,尽管它受到环境光强度的限制,光伏发电仍然是提供电力最先进的方式。

便携式设备的电力需求现在已经很低，集成在衣服上的太阳能电池可以为大多数移动电子设备提供电力[85].
**相关工作：**- The ILLUM jacket，使用了印刷电致发光油墨和一刷光伏发电技术

![在这里插入图片描述](https://img-blog.csdnimg.cn/2018111119510691.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
- Thermotron of UNITIKA (Osaka, Japan)， 一种特殊纺织物能将太阳光转化为热能，并能无损存储。
- German research team led by The Fraunhofer Institute for Reliability and Micro-integration (FhG-IZM, Berlin, Germany)制造出一种能够为交互式电子纺织品提供电力的电池。这个研究小组研发了一种印刷在基板上的小型电池，约为纽扣大小或硬币形状，可快速制造，并且成本低于1美元。最后的成果是120um厚的AgO-Zn电池，可以被印刷在任何基板上。

#### 2.6 Planar Fashionable Circuit Board (P-FCB)

P-FCB 可以制作在纤维或可穿戴设备上的电路板。柔性超高，就想普通的衣服一样。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181111201439221.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
P-FCB的基板是使用梭织布制成，平面电极通过导电环氧树脂的丝印或金溅射直接沉积在织物贴片上。首先，电路板通过丝网印刷技术印刷在梭织布上，接着，IC放在纺织物上，并与图形电极连接；最后使用不导电环氧树脂将IC模压成型。

利用这些技术，KAIST 的研究人员研发一种多层电路，之前的研究包括如何获得好的电气和机械性能以及系统设计参数，比如最大功耗，最大电流密度，相近两根线之间的串扰以及耐久度等[90]

#### 2.7 Wearable Antenna 可穿戴天线

对于可穿戴天线的研究已经开展，探索新型的舒适灵活的智能结构。

如果想要研制一套可穿戴的自主系统，天线显得十分重要。它允许系统将信息从服装内部的传感器传输到控制单元或监控其他电子参数。

因此，可穿戴天线是将衣服融入通讯系统的纽带，使电子设备更隐蔽。为了达到良好的效果，可穿戴天线(wearable antennas)必须薄、轻、低维护、鲁棒性高、便宜且易于集成在射频电路（RF circuit）中。平面结构、柔性导电材料和介质材料是可穿戴天线的具体要求[92,93]。一些材料的属性会影响天线的行为。比如，平面微带天线的带宽和效率主要取决于基板的介电常数和厚度

一般来说，介电常数很低的纺织品可以减少表面波损耗并增加天线的阻抗带宽。因此，为了减少不想要的影响，了解这些特性是如何影响天线性能十分重要。
另一个问题是，身体的运动会使天线的空间几何形状发生变形并影响其性能。当织物适应表面轮廓时，它会发生弯曲和变形，从而改变其电磁特性，从而影响天线性能。因此要设计一种可穿戴的调频天线，要比FM带宽(大约81-130MHz)更宽，并且不受身体干扰[97]。

以下是天线设计的要求：
- 选择纺织天线的放置位置[98,99]; choosing the correct positioning of the textile antenna
- 在堆叠不同的织物时，纺织天线必须有准确的厚度[99]。 the textile antenna must be made with an accurate thickness stacking the different fabrics
- 贴片的几何尺寸必须保持稳定[100]the geometrical dimensions of the patch must remain stable [
- 层之间的连接不能影响电气性能，与其他电子服装(e-garments)的连接稳定可靠。[101-103] the connections between the layers must not affect the electrical properties and the connections with other parts of "e-garments"have to be stable and robust.

**相关的工作**
- 
[104] 研制了一种全纺织波导天线，使用一种受晶胞启发的材料，改材料也用于合成右旋/左旋传输线。可用于2.45和5.4 GHz双频无线局域网应用。

- 
[105] 使用传统或工业纺织品制造纺织天线。典型的到点天线部件使用现代的导电纤维制成

- 
[106]  纺织品RFID是天线的一个特殊解决方案

- 
[107] 在组件和器件之间如何连接？在大多数情况下，使用的方法是在可拉伸的基体材料中研发可弯曲的电镀金属丝

- 
[108] 下图展示了一种可伸缩的互联结构，“horseshoe-shape”马蹄形的结构，能够承受较大的机械变形而保持良好的电学性能。
![在这里插入图片描述](https://img-blog.csdnimg.cn/2018111209374257.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)- 
[109] 对于高频信号而言，连接必须是可拉伸的，所以基板和道题也必须会可拉伸的。通常使用聚合物材料作为封装基板，因为其具有拉伸性和生物兼容性。


### 3. Discussion

前一节中介绍的方法有很大的不同，每一种方法都有非常具体的特性，如导电性(conductivity)、柔韧性(flexibility)、生物相容性(biocompatibility)、机械阻力(mechanical resistance)和耐洗性(washability)。然而，只有少数几种方法能够同时满足所有这些需求。下面的章节旨在澄清一些材料和方法的特点。

#### 3.1 Conductive Wires 导电线

对于衣服来说，一些触觉属性比如拉伸性，恢复性很重要。因此所用的纤维应该是细的，织物的单位面积质量应该很低(不超过300$g/m^2$)。然而这些要求与要达到合理导电率所需的材料和几何形状相冲突，因为将金属线等导电材料与纺织品结合时候增加衣服的硬度，降低弹性[110]。

在一些文献中，金属丝和金属纤维没有区别。然而，斯普林特金属公司(德国赫默公司)根据直径来定义金属纤维和金属丝。金属丝的直径为$30um~1.4mm$，金属纤维的直径为$2~40um$[111]。

金属纤维的优点是强度(strength)，合成物(composition),生物惰性(biological inertness),可以低成本获取。由于其惰性，对洗涤和汗水不敏感。

然而，他们不能提供均匀的温度，随着时间的推移，本身的机械脆性也会损耗纺织品的机械结构。此外，他们要比大多数纺织纤维更重，不容易生产质量均匀的混合物。[112]

使用“导电线”方法，在织物生产后不需要额外的步骤来建立导电率,导电线的电阻约为$10 - 500 \Omega	/m$[113]

下图显示铜丝缠绕在纱线周围，由于铜线螺旋缠绕纱线，它们在织物内部的位置没有精确定义[114]，电阻约为$17.8 \Omega /m$
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181112101042610.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
### 3.2  treated coatings 涂覆层

涂层的优点是，它们适用于许多类型的纤维，可以产生良好的导电性，而不显著改变基体的关键性能，如密度、柔韧性和手感(density, flexibility

and handle)。然而，金属和纤维之间的粘附以及耐腐蚀性会导致问题.

即使这些纺织品的主要用途是防止静电和电磁干扰(electromagnetic

interference , EMI)，通过涂层处理也可以制成导电织物。正常纺织品的表面电阻率(surface resistivity , Rs)变化从超过$10^5 \Omega/sq$到低于$100 \Omega /sq$,低电阻率用于静电防护，电磁干扰防护和加热应用。通常是通过吸收或浸渍于有机抗静电剂实现的。

目前生产导电纤维最重要的方法是：
- 在纤维中嵌入导电材料（carbon black, metal wires, graphite and metal powder or

flakes of Al, Cu, Ni, Ag）
- 使用导电金属或聚合物纤维的混合纺织品（比如polypyrrole, polyaniline）如下图
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181112102812821.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
[116] polypyrrole-coated的使用来降低表面电阻率。

直接操作纱线，使其具有导电性的最大优点是可以保持纱线柔性等特点。

#### 3.3 Conductive Fabric PETEX

通常，由导电纤维制成的织物具有如下图所示的结构
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181112103546391.png)
如果目的是利用导电纤维来创建一个电路，我们必须考虑到织物不规则的形状，不可能获得导电纤维之间接触点位置的几何重复性， 也就是接触点的几何是无规律的，会偏离目标位置。

ETH研究者采用的解决方案是开发一种合成纤维,它由直径42μm的聚酯单丝线(PET)与直径为50 ± 8 μm (AWG461）

的铜合金导线构成。铜线为屠夫一层 聚氨酯作为绝缘材料。这种材料与在维度方向相比，在经度方向有很高的抗拉伸能力。这种方向上的差异可以用来进行更可靠的制造过程，并尽量减少破损。

下图展示了普通的电子纺织品和PETEX的差别：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181112104510492.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
#### 3.4 MIT CAD Embroidery 刺绣

这种技术允许在计算机辅助设计(CAD)环境中精确地指定电路布局和缝合图案。这种技术也允许控制和集成不同电子性质，比如电阻的纱线。刺绣比针织或编织(knitting or weaving)更有优势[39]

#### 3.5 Conductive Inks 导电性油墨

在织物上直接打印导电图案是一种通用的技术，但有时也会暴露其局限性。大多数导电油墨和浆料都采用银，脆性大。在打印过程中，一个限制是银的厚度。要实现40um的厚度需要进过几次印刷，而电阻与印刷次数也有关，如下图：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181112105628167.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
另一个问题是机械阻力，如下图a，因数可能使得纤维起皱，从而破坏了纤维的导电性。

导电率与印刷次数和褶皱程度有关[47]
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181112105929183.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
**这是可能探索的一个小点！！！！**
North Carolina Institute的研究人员，发现了一种新的方法来控制印刷电路的耐久度，印刷电路中包含印刷线的涂层。油墨的粘度决定了印刷介质在洗涤过程中的性能。

结果显示，附着在表面的油墨粘度越高，阻抗的短期效果越好

所使用的油墨是由Creative Materials制成的，与其他商用导电油墨相比，它的阻抗曲线更平滑、更低。相关的更细致的报告：[117]
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181112111703910.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

#### 3.6  Stretchable Sensors 伸缩传感器

通常来说，由纤维构成的纺织材料，在连接处形成复杂的连接网络，在发生形变时，会产生很多有趣的现象：
- 接触点的数量会改变 the number of contact points changes
- 纤维延伸， fibers are extended
- 纤维的横截面积减小 fiber cross-section is decreased

上面列出的这些因素可能会影响拉伸传感器的质量[118]，使用聚合物制造大应变的应变传感器是会产生有趣的现象。Mattmann et al[119]研发的拉伸传感器，由50wt -%的热塑性弹（thermoplastic elastomer， TPE）和50 wt-%炭黑颗粒组成。炭黑颗粒制作成直径为0.315um的纤维形状。利用应变仪和电阻测量仪对传感器的进行测量，结果表明，该体系对应变具有线性电阻响应，滞后小，没有老化效应，对应变速度的依赖性小。传感器与织物的连接是通过硅胶薄膜实现的。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181112130806103.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
图24表示的是传感器的行为和电阻随着电阻的变化情况。图25表示的是当改变应力是，电阻成线性增长，并且机会没有滞后效应。

[118]中提高使用此方法制造三种拉伸传感器及性能变化。

#### 3.7  pressure sensor 压力传感器

**使用电容来测量压力的**

Meyer发明的压力传感器，空间分辨率为$2 \times 2 cm$。在$0-10N/ cm^2$的测量范围内，误差低于4%。基于电容，下图为电容VS压力，滞后高达20%。滞后高的原因或许是因为在两个导电层中之间插入一层电介质材料。

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181113161130758.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

一个理想的压力传感器低滞后且可重复测量。因此，纺织压力传感器适用于不需要高精度的领域

### 3.8  Electrochemical Sensors 电化学传感器

由于在运行过程中传感器本身活性的微小变化或表面污染。化学/生化传感器需要校准。这严重限制了这些传感器在连续监测场景中的长期应用。

在这些限制中，还没想到合适的办法将电子器件和无线发射器无缝的整合到传感器中。下图(图27)显示了可穿戴汗水传感器在运动试验中的性能。PH测试和心率测试，呼吸频率测试仪器进行。分析是通过将电极放置在参考织物贴片上进行的。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181113161954505.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
### 3.9 Power Supply 能量供给

下图是不同的能量源和可提供的能量[120,121]

![在这里插入图片描述](https://img-blog.csdnimg.cn/2018111316212062.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

从表格中可以得出，尽管身体乐意产生能量，但是能获取利用的确很少。所以未来的可穿戴设备如果想只利用使用者自身运动，发热产生的能量，则必须低功耗并且有很高的能量收集效率。

### 3.10 Planar Fashionable Circuit Board （P-FCB）

适用于服装的电路板

通过四探针法测得P_FCB上导电薄膜的电阻为$44m\Omega/m$。尽管在之后

50个清洗周期都没有观察到明显的退化，，仍然需要更多的耐久性测试。

一根15cm长，1mm宽的P-FCB传输线的电阻和带宽如下图所示：
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181113163230498.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

使用vector network analyzer[122]进行结果分析。带宽为80MHz。为了得到结论，使用超过100个连接(connections),直接同过的传输线电阻为0.34 $\Omega$， 使用导电粘合剂后电阻降到$0.24 \Omega$。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181113163557123.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

通过水洗试验测试了P-FCB的机械强度;50+周期后，电特性变化可忽略[123]。结果表明，该材料成本低，易于制作，提高了耐磨性和柔韧性

### 3.11 可穿戴天线 Wearable Antenna

可穿戴天线需要穿着舒适，所以传统的基于硬基板的设计不合适。

材料的介电属性会影响的天线的性能。

要谨慎选择材料

材料的介电常数,复数：
$$\epsilon_r : \epsilon = \epsilon_0 \cdot \epsilon_r = \epsilon_0 \cdot(\epsilon{_r^{&#x27;}} - j\cdot\epsilon{_0^{&#x27;&#x27;}})$$

其中，$\epsilon_0$是真空介电常数，介质性能是不同物理参数的函数:温度、频率、表面粗糙度、含水率、纯度和材料均匀性

下图为介电常数$\epsilon{_r^{&#x27;}}$,和虚部与实部的比值， $tan\delta = \frac{\epsilon{_0^{&#x27;&#x27;}}}{\epsilon{_r^{&#x27;}}}$.

![在这里插入图片描述](https://img-blog.csdnimg.cn/2018111316503511.png)

低介电常数(1到2之间)减少表面波损失。因此，降低介电常数会增加空间波，从而增加天线的阻抗带宽

此外，水也会影响纺织品的介电性能。当水被纤维吸收时，介电常数和损耗角度正切就会增加。物覆盖或表面处理可以提供足够的保护，免受潮湿和各种气候条件的影响.[100]

[126]发明了一种新型的可穿戴天线。

介质材料的厚度改变了平面天线的带宽。必须在效率和带宽之间找到一个折中方案[125].还有天线几何尺寸的事儿。

影响天线导电线的一个参数是**纺织物的导电性**，$\sigma$，单位是$S/m$:
$$\sigma=1 / (\rho_s \cdot t)$$其中，$\rho_s$为表面电阻，$t$为纺织物的厚度。一个问题是在纺织品内部$\sigma$并不相同，而是有几个不连续的值，这影响了电流流通。[127]

另一个影响导电的因素是**纺织物的结构**电流的流动取决于所使用的纺织结构的类型，为了减少导电损耗，最好使导电路径与电流方向保持一致[98]

通过选择合**适的材料和纺织品结构**，可以实现较好的天线性能

### 3.12 Stretchable Interconnections 可伸缩的互联结构

电导体的延展性是满足不同设计的要求

采用FEM模型或其他的测试（图29）进行设计优化。[108]对马蹄形设计做了测试，

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181113170517239.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

随着变形能力的增大，导体损伤明显减小。

Hu[128]使用不同的方法得到了一种可变形结构
## 4. conclusion/outlook

纺织品是实现可穿戴生物传感器的一种很有吸引力的基底材料。

用**柔性电子**器件取代pcb是降低电子纺织品刚性和提高其耐磨性的一种很有前途的方法

*[15] 发电：Edmison, J.; Jones, M.; Nakad, Z.; Martin, T. Using piezoelectric materials for wearableelectronic textiles. In Proceedings of the 6th International Symposium on Wearable Computers(ISWC), Seattle, WA, USA, 7–10 October 2002; pp. 41–48*[18] 对形状敏感的纤维结构18. Pacelli, M.; Loriga, G.; Taccini, N.; Paradiso, R. Sensing Fabrics for Monitoring Physiologicaland Biomechanical Variables: E-textile solutions. In Proceeding of the IEEE/EMBS InternationalSummer School on Medical Devices and Biosensors, St. Catharine‘s College, Cambridge, UK,19–22 August 2007; pp. 1–4*[17] 扬声器：How to Get What You Want? Available online: [http://www.kobakant.at/DIY/?p=48](http://www.kobakant.at/DIY/?p=48) (accessed on11 January 2014).*[19]电发光线：Dias, T. Development and Analysis of Novel Electroluminescent Yarns and Fabrics forLocalised Automotive Interior Illumination: El Yarns and Fabrics. Text. Res. J. 2012, 82,1164–1176.*[20] 集成oleds的纱线：Janietz, S.; Gruber, B.; Schattauer, S.; Schulze, K.; Integration of OLEDs in Textiles. Adv. Sci.Technol. 2012, 80, 14–21.*****






































