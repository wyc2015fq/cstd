# the different between call and apply in javascript - Arthur的随笔 - CSDN博客
2011年08月23日 13:12:57[largetalk](https://me.csdn.net/largetalk)阅读数：488标签：[javascript																[function																[object](https://so.csdn.net/so/search/s.do?q=object&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=javascript&t=blog)
个人分类：[杂七杂八](https://blog.csdn.net/largetalk/article/category/823511)
```java
function sayColor(spre, ssuf) {alert(spre + this.color + ssuf);};
var obj = new Object();
obj.color = 'red';
sayColor.call(obj, "this is ", " , a very nice"); # this is red a very nice
sayColor( "this is ", " , a very nice"); # this is undefined a very nice
sayColor.apply(obj, new Array("this is ", " a nice very")); #this is red a nice very
```
