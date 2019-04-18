# java学习：使用dom4j读写xml文件 - z69183787的专栏 - CSDN博客
2013年11月13日 14:20:02[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1561
[dom4j](http://dom4j.sourceforge.net/)是一个优秀的开源项目，专用于xml文件的读写，而且支持强大的[xpath](http://baike.baidu.com/view/307399.htm).
下面的代码演示了对xml文件demo.xml的读写
demo.xml内容：
```
<?xml version="1.0" encoding="UTF-8" ?>
<address-list>
  <card name="yangjm" id="1">
    <sex>男</sex>
    <address><![CDATA[中国上海外滩No.01]]>    </address>
    <telephone>13916732212</telephone>
  </card>
  <card name="zhangsan" id="2">
    <sex>女</sex>
    <address>
      <item type="家庭地址"><![CDATA[中国.北京.东直门外大街]]>      </item>
      <item type="单位地址"><![CDATA[中国.上海.田林路888号]]>      </item>
    </address>
    <telephone>010-123123</telephone>
  </card>
</address-list>
```
java代码：
```java
package jmyang.dom4j;
/**
 * <p>Title:dom4j测试 </p>
 * <p>Description: 使用xpath读取节点内容、属性</p>
 * <p>Copyright: Copyright (c) 2012</p>
 * <p>Company: infosky.com.cn</p>
 * @author:yjmyzz@126.com 菩提树下的杨过
 * @version 1.0
 */
import java.util.*;
import java.io.*;
import org.dom4j.*;
import org.dom4j.io.*;
public class XmlTest {
    public static void Test() {
        String fileName = System.getProperty("user.dir") + "\\demo.xml"; //当前路径下的demo.xml
        try {
            SAXReader reader = new SAXReader();
            Document doc = reader.read(fileName); //加载xml文件
            List peoples = doc.selectNodes("//*[@name]"); //选择所有具有name属性的节点(即demo.xml中的所有card节点)
            for (Iterator iter = peoples.iterator(); iter.hasNext(); ) {
                Element card = (Element) iter.next();
                //System.out.println(node.asXML());
                //System.out.println("---------------------------------------");
                //System.out.println(node.attributeCount());
                List attrList = card.attributes();
                //输出每个card的所有属性
                for (Iterator attr = attrList.iterator(); attr.hasNext(); ) {
                    Attribute a = (Attribute) attr.next();
                    System.out.println(a.getName() + "=" + a.getValue());
                }
                System.out.println(
                        "----------------------------------------------------");
            }
            Element zhangsan = (Element) doc.selectSingleNode("//card[@id='2']"); //查找“id属性”=2的card元素
            System.out.println("张三的名称：" + zhangsan.attribute("name").getValue()); //输出zhangsan的name属性
            Node addrFamily = zhangsan.selectSingleNode("./address/item[2]"); //选择zhangsan元素下的address节点下的第2个item子节点
            System.out.println("张三的单位地址：" + addrFamily.getStringValue()); //输出cdata内容
            System.out.println(
                    "----------------------------------------------------");
            //为zhangsan下增加二个节点
            zhangsan.addElement("email").addAttribute("type",
                    "工作").addText("work@some-domain.com");
            zhangsan.addElement("email").addAttribute("type",
                    "私人").addCDATA("private@some-domain.com"); //设置CDATA内容
            System.out.println(zhangsan.asXML()); //打印zhangsan节点的xml内容(调试用)
            System.out.println(
                    "----------------------------------------------------");
            //将上述改动保存到文件demo2.xml
            FileWriter fileWriter = new FileWriter(System.getProperty(
                    "user.dir") + "\\demo2.xml");
            OutputFormat format = OutputFormat.createPrettyPrint(); //设置美观的缩进格式，便于阅读
            //format = OutputFormat.createCompactFormat();//设置紧凑格式（消除多余空格），便于下载
            XMLWriter writer = new XMLWriter(System.out, format);
            writer.setWriter(fileWriter);
            writer.write(doc);
            writer.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
```
 jBuilder下的运行结果：
name=yangjm
id=1
----------------------------------------------------
name=zhangsan
id=2
----------------------------------------------------
张三的名称：zhangsan
张三的单位地址：中国.上海.田林路888号      
----------------------------------------------------
<card name="zhangsan" id="2">
    <sex>女</sex>
    <address>
      <item type="家庭地址"><![CDATA[中国.北京.东直门外大街]]>      </item>
      <item type="单位地址"><![CDATA[中国.上海.田林路888号]]>      </item>
    </address>
    <telephone>010-123123</telephone>
  <email type="工作">work@some-domain.com</email><email type="私人"><![CDATA[private@some-domain.com]]></email></card>
----------------------------------------------------
保存后的demo2.xml
<?xml version="1.0" encoding="UTF-8"?>
<address-list> 
  <card name="yangjm" id="1"> 
    <sex>男</sex>  
    <address><![CDATA[中国上海外滩No.01]]> </address>  
    <telephone>13916732212</telephone> 
  </card>  
  <card name="zhangsan" id="2"> 
    <sex>女</sex>  
    <address> 
      <item type="家庭地址"><![CDATA[中国.北京.东直门外大街]]> </item>  
      <item type="单位地址"><![CDATA[中国.上海.田林路888号]]> </item> 
    </address>  
    <telephone>010-123123</telephone>  
  <email type="工作">work@some-domain.com</email>
    <email type="私人"><![CDATA[private@some-domain.com]]></email>
  </card> 
</address-list>
