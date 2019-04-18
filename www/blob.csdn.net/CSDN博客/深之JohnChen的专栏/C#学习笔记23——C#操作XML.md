# C#学习笔记23——C#操作XML - 深之JohnChen的专栏 - CSDN博客

2011年09月03日 10:35:40[byxdaz](https://me.csdn.net/byxdaz)阅读数：1323标签：[c#																[encoding																[xml																[string																[null																[user](https://so.csdn.net/so/search/s.do?q=user&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=xml&t=blog)](https://so.csdn.net/so/search/s.do?q=encoding&t=blog)](https://so.csdn.net/so/search/s.do?q=c#&t=blog)
个人分类：[.Net & C#](https://blog.csdn.net/byxdaz/article/category/628880)


**C#操作XML**

一、简单介绍

using System.Xml;
//初始化一个xml实例
XmlDocument xml=new XmlDocument();

//导入指定xml文件
xml.Load(path);
xml.Load(HttpContext.Current.Server.MapPath("~/file/bookstore.xml"));

//指定一个节点
XmlNode root=xml.SelectSingleNode("/root");

//获取节点下所有直接子节点
XmlNodeList childlist=root.ChildNodes;

//判断该节点下是否有子节点
root.HasChildNodes;

//获取同名同级节点集合
XmlNodeList nodelist=xml.SelectNodes("/Root/News");

//生成一个新节点
XmlElement node=xml.CreateElement("News");

//将节点加到指定节点下，作为其子节点
root.AppendChild(node);

//将节点加到指定节点下某个子节点前
root.InsertBefore(node,root.ChildeNodes[i]);

//为指定节点的新建属性并赋值
node.SetAttribute("id","11111");

//为指定节点添加子节点
root.AppendChild(node);

//获取指定节点的指定属性值
string id"].Value;

//获取指定节点中的文本
string content=node.InnerText;

//保存XML文件
string path=Server.MapPath("~/file/bookstore.xml");
xml.Save(path);
//or use:xml.Save(HttpContext.Current.Server.MapPath("~/file/bookstore.xml"));

二、具体实例

定义几个公共对象：
XmlDocument xmldoc ;
XmlNode xmlnode ;
XmlElement xmlelem ;

1，创建到服务器同名目录下的xml文件：

方法一：
xmldoc = new XmlDocument ( ) ;
//加入XML的声明段落,<?xmlversion="1.0" encoding="gb2312"?>
XmlDeclaration xmldecl;
xmldecl = xmldoc.CreateXmlDeclaration("1.0","gb2312",null);
xmldoc.AppendChild ( xmldecl);

//加入一个根元素
xmlelem = xmldoc.CreateElement ( "" , "Employees" ,"" ) ;
xmldoc.AppendChild ( xmlelem ) ;
//加入另外一个元素
for(int i=1;i<3;i++)
{

XmlNoderoot=xmldoc.SelectSingleNode("Employees");//查找<Employees> 
XmlElement xe1=xmldoc.CreateElement("Node");//创建一个<Node>节点 
xe1.SetAttribute("genre","李赞红");//设置该节点genre属性 
xe1.SetAttribute("ISBN","2-3631-4");//设置该节点ISBN属性 

XmlElementxesub1=xmldoc.CreateElement("title"); 
xesub1.InnerText="CS从入门到精通";//设置文本节点 
xe1.AppendChild(xesub1);//添加到<Node>节点中 
XmlElement xesub2=xmldoc.CreateElement("author"); 
xesub2.InnerText="候捷"; 
xe1.AppendChild(xesub2); 
XmlElement xesub3=xmldoc.CreateElement("price"); 
xesub3.InnerText="58.3"; 
xe1.AppendChild(xesub3); 

root.AppendChild(xe1);//添加到<Employees>节点中 
}
//保存创建好的XML文档
xmldoc.Save ( Server.MapPath("data.xml") ) ; 

//////////////////////////////////////////////////////////////////////////////////////
结果：在同名目录下生成了名为data.xml的文件，内容如下，
<?xml version="1.0" encoding="gb2312"?>
<Employees>
<Node genre="李赞红" ISBN="2-3631-4">
<title>CS从入门到精通</title>
<author>候捷</author>
<price>58.3</price>
</Node>
<Node genre="李赞红" ISBN="2-3631-4">
<title>CS从入门到精通</title>
<author>候捷</author>
<price>58.3</price>
</Node>
</Employees>

方法二：
XmlTextWriter xmlWriter;
string strFilename = Server.MapPath("data1.xml") ;

xmlWriter = newXmlTextWriter(strFilename,Encoding.Default);//创建一个xml文档
xmlWriter.Formatting = Formatting.Indented;
xmlWriter.WriteStartDocument();
xmlWriter.WriteStartElement("Employees");

xmlWriter.WriteStartElement("Node");
xmlWriter.WriteAttributeString("genre","李赞红");
xmlWriter.WriteAttributeString("ISBN","2-3631-4");

xmlWriter.WriteStartElement("title");
xmlWriter.WriteString("CS从入门到精通");
xmlWriter.WriteEndElement();

xmlWriter.WriteStartElement("author");
xmlWriter.WriteString("候捷");
xmlWriter.WriteEndElement();

xmlWriter.WriteStartElement("price");
xmlWriter.WriteString("58.3");
xmlWriter.WriteEndElement();

xmlWriter.WriteEndElement();

xmlWriter.WriteEndElement();

xmlWriter.Close();
//////////////////////////////////////////////////////////////////////////////////////
结果：
<?xml version="1.0" encoding="gb2312"?>
<Employees>
<Node genre="李赞红" ISBN="2-3631-4">
<title>CS从入门到精通</title>
<author>候捷</author>
<price>58.3</price>
</Node>
</Employees>

2，添加一个结点：

XmlDocument xmlDoc=new XmlDocument(); 
xmlDoc.Load(Server.MapPath("data.xml")); 
XmlNode root=xmlDoc.SelectSingleNode("Employees");//查找<Employees> 
XmlElement xe1=xmlDoc.CreateElement("Node");//创建一个<Node>节点 
xe1.SetAttribute("genre","张三");//设置该节点genre属性 
xe1.SetAttribute("ISBN","1-1111-1");//设置该节点ISBN属性 

XmlElementxesub1=xmlDoc.CreateElement("title"); 
xesub1.InnerText="C#入门帮助";//设置文本节点 
xe1.AppendChild(xesub1);//添加到<Node>节点中 
XmlElement xesub2=xmlDoc.CreateElement("author"); 
xesub2.InnerText="高手"; 
xe1.AppendChild(xesub2); 
XmlElement xesub3=xmlDoc.CreateElement("price"); 
xesub3.InnerText="158.3"; 
xe1.AppendChild(xesub3); 

root.AppendChild(xe1);//添加到<Employees>节点中 
xmlDoc.Save ( Server.MapPath("data.xml") );

//////////////////////////////////////////////////////////////////////////////////////
结果：在xml原有的内容里添加了一个结点，内容如下，
<?xml version="1.0" encoding="gb2312"?>
<Employees>
<Node genre="李赞红" ISBN="2-3631-4">
<title>CS从入门到精通</title>
<author>候捷</author>
<price>58.3</price>
</Node>
<Node genre="李赞红" ISBN="2-3631-4">
<title>CS从入门到精通</title>
<author>候捷</author>
<price>58.3</price>
</Node>
<Node genre="张三" ISBN="1-1111-1">
<title>C#入门帮助</title>
<author>高手</author>
<price>158.3</price>
</Node>
</Employees>

3，修改结点的值（属性和子结点）：

XmlDocument xmlDoc=new XmlDocument(); 
xmlDoc.Load( Server.MapPath("data.xml") ); 

XmlNodeListnodeList=xmlDoc.SelectSingleNode("Employees").ChildNodes;//获取Employees节点的所有子节点 

foreach(XmlNode xn in nodeList)//遍历所有子节点 
{ 
XmlElement xe=(XmlElement)xn;//将子节点类型转换为XmlElement类型 
if(xe.GetAttribute("genre")=="张三")//如果genre属性值为“张三” 
{ 
xe.SetAttribute("genre","update张三");//则修改该属性为“update张三” 

XmlNodeList nls=xe.ChildNodes;//继续获取xe子节点的所有子节点 
foreach(XmlNode xn1 innls)//遍历 
{ 
XmlElement xe2=(XmlElement)xn1;//转换类型 
if(xe2.Name=="author")//如果找到 
{ 
xe2.InnerText="亚胜";//则修改
} 
} 
} 
} 
xmlDoc.Save( Server.MapPath("data.xml") );//保存。

//////////////////////////////////////////////////////////////////////////////////////
结果：将原来的所有结点的信息都修改了，xml的内容如下，
<?xml version="1.0" encoding="gb2312"?>
<Employees>
<Node genre="李赞红" ISBN="2-3631-4">
<title>CS从入门到精通</title>
<author>候捷</author>
<price>58.3</price>
</Node>
<Node genre="李赞红" ISBN="2-3631-4">
<title>CS从入门到精通</title>
<author>候捷</author>
<price>58.3</price>
</Node>
<Node genre="update张三" ISBN="1-1111-1">
<title>C#入门帮助</title>
<author>亚胜</author>
<price>158.3</price>
</Node>
</Employees>

4，修改结点（添加结点的属性和添加结点的自结点）：
XmlDocument xmlDoc=new XmlDocument(); 
xmlDoc.Load( Server.MapPath("data.xml") ); 

XmlNodeListnodeList=xmlDoc.SelectSingleNode("Employees").ChildNodes;//获取Employees节点的所有子节点 

foreach(XmlNode xn in nodeList) 
{ 
XmlElement xe=(XmlElement)xn; 
xe.SetAttribute("test","111111");

XmlElementxesub=xmlDoc.CreateElement("flag"); 
xesub.InnerText="1"; 
xe.AppendChild(xesub); 
} 
xmlDoc.Save( Server.MapPath("data.xml") );

//////////////////////////////////////////////////////////////////////////////////////
结果：每个结点的属性都添加了一个，子结点也添加了一个，内容如下，
<?xml version="1.0" encoding="gb2312"?>
<Employees>
<Node genre="李赞红" ISBN="2-3631-4" test="111111">
<title>CS从入门到精通</title>
<author>候捷</author>
<price>58.3</price>
<flag>1</flag>
</Node>
<Node genre="李赞红" ISBN="2-3631-4" test="111111">
<title>CS从入门到精通</title>
<author>候捷</author>
<price>58.3</price>
<flag>1</flag>
</Node>
<Node genre="update张三" ISBN="1-1111-1" test="111111">
<title>C#入门帮助</title>
<author>亚胜</author>
<price>158.3</price>
<flag>1</flag>
</Node>
</Employees>

5，删除结点中的某一个属性：
XmlDocument xmlDoc=new XmlDocument(); 
xmlDoc.Load( Server.MapPath("data.xml") ); 
XmlNodeList xnl=xmlDoc.SelectSingleNode("Employees").ChildNodes; 
foreach(XmlNode xn in xnl) 
{ 
XmlElement xe=(XmlElement)xn; 
xe.RemoveAttribute("genre");//删除genre属性 

XmlNodeList nls=xe.ChildNodes;//继续获取xe子节点的所有子节点 
foreach(XmlNode xn1 innls)//遍历 
{ 
XmlElement xe2=(XmlElement)xn1;//转换类型 
if(xe2.Name=="flag")//如果找到 
{ 
xe.RemoveChild(xe2);//则删除
} 
} 
} 
xmlDoc.Save( Server.MapPath("data.xml") ); 

//////////////////////////////////////////////////////////////////////////////////////]
结果：删除了结点的一个属性和结点的一个子结点，内容如下，
<?xml version="1.0" encoding="gb2312"?>
<Employees>
<Node ISBN="2-3631-4" test="111111">
<title>CS从入门到精通</title>
<author>候捷</author>
<price>58.3</price>
</Node>
<Node ISBN="2-3631-4" test="111111">
<title>CS从入门到精通</title>
<author>候捷</author>
<price>58.3</price>
</Node>
<Node ISBN="1-1111-1" test="111111">
<title>C#入门帮助</title>
<author>亚胜</author>
<price>158.3</price>
</Node>
</Employees>

6，删除结点：
XmlDocument xmlDoc=new XmlDocument(); 
xmlDoc.Load( Server.MapPath("data.xml") ); 
XmlNode root=xmlDoc.SelectSingleNode("Employees");
XmlNodeList xnl=xmlDoc.SelectSingleNode("Employees").ChildNodes; 
for(int i=0;i<xnl.Count;i++)
{
XmlElement xe=(XmlElement)xnl.Item(i); 
if(xe.GetAttribute("genre")=="张三") 
{ 
root.RemoveChild(xe);
if(i<xnl.Count)i=i-1;
} 
}
xmlDoc.Save( Server.MapPath("data.xml") ); 

//////////////////////////////////////////////////////////////////////////////////////]
结果：删除了符合条件的所有结点，原来的内容：

<?xml version="1.0"encoding="gb2312"?>
<Employees>
<Node genre="李赞红" ISBN="2-3631-4">
<title>CS从入门到精通</title>
<author>候捷</author>
<price>58.3</price>
</Node>
<Node genre="李赞红" ISBN="2-3631-4">
<title>CS从入门到精通</title>
<author>候捷</author>
<price>58.3</price>
</Node>
<Node genre="张三" ISBN="1-1111-1">
<title>C#入门帮助</title>
<author>高手</author>
<price>158.3</price>
</Node>
<Node genre="张三" ISBN="1-1111-1">
<title>C#入门帮助</title>
<author>高手</author>
<price>158.3</price>
</Node>
</Employees>

删除后的内容：
<?xml version="1.0" encoding="gb2312"?>
<Employees>
<Node genre="李赞红" ISBN="2-3631-4">
<title>CS从入门到精通</title>
<author>候捷</author>
<price>58.3</price>
</Node>
<Node genre="李赞红" ISBN="2-3631-4">
<title>CS从入门到精通</title>
<author>候捷</author>
<price>58.3</price>
</Node>
</Employees>

7，按照文本文件读取xml

System.IO.StreamReader myFile =new 
System.IO.StreamReader(Server.MapPath("data.xml"),System.Text.Encoding.Default);
//注意System.Text.Encoding.Default

string myString =myFile.ReadToEnd();//myString是读出的字符串
myFile.Close();

三、高级应用

/*读取xml数据 两种xml方式*/
<aaa>
<bb>something</bb>
<cc>something</cc>
</aaa>

<aaa>
<add key="123" value="321"/>
</aaa>

/*第一种方法*/

Dataset DS;
DS.ReadXml("your xmlfile name");
Container.DataItem("bb");
Container.DataItem("cc");
DS.ReadXmlSchema("your xmlfile name");

/*第二种方法*/
<aaa>
<add key="123" value="321"/>
</aaa>
如果我要找到123然后取到321应该怎么写呢？

using System.XML;
XmlDataDocument xmlDoc = new System.Xml.XmlDataDocument();
xmlDoc.Load(@"c:\Config.xml");
XmlElement elem = xmlDoc.GetElementById("add");
string str = elem.Attributes["value"].Value

/*第三种方法:SelectSingleNode 读取两种格式的xml*---/
--------------------------------------------------------------------
<?xml version="1.0" encoding="utf-8" ?>
<configuration>
<appSettings>
<ConnectionString>Data Source=yf; user </ConnectionString>
</appSettings>
</configuration>
--------------------------------------------------------------------------
XmlDocument doc = new XmlDocument();
doc.Load(strXmlName);

XmlNodenode=doc.SelectSingleNode("/configuration/appSettings/ConnectionString");
if(node!=null)
{
string k1=node.Value; //null
string k2=node.InnerText;//Data Source=yf; user > stringk3=node.InnerXml;//Data Source=yf; user > node=null;
}

********************************************************************
<?xml version="1.0" encoding="utf-8" ?>
<configuration>
<appSettings>
<add key="ConnectionString" value="Data Source=yf; user />
</appSettings>
</configuration>
**--------------------------------------------------------------------**
XmlNode node=doc.SelectSingleNode("/configuration/appSettings/add");
if(node!=null)
{
string k=node.Attributes["key"].Value;
string v=node.Attributes["value"].Value;
node=null;
}
*--------------------------------------------------------------------*
XmlNode node=doc.SelectSingleNode("/configuration/appSettings/add");
if(node!=null)
{
XmlNodeReader nr=new XmlNodeReader(node);
nr.MoveToContent();
//检查当前节点是否是内容节点。如果此节点不是内容节点，则读取器向前跳至下一个内容节点或文件结尾。
nr.MoveToAttribute("value");
string s=nr.Value;
node=null;
}

引文来源 [C#操作XML小结 - 祈离的专栏 -CSDN博客](http://blog.csdn.net/xjj51296646/archive/2009/06/04/4241548.aspx)


