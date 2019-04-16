# Java学习笔记39：Spring XML使用 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2013年04月24日 20:12:50[initphp](https://me.csdn.net/initphp)阅读数：1319
个人分类：[Java学习笔记系列](https://blog.csdn.net/initphp/article/category/1230072)

所属专栏：[Java学习笔记](https://blog.csdn.net/column/details/javago.html)









我们使用jdom的XML解析库，所以pom.xml中需要依赖：



```
<!-- XML解析 -->
		<dependency>
			<groupId>org.jdom</groupId>
			<artifactId>jdom</artifactId>
			<version>2.0.2</version>
		</dependency>
```


**创建XML实例：**



```java
Element rootElement = new Element("root"); //创建根节点
        
        //属性创建
        rootElement.setAttribute("title", "this is test xml"); //root节点上穿件一个title属性
        rootElement.setAttribute("id", "root"); //root节点上穿件一个title属性
        
        //创建person1子节点
        Element person1 = new Element("person1"); //创建person1节点
        person1.addContent(new Element("name").setText("zhuli")); //创建person1中的name节点和内容
        person1.addContent(new Element("age").setText("30岁"));//创建person1中的age节点和内容
        
        //创建person2子节点
        Element person2 = new Element("person2"); //创建person2节点
        person2.addContent(new Element("name").setText("zhuli")); //创建person2中的name节点和内容
        person2.addContent(new Element("age").setText("30岁"));//创建person2中的age节点和内容
        
        //root节点上面添加子节点
        rootElement.addContent(person1); //将person1节点加入到root根节点
        rootElement.addContent(person2); //将person1节点加入到root根节点
        
        rootElement.addContent(new Element("result").setText("ss").setAttribute("id", "result")); //只有一层的子节点
        rootElement.addContent(new Element("detail").setText("asdasda"));
        
        Document myDocument = new Document(rootElement);
        XMLOutputter xmlOut = new XMLOutputter(Format.getPrettyFormat());
        return xmlOut.outputString(myDocument);
```


结果：





```
<?xml version="1.0" encoding="UTF-8"?>
<root title="this is test xml" id="root">
  <person1>
    <name>zhuli</name>
    <age>30岁</age>
  </person1>
  <person2>
    <name>zhuli</name>
    <age>30岁</age>
  </person2>
  <result id="result">ss</result>
  <detail>asdasda</detail>
</root>
```


**解析XML实例：**

解析上面的xml文档****



```java
@SuppressWarnings("deprecation")
        SAXBuilder builder = new SAXBuilder(false);    
        try {     
            InputStream isXml = new ByteArrayInputStream(xml.getBytes("UTF-8"));
            Document document = builder.build(isXml); //获取XML文档实例，字符串XML格式，如果是URL是builder.url()
            Element root = document.getRootElement(); //获取根节点
            
            //获取根节点root上的title和id属性
            System.out.println("root title:" + root.getAttributeValue("title"));
            System.out.println("root id:" + root.getAttributeValue("id"));
            
            //person1节点：
            System.out.println("person1节点：");
            Element person1 = root.getChild("person1");
            System.out.println("person1 name:" + person1.getChildText("name"));
            System.out.println("person1 age:" + person1.getChildText("age"));
            
            //person2节点：
            System.out.println("person2节点：");
            Element person2 = root.getChild("person2");
            System.out.println("person2 name:" + person2.getChildText("name"));
            System.out.println("person2 age:" + person2.getChildText("age"));
            
            //获取result值
            System.out.println("result text:" + root.getChildText("result"));
            
            //
            System.out.println("detail text:" + root.getChildText("detail"));
        } catch (Exception e) {

        }
```

控制台结果：


```
root title:this is test xml
root id:root
person1节点：
person1 name:zhuli
person1 age:30岁
person2节点：
person2 name:zhuli
person2 age:30岁
result text:ss
detail text:asdasda
```









