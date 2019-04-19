# 代码里将中文插入oracle 乱码问题 - 左直拳的马桶_日用桶 - CSDN博客
2018年05月07日 18:50:26[左直拳](https://me.csdn.net/leftfist)阅读数：275
代码中，插入到oracle数据库，中文变乱码。用pl/sql developer，却没事。
找来找去，又说调字符集，又说啥的，都不起作用。索性将心一横：我这个代码是直接执行的SQL语句，而oracle的中文字符集为GBK，而我们的程序字符集为UTF-8，那么将SQL语句在执行前，先转换成GBK如何？
```
string pathgbk = Utf8ToGBK(folder.PATH);//含中文的字符串转换成GBK
                string sql = "select work.SEQ_FOLDER.nextval from dual";
                int id = db.ExecuteScalar(sql);
                sql = $@"INSERT INTO work.folder (id,category,f1,f2,f3,f4,f5,f6,f7,f8,f9,f10,path,datadate,updatedate,status) 
VALUES ({id},{folder.CATEGORY},
{folder.F1},{folder.F2},{folder.F3},{folder.F4},{folder.F5},{folder.F6},{folder.F7},{folder.F8},{folder.F9},{folder.F10},
'{pathgbk}',to_date('{sDataDate}','yyyy-mm-dd HH24:MI:SS'),to_date('{sUpdateDate}','yyyy-mm-dd HH24:MI:SS'),{folder.STATUS})";
                db.ExecuteSQLCmd(sql);
string Utf8ToGBK(string utf8String)
{
    Encoding fromEncoding = Encoding.UTF8;
    Encoding toEncoding = Encoding.GetEncoding("gbk");
    return EncodingConvert(utf8String, fromEncoding, toEncoding);
}
string EncodingConvert(string fromString, Encoding fromEncoding, Encoding toEncoding)
{
    byte[] fromBytes = fromEncoding.GetBytes(fromString);
    byte[] toBytes = Encoding.Convert(fromEncoding, toEncoding, fromBytes);
    string toString = toEncoding.GetString(toBytes);
    return toString;
}
```
问题解决。
之前没发现有这个问题，程序是UTF-8，ORACLE是GBK，一直相安无事，大概是客户端已经做了自动转换。但上次导入了数据库备份后就出现了问题，估计是导出数据库那台机的字符集是有问题的。
