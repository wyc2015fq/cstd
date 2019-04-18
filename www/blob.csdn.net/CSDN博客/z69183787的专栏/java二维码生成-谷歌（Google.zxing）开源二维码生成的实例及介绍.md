# java二维码生成-谷歌（Google.zxing）开源二维码生成的实例及介绍 - z69183787的专栏 - CSDN博客
2017年10月24日 17:23:02[OkidoGreen](https://me.csdn.net/z69183787)阅读数：707
[http://www.cnblogs.com/lanxiamo/p/6293580.html](http://www.cnblogs.com/lanxiamo/p/6293580.html)
这里我们使用**比特矩阵(位矩阵)的QR码编码在缓冲图片上画出二维码**
**　　实例有以下一个传入***参数*
**　　　　　　OutputStream outputStream, 要存储的文件**
**　　　　　　String content, 携带信息的内容**
**　　　　　　int qrCodeSize, 图片大小**
**　　　　　　String imageFormat 编码**
**　　步骤：**
**　　1.设置二维码的纠错级别参数**
```
//设置二维码纠错级别ＭＡＰ
       Hashtable<EncodeHintType, ErrorCorrectionLevel> hintMap = new Hashtable<EncodeHintType, ErrorCorrectionLevel>(); 
       hintMap.put(EncodeHintType.ERROR_CORRECTION, ErrorCorrectionLevel.L); // 矫错级别
```
**   2.创建比特矩阵**
```
1 QRCodeWriter qrCodeWriter = new QRCodeWriter(); 
2 //创建比特矩阵(位矩阵)的QR码编码的字符串 
3 BitMatrix byteMatrix = qrCodeWriter.encode(content, BarcodeFormat.QR_CODE, qrCodeSize, qrCodeSize, hintMap); 
4 // 使BufferedImage勾画QRCode (matrixWidth 是行二维码像素点)
5 int matrixWidth = byteMatrix.getWidth();
```
**3.开始在缓冲图片中画二维码**
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1  BufferedImage image = new BufferedImage(matrixWidth, matrixWidth, BufferedImage.TYPE_INT_RGB);  
 2             image.createGraphics();  
 3             Graphics2D graphics = (Graphics2D) image.getGraphics();  
 4             graphics.setColor(Color.WHITE);  
 5             graphics.fillRect(0, 0, matrixWidth, matrixWidth);  
 6             // 使用比特矩阵画并保存图像
 7             graphics.setColor(Color.BLACK);  
 8             for (int i = 0; i < matrixWidth; i++){
 9                 for (int j = 0; j < matrixWidth; j++){
10                     if (byteMatrix.get(i, j)){
11                         graphics.fillRect(i, j, 1, 1);  
12                     }
13                 }
14             }
15             ImageIO.write(image, imageFormat, outputStream);  
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
***二维码生成的工具类代码；***
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
 1 package 。。。;
 2 
 3 import java.awt.Color;
 4 import java.awt.Graphics2D;
 5 import java.awt.image.BufferedImage;
 6 import java.io.File;
 7 import java.io.FileInputStream;
 8 import java.io.FileOutputStream;
 9 import java.io.IOException;
10 import java.io.InputStream;
11 import java.io.OutputStream;
12 import java.util.Hashtable;
13 
14 import javax.imageio.ImageIO;
15 
16 import com.google.zxing.BarcodeFormat;
17 import com.google.zxing.BinaryBitmap;
18 import com.google.zxing.EncodeHintType;
19 import com.google.zxing.LuminanceSource;
20 import com.google.zxing.ReaderException;
21 import com.google.zxing.Result;
22 import com.google.zxing.WriterException;
23 import com.google.zxing.client.j2se.BufferedImageLuminanceSource;
24 import com.google.zxing.common.BitMatrix;
25 import com.google.zxing.common.HybridBinarizer;
26 import com.google.zxing.qrcode.QRCodeReader;
27 import com.google.zxing.qrcode.QRCodeWriter;
28 import com.google.zxing.qrcode.decoder.ErrorCorrectionLevel;
29 
30 /**
31  * 二维码生成和读的工具类
32  *
33  */
34 public class QrCodeCreateUtil {
35     
36     /**
37      * 生成包含字符串信息的二维码图片
38      * @param outputStream 文件输出流路径
39      * @param content 二维码携带信息
40      * @param qrCodeSize 二维码图片大小
41      * @param imageFormat 二维码的格式
42      * @throws WriterException 
43      * @throws IOException 
44      */
45     public static boolean createQrCode(OutputStream outputStream, String content, int qrCodeSize, String imageFormat) throws WriterException, IOException{  
46             //设置二维码纠错级别ＭＡＰ
47             Hashtable<EncodeHintType, ErrorCorrectionLevel> hintMap = new Hashtable<EncodeHintType, ErrorCorrectionLevel>();  
48             hintMap.put(EncodeHintType.ERROR_CORRECTION, ErrorCorrectionLevel.L);  // 矫错级别  
49             QRCodeWriter qrCodeWriter = new QRCodeWriter();  
50             //创建比特矩阵(位矩阵)的QR码编码的字符串  
51             BitMatrix byteMatrix = qrCodeWriter.encode(content, BarcodeFormat.QR_CODE, qrCodeSize, qrCodeSize, hintMap);  
52             // 使BufferedImage勾画QRCode  (matrixWidth 是行二维码像素点)
53             int matrixWidth = byteMatrix.getWidth();  
54             BufferedImage image = new BufferedImage(matrixWidth-200, matrixWidth-200, BufferedImage.TYPE_INT_RGB);  
55             image.createGraphics();  
56             Graphics2D graphics = (Graphics2D) image.getGraphics();  
57             graphics.setColor(Color.WHITE);  
58             graphics.fillRect(0, 0, matrixWidth, matrixWidth);  
59             // 使用比特矩阵画并保存图像
60             graphics.setColor(Color.BLACK);  
61             for (int i = 0; i < matrixWidth; i++){
62                 for (int j = 0; j < matrixWidth; j++){
63                     if (byteMatrix.get(i, j)){
64                         graphics.fillRect(i-100, j-100, 1, 1);  
65                     }
66                 }
67             }
68             return ImageIO.write(image, imageFormat, outputStream);  
69     }  
70       
71     /**
72      * 读二维码并输出携带的信息
73      */
74     public static void readQrCode(InputStream inputStream) throws IOException{  
75         //从输入流中获取字符串信息
76         BufferedImage image = ImageIO.read(inputStream);  
77         //将图像转换为二进制位图源
78         LuminanceSource source = new BufferedImageLuminanceSource(image);  
79         BinaryBitmap bitmap = new BinaryBitmap(new HybridBinarizer(source));  
80         QRCodeReader reader = new QRCodeReader();  
81         Result result = null ;  
82         try {
83          result = reader.decode(bitmap);  
84         } catch (ReaderException e) {
85             e.printStackTrace();  
86         }
87         System.out.println(result.getText());  
88     }
89     /**
90      * 测试代码
91      * @throws WriterException 
92      */
93     public static void main(String[] args) throws IOException, WriterException {  
94         
95         createQrCode(new FileOutputStream(new File("d:\\qrcode.jpg")),"WE1231238239128sASDASDSADSDWEWWREWRERWSDFDFSDSDF123123123123213123",900,"JPEG");
96         readQrCode(new FileInputStream(new File("d:\\qrcode.jpg")));  
97     }  
98   
99 }  
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
