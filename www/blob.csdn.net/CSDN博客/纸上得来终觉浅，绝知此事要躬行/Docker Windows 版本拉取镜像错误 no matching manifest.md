# Docker Windows 版本拉取镜像错误 no matching manifest - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月09日 21:27:28[boonya](https://me.csdn.net/boonya)阅读数：421标签：[docker																[images																[pull](https://so.csdn.net/so/search/s.do?q=pull&t=blog)
个人分类：[Docker](https://blog.csdn.net/boonya/article/category/6559498)





windows 10 x64安装版本需要开启Hyper-V

![](https://img-blog.csdnimg.cn/20181209212531338.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

Docker镜像拉取错误码：

```
C:\Users\Administrator>docker pull mysql:5.7
5.7: Pulling from library/mysql
no matching manifest for unknown in the manifest list entries
```

解决方法：

![](https://img-blog.csdnimg.cn/20181209212627751.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

 此后便可以拉取了：

![](https://img-blog.csdnimg.cn/20181209213217798.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)](https://so.csdn.net/so/search/s.do?q=images&t=blog)](https://so.csdn.net/so/search/s.do?q=docker&t=blog)




