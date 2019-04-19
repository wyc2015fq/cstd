# Qt之QStringList运用 - 建建的博客 - CSDN博客
2017年02月24日 14:40:46[纪建](https://me.csdn.net/u013898698)阅读数：412
QStringList类提供了一个字符串列表
　　从QString继承而来，它提供快速索引为基础的接入以及快速插入和清除。
　　成员函数用于操作这个字符串列表如：
　　append(),insert(),replace(),removeAll(),removeAt(),removeFirst(),
　　removeLast(),and removeOne()等
1.增加字符串
　　可以通过append(),或使用<<,
　　如：
　　QStringList fonts;
　　fonts<<"Arial"<<"Helvetica"<<"Times"<<"Courier";
　　// fonts:[ "Arial" ,"Helvetica", "Times" , "Courier"]
2.合并字符串使用join()
　　QString str = fonts.join(",");
　　// str == "Arial,Helvetica,Times,Courier"
3.拆分字符串
　　QString str = " Arial,Helvetica, ,Times,Courier ";
　　QStringList list1 = str.split(",");
　　// list1: [ " Arial ", " Helvetica ",” “, " Times ", " Courier " ]
　　QStringList list2 = str.split(",", QString::SkipEmptyParts);
　　// list2: [ " Arial ", " Helvetica ", " Times ", " Courier " ]
4.索引
　　IndexOf()函数给定字符串的第一个出现的索引。
　　而lastIndexOf()函数，返回字符串的最后一次出现的索引。
5.替换replaceInStrings()
　　QStringList files;
　　files<<"$QTDIR/src/moc/moc.y"
　　<<"$QTDIR/src/moc/moc.l"
　　<<"$QTDIR/include/qconfig.h"
　　files.replaceInStrings("$QTDIR","/usr/lib/qt");
6.过滤filter()
　　可以让你提取一个新的列表只包含这些字符串包含一个特定的字符串（或匹配特定正则表达式）
　　QStringList list;
　　list<<"Bill Murray"<<"John Doe"<<"Bill Clinton";
　　QStringList result;
　　result= list.filer("Bill");
　　//result:["Bill Murray","Bill Clinton"]

QStringList fonts;
fonts << "Arial" << "Helvetica" << "Times" << "Courier";
QString str = fonts.join(","); 
QString str = "name1,path1;name2,path2;name3,path3"; 
QStringList list1 = str.split(";"); // 注意，如果str是空字符串，list1会增加一个空字符串到列表里，其size=1，我为此吃过苦头～
for(int i=0;i<list1.size();i++) 
{ 
QStringList list2 = list1[i].split(tr(",")); 
}
