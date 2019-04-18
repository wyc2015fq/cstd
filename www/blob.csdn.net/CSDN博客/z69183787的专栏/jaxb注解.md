# jaxb注解 - z69183787的专栏 - CSDN博客
2013年10月23日 14:09:30[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5503
 JAXB（Java API for XML Binding），提供了一个快速便捷的方式将Java对象与XML进行转换。在JAX-WS（Java的WebService规范之一）中，JDK1.6 自带的版本JAX-WS2.1，其底层支持就是JAXB。
    JAXB 可以实现Java对象与XML的相互转换，在JAXB中，将一个Java对象转换为XML的过程称之为Marshal，将XML转换为Java对象的过程称之为UnMarshal。我们可以通过在 Java 类中标注注解的方式将一个Java对象绑定到一段XML，也就是说，在Java类中标注一些注解，这些注解定义了如何将这个类转换为XML，怎么转换，以及一段XML如何被解析成这个类所定义的对象；也可以使用JAXB的XJC工具，通过定义schema的方式实现Java对象与XML的绑定（这个下次研究）。
    下面来了解一下如何通过标注注解来完成 Marshal 和 UnMarshal 的过程。我用的是 JAXB2_20101209.jar ，可以到[http://jaxb.java.net/](http://jaxb.java.net/)下载最新版本。
首先看个小例子：
定义一个java类
Java代码
package com.why.jaxb;   
import javax.xml.bind.annotation.XmlRootElement;   
@XmlRootElement  
public class People {   
    public String id = "001";   
    public String name = "灰太狼";   
    public int age = 26;   
}  
//我们在实际中一般在get方法上添加相关注解
Java To XML（Marshal）
Java代码
package com.why.jaxb;   
import javax.xml.bind.JAXBContext;   
import javax.xml.bind.JAXBException;   
import javax.xml.bind.Marshaller;   
public class Java2XML {   
    /**  
     * @param args  
     * @throws JAXBException   
     */  
    public static void main(String[] args) throws JAXBException {   
        JAXBContext context = JAXBContext.newInstance(People.class);   
        Marshaller marshaller = context.createMarshaller();   
        marshaller.setProperty(Marshaller.JAXB_ENCODING,"gb2312");//编码格式
        //我在实际开发中重新封装了JAXB基本类，这里就使用到了该属性。不过默认的编码格式UTF-8
        marshaller.setProperty(Marshaller.JAXB_FORMATTED_OUTPUT, true);//是否格式化生成的xml串   
        marshaller.setProperty(Marshaller.JAXB_FRAGMENT, false);//是否省略xml头信息（<?xml version="1.0" encoding="gb2312" standalone="yes"?>）   
        People people = new People();   
        marshaller.marshal(people, System.out);   
    }   
}  
XML To Java（UnMarshal）
Java代码
package com.why.jaxb;   
import java.io.File;   
import javax.xml.bind.JAXBContext;   
import javax.xml.bind.JAXBException;   
import javax.xml.bind.Unmarshaller;   
import org.xml.sax.SAXException;   
public class XML2Java {   
    /**  
     * @param args  
     * @throws JAXBException   
     * @throws JAXBException   
     * @throws SAXException   
     */  
    public static void main(String[] args) throws JAXBException {   
        JAXBContext context = JAXBContext.newInstance(People.class);   
        Unmarshaller unmarshaller = context.createUnmarshaller();   
        File file = new File("src/people.xml");   
        People people = (People)unmarshaller.unmarshal(file);   
        System.out.println(people.id);   
        System.out.println(people.name);   
        System.out.println(people.age);   
    }   
}
       其实Marshal 和 UnMarshal的过程并不复杂，只需要从JAXBContext中获得Marshaller或Unmarshaller对象，就可以让JAXB帮我们来进行转换了。我们需要操作的主要内容是定义一个规则，告诉JAXB如何将一个类、按照什么样的格式转换为XML，下面是JAXB中主要的一些注解。
@XmlRootElement   将一个Java类映射为一段XML的根节点
参数：name            定义这个根节点的名称
          namespace   定义这个根节点命名空间
@XmlAccessorType  定义映射这个类中的何种类型需要映射到XML。可接收四个参数，分别是：
      XmlAccessType.FIELD：映射这个类中的所有字段到XML
      XmlAccessType.PROPERTY：映射这个类中的属性（get/set方法）到XML
      XmlAccessType.PUBLIC_MEMBER：将这个类中的所有public的field或property同时映射到XML（默认）
      XmlAccessType.NONE：不映射
@XmlElement  指定一个字段或get/set方法映射到XML的节点。如，当一个类的XmlAccessorType 被标注为PROPERTY时，在某一个没有get/set方法的字段上标注此注解，即可将该字段映射到XML。
参数：defaultValue  指定节点默认值
         name             指定节点名称
         namespace    指定节点命名空间
**required         是否必须（默认为false，解决null 或 空值 标签不显示）**
         nillable           该字段是否包含 nillable="true" 属性（默认为false）
         type               定义该字段或属性的关联类型
@XmlAttribute  指定一个字段或get/set方法映射到XML的属性。
参数：name             指定属性名称
         namespace    指定属性命名空间
** required         是否必须（默认为false）**
@XmlTransient  定义某一字段或属性不需要被映射为XML。如，当一个类的XmlAccessorType 被标注为PROPERTY时，在某一get/set方法的字段上标注此注解，那么该属性则不会被映射。
@XmlType 定义映射的一些相关规则
参数：propOrder        指定映射XML时的节点顺序
         factoryClass     指定UnMarshal时生成映射类实例所需的工厂类，默认为这个类本身
         factoryMethod  指定工厂类的工厂方法
         name               定义XML Schema中type的名称
         namespace      指定Schema中的命名空间
@XmlElementWrapper  为数组元素或集合元素定义一个父节点。如，类中有一元素为List items，若不加此注解，该元素将被映射为
    <items>...</items>
    <items>...</items>
这种形式，此注解可将这个元素进行包装，如：
    @XmlElementWrapper(name="items")
    @XmlElement(name="item")
    public List items;
将会生成这样的XML样式：
    <items>
        <item>...</item>
        <item>...</item>
    </items>
@XmlJavaTypeAdapter  自定义某一字段或属性映射到XML的适配器。如，类中包含一个接口，我们可以定义一个适配器（继承自 javax.xml.bind.annotation.adapters.XmlAdapter类），指定这个接口如何映射到XML。
@XmlSchema 配置整个包的namespace，这个注解需放在package-info.java文件中。
jaxb编码：
           JAXBContext jaxbContext = JAXBContext.newInstance(clazz);
            Unmarshaller unmarshaller = jaxbContext.createUnmarshaller();
            InputStreamReader reader=new InputStreamReader(inputStream,"GBK");   //在此修改编码
            return unmarshaller.unmarshal(reader);
上述文档来自于百度一个网友，现在出处不明了，但是在实际开发中我重新封装了一些，并做了相应的处理。另外可能会遇到一些List和Map之间的映射，看到后应该可以模仿。我有参考实例，需要的网友联系我。。。
