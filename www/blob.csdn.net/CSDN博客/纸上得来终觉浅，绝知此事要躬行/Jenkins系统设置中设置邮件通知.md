# Jenkins系统设置中设置邮件通知 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年08月17日 16:51:17[boonya](https://me.csdn.net/boonya)阅读数：1726








Jenkins邮件通知的密码是客户端的授权码而不是邮箱的密码，请务必搞清楚。设置邮件需要修改三个部分：1、修该邮箱的SMTP授权开启并获取客户端授权码；2、Jenkins系统设置设置管理员邮箱；3、设置邮件通知。完成以上三步就可以实现Jenkins邮件通知了。下面以163邮箱为例进行讲解。




## SMTP授权码

开启SMTP服务：

![](https://img-blog.csdn.net/20170817164835752)


设置或者重置客户端授权码：


![](https://img-blog.csdn.net/20170817164848715)


## Jenkins管理员邮箱

![](https://img-blog.csdn.net/20170817165002640)





## 邮件通知设置

![](https://img-blog.csdn.net/20170817165014322)

测试发送邮件成功！！！

## 测试构建发送邮件


这里故意将代码改出问题，push代码到github或者svn代码库。

![](https://img-blog.csdn.net/20170817171215763)

通过Jenkins查看构建错误的#9

![](https://img-blog.csdn.net/20170817171332266)

可见每次构建都发送了邮件，成功还是失败管理员都可以第一时间知道。




