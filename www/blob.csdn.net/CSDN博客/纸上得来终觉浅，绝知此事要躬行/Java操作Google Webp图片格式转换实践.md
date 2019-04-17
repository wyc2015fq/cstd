# Java操作Google Webp图片格式转换实践 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2019年03月27日 17:23:13[boonya](https://me.csdn.net/boonya)阅读数：89








**目录**

[Java使用webp代码编译](#%C2%A0Java%E4%BD%BF%E7%94%A8webp%E4%BB%A3%E7%A0%81%E7%BC%96%E8%AF%91)

[Webp开源项目](#Webp%E5%BC%80%E6%BA%90%E9%A1%B9%E7%9B%AE)

[nintha/webp-imageio-core](#nintha%2Fwebp-imageio-core)

[qwong/j-webp](#qwong%2Fj-webp)

[可用的webp依赖和包](#%E5%8F%AF%E7%94%A8%E7%9A%84webp%E4%BE%9D%E8%B5%96%E5%92%8C%E5%8C%85)

[Webp 编解码](#Webp%20%E7%BC%96%E8%A7%A3%E7%A0%81)

[编码](#%E7%BC%96%E7%A0%81)

[解码](#%E8%A7%A3%E7%A0%81)

[Webp遇到的坑](#Webp%E9%81%87%E5%88%B0%E7%9A%84%E5%9D%91)

##  Java使用webp代码编译
- 
webp-imageio
[开源库](https://link.jianshu.com?t=https%3A%2F%2Fbitbucket.org%2Fluciad%2Fwebp-imageio%2Fsrc) 下载压缩包到本地进行编译jar，工程用gradle来维护。

- 
libwebp
[google-developer](https://link.jianshu.com?t=https%3A%2F%2Fdevelopers.google.com%2Fspeed%2Fwebp%2Fdownload%3Fhl%3Dzh-cN)下载压缩包到本地编译动态链接库-不同平台的动态链接库编译出来是不同的。mac下是**.dylib**, windowns下是**.dll**, unix/linux下是**.so**. 我在mac和linux下编译过，记录下linux下编译过程。

- 
compiling


```
libwebp-0.6.1.tar.gz luciad-webp-imageio-8f9b44b41902.zip

tar -xzvf libwebp-0.6.1.tar.gz
unzip luciad-webp-imageio-8f9b44b41902.zip
mv libwebp-0.6.1 ./luciad-webp-imageio-8f9b44b41902/libwebp
```

```
cd luciad-webp-imageio-8f9b44b41902
cmake .
cmake --build .
```

cmake如果装的话，请自行下载安装[官网](https://link.jianshu.com?t=https%3A%2F%2Fcmake.org%2Fdownload%2F)[安装用法](https://link.jianshu.com?t=http%3A%2F%2Fblog.csdn.net%2Ffly_yr%2Farticle%2Fdetails%2F49815705)

```
./gradlew build -x test
```
- java project

	将libwebp-imageio.so放入工程的当前目录，因为默认的java.library.path含工程当前路径。同时引入打好的luciad-webp-imageio-8f9b44b41902-0.6.0-SNAPSHOT.jar引入工程中。如果是maven工程的话，可以把jar先推到远程的仓库里。
- java transfer

```
image = ImageIO.read(new URL(imageUrl));
// Encode it as webp using default settings
boolean result = ImageIO.write(image, "webp", bos);
```
- gradle push to repositry

	update build.gradle

```
apply plugin: "maven-publish" //插件required
group = "com.luciad.imageio.webp"

publishing {
    publications {
        mavenJava(MavenPublication) {
           from components.java
        }
    }
    repositories {
        maven {
             url "http://***"
             credentials {
                      username = ****
                      password = ****
            }
      }
   }
}
```

publish
`gradle publishMavenPublicationToMavenRepository`
有时间的话可以自己编译，此处来自：[https://www.jianshu.com/p/5827f9063fe9](https://www.jianshu.com/p/5827f9063fe9)

## Webp开源项目

### [nintha](https://github.com/nintha)/**[webp-imageio-core](https://github.com/nintha/webp-imageio-core)**

### [qwong](https://github.com/qwong)/**[j-webp](https://github.com/qwong/j-webp)**

### **可用的webp依赖和包**

下载此开源项目没有跑起来，dll包有问题。直接使用0.4.2可以正常使用，下载地址如下：
- [http://download.csdn.net/download/geeklei/8163329](http://download.csdn.net/download/geeklei/8163329)
- [https://download.csdn.net/download/geeklei/8161731](https://download.csdn.net/download/geeklei/8161731)
- 链接：[https://pan.baidu.com/s/1JZB_cNNr3Pf8sv2hdvu_-A](https://pan.baidu.com/s/1JZB_cNNr3Pf8sv2hdvu_-A)

	提取码：zvql

	复制这段内容后打开百度网盘手机App，操作更方便哦

## Webp 编解码

### 编码

```java
package example;

import com.luciad.imageio.webp.WebPWriteParam;

import javax.imageio.IIOImage;
import javax.imageio.ImageIO;
import javax.imageio.ImageWriter;
import javax.imageio.stream.FileImageOutputStream;

import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

public class EncodeTest {
    public static void main(String args[]) throws IOException {
        String inputPngPath = "test_pic/test.png";
        String inputJpgPath = "test_pic/test.jpg";
        String outputWebpPath = "test_pic/test_.webp";

        // Obtain an image to encode from somewhere
        BufferedImage image = ImageIO.read(new File(inputJpgPath));

        // Obtain a WebP ImageWriter instance
        ImageWriter writer = ImageIO.getImageWritersByMIMEType("image/webp").next();

        // Configure encoding parameters
        WebPWriteParam writeParam = new WebPWriteParam(writer.getLocale());
        writeParam.setCompressionMode(WebPWriteParam.MODE_DEFAULT);

        // Configure the output on the ImageWriter
        writer.setOutput(new FileImageOutputStream(new File(outputWebpPath)));

        // Encode
        long st = System.currentTimeMillis();
        writer.write(null, new IIOImage(image, null, null), writeParam);
        System.out.println("cost: " + (System.currentTimeMillis() - st));
    }
}
```

### 解码

```java
package example;

import com.luciad.imageio.webp.WebPReadParam;

import javax.imageio.ImageIO;
import javax.imageio.ImageReader;
import javax.imageio.stream.FileImageInputStream;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;

public class DecodeTest {
    public static void main(String args[]) throws IOException {
    	System.load("C:\\Users\\admin\\Desktop\\webp\\webp-code\\webp-imageio-core\\src\\main\\resources\\META-INF\\lib\\windows_64\\webp-imageio.dll");
        String inputWebpPath = "test_pic/test.webp";
        String outputJpgPath = "test_pic/test_.jpg";
        String outputJpegPath = "test_pic/test_.jpeg";
        String outputPngPath = "test_pic/test_.png";

        // Obtain a WebP ImageReader instance
        ImageReader reader = ImageIO.getImageReadersByMIMEType("image/webp").next();

        // Configure decoding parameters
        WebPReadParam readParam = new WebPReadParam();
        readParam.setBypassFiltering(true);

        // Configure the input on the ImageReader
        reader.setInput(new FileImageInputStream(new File(inputWebpPath)));

        // Decode the image
        BufferedImage image = reader.read(0, readParam);

        ImageIO.write(image, "png", new File(outputPngPath));
        ImageIO.write(image, "jpg", new File(outputJpgPath));
        ImageIO.write(image, "jpeg", new File(outputJpegPath));

    }
}
```

## Webp遇到的坑

需要注意在用完文件流之后记得关闭，否则删除文件会失败

![](https://img-blog.csdnimg.cn/20190328124506494.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

更多阅读：

[https://segmentfault.com/a/1190000016324137](https://segmentfault.com/a/1190000016324137)

[https://blog.csdn.net/xu_san_duo/article/details/79085718](https://blog.csdn.net/xu_san_duo/article/details/79085718)



