# Lucene 3.0.0 之样例解析(5)-IndexFiles.java - LeftNotEasy - 博客园







# [Lucene 3.0.0 之样例解析(5)-IndexFiles.java](https://www.cnblogs.com/LeftNotEasy/archive/2010/01/10/1643464.html)





    之前我谈到了把Term加入到Posting表中的过程, 接下来我将大致说说把Posting表写入到文件中的过程.

    这些内容我看得不算仔细, 之后有时间可以详细的扩展一下, 先留个**脚印**在这儿 :-D

    之前的准备:

![image](https://images.cnblogs.com/cnblogs_com/LeftNotEasy/WindowsLiveWriter/Lucene3.0.05IndexFiles.java_B71A/image_thumb.png)

    这六步就是写入前的前期准备工作, 当调用了IndexWriter.Close()过程后, 写入就开始了. 第6)步是新建一个段索引的状态, 接下来的步骤是:

![image](https://images.cnblogs.com/cnblogs_com/LeftNotEasy/WindowsLiveWriter/Lucene3.0.05IndexFiles.java_B71A/image_thumb_1.png)

    这样一个顺序的调用使得在多个线程和多个文档之中的Posting表得以合并, 这几个方法很是重要, 可以详细阅读一下, 这层层的调用关系有点类似于Google Map那种感觉, 从宇宙的层面上来看, 什么都看不清, 要想知道细节, 就得深入街道的层次, 给一本书给你, 如果只看到封面, 肯定不能写出阅读心得, 只有一页一页的翻开.

    上面的这张图就是这个过程, 翻开每一页, 然后写入, 写入的过程也是有道理的, 我就不画图了, 直接抄上一段Lucene的注释:


Current writer chain:  FormatPostingsFieldsConsumer    -> IMPL: FormatPostingsFieldsWriter      -> FormatPostingsTermsConsumer        -> IMPL: FormatPostingsTermsWriter          -> FormatPostingsDocConsumer            -> IMPL: FormatPostingsDocWriter              -> FormatPostingsPositionsConsumer                -> IMPL: FormatPostingsPositionsWriter






     这里就是真正负责写入的类, 在10)中的方法里进行初始化和调用, 可以深入的阅读一下.

     (本节写得比较草率, 希望以后有时间再完善一点)

     接下来我将对检索文件时候的过程进行一些讲解












