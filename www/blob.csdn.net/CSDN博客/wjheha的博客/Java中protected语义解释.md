# Java中protected语义解释 - wjheha的博客 - CSDN博客
2017年10月28日 10:46:04[wjheha](https://me.csdn.net/wjheha)阅读数：108标签：[java](https://so.csdn.net/so/search/s.do?q=java&t=blog)
个人分类：[Java/Java Web](https://blog.csdn.net/wjheha/article/category/6775424)
### protected的确切语义是：protected修饰的方法或变量将会被任何位置的子类继承，但是永远只能被最早定义他的父类所在的包的类所见（除了该类以及其子类能看到本身的该protected方法或变量之外。）
原文链接：[http://blog.csdn.net/hongyuan19/article/details/1946636](http://blog.csdn.net/hongyuan19/article/details/1946636)
