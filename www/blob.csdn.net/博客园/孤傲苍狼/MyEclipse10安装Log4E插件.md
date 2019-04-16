# MyEclipse10安装Log4E插件 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [MyEclipse10安装Log4E插件](https://www.cnblogs.com/xdp-gacl/p/4231812.html)



## 一、 Log4E插件下载

　　下载地址：[http://log4e.jayefem.de/content/view/3/2/](http://log4e.jayefem.de/content/view/3/2/)

![](https://images0.cnblogs.com/blog/289233/201501/181508170896568.png)

## 二、安装Log4E插件

　　将下载下来的压缩包解压缩，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201501/181513100269230.png)

　　解压缩生成的【de.jayefem.log4e_free_1.5.0】文件夹里面有如下的两个文件夹

![](https://images0.cnblogs.com/blog/289233/201501/181510257146964.png)

　　将【de.jayefem.log4e_free_1.5.0】文件夹直接复制到MyEclipse10的【dropins】目录，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201501/181517268701964.png)

　　这样就完成了Log4E插件的安装了。

## 三、在MyEclipse中使用Log4E

　　Log4E插件主要是配合Log4j来使用的，例如帮助我们生成这样的代码：

```
1 　　/**
2      * Logger for this class
3      */
4     private static final Logger logger = Logger
5             .getLogger(UserServiceImpl.class);
```

![](https://images0.cnblogs.com/blog/289233/201501/181527494486398.gif)









