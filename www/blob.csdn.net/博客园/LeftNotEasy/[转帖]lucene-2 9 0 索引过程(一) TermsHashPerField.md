# [转帖]lucene-2.9.0 索引过程(一) TermsHashPerField - LeftNotEasy - 博客园







# [[转帖]lucene-2.9.0 索引过程(一) TermsHashPerField](https://www.cnblogs.com/LeftNotEasy/archive/2010/01/25/1655966.html)





TermsHashPerField 类



一、类功能概述：

负责词项的索引过程,每个字段有相应的一个TermsHashPerField；当索引某字段词项时，使用对应TermsHashPerField的add()函数完成(一个)词项索引过程，并将索引内容(词项字符串/指针信息/位置信息等)存储于内存缓冲中



二、类成员说明:



2.1 final int streamCount;

如果需要记录词频和位置，此值为2（用两个int记录偏移指针），否则为1



// 代码如下

streamCount = consumer.getStreamCount(); // FreqProxTermsWriterPerField // 



2.2 TermAttribute termAtt;

分词后的词项存储于此，因此索引时从termAtt中将词项取出



2.3  RawPostingList[]和RawPostingList

代码如下

private RawPostingList[] postingsHash = new RawPostingList[postingsHashSize];

private RawPostingList p;



每个p记录了该词项的偏移地址、文档编号、文档频率(df)、位置信息

并存储在postingsHash中，下标是哈希值



保存了偏移地址

abstract class RawPostingList {

  int textStart; 

  int intStart;

  int byteStart;

}



但实际上存储的是FreqProxTermsWriter.PostingList

意思很浅显，代码如下:

static final class PostingList extends RawPostingList {

    int docFreq;    // # times this term occurs in the current doc

    int lastDocID;  // Last docID where this term occurred

    int lastDocCode;  // Code for prior doc

    int lastPosition;    // Last position where this term occurred

  }



因此postingsHash存储的其实是

[0]= FreqProxTermsWriter$PostingList  (id=23)    

         byteStart= 0     

         docFreq= 1       

         intStart= 0        

         lastDocCode= 0        

         lastDocID= 0    

         lastPosition= 0          

         textStart= 0     



postingsHash存储的是该字段所有词项的一些索引信息，根据词项字符的编码值作为初始值hash到postingsHash中

代码如下:

int hashPos = code & postingsHashMask;



如果出现地址冲突,以固定步长递增编码值,查找空位置，填入p，此步长为



final int inc = ((code>>8)+code)|1;



代码如下



do {

        code += inc;

        hashPos = code & postingsHashMask ;

        p = postingsHash[hashPos];

} while (p != null && !postingEquals(tokenText, tokenTextLen));





另外，如果postingsHash的使用率超过一半(使用率可调)，将扩展之

代码如下

      if (numPostings == postingsHashHalfSize)

        rehashPostings(2*postingsHashSize);



2.2 字符串缓冲



以下三个是存储索引内容的缓冲，最终还是从DocumentsWriter中获得，此缓冲中的索引结构并不是最终写入磁盘的索引内容结构

intPool = perThread.intPool;

charPool = perThread.charPool;

bytePool = perThread.bytePool;



2.2.1 final CharBlockPool charPool; - 

存储词项内容(字符串内容)

连续存储，相邻词项间隔1个char，即存储textLen+1长度

字符串指针记录了每个词项的开始位置(无记录长度和结束位置，仅通过相邻字符串起始位置得知长度)



例如



华  南 理 工 大 学  2 0 0 9 1 0 1 9 0 1 4 6

|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|-|



2.2.2 final IntBlockPool intPool;





存储的是位置信息的偏移值



 for(int i=0;i<streamCount;i++) 

 {

// intUptos[intUptoStart+1]记录的是位置信息偏移值， 

// intUptos[intUptoStart+0]尚未明白

     final int upto = bytePool.newSlice(ByteBlockPool.FIRST_LEVEL_SIZE);

        intUptos[intUptoStart+i] = upto + bytePool.byteOffset;

 }



隔2(streamCount)存储一个词项,即每个词项记录了两个指针



2.2.3 final ByteBlockPool bytePool;



写入词项位置信息



隔10存储一个词项位置(编码后)

|-|-|---|

0-5-10-15



间隔10 这个值由以下决定的

1. ByteBlockPool.FIRST_LEVEL_SIZE =5 

2. 已经提及到的:当需要保存词项频率和词项位置时streamCount =2 

代码如下

for(int i=0;i<streamCount;i++) 

{

final int upto = bytePool.newSlice(ByteBlockPool.FIRST_LEVEL_SIZE);

   intUptos[intUptoStart+i] = upto + bytePool.byteOffset;

}









三、类成员函数说明



3.1 构造函数

  public TermsHashPerField(DocInverterPerField docInverterPerField, 

                           final TermsHashPerThread perThread, 

                           final TermsHashPerThread nextPerThread, 

                           final FieldInfo fieldInfo)



一些索引全局变量在构造函数时传入

final FieldInfo fieldInfo是字段信息



传入缓冲内存池

    intPool = perThread.intPool;

    charPool = perThread.charPool;

bytePool = perThread.bytePool;





3.2 索引过程

索引过程由add()函数完成

  void add() throws IOException {}





  void add() throws IOException {



    // 获得分词后的词项内容和长度

    final char[] tokenText = termAtt.termBuffer();

    final int tokenTextLen = termAtt.termLength();





    // 计算postingsHash位置，使用词项字符做编码值

    int downto = tokenTextLen;

    int code = 0;



    while (downto > 0) {



      // 高位起



      char ch = tokenText[--downto];



     //  中间省去Unicode情况



      code = (code*31) + ch;  // 字符编码

    } // while (downto > 0)



    // 词项最终编码



    int hashPos = code & postingsHashMask;



    // Locate RawPostingList in hash

    p = postingsHash[hashPos];



    // 如果该哈席位已经填充，遍历寻找下一个空位

    if (p != null && !postingEquals(tokenText, tokenTextLen)) {

      // Conflict: keep searching different locations in

      // the hash table.

      final int inc = ((code>>8)+code)|1;

      do {

        code += inc;

        hashPos = code & postingsHashMask;

        p = postingsHash[hashPos];

      } while (p != null && !postingEquals(tokenText, tokenTextLen));

    }



    // 找到一个可以填充的位置(postingsHash未曾索引过此词项)

    if (p == null) { // 第一次p为空，因为没有哈希冲突



      final int textLen1 = 1+tokenTextLen;



      // 存储词项字符的缓冲已满，扩展之

      if (textLen1 + charPool.charUpto > DocumentsWriter.CHAR_BLOCK_SIZE)  // 16384

      {

        if (textLen1 > DocumentsWriter.CHAR_BLOCK_SIZE) {



          if (docState.maxTermPrefix == null)

            docState.maxTermPrefix = new String(tokenText, 0, 30);



          consumer.skippingLongTerm();

          return;

        }

        charPool.nextBuffer();

      }



      // Pull next free RawPostingList from free list

      p = perThread.freePostings[--perThread.freePostingsCount];





      final char[] text = charPool.buffer;

      final int textUpto = charPool.charUpto;



      // 记录词项字符的偏移地址值

      p.textStart = textUpto + charPool.charOffset; 

      charPool.charUpto += textLen1(原字符长度+1);



     // 将词项字符拷贝至缓冲中

      System.arraycopy(tokenText, 0, text, textUpto, tokenTextLen);

      text[textUpto+tokenTextLen] = 0xffff;



      assert postingsHash[hashPos] == null;

      postingsHash[hashPos] = p;

      numPostings++;



      // postingsHash如果使用率超过一半，扩展之

      if (numPostings == postingsHashHalfSize)

        rehashPostings(2*postingsHashSize);



      // Init stream slices

      if (numPostingInt + intPool.intUpto > DocumentsWriter.INT_BLOCK_SIZE)

        intPool.nextBuffer(); // 内存不足，申请



      if (DocumentsWriter.BYTE_BLOCK_SIZE - bytePool.byteUpto < numPostingInt*ByteBlockPool.FIRST_LEVEL_SIZE)

        bytePool.nextBuffer(); //内存不足，申请



      intUptos = intPool.buffer; // 记录偏移量

      intUptoStart = intPool.intUpto;

      intPool.intUpto += streamCount; // 每个词项使用streamCount偏移值



      p.intStart = intUptoStart + intPool.intOffset;



      for(int i=0;i<streamCount;i++) 

      {

        final int upto = bytePool.newSlice(ByteBlockPool.FIRST_LEVEL_SIZE);

        intUptos[intUptoStart+i] = upto + bytePool.byteOffset;

      }



      p.byteStart = intUptos[intUptoStart];



      consumer.newTerm(p);



    } else {

    // 这说明已经索引过此词项(再次在原始文档中出现)



      intUptos = intPool.buffers[p.intStart >> DocumentsWriter.INT_BLOCK_SHIFT];

      intUptoStart = p.intStart & DocumentsWriter.INT_BLOCK_MASK;

      consumer.addTerm(p); // 添加posting list 且压缩编码

    }

  }



四 实验

以下的数据支撑以上的解析，索引文档,

文档一的内容为”新浪新闻”，而文档二的内容为”联合早报”



adding D:\file\2.txt

新

intUptoStart= 16

intPool.intUpto= 18

p.intStart= 16

intUptos[intUptoStart+i]= 80

intUptos[intUptoStart+i]= 85

p.byteStart= 80

bytes[85]= 0



RawPostingList P 的值如下:

byteStart= 80 

docFreq= 1 

intStart= 16 

lastDocCode= 2 

lastDocID= 1 

lastPosition= 0 

textStart= 39 



termsHashPerField.writeVInt(1,0)



浪

intUptoStart= 18

intPool.intUpto= 20

p.intStart= 18

intUptos[intUptoStart+i]= 90

intUptos[intUptoStart+i]= 95

p.byteStart= 90

bytes[95]= 2



RawPostingList P 的值如下:

byteStart= 90 

docFreq= 1 

intStart= 18 

lastDocCode= 2 

lastDocID= 1 

lastPosition= 0 

textStart= 41 



termsHashPerField.writeVInt(1,1)



新（注意:第二次出现此词项）

intUptoStart= 16

intPool.intUpto= 20

p.intStart= 16

bytes[86]= 4 // 连续存储词项位置信息



RawPostingList P 的值如下:

byteStart= 80 

docFreq= 2  // 文档df设置为2

intStart= 16 

lastDocCode= 2 

lastDocID= 1 

lastPosition= 0 

textStart= 39 



termsHashPerField.writeVInt(1,2)



闻

intUptoStart= 20

intPool.intUpto= 22

p.intStart= 20

intUptos[intUptoStart+i]= 100

intUptos[intUptoStart+i]= 105

p.byteStart= 100

bytes[105]= 6



RawPostingList P 的值如下:

byteStart= 100 

docFreq= 1 

intStart= 20 

lastDocCode= 2 

lastDocID= 1 

lastPosition= 0 

textStart= 43 



termsHashPerField.writeVInt(1,3)



200910190147

intUptoStart= 22

intPool.intUpto= 24

p.intStart= 22

intUptos[intUptoStart+i]= 110

intUptos[intUptoStart+i]= 115

p.byteStart= 110

bytes[115]= 0



byteStart= 110 

docFreq= 1 

intStart= 22 

lastDocCode= 2 

lastDocID= 1 

lastPosition= 0 

textStart= 45 





D:\file\2.txt

intUptoStart= 24

intPool.intUpto= 26

p.intStart= 24

intUptos[intUptoStart+i]= 120

intUptos[intUptoStart+i]= 125

p.byteStart= 120

bytes[125]= 0



byteStart= 120 

docFreq= 1 

intStart= 24 

lastDocCode= 2 

lastDocID= 1 

lastPosition= 0 

textStart= 58 



adding D:\file\3.txt

联

intUptoStart= 26

intPool.intUpto= 28

p.intStart= 26

intUptos[intUptoStart+i]= 130

intUptos[intUptoStart+i]= 135

p.byteStart= 130

bytes[135]= 0



RawPostingList P 的值如下:

byteStart= 130 

docFreq= 1 

intStart= 26 

lastDocCode= 4 

lastDocID= 2 

lastPosition= 0 

textStart= 72 





合

intUptoStart= 28

intPool.intUpto= 30

p.intStart= 28

intUptos[intUptoStart+i]= 140

intUptos[intUptoStart+i]= 145

p.byteStart= 140

bytes[145]= 2



RawPostingList P 的值如下:

byteStart= 140 

docFreq= 1 

intStart= 28 

lastDocCode= 4 

lastDocID= 2 

lastPosition= 0 

textStart= 74 





早

intUptoStart= 30

intPool.intUpto= 32

p.intStart= 30

intUptos[intUptoStart+i]= 150

intUptos[intUptoStart+i]= 155

p.byteStart= 150

bytes[155]= 4



RawPostingList P 的值如下:

byteStart= 150 

docFreq= 1 

intStart= 30 

lastDocCode= 4 

lastDocID= 2 

lastPosition= 0 

textStart= 76 





报

intUptoStart= 32

intPool.intUpto= 34

p.intStart= 32

intUptos[intUptoStart+i]= 160

intUptos[intUptoStart+i]= 165

p.byteStart= 160

bytes[165]= 6



RawPostingList P 的值如下:

byteStart= 160 

docFreq= 1 

intStart= 32 

lastDocCode= 4 

lastDocID= 2 

lastPosition= 0 

textStart= 78 





200910190148

intUptoStart= 34

intPool.intUpto= 36

p.intStart= 34

intUptos[intUptoStart+i]= 170

intUptos[intUptoStart+i]= 175

p.byteStart= 170

bytes[175]= 0



D:\file\3.txt

intUptoStart= 36

intPool.intUpto= 38

p.intStart= 36

intUptos[intUptoStart+i]= 180

intUptos[intUptoStart+i]= 185

p.byteStart= 180


本文来自CSDN博客，转载请标明出处：[http://blog.csdn.net/todaylxp/archive/2009/10/25/4726017.aspx](http://blog.csdn.net/todaylxp/archive/2009/10/25/4726017.aspx)












