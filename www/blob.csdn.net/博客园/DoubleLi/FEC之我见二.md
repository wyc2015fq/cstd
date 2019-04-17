# FEC之我见二 - DoubleLi - 博客园






前面简单说了一下FEC，以及它的配合使用的方法。下面我想详细说一下FEC[算法](http://lib.csdn.net/base/datastructure)：

曾经有位大神在帖子里这么写着：采用改进型的vandermonde矩阵RS算法.其优点算法运算复杂度更低且解决了利用矩阵构造RS码当矩阵奇异时，构造的纠错码不为RS码的问题。


- FEC的方案：在RTP或私有协议头上扩展出包组头（Group head）,一个Group有k个媒体包和r个冗余包组成，他们在Group内拥有不同的组号，通过组号的连续性可以判断本组内数据包的丢失情况，从而选择性的予以恢复（冗余包丢失无需恢复）、因为UDP协议保障了包内数据的正确性，所以我们无需考虑包内纠错的情况。Group是一个完整的独立的FEC处理单元，不同Group之间无相关性。由于冗余性的存在，一个Group中任意k个数据包可以用来重建k个原始媒体包，如果丢失数据包数小于等于r，接受者收到一个Group中任意的k个数据包后，即可以通过组号信息确定丢失包的相对位置并进行FEC解码，以恢复k个原始媒体包。这里我们定义冗余包数r与原始媒体包数k的比值为FEC编码冗余度r/k，冗余度越高，抗丢包能力越强，同时传输效率也越低。



下面借鉴大神的FEC编解码算法进行简述：

1）数据包分割

对数据包FEc编码运算首先进行的是包内分割，将数据包分割为多个定长单元，定长单元成为自，设字长为w bits，w的取值一般为8/16/32。FEc编码对k个原始媒体包朱子进行处理，生成m个冗余数据包与之对应的字。

例如：现有两个原始数据包D1、D2，包的长度都为b bytes（对于包长不足b bytes的使用0补齐）-- b B，字长为w  bits -- w 位，那么一个数据包的总字长为1 = 8b/w。用这两个冗余包C1、C2的过程简述如下：

![](http://img.blog.csdn.net/20160325153946226?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

图中F代表FEC编码运算

2）Vandermonde编解码以及改进

设k个原始媒体包D=（D1，D2，...，Dk），，r个冗余数据包C=（C1，C2，...，Cr），那么传输组Group表示为Y=（Y1，Y2，...,Yn），其中Yi=Di（0<=i<=k-1）,Y

j=Cj(k<=j<=n-1)。B为 n x k 维 FEC生成矩阵，有单位矩阵I和矩阵G组成，则一个Group可表示为如下所示：

![](http://img.blog.csdn.net/20160325155308700?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

通过这种方式构造的RS码是系统码，信息组以不变的形式在码组的任意k位（通常在最前面： D1，D2，...，Dk）。如果以数据包为对象，那么传输组的前k个包就是k个被保护的数据包。在接收端，如果接收者收到了Group中的任意k个数据包，即可根据所收到的数据包在Group中的位置信息，从FEC生成矩阵B中提取对应的行，组成一个新的 kxk 维矩阵B‘，显然 

![](http://img.blog.csdn.net/20160325155514210?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



如果B’ 为非奇异矩阵，那么就可以通过如下逆变换得到原始数据报，完成恢复。

![](http://img.blog.csdn.net/20160325160456159?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



设计RS码的关键在于怎样设计生成矩阵B，也就是其系数矩阵G。本方案使用Vandermonde矩阵来构建系数矩阵G。常规定义Vandermonde矩阵V，r x k 维，如下所示：



![](http://img.blog.csdn.net/20160325160534048?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





系数矩阵G=V，该矩阵元素的运算都是在有限域GF（2^8）中进行的。Gij(i=0,1,...,r-1; j=0,1,...,k-1)为系数矩阵的元素，Ci（i=1,2，。。。，r）表示第i个冗余包，Dj（j=1,2，。。。，k）表示第j个原始媒体包，根据下式：

![](http://img.blog.csdn.net/20160325162740529?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

上式运算时以包分割后的数据为运算单位的，模运算使用查表方式实现。例如发端使用k=6，r=2的荣誉模式，那么对应的系数矩阵为：

![](http://img.blog.csdn.net/20160325163101080?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


根据上面系数矩阵，可以计算得到冗余包为：

![](http://img.blog.csdn.net/20160325163708545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

生成冗余包C1C2，发送端就可以一次发送原始媒体包和冗余包。如果发送的途中原始媒体包D3，D4丢失，那么接收端就可以根据收到的包恢复丢失的原始媒体包，具体过程如下：
- 由于接收到的原始媒体包再次产生冗余包：



![](http://img.blog.csdn.net/20160325165756131?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


- 将其与接收到的冗余包作比较，就能得到丢失的原始媒体包的表达式：



![](http://img.blog.csdn.net/20160325170053210?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


- 要求出D3、D4，可利**用高斯消除法**求出系数矩阵 的逆 矩阵，前提是该矩阵是非奇异的。


传统的Vandermonde矩阵构造RS码是，需要非奇异矩阵，由于Vandermonde矩阵元素取值与有限域，且元素的运算遵循有限域的运算规则，就会存在一定概率出现矩阵奇异，用该矩阵构造的纠错码就不是RS码，不能从任意k个包中恢复出原始媒体包。为此长沙这位大婶对该传统Vandermonde矩阵进行改进，解决了矩阵概率奇异的问题，具体实现见代码。



3）私有协议/RTP与FEC的结合，下文继续讲解









