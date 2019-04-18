# JPEG编解码过程详解 - gauss的专栏 - CSDN博客
2013年01月06日 22:56:44[gauss](https://me.csdn.net/mathlmx)阅读数：492
个人分类：[图形与图像处理](https://blog.csdn.net/mathlmx/article/category/605907)
（转载 作者未知）  
JPEG（Joint Photographic Experts Group）是联合图像专家小组的英文缩写。它由国际电话与电报咨询委员会CCITT（The International Telegraph and Telephone Consultative Committee）与国际标准化组织ISO于1986年联合成立的一个小组，负责制定静态数字图像的编码标准。
       小组一直致力于标准化工作，开发研制出连续色调、多级灰度、静止图像的数字图像压缩编码方法，即JPEG算法。JPEG算法被确定为国际通用标准，其适用范围广泛，除用于静态图像编码外，还推广到电视图像序列的帧内图像压缩。而用JPEG算法压缩出来的静态图片文件称为JPEG文件，扩展名通常为*.jpg、*.jpe*.jpeg。
一.JPEG编码器和解码器的基本系统结构。
1.1 JPEG文件格式简介
    JPEG文件使用的数据存储方式有多种。最常用的格式称为JPEG文件交换格式（JPEG File Interchange Format，JFIF）。而JPEG文件大体上可以分成两个部分：标记码(Tag)和压缩数据。标记码由两个字节构成，其前一个字节是固定值0xFF，后一个字节则根据不同意义有不同数值。在每个标记码之前还可以添加数目不限的无意义的0xFF填充，也就说连续的多个0xFF可以被理解为一个0xFF，并表示一个标记码的开始。而在一个完整的两字节的标记码后，就是该标记码对应的压缩数据流，记录了关于文件的诸种信息。
      常用的标记有[SOI](http://blog.csdn.net/crfoxzl/archive/2007/11/03/fckeditor.html?InstanceName=ctl00_ContentPlaceHolder1_EntryEditor1_richTextEditor_richTextEditor&Toolbar=Default#SOI)、[APP0](http://blog.csdn.net/crfoxzl/archive/2007/11/03/fckeditor.html?InstanceName=ctl00_ContentPlaceHolder1_EntryEditor1_richTextEditor_richTextEditor&Toolbar=Default#APP0)、[DQT](http://blog.csdn.net/crfoxzl/archive/2007/11/03/fckeditor.html?InstanceName=ctl00_ContentPlaceHolder1_EntryEditor1_richTextEditor_richTextEditor&Toolbar=Default#DQT)、[SOF0](http://blog.csdn.net/crfoxzl/archive/2007/11/03/fckeditor.html?InstanceName=ctl00_ContentPlaceHolder1_EntryEditor1_richTextEditor_richTextEditor&Toolbar=Default#SOF)、[DHT](http://blog.csdn.net/crfoxzl/archive/2007/11/03/fckeditor.html?InstanceName=ctl00_ContentPlaceHolder1_EntryEditor1_richTextEditor_richTextEditor&Toolbar=Default#DHT)、[DRI](http://blog.csdn.net/crfoxzl/archive/2007/11/03/fckeditor.html?InstanceName=ctl00_ContentPlaceHolder1_EntryEditor1_richTextEditor_richTextEditor&Toolbar=Default#DRI)、[SOS](http://blog.csdn.net/crfoxzl/archive/2007/11/03/fckeditor.html?InstanceName=ctl00_ContentPlaceHolder1_EntryEditor1_richTextEditor_richTextEditor&Toolbar=Default#SOS)、[EOI](http://blog.csdn.net/crfoxzl/archive/2007/11/03/fckeditor.html?InstanceName=ctl00_ContentPlaceHolder1_EntryEditor1_richTextEditor_richTextEditor&Toolbar=Default#EOI)。注意，SOI等都是标记的名称。在文件中，标记码是以标记代码形式出现。例如SOI的标记代码为0xFFD8，即在JPEG文件中的如果出现数据0xFFD8，则表示此处为一个SOI标记。
1.2 JPEG编解码基本过程
![JPEG编解码过程详解（转载 作者未知） - tomjark - 蹒跚学步](http://img.blog.163.com/photo/LCzYyfJ8FczpqESBnBIBvQ==/5730830525829365476.jpg)
JPEG基本系统结构
二 ，JPEG编码过程。
    2.1  RGB格式转换为YUV格式
RGB介绍：
       在记录计算机图像时，最常见的是采用RGB(红、绿，蓝)颜色分量来保存颜色信息，例如非压缩的24位的BMP图像就采用RGB空间来保存图像。一个像素24位，每8位保存一种颜色强度(0-255)，例如红色保存为 0xFF0000。
YUV介绍：
       YUV是被欧洲电视系统所采用的一种颜色编码方法，我国广播电视也普遍采用这类方法。其中“Y”表示明亮度（Luminance或Luma），也就是灰阶值；而“U”和“V”表示的则是色度（Chrominance或Chroma）。彩色电视采用YUV空间正是为了用亮度信号Y解决彩色电视机与黑白电视机的兼容问题，使黑白电视机也能接收彩色电视信号。
YUV与RGB相互转换的公式如下（RGB取值均为0-255）：
    Y = 0.299R + 0.587G + 0.114B
　　U = -0.147R - 0.289G + 0.436B
　　V = 0.615R - 0.515G - 0.100B
　　R = Y + 1.14V
　　G = Y - 0.39U - 0.58V
　　B = Y + 2.03U
2.2 将图像8*8分块
     将原始图像转换为YUV格式后，对图像按一定的采样格式进行采样，常见的格式有4：4：4，4：2：2和4：2：0。
     取样完成后，将图像按8*8（pixel）划分成MCU。 
2.3  离散余弦变换（DCT）
    离散余弦变换DCT（Discrete Cosine Transform）是数码率压缩需要常用的一个变换编码方法。任何连续的实对称函数的付立叶变换中只含余弦项，因此余弦变换与付立叶变换一样有明确的物理意义。DCT是先将整体图像分成N*N像素块，然后对N*N像素块逐一进行DCT变换。由于大多数图像的高频分量较小，相应于图像高频分量的系数经常为零，加上人眼对高频成分的失真不太敏感，所以可用更粗的量化。
    因此，传送变换系数的数码率要大大小于传送图像像素所用的数码率。到达接收端后通过反离散余弦变换回到样值，虽然会有一定的失真，但人眼是可以接受的。二维正反离散余弦变换的算式：
![JPEG编解码过程详解（一）](http://img.blog.163.com/photo/JHRoSXa4OvDVId-onKrbMQ==/5730830525829365477.jpg)
![JPEG编解码过程详解（一）](http://img.blog.163.com/photo/d3_cx5H0yK2bGP-zBvc2zA==/5730830525829365478.jpg)
    其中N是像块的水平、垂直像素数，一般取N=8。N大于8时效率增加不多而复杂性大为增加。8*8的二维数据块经DCT后变成8*8个变换系数，这些系数都有明确的物理意义。譬如当U=0，V=0时F(0，0)是原64个样值的平均，相当于直流分量，随着U，V值增加，相应系数分别代表逐步增加的水平空间频率和垂直空间频率分量的大小。当我们先只考虑水平方向上一行数据（8个像素）的情况时，如图所示：
![JPEG编解码过程详解（一）](http://img.blog.163.com/photo/Q2twt4ID2Q8uWyjKn40u1A==/5730830525829365479.jpg)
    可见图像信号被分解成为直流成分；以及从低频到高频的各种余弦成分；而DCT系数只是表示了该种成分所占原图像信号的份额大小；显然，恢复图像信息可以表示为这样一个矩阵形式：F(n)=C(n)*E(n)
　　式中E(n)是一个基底 ，C(n)是DCT系数，F(n)则是图像信号。
　　如果再考虑垂直方向上的变化，那么，就需要一个二维的基底，即该基底不仅要反映水平方向频率的变化；而且要反映垂直空间频率的变化；对应于8*8的像素块；其空间基底如图2所示：它是由64个像素值所组成的图像，通常也称之为基本图像。把它们称为基本图像是因为在离散余弦变换的反变换式中，任何像块都可以表示成64个系数的不同大小的组合。既然基本图像相当于变换域中的单一的系数，那么任何像元也可以看成由64个不同幅度的基本图像的组合。这与任何信号可以分解成基波和不同幅度的谐波的组合具有相同的物理意义。
2.4  量化(quantization)
    量化过程是一个将信号的幅度离散化的过程，离散信号经过量化后变为数字信号。
    由于HVS对低频信号更为敏感，所以对信号的低频部分采用相对短的量化步长，对信号的高频部分采用相对长的量化步长。这样可以在一定程度上，得到相对清晰的图像和更高的压缩率。
2.5  Z字形编码(zigzag scan)
          按Z字形把量化后的数据读出，例：
![JPEG编解码过程详解（二）](http://img.blog.163.com/photo/cmuouJjjE4wywXU67X-9AQ==/5730830525829365480.jpg)
2.6  使用行程长度编码(RLE)对交流系数(AC)进行编码
    所谓游程长度编码是指一个码可以同时表示码的值和前面有几个零。这样就发挥了Z字型读出的优点，因为Z字型读出，出现连零的机会比较多，特别到最后，如果都是零，在读到最后一个数后，只要给出“块结束”（EOB）码，就可以结束输出，因此节省了很多码率。
    例：图中按Z字形抽取和游程编码得到码值为
![JPEG编解码过程详解（二）](http://img.blog.163.com/photo/36UQi7qTsc5r8LisFSUw4w==/5730830525829365481.jpg)
                         (0,1,0)(1,2,0)(0,5,0)(0,4,0)(4,8,1)EOB
                   这样一个4*4的矩阵用很少的数值就能表示！  
2.7  熵编码
    常用的熵编码有变长编码，即哈夫曼编码。
    哈夫曼的编码方法：对出现概率大的符号分配短字长的二进制码，对出现概率小的符号分配长字长的二进制码，得到符号平均码长最短的码。
    哈夫曼编码的步骤：(1). 把信源符号按概率大小顺序排列， 并设法按逆次序分配码字的长度。 (2). 在分配码字长度时，首先将出现概率最小的两个符号的概率相加合成一个概率 (3). 把这个合成概率看成是一个新组合符号地概率，重复上述做法直到最后只剩下两个符号概率为止。 (4). 完成以上概率顺序排列后，再反过来逐步向前进行编码，每一次有二个分支各赋予一个二进制码，可以对概率大的赋为零，概率小的赋为1。
关于AC/DC系数的编码
1.AC系数的Huffman编码
     经过Z扫描和游程编码后的非零AC系数被表述为符号A和符号B。符号A由（Runlength，Size）构成，符号B（Amplitude）。
    Runlength为非零AC系数前连续为0的AC系数；
    Size则是表示编码Amplitude所需要的比特位数；
    Amplitude为AC系数的幅值。
    实际操作时，JPEG用一个8位的值RS来表示符号A，RS=RRRRSSSS，对于一个非零的AC系数，高四位用来表示Runlength，低四位用来表示Size。（00000000）用来表示EOB。
    对符号B进行变字长整数（VLI）编码，将符号B的VLI码放在A后从而形成对A,B编码的最终结果。
2.DC系数的Huffman编码
      对于DC系数，与非零的AC系数类似，它将相邻两块DC系数的差值（DIFF）描述如下的符号对：符号A为（Size）,符号B为（Amplitude）。
     Size表示为编码Amplitude所需要的位数；
     Amplitude表示DC系数的幅值。
     在JPEG标准中，对符号A根据相应的Huffman表进行变字长编码，对符号B进行变字长整数编码，而后将符号B 的VLI码放在符号A的Huffman码后，从而完成了对DIFF的编码。
     在JPEG标准中没有定义缺省的Huffman表，用户可以根据实际应用自由选择，可以预先定义一个通用的Huffman表，也可以针对一幅特定的图像，在压缩编码前通过搜集其统计特性来计算Huffman表。
三，JPEG解码的主要过程。
   3.1  读入文件的相关信息
            按照JPEG文件数据存储方式，把要解码的文件的相关信息一一读出，为接下来的解码工作做好准备。参考方法是，设计一系列的结构体对应各个标记，并存储标记内表示的信息。其中图像长宽、多个量化表和哈夫曼表、水平/垂直采样因子等多项信息比较重要。以下给出读取过程中的几个问题。
     1. 读取文件的大体结构
              JFIF格式的JPEG文件(*.jpg)的一般顺序为：
              SOI(0xFFD8),APP0(0xFFE0),[APPn(0xFFEn)]可选,
              DQT(0xFFDB),SOF0(0xFFC0),DHT(0xFFC4),SOS(0xFFDA),
              压缩数据,EOI(0xFFD9)。
     2. 读取哈夫曼表数据；
     3. 建立哈夫曼树。
     在准备好所有的图片信息后，就可以对图片数据进行解码了。
关于AC,DC系数的解码
 1. AC系数的解码
    通过查询Huffman数据解出RS，从中的到Runlength和Size的值。因为符号B是通过VLI表来编码的，所以通过查询Size的值可以得到Amplitude。这样就可以解出符号A和符号B的值了。
 2. DC系数的解码
    同理，先查询Huffman表解出Size的，通过Size解出DIFF，将其与上一个8*8块的DC系数数值相加，最终得到该块的DC系数。
3.2 MCU中颜色分量(Y,U,V)的解码
    图像数据流是有MCU组成，而MCU是用数据单元和颜色分量构成。图像数据流是以位（bit）为单位存储信息的。并且内部的数据都是在编码时通过正向离散余弦变换（FDCT）进行时空域向频率域变换而得到的结果，所以对于每个颜色分量单元都应该由两部分组成：1个直流分量和63个交流分量。
    颜色分量单元内部综合运用了RLE行程编码和哈夫曼编码来压缩数据。每个像素的数据流由两部分构成：编码和数值，并且两者基本以互相隔开方式出现（除非该编码的权值为零）。解码的过程其实就是哈夫曼树的查找过程。
3.3 直流系数的差分编码
    把所有的颜色分量单元按颜色分量（Y、Cr、Cb）分类。每一种颜色分量内，相邻的两个颜色分量单元的直流变量是以差分来编码的。也就是说，通过之前解码出来的直流变量数值只是当前颜色分量单元的实际直流变量减去前一个颜色分量单元的实际直流变量。也就是说，当前直流变量要通过前一个颜色分量单元的实际（非解码）直流分量来校正：
                                       DCn=DCn-1+Diff
其中Diff为差分校正变量，也就是直接解码出来的直流系数。但如果当前颜色分量单元是第一个单元，则解码出来的直流数值就是真正的直流变量。
              3个颜色分量的直流变量是分开进行差分编码的。也就是说，为1张图片解码时应设置3个独立的直流校正变量。
3.4 反量化
    反量化的过程比较简单。只需要对8*8的颜色分量单元的64个值逐一乘以对应的量化表内位置相同的值则可。图像内全部的颜色分量单元都要进行反量化。
3.5 反Zig-zag编码
3.6 反离散余弦变换
![JPEG编解码过程详解（二）](http://img.blog.163.com/photo/FpKmWTVy2GEL0oVsoAdvZg==/5730830525829365482.jpg)
    之前提到，文件中的数据是在编码时通过正向离散余弦变换（FDCT）进行时空域向频率域变换而得到的结果，所以现在解码就必须将其反向离散余弦变换（IDCT），就是把颜色分量单元矩阵中的频率域数值向时空域转换。并且，原来的频率域的矩阵大小为8*8，则经过反向离散余弦变换后，时空域的矩阵仍然是8*8。
3.7 YCrCb向RGB转换
    要在屏幕上显示图像，就必须以RGB模式表示图像的颜色。所以，解码时需要把YCrCb模式向RGB模式转换。
    另外，由于离散余弦变化要求定义域的对称，所以在编码时把RGB的数值范围从[0，255]统一减去128偏移成[-128，127]。因此解码时必须为每个分量加上128。具体公式如下：
                 R=       Y        +1.402*Cb     +128;
                 G=Y-0.34414*Cr    -0.71414*Cb   +128;
                 B=       Y        +1.772*Cb     +128;
    还有一个问题，通过变换得出的R、G、B值可能超出了其定义域，所以要作出检查。如果大于255，则截断为255；如果小于0，则截断为0。 
    至此，每个MCU的解码已经完成。
