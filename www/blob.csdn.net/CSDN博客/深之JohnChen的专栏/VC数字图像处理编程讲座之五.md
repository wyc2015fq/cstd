# VC数字图像处理编程讲座之五 - 深之JohnChen的专栏 - CSDN博客

2005年11月30日 09:26:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：6971


**图像压缩编码基础**

图像编码与压缩，从本质上来说，就是对要处理的图像源数据用一定的规则进行变换和组合，从而达到以尽可能少的代码（符号）来表示尽可能多的数据信息的目的。压缩通过编码来实现，或者说编码带来压缩的效果。所以，一般把此项处理称为压缩编码。

压缩的理论基础是信息论。从信息论的角度来看，压缩就是去掉信息中的冗余，即保留不确定的信息，去掉确定的信息（可推知的），也就是用一种更接近信息本质的描述来代替原有冗余的描述。这个本质的东西就是信息量（即不确定因素）。

在数字图像压缩中，有三种基本的数据冗余：像素相关冗余，编码冗余，心理视觉冗余。如果能减少或者消除其中的一种或多种冗余，就能取得数据压缩的效果。
1. 像素相关冗余：指图像的像素之间一般都有相关性。根据相关性，由一个像素的性质可以获得其相邻像素的性质。这种冗余常称为空间冗余或几何冗余。另外连续图像序列中的各连续帧之间的冗余也是一种像素相关冗余。
2. 编码冗余：图像编码需要用一些符号（序列），根据一定的规则来表达图像。这些符号序列就叫码字，每个码字里的符号个数就叫码字长度。在实际图像中，每个信息出现的概率不一样，如果它们对应的码字长度都一样的话，就不能使编码产生的符号序列达到最小，就会产生编码冗余。
3. 心理视觉冗余：因为人的眼睛并不是对所有视觉信息有相同的敏感度，这些不敏感的信息就是心理视觉冗余的。去掉这种冗余信息，并不会明显降低主观图像质量，但实际上会导致定量信息的损失。

一般情况下，编码器包括以下顺序的三个独立操作，而对应的解码器包含反序的两个独立操作，如图所示：
![](http://hqtech.nease.net/Article/Images/ImageCodec.jpg)
注意：因为量化操作是不可反转的，所以解码器里没有对量化的逆操作模块。

根据解码图像对原始被压缩图像的保真程度，图像的压缩方法可分成两大类：有损压缩和无损压缩。下面介绍图像编码的香农（Shannon）信息论第一定理——无损编码理论。如果用H(u)表示信源的熵，定义观察到单个信源符号输出时所获得的平均信息量。熵达到最大的情况出现在信源各符号的出现概率相等时，而信源此时提供最大可能的每信源符号平均信息量。那么，信源编码可以使每个信源符号所需要的码字符号的平均数L’avg/n小到逼近信源的熵，但不能比信源的信息熵更小。根据这一定理，图像信源无损编码所能达到的最低码率是受图像信源的信息熵限制的。一个给定的编码方案，其编码效率y可定义为：
y = n * H(u) / L’avg。

图像编码的方法很多，主要分成四大类：像素编码，预测编码，变换编码 和其他编码。
1. 像素编码：指在编码时对每个像素单独处理，不考虑像素之间的相关性。常用的方法有：脉冲编码调制（Pulse Code Modulation，简称PCM）、熵编码（Entropy Coding）、游程编码（Run Length Coding）、位平面编码（Bit Plane Coding）。
2. 预测编码：指只对新的信息进行编码，从而去掉相邻像素之间的相关性和冗余性。常用的方法有：增量调制（Delta Modulation，简称DM）、差分脉冲编码调制（Differential Pulse Code Modulation，简称DPCM）。
3. 变换编码：指将给定的图像变换到另一个数据域（如频域）上，使得大量的信息能有较少的数据来表示。常用的方法有：离散傅立叶变换（DFT）、离散余弦变换（DCT）、离散哈达玛变换（DHT）。
4. 其他编码方法，常见的有：混合编码（Hybrid Coding）、矢量量化（Vector Quantize，简称VQ）、LZW算法等。还有近年来新出现的编码方法，如使用人工神经元网络（Artificial Neural Network，简称ANN）的算法、分形（Fractal）、小波（Wavelet）、基于对象（Object-Based）的算法、基于模型（Model-Based）的算法等。
常见的霍夫曼（Huffman）编码、香农-费诺（Shannon-Fano）编码以及算术编码（Arithmetic Coding）都属于统计编码 （对于无记忆性信源来说，像素间没有相关性，而根据像素灰度值出现概率的分布特性进行的压缩编码叫做统计编码）的范畴。

从图像压缩编码的发展过程来说，可以分成三个阶段：
第一代，着重于图像信息冗余度的压缩方法。
第二代，着重于图像视觉冗余信息的压缩方法。
第三代基于模型的图像压缩方法。

有关图像压缩编码的国际标准主要有以下几种：
1. JPEG标准，Joint Picture Expert Group，于1991年提出。
2. JPEG 2000标准，1997年提出，1999年完成。JPEG 2000基于小波变换的静止图像压缩标准，不仅有更优秀的压缩性能，而且有更丰富的处理功能。
3. MPEG-1标准，用于多媒体和广播电视，数据率要求1.5Mbps。
4. MPEG_2/H.262标准，DVD制式的压缩标准，数据率要求4-10Mbps。
5. MPEG-4标准，1999年完成第三版，是一个新的视频和音频编码的国际标准。最大特点是基于对象的编码方式以及对合成对象的编码能力。支持固定和可变速率视频编码（低速小于64Kbps，中速64K-384Kbps，高速384K-4Mbps），目的在于提供适合用于交互多媒体环境下应用的核心技术，解决视频信号的有效存储和传输问题。
6. MPEG-7标准，对图像内容进行描述，根据描述一般检索。
7. H.261，全彩色实时视频图像压缩标准。
8. H.263，低码率通信视频图像编码标准。

**数字压缩编码技术**

**1.图像压缩编码的可能性**

从信息论观点来看，图像作为一个信源，描述信源的数据 是信息量(信源熵)和信息冗余量之和。信息冗余量有许多种，如空间冗余，时间冗余，结构冗余，知识冗余，视觉冗余等，数据压缩实质上是减少这些冗余量。可见冗余量减少可以减少数据量而不减少信源的信息量。从数学上讲，图像可以看作一个多维函数，压缩描述这个 函数的数据量实质是减少其相关性。另外在一些情况下，允许图像有一定的失真，而并不妨碍图像的实际应用，那么数据量压缩的可能性就更大了。

**2.图像压缩编码方法的分类**

编码压缩方法有许多种，从不同的角度出发有不同的分类方法，比如从信息论角度出发可分 为两大类：
 (1)冗余度压缩方法，也称无损压缩，信息保持编码或熵编码。具体讲就是解码图像和压缩 编码前的图像严格相同，没有失真，从数学上讲是一种可逆运算。
 (2)信息量压缩方法，也称有损压缩，失真度编码或熵压缩编码。也就是讲解码图像和原始图像是有差别的，允许有一定的失真。
 应用在多媒体中的图像压缩编码方法，从压缩编码算法原理上可以分类为：
 (1)无损压缩编码种类
·哈夫曼编码
 ·算术编码
 ·行程编码
 ·Lempel zev编码
 (2)有损压缩编码种类
 ·预测编码：DPCM，运动补偿
 ·频率域方法：正文变换编码(如DCT)，子带编码
 ·空间域方法：统计分块编码
 ·模型方法：分形编码，模型基编码
 ·基于重要性：滤波，子采样，比特分配，矢量量化
 (3)混合编码
 ·JBIG，H261，JPEG，MPEG等技术标准
 衡量一个压缩编码方法优劣的重要指标是：
 (1)压缩比要高，有几倍、几十倍，也有几百乃至几千倍；
 (2)压缩与解压缩要快，算法要简单，硬件实现容易；
 (3)解压缩的图像质量要好。
 最后要说明的是选用编码方法时一定要考虑图像信源本身的统计特征；多媒体系统(硬件和 软件产品)的适应能力；应用环境以及技术标准。

**3.压缩编码方法简介**

压缩编码的方法有几十种之多，并在编码过程中涉及较深的的数学里理论基础问题，在此仅介绍 几种常用的压缩编码方法，主要是从物理意义上作一定的解释，读者如对数据压缩专题感兴 趣的话，请参看讲座结束后所附的参考资料。

3.1 莫尔斯码与信源编码
 莫尔斯码即电报码，其精华之处在于用短码来表示常出现的英文字母，用长码来表示不常出 现的字母，以减小码率。这种方法非常有效，故延用至今。电视信号经过变换后，例如经差值脉冲编码后，发现前后像素幅度差值小的概率大，而差值大的概率小，因此可用短码表示 概率大的信号，而用长码来代表概率小的信号，从而达到压缩码率的目的。

3.2 差值脉冲编码
 电视图像基本上是由面积较大的像块(如蓝天、大地、服装等)组成。虽然每个像块的幅值各 不相同，但像块内各样值的幅度是相近的或相同的，幅值跃变部分相应于像块的轮廓，只占整幅图像的很小一部分。帧间相同的概率就更大了，静止图像相邻帧间的相应位置的像素完 全一样，这意味着前后像素之差或前后帧间相应位置像素之差为零或差值小的概率大，差值 大的概率小。这就是差值编码的基本想法，其原理框图见图4－1(a)。发端将当前样值和前一 样值相减所得差值经量化后进行传输，收端将收到的差值与前一个样值相加得到当前样值。 在这个原理图中，输出的当前样值是输出的前一样值加上收到的差值，由于在当前差值中包 括当前的量化误差，而输出的前一样值又包括前一样值的量化误差，这就造成了量化误差的 积累。因此实用电路为图4－1(b)。这时输入当前样值不是与输入的前一样值相减，而是与 输 出的前一样值相减，因此在差值中已经包含了前一样值的量化误差的负值，在与输出的前一 个样值相加时，这部分量化误差被抵消，只剩下当前的量化误差，这就避免了量化误差的积累。
![](http://hqtech.nease.net/Article/Images/DigitalCoding32.gif)

3.3 预测编码
 预测编码利用像素的相关性，可进一步减小差值。
 从前面的分析可以看出，如果差值编码中小幅度出现的机会增加，由于其对应的码长较短， 总数码率会进一步减小。如果能猜出下一个样值，那么差值就会是零，当然这种情况是没有 意义的，因为若预先知道下一样值，就不需要进行通信了。但可以肯定，如果我们不仅利用 前后样值的相关性，同时也利用其它行、其它帧的像素的相关性，用更接近当前样值的预测 值与当前样值相减，小幅度差值就会增加，总数码率就会减小，这就是预测编码的方法。预 测编码的电路与差值编码类似，或者说差值编码就是以前一样值为预测值的预测编码，又称为一维预测。如果用到以前行的像素或以前帧的像素，则称为二维或三维预测。在美国国际 电话电报公司(ITT)生产的数字电视机芯片中有一个视频存储控制器芯片VMC2260就用了二维 预测编码，预测器用了三个像素作为下一个像素的预测值，即预测值等于1/2前一像素加1/4 上一行相应像素再加上1/4上一行相应的前一像素。这样不仅利用了前一像素的相关性，也 利用了上一行相应像素的相关性，这样做要比差值编码有更大的码率压缩。如果再用上前一 帧的像素会进一步降低数码率。但为了得到前一帧的像素必须要使用帧存储器，造价比较高 。只用到帧内像素的处理称为帧编码(Intraframe Coding)，用到前后帧像素的处理称为帧 间编码(Interframe Coding)。要得到较大的码率压缩就必须使用帧间编码。JPEG是典型的帧内编码方案，而MPEG是帧间编码方法。前者大多用于静止图像处理，而后者主要用于对运 动图像的处理。 
 
3.4 哈达玛特变换
 这是一种有效地去除噪波的方法，噪波的存在往往容易和小幅度变化的信号相混淆，利用多帧平均的方法，对于静止图像，各帧相同，平均的结果其值不变，对于噪波，多帧平均趋于零。
 但如果图像中有运动，多帧平均就会造成运动模糊，故不能简单地进行平均，需要根据运动的大小来调节反馈量，即调节平均的程度，做到运动自适应降噪。
 大多数情况下是利用帧差信号来判断图像中是否有运动，如果帧差小于一定值，就可视为是因噪波引起的，可取较大的反馈量；如果帧差大于一定值，就可视为图像中有运动。
 但在许多情况下，仅从幅度的大小来判断是杂波还是图像是很困难的，如移动的云，近摄的 绿草地等图像信号所得到帧差信号也很小，所以BKU-904采用二维哈达玛特变换(Hadamard Transform)来区分是噪波还是图像信号。先将输入值按4×2分成小块，分别进行实时快速哈 达玛特变换(FHT)。
 图像经变换后，转换成相应成分的系数，这些系数分别代表直流分量；水平方向细节和色度 分量等；垂直方向细节；斜方向细节及色度分量等，而噪波变换后均匀散在各系数中。这样 就更有效地区分出信号和噪波，从而达到更有效地进行自适应降噪的目的。
 
3.5 离散余弦变换
离散余弦变换(Discrete cosine Transform)简称DCT。任何连续的实对称函数的傅里叶变换 中只含余弦项，因此余弦变换与傅里叶变换一样有明确 的物理量意义。DCT是先将整体图像分成N×N像素块，然后对N×N像素块逐一进行DCT变换。 由于 大多数图像的高频分量较小，相应于图像高频成分的系数经常为零，加上人眼对高频成分的 失真不太敏感，所以可用更粗的量化，因此传送变换系数所用的数码率要大大小于传送图像 像素所用的数码率。到达接收端后再通过反离散余弦变换回到样值，虽然会有一定的失真 ，但人眼是可以接受的。
 N代表像素数，一般N=8，8×8的二维数据块经DCT后变成8×8个变换系数，这些系数都 有 明确的物理意义：U代表水平像素号，V代表垂直像素号。如当U=0，V=0时，F(0，0)是原 64个 样值的平均，相当于直流分量，随着U、V值增加，相应系数分别代表逐步增加的水平空间频 率分量和垂直空间频率分量的大小。

3.6 量化(Q)
 严格说DCT本身并不能进行码率压缩，因为64个样值仍然得到64个系数，如图4－2所示。这 里 给出了一个8×8像块的具体例子，经DCT变换后，比特数增加了。在这个例子中样值是8比特 ，从0~225得到的XX,即直流分量的最大值是原来256的64/8倍，即0~2047，交流分 量的范围是-1024~1023。只是在经过量化后，特别是按人眼的生理特征对低频分量和高频分 量设置不同的量化，会使大多数高频分量的系数变为零。一般说来，人眼对低频分量比较敏 感，而对高频分量不太敏感。因此对低频分量采用较细的量化，而对高频分量采用较粗的量化。![](http://hqtech.nease.net/Article/Images/DigitalCoding36.gif)
所谓量化，即根据不同的要求，设置不同的量化等级，从而降低数码率。 

3.7 游程长度编码
读出数据和表示数据的方式也是减少码率的一个重要因素。读出的方式可以有多种选择 ，如 水平逐行读出、垂直逐列读出、之字型读出和交替读出等，其中之字型读出(Zig－Zag) 是最常用的一种。由于经DCT变换以后，系数大多数集中在左上角，即低频分量区，因此之 字型读出实际上是按二维频率的高低顺序读出系数的，这样一来就为游程长度编码(Runleng th Encoding)创造了条件。所谓游程长度编码是指一个码可同时表示码的值和前面几个零， 这样就可以把之字型读出的优点显示出来了。因为之字型读出在大多数情况下出现连零的机 会比较多，尤其在最后，如果都是零，在读到最后一个数后只要给出“块结束”(EOB)码， 就可以结束输出，因此节省了很多码率。
 游程长度指的是由字构成的数据流中各个字符连续重复出现而形成字符串的长度。 基本的游程编码就是在数据流中直接用三个字符来给出上述三种信息，其数据结构如图4－3 所示。
![](http://hqtech.nease.net/Article/Images/DigitalCoding37.gif)
SC表示有一个字符串在此位置，X代表构成串的字符，CC代表串的长度。
游程编码和哈夫曼编码等属于统计编码。 

3.8 霍夫曼编码
霍夫曼编码是可变字长编码(VLC)的一种。 Huffman于1952年提出一种编码方法，该方法完全依据字符出现概率来构造异字头的平均长 度最短的码字，有时称之为最佳编码，一般就叫作Huffman编码。下面引证一个定理，该定 理保证了按字符出现概率分配码长，可使平均码长最短。
 定理：在变字长编码中，如果码字长度严格按照对应符号出现的概率大小逆序排列，则其平 均码字长度为最小。
 现在通过一个实例来说明上述定理的实现过程。设将信源符号按出现的概率大小顺序排列为 ：  

U： ( a1 a2 a3 a4 a5 a6 a7 ) 
0.20 0.19 0.18 0.17 0.15 0.10 0.01 

 给概率最小的两个符号a6与a7分别指定为“1”与“0”，然后将它们的概率相加再与原来的 a1~a5组合并重新排序成新的原为：

U′： ( a1 a2 a3 a4 a5 a6′ ) 
0.20 0.19 0.18 0.17 0.15 0.11 

 对a5与a′6分别指定“1”与“0”后，再作概率相加并重新按概率排序得
U″：（0.26 0.20 0.19 0.18 0.17）…
 直到最后得 U″″：（0.61 0.39）
 分别给以“0”，“1”为止，如图4－4所示。}
 霍夫曼编码的具体方法：先按出现的概率大小排队，把两个最小的概率相加，作为新的概率 和剩余的概率重新排队，再把最小的两个概率相加，再重新排队，直到最后变成1。每次相 加时都将“0”和“1”赋与相加的两个概率，读出时由该符号开始一直走到最后的“1”， 将路线上所遇到的“0”和“1”按最低位到最高位的顺序排好，就是该符号的霍夫曼编码。 

![](http://hqtech.nease.net/Article/Images/DigitalCoding38.gif)
 例如a7从左至右，由U至U″″，其码字为0000；
 a6按践线将所遇到的“0”和“1”按最低位到最高位的顺序排好，其码字为0001…
 用霍夫曼编码所得的平均比特率为：Σ码长×出现概率
 上例为： 0.2×2+0.19×2+0.18×3+0.17×3+0.15×3+0.1×4+0.01×4=2.72 bit
 可以算出本例的信源熵为2.61bit，二者已经是很接近了。
 
3.9 运动估计的运动补偿编码
这是一种帧间编码的方法，其原理是利用帧间的空间相关性，减小空间冗余度。 帧间编码为什么可以减小冗余度，这是因为两帧之间有很大的相似性。如果将前后两帧相减 (移动物体作相应位移)得到的误差作编码所需比特要比帧内编码所需的比特少，帧间差集中 在零附近，可以用短的码字传送。
 实现帧间编码的方法是运动估计和运动补偿。用图4－5来说明这个过程。
![](http://hqtech.nease.net/Article/Images/DigitalCoding39.gif)

 当前帧在过去帧的窗口中寻找匹配部分，从中找到运动矢量；
 根据运动矢量，将过去帧位移，求得对当前帧的估计；
 将这个估计和当前帧相减，求得估计的误差值；
 将运动矢量和估计的误差值送到接收端去。
 接收端根据收到的运动矢量将过去帧作位移(也就是对当前帧的估计)，再加上接收到的误差 值，就是当前帧了。
![](http://hqtech.nease.net/Article/Images/DigitalCoding391.gif)![](http://hqtech.nease.net/Article/Images/DigitalCoding392.gif)

图4－7 运动估计的全局搜索块匹配 实际上，在做运动估计和运动补偿时，是以16×16的块(称宏块)逐个进行的，如图4－6所示 ，这是将当前帧划分为N×N(16×16)的块。对每一块在过去帧中范围为![](http://hqtech.nease.net/Article/Images/DigitalCoding393.gif)的范围内进行搜索，以求得最优匹配，从而得到运动矢量的估值(dx,dy)。衡量匹配好坏 的准则可以是均方误差最小准则。搜索方法可以是全局搜索法，即对搜索范围内的每一点都 计算均方误差，选最小值即对应最优匹配，如图4－7所示。

**4.MPEG压缩编码方法简介**

MPEG用句法规定了一个层次性的结构，共分六层。这六层是图像序列(Video Sequence)-图像组(Group of Picture)-图像(Picture)-宏块条(Silce)- 宏块(Macroblock )-块(Block)。 一个图像包含亮度阵列和色度阵列。在MPEG- 1中亮度和色度的格式是4∶2∶0，而在MPEG-2 中，除这一格式外，还允许有4∶2∶2及4∶4∶4。
六个层次有其不同的功能，如下表所示：

|语法规定的层次|功 能|语法规定的层次|功 能|
|----|----|----|----|
|序列层(Sequence layer)|随机存取段落|宏块条层(Slice layer)|重新同步单元|
|图像组层(Group of picture layer)|随机存取视频单元|宏块层(Macroblock layer)|运动补偿单元|
|图像层(Picture layer)|基本编码单元|块层(Block layer)|DCT单元|
所谓4∶2∶0是指在垂直方向每隔一行去除一行色度信号。

4.1 MPEG-1和MPEG-2中三种类型图像
MPEG在利用块匹配运动补偿来减小时间冗余度时，不仅用上一帧的图像预测当前图像， 而且也使用下一帧图像预测当前图像，即双向预测，这是和H.261标准有重要区别的地方。 因此 ，MPEG-1和2中有三种类型图像，即I、B、P三种，如图4－12所示。

![](http://hqtech.nease.net/Article/Images/DigitalCoding41.gif)
I帧是帧内编码，P帧是正向预测，如图4－13所示，通过正向预测得到的图。B帧是双向预测图 ，如图4－14所示。由于B帧不能作其他图像的基准，所以对B帧编码较粗(步长较大)，需要比特数比预测图P少。

![](http://hqtech.nease.net/Article/Images/DigitalCoding42.gif)![](http://hqtech.nease.net/Article/Images/DigitalCoding43.gif)

4.2 MPEG- 1的视频压缩编码
MPEG-1中视频编码的关键压缩技术和H.261一样，仍是DCT、运动补偿和Huffman编码。编码 器和解码器的原理框图也和H.261没有区别。
 MPEG-1的信源输入格式是SIF(Source Input Format)。CCIR601格式的信源要转换成SIF后进 入MPEG- 1编码器，解码器输出也是SIF格式，显示时要经内插，扩充为CCIR601格式。PAL/NT SC都作如此处理。SIF格式如下表所示。

信号分量 行数/帧 象素数/行 
亮度(Y) 240/288 352 
色度(Cb) 120/144 176 
色度(Cr) 120/144 176 

图4－15所示为MPEG-1视频编码器。图中和H.261不同的部分是有二组运动补偿和存储器， 并有相应的转换开关。这是因为MPEG- 1有B图像要处理。开关有4种状态，‘0’是针对帧内 编码，“2”或“3”是用上一帧图像(经S2的虚线a/c)或用下一帧图像(经S2的实线 d/b) ，“4”是用了上一帧和下一帧两幅图像(经S2的实线b和实线c)，这时候要相应产生2 个运 动矢量。

![](http://hqtech.nease.net/Article/Images/DigitalCoding44.gif)

4.3 MPEG- 2视频压缩编码
 研究制定MPEG标准是为了使应用于数字存储媒体、电视广播、通信等方面的运动图像和 伴音 有一种通用的编码方法。使用这个标准意味着运动图像可以作为一种计算机数据来处理 ，并且能够存储在各种各样的存储器中，也可以在现有的或未来的网络中传送和接收，还可以在现在的或将来的广播信道中广播。
 这个标准的应用很广泛，覆盖了从电视电话到高清晰度电视。
 (1)划分为不同的“Profile”和“Level”(“类”和“级”)。由于想使这个标准适用于各种不同的应用，而各种应用在码率、分辨力、图像质量和服务方面要求又各不相同，所以希望这个标准要有通用性。在这个标准产生的过程中，考虑了各种应用的不同要求，开发了必要的算法，并且把它们集中成为一种单一的句法。正因为如此，这个标准使得各种不同应用 的码 流之间可以灵活地相互改变。
 既要使句法有通用性，又不能要求具体的解码器全面满足整个句法的需要，那样具体的解码器就太复杂了。为了解决通用性和特殊性的矛盾，设想将整个ISO/IEC的句法分成子集，这样分的子集就称为Profile，我们称之为“类”。这个“类”规定的子集还觉得太大，有必要再分得细一些。例如图像的尽寸有小的，有大的，大的可大到![](http://hqtech.nease.net/Article/Images/DigitalCoding45.gif)，实际上不会有一个解码器能够处理这么大的动态范围的图像，因此，在Profile中又规定了“level”，称为级。
 ISO/IEC13818-2(即MPEG-2视频)规定了5个类和4个级如下表所示。

|类级|Simple Profile|Main Profile|SNR Scalable Profile|SpstiallyScalable Profile|HighProfile|
|----|----|----|----|----|----|
|High Level||√|||√|
|High-1440 Level||√||√|√|
|Main Level|√|√|√||√|
|Low Level||√|√|||
注：Main Profile(主类)：图像质量合乎一定要求，允许有一定损伤，不具有可分级性；
 Simple Profile (简化类)：如Main Profile相同，只是不用B帧，这是为了节约RAM；
 SNR Scalable Profile(信噪比可选类）：比Main Profile改进之处是信噪比可分级；
 Spatially Scalable Profile（空间尺寸可选类）：空间分辨率方面也可分级；
 HIgh Profile(高质量类)：支持4∶2∶2并全面可分级(宏块的组成是：4个Y块+2个Cb块 +2个Cr块)；
 Low Level(低级)：类似H.261中的CIF格式；
 Wain Level(主级)：相应于普通电视；
 High 1440 Level(高1440级)：大致相当于具有每行1440个采样的HDTV；
 High Level(高级)：大致相当于每行1920个采样的HDTV。
 MPEG-2格式经常用级和类的缩写表示，如：MP＠ML是指主类和主级，目前用于普通数字电视 、卫星、电缆、广播的DVB标准就是用这一格式。
 (2)MPEG-2的编解码器和MPEG-1的异同。
 ①MPEG-1只处理逐行扫描的电视图像而MPEG-2既处理逐行扫描也处理隔行扫描，因此编码器中要有场/帧决策的功能；
 ②DCT有二种： 帧DCT或场DCT；
 ③运动估计有四种：场预测、帧预测、双场预测和16×8的运动补偿；
 ④压缩编码方式均采用运动检测补偿、DCT、量化、霍夫曼编码、游程编码和VLC等。 

4.4 MPEG-2系统 
![](http://hqtech.nease.net/Article/Images/DigitalCoding46.gif)
 MPEG-2系统的作用如图4－16所示。它有以下几个任务：
 (1)对音频、视频、数据、控制等基本比特流起系统复用的作用。
 (2)提供用于恢复时间基准的时间标志，缓冲器初始化和管理，音频和视频的解码时间，显示时间。
 (3)给解码器提供一种信息(PSI)，使之更容易和更迅速地找到所需节目。
 (4)给误差恢复，有条件接入，随机接入，数字存储控制提供支持。
 视频或音频编码器的输出被打成PES(Packetised Elementary Streanis)，然后在PS复用器中被组合成PS(Program Stream)或在TS复用器中被组合成TS(Transport Strea m)，前者用于相对无误差的环境，后者用于有噪声媒质。 

注：几种常见编码的代码：

1>LZ77压缩编码（C++类）
//////////////////////////////
// LZ77.h
//////////////////////////////

// 使用在自己的堆中分配索引节点，不滑动窗口
// 每次最多压缩 65536 字节数据
// 的优化版本

#ifndef _WIX_LZ77_COMPRESS_HEADER_001_
#define _WIX_LZ77_COMPRESS_HEADER_001_

// 滑动窗口的字节大小
#define _MAX_WINDOW_SIZE 65536

class CCompress
{
public:
 CCompress() {};
 virtual ~CCompress() {};

public:
 virtual int Compress(BYTE* src, int srclen, BYTE* dest) = 0;
 virtual BOOL Decompress(BYTE* src, int srclen, BYTE* dest) = 0;

protected:
 // tools 

 /////////////////////////////////////////////////////////
 // CopyBitsInAByte : 在一个字节范围内复制位流
 // 参数含义同 CopyBits 的参数
 // 说明：
 //  此函数由 CopyBits 调用，不做错误检查，即
 //  假定要复制的位都在一个字节范围内
 void CopyBitsInAByte(BYTE* memDest, int nDestPos, 
      BYTE* memSrc, int nSrcPos, int nBits);

 ////////////////////////////////////////////////////////
 // CopyBits : 复制内存中的位流
 //  memDest - 目标数据区
 //  nDestPos - 目标数据区第一个字节中的起始位
 //  memSrc - 源数据区
 //  nSrcPos - 源数据区第一个字节的中起始位
 //  nBits - 要复制的位数
 // 说明：
 //  起始位的表示约定为从字节的高位至低位（由左至右）
 //  依次为 0，1，... , 7
 //  要复制的两块数据区不能有重合
 void CopyBits(BYTE* memDest, int nDestPos, 
      BYTE* memSrc, int nSrcPos, int nBits);

 //////////////////////////////////////////////////////////////
 // 将DWORD值从高位字节到低位字节排列
 void InvertDWord(DWORD* pDW);

 /////////////////////////////////////////////////////////////
 // 设置byte的第iBit位为aBit
 //  iBit顺序为高位起从0记数（左起）
 void SetBit(BYTE* byte, int iBit, BYTE aBit);

 ////////////////////////////////////////////////////////////
 // 得到字节byte第pos位的值
 //  pos顺序为高位起从0记数（左起）
 BYTE GetBit(BYTE byte, int pos);

 ////////////////////////////////////////////////////////////
 // 将位指针*piByte(字节偏移), *piBit(字节内位偏移)后移num位
 void MovePos(int* piByte, int* piBit, int num);

 /////////////////////////////////////////////////////////
 // 取log2(n)的upper_bound
 int UpperLog2(int n);

 /////////////////////////////////////////////////////////
 // 取log2(n)的lower_bound
 int LowerLog2(int n);
};

class CCompressLZ77 : public CCompress
{
public:
 CCompressLZ77();
 virtual ~CCompressLZ77();
public:
 /////////////////////////////////////////////
 // 压缩一段字节流
 // src - 源数据区
 // srclen - 源数据区字节长度, srclen <= 65536
 // dest - 压缩数据区，调用前分配srclen字节内存
 // 返回值 > 0 压缩数据长度
 // 返回值 = 0 数据无法压缩
 // 返回值 < 0 压缩中异常错误
 int Compress(BYTE* src, int srclen, BYTE* dest);

 /////////////////////////////////////////////
 // 解压缩一段字节流
 // src - 接收原始数据的内存区, srclen <= 65536
 // srclen - 源数据区字节长度
 // dest - 压缩数据区
 // 返回值 - 成功与否
 BOOL Decompress(BYTE* src, int srclen, BYTE* dest);

protected:

 BYTE* pWnd;
 // 窗口大小最大为 64k ，并且不做滑动
 // 每次最多只压缩 64k 数据，这样可以方便从文件中间开始解压
 // 当前窗口的长度
 int nWndSize;

 // 对滑动窗口中每一个2字节串排序
 // 排序是为了进行快速术语匹配
 // 排序的方法是用一个64k大小的指针数组
 // 数组下标依次对应每一个2字节串：(00 00) (00 01) ... (01 00) (01 01) ...
 // 每一个指针指向一个链表，链表中的节点为该2字节串的每一个出现位置
 struct STIDXNODE
 {
  WORD off;  // 在src中的偏移
  WORD off2;  // 用于对应的2字节串为重复字节的节点
      // 指从 off 到 off2 都对应了该2字节串
  WORD next;  // 在SortHeap中的指针
 };

 WORD SortTable[65536];  // 256 * 256 指向SortHeap中下标的指针

 // 因为窗口不滑动，没有删除节点的操作，所以
 // 节点可以在SortHeap 中连续分配
 struct STIDXNODE* SortHeap;
 int HeapPos; // 当前分配位置

 // 当前输出位置(字节偏移及位偏移)
 int CurByte, CurBit;

protected:
 ////////////////////////////////////////
 // 输出压缩码
 // code - 要输出的数
 // bits - 要输出的位数(对isGamma=TRUE时无效)
 // isGamma - 是否输出为γ编码
 void _OutCode(BYTE* dest, DWORD code, int bits, BOOL isGamma);

 ///////////////////////////////////////////////////////////
 // 在滑动窗口中查找术语
 // nSeekStart - 从何处开始匹配
 // offset, len - 用于接收结果，表示在滑动窗口内的偏移和长度
 // 返回值- 是否查到长度为3或3以上的匹配字节串
 BOOL _SeekPhase(BYTE* src, int srclen, int nSeekStart, int* offset, int* len);

 ///////////////////////////////////////////////////////////
 // 得到已经匹配了3个字节的窗口位置offset
 // 共能匹配多少个字节
 inline int _GetSameLen(BYTE* src, int srclen, int nSeekStart, int offset);

 //////////////////////////////////////////
 // 将窗口向右滑动n个字节
 inline void _ScrollWindow(int n);

 // 向索引中添加一个2字节串
 inline void _InsertIndexItem(int off);

 // 初始化索引表，释放上次压缩用的空间
 void _InitSortTable();
};

#endif // _WIX_LZW_COMPRESS_HEADER_001_

//////////////////////////////
// LZ77.CPP
//////////////////////////////

#include <windows.h>
#include <stdio.h>
#include <memory.h>
#include <crtdbg.h>

#include "lz77.h"

/////////////////////////////////////////////////////////
// 取log2(n)的upper_bound
int CCompress::UpperLog2(int n)
{
 int i = 0;
 if (n > 0)
 {
  int m = 1;
  while(1)
  {
   if (m >= n)
    return i;
   m <<= 1;
   i++;
  }
 }
 else 
  return -1;
}
// UpperLog2
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
// 取log2(n)的lower_bound
int CCompress::LowerLog2(int n)
{
 int i = 0;
 if (n > 0)
 {
  int m = 1;
  while(1)
  {
   if (m == n)
    return i;
   if (m > n)
    return i - 1;
   m <<= 1;
   i++;
  }
 }
 else 
  return -1;
}
// LowerLog2
/////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// 将位指针*piByte(字节偏移), *piBit(字节内位偏移)后移num位
void CCompress::MovePos(int* piByte, int* piBit, int num)
{
 num += (*piBit);
 (*piByte) += num / 8;
 (*piBit) = num % 8;
}
// MovePos
////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////
// 得到字节byte第pos位的值
//  pos顺序为高位起从0记数（左起）
BYTE CCompress::GetBit(BYTE byte, int pos)
{
 int j = 1;
 j <<= 7 - pos;
 if (byte & j)
  return 1;
 else 
  return 0;
}
// GetBit
/////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
// 设置byte的第iBit位为aBit
//  iBit顺序为高位起从0记数（左起）
void CCompress::SetBit(BYTE* byte, int iBit, BYTE aBit)
{
 if (aBit)
  (*byte) |= (1 << (7 - iBit));
 else
  (*byte) &= ~(1 << (7 - iBit));
}
// SetBit
//////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////
// 将DWORD值从高位字节到低位字节排列
void CCompress::InvertDWord(DWORD* pDW)
{
 union UDWORD{ DWORD dw; BYTE b[4]; };
 UDWORD* pUDW = (UDWORD*)pDW;
 BYTE b;
 b = pUDW->b[0]; pUDW->b[0] = pUDW->b[3]; pUDW->b[3] = b;
 b = pUDW->b[1]; pUDW->b[1] = pUDW->b[2]; pUDW->b[2] = b;
}
// InvertDWord
//////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////
// CopyBits : 复制内存中的位流
//  memDest - 目标数据区
//  nDestPos - 目标数据区第一个字节中的起始位
//  memSrc - 源数据区
//  nSrcPos - 源数据区第一个字节的中起始位
//  nBits - 要复制的位数
// 说明：
//  起始位的表示约定为从字节的高位至低位（由左至右）
//  依次为 0，1，... , 7
//  要复制的两块数据区不能有重合
void CCompress::CopyBits(BYTE* memDest, int nDestPos, 
     BYTE* memSrc, int nSrcPos, int nBits)
{
 int iByteDest = 0, iBitDest;
 int iByteSrc = 0, iBitSrc = nSrcPos;

 int nBitsToFill, nBitsCanFill;

 while (nBits > 0)
 {
  // 计算要在目标区当前字节填充的位数
  nBitsToFill = min(nBits, iByteDest ? 8 : 8 - nDestPos);
  // 目标区当前字节要填充的起始位
  iBitDest = iByteDest ? 0 : nDestPos;
  // 计算可以一次从源数据区中复制的位数
  nBitsCanFill = min(nBitsToFill, 8 - iBitSrc);
  // 字节内复制
  CopyBitsInAByte(memDest + iByteDest, iBitDest, 
   memSrc + iByteSrc, iBitSrc, nBitsCanFill);  
  // 如果还没有复制完 nBitsToFill 个
  if (nBitsToFill > nBitsCanFill)
  {
   iByteSrc++; iBitSrc = 0; iBitDest += nBitsCanFill;
   CopyBitsInAByte(memDest + iByteDest, iBitDest, 
     memSrc + iByteSrc, iBitSrc, 
     nBitsToFill - nBitsCanFill);
   iBitSrc += nBitsToFill - nBitsCanFill;
  }
  else 
  {
   iBitSrc += nBitsCanFill;
   if (iBitSrc >= 8)
   {
    iByteSrc++; iBitSrc = 0;
   }
  }

  nBits -= nBitsToFill; // 已经填充了nBitsToFill位
  iByteDest++;
 } 
}
// CopyBits
/////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
// CopyBitsInAByte : 在一个字节范围内复制位流
// 参数含义同 CopyBits 的参数
// 说明：
//  此函数由 CopyBits 调用，不做错误检查，即
//  假定要复制的位都在一个字节范围内
void CCompress::CopyBitsInAByte(BYTE* memDest, int nDestPos, 
     BYTE* memSrc, int nSrcPos, int nBits)
{
 BYTE b1, b2;
 b1 = *memSrc;
 b1 <<= nSrcPos; b1 >>= 8 - nBits; // 将不用复制的位清0
 b1 <<= 8 - nBits - nDestPos;  // 将源和目的字节对齐
 *memDest |= b1;  // 复制值为1的位
 b2 = 0xff; b2 <<= 8 - nDestPos;  // 将不用复制的位置1
 b1 |= b2;
 b2 = 0xff; b2 >>= nDestPos + nBits;
 b1 |= b2;
 *memDest &= b1;  // 复制值为0的位
}
// CopyBitsInAByte
/////////////////////////////////////////////////////////

//------------------------------------------------------------------

CCompressLZ77::CCompressLZ77()
{ 
 SortHeap = new struct STIDXNODE[_MAX_WINDOW_SIZE];
}

CCompressLZ77::~CCompressLZ77()
{
 delete[] SortHeap;
}

// 初始化索引表，释放上次压缩用的空间
void CCompressLZ77::_InitSortTable()
{
 memset(SortTable, 0, sizeof(WORD) * 65536);
 nWndSize = 0;
 HeapPos = 1;
}

// 向索引中添加一个2字节串
void CCompressLZ77::_InsertIndexItem(int off)
{
 WORD q;
 BYTE ch1, ch2;
 ch1 = pWnd[off]; ch2 = pWnd[off + 1]; 

 if (ch1 != ch2)
 {
  // 新建节点
  q = HeapPos;
  HeapPos++;
  SortHeap[q].off = off;
  SortHeap[q].next = SortTable[ch1 * 256 + ch2];
  SortTable[ch1 * 256 + ch2] = q;
 }
 else
 {
  // 对重复2字节串
  // 因为没有虚拟偏移也没有删除操作，只要比较第一个节点
  // 是否和 off 相连接即可
  q = SortTable[ch1 * 256 + ch2];
  if (q != 0 && off == SortHeap[q].off2 + 1)
  {  
   // 节点合并
   SortHeap[q].off2 = off;
  }  
  else
  {
   // 新建节点
   q = HeapPos;
   HeapPos++;
   SortHeap[q].off = off;
   SortHeap[q].off2 = off;
   SortHeap[q].next = SortTable[ch1 * 256 + ch2];
   SortTable[ch1 * 256 + ch2] = q;
  }
 }
}

//////////////////////////////////////////
// 将窗口向右滑动n个字节
void CCompressLZ77::_ScrollWindow(int n)
{ 
 for (int i = 0; i < n; i++)
 {  
  nWndSize++;  
  if (nWndSize > 1)   
   _InsertIndexItem(nWndSize - 2);
 }
}

///////////////////////////////////////////////////////////
// 得到已经匹配了2个字节的窗口位置offset
// 共能匹配多少个字节
int CCompressLZ77::_GetSameLen(BYTE* src, int srclen, int nSeekStart, int offset)
{
 int i = 2; // 已经匹配了2个字节
 int maxsame = min(srclen - nSeekStart, nWndSize - offset);
 while (i < maxsame
   && src[nSeekStart + i] == pWnd[offset + i])
  i++;
 _ASSERT(nSeekStart + i <= srclen && offset + i <= nWndSize);
 return i;
}

///////////////////////////////////////////////////////////
// 在滑动窗口中查找术语
// nSeekStart - 从何处开始匹配
// offset, len - 用于接收结果，表示在滑动窗口内的偏移和长度
// 返回值- 是否查到长度为2或2以上的匹配字节串
BOOL CCompressLZ77::_SeekPhase(BYTE* src, int srclen, int nSeekStart, int* offset, int* len)
{ 
 int j, m, n;

 if (nSeekStart < srclen - 1)
 {
  BYTE ch1, ch2;
  ch1 = src[nSeekStart]; ch2 = src[nSeekStart + 1];
  WORD p;
  p = SortTable[ch1 * 256 + ch2];
  if (p != 0)
  {
   m = 2; n = SortHeap[p].off;
   while (p != 0)
   {
    j = _GetSameLen(src, srclen, 
     nSeekStart, SortHeap[p].off);
    if ( j > m )
    { 
     m = j; 
     n = SortHeap[p].off;
    }   
    p = SortHeap[p].next;
   } 
   (*offset) = n; 
   (*len) = m;
   return TRUE;  
  } 
 }
 return FALSE;
}

////////////////////////////////////////
// 输出压缩码
// code - 要输出的数
// bits - 要输出的位数(对isGamma=TRUE时无效)
// isGamma - 是否输出为γ编码
void CCompressLZ77::_OutCode(BYTE* dest, DWORD code, int bits, BOOL isGamma)
{ 
 if ( isGamma )
 {
  BYTE* pb;
  DWORD out;
  // 计算输出位数
  int GammaCode = (int)code - 1;
  int q = LowerLog2(GammaCode);
  if (q > 0)
  {
   out = 0xffff;
   pb = (BYTE*)&out;
   // 输出q个1
   CopyBits(dest + CurByte, CurBit, 
    pb, 0, q);
   MovePos(&CurByte, &CurBit, q);
  }
  // 输出一个0
  out = 0;
  pb = (BYTE*)&out;  
  CopyBits(dest + CurByte, CurBit, pb + 3, 7, 1);
  MovePos(&CurByte, &CurBit, 1);
  if (q > 0)
  {
   // 输出余数, q位
   int sh = 1;
   sh <<= q;
   out = GammaCode - sh;
   pb = (BYTE*)&out;
   InvertDWord(&out);
   CopyBits(dest + CurByte, CurBit, 
    pb + (32 - q) / 8, (32 - q) % 8, q);
   MovePos(&CurByte, &CurBit, q);
  }
 }
 else 
 {
  DWORD dw = (DWORD)code;
  BYTE* pb = (BYTE*)&dw;
  InvertDWord(&dw);
  CopyBits(dest + CurByte, CurBit, 
    pb + (32 - bits) / 8, (32 - bits) % 8, bits);
  MovePos(&CurByte, &CurBit, bits);
 }
}

/////////////////////////////////////////////
// 压缩一段字节流
// src - 源数据区
// srclen - 源数据区字节长度
// dest - 压缩数据区，调用前分配srclen+5字节内存
// 返回值 > 0 压缩数据长度
// 返回值 = 0 数据无法压缩
// 返回值 < 0 压缩中异常错误
int CCompressLZ77::Compress(BYTE* src, int srclen, BYTE* dest)
{
 int i;
 CurByte = 0; CurBit = 0; 
 int off, len;

 if (srclen > 65536) 
  return -1;

 pWnd = src;
 _InitSortTable();
 for (i = 0; i < srclen; i++)
 {  
  if (CurByte >= srclen)
   return 0;
  if (_SeekPhase(src, srclen, i, &off, &len))
  {   
   // 输出匹配术语 flag(1bit) + len(γ编码) + offset(最大16bit)
   _OutCode(dest, 1, 1, FALSE);
   _OutCode(dest, len, 0, TRUE);

   // 在窗口不满64k大小时，不需要16位存储偏移
   _OutCode(dest, off, UpperLog2(nWndSize), FALSE);

   _ScrollWindow(len);
   i += len - 1;
  }
  else
  {
   // 输出单个非匹配字符 0(1bit) + char(8bit)
   _OutCode(dest, 0, 1, FALSE);
   _OutCode(dest, (DWORD)(src[i]), 8, FALSE);
   _ScrollWindow(1);
  }
 }
 int destlen = CurByte + ((CurBit) ? 1 : 0);
 if (destlen >= srclen)
  return 0;
 return destlen;
}

/////////////////////////////////////////////
// 解压缩一段字节流
// src - 接收原始数据的内存区
// srclen - 源数据区字节长度
// dest - 压缩数据区
// 返回值 - 成功与否
BOOL CCompressLZ77::Decompress(BYTE* src, int srclen, BYTE* dest)
{
 int i;
 CurByte = 0; CurBit = 0;
 pWnd = src;  // 初始化窗口
 nWndSize = 0;

 if (srclen > 65536) 
  return FALSE;

 for (i = 0; i < srclen; i++)
 {  
  BYTE b = GetBit(dest[CurByte], CurBit);
  MovePos(&CurByte, &CurBit, 1);
  if (b == 0) // 单个字符
  {
   CopyBits(src + i, 0, dest + CurByte, CurBit, 8);
   MovePos(&CurByte, &CurBit, 8);
   nWndSize++;
  }
  else  // 窗口内的术语
  {
   int q = -1;
   while (b != 0)
   {
    q++;
    b = GetBit(dest[CurByte], CurBit);
    MovePos(&CurByte, &CurBit, 1);    
   }
   int len, off;
   DWORD dw = 0;
   BYTE* pb;
   if (q > 0)
   {    
    pb = (BYTE*)&dw;
    CopyBits(pb + (32 - q) / 8, (32 - q) % 8, dest + CurByte, CurBit, q);
    MovePos(&CurByte, &CurBit, q);
    InvertDWord(&dw);
    len = 1;
    len <<= q;
    len += dw;
    len += 1;
   }
   else
    len = 2;

   // 在窗口不满64k大小时，不需要16位存储偏移
   dw = 0;
   pb = (BYTE*)&dw;
   int bits = UpperLog2(nWndSize);
   CopyBits(pb + (32 - bits) / 8, (32 - bits) % 8, dest + CurByte, CurBit, bits);
   MovePos(&CurByte, &CurBit, bits);
   InvertDWord(&dw);
   off = (int)dw;
   // 输出术语
   for (int j = 0; j < len; j++)
   {
    _ASSERT(i + j <  srclen);
    _ASSERT(off + j <  _MAX_WINDOW_SIZE);

    src[i + j] = pWnd[off + j];
   }
   nWndSize += len;
   i += len - 1;
  }
  // 滑动窗口
  if (nWndSize > _MAX_WINDOW_SIZE)
  {
   pWnd += nWndSize - _MAX_WINDOW_SIZE;
   nWndSize = _MAX_WINDOW_SIZE;   
  }
 }

 return TRUE;
}

2>lzw
//////////////////////////////////////////////////////////////////
//  用途 : LZW 压缩算法                //
//  创建 : [Foolish] / 2001-9-17       //
//////////////////////////////////////////////////////////////////
#ifndef  __FOO_LZW_COMPRESS_H__
#define  __FOO_LZW_COMPRESS_H__
#include <windows.h>
#pragma once

//===================================================================
// 定义
//===================================================================
// 解码程序用到String Table的最大长度
#define  LZW_MAX_TABLE_SIZE  4096

// 编码程序只用到Hash表, 不需要String Table, 因为它不需要知道String Table中的内容, 
// 只需要知道Old + Pixel字串是否在表中和表中的index
// Hash表设计为  :  (Old << 8) | Pixel 其中存放的是String Table的Index
#define  LZW_MAX_HASH_SIZE  0x1000FF // (4096 << 8) + 0xFF

// 解码程序用到String Table - string结构
// 每个string可以形成一棵二叉树, 此二叉树仅有一个右节点
// 因为wPrefix总是指向String Table中的另一位置, 而wSuffix指向0 ~ (clear-1)
typedef struct tagLZW_STRING
{
 WORD wPrefix ; // 为 Old
 WORD wSuffix ; // 为 GetFirstChar (Old) 或 GetFirstChar (Code)
} LZW_STRING, * PLZW_STRING ;

//===================================================================
// LZW - 压缩算法
//===================================================================
class FCLzw
{
public :
  FCLzw () ;
 ~FCLzw () ;

 /********************************************************************/
 /* 功  能 ：通用LZW编码           */
 /* 说  明 ：OutBuffer必须预先置0         */
 /*    输出的OutBuffer按 8 + ...data... 格式存储    */
 /********************************************************************/
 DWORD LZW_Encode (BYTE * InBuffer, DWORD dwLength,
      BYTE * OutBuffer) ;

 /********************************************************************/
 /* 功  能 ：通用LZW解码           */
 /* 说  明 ：InBuffer必须按 8 + ...data... 格式存储     */
 /********************************************************************/
 void LZW_Decode (BYTE * InBuffer, BYTE * OutBuffer) ;

 /********************************************************************/
 /* 功  能 ：编码DIB为GIF           */
 /* 参  数 ：把DIB左上角的指针传给DibBuffer       */
 /* 返回值 ：写入OutBuffer中的字节数        */
 /* 说  明 ：OutBuffer必须预先置0         */
 /*    暂时只支持4, 8位色位图编码，不支持1位色位图   */
 /********************************************************************/
 DWORD LZW_GIF_Encode (BYTE * DibBuffer, BYTE * OutBuffer,
       DWORD dwDibWidth, DWORD dwDibHeight,
       WORD wColorBit) ;

 /********************************************************************/
 /* 功  能 ：解码GIF到DIB           */
 /* 参  数 ：把DIB左上角的指针传给DibBuffer       */
 /* 说  明 ：所有位色GIF一律解码为8位色DIB, DibBuffer为8位色  */
 /*    InBuffer必须按 Min_Code_Length + ...data... 格式存储 */
 /*    所以GIF数据(InBuffer)先要解包      */
 /********************************************************************/
 void LZW_GIF_Decode (BYTE * InBuffer, BYTE * DibBuffer,
       DWORD dwDibWidth, DWORD dwDibHeight,
       bool bInterlace) ;

private :
 LZW_STRING * m_pStrBegin ; // String Table
 WORD  * m_pHash ;  // Hash Table (16bit存放m_pStrBegin的Index)
 WORD  m_LZW_CLEAR, m_LZW_END ; // clear和end标志

 // 交错存储用---------------------------------------------------+
 BYTE  m_byInterval ; // 用在交错存储的gif中, 作为m_Interval的索引, 0xFF为非交错存储
 BYTE  m_Interval[7] ; // 交错排列 : 8 8 4 2
 // 交错存储用---------------------------------------------------+

 BYTE  * m_pOrigin ;
 BYTE  * m_pCurIn ;
 BYTE  * m_pCurOut ;
 BYTE  m_byInBit ;  // 输入缓冲Bit位置
 BYTE  m_byOutBit ; // 输出缓冲Bit位置

 BYTE  m_byMinCode ;  // 最小码长度 (即原始颜色位数)
 BYTE  m_byCurrBits ;  // 现阶段码长
 WORD  m_CurrTableIndex ; // 当前的String Table Index 
 BYTE  m_Padding[2] ;

 DWORD  m_dwDibHeight ;  // DIB的高度
 DWORD  m_dwDibWidth ;   // DIB的宽度
 DWORD  m_dwCurrPixel ;  // 当前行写入的像素个数
 DWORD  m_dwCurrHeight ; // 当前的高
 DWORD  m_dwPitch ;   // DIB每行的字节数

private :
 // 解码
 void Decode_InitStringTable () ;
 WORD Decode_GetNextCode () ; // 最长12位
 bool Decode_IsInTable (WORD Code) ;
 void Decode_AddStringToTable (WORD wPrefix, WORD wSuffix) ;
 BYTE Decode_GetFirstChar (WORD Code) ;
 void Decode_WriteString_to8 (WORD Code) ; // 解码到目标8位色位图
 void Decode_SwitchToFollowLine () ; // 使m_pCurOut切换到下一行
 // 编码
 void Encode_InitStringTable () ;
 BYTE Encode_GetNextPixel () ;
 bool Encode_IsInTable (WORD Old, WORD Pixel) ; // Old和Pixel都指向在m_pStrBegin中的index。
 void Encode_AddStringToTable (WORD Old, WORD Pixel) ;
 void Encode_WriteIndex (DWORD wIndex) ; // 压缩时写String Index
} ;

//===================================================================
// Implement
//===================================================================
inline  FCLzw::FCLzw () {
 m_Interval[0] = m_Interval[1] = 8 ;
 m_Interval[2] = 4 ; m_Interval[3] = 2 ; // 交错行数
}
inline  FCLzw::~FCLzw () {
}
inline void  FCLzw::Decode_InitStringTable () {
 memset (m_pStrBegin, 0xFF, LZW_MAX_TABLE_SIZE * sizeof(LZW_STRING)) ;
 for (WORD i = 0 ; i < m_LZW_CLEAR ; i++) // 初始化String Table
  m_pStrBegin[i].wSuffix = i ;
 m_CurrTableIndex = m_LZW_END + 1 ;
 m_byCurrBits = m_byMinCode + 1 ;
}
inline void  FCLzw::Decode_SwitchToFollowLine () {
 m_dwCurrPixel = 0 ; // 到行开头
 if (m_byInterval == 0xFF) // 非交错存储, 直接到下一行
  m_pCurOut -= m_dwPitch ;
 else
 {
  m_dwCurrHeight += m_Interval[m_byInterval] ; // 交错存储的gif, 计算下一行
  if (m_dwCurrHeight >= m_dwDibHeight)
   if (++m_byInterval < 4)
    m_dwCurrHeight = m_Interval[m_byInterval]/2 ;
   else
    return ;
  m_pCurOut = m_pOrigin - m_dwCurrHeight * m_dwPitch ;
 }
}
inline WORD  FCLzw::Decode_GetNextCode () {
 // 与Encode_WriteIndex ()是相对应的, 最长为12位(最多跨越2-BYTE)
 register DWORD   dwRet = 0 ;
 register unsigned int uiAdd = m_byInBit + m_byCurrBits ;

 if (uiAdd <= 8) // 在当前BYTE内
  dwRet |= * m_pCurIn ;
 else
  if (uiAdd <= 16) // 跨1-BYTE
   dwRet |= * (WORD *) m_pCurIn ;
  else // 跨2-BYTE
  {
   dwRet |= * (m_pCurIn + 2) ;
   dwRet <<= 16 ;
   dwRet |= * (WORD *) m_pCurIn ;
  }
 m_pCurIn += uiAdd / 8 ;
 m_byInBit = uiAdd % 8 ;
 dwRet <<= 32 - uiAdd ;
 dwRet >>= 32 - m_byCurrBits ; // 左右清零
 return (WORD)dwRet ;
}
inline bool  FCLzw::Decode_IsInTable (WORD Code) {
 return (Code < m_CurrTableIndex) ;
}
inline BYTE  FCLzw::Decode_GetFirstChar (WORD Code) {
 while (m_pStrBegin[Code].wPrefix != 0xFFFF)
  Code = m_pStrBegin[Code].wPrefix ;
 return (BYTE) m_pStrBegin[Code].wSuffix ;
}
inline void  FCLzw::Decode_AddStringToTable (WORD wPrefix, WORD wSuffix) {
 m_pStrBegin[m_CurrTableIndex  ].wPrefix = wPrefix ;
 m_pStrBegin[m_CurrTableIndex++].wSuffix = wSuffix ;
 if ((m_CurrTableIndex == 0x008) || (m_CurrTableIndex == 0x010) ||
  (m_CurrTableIndex == 0x020) || (m_CurrTableIndex == 0x040) ||
  (m_CurrTableIndex == 0x080) || (m_CurrTableIndex == 0x100) ||
  (m_CurrTableIndex == 0x200) || (m_CurrTableIndex == 0x400) ||
  (m_CurrTableIndex == 0x800))
  m_byCurrBits++ ;
}
inline void  FCLzw::LZW_Decode (BYTE * InBuffer, BYTE * OutBuffer) {
 // 给dwDibWidth传入0则不换行
 this->LZW_GIF_Decode (InBuffer, OutBuffer, 0, 0, false) ;
}
//===================================================================
// Encode
//===================================================================
inline void  FCLzw::Encode_InitStringTable () {
 m_CurrTableIndex = m_LZW_END + 1 ;
 m_byCurrBits = m_byMinCode + 1 ;
 memset (m_pHash, 0x00, LZW_MAX_HASH_SIZE * sizeof(WORD)) ; // Hash Table置位0
}
inline BYTE  FCLzw::Encode_GetNextPixel () {
 // 目前只支持1, 4, 8位色, 所以不会跨BYTE
 register BYTE  byRet ;
 switch (m_byMinCode)
 {
  case 8 : byRet = m_pCurIn[m_dwCurrPixel] ; break ;
  case 4 : byRet = (m_dwCurrPixel % 2 == 0)
       ? m_pCurIn[m_dwCurrPixel / 2] >> 4
       : m_pCurIn[m_dwCurrPixel / 2] & 0x0F ; break ;
  case 1 : byRet = 0x01 & (m_pCurIn[m_dwCurrPixel / 8] >> (7 - (m_dwCurrPixel & 7))) ; break ;
 }
 if (++m_dwCurrPixel == m_dwDibWidth) // 行结束
 {
  m_dwCurrPixel = 0 ;
  m_dwCurrHeight++ ; // DIB下一行
  m_pCurIn = m_pOrigin - m_dwCurrHeight * m_dwPitch ;
 }
 return byRet ;
}
inline bool  FCLzw::Encode_IsInTable (WORD Old, WORD Pixel) {
 return (m_pHash[(Old << 8) | Pixel] != 0) ;
}
inline void  FCLzw::Encode_AddStringToTable (WORD Old, WORD Pixel) {
 m_pHash[(Old << 8) | Pixel] = m_CurrTableIndex++ ;
 if ((m_CurrTableIndex == 0x009) || (m_CurrTableIndex == 0x011) ||
  (m_CurrTableIndex == 0x021) || (m_CurrTableIndex == 0x041) ||
  (m_CurrTableIndex == 0x081) || (m_CurrTableIndex == 0x101) ||
  (m_CurrTableIndex == 0x201) || (m_CurrTableIndex == 0x401) ||
  (m_CurrTableIndex == 0x801))
  m_byCurrBits++ ;
}
inline void  FCLzw::Encode_WriteIndex (DWORD Index) {
 // 压缩时写String Index, 最长为12位(最多跨越2-BYTE)
 // 与Decode_GetNextCode ()是相对应的
 register unsigned int uiAdd = m_byOutBit + m_byCurrBits ;

 // 与解码不一样, 压缩时会预留一些内存, 可以用DWORD
 * (DWORD *) m_pCurOut |= (Index << m_byOutBit) ;
 m_byOutBit = uiAdd % 8 ;
 m_pCurOut += uiAdd / 8 ;
}
inline DWORD  FCLzw::LZW_Encode (BYTE * InBuffer, DWORD dwLength, BYTE * OutBuffer) {
 // 给dwDibWidth传入0则不换行
 *OutBuffer++ = 8 ;
 return 1 + this->LZW_GIF_Encode (InBuffer, OutBuffer, dwLength, 1, 8) ;
}

#endif

#include "stdafx.h"
#include "Lzw.h"

//===================================================================
// 目标为8位色
void  FCLzw::Decode_WriteString_to8 (WORD Code)
{
 if (Code < m_LZW_CLEAR)
 {
  m_pCurOut[m_dwCurrPixel++] = (BYTE) m_pStrBegin[Code].wSuffix ;
  if (m_dwCurrPixel == m_dwDibWidth) // 写完一行, 切换值至下一行
   this->Decode_SwitchToFollowLine () ;
 }
 else
 {
  this->Decode_WriteString_to8 (m_pStrBegin[Code].wPrefix) ;
  this->Decode_WriteString_to8 (m_pStrBegin[Code].wSuffix) ;
 }
}
//===================================================================
void  FCLzw::LZW_GIF_Decode (BYTE * InBuffer, BYTE * DibBuffer, DWORD dwDibWidth, DWORD dwDibHeight, bool bInterlace)
{
 m_pStrBegin = new LZW_STRING [LZW_MAX_TABLE_SIZE + 32] ;
 if ((m_pStrBegin == NULL) || (InBuffer == NULL) || (DibBuffer == NULL))
   return ;
 // 初始化设置-----------------------------------------+
 m_pCurIn = InBuffer + 1 ;
 m_pCurOut = DibBuffer ;
 m_byInBit = 0 ; // 输入从0开始, 输出为8位色位图
 m_byMinCode = *InBuffer ; // 获得最小码长度
 m_LZW_CLEAR = 1 << m_byMinCode ; // clear
 m_LZW_END = m_LZW_CLEAR + 1 ; // end

 m_dwDibWidth = dwDibWidth ; // DIB宽
 m_dwCurrPixel = 0 ;
 m_dwPitch = 4 * ((dwDibWidth * 8 + 31) / 32) ;

 m_byInterval = bInterlace ? 0 : 0xFF ; // 设置交错索引
 if (m_byInterval == 0) // 交错存储
 {
  m_pOrigin = m_pCurOut ;
  m_dwDibHeight = dwDibHeight ; // DIB高
  m_dwCurrHeight = 0 ; // 从最顶行开始
 }
 // 初始化设置完成-------------------------------------+

 WORD  Code, Old ;
 this->Decode_InitStringTable () ;
 while ((Code = this->Decode_GetNextCode ()) != m_LZW_END)
 {
  if (Code == m_LZW_CLEAR)
  {
   this->Decode_InitStringTable () ;
   while ((Code = this->Decode_GetNextCode ()) == m_LZW_CLEAR) ;
  }
  else
   if (this->Decode_IsInTable (Code))
    this->Decode_AddStringToTable (Old, this->Decode_GetFirstChar (Code)) ;
   else
    this->Decode_AddStringToTable (Old, this->Decode_GetFirstChar (Old)) ;

  this->Decode_WriteString_to8 (Code) ;
  Old = Code ;
 }
 delete[] m_pStrBegin ;
}
//===================================================================
DWORD  FCLzw::LZW_GIF_Encode (BYTE * DibBuffer, BYTE * OutBuffer,
         DWORD dwDibWidth, DWORD dwDibHeight,
         WORD wColorBit)
{
 // 编码阶段不需要String Table，只需要m_CurrTableIndex来获知写位数
 m_pHash = new WORD [LZW_MAX_HASH_SIZE] ;
 if ((m_pHash == NULL) || (DibBuffer == NULL) || (OutBuffer == NULL))
  return 0 ;
 // 初始化设置-----------------------------------------+
 m_byMinCode = (wColorBit == 1) ? 2 : wColorBit ; // 1, 4, 8位色(最小码长度)，1位色为2
 m_LZW_CLEAR = 1 << m_byMinCode ;
 m_LZW_END = m_LZW_CLEAR + 1 ;
 m_pOrigin = m_pCurIn = DibBuffer ;
 m_pCurOut = OutBuffer ;
 m_byInBit = 8 ;
 m_byOutBit = 0 ; // 输出从0开始

 m_dwDibWidth = dwDibWidth ;  // DIB宽
 m_dwDibHeight = dwDibHeight ; // DIB高
 m_dwCurrPixel = 0 ;
 m_dwCurrHeight = 0 ; // 从最顶行开始
 m_dwPitch = 4 * ((dwDibWidth * m_byMinCode + 31) / 32) ;
 // 初始化设置完成-------------------------------------+

 WORD  Old ; // 保留字串
 BYTE  Pixel ; // 当前读入字符
 this->Encode_InitStringTable () ;
 this->Encode_WriteIndex (m_LZW_CLEAR) ; // 首先写clear
 Old = this->Encode_GetNextPixel () ;
 // 编码
 while (m_dwCurrHeight < m_dwDibHeight)
 {
  Pixel = this->Encode_GetNextPixel () ;
  if (this->Encode_IsInTable (Old, Pixel))
   Old = m_pHash[(Old << 8) | Pixel] ; // 已在表中, 取出索引, Hash Table中存放的是String Table的Index
  else
  {
   // 不在表中, 把Old + Pixel添加到String Table中
   this->Encode_WriteIndex (Old) ;
   this->Encode_AddStringToTable (Old, Pixel) ;
   Old = Pixel ;
   if (m_CurrTableIndex == LZW_MAX_TABLE_SIZE) // 表填满
   {
    this->Encode_WriteIndex (Pixel) ;
    this->Encode_WriteIndex (m_LZW_CLEAR) ;
    this->Encode_InitStringTable () ;
    Old = this->Encode_GetNextPixel () ;
   }
  }
 }
 this->Encode_WriteIndex (Old) ;
 this->Encode_WriteIndex (m_LZW_END) ;
 delete[] m_pHash ;
 return (m_pCurOut - OutBuffer + 1) ;
}
//===================================================================

3>jpeg编码

#include <windows.h>
#define M_SOF0  0xc0
#define M_DHT   0xc4
#define M_EOI   0xd9
#define M_SOS   0xda
#define M_DQT   0xdb
#define M_DRI   0xdd
#define M_APP0  0xe0

static int Zig_Zag[8][8]={{0,1,5,6,14,15,27,28},
        {2,4,7,13,16,26,29,42},
        {3,8,12,17,25,30,41,43},
        {9,11,18,24,37,40,44,53},
        {10,19,23,32,39,45,52,54},
        {20,22,33,38,46,51,55,60},
        {21,34,37,47,50,56,59,61},
        {35,36,48,49,57,58,62,63}
       };

#define W1 2841 /* 2048*sqrt(2)*cos(1*pi/16) */
#define W2 2676 /* 2048*sqrt(2)*cos(2*pi/16) */
#define W3 2408 /* 2048*sqrt(2)*cos(3*pi/16) */
#define W5 1609 /* 2048*sqrt(2)*cos(5*pi/16) */
#define W6 1108 /* 2048*sqrt(2)*cos(6*pi/16) */
#define W7 565  /* 2048*sqrt(2)*cos(7*pi/16) */

//////////////////////////////////////////////////////////////////////////////

#define WIDTHBYTES(i)    ((i+31)/32*4)

#define FUNC_OK 0

#define FUNC_MEMORY_ERROR 1

#define FUNC_FILE_ERROR 2

#define FUNC_FORMAT_ERROR 3

typedef short FUNCRET;

////////////////////////////////////////////////////////////////////////////

void    DecodeJPG(HWND hWnd,char *fn,HGLOBAL *hbi);

void  InitTable();

void  showerror(FUNCRET funcret);

FUNCRET InitTag();

FUNCRET Decode();

FUNCRET DecodeMCUBlock();

FUNCRET HufBlock(BYTE dchufindex,BYTE achufindex);

FUNCRET DecodeElement();

void  IQtIZzMCUComponent(short flag);

void IQtIZzBlock(short  *s ,int * d,short flag);

void   GetYUV(short flag);

void    StoreBuffer();

BYTE    ReadByte();

void  DispABand();

void  GetJPGSize(char *fn);

void  Initialize_Fast_IDCT();

void  Fast_IDCT(int * block);

static void idctrow(int * blk);

static void idctcol(int * blk);

////////////////////////////////////////////////////////////////////////////

extern  int imgheight,imgwidth;

static  long iclip[1024];

static  long *iclp;

////////////////////////////////////////////////////////////////////////////

HFILE   hfjpg;

unsigned char  *lpbi;

DWORD   bibufsize;

DWORD   linesize;

HGLOBAL hjpgbuf;

unsigned char  *lpjpgbuf;

unsigned char  *lp;

DWORD   jpgbufsize;

short     SampRate_Y_H,SampRate_Y_V;

short     SampRate_U_H,SampRate_U_V;

short     SampRate_V_H,SampRate_V_V;

short     H_YtoU,V_YtoU,H_YtoV,V_YtoV;

short     Y_in_MCU,U_in_MCU,V_in_MCU;

short     sizei,sizej;

short  restart;

short     qt_table[3][64];

short     comp_num;

BYTE    comp_index[3];

BYTE    YDcIndex,YAcIndex,UVDcIndex,UVAcIndex;

BYTE    HufTabIndex;

short     *YQtTable,*UQtTable,*VQtTable;

BYTE    And[9]={0,1,3,7,0xf,0x1f,0x3f,0x7f,0xff};

short     code_pos_table[4][16],code_len_table[4][16];

unsigned short code_value_table[4][256];

unsigned short huf_max_value[4][16],huf_min_value[4][16];

short     BitPos,CurByte;

short     rrun,vvalue;

short    MCUBuffer[10*64];

int   QtZzMCUBuffer[10*64];

short    BlockBuffer[64];

short    ycoef,ucoef,vcoef;

BOOL    IntervalFlag;

 short  interval=0;

int    Y[4*64],U[4*64],V[4*64];

HGLOBAL  hbiband;

unsigned char  *lpbibandheader;

unsigned char  *lpbibandbits;

DWORD    bibandsize;

HWND     hwnd;

////////////////////////////////////////////////////////////////////////////

void DecodeJPG(HWND hWnd,char *fn,HGLOBAL *hbi)

{

  FUNCRET  funcret;

  BITMAPINFOHEADER bi;

  char *pbi;

  short  i,j;

  hwnd=hWnd;

  if((hfjpg=_lopen(fn,OF_READ))==HFILE_ERROR)

  {

  showerror(FUNC_FILE_ERROR);

  *hbi=NULL;

  return ;

  }

  //get jpg file length

  jpgbufsize=_llseek(hfjpg,0L,SEEK_END);

  //rewind to the beginning of the file

  _llseek(hfjpg,0L,SEEK_SET);

 if((hjpgbuf=GlobalAlloc(GHND,jpgbufsize))==NULL)

  {

  _lclose(hfjpg);

  showerror(FUNC_MEMORY_ERROR);

  *hbi=NULL;

  return ;

   }

 lpjpgbuf=(unsigned char  *)GlobalLock(hjpgbuf);

 _hread(hfjpg,(unsigned char  *)lpjpgbuf,jpgbufsize);

 _lclose(hfjpg);

  InitTable();

  if((funcret=InitTag())!=FUNC_OK)

  {

  GlobalUnlock(hjpgbuf);

  GlobalFree(hjpgbuf);

  showerror(funcret);

  *hbi=NULL;

  return ;

  }

  bi.biSize=(DWORD)sizeof(BITMAPINFOHEADER);

  bi.biWidth=(LONG)(imgwidth);

  bi.biHeight=(LONG)(imgheight);

  bi.biPlanes=1;

  bi.biBitCount=24;

  bi.biClrUsed=0;

 bi.biClrImportant=0;

 bi.biCompression=BI_RGB;

 linesize=WIDTHBYTES(bi.biWidth*bi.biBitCount);

  bi.biSizeImage=(DWORD)linesize*bi.biHeight;

  bibufsize=bi.biSizeImage+sizeof(BITMAPINFOHEADER);

  bi.biXPelsPerMeter=0;

  bi.biYPelsPerMeter=0;

 if((*hbi=GlobalAlloc(GHND,bibufsize))==NULL)

  {

  GlobalUnlock(hjpgbuf);

   GlobalFree(hjpgbuf);

  showerror(FUNC_MEMORY_ERROR);

  *hbi=NULL;

  return ;

  }

 lpbi=(unsigned char  *)GlobalLock(*hbi);

  pbi=(char *)&bi;

  for(i=0;i<sizeof(bi);i++)

  *lpbi++=*pbi++;

 bibandsize=linesize*8*SampRate_Y_V+sizeof(BITMAPINFOHEADER);

 if((hbiband=GlobalAlloc(GHND,bibandsize))==NULL)

  {

  GlobalUnlock(hjpgbuf);

   GlobalFree(hjpgbuf);

  GlobalUnlock(*hbi);

  GlobalFree(*hbi);

  showerror(FUNC_MEMORY_ERROR);

  *hbi=NULL;

  return ;

  }

 lpbibandheader=(unsigned char  *)GlobalLock(hbiband);

 lpbibandbits=lpbibandheader;

  bi.biHeight=8*SampRate_Y_V;

  pbi=(char *)&bi;

  for(i=0;i<sizeof(bi);i++)

  *lpbibandbits++=*pbi++;

 lpbibandbits--;

 if((SampRate_Y_H==0)||(SampRate_Y_V==0))

  {

  GlobalUnlock(hjpgbuf);

   GlobalFree(hjpgbuf);

  GlobalUnlock(*hbi);

  GlobalFree(*hbi);

  GlobalUnlock(hbiband);

  GlobalFree(hbiband);

  *hbi=NULL;

   showerror(FUNC_FORMAT_ERROR);

   return ;

   }

 i=imgwidth/(SampRate_Y_H*8);

 j=imgwidth%(SampRate_Y_H*8);

 funcret=Decode();

 if(funcret==FUNC_OK)

  {

   GlobalUnlock(hjpgbuf);

   GlobalFree(hjpgbuf);

  GlobalUnlock(hbiband);

  GlobalFree(hbiband);

  GlobalUnlock(*hbi);

  return ;

  }

 else

  {

   GlobalUnlock(hjpgbuf);

   GlobalFree(hjpgbuf);

  GlobalUnlock(*hbi);

  GlobalFree(*hbi);

  GlobalUnlock(hbiband);

  GlobalFree(hbiband);

  *hbi=NULL;

  showerror(funcret);

  return ;

  }

}

////////////////////////////////////////////////////////////////////////////////

FUNCRET InitTag()

{

 BOOL finish=FALSE;

 BYTE id;

 short  llength;

 short  i,j,k;

 short  huftab1,huftab2;

 short  huftabindex;

 BYTE hf_table_index;

 BYTE qt_table_index;

 BYTE comnum;

 unsigned char  *lptemp;

 short  ccount;

 lp=lpjpgbuf+2;

 while (!finish)

  {

  id=*(lp+1);

  lp+=2;

  switch (id)

   {

   case M_APP0:

    llength=MAKEWORD(*(lp+1),*lp);

    lp+=llength;

    break;

   case M_DQT:

    llength=MAKEWORD(*(lp+1),*lp);

    qt_table_index=(*(lp+2))&0x0f;

    lptemp=lp+3;

    if(llength<80)

     {

     for(i=0;i<64;i++)

      qt_table[qt_table_index][i]=(short)*(lptemp++);

     }

    else

     {

     for(i=0;i<64;i++)

      qt_table[qt_table_index][i]=(short)*(lptemp++);

                    qt_table_index=(*(lptemp++))&0x0f;

       for(i=0;i<64;i++)

      qt_table[qt_table_index][i]=(short)*(lptemp++);

       }

      lp+=llength;  

    break;

   case M_SOF0:

     llength=MAKEWORD(*(lp+1),*lp);

     imgheight=MAKEWORD(*(lp+4),*(lp+3));

     imgwidth=MAKEWORD(*(lp+6),*(lp+5));

                comp_num=*(lp+7);

      if((comp_num!=1)&&(comp_num!=3))

       return FUNC_FORMAT_ERROR;

    if(comp_num==3)

     {

     comp_index[0]=*(lp+8);

       SampRate_Y_H=(*(lp+9))>>4;

       SampRate_Y_V=(*(lp+9))&0x0f;

       YQtTable=(short *)qt_table[*(lp+10)];

     comp_index[1]=*(lp+11);

     SampRate_U_H=(*(lp+12))>>4;

       SampRate_U_V=(*(lp+12))&0x0f;

       UQtTable=(short *)qt_table[*(lp+13)];

       comp_index[2]=*(lp+14);

       SampRate_V_H=(*(lp+15))>>4;

       SampRate_V_V=(*(lp+15))&0x0f;

     VQtTable=(short *)qt_table[*(lp+16)];

       }

      else 

     {

       comp_index[0]=*(lp+8);

     SampRate_Y_H=(*(lp+9))>>4;

       SampRate_Y_V=(*(lp+9))&0x0f;

       YQtTable=(short *)qt_table[*(lp+10)];

     comp_index[1]=*(lp+8);

       SampRate_U_H=1;

       SampRate_U_V=1;

       UQtTable=(short *)qt_table[*(lp+10)];

     comp_index[2]=*(lp+8);

     SampRate_V_H=1;

       SampRate_V_V=1;

       VQtTable=(short *)qt_table[*(lp+10)];

      }

      lp+=llength;          

    break;

   case M_DHT:             

    llength=MAKEWORD(*(lp+1),*lp);

    if (llength<0xd0)

       {

     huftab1=(short)(*(lp+2))>>4;     //huftab1=0,1

      huftab2=(short)(*(lp+2))&0x0f;   //huftab2=0,1

     huftabindex=huftab1*2+huftab2;

      lptemp=lp+3;

     for (i=0; i<16; i++)

      code_len_table[huftabindex][i]=(short)(*(lptemp++));

     j=0;

     for (i=0; i<16; i++)

      if(code_len_table[huftabindex][i]!=0)

       {

       k=0;

       while(k<code_len_table[huftabindex][i])

        {

        code_value_table[huftabindex][k+j]=(short)(*(lptemp++));

        k++;

        }

       j+=k; 

       }

     i=0;

     while (code_len_table[huftabindex][i]==0)

       i++;

        for (j=0;j<i;j++)

         {

      huf_min_value[huftabindex][j]=0;

      huf_max_value[huftabindex][j]=0;

      }

     huf_min_value[huftabindex][i]=0;

     huf_max_value[huftabindex][i]=code_len_table[huftabindex][i]-1;

     for (j=i+1;j<16;j++)

      {

      huf_min_value[huftabindex][j]=(huf_max_value[huftabindex][j-1]+1)<<1;

      huf_max_value[huftabindex][j]=huf_min_value[huftabindex][j]+code_len_table[huftabindex][j]-1;

      }

        code_pos_table[huftabindex][0]=0;

     for (j=1;j<16;j++)

        code_pos_table[huftabindex][j]=code_len_table[huftabindex][j-1]+code_pos_table[huftabindex][j-1];

       lp+=llength;

     }  //if

    else

      {

      hf_table_index=*(lp+2);

     lp+=2;

     while (hf_table_index!=0xff)

        {

      huftab1=(short)hf_table_index>>4;     //huftab1=0,1

       huftab2=(short)hf_table_index&0x0f;   //huftab2=0,1

      huftabindex=huftab1*2+huftab2;

      lptemp=lp+1;

      ccount=0;

      for (i=0; i<16; i++)

       {

       code_len_table[huftabindex][i]=(short)(*(lptemp++));

       ccount+=code_len_table[huftabindex][i];

       }

      ccount+=17; 

      j=0;

      for (i=0; i<16; i++)

       if(code_len_table[huftabindex][i]!=0)

        {

        k=0;

        while(k<code_len_table[huftabindex][i])

         {

         code_value_table[huftabindex][k+j]=(short)(*(lptemp++));

         k++;

         }

        j+=k;

        }

      i=0;

      while (code_len_table[huftabindex][i]==0)

       i++;

       for (j=0;j<i;j++)

       {

       huf_min_value[huftabindex][j]=0;

       huf_max_value[huftabindex][j]=0;

       }

      huf_min_value[huftabindex][i]=0;

      huf_max_value[huftabindex][i]=code_len_table[huftabindex][i]-1;

      for (j=i+1;j<16;j++)

       {

       huf_min_value[huftabindex][j]=(huf_max_value[huftabindex][j-1]+1)<<1;

       huf_max_value[huftabindex][j]=huf_min_value[huftabindex][j]+code_len_table[huftabindex][j]-1;

       }

         code_pos_table[huftabindex][0]=0;

      for (j=1;j<16;j++)

       code_pos_table[huftabindex][j]=code_len_table[huftabindex][j-1]+code_pos_table[huftabindex][j-1];

      lp+=ccount;

      hf_table_index=*lp;

      }  //while

     }  //else

    break;

   case M_DRI:

     llength=MAKEWORD(*(lp+1),*lp);

       restart=MAKEWORD(*(lp+3),*(lp+2));

       lp+=llength;

    break;

   case M_SOS:

    llength=MAKEWORD(*(lp+1),*lp);

    comnum=*(lp+2);

    if(comnum!=comp_num)

     return FUNC_FORMAT_ERROR;

    lptemp=lp+3;

    for (i=0;i<comp_num;i++)

     {

     if(*lptemp==comp_index[0])

      {

      YDcIndex=(*(lptemp+1))>>4;   //Y

      YAcIndex=((*(lptemp+1))&0x0f)+2;

      }

     else

      {

      UVDcIndex=(*(lptemp+1))>>4;   //U,V

      UVAcIndex=((*(lptemp+1))&0x0f)+2;

      }

     lptemp+=2;

     }

    lp+=llength;

    finish=TRUE;

    break;

   case M_EOI:    

    return FUNC_FORMAT_ERROR;

    break;

   default:

     if ((id&0xf0)!=0xd0)

     {

     llength=MAKEWORD(*(lp+1),*lp);

      lp+=llength;

     }

    else 

     lp+=2;

    break;

     }  //switch

  } //while

 return FUNC_OK;

}

/////////////////////////////////////////////////////////////////////////

FUNCRET Decode()

{

 FUNCRET funcret;

 Y_in_MCU=SampRate_Y_H*SampRate_Y_V;

 U_in_MCU=SampRate_U_H*SampRate_U_V;

 V_in_MCU=SampRate_V_H*SampRate_V_V;

 H_YtoU=SampRate_Y_H/SampRate_U_H;

 V_YtoU=SampRate_Y_V/SampRate_U_V;

 H_YtoV=SampRate_Y_H/SampRate_V_H;

 V_YtoV=SampRate_Y_V/SampRate_V_V;

 Initialize_Fast_IDCT();

 while((funcret=DecodeMCUBlock())==FUNC_OK)

  {

  interval++;

  if((restart)&&(interval % restart==0))

    IntervalFlag=TRUE;

  else

   IntervalFlag=FALSE;

//lfj

  IQtIZzMCUComponent(0);

  IQtIZzMCUComponent(1);

  IQtIZzMCUComponent(2);

    GetYUV(0);

    GetYUV(1);

    GetYUV(2);

  StoreBuffer();

  sizej+=SampRate_Y_H*8;

  if(sizej>=imgwidth)

   {

   DispABand();

   sizej=0;

   sizei+=SampRate_Y_V*8;

   }

  if ((sizej==0)&&(sizei>=imgheight))

   break;

  }

 return funcret;

}

/////////////////////////////////////////////////////////////////////////////////////////

void  GetYUV(short flag)

{

 short H,VV;

 short i,j,k,h;

 int *buf;

 int *pQtZzMCU;

 switch(flag)

  {

  //lfj

  case 0:

   H=SampRate_Y_H;

   VV=SampRate_Y_V;

   buf=Y;

   pQtZzMCU=QtZzMCUBuffer;

   break;

  case 1:

   H=SampRate_U_H;

   VV=SampRate_U_V;

   buf=U;

   pQtZzMCU=QtZzMCUBuffer+Y_in_MCU*64;

   break;

  case 2:

   H=SampRate_V_H;

   VV=SampRate_V_V;

   buf=V;

   pQtZzMCU=QtZzMCUBuffer+(Y_in_MCU+U_in_MCU)*64;

   break;

  }

 for (i=0;i<VV;i++)

   for(j=0;j<H;j++)

  for(k=0;k<8;k++)

    for(h=0;h<8;h++)

    buf[(i*8+k)*SampRate_Y_H*8+j*8+h]=*pQtZzMCU++;

}

///////////////////////////////////////////////////////////////////////////////

void StoreBuffer()

{

 short i,j;

 unsigned char  *lpbmp;

 unsigned char  *lpbibandtemp;

 unsigned char R,G,B;

 int y,u,v,rr,gg,bb;

 for(i=0;i<SampRate_Y_V*8;i++)

  {

  if((sizei+i)<imgheight)

   {

   lpbmp=(lpbi+(DWORD)(imgheight-sizei-i-1)*linesize+sizej*3);

   lpbibandtemp=(lpbibandbits+(DWORD)(SampRate_Y_V*8-i-1)*linesize+sizej*3);

   for(j=0;j<SampRate_Y_H*8;j++)

    {

    if((sizej+j)<imgwidth)

     {

     y=Y[i*8*SampRate_Y_H+j];

     u=U[(i/V_YtoU)*8*SampRate_Y_H+j/H_YtoU];

     v=V[(i/V_YtoV)*8*SampRate_Y_H+j/H_YtoV];

     rr=((y<<8)+18*u+367*v)>>8;

     gg=((y<<8)-159*u-220*v)>>8;

     bb=((y<<8)+411*u-29*v)>>8;

     R=(unsigned char)rr;

     G=(unsigned char)gg;

     B=(unsigned char)bb;

     if (rr&0xffffff00) if (rr>255) R=255; else if (rr<0) R=0;

     if (gg&0xffffff00) if (gg>255) G=255; else if (gg<0) G=0;

     if (bb&0xffffff00) if (bb>255) B=255; else if (bb<0) B=0;

     *lpbmp++=B;

     *lpbmp++=G;

     *lpbmp++=R;

     *lpbibandtemp++=B;

     *lpbibandtemp++=G;

     *lpbibandtemp++=R;

     }

    else  break;

    }

   }

  else break;

  }

}

///////////////////////////////////////////////////////////////////////////////

FUNCRET DecodeMCUBlock()

{

 short *lpMCUBuffer;

 short i,j;

 FUNCRET funcret;

 if (IntervalFlag)

  {

  lp+=2;

  ycoef=ucoef=vcoef=0;

  BitPos=0;

  CurByte=0;

  }

 switch(comp_num)

  {

  case 3:

   lpMCUBuffer=MCUBuffer;

   for (i=0;i<SampRate_Y_H*SampRate_Y_V;i++)  //Y

    {

    funcret=HufBlock(YDcIndex,YAcIndex);

    if (funcret!=FUNC_OK)

     return funcret;

    BlockBuffer[0]=BlockBuffer[0]+ycoef;

    ycoef=BlockBuffer[0];

    for (j=0;j<64;j++)

     *lpMCUBuffer++=BlockBuffer[j];

    }

   for (i=0;i<SampRate_U_H*SampRate_U_V;i++)  //U

    {

    funcret=HufBlock(UVDcIndex,UVAcIndex);

    if (funcret!=FUNC_OK)

     return funcret;

    BlockBuffer[0]=BlockBuffer[0]+ucoef;

    ucoef=BlockBuffer[0];

    for (j=0;j<64;j++)

     *lpMCUBuffer++=BlockBuffer[j];

    }

   for (i=0;i<SampRate_V_H*SampRate_V_V;i++)  //V

    {

    funcret=HufBlock(UVDcIndex,UVAcIndex);

    if (funcret!=FUNC_OK)

     return funcret;

    BlockBuffer[0]=BlockBuffer[0]+vcoef;

    vcoef=BlockBuffer[0];

    for (j=0;j<64;j++)

     *lpMCUBuffer++=BlockBuffer[j];

    }

   break;

  case 1:

   lpMCUBuffer=MCUBuffer;

   funcret=HufBlock(YDcIndex,YAcIndex);

   if (funcret!=FUNC_OK)

    return funcret;

   BlockBuffer[0]=BlockBuffer[0]+ycoef;

   ycoef=BlockBuffer[0];

   for (j=0;j<64;j++)

    *lpMCUBuffer++=BlockBuffer[j];

   for (i=0;i<128;i++)

    *lpMCUBuffer++=0;

   break;

  default:

   return FUNC_FORMAT_ERROR;

  }

 return FUNC_OK;

}

///////////////////////////////////////////////////////////////////////////////

FUNCRET HufBlock(BYTE dchufindex,BYTE achufindex)

{

 short count=0;

 short i;

 FUNCRET funcret;

 //dc

 HufTabIndex=dchufindex;

 funcret=DecodeElement();

 if(funcret!=FUNC_OK)

  return funcret;

 BlockBuffer[count++]=vvalue;

 //ac

 HufTabIndex=achufindex;

 while (count<64)

  {

  funcret=DecodeElement();

  if(funcret!=FUNC_OK)

   return funcret;

  if ((rrun==0)&&(vvalue==0))

   {

   for (i=count;i<64;i++)

    BlockBuffer[i]=0;

   count=64;

   }

  else

   {

   for (i=0;i<rrun;i++)

    BlockBuffer[count++]=0;

   BlockBuffer[count++]=vvalue;

   }

  }

 return FUNC_OK;

}

//////////////////////////////////////////////////////////////////////////////

FUNCRET DecodeElement()

{

 int thiscode,tempcode;

 unsigned short temp,valueex;

 short codelen;

 BYTE hufexbyte,runsize,tempsize,sign;

 BYTE newbyte,lastbyte;

 if(BitPos>=1)

  {

  BitPos--;

  thiscode=(BYTE)CurByte>>BitPos;

  CurByte=CurByte&And[BitPos];

  }

 else

  {

  lastbyte=ReadByte();

  BitPos--;

  newbyte=CurByte&And[BitPos];

  thiscode=lastbyte>>7;

  CurByte=newbyte;

  }

 codelen=1;

 while ((thiscode<huf_min_value[HufTabIndex][codelen-1])||

    (code_len_table[HufTabIndex][codelen-1]==0)||

    (thiscode>huf_max_value[HufTabIndex][codelen-1]))

  {

  if(BitPos>=1)

   {

   BitPos--;

   tempcode=(BYTE)CurByte>>BitPos;

   CurByte=CurByte&And[BitPos];

   }

  else

   {

   lastbyte=ReadByte();

   BitPos--;

   newbyte=CurByte&And[BitPos];

   tempcode=(BYTE)lastbyte>>7;

   CurByte=newbyte;

   }

  thiscode=(thiscode<<1)+tempcode;

  codelen++;

  if(codelen>16)

   return FUNC_FORMAT_ERROR;

  }  //while

 temp=thiscode-huf_min_value[HufTabIndex][codelen-1]+code_pos_table[HufTabIndex][codelen-1];

 hufexbyte=(BYTE)code_value_table[HufTabIndex][temp];

 rrun=(short)(hufexbyte>>4);

 runsize=hufexbyte&0x0f;

 if(runsize==0)

  {

  vvalue=0;

  return FUNC_OK;

  }

 tempsize=runsize;

 if(BitPos>=runsize)

  {

  BitPos-=runsize;

  valueex=(BYTE)CurByte>>BitPos;

  CurByte=CurByte&And[BitPos];

  }

 else

  {

  valueex=CurByte;

  tempsize-=BitPos;

  while(tempsize>8)

   {

   lastbyte=ReadByte();

   valueex=(valueex<<8)+(BYTE)lastbyte;

   tempsize-=8;

   }  //while

  lastbyte=ReadByte();

  BitPos-=tempsize;

  valueex=(valueex<<tempsize)+(lastbyte>>BitPos);

  CurByte=lastbyte&And[BitPos];

  }  //else

 sign=valueex>>(runsize-1);

 if(sign)

  vvalue=valueex;

 else

  {

  valueex=valueex^0xffff;

  temp=0xffff<<runsize;

  vvalue=-(short)(valueex^temp);

  }

 return FUNC_OK;

}

/////////////////////////////////////////////////////////////////////////////////////

void IQtIZzMCUComponent(short flag)

{

 short H,VV;

 short i,j;

 int *pQtZzMCUBuffer;

 short  *pMCUBuffer;

 switch(flag)

  {

  case 0:

   H=SampRate_Y_H;

   VV=SampRate_Y_V;

   pMCUBuffer=MCUBuffer;

   pQtZzMCUBuffer=QtZzMCUBuffer;

   break;

  case 1:

   H=SampRate_U_H;

   VV=SampRate_U_V;

   pMCUBuffer=MCUBuffer+Y_in_MCU*64;

   pQtZzMCUBuffer=QtZzMCUBuffer+Y_in_MCU*64;

   break;

  case 2:

   H=SampRate_V_H;

   VV=SampRate_V_V;

   pMCUBuffer=MCUBuffer+(Y_in_MCU+U_in_MCU)*64;

   pQtZzMCUBuffer=QtZzMCUBuffer+(Y_in_MCU+U_in_MCU)*64;

   break;

  }

 for(i=0;i<VV;i++)

  for (j=0;j<H;j++)

   IQtIZzBlock(pMCUBuffer+(i*H+j)*64,pQtZzMCUBuffer+(i*H+j)*64,flag);

}

//////////////////////////////////////////////////////////////////////////////////////////

void IQtIZzBlock(short  *s ,int * d,short flag)

{

 short i,j;

 short tag;

 short *pQt;

 int buffer2[8][8];

 int *buffer1;

 short offset;

 switch(flag)

  {

  case 0:

   pQt=YQtTable;

   offset=128;

   break;

  case 1:

   pQt=UQtTable;

   offset=0;

   break;

  case 2:

   pQt=VQtTable;

   offset=0;

   break;

  }

 for(i=0;i<8;i++)

  for(j=0;j<8;j++)

    {

    //tag=Zig_Zag[j][i];

   tag=Zig_Zag[i][j];

    buffer2[i][j]=(int)s[tag]*(int)pQt[tag];

    }

 buffer1=(int *)buffer2;

 Fast_IDCT(buffer1);

 for(i=0;i<8;i++)

  for(j=0;j<8;j++)

   d[i*8+j]=buffer2[i][j]+offset;

}

///////////////////////////////////////////////////////////////////////////////

void Fast_IDCT(int * block)

{

 short i;

 for (i=0; i<8; i++)

  idctrow(block+8*i);

 for (i=0; i<8; i++)

  idctcol(block+i);

}

///////////////////////////////////////////////////////////////////////////////

BYTE  ReadByte()

{

 BYTE  i;

 i=*(lp++);

 if(i==0xff)

  lp++;

 BitPos=8;

 CurByte=i;

 return i;

}

/////////////////////////////////////////////////

void showerror(FUNCRET funcret)

{

 switch(funcret)

  {

  case FUNC_MEMORY_ERROR:

   MessageBox(NULL,"Not enough memory!","Error Message",MB_OK);

   break;

  case FUNC_FILE_ERROR:

   MessageBox(NULL,"File read error!","Error Message",MB_OK);

   break;

  case FUNC_FORMAT_ERROR:

   MessageBox(NULL,"File format error!","Error Message",MB_OK);

   break;

  }

}

/////////////////////////////////////////////////////////////////

void InitTable()

{

 short i,j;

 sizei=sizej=0;

 imgwidth=imgheight=0;

 rrun=vvalue=0;

 BitPos=0;

 CurByte=0;

 IntervalFlag=FALSE;

 restart=0;

 for(i=0;i<3;i++)

  for(j=0;j<64;j++)

   qt_table[i][j]=0;

  comp_num=0;

  HufTabIndex=0;

  for(i=0;i<3;i++)

  comp_index[i]=0;

 for(i=0;i<4;i++)

  for(j=0;j<16;j++)

   {

   code_len_table[i][j]=0;

   code_pos_table[i][j]=0;

   huf_max_value[i][j]=0;

   huf_min_value[i][j]=0;

   }

 for(i=0;i<4;i++)

  for(j=0;j<256;j++)

   code_value_table[i][j]=0;

 for(i=0;i<10*64;i++)

  {

  MCUBuffer[i]=0;

  QtZzMCUBuffer[i]=0;

  }

 for(i=0;i<64;i++)

  {

  Y[i]=0;

  U[i]=0;

  V[i]=0;

  BlockBuffer[i]=0;

  }

 ycoef=ucoef=vcoef=0;

}

////////////////////////////////////////////////////////////////////

void DispABand()

{

 HDC         hDC;

 //RECT      destrc,sourc;

 hDC = GetDC (hwnd);

  if (!hbiband)

  return;

  /*

  sourc.left   = 0;

  sourc.top    = 0;

  sourc.right  = imgwidth;

  sourc.bottom = 8*SampRate_Y_V;

  destrc.left=0;

  destrc.top=imgheight-sizei-8*SampRate_Y_V;

  destrc.right=destrc.left+imgwidth;

  destrc.bottom=imgheight-sizei;

  */

 SetStretchBltMode (hDC, COLORONCOLOR);

  SetDIBitsToDevice (hDC,

      0,

           sizei,

           imgwidth,

           8*SampRate_Y_V,

           0,

           0,

      0,

           8*SampRate_Y_V,

      lpbibandbits,

         (LPBITMAPINFO)lpbibandheader,

         DIB_RGB_COLORS);

 ReleaseDC(hwnd,hDC);

}

///////////////////////////////////////////////////////////////

void GetJPGSize(char *fn)

{

 HFILE  hf;

 HLOCAL hinfo;

 unsigned char *lpinfo;

 unsigned short llength;

 BYTE id;

 short  count=0;

  if((hf=_lopen(fn,OF_READ))==HFILE_ERROR)

  {

  showerror(FUNC_FILE_ERROR);

  imgwidth=0;

  imgheight=0;

  return ;

  }

 if((hinfo=LocalAlloc(LHND,2000))==NULL)

  {

  _lclose(hf);

  showerror(FUNC_MEMORY_ERROR);

  imgwidth=0;

  imgheight=0;

  return ;

     }

 lpinfo=(unsigned char *)LocalLock(hinfo);

 _lread(hf,(unsigned char *)lpinfo,2000);

 _lclose(hf);

 if((llength=MAKEWORD(*(lpinfo+1),*lpinfo))!=0xffd8)

  {

  showerror(FUNC_FORMAT_ERROR);

  LocalUnlock(hinfo);

  LocalFree(hinfo);

  imgwidth=0;

  imgheight=0;

  return;

  }

 lpinfo+=2;

 count+=2;

 while (count<2000)

   {

  id=*(lpinfo+1);

   lpinfo+=2;

   count+=2;

    switch (id)

     {

   case M_APP0:

   case M_DQT:

   case M_DHT:

   case M_DRI:

   case M_SOS:

     llength=MAKEWORD(*(lpinfo+1),*lpinfo);

     lpinfo+=llength;

     count+=llength;

    break;

   case M_SOF0:    

    llength=MAKEWORD(*(lpinfo+1),*lpinfo);

    imgheight=MAKEWORD(*(lpinfo+4),*(lpinfo+3));

    imgwidth=MAKEWORD(*(lpinfo+6),*(lpinfo+5));

    LocalUnlock(hinfo);

     LocalFree(hinfo);

     return;

   case M_EOI:

    showerror(FUNC_FORMAT_ERROR);

    LocalUnlock(hinfo);

    LocalFree(hinfo);

    imgwidth=0;

    imgheight=0;

    return;

   default:

     if ((id&0xf0)!=0xd0)

      {     

     llength=MAKEWORD(*(lpinfo+1),*lpinfo);

      lpinfo+=llength;

      count+=llength;

     }

    else

     {

     lpinfo+=2;

     count+=2;

     }

    break;

     }  //switch

   } //while

 showerror(FUNC_FORMAT_ERROR);

 LocalUnlock(hinfo);

 LocalFree(hinfo);

 imgwidth=0;

 imgheight=0;

 return;

}

///////////////////////////////////////////////////////////////////////

void Initialize_Fast_IDCT()

{

 short i;

 iclp = iclip+512;

 for (i= -512; i<512; i++)

  iclp[i] = (i<-256) ? -256 : ((i>255) ? 255 : i);

}

////////////////////////////////////////////////////////////////////////

static void idctrow(int * blk)

{

 int x0, x1, x2, x3, x4, x5, x6, x7, x8;

 /* intcut */

 if (!((x1 = blk[4]<<11) | (x2 = blk[6]) | (x3 = blk[2]) |

  (x4 = blk[1]) | (x5 = blk[7]) | (x6 = blk[5]) | (x7 = blk[3])))

 {

  blk[0]=blk[1]=blk[2]=blk[3]=blk[4]=blk[5]=blk[6]=blk[7]=blk[0]<<3;

  return;

 }

 x0 = (blk[0]<<11) + 128; /* for proper rounding in the fourth stage */

 /* first stage */

 x8 = W7*(x4+x5);

 x4 = x8 + (W1-W7)*x4;

 x5 = x8 - (W1+W7)*x5;

 x8 = W3*(x6+x7);

 x6 = x8 - (W3-W5)*x6;

 x7 = x8 - (W3+W5)*x7;

 /* second stage */

 x8 = x0 + x1;

 x0 -= x1;

 x1 = W6*(x3+x2);

 x2 = x1 - (W2+W6)*x2;

 x3 = x1 + (W2-W6)*x3;

 x1 = x4 + x6;

 x4 -= x6;

 x6 = x5 + x7;

 x5 -= x7;

 /* third stage */

 x7 = x8 + x3;

 x8 -= x3;

 x3 = x0 + x2;

 x0 -= x2;

 x2 = (181*(x4+x5)+128)>>8;

 x4 = (181*(x4-x5)+128)>>8;

 /* fourth stage */

 blk[0] = (x7+x1)>>8;

 blk[1] = (x3+x2)>>8;

 blk[2] = (x0+x4)>>8;

 blk[3] = (x8+x6)>>8;

 blk[4] = (x8-x6)>>8;

 blk[5] = (x0-x4)>>8;

 blk[6] = (x3-x2)>>8;

 blk[7] = (x7-x1)>>8;

}

//////////////////////////////////////////////////////////////////////////////

// column (vertical) IDCT

//

//             7                         pi         1

// dst[8*k] = sum c[l] * src[8*l] * cos( -- * ( k + - ) * l )

//            l=0                        8          2

//

// where: c[0]    = 1/1024

//        c[1..7] = (1/1024)*sqrt(2)

//

static void idctcol(int * blk)

{

 int x0, x1, x2, x3, x4, x5, x6, x7, x8;

 /* intcut */

 if (!((x1 = (blk[8*4]<<8)) | (x2 = blk[8*6]) | (x3 = blk[8*2]) |

  (x4 = blk[8*1]) | (x5 = blk[8*7]) | (x6 = blk[8*5]) | (x7 = blk[8*3])))

 {

  blk[8*0]=blk[8*1]=blk[8*2]=blk[8*3]=blk[8*4]=blk[8*5]

   =blk[8*6]=blk[8*7]=iclp[(blk[8*0]+32)>>6];

  return;

 }

 x0 = (blk[8*0]<<8) + 8192;

 /* first stage */

 x8 = W7*(x4+x5) + 4;

 x4 = (x8+(W1-W7)*x4)>>3;

 x5 = (x8-(W1+W7)*x5)>>3;

 x8 = W3*(x6+x7) + 4;

 x6 = (x8-(W3-W5)*x6)>>3;

 x7 = (x8-(W3+W5)*x7)>>3;

 /* second stage */

 x8 = x0 + x1;

 x0 -= x1;

 x1 = W6*(x3+x2) + 4;

 x2 = (x1-(W2+W6)*x2)>>3;

 x3 = (x1+(W2-W6)*x3)>>3;

 x1 = x4 + x6;

 x4 -= x6;

 x6 = x5 + x7;

 x5 -= x7;

 /* third stage */

 x7 = x8 + x3;

 x8 -= x3;

 x3 = x0 + x2;

 x0 -= x2;

 x2 = (181*(x4+x5)+128)>>8;

 x4 = (181*(x4-x5)+128)>>8;

 /* fourth stage */

 blk[8*0] = iclp[(x7+x1)>>14];

 blk[8*1] = iclp[(x3+x2)>>14];

 blk[8*2] = iclp[(x0+x4)>>14];

 blk[8*3] = iclp[(x8+x6)>>14];

 blk[8*4] = iclp[(x8-x6)>>14];

 blk[8*5] = iclp[(x0-x4)>>14];

 blk[8*6] = iclp[(x3-x2)>>14];

 blk[8*7] = iclp[(x7-x1)>>14];

}

2>lzw
//////////////////////////////////////////////////////////////////
//  用途 : LZW 压缩算法                //
//  创建 : [Foolish] / 2001-9-17       //
//////////////////////////////////////////////////////////////////
#ifndef  __FOO_LZW_COMPRESS_H__
#define  __FOO_LZW_COMPRESS_H__
#include <windows.h>
#pragma once

//===================================================================
// 定义
//===================================================================
// 解码程序用到String Table的最大长度
#define  LZW_MAX_TABLE_SIZE  4096

// 编码程序只用到Hash表, 不需要String Table, 因为它不需要知道String Table中的内容, 
// 只需要知道Old + Pixel字串是否在表中和表中的index
// Hash表设计为  :  (Old << 8) | Pixel 其中存放的是String Table的Index
#define  LZW_MAX_HASH_SIZE  0x1000FF // (4096 << 8) + 0xFF

// 解码程序用到String Table - string结构
// 每个string可以形成一棵二叉树, 此二叉树仅有一个右节点
// 因为wPrefix总是指向String Table中的另一位置, 而wSuffix指向0 ~ (clear-1)
typedef struct tagLZW_STRING
{
 WORD wPrefix ; // 为 Old
 WORD wSuffix ; // 为 GetFirstChar (Old) 或 GetFirstChar (Code)
} LZW_STRING, * PLZW_STRING ;

//===================================================================
// LZW - 压缩算法
//===================================================================
class FCLzw
{
public :
  FCLzw () ;
 ~FCLzw () ;

 /********************************************************************/
 /* 功  能 ：通用LZW编码           */
 /* 说  明 ：OutBuffer必须预先置0         */
 /*    输出的OutBuffer按 8 + ...data... 格式存储    */
 /********************************************************************/
 DWORD LZW_Encode (BYTE * InBuffer, DWORD dwLength,
      BYTE * OutBuffer) ;

 /********************************************************************/
 /* 功  能 ：通用LZW解码           */
 /* 说  明 ：InBuffer必须按 8 + ...data... 格式存储     */
 /********************************************************************/
 void LZW_Decode (BYTE * InBuffer, BYTE * OutBuffer) ;

 /********************************************************************/
 /* 功  能 ：编码DIB为GIF           */
 /* 参  数 ：把DIB左上角的指针传给DibBuffer       */
 /* 返回值 ：写入OutBuffer中的字节数        */
 /* 说  明 ：OutBuffer必须预先置0         */
 /*    暂时只支持4, 8位色位图编码，不支持1位色位图   */
 /********************************************************************/
 DWORD LZW_GIF_Encode (BYTE * DibBuffer, BYTE * OutBuffer,
       DWORD dwDibWidth, DWORD dwDibHeight,
       WORD wColorBit) ;

 /********************************************************************/
 /* 功  能 ：解码GIF到DIB           */
 /* 参  数 ：把DIB左上角的指针传给DibBuffer       */
 /* 说  明 ：所有位色GIF一律解码为8位色DIB, DibBuffer为8位色  */
 /*    InBuffer必须按 Min_Code_Length + ...data... 格式存储 */
 /*    所以GIF数据(InBuffer)先要解包      */
 /********************************************************************/
 void LZW_GIF_Decode (BYTE * InBuffer, BYTE * DibBuffer,
       DWORD dwDibWidth, DWORD dwDibHeight,
       bool bInterlace) ;

private :
 LZW_STRING * m_pStrBegin ; // String Table
 WORD  * m_pHash ;  // Hash Table (16bit存放m_pStrBegin的Index)
 WORD  m_LZW_CLEAR, m_LZW_END ; // clear和end标志

 // 交错存储用---------------------------------------------------+
 BYTE  m_byInterval ; // 用在交错存储的gif中, 作为m_Interval的索引, 0xFF为非交错存储
 BYTE  m_Interval[7] ; // 交错排列 : 8 8 4 2
 // 交错存储用---------------------------------------------------+

 BYTE  * m_pOrigin ;
 BYTE  * m_pCurIn ;
 BYTE  * m_pCurOut ;
 BYTE  m_byInBit ;  // 输入缓冲Bit位置
 BYTE  m_byOutBit ; // 输出缓冲Bit位置

 BYTE  m_byMinCode ;  // 最小码长度 (即原始颜色位数)
 BYTE  m_byCurrBits ;  // 现阶段码长
 WORD  m_CurrTableIndex ; // 当前的String Table Index 
 BYTE  m_Padding[2] ;

 DWORD  m_dwDibHeight ;  // DIB的高度
 DWORD  m_dwDibWidth ;   // DIB的宽度
 DWORD  m_dwCurrPixel ;  // 当前行写入的像素个数
 DWORD  m_dwCurrHeight ; // 当前的高
 DWORD  m_dwPitch ;   // DIB每行的字节数

private :
 // 解码
 void Decode_InitStringTable () ;
 WORD Decode_GetNextCode () ; // 最长12位
 bool Decode_IsInTable (WORD Code) ;
 void Decode_AddStringToTable (WORD wPrefix, WORD wSuffix) ;
 BYTE Decode_GetFirstChar (WORD Code) ;
 void Decode_WriteString_to8 (WORD Code) ; // 解码到目标8位色位图
 void Decode_SwitchToFollowLine () ; // 使m_pCurOut切换到下一行
 // 编码
 void Encode_InitStringTable () ;
 BYTE Encode_GetNextPixel () ;
 bool Encode_IsInTable (WORD Old, WORD Pixel) ; // Old和Pixel都指向在m_pStrBegin中的index。
 void Encode_AddStringToTable (WORD Old, WORD Pixel) ;
 void Encode_WriteIndex (DWORD wIndex) ; // 压缩时写String Index
} ;

//===================================================================
// Implement
//===================================================================
inline  FCLzw::FCLzw () {
 m_Interval[0] = m_Interval[1] = 8 ;
 m_Interval[2] = 4 ; m_Interval[3] = 2 ; // 交错行数
}
inline  FCLzw::~FCLzw () {
}
inline void  FCLzw::Decode_InitStringTable () {
 memset (m_pStrBegin, 0xFF, LZW_MAX_TABLE_SIZE * sizeof(LZW_STRING)) ;
 for (WORD i = 0 ; i < m_LZW_CLEAR ; i++) // 初始化String Table
  m_pStrBegin[i].wSuffix = i ;
 m_CurrTableIndex = m_LZW_END + 1 ;
 m_byCurrBits = m_byMinCode + 1 ;
}
inline void  FCLzw::Decode_SwitchToFollowLine () {
 m_dwCurrPixel = 0 ; // 到行开头
 if (m_byInterval == 0xFF) // 非交错存储, 直接到下一行
  m_pCurOut -= m_dwPitch ;
 else
 {
  m_dwCurrHeight += m_Interval[m_byInterval] ; // 交错存储的gif, 计算下一行
  if (m_dwCurrHeight >= m_dwDibHeight)
   if (++m_byInterval < 4)
    m_dwCurrHeight = m_Interval[m_byInterval]/2 ;
   else
    return ;
  m_pCurOut = m_pOrigin - m_dwCurrHeight * m_dwPitch ;
 }
}
inline WORD  FCLzw::Decode_GetNextCode () {
 // 与Encode_WriteIndex ()是相对应的, 最长为12位(最多跨越2-BYTE)
 register DWORD   dwRet = 0 ;
 register unsigned int uiAdd = m_byInBit + m_byCurrBits ;

 if (uiAdd <= 8) // 在当前BYTE内
  dwRet |= * m_pCurIn ;
 else
  if (uiAdd <= 16) // 跨1-BYTE
   dwRet |= * (WORD *) m_pCurIn ;
  else // 跨2-BYTE
  {
   dwRet |= * (m_pCurIn + 2) ;
   dwRet <<= 16 ;
   dwRet |= * (WORD *) m_pCurIn ;
  }
 m_pCurIn += uiAdd / 8 ;
 m_byInBit = uiAdd % 8 ;
 dwRet <<= 32 - uiAdd ;
 dwRet >>= 32 - m_byCurrBits ; // 左右清零
 return (WORD)dwRet ;
}
inline bool  FCLzw::Decode_IsInTable (WORD Code) {
 return (Code < m_CurrTableIndex) ;
}
inline BYTE  FCLzw::Decode_GetFirstChar (WORD Code) {
 while (m_pStrBegin[Code].wPrefix != 0xFFFF)
  Code = m_pStrBegin[Code].wPrefix ;
 return (BYTE) m_pStrBegin[Code].wSuffix ;
}
inline void  FCLzw::Decode_AddStringToTable (WORD wPrefix, WORD wSuffix) {
 m_pStrBegin[m_CurrTableIndex  ].wPrefix = wPrefix ;
 m_pStrBegin[m_CurrTableIndex++].wSuffix = wSuffix ;
 if ((m_CurrTableIndex == 0x008) || (m_CurrTableIndex == 0x010) ||
  (m_CurrTableIndex == 0x020) || (m_CurrTableIndex == 0x040) ||
  (m_CurrTableIndex == 0x080) || (m_CurrTableIndex == 0x100) ||
  (m_CurrTableIndex == 0x200) || (m_CurrTableIndex == 0x400) ||
  (m_CurrTableIndex == 0x800))
  m_byCurrBits++ ;
}
inline void  FCLzw::LZW_Decode (BYTE * InBuffer, BYTE * OutBuffer) {
 // 给dwDibWidth传入0则不换行
 this->LZW_GIF_Decode (InBuffer, OutBuffer, 0, 0, false) ;
}
//===================================================================
// Encode
//===================================================================
inline void  FCLzw::Encode_InitStringTable () {
 m_CurrTableIndex = m_LZW_END + 1 ;
 m_byCurrBits = m_byMinCode + 1 ;
 memset (m_pHash, 0x00, LZW_MAX_HASH_SIZE * sizeof(WORD)) ; // Hash Table置位0
}
inline BYTE  FCLzw::Encode_GetNextPixel () {
 // 目前只支持1, 4, 8位色, 所以不会跨BYTE
 register BYTE  byRet ;
 switch (m_byMinCode)
 {
  case 8 : byRet = m_pCurIn[m_dwCurrPixel] ; break ;
  case 4 : byRet = (m_dwCurrPixel % 2 == 0)
       ? m_pCurIn[m_dwCurrPixel / 2] >> 4
       : m_pCurIn[m_dwCurrPixel / 2] & 0x0F ; break ;
  case 1 : byRet = 0x01 & (m_pCurIn[m_dwCurrPixel / 8] >> (7 - (m_dwCurrPixel & 7))) ; break ;
 }
 if (++m_dwCurrPixel == m_dwDibWidth) // 行结束
 {
  m_dwCurrPixel = 0 ;
  m_dwCurrHeight++ ; // DIB下一行
  m_pCurIn = m_pOrigin - m_dwCurrHeight * m_dwPitch ;
 }
 return byRet ;
}
inline bool  FCLzw::Encode_IsInTable (WORD Old, WORD Pixel) {
 return (m_pHash[(Old << 8) | Pixel] != 0) ;
}
inline void  FCLzw::Encode_AddStringToTable (WORD Old, WORD Pixel) {
 m_pHash[(Old << 8) | Pixel] = m_CurrTableIndex++ ;
 if ((m_CurrTableIndex == 0x009) || (m_CurrTableIndex == 0x011) ||
  (m_CurrTableIndex == 0x021) || (m_CurrTableIndex == 0x041) ||
  (m_CurrTableIndex == 0x081) || (m_CurrTableIndex == 0x101) ||
  (m_CurrTableIndex == 0x201) || (m_CurrTableIndex == 0x401) ||
  (m_CurrTableIndex == 0x801))
  m_byCurrBits++ ;
}
inline void  FCLzw::Encode_WriteIndex (DWORD Index) {
 // 压缩时写String Index, 最长为12位(最多跨越2-BYTE)
 // 与Decode_GetNextCode ()是相对应的
 register unsigned int uiAdd = m_byOutBit + m_byCurrBits ;

 // 与解码不一样, 压缩时会预留一些内存, 可以用DWORD
 * (DWORD *) m_pCurOut |= (Index << m_byOutBit) ;
 m_byOutBit = uiAdd % 8 ;
 m_pCurOut += uiAdd / 8 ;
}
inline DWORD  FCLzw::LZW_Encode (BYTE * InBuffer, DWORD dwLength, BYTE * OutBuffer) {
 // 给dwDibWidth传入0则不换行
 *OutBuffer++ = 8 ;
 return 1 + this->LZW_GIF_Encode (InBuffer, OutBuffer, dwLength, 1, 8) ;
}

#endif

#include "stdafx.h"
#include "Lzw.h"

//===================================================================
// 目标为8位色
void  FCLzw::Decode_WriteString_to8 (WORD Code)
{
 if (Code < m_LZW_CLEAR)
 {
  m_pCurOut[m_dwCurrPixel++] = (BYTE) m_pStrBegin[Code].wSuffix ;
  if (m_dwCurrPixel == m_dwDibWidth) // 写完一行, 切换值至下一行
   this->Decode_SwitchToFollowLine () ;
 }
 else
 {
  this->Decode_WriteString_to8 (m_pStrBegin[Code].wPrefix) ;
  this->Decode_WriteString_to8 (m_pStrBegin[Code].wSuffix) ;
 }
}
//===================================================================
void  FCLzw::LZW_GIF_Decode (BYTE * InBuffer, BYTE * DibBuffer, DWORD dwDibWidth, DWORD dwDibHeight, bool bInterlace)
{
 m_pStrBegin = new LZW_STRING [LZW_MAX_TABLE_SIZE + 32] ;
 if ((m_pStrBegin == NULL) || (InBuffer == NULL) || (DibBuffer == NULL))
   return ;
 // 初始化设置-----------------------------------------+
 m_pCurIn = InBuffer + 1 ;
 m_pCurOut = DibBuffer ;
 m_byInBit = 0 ; // 输入从0开始, 输出为8位色位图
 m_byMinCode = *InBuffer ; // 获得最小码长度
 m_LZW_CLEAR = 1 << m_byMinCode ; // clear
 m_LZW_END = m_LZW_CLEAR + 1 ; // end

 m_dwDibWidth = dwDibWidth ; // DIB宽
 m_dwCurrPixel = 0 ;
 m_dwPitch = 4 * ((dwDibWidth * 8 + 31) / 32) ;

 m_byInterval = bInterlace ? 0 : 0xFF ; // 设置交错索引
 if (m_byInterval == 0) // 交错存储
 {
  m_pOrigin = m_pCurOut ;
  m_dwDibHeight = dwDibHeight ; // DIB高
  m_dwCurrHeight = 0 ; // 从最顶行开始
 }
 // 初始化设置完成-------------------------------------+

 WORD  Code, Old ;
 this->Decode_InitStringTable () ;
 while ((Code = this->Decode_GetNextCode ()) != m_LZW_END)
 {
  if (Code == m_LZW_CLEAR)
  {
   this->Decode_InitStringTable () ;
   while ((Code = this->Decode_GetNextCode ()) == m_LZW_CLEAR) ;
  }
  else
   if (this->Decode_IsInTable (Code))
    this->Decode_AddStringToTable (Old, this->Decode_GetFirstChar (Code)) ;
   else
    this->Decode_AddStringToTable (Old, this->Decode_GetFirstChar (Old)) ;

  this->Decode_WriteString_to8 (Code) ;
  Old = Code ;
 }
 delete[] m_pStrBegin ;
}
//===================================================================
DWORD  FCLzw::LZW_GIF_Encode (BYTE * DibBuffer, BYTE * OutBuffer,
         DWORD dwDibWidth, DWORD dwDibHeight,
         WORD wColorBit)
{
 // 编码阶段不需要String Table，只需要m_CurrTableIndex来获知写位数
 m_pHash = new WORD [LZW_MAX_HASH_SIZE] ;
 if ((m_pHash == NULL) || (DibBuffer == NULL) || (OutBuffer == NULL))
  return 0 ;
 // 初始化设置-----------------------------------------+
 m_byMinCode = (wColorBit == 1) ? 2 : wColorBit ; // 1, 4, 8位色(最小码长度)，1位色为2
 m_LZW_CLEAR = 1 << m_byMinCode ;
 m_LZW_END = m_LZW_CLEAR + 1 ;
 m_pOrigin = m_pCurIn = DibBuffer ;
 m_pCurOut = OutBuffer ;
 m_byInBit = 8 ;
 m_byOutBit = 0 ; // 输出从0开始

 m_dwDibWidth = dwDibWidth ;  // DIB宽
 m_dwDibHeight = dwDibHeight ; // DIB高
 m_dwCurrPixel = 0 ;
 m_dwCurrHeight = 0 ; // 从最顶行开始
 m_dwPitch = 4 * ((dwDibWidth * m_byMinCode + 31) / 32) ;
 // 初始化设置完成-------------------------------------+

 WORD  Old ; // 保留字串
 BYTE  Pixel ; // 当前读入字符
 this->Encode_InitStringTable () ;
 this->Encode_WriteIndex (m_LZW_CLEAR) ; // 首先写clear
 Old = this->Encode_GetNextPixel () ;
 // 编码
 while (m_dwCurrHeight < m_dwDibHeight)
 {
  Pixel = this->Encode_GetNextPixel () ;
  if (this->Encode_IsInTable (Old, Pixel))
   Old = m_pHash[(Old << 8) | Pixel] ; // 已在表中, 取出索引, Hash Table中存放的是String Table的Index
  else
  {
   // 不在表中, 把Old + Pixel添加到String Table中
   this->Encode_WriteIndex (Old) ;
   this->Encode_AddStringToTable (Old, Pixel) ;
   Old = Pixel ;
   if (m_CurrTableIndex == LZW_MAX_TABLE_SIZE) // 表填满
   {
    this->Encode_WriteIndex (Pixel) ;
    this->Encode_WriteIndex (m_LZW_CLEAR) ;
    this->Encode_InitStringTable () ;
    Old = this->Encode_GetNextPixel () ;
   }
  }
 }
 this->Encode_WriteIndex (Old) ;
 this->Encode_WriteIndex (m_LZW_END) ;
 delete[] m_pHash ;
 return (m_pCurOut - OutBuffer + 1) ;
}
//===================================================================

3>jpeg编码

#include <windows.h>
#define M_SOF0  0xc0
#define M_DHT   0xc4
#define M_EOI   0xd9
#define M_SOS   0xda
#define M_DQT   0xdb
#define M_DRI   0xdd
#define M_APP0  0xe0

static int Zig_Zag[8][8]={{0,1,5,6,14,15,27,28},
        {2,4,7,13,16,26,29,42},
        {3,8,12,17,25,30,41,43},
        {9,11,18,24,37,40,44,53},
        {10,19,23,32,39,45,52,54},
        {20,22,33,38,46,51,55,60},
        {21,34,37,47,50,56,59,61},
        {35,36,48,49,57,58,62,63}
       };

#define W1 2841 /* 2048*sqrt(2)*cos(1*pi/16) */
#define W2 2676 /* 2048*sqrt(2)*cos(2*pi/16) */
#define W3 2408 /* 2048*sqrt(2)*cos(3*pi/16) */
#define W5 1609 /* 2048*sqrt(2)*cos(5*pi/16) */
#define W6 1108 /* 2048*sqrt(2)*cos(6*pi/16) */
#define W7 565  /* 2048*sqrt(2)*cos(7*pi/16) */

//////////////////////////////////////////////////////////////////////////////

#define WIDTHBYTES(i)    ((i+31)/32*4)

#define FUNC_OK 0

#define FUNC_MEMORY_ERROR 1

#define FUNC_FILE_ERROR 2

#define FUNC_FORMAT_ERROR 3

typedef short FUNCRET;

////////////////////////////////////////////////////////////////////////////

void    DecodeJPG(HWND hWnd,char *fn,HGLOBAL *hbi);

void  InitTable();

void  showerror(FUNCRET funcret);

FUNCRET InitTag();

FUNCRET Decode();

FUNCRET DecodeMCUBlock();

FUNCRET HufBlock(BYTE dchufindex,BYTE achufindex);

FUNCRET DecodeElement();

void  IQtIZzMCUComponent(short flag);

void IQtIZzBlock(short  *s ,int * d,short flag);

void   GetYUV(short flag);

void    StoreBuffer();

BYTE    ReadByte();

void  DispABand();

void  GetJPGSize(char *fn);

void  Initialize_Fast_IDCT();

void  Fast_IDCT(int * block);

static void idctrow(int * blk);

static void idctcol(int * blk);

////////////////////////////////////////////////////////////////////////////

extern  int imgheight,imgwidth;

static  long iclip[1024];

static  long *iclp;

////////////////////////////////////////////////////////////////////////////

HFILE   hfjpg;

unsigned char  *lpbi;

DWORD   bibufsize;

DWORD   linesize;

HGLOBAL hjpgbuf;

unsigned char  *lpjpgbuf;

unsigned char  *lp;

DWORD   jpgbufsize;

short     SampRate_Y_H,SampRate_Y_V;

short     SampRate_U_H,SampRate_U_V;

short     SampRate_V_H,SampRate_V_V;

short     H_YtoU,V_YtoU,H_YtoV,V_YtoV;

short     Y_in_MCU,U_in_MCU,V_in_MCU;

short     sizei,sizej;

short  restart;

short     qt_table[3][64];

short     comp_num;

BYTE    comp_index[3];

BYTE    YDcIndex,YAcIndex,UVDcIndex,UVAcIndex;

BYTE    HufTabIndex;

short     *YQtTable,*UQtTable,*VQtTable;

BYTE    And[9]={0,1,3,7,0xf,0x1f,0x3f,0x7f,0xff};

short     code_pos_table[4][16],code_len_table[4][16];

unsigned short code_value_table[4][256];

unsigned short huf_max_value[4][16],huf_min_value[4][16];

short     BitPos,CurByte;

short     rrun,vvalue;

short    MCUBuffer[10*64];

int   QtZzMCUBuffer[10*64];

short    BlockBuffer[64];

short    ycoef,ucoef,vcoef;

BOOL    IntervalFlag;

 short  interval=0;

int    Y[4*64],U[4*64],V[4*64];

HGLOBAL  hbiband;

unsigned char  *lpbibandheader;

unsigned char  *lpbibandbits;

DWORD    bibandsize;

HWND     hwnd;

////////////////////////////////////////////////////////////////////////////

void DecodeJPG(HWND hWnd,char *fn,HGLOBAL *hbi)

{

  FUNCRET  funcret;

  BITMAPINFOHEADER bi;

  char *pbi;

  short  i,j;

  hwnd=hWnd;

  if((hfjpg=_lopen(fn,OF_READ))==HFILE_ERROR)

  {

  showerror(FUNC_FILE_ERROR);

  *hbi=NULL;

  return ;

  }

  //get jpg file length

  jpgbufsize=_llseek(hfjpg,0L,SEEK_END);

  //rewind to the beginning of the file

  _llseek(hfjpg,0L,SEEK_SET);

 if((hjpgbuf=GlobalAlloc(GHND,jpgbufsize))==NULL)

  {

  _lclose(hfjpg);

  showerror(FUNC_MEMORY_ERROR);

  *hbi=NULL;

  return ;

   }

 lpjpgbuf=(unsigned char  *)GlobalLock(hjpgbuf);

 _hread(hfjpg,(unsigned char  *)lpjpgbuf,jpgbufsize);

 _lclose(hfjpg);

  InitTable();

  if((funcret=InitTag())!=FUNC_OK)

  {

  GlobalUnlock(hjpgbuf);

  GlobalFree(hjpgbuf);

  showerror(funcret);

  *hbi=NULL;

  return ;

  }

  bi.biSize=(DWORD)sizeof(BITMAPINFOHEADER);

  bi.biWidth=(LONG)(imgwidth);

  bi.biHeight=(LONG)(imgheight);

  bi.biPlanes=1;

  bi.biBitCount=24;

  bi.biClrUsed=0;

 bi.biClrImportant=0;

 bi.biCompression=BI_RGB;

 linesize=WIDTHBYTES(bi.biWidth*bi.biBitCount);

  bi.biSizeImage=(DWORD)linesize*bi.biHeight;

  bibufsize=bi.biSizeImage+sizeof(BITMAPINFOHEADER);

  bi.biXPelsPerMeter=0;

  bi.biYPelsPerMeter=0;

 if((*hbi=GlobalAlloc(GHND,bibufsize))==NULL)

  {

  GlobalUnlock(hjpgbuf);

   GlobalFree(hjpgbuf);

  showerror(FUNC_MEMORY_ERROR);

  *hbi=NULL;

  return ;

  }

 lpbi=(unsigned char  *)GlobalLock(*hbi);

  pbi=(char *)&bi;

  for(i=0;i<sizeof(bi);i++)

  *lpbi++=*pbi++;

 bibandsize=linesize*8*SampRate_Y_V+sizeof(BITMAPINFOHEADER);

 if((hbiband=GlobalAlloc(GHND,bibandsize))==NULL)

  {

  GlobalUnlock(hjpgbuf);

   GlobalFree(hjpgbuf);

  GlobalUnlock(*hbi);

  GlobalFree(*hbi);

  showerror(FUNC_MEMORY_ERROR);

  *hbi=NULL;

  return ;

  }

 lpbibandheader=(unsigned char  *)GlobalLock(hbiband);

 lpbibandbits=lpbibandheader;

  bi.biHeight=8*SampRate_Y_V;

  pbi=(char *)&bi;

  for(i=0;i<sizeof(bi);i++)

  *lpbibandbits++=*pbi++;

 lpbibandbits--;

 if((SampRate_Y_H==0)||(SampRate_Y_V==0))

  {

  GlobalUnlock(hjpgbuf);

   GlobalFree(hjpgbuf);

  GlobalUnlock(*hbi);

  GlobalFree(*hbi);

  GlobalUnlock(hbiband);

  GlobalFree(hbiband);

  *hbi=NULL;

   showerror(FUNC_FORMAT_ERROR);

   return ;

   }

 i=imgwidth/(SampRate_Y_H*8);

 j=imgwidth%(SampRate_Y_H*8);

 funcret=Decode();

 if(funcret==FUNC_OK)

  {

   GlobalUnlock(hjpgbuf);

   GlobalFree(hjpgbuf);

  GlobalUnlock(hbiband);

  GlobalFree(hbiband);

  GlobalUnlock(*hbi);

  return ;

  }

 else

  {

   GlobalUnlock(hjpgbuf);

   GlobalFree(hjpgbuf);

  GlobalUnlock(*hbi);

  GlobalFree(*hbi);

  GlobalUnlock(hbiband);

  GlobalFree(hbiband);

  *hbi=NULL;

  showerror(funcret);

  return ;

  }

}

////////////////////////////////////////////////////////////////////////////////

FUNCRET InitTag()

{

 BOOL finish=FALSE;

 BYTE id;

 short  llength;

 short  i,j,k;

 short  huftab1,huftab2;

 short  huftabindex;

 BYTE hf_table_index;

 BYTE qt_table_index;

 BYTE comnum;

 unsigned char  *lptemp;

 short  ccount;

 lp=lpjpgbuf+2;

 while (!finish)

  {

  id=*(lp+1);

  lp+=2;

  switch (id)

   {

   case M_APP0:

    llength=MAKEWORD(*(lp+1),*lp);

    lp+=llength;

    break;

   case M_DQT:

    llength=MAKEWORD(*(lp+1),*lp);

    qt_table_index=(*(lp+2))&0x0f;

    lptemp=lp+3;

    if(llength<80)

     {

     for(i=0;i<64;i++)

      qt_table[qt_table_index][i]=(short)*(lptemp++);

     }

    else

     {

     for(i=0;i<64;i++)

      qt_table[qt_table_index][i]=(short)*(lptemp++);

                    qt_table_index=(*(lptemp++))&0x0f;

       for(i=0;i<64;i++)

      qt_table[qt_table_index][i]=(short)*(lptemp++);

       }

      lp+=llength;  

    break;

   case M_SOF0:

     llength=MAKEWORD(*(lp+1),*lp);

     imgheight=MAKEWORD(*(lp+4),*(lp+3));

     imgwidth=MAKEWORD(*(lp+6),*(lp+5));

                comp_num=*(lp+7);

      if((comp_num!=1)&&(comp_num!=3))

       return FUNC_FORMAT_ERROR;

    if(comp_num==3)

     {

     comp_index[0]=*(lp+8);

       SampRate_Y_H=(*(lp+9))>>4;

       SampRate_Y_V=(*(lp+9))&0x0f;

       YQtTable=(short *)qt_table[*(lp+10)];

     comp_index[1]=*(lp+11);

     SampRate_U_H=(*(lp+12))>>4;

       SampRate_U_V=(*(lp+12))&0x0f;

       UQtTable=(short *)qt_table[*(lp+13)];

       comp_index[2]=*(lp+14);

       SampRate_V_H=(*(lp+15))>>4;

       SampRate_V_V=(*(lp+15))&0x0f;

     VQtTable=(short *)qt_table[*(lp+16)];

       }

      else 

     {

       comp_index[0]=*(lp+8);

     SampRate_Y_H=(*(lp+9))>>4;

       SampRate_Y_V=(*(lp+9))&0x0f;

       YQtTable=(short *)qt_table[*(lp+10)];

     comp_index[1]=*(lp+8);

       SampRate_U_H=1;

       SampRate_U_V=1;

       UQtTable=(short *)qt_table[*(lp+10)];

     comp_index[2]=*(lp+8);

     SampRate_V_H=1;

       SampRate_V_V=1;

       VQtTable=(short *)qt_table[*(lp+10)];

      }

      lp+=llength;          

    break;

   case M_DHT:             

    llength=MAKEWORD(*(lp+1),*lp);

    if (llength<0xd0)

       {

     huftab1=(short)(*(lp+2))>>4;     //huftab1=0,1

      huftab2=(short)(*(lp+2))&0x0f;   //huftab2=0,1

     huftabindex=huftab1*2+huftab2;

      lptemp=lp+3;

     for (i=0; i<16; i++)

      code_len_table[huftabindex][i]=(short)(*(lptemp++));

     j=0;

     for (i=0; i<16; i++)

      if(code_len_table[huftabindex][i]!=0)

       {

       k=0;

       while(k<code_len_table[huftabindex][i])

        {

        code_value_table[huftabindex][k+j]=(short)(*(lptemp++));

        k++;

        }

       j+=k; 

       }

     i=0;

     while (code_len_table[huftabindex][i]==0)

       i++;

        for (j=0;j<i;j++)

         {

      huf_min_value[huftabindex][j]=0;

      huf_max_value[huftabindex][j]=0;

      }

     huf_min_value[huftabindex][i]=0;

     huf_max_value[huftabindex][i]=code_len_table[huftabindex][i]-1;

     for (j=i+1;j<16;j++)

      {

      huf_min_value[huftabindex][j]=(huf_max_value[huftabindex][j-1]+1)<<1;

      huf_max_value[huftabindex][j]=huf_min_value[huftabindex][j]+code_len_table[huftabindex][j]-1;

      }

        code_pos_table[huftabindex][0]=0;

     for (j=1;j<16;j++)

        code_pos_table[huftabindex][j]=code_len_table[huftabindex][j-1]+code_pos_table[huftabindex][j-1];

       lp+=llength;

     }  //if

    else

      {

      hf_table_index=*(lp+2);

     lp+=2;

     while (hf_table_index!=0xff)

        {

      huftab1=(short)hf_table_index>>4;     //huftab1=0,1

       huftab2=(short)hf_table_index&0x0f;   //huftab2=0,1

      huftabindex=huftab1*2+huftab2;

      lptemp=lp+1;

      ccount=0;

      for (i=0; i<16; i++)

       {

       code_len_table[huftabindex][i]=(short)(*(lptemp++));

       ccount+=code_len_table[huftabindex][i];

       }

      ccount+=17; 

      j=0;

      for (i=0; i<16; i++)

       if(code_len_table[huftabindex][i]!=0)

        {

        k=0;

        while(k<code_len_table[huftabindex][i])

         {

         code_value_table[huftabindex][k+j]=(short)(*(lptemp++));

         k++;

         }

        j+=k;

        }

      i=0;

      while (code_len_table[huftabindex][i]==0)

       i++;

       for (j=0;j<i;j++)

       {

       huf_min_value[huftabindex][j]=0;

       huf_max_value[huftabindex][j]=0;

       }

      huf_min_value[huftabindex][i]=0;

      huf_max_value[huftabindex][i]=code_len_table[huftabindex][i]-1;

      for (j=i+1;j<16;j++)

       {

       huf_min_value[huftabindex][j]=(huf_max_value[huftabindex][j-1]+1)<<1;

       huf_max_value[huftabindex][j]=huf_min_value[huftabindex][j]+code_len_table[huftabindex][j]-1;

       }

         code_pos_table[huftabindex][0]=0;

      for (j=1;j<16;j++)

       code_pos_table[huftabindex][j]=code_len_table[huftabindex][j-1]+code_pos_table[huftabindex][j-1];

      lp+=ccount;

      hf_table_index=*lp;

      }  //while

     }  //else

    break;

   case M_DRI:

     llength=MAKEWORD(*(lp+1),*lp);

       restart=MAKEWORD(*(lp+3),*(lp+2));

       lp+=llength;

    break;

   case M_SOS:

    llength=MAKEWORD(*(lp+1),*lp);

    comnum=*(lp+2);

    if(comnum!=comp_num)

     return FUNC_FORMAT_ERROR;

    lptemp=lp+3;

    for (i=0;i<comp_num;i++)

     {

     if(*lptemp==comp_index[0])

      {

      YDcIndex=(*(lptemp+1))>>4;   //Y

      YAcIndex=((*(lptemp+1))&0x0f)+2;

      }

     else

      {

      UVDcIndex=(*(lptemp+1))>>4;   //U,V

      UVAcIndex=((*(lptemp+1))&0x0f)+2;

      }

     lptemp+=2;

     }

    lp+=llength;

    finish=TRUE;

    break;

   case M_EOI:    

    return FUNC_FORMAT_ERROR;

    break;

   default:

     if ((id&0xf0)!=0xd0)

     {

     llength=MAKEWORD(*(lp+1),*lp);

      lp+=llength;

     }

    else 

     lp+=2;

    break;

     }  //switch

  } //while

 return FUNC_OK;

}

/////////////////////////////////////////////////////////////////////////

FUNCRET Decode()

{

 FUNCRET funcret;

 Y_in_MCU=SampRate_Y_H*SampRate_Y_V;

 U_in_MCU=SampRate_U_H*SampRate_U_V;

 V_in_MCU=SampRate_V_H*SampRate_V_V;

 H_YtoU=SampRate_Y_H/SampRate_U_H;

 V_YtoU=SampRate_Y_V/SampRate_U_V;

 H_YtoV=SampRate_Y_H/SampRate_V_H;

 V_YtoV=SampRate_Y_V/SampRate_V_V;

 Initialize_Fast_IDCT();

 while((funcret=DecodeMCUBlock())==FUNC_OK)

  {

  interval++;

  if((restart)&&(interval % restart==0))

    IntervalFlag=TRUE;

  else

   IntervalFlag=FALSE;

//lfj

  IQtIZzMCUComponent(0);

  IQtIZzMCUComponent(1);

  IQtIZzMCUComponent(2);

    GetYUV(0);

    GetYUV(1);

    GetYUV(2);

  StoreBuffer();

  sizej+=SampRate_Y_H*8;

  if(sizej>=imgwidth)

   {

   DispABand();

   sizej=0;

   sizei+=SampRate_Y_V*8;

   }

  if ((sizej==0)&&(sizei>=imgheight))

   break;

  }

 return funcret;

}

/////////////////////////////////////////////////////////////////////////////////////////

void  GetYUV(short flag)

{

 short H,VV;

 short i,j,k,h;

 int *buf;

 int *pQtZzMCU;

 switch(flag)

  {

  //lfj

  case 0:

   H=SampRate_Y_H;

   VV=SampRate_Y_V;

   buf=Y;

   pQtZzMCU=QtZzMCUBuffer;

   break;

  case 1:

   H=SampRate_U_H;

   VV=SampRate_U_V;

   buf=U;

   pQtZzMCU=QtZzMCUBuffer+Y_in_MCU*64;

   break;

  case 2:

   H=SampRate_V_H;

   VV=SampRate_V_V;

   buf=V;

   pQtZzMCU=QtZzMCUBuffer+(Y_in_MCU+U_in_MCU)*64;

   break;

  }

 for (i=0;i<VV;i++)

   for(j=0;j<H;j++)

  for(k=0;k<8;k++)

    for(h=0;h<8;h++)

    buf[(i*8+k)*SampRate_Y_H*8+j*8+h]=*pQtZzMCU++;

}

///////////////////////////////////////////////////////////////////////////////

void StoreBuffer()

{

 short i,j;

 unsigned char  *lpbmp;

 unsigned char  *lpbibandtemp;

 unsigned char R,G,B;

 int y,u,v,rr,gg,bb;

 for(i=0;i<SampRate_Y_V*8;i++)

  {

  if((sizei+i)<imgheight)

   {

   lpbmp=(lpbi+(DWORD)(imgheight-sizei-i-1)*linesize+sizej*3);

   lpbibandtemp=(lpbibandbits+(DWORD)(SampRate_Y_V*8-i-1)*linesize+sizej*3);

   for(j=0;j<SampRate_Y_H*8;j++)

    {

    if((sizej+j)<imgwidth)

     {

     y=Y[i*8*SampRate_Y_H+j];

     u=U[(i/V_YtoU)*8*SampRate_Y_H+j/H_YtoU];

     v=V[(i/V_YtoV)*8*SampRate_Y_H+j/H_YtoV];

     rr=((y<<8)+18*u+367*v)>>8;

     gg=((y<<8)-159*u-220*v)>>8;

     bb=((y<<8)+411*u-29*v)>>8;

     R=(unsigned char)rr;

     G=(unsigned char)gg;

     B=(unsigned char)bb;

     if (rr&0xffffff00) if (rr>255) R=255; else if (rr<0) R=0;

     if (gg&0xffffff00) if (gg>255) G=255; else if (gg<0) G=0;

     if (bb&0xffffff00) if (bb>255) B=255; else if (bb<0) B=0;

     *lpbmp++=B;

     *lpbmp++=G;

     *lpbmp++=R;

     *lpbibandtemp++=B;

     *lpbibandtemp++=G;

     *lpbibandtemp++=R;

     }

    else  break;

    }

   }

  else break;

  }

}

///////////////////////////////////////////////////////////////////////////////

FUNCRET DecodeMCUBlock()

{

 short *lpMCUBuffer;

 short i,j;

 FUNCRET funcret;

 if (IntervalFlag)

  {

  lp+=2;

  ycoef=ucoef=vcoef=0;

  BitPos=0;

  CurByte=0;

  }

 switch(comp_num)

  {

  case 3:

   lpMCUBuffer=MCUBuffer;

   for (i=0;i<SampRate_Y_H*SampRate_Y_V;i++)  //Y

    {

    funcret=HufBlock(YDcIndex,YAcIndex);

    if (funcret!=FUNC_OK)

     return funcret;

    BlockBuffer[0]=BlockBuffer[0]+ycoef;

    ycoef=BlockBuffer[0];

    for (j=0;j<64;j++)

     *lpMCUBuffer++=BlockBuffer[j];

    }

   for (i=0;i<SampRate_U_H*SampRate_U_V;i++)  //U

    {

    funcret=HufBlock(UVDcIndex,UVAcIndex);

    if (funcret!=FUNC_OK)

     return funcret;

    BlockBuffer[0]=BlockBuffer[0]+ucoef;

    ucoef=BlockBuffer[0];

    for (j=0;j<64;j++)

     *lpMCUBuffer++=BlockBuffer[j];

    }

   for (i=0;i<SampRate_V_H*SampRate_V_V;i++)  //V

    {

    funcret=HufBlock(UVDcIndex,UVAcIndex);

    if (funcret!=FUNC_OK)

     return funcret;

    BlockBuffer[0]=BlockBuffer[0]+vcoef;

    vcoef=BlockBuffer[0];

    for (j=0;j<64;j++)

     *lpMCUBuffer++=BlockBuffer[j];

    }

   break;

  case 1:

   lpMCUBuffer=MCUBuffer;

   funcret=HufBlock(YDcIndex,YAcIndex);

   if (funcret!=FUNC_OK)

    return funcret;

   BlockBuffer[0]=BlockBuffer[0]+ycoef;

   ycoef=BlockBuffer[0];

   for (j=0;j<64;j++)

    *lpMCUBuffer++=BlockBuffer[j];

   for (i=0;i<128;i++)

    *lpMCUBuffer++=0;

   break;

  default:

   return FUNC_FORMAT_ERROR;

  }

 return FUNC_OK;

}

///////////////////////////////////////////////////////////////////////////////

FUNCRET HufBlock(BYTE dchufindex,BYTE achufindex)

{

 short count=0;

 short i;

 FUNCRET funcret;

 //dc

 HufTabIndex=dchufindex;

 funcret=DecodeElement();

 if(funcret!=FUNC_OK)

  return funcret;

 BlockBuffer[count++]=vvalue;

 //ac

 HufTabIndex=achufindex;

 while (count<64)

  {

  funcret=DecodeElement();

  if(funcret!=FUNC_OK)

   return funcret;

  if ((rrun==0)&&(vvalue==0))

   {

   for (i=count;i<64;i++)

    BlockBuffer[i]=0;

   count=64;

   }

  else

   {

   for (i=0;i<rrun;i++)

    BlockBuffer[count++]=0;

   BlockBuffer[count++]=vvalue;

   }

  }

 return FUNC_OK;

}

//////////////////////////////////////////////////////////////////////////////

FUNCRET DecodeElement()

{

 int thiscode,tempcode;

 unsigned short temp,valueex;

 short codelen;

 BYTE hufexbyte,runsize,tempsize,sign;

 BYTE newbyte,lastbyte;

 if(BitPos>=1)

  {

  BitPos--;

  thiscode=(BYTE)CurByte>>BitPos;

  CurByte=CurByte&And[BitPos];

  }

 else

  {

  lastbyte=ReadByte();

  BitPos--;

  newbyte=CurByte&And[BitPos];

  thiscode=lastbyte>>7;

  CurByte=newbyte;

  }

 codelen=1;

 while ((thiscode<huf_min_value[HufTabIndex][codelen-1])||

    (code_len_table[HufTabIndex][codelen-1]==0)||

    (thiscode>huf_max_value[HufTabIndex][codelen-1]))

  {

  if(BitPos>=1)

   {

   BitPos--;

   tempcode=(BYTE)CurByte>>BitPos;

   CurByte=CurByte&And[BitPos];

   }

  else

   {

   lastbyte=ReadByte();

   BitPos--;

   newbyte=CurByte&And[BitPos];

   tempcode=(BYTE)lastbyte>>7;

   CurByte=newbyte;

   }

  thiscode=(thiscode<<1)+tempcode;

  codelen++;

  if(codelen>16)

   return FUNC_FORMAT_ERROR;

  }  //while

 temp=thiscode-huf_min_value[HufTabIndex][codelen-1]+code_pos_table[HufTabIndex][codelen-1];

 hufexbyte=(BYTE)code_value_table[HufTabIndex][temp];

 rrun=(short)(hufexbyte>>4);

 runsize=hufexbyte&0x0f;

 if(runsize==0)

  {

  vvalue=0;

  return FUNC_OK;

  }

 tempsize=runsize;

 if(BitPos>=runsize)

  {

  BitPos-=runsize;

  valueex=(BYTE)CurByte>>BitPos;

  CurByte=CurByte&And[BitPos];

  }

 else

  {

  valueex=CurByte;

  tempsize-=BitPos;

  while(tempsize>8)

   {

   lastbyte=ReadByte();

   valueex=(valueex<<8)+(BYTE)lastbyte;

   tempsize-=8;

   }  //while

  lastbyte=ReadByte();

  BitPos-=tempsize;

  valueex=(valueex<<tempsize)+(lastbyte>>BitPos);

  CurByte=lastbyte&And[BitPos];

  }  //else

 sign=valueex>>(runsize-1);

 if(sign)

  vvalue=valueex;

 else

  {

  valueex=valueex^0xffff;

  temp=0xffff<<runsize;

  vvalue=-(short)(valueex^temp);

  }

 return FUNC_OK;

}

/////////////////////////////////////////////////////////////////////////////////////

void IQtIZzMCUComponent(short flag)

{

 short H,VV;

 short i,j;

 int *pQtZzMCUBuffer;

 short  *pMCUBuffer;

 switch(flag)

  {

  case 0:

   H=SampRate_Y_H;

   VV=SampRate_Y_V;

   pMCUBuffer=MCUBuffer;

   pQtZzMCUBuffer=QtZzMCUBuffer;

   break;

  case 1:

   H=SampRate_U_H;

   VV=SampRate_U_V;

   pMCUBuffer=MCUBuffer+Y_in_MCU*64;

   pQtZzMCUBuffer=QtZzMCUBuffer+Y_in_MCU*64;

   break;

  case 2:

   H=SampRate_V_H;

   VV=SampRate_V_V;

   pMCUBuffer=MCUBuffer+(Y_in_MCU+U_in_MCU)*64;

   pQtZzMCUBuffer=QtZzMCUBuffer+(Y_in_MCU+U_in_MCU)*64;

   break;

  }

 for(i=0;i<VV;i++)

  for (j=0;j<H;j++)

   IQtIZzBlock(pMCUBuffer+(i*H+j)*64,pQtZzMCUBuffer+(i*H+j)*64,flag);

}

//////////////////////////////////////////////////////////////////////////////////////////

void IQtIZzBlock(short  *s ,int * d,short flag)

{

 short i,j;

 short tag;

 short *pQt;

 int buffer2[8][8];

 int *buffer1;

 short offset;

 switch(flag)

  {

  case 0:

   pQt=YQtTable;

   offset=128;

   break;

  case 1:

   pQt=UQtTable;

   offset=0;

   break;

  case 2:

   pQt=VQtTable;

   offset=0;

   break;

  }

 for(i=0;i<8;i++)

  for(j=0;j<8;j++)

    {

    //tag=Zig_Zag[j][i];

   tag=Zig_Zag[i][j];

    buffer2[i][j]=(int)s[tag]*(int)pQt[tag];

    }

 buffer1=(int *)buffer2;

 Fast_IDCT(buffer1);

 for(i=0;i<8;i++)

  for(j=0;j<8;j++)

   d[i*8+j]=buffer2[i][j]+offset;

}

///////////////////////////////////////////////////////////////////////////////

void Fast_IDCT(int * block)

{

 short i;

 for (i=0; i<8; i++)

  idctrow(block+8*i);

 for (i=0; i<8; i++)

  idctcol(block+i);

}

///////////////////////////////////////////////////////////////////////////////

BYTE  ReadByte()

{

 BYTE  i;

 i=*(lp++);

 if(i==0xff)

  lp++;

 BitPos=8;

 CurByte=i;

 return i;

}

/////////////////////////////////////////////////

void showerror(FUNCRET funcret)

{

 switch(funcret)

  {

  case FUNC_MEMORY_ERROR:

   MessageBox(NULL,"Not enough memory!","Error Message",MB_OK);

   break;

  case FUNC_FILE_ERROR:

   MessageBox(NULL,"File read error!","Error Message",MB_OK);

   break;

  case FUNC_FORMAT_ERROR:

   MessageBox(NULL,"File format error!","Error Message",MB_OK);

   break;

  }

}

/////////////////////////////////////////////////////////////////

void InitTable()

{

 short i,j;

 sizei=sizej=0;

 imgwidth=imgheight=0;

 rrun=vvalue=0;

 BitPos=0;

 CurByte=0;

 IntervalFlag=FALSE;

 restart=0;

 for(i=0;i<3;i++)

  for(j=0;j<64;j++)

   qt_table[i][j]=0;

  comp_num=0;

  HufTabIndex=0;

  for(i=0;i<3;i++)

  comp_index[i]=0;

 for(i=0;i<4;i++)

  for(j=0;j<16;j++)

   {

   code_len_table[i][j]=0;

   code_pos_table[i][j]=0;

   huf_max_value[i][j]=0;

   huf_min_value[i][j]=0;

   }

 for(i=0;i<4;i++)

  for(j=0;j<256;j++)

   code_value_table[i][j]=0;

 for(i=0;i<10*64;i++)

  {

  MCUBuffer[i]=0;

  QtZzMCUBuffer[i]=0;

  }

 for(i=0;i<64;i++)

  {

  Y[i]=0;

  U[i]=0;

  V[i]=0;

  BlockBuffer[i]=0;

  }

 ycoef=ucoef=vcoef=0;

}

////////////////////////////////////////////////////////////////////

void DispABand()

{

 HDC         hDC;

 //RECT      destrc,sourc;

 hDC = GetDC (hwnd);

  if (!hbiband)

  return;

  /*

  sourc.left   = 0;

  sourc.top    = 0;

  sourc.right  = imgwidth;

  sourc.bottom = 8*SampRate_Y_V;

  destrc.left=0;

  destrc.top=imgheight-sizei-8*SampRate_Y_V;

  destrc.right=destrc.left+imgwidth;

  destrc.bottom=imgheight-sizei;

  */

 SetStretchBltMode (hDC, COLORONCOLOR);

  SetDIBitsToDevice (hDC,

      0,

           sizei,

           imgwidth,

           8*SampRate_Y_V,

           0,

           0,

      0,

           8*SampRate_Y_V,

      lpbibandbits,

         (LPBITMAPINFO)lpbibandheader,

         DIB_RGB_COLORS);

 ReleaseDC(hwnd,hDC);

}

///////////////////////////////////////////////////////////////

void GetJPGSize(char *fn)

{

 HFILE  hf;

 HLOCAL hinfo;

 unsigned char *lpinfo;

 unsigned short llength;

 BYTE id;

 short  count=0;

  if((hf=_lopen(fn,OF_READ))==HFILE_ERROR)

  {

  showerror(FUNC_FILE_ERROR);

  imgwidth=0;

  imgheight=0;

  return ;

  }

 if((hinfo=LocalAlloc(LHND,2000))==NULL)

  {

  _lclose(hf);

  showerror(FUNC_MEMORY_ERROR);

  imgwidth=0;

  imgheight=0;

  return ;

     }

 lpinfo=(unsigned char *)LocalLock(hinfo);

 _lread(hf,(unsigned char *)lpinfo,2000);

 _lclose(hf);

 if((llength=MAKEWORD(*(lpinfo+1),*lpinfo))!=0xffd8)

  {

  showerror(FUNC_FORMAT_ERROR);

  LocalUnlock(hinfo);

  LocalFree(hinfo);

  imgwidth=0;

  imgheight=0;

  return;

  }

 lpinfo+=2;

 count+=2;

 while (count<2000)

   {

  id=*(lpinfo+1);

   lpinfo+=2;

   count+=2;

    switch (id)

     {

   case M_APP0:

   case M_DQT:

   case M_DHT:

   case M_DRI:

   case M_SOS:

     llength=MAKEWORD(*(lpinfo+1),*lpinfo);

     lpinfo+=llength;

     count+=llength;

    break;

   case M_SOF0:    

    llength=MAKEWORD(*(lpinfo+1),*lpinfo);

    imgheight=MAKEWORD(*(lpinfo+4),*(lpinfo+3));

    imgwidth=MAKEWORD(*(lpinfo+6),*(lpinfo+5));

    LocalUnlock(hinfo);

     LocalFree(hinfo);

     return;

   case M_EOI:

    showerror(FUNC_FORMAT_ERROR);

    LocalUnlock(hinfo);

    LocalFree(hinfo);

    imgwidth=0;

    imgheight=0;

    return;

   default:

     if ((id&0xf0)!=0xd0)

      {     

     llength=MAKEWORD(*(lpinfo+1),*lpinfo);

      lpinfo+=llength;

      count+=llength;

     }

    else

     {

     lpinfo+=2;

     count+=2;

     }

    break;

     }  //switch

   } //while

 showerror(FUNC_FORMAT_ERROR);

 LocalUnlock(hinfo);

 LocalFree(hinfo);

 imgwidth=0;

 imgheight=0;

 return;

}

///////////////////////////////////////////////////////////////////////

void Initialize_Fast_IDCT()

{

 short i;

 iclp = iclip+512;

 for (i= -512; i<512; i++)

  iclp[i] = (i<-256) ? -256 : ((i>255) ? 255 : i);

}

////////////////////////////////////////////////////////////////////////

static void idctrow(int * blk)

{

 int x0, x1, x2, x3, x4, x5, x6, x7, x8;

 /* intcut */

 if (!((x1 = blk[4]<<11) | (x2 = blk[6]) | (x3 = blk[2]) |

  (x4 = blk[1]) | (x5 = blk[7]) | (x6 = blk[5]) | (x7 = blk[3])))

 {

  blk[0]=blk[1]=blk[2]=blk[3]=blk[4]=blk[5]=blk[6]=blk[7]=blk[0]<<3;

  return;

 }

 x0 = (blk[0]<<11) + 128; /* for proper rounding in the fourth stage */

 /* first stage */

 x8 = W7*(x4+x5);

 x4 = x8 + (W1-W7)*x4;

 x5 = x8 - (W1+W7)*x5;

 x8 = W3*(x6+x7);

 x6 = x8 - (W3-W5)*x6;

 x7 = x8 - (W3+W5)*x7;

 /* second stage */

 x8 = x0 + x1;

 x0 -= x1;

 x1 = W6*(x3+x2);

 x2 = x1 - (W2+W6)*x2;

 x3 = x1 + (W2-W6)*x3;

 x1 = x4 + x6;

 x4 -= x6;

 x6 = x5 + x7;

 x5 -= x7;

 /* third stage */

 x7 = x8 + x3;

 x8 -= x3;

 x3 = x0 + x2;

 x0 -= x2;

 x2 = (181*(x4+x5)+128)>>8;

 x4 = (181*(x4-x5)+128)>>8;

 /* fourth stage */

 blk[0] = (x7+x1)>>8;

 blk[1] = (x3+x2)>>8;

 blk[2] = (x0+x4)>>8;

 blk[3] = (x8+x6)>>8;

 blk[4] = (x8-x6)>>8;

 blk[5] = (x0-x4)>>8;

 blk[6] = (x3-x2)>>8;

 blk[7] = (x7-x1)>>8;

}

//////////////////////////////////////////////////////////////////////////////

// column (vertical) IDCT

//

//             7                         pi         1

// dst[8*k] = sum c[l] * src[8*l] * cos( -- * ( k + - ) * l )

//            l=0                        8          2

//

// where: c[0]    = 1/1024

//        c[1..7] = (1/1024)*sqrt(2)

//

static void idctcol(int * blk)

{

 int x0, x1, x2, x3, x4, x5, x6, x7, x8;

 /* intcut */

 if (!((x1 = (blk[8*4]<<8)) | (x2 = blk[8*6]) | (x3 = blk[8*2]) |

  (x4 = blk[8*1]) | (x5 = blk[8*7]) | (x6 = blk[8*5]) | (x7 = blk[8*3])))

 {

  blk[8*0]=blk[8*1]=blk[8*2]=blk[8*3]=blk[8*4]=blk[8*5]

   =blk[8*6]=blk[8*7]=iclp[(blk[8*0]+32)>>6];

  return;

 }

 x0 = (blk[8*0]<<8) + 8192;

 /* first stage */

 x8 = W7*(x4+x5) + 4;

 x4 = (x8+(W1-W7)*x4)>>3;

 x5 = (x8-(W1+W7)*x5)>>3;

 x8 = W3*(x6+x7) + 4;

 x6 = (x8-(W3-W5)*x6)>>3;

 x7 = (x8-(W3+W5)*x7)>>3;

 /* second stage */

 x8 = x0 + x1;

 x0 -= x1;

 x1 = W6*(x3+x2) + 4;

 x2 = (x1-(W2+W6)*x2)>>3;

 x3 = (x1+(W2-W6)*x3)>>3;

 x1 = x4 + x6;

 x4 -= x6;

 x6 = x5 + x7;

 x5 -= x7;

 /* third stage */

 x7 = x8 + x3;

 x8 -= x3;

 x3 = x0 + x2;

 x0 -= x2;

 x2 = (181*(x4+x5)+128)>>8;

 x4 = (181*(x4-x5)+128)>>8;

 /* fourth stage */

 blk[8*0] = iclp[(x7+x1)>>14];

 blk[8*1] = iclp[(x3+x2)>>14];

 blk[8*2] = iclp[(x0+x4)>>14];

 blk[8*3] = iclp[(x8+x6)>>14];

 blk[8*4] = iclp[(x8-x6)>>14];

 blk[8*5] = iclp[(x0-x4)>>14];

 blk[8*6] = iclp[(x3-x2)>>14];

 blk[8*7] = iclp[(x7-x1)>>14];

}

