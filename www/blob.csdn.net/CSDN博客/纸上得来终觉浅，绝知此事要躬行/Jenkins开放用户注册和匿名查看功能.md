# Jenkins开放用户注册和匿名查看功能 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年08月18日 10:04:25[boonya](https://me.csdn.net/boonya)阅读数：2162








Jenkins开放用户注册和匿名查看功能便于团队之间进行协作管理和查看，团队中尽量使用一样的邮件后缀，通过jenkins进行配置，每次构建和部署都可以通知到团队中的每一位成员，方便第一时间发现问题，这也符合DevOps的精神。修改Jenkins系统设置位于Configure Global Security。

## 开放用户注册


进入系统配置主面板：

![](https://img-blog.csdn.net/20170818095508753)


点击”Configure Global Security“功能进行设置：

![](https://img-blog.csdn.net/20170818095609220)

## 允许匿名用户读取访问

![](https://img-blog.csdn.net/20170818095707133)

## 测试验证设置是否生效

退出登录查看jenkins情况，请看下图：

![](https://img-blog.csdn.net/20170818095906893)



注册一个用户：
![](https://img-blog.csdn.net/20170818101306046)

可见，用户已可以进行注册和匿名访问。






