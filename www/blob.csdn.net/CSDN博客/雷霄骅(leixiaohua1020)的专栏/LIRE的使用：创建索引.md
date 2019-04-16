# LIRE的使用：创建索引 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年09月20日 16:04:52[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：10256








LIRE（Lucene Image REtrieval）提供一种的简单方式来创建基于图像特性的Lucene索引。利用该索引就能够构建一个基于内容的图像检索(content- based image retrieval，CBIR)系统，来搜索相似的图像。LIRE使用的特性都取自MPEG-7标准： ScalableColor、ColorLayout、EdgeHistogram。



使用`DocumentBuilderFactory` 创建 `DocumentBuilder`，例如`DocumentBuilderFactory.getCEDDDocumentBuilder()`. 

将图片加入索引index 需要以下2步：
- 
使用 `DocumentBuilder` 创建Document：`builder.createDocument(FileInputStream, String)`.（第一个参数是图片文件）

- 
将document 加入 index.


 LIRE支持很多种的特征值。具体可以看 `DocumentBuilderFactory` 类的源代码。也可以使用 `ChainedDocumentBuilder` 同时使用多种特征值。

创建索引的方法如下代码所示

```java
/**
 * Simple index creation with Lire
 *
 * @author Mathias Lux, mathias@juggle.at
 */
public class Indexer {
    public static void main(String[] args) throws IOException {
        // Checking if arg[0] is there and if it is a directory.
        boolean passed = false;
        if (args.length > 0) {
            File f = new File(args[0]);
            System.out.println("Indexing images in " + args[0]);
            if (f.exists() && f.isDirectory()) passed = true;
        }
        if (!passed) {
            System.out.println("No directory given as first argument.");
            System.out.println("Run \"Indexer <directory>\" to index files of a directory.");
            System.exit(1);
        }
        // Getting all images from a directory and its sub directories.
        ArrayList<String> images = FileUtils.getAllImages(new File(args[0]), true);
 
        // Creating a CEDD document builder and indexing al files.
        DocumentBuilder builder = DocumentBuilderFactory.getCEDDDocumentBuilder();
        // Creating an Lucene IndexWriter
        IndexWriterConfig conf = new IndexWriterConfig(LuceneUtils.LUCENE_VERSION,
                new WhitespaceAnalyzer(LuceneUtils.LUCENE_VERSION));
        IndexWriter iw = new IndexWriter(FSDirectory.open(new File("index")), conf);
        // Iterating through images building the low level features
        for (Iterator<String> it = images.iterator(); it.hasNext(); ) {
            String imageFilePath = it.next();
            System.out.println("Indexing " + imageFilePath);
            try {
                BufferedImage img = ImageIO.read(new FileInputStream(imageFilePath));
                Document document = builder.createDocument(img, imageFilePath);
                iw.addDocument(document);
            } catch (Exception e) {
                System.err.println("Error reading image or indexing it.");
                e.printStackTrace();
            }
        }
        // closing the IndexWriter
        iw.close();
        System.out.println("Finished indexing.");
    }
}
```
原文：
[http://www.semanticmetadata.net/wiki/doku.php?id=lire:createindex](http://www.semanticmetadata.net/wiki/doku.php?id=lire:createindex)





