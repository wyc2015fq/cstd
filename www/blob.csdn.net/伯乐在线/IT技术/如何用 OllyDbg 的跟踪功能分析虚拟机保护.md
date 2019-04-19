# 如何用 OllyDbg 的跟踪功能分析虚拟机保护 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [木羊同学](http://www.jobbole.com/members/woosheep) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
虚拟机保护已经是现代保护壳不可缺少的一环，虽然逆向方也发展出各种插件帮助分析，但只针对特定某款，通用性的方法却不多见。我总在想，既然虚拟机的结构是固定的，如果有一款工具能够记录指令流，那么按图索骥，也许能发展出一套通用的分析方法来。其实OD（OllyDbg）就有记录指令流的功能，叫跟踪（trace），也许是效果不好或者操作不便，用的人甚至知道的人不多。先介绍下怎么用。
OD的跟踪功能原理很简单，就是每一步都自动下单步断点，然后记录断下来的指令信息。这项功能涉及到几项设置，第一项是缓存大小，不难想象，跟踪得到的这一些列的指令记录是需要占地方存储的，占多大可以设置，位置在调试选项（Debugging options）->跟踪（Trace），如图1。
![file0001](http://jbcdn2.b0.upaiyun.com/2017/01/1a016cb4c1c92a441a0fd4ab9e6bd879.jpg)
图1
第一项就是缓存的大小，内存允许的话，自然是多多益善，毕竟缓存越大，允许记录的信息越多。第二项是记录的内容，跟踪会自动记录地址模块等信息，此外可以选择是否记录指令、ESP和标志位的信息。设置位置紧接着缓存大小，见图2，可以按需勾选，本文只需要记录指令即可。最后一项是在调试（DEBUG）菜单中打开Trace。
![file0002](http://jbcdn2.b0.upaiyun.com/2017/01/461237d368474b64ecfb6a277b900835.jpg)
图2
现在Trace已经设置完毕了，按下Ctrl+F12，查看Trace窗口，应该已经开始记录执行过的指令。否则请检查前述设置和操作是否正确。
那么，虚拟机保护要怎么入手分析呢？前面我提到，虚拟机是有固定结构的，既然要分析，那对应的找到这些结构应该就可以了。传统保护虚拟机的结构其实很简单，大致可以看成一只章鱼，有三个部分，分别是init（头），Dispatch（身）和Handle（触须），如图3：
![file0003](http://jbcdn2.b0.upaiyun.com/2017/01/a0ec974c5655b05c9920257d38c5ff95.jpg)
图3
Init主要完成虚拟机初始化工作，例如申请内存填写初始值之类，每次进入虚拟机，这个“头部”通常只执行一次。Dispatch是虚拟机的主体，可以看成一个主循环，它是每一条虚拟机指令的开始之处，也是结束之处，负责读取虚拟机指令，进入具体handle解释等工作。Handle就是虚拟机的“指令”了，实际完成各项虚拟机指令的功能。 我曾写过一篇《基于虚拟机的软件保护技术》较为详细的介绍过虚拟机保护技术，对基本结构还不太熟的同学，此文会对上述概念有更详细的说明。
现在，我们就要在具体的软件中找这只“章鱼”了。以一个CrackMe为例，首先清理所有断点，打开Trace，Ctrl+F12跟踪步过运行，看到程序跑起来了，F12暂停，看Trace的窗口如下（图4）：
![file0004](http://jbcdn2.b0.upaiyun.com/2017/01/54c7c30af4f097638622028ca5ae901b.jpg)
图4
记录是从下往上看的，可以看出，在程序空间的最后一条支流，是00401534的一个call，调用了DialogBoxInDirectParamA，这是一个调出系统对话框的API，其中有一个参数DlgProc用来指明消息回调函数的位置，我们直接在反汇编窗口查看这个API，发现回调函数是0x401572（图5）：
![file0005](http://jbcdn2.b0.upaiyun.com/2017/01/d965452de3ca2c5d42de23efbedbbbf7.jpg)
图5
0x401572处代码不长，有好几条Call，但大部分都是系统Call，只有一处调用了程序空间的函数，这个函数就是虚拟机的入口。到这里，我们对虚拟机的分析的工作才刚刚开始。
首先对虚拟机的入口下断，然后重新运行程序。目的是保证能够正确找到init。现在应该端在虚拟机的入口处，如下图：
![file0006](http://jbcdn2.b0.upaiyun.com/2017/01/d125fc8a809b6de97cbdba3d35762398.jpg)
图6
这是个非常简单的虚拟机，有经验的同学也许可以一眼就看出来图6包含了Init和Dispatch分别在哪里。当然也可以用Trace快速找出虚拟机的各个结构。现在去掉断点，打开Trace，Ctrl+F12跟踪步过，这时程序会跑起来，多点击几下按钮，目的是让主要分支得到更充分的执行（即增加获得执行的次数），然后F12暂停。回到Trace窗口，对着任意一行程序空间的指令点击右键，选择模块统计，结果如下图：
![file0007](http://jbcdn2.b0.upaiyun.com/2017/01/a4069cd955169adc70fa5ef0bc894680.jpg)
图7
统计是以代码段来划分的，第一栏显示的是这段代码在刚才的跟踪执行中执行的次数，第二栏显示了某个代码段的首地址。我们先找执行了一次的指令首地址。可以找到第5行的地址就是虚拟机的入口地址，点击在反汇编窗口跟随，可以看到这段代码是从0x00401060到0x004010B9，这就是init：

Assembly (x86)
```
00401060 $ 55 push ebp
00401061 . 8BEC mov ebp, esp
00401063 . 81C4 D0FEFFFF add esp, -0x130
00401069 . C745 E4 00000>mov dword ptr [ebp-0x1C], 0x0
00401070 . C745 E8 00000>mov dword ptr [ebp-0x18], 0x0
00401077 . C745 F1 00000>mov dword ptr [ebp-0xF], 0x0
0040107E . C645 FD 00 mov byte ptr [ebp-0x3], 0x0
00401082 . C645 FE 00 mov byte ptr [ebp-0x2], 0x0
00401086 . C745 F5 00000>mov dword ptr [ebp-0xB], 0x0
0040108D . 8D85 D0FEFFFF lea eax, dword ptr [ebp-0x130]
00401093 . 8945 F1 mov dword ptr [ebp-0xF], eax
00401096 . 8B45 14 mov eax, dword ptr [ebp+0x14]
00401099 . 8945 E0 mov dword ptr [ebp-0x20], eax
0040109C . 8B45 08 mov eax, dword ptr [ebp+0x8]
0040109F . 8945 D0 mov dword ptr [ebp-0x30], eax
004010A2 . 8B45 0C mov eax, dword ptr [ebp+0xC]
004010A5 . 8945 D8 mov dword ptr [ebp-0x28], eax
004010A8 . C745 DC 00000>mov dword ptr [ebp-0x24], 0x0
004010AF . C745 D4 00000>mov dword ptr [ebp-0x2C], 0x0
004010B6 . 8B45 10 mov eax, dword ptr [ebp+0x10]
004010B9 . 8945 EC mov dword ptr [ebp-0x14], eax
```
接着找Dispatch，刚才说过，它既是虚拟机指令的开始，又是结束，它得到的执行次数一定也最多。可以看到第三行的0x004010B9，这个地址在虚拟机入口地址之后，执行次数最多，同样的办法可以看到这段代码的终止位置是0x004010D9：

Assembly (x86)
```
004010BC > /FF45 EC inc dword ptr [ebp-0x14]
004010BF . |8B45 EC mov eax, dword ptr [ebp-0x14]
004010C2 . |8A00 mov al, byte ptr [eax]
004010C4 . |8845 F0 mov byte ptr [ebp-0x10], al
004010C7 . |B8 00204000 mov eax, 00402000
004010CC . |0FB65D F0 movzx ebx, byte ptr [ebp-0x10]
004010D0 . |C1E3 02 shl ebx, 0x2
004010D3 . |03C3 add eax, ebx
004010D5 . |FF20 jmp dword ptr [eax]
```
最后是找这次执行虚拟机用到的handle。这个不难，虚拟机入口地址之后的代码段除了init和dispatch，其它都是handle，所有执行过的handle都会在里面出现。当然了，某条handle的具体作用，以及没有执行过的handle，就只能靠人肉分析了。还有就是，就分析虚拟机保护来说，了解执行了哪些handle，以及哪些handle更常用，这些信息都是十分有用的。
