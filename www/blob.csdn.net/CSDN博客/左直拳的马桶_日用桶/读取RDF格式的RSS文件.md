# 读取RDF格式的RSS文件 - 左直拳的马桶_日用桶 - CSDN博客
2007年02月01日 16:25:00[左直拳](https://me.csdn.net/leftfist)阅读数：3983
读取RDF格式的RSS文件
左直拳
RSS目前据说有3种格式：0.9X，1.0，2.0。用得最多的是2.0。对于RSS2.0，没有什么难的，在C#里，采用
XmlDocument xml = newXmlDocument();
xml.LoadXml(xmlFile);
XmlNodeList nodes = xml.SelectNodes("/rss/channel/item");
就可以轻松获得里面的每一个节点。
但对于RDF格式（也许就是RSS1.0吧？RSS的定义很混乱，搞不清楚）就不行了。比如，现在有一个RSS，格式如下：
<?xml version="1.0" encoding="utf-8"?>
<?xml-stylesheet title="XSL_formatting"type="text/xsl" href="/china/rss.xsl"?>
<**rdf:RDF**xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#" xmlns:dc="http://purl.org/dc/elements/1.1/" xmlns:sy="http://purl.org/rss/1.0/modules/syndication/" xmlns:cc="http://web.resource.org/cc/" xmlns="http://purl.org/rss/1.0/">
……
<**item** rdf:about="http://www.bbc.co.uk/go/wsy/pub/rss/1.0/-/china/learningenglish/takeawayenglish/tae/2007/01/070131_music_in_schools.shtml">
<**title** xml:lang="zh-Hans">Music in Schools 校园音乐课程</title>
<**link**>http://www.bbc.co.uk/go/wsy/pub/rss/1.0/-/china/learningenglish/takeawayenglish/tae/2007/01/070131_music_in_schools.shtml</link>
<description xml:lang="zh-Hans">Can you play a musical instrument? Learn all about British school music lessons in Take Away English. </description>
<**dc:date**>2007-01-31T17:30:57+00:00</dc:date>
<dc:format>text/plain</dc:format>
<dc:language>zh-Hans</dc:language>
<dc:publisher>http://www.bbcchina..com.cn</dc:publisher>
<dc:creator>http://www.bbc.co.uk/china</dc:creator>
<dc:type>Tae, Takeaway English</dc:type>
</item>
<item>……</item>
<item>……</item>
……
</rdf:RDF>
对于这种格式，表面上看，也是XML嘛，可直接用上面的方式去读取就是不行，一个节点也拿不到。
开始以为是什么XPath写得不对，一会儿xml.SelectNodes("//RDF/item")不行，一会儿xml.SelectNodes("//rdf:RDF/item")也不行。
后来才知道，要指定命名空间。
留意一下RSS文件中的代码
<**rdf:RDF**xmlns:rdf="http://www.w3.org/1999/02/22-rdf-syntax-ns#" xmlns:dc="http://purl.org/dc/elements/1.1/" xmlns:sy="http://purl.org/rss/1.0/modules/syndication/" xmlns:cc="http://web.resource.org/cc/" xmlns="http://purl.org/rss/1.0/">
两个红色部分分别表示两个不同的命名空间。其中第二个（xmlns="http://purl.org/rss/1.0/"）没有前缀，说明它是默认命名空间，则其所有子孙节点没有指定前缀的，都使用该命名空间。比如，<item>就属于默认命名空间，<dc:date>就属于命名空间dc。
明白了这个道理，就可以写C#代码如下：
XmlNamespaceManager nsMgr = newXmlNamespaceManager(xml.NameTable);
nsMgr.AddNamespace("rdf", "http://www.w3.org/1999/02/22-rdf-syntax-ns#");
nsMgr.AddNamespace("rss", "http://purl.org/rss/1.0/"); 
XmlNodeList nodes = xml.SelectNodes("**/rdf:RDF/rss:item**", nsMgr);
这样就可以获得节点<item>的集合了。
同理，要获得<Item>里面的子节点也应该要指定命名空间：
string link = nodes[i].SelectSingleNode("rss:link",nsMgr).InnerText
