# Tinyxml简单使用法 - L_Andy的专栏 - CSDN博客

2015年04月10日 17:57:38[卡哥](https://me.csdn.net/L_Andy)阅读数：811
个人分类：[VC/MFC编程](https://blog.csdn.net/L_Andy/article/category/1099539)



1、概要介绍一下：

尝试了tinyxml,rapidxml,libxml2等3中第3方库,前两者相对较为轻量级，libxml2非常完善,强大。

这是网上总结出来的优缺点：

1、tinyxml当节点内容的空而使用GetText()方法时候会出错

2、rapidxml编码问题不是特别好

3、libxml2对内存释放要求较高，否则时常遇到core dump.

TinyXML是一个开源的解析XML的解析库，能够用于C++，能够在Windows或Linux中编译。这个解析库的模型通过解析XML文件，然后在内存中生成DOM模型，从而让我们很方便的遍历这棵XML树。

     DOM模型即文档对象模型，是将整个文档分成多个元素（如书、章、节、段等），并利用树型结构表示这些元素之间的顺序关系以及嵌套包含关系。

先介绍一下在TinyXML中，根据XML的各种元素来定义了一些类：

        TiXmlBase：整个TinyXML模型的基类。

        TiXmlAttribute：对应于XML中的元素的属性。

        TiXmlNode：对应于DOM结构中的节点。

        TiXmlComment：对应于XML中的注释。

        TiXmlDeclaration：对应于XML中的申明部分，即<？versiong="1.0" ?>。

        TiXmlDocument：对应于XML的整个文档。

        TiXmlElement：对应于XML的元素。

        TiXmlText：对应于XML的文字部分。
TiXmlUnknown：对应于XML的未知部分。 
TiXmlHandler：定义了针对XML的一些操作。

tinyxml类图:

![tinyxml类图](http://www.2009fly.com/index/uploads/allimg/110301/163142Gc-0.png)

那么这些元素是如何对应的呢? 例如

<?xml version="1.0" encoding="utf-8"?>

<!– Our to do list data –>

<ToDo>

<Item priority="1"> Go to the <bold>Toy store!</bold></Item>

<Item priority="2"> Do bills</Item>

</ToDo> 

整个对象树： 

TiXmlDocument "demo.xml"

TiXmlDeclaration version="1.0" encoding="utf-8"?>

TiXmlComment " Our to do list data"

TiXmlElement "ToDo"

TiXmlElement "Item" Attribtutes: priority = 1

TiXmlText "Go to the "

TiXmlElement "bold"

TiXmlText "Toy store!"

TiXmlElement "Item" Attributes: priority=2

TiXmlText "Do bills" 

在tinyXML中,用FirstChild（"名字"）查找节点时,调用FirstChild函数的节点与要查找的节点必须成“父子关系”。

2、实现使用元素

如下是一个XML片段：

<Persons>

    <Person ID="1">

        <name>周星驰</name>

        <age>40</age>

    </Person>

    <Person ID="2">

        <name>白晶晶</name>

        <age>30</age>

    </Person>

</Persons>

一、读取XML

//创建一个XML的文档对象。

TiXmlDocument *myDocument = new TiXmlDocument("填上你的Xml文件名");

myDocument->LoadFile();

//获得根元素，即Persons。

TiXmlElement *RootElement = myDocument.RootElement();

//输出根元素名称，即输出Persons

cout << RootElement->Value() << endl;

//获得第一个Person节点。

TiXmlElement *FirstPerson = RootElement->FirstChildElement();

//获得第一个Person的name节点和age节点和ID属性。

TiXmlElement *NameElement = FirstPerson->FirstChildElement();

TiXmlElement *AgeElement = NameElement->NextSiblingElement();

TiXmlAttribute *IDAttribute = FirstPerson->FirstAttribute();

//输出第一个Person的name内容，即周星驰；age内容，即40；ID属性，即1。

cout << NameElement->FirstChild()->Value << endl;

cout << AgeElement->FirstChild()->Value << endl;

cout << IDAttribute->Value() << endl;

二、生成XML内容

//创建一个XML的文档对象。

TiXmlDocument *myDocument = new TiXmlDocument();

//创建一个根元素并连接。

TiXmlElement *RootElement = new TiXmlElement("Persons");

myDocument->LinkEndChild(RootElement);

//创建一个Person元素并连接。

TiXmlElement *PersonElement = new TiXmlElement("Person");

RootElement->LinkEndChild(PersonElement);

//设置Person元素的属性。

PersonElement->SetAttribute("ID", "1");

//创建name元素、age元素并连接。

TiXmlElement *NameElement = new TiXmlElement("name");

TiXmlElement *AgeElement = new TiXmlElement("age");

PersonElement->LinkEndChild(NameElement);

PersonElement->LinkEndChild(AgeElement);

//设置name元素和age元素的内容并连接。

TiXmlText *NameContent = new TiXmlText("周星驰");

TiXmlText *AgeContent = new TiXmlText("40");

NameElement->LinkEndChild(NameContent);

AgeElement->LinkEndChild(AgeContent);

//保存到文件

myDocument->SaveFile("要保存的xml文件名");

这样，便创建了一个如下的xml文件：

<Persons>

    <Person ID="1">

        <name>周星驰</name>

        <age>40</age>

    </Person>

</Persons>

3、举例说明一下常见用法,以我当前做的项目取代码片段说明一下：

1、解析如下xml内容

<?xml version="1.0" encoding="utf-8"?>

<server ip = "" port = "" name = "" version = ""/>

核心解析代码：

for (element = document.FirstChildElement(); element; element
= document.NextSiblingElement()) {
while (sAttr != NULL) {
name = sAttr->Name();
value = sAttr->Value();
if (xmlstr_compare("ip", name)) {
if ('0' <= value[0] && value[0] <= '9') {
bc.ip = xml_inet_addr(value);
} else {
bc.ip = IP_INADDR_ANY;
}
} else if (xmlstr_compare("port", name)) {
bc.port = atoi(value);
} else if (xmlstr_compare("name", name)) {
memcpy((void*) bc.name, value, 16);
} else if (xmlstr_compare("version", name)) {
memcpy((void*) bc.version, value, 16);
}

sAttr = sAttr->Next();
}

}

2、如下的内容解析

<?xml version="1.0" encoding="utf-8"?>

<request id="" type="addeventhub">
<item name="touchscreen" multed = "16"  width="" height="" dpi="" componentid="1"/>
<item name="mouse" width="" height="" dpi="" componentid="2"/>
<item name="keyboard" componentid="3" />
<item name="dpad" componentid="4"/>
<item name="gamepad" componentid="5"/>
<item name="trackball" componentid="6"/>

</request>

核心解析代码：
while (sAttr != NULL) {
name = sAttr->Name();
value = sAttr->Value();
...
sAttr = sAttr->Next();
}

itemelement = element->FirstChildElement();
while (itemelement != NULL) {
int itemNo = -1;
name  = itemelement->GetText();
value = itemelement->Value();

itemAttr = itemelement->FirstAttribute();
while (itemAttr != NULL) {
name = itemAttr->Name();
value = itemAttr->Value();

...

itemAttr = itemAttr->Next();
} 
}

3、如何拼包

有如下xml buff内容，如何进行拼包呢？

<?xml version="1.0" encoding="utf-8"?>

<server ip = "" port = "" name = "" version = ""/>

xml_udp_bc *xbc = (xml_udp_bc*)xml_param->param;

if(xml_param->head_flag == 1){

p += sprintf(p, xml_head);

}

p += sprintf(p, "<server ip = \" ");

p += sprintf(p, xml_iptostr(xbc->ip));

p += sprintf(p, "\"");

p += sprintf(p, " port = \"%d\"",xbc->port);

p += sprintf(p, " name = \"%s\"",xbc->name);

p += sprintf(p, " version = \"%s\"",xbc->version);

p += sprintf(p, "/>");

对于拼包的话可以使用xml，但具体项目中没有使用这么复杂的东东，所以直接以字符串拼接好了

ok,简单的使用了一下非常简单!


