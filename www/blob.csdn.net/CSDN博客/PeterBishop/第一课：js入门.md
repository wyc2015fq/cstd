# 第一课：js入门 - PeterBishop - CSDN博客





2018年12月07日 20:26:05[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：27
个人分类：[JavaScript](https://blog.csdn.net/qq_40061421/article/category/8510184)









  这节课开始我们开始学js,全称叫Javascript



  说下什么是js，js是一种脚本语言，目前市面上的浏览器能直接解析的编程语言只有js(html和css是不是编程语言至今都有争论的，说白了就是有人鄙视html和css)，当然微软家的IE还支持vbscript，不过也只有ie支持了，所以现在VBScript根本没人用，我们就学js好了。



  js是一种解释性语言，所谓解释性，就是指没有编译的过程，因为编译是为生成中间代码的，比如.java文件编译后会变成.class文件，但js没有编译一说。



  js是弱类型的，所谓弱类型是指在写代码的时候不显示声明数据类型，java是强类型的。



  基本概念说完后，说下编辑工具。



  js记事本都可以写，但是不方便，现在主流的前端代码编辑器分为两类: 文本编辑器，ide



  比较好用的文本编辑器: sublime(我在用)，vscode(很多人吹爆),vim、emacs(这俩是大神用的)

  比较好用的前端ide: webstorm(我在用),hbuilder，强调:adobe家的dreamweaver是垃圾，连文本编辑器都不如的垃圾ide



  Webstorm要钱，hbuilder免费。

  Webstorm和idea是同一家公司的产品，hbuilder国产的(这个还是很多人用的，我看过我同学用，挺不错的)



  idea支持前端，但不支持建前端项目，所以用webstorm教学。



  屁话扯了这么多，开始进入正题。



  转义字符:

![](https://img-blog.csdnimg.cn/20181207202138578.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwMDYxNDIx,size_16,color_FFFFFF,t_70)

只有这些可以直接用\转



```
//先来写个helloworld



//js代码 语句结尾可以有分号可以没有分号，看个人习惯

//输出就用document.write()函数

//js里的关键字叫function，所以还是说函数吧。。。其实函数就是java里的方法

document.write("hello world");

//说明一下，\n并没有办法在页面上换行，要换行输出<br>

document.write("<br>");



//js 里<和>属于特殊字符，要用就需要转义

document.write("<br>是换行标签<br>");

//这样好像不能转义。。

//有对应的转义特殊字符，这个可以查

//有些特殊字符的转义要用类似 &xxx; 这样的字符串来转义，所以很明显&是特殊字符

//这个html里应该有教



//定义变量语法: var 变量名

//js是弱类型，所以所有变量名不必显示声明类型，但这不代表变量没有类型

//变量的类型就是变量存的值的类型

var i = 1;

document.write(i);



//下面讲if

//if和java里的if完全一样

//switch同上

//三大循环同上



document.write("<br>");

//写个小demo

var sum= 0;

for (var a = 0; a < 100; a++)

{

    //js好像不支持+=这种写法

    sum = sum + a;

}

document.write(sum)
```





