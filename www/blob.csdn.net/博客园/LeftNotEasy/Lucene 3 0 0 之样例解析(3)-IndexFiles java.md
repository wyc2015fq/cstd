# Lucene 3.0.0 之样例解析(3)-IndexFiles.java - LeftNotEasy - 博客园







# [Lucene 3.0.0 之样例解析(3)-IndexFiles.java](https://www.cnblogs.com/LeftNotEasy/archive/2010/01/09/1643022.html)







**一. consumer的来源**

    接着上一小节的内容, 还是从这一段程序(位于DocumentWriter.updateDocument(Document, Analyzer, Term) 中继续.


try {// This call is not synchronized and does all the// workfinal DocWriter perDoc = state.consumer.processDocument();// This call is synchronized but fast  finishDocument(state, perDoc);  success = true;}


     第一句话就是生成索引的一个过程, 下面我将详细的剖析这个过程.

     首先一个问题就是:consumer是怎么来的? 什么时候初始化的, 我们用单步程序的方法来看看.

     state是从上一节提到的


synchronized DocumentsWriterThreadState getThreadState(Document doc, Term delTerm)


     中得到的, 我们进去看看... 原来是这一句话, 


state = newArray[threadStates.length] = new DocumentsWriterThreadState(this);


     跟踪进去看看, DocumentWriterThreadState的构造函数, 发现了这一句:


consumer = docWriter.consumer.addThread(this);




     原来还是从docWriter中来的, 我们继续查看docWriter里面是怎样进行的初始化的.


DocumentsWriter(Directory directory, IndexWriter writer, IndexingChain indexingChain) throws IOException {//...省略了  consumer = indexingChain.getChain(this);if (consumer instanceof DocFieldProcessor) {    docFieldProcessor = (DocFieldProcessor) consumer;  }}




    咱们再看看indexingChain.getChain(this)这个方法是怎么回事

    再跟踪进去, 就真相大白了:


DocConsumer getChain(DocumentsWriter documentsWriter) {// Build up indexing chain:final TermsHashConsumer termVectorsWriter = new TermVectorsTermsWriter(documentsWriter);final TermsHashConsumer freqProxWriter = new FreqProxTermsWriter();final InvertedDocConsumer  termsHash = new TermsHash(documentsWriter, true, freqProxWriter,new TermsHash(documentsWriter, false, termVectorsWriter, null));final NormsWriter normsWriter = new NormsWriter();final DocInverter docInverter = new DocInverter(termsHash, normsWriter);returnnew DocFieldProcessor(documentsWriter, docInverter);}


    这个就是一个非常典型的 装饰者 + 责任链 模式, 这样当我们运行:


final DocWriter perDoc = state.consumer.processDocument();




    时, 就会在IndexChain内部进行一系列的击鼓传花似的活动, 最终得到处理后的文档.



**二. IndexChain的运转过程**

    前面说了, IndexChian就是一个击鼓传花的过程, 那这个花是怎么传的呢? 让我们来揭开这个迷

    我画了一个流程图, 希望可以讲得更清楚一点, 另外我先说明一下图的颜色所代表的含义, 同样的颜色表示的是在同一个类中运行的过程. 箭头的指向表示过程或者关系, 比如说 DocInverterPerThread就是DocFieldProcessorPerThread的一个consumer. 前面的数字表示调用的顺序

![image](https://images.cnblogs.com/cnblogs_com/LeftNotEasy/WindowsLiveWriter/Lucene3.0.03IndexFiles.java_D4F9/image_thumb_1.png)

     1)的入口就是processDocument, 下面调用了一系列的startDocument()函数,  这些函数的作用是对之后做postings所使用的consumer, processor进行初始化, 其实大部分的startDocument()内容为空, 显然, 这是Lucene的开发者为后面的人所预留的一个接口. 这6部可以认为是初始化的内容.

     然后下面一张图就是处理的一个具体内容了:

![image](https://images.cnblogs.com/cnblogs_com/LeftNotEasy/WindowsLiveWriter/Lucene3.0.03IndexFiles.java_D4F9/image_thumb_2.png)

    先从类命名来说说吧. 可以大概看出, 跟consumer相关的类有几种主要的命名规则:

    1) 结尾带有Processor的,  这些类是一个基类, 负责调用结尾结尾是Consumer.

    2) 结尾带有Consumer的, 这些类通常被Processor来调用

    3) 结尾带有PerThread的, 这些类负责调用PerField的

    也可以认为, PerThread是负责调用一系列的PerField, 为什么这样设计我也不能完全说清楚, 如果有哪些朋友知道请指教.



**接下来我将详细的说说TermHashPerField.add()方法, 这个方法是把一个Term加入到Posting表中的过程, 我非常希望能够理解大部分的内容, 如果有错误或者不清楚的地方也希望大家能够不吝赐教**












