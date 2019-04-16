# MyEclipse使用总结——MyEclipse去除网上复制下来的来代码带有的行号 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [MyEclipse使用总结——MyEclipse去除网上复制下来的来代码带有的行号](https://www.cnblogs.com/xdp-gacl/p/3544208.html)



## 一、正则表达式去除代码行号

作为开发人员，我们经常从网上复制一些代码，有些时候复制的代码前面是带有行号，如：

![](https://images0.cnblogs.com/blog/289233/201402/111327420997477.png)

MyEclipse本身自带有查找替换功能，并且支持正则表达式替换，使用正则替换就可以很容易去除这些行号

使用快捷键“ctrl+F”打开MyEclipse的查找替换功能，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201402/111328185053734.png)

这样就可以把代码中的行号全部去掉

去除代码行号的正则表达式：**^\s*\d+\.**









