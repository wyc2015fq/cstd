# java解析XML的几种方式 - 零度的博客专栏 - CSDN博客
2015年06月26日 16:55:14[零度anngle](https://me.csdn.net/zmx729618)阅读数：609
第一种：DOM。
DOM的全称是Document Object Model，也即文档对象模型。在应用程序中，基于DOM的XML分析器将一个XML文档转换成一个对象模型的集合（通常称DOM树），应用程序正是通过对这个对象模型的操作，来实现对XML文档数据的操作。通过DOM接口，应用程序可以在任何时候访问XML文档中的任何一部分数据，因此，这种利用DOM接口的机制也被称作随机访问机制。
DOM接口提供了一种通过分层对象模型来访问XML文档信息的方式，这些分层对象模型依据XML的文档结构形成了一棵节点树。无论XML文档中所描述的是什么类型的信息，即便是制表数据、项目列表或一个文档，利用DOM所生成的模型都是节点树的形式。也就是说，DOM强制使用树模型来访问XML文档中的信息。由于XML本质上就是一种分层结构，所以这种描述方法是相当有效的。
DOM树所提供的随机访问方式给应用程序的开发带来了很大的灵活性，它可以任意地控制整个XML文档中的内容。然而，由于DOM分析器把整个XML文档转化成DOM树放在了内存中，因此，当文档比较大或者结构比较复杂时，对内存的需求就比较高。而且，对于结构复杂的树的遍历也是一项耗时的操作。所以，DOM分析器对机器性能的要求比较高，实现效率不十分理想。不过，由于DOM分析器所采用的树结构的思想与XML文档的结构相吻合，同时鉴于随机访问所带来的方便，因此，DOM分析器还是有很广泛的使用价值的。
```java
import java.io.File;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NodeList;
public class DomTest1
{
	public static void main(String[] args) throws Exception
	{
		// step 1: 获得dom解析器工厂（工作的作用是用于创建具体的解析器）
		DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
		
//		System.out.println("class name: " + dbf.getClass().getName());
		
		// step 2:获得具体的dom解析器
		DocumentBuilder db = dbf.newDocumentBuilder();
		
//		System.out.println("class name: " + db.getClass().getName());
		
		// step3: 解析一个xml文档，获得Document对象（根结点）
		Document document = db.parse(new File("candidate.xml"));
		
		NodeList list = document.getElementsByTagName("PERSON");
		
		for(int i = 0; i < list.getLength(); i++)
		{
			Element element = (Element)list.item(i);
			
			String content = element.getElementsByTagName("NAME").item(0).getFirstChild().getNodeValue();
			
			System.out.println("name:" + content);
			
			content = element.getElementsByTagName("ADDRESS").item(0).getFirstChild().getNodeValue();
			
			System.out.println("address:" + content);
			
			content = element.getElementsByTagName("TEL").item(0).getFirstChild().getNodeValue();
			
			System.out.println("tel:" + content);
			
			content = element.getElementsByTagName("FAX").item(0).getFirstChild().getNodeValue();
			
			System.out.println("fax:" + content);
			
			content = element.getElementsByTagName("EMAIL").item(0).getFirstChild().getNodeValue();
			
			System.out.println("email:" + content);
			
			System.out.println("--------------------------------------");
		}
	}
}
```
```java
import java.io.File;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import org.w3c.dom.Attr;
import org.w3c.dom.Comment;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
import org.w3c.dom.NamedNodeMap;
import org.w3c.dom.Node;
import org.w3c.dom.NodeList;
/**
 * 使用递归解析给定的任意一个xml文档并且将其内容输出到命令行上
 * @author zhanglong
 *
 */
public class DomTest3
{
	public static void main(String[] args) throws Exception
	{
		DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
		DocumentBuilder db = dbf.newDocumentBuilder();
		
		Document doc = db.parse(new File("student.xml"));
		//获得根元素结点
		Element root = doc.getDocumentElement();
		
		parseElement(root);
	}
	
	private static void parseElement(Element element)
	{
		String tagName = element.getNodeName();
		
		NodeList children = element.getChildNodes();
		
		System.out.print("<" + tagName);
		
		//element元素的所有属性所构成的NamedNodeMap对象，需要对其进行判断
		NamedNodeMap map = element.getAttributes();
		
		//如果该元素存在属性
		if(null != map)
		{
			for(int i = 0; i < map.getLength(); i++)
			{
				//获得该元素的每一个属性
				Attr attr = (Attr)map.item(i);
				
				String attrName = attr.getName();
				String attrValue = attr.getValue();
				
				System.out.print(" " + attrName + "=\"" + attrValue + "\"");
			}
		}
		
		System.out.print(">");
		
		for(int i = 0; i < children.getLength(); i++)
		{
			Node node = children.item(i);
			//获得结点的类型
			short nodeType = node.getNodeType();
			
			if(nodeType == Node.ELEMENT_NODE)
			{
				//是元素，继续递归
				parseElement((Element)node);
			}
			else if(nodeType == Node.TEXT_NODE)
			{
				//递归出口
				System.out.print(node.getNodeValue());
			}
			else if(nodeType == Node.COMMENT_NODE)
			{
				System.out.print("<!--");
				
				Comment comment = (Comment)node;
				
				//注释内容
				String data = comment.getData();
				
				System.out.print(data);
				
				System.out.print("-->");
			}
		}
		
		System.out.print("</" + tagName + ">");
	}
}
```
sax：SAX的全称是Simple APIs for XML，也即XML简单应用程序接口。与DOM不同，SAX提供的访问模式是一种顺序模式，这是一种快速读写XML数据的方式。当使用SAX分析器对XML文档进行分析时，会触发一系列事件，并激活相应的事件处理函数，应用程序通过这些事件处理函数实现对XML文档的访问，因而SAX接口也被称作事件驱动接口。
```java
import java.io.File;
import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;
import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;
public class SaxTest1
{
	public static void main(String[] args) throws Exception
	{
		//step1： 获得SAX解析器工厂实例
		SAXParserFactory factory = SAXParserFactory.newInstance();
		
		//step2: 获得SAX解析器实例
		SAXParser parser = factory.newSAXParser();
		
		//step3: 开始进行解析
		parser.parse(new File("student.xml"), new MyHandler());
		
	}
}
class MyHandler extends DefaultHandler
{
	@Override
	public void startDocument() throws SAXException
	{
		System.out.println("parse began");
	}
	
	@Override
	public void endDocument() throws SAXException
	{
		System.out.println("parse finished");
	}
	
	@Override
	public void startElement(String uri, String localName, String qName,
			Attributes attributes) throws SAXException
	{
		System.out.println("start element");
	}
	
	@Override
	public void endElement(String uri, String localName, String qName)
			throws SAXException
	{
		System.out.println("finish element");
	}
}
```
```java
import java.io.File;
import java.util.Stack;
import javax.xml.parsers.SAXParser;
import javax.xml.parsers.SAXParserFactory;
import org.xml.sax.Attributes;
import org.xml.sax.SAXException;
import org.xml.sax.helpers.DefaultHandler;
public class SaxTest2
{
	public static void main(String[] args) throws Exception
	{
		SAXParserFactory factory = SAXParserFactory.newInstance();
		
		SAXParser parser = factory.newSAXParser();
		
		parser.parse(new File("student.xml"), new MyHandler2());
	}
}
class MyHandler2 extends DefaultHandler
{
	private Stack<String> stack = new Stack<String>();
	
	private String name;
	
	private String gender;
	
	private String age;
	
	@Override
	public void startElement(String uri, String localName, String qName,
			Attributes attributes) throws SAXException
	{
		stack.push(qName);
		
		for(int i = 0; i < attributes.getLength(); i++)
		{
			String attrName = attributes.getQName(i);
			String attrValue = attributes.getValue(i);
			
			System.out.println(attrName + "=" + attrValue);
		}
	}
	
	@Override
	public void characters(char[] ch, int start, int length)
			throws SAXException
	{
		String tag = stack.peek();
		
		if("姓名".equals(tag))
		{
			name = new String(ch, start,length);
		}
		else if("性别".equals(tag))
		{
			gender = new String(ch, start, length);
		}
		else if("年龄".equals(tag))
		{
			age = new String(ch, start, length);
		}
	}
	
	@Override
	public void endElement(String uri, String localName, String qName)
			throws SAXException
	{
		stack.pop(); //表示该元素已经解析完毕，需要从栈中弹出
		
		if("学生".equals(qName))
		{
			System.out.println("姓名：" + name);
			System.out.println("性别：" + gender);
			System.out.println("年龄：" + age);
			
			System.out.println();
		}
		
	}
}
```
JDOM:
JDOM是一个开源项目，它基于树型结构，利用纯JAVA的技术对XML文档实现解析、生成、序列化以及多种操作。（http://jdom.org）
•JDOM 直接为JAVA编程服务。它利用更为强有力的JAVA语言的诸多特性（方法重载、集合概念等），把SAX和DOM的功能有效地结合起来。
•JDOM是用Java语言读、写、操作XML的新API函数。在直接、简单和高效的前提下，这些API函数被最大限度的优化。
jdom创建xml
```java
import java.io.FileWriter;
import org.jdom.Attribute;
import org.jdom.Comment;
import org.jdom.Document;
import org.jdom.Element;
import org.jdom.output.Format;
import org.jdom.output.XMLOutputter;
public class JDomTest1
{
	public static void main(String[] args) throws Exception
	{
		Document document = new Document();
		Element root = new Element("root");
		document.addContent(root);
		Comment comment = new Comment("This is my comments");
		root.addContent(comment);
		Element e = new Element("hello");
		e.setAttribute("sohu", "www.sohu.com");
		root.addContent(e);
		Element e2 = new Element("world");
		Attribute attr = new Attribute("test", "hehe");
		e2.setAttribute(attr);
		e.addContent(e2);
		e2.addContent(new Element("aaa").setAttribute("a", "b")
				.setAttribute("x", "y").setAttribute("gg", "hh").setText("text content"));
		
		Format format = Format.getPrettyFormat();
		
		format.setIndent("    ");
//		format.setEncoding("gbk");
		
		XMLOutputter out = new XMLOutputter(format);
		out.output(document, new FileWriter("jdom.xml"));
		
	}
}
```
JDOM解析xml
```java
import java.io.File;
import java.io.FileOutputStream;
import java.util.List;
import org.jdom.Attribute;
import org.jdom.Document;
import org.jdom.Element;
import org.jdom.input.SAXBuilder;
import org.jdom.output.Format;
import org.jdom.output.XMLOutputter;
public class JDomTest2
{
	public static void main(String[] args) throws Exception
	{
		SAXBuilder builder = new SAXBuilder();
		
		Document doc = builder.build(new File("jdom.xml"));
		
		Element element = doc.getRootElement();
		
		System.out.println(element.getName());
		
		Element hello = element.getChild("hello");
		
		System.out.println(hello.getText());
		
		List list = hello.getAttributes();
		
		for(int i = 0 ;i < list.size(); i++)
		{
			Attribute attr = (Attribute)list.get(i);
			
			String attrName = attr.getName();
			String attrValue = attr.getValue();
			
			System.out.println(attrName + "=" + attrValue);
		}
		
		hello.removeChild("world");
		
		XMLOutputter out = new XMLOutputter(Format.getPrettyFormat().setIndent("    "));
		
		
		out.output(doc, new FileOutputStream("jdom2.xml"));		
		
	}
}
```
Dom4j
```java
import java.io.FileOutputStream;
import java.io.FileWriter;
import org.dom4j.Document;
import org.dom4j.DocumentHelper;
import org.dom4j.Element;
import org.dom4j.io.OutputFormat;
import org.dom4j.io.XMLWriter;
public class Test1
{
	public static void main(String[] args) throws Exception
	{
		// 创建文档并设置文档的根元素节点 ：第一种方式
		// Document document = DocumentHelper.createDocument();
		//
		// Element root = DocumentHelper.createElement("student");
		//
		// document.setRootElement(root);
		// 创建文档并设置文档的根元素节点 ：第二种方式
		Element root = DocumentHelper.createElement("student");
		Document document = DocumentHelper.createDocument(root);
		root.addAttribute("name", "zhangsan");
		Element helloElement = root.addElement("hello");
		Element worldElement = root.addElement("world");
		helloElement.setText("hello");
		worldElement.setText("world");
		helloElement.addAttribute("age", "20");
		XMLWriter xmlWriter = new XMLWriter();
		xmlWriter.write(document);
		
		OutputFormat format = new OutputFormat("    ", true);
		
		XMLWriter xmlWriter2 = new XMLWriter(new FileOutputStream("student2.xml"), format);
		xmlWriter2.write(document);
		
		XMLWriter xmlWriter3 = new XMLWriter(new FileWriter("student3.xml"), format);
		
		xmlWriter3.write(document);
		xmlWriter3.close();
	}
}
```
```java
import java.io.File;
import java.util.Iterator;
import java.util.List;
import javax.xml.parsers.DocumentBuilder;
import javax.xml.parsers.DocumentBuilderFactory;
import org.dom4j.Document;
import org.dom4j.Element;
import org.dom4j.io.DOMReader;
import org.dom4j.io.SAXReader;
public class Test2
{
	public static void main(String[] args) throws Exception
	{
		SAXReader saxReader = new SAXReader();
		
		Document doc = saxReader.read(new File("student2.xml"));
		
		Element root = doc.getRootElement();
		
		System.out.println("root element: " + root.getName());
		
		List childList = root.elements();
		
		System.out.println(childList.size());
		
		List childList2 = root.elements("hello");
		
		System.out.println(childList2.size());
		
		Element first = root.element("hello");
		
		System.out.println(first.attributeValue("age"));
		
		for(Iterator iter = root.elementIterator(); iter.hasNext();)
		{
			Element e = (Element)iter.next();
			
			System.out.println(e.attributeValue("age"));
		}
		
		System.out.println("---------------------------");
		
		DocumentBuilderFactory dbf = DocumentBuilderFactory.newInstance();
		DocumentBuilder db = dbf.newDocumentBuilder();
		org.w3c.dom.Document document = db.parse(new File("student2.xml"));
		
		DOMReader domReader = new DOMReader();
		
		//将JAXP的Document转换为dom4j的Document
		Document d = domReader.read(document);
		
		Element rootElement = d.getRootElement();
		
		System.out.println(rootElement.getName());
	}
}
```
