# 构建XML的架构文件XSD - 左直拳的马桶_日用桶 - CSDN博客
2008年10月24日 12:08:00[左直拳](https://me.csdn.net/leftfist)阅读数：1043标签：[xml																[dataset																[.net																[工具](https://so.csdn.net/so/search/s.do?q=工具&t=blog)](https://so.csdn.net/so/search/s.do?q=.net&t=blog)](https://so.csdn.net/so/search/s.do?q=dataset&t=blog)](https://so.csdn.net/so/search/s.do?q=xml&t=blog)
个人分类：[.NET](https://blog.csdn.net/leftfist/article/category/94497)
构建XML的架构文件XSD
左直拳
<?xml:namespace prefix = o ns = "urn:schemas-microsoft-com:office:office" />
我尝试先写好XML，然后再根据XML去写XSD。但XSD的标记实在复杂，看上去好像天书。写好之后，用XSD去匹配XML，结果没有读出正确的数据。
后来发现有个工具可以根据XML推导出XSD，它就叫做xsd.exe，藏在.NET所在的文件夹里，搜索一下就出来了。
然后运行 xsd.exe myXml.xml，就可以在同一目录下生成myXml.xsd。
如果要生成别的名字或放在什么路径，可以 xsd.exe/? 看看帮助。
XmlDataDocument doc = newXmlDataDocument();
doc.DataSet.ReadXmlSchema(filXml);
doc.Load(filXml);
DataSet ds = doc.DataSet;
