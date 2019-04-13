
# OmniVision的CMOS 图像传感器技术发展路线 - zouxy09的专栏 - CSDN博客


2012年08月22日 11:27:31[zouxy09](https://me.csdn.net/zouxy09)阅读数：10278


**OmniVision的CMOS 图像传感器技术发展路线：**
zouxy09@qq.com
[http://blog.csdn.net/zouxy09](http://blog.csdn.net/zouxy09)



**1、从OmniPixel1，到OmniPixel2，再到OmniPixel3**
|技术
|推出时间
|像素尺寸（um）
|CMOS工艺（nm）
|结构
|
|OmniPixel-1
|2004.8

|FSI
|
|OmniPixel-2
|2005.9
|2.2
|130
|FSI
|
|OmniPixel-3
|2007.5
|1.75
|110
|FSI
|
|OmniBSI
|2008.5
|1.4
|110
|BSI
|
|OmniBSI-2
|2010.2
|1.1
|65
|BSI
|
**OmniPixel-1**
OmniPixel technology enabled the first generation of CMOS image sensors that deliver the light sensitivity, resolution, color fidelity and low noise of advanced CCD products, while also providing the proven advantages of CMOS-based solutions: low cost, low power consumption, high integration, wide dynamic range and the flexibility of still-image or video capture.
**OmniPixel-2**
Increase the pixel density within a compact sensor size and maximizes the performance of the smaller pixel。
•      An increase in fill factor by 40 percent along with more vibrant and truer-to-life color reproduction.
•      A zero-gap micro-lens design, which eliminates the space between the lenses that are placed over each pixel, leading to a more efficient direction of light to the pixel's active area. This allows the pixels to capture over 20 percent more light, more effectively using the smaller pixel's active area.
•      A pixel design that features higher quantum efficiency and improved full well capacity, which brings significant improvements to the pixel's dynamic range.
**OmniPixel-3**
960 millivolts per lux-second，enhances image capture under very low lighting conditions。The new pixel cuts fundamental noise sources in half, increasing internal pixel gain and quantum efficiency to provide a sensor with truer-to-life color reproduction, superior low-light sensitivity and strong performance in all lighting conditions.
OmniPixel3-HS™ is the latest and most advanced generation of OmniVision's family of front side illumination (FSI) OmniPixel architectures.

**2. OmniVision 推出新型 OmniBSI****TM****传感器架构**
**2.1时间：**
2008年5月27日
**2.2新技术：**
BSI技术颠倒CameraChip传感器的上下层，因此传感器可以通过原本是传感器底层的硅基来收集光线。这种方法与传统的前面照度(FSI)影像传感器不同，在FSI影像传感器上，到达感光区的光线在某种程度上被传感器中能将光子转换成电子的金属和介电层所限制。FSI方法会阻碍光线到达像素或使光线偏离像素，最终降低填充系数，并带来像素之间串扰等其它问题。BSI颠倒了各层之间的安排，从而使得金属和介电层位于传感器阵列的下方，为光线提供了到达像素最直接的通道。这种新方法优化了光线吸收，使得OmniVision能够建立一种超过1.4微米所有性能指标的1.4微米BSI像素，甚至超过大多数1.75微米FSI像素。
OmniVision工艺设计副总裁Howard Rhodes表示：“根据现行设计规则，将FSI像素架构缩小至1.4微米或以下带来了一些真正的挑战，因为金属线和晶体管正不断使像素光圈接近其物理极限——光的波长。要想利用传统FSI像素技术解决这一问题将需要转换为65nm铜工艺技术，这会大大增加生产的复杂性和成本。由于支持三层以上的金属，BSI实现了巨大的生产优势，同时无需转变为更小的工艺节点。这意味着可简化布线，且与FSI传感器相比，芯片尺寸更小，同时不会带来因转变为更小的工艺节点而产生的复杂性和额外成本。”
OmniVision相信BSI技术将会成为未来市场上的主要竞争力，如今OmniVision依然可以使用TSMC的110纳米工艺，但FSI技术需要用到65纳米工艺，因为当FSI的架构进入1.4微米或者更小的级别，金属线和晶体管使得像素间的缝隙接近光波长，这将成为它的物理瓶颈。
![](https://img-my.csdn.net/uploads/201208/22/1345606734_7867.jpg)
**2.3该技术带来的好处：**
Ø  在提供更出色影像质量的同时将其像素尺寸降低到0.9微米，这是数字影像技术不断小型化的关键。
ØOmniBSI架构提供了多项超越FSI的性能改进，包括每单位区域更强的灵敏度、更高的量子效率以及减少串扰和光响应非均匀性，所有这些改进都能够显着改善影像质量。
Ø  显着增高的主光线角度能够实现更短的镜头高度，从而可支持更薄的相机模块，这种模块是新一代超薄手机的理想之选。
Ø  BSI 技术可支持更大的孔径尺寸，从而可实现更低的光圈系数 (f stops)

**3.OmniVision 推出第二代背面照度像素技术**
**3.1时间：**
2010年02月15日
**3.2新技术：**
OmniBSI-2 是 OmniVision 的第二代 BSI 技术，也是第一个利用了 65 纳米设计制程，以 300 毫米铜材料工艺完成的像素技术。在结合了特制的 65 纳米设计制程和新的生产工艺模块之后，这个 1.1 微米 OmniBSI-2 像素技术达到了行业领先的低照度敏感度，同时还可以大量改善暗电流和最大阱容。OmniBSI-2 特制的像素设计规则可以取得更优越的像素排列、更好的像素隔离，并显著降低像素串扰。这些优势大大超越了第一代 OmniBSITM 技术，可以产生更好的图像质量、增强图像的色彩并改善相机的性能。
OmniVision 的工艺工程部门副总裁 Howard Rhodes 博士表示：“将新款的 1.1 微米 OmniBSI-2 像素技术与当前的 1.75 微米 FSI 架构相比，前者的性能远远超越后者；同时前者还与当前正在量产的 1.4 微米BSI技术相当。在我们向 1.1 微米 BSI 像素结构转移的过程中，必须使用台积电最新的 300 毫米铜材料工艺，这个工艺可以显著改善设计规则，使用更多的先进工艺工具，因此获得更严谨的工艺控制，减少缺陷密度。

![](https://img-my.csdn.net/uploads/201208/22/1345606761_3236.jpg)
![](https://img-my.csdn.net/uploads/201208/22/1345606778_8929.jpg)
**3.3该技术带来的好处：**
Ø  灵敏度、量子效率和满井容量得到很大的提升；
Ø  OmniBSI-2 让高解析度成像感应器解决方案产品的外观尺寸不断缩小，凭借较小的 z 高度可制造超薄产品。
Ø  改善成像质量，增强低照度的性能；
Ø  OmniBSI-2 技术还可以应用在大像素成像产品的设计上，从而获得更佳性能，超越当前的 BSI 和 FSI 成像传感器。

