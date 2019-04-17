# 如何kill掉一个screen - 家家的专栏 - CSDN博客





2015年02月22日 16:08:08[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：30138
个人分类：[unix](https://blog.csdn.net/yihaizhiyan/article/category/2542449)









两种方式：




1、使用screen名字，kill掉。

      screen -S session_name -X quit




2、激活screen：

     screen -r session_name

     并利用exit退出并kiil掉session。



