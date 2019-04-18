# Js 实现 replaceAll 方法 - z69183787的专栏 - CSDN博客
2012年11月02日 16:27:06[OkidoGreen](https://me.csdn.net/z69183787)阅读数：539
js 代码
String.prototype.replaceAll = function(s1,s2){    
return this.replace(new RegExp(s1,"gm"),s2);    
}
方法： string.replace(new RegExp(oldString,"gm"),newString))
gm     g=global, m=multiLine ， 大致上方法就是这样的，可以实现替换全部指定字串
sample：
str.replaceAll("1","")
