# C#下把txt文件数据读进sql server中存储所遇到的乱码问题 - TaigaComplex求职中 - 博客园







# [C#下把txt文件数据读进sql server中存储所遇到的乱码问题](https://www.cnblogs.com/TaigaCon/archive/2012/07/15/2592763.html)





txt文件的默认编码为ANSI，C#的默认读取数据便秘为UTF-8

这样的话就会造成文本数据格式的显示乱码问题

解决问题的方式有两种

一是改变txt文件的编码，改为UTF-8

另一种是在读入txt文件时采用ANSI编码，也就是系统的默认编码

这样在写入数据库的时候就不会再有问题

代码如下：

```
1 using (FileStream fileStream = File.OpenRead(ofdImport.FileName))   {              //打开文件,using实现IDisposable（析构自动化）

2 using (StreamReader streamReader = new StreamReader(fileStream,**Encoding.Default**))                                      //以系统默认编码加载数据流
```












