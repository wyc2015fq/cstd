# 使用lucene 3.0.0 索引和检索中文文件 - LeftNotEasy - 博客园







# [使用lucene 3.0.0 索引和检索中文文件](https://www.cnblogs.com/LeftNotEasy/archive/2010/01/14/1647775.html)







**一. 我本来的程序**

    其实我本来的程序挺简单, 完全修改自Demo里面的SearchFiles和IndexFiles. 唯一不同的是引用了SmartCN的分词器.

    我把修改那一点的代码贴出来.

    IndexhChinese.java:


Date start = new Date();try {  IndexWriter writer = new IndexWriter(FSDirectory.open(INDEX_DIR), new SmartChineseAnalyzer(Version.LUCENE_CURRENT), true, IndexWriter.MaxFieldLength.LIMITED);  indexDocs(writer, docDir);  System.out.println("Indexing to directory '" +INDEX_DIR+ "'...");  System.out.println("Optimizing...");//writer.optimize();  writer.close();  Date end = new Date();  System.out.println(end.getTime() - start.getTime() + " total milliseconds");} 


    SearchChinese.java


Analyzer analyzer = new SmartChineseAnalyzer(Version.LUCENE_CURRENT);BufferedReader in = null;if (queries != null) {  in = new BufferedReader(new FileReader(queries));} else {  in = new BufferedReader(new InputStreamReader(System.in, "GBK"));}


     在这里, 我制定了输入的查询是采用GBK编码的.

     然后我充满信心的运行后......发现无法检索出中文, 里面的英文检索是正常的.



**二. 发现问题.**

     于是我就郁闷了, 由于对于java与lucene都是太熟悉, 而且用的3.0.0版外面的讨论又不是太多, 就瞎折腾了一会儿, 发现我如果把文件的格式另存为ansi就可以检索中文了(以前是utf-8的), 看来是文件编码的问题, 摸索了一下, 在indexChinese.java中发现了如下的代码:


staticvoid indexDocs(IndexWriter writer, File file)throws IOException {// do not try to index files that cannot be readif (file.canRead()) {if (file.isDirectory()) {      String[] files = file.list();// an IO error could occurif (files != null) {for (int i = 0; i < files.length; i++) {          indexDocs(writer, new File(file, files[i]));        }      }    } else {      System.out.println("adding " + file);try {        writer.addDocument(FileDocument.Document(file));      }// at least on windows, some temporary files raise this exception with an "access denied" message// checking if the file can be read doesn't helpcatch (FileNotFoundException fnfe) {        ;      }    }  }


     重点在于这一句:


try {  writer.addDocument(FileDocument.Document(file));}


    读取文件的代码应该就在这里面, 跟踪进去:


publicstatic Document Document(File f)throws java.io.FileNotFoundException, UnsupportedEncodingException {  Document doc = new Document();  doc.add(new Field("path", f.getPath(), Field.Store.YES, Field.Index.NOT_ANALYZED));  doc.add(new Field("modified",      DateTools.timeToString(f.lastModified(), DateTools.Resolution.MINUTE),      Field.Store.YES, Field.Index.NOT_ANALYZED));  doc.add(new Field("contents", FileReader(f)));// return the documentreturn doc;}private FileDocument() {}}




     这是Lucene的一个内部类, 作用就是从一个文本文件中获取内容, 生成的Document默认有3个域: path, modified, content, 而content就是文件的文本内容了. 看来是FileReader(f), 这个函数出了问题了, 根本没有制定采用什么编码进行读取啊, 于是把这儿简单的修改了一下.


FileInputStream fis=new FileInputStream(f);//   按照 UTF-8 编码方式将字节流转化为字符流InputStreamReader isr=new InputStreamReader(fis,"UNICODE");//   从字符流中获取文本并进行缓冲BufferedReader br=new BufferedReader(isr);doc.add(new Field("contents", br));


     至于那个"Unicode"可以修改为支持的所有编码, 当我修改为"utf-8"后就可以正常使用了.



**三. 一些猜测:**

     对于Lucene索引文件的时候, 编码是没有关系的, 只要正确指定了, 那么输出的文件都是可以被正常检索到的, 也就是说, 不同的编码文件索引后的结果一样(求证)












