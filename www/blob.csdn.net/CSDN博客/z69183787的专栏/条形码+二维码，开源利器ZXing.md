# 条形码+二维码，开源利器ZXing - z69183787的专栏 - CSDN博客
2015年08月05日 16:45:08[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1862
看到一篇：条形码+二维码，编码解码的文章
转自：[http://sjsky.iteye.com/blog/1142177](http://sjsky.iteye.com/blog/1142177)
继前面介绍的一个日本开源软件（该软件只能实现QRCode）原文： [Java实现二维码QRCode的编码和解码](http://jun1986.iteye.com/blog/1136934)([http://sjsky.iteye.com/blog/1136934](http://jun1986.iteye.com/blog/1136934) )，今发现又一优秀的开源利器--
 ZXing，相比而言它更加灵活方便，可以实现多种编码格式。
全文目录：
- 基本介绍
- 二维码（比如：QRCode）的编码和解码演示
- 条形码（比如：EAN-13）的编码和解码演示
【一】、基本介绍：
 1-1. ZXing是一个开源Java类库用于解析多种格式的条形码和二维码.
官网：[http://code.google.com/p/zxing/](http://code.google.com/p/zxing/)
截止目前为止最新版本为1.7，提供以下编码格式的支持：
- UPC-A and UPC-E
- EAN-8 and EAN-13
- Code 39
- Code 93
- Code 128
- QR Code
- ITF
- Codabar
- RSS-14 (all variants)
- Data Matrix
- PDF 417 ('alpha' quality)
- Aztec ('alpha' quality)
同时官网提供了 Android、cpp、C#、iPhone、j2me、j2se、jruby、objc、rim、symbian等多种应用的类库，具体详情可以参考下载的源码包中。
1-2. 本文和之前的那篇文章一样，主要是在PC上实现条形码（EAN-13）和二维码（QRCode） 的编码和解码的示例，以供大家参考，用到了源码中core和javase下面的相关源代码，附件提供自己编译之后的lib包：
- [zxing.jar](http://dl.iteye.com/topics/download/3f8d976d-b6da-3bd4-978e-18ac01cf627a)
- [zxing-j2se.jar](http://dl.iteye.com/topics/download/3ec63ac0-8f42-302f-bd1e-323732aff8f5)
有关各种手机系统的应用，有兴趣的朋友可以下载官方源码包，包下有具体详细的应用介绍。
【二】、二维码（QRCode）的编码和解码演示：
 2-1. 编码示例：
Java代码  ![收藏代码](http://jun1986.iteye.com/images/icon_star.png)
- package michael.zxing;     
- 
- import java.io.File;     
- import java.util.Hashtable;     
- 
- import com.google.zxing.BarcodeFormat;     
- import com.google.zxing.EncodeHintType;     
- import com.google.zxing.MultiFormatWriter;     
- import com.google.zxing.client.j2se.MatrixToImageWriter;     
- import com.google.zxing.common.BitMatrix;     
- import com.google.zxing.qrcode.decoder.ErrorCorrectionLevel;     
- 
- /**  
-  * @blog http://sjsky.iteye.com  
-  * @author Michael  
-  */
- publicclass ZxingEncoderHandler {     
- 
- /**  
-      * 编码  
-      * @param contents  
-      * @param width  
-      * @param height  
-      * @param imgPath  
-      */
- publicvoid encode(String contents, int width, int height, String imgPath) {     
-         Hashtable<Object, Object> hints = new Hashtable<Object, Object>();     
- // 指定纠错等级   
-         hints.put(EncodeHintType.ERROR_CORRECTION, ErrorCorrectionLevel.L);     
- // 指定编码格式   
-         hints.put(EncodeHintType.CHARACTER_SET, "GBK");     
- try {     
-             BitMatrix bitMatrix = new MultiFormatWriter().encode(contents,     
-                     BarcodeFormat.QR_CODE, width, height, hints);     
- 
-             MatrixToImageWriter     
-                     .writeToFile(bitMatrix, "png", new File(imgPath));     
- 
-         } catch (Exception e) {     
-             e.printStackTrace();     
-         }     
-     }     
- 
- /**  
-      * @param args  
-      */
- publicstaticvoid main(String[] args) {     
-         String imgPath = "d:/test/twocode/michael_zxing.png";     
-         String contents = "Hello Michael(大大),welcome to Zxing!"
-                 + "\nMichael’s blog [ http://sjsky.iteye.com ]"
-                 + "\nEMail [ sjsky007@gmail.com ]" + "\nTwitter [ @suncto ]";     
- int width = 300, height = 300;     
-         ZxingEncoderHandler handler = new ZxingEncoderHandler();     
-         handler.encode(contents, width, height, imgPath);     
- 
-         System.out.println("Michael ,you have finished zxing encode.");     
-     }     
- }    
运行后生成的二维码图片如下：
![](http://dl.iteye.com/upload/attachment/532163/e959e49a-e42c-373a-a7b3-8e2259a703fb.png)
 和前篇介绍一样，用手机的二维码扫描软件（本人用的：android 快拍二维码 ）来测试下，识别成功的截图如下：
![](http://dl.iteye.com/upload/attachment/532172/532a14f5-dace-3d3e-b585-c48a0a8bf60f.png)
2-2. 解码示例：
Java代码  ![收藏代码](http://jun1986.iteye.com/images/icon_star.png)
- package michael.zxing;     
- 
- import java.awt.image.BufferedImage;     
- import java.io.File;     
- import java.util.Hashtable;     
- 
- import javax.imageio.ImageIO;     
- 
- import com.google.zxing.BinaryBitmap;     
- import com.google.zxing.DecodeHintType;     
- import com.google.zxing.LuminanceSource;     
- import com.google.zxing.MultiFormatReader;     
- import com.google.zxing.Result;     
- import com.google.zxing.client.j2se.BufferedImageLuminanceSource;     
- import com.google.zxing.common.HybridBinarizer;     
- 
- /**  
-  * @blog http://sjsky.iteye.com  
-  * @author Michael  
-  */
- publicclass ZxingDecoderHandler {     
- 
- /**  
-      * @param imgPath  
-      * @return String  
-      */
- public String decode(String imgPath) {     
-         BufferedImage image = null;     
-         Result result = null;     
- try {     
-             image = ImageIO.read(new File(imgPath));     
- if (image == null) {     
-                 System.out.println("the decode image may be not exit.");     
-             }     
-             LuminanceSource source = new BufferedImageLuminanceSource(image);     
-             BinaryBitmap bitmap = new BinaryBitmap(new HybridBinarizer(source));     
- 
-             Hashtable<Object, Object> hints = new Hashtable<Object, Object>();     
-             hints.put(DecodeHintType.CHARACTER_SET, "GBK");     
- 
-             result = new MultiFormatReader().decode(bitmap, hints);     
- return result.getText();     
-         } catch (Exception e) {     
-             e.printStackTrace();     
-         }     
- returnnull;     
-     }     
- 
- /**  
-      * @param args  
-      */
- publicstaticvoid main(String[] args) {     
-         String imgPath = "d:/test/twocode/michael_zxing.png";     
-         ZxingDecoderHandler handler = new ZxingDecoderHandler();     
-         String decodeContent = handler.decode(imgPath);     
-         System.out.println("解码内容如下：");     
-         System.out.println(decodeContent);     
-         System.out.println("Michael ,you have finished zxing decode.");     
- 
-     }     
- }    
运行结果如下：
解码内容如下：
Hello Michael(大大),welcome to Zxing!
Michael’s blog [ http://sjsky.iteye.com ]
EMail [ sjsky007@gmail.com ]
Twitter [ @suncto ]
Michael ,you have finished zxing decode.
  从测试结果可见：解码出的内容和之前编码的内容是一致
【三】、条形码（EAN-13）的编码和解码演示：
 3-1. 编码示例：
Java代码  ![收藏代码](http://jun1986.iteye.com/images/icon_star.png)
- package michael.zxing;     
- 
- import java.io.File;     
- 
- import com.google.zxing.BarcodeFormat;     
- import com.google.zxing.MultiFormatWriter;     
- import com.google.zxing.client.j2se.MatrixToImageWriter;     
- import com.google.zxing.common.BitMatrix;     
- 
- /**  
-  * @blog http://sjsky.iteye.com  
-  * @author Michael  
-  */
- publicclass ZxingEAN13EncoderHandler {     
- 
- /**  
-      * 编码  
-      * @param contents  
-      * @param width  
-      * @param height  
-      * @param imgPath  
-      */
- publicvoid encode(String contents, int width, int height, String imgPath) {     
- int codeWidth = 3 + // start guard   
-                 (7 * 6) + // left bars   
- 5 + // middle guard   
-                 (7 * 6) + // right bars   
- 3; // end guard   
-         codeWidth = Math.max(codeWidth, width);     
- try {     
-             BitMatrix bitMatrix = new MultiFormatWriter().encode(contents,     
-                     BarcodeFormat.EAN_13, codeWidth, height, null);     
- 
-             MatrixToImageWriter     
-                     .writeToFile(bitMatrix, "png", new File(imgPath));     
- 
-         } catch (Exception e) {     
-             e.printStackTrace();     
-         }     
-     }     
- 
- /**  
-      * @param args  
-      */
- publicstaticvoid main(String[] args) {     
-         String imgPath = "d:/test/twocode/zxing_EAN13.png";     
- // 益达无糖口香糖的条形码   
-         String contents = "6923450657713";     
- 
- int width = 105, height = 50;     
-         ZxingEAN13EncoderHandler handler = new ZxingEAN13EncoderHandler();     
-         handler.encode(contents, width, height, imgPath);     
- 
-         System.out.println("Michael ,you have finished zxing EAN13 encode.");     
-     }     
- }    
6 923450 657713 对应的是益达无糖口香糖：
![](http://dl.iteye.com/upload/attachment/532281/3f0858b6-a3b3-3323-a0ea-86264ea695df.jpg)
运行后生成的条形码图片如下：
![](http://dl.iteye.com/upload/attachment/532270/5d869b86-6f29-3888-a581-4520c1c089a2.png)
用手机的扫描软件，识别成功的截图如下：
![](http://dl.iteye.com/upload/attachment/532285/c64a6fcb-9d8a-36db-845e-244a4156bd79.png)
3-2. 解码示例：
Java代码  ![收藏代码](http://jun1986.iteye.com/images/icon_star.png)
- package michael.zxing;     
- 
- import java.awt.image.BufferedImage;     
- import java.io.File;     
- 
- import javax.imageio.ImageIO;     
- 
- import com.google.zxing.BinaryBitmap;     
- import com.google.zxing.LuminanceSource;     
- import com.google.zxing.MultiFormatReader;     
- import com.google.zxing.Result;     
- import com.google.zxing.client.j2se.BufferedImageLuminanceSource;     
- import com.google.zxing.common.HybridBinarizer;     
- 
- /**  
-  * @blog http://sjsky.iteye.com  
-  * @author Michael  
-  */
- publicclass ZxingEAN13DecoderHandler {     
- 
- /**  
-      * @param imgPath  
-      * @return String  
-      */
- public String decode(String imgPath) {     
-         BufferedImage image = null;     
-         Result result = null;     
- try {     
-             image = ImageIO.read(new File(imgPath));     
- if (image == null) {     
-                 System.out.println("the decode image may be not exit.");     
-             }     
-             LuminanceSource source = new BufferedImageLuminanceSource(image);     
-             BinaryBitmap bitmap = new BinaryBitmap(new HybridBinarizer(source));     
- 
-             result = new MultiFormatReader().decode(bitmap, null);     
- return result.getText();     
-         } catch (Exception e) {     
-             e.printStackTrace();     
-         }     
- returnnull;     
-     }     
- 
- /**  
-      * @param args  
-      */
- publicstaticvoid main(String[] args) {     
-         String imgPath = "d:/test/twocode/zxing_EAN13.png";     
-         ZxingEAN13DecoderHandler handler = new ZxingEAN13DecoderHandler();     
-         String decodeContent = handler.decode(imgPath);     
-         System.out.println("解码内容如下：");     
-         System.out.println(decodeContent);     
-         System.out.println("Michael ,you have finished zxing EAN-13 decode.");     
- 
-     }     
- }    
 运行结果如下：
写道
解码内容如下：
6923450657713
Michael ,you have finished zxing decode.
从测试结果可见：解码出的内容和之前编码的内容是一致。
