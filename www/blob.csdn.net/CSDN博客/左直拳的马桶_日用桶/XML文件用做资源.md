# XML文件用做资源 - 左直拳的马桶_日用桶 - CSDN博客
2008年10月24日 11:55:00[左直拳](https://me.csdn.net/leftfist)阅读数：1110标签：[xml																[assembly																[string																[exe																[dll](https://so.csdn.net/so/search/s.do?q=dll&t=blog)](https://so.csdn.net/so/search/s.do?q=exe&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=assembly&t=blog)](https://so.csdn.net/so/search/s.do?q=xml&t=blog)
个人分类：[.NET																[XML](https://blog.csdn.net/leftfist/article/category/94499)](https://blog.csdn.net/leftfist/article/category/94497)
XML文件用做资源
左直拳
<?xml:namespace prefix = o ns = "urn:schemas-microsoft-com:office:office" />
很简单，在VS里，选该XML，属性，“生成操作”改为“嵌入的资源”，编译后，它就变成dll或EXE的一部分了。
运行时再读取出来：
假设我有个XML架构文件 myXsd.xsd，嵌入到资源里，怎么检索出来呢？
Assembly ass = Assembly.LoadFrom(System.AppDomain.CurrentDomain.BaseDirectory + "//Interface.dll");//反射
string xsd = "InterfaceNameSpace.myXsd.xsd";//注意这里的格式：命名空间.资源名称
System.IO.Stream filXsd = ass.GetManifestResourceStream(xsd);//读取出来了

