# Mysql全局变量查询及设置 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年06月13日 17:34:28[boonya](https://me.csdn.net/boonya)阅读数：8983








在开发和测试mysql的性能过程中我们需要对mysql进行适当的优化处理。最直接的方式就是修改全局变量以适应实际的需求。

## 查看全局变量

命令：




```
SHOW GLOBAL VARIABLES;
```

![](https://img-blog.csdn.net/20170613172942873)

## 设置全局变量

命令：


`SET GLOBAL max_allowed_packet=53687091200;`
![](https://img-blog.csdn.net/20170613173241014)










