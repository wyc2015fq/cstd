# 图灵奖得主华人高徒发布首款终端AI芯片！64位RISC-V、高度可编程，低功耗 - 知乎
# 



> 李根 发自 凹非寺 
量子位 报道 | 公众号 QbitAI

[还记得OURS吗](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzIzNjc1NzUzMw%3D%3D%26mid%3D2247496071%26idx%3D3%26sn%3D5dcc9532d792c08706c3e588d274d811%26chksm%3De8d046f5dfa7cfe3619df45e472b995208c97aa87dc139f4b6b8b6906a989f63f11767ef45da%26scene%3D21%23wechat_redirect)？

没错，就是那家简写自Optical Universal RISC Systems，基于RISC-V指令集设计AI芯片的创新公司。

而且之前也介绍过，OURS的创始人谭章熹，正是2017年图灵奖得主、计算体系架构一代宗师David Patterson教授的华人博士——中国大陆唯一一个学计算机系统结构/处理器的弟子。
![](https://pic4.zhimg.com/v2-e27853954cf1bdd8b7f2f2e668990fa7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='809'></svg>)
**△**Patterson教授和2位华人弟子：Robert Yung和谭章熹(右）

现在，历时7个月，OURS的第一款芯片——Pygmy人工智能芯片发布。


连David Patterson都感叹，这是一个不可思议的速度。

此外，OURS还正式在深圳建立了中资RISC-V技术公司**睿思芯科**。

AI芯片市场，迎来重要玩家。
![](https://pic4.zhimg.com/v2-4af99222f69bdee37f31ac41f52a6ba7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='999' height='562'></svg>)



## **AI芯片Pygmy**

睿思芯科此次发布的这款Pygmy人工智能芯片，是一款基于64位RISC-V指令集、具有高度可编程、低功耗高能效的优点、可广泛应用于各种物联网终端AI inference场景的芯片。

技术细节上，Pygmy芯片基于最先进的RISC-V开源指令集，采用了多核异构架构，基于台积电28nm工艺。

芯片中的CPU架构是睿思芯科基于RISC-V指令集设计而成，并针对多种AI应用进行了优化。

芯片中的12个高度可编程AI加速引擎，也是基于睿思芯科自定义开发的RISC-V矢量扩展指令集设计而成。

完全采用RISC-V指令架构，能够令SoC芯片各个功能模块更好协调，提升性能。
![](https://pic4.zhimg.com/v2-cbb4f5bf7d716b31d94f29e57789096f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='717'></svg>)



就具体性能而言，Pygmy中的RISC-V CPU具有64位位宽，主频600 MHz，基于RV64G指令集，支持双精度浮点运算，具备乘法器、除法器、开方器等，主控CPU功耗仅为10mW。


对比ARM的同级别芯片，能耗下降85%，面积减少80%，相当于用一般的32位处理器的面积和功耗就实现了64位处理器的性能。

Pygmy中的12个AI内核均为高度可编程，可以支持各种主流AI算法。

各个引擎可以灵活调配，令用户可以在高性能和低功耗间自由配置。

芯片上还搭载了1 MB的SRAM，支持LPDDR4、SPI、UART等数据输入输出模式。

通过前端/后段的全栈设计，Pygmy实现了energy proportional computing，待机设计功耗不超过1mW.

Pygmy芯片中还支持硬件断点，可实时读取关键寄存器；睿思芯科在研制Pygmy芯片的同时，还自主开发了完善的编译器、SDK、工具链，基于GCC、LLVM等开源实现。

这些工作，能够支撑用户更好的进行二次开发。

应用场景方面，睿思芯科方面介绍称，因兼具高可编程性和高能效比，Pygmy能够将高性能的AI算力注入到低功耗的物联网终端的各类AI应用中。

例如智能家居的人机交互、安防监控的人脸识别、无人驾驶的传感器融合等场景。
![](https://pic2.zhimg.com/v2-d138db42637d905a1a7d4132a8703eb5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='611'></svg>)



## **基于RISC-V可编程架构**

作为RISC-V架构下的旗手玩家，睿思芯科自然希望通过产品展现出RISC-V可编程架构的巨大潜力。

睿思芯科创始人谭章熹说，Pygmy还兼具两大特点。

第一，通用处理器的多可编程性，但可比肩ASIC的能效。

在上述低功耗的情况下，Pygmy可实现4 TOPS/watt（整数Int8）。

作为对比Google第一代28 nm TPU 92 TOPs over 40 watt，差不多2.3 TOPS/watt. Google使用的是固定的脉动阵列（systolic array），而作为基于RISC-V的多核可编程AI引擎，能效却可超过纯ASIC加速器的实现。

这充分体现了开放RISC-V架构为计算机架构的innovation提供了巨大的空间, 灵活性和潜力。
![](https://pic3.zhimg.com/v2-edc1b13c324e3f676d22ec6277e21c82_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)
**△**图灵奖得主Patterson教授在OURS展台

第二，能大幅缩短从0到1的时间。


这样一款高性能、高复杂度的芯片，睿思芯科的工程师仅用了创纪录的7个月时间，就完成了从零开始设计验证到交付流片的全部研发工作。

而且芯片经过生产封测，回到公司第一天就能加电启动。

除了睿思芯科研发团队的技术实力，背后自然密不可分的是RISC-V极为干净简洁的优秀架构。
![](https://pic4.zhimg.com/v2-d7adec40842657a1c16f53df3406ca03_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)
**△**Patterson教授乌镇演讲


## **亮相乌镇，图灵奖得主亲自介绍**

当然，如此性能的芯片，Patterson教授在乌镇世界互联网大会上也亲自以代表案例对外介绍。

这位新晋图灵奖得主，在其**The Future of the Internet is the Cloud and Edge, which the Free and Open RISC-V Architecture Accelerates**的演讲中，以Pygmy为例，说明RISC-V的先进和强大。

Patterson教授还表示，RISC-V的未来在中国，而中国的半导体芯片技术和市场发展也离不开RISC-V。
![](https://pic4.zhimg.com/v2-9155f4d9dd0c3fafa553af09545f8fe3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)
**△**Patterson、谭章熹师徒二人


而这也是OURS成立中资公司睿思芯科的核心原因，希望凭借在RISC-V和AI SoC领域的正宗传承和丰富经验，支撑中国处理器和人工智能芯片生态体系建设，推动中国芯片设计行业发展。

谭章熹还透露，OURS已进行了包括基于RISC-V的AI SoC在内的数次成功流片，在国内拥有IP付费用户，并与国内知名大型企业展开了深度合作。

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！

相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态




