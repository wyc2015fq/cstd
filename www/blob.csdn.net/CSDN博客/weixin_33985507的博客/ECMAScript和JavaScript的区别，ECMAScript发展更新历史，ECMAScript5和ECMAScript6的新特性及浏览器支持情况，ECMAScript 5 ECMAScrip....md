# ECMAScript和JavaScript的区别，ECMAScript发展更新历史，ECMAScript5和ECMAScript6的新特性及浏览器支持情况，ECMAScript 5/ECMAScrip... - weixin_33985507的博客 - CSDN博客
2014年11月04日 11:01:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6

**ECMAScript和JavaScript的区别**
ECMA是European Computer Manufacturers Association的缩写，即欧洲计算机制造商协会。欧洲计算机制造商协会是制定信息传输与通讯的国际化标准组织。
ECMAScript是ECMA制定的标准化脚本语言。
目前JavaScript使用的ECMAScript版本为ECMAScript-262。
ECMAScript被国际化标准组织采纳为ISO/IEC 16262。
**ECMAScript发展更新历史总结：**
ecma script 简写是 es
1995 js诞生
1997 es 1.0
1998 es 2.0
1999 es 3.0
ecma script 5 2008草稿，2009年发布es 5，es 6=ecma script 2015（Ecma国际意在更频繁地发布包含小规模增量更新的新版本，下一版本将于2016年发布，命名为ECMAScript 2016。从现在开始，新版本将按照ECMAScript+年份的形式发布）
2015 es 6
ie9下支持es 5一部分特性
ie10以上和其他浏览器支持es 5,移动端完全支持es 5
es 6现没有完全支持的浏览器和服务端，预计一年左右2017年会出现
以大中型企业互联网技术的普及发展速度，我估计要4年左右即2019年，浏览器完全支持企业也采用的成熟时代估计是2016年
以上参考：
[ECMAScript5和ECMAScript6的新特性及浏览器支持情况](http://www.jianshu.com/p/b6d76160889d)
[ECMAScript 2015正式发布](http://www.infoq.com/cn/news/2015/06/ecmascript-2015-es6)
**JavaScript的一些基础语法**
创建脚本块
<script language=”JavaScript”>
      JavaScript code goes here
</script>
隐藏脚本代码
<script language=”JavaScript”>
<!--
      document.write(“Hello”);
// -->
</script>
在不支持JavaScript的浏览器中将不执行相关代码
浏览器不支持的时候显示
<noscript>
    Hello to the non-JavaScript browser.
</noscript>
链接外部脚本文件
<script language=”JavaScript” src="/”filename.js"”></script>
注释脚本
// This is a comment
document.write(“Hello”); // This is a comment
/*
      All of this  is a comment
*/
输出到浏览器
document.write(“<strong>Hello</strong>”);
定义变量
var myVariable = “some value”;
字符串相加
var myString = “String1” + “String2”;
字符串搜索
<script language=”JavaScript”>
<!--
     var myVariable = “Hello there”;
     var therePlace = myVariable.search(“there”);
    document.write(therePlace);
// -->
</script> 
