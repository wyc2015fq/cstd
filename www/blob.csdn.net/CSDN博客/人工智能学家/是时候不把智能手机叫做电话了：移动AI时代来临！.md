# 是时候不把智能手机叫做电话了：移动AI时代来临！ - 人工智能学家 - CSDN博客
2018年02月13日 00:00:00[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：131
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/f84kJBXzrBWKd7T3WP8omicgs6AxNX8lesPIricnQWnnwy1pu38AnWqpBbv1q86LtDhemQWIW2pOsA2wWv5OQMdw/640?wx_fmt=png)
*来源：**EET电子工程专辑*
*概要：全新的iPhone X集成了用于人脸识别的神经引擎，但这仅仅是一个开始。*
全新的iPhone X集成了用于人脸识别的神经引擎，但这仅仅是一个开始。嵌入式神经引擎和专用智能处理器在边缘设备上实现了人工智能(AI)，打破了对于云的依赖。边缘处理的好处包括减少延迟，全网络覆盖，增加了隐私和安全性，并减少了与云端的通信，从而降低了成本。正因为具备上述优势，移动设备可以利用人工智能去实现不久前还只能出现在科幻小说中的场景。
### **过去的机器现在是实时数据处理中心**
我在参加我们的年度研讨会时，有机会亲密接触到了嵌入式世界中的AI技术。曾经是纯机械的机器，比如汽车、无人机和机器人，现在正变得智能起来，具备了可视、感知、跟踪、分类、检测、识别等能力。现在，这些设备使用计算机视觉和传感器融合来收集和处理数据，并进行实时决策。在某些情况下，比如无人驾驶汽车和无人机，决策是至关重要的，云端处理的延时可能导致难以接受的响应时间。通过片上智能化，这些机器更准确地被定义为了数据中心。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBWKd7T3WP8omicgs6AxNX8le1vv0QZq8SzAPibN2zmCFQhicxFxMev0JZlOb46Zm7c27pDJ5eIwrfQ7A/640?wx_fmt=jpeg)
*图1：AI的边缘化必须实时和低功耗地处理大量信息。(来源于：CEVA)*
无人驾驶车辆就是一个很好的例子，它需要大量的视觉和其它传感器，以及卫星定位信息和各种连接解决方案。它还必须有一个“大脑”完成数据融合和分析。同时基于云的处理和信息也将在自动驾驶功能中发挥作用，因此必须有一个可以瞬间做出决策的车载处理器。即使出现零星的情况，操作车辆也不会有危险是非常关键的。因此，处理器能够处理密集的深度学习计算是必要的，而不是一个可选的功能。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBWKd7T3WP8omicgs6AxNX8levUYPjVJibks8bfib0BWibZ2JhAqD9OicYpNxlZyALYUAUoTDah9Q8Xmsxg/640?wx_fmt=jpeg)
*图2：汽车上的AI边缘处理要求高水平的车载智能。(来源于：CEVA)*
### **神经网络的边缘处理正在成为主流**
在智能手机领域，苹果通常是一个新增特性被作为主流必备还是利基市场(niche-market )配件的试金石。随着苹果新旗舰iPhone X的发布，手机上配有一个专门的神经引擎是人工智能边缘处理的一件大事。如同先前的预测，这意味很快每个带有摄像头的设备将包括一个视觉DSP，或其它专门的神经网络处理器。
iPhone X里的神经引擎实现了Face ID技术，允许用户注视手机从而解锁他们的iPhone。超快的响应时间加上隐私和安全级别的考虑需要所有的识别处理必须在手机上完成。以现在设备上具备的AI能力，肯定会不断推出更多令人兴奋的AI功能。
谷歌也在其最新的旗舰手机Pixel 2中增加了类似的功能，通过称之为像素视觉核心(Pixel Visual Core)的处理器实现。在竞争激烈的智能手机领域，谷歌必须实现差异化。方法之一是为Pixel智能手机的相机搭载出众的软件。但是，图像增强所需的密集计算，单镜头背景虚化效果，提高照片的动态范围，这些功能在目前大部分领先的智能手机自带的标准处理器上不能高效地运行。因此，谷歌决定为这些功能添加第二个芯片，通过添加AI功能可能是另一个主要差异化的体现。华为最近也宣布了在麒麟970中集成了神经引擎，另外还有许多其它公司也加入到竞赛中。
### **基于视觉DSP的引擎如何实现片上智能化？**
虽然边缘处理的好处显而易见，它同时也带来了挑战。难题是如何将可以在巨型服务器上完成的数据运算，放入到一个很小的手持设备中，同时电量还被消耗在许多其它处理任务上。这就是视觉DSP对于能否成功实现边缘AI处理至关重要的原因。精简和高效，但强大的矢量化性能，让DSP处理器成为完成神经引擎工作负载的最佳选择。
另一个挑战是如何将现有的神经网络移植到嵌入式DSP环境里。这可能会消耗大量的开发时间，代价变得非常昂贵。但是自动化工具链可以支持“按键操作”，一站式服务将网络的分析和优化转换到嵌入式环境中。对这样的工具而言，覆盖大量最先进的网络是非常重要的，以确保任何网络都可以很容易地优化并运行在嵌入式设备上。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBWKd7T3WP8omicgs6AxNX8leK37RZXiazxzbCe5SUOaOu9mAmzGCNGDtyIXW3YGStDc3Y3wCmZaQJVg/640?wx_fmt=jpeg)
*图3：Faster RCNN——CEVA完整的自动网络生成器可以有效的降低带宽和保持比特精度。(来源于：CEVA)*
一旦移植和优化过程完成后，通常会对输入数据进行下采样，从而以最少的信息丢失完成更快速的处理。例如在CEVA的全自动网络生成器Faster RCNN的流程中，我们有两个处理阶段，区域建议(proposal regions)和区域分类(classify regions)。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBWKd7T3WP8omicgs6AxNX8le1RwicsMKJtsfXG4Xs1swibG45PicuiaCzFKVicIsSnmFPKbElFFSoic9W9tg/640?wx_fmt=jpeg)
*图4：Faster RCNN处理流(来源于：CEVA)*
CEVA-XM家族处理器是超低功耗的视觉DSP，非常适合完成此类工作。通过添加CEVA-CNN硬件加速器(HWA)可以更进一步提升性能，加速神经网络处理(例如Faster RCNN)。从图5中可以看到，CEVA的第五代视觉处理器CEVA-XM6，相比前一代CEVA-XM4性能有了显著改善。添加CEVA-CNN硬件加速器则使性能又向前迈进了一大步。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBWKd7T3WP8omicgs6AxNX8leZlpnO2ibrzWBibG5iceHYKULkusRNOFUgwtvibBVAQ89FqyVtzEAOEsjVA/640?wx_fmt=jpeg)
*图5：使用CEVA-XM视觉DSP家族后Faster RCNN的性能。(来源于：CEVA)*
基于深度学习的人工智能为手持设备带来了无尽的机会：通过图像增强获得DSLR质量的照片，增强和虚拟现实应用，环境感知、避让和导航、检测、跟踪、识别、分类、分割、映射、定位、视频增强等等。我们手掌中拥有这样的力量，看起来智能手机的通话功能就微不足道了。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBWKd7T3WP8omicgs6AxNX8lexQwRJs9o6xicL71NcibcuOSsJVGSfNiaKxyYBfRXA3yicibqibrRKiccvusVg/640?wx_fmt=jpeg)
*图6：AI支撑的可视化智能应用。(来源于：CEVA)*
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。由互联网进化论作者，计算机博士刘锋与中国科学院虚拟经济与数据科学研究中心石勇、刘颖教授创建。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)
