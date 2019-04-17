# Matlab Attempt to execute SCRIPT*as a function解决方法 - 家家的专栏 - CSDN博客





2010年12月13日 19:20:00[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：50210标签：[matlab																[function																[编译器](https://so.csdn.net/so/search/s.do?q=编译器&t=blog)
个人分类：[数学 Matlab 基本知识](https://blog.csdn.net/yihaizhiyan/article/category/749753)





原因：

1、现有的.m文件有的与*函数重名，所以matlab编译器不知道在遇到*的时候到底该执行哪一个函数。

2、现有的变量名字，如数组名S，matlab编译器不知道在越到这个变量名或者数组名时，是执行函数还是数组。






解决方法：
把自定义的函数名或者变量、数组等，改成其他名字。](https://so.csdn.net/so/search/s.do?q=function&t=blog)](https://so.csdn.net/so/search/s.do?q=matlab&t=blog)




