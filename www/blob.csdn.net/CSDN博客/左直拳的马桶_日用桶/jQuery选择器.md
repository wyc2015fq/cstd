# jQuery选择器 - 左直拳的马桶_日用桶 - CSDN博客
2018年07月22日 10:14:11[左直拳](https://me.csdn.net/leftfist)阅读数：100
个人分类：[JavaScript](https://blog.csdn.net/leftfist/article/category/94500)
直接上干货：
0、选中的checkbox
```java
$(":checked")
//等同于
$("input:checked")
```
1、不被选中的checkbox
`$("input:not(:checked)")`
2、多条件组合 
1）属性l=4并且value!=0
`$('input[l=4][value!=0]')`
2）属性l=4或者value!=0
`$('input[l=4],[value!=0]')`
3)属性l=4并且value!=0并且不被选中的checkbox
`$("input:not(:checked)[l=4][value!=0]");`
参考文章：
[jQuery 参考手册 - 选择器](http://www.w3school.com.cn/jquery/jquery_ref_selectors.asp)
[Jquery属性选择器（同时匹配多个条件,与或非）（附样例）](http://www.cnblogs.com/fanbi/p/7705235.html)
