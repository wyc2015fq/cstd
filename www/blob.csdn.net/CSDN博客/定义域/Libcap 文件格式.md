# Libcap 文件格式 - 定义域 - CSDN博客
置顶2015年11月16日 18:16:01[定义域](https://me.csdn.net/u010278882)阅读数：604
# Libcap 文件格式
`Libcap` 作为捕获网络通信数据的事实上的标准工具。其存储截获的数据的文件格式也几乎成为事实上的标准格式。通常，推荐使用的文件扩展名是`pcap`。下面介绍`Libcap`库当前版所使用的文件格式，该文件格式的版本是`2.4`，从`Libcap 0.4`开始到现在该文件格式时始终没有发生变化。
> 
注意：文件格式没有发生变化，但是解析库有变化。`Lipcap 1.5.0`开始支持纳秒级别的时间精度的时间戳，但是该时间精度仅能被新版`Libcap`读取，老版本的`Libcap`以及`Wincap`均不能读取其中的纳秒数据。老版的`WireShark`也不支持读取纳秒时间的时间戳，不过现在的版本支持读取。
文件结构
![这里写图片描述](https://img-blog.csdn.net/20151116154609040)
每个截取的文件以全局头开始，然后截取的每个包的包头和包数据。
## Global Header
|field|length|desc|
|----|----|----|
|magic_number|4 bytes|1.文件格式的标识（其他程序根据该魔术确定是`Libcap`文件，比如`.class`文件的魔数是`cafe babe`;  2. 本地字节读取顺序（i）`0xa1b2c3d4` MSB 直接读（ii）`0xd4c3b2a1` LSB 反向读|
|version_major|2 bytes|`Libcap` 文件格式主版本号|
|version_minor|2 bytes|`Libcap` 文件格式次版本号|
|thiszone|4 bytes|本地时间与`GMT` 时间的差别时间，单位是秒。比如本地是`UTC+8:00`,那么thiszone则为+3600|
|sigfigs|4 bytes|截取的数据的时间戳的时间精度，通常所有工具默认为0|
|snaplen|4 bytes|截取的数据的最大长度（受各种不同的工具约束），单位是字节（byte），通常是比65536(0xFFFF)更大的值|
|network|4 bytes|截取的数据的所在网络链路层头类型，参考[各种链路层头类型](http://www.tcpdump.org/linktypes.html)|
### 为什么要定义本地字节的读取顺序?
内存可以想象成存储单元的数组，每个存储单元通常为8位(bit)； 
当数据的表示/编码所需要的位数 > 存储单元的能够存储的数据的位数（比如，Java中int类型为4个字节32位）时，常用的方法是使用多个连续存储单元来存储一个数据。 
关于具体如何存储，目前有两种存储方式（本地字节顺序）：最高有效位存在低地址中（big-endian/MSB Most Significant Bit）和最低有效位存在低地址中（little-endian/LSB Least Significant Bit）。
示例，0x90AB12CD 
![MSB存储](https://img-blog.csdn.net/20151116164139018)
![LSB存储](https://img-blog.csdn.net/20151116164153937)
### 字节一定是8位吗?
不一定。还存在半字节的说法。
- 8位字节 octet
- 4位字节 quartet/nibbles
- 2位字节 doublet
## Packet Header
|field|length|desc|
|----|----|----|
|ts_sec|4 bytes|包被截取的时间的秒数，从UN*X时刻开始|
|ts_usec|4 bytes|包被截取的时间的毫秒数|
|incl_len|4 bytes|真正被保存在文件中截取的数据的8位字节的数量|
|orig_len|4 bytes|包的实际8位字节长度|
### UN*X 时刻
1970年1月1日 00:00:00 GMT
## Packet Data
数据直接跟在 `Packet Header` 后，具体截取的数据与截取数据所使用的工具及其参数有关，参考《802.3 和 802.11 网络帧格式》
![Libcap File Format](https://img-blog.csdn.net/20151116181517471)
## 支持库
- [libcap](http://www.tcpdump.org/) 文件格式的最初定义，用于类UNIX系统
- [WinCap](http://www.winpcap.org/) 用于Windows系统的移植
Java 编程语言支持（使用前提需要使用上面的任意一个库）
- [pcap4](https://github.com/kaitoy/pcap4j)
- [jpcap](https://github.com/jpcap/jpcap)
- [jNetPcap](http://jnetpcap.com/) tcpdump的Java实现
## PcagNg
当前版本的文件格式比较简单，不支持许多必要的特定，因此`Libcap` 提出了[`下一代文件截取格式`](https://github.com/pcapng/pcapng) 的计划。该文件格式的支持情况：
- Libcap 1.1.0 开始能够读但不能写
- Wireshark 能够在一定程度山读写
## 参考
- [Libpcap File Format](https://wiki.wireshark.org/Development/LibpcapFileFormat)
- [Big & Little endian](https://www.cs.umd.edu/class/sum2003/cmsc311/Notes/Data/endian.html)
