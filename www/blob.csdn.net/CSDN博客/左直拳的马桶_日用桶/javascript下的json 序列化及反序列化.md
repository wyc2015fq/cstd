# javascript下的json 序列化及反序列化 - 左直拳的马桶_日用桶 - CSDN博客
2015年01月13日 22:53:03[左直拳](https://me.csdn.net/leftfist)阅读数：1609
个人分类：[JavaScript																[json](https://blog.csdn.net/leftfist/article/category/2860257)](https://blog.csdn.net/leftfist/article/category/94500)
1、序列化
将json对象转为字符串：
JSON.stringify(jsonObj)
2、反序列化
var jsonObj = eval("(" + jsonstring + ")");
