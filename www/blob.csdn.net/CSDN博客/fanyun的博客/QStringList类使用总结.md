# QStringList类使用总结 - fanyun的博客 - CSDN博客
2017年01月25日 20:54:52[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：2892
QStringList类提供了一个字符串列表
从QList <QString>继承而来，它提供快速索引为基础的接入以及快速插入和清除
其成员函数用于操作这个字符串列表如
append()，insert(), replace(),removeAll(), removeAt(), removeFirst(), removeLast(), and removeOne()等
1.   增加字符串
   可以通过append()，或使用<<，如
```cpp
QStringList fonts;
   fonts <<"Arial" << "Helvetica" << "Times"<< "Courier";
           //fonts:[ "Arial" ,"Helvetica", "Times" ,"Courier"]
```
2.    合并字符串使用join()
```cpp
QString str = fonts.join(",");
          //str == "Arial,Helvetica,Times,Courier"
```
3.   拆分字符串
```cpp
QStringstr = " Arial,Helvetica, ,Times,Courier ";
        QStringListlist1 = str.split(",");
        //list1: [ " Arial ", " Helvetica ",” “, " Times "," Courier " ]
       QStringListlist2 = str.split(",", QString::SkipEmptyParts);
       //list2: [ " Arial ", " Helvetica ", " Times "," Courier " ]
```
     也就是说如果有QString：：SkipEmptyParts，空项不会出现在结果。默认情况下，空项被保留
4.   索引
       IndexOf（）函数返回给定字符串的第一个出现的索引。
       而lastIndexOf（）函数，返回字符串的最后一次出现的索引。
5.    替换replaceInStrings（）
```cpp
QStringList files;
        files<< "$QTDIR/src/moc/moc.y"
                 <<"$QTDIR/src/moc/moc.l"
                  <<"$QTDIR/include/qconfig.h";
            files.replaceInStrings("$QTDIR","/usr/lib/qt");
                  //files: [ "/usr/lib/qt/src/moc/moc.y", ...]
```
6.     过滤filter（）
可以让你提取一个新的列表只包含这些字符串包含一个特定的字符串（或匹配特定正则表达式）：
```cpp
QStringListlist;
           list<< "Bill Murray" << "John Doe" <<"Bill Clinton";
   QStringList result;
          result= list.filter("Bill");
          //result: ["Bill Murray", "Bill Clinton"]
```
