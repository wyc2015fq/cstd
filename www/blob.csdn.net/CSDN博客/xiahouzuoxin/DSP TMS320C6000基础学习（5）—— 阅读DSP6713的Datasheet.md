# DSP TMS320C6000基础学习（5）—— 阅读DSP6713的Datasheet - xiahouzuoxin - CSDN博客





2013年07月30日 20:11:08[xiahouzuoxin](https://me.csdn.net/xiahouzuoxin)阅读数：10536
个人分类：[DSP](https://blog.csdn.net/xiahouzuoxin/article/category/1520295)









本文将阅读TMS320C6713的数据手册（Datasheet）过程记录下来。

因为本次阅读数据手册的目的是获取最小系统的硬件设计相关的信息，因此将会说明如何筛选数据手册中的相关部分阅读。

TMS320C6713的数据手册可以在[http://www.alldatasheet.com/](http://www.alldatasheet.com/)搜索下载获得，也可以到TI官网下载。




***1. 确定阅读目标***

为绘制TMS320C6713的最小系统，因此，主要关注核心的硬件，比如供电、复位、时钟以及启动方式配置等。如果不添加相关的外设，则数据手册中有关外设的讲解可以略过。




***2. 阅读数据手册首页的关于芯片功能的基本信息***


![TMS320C6713_1](https://img-my.csdn.net/uploads/201308/02/1375411770_9540.JPG)


数据手册上的第一页很重要，比如从上面很容易就可以看出6713为浮点DSP，主频、指令速度等。

还有启动的配置方式，封装方式，供电方式，这些是在设计最小系统时都需要考虑的问题。比如我们从中可以看出，6713分别需要3.3V和1.2给IO口和DSP内核供电，因此我们必需设计电源电路能同时产生1.2V和3.3V的稳定供电电压。




***3. 熟悉最小系统的构成***

TMS320C6713最小系统由7部分组成，

![](https://img-blog.csdn.net/20130802105856312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhaG91enVveGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


熟悉了最小系统我们就知道电路设计过程中要完成哪些功能呢，在继续阅读数据手册时也知道要关注哪些功能了。

我们要关注电源的设计，功能设置一般包括启动方式配置，下载方式配置，存储器访问方式配置等等，关注复位的方法（高电平复位？低电平复位），关注需要哪几种时钟，是否需要外扩存储器、存储器的映射关系（如果要外扩存储器需要知道映射关系）。JTAG电路一般比较固定，可以在网络上查找近乎固定的电路模式，但不同芯片的JTAG电路略有不同，注意区分。DSP6713使用14脚的JTAG接口。




***4. 快速阅览数据手册***


如果这是第一次阅读该数据手册，强烈建议先不要直接大范围地跳着阅读，而应该“**阅览**”，阅览指快速的往下读，觉得与最小系统设计无关的部分可以直接很快跳过。

阅读时对芯片整体系统的描述最好仔细阅读，比如下面的描写6713处理器特性的表格和功能图（对存在的外设要知道外设是干嘛用的，但不用知道用法），其它相关的内容应该形成一个框架，需要知道大致的位置，需要时再详细阅读。

![6713特性](https://img-my.csdn.net/uploads/201308/02/1375411708_6289.JPG)


存储器的映射图

![6713存储映射图](https://img-my.csdn.net/uploads/201308/02/1375411705_3002.JPG)


若外扩存储器，外扩存储器的地址至少从0x8000 0000（EMIF CE0）开始。




数据手册接着就是各种详细的关于外设的寄存器配置了，这些可以暂时不用去管，记住大概位置，快速翻过，用到的时候再详细看，接着往下，看到**“DEVICE CONFIGURATIONS”**，设备配置，当然很重要了（与最小系统中的功能设置部分对应），我们要详细地看看，如果可以还可以按照自己的方式做个笔记，首先有下面一段，

*On the C6713 device, bootmode and certain device configurations/peripheral selections are determined atdevice reset, while other device configurations/peripheral selections are software-configurable via the deviceconfigurations register (DEVCFG) [address location 0x019C0200] after device reset.*


为什么要有设备配置？因为现在的芯片有很管脚具备复用功能（有多种功能），我们要按照自己的方式配置成自己需要的功能。

上面那段话的意思是说：启动模式和设备配置/外设选择在设备复位时决定了（暗指上电时就由硬件决定了），还有一些设备配置是由软件寄存器（DEVCFG）配置的。很明显，我们这里更关注上电前硬件要做哪些配置。

![6713上电前硬件配置](https://img-my.csdn.net/uploads/201308/02/1375411756_3711.JPG)


哦，这手册很总结地对要进行复位时要配置好的硬件部分进行了描述（H12的上拉，HD8大小端配置，HD[4:3]启动模式，CLKMODE0时钟选择模式），我在自己的电路中选择配置为HD12=1上拉，HD8=1小端，HD[4:3]=10，CLKMODE0=1方波时钟输入。除了对芯片的模式进行配置外，还要对“外设选择”进行配置（下图），这里配置HD14=0即不使能HPI。

![6713外设选择配置](https://img-my.csdn.net/uploads/201308/02/1375411756_8161.JPG)





接着往下看，**“TERMINAL FUNCTIONS ”**，管脚功能，我们记住该位置（可以在pdf中添加书签），暂不细看，**在绘制芯片的原理图封装时主要参考该表格**。

![6713管脚功能示意](https://img-my.csdn.net/uploads/201308/02/1375411756_3352.JPG)


表格中描述了管脚信号名称，管脚号，管脚类型（输入/输出/三态等），也对管脚功能做了描述。







接下来看到TMS320C6000系列的命名方法，作为课外知识学习下，

![TMS320C6000命名方法](https://img-my.csdn.net/uploads/201308/02/1375411757_1126.JPG)


接着，看到曙光——“文档支持”，该部分给出了一些参考文档，是继续深入学习的最好参考资料，因此至少要记着有这么些资料可供参考。

![](https://img-blog.csdn.net/20130802105720687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveGlhaG91enVveGlu/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


还有很多资源可以从TI的官网上获得。




然后数据手册中有一大块是对各种外设寄存器的描述，可暂时跳过。




然后，也是这里关注的重点，电源供电：（1）电源供电顺序是DSP核要比IO口先上电，断电顺序刚好相反，数据手册上给出了相应的电源供电电路的方案，

![6713供电](https://img-my.csdn.net/uploads/201308/02/1375411706_7039.JPG)


数据手册中还对供电去耦的相关内容进行了描述：

***In order to properly decouple the supply planes from system noise, place as many capacitors (caps) as possibleclose to the DSP**. Assuming 0603 caps, the user should be able to fit a total of 60 caps — 30 for the core supplyand 30 for the I/O supply. These caps need to be close (no more than 1.25 cm maximum distance) to the DSPto be effective. Physically smaller caps are better, such as 0402, but the size needs to be evaluated from ayield/manufacturing point-of-view. Parasitic inductance limits the effectiveness of the decoupling capacitors,therefore physically smaller capacitors should be used while maintaining the largest available capacitancevalue. As with the selection of any component, verification of capacitor availability over the product’s productionlifetime needs to be considered.*


接着还对JTAG的接口、启动模式（Bootmode）和复位相关的内容进行了描述。

（1）JTAG的接口

*The TMS320C6713 DSP requires that both TRSTand RESETresets be asserted upon power up to be properlyinitialized. While RESETinitializes the DSP core, TRSTinitializes the DSP’s emulation logic. Both resets arerequired for proper operation.Note: TRSTis synchronous and must be clocked by TCLK; otherwise, BSCAN may not respond as expectedafter TRSTis asserted.While both TRSTand RESETneed to be asserted upon power up, only RESETneeds to be released for theDSP to boot properly. TRSTmay be asserted indefinitely for normal operation, keeping the JTAG port interfaceand DSP’s emulation logic in the reset state. TRSTonly needs to be released when it is necessary to use a JTAGcontroller to debug the DSP or exercise the DSP’s boundary scan functionality.The TMS320C6713 DSP includes an internal pulldown (IPD) on the TRSTpin to ensure that TRSTwill alwaysbe asserted upon power up and the DSP’s internal emulation logic will always be properly initialized when thispin is not routed out. JTAG controllers from Texas Instruments actively drive TRSThigh. However, somethird-party JTAG controllers may not drive TRSThigh but expect the use of an external pullup resistor on TRST.When using this type of JTAG controller, assert TRSTto initialize the DSP after powerup and externally driveTRSThigh before attempting any emulation or boundary scan operations.Following the release of RESET, the low-to-high transition of TRSTmust be “seen” to latch the state of EMU1and EMU0. The EMU[1:0] pins configure the device for either Boundary Scan mode or Emulation mode. Formore detailed information, see the terminal functions section of this data sheet. Note: The DESIGN−WARNINGsection of the TMS320C6713 BSDL file contains information and constraints regarding proper device operationwhile in Boundary Scan Mode. **For more detailed information on the C6713 JTAG emulation, see theTMS320C6000 DSP Designing for JTAG Emulation Reference Guide(literature number SPRU641).***


（2）启动模式

***The C6713 has three types of boot modes:***
*** Host boot**If host boot is selected, upon release of internal reset, the CPU is internally “stalled” while the remainder ofthe device is released. During this period, an external host can initialize the CPU’s memory space asnecessary through the host interface, including internal configuration registers, such as those that controlthe EMIF or other peripherals. Once the host is finished with all necessary initialization, it must set theDSPINT bit in the HPIC register to complete the boot process. This transition causes the boot configurationlogic to bring the CPU out of the “stalled” state. The CPU then begins execution from address 0. The DSPINTcondition is not latched by the CPU, because it occurs while the CPU is still internally “stalled”. Also, DSPINTbrings the CPU out of the “stalled” state only if the host boot process is selected. All memory may be writtento and read by the host. This allows for the host to verify what it sends to the DSP if required. After the CPU isout of the “stalled” state , the CPU needs to clear the DSPINT, otherwise, no more DSPINTs can be received.** Emulation boot**Emulation boot mode is a variation of host boot. In this mode, it is not necessary for a host to load code or toset DSPINT to release the CPU from the “stalled” state. Instead, the emulator will set DSPINT if it has notbeen previously set so that the CPU can begin executing code from address 0. Prior to beginning execution,the emulator sets a breakpoint at address 0. This prevents the execution of invalid code by halting the CPUprior to executing the first instruction. Emulation boot is a good tool in the debug phase of development.** EMIF boot (using default ROM timings)**Upon the release of internal reset, the 1K-Byte ROM code located in the beginning of CE1is copied toaddress 0 by the EDMA using the default ROM timings, while the CPU is internally “stalled”. The data shouldbe stored in the endian format that the system is using. The boot process also lets you choose the width ofthe ROM. In this case, the EMIF automatically assembles consecutive 8-bit bytes or 16-bit half-words toform the 32-bit instruction words to be copied. The transfer is automatically done by the EDMA as asingle-frame block transfer from the ROM to address 0. After completion of the block transfer, the CPU isreleased from the “stalled” state and start running from address 0.*


（3）复位

*A hardware reset (RESET) is required to place the DSP into a known good state out of power−up.**The RESETsignal can be asserted (pulled low) prior to ramping the core and I/O voltages or after the core and I/O voltageshave reached their proper operating conditions. As a best practice, reset should be held low during power−up.Prior to deasserting RESET(low−to−high transition)**, the core and I/O voltages should be at their properoperating conditions and CLKIN should also be running at the correct frequency.*





再接着数据手册的后面就是一些相关的时序，在编程时尤为关键，但不是现在考虑的问题；最后就是机械封装结构了，PCB封装的绘制主要参考数据手册结尾处的机械封装描述。




***5. 结尾***


到此，我们已经完整的阅览了遍TMS320C6713的数据手册了。有人觉得阅读数据手册很枯燥，但有目的有重点地去阅读，自我觉得阅读数据手册是了解一个芯片最快速有效的方法。在阅读时注意：

（1）确定阅读目标

（2）确定关注框架/内容：可以绘制思维导图（推荐XMind软件）帮助阅读

（3）第一次阅读要“**阅览**”，并对重点但一时无法记忆的内容使用pdf书签标记，其它次要内容知道大概位置就OK了，以后阅读直接跳转到需要了解的位置阅读即可。

（4）"好记性不如烂笔头“，数据手册就是一本讲解芯片的书，阅读时要时刻注意总结记笔记。

原则上，一份100多页的数据手册在4个小时的时间内完全可以阅览完，若4小时内没有阅览完毕，说明太过关注细节了，否则说明可能漏掉了重要的东西。









