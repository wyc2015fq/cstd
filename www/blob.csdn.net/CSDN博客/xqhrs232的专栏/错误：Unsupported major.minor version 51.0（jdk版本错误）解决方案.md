# 错误：Unsupported major.minor version 51.0（jdk版本错误）解决方案 - xqhrs232的专栏 - CSDN博客
2018年11月02日 14:20:16[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：43
原文地址::[https://www.cnblogs.com/snowcity999/p/5226719.html](https://www.cnblogs.com/snowcity999/p/5226719.html)
在Win7+MyEclipse2014环境下，部署可执行项目源文件，需要根据开发开发时使用的JDK版本重新引入jar包：
　　方法：①在对应项目上右键选择Build Path——>Configure Bulid Path...，点击Libraries，查找并替换带红叉的jar包（通过Add External JARs...）；
　　        ②查看JRE System Library[jdk……]版本是否正确，通过Add Library...替换正确JDK版本的jar包。
发布程序，运行后出现ERROR：Unsupported major.minor version 51.0（为JDK版本错误），原因是软件开发环境和运行环境的JDK版本不同造成的，修改方法：在项目上右键选择Properties——>Java Compiler，勾选最上面Enable project specific settings后，选择Compiler compliance level版本——>完成。
![](https://img-blog.csdnimg.cn/20181102144529886.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3hxaHJzMjMy,size_16,color_FFFFFF,t_70)
