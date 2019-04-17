# Firefox上网问题:管理员没有正确配置网站 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年06月13日 13:22:35[boonya](https://me.csdn.net/boonya)阅读数：4392








**网页访问异常：**

www.baidu.com 的管理员没有正确配置好此网站。为避免您的信息失窃,Firefox 并未....

![](https://img-blog.csdn.net/20180613131838827)


**解决方法：**

参考文章：[http://tieba.baidu.com/p/4830309617?traceid=](http://tieba.baidu.com/p/4830309617?traceid=)


考虑是系统根证书被导入了一个劫持证书，Firefox 49 版本开始添加了一个参数，如果遇到未知的 CA 证书，浏览器可以直接对正在使用的 Windows 系统证书存储机制进行检查对比。

要激活此功能步骤如下：

在地址栏键入"about:config" 点击“我了解此风险”在下方任意位置右键，选择新建布尔值

![](https://img-blog.csdn.net/20180613132023263)

输入首选项名称为“security.enterprise_roots.enabled”并把值设置为 true


重启浏览器，HTTPS 网站即可正常访问。






