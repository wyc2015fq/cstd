# SD卡FAT16文件系统的学习笔记 - xqhrs232的专栏 - CSDN博客
2013年11月02日 10:17:59[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：880
原文地址::[http://blog.chinaunix.net/uid-22935566-id-2602714.html](http://blog.chinaunix.net/uid-22935566-id-2602714.html)

相关文章
1、SD卡上读写FAT16文件系统成功----[http://hi.baidu.com/smallwormsea/item/5cf299fc519fb42c753c4cdc](http://hi.baidu.com/smallwormsea/item/5cf299fc519fb42c753c4cdc)
2、老外的SD读写开源库网址----[http://www.roland-riegel.de/sd-reader/index.html](http://www.roland-riegel.de/sd-reader/index.html)
3、WINAVR环境下一个相当不错的MMC/SD卡FAT16文件系统----[http://wapwenku.baidu.com/view/76e8b368a98271fe910ef9e7?bd_page_type=1&uid=wk_1336016707_90&pu=rc@1,pic@on,sl@2,pw@500,sz@176_208,pd@1,fz@2,lp@24,tpl@wml,&st=1&pos=rec](http://wapwenku.baidu.com/view/76e8b368a98271fe910ef9e7?bd_page_type=1&uid=wk_1336016707_90&pu=rc@1,pic@on,sl@2,pw@500,sz@176_208,pd@1,fz@2,lp@24,tpl@wml,&st=1&pos=rec)
一、      读文件的流程 
- 
读物理扇区0，得到引导扇（逻辑扇区0）的偏移地址。
- 
读引导扇的内容，得到文件系统基本配置信息。
- 
根据文件系统的基本配置信息计算FAT，FDT，数据簇的起始地址和大小。
- 
根据要读的文件名搜索FDT表，找到要读文件的起始数据簇编号，大小。
- 
根据文件的起始数据簇编号在FAT表中查找所有该文件占用的数据簇及数据簇访问的先后关系。
- 
读取该文件的起始数据簇的内容，及下一数据簇内容（有需要时）。
二、 读取物理扇区0 
先读取SD卡的第一个扇区（512字节），即扇区0，然后该扇区最后部分的数据如下图所示
![](http://fmn.rrfmn.com/fmn053/20110312/1715/b_large_Es0V_2f6200009f215c3f.jpg)
正常的话，该扇区最后两个字节如上图所示为55 AA，如果不是则证明是读错了，或者该SD没有被格式化。
该扇区有两个重要信息：
一、 在0x1ca开始的四个字节9f c9 03 00,即0x3c99f=248223，代表该SD卡友248223个扇区，因为每个扇区有512字节，所以该SD卡容量为248223*512/1024/1024=121.2MByte
二、在0x1c6开始的四个字节61 00 00 00，即0x61=97，它表示引导扇区在扇区97。那我们就接着读扇区97，获取SD卡里更详细的信息，这个扇区0就可以不用管了。
三、      读取引导扇区
以下是扇区97前64Byte的内容。因为97*512=0xc200，所以可以下图左边的偏移地址为c200
![](http://fmn.rrfmn.com/fmn046/20110312/1720/b_large_2kBs_0c960003cdd45c41.jpg)
首先第0x3到0xA的内容为ASCII码的“MSDOS5.0”，不是重要信息
第0xb开始的两个字节00 02,即0x2000=512，代表每个扇区(sector)有512个字节(byte)
接下来的04代表每个簇(cluster)有4个扇区
接下来的04 00 即0x4代表有4-1个保留扇区，即是第一个FAT表所在扇区为引导扇区(97)+4=扇区101
接下来的02 代表有两个FAT表
接下来的00 02，即0x2000=512，代表FDT（目录区)有512登记项
第0x16开始的两个字节f2 00，即0xf2=242，代表每个FAT表占242个扇区
第0x36到0x3d代表的就是“FAT16   ”的ASCII码，说明这SD卡是FAT16的格式
四、 FAT16文件系统的结构
知道以上的信息之后我们就可以根据以上信息计算出FAT1，FAT2，FDT和数据簇的首地址和结束地址，但在计算之前，我有必要介绍一下整个FAT16文件系统的结构和各个区的含义与作用。
![](http://fmn.rrfmn.com/fmn054/20110312/1720/b_large_7qLf_27d0000563575c44.jpg)
五、 获取文件系统基本配置信息
现在既然已经大概了解了引导扇，FAT1，FAT2，FDT和数据簇的作用，接下来就说一下怎么计算它们的起始地址和结束地址。我们用扇区来作为地址单位。
我们首先定义两个结构体
typedef struct
{
       U16 BytesPerSector;          //每个扇区多少字节
       U8  SectorsPerCluster;         //每个簇有多少个扇区
       U16 ReserveSectors;          //保留扇区数
       U8  FatTableNums;           //有多少个FAT表
       U16 RootDirRegNums;      //根目录允许的登记项数目
       U16 SectorsPerFat;                //每个FAT表有多少个扇区
       U32 SectorNums;               //总的扇区数
       U8   FileType[7];              //文件系统类型
}FAT_PARA;
typedef struct
{
       U32 Logic;  //引导扇（逻辑扇区0）对物理0扇区里的偏移地址
       U32 FAT1;
       U32 FAT2;
       U32 FDT;
       U32 Cluster;  //数据簇的偏移地址
}FAT_OFFSET;
FAT_PARA      SD_para;   //声明两个结构体变量
FAT_OFFSET   SD_offset;
由之前的知识可以知道，我们首先从物理扇区0知道SD_offset.Logic的地址，然后就可以找到引导扇.然后再在引导扇里找到SD_para里面所有变量的值。
U8  buffer[512];
Read_Single_Block(0, buffer)；//把物理扇区0的512个字节的数据读到buffer里
SD_offset.Logic = (* (U16 *) (buffer + 0x1c6)) + ((* (U16 *) (buffer + 0x1c8)) << 16); //得到引导扇的偏移地址
Read_Single_Block(SD_offset.Logic, buffer) ; //读引导扇的数据
//获取参数，以便计算各个区的偏移地址
SD_para.BytesPerSector = (* (U8 *) (buffer + 0xb)) + ((* (U8 *) (buffer + 0xc)) << 8);
SD_para.SectorsPerCluster = * (buffer + 0x0d);
SD_para.ReserveSectors = * (U16 *) (buffer + 0x0e);
SD_para.FatTableNums = * (buffer + 0x10);
SD_para.RootDirRegNums =  (* (U8 *) (buffer + 0x11)) + ((* (U8 *) (buffer + 0x12)) << 8);
SD_para.SectorsPerFat = * (U16 *) (buffer + 0x16);
SD_para.SectorNums = * (U32 *) (buffer + 0x20);
for(i = 0; i < 6; i++)
       SD_para.FileType[i] = *(buffer+0x36+i);
SD_para.FileType[6] = 0;
六、 计算各重要区域的大小与起始地址
//计算各个区的偏移地址
//FAT1地址=引导扇地址+保留扇区数，大小为SD_para.SectorsPerFat
SD_offset.FAT1 = SD_offset.Logic + SD_para.ReserveSectors; 
//如果存在两个FAT表，一般不是1就是2                                                                                 
if (SD_para.FatTableNums == 2)      
       //FAT2地址=FAT1地址+ SD_para.SectorsPerFat
       SD_offset.FAT2 = SD_offset.FAT1 + SD_para.SectorsPerFat;
else SD_offset.FAT2 = 0;
//FDT地址=FAT1+FAT表数*FAT表大小
SD_offset.FDT=SD_offset.FAT1+SD_para.FatTableNums*SD_para.SectorsPerFat;
//因为数据簇2紧跟在FDT后，所以数据簇0易求得 
SD_offset.Cluster = SD_offset.FDT + 32 - 2 * SD_para.SectorsPerCluster;     
七、 FDT与FAT表的简单介绍
读取文件之前要先详细了解一下FDT，和FAT表的内容 
一个FDT表占32个扇区，共有512个文件登记信息，所以每个文件登记信息的大小为32*512/512=32Byte
每个文件登记信息如下图所示![](http://fmn.rrfmn.com/fmn048/20110312/1725/b_large_PhaV_0c9400024cc55c41.jpg)
对于我们来说，这个文件记录信息最重要的就是最后六个字节
最后四个字节代表文件大小，由文件大小可以推算出该文件占用多少个数据簇
第0x1a到0x1b个字节道标文件开始的首簇号，知道文件的首簇号我们就可以查看FAT表的相应信息，就可得到该文件所占用的所有数据簇的簇号。
以下是FAT表的结构
![](http://fmn.rrfmn.com/fmn051/20110312/1725/b_large_wCBq_27c80000acd05c44.jpg)
上表中，06、07单元映射了磁盘3号簇区。有之前的介绍中可以知道，我这张SD卡1个簇包含4个扇区。也就是说在写数据时，只有写完了3号簇的4个扇后，将FAT表的06，07单元填写04，00；才可继续在04号簇上写数据。如果数据写完后还没有写满3号簇，则在FAT表的06，07单元填写FF，FF.
也就是说在FAT表中记录着每个数据簇的状态，且每个数据簇的状态占用两个字节。如果这两个字节等于0xffff，则代表该数据簇以被占用，且文件在该数据簇中结束。如果这两个字节等于0x0001~0xfffe，则代表该数据簇已被占用，且该文件没有结束，而该文件存放的下一数据簇的簇号就等于这两个字节的大小。
八、 读取一个文件
下面以我的SD卡为例子，向大家介绍读写SD的FAT文件系统的文件(最好先安装一个叫做winhex的软件)。
首先我的SD卡存放着这样一个文件
![](http://fmn.rrfmn.com/fmn048/20110312/1715/b_large_BIB9_06ff0003ddfb5c40.jpg)
要打开我这个名为lqz.txt的文件的，我们先查找FDT表中关于lqz.txt这个文件的登记信息。
因为之前已经知道了FDT的首地址是第585扇区，我们来到585扇区，开始搜索LQZ.TXT（必须先转换成大写字母）
最后在地址为0x4a310（也就是第0x4a310/512=593扇区）的地方搜索到LQZ.TXT的文件登记信息，在最后四个字节得知该文件大小为0x00002c89=11401Byte，占用11401/512/4=6个数据簇，从倒数第5，6个字节可以知道文件的首簇号为0x2fe4，然后在FAT表根据文件的首簇号查找接下来文件占用的五个数据簇簇号，数据簇0x2fe4在FAT的登记位置=FAT地址+0xfe4*2=0xca00+0x2fe4*2=0x129c8，我们来到0x129c8这个地址
数据簇0x2fe4的信息就存放在0x129c8，和0x129c9这个字节里，从上图可以看出这两个字节等于0x46F4，也就是说LQZ.TXT存放的下一个数据簇的簇号为0x46F4，由于该文件占用6个数据簇，所以我们必须继续查找剩下的4个数据簇的簇号。我们继续查找簇号为0x46f4的数据簇在FAT表的信息，地址为0xca00+0x46f4*2=0x157e8
由上图可以下一数据簇的簇号为0x46f5。然后按照上诉方法查找剩余三个簇号分别为0x46f6，0x46f7，0x46f8，最后在0x46f8对应的地方存放着0xffff，代表文件到此结束。
经过上诉步骤我们知道我lqz.txt文件依次存放在0x2fe4，0x46f4，0x46f5，0x46f6，0x46f7，0x46f8这6个数据簇，接下来我们就读取这6个数据簇的内容即可。
比如说：数据簇0x2fe4的地址=数据簇0地址+0x2fe4*4*512=0x4c200+0x2fe4*4*512=0x183e200，0x183e200/512=49649，也就是在物理扇区49649~49652这个四个扇区都是数据簇0x2fe4的内容
