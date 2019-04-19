# 极详细的ECC讲解 -OOB与ECC - maopig的专栏 - CSDN博客
2012年12月04日 23:38:15[maopig](https://me.csdn.net/maopig)阅读数：14175
[http://blog.csdn.net/dongzhichen/article/details/8249228](http://blog.csdn.net/dongzhichen/article/details/8249228)
详细的ECC讲解 -OOB与ECC
在网络编程中
OOB（out of band）带外数据
在MTD设备中
OOB  如下所示：
[http://www.cnblogs.com/bcxx_qin/archive/2009/06/11/1501271.html](http://www.cnblogs.com/bcxx_qin/archive/2009/06/11/1501271.html)
# [极详细的ECC讲解](http://www.cnblogs.com/bcxx_qin/archive/2009/06/11/1501271.html)
ECC的全称是Error Checking and Correction，是一种用于Nand的差错检测和修正算法。如果操作时序和电路稳定性不存在问题的话，NAND
 Flash出错的时候一般不会造成整个Block或是Page不能读取或是全部出错，而是整个Page（例如512Bytes）中只有一个或几个bit出错。ECC能纠正1个比特错误和检测2个比特错误，而且计算速度很快，但对1比特以上的错误无法纠正，对2比特以上的错误不保证能检测。
校验码生成算法：ECC校验每次对256字节的数据进行操作，包含列校验和行校验。对每个待校验的Bit位求异或，若结果为0，则表明含有偶数个1；若结果为1，则表明含有奇数个1。列校验规则如表1所示。256字节数据形成256行、8列的矩阵，矩阵每个元素表示一个Bit位。
![](http://blog.chinaunix.net/photo/7459_090606223939.png)
其中CP0 ~ CP5 为六个Bit位，表示Column Parity（列极性），
CP0为第0、2、4、6列的极性，CP1为第1、3、5、7列的极性，
CP2为第0、1、4、5列的极性，CP3为第2、3、6、7列的极性，
CP4为第0、1、2、3列的极性，CP5为第4、5、6、7列的极性。
用公式表示就是：CP0=Bit0^Bit2^Bit4^Bit6， 表示第0列内部256个Bit位异或之后再跟第2列256个Bit位异或，再跟第4列、第6列的每个Bit位异或，这样，CP0其实是256*4=1024个Bit位异或的结果。CP1
 ~ CP5 依此类推。
行校验如下图所示
![](http://blog.chinaunix.net/photo/7459_090606223947.png)
其中RP0 ~ RP15 为十六个Bit位，表示Row Parity（行极性），
RP0为第0、2、4、6、….252、254个字节的极性
RP1-----1、3、5、7……253、255
RP2----0、1、4、5、8、9…..252、253（处理2个Byte，跳过2个Byte）
RP3---- 2、3、6、7、10、11…..254、255（跳过2个Byte，处理2个Byte）
RP4---- 处理4个Byte，跳过4个Byte；
RP5---- 跳过4个Byte，处理4个Byte；
RP6---- 处理8个Byte，跳过8个Byte
RP7---- 跳过8个Byte，处理8个Byte；
RP8---- 处理16个Byte，跳过16个Byte
RP9---- 跳过16个Byte，处理16个Byte；
RP10----处理32个Byte，跳过32个Byte
RP11----跳过32个Byte，处理32个Byte；
RP12----处理64个Byte，跳过64个Byte
RP13----跳过64个Byte，处理64个Byte；
RP14----处理128个Byte，跳过128个Byte
RP15----跳过128个Byte，处理128个Byte；
可见，RP0 ~ RP15 每个Bit位都是128个字节（也就是128行）即128*8=1024个Bit位求异或的结果。
综上所述，对256字节的数据共生成了6个Bit的列校验结果，16个Bit的行校验结果，共22个Bit。在Nand中使用3个字节存放校验结果，多余的两个Bit位置1。存放次序如下表所示：
![](http://blog.chinaunix.net/photo/7459_090606223952.png)
以K9F1208为例，每个Page页包含512字节的数据区和16字节的OOB区。前256字节数据生成3字节ECC校验码，后256字节数据生成3字节ECC校验码，共6字节ECC校验码存放在OOB区中，存放的位置为OOB区的第0、1、2和3、6、7字节。
**校验码生成算法的C语言实现**
**在Linux内核中ECC校验算法所在的文件为drivers/mtd/nand/nand_ecc.c，其实现有新、旧两种，在2.6.27及更早的内核中使用的程序，从2.6.28开始已经不再使用，而换成了效率更高的程序。可以在Documentation/mtd/nand_ecc.txt 文件中找到对新程序的详细介绍。**
首先分析一下2.6.27内核中的ECC实现，源代码见:
http://lxr.linux.no/linux+v2.6.27/drivers/mtd/nand/nand_ecc.c
[43](http://control.cublog.cn/article_change.php?id=1957543#L43)/*
[44](http://control.cublog.cn/article_change.php?id=1957543#L44) * Pre-calculated 256-way 1 byte column parity
[45](http://control.cublog.cn/article_change.php?id=1957543#L45) */
[46](http://control.cublog.cn/article_change.php?id=1957543#L46)static const[u_char](http://lxr.linux.no/linux+v2.6.27/+co%3Cwbr%3Ede=u_char)
[nand_ecc_precalc_table](http://lxr.linux.no/linux+v2.6.27/+co%3Cwbr%3Ede=nand_ecc_precalc_table)[] = {
[47](http://control.cublog.cn/article_change.php?id=1957543#L47)   0x00, 0x55, 0x56, 0x03, 0x59, 0x0c, 0x0f, 0x5a, 0x5a, 0x0f, 0x0c,
 0x59, 0x03, 0x56, 0x55, 0x00,
[48](http://control.cublog.cn/article_change.php?id=1957543#L48)   0x65, 0x30, 0x33, 0x66, 0x3c, 0x69, 0x6a, 0x3f, 0x3f, 0x6a, 0x69,
 0x3c, 0x66, 0x33, 0x30, 0x65,
[49](http://control.cublog.cn/article_change.php?id=1957543#L49)   0x66, 0x33, 0x30, 0x65, 0x3f, 0x6a, 0x69, 0x3c, 0x3c, 0x69, 0x6a,
 0x3f, 0x65, 0x30, 0x33, 0x66,
[50](http://control.cublog.cn/article_change.php?id=1957543#L50)   0x03, 0x56, 0x55, 0x00, 0x5a, 0x0f, 0x0c, 0x59, 0x59, 0x0c, 0x0f,
 0x5a, 0x00, 0x55, 0x56, 0x03,
[51](http://control.cublog.cn/article_change.php?id=1957543#L51)   0x69, 0x3c, 0x3f, 0x6a, 0x30, 0x65, 0x66, 0x33, 0x33, 0x66, 0x65,
 0x30, 0x6a, 0x3f, 0x3c, 0x69,
[52](http://control.cublog.cn/article_change.php?id=1957543#L52)   0x0c, 0x59, 0x5a, 0x0f, 0x55, 0x00, 0x03, 0x56, 0x56, 0x03, 0x00,
 0x55, 0x0f, 0x5a, 0x59, 0x0c,
[53](http://control.cublog.cn/article_change.php?id=1957543#L53)   0x0f, 0x5a, 0x59, 0x0c, 0x56, 0x03, 0x00, 0x55, 0x55, 0x00, 0x03,
 0x56, 0x0c, 0x59, 0x5a, 0x0f,
[54](http://control.cublog.cn/article_change.php?id=1957543#L54)   0x6a, 0x3f, 0x3c, 0x69, 0x33, 0x66, 0x65, 0x30, 0x30, 0x65, 0x66,
 0x33, 0x69, 0x3c, 0x3f, 0x6a,
[55](http://control.cublog.cn/article_change.php?id=1957543#L55)   0x6a, 0x3f, 0x3c, 0x69, 0x33, 0x66, 0x65, 0x30, 0x30, 0x65, 0x66,
 0x33, 0x69, 0x3c, 0x3f, 0x6a,
[56](http://control.cublog.cn/article_change.php?id=1957543#L56)   0x0f, 0x5a, 0x59, 0x0c, 0x56, 0x03, 0x00, 0x55, 0x55, 0x00, 0x03,
 0x56, 0x0c, 0x59, 0x5a, 0x0f,
[57](http://control.cublog.cn/article_change.php?id=1957543#L57)   0x0c, 0x59, 0x5a, 0x0f, 0x55, 0x00, 0x03, 0x56, 0x56, 0x03, 0x00,
 0x55, 0x0f, 0x5a, 0x59, 0x0c,
[58](http://control.cublog.cn/article_change.php?id=1957543#L58)   0x69, 0x3c, 0x3f, 0x6a, 0x30, 0x65, 0x66, 0x33, 0x33, 0x66, 0x65,
 0x30, 0x6a, 0x3f, 0x3c, 0x69,
[59](http://control.cublog.cn/article_change.php?id=1957543#L59)   0x03, 0x56, 0x55, 0x00, 0x5a, 0x0f, 0x0c, 0x59, 0x59, 0x0c, 0x0f,
 0x5a, 0x00, 0x55, 0x56, 0x03,
[60](http://control.cublog.cn/article_change.php?id=1957543#L60)   0x66, 0x33, 0x30, 0x65, 0x3f, 0x6a, 0x69, 0x3c, 0x3c, 0x69, 0x6a,
 0x3f, 0x65, 0x30, 0x33, 0x66,
[61](http://control.cublog.cn/article_change.php?id=1957543#L61)   0x65, 0x30, 0x33, 0x66, 0x3c, 0x69, 0x6a, 0x3f, 0x3f, 0x6a, 0x69,
 0x3c, 0x66, 0x33, 0x30, 0x65,
[62](http://control.cublog.cn/article_change.php?id=1957543#L62)
0x00, 0x55, 0x56, 0x03, 0x59, 0x0c, 0x0f, 0x5a, 0x5a, 0x0f, 0x0c, 0x59, 0x03, 0x56, 0x55, 0x00
[63](http://control.cublog.cn/article_change.php?id=1957543#L63)};
为了加快计算速度，程序中使用了一个预先计算好的列极性表。这个表中每一个元素都是unsigned char类型，表示8位二进制数。
表中8位二进制数每位的含义：
![](http://blog.chinaunix.net/photo/7459_090606223957.png)
这个表的意思是：对0~255这256个数，计算并存储每个数的列校验值和行校验值，以数作数组下标。比如nand_ecc_precalc_table[13
 ]  存储13的列校验值和行校验值，13的二进制表示为00001101，
 其CP0 =Bit0^Bit2^Bit4^Bit6 = 0；
CP1 = Bit1^Bit3^Bit5^Bit7 = 1；
CP2 = Bit0^Bit1^Bit4^Bit5 = 1;
CP3 = Bit2^Bit3^Bit6^Bit7 = 0;
CP4 = Bit0^Bit1^Bit2^Bit3 = 1;
CP5 = Bit4^Bit5^Bit6^Bit7 = 0;
其行极性RP = Bit0^Bit1^Bit2^Bit3^Bit4^Bit5^Bit6^Bit7
 = 1；
则nand_ecc_precalc_table[13 ]处存储的值应该是
 0101 0110，即0x56.
注意，数组nand_ecc_precalc_table的下标其实是我们要校验的一个字节数据。
理解了这个表的含义，也就很容易写个程序生成这个表了。程序见附件中的 MakeEccTable.c文件。
有了这个表，对单字节数据dat，可以直接查表 nand_ecc_precalc_table[ dat ]得到dat的行校验值和列校验值。
 但是ECC实际要校验的是256字节的数据，需要进行256次查表，对得到的256个查表结果进行按位异或，最终结果的
 Bit0 ~ Bit5 即是256字节数据的CP0 ~ CP5.
/* Build up column parity */
[81](http://control.cublog.cn/article_change.php?id=1957543#L81)        for([i](http://lxr.linux.no/linux+v2.6.27/+co%3Cwbr%3Ede=i)
 = 0; [i](http://lxr.linux.no/linux+v2.6.27/+co%3Cwbr%3Ede=i) < 256;[i](http://lxr.linux.no/linux+v2.6.27/+co%3Cwbr%3Ede=i)++)
 {
[82](http://control.cublog.cn/article_change.php?id=1957543#L82)
/* Get CP0 - CP5 from table */
[83](http://control.cublog.cn/article_change.php?id=1957543#L83)
[idx](http://lxr.linux.no/linux+v2.6.27/+co%3Cwbr%3Ede=idx) =[nand_ecc_precalc_table](http://lxr.linux.no/linux+v2.6.27/+co%3Cwbr%3Ede=nand_ecc_precalc_table)[*[dat](http://lxr.linux.no/linux+v2.6.27/+co%3Cwbr%3Ede=dat)++];
[84](http://control.cublog.cn/article_change.php?id=1957543#L84)
[reg1](http://lxr.linux.no/linux+v2.6.27/+co%3Cwbr%3Ede=reg1) ^= ([idx](http://lxr.linux.no/linux+v2.6.27/+co%3Cwbr%3Ede=idx)
 & 0x3f);
[85](http://control.cublog.cn/article_change.php?id=1957543#L85)
[86](http://control.cublog.cn/article_change.php?id=1957543#L86)            //这里省略了一些，后面会介绍
[91](http://control.cublog.cn/article_change.php?id=1957543#L91)        }
Reg1
![](http://blog.chinaunix.net/photo/7459_090606224004.png)
在这里，计算列极性的过程其实是先在一个字节数据的内部计算CP0 ~ CP5,每个字节都计算完后再与其它字节的计算结果求异或。而表1中是先对一列Bit0求异或，再去异或一列Bit2。
 这两种只是计算顺序不同，结果是一致的。 因为异或运算的顺序是可交换的。
行极性的计算要复杂一些。
nand_ecc_precalc_table[]表中的Bit6已经保存了每个单字节数的行极性值。对于待校验的256字节数据，分别查表，如果其行极性为1，则记录该数据所在的行索引（也就是for循环的i值），这里的行索引是很重要的，因为RP0
 ~ RP15 的计算都是跟行索引紧密相关的，如RP0只计算偶数行，RP1只计算奇数行，等等。
/* Build up column parity */
[81](http://control.cublog.cn/article_change.php?id=1957543#L81)        for([i](http://lxr.linux.no/linux+v2.6.27/+co%3Cwbr%3Ede=i)
 = 0; [i](http://lxr.linux.no/linux+v2.6.27/+co%3Cwbr%3Ede=i) < 256;[i](http://lxr.linux.no/linux+v2.6.27/+co%3Cwbr%3Ede=i)++)
 {
[82](http://control.cublog.cn/article_change.php?id=1957543#L82)
/* Get CP0 - CP5 from table */
[83](http://control.cublog.cn/article_change.php?id=1957543#L83)
[idx](http://lxr.linux.no/linux+v2.6.27/+co%3Cwbr%3Ede=idx) =[nand_ecc_precalc_table](http://lxr.linux.no/linux+v2.6.27/+co%3Cwbr%3Ede=nand_ecc_precalc_table)[*[dat](http://lxr.linux.no/linux+v2.6.27/+co%3Cwbr%3Ede=dat)++];
[84](http://control.cublog.cn/article_change.php?id=1957543#L84)
[reg1](http://lxr.linux.no/linux+v2.6.27/+co%3Cwbr%3Ede=reg1) ^= ([idx](http://lxr.linux.no/linux+v2.6.27/+co%3Cwbr%3Ede=idx)
 & 0x3f);
[85](http://control.cublog.cn/article_change.php?id=1957543#L85)
[86](http://control.cublog.cn/article_change.php?id=1957543#L86)
/* All bit XOR = 1 ? */
[87](http://control.cublog.cn/article_change.php?id=1957543#L87)                if ([idx](http://lxr.linux.no/linux+v2.6.27/+co%3Cwbr%3Ede=idx)
 & 0x40) {
[88](http://control.cublog.cn/article_change.php?id=1957543#L88)
[reg3](http://lxr.linux.no/linux+v2.6.27/+co%3Cwbr%3Ede=reg3) ^= ([uint8_t](http://lxr.linux.no/linux+v2.6.27/+co%3Cwbr%3Ede=uint8_t))[i](http://lxr.linux.no/linux+v2.6.27/+co%3Cwbr%3Ede=i);
[89](http://control.cublog.cn/article_change.php?id=1957543#L89)
[reg2](http://lxr.linux.no/linux+v2.6.27/+co%3Cwbr%3Ede=reg2) ^= ~(([uint8_t](http://lxr.linux.no/linux+v2.6.27/+co%3Cwbr%3Ede=uint8_t))[i](http://lxr.linux.no/linux+v2.6.27/+co%3Cwbr%3Ede=i));
[90](http://control.cublog.cn/article_change.php?id=1957543#L90)                }
[91](http://control.cublog.cn/article_change.php?id=1957543#L91)        }
这里的关键是理解第88和89行。Reg3和reg2都是unsigned
 char 型的变量，并都初始化为零。
行索引（也就是for循环里的i）的取值范围为0~255，根据表2可以得出以下规律：
RP0只计算行索引的Bit0为0的行，RP1只计算行索引的Bit0为1的行；
RP2只计算行索引的Bit1为0的行，RP3只计算行索引的Bit1为1的行；
RP4只计算行索引的Bit2为0的行，RP5只计算行索引的Bit2为1的行；
RP6只计算行索引的Bit3为0的行，RP7只计算行索引的Bit3为1的行；
RP8只计算行索引的Bit4为0的行，RP9只计算行索引的Bit4为1的行；
RP10只计算行索引的Bit5为0的行，RP11只计算行索引的Bit5为1的行；
RP12只计算行索引的Bit6为0的行，RP13只计算行索引的Bit6为1的行；
RP14只计算行索引的Bit7为0的行，RP15只计算行索引的Bit7为1的行；
已经知道，异或运算的作用是判断比特位为1的个数，跟比特位为0的个数没有关系。如果有偶数个1则异或的结果为0，如果有奇数个1则异或的结果为1。
那么，程序第88行，对所有行校验为1的行索引按位异或运算，作用便是：
判断在所有行校验为1的行中，
属于RP1计算范围内的行有多少个------由reg3的Bit
 0指示，0表示有偶数个，1表示有奇数个；
属于RP3计算范围内的行有多少个------由reg3的Bit1指示，0表示有偶数个，1表示有奇数个；
属于RP5计算范围内的行有多少个------由reg3的Bit2指示，0表示有偶数个，1表示有奇数个；
属于RP7计算范围内的行有多少个------由reg3的Bit3指示，0表示有偶数个，1表示有奇数个；
属于RP9计算范围内的行有多少个------由reg3的Bit4指示，0表示有偶数个，1表示有奇数个；
属于RP11计算范围内的行有多少个------由reg3的Bit5指示，0表示有偶数个，1表示有奇数个；
属于RP13计算范围内的行有多少个------由reg3的Bit6指示，0表示有偶数个，1表示有奇数个；
属于RP15计算范围内的行有多少个------由reg3的Bit7指示，0表示有偶数个，1表示有奇数个；
所以，reg3每个Bit位的作用如下表所示：
Reg3
![](http://blog.chinaunix.net/photo/7459_090606224008.png)
第89行，对所有行校验为1的行索引按位取反之后，再按位异或，作用就是判断比特位为0的个数。比如reg2的Bit0为0表示：所有行校验为1的行中，行索引的Bit0为0的行有偶数个，也就是落在RP0计算范围内的行有偶数个。所以得到结论：
在所有行校验为1的行中，
属于RP0计算范围内的行有多少个------由reg2的Bit
 0指示，0表示有偶数个，1表示有奇数个；
属于RP2计算范围内的行有多少个------由reg2的Bit1指示，0表示有偶数个，1表示有奇数个；
属于RP4计算范围内的行有多少个------由reg2的Bit2指示，0表示有偶数个，1表示有奇数个；
属于RP6计算范围内的行有多少个------由reg2的Bit3指示，0表示有偶数个，1表示有奇数个；
属于RP8计算范围内的行有多少个------由reg2的Bit4指示，0表示有偶数个，1表示有奇数个；
属于RP10计算范围内的行有多少个------由reg2的Bit5指示，0表示有偶数个，1表示有奇数个；
属于RP12计算范围内的行有多少个------由reg2的Bit6指示，0表示有偶数个，1表示有奇数个；
属于RP14计算范围内的行有多少个------由reg2的Bit7指示，0表示有偶数个，1表示有奇数个；
所以，reg2每个Bit位的作用如下表所示：
Reg2
![](http://blog.chinaunix.net/photo/7459_090606224015.png)
至此，只用了一个查找表和一个for循环，就把所有的校验位CP0 ~ CP5和RP0
 ~ RP15全都计算出来了。下面的任务只是按照表3的格式，把这些比特位重新排列一下顺序而已。
从reg2和reg3中抽取出 RP8~RP15放在tmp1中，抽取出RP0~RP7放在tmp2中，
Reg1左移两位，低两位置1，
然后把tmp2, tmp1, reg1放在ECC码的三个字节中。
程序中还有[CONFIG_MTD_NAND_ECC_SMC](http://lxr.linux.no/linux+v2.6.27/+co%3Cwbr%3Ede=CONFIG_MTD_NAND_ECC_SMC)，
 又进行了一次取反操作，暂时还不知为何。
**ECC纠错算法**
当往NAND Flash的page中写入数据的时候，每256字节我们生成一个ECC校验和，称之为原ECC校验和，保存到PAGE的OOB（out-of-band）数据区中。当从NAND
 Flash中读取数据的时候，每256字节我们生成一个ECC校验和，称之为新ECC校验和。
将从OOB区中读出的原ECC校验和新ECC校验和按位异或，若结果为0，则表示不存在错（或是出现了ECC无法检测的错误）；若3个字节异或结果中存在11个比特位为1，表示存在一个比特错误，且可纠正；若3个字节异或结果中只存在1个比特位为1，表示OOB区出错；其他情况均表示出现了无法纠正的错误。
假设ecc_code_raw[3]保存原始的ECC校验码，ecc_code_new[3]保存新计算出的ECC校验码，其格式如下表所示：
![](http://blog.chinaunix.net/photo/7459_090606224019.png)
对ecc_code_raw[3] 和ecc_code_new[3]
 按位异或，得到的结果三个字节分别保存在s0,s1,s2中，如果s0s1s2中共有11个Bit位为1，则表示出现了一个比特位错误，可以修正。定位出错的比特位的方法是，先确定行地址（即哪个字节出错），再确定列地址（即该字节中的哪一个Bit位出错）。
确定行地址的方法是，设行地址为unsigned charbyteoffs，抽取s1中的Bit7,Bit5,Bit3,Bit1，作为byteoffs的高四位，
 抽取s0中的Bit7,Bit5,Bit3,Bit1 作为byteoffs的低四位，
 则byteoffs的值就表示出错字节的行地址（范围为0 ~ 255）。
确定列地址的方法是：抽取s2中的Bit7,Bit5,Bit3作为bitnum的低三位，bitnum其余位置0，则bitnum的表示出错Bit位的列地址
 （范围为0 ~ 7）。
下面以一个简单的例子探索一下这其中的奥妙。
假设待校验的数据为两个字节，0x45（二进制为0100 0101）和0x38（二进制为0011
 1000），其行列校验码如下表所示：
![](http://blog.chinaunix.net/photo/7459_090606224023.png)
从表中可以计算出CP5 ~ CP0的值，列在下表的第一行（原始数据）。假设现在有一个数据位发生变化，0x38变为0x3A，也就是Byte
1的Bit 1由0变成了1，计算得到新的CP5
 ~ CP0值放在下表第2行（变化后数据）。新旧校验码求异或的结果放在下表第三行。
可见，当 Bit
1发生变化时，列校验值中只有CP1，CP2，CP4发生了变化，而CP0，CP3，CP5没变化，也就是说6个Bit校验码有一半发生变化，则求异或的结果中有一半为1。同理，行校验求异或的结果也有一半为1。这就是为什么前面说256字节数据中的一个Bit位发生变化时，新旧22Bit校验码求异或的结果中会有11个Bit位为1。
![](http://blog.chinaunix.net/photo/7459_090606224028.png)
再来看怎么定位出错的Bit位。以列地址为例，若CP5发生变化（异或后的CP5=1），则出错处肯定在Bit
 4 ~ Bit 7中；若CP5无变化（异或后的CP5=0）,则出错处在Bit
 0 ~ Bit 3 中，这样就筛选掉了一半的Bit位。剩下的4个Bit位中，再看CP3是否发生变化，又选出2个Bit位。剩下的2Bit位中再看CP1是否发生变化，则最终可定位1个出错的Bit位。下面的树形结构更清晰地展示了这个判决过程：
![](http://blog.chinaunix.net/photo/7459_090606225103.png)
图表 1  出错Bit列地址定位的判决树
注意：图中的CP指的是求异或之后的结果中的CP
为什么只用CP4，CP2，CP0呢？其实这里面包含冗余信息，因为CP5=1则必有CP4=0，CP5=0则必有CP4=1，也就是CP5跟CP4一定相反，同理，CP3跟CP2一定相反，CP1跟CP0一定相反。所以只需要用一半就行了。
这样，我们从异或结果中抽取出CP5，CP3，CP1位，便可定位出错Bit位的列地址。比如上面的例子中CP5/CP3/CP1
 = 001，表示Bit 1出错。
同理，行校验RP1发生变化，抽取RP1，可知Byte
 1发生变化。这样定位出Byte 1的Bit 0出错。
当数据位256字节时，行校验使用RP0 ~ RP15，抽取异或结果的RP15，RP13，RP11，RP9，RP7，RP5，RP3，RP1位便可定位出哪个Byte出错，再用CP5,CP3,CP1定位哪个Bit出错。
[TestEcc.rar](http://linux.chinaunix.net/bbs/attachment.php?aid=231922)(2.12 KB) 
 用ECC定位出错Bit的实验程序  http://linux.chinaunix.net/bbs/attachment.php?aid=231922
[MakeEccTable.rar](http://linux.chinaunix.net/bbs/attachment.php?aid=231869)Make_Ecc_Table.c 代码  [http://linux.chinaunix.net/bbs/attachment.php?aid=231869](http://linux.chinaunix.net/bbs/attachment.php?aid=231869)
## [Nand Flash原理分析与编程](http://www.cnblogs.com/hnrainll/archive/2011/06/01/2065377.html)
NAND Flash 在嵌入式系统中的地位与PC机上的硬盘是类似的。用于保存系统运行所必需的操作系统，应用程序，用户数据，运行过程中产生的各类数据，系统掉电后数据不会护丢失.本文主要介绍关于NAND Flash的组织结构和编写程序的方法。
  在三星的NAND Flash 中，当CPU从NAND Flash开始启动时，CPU会通过内部的硬件将NAND Flash开始的4KB数据复制到称为“Steppingstone”的4KB的内部RAM中，起始地址为0，然后跳到地址0处开始执行。这也就是我们为什么可以把小于4KB的程序烧到NAND Flash中，可以运行，而当大于4KB时，却没有办法运行，必须借助于NAND
 Flash的读操作，读取4KB以后的程序到内存中。 
NAND Flash的寻址方式和NAND Flash的memory组织方式紧密相关。NAND Flash的数据是以bit的方式保存在 memory cell（存储单元）。一般情况下，一个cell中只能存储一个bit。这些cell以8个或者16个为单位，连成 bit line ,形成所谓的byte(x8)/word(x16),这就是NAND Flash的位宽。
这些Line会再组成Pape（页）。然后是每32个page形成一个Block，所以一个Block（块）大小是16k.Block是NAND Flash中最大的操作单元，其中的擦除操作是以Block为单位进行擦除的，而读写和编程是以page为单位进行操作的，并且读写之前必须进行flash的擦写。我们这里以三星K9F1208U0M的NAND
 Flash 为例，它的大小是64MB的。 
1block = 32page
1page = 512bytes(datafield) + 16bytes(oob) 
K9F1208U0B总共有4096 个Blocks，故我们可以知道这块flash的容量为4096 *(32 *528)= 69206016 Bytes = 66 MB
但事实上每个Page上的最后16Bytes是用于存贮检验码用的，并不能存放实际的数据，所以实际上我们可以操作的芯片容量为 
4096 *(32 *512) = 67108864 Bytes = 64 MB 
Nand Flash 物理结构图 
在NAND Flash中有8个I/O引脚（IO0—IO7）、5个全能信号（nWE ALE CLE nCE nRE）、一个引脚，1个写保护引脚。操作NAND Flash时，先传输命令，然后传输地址，最后读写数据。对于64MB的NAND Flash，需要一个26位的地址。只能8个I/O引脚充当地址、数据、命令的复用端口，所以每次传地址只能传8位。这样就需要4个地址序列。因此读写一次nand flash需要传送4次（A[7:0] A[16:9] A[24:17] A[25]）。64M的NAND Flash的地址范围为0x00000000—0x03FFFFFF。
一页有528个字节，而在前512B中存放着用户的数据。在后面的16字节中（OOB）中存放着执行命令后的状态信息。主要是ECC校验的标识。列地址A0-A7可以寻址的范围是256个字节，要寻址528字节的话，将一页分为了A.（1half array）B(2 half array) C(spare array)。A区0—255字节，B区 256-511 字节C区512—527字节。访问某页时必须选定特定的区。这可以使地址指针指向特定的区实现。
在NAND Flash 中存在三类地址，分别为Block Address 、Column Address Page Address.。 
Column Address 用来选择是在上半页寻址还是在下半页寻址A[0]—A[7].也就相当于页内的偏移地址。在进行擦除时不需要列地址，因为擦除是以块为单位擦除。32个Page需要5bit来表示。也就是A[13:9];也就是页在块内的相对地址。A8这一位用来设置512字节的上半页，还是下半页，1表示是在上半页，而2表示是在下半页。Block的地址有A[25:14]组成。
存储操作特点： 
1.擦除操作的最小单位是块 
2.Nand Flash芯片每一位只能从1变为0，而不能从0变为1，所以在对其进行写入操作之前一定要将相应块擦除（擦除就是将相应块的位全部变为1
3 OOB部分的第六字节（即517字节）标志是否坏块，如果不是坏块该值为FF，否则为坏块
4 除OOB第六字节外，通常至少把OOB前3字节存放Nand Flash硬件ECC码
一个容量为64M(512Mbit)的NAND Flash,分为131072页，528列。（实际中由于存在spare area,故都大于这个值），有4096块，需要12bit来表示即A[25:14].如果是128M（1Gbit）的话，blodk Address为A[26:14].由于地址只能在IO0—IO7上传送。编程时通常通过移位来实现地址的传送。传送过程如下：
第1个地址序列：传递column address，也就是NAND Flash[7:0],这一周期不需要移位即可传递到I/O[7:0]上，而half page pointer 即A8是由操作指令决定，00h，在A区，01h在B区，指令决定在哪个half page上进行读写，而真正A8的值是不需要程序员关心的；
第2个地址序列：就是将NAND_ADDR 右移9位，而不是8位，将NAND_ADDR[16:9]传递到I/O[7:0]上； 
第3个地址序列：将NAND_ADDR[24:17] 传递到I/O[7:0]上； 
第4个地址序列：将NAND_ADDR[25]传送到I/O上。 
整个地址的传送过程需要4步才能完成。如果NAND Flash 的大小是32MB的以下的话，那么block address 最高位只到bit24,因此寻址只需要3步，就可以完成。
在进行擦除操作时由于是以块进行擦除，所以只需要3个地址序列，也就是只传递块的地址，即A[14:25]。 
NAND Flash地址的计算： 
Column Address 翻译过来是列地址，也就是在一页里的偏移地址。其实是指定Page上的某个Byte，指定这个Byte，其实也就是指定此页的读写起始地址。
Page Address:页地址。页的地址总是以512Bytes对齐的，所以它的低9位问题0，确定读写操作在NAND Flash中的哪个页进行。 

