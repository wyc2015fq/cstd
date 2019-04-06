#  			[CPU硬件辅助虚拟化技术](https://www.cnblogs.com/echo1937/p/7218201.html) 		



目前主要有Intel的VT-x和AMD的AMD-V这两种技术。其核心思想都是通过引入新的指令和运行模式，使VMM和Guest  OS分别运行在不同模式（ROOT模式和非ROOT模式）下，且Guest OS运行在Ring 0下。通常情况下，Guest  OS的核心指令可以直接下达到计算机系统硬件执行，而不需要经过VMM。当Guest  OS执行到特殊指令的时候，系统会切换到VMM，让VMM来处理特殊指令。

### 1、Intel VT-x技术

为弥补x86处理器的虚拟化缺陷，市场的驱动催生了VT-x，Intel推出了基于x86架构的硬件辅助虚拟化技术Intel VT(Intel Virtualization Technology)。

目前，Intel VT技术包含CPU、内存和I/O三方面的虚拟化技术。

- CPU硬件辅助虚拟化技术，分为对应安腾架构的VT-i(Intel Virtualization Technology for ltanium)和对应x86架构的VT-x(Intel Virtualization Technology for x86)。
- 内存硬件辅助虚拟化技术包括EPT(Extended Page Table)技术。
- I/O硬件辅助虚拟化技术的代表VT-d(Intel Virtualization Technology for Directed I/O)。

Intel VT-x技术解决了早期x86架构在虚拟化方面存在的缺陷，可使未经修改的Guest  OS运行在特权级0，同时减少VMM对Guest OS的干预。Intel VT-d技术通过使VMM将特定I/O设备直接分配给特定的Guest  OS，减少VMM对I/O处理的管理，不但加速数据传输，且消除了大部分性能开销。如下图所示。CPU硬件辅助虚拟化技术简要说明流程图：

[![wKioL1afGVfxeUomAAAz5_tFJmI638.png](http://s3.51cto.com/wyfs02/M01/79/F2/wKioL1afGVfxeUomAAAz5_tFJmI638.png)](http://s3.51cto.com/wyfs02/M01/79/F2/wKioL1afGVfxeUomAAAz5_tFJmI638.png)

效法IBM 大型机，VT-x提供了2 个运行环境：根（Root）环境和非根（Non-root）环境。根环境专门为VMM准备，很像原来没有VT-x 的x86，只是多了对VT-x 支持的几条指令。非根环境作为一个受限环境用来运行多个虚拟机。

[![wKiom1afGTSh8RorAABRoN9MaUU781.png](http://s1.51cto.com/wyfs02/M01/79/F3/wKiom1afGTSh8RorAABRoN9MaUU781.png)](http://s1.51cto.com/wyfs02/M01/79/F3/wKiom1afGTSh8RorAABRoN9MaUU781.png)

如上图所示，根操作模式与非根操作模式都有相应的特权级0至特权级3。VMM运行在根模式的特权级0，GuestOS的内核运行在非根模式的特权级0，GuestOS的应用程序运行在非根模式的特权级3。运行环境之间相互转化，从根环境到非根环境叫VMEntry；从非根环境到根环境叫VMExit。VT-x定义了VMEntry操作，使CPU由根模式切换到非根模式，运行客户机操作系统指令。若在非根模式执行了敏感指令或发生了中断等，会执行VMExit操作，切换回根模式运行VMM。

根模式与非根模式之问的相互转换是通过VMX操作实现的。VMM 可以通过VMXON 和VMXOFF打开或关闭VT-x。如下图所示：

[![wKiom1afGVDjubB0AAA3uBxIztk294.png](http://s2.51cto.com/wyfs02/M02/79/F3/wKiom1afGVDjubB0AAA3uBxIztk294.png)](http://s2.51cto.com/wyfs02/M02/79/F3/wKiom1afGVDjubB0AAA3uBxIztk294.png)

VMX操作模式流程：

1)、VMM执行VMXON指令进入VMX操作模式。

2)、VMM可执行VMLAUNCH指令或VMRESUME指令产生VM Entry操作，进入到Guest OS，此时CPU处于非根模式。

3)、Guest OS执行特权指令等情况导致VMExit的发生，此时将陷入VMM，CPU切换为根模式。VMM根据VMExit的原因作出相应处理，处理完成后将转到2)，继续运行GuestOS。

4)、VMM可决定是否退出VMX操作模式，通过执行VMXOFF指令来完成。

为更好地支持CPU虚拟化，VMX新定义了虚拟机控制结构VMCS(Virtual Machine ControlStructure)。VMCS是保存在内存中的数据结构，其包括虚拟CPU的相关寄存器的内容及相关的控制信息。CPU在发生VM Entry或VMExit时，都会查询和更新VMCS。VMM也可通过指令来配置VMCS，达到对虚拟处理器的管理。VMCS架构图如下图所示：

[![wKiom1afGaLTpUkXAAA7ZB52HHU125.png](http://s5.51cto.com/wyfs02/M00/79/F3/wKiom1afGaLTpUkXAAA7ZB52HHU125.png)](http://s5.51cto.com/wyfs02/M00/79/F3/wKiom1afGaLTpUkXAAA7ZB52HHU125.png)

每个虚拟处理器都需将VMCS与内存中的一块区域联合起来，此区域称为VMCS区域。对VMCS区域的操纵是通过VMCS指针来实现的，这个指针是一个指向VMCS的64位的地址值。VMCS区域是一个最大不超过4KB的内存块，且需4KB对齐。

VMCS区域分为三个部分：

- 偏移0起是VMCS版本标识，通过不同的版本号，CPU可维护不同的VMCS数据格式；
- 偏移4起是VMX中止指示器，在VMX中止发生时，CPU会在此处存入中止的原因；
- 偏移8起是VMCS数据区，这一部分控制VMX非根操作及VMX切换。

VMCS 的数据区包含了VMX配置信息：VMM在启动虚拟机前配置其哪些操作会触发VMExit。VMExit 产生后，处理器把执行权交给VMM 以完成控制，然后VMM 通过指令触发VMEntry 返回原来的虚拟机或调度到另一个虚拟机。

VMCS 的数据结构中，每个虚拟机一个，加上虚拟机的各种状态信息，共由3个部分组成，如之前的VMCS架构图所示：

1） Gueststate：该区域保存了虚拟机运行时的状态，在VMEntry 时由处理器装载；在VMExit时由处理器保存。它又由两部分组成：

- Guest OS寄存器状态。它包括控制寄存器、调试寄存器、段寄存器等各类寄存器的值。
- Guest OS非寄存器状态。用它可以记录当前处理器所处状态，是活跃、停机（HLT）、关机（Shutdown）还是等待启动处理器间中断（Startup-IPI）。

2） Hoststate：该区域保存了VMM 运行时的状态，主要是一些寄存器值，在VMExit 时由处理器装载。

3） Control data：该区域包含几部分数据信息，分别是：

- 虚拟机执行控制域（VM-Execution control fields）。VMM  主要通过配置该区域来控制虚拟机在非根环境中的执行行为。基于针脚的虚拟机执行控制。它决定在发生外部中断或不可屏蔽中断（NMI）要不要发生VMExit。基于处理器的虚拟机执行控制。它决定虚拟机执行RDTSC、HLT、INVLPG  等指令时要不要发生VMExit。
- VMExit 控制域（VMExit control fields）。该区域控制VMExit 时的行为。当VMExit  发生后处理器是否处于64 位模式；当因为外部中断发生VMExit 时，处理器是否响应中断控制器并且获得中断向量号。VMM  可以用它来定制当VMExit 发生时要保存哪些MSR  并且装载哪些MSR。MSR是CPU的模式寄存器，设置CPU的工作环境和标识cpu的工作状态。
- VMEntry 控制域（VMEntry control fields）。该区域控制VMEntry 时的行为。它决定处理器VMEntry  后是否处于IA-32e 模式。与VMExit 的MSR控制类似，VMM 用它来定制当VMEntry 发生时要装载哪些MSR。VMM  可以配置VMEntry 时通过虚拟机的IDT向其发送一个事件。在此可以配置将使用IDT 的向量、中断类型（硬件或软件中断）、错误码等。
- VMExit 信息域（VMExit information fields）。该只读区域包括最近一次发生的VMExit  信息。试图对该区域执行写操作将产生错误。。此处存放VMExit 的原因以及针对不同原因的更多描述信息、中断或异常向量号、中断类型和错误码、通过  IDT 发送事件时产生的VMExit 信息、指令执行时产生的 VMExit 信息。

有了VMCS结构后，对虚拟机的控制就是读写VMCS结构。后面对vCPU设置中断，检查状态实际上都是在读写VMCS数据结构。

### 2、AMD-V技术

​       我们在上面小节介绍了 Intel 的硬件辅助虚拟化技术，那么 AMD 的硬件辅助虚拟化技术又有什么特点呢？AMD 从  2006 年便开始致力于硬件辅助虚拟化技术的研究，AMD-V全称是AMD Virtualization，AMD-V从代码的角度分别称为 AMD和  SVM，AMD开发这项虚拟化技术时的内部项目代码为Pacifica，是AMD推出的一种硬件辅助虚拟化技术。

[![wKioL1afGj7xq-X4AAISdUtYJcE211.png](http://s3.51cto.com/wyfs02/M00/79/F2/wKioL1afGj7xq-X4AAISdUtYJcE211.png)](http://s3.51cto.com/wyfs02/M00/79/F2/wKioL1afGj7xq-X4AAISdUtYJcE211.png)

Intel VT-x 和 AMD-V 提供的特征大多功能类似，但名称可能不一样，如 Intel VT-x  将用于存放虚拟机状态和控制信息的数据结构称为 VMCS， 而 AMD-V 称之为VMCB； Intel VT-x 将 TLB 记录中用于标记  VM 地址空间的字段为 VPID， 而AMD-V 称之为 ASID； Intel VT-x 将二级地址翻译称之为 EPT， AMD 则称为  NPT，等等一些区别。尽管其相似性，Intel VT-x 和 AMD-V 在实现上对 VMM 而言是不兼容的。

AMD-V 在 AMD 传统的x86-64 基础上引入了“guest”操作模式。“guest”操作模式就是 CPU  在进入客操作系统运行时所处的模式。 “guest”操作模式为客操作系统设定了一个不同于 VMM 的运行环境而不需要改变客操作系统已有的 4  个特权级机制，也就是说在“guest”模式下，客操作系统的内核仍然运行在 Ring 0， 用户程序仍然在 Ring 3。 裸机上的操作系统和  VMM 所在的操作模式依然和传统的 x86 中一样，且称之为“host”操作模式。 VMM 通过执行 VMRUN 指令使CPU  进入“guest”操作模式而执行客操作系统的代码； 客操作系统在运行时，遇到敏感指令或事件，硬件就执行 VMEXIT 行为，使 CPU  回到“host”模式而执行 VMM 的代码。 VMRUN 指令运行的参数是一个物理地址指针，其指向一个 Virtual Machine  Control Block (VMCB) 的内存数据结构， 该数据结构包含了启动和控制一个虚拟机的全部信息。

[![wKiom1afGimzbOjaAABAsxMi2CI065.png](http://s4.51cto.com/wyfs02/M00/79/F3/wKiom1afGimzbOjaAABAsxMi2CI065.png)](http://s4.51cto.com/wyfs02/M00/79/F3/wKiom1afGimzbOjaAABAsxMi2CI065.png)

“guest”模式的意义在于其让客操作系统处于完全不同的运行环境，而不需要改变客操作系统的代码。“guest”模式的设立在系统中建立了一个比  Ring 0 更强的特权控制，即客操作系统的 Ring 0 特权必须让位于 VMM 的 Ring 0  特权。客操作系统上运行的那些特权指令，即便是在 Ring 0 上也变的可以被 VMM 截取的了，“Ring  Deprivileging”由硬件自动搞定。此外，VMM 还可以通过 VMCB  中的各种截取控制字段选择性的对指令和事情进行截取，或设置有条件的截取，所有的敏感的特权或非特权指令都在其控制之中。

[![wKioL1afGpTiDMn-AABqRRMT8Wk023.jpg](http://s1.51cto.com/wyfs02/M01/79/F2/wKioL1afGpTiDMn-AABqRRMT8Wk023.jpg)](http://s1.51cto.com/wyfs02/M01/79/F2/wKioL1afGpTiDMn-AABqRRMT8Wk023.jpg)

VMCB 数据结构主要包含如下内容 :

\1. 用于描述需要截取的指令或事件的字段列表。其中 :

- 2 个 16 位的字段用于控制对 CR 类控制寄存器读写的截取
- 2 个 16 位的字段用于控制对 DR 类调试寄存器的读写的截取
- 一个 32 位的字段用于控制 exceptions 的截取
- 一个 64 位的字段用于控制各种引起系统状态变化的事件或指令的截取，如 INTR， NMI， SMI 等事 件， HLT，  CPUID，INVD/WBINVD，INVLPG/INVLPGA，MWAIT 等指令， 还包括两位分别标志是否对 IO 指令和 MSR  寄存器的读写进行控制
- 指向IO端口访问控制位图和MSR读写控制位图的物理地址指针字段。该位图用于差别性地控制虚拟机对不同的 IO 端口和 MSR 寄存器进行读写访问。
- 描述虚拟机CPU状态的信息。包含除通用寄存器外的大部分控制寄存器，段寄存器，描述符表寄存器，代码指针等。 RAX 寄存器也在其中，因为  RAX 在 VMM 执行 VMRUN 时是用来存放VMCB 物理地址的。 对于段寄存器，该信息中还包含段寄存器对应的段描述符，也就那些传统  x86 上对软件隐藏的信息。
- 对虚拟机的执行进行控制的字段。主要是控制虚拟机中断和 NPT 的字段。
- 指示虚拟机进入“guest”模式后要执行的行动的字段。包括用来描述 VMM 向虚拟机注入的中断或异常的信息的字段。 注入的中断或异常在 VMRUN 进入“guest”模式后立即执行，就象完全发生在虚拟机内一样。
- 提供VMEXIT信息的字段。包括导致 VMEXIT 的事件的代码，异常或中断的号码，page fault 的线性地址，被截获的指令的编码等。

[![wKiom1afGuOSVGDWAACiyrhiMUI787.png](http://s3.51cto.com/wyfs02/M02/79/F3/wKiom1afGuOSVGDWAACiyrhiMUI787.png)](http://s3.51cto.com/wyfs02/M02/79/F3/wKiom1afGuOSVGDWAACiyrhiMUI787.png)

VMCB  以及其涉及的控制位图，完全通过物理地址进行指向，这就避免了“guest”和“host”模式切换的过程依赖于“guest”空间的线性地址 (  传统操作系统内用户空间到内核的切换确实依赖于 IDT 中提供的目标的线性地址 )，使得 VMM 可以采用和客操作系统完全不同的地址空间。

VMCB 的内容在物理上被分成了俩部分，其中用于保存虚拟机 CPU 状态的信息占据 2048 字节的后半部分，我们可称之为 VMCB.SAVE； 其他信息，占据前 1024 字节范围，我们可称之为 VMCB.CONTROL。

VMRUN 命令以 VMCB 为参数，使CPU 进入“guest”状态， 按 VMCB.SAVE 的内容恢复虚拟机的 CPU  寄存器状态，并按 VMCB.SAVE 中 CS:RIP 字段指示的地址开始执行虚拟机 的代码， 并将之前 VMM 的 CPU  状态保存在MSR_VM_HSAVE_PA 寄存器所指向的物理内存区域中。VMRUN 所保存的 VMM 的 CPU状态的 CS:RIP 实际上就是  VMM 的代码中 VMCB 的下一个指令，当虚拟机因某种原因而导致 #VMEXIT 时，VMM 会从 VMRUN 后的一条指令开始执行。CPU  执行 #VMEXIT 行为时，会自动将虚拟机的状态保存到 VMCB.SAVE 区，并从 MSR_VM_HSAVE_PA 指定的区域加载 VMM  的 CPU 状态。

VMLOAD 和 VMSAVE 指令是对 VMRUN 的补充，他们用来加载和恢复一些并不需要经常使用的 CPU 状态，如 FS， GS，  TR， LDTR 寄存器以及其相关的隐含的描述符寄存器的内容，VMLOAD 和 VMSAVE 可以让 VMM  的实现对“guest”进入和退出的过程进行优化，让多数情况下只使用 VMRUN 进行最少的状态保存和恢复。

VMMCALL 指令是 AMD-V 为客操作系统内核提供的明确的功能调用接口，类似于 syscall 指令 ( 从 Ring3 到 Ring 0)， VMMCALL 让客操作系统直接执行 #VMEXIT 而进入 VMM，请求VMM 的服务。

### 3、总结

回顾一下CPU虚拟化技术的实现，纯软件的CPU虚拟化使用了陷入-模拟的模式来模拟特权指令，而在x86架构中由于只能模拟特权指令，无法模拟某些敏感指令而无法实现完全的虚拟化。（在x86架构中，特权指令一定是敏感指令，但是敏感指令比特权指令多，造成某系敏感指令不是特权指令而无法模拟，使得CPU虚拟化异常），而硬件辅助虚拟化引入了根模式（root  operation）和非根模式（none-root  operation），每种模式都有ring0-3的四级特权级别。所以，在硬件辅助虚拟化中，陷入的概念实际上被VM-EXIT操作取代了，它代表从非根模式退出到根模式，而从根模式切换到非根模式是VM-Entry操作。



分类: [Virtualization](https://www.cnblogs.com/echo1937/category/1086868.html)