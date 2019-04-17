# 工善配置：Intellij IDEA Maven dependency自动补全 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月13日 16:24:27[boonya](https://me.csdn.net/boonya)阅读数：125标签：[maven																[dependency](https://so.csdn.net/so/search/s.do?q=dependency&t=blog)](https://so.csdn.net/so/search/s.do?q=maven&t=blog)
个人分类：[编程效率](https://blog.csdn.net/boonya/article/category/8058247)








原文地址：[https://blog.csdn.net/w605283073/article/details/80065566](https://blog.csdn.net/w605283073/article/details/80065566)

在settings ->Build ->Build Tools-->Maven -->Repositories

![](https://img-blog.csdnimg.cn/2018121316225530.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

没有更新过的Updated对应的值为Never，点击右侧的Update按钮

![](https://img-blog.csdnimg.cn/20181213162002789.png)

等待更新完成（时间较长，需要耐心等待，也可以使用代理），完成后会Processing indices（处理索引）

![](https://img-blog.csdnimg.cn/20181213162014880.png)

然后更新本地仓库

![](https://img-blog.csdnimg.cn/20181213162033365.png)

等更新以后，核对一下是否更新成功：

![](https://img-blog.csdnimg.cn/20181213162047459.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

这里应该有对应的更新日期，如果这一行显示的是粉红色，可以看到对应的原因，如：

![](https://img-blog.csdnimg.cn/20181213162104720.png)

那么需要在setting.xml中或者IDEA中设置代理

Maven依赖就可以自动提示补全了

![](https://img-blog.csdnimg.cn/20181213161905197.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

如果还没有添加依赖也可以使用IDEA来搜索和添加maven依赖了。

![](https://img-blog.csdnimg.cn/20181213162126153.png)

![](https://img-blog.csdnimg.cn/20181213162204983.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)




