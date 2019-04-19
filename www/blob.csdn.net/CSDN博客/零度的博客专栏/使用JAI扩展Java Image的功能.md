# 使用JAI扩展Java Image的功能 - 零度的博客专栏 - CSDN博客
2017年02月28日 11:19:52[零度anngle](https://me.csdn.net/zmx729618)阅读数：1883
Java Image功能一直都在增强，但是向磁盘写一个图像文件或者返回一个PNG或JPEG依然比较难实现。但是我们可以使用Java Advanced Imaging（JAI）API来解决这个问题。JAI可以从SUN的Java站点下载，它包含在JDK 1.4的javax.imageio包中。要安装JAI，你需要三个jar文件，mlibwrapper_jai.jar, jai_codec.jar, and jai_core.jar放到你的classpath中。推荐将他们放到你的JDK的jre/lib/ext目录下。
对于Java来说，JAI是一个奇怪的系统，不像其他的方面有很多方法需要了解，这里只有一个叫做JAI的高级类和一些辅助方法。这些方法的第一个参数是一个操作类型名字，所以我们将会看到这样的代码：
source = JAI.create("fileload", .. );
JAI.create("extrema", src, ...);
JAI.create("histogram", src, ...); 
在这个系统中，你可以很容易的添加你自己的或者第三方的功能，对类型的要求比较松，所以利用它进行开发难度比较大。
将一个AWT图像转化成一个PNG文件需要下面的一小段代码：
import java.awt.Image;
import java.awt.image.renderable.ParameterBlock;
import javax.media.jai.JAI;
import javax.media.jai.PlanarImage;
.....
Image img = ....
OutputStream out =....
ParameterBlock pb = new ParameterBlock().add(img);
PlanarImage src = (PlanarImage) JAI.create("awtImage", pb);
JAI.create("encode", src, out, "PNG", null); 
从上面的例子可以看出向create方法传递参数有两种方法。新方法是使用ParameterBlock包含所有的参数。我们使用这种方法实现了awtImage操作。老方法是则使用一个重载方法，现在已不提倡使用，但是我们可以使用这种方法实现解码操作。上面的代码将把一个Image编码成为一个PNG并将其写入OutputStream中。
### 实例Java jai 读取tif文件页数：
- **import** java.io.File;  
- import java.io.IOException;  
- 
- import com.sun.media.jai.codec.FileSeekableStream;  
- import com.sun.media.jai.codec.ImageCodec;  
- import com.sun.media.jai.codec.ImageDecoder;  
- import com.sun.media.jai.codec.SeekableStream;  
- import com.sun.media.jai.codec.TIFFDecodeParam;  
- import com.sun.media.jai.codec.TIFFDirectory;  
- 
- publicclass TifTest {  
- publicstaticvoid readTifFIle(String fileName) {  
-         File file = new File(fileName);  
-         SeekableStream s = null;  
-         TIFFDecodeParam param = null;  
- try {  
-             s = new FileSeekableStream(file);  
-             ImageDecoder dec = ImageCodec.createImageDecoder("tiff", s, param);  
- int numofpages;  
-             numofpages = dec.getNumPages();  
-             System.out.print(numofpages);  
-         } catch (IOException e1) {  
-             e1.printStackTrace();  
-         }  
-     }  
- //读取文件的 页数
- publicstaticvoid readTifFIle2(String fileName){  
-         File file = new File(fileName);  
-         SeekableStream s = null;  
- try {  
-             s = new FileSeekableStream(file);  
-             System.out.println(TIFFDirectory.getNumDirectories(s));  
-         } catch (IOException e) {  
-             e.printStackTrace();  
-         }  
-     }  
- 
- publicstaticvoid main(String[] args) {  
-         String fileName = "C:\\证明.tif";  
-         readTifFIle2(fileName);  
-     }  
- 
- /*  public static int DISPLAY_WIDTH = 300;
- 
-     public static boolean doTiff2JPEG (String filename, String imageDir) {
-         File file = new File(imageDir , filename);
-         SeekableStream s = null;
-         TIFFDecodeParam param = null;
-         RenderedImage op = null;
-         String simplefilename = filename.substring(0,filename.lastIndexOf("."));
- try {
-           s = new FileSeekableStream(file);
-           ImageDecoder dec = (ImageDecoder) ImageCodec.createImageDecoder("tiff", s, param);
- int numofpages = dec.getNumPages();
- for (int i=0; i < numofpages; i++) {
-             op = new NullOpImage
-                (dec.decodeAsRenderedImage(i),null,null,OpImage.OP_COMPUTE_BOUND);
- int width = op.getWidth();
- int height = op.getHeight();
- double conversionFactor = (double)DISPLAY_WIDTH / (double)width;
- int thumbHeight = (int)((double)height * conversionFactor);
- int thumbWidth = (int)((double)width * conversionFactor);
-             Dimension dim = new Dimension(thumbHeight, thumbWidth);       
-             JAI.setDefaultRenderingSize(dim);
-             JAI.setDefaultTileSize(dim);
-             JAI.create("filestore",
-                     op, imageDir <i>File.separator</i> simplefilename + "." + i + ".jpg", "png");
- }
- } catch (IOException e) {
-           System.out.println(e.getLocalizedMessage());
- return false;
- }
- return true;
- }
- 
-     public static void main(String[] args) {
-         TifTest.doTiff2JPEG("aa.tiff", "d:temp");
-     }*/
- }  
