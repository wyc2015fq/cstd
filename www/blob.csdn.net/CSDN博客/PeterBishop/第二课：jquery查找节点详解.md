# 第二课：jquery查找节点详解 - PeterBishop - CSDN博客





2018年12月15日 13:44:21[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：43
个人分类：[JavaScript](https://blog.csdn.net/qq_40061421/article/category/8510184)









  这节课我们重点学习jquery查找元素


document.write("<script src='jquery1.9.js'></script>");



*/** * DOM**树的元素结点就是各个标签 */*/*

jQuery 元素选择器

jQuery 使用 CSS 选择器来选取 HTML 元素。



$("p") 选取 <p> 元素。



$("p.intro") 选取所有 class="intro" 的 <p> 元素。



$("p#demo") 选取所有 id="demo" 的 <p> 元素。





jQuery 属性选择器

jQuery 使用 XPath 表达式来选择带有给定属性的元素。



$("[href]") 选取所有带有 href 属性的元素。



$("[href='#']") 选取所有带有 href 值等于 "#" 的元素。



$("[href!='#']") 选取所有带有 href 值不等于 "#" 的元素。



$("[href$='.jpg']") 选取所有 href 值以 ".jpg" 结尾的元素。



 */



//选择name属性为abc的a标签

**var **$a = $("a[name='abc']");



/*

* 这些 jQuery 方法很有用，它们用于向上遍历 DOM 树：



parent()  只有这个常用

parents()

parentsUntil()

*/



/*

jQuery children() 方法

children() 方法返回被选元素的所有直接子元素。



该方法只会向下一级对 DOM 树进行遍历。



jQuery find() 方法

find() 方法返回被选元素的后代元素，一路向下直到最后一个后代。

* */



/*

有许多有用的方法让我们在 DOM 树进行水平遍历：



siblings()

next()   只有这个常用，就是找本元素节点的下一个元素

nextAll()

nextUntil()

prev()

prevAll()

prevUntil()

 */



/*

jquery过滤只有first方法常用，剩下的直接用数组下标搞定

first方法是取元素结点数组的第一个，相当于[0]

 */


Jquery01.js里的一点小错误
//1.2 根据name,返回的是数组

//这个写错了

//根据name实际上相当于根据属性，在下一颗

//$("a")还是根还有TagName

**var **$a = $("a");




