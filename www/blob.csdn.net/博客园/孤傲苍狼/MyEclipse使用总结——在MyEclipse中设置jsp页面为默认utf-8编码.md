# MyEclipse使用总结——在MyEclipse中设置jsp页面为默认utf-8编码 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [MyEclipse使用总结——在MyEclipse中设置jsp页面为默认utf-8编码](https://www.cnblogs.com/xdp-gacl/p/3496161.html)



在MyEclispe中创建Jsp页面，Jsp页面的默认编码是“ISO-8859-1”，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201312/29102435-d5f1bec5d7b14022b4e32927e97268a1.png)

在这种编码下编写中文是没有办法保存Jsp页面的，会出现如下的错误提示：

![](https://images0.cnblogs.com/blog/289233/201312/29102528-fe262ee7f2c1459e8a4bbf8e84a41ba6.png)

因此可以设置Jsp默认的编码为utf-8，具体步骤如下：

启动MyEclipse，点击菜单上的window--->preferences

![](https://images0.cnblogs.com/blog/289233/201312/29102623-48d002cde470418d8254406f840763e3.png)

在弹出的对话框中点击MyEclise--->Files and Editors--->JSP，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201312/29102744-1cc195927b9143f087a675e6c3214e3d.png)

此时就可以看到JSP编码设置面板，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201312/29103012-16e44eeaf0f74c589fd3b993f99d64e8.png)

在Encoding那个下拉框选择UTF-8编码,，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201312/29103044-eee3744d629f4231ab8e0be583cc7463.png)

这样JSP默认的编码就设置成UTF-8了，以后新建页面，页面的编码默认就是采用UTF-8了。![](https://images0.cnblogs.com/blog/289233/201312/29103217-93551d5e91a4452582126712e78b21c2.png)









