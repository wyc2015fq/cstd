
# sgen.exe 已退出，代码为1 - 追求卓越,做到专业 - CSDN博客


2018年12月07日 16:21:34[Waldenz](https://me.csdn.net/enter89)阅读数：228


web项目或者控制台应用程序，发布的时候会报错 “sgen.exe” 已退出，代码为1.
![](https://img-blog.csdnimg.cn/20181207161941415.png)
![](https://img-blog.csdnimg.cn/20181207152139848.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)
将项目的生成中，在Release配置下，将 生成序列化程序集设置为 开或者自动，即可。
![](https://img-blog.csdnimg.cn/20181207152255797.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2VudGVyODk=,size_16,color_FFFFFF,t_70)

