# Javascript函数返回多个值 - 左直拳的马桶_日用桶 - CSDN博客
2008年04月09日 12:44:00[左直拳](https://me.csdn.net/leftfist)阅读数：11119标签：[javascript																[function																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=javascript&t=blog)
个人分类：[JavaScript](https://blog.csdn.net/leftfist/article/category/94500)
Javascript函数返回多个值
左直拳
我从来就没有真正掌握过Javascript，又或许是Javascript不断地在发展，有好多东西看起来常常令我觉得十分惊奇。
比如说，在网上看到一条Javascript语句：
Return [a,b,c,d];
这是返回语句，可是它有对方括号，里面多个值。
什么意思？难道是返回多个值？那么，在调用方，又如何提取这些返回值？
在网上寻寻觅觅，看了些介绍 Return 语句的文章，发觉都是说些很基本的东西，并且几乎全部是同一篇文章。还有的斩钉截铁地声称，Javascript绝无可能在函数里返回多个值。
不过我有所怀疑，在代码里添加了这条Return [a,b,c,d]; 语句，发觉没有错误提示，于是
var re = (function f(){return [0,1,2,3];})();
然后 alert(re[0]);
显示 0,说明提取成功。
不是我不明白，是这个世界变化太快。
