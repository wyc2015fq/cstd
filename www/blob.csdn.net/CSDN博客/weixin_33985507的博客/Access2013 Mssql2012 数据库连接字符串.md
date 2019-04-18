# Access2013 Mssql2012 数据库连接字符串 - weixin_33985507的博客 - CSDN博客
2014年12月24日 09:08:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
直接在调用的地方设置
```
```
// access 2003
DBConStr = "Provider=Microsoft.Jet.OLEDB.4.0; Data Source=" + AppPath + "sample.mdb";
// access 2007
DBConStr = "Provider=Microsoft.ACE.OLEDB.12.0; Data Source=" + AppDomain.CurrentDomain.BaseDirectory + "Sample.accdb";
Driver ={ Microsoft Access Driver(*.mdb, *.accdb)}; DBQ = path to mdb/ accdb file”
// mssql 
DBConStr = "Provider = SQLOLEDB.1;Data Source = 192.168.3.251;Initial Catalog = Sample;User ID = sa; Password = 1";
```
```
