# ISO/IEC 14443协议浅谈：TYPE A与TYPE B - xqhrs232的专栏 - CSDN博客
2018年10月08日 23:10:37[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：115
个人分类：[NFC技术](https://blog.csdn.net/xqhrs232/article/category/2906045)
原文地址::[https://blog.csdn.net/luoqindong/article/details/54705862](https://blog.csdn.net/luoqindong/article/details/54705862)
相关文章
1、ISO14443 Type B类型卡的防碰撞过程以及命令解析----[https://blog.csdn.net/andy001847/article/details/51233971](https://blog.csdn.net/andy001847/article/details/51233971)
2、14443-A 与14443-B区别----[https://blog.csdn.net/liuxd3000/article/details/23170145](https://blog.csdn.net/liuxd3000/article/details/23170145)
3、Type A卡和Type B卡的主要区别----[https://blog.csdn.net/lihenair/article/details/8025767](https://blog.csdn.net/lihenair/article/details/8025767)
ISO/IEC14443规定了邻近卡(PICC)的物理特性;需要供给能量的场的性质与特征，以及邻近耦合设备(PCDs)和邻近卡(PICCs)之间的双向通信;卡(PICCs)进入邻近耦合设备(PCDs)时的轮寻，通信初始化阶段的字符格式，帧结构，时序信息;非接触的半双功的块传输协议并定义了激活和停止协议的步骤。传输协议同时适用于TYPE A 和 TYPE B。
　　TYPE A和TYPE B型卡片主要的区别在于载波调制深度及二进制数的编码方式和防冲突机制。
　　1、 调制解调与编码解码技术
　　根据信号发送和接收方式的不同，ISO/IEC14443-3定义了TYPEA、TYPEB两种卡型。它们的不同主要在于载波的调制深度及二进制数的编码方式。
![](http://dev.yesky.com/imagelist/2009/244/z88dc4eo3ef5.jpg)
　　从PCD向PICC传送信号时，二者是通过13.56Mhz的射频载波传送信号。从PICC向PCD传送信号时，二者均通过调制载波传送信号,副载波频率皆为847KHz。
　　图1：TYEP A、B 接口的通信信号
　　Type A型卡在读写机上向卡传送信号时，是通过13.65MHz的射频载波传送信号。其采用方案为同步、改进的Miller编码方式，通过100%ASK传送;当卡向读写机具传送信号时，通过调制载波传送信号。使用847kHz的副载波传送Manchester编码。简单说，当表示信息“1”时，信号会有0.3微妙的间隙，当表示信息“0”时，信号可能有间隙也可能没有，与前后的信息有关。这种方式的优点是信息区别明显，受干扰的机会少，反应速度快，不容易误操作;缺点是在需要持续不断的提高能量到非接触卡时，能量有可能会出现波动。
　　Type B型卡在读写机具向卡传送信号时，也是通过13.65MHz的射频载波信号，但采用的是异步、NRZ编码方式，通过用10%ASK传送的方案;在卡向读写机具传送信号时，则是采用的BPSK编码进行调制。即信息“1”和信息“0”的区别在于信息“1”的信号幅度大，即信号强，信息“0”的信号幅度小，即信号弱。这种方式的优点是持续不断的信号传递，不会出现能量波动的情况;
![](http://dev.yesky.com/imagelist/2009/244/errbv0o2w66c.jpg)
　　从PCD到PICC的通信信号接口主要区别在信号调制方面，TYPE A调制使用RF工作场的ASK100%调制原理来产生一个“暂停(pause)”状态来进行PCD和PICC间的通信。
　　图2 TYPE A 调制波形
　　TYPE B调制使用RF工作场的ASK10%调幅来进行PCD和PICC间的通信。
　　调制指数最小应为8%，最大应为14%。
![](http://dev.yesky.com/imagelist/2009/244/5ig85d5mb0tz.jpg)
　　图3 TYPE B 调制波形
　　根据二者的设计方案不同，可看出，TYPE A 和 TYPE B有以下不同：
　　◆TYPE B接收信号时，不会因能量损失而使芯片内部逻辑及软件工作停止。在NPAUSE到来，TYPE A的芯片得不到时钟，而TYPE B用10%ASK，卡片可以从读写器获得持续的能量; TYPE B时容易稳压，所以比较安全可靠。TYPE A卡采用100%调制方式，在调制发生时候无能量传输，仅仅靠卡片内部电容维持，所以卡片的通讯必须达到一定的速率，在电容电量耗完之前结束本次调制，否则卡片会复位。
　　◆负载波采用BPSK调制技术，TYPE B较TYPEA方案降低了6dB的信号燥声，抗干扰能力更强。
　　◆外围电路设计简单。读写机具到卡及卡到读写机具的编码方式均采用NRZ方案，电路设计对称，设计时可使用简单的UARTS，TYPE B更容易实现。
　　2、 防冲突机制
　　ISO/IEC 14443-3规定了TYPEA，TYPEB 的防冲突机制。二者防冲突机制的原理完全不同。前者是基于BIT冲突检测协议，后者则是通过字节、帧及命令完成防冲突。
　　RFID的核心是防冲突技术，这也是和接触式IC卡的主要区别。
　　TYPE A PICC防冲突和通信使用标准帧用于数据交换，并按以下顺序组成：
　　——通信开始;
　　——n*(8个数据位+奇数奇偶校验位)，n≥1。每个字节的LSB首先被发送。每个字节后面跟随
　　一个奇数奇偶校验位。奇偶校验位P被设置，使在(b1到b8，P)中1s的数目为奇数;
　　——通信结束。
![](http://dev.yesky.com/imagelist/2009/244/hq8ebe5xggd4.gif)
　　第1字节
　　图4 TYPE A 标准帧
　　TYPE A PICC的初始化和比特冲突检测协议是当至少两个PICC同时传输带有一个或多个比特位置(该位置内至少有两个PICC在传输补充值)的比特模式时，PCD会检测到冲突。在这种情况下，比特模式合并，并且在整个(100%)位持续时间内载波以负载波进行调制。
![](http://dev.yesky.com/imagelist/2009/244/8y38j02n8lb7.jpg)
　　图5 TYPE A PICC状态图
　　TYPE B PICC防冲突和通信初始化期间使用的字节、帧和命令的格式。
　　PICC和PCD之间的字节通过字符来发送和接收，在防冲突序列期间，字符的格式如下：
　　——1个逻辑“0”起始位;
　　——8个数据位发送，首先发送LSB;
　　——1个逻辑“1”停止位。
　　最高位 停止位 EGT
　　起始位 最低位
　　用一个字符执行一个字节的发送需要10etu，如图18示。
![](http://dev.yesky.com/imagelist/2009/244/t2j5iklmpgpo.jpg)
　　图6 TYPE B字符格式
　　PCD和PICC按帧发送字符。帧通常用SOF(帧的起始)和EOF(帧的结束)定界。　　
|SOF|字符|EOF|
　　图7 TYPE B帧格式
　　在防冲突序列期间，可能发生两个或两个以上的PICC同时响应：这就是冲突。命令集和允许PCD处理冲突序列以便及时分离PICC传输。
　　在完成防冲突序列后，PICC通信将完全处于PCD的控制之下，每次只允许一个PICC通信。
　　防冲突方案以时间槽的定义为基础，要求PICC在时间槽内用最小标识数据进行应答。时间槽数被参数化，范围从1到某一整数。在每一个时间槽内，PICC响应的概率也是可控制的。在防冲突序列中，PICC仅被允许应答一次。从而，即便在PCD场中有多个卡，在一个时间槽内也仅有一个卡应答，并且PCD在这个时间槽内能捕获标识数据。根据标识数据，PCD能够与被标识的卡建立一个通信信道。
　　防冲突序列允许选择一个或多个PICC以便在任何时候进行进一步的通信。
![](http://dev.yesky.com/imagelist/2009/244/65fin9du0nv6.gif) 图8 TYPE B PICC状态图
　　从建立PCD与PICC([CPU](http://product.yesky.com/cpu/)卡)之间通信的方面来比较：
　　TYPE A类型卡片 需要的基本命令有：
　　l REQA 对A型卡的请求 或(WAKE-UP 唤醒)
　　l ANTICOLLISION 防冲突
　　l SELECT 选择命令
　　l RATS 应答响应
![](http://dev.yesky.com/imagelist/2009/244/4w1w5mcw28aq.gif)
　　图9 TYPE A PICC激活
　　TYPE B类型卡片 需要的基本命令有：
　　l REQB 对B型卡的请求
　　l ATTRIB PICC选择命
　　TYPE B PICC激活如图8 所示
　　从以上的比较可以看出：
　　u TYPE B类型卡片具有使用更少的命令，更快的响应速度来实现防冲突和选择卡片的能力。
　　u TYPEA的防冲突需要卡片上较高和较精确的时序，因此需要在卡和读写器中分别加更多硬件，而TYPE B的防冲突更容易实现。
　　目前TYPE A和TYPEB 孰优孰劣尚在争议中。TYPE A的产品(Mifare卡)具有更高的市场普及率;但是TYPE B应该在安全性、高速率和适应性方面有更好的前景，代表产品如二代身份证。
--------------------- 本文来自 青蛙嘎嘎 的CSDN 博客 ，全文地址请点击：https://blog.csdn.net/luoqindong/article/details/54705862?utm_source=copy
