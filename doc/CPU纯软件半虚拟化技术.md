#  			[CPU纯软件半虚拟化技术](https://www.cnblogs.com/echo1937/p/7227385.html) 		



在2003年出现的Xen，使用了另外的一种半虚拟化的方案来解决x86架构下CPU的敏感指令问题。主要采用Hypercall技术。Guest  OS的部分代码被改变，从而使Guest  OS会将和特权指令相关的操作都转换为发给VMM的Hypercall（超级调用），由VMM继续进行处理。而Hypercall支持的批处理和异步这两种优化方式，使得通过Hypercall能得到近似于物理机的速度。

[![wKiom1afF8rRLK3ZAACfLO6bwIA867.png](http://s4.51cto.com/wyfs02/M02/79/F3/wKiom1afF8rRLK3ZAACfLO6bwIA867.png)](http://s4.51cto.com/wyfs02/M02/79/F3/wKiom1afF8rRLK3ZAACfLO6bwIA867.png)

### １、Hypercall技术

　　对于x86体系结构CPU，Xen使用超级调用来替换被监控的操作，其中包括x86架构下的敏感指令。Xen所采用的超级替换的方法是一种全新的设计理念：它将问题的中心，由VMM移向Guest OS自身，通过主动的方式由Guest OS去处理这些指令，而不是被移交给VMM做处理，在这种设计理念下，修改Guest OS内核。

   能修改Guest OS是半虚拟化的一个技术核心。通过修改Guest OS的内核。使Guest  OS明确知道自己是运行在1环上，而不是通常OS的0环，有效的避免了虚拟化的执行冲突问题。Guest  OS也清楚VMM给自己提供了一个虚拟的寄存器组，并能通过其他方式去访问他们，避免了访问冲突的问题。

​    解决了敏感指令问题只是解决了x86架构下的半虚拟化的第一步。运行在1环的操作系统没有权限执行的指令，交给0环的VMM来处理，这个很大程度上与应用程序的系统调用很类似：系统调用的作用是把应用程序无权执行的指令交给操作系统完成。因此，Xen向Guest OS提供了一套“系统调用”。以方便Guest OS调用，这部分”系统调用“就是超级调用Hypercall。

超级调用Hypercall的机制使用，不仅使x86架构的指令虚拟化得以实现，也为后面的内存虚拟化和I/O虚拟化提供了新的思路和方法，超级调用和事件通道是整个半虚拟化的基础。

下面我们来看看半虚拟化情况下整体的访问流程图，如图所示。

[![wKiom1afF9-BZbZuAAAotW_0zjg092.png](http://s2.51cto.com/wyfs02/M01/79/F3/wKiom1afF9-BZbZuAAAotW_0zjg092.png)](http://s2.51cto.com/wyfs02/M01/79/F3/wKiom1afF9-BZbZuAAAotW_0zjg092.png)

CPU半虚拟化技术

上图明确的显示了Hypercall的调用位置，在Xen中，各组件通信方式如下所示，Hypercall的调用性质是同步的。其他Xen的通信方式几乎都是异步的。

[![wKiom1afF_iQ2q87AACww5CfWNE788.png](http://s4.51cto.com/wyfs02/M00/79/F3/wKiom1afF_iQ2q87AACww5CfWNE788.png)](http://s4.51cto.com/wyfs02/M00/79/F3/wKiom1afF_iQ2q87AACww5CfWNE788.png)

其中，在虚拟机和Xen的通信过程中，如果虚拟机需要调用敏感指令，会主动向虚拟机监控器发起Hypercall调用。Hypercall就如同传统操作系统下的系统调用，监管程序通过它向其上各虚拟机提供各种服务，如MMU  更新、Domain0 操作请求和虚拟处理器状态等。

下图显示了半虚拟化模式下的特权模式：

[![wKioL1afGFCTwrrgAADLqoaNu8Q729.png](http://s2.51cto.com/wyfs02/M01/79/F2/wKioL1afGFCTwrrgAADLqoaNu8Q729.png)](http://s2.51cto.com/wyfs02/M01/79/F2/wKioL1afGFCTwrrgAADLqoaNu8Q729.png)

在x86架构下，原生系统和半虚拟化环境下存在差异。原生环境下，CPU有4个特权级(ring0--ring3)，操作系统是处于最高级别的ring0，应用程序处于最低级别的ring3。而在半虚拟化环境下，虚拟机监视器是处于最高级别的ring0，操作系统是处于中级级别的ring１，应用程序处于最低级别的ring3。

[![wKiom1afGCqRoKQeAABTEciqbX8993.png](http://s5.51cto.com/wyfs02/M01/79/F3/wKiom1afGCqRoKQeAABTEciqbX8993.png)](http://s5.51cto.com/wyfs02/M01/79/F3/wKiom1afGCqRoKQeAABTEciqbX8993.png)

只有特权级别为1 的代码（准虚拟化Guest VM 的内核）才能向Xen 发送Hypercall 请求，以防止应用程序（特权级3）的错误调用导致对系统可能的破坏。因此，只有运行在特权级1  的虚拟机操作系统内核才能申请Hypercall。但是，一些Xen 专用的特别程序，如xend 或xｅ也需要有Hypervisor  的服务来完成特殊的操作，如生成一个新的GuestVM 等，这在Xen Linux 中是通过一个称为privcmd  的内核驱动程序实现。应用程序通过ioctl 向该驱动程序提出服务请求，运行在虚拟机内核（特权级1）的privcmd  驱动程序再将服务请求以Hypercall 形式转向Hypervisor，并由后者真正完成生成新Guest VM 的动作。

[![wKioL1afGH6wRgTgAAELB5Omcd0071.png](http://s2.51cto.com/wyfs02/M02/79/F2/wKioL1afGH6wRgTgAAELB5Omcd0071.png)](http://s2.51cto.com/wyfs02/M02/79/F2/wKioL1afGH6wRgTgAAELB5Omcd0071.png)

上图中显示了Hypercall所在的位置，Hypercall位于图中右上方，内核向Hypervisor发起调用的哪里。Xen启用130号中断向量端口（十六进制的82H）作为超级调用的中断号。这一个中断向量的DPL被设置为类型为1，类型为中断门。这样，超级调用能够由处于特权级1的客户机操作系统发起，而不能从用户态发起。

另外，在x86指令集的指令中，有17 条指令不能有效的在ring 1 特权级上运行，Hypercall 的存在解决了这些指令不能正常执行的问题。

[![wKioL1afGJeBGHKJAAGdRJhdhPM639.png](http://s3.51cto.com/wyfs02/M02/79/F2/wKioL1afGJeBGHKJAAGdRJhdhPM639.png)](http://s3.51cto.com/wyfs02/M02/79/F2/wKioL1afGJeBGHKJAAGdRJhdhPM639.png)

Hypercall 机制中，在32 位x86 架构下，Hypercall 通过int0x82陷阱(Trap)指令实现，因为传统操作系统本身并不使用int0x82 (Linux 使用int 0x80 作为系统调用指令，int 0x82 并未使用)。

int0x82包括：

- 超级调用号：xen/include/public/xen.h中定义了45个超级调用，其中有7个是平台相关调用。
- 超级调用表：xen/arch/x86/x86_32/entry.S中定义了超级调用表，通过超级调用号索引就可以方便的找到对应的处理函数。
- 超级调用页：超级调用页是Xen为Guest OS准备的一个页，可以做到不同Guest OS有不同的超级调用页内容。

　　Hypercall 的具体功能识别号由eax 表明，而其他参数则在ebx, ecx, edx, esi 和edi  中。为了减少虚拟机和Hypervisor 之间的特权级别(Ring)切换次数，Xen  提供对Hypercall的批处理，即将几个Hypercall 功能请求放在一个列表中由专门的Hypercall 批处理请求完成。在Xen  中，系统调用表与Hypercall 表都在entry.S 文件中被定义。

### ２、X86架构特权级

x86 硬件支持 4 个特权级 (Ring)，一般内核运行在 Ring 0， 用户应用运行在 Ring 3， 更小的 Ring 有比更高的  Ring 能访问更多的系统全局资源，即更高的特权。有些指令只能在 Ring 0 才能正确执行，如 LGDT、LMSW  指令，我们称之为特权指令；另外有些指令可以在 Ring 3 正确执行，如 SGDT、 SMSW、PUSHF/POPF，我们称之为非特权指令。

正常模式和虚拟化两种情况叙述下，特权模式说明如下：

**正常模式**：特权级别是针对段来讲的，段描述符的最后两位标识了该段所位于的特权级别，比如，中断处理程序运行于ring0（），此时的内核程序是具有特权的，即ring0。位于ring3用户程序可以通过系统调用的方式，int80，后特权翻转入ring0，然后就可以顺利执行中断处理程序（好像是用户程序调用内核程序的唯一途径）。

**虚拟化情况下**：

**特权解除**：是指解除正常情况下运行于ring0的段，比如中断处理程序，为了虚拟化需要，此时解除其特权，将其运行于ring1。当用户程序通过系统调用时，其跳转到的中断处理程序运行于ring1。但是，在中断处理程序中，有部分指令是必须在ring0才能执行的，此时，便会自动陷入，然后模拟。也就是说，用户程序运行特权指令，会有两次特权下降，一次是通过系统调用进入ring1，第二次是通过特权指令陷入进入ring0。这说明，中断发生时的中断处理程序还是以前的位于内核的代码，但是其运行级别为ring1，部分指令还需要再次陷入，才能执行。另外，还有一个重要问题，就是部分敏感非特权指令无法陷入的问题：存在二进制翻译、超级调用等方式，强迫其陷入，然后模拟。

在传统的 X86 平台上支持虚拟化上存在如下问题 :

X86 指令集中存在 **17 条敏感的非特权指令**，“非特权指令”表明这些指令可以在 x86 的 ring 3 执行， 而“敏感性”说明 VMM 是不可以轻易让客操作系统执行这些指令的。 这 17 条指令在客操作系统上的执行或者会导致系统全局状态的破坏，如 POPF 指令，或者会导致客操作系统逻辑上的问题，如 SMSW 等读系统状态或控制寄存器的指令。传统的 X86 没法捕获这些敏感的非特权指令。

除了那 17 条敏感的非特权指令，其他敏感的指令都是敏感的特权指令。在 x86 虚拟化环境，VMM  需要对系统资源进行统一的控制，所以其必然要占据最高的特权级，即 Ring 0， 所以为了捕获特权指令，在传统 x86 上一个直接可行的方法是  “Ring deprivileging”， 如将客操作系统内核的特权级从 Ring 0改为 Ring 1 或 Ring 3， 即 “消除”  客内核的特权，以低于 VMM所在的 Ring 0， 从而让 VMM 捕获敏感的特权指令。

### ３、总结

　　半虚拟化的思想就是，让客户操作系统知道自己是在虚拟机上跑的，工作在非ring0状态，那么它原先在物理机上执行的一些特权指令，就会修改成其他方式，这种方式是可以和VMM约定好的，这就相当于，通过修改代码把操作系统移植到一种新的架构上来，就像是定制化。所以XEN这种半虚拟化技术，客户机操作系统都是有一个专门的定制内核版本，和x86、mips、arm这些内核版本。这样以来，就不会有捕获异常、翻译、模拟的过程了，性能损耗非常低。这就是XEN这种半虚拟化架构的优势。这也是为什么XEN半虚拟化只支持虚拟化Linux，无法虚拟化windows原因，微软不修改代码无法实现半虚拟化。



分类: [Virtualization](https://www.cnblogs.com/echo1937/category/1086868.html)