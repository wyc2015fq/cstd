# LIRE的使用：搜索相似的图片 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年09月20日 16:13:31[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：5943








使用 `ImageSearcherFactory` 创建 `ImageSearcher。`例如`ImageSearcherFactory.createDefaultSearcher()`。

`ImageSearcher` 可以通过`InputStream` 或`BufferedImage`，或者一个描述图像的Lucene的 `Document` 进行检索。 例如使用`search(BufferedImage, IndexReader)` 或者`search(Document, IndexReader)`.

返回的结果是一个 `ImageSearchHits` 类似于Lucene 中的`Hits。`

``

```java
/**
 * Simple image retrieval with Lire
 * @author Mathias Lux, mathias <at> juggle <dot> at
 */
public class Searcher {
    public static void main(String[] args) throws IOException {
        // Checking if arg[0] is there and if it is an image.
        BufferedImage img = null;
        boolean passed = false;
        if (args.length > 0) {
            File f = new File(args[0]);
            if (f.exists()) {
                try {
                    img = ImageIO.read(f);
                    passed = true;
                } catch (IOException e) {
                    e.printStackTrace();  
                }
            }
        }
        if (!passed) {
            System.out.println("No image given as first argument.");
            System.out.println("Run \"Searcher <query image>\" to search for <query image>.");
            System.exit(1);
        }
 
        IndexReader ir = DirectoryReader.open(FSDirectory.open(new File("index")));
        ImageSearcher searcher = ImageSearcherFactory.createCEDDImageSearcher(10);
 
        ImageSearchHits hits = searcher.search(img, ir);
        for (int i = 0; i < hits.length(); i++) {
            String fileName = hits.doc(i).getValues(DocumentBuilder.FIELD_NAME_IDENTIFIER)[0];
            System.out.println(hits.score(i) + ": \t" + fileName);
        }
    }
}
```







