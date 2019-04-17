# Lucene 3.0.0细节初窥(1)-深入探索Lucene的consumer与processor - LeftNotEasy - 博客园







# [Lucene 3.0.0细节初窥(1)-深入探索Lucene的consumer与processor](https://www.cnblogs.com/LeftNotEasy/archive/2010/01/25/1655844.html)





**写在前面的一些废话:**

     对于Lucene 3.0.0的线程模型我非常的感兴趣, 因为对于多线程我也是最近才接触, 别看我接触程序都快十年了, 有几个地方我一直非常的遗憾 : **没有写过网络相关的代码, 没有写过多线程程序, 没有写过数据库相关的内容, 没有写过Linux相关的程序**. 可能各位会觉得非常奇怪了: **那你这十年干嘛去了? 这不是基本上等同于不懂程序啊! (–_-)**

     我花了6年的时间巩固了算法和数据结构基础, 另外4年糊里糊涂的搞了很多比如3D游戏, 游戏的人工智能程序等内容, 总体上来说, 没有太多虚度时间 代码写得不算多, 算法基础不坏, 不过效率也不算蛮高, 我希望能够在接下来的几年内好好补一补, 慢慢变成一个"万金油" 把乱七八糟的东西都搞搞.

     对于研究别人的程序我有一个爱好, 相对于停留在"使用"的层面上, 还不如停留在"研究"的层面上, 我的感觉是, 在工作之前应该尽量的把基础作牢固, 能够多把别人代码中先进的思想学会, 不然工作了以后, 项目的压力和生活的压力会让人浮躁, 会让人没有一个定力来研究需要花费很多时间去搞懂的内容. 其实目前的开源框架(比如说apache下面的)和可以免费使用的框架(比如WPF之类的)已经非常的多了, 想要仅仅是"做"完一个项目其实不算难事, 当然是一般的项目, 不是庞大的项目. 重要的是, 怎么把自己的思想提升到更高的层面, 我希望我能按照自己的想法来发展. 可能这也是我大学来没有接过一个外包项目的原因吧.

     通过写日志我想和大家分享我的心得, 对于一个开源框架的研究, 可能也是需要一个小小的团队一起来做, 一个人阅读着海量的代码是一件非常苦的事情, 如果有人讨论一下, 可能很多问题都会迎刃而解了. 另外对于Lucene, 我希望能够在对源代码有着相当的把握后, 使用操作系统无关的库和c++进行一些重写, 对于关键的代码实现一下, 一方面的原因是, 以后工作的语言是c++而不是java, 另一方面也是为自己留下一些"可用"的代码. 这个依然是停留在"研究"的层面上, 未来这个项目能否发展为"可用" 需要再看看.

     另外转帖请注明出处: leftnoteasy.cnblogs.com, leftnoteasy原创

**Lucene 3.0.0的Consumer调用部分(续):**

     Lucene的线程结构和Lucene的Consumer模型是密切相关的, 之前我有一篇日志记载了一些关于Lucene的Consumer的部分, 关于这些内容请见: [Lucene 3.0.0 之样例解析(3)-IndexFiles.java](http://www.cnblogs.com/LeftNotEasy/archive/2010/01/09/1643022.html), 之前写的比较粗糙, 有好些内容是没有记载完全的, 这里就补充一下:

     为了解开复杂的consumer调用模型, 我试着绘制一下uml图来理清思路:

    跟consumer和processor类直接相关的类就有下面的这些, 看起来挺恐怖的吧, 下面我来试图将他们之间的关系理清楚:

    值得注意的是, 在阅读代码的时候发现, 有些时候, 就算是两个抽象类的方法一模一样, 也没有使用继承的关系来做, 可能是Lucene的开发者为了不出现RTTI的错误吧, 这个在初次阅读代码的时候很容易把人弄混淆.

![consumers_uml](https://images.cnblogs.com/cnblogs_com/LeftNotEasy/WindowsLiveWriter/Lucene3.0.01Lucene3.0.0_954A/consumers_uml_thumb.jpg)

  1) DocFieldProcessor, DocFieldProcessorPerThread与DocInverter:

![image](https://images.cnblogs.com/cnblogs_com/LeftNotEasy/WindowsLiveWriter/Lucene3.0.01Lucene3.0.0_954A/image7_thumb.png)

**DocFieldProcessor**

    DocFieldProcessor继承自DocConsumer, 这个DocConsumer可以说是最高层的Consumer了. 下面我们来看看DocFieldProcessor的内容, 查看代码的时候会发现一句话:


   1:/**   2: * This is a DocConsumer that gathers all fields under the   3: * same name, and calls per-field consumers to process field   4: * by field.  This class doesn't doesn't do any "real" work   5: * of its own: it just forwards the fields to a   6: * DocFieldConsumer.   7: */
      翻译一下就是: DocConsumer把全部名称相同的Field聚集起来, 然后调用PerField为后缀的consumer进行处理, 这个类不做"实际"的工作, 它仅仅把工作推向了DocFieldConsumer.

    看看实际的代码就可能更容易理解一点:


   1:publicvoid flush(Collection<DocConsumerPerThread> threads, SegmentWriteState state) throws IOException {   2:   3:   Map<DocFieldConsumerPerThread, Collection<DocFieldConsumerPerField>> childThreadsAndFields = new HashMap<DocFieldConsumerPerThread, Collection<DocFieldConsumerPerField>>();   4:for ( DocConsumerPerThread thread : threads) {   5:        ......   6:   }


    这个flush函数就可以看出, 实际上DocFieldProcessor是调用了多个DocFieldProcessorPerThread, 下面我们来看看DocFieldProcessorPerThread:

**DocFieldProcessorPerThread:**

    这个函数继承自DocConsumerPerField,也就是刚刚提到的做"实际"工作的类,  而继承自基类的processDocument()函数更是重点中的重点了

    还是老样子, 看看注释:


   1:/**   2: * Gathers all Fieldables for a document under the same   3: * name, updates FieldInfos, and calls per-field consumers   4: * to process field by field.   5: *   6: * Currently, only a single thread visits the fields,   7: * sequentially, for processing.   8: */


     翻译过来: 把全部名字相同的Fieldable(同Field)聚集起来, 更新FieldInfo, 然后调用per-Field consumers来一个一个Field地进行处理. 当前, 就只有一个Thread来访问这些Fields, 按照顺序的来处理(注:我这里不太理解"currently"的含义是在这个程序里还是这个版本)

     processDocument()的代码很长, 有一百多行, 我之前的文章也有过一些分析.  归纳出来, 这个函数就做了几件事情:

     i. 把Field收集起来, 看看之前出现过没有, 如果没有出现过, 则扩充hash

     ii. 对这些Field进行排序(请见里面的quickSort()函数)

     iii. 对每个Field调用其consumer(也就是基类为DocFieldConsumerPerField)进行处理.

**DocInverter:**


   1:/** This is a DocFieldConsumer that inverts each field,   2: *  separately, from a Document, and accepts a   3: *  InvertedTermsConsumer to process those terms. */




    对来自同一篇文档的每一个Field进行互不相关的倒排操作, 然后调用InvertedTermsConsumer对这些Term进行处理.

**2) DocInverterPerField, TermsHashPerField**

![image](https://images.cnblogs.com/cnblogs_com/LeftNotEasy/WindowsLiveWriter/Lucene3.0.01Lucene3.0.0_954A/image11_thumb.png)

**DocInverterPerField.**

    这个类是DocProcessorPerThread中调用的, 请见DocProcessorPerThread中的processDocument()函数中的一段话


   1:for(int i=0;i<fieldCount;i++)   2:   fields[i].consumer.processFields(fields[i].fields, fields[i].fieldCount);


   这里的consumer就是DocInverterPerField所继承的DocFieldConsumerPerField


   1:/**   2: * Holds state for inverting all occurrences of a single   3: * field in the document.  This class doesn't do anything   4: * itself; instead, it forwards the tokens produced by   5: * analysis to its own consumer   6: * (InvertedDocConsumerPerField).  It also interacts with an   7: * endConsumer (InvertedDocEndConsumerPerField).   8: */


    保留一篇文档中的一个field的倒排状态, 这个类什么都不干(其实也不完全是, 后面再说说), 它将token的处理丢给自己的consumer(InvertedDocConsumerPerField), 另外它还和endConsumer(InvertedDocEndConsumerPerField)发生关系.

    这里的processField主要干了下面几件事情:


   1: @Override   2:publicvoid processFields(final Fieldable[] fields,   3:finalint count) throws IOException {   4:     1) 首先判断是否是需要tokenized, 如果不需要, 就直接把字符串放进来   5:         如果需要就调用analyzer   6:     2) 调用自己的consumer进行处理   7: }




**TermsHashPerField**

在之前, 我分析过TermHashPerField, 这次主要从调用的方面来说说

    TermsHashPerField主要由DocInverterPerField调用, 在DocInverterPerField中对TermsHashPerField的初始化:


   1: @Override   2:publicvoid processFields(final Fieldable[] fields,   3:finalint count) throws IOException {   4: ....   5:try {   6:int offsetEnd = fieldState.offset-1;   7:   8:boolean hasMoreTokens = stream.incrementToken();   9:  10:   fieldState.attributeSource = stream;  11:  12:   OffsetAttribute offsetAttribute = fieldState.attributeSource.addAttribute(OffsetAttribute.class);  13:   PositionIncrementAttribute posIncrAttribute = fieldState.attributeSource.addAttribute(PositionIncrementAttribute.class);  14:  15:   consumer.start(field);  16:   ....  17: }


    在看看TermsHashPerField里面的start():


   1: @Override   2:void start(Fieldable f) {   3:   termAtt = fieldState.attributeSource.addAttribute(TermAttribute.class);   4:   consumer.start(f);   5:if (nextPerField != null) {   6:     nextPerField.start(f);   7:   }   8: }


     其实就是对termAtt进行初始化, termAtt是跟TokenStream相关联的类, 表示一个词的属性, 内容等等, 这里不多说, 相关的资料不少.   TermHashPerField重要的是add()函数, add()是一个巨无霸函数, 大概接近150行左右, 要逐行理解很麻烦, 我说说大概的意思:


   1: @Override   2:void add() throws IOException {   3:     1) 获取token文字信息   4:      2) 进行hash处理, 并移除无效字符   5:      3) 如果单词出现过, 就调用consumer.addTerm(), 否则调用consumer.newTerm()   6: }




**3) TermVectorsTermsWriterPerField, FreqProxTermsWriterPerField**

![image](https://images.cnblogs.com/cnblogs_com/LeftNotEasy/WindowsLiveWriter/Lucene3.0.01Lucene3.0.0_954A/image15_thumb.png)

     这两个类都是继承自TermsHashConsumerPerField, 但是情况不一样, TermsVectorsTermsWriterPerField是仅仅当Field指定了TermVector存储的时候才调用的.

     他们都具有addTerm与newTerm的方法, 意思也比较好理解, 但是程序也比较复杂, 我目前还没有怎么去看, 如果有可能我希望写一篇专门关于Lucene里面的buffer相关的内容的文章来说明一下.












