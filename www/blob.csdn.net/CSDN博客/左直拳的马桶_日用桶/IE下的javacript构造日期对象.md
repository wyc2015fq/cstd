# IE下的javacript构造日期对象 - 左直拳的马桶_日用桶 - CSDN博客
2018年09月18日 16:44:13[左直拳](https://me.csdn.net/leftfist)阅读数：90
个人分类：[JavaScript](https://blog.csdn.net/leftfist/article/category/94500)
javascript中，可以采用 `var date = new Date(日期字符串)` 的方式来构造日期对象。问题是，IE下有些奇怪，字符串的格式必须是“yyyy/MM/dd HH:mm”这种方式，“yyyy-MM-dd HH:mm”是不行的。
比如说，`var date = new Date("2018-09-18 16:42");`，在IE下，这个date是无效日期（Invalid date）。
chrome没问题。
