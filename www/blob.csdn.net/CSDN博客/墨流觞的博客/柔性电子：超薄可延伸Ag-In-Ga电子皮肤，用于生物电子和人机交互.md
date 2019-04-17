# 柔性电子：超薄可延伸Ag-In-Ga电子皮肤，用于生物电子和人机交互 - 墨流觞的博客 - CSDN博客





2018年12月25日 22:48:48[墨氲](https://me.csdn.net/dss_dssssd)阅读数：218标签：[柔性电子](https://so.csdn.net/so/search/s.do?q=柔性电子&t=blog)
个人分类：[柔性电子](https://blog.csdn.net/dss_dssssd/article/category/8333955)









![在这里插入图片描述](https://img-blog.csdnimg.cn/20181225165144417.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

### 摘要

~5um厚的电路制作过程：使用台式激光打印机将图案打印在临时的纹身纸上，接下来使用银墨水和共晶镓−铟(EGaIn)液态金属合金涂覆。【The ∼5 μm thick circuit is fabricated by printing the pattern over a temporary tattoo paper using a desktop laser printer, which is then coated with a silver ink and eutectic gallium−indium (EGaIn) liquid metal alloy】

形成的“Ag-In-Ga”路径高导电，并且当适应3D表面是维持着低的导电电阻【The resulting “Ag−In−Ga” traces are highly conductive and maintain low electrical resistivity as the circuit is stretched to conform to nondevelopable 3D surfaces】

通过引入一种新兴的z-axis的导电接口，该结构有磁性排列的EGaIn涂覆的嵌入在PVA(polyvinyl alcohol)中的Ag-In微粒组成，也提出了表面贴装微电子芯片的集成化【We also address integration of surface-mounted microelectronic chips by introducing a novel zaxis conductive interface composed of magnetically aligned EGaIn-coated Ag−Ni microparticles embedded in polyvinyl alcohol (PVA)】

这种“zPVA导电胶”能与管脚尺寸为300um的微芯片形成稳定的点接触【This “zPVA conductive glue” allows for robust electrical contacts with microchips that have pins with dimensions as small as 300 μm】

如果打印在临时的纹身转移纸上，可以通过hydrographic transfer将电路转移到3D表面上【If printed on the temporary tattoo transfer paper, the populated circuit can be attached to a 3D surface using hydrographic transfer】

打印和接口处理都可以在室温下进行【Both printing and interfacing processes can be performed at the room temperature】

我们演示了一些例子，包括人体表皮上用于肌电图信号采集的电子纹身，带有触控按钮的交互式电路，转移到机器假肢上3D打印外壳上的LED和转移到到3D表面上的接近度测量皮肤【We demonstrate examples of applications, including an electronic tattoo over the human epidermis for electromyography signal acquisition, an interactive circuit with touch buttons, and light-emitting diodes transferred over the 3D printed shell of a robotic prosthetic hand, and a proximity measurement skin transferred over a 3D surface】

### introduction

Electronics circuits that interface with the human body or surface of objects can enable functionalities such as acquisition of biosignals, detection of human touch, proximity, temperature, and energy harvesting.[1,2] These circuits can also integrate antennas and display[3] over the surface of the object, and turn them into an interactive reciprocal surface, which has applications in human−machine interfaces (HMIs) and large area displays. Moreover, when integrated over the human epidermis, these circuits enable applications in wearable computing and health monitoring 与人脸表皮集成后，这些电路可用于可计算穿戴和健康监测

[4,5]最近致力于发展新的制造技术来建立集成的，三维材料系统来实现这些功能。【Recent efforts have focused on additive manufacturing techniques to create integrated, three-dimensional (3D) material systems that achieve these functionalities】

这些3D打印技术已经涵盖广泛的材料：
- carbon nanotubes for super capacitors[6]
- ferromagnetic photopolymers for magnetic sensors[7]
- shape memory polymers for “4D printing”[8]
- conductive materials for strain or pressure sensors[9,10]

最近有一些在3D表面[11]或人体[12]打印电子器件和其他功能材料 的示例【There have also been recent demonstrations of printing electronics and other functional materials over an existing 3D surface[11] or on the human body [12]   】

这边说了一下3D打印的缺点，能准确的随着表面形态沉积导电油墨或浆。通常很慢并且没有统一的方法。【Although promising, direct printing over three dimensional surfaces is a complex process, requiring the print head to precisely follow the surface morphology to deposit a conductive ink or paste. This is generally a slow and not a scalable process】

一种可能替代的方法是，打印2D电路，能转移到3D表面上。

这能通过hydroprinting实现，当物体从水缸中移开时，浮在水缸表面的有图案的薄膜被转移到三维物体的表面。【One possible alternative is 2D printing of circuits which can be transferred to a 3D surface。 This can be accomplished through hydroprinting, in which patterned thin films floating on the surface of a water bath are transferred to the surface of 3D objects as the objects are removed from the bath】

但是，将这种技术推广到印刷电子产品可能具有挑战性，因为成功地将2D电路转移到复杂的，可扩展的3D表面要求电路可弯曲和可伸缩。【However, extension of this technique to printed electronics can be challenging because successful transfer of a 2D circuit over a complex, nondevelopable 3D surface requires the circuit not only to bend but also to stretch】

为了克服这一困难，工程师们已经开发出由传感器和数字集成电路芯片组成的“表皮电子产品”，这些产品被集成到一种超薄的载体薄膜中，能够轻易地转移到人体皮肤上[13]【To accomplish this, engineers have developed “epidermal electronics” composed of sensors

and digital integrated circuit chips incorporated into an ultrathin carrier film that can readily transfer to the human skin】

他们就像电子文身一样工作，已经用于：
- biomonitoring [13]
- implantable energy harvesting[14]
- pulse oximetry[3]
- neural interfaces[15]

制造软性物质电子器件开创性的努力主要集中在确定性的电路结构，其中可延伸性主要是利用具有波形或蛇形几何结构的电路布线[13]。【Seminal efforts on fabrication of soft-matter electronics have focused on deterministic circuit architectures in which stretchable functionality is achieved with circuit traces that have a wavy or serpentine geometry】

尽管很有潜力，但是这些方法都要依赖于洁净室光刻或特殊处理过程，因而很难低成本打印或快速原型设计。【However, although promising, these approaches typically rely on cleanroom lithography or specialized processing steps that are difficult to perform with low-cost printing or rapid prototyping methods】

将表皮电子扩展到机器人，HMIs的应用需要继续研究新的材料结构和处理技术，以至于能更广泛的使用。【Extending epidermal electronics to applications in robotics, HMIs require continued exploration of new material architectures and processing techniques that are accessible to a wider user community】

特别的，这些方法应当不依赖于净室制造方法（比如光刻）和高温处理过程。另外，这些电路应当当能够直接与微电子芯片或PCBs连接进行数据处理和传输，而不需要额外的电线连接。【In particular, these approaches should have limited dependency on clean-room fabrication methods (e.g., photolithography) or high-temperature processing steps. Moreover, such circuits should be able to directly interface with microelectronic chips or printed circuit boards (PCBs) for data processing and transmission without the need for external electrical wiring.】

我们利用桌面印刷和水印转移技术展示了一种新的方法，来快速制造类似于纹身的薄膜电路【Here, we demonstrate a novel method for rapid fabrication of tattoo-like, thin-film circuits with integrated microelectronics that utilizes desktop printing and hydrographic transfer techniques.】

通过取消了对微制作，后烧结， 薄膜金属沉积，和平板印刷图案，目前的工作大大简化了步骤。【The current work greatly simplifies these manufacturing techniques by eliminating the need for microfabrication, post sintering, thin-film metal deposition, and lithographic patterning】

与其他制造超薄和表皮电子的方法不同，本方法使用和商用桌面打印机制作，不需要昂贵的处理步骤和自制器材。【Unlike other methods for producing ultrathin and epidermal electronics, fabrication can be performed using a commercially-available desktop printer

and eliminates the need for expensive processing steps or custom-built equipment.】
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181225204603231.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

电路能打印到纹身转移纸(tansfer tattoo paper TTP)或 水纹纸(hydrographic paper)，并转移到身体或3D平面上(Figure 1A)【Circuits can be printed on a transfer tattoo paper (TTP) or hydrographic paper and transferred to the body or a 3D surface】

本任务中使用的TTP有一层超薄(<5um)的载体膜，一种水溶性聚乙烯醇(PVA)层和 背纸组成【The TTP used in this work is composed of an ultrathin (<5 μm) carrier film, a water soluble polyvinyl alcohol (PVA) layer, and backing paper (Figure S1)】

当与水接触时，载体膜从纸上分离，漂 浮在水的表面，载体膜可以在水箱中转移到不同的材料上比如皮肤，衣服，塑料，金属和玻璃 (Figure 1A−D).【When in contact with water, the carrier film separates from the paper, and floats over the surface of the water, which can be then transferred in a water tank over various materials, for example, skin, cloth, plastic, metal, and glass】

电路是通过首先使用桌面打印机在TTP上打印电路模板来创建的。【Circuits are created by first printing a circuit template on TTP using a desktop printer.】

***?有点不太明白过程呢！！！***
**电路可以使用喷墨打印机打印银纳米颗粒墨水实现，接着使用液相的共晶镓−铟(EGaIn 75.5 wt % Ga, 24.5 wt % In, melting point: 15.7 °C)合金覆盖打印线路[16]**

EGaIn 的涂覆使得导电率提高了6个数量级，使得电路对于机械应变的容忍度提高【EGaIn coating increases the conductivity of the printed AgNPs by 6 orders of magnitude and makes the circuits considerably tolerant to mechanical strain】

我们提出了一种新的方法制造“Ag-In-Ga”电路，首先使用打印碳粉(print toner)打印不导电模板，接着用银浆料(sliver paste)覆盖,银浆会选择性地粘附在印刷的墨粉上,作为润湿层沉积EGaIn。【Here, we show a novel method to produce “Ag−In−Ga” circuits, which starts by using a desktop laserjet to print a nonconductive template with print toner and then coat the

template with silver paste. The silver paste will selectively adhere only to the printed toner ink and function as a wetting layer on which to deposit the EGaIn】

在两种情况下，产生的电路高导电，在转移后，能完美贴合不可展开的3D表面，而导电率不会降低【In both cases, the resulting circuits are highly conductive, and after the transfer, they conform to nondevelopable 3D surfaces without losing their conductivity】

与纳米银颗粒油墨相比，这种方法简单快捷，无需对喷墨打印机进行修改、清洗、墨盒充墨，也无需由于堵塞而 对墨盒进行周期性的滤墨、声滤、更换或清洗等操作。【When compared to AgNP inks, this method is simpler and faster because it eliminates the need for modification of an inkjet printer, cleaning, and filling the cartridges with the ink, as well as periodical ink filtering, ink sonication, and replacement or cleaning of the cartridge due to clogging.】

当然，导电浆料使用微颗粒和微片，通过改变填料含量、填料尺寸和载体材料等参数，它们在导电性和附着力方面更容易生产和定制。【Also, conductive pastes are made with microparticles or microflakes; they are considerably easier to produce and customize in terms of conductivity and adhesion, by changing parameters such as the filler content, the filler size, and the carrier material】

因而与AgNP相比，种类更广泛，价格更实惠。【Therefore, they exist in a wider variety and are more affordable when compared to AgNP inks】

然而之前，他们的沉积需要模板和后烘干步骤，这与喷墨打印相比是一个缺点。 所有的这些要求在我们的方法下都不需要了【However, previously their deposition had required stencils and a postbaking step, which is a disadvantage compared to inkjet printing. Both of these requirements have been eliminated with the current technique.】

尽管提出的方法促进了互连电路的打印，这些电路的实际应用仍然依赖于发展方法来与外部电路的更稳定电连接和与印刷图案与微芯片直接接触.在本文中，我们引入了新的Z-zhou的导电接口来解决这两个问题(Figure 1D)【Although the presented method significantly facilitates printing of the circuit interconnects, actual application of these circuits still depends on development of methods for robust electrical interfacing with external circuits and direct interfacing of the printed pattern with microchips. In this article, we address both by introducing a novel z-axis conductive interface (Figure 1D)】

接口由嵌在PVA薄膜的垂直排列的磁颗粒组成。这种凝胶状的“zPVA胶”可用于微电子器件和柔性电路与薄膜表面的连接。【The interface is composed of vertically aligned magnetic particles embedded in a PVA film. This gel-like “zPVA glue” can be used for interfacing the

thin-film skin with microelectronics and flex circuits】

zPVA的凝胶到固体转化是通过溶液中水分的蒸发实现的，可以在室温下进行【The gel to

solid transformation of zPVA is achieved by evaporation of water in the solution, which can be performed at room temperature】

与之前基于PDMS的各向异性的导电体相比[17,18]，zPVA可以贴合到更广泛材料上，包括基于PVA的转印纸。【Compared to previous PDMS-based anisotropic conductors,[17,18] zPVA adheres to a wider range of materials, including the PVA-based transfer paper.】

聚乙烯醇：可用于制作胶水[https://zh.wikipedia.org/wiki/%E8%81%9A%E4%B9%99%E7%83%AF%E9%86%87](https://zh.wikipedia.org/wiki/%E8%81%9A%E4%B9%99%E7%83%AF%E9%86%87)

### background

我们制造可拉伸的电子是建立在之前"双极性"材料架构的基础上的，该方法将EGaIn与金属模板结合创造机械鲁棒性高的电路。【Our approach to thin-film stretchable electronics builds on previous efforts in “biphasic” material architectures that combine EGaIn with metallic templates to create mechanically robust circuits】

在这些方法中，EGaIn涂覆在溅镀沉积的金属(Au, Ag, Au)薄膜上，使用光刻[19-21]或紫外激光微加工[22,23]来刻画金属薄膜图案。【EGaIn is coated on a sputter-deposited thin film of the metal (Au, Ag, or Cu) that is patterned using either photolithography or UV laser micromachining】

其他的努力集中在能够打印在可拉伸基底上的EGaIn纳米颗粒的合成[24]和“机械烧结”来创造导电电路[25,26]。【Other research efforts focused on synthesis of EGaIn nanoparticles that can be printed on a stretchable substrate and “mechanically sintered” to create conductive traces】

我们改进了这种方法，通过使用任何标准的商用喷墨打印机将图案打印在TTP薄膜上。【We improve upon this past work by introducing “facile” fabrication techniques that enable patterning on a TTP thin film using any standard commercial desktop printer】

EGaIn辅助的AgNP的室温烧结的分析本文没有涉及，已经在[16]中涉及。相反的，我们聚焦在能提供快速制造且不需要洁净室或特殊处理仪器的技术上。【The analysis of the EGaIn-assisted room temperature “sintering” of the AgNP ink is not the focus of this article and was covered in a recent companion study.16 Instead, we focus on techniques that allow for rapid fabrication of thin-film circuits without dependency on a clean-room or specialized processing equipment.】

我们展示了一些人机交互的应用和如何快速打印并将这些薄膜电路转移到3D表面上，并且使用它作为感知皮肤。【As a case study, we demonstrate applications to human−machine interaction and demonstrate how one can be rapidly printed and transfer these thin-film circuits to 3D surfaces, and use it as a sensing skin】

特别的，我们展示了使用hydroprinting方法可以将电路转移到不可扩展的3D表面上(比如，球形)和有锐利边缘的表面。【In particular, we show that the circuits can be transferred over complex nondevelopable 3D surfaces (i.e. spherical features), and over sharp edges using hydroprinting methods.】

我们也展示了这种电路能自粘附于人体表皮，总肌肉活动中获取生物电势，比如肌电图(EMG)。【We also demonstrate that such circuits can self-adhere to the human epidermis

for acquisition of biopotentials from muscle activity, for example, electromyography (EMG)】

尽管由PEDOT:PSS 聚合物【27】和打印银油墨【28】的自由纳米薄膜已经展示位可以在水中释放的薄膜导体，本工作利用“Ag-In-Ga”互联结构可伸缩和能转移到更复杂表面，包括180度的折叠的优势，比前者更优势。【Although free-standing nanofilms composed of the poly(3,4-ethylenedioxythiophene)/poly(styrene sulfonate) (PEDOT:PSS) polymer and printed Ag ink have been previously shown as thin-film conductors that can be released in water, the current work improves over its predecessors by taking advantage of “Ag−In−Ga” interconnects which are stretchable and allows the transfer to be performed over more complex surfaces, including 180° folding.】

除此之外，使用zPVA各向异性导体密封电路能允许表面填充SMD组件和与外部电路形成鲁棒性更高的电连接。【In addition, sealing the circuit with a zPVA anisotropic conductor allows the surface to be populated with SMD components and enables robust interfacing with external electronics】

最后，简易的制造技术允许粘性银胶料的无模板处理，能够快速制作定制电路的原型，比如用于生物监测应用的多电极模式的保形的和不易察觉的纹身状电路。【Finally, the facile fabrication technique presented here allows stencil-free disposition of viscous silver pastes that permits rapid prototyping of customized circuits, such as multielectrode patterns for conformal and imperceptible tattoo-like circuits for biomonitoring applications.】

### results

制造过程分为四个步骤：
- circuit printing
- materials postprocessing
- microchip integration
- hydrographic transfer

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181226120155635.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

在第一阶段，Figure 2A

使用台式激光打印在TTP薄膜上打印图案，然后通过银胶料或环氧树脂的选择性沉积制造电路。为了提高导电性和延伸性，电路结社覆盖一层EGaIn，步骤如Figure 2A和Movie S1中展示，描述在**materials and methods**章节。【a circuit is produced through selective deposition of silver paste or epoxy on a TTP film after printing the pattern by a desktop laser printer (Figure 2A). In order to improve electrical conductivity and stretchability, the circuits are then coated with EGaIn using the steps presented in Figure 2A, Movie S1, and described in the Materials and Methods section】
液态金属(LM)合金当暴露在弱(2  wt%)的冰醋酸水溶液或盐酸蒸气时会选择性的润湿Ag痕迹上(Figure 2A Movie S1)，这就是说，在初始沉积之后，EGaIn氧化物会浸润TTP表面，包括打印和非打印区域，但是，使用

CH3COOH或 HCl处理过的EGaIn会移除表面氧化物($Ga_2O_3$),这回导致未涂覆的TTP去浸润(由于未氧化EGaIn的表面张力大)和对于银颗粒有更强的粘附能力。氧化物的去除也可以使用氢氧化钠溶液，其中，银颗粒就像“锚点”一样防止LM去浸润，和冲洗掉。【The liquid metal (LM)

alloy selectively wets to Ag traces when exposed to a weak (2 wt %) acetic acid solution or hydrochloric acid vapor (Figure 2A, and Movie S1). That is, after the initial deposition, EGaIn oxide wets the TTP surface, including the printed and nonprinted areas. However, treating EGaIn with CH3COOH or HCl removes its surface oxide (Ga2O3), which results in dewetting from the uncoated TTP (because of the high surface tension of nonoxidized EGaIn) and stronger adhesion (and possible alloying) to the silver particles. Oxide reduction can also be performed with aqueous solutions of NaOH,29 in which case the silver particles act as “anchoring points” that prevent the LM from dewetting and rinsing away.】
得到的“Ag-In-Ga”电路使用zPVA复合物覆盖密封(Figure 2B),有垂直排列的嵌在PVA胶中的导电颗粒组成(Figure 1D)。【The resulting “Ag−In−Ga” circuit is sealed with a coating of zPVA composite (Figure 2B), which is composed of vertically aligned conductive particles embedded in a PVA gel (Figure 1D)】

封口处只在垂直厚度方向上导电，使的在电路终端和表面组装微电子和外部电路的引脚间形成电连接。【The seal is conductive only through its thickness and enables the formation of electrical vias between the terminals of the circuit and pins of surface-mounted microelectronics and circuit board connectors.】

使用镀银镍颗粒与聚乙烯醇混合制备复合材料，使用thin-film applicator(ZUA12-ZEHNER)将混合物薄膜覆盖到电路上，组件安放在薄膜上。【The composite is prepared by mixing Ag-coated Ni particles with PVA. A thin film of the mixture is applied over the circuit using a thin-film applicator (ZUA12-ZEHNER), and the components are placed over the film】

**也可以通过丝网印刷或旋涂将zPVA薄膜沉积到电路上**【Alternatively, screen printing or spin-coating can be used to deposit a thin film of the zPVA mixture】

将电路放在磁铁上，当水从薄膜中蒸发时。通过这种方式，Ag-Ni颗粒将垂直排列，只能在垂直方向上导电。【The circuit is then placed over a magnet, while the water is being evaporated from the film. In this way, the Ag−Ni particles align to form vertical columns that conduct electricity only through the film thickness】

这能用于与柔性电路，微电子组件和其他的PCB板电连接(Figure 2B), Movie S5展示了由LED构成和与其他柔性PCB电连接的电路的转移处理过程【This can be used to interface flex circuits and

microelectronics components and other PCBs (Figure 2B). Movie S5 shows the transferring process for a circuit populated by light-emitting diode (LED) chips and interfaced with the flexible PCB (FPCB) through zPVA.】

在接触垫或Ag-Ni颗粒上涂覆一层EGaIn薄膜会进一步改善接触电阻和机电一体性。【Contact resistance and electromechanical integrity can be further improved by coating the contact pads or Ag−Ni particles with a thin layer of EGaIn】

上述方法制造的电路能够使用hydrographic transfer (也称为 water transfer printing 水转印，immersion printing )转移到其他表面上(Figure 2C 和4A)。【Circuits produced with the aforementioned method can be transferred to other surfaces using hydrographic transfer, also referred to as water transfer printing or immersion printing 】
- 首先，将制作完的电路完全进入到水缸中，【First, a host object is fully immersed in a water bath or tank.】
- 接着，“Ag-In-Ga”漂浮在水表面【Next, the “Ag−In−Ga” circuit is suspended on the surface of a water bath】
- 几秒后，TTP的水溶性中间层基质溶解，5μm厚的载体薄膜从背纸分离【After some seconds, the watersoluble middle layer of the TTP substrate dissolves and separates the 5 μm carrier film from the backing paper】
- 当物体从水中提出时，载体膜将依附到物体上，并贴合物体表面【The carrier film then clings to the object and conforms to its surface as the object is subsequently lifted out of the water bath.】

由于载体膜很薄，并且具有延伸性，薄膜能够很紧密的依附表面的形状，甚至能够支持180度的自折叠和弯曲(Figure 4A)【Because it is thin and stretchable, the film is able to follow closely the shape of the surface and can even support 180° self-folding or bending around a thin part】
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181226143424334.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

当从水缸中移除后，电路和基底将被干燥，转移之后，可以喷涂一层塑料保护膜密封。【After removal from the bath, the substrate and circuit are allowed to dry. After transfer, the circuit may be spray coated with a protective plastic sea】
基于不同的转印纸，转移过程可能有略微不同，比如水纹纸(hydrographic paper)与TTP相比，需要不同的步骤,对于水纹纸而言，载体膜由聚乙烯醇(PVA)和聚醋酸乙烯酯(PVAc)制成，没有单独的水溶性层。此时，在浮在水之前需要手动除掉支撑层。【The procedure for the film transfer differs slightly based on the type of the transfer paper used. As an example, the hydrographic paper requires different steps than the TTP — for the hydrographic paper, the carrier film is made out of PVA and polyvinyl acetate (PVAc), and there is no separate water soluble layer. In this case, the backing paper is removed manually prior to floating over water】

除此之外，当与电路结合时，物体表面应从上面靠近薄膜和浸入水中。制作和转移过程在Movie S1中展示【In addition, the host surface should approach the film from above and enter the water when bonding with the circuit. The fabrication and transfer process is also shown in Movie S1】
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181226144632328.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)
使用激光打印机打印测试电路，接下来是Ag和EGaIn沉积。Figure 3A，等宽的线，之间的间隙从50到1000um，被打印，之后是Ag和EGaIn沉积。从图中可以看出，能够打印宽度为200um线。小的线间隙会导致相邻线之间预想不到的链接【In order to establish the patterning resolution, a test circuit was printed using a laser printer, followed by the Ag and EGaIn deposition, as previously described. Referring to Figure 3A, lines with equal width and spacing from 50 to 1000 μm were printed and observed after deposition of Ag epoxy and EGaIn coating. As can be seen in the figure, this technique is able to successfully print lines with a pitch of 200 μm. Smaller spacing resulted in unintended connections between adjacent lines】

Figure 3B，GaIn涂覆的Ag导线在电导率上至少会增加6个数量级。

为了证实这一现象，测试了激光打印和Ag沉积之后10个样本的导电率

在沉积EGaIn后，不导电的Ag胶料导线的表面电阻为0.13 $\Omega /$□（10次测量的平均值），在这些样本中，Ag电路在EGaIn沉积之前在40摄氏度下处理2h。

这样的话，Ag会粘附在下面的打印墨粉上，多余的Ag可以使用不起毛的布去掉。或者，Ag胶料可以在室温下干燥24h。如果加热到更高的温度(比如80摄氏度)，在沉积EGaIn之前电阻为0.80$\Omega /$□，沉积后为0.16$\Omega /$□。

在低温下干燥，Ag和打印电路与其他未打印的基底之间的键合力有很大的不同。这可以用一块布使用很轻柔的力，在不损坏电路的情况下除去过量的银【there is a significant difference between the bonding forces of Ag over the printed circuit and rest of the substrate. This makes it easy to remove the excessive Ag with a piece of cloth and mild mechanical force and without damaging the circuit.】

在更高的温度下干燥，会增加Ag与整个基底的键合力，需要额外的力，会损坏电路，除此之外，载体薄膜在高温下回由于热膨胀而变形，会影响薄膜的基底转移。【Curing at higher temperature improves the bonding of the deposited Ag with the whole substrate, which makes it necessary to apply an additional force, which can damage the circuit. In addition, at higher temperature, the carrier film deforms through thermal expansion, which affects the subsequent transfer of the film】

为了测量zPVA中z轴方向的电阻，我们在玻璃上沉积200,300,400um后的薄膜，当干燥后，从玻璃上玻璃脱落。【In order to characterize the z-axis resistance of zPVA, we deposited films of 200, 300, and 400 μm thickness over glass and then released the film by peeling it from the glass after it dried.】

3mm直接的圆形接触垫放在薄膜两端，使用四端子法测量其电阻。Figure 3D展示了10次测量的平均结果。【Two pieces of the flex circuit with round contact pads of 3 mm diameter were placed on both sides of the zPVA film, and the resistance between them was measured with fourterminal sensing. Figure 3D shows average results for 10 measurements】

正如所期望的，电阻随着厚度增加而增大。

在另一项测试中，我们将200um的zPVA薄膜直接沉积在Cu基底上，干燥后，之后使用激光图案化的模板在zPVA薄膜上喷涂直径3毫米的EGaIn圆(Figure S4)【In another test, we deposited a 200 μm film of zPVA directly over a copper substrate and allowed it to dry. Afterward, we spray-coated 3 mm diameter circles of EGaIn over the zPVA film, using a laser-patterned stencil (Figure S4】

喷涂沉积使用与液体金属(LM)相似的雾化液滴,使用与文献30相似的喷涂枪。【LM similar, using a spray gun similar to the method presented in ref 30】

此时Cu−zPVA−Cu的电阻为0.86$\Omega$,Cu−zPVA−LMd的电阻为0.57$\Omega$【Within this setup, the average value of z-axis resistance was reduced from 0.86 Ω for the Cu−zPVA−Cu    interface to 0.57 Ω for the Cu−zPVA−LM interface (Figure 3E)】

这可以由 Ag覆盖的Ni例子的数量来解释，这些颗粒相互连接，在zPVA两边形成连续的导电通路。而且由于液体金属合金填充接口形成更紧密的接触，电阻会进一步下降。当将Cu基底替换为导电纤维(Cu-Ni涂覆的无纺布-3M CM-3490)时，电阻减小了约10倍(平均0.03$\Omega$, Figure 3E)【This improvement can be explained by the number of Agcoated Ni particles that are able to connect and form a continuous electrically conductive pathway between the opposite sides of the zPVA film. Such conductivity is aided by the ability of the LM alloy to fill the interface and form a more conformal contact. When replacing the Cu substrate with a conductive fabric (Cu/Ni-coated nonwoven fabric-3M CN- 3490), the resistance of the z-axis  interface was further reduced by over a factor of ten (average = 0.03 Ω; see Figure 3E).】

这种戏剧性的降低可以解释为镀镍导电织物的铁磁性能，当zPVA在磁铁上干燥时，Ag覆盖的Ni与织物中的Ni形成紧密粘附，促进导电  【This dramatic enhancement can be explained by the ferromagnetic properties of the nickel coating of the conductive fabric. When zPVA is drying over the magnet, the Ag-coated Ni beads can make strong adhesion with the underlying Ni coating, which contributes to better conductivity】

为了特使电路在机械压力下的电学表现，做了一系列机电测试(Figure 3F).【In order to characterize the electrical behavior of these circuits under mechanical strain, we performed a set of electromechanical tests (Figure 3F).】

测试包括当受到单轴向应变时测量电路的相对电阻。【The test includes measuring the relative resistance of the circuits when subject to uniaxial strain】

一种是在TTP上打印“Ag-In-Ga”，使用上述提到的方法，接着夹在两个PDMS层中(Figure 3ii),制作的更详细信息在Materials and Methods中。结果显示样本最大可以承受73.1%(std=4.7%)的形变。当与zPVA集成后(Figure 3F(ii))，值降低为56.7%（std=4.2%）【One set of tests includes samples of printed  “Ag−In−Ga” over the TTP, which was prepared by the method referred above, and then sandwiched between two PDMS layers (Figure 3F(ii)). More information on sample preparation can be found in the *Materials and Methods* section. Results show that “Ag−In−Ga” samples can withstand a maximum strain of 73.1% (std = 4.7%). When integrating zPVA (Figure 3F(iii)), this value reduces to 56.7% (std = 4.2%).】

与预期一样，由于zPVA和TTP的杨氏模量不同，zPVA试样的应变率较低。所有的样本在zPVA处破裂。【As expected, the sample with zPVA breaks at a lower strain rate because of the difference in the Young’s modulus of zPVA and TTP. All these samples broke at the zPVA interface】

在60%的形变下，电阻的变化不算很大(R/$R_0$=1.5, std=0.22)。这对于可伸缩数字电路是一个大的优势。【Also, the changes on the resistance of samples against strain is the modest (R/Ro = 1.5, std = 0.22), at 60% strain. This is an important beneficial factor for application in stretchable digital circuits】

最后，“Ag-In-Ga”电路与水接触时有很强的鲁棒性，Figure S3展示了漂浮在水面上的功能性电路。Movie S4展示了将电路浸入水中，当从水中取出时仍具有功能性。【Last, “Ag−In−Ga” circuits are very robust when in contact with water. Figure S3 shows a functional circuit floated on water and Movie S4 shows a circuit that is immersed in a water tank and remains functional after being removed from water.】

### applications

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181227105526232.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

Figure 4提出两个点入用于机器人应用和人机交互的例子。【Figure 4 presents two examples of how the circuits can be used for applications in robotics and human−machine interaction】

两个电路都设计用于提升对义肢的控制。【Both circuits are designed to improve the user control over a prosthetic hand】

正如之前所说的，尽管义肢的机电一体化在快速发展，也就是说，一个义肢使用者无法完全控制义肢所有的抓取姿势[31].【As we previously reported, despite rapid advances in the mechatronics of prosthetic hands, the HMI still has limited functionality, that is, the amputee does not have enough control inputs to operate all available grasping postures of an advanced prosthetic hand】

在这，我们展示了水印电子时如何以低成本方式提升HMI的功能。【Here, we show how hydroprinted electronics can improve HMI functionality in a low cost manner】

在一种实现方式中，我们创建了一种电子纹身，纺织手臂上，收集EMG信号(Fgure 4B). 使用者能使用“EMG 文身”来控制机器假肢。【In one implementation, we can create an electronic tattoo that is placed over the human forearm and collects EMG signals (Figure 4B). The user can use the “EMG tattoo” to control a robot hand prosthetic】

在第二种实现方式中，义肢表面覆盖一层LED提供视觉反馈和一个触摸接口俩出入额外的控制信息：【In second implementation, the surface of the prosthetic is coated with LEDs for visual feedback and a tactile interface for additional control inputs】
- (i)closing further the hand,
- (ii) emergency opening,
- (iii) changing the closing pattern

Figure 4A展示有触摸输入的电路是如何转移到3D打印的义肢外壳上的，包括180度的弯曲。【Figure 4A shows how the circuit with tactile inputs is transferred over the 3D printed shell of the prosthetic hand, including a 180° bend】

Figure 4B展示了带有采集电极的“电子纹身”是如何施加到人手的。【Figure 4B demonstrates how the “electronic tattoo” with acquisition electrodes is applied over the human hand】

Figure 4C和Movie S2展示了EMG和触摸输入的应用。【Figure 4C and Movie S2 demonstrate application of both EMG and tactile input.】

注意，不需要改变3D打印的结构来适应电路。电路由桌面激光打印机制造，根据设计，维度和图形，都是高度可定制的。【Note that there is no need to change the original design of the 3D printed shell to accommodate the circuit. Both tactile inputs and surface mount LEDs adhere to the surface and function as they are programmed. Because circuits are produced by desktop laserjet printing, they are highly customizable in terms of design, dimensions, and graphics】

成本低，能够在~30min内快速制造和应用。【They are also low cost and can be rapidly fabricated and applied in ∼30 min.】

使用EMG感知器和触输入来控制手在Movie S2中展示。【Control of the hand both with the EMG sensor and the tactile input are also demonstrated in Movie S2】

Movie S5展示了转移到手前臂的电路(Figure 1C)，电路上有SMD LEDs和FPCB接口，其电线使用焊锡焊接。【Movie S5 shows an example of a circuit transferred to the human forearm (Figure 1C). The circuit is populated with SMD LEDs as well as a FPCB interface to which cables are soldered】

![在这里插入图片描述](https://img-blog.csdnimg.cn/20181227131833157.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Rzc19kc3Nzc2Q=,size_16,color_FFFFFF,t_70)

另一个例子在Figure 5和Movie S3，三个接近传感器和9个 led集成到薄膜上。当各自的接近传感器被激活时该行上的3个LEDs将会亮。灯的亮度可以有手到薄膜的距离控制。【Another example is presented in Figure 5 and Movie S3, which shows three proximity sensors and nine LEDs to the film. Each row of three LEDs turns on when the respective proximity sensor is activated. Also, the light intensity can be controlled by changing the distance of the hand from the surface】

这些薄膜电路寿司安使用激光打印机打印，接着是Ag胶和LM沉积(Figure 5A)【These thin-film circuits were first printed with a laser printer, followed by Ag paste, and LM deposition (Figure 5A)】

Figure 5B展示了转移之后的电路。当添加触觉输入和LEDs，表面的3D打印前面变为用户界面。当必要时，处理电路可以隐藏在后面。Figure 5D展示了用作与E-skin接口的zPVA，中间弯曲电路和后面的PCB【Figure 5B shows the circuit after the transfer. By adding the tactile inputs and LEDs, the 3D-printed front side of the surface is turned into a reciprocal user interface, and the processing circuits can be hidden on the backside when necessary. Figure 5D shows zPVA for interfacing the applied E-skin, an intermediate flex circuit, and the backside PCB】

当与水接触后，水纹纸的PVA薄膜称为凝胶状态，其中携带者“Ag-In-Ga”电路。PVA会适应物体表面和纹理，当干燥后形成永久的连接。【When in contact with water, the PVA film of the hydrographic paper transforms to a gel state, which carries the “Ag−In−Ga” traces. The PVA gel conforms to the host surface and texture, and achieves a permanent bond when dried.】

在这种情况下，电路成为物体的一个不可分离的集成部分【In this case, the circuit becomes an integrated part of the host that cannot be peeled off】

**为了增加耐穿性和抗湿性，在电路上通过旋涂增加其他聚合物，Movie S1。**【For increasing the resistance to wear and humidity, other polymers can be coated over the circuit, for example, by spray coating, as can be seen in Movie】

PVA与大多数日常使用的材料，比如木材，纸，纤维和大多数的塑料和金属。【PVA achieves a strong bond with most of the daily use materials such as wood, paper, fabric, and most plastics and metals】

### Discussion

水印电子技术对我们在机器人、机器和其他3D物体上制造和安装薄膜电子技术有潜在的革命性影响【Hydroprinted electronics have a potentially transformative impact on the way we fabricate and mount thin-film electronics onto robots, machines, and other 3D objects】

我们选用EGaIn作为LM是由于其无毒和

能与阴离子结合形成半固体物质，展现出好的导电性和低的机电耦合效应。这种室温下阴离子金属熔化的方法不需要热处理或热烧结[15].尽管需要在高温下诱导银胶料或环氧树脂的导电性，但这会永久性的损坏TTP基底。【We have selected EGaIn as the LM because it is nontoxic and fuses with silver particles to form a semisolid substance that exhibits high electrical conductivity and low electromechanical oupling. This room-temperature method for metallic fusion of silver particles eliminates the need for heat treatment or thermal sintering.15 Although elevated temperatures are typically required for inducing electrical conductivity in silver pastes or epoxies, they can permanently damage the TTP substrate. This postprocessing results in a mechanically

robust circuit that can be populated with surface-mounted electronics and transferred to a 3D surfac】
zPVA各向异性的导电体。商用的各向异性导电体(3M z-axis 薄膜)，z-轴电阻为~0.2$\Omega$

skin-like structures with gas sensors,【32】 temperature sensors,【33】

proximity, and pressure sensing.【34−36】

### 结论

Epidermal electronics and E-skins represent promising technologies for more ubiquitous physical human−machine interaction. We present a rapid, low cost, and highly customizable approach to create ultrathin E-skins that can be transferred to natural human skin or the surface of rigid 3D objects. To demonstrate its potential role in robotics, we presented several applications involving skin-mounted biopotentials

acquisition and control of a robotic hand prosthesis. Because the circuits are thin and stretchable, they can be transferred to nondevelopable features like the surface protrusions shown in Figure 5. This transfer is accomplished with hydrographic printing, which has been popular for visual design and decorative arts in industries like automotive. Although promising, the extension of hydrographic transfer to printed electronics is still in its nascent stages and requires further study on factors like wear resistance and long-term durability. Moreover, the zPVA interface requires further

characterization of its mechanical properties and electrical compatibility with a wider range of materials.
### 实验和材料










