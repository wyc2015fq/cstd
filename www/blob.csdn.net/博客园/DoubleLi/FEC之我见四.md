# FEC之我见四 - DoubleLi - 博客园







接上文，来详细的说明一下FEC前向纠错的具体实现：

FEC_matrix是一个比较常用的[算法](http://lib.csdn.net/base/datastructure)，Vandermonde,范德蒙矩阵是法国数学家范德蒙提出的一种各列为[几何级数](http://baike.baidu.com/subview/703105/703105.htm)的矩阵。

范德蒙矩阵的定义:

V =![](http://img.blog.csdn.net/20160328150929196?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

其第i 行、第j 列可以表示为(αi)^(j-1)。

范德蒙矩阵的性质：范德蒙矩阵行数为m，列数为n，矩阵具有最大的秩min(m, n)。

范德蒙矩阵的应用：范德蒙矩阵应用之一就是在[纠错编码](http://baike.baidu.com/subview/11770706/12138701.htm)中，常用的[纠错码](http://baike.baidu.com/subview/126214/126214.htm)Reed-solomon 编码中冗余块的编码采用的即为范德蒙矩阵。

1）码流层面上的FEC编码

在块编码中，信道编码器将对码流中连续的k个比特划分成一块，然后对这k个比特添加n-k个冗余比特，产生一个n比特的编码块。编码块经过信道后传送到接收端，这种编码块称为（n，k）块编码，其中k个比特称作信息位，n-k个比特称作校验位

![](http://img.blog.csdn.net/20160328155106680?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

码流层面上的FEC编码示意图

（n，k）可以纠正长为b比特的突发错误，其中b<=[1/2(n-k)}];如果知道发生错位的位置，那么FEC可以纠正长度为b比特的错误， b<=n-k。

2）数据包层面上的FEC编码

数据包层面上的FEC编码常用语恢复传输过程中丢失的数据包，这种FEC编码的基本思想是首先将码流分成多个分段，这些分段构成多个原始数据包，然后采用块编码，有k个原始的数据包产生n-k个冗余包，构成包含n个数据包的块，其中n>k。这个n-k个冗余包和k个数据包一起都通过信道进行传输。如果原始数据包没有丢失，那么接收端可以忽略所有的冗余包，如下图是有误码情况下FEC编解码示意图。

![](http://img.blog.csdn.net/20160328162844460?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


有误码（或丢失）情况下的FEC编解码示意图

一般地，FEC编码会存在以下问题：

1）FEC造成传输速率的增加。这是因为k个信息比特就要增加n-k个冗余比特，因此传输码率要扩大n/k倍。另外，信道误码率越高，恢复误码所需要的传输速率就越高。

2）FEC使传输时延增加。这是因为信道编码器需要得到k个数据包才能开始进行信道编码。而信道解码器也必须正确接收到k个数据包后才能开始解码。![](http://img.blog.csdn.net/20160329115608428?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

3）FEC难以适应信道误码特征性的动态变化，只有在信道状态稳定时才能得到良好的性能。在信道状况恶化的情况下，如果信道保护不足，传输中出现的误码超出了FEC的误码恢复能力，那么FEC编码不仅没有起到保护作用反而造成传输带宽的浪费。反之，如果在信道状况良好的时候施加过多的信道保护，人会造成传输资源的浪费。因此，在无线、因特网这类时变得网络中进行视频流传输时，往往采用自适应的FEC保护机制。

RS码类纠删码：RS码类生成的矩阵为范德蒙矩阵和柯西矩阵，相应的纠删码分别为范德蒙码和柯西码。

低密度纠删码： 基于删除信道的低密度校验码（LDPC码）称为低密度纠删码，它的生成矩阵为系数矩阵。

RS码是一类有很强纠错能力的多进制BCH码，也是一类典型的代数几何码。RS码广义上属于BCH码的一个子类，但因为RS编码基于非二进制符号，所以它不但继承了BCH码抗随机误码的能力，同时又具有抗突发误码的能力，通常作为纠删码的使用。

RS码根据其生成矩阵不同，可分为范德蒙码和柯西码。


- 范德蒙码
- 定义：若选取编码生成矩阵G*kxm*，使得![](http://img.blog.csdn.net/20160329115052864?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)，其中![](http://img.blog.csdn.net/20160329120006946?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)，![](http://img.blog.csdn.net/20160329115948680?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)（p为素数，r为正整数），则所得纠删码为范德蒙码，G的任意k列组成的子方阵
 G’ 的转置矩阵![](http://img.blog.csdn.net/20160329120204899?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)为范德蒙矩阵，若xi（i=1,2，...，k）互不相同，则![](http://img.blog.csdn.net/20160329120657448?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)，从而![](http://img.blog.csdn.net/20160329120710567?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)，即G的任意k列组成的子方阵
 G‘ 为非奇异（G的任意k列线性无关）的，因此这样得到的矩阵满足最优纠删码生成矩阵的特性。
- 柯西码
- 定义：设{x1,x2,...,xn}和{y1，y2，...，yn}是有限域F中两个元素集，若对
- （1）![](http://img.blog.csdn.net/20160329131416802?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)，![](http://img.blog.csdn.net/20160329131432365?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)有xi+yi#0
- （2）对![](http://img.blog.csdn.net/20160329131601709?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
 有xi#xj和![](http://img.blog.csdn.net/20160329131728741?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)有yi#yj
- 则称下图的矩阵为域F上的柯西矩阵。
- ![](http://img.blog.csdn.net/20160329133213872?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

- 在有限域F上，设![](http://img.blog.csdn.net/20160329133416038?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)为单位矩阵，为柯西矩阵，若取生成矩阵G=（I|C），则称所得纠删码为柯西码。



理想情况下，基于RS码的每个编码组共有n个分组，其中有k个数据分组，其余为n-k个校验分组，这n个分组在网络中传输时，接收端只要收到任意k个分组就可以恢复出全部k个数据分组，这样的码又称为极大最小距离可分码（MDS）。由于实际传输信道的丢包率经常会发生变化，当某个时段内丢包率很大时，即使胫骨RS码的信道编码保护，仍然会存在分组丢失的可能。




- 范德蒙码的纠删性能，其编码时间复杂度为O(n*n)
- 柯西码，其编码时间复杂度O(n*n)
- 由于柯西解码不用求大矩阵的逆，而且把乘法和除法运算分别转化为有限域上的加法和减法运算，可用异或运算实现，因此，柯西码运算度咋读低于范德蒙码。整体来讲RS码的缺点是编译码速度较慢，且不能避免数据的重传。














