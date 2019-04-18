# 介绍两个 XML 文件操作类 - 深之JohnChen的专栏 - CSDN博客

2006年03月29日 14:48:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：1636标签：[xml																[string																[分布式应用																[null																[tree																[数据结构](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)](https://so.csdn.net/so/search/s.do?q=tree&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=分布式应用&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=xml&t=blog)
个人分类：[数据存储](https://blog.csdn.net/byxdaz/article/category/8249840)


**介绍两个 XML 文件操作类**

可扩展标记语言XML(Extend Mark Language)是一种描述数据和数据结构的语言，XML数据可以保存在任何可以存储文本的文档中。HTML用来描述外观，而XML从一开始就被设计为表示原始信息而完全忽略数据呈现方式。XML之所以功能强大，是因为计算机行业已经把XML为数据交换的标准，并提供了相当数量的支持工具。

　　窃认为，XML最激动人心的应用是可以在分布式应用中作为消息传递的封装格式(SOAP消息)，最常用的是作为应用程序的设置选项保存。

　　本文实现了一个CXMLFile类来操作XML文件，对应用程序设置选项的保存作了特别的接口支持，并实现了一个CPersistentTreeCtrl(一个可保存标签项的树控件)来演示CXMLFile类的用法。

　　C++的XML解析器，最著名的恐怕要数开放源码的Xerces-C++和Xml4C了。

　　Xerces-C++是Apache团队的开发成果，它严格遵循DOM与SAX规范，并且提供了良好的易用性和跨平台特性，并保证很高的执行效率，一直是国外很多项目的首选XML解析器。但是它对中文兼容性不好，在处理含中文字符XML内容时，常常出现错误，这对我国的C++程序员来说，是不可饶恕的致命缺陷。

　　Xml4C是IBM在Xerces-C++的基础上开发的XML解析器。它与Xerces-C++有着完全相同的调用接口，并且它支持更多的文字编码。Xml4C所支持的文字编码比Xerces-C++多10倍甚至20倍。但因为Xml4C是　　在Xerces-C++基础上开发的，所以它对中文XML文档内容的兼容性也有同样的问题。

　　邹月明先生在developerWorks 中国网站:XML:所有的文章:"剖析Xml4C源码，完美兼容中文XML"中提出了一个简易的方案，解决了中文字符兼容性的部分问题：使Xml4C可以支持中文节点值，但仍然不支持节点名称或属性值为中文。（这是去年的试验结果，不知道现在有没有完全的解决方案了，估计要重写XMLString类和字符串函数，还有相关的众多东东了，作者考虑了一下，没有动手试着改，万一改好了，敌人又出新版本了，那就划不来了，呵呵）。我们发现MSXML SDK就非常完美地支持中文，并且程序接口和Xml4C、Xerces-C++差不多，在一般的应用程序中，就开始使用MSXML SDK。CXMLFile类就是用来保存程序设置值的，通常只使用与XML解释器无关的成员函数。

```
//得到节点值(长整型)
long GetLong(const char* cstrBaseKeyName, const char* cstrValueName, long lDefaultValue);
//设置节点值(长整型)
long SetLong(const char* cstrBaseKeyName, const char* cstrValueName, long lValue);

//得到节点值(字符串)
std::string GetString(const char* cstrBaseKeyName, 
                      const char* cstrValueName, 
                      const char* cstrDefaultValue);
//设置节点值(字符串)
long SetString(const char* cstrBaseKeyName, const char* cstrValueName, const char* cstrValue);

//得到节点属性
std::string GetAttribute(const char* cstrBaseKeyName, const char* cstrValueName,
	                const char* cstrAttributeName, const char* cstrDefaultAttributeValue);
//设置节点属性
long SetAttribute(const char* cstrBaseKeyName, const char* cstrValueName,
		const char* cstrAttributeName, const char* cstrAttributeValue);

//得到节点值
long GetNodeValue(const char* cstrBaseKeyName, const char* cstrValueName, 
	const char* cstrDefaultValue, std::string& strValue, const char* cstrAttributeName, 
	const char* cstrDefaultAttributeValue,std::string& strAttributeValue);

//设置节点值
long SetNodeValue(const char* cstrBaseKeyName, const char* cstrValueName, 
	const char* cstrValue=NULL, const char* cstrAttributeName=NULL,
	const char* cstrAttributeValue=NULL);

//删除某节点和其所有子节点
long DeleteSetting(const char* cstrBaseKeyName, const char* cstrValueName);

//得到某节点的子节点的键值
long GetKeysValue(const char* cstrBaseKeyName, 
	std::map<std::string, std::string>& keys_val);

//得到某节点的子节点的键名
long GetKeys(const char* cstrBaseKeyName, 
	std::vector<std::string>& keys);

//保存XML文件
bool save(const char* filename=NULL);
//装载XML文件
bool load(const char* filename, const char* root_name="xmlRoot");

//不保存改变
void DiscardChanges();
//清空内容
void clear();
```

一个简单的写入、保存例子：

```
CXMLFile xmlFile;
xmlFile.load("file.xml ");
xmlFile.SetLong("Student/情况", "年龄", 24);
xmlFile.SetString("Student /个况", "籍贯", "浙江嵊州");
xmlFile.SetString("Student /个况", "性别", "男");
xmlFile.save();
```

一个简单的读取例子：

```
CXMLFile xmlFile;
xmlFile.load("file.xml ");
long age= xmlFile.GetLong("Student/个况", "年龄", 25;
std::string strHomeplace =xmlFile.GetString ("Student /个况", "籍贯", "浙江嵊州");
std::string strSex =xmlFile. GetString ("Student /个况", "性别", "男");
```

CPersistentTreeCtrl类主要有两个成员函数：

```
//load from a XML file
bool Load(const char* filename, const char* tree_name="TreeName", bool bImage=false);
//save to a XML file
bool Save(const char* filename, const char* tree_name="TreeName", bool bImage=false);
```

　　分别用于保存和装入树的节点，存取的效率可能不高，权作演示吧。

注：Win2K自带MSXML3.0，WinXP自带MSXML4.0，在目标计算机上应当保证安装过MSXML。

[下载源代码](http://www.vckbase.com/code/downcode.asp?id=2526)


