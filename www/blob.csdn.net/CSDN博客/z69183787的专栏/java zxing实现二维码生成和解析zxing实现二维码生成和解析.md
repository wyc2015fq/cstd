# java zxing实现二维码生成和解析zxing实现二维码生成和解析 - z69183787的专栏 - CSDN博客
2018年03月12日 10:23:26[OkidoGreen](https://me.csdn.net/z69183787)阅读数：738
[https://www.cnblogs.com/zhangzhen894095789/p/6623041.html](https://www.cnblogs.com/zhangzhen894095789/p/6623041.html)
### [zxing实现二维码生成和解析](http://kesun-shy.iteye.com/blog/2154169)
- 
- [二维码 zxing](http://kesun-shy.iteye.com/category/327154)
二维码的生成与解析。有多种途径。我选择用大品牌，google老大的zxing。
gitHub链接是(我用的3.0.0，已经是nio了)
[https://github.com/zxing/zxing/tree/zxing-3.0.0](https://github.com/zxing/zxing/tree/zxing-3.0.0)
Java代码  ![收藏代码](http://kesun-shy.iteye.com/images/icon_star.png)
- // 其中输出图像和读取图像的类在core包  
- 
- 
MultiFormatReader  
- 
- 
MultiFormatWriter  
- 
- // 生成矩阵的类在javase的包里  
- 
- 
MatrixToImageWriter  
pom.xml中的配置为
Xml代码  ![收藏代码](http://kesun-shy.iteye.com/images/icon_star.png)
- <project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"  
- xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">  
- <modelVersion>4.0.0</modelVersion>  
- 
- <groupId>com.shihy</groupId>  
- <artifactId>qrcode</artifactId>  
- <version>0.0.1-SNAPSHOT</version>  
- <packaging>jar</packaging>  
- 
- <name>qrcode</name>  
- <url>http://maven.apache.org</url>  
- 
- <properties>  
- <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>  
- </properties>  
- 
- <dependencies>  
- <dependency>  
- <groupId>com.google.zxing</groupId>  
- <artifactId>core</artifactId>  
- <version>3.0.0</version>  
- </dependency>  
- <dependency>  
- <groupId>com.google.zxing</groupId>  
- <artifactId>javase</artifactId>  
- <version>3.0.0</version>  
- </dependency>  
- <dependency>  
- <groupId>junit</groupId>  
- <artifactId>junit</artifactId>  
- <version>4.10</version>  
- </dependency>  
- <dependency>  
- <groupId>com.alibaba</groupId>  
- <artifactId>fastjson</artifactId>  
- <version>1.1.29</version>  
- </dependency>  
- </dependencies>  
- </project>  
生成图像与解析图像的测试工具类：
Java代码  ![收藏代码](http://kesun-shy.iteye.com/images/icon_star.png)
- package com.polysaas.edu.qrcode;  
- 
- import java.awt.image.BufferedImage;  
- import java.io.File;  
- import java.io.IOException;  
- import java.nio.file.FileSystems;  
- import java.nio.file.Path;  
- import java.util.HashMap;  
- import java.util.Map;  
- 
- import javax.imageio.ImageIO;  
- 
- import org.junit.Test;  
- 
- import com.alibaba.fastjson.JSONObject;  
- import com.google.zxing.BarcodeFormat;  
- import com.google.zxing.Binarizer;  
- import com.google.zxing.BinaryBitmap;  
- import com.google.zxing.DecodeHintType;  
- import com.google.zxing.EncodeHintType;  
- import com.google.zxing.LuminanceSource;  
- import com.google.zxing.MultiFormatReader;  
- import com.google.zxing.MultiFormatWriter;  
- import com.google.zxing.NotFoundException;  
- import com.google.zxing.Result;  
- import com.google.zxing.WriterException;  
- import com.google.zxing.client.j2se.BufferedImageLuminanceSource;  
- import com.google.zxing.client.j2se.MatrixToImageWriter;  
- import com.google.zxing.common.BitMatrix;  
- import com.google.zxing.common.HybridBinarizer;  
- 
- public class QRCodeTest {  
- 
- /** 
-      * 生成图像 
-      *  
-      * @throws WriterException 
-      * @throws IOException 
-      */  
- @Test  
- public void testEncode() throws WriterException, IOException {  
- 
        String filePath = "D://";  
- 
        String fileName = "zxing.png";  
- 
        JSONObject json = new JSONObject();  
- 
        json.put(  
- "zxing",  
- "https://github.com/zxing/zxing/tree/zxing-3.0.0/javase/src/main/java/com/google/zxing");  
- 
        json.put("author", "shihy");  
- 
        String content = json.toJSONString();// 内容  
- int width = 200; // 图像宽度  
- int height = 200; // 图像高度  
- 
        String format = "png";// 图像类型  
- 
        Map<EncodeHintType, Object> hints = new HashMap<EncodeHintType, Object>();  
- 
        hints.put(EncodeHintType.CHARACTER_SET, "UTF-8");  
- 
        BitMatrix bitMatrix = new MultiFormatWriter().encode(content,  
- 
                BarcodeFormat.QR_CODE, width, height, hints);// 生成矩阵  
- 
        Path path = FileSystems.getDefault().getPath(filePath, fileName);  
- 
        MatrixToImageWriter.writeToPath(bitMatrix, format, path);// 输出图像  
- 
        System.out.println("输出成功.");  
- 
    }  
- 
- /** 
-      * 解析图像 
-      */  
- @Test  
- public void testDecode() {  
- 
        String filePath = "D://zxing.png";  
- 
        BufferedImage image;  
- try {  
- 
            image = ImageIO.read(new File(filePath));  
- 
            LuminanceSource source = new BufferedImageLuminanceSource(image);  
- 
            Binarizer binarizer = new HybridBinarizer(source);  
- 
            BinaryBitmap binaryBitmap = new BinaryBitmap(binarizer);  
- 
            Map<DecodeHintType, Object> hints = new HashMap<DecodeHintType, Object>();  
- 
            hints.put(DecodeHintType.CHARACTER_SET, "UTF-8");  
- 
            Result result = new MultiFormatReader().decode(binaryBitmap, hints);// 对图像进行解码  
- 
            JSONObject content = JSONObject.parseObject(result.getText());  
- 
            System.out.println("图片中内容：  ");  
- 
            System.out.println("author： " + content.getString("author"));  
- 
            System.out.println("zxing：  " + content.getString("zxing"));  
- 
            System.out.println("图片中格式：  ");  
- 
            System.out.println("encode： " + result.getBarcodeFormat());  
- 
        } catch (IOException e) {  
- 
            e.printStackTrace();  
- 
        } catch (NotFoundException e) {  
- 
            e.printStackTrace();  
- 
        }  
- 
    }  
- 
}  
测试生成图像为：
![](http://dl2.iteye.com/upload/attachment/0103/0259/f1c4cf6c-c825-3504-8c9e-97019e3796d1.png)
解析到的内容为：
Java代码  ![收藏代码](http://kesun-shy.iteye.com/images/icon_star.png)
- 
图片中内容：    
- 
author： shihy  
- 
zxing：  https://github.com/zxing/zxing/tree/zxing-3.0.0/javase/src/main/java/com/google/zxing  
- 
图片中格式：    
- 
encode： QR_CODE  
