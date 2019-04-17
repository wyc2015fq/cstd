# Lucene 3.0.0的细节初窥(2)-研究在索引过程中的缓存 - LeftNotEasy - 博客园







# [Lucene 3.0.0的细节初窥(2)-研究在索引过程中的缓存](https://www.cnblogs.com/LeftNotEasy/archive/2010/01/26/1656426.html)





    Lucene有一个问题一直困扰着我, 就是如何在索引文件的时候节省空间, 合理的分配不大也不小的空间有助于在提高搜索速度的同时也能够监测内存的使用情况, 在内存使用到达某个阈值的时候可以触发合并的操作

    之前在写一个小程序, 来实现类似于Lucene索引文件的时候, 我是用c++写的, 没有使用内存池, 需要的时候就找操作系统"要"一块, 不需要的时候就还给它, 这样不仅仅在内存的频繁的分配中造成大量的内存碎片, 而且没有用内存池还会带来操作系统的换页情况, 使得本来一共就3M左右的文件读取到内存中, 进行分词操作后要70M(当然是增加了一些属性了, 不过也没有这么高吧!), 把豆腐都盘成肉价钱了.

    另外还是声明一下: 本文的著作权归LeftNoteasy所有, 如果需要转载请保留这句话并注明出处.

    在TermsHashPerField中有三个Pool, CharBlockPool, IntBlockPool, ByteBlockPool. 三者之间非常类似, 其实我觉得Lucene的开发者应该加以统一, 把代码进行规范化处理, 我看到Lucene中随处可见重复的代码, 这样不仅给开发带来很多重复的工作, 而且对维护也不一定好, 如果那一天发现某个算法可以改进或者出现了bug, 那估计得忙上一阵子了.

     三个缓存都是来自于TermsHashPerThread, 也就是说, 每个线程都有独立的Pool,这样的设计很好, 可以利用多核来对索引进行加速.

    1. CharBlockPool


   1:finalclass CharBlockPool   2: {   3:char[][] buffers = newchar[10][];   4:int bufferUpTo = -1;   5:int charUpTo = DocumentWriter.CHAR_BLOCK_SIZE;   6:char[] buffer;   7:int charOffset = -DocumentWriter.CHAR_BLOCK_SIZE;   8: }
      这里给出了CharBlockPool的主要成员变量, Lucene的Buffer设计有一个特点, 就是采用了二维数组, 就像房子一样, 最开始是个平房, 如果不够住了就往天上盖, 越盖越高, 当然, 是当前层住不下了才盖新的一层.

    buffers就是这个楼房

    bufferUpTo是这个楼房有多少层

    charUpTo是我们在当前层的定位(where we are)

    charOffset是我们总体的偏移量.

    下面看看CharBlockPool的nextBuffer()函数, 也就是房子不够, 修楼房的函数:


   1:publicvoid nextBuffer() {   2:if (1+bufferUpto == buffers.length) {   3:char[][] newBuffers = newchar[(int) (buffers.length*1.5)][];   4:     System.arraycopy(buffers, 0, newBuffers, 0, buffers.length);   5:     buffers = newBuffers;   6:   }   7:   buffer = buffers[1+bufferUpto] = docWriter.getCharBlock();   8:   bufferUpto++;   9:  10:   charUpto = 0;  11:   charOffset += DocumentsWriter.CHAR_BLOCK_SIZE;  12: }


    程序应该是比较好理解的, 首先看看buffers.length(房子的规划)是不是不够了, 如果规划少了, 就把规划*1.5得到新的规划, 比如本来预计房子会修到10层, 就提高到15层.

    然后把bufferUpto指针++, 从docWriter.getCharBlock函数来"要"内存, 在getCharBlock()函数中还会对申请的内容进行记载, 以进行内存的监控.

    然后我们看看CharBlockPool是怎样被调用的, 在TermsHashPerField的add()函数中:


   1:if (p == null) {   2:finalint textLen1 = 1+tokenTextLen;   3:if (textLen1 + charPool.charUpto > DocumentsWriter.CHAR_BLOCK_SIZE) {   4:if (textLen1 > DocumentsWriter.CHAR_BLOCK_SIZE) {   5:if (docState.maxTermPrefix == null)   6:         docState.maxTermPrefix = new String(tokenText, 0, 30);   7:       consumer.skippingLongTerm();   8:return;   9:     }  10:     charPool.nextBuffer();  11:   }


     这段代码首先计算出当前单词的长度(textLen1), 然后看看当前缓存的"层"是否够住了, 如果不够, 就调用charPool.nextBuffer()加盖楼房.


   1:finalchar[] text = charPool.buffer;   2:finalint textUpto = charPool.charUpto;   3: p.textStart = textUpto + charPool.charOffset;   4: charPool.charUpto += textLen1;   5: System.arraycopy(tokenText, 0, text, textUpto, tokenTextLen);   6: text[textUpto+tokenTextLen] = 0xffff;




    这段代码就是怎么使用缓存的了. 首先拿到charBlockPool的buffer, 然后修改p.textStart, 这个成员变量的意思是这个posting(也就是这个单词了)是从charPool中什么位置起始的. 然后把单词结束后的一位设置为0xffff.

    可以看出, charBlockPool的用法还是很朴素的, 这可能也说明了, 越朴素, 越灵活吧, 或许lucene的开发者是这样考虑的, 呵呵.

     2. IntBlockPool, ByteBlockPool

     这个类非常类似于charBlockPool, 在lucene的缓存中, 对每一种类型都设置了一种内存池, 对于token来说, text使用charPool, 位置采用一个一个的byte来构成, 而表示位置byte的起始序列的值采用IntPool来保存, 起始我觉得IntBlockPool并不是那样的有必要, 我感觉保存在postingHash中也是可以的, 具体我慢慢再理解一下, 也欢迎大家讨论.

     IntBlockPool与ByteBlockPool结合非常紧密, 从代码中就可以看得出来:


   1: intUptos = intPool.buffer;   2: intUptoStart = intPool.intUpto;   3: intPool.intUpto += streamCount;   4:   5: p.intStart = intUptoStart + intPool.intOffset;   6:   7:for(int i=0;i<streamCount;i++) {   8:finalint upto = bytePool.newSlice(ByteBlockPool.FIRST_LEVEL_SIZE);   9:   intUptos[intUptoStart+i] = upto + bytePool.byteOffset;  10: }  11: p.byteStart = intUptos[intUptoStart];  12:  13: consumer.newTerm(p);  14:  15:else {  16: intUptos = intPool.buffers[p.intStart >> DocumentsWriter.INT_BLOCK_SHIFT];  17: intUptoStart = p.intStart & DocumentsWriter.INT_BLOCK_MASK;  18: consumer.addTerm(p);  19:     }




    streamCount就是表示使用几个int表示偏移指针. intUptos就是一个int[], 保留当前的"层",  下面研究一下这个for循环:


   1:for(int i=0;i<streamCount;i++) {   2:finalint upto = bytePool.newSlice(ByteBlockPool.FIRST_LEVEL_SIZE);   3:   intUptos[intUptoStart+i] = upto + bytePool.byteOffset;   4: }


    newSlice就是为bytePool分配一个int大小的空间, FIRST_LEVEL_SIZE为5个byte, 如果按这个程序运行, intUptos[intUptoStart]的值与bytePool的byteUpto并不一致, 要小了5. 这样说吧. 我来画一个例子

bytePool

....../xxxxxx/…… : in memory

      ^(A),xxxxxx为一段数据



intPool

……A, 指向的应该是这段数据的开始位置, 所以int的Upto要比byte的Upto小了一个size的大小, 这个size就是bytePool对应的这段数据的长度.

    接下来的问题是: 当对内存进行数据的写入的时候, 分配的内存不够怎么办呢, 之前已经说了, bytePool是存储单词出现的位置的, 但是在分配内存的时候, 却不知道单词将会出现多少次, 所以必然会出现扩展内存的情况, 下面举一个例子:

    AA. . .B . . . C . . .这样的一个buffer, 假设现在要在B后面写入4位, 但是B却只有3位的剩余空间了, 怎么办呢, 首先我们分配一个更大的片(slice), 具体的slice的大小可以参见levelSizeArray[], 然后把B的后三位复制到新的slice中, 并且把B的后三位改成一个offset, 指向新的slice, 可能看起来是这样的:

    AA. . .B offset C . . . (new B): B B B B, 这段程序请见TermsHashPerField中的writeByte.

    不过说起来, 这段Pool相关的代码写得也确实恶心了一点, 很容易让人糊涂, 而且注释也是很金贵的.

    另外在Lucene中还广泛应用的就是RawPostingList[]这样类似于数组的hash形式, 之前一直觉得很奇怪, 为什么作者不能用泛型的hashMap呢, 想了想, 觉得第一个这样做会造成一些不必要的时间开销, 另外要是需要对这个hash排序就不是那样的友好了. 这种hash形式确实很不错哦.












