# C#基础系列：Linq to Xml读写xml - 文章 - 伯乐在线
原文出处： [懒得安分](http://www.cnblogs.com/landeanfen/p/4636102.html)
前言：xml的操作方式有多种，但要论使用频繁程度，博主用得最多的还是Linq to xml的方式，觉得它使用起来很方便，就用那么几个方法就能完成简单xml的读写。之前做的一个项目有一个很变态的需求：C#项目调用不知道是什么语言写的一个WebService，然后添加服务引用总是失败，通过代理的方式动态调用也总是报错，最后没办法，通过发送原始的WebRequest请求直接得到对方返回的一个xml文件。注意过webservice的wsdl文件的朋友应该知道这个是系统生成的xml文件，有点复杂，研究了半天终于能正常读写了。今天在这里和大家分享下。
1、介绍之前，首先回顾下Linq to xml的方式读写简单xml的方法
（1）读取xml


```
<?xml version="1.0" encoding="utf-8"?>
<BizADsList>
  <adData aid="1" image="baidu.jpg" link="www.baidu.com" title="百度"/>
  <adData aid="2" image="qq.jpg" link="www.qq.com" title="腾讯"/>
</BizADsList>
```


C#
```
var strPath = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, @"data\test.xml");
XDocument adList = XDocument.Load(strPath);
var ad = from a in adList.Descendants("BizADsList").Elements("adData")
    select new
    {
        image = a.Attribute("image").Value,
        link = a.Attribute("link").Value,
        title = a.Attribute("title").Value
    };
    string s = "";
    foreach (var a in ad)
    s += a.image;
```
（2）写xml

C#
```
/// <summary>
        /// 根据得到的Document集合生成XML
        /// </summary>
        /// <param name="lstDocumentBD"></param>
        /// <param name="docNE"></param>
        /// <param name="strSpiderTime"></param>
        /// <param name="strNewRate"></param>
        private static void SaveXmlByLstDocument(List<Document> lstDocumentBD, Document docNE, string strSpiderTime, string strNewRate)
        {
            try
            {
                XDocument xDoc = new XDocument();
                XElement xRoot = new XElement(CSpiderConst.XML_ELE_ROOT);
                //1.构造Device节点
                XElement xDevice = new XElement(CSpiderConst.XML_ELE_DEVICE);
                //2.构造NE节点
                XElement xNE = new XElement(CSpiderConst.XML_ELE_NE);
                foreach (var oDocNE in docNE)
                {
                    XElement xItem = new XElement(CSpiderConst.XML_ELE_ITEM, new XAttribute(CSpiderConst.XML_PROP_NAME, oDocNE.Key), oDocNE.Value);
                    xNE.Add(xItem);
                }
                //这里增加一个<Item name='NewRate'>和<Item name='SpiderTimeEx'>节点用来保存当前这次的利用率和当次的采集时间
                AddNewRateAndSpiderTime(strSpiderTime, strNewRate, xNE);
                xDevice.Add(xNE);
                //3.循环构造BD节点并添加到Device节点中
                foreach (var oDocument in lstDocumentBD)
                {
                    XElement xBD = new XElement(CSpiderConst.XML_ELE_BD);
                    foreach (var oDocBD in oDocument)
                    {
                        XElement xItem = new XElement(CSpiderConst.XML_ELE_ITEM, new XAttribute(CSpiderConst.XML_PROP_NAME, oDocBD.Key), oDocBD.Value);
                        xBD.Add(xItem);
                    }
                    AddNewRateAndSpiderTime(strSpiderTime, strNewRate, xBD);
                    xDevice.Add(xBD);
                }
                xRoot.Add(xDevice);
                xDoc.Add(xRoot);
                //4.保存到采集器本地，以服务器的时间和网元的AssetID来命名
                var strDirectoryPath = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, "ReportFailed\\");
                if (!Directory.Exists(strDirectoryPath))
                {
                    Directory.CreateDirectory(strDirectoryPath);
                }
                xDoc.Save(strDirectoryPath + docNE[TBLDeviceLCBB.PROP_ASSETID] + "_" + strSpiderTime.Replace(":", "_") + ".xml");
            }
            catch
            {
                CLogService.Instance.Debug("保存XML失败");
            }
        }
```
通过XDocument、XElement对象和Element()、Elements()两个方法能完成大部分xml文件的操作。
2、进入今天的正题：读写带命名空间的xml文件。
首先来看一段xml


```
<?xml version="1.0" encoding="utf-8" ?>
<DataSet xmlns="http://WebXml.com.cn/">
  <xs:schema xmlns="" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:msdata="urn:schemas-microsoft-com:xml-msdata" id="getRegion">
    <msdata:aa>
      test
    </msdata:aa>
    <xs:element name="getRegion" msdata:IsDataSet="true" msdata:UseCurrentLocale="true">
      <xs:element name="Province">
        <xs:sequence>
          <xs:element name="RegionID" type="xs:string" minOccurs="0"/>
          <xs:element name="RegionName" type="xs:string" minOccurs="0"/>
        </xs:sequence>
      </xs:element>
    </xs:element>
  </xs:schema>
</DataSet>
```
第一次看到这个文件确实让人萌神了，比如需要取一个msdata:IsDataSet=”true”这个属性，该怎么取…
解析之前，先来分析下这段xml，<DataSet xmlns=”http://WebXml.com.cn/”>这段里面有一个xmlns属性，这个属性是每一个标签自带的属性，不信你可以新建一个xml文件，然后在任何一个标签里面输入xmlns属性，后面都会出来很多的系统自带的命名空间。这个属性表示所属标签在哪个命名空间下面，所以在取的时候要带上这个命名空间。
先来看看解析的代码：

C#
```
var strPath = Path.Combine(AppDomain.CurrentDomain.BaseDirectory, @"data\test.xml");
var oRoot = XDocument.Load(strPath);
//取DataSet标签
var oDataSet = oRoot.Element(XName.Get("DataSet", "http://WebXml.com.cn/"));
//取schema标签
var oSchema = oDataSet.Element(XName.Get("schema", "http://www.w3.org/2001/XMLSchema"));
//取element标签
var oElement = oSchema.Element(XName.Get("element", "http://www.w3.org/2001/XMLSchema"));//这两个节点都是以xs打头，所以命名空间都是xs的命名空间
//取element标签下面的IsDataSet属性
var oElementValue = oElement.Attribute(XName.Get("IsDataSet", "urn:schemas-microsoft-com:xml-msdata"));
//取aa标签
var oAA = oSchema.Element(XName.Get("aa", "urn:schemas-microsoft-com:xml-msdata"));
```
我们来解析下几个关键的地方：
（1）我们来解析下


```
<xs:schema xmlns="" xmlns:xs="http://www.w3.org/2001/XMLSchema" xmlns:msdata="urn:schemas-microsoft-com:xml-msdata" id="getRegion">
```
这一句，最前面的”xs”表示标签所属命名空间的变量，xmlns:xs=”http://www.w3.org/2001/XMLSchema”这个表示xs这个命名空间的值。所以要得到schema这个标签需要带上命名空间var oSchema = oDataSet.Element(XName.Get(“schema”, “http://www.w3.org/2001/XMLSchema”));这个标签还定义了另一个命名空间xmlns:msdata=”urn:schemas-microsoft-com:xml-msdata”。
（2）再来看看aa标签


```
<msdata:aa>
      test
 </msdata:aa>
```
msdata就是上面schema标签里面定义的另一个命名空间，表示aa标签属于msdata命名空间下面。
（3）再看来看属性的取法：

C#
```
<xs:element name="getRegion" msdata:IsDataSet="true" msdata:UseCurrentLocale="true">
```
如果要取msdata:IsDataSet=”true”，由于这个属性也带有命名空间，所以取属性时也要加上命名空间了。所以需要这样取。
var oElementValue = oElement.Attribute(XName.Get(“IsDataSet”, “urn:schemas-microsoft-com:xml-msdata”));
现在大伙们应该对这种xml有一个更加清晰的认识了吧。其实一般情况下这种场景比较少见，因为这么复杂的xml一般是由引用服务时代理对象去解析的。但如果真的有这么变态的需求我们也不用担心了。在此记录下，以后如果大家遇到希望能节约大伙的时间。
### 本系列：
- [C#基础系列：再也不用担心面试官问我”事件”了](http://blog.jobbole.com/98200/)
- [C#基础系列：扩展方法的使用](http://blog.jobbole.com/98230/)
- [C#基础系列：反射笔记](http://blog.jobbole.com/98266/)
- [C#基础系列：Attribute特性使用](http://blog.jobbole.com/98348/)
- [C#基础系列：小话泛型](http://blog.jobbole.com/98359/)
- [C#基础系列：多线程的常见用法详解](http://blog.jobbole.com/98370/)
- [C#基础系列：委托实现简单设计模式（1）](http://blog.jobbole.com/98378/)
- [C#基础系列：委托和设计模式（2）](http://blog.jobbole.com/98388/)
- [C#基础系列：序列化效率比拼](http://blog.jobbole.com/98397/)
- [C#基础系列：异步编程初探async和await](http://blog.jobbole.com/98405/)
