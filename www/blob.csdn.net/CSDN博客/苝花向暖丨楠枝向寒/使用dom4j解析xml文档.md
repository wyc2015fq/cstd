# 使用dom4j解析xml文档 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年05月14日 14:44:37[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：35标签：[dom4j																[xml](https://so.csdn.net/so/search/s.do?q=xml&t=blog)](https://so.csdn.net/so/search/s.do?q=dom4j&t=blog)
个人分类：[xml](https://blog.csdn.net/weixin_40247263/article/category/7662898)


***转自：[点击打开链接](https://www.jianshu.com/p/e8561ea99d03)***

***注意***：在使用dom4j解析xml文档的时候首先我们需要导入dom4j的jar包***dom4j-1.6.1.jar***

# 一．dom4j中，获得Document对象的方式有三种

## 1.读取xml文件，获得Document对象

```java
```java
SAXReader reader = new SAXReader();
Document document = reader.read(new File("input.xml"));
```
```

## 2.解析xml形式的文本，得到Document对象，将字符串转换为

```java
```java
Document String text = "<members></members>";
Document document = DocumentHelper.parseText(text);
```
```

## 3.主动创建Document对象（直接创建）

```java
```java
Document document = DocumentHelper.createDocument(); //创建根节点
Element root = document.addElement("members");
```
```

# 二、节点对象

## 1.获取文档的根节点：

```java
```java
Element root = document.getRootElement();
```
```

## 2.取得某个节点的子节点

```
Element element=node.element(“书名");
```

## 3.取得节点的文本

```java
```java
String text=node.getText();
```
```

## 4.取得某节点下所有名为“member“的子节点，并进行遍历

```
```
List nodes = rootElm.elements("member");
for (Iterator it = nodes.iterator(); it.hasNext();) {     
    Element elm = (Element) it.next();   
     // do something 
}
```
```

## 5.对某节点下的所有子节点进行遍历（遍历方法2）

```
```
for(Iterator it=root.elementIterator();it.hasNext();){       
    Element element = (Element) it.next();       
    // do something   
}
```
```

## 6.在某节点下添加子节点

```bash
Element ageElm = newMemberElm.addElement("age");
```

## 7.设置节点文本

```bash
element.setText("29");
```

## 8.删除某节点

```cpp
```cpp
//childElm是待删除的节点,parentElm是其父节点
parentElm.remove(childElm);
```
```

## 9．添加一个CDATA（前面有说明）节点

```bash
Element contentElm =infoElm.addElement("content");contentElm.addCDATA(diary.getContent());
```

# 三、节点对象属性

## 1.取得某节点下的某属性

```java
```java
Element root=document.getRootElement();    //属性名name
Attribute attribute=root.attribute("size");
```
```

## 2.取得属性的文本

```java
```java
String text=attribute.getText();
```
```

## 3.删除某属性

```bash
Attribute attribute=root.attribute("size");root.remove(attribute);
```

## 4.遍历某节点的所有属性

```java
```java
Element root=document.getRootElement();    
for(Iterator it=root.attributeIterator();it.hasNext();){    
     Attribute attribute = (Attribute) it.next();     
     String text=attribute.getText();     
     System.out.println(text);
}
```
```

## 5.设置某节点的属性和文本

```bash
newMemberElm.addAttribute("name", "sitinspring");
```

## 6.设置属性的文本

```bash
Attribute attribute=root.attribute("name");attribute.setText("sitinspring");
```

# 四、将文档写入xml文件

## 1.文档中全文英文，不设置编码，直接写入的形式

```java
```java
XMLWriter writer = new XMLWriter(new  FileWriter("output.xml"));
writer.write(document);writer.close();
```
```

## 2.文档中含有中文，设置编码格式写入的形式

```java
```java
OutputFormat format = OutputFormat.createPrettyPrint();// 指定XML编码                 
format.setEncoding("GBK");       
XMLWriter writer = new XMLWriter(newFileWriter("output.xml"),format);
writer.write(document);
writer.close();
```
```

# 五、在指定位置插入节点步骤

（1）得到插入位置的节点列表（list）
（2）调用list.add(index,elemnent)，由index决定element的插入位置；
（3）Element元素可以通过DocumentHelper对象得到。示例：

```php
```php
Element aaa = DocumentHelper.*createElement*("aaa");
aaa.setText("aaa");
List list = root.element("书").elements();
list.add(1, aaa);
//更新document
```
```

# 六、字符串与xml的转换

## 1.将字符串转换为xml

```xml
String text = "<members> <member>sitinspring</member></members>";
Document document = DocumentHelper.parseText(text);
```

## 2.将文档或节点的xml转换为字符串

```java
```java
SAXReader reader = new SAXReader();
Document document = reader.read(new File("input.xml"));       
Element root=document.getRootElement();      
String docXmlText=document.asXML();
String rootXmlText=root.asXML();
Element memberElm=root.element("member");
String memberXmlText=memberElm.asXML();
```
```

# 例子：工程dom4j

首先我们需要三个依赖jar包：dom4j-1.6.1.jar、jaxen-1.1-beta-6.jar和jaxme-api-0.3.jar包

***xml/book.xml***

```xml
<?xml version="1.0" encoding="UTF-8"?>
<书架> 
  <书 name="yyy"> 
    <售价>59元</售价>  
    <售价>109元</售价>
    <书名>Java就业培训教程</书名>  
    <作者>张孝祥</作者> 
  </书> 
</书架>
```

***xml/user.xml***

```xml
<?xml version="1.0" encoding="UTF-8"?>
<users>
    <user username="aaa" password="111"  />
    <user username="bbb" password="222"  />
    <user username="ccc" password="333"  />
</users>
```

## 测试类

***/dom4j/src/cn/itcast/Demo1.java***

```java
```java
package cn.itcast;
import java.io.File;
import java.io.FileOutputStream;
import java.util.List;
import org.dom4j.Document;
import org.dom4j.DocumentException;
import org.dom4j.DocumentHelper;
import org.dom4j.Element;
import org.dom4j.io.OutputFormat;
import org.dom4j.io.SAXReader;
import org.dom4j.io.XMLWriter;
import org.junit.Test;

public class Demo1 {
    
    //读取文档中的数据
    @Test
    public void readText() throws Exception{
        SAXReader reader = new SAXReader();//得到解析器对象
        //通过解析器对象得到文档对象
        Document document = reader.read(new File("xml/book.xml"));
        //得到根节点（此处的根节点是“书架”）
        Element root = document.getRootElement();
        //得到子节点“书”的子节点“书名”
        Element bookname = root.element("书").element("书名");
        //得到节点的值或文本
        System.out.println(bookname.getText());
    }
    
    //读取xml文档中的属性
    @Test
    public void readAttr() throws Exception{
        SAXReader reader = new SAXReader();
        Document document = reader.read(new File("xml/book.xml"));
        Element root = document.getRootElement();//得到了“书架”节点
        //得到标签中属性的值
        String value = root.element("书").attributeValue("name");
        System.out.println(value);
    }
    
    //添加一个标签和其文本（值），这里添加一个售价节点
    @Test
    public void addNode() throws Exception{
        SAXReader reader = new SAXReader();
        Document document = reader.read(new File("xml/book.xml"));
        //得到“书”这个标签，之后我们需要将售价节点挂到此节点上
        Element booknode = document.getRootElement().element("书");
        
        //创建一个“售价”节点
        Element price = DocumentHelper.createElement("售价");
        //设置其文本
        price.setText("109元");
        
        //创建一个编码格式化工具，设置编码
        OutputFormat format = OutputFormat.createPrettyPrint();
        format.setEncoding("UTF-8");
        
        //将相关内容输出到xml文档中去
        //注意：这里不能使用FileWriter("xml/book.xml")因为这种方式是输出字符流，于是会按照默认编码
        //gb2312进行输出，所以这里需要使用字节流FileOutputStream进行输出
        XMLWriter writer = new XMLWriter(new FileOutputStream("xml/book.xml"), format);
        writer.write(document);
        writer.close();//关闭流
    }
    //修改售价
    @Test
    public void update() throws Exception{
        SAXReader reader = new SAXReader();
        Document document = reader.read(new File("xml/book.xml"));
        //取得售价节点
        Element price = (Element) document.getRootElement().element("书").elements("售价").get(1);
        price.setText("59元");
        
        OutputFormat format = OutputFormat.createPrettyPrint();
        format.setEncoding("UTF-8");
        //将内存中修改过的内容写到文档中去，注意使用字符流
        XMLWriter writer = new XMLWriter(new FileOutputStream("xml/book.xml"), format);
        writer.write(document);
        writer.close();
    }
    
    //删除一个售价节点
    @Test
    public void delete() throws Exception{
        SAXReader reader = new SAXReader();
        Document document = reader.read(new File("xml/book.xml"));
        
        Element price = (Element) document.getRootElement().element("书").elements("售价").get(0);
        price.getParent().remove(price);
        
        OutputFormat format = OutputFormat.createPrettyPrint();
        format.setEncoding("UTF-8");
        XMLWriter writer = new XMLWriter(new FileOutputStream("xml/book.xml"), format);
        writer.write(document);
        writer.close();
    }
    
    //向指定位置增加节点
    @Test
    public void add2() throws Exception{
        SAXReader reader = new SAXReader();
        Document document = reader.read(new File("xml/book.xml"));
        //自己创建一个节点
        Element price = DocumentHelper.createElement("售价");
        price.setText("109元");
        
        //将节点挂到指定位置
        //得到书节点下的所有节点
        List list = document.getRootElement().element("书").elements();
        list.add(1, price);//将次节点挂在位置1处
        
        OutputFormat format = OutputFormat.createPrettyPrint();
        format.setEncoding("UTF-8");
        XMLWriter writer = new XMLWriter(new FileOutputStream("xml/book.xml"), format);
        writer.write(document);
        writer.close();
    }
    
    //使用xpath解析
    //注意需要jaxen-1.1-beta-6.jar和jaxme-api-0.3.jar包
    @Test
    public void findWithXpath() throws Exception{
        SAXReader reader = new SAXReader();
        Document document = reader.read(new File("xml/book.xml"));
        
        Element element = (Element) document.selectNodes("//书名").get(0);
        System.out.println(element.getText());
        
    }
    
    //使用xpath查找
    @Test
    public void findUser() throws Exception{
        String username = "aaa";
        String password = "111";
        
        SAXReader reader = new SAXReader();
        Document document = reader.read(new File("xml/user.xml"));
        //下面的这种写法我们不需要记，直接看xpath的文档即可。
        Element element = (Element) document.selectSingleNode("//user[@username='" + 
                                    username + "' and @password='"+password+"']");
        
        if(element != null){
            System.out.println("登录成功");
        }else{
            System.out.println("登录失败");
        }   
    }
}
```
```

