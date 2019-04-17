# Junit Unrooted Tests 分析 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2015年05月20日 12:46:48[boonya](https://me.csdn.net/boonya)阅读数：1026标签：[Java																[junit](https://so.csdn.net/so/search/s.do?q=junit&t=blog)](https://so.csdn.net/so/search/s.do?q=Java&t=blog)
个人分类：[WorkPractice](https://blog.csdn.net/boonya/article/category/1796195)








如下图所示：

![](https://img-blog.csdn.net/20150520124136887?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYm9vbnlh/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

#### 问题原因：

此问题发生的原因是因为Junit测试方法不正确：未添加任何注解或使用注解有问题，或选择测试的方法非@Test注解的方法。

#### **问题解决：**

Junit需要你对你测试的方法**添加@Test注解**，另外其他的注解是不能直接选择其Run as为Junit的。检查你的方法是否缺失注解。





