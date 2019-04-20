# JS截取字符串  - 纯洁的微笑 - CSDN博客
2011年12月28日 13:58:13[微笑很纯洁](https://me.csdn.net/ityouknow)阅读数：29
个人分类：[杂项](https://blog.csdn.net/ityouknow/article/category/8364190)
                JS截取字符串 
使用 substring()或者slice()
函数：split() 
功能：使用一个指定的分隔符把一个字符串分割存储到数组
例子：
str=”jpg|bmp|gif|ico|png”;
arr=theString.split(”|”);
//arr是一个包含字符值”jpg”、”bmp”、”gif”、”ico”和”png”的数组
函数：John() 
功能：使用您选择的分隔符将一个数组合并为一个字符串
例子：
var delimitedString=myArray.join(delimiter);
var myList=new Array(”jpg”,”bmp”,”gif”,”ico”,”png”);
var portableList=myList.join(”|”);
//结果是jpg|bmp|gif|ico|png
函数：substring() 
功能：字符串截取，比如想从”MinidxSearchEngine”中得到”Minidx”就要用到substring(0,6)
函数：indexOf()
功能：返回字符串中匹配子串的第一个字符的下标
var myString=”JavaScript”;
var w=myString.indexOf(”v”);w will be 2
var x=myString.indexOf(”S”);x will be 4
var y=myString.indexOf(”Script”);y will also be 4
var z=myString.indexOf(”key”);z will be -1            
