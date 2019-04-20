# 物理层下行信道之PCFICH - 风雪夜归人 - CSDN博客
2017年03月17日 16:18:20[cdjccio](https://me.csdn.net/qq_34624951)阅读数：556
PCFICH仅携带CFI而没有任何其它信息。即使位模式的类型仅为4，CFI也由31位数据组成。
## 一、信道描述
   1、其携带可用于控制信道（PDCCH和PHICH）的符号的数目。
      2、映射到每个下行链路子帧中的第一OFDM符号这包含关于从PBCHUE接收的用于PDCCH的OFDM符号的数量和PHICH符号持续时间的信息，对该信道进行解码，以找出多少个OFDM符号被分配用于PDCCH。
      3、它是子帧的第一OFDM符号的16个数据子载波。
      4、PCFICH数据由4个REG承载，并且这四个REG在整个频带上均匀分布，而与带宽无关。
      5、PCFICH的确切位置由小区ID和带宽确定。
## 二、处理PCFICH信息
![](https://img-blog.csdn.net/20170317161324003?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzQ2MjQ5NTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
## 三、CFI信息
CFI是指示在每个子帧处有多少OFDM符号用于承载控制信道（例如，PDCCH和PHICH）的指示符。 
      从表面看，PCFICH携带的信息很简单。它是CFI号，可以是1或2或3。如果对于子帧将CFI设置为1，则意味着子帧处的一个符号（第一符号）用于PDCCH分配。如果CFI是2，则意味着两个符号（第一和第二符号）用于PDCCH。...
![](https://img-blog.csdn.net/20170317161449544?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzQ2MjQ5NTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
     比特模式和CFI值映射如下（3GPP 36.212 5.3.4控制格式指示符）：
![](https://img-blog.csdn.net/20170317161529973?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzQ2MjQ5NTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
     对于具有“系统带宽> 10的RB的最大数目”的系统带宽，以OFDM符号为单位的DCI的跨度（1、2、3）由CFI给出。
     对于具有“系统带宽<= 10的RB的最大数目”的系统带宽，以OFDM符号为单位的DCI的跨度（2、3、4）由CFI+1给出。
     可以在任何带宽中指定任何值1,2,3，但是从“CFI值”到“DCI范围的OFDM符号数”的转换是不同的，如下：
     情况1：对于具有“系统带宽> 10的RB的最大数目”的系统带宽，DCI的跨度的OFDM符号的数量= CFI值。
     情况2：对于具有“系统带宽<= 10的RB的最大数目”的系统带宽，DCI的跨度的OFDM符号的数目= CFI值+1。
     可以在4.3.3.3下行物理信道和信号到物理资源的映射中找到另一个指南，如下所示：
     对于1.4,3和5 MHz系统带宽，CFI = 3
     对于10,15和20 MHz系统带宽，CFI = 2
## 四、PCFICH的信息
不管系统带宽如何，PCFICH总是由每个子帧的第一符号处的4个REG（16个RE）来承载。 PCFICH的这四个REG的确切位置由物理小区ID和系统BW（带宽）根据如下所示的公式确定。
![](https://img-blog.csdn.net/20170317161608921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzQ2MjQ5NTE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



