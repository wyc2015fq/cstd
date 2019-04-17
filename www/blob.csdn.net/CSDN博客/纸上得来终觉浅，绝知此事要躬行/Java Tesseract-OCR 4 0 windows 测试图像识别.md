# Java Tesseract-OCR 4.0 windows 测试图像识别 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年07月31日 10:22:51[boonya](https://me.csdn.net/boonya)阅读数：900








Java Tesseract-OCR 4.0 windows 测试图像识别：这里需要注意Tesseract-OCR 3.0.1以上才支持中文。我们拿到网上文章的代码来测试不一定对，但是需要找出为什么不对，哪里不对，从而可以解决很多疑惑。

文字识别可应用于许多领域，如阅读、翻译、文献资料的检索、信件和包裹的分拣、稿件的编辑和校对、大量统计报表和卡片的汇总与分析、银行支票的处理、商品发票的统计汇总、商品编码的识别、商品仓库的管理，以及水、电、煤气、房租、人身保险等费用的征收业务中的大量信用卡片的自动处理和办公室打字员工作的局部自动化等。以及文档检索，各类证件识别，方便用户快速录入信息，提高各行各业的工作效率。

### Tesseract-OCR 4.0 windows版本安装

```
下载地址：
win32 ：https://digi.bib.uni-mannheim.de/tesseract/tesseract-ocr-w32-setup-v4.0.0-beta.1.20180608.exe
win64 ：https://digi.bib.uni-mannheim.de/tesseract/tesseract-ocr-w64-setup-v4.0.0-beta.1.20180608.exe
```

```
==========================================================================================
Java文字识别程序的关键是寻找一个可以调用的OCR引擎。
tesseract-ocr就是一个这样的OCR引擎，在1985年到1995
年由HP实验室开发，现在在Google。tesseract-ocr 3.0
发布，支持中文。不过tesseract-ocr 3.0不是图形化界面
的客户端，别人写的FreeOCR图形化客户端还不支持导入新
的 3.0 traineddata。但这标志着，现在有自由的中文OCR
软件了。
==========================================================================================
参考地址:https://www.jb51.net/article/91499.htm
==========================================================================================
java中使用tesseract-ocr3.01的步骤如下：

1.下载安装tesseract-ocr-setup-3.01-1.exe（3.0以上版本才增加了中文识别）

2.在安装向导中可以选择需要下载的语言包。

3.到网上搜索下载java图形处理所需的2个包：jai_imageio-1.1-alpha.jar，swingx-1.6.1.jar
==========================================================================================
Github:https://github.com/tesseract-ocr/tesseract/wiki/4.0-with-LSTM#400-alpha-for-windows

============================================4.0.0=========================================
安装说明：
https://jingyan.baidu.com/article/219f4bf788addfde442d38fe.html
https://www.cnblogs.com/jianqingwang/p/6978724.html
===========================================================================================
```

Path环境配置：

![](https://img-blog.csdn.net/20180731100719145?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

变量配置：

![](https://img-blog.csdn.net/20180731100601231?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### Java图像识别测试代码

maven pom.xml

```
<?xml version="1.0" encoding="UTF-8"?>
<project xmlns="http://maven.apache.org/POM/4.0.0"
         xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
         xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>

    <groupId>com.boonya.ocr</groupId>
    <artifactId>OcrDemo</artifactId>
    <version>1.0-SNAPSHOT</version>

    <dependencies>
        <!-- https://mvnrepository.com/artifact/com.sun.media/jai_imageio -->
        <dependency>
            <groupId>com.sun.media</groupId>
            <artifactId>jai_imageio</artifactId>
            <version>1.1</version>
        </dependency>
        <!-- https://mvnrepository.com/artifact/com.github.jai-imageio/jai-imageio-core -->
        <dependency>
            <groupId>com.github.jai-imageio</groupId>
            <artifactId>jai-imageio-core</artifactId>
            <version>1.3.1</version>
        </dependency>
        <!-- https://mvnrepository.com/artifact/org.swinglabs/swingx -->
        <dependency>
            <groupId>org.swinglabs</groupId>
            <artifactId>swingx</artifactId>
            <version>1.6.1</version>
        </dependency>
    </dependencies>

</project>
```

ImageUtil.java

```java
package com.boonya.ocr.tesseract;

import com.github.jaiimageio.plugins.tiff.TIFFImageWriteParam;
import javax.imageio.*;
import javax.imageio.metadata.IIOMetadata;
import javax.imageio.stream.ImageInputStream;
import javax.imageio.stream.ImageOutputStream;
import java.awt.image.BufferedImage;
import java.io.File;
import java.io.IOException;
import java.util.Iterator;
import java.util.Locale;

/**
 * @ClassName: ImageIOHelper
 * @Description: TODO(图片工具类)
 * @author: pengjunlin
 * @company: ******科技有限公司
 * @date 2018-07-30
 */
public class ImageUtil {

    public static File createImage(File imageFile, String imageFormat) {
        File tempFile = null;
        try {
            Iterator readers = ImageIO.getImageReadersByFormatName(imageFormat);
            ImageReader reader = (ImageReader)readers.next();

            ImageInputStream iis = ImageIO.createImageInputStream(imageFile);
            reader.setInput(iis);
            //Read the stream metadata
            IIOMetadata streamMetadata = reader.getStreamMetadata();

            //Set up the writeParam
            TIFFImageWriteParam tiffWriteParam = new TIFFImageWriteParam(Locale.CHINESE);
            tiffWriteParam.setCompressionMode(ImageWriteParam.MODE_DISABLED);

            //Get tif writer and set output to file
            Iterator writers = ImageIO.getImageWritersByFormatName("tiff");
            ImageWriter writer = (ImageWriter)writers.next();

            BufferedImage bi = reader.read(0);
            IIOImage image = new IIOImage(bi,null,reader.getImageMetadata(0));
            tempFile = tempImageFile(imageFile);
            ImageOutputStream ios = ImageIO.createImageOutputStream(tempFile);
            writer.setOutput(ios);
            writer.write(streamMetadata, image, tiffWriteParam);
            ios.close();

            writer.dispose();
            reader.dispose();

        } catch (IOException e) {
            e.printStackTrace();
        }
        return tempFile;
    }

    private static File tempImageFile(File imageFile) {
        String path = imageFile.getPath();
        StringBuffer strB = new StringBuffer(path);
        strB.insert(path.lastIndexOf('.'),0);
        return new File(strB.toString().replaceFirst("(?<=//.)(//w+)$", "tif"));
    }
}
```

OcrRecognizer.java

```java
package com.boonya.ocr.tesseract;

import org.jdesktop.swingx.util.OS;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

/**
 * @ClassName: OCR
 * @Description: TODO(OCR图像识别类)
 * @author: pengjunlin
 * @company: ******科技有限公司
 * @date 2018-07-30
 */
public class OcrRecognizer {

    private final String LANG_OPTION = "-l"; //英文字母小写l，并非数字1

    private final String EOL = System.getProperty("line.separator");

    private String tessPath = "C://Program Files (x86)//Tesseract-OCR";

    //private String tessPath = new File("tesseract").getAbsolutePath();

    public String recognizeText(File imageFile,String imageFormat)throws Exception{
        File tempImage = ImageUtil.createImage(imageFile,imageFormat);
        File outputFile = new File(imageFile.getParentFile(),"output");
        StringBuffer strB = new StringBuffer();
        List cmd = new ArrayList();
        if(OS.isWindowsXP()){
            cmd.add(tessPath+"//tesseract");
        }else if(OS.isLinux()){
            cmd.add("tesseract");
        }else{
            cmd.add(tessPath+"//tesseract");
        }
        cmd.add("");
        cmd.add(outputFile.getName());
        cmd.add(LANG_OPTION);
        //cmd.add("chi_sim");
        cmd.add("eng");

        ProcessBuilder pb = new ProcessBuilder();
        //需要单独配置下配置字库否则解析不了
        //还会报Errors accessing files.There may be spaces in your image's filename.
        Map<String, String> env = pb.environment(); //获得进程的环境
        env.put("TESSDATA_PREFIX", "C:\\Program Files (x86)\\Tesseract-OCR\\tessdata");
        pb.directory(imageFile.getParentFile());

        cmd.set(1, tempImage.getName());
        pb.command(cmd);
        pb.redirectErrorStream(true);

        Process process = pb.start();
        //tesseract.exe 1.jpg 1 -l chi_sim
        int w = process.waitFor();

        //删除临时正在工作文件
        tempImage.delete();

        if(w==0){
            BufferedReader in = new BufferedReader(new InputStreamReader(new FileInputStream(outputFile.getAbsolutePath()+".txt"),"UTF-8"));

            String str;
            while((str = in.readLine())!=null){
                strB.append(str).append(EOL);
            }
            in.close();
        }else{
            String msg;
            switch(w){
                case 1:
                    msg = "Errors accessing files.There may be spaces in your image's filename.";
                    break;
                case 29:
                    msg = "Cannot recongnize the image or its selected region.";
                    break;
                case 31:
                    msg = "Unsupported image format.";
                    break;
                default:
                    msg = "Errors occurred.";
            }
            tempImage.delete();
            throw new RuntimeException(msg);
        }
        new File(outputFile.getAbsolutePath()+".txt").delete();
        return strB.toString();
    }
}
```

OcrTest.java

```java
package com.boonya.ocr.tesseract;

import java.io.File;
import java.io.IOException;

/**
 * @ClassName: OcrTest
 * @Description: TODO(OCR图像识别功能测试)
 * @author: pengjunlin
 * @company: ******科技有限公司
 * @date 2018-07-30
 */
public class OcrTest {

    /**
     * 函数入口
     * @param args
     */
    public static void main(String[] args) {
        String path = "C:\\Users\\Administrator\\Desktop\\Lincoln.png";
        System.out.println("ORC Test Begin......");
        try {
            String valCode = new OcrRecognizer().recognizeText(new File(path), "png");
            System.out.println(valCode);
        } catch (IOException e) {
            e.printStackTrace();
        } catch (Exception e) {
            e.printStackTrace();
        }
        System.out.println("ORC Test End......");
    }

}
```



### Java 代码测试运行效果

以林肯像为测试样例：

![](https://img-blog.csdn.net/20180731100005155?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

识别文字如下：

```
ORC Test Begin......
Abraham Lincoln

 

President Lincoln in November 1863

16th

In office
March 4, 1861 - April 15, 1865

(1861-1865)
Andrew Johnson
(1865)

Preceded by

Succeeded by


ORC Test End......

Process finished with exit code 0
```

测试中文，需要查看支持的中文字库：

![](https://img-blog.csdn.net/20180731125536674?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

修改测试识别指令：

```java
package com.boonya.ocr.tesseract;

import org.jdesktop.swingx.util.OS;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.InputStreamReader;
import java.util.ArrayList;
import java.util.List;
import java.util.Map;

/**
 * @ClassName: OCR
 * @Description: TODO(OCR图像识别类)
 * @author: pengjunlin
 * @company: 上海势航网络科技有限公司
 * @date 2018-07-30
 */
public class OcrRecognizer {

    private final String LANG_OPTION = "-l"; //英文字母小写l，并非数字1

    private final String EOL = System.getProperty("line.separator");

    private String tessPath = "C://Program Files (x86)//Tesseract-OCR";

    //private String tessPath = new File("tesseract").getAbsolutePath();

    public String recognizeText(File imageFile,String imageFormat)throws Exception{
        File tempImage = ImageUtil.createImage(imageFile,imageFormat);
        File outputFile = new File(imageFile.getParentFile(),"output");
        StringBuffer strB = new StringBuffer();
        List cmd = new ArrayList();
        if(OS.isWindowsXP()){
            cmd.add(tessPath+"//tesseract");
        }else if(OS.isLinux()){
            cmd.add("tesseract");
        }else{
            cmd.add(tessPath+"//tesseract");
        }
        cmd.add("");
        cmd.add(outputFile.getName());
        cmd.add(LANG_OPTION);
        cmd.add("chi_sim_vert");
        //cmd.add("eng");

        ProcessBuilder pb = new ProcessBuilder();
        //需要单独配置下配置代码如下：
        Map<String, String> env = pb.environment(); //获得进程的环境
        env.put("TESSDATA_PREFIX", "C:\\Program Files (x86)\\Tesseract-OCR\\tessdata");
        pb.directory(imageFile.getParentFile());

        cmd.set(1, tempImage.getName());
        pb.command(cmd);
        pb.redirectErrorStream(true);

        Process process = pb.start();
        //tesseract.exe 1.jpg 1 -l chi_sim
        int w = process.waitFor();

        //删除临时正在工作文件
        tempImage.delete();

        if(w==0){
            BufferedReader in = new BufferedReader(new InputStreamReader(new FileInputStream(outputFile.getAbsolutePath()+".txt"),"UTF-8"));

            String str;
            while((str = in.readLine())!=null){
                strB.append(str).append(EOL);
            }
            in.close();
        }else{
            String msg;
            switch(w){
                case 1:
                    msg = "Errors accessing files.There may be spaces in your image's filename.";
                    break;
                case 29:
                    msg = "Cannot recongnize the image or its selected region.";
                    break;
                case 31:
                    msg = "Unsupported image format.";
                    break;
                default:
                    msg = "Errors occurred.";
            }
            tempImage.delete();
            throw new RuntimeException(msg);
        }
        new File(outputFile.getAbsolutePath()+".txt").delete();
        return strB.toString();
    }
}
```

主要将"chi_sim" 改为"chi_sim_vert",运行得到中文如下：

原图：

![](https://img-blog.csdn.net/20180731125840525?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

识别文字：

```
"C:\Program Files (x86)\Java\jdk1.8.0_172\bin\java" "-javaagent:D:\Program Files\IntelliJ IDEA 2017.2.5\lib\idea_rt.jar=59579:D:\Program Files\IntelliJ IDEA 2017.2.5\bin" -Dfile.encoding=UTF-8 -classpath "C:\Program Files (x86)\Java\jdk1.8.0_172\jre\lib\charsets.jar;C:\Program Files (x86)\Java\jdk1.8.0_172\jre\lib\deploy.jar;C:\Program Files (x86)\Java\jdk1.8.0_172\jre\lib\ext\access-bridge-32.jar;C:\Program Files (x86)\Java\jdk1.8.0_172\jre\lib\ext\cldrdata.jar;C:\Program Files (x86)\Java\jdk1.8.0_172\jre\lib\ext\dnsns.jar;C:\Program Files (x86)\Java\jdk1.8.0_172\jre\lib\ext\jaccess.jar;C:\Program Files (x86)\Java\jdk1.8.0_172\jre\lib\ext\jfxrt.jar;C:\Program Files (x86)\Java\jdk1.8.0_172\jre\lib\ext\localedata.jar;C:\Program Files (x86)\Java\jdk1.8.0_172\jre\lib\ext\nashorn.jar;C:\Program Files (x86)\Java\jdk1.8.0_172\jre\lib\ext\sunec.jar;C:\Program Files (x86)\Java\jdk1.8.0_172\jre\lib\ext\sunjce_provider.jar;C:\Program Files (x86)\Java\jdk1.8.0_172\jre\lib\ext\sunmscapi.jar;C:\Program Files (x86)\Java\jdk1.8.0_172\jre\lib\ext\sunpkcs11.jar;C:\Program Files (x86)\Java\jdk1.8.0_172\jre\lib\ext\zipfs.jar;C:\Program Files (x86)\Java\jdk1.8.0_172\jre\lib\javaws.jar;C:\Program Files (x86)\Java\jdk1.8.0_172\jre\lib\jce.jar;C:\Program Files (x86)\Java\jdk1.8.0_172\jre\lib\jfr.jar;C:\Program Files (x86)\Java\jdk1.8.0_172\jre\lib\jfxswt.jar;C:\Program Files (x86)\Java\jdk1.8.0_172\jre\lib\jsse.jar;C:\Program Files (x86)\Java\jdk1.8.0_172\jre\lib\management-agent.jar;C:\Program Files (x86)\Java\jdk1.8.0_172\jre\lib\plugin.jar;C:\Program Files (x86)\Java\jdk1.8.0_172\jre\lib\resources.jar;C:\Program Files (x86)\Java\jdk1.8.0_172\jre\lib\rt.jar;C:\Users\Administrator\Desktop\Hadoop\code\OcrDemo\target\classes;D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\javax\media\jai_core\1.1.3\jai_core-1.1.3.jar;D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\com\github\jai-imageio\jai-imageio-core\1.3.1\jai-imageio-core-1.3.1.jar;D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\org\swinglabs\swingx\1.6.1\swingx-1.6.1.jar;D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\com\jhlabs\filters\2.0.235\filters-2.0.235.jar;D:\DEVELOPERS\maven\apache-maven-3.5.0\repository\org\swinglabs\swing-worker\1.1\swing-worker-1.1.jar" com.boonya.ocr.tesseract.OcrTest
ORC Test Begin......
尾 吐 如 ( 丰 第 二 3

可 * 画 本 着 洲 (三 旭 吕 | 人 由 融 )| 癌 诛 恒 库 姑 (遇害 洲 )

恒隆 乔 ( 吕 因 是 【| 于 各 二 吕 栈 】! 人 二 ,可 守卫 )、 如 本 眷 洲 ( 吕 证 | 时 名 填 村 世 “生计 是 浊音
《小 深 国 里 呈 )。 神 请 于 贡 证 兰芝 阐 训 可 于 让 。 靖 旧 器 | 舍 骨 吧 。 江 正 可 轩 送 庆 。 征 器 训 。 州 衣 测 。 王 。

 

 

 

 

 

 

必用 剧 字 州 并州 于 计
诈 “ 庄 计 册 下 证 凋 》 身 广 济 ” 演

加 尖 可 。 友 二 枉 6 序 “ 评 隆 乔 芭 讽 下 如 几 , 计 站 简 证 黄 放 。 坎 烷
撞 本 三 ,放任 寺 州 机 “ 圭 刘 点 过 得 上游 | 详 毅 如 了 漳 尘 汪汪 。

  
 

半 放 本 党 旧 ( 吕 卫 )“ 翌 什 乔 浇 过 计生 了 人 寺 党 沪 面 中 喜悦 木 并州 克 时 。 轩 部 着 必 半 时 半 |> 莽
计 。 漠 和 过 着 上。 才 而 荆 荔 放 关 二 压 “ 评 什 天 如 态 “ 下 半 双 当 。(]

 

改 咱 天 认 舍 训 丁 “ 增 落 母 增强 阳 可 于 沁 “ 音 于 习 半 着 “好 情 淮 东 “ 癌 直 上 过 “ 洋 渐 半 淹 、 荐 公社 贡 呈 并 济 生 吉 .加 清和
基 “ 冲 营 > 呈 回回 癌 英 信鸽 员 当 人 部。 六 半 症 靖 首 二。 地衣 并 肖 天 申 轩 六 吊 。 商 测 于 如 , 融 焙 ,时 或 攻 。 员 时 于 村 加 员
轩 四 蓝 。 苏 用 济 理 填 蔓 疮 “ 江 贡 江 天 机 本 过半 巴 半 : 这 序 并 昌 坊 | 可 计 人 全 了 作 尖 计 间 尖 且 于 。

 

 

沁 当 | 者 册 可 中田 ( 吕 名 放 可 证 卫 )、“ 情 嘿 居 因 谢 虹 强 吓 压 习 于 “ 放 填 台 二 |。 附 信 | 十 = 者。 请 如 证章 。 轩 出
机 。

恒 咱 天 漠 二 全 上 点 计 。 坟 世 书 晤 朋 计 在任 。

 


ORC Test End......

Process finished with exit code 0
```

### Java示例运行注意事项

工具类设置了系统变量任然找不到的需要手动设置：

```java
Map<String, String> env = pb.environment(); //获得进程的环境
 env.put("TESSDATA_PREFIX", "C:\\Program Files (x86)\\Tesseract-OCR\\tessdata");
```

如不设置会一直报错：

![](https://img-blog.csdn.net/2018073113101367?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 通过工具使用命令测试

输出文件为result.txt,命令如下：

识别英文
`tesseract Lincoln.png result -l eng`
识别中文
`tesseract lishimin.png result -l chi_sim_vert`


注：总的来说这款图像识别软件英文识别率还是很高的，唯一不足的是只能识别黑色文字，另外中文还需要训练。

中文字体库（这里可以下载chi_sim.traineddata,此库识别中文效率更高）：

[https://github.com/tesseract-ocr/tessdata/find/master](https://github.com/tesseract-ocr/tessdata/find/master)



