# error   LNK2001:   unresolved   external   symbol   _DrawDibOpen@0  - 家家的专栏 - CSDN博客





2010年08月25日 19:25:00[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：1331
个人分类：[c++中常见错误解析](https://blog.csdn.net/yihaizhiyan/article/category/716601)









VC++错误：

               error   LNK2001:   unresolved   external   symbol   [_DrawDibOpen@0](mailto:_DrawDibOpen@0)


解决方法：

              Project-> Settings->Link页，在libs文字框加上Vfw32.lib 



