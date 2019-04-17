# Lucene 3.0.0 之样例解析(2)-IndexFiles.java - LeftNotEasy - 博客园







# [Lucene 3.0.0 之样例解析(2)-IndexFiles.java](https://www.cnblogs.com/LeftNotEasy/archive/2010/01/09/1642870.html)





    这一小结的内容我将大概讲讲IndexFiles的一个过程.

    为了方便查看, 还是把IndexFile.java的源代码放在前面.


   1:publicclass IndexFiles {   2:   3:private IndexFiles() {}   4:   5:staticfinal File INDEX_DIR = new File("index");   6:   7:/** Index all text files under a directory. */   8:publicstaticvoid main(String[] args) {   9:     String usage = "java org.apache.lucene.demo.IndexFiles <root_directory>";  10:if (args.length == 0) {  11:       System.err.println("Usage: " + usage);  12:       System.exit(1);  13:     }  14:  15:if (INDEX_DIR.exists()) {  16:       System.out.println("Cannot save index to '" +INDEX_DIR+ "' directory, please delete it first");  17:       System.exit(1);  18:     }  19:  20:final File docDir = new File(args[0]);  21:if (!docDir.exists() || !docDir.canRead()) {  22:       System.out.println("Document directory '" +docDir.getAbsolutePath()+ "' does not exist or is not readable, please check the path");  23:       System.exit(1);  24:     }  25:  26:     Date start = new Date();  27:try {  28:       IndexWriter writer = new IndexWriter(FSDirectory.open(INDEX_DIR), new StandardAnalyzer(Version.LUCENE_CURRENT), true, IndexWriter.MaxFieldLength.LIMITED);  29:       System.out.println("Indexing to directory '" +INDEX_DIR+ "'...");  30:       indexDocs(writer, docDir);  31:       System.out.println("Optimizing...");  32://writer.optimize();  33:       writer.close();  34:  35:       Date end = new Date();  36:       System.out.println(end.getTime() - start.getTime() + " total milliseconds");  37:  38:     } catch (IOException e) {  39:       System.out.println(" caught a " + e.getClass() +  40:"\n with message: " + e.getMessage());  41:     }  42:   }  43:  44:staticvoid indexDocs(IndexWriter writer, File file)  45:throws IOException {  46:// do not try to index files that cannot be read  47:if (file.canRead()) {  48:if (file.isDirectory()) {  49:         String[] files = file.list();  50:// an IO error could occur  51:if (files != null) {  52:for (int i = 0; i < files.length; i++) {  53:             indexDocs(writer, new File(file, files[i]));  54:           }  55:         }  56:       } else {  57:         System.out.println("adding " + file);  58:try {  59:           writer.addDocument(FileDocument.Document(file));  60:         }  61:// at least on windows, some temporary files raise this exception with an "access denied" message  62:// checking if the file can be read doesn't help  63:catch (FileNotFoundException fnfe) {  64:           ;  65:         }  66:       }  67:     }  68:   }  69:  70: }




    这段代码应该是比较好理解的, 首先main函数接受参数, 调用indexDocs过程, 把一个文件夹下的所有的文件进行索引, 使用FileDocument.Document(file)的方法来生成一个一个的Document对象, 使用这个方法生成的文档将具有3个默认的域: path, modified, content, 默认情况下, path和modified是不分词的, content是需要分词的.

    然后再调用writer.addDocument方法, 进入生成索引文件的主过程.

    一. IndexWriter.addDocument(Document)                       //没有什么好说的

    -> 二. IndexWriter.addDocument(Document, Analyzer),     //由于没有传入Analyzer参数, 使用默认的

    ---> 三.DocumentWriter.addDocument(Document, Analyzer)

    -----> 四.DocumentWriter.updateDocument(Document, Analyzer, Term)  //下面详细说一下这个方法)

    这个方法的前两个参数没什么好说的, 第三个参数表示需要删除的Term, 这里先不管他.



1) 在DocumentWriter.updateDocument(Document, Analyzer, Term)中

final DocumentsWriterThreadState state = getThreadState(doc, delTerm);

    该方法获取一个ThreadState, 下面我再解释一下这个方法.

    在一个DocumentWriter中, 可能是多线程的, 也就是说, 可能有很多个进程同时对文档进行处理, 在需要这些进程的时候, 他们可能是空闲的, 也可能是繁忙的, 有可能是使用一个现有的进程, 也有可能是需要创建一个新的进程, 每个进程包含了独立的Posting Table与其他状态信息, 需要在完成后合并的.

    怎么选择进程就是在上面这一段程序了, 让我们进入这个方法内部.



2)在DocumentWriter.getThreadState中, 我来把方法列举出来, 去掉一些不太重要的代码:


synchronized DocumentsWriterThreadState getThreadState(Document doc, Term delTerm) throws IOException {//首先查看当前的进程状态信息, threadBindings是一个Hash表, 进程与状态一一对应的  DocumentsWriterThreadState state = threadBindings.get(Thread.currentThread());//如果当前的进程状态信息是不可用的if (state == null) {//查看其他进程, 选择一个最少使用的进程    DocumentsWriterThreadState minThreadState = null;for(int i=0;i<threadStates.length;i++) {      DocumentsWriterThreadState ts = threadStates[i];if (minThreadState == null || ts.numThreads < minThreadState.numThreads)        minThreadState = ts;    }if (minThreadState != null && (minThreadState.numThreads == 0 || threadStates.length >= MAX_THREAD_STATE)) {      state = minThreadState;      state.numThreads++;    } else {//否则新建一个"私有的Thread State,在docWriter中加入该Thread, //并且在consumer中加入这个 Thread,            DocumentsWriterThreadState[] newArray = new DocumentsWriterThreadState[1+threadStates.length];if (threadStates.length > 0)        System.arraycopy(threadStates, 0, newArray, 0, threadStates.length);      state = newArray[threadStates.length] = new DocumentsWriterThreadState(this);      threadStates = newArray;    }    threadBindings.put(Thread.currentThread(), state);  }//然后调用waitReady(DocumentWriterThreadState)函数, 等待进程控线  waitReady(state);//分配Segment名称  initSegmentName(false);  state.isIdle = false;boolean success = false;try {    state.docState.docID = nextDocID;assert writer.testPoint("DocumentsWriter.ThreadState.init start");if (delTerm != null) {      addDeleteTerm(delTerm, state.docState.docID);      state.doFlushAfter = timeToFlushDeletes();    }assert writer.testPoint("DocumentsWriter.ThreadState.init after delTerm");    nextDocID++;    numDocsInRAM++;// We must at this point commit to flushing to ensure we// always get N docs when we flush by doc count, even if// > 1 thread is adding documents:if (!flushPending &&        maxBufferedDocs != IndexWriter.DISABLE_AUTO_FLUSH        && numDocsInRAM >= maxBufferedDocs) {      flushPending = true;      state.doFlushAfter = true;    }    success = true;  } finally {if (!success) {// Forcefully idle this ThreadState:      state.isIdle = true;      notifyAll();if (state.doFlushAfter) {        state.doFlushAfter = false;        flushPending = false;      }    }  }return state;}




3) 返回到DocumentWriter.updateDocument(Document, Analyzer, Term)中, 执行到:


try {// This call is not synchronized and does all the// workfinal DocWriter perDoc = state.consumer.processDocument();// This call is synchronized but fast  finishDocument(state, perDoc);  success = true;}




    这个模块是非常难以理解的. 首先讲讲Lucene 3.0.0中的consumer是怎样的. 

    其实Lucene 3.0.0中使用的是设计模式中的责任链模式, 在网上有很多的文章讲解这个模式, 责任链模式其实对于用户来说就是使得从用户的角度看到只有一个输入输出端口, 而程序根据自己的需要添加其他的操作, 下面我拿一个好理解的UML图出来大概说说, 使用了网上一篇文章的图.

![](http://www.yesky.com/SoftChannel/72342371961929728/20020415/jt-2002-4-15-flowerpassing2.gif)

     这类似于一个击鼓传花的游戏, 每个人都有一个下家,  击鼓者相当于客户, 传花者则为责任链中的一个一个的人,  击鼓者不用担心花会出现在谁的手上, 只需要传花者的一系列的传递就行了, 所有的具体类都是继承自传花者这个抽象类. 每个人可以接下花, 也可以交给下一个人, 这有点类似于Lucene中的索引文档的过程.



     读取文档 –> 分析Field内容 –> 1) 分词等等                - > 排序 –> ... 

                                             2) 加入Posting Table

                                             3) 计算Hash



     我们可以把其中的一些内容独立出来成为一个一个类, 这样可以有效减少程序的耦合性.

     下一章我将详细讲讲Lucene中的Index Chain与一些比较容易出现混淆的内容.












