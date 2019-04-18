# dom4j 解析XML文件 - weixin_33985507的博客 - CSDN博客
2017年06月20日 23:25:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
![](https://images2015.cnblogs.com/blog/1034798/201706/1034798-20170620231830898-1913852784.jpg)
- 编写config文件
```
<?xml version="1.0" encoding="UTF-8"?>
<chuanke>
    <browser>firefox</browser>
    <url>http://www.cnblogs.com/lizitest/</url>
    <waitTime>2</waitTime>
</chuanke>
```
- 解析XML文件代码
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
package com.selenium.tool;
import java.io.File;
import org.dom4j.Document;
import org.dom4j.DocumentException;
import org.dom4j.Element;
import org.dom4j.io.SAXReader;
/**
 * 使用SAX(Simple APIs for XML，也即XML简单应用程序接口)解析XML文件
 */
public class ParseXML {
    
    private Document document;
    
    /**
     * 构造函数
     * 在新建对象的同时加载XML文件
     */
    public ParseXML(String filePath){
        this.loadXML(filePath);
    }
    
    /**
     * 加载XML文件
     */
    public void loadXML(String filePath){
        
        //新建文件
        File file = new File(filePath);
        
        if(file.exists()){
            //dom4j包中SAXReader
            SAXReader saxReader = new SAXReader();
            try {
                
                document = saxReader.read(file);
                
            } catch (DocumentException e) {
                
                e.printStackTrace();
            }
        }else{
            
            System.out.println("XML文件不存在");
        }
    }
    
    /**
     * 获取节点上的文本
     */
    public String getSingleNodeText(String nodePath){
        
        Element element = (Element) document.selectSingleNode(nodePath);
        
        if(element != null){
            
            return element.getTextTrim();
            
        }else{
            
            System.out.println("节点不存在！");
            return null;
        }
    }
    
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
- 解析config文件
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
package com.selenium.util;
import com.selenium.tool.ParseXML;
public interface Config {
    public static ParseXML xml = new ParseXML(".\\config\\config.xml");-----------对应的是整个config.xml文件
    public static String browser = xml.getSingleNodeText("//browser");-----------<browser>firefox</browser>
    public static String url = xml.getSingleNodeText("//url");----------<url>http://www.cnblogs.com/lizitest/</url>
    public static String waitTime = xml.getSingleNodeText("//waitTime");----------<waitTime>2</waitTime>
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
