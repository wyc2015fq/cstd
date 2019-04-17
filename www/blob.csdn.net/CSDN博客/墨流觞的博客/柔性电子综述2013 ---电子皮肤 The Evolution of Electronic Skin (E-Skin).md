# 柔性电子综述2013 ---电子皮肤 The Evolution of Electronic Skin (E-Skin) - 墨流觞的博客 - CSDN博客





2018年11月14日 15:01:10[墨氲](https://me.csdn.net/dss_dssssd)阅读数：264








###### The Evolution of Electronic Skin (E-Skin): A Brief History, Design Considerations, and Recent Progress

Mallory L. Hammock , Alex Chortos , Benjamin C.-K. Tee , Jeffrey B.-H. Tok , and Zhenan Bao

ADVANCE MATERIALS

原论文:[https://download.csdn.net/download/dss_dssssd/10784139](https://download.csdn.net/download/dss_dssssd/10784139)

摘要：皮肤是很6的器官，它由一个集成化的、可伸缩的传感器网络组成，将触觉和热刺激信息传递给大脑，使我们能够在我们的环境中安全有效地行动。人类皮肤激发了人们对大面积电子器件网络的研究，这主要是受创造自主智能机器人和仿生假肢等应用前景的推动。由柔性，可伸缩的，鲁棒性强的并且可大面积实现和多功能合成的器件组成的电子网络的发展，证实了研究E-skin的可行性。电子皮肤在空间分辨率和热灵敏度方面已经能够提供比人的皮肤更好的性能。通过添加额外的感知功能(chemical and biological sensing)和更先进的预期功能(biodegradability and self-powering, 生物降解能力和能源的自我供给)， E-skin还有更具前景的未来。该领域的持续快速进展有望在不久的将来开发出完全集成的e-skin。
## 1. Introduction：A Brief Chronology

我们的皮肤是我们与周围环境互动的物理屏障，允许我们感知各种形状和纹理、温度变化和不同程度的接触压力。为了实现这种高度精密的传感能力，有几种不同类型的高度专门化的感觉感受器集成在在皮肤中。这些接收器将物理接触转换为电信号，传给大脑进一步处理。收集到的信号最终会被躯体感觉皮层（somatosensory cortex）解释为热觉，触觉等不同的感觉，让我们能够轻松自如地驾驭我们的物理世界。

创造一种具有类人感觉能力的人造皮肤的努力是制造大面积的、多感官表面可能性所激发的，这些表面可用于自主人工智能（比如机器人），医疗诊断和义肢，这些义肢旨在提供和人体皮肤相比，同等或更好的体感功能。赋予机器人感知能力可以扩展其应用范围，包括高度互动的任务，如照顾老人[2]。将传感器皮肤植在身体上可以提供前所未有的诊断和监测能力[3],这种具有感知能力的人造皮肤在文献中通常被称为：sensitive skin, smart skin, or electronic skin(E-skin).

尽管人类皮肤最主要的功能是感知机械压力。通过增加额外功能，电子皮肤的功能会得到增强。

可以将生物和化学传感器集成到柔性基底上。通过“电子舌头或电子鼻子”，可以在液体或气体中感知特定物质的存在。其他的感知模式(温度)和其他的功能，比如(biocompatibility, selfhealing

and self-powering, 生物兼容性，自我恢复，自我供电)也很重要，将在后边介绍。
### 1.1 1970s-1990s

受科学幻想小说启发。

1970s 出现一些科幻小说《Cyborg》；《Six Million Dollar Man》

1980s 《Star Wars》 Luke Skywalker装了一个机械手(electronic hand)；《Terminator》 人造机器人，具有自我修复能力。
1974 展示能处理离散感知反馈的拟人机械手[7]；大约10年后，HP发布了很简易的触摸屏；1985，GE使用基于柔性基板的离散红外传感器，为机械手臂覆盖一层电子皮肤，分辨率为5cm[8]。这种人造皮肤可以近距离感知周围环境，使机器人的手臂能够避开潜在的障碍，在物理环境中有效地活动.尽管机械手臂没有手指并且分辨率很低，但是已经证明了通过将电子元件集成到薄膜中可以实现真实环境下的人机交互。例如，这个机械手臂可以与芭蕾舞女演员“跳舞”，而不需要任何预先设定的动作[8].除了人工皮肤与周围环境相互作用的能力之外，人工皮肤模仿人体皮肤的机械特性以适应其各种运动也是同样重要的。soft, flexible, and stretchable electronics很重要。

1990s，使用柔性电子材料制作大面积、低成本、可打印的传感器薄片。[9]第一个柔性传感器片，蚀刻薄型硅晶圆片，并将其集成在聚酰亚胺薄片上。[10]提高对机械弯曲的可靠性。[11]使用有机半导体制造的柔性阵列出现，其性能与a-si（非晶硅）相媲美。

世纪之交，讨论了E-skin在机械，医疗等多个领域的可能应用。[12]给出了会议的概要总结。

21实际初，研究者开始探索不同种类的传感器，其更容易与微处理器结合。

### 1.2 2000s至今

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181114145323824.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

[13]发明了可拉伸电极(stretchable electrodes),[14]通过使用超薄（100你nm）的薄膜脆性材料，这种薄膜使用可拉伸的材料连接，将Si转化为柔性的，高性能的电子材料。[15]制造了有机薄膜电晶体（OFETs），用于使用有源矩阵读出的大面积集成的压敏片。使用泡沫介电材料[16]和铁介质[17]与FETs集成来感知压力变化。[18]提出了用于高灵敏度电容式压力传感器的微结构弹性介质，并研发出一种复合导电弹性体，具有重复的自我修复能力和机械压力传感能力[19],其他研究小组已经y研发出可伸缩的光电元件，包括发光二极管(LEDs)[20]和有机光伏(OPVs)[21]，可用于与e-skin集成。

在下面，首先讨论用于柔性，可伸缩E-skin的方法和材料(section 2)，触觉传感器（tactile sensors ）(Section 3) flexible chemical and biological sensors 柔性生物，化学传感器(section 4)， e-skin应用的其他重要特性（section 5），一些成果展示(section 6), 该领域的前景展望(section 7)。

## 2. Design Considerations for E-Skin

人皮肤作为性能基准。模量地，可伸缩性柔性(low modulus, stretchability

and fl exibility).

接下来主要讨论一些材料和技术方法。
### 2.1 材料 materials

人体皮肤的一大特点是机械顺从性（mechanically compliant），能够在不造成物理损伤的情况下拉伸。low mechanical modulus（低机械模量）使得可以将机械变换传给皮肤层之下的受体。因而材料的选择要增加人造皮肤的可伸缩性和柔性。同时也要提高自我修复能力[28]

材料的选择是至关重要的。除了**机械顺从性**，**良好的电气性能和与大面积处理技术的兼容性**对于创建高功能、低成本的设备非常重要。**纳米材料**在物理性能和成本上是好的选择。到伦理集中常用的材料，也推荐了讨论可伸缩电子材料的**综述[29]**.

#### 2.1.1 Substrates 基板

Polydimethylsiloxane (PDMS) 聚二甲基硅氧烷， 用于E-skin和其他可伸缩电子的制作。

优点：化学惰性、在较大的温度范围内稳定、透明、可变的机械属性，通过在紫外线下曝光可以区分粘附和非粘附区域，(chemical inertness, stability over a

wide range of temperatures, transparency, variable mechanical

properties, and the ability to defi ne adhesive and non-adhesive

regions through exposure to UV irradiation)这在电子材料附着于其表面上很重要[30]
其他的基板材料:
- polyurethane： 聚氨酯[31]
- electrospun elastomeric fibers: 静电弹性纤维[32]
- common textiles： 纺织品[33]

#### 2.1.2 Dielectrics 电介质

PDMS对于FETs(场效应管)来说十号的电介质[34]

研发高的介电常数(k)和低的漏电流的弹性电解质对于制作e-skin使用的可伸缩的晶体管和电容式触觉传感器是很有前景的。研发高的介电常数(k)的聚合物的途径有三个：
- 1） chemical design
- 2）addition of highk inorganic particles 添加k高的无机粒子
- 3）addition of conductive fillers  增加导电膜

chemical design通常涉及到聚合物链的修改[35][36], [37]关于k的综述;

用于弹性体的高k的导电粒子包括$TiO_2$[38],NPs,$BaTiO_3$;导电薄膜包括metal particles[40], conductive polymers (CPs), [ 41 ] 和  carbon nanotubes (CNTs)[42]。增加电解常数[43]和有效的电极区域[44],**离子凝胶**是聚合物和离子液体的复合材料，在形成电双层的基础上表现出极高的电容[45],电容对介电层的厚度不敏感，随着应变变化保持恒定，适用于耐应变系统。最近在机械鲁棒性方面的改进使离子凝胶成为更可行的结构应用选择[46]

#### 2.1.3 CNT-Based Active Materials 基于碳纳米管的活性材料

CNTs有极好的电子，机械性能以及化学稳定性。

载流子迁移率达 (u)$10, 000 cm^2 V^{-1}s^{-1}$ [47], **[48]总结了CNTs的特性**。只强调对于E-skin而言最重要的特性。

可靠和可扩展的简单合成技术对于大面积、低成本设备的生产至关重要。CNTs可以通过各种高容量、低纯度的技术生产，比如激光烧蚀（laser ablation）[50]，电弧放电(arc duscharge)[49]。大批量生产的CNTs与大面积溶解处理技术兼容，这可以用来制作高纯度和直接将CNT沉积到荣幸或可伸缩基板上。常用的溶解沉淀技术包括vacuum filtration(真空过淋)[51], spin-coating [52],spray-coating [ 22 ] and inkjet printing. [ 53 ]。碳纳米管之间要**对齐 alignment**，来优化器件[54], LB films[55], 机械剪切技术(mechanical shear techniques)[56],contact and roll printing[57]来实现大批量CNT对齐。此外，我们还开发了一种技术，可以通过在胺基或氮基功能化表面上的自旋涂层同时对CNT 薄膜进行分类和校准[52a, 52c]。分散在绝缘弹性体中的CNT具有半导体的特性[58],但是没有验证系统在应变下的性能。从制造角度考虑，CNTs network更好，有更统一的性能和对于传统的光刻和打印技术更兼容[48a]
#### 2.1.4 Graphene-Based Active Materials 基于石墨烯的活性材料

碳的第二种同素异形体，石墨烯。

[59]石墨烯的结构，[60]载流子迁移率(carrier mobility)$20, 000 cm^2 V^{-1}s^{-1}$ 。基于不同的处理方式，石墨烯可以作为导体和半导体。**[61]对于石墨烯的特性有详细的探讨**

石墨烯可以通过各种适合于制造电子皮肤器件的高产量技术生产，包括CVD体生长和从大块石墨上进行化学脱落（bulk growth with CVD and chemical exfoliation from bulk graphite），在金属表面[62]生长和硅表面[63]外延生长可以大量生产高电气性能的大面积薄膜[62b]。但是要转移到塑料基板上[62a, 65]，这回引入缺陷和限制[64]，同时会限制期间的面积[65].尽管最近提出了大面积转移薄膜的技术[66];也可以通过对剥离的氧化石墨烯篇进行还原反应制造(包括用过化学[chemical 68]，热[thermal 69]，光热[photothermal 70]或电化学[electrochemical 71]来对氧化石墨烯(rGo)进行还原反应),通过此方法可进行低成本，大批量生产石墨烯片，与溶解处理技术兼容，比如spray coating, [ 72 ] vacuum fi ltration, [ 73 ] dip-coating, [ 74 ] spin-coating, [ 75 ] and inkjet printing [ 76 ]，可以直接沉积在大面积，柔性基板上来制作E-skin。

#### 2.1.5 Nanowire (NW)-Based Active Materials 基于纳米线的活性材料

Si，氧化锌(ZnO)，砷化镓(GaAs)，硒化镉(CdSe)等许多材料的纳米线（nanowires）已被用于制造电子皮肤[23].由于其高的生产成本，无论是采用top-down还是bottom-up策略[77]都限制了其在e-skin的应用。由于加工和装配技术的限制，导致不能集成宏观尺度上有序阵列，导致基于NW的器制备受限。虽然单个的NW器件可以通过电极的平版沉积来制造[78]，这个过程耗时而昂贵，不适用于e-skin的制备。

最近在layer-by-layer (LBL) assembly(LBL)[79]，LB films[80]和contact printing[81]方面的进展使集成制造成为可能，在柔性基片上的SiNW器件大小可达1$cm^2$,

[23a]

#### 2.1.6 Organic and Polymer-Based Active Materials 基于有机和聚合物的活性材料

**许多性质可以通过改变分子结构来改变。。。。，似乎promising**

导电小分子(OSs)和聚合去是柔性的，碳基的，π -conjugated（π 共轭） 的材料，更适合于e-skin。常用的小分子半导体包括：acenes[82],oligothiophenes[83]和其他有熔合芳香环的分子，常用的半导体和导电聚合物包括regioregular poly(3-hexylthiophene) (P3HT), [ 84 ] polyaniline(PANI), polypyrrole (PPY), poly(3,4-ethylenedioxythiophene)

poly(styrenesulfonate) (PEDOT:PSS), 和他们的衍生物[85],

[82a, 89]探讨了有机电子。
尽管它们的电子性能还不能与无机半导体相比[86],相比于CNTs, graphene, and NWs，这些材料有更强的加工性，这也是得他们被认为适合于低成本，大面积阵列的制造。另外,尽管一些OSs在长期运行过程中存在栅偏压[87]，通过加入更复杂的电路元件(环形振荡器)[88]或脉冲栅偏压可以缓解这一问题。尽管有这些性能上的缺陷，但OS还是很有吸引力的，因为它们的化学和物理性质是高度可调的，并且可以通过化学合成来控制改变它们的分子结构[90],通过精确的控制沉积条件可以影响参数，比如形态学，形态学（morphology）与电子性能有关[91]。

光敏和水敏问题可以通过分子结构的操纵来解决，从而制造出适用于电子皮肤应用的高度稳定的os[90a, 91]有机分子的溶解度也可以调整，以便与大面积的溶解处理技术兼容,比如spin coating, screen printing, [ 96 ] and inkjet printing. [ 93].

OSs and CPs一个很好的特性是，和那些无机材料相比，他们的机械性能与人的皮肤更兼容。机械依从度取决于分子包裹结构和分子间力[94]。比如有些os对形变几乎零容忍[95]，而一些其他的聚合物却可以很大程度的变形[94b, 99],Poly(2,5-bis(3- alkylthiophen-2-yl)thieno(3,2-b)thiophene) (PBTTT)存在刚性的3维结构，在3%的应变下破裂，而由于其2d的平面结构，P3HT可以承受超过150%的形变，仍然具有好的电气性能。[94b, 99a, 100].P3HT恢复需要与半导体交联提供恢复力[97],将P3HT均匀的分布在PDMS可以实现可拉伸材料[98]OS NWs分布在弹性基体可以提高电气性能[119]

#### 2.1.7 Stretchable Conductors 可伸缩导体

**想弹性基底中注入导电粒子。。。**

尽管导电弹性体作为压敏元件已使用多年，[101]，可拉伸电子产品的普及推动了导电弹性体的快速发展，其特点是适用于高性能电子产品。理想的可拉伸导体在大应变范围内保持高导电性，并在拉伸过程中保证可靠性[15b, 102].

由于弹性体的绝缘特性，导电性通常是通过引入导电材料来实现的。复合导体的性能取决于形态学和弹性基体与导电填充物的交互连接[104],大的填料浓度可以增强弹性模量，减少断裂应变[102, 104a, 105]。所以，高特向异性的填充材料是优势的，它们的低渗滤阈值允许在低的填充浓度下有好的电气性能[104d, 106].
printed[20b, 102, 108]和photo-patterned [ 111 ]的可拉伸道题已经被广泛研究，[32]发明了一种低沉本的打印方法，使用electrospun elastomeric

fi bers 结合 metal NPs打印高可伸缩性的电路。

金属的高导电性，金属球，粉和线（metallic spheres, [ 109 ] flakes, [ 102,108 ] and wires [ 110]）认为在可伸缩导体中有良好的性能。一般来说，通过将预先形成的粒子与基体聚合物混合，金属可以被纳入导电矩阵中[102, 109b]通过使用有机金属前驱体在聚合物基体中生长金属颗粒[32, 111]或低能离子注入[104b, 109b],嵌入弹性通道的液态金属显示了一些最高的性能，但成本高[112],。炭黑(CB)是目前市面上最便宜的导电薄膜之一,但是导电率很低，

只能用于一些特定的应用中，比如压敏橡胶和人造肌肉的电极中[104b]。

CNTs是目前最成功的的可伸缩导体的填充物[ 15b,20b,102,109a,113]。嵌入到弹性介质爽整齐排列的CNTs在300%的应变下，电阻仅有很小的改变[114]

CNTs, AgNWs和聚合物的复合物导电性高达$5710 S \cdot cm ^{−1}$[102].由于碳纳米管聚集强度高，复合材料的制备需要高度优化的混合过程[20b, 102, 113c],使用预定的形态学结构。(113 d,114], 气凝胶[115]和CNTs的其他低密度结构[116]。石墨烯[117]还具有导电性和优异的机械性能，有潜力克服弹性材料固有的蠕变和松弛的缺点.

CPs因其与聚合物基质的相容性和低成本而具有吸引力.[104]讲述了CPs在不同应变下导电性变化，使用CPs的一个挑战是它们对环境条件的敏感性，这导致它们的电特性随时间的推移而退化[118],[121]提出可以通过化学或形态学调整来调节。
### 2.2  Strategies to Achieve Stretchability












