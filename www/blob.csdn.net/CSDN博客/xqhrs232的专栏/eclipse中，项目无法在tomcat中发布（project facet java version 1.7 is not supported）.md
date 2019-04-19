# eclipse中，项目无法在tomcat中发布（project facet java version 1.7 is not supported） - xqhrs232的专栏 - CSDN博客
2018年11月05日 14:40:09[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：67
个人分类：[JavaWeb技术](https://blog.csdn.net/xqhrs232/article/category/8231541)
原文地址::[https://www.cnblogs.com/linjinfeng/p/9047335.html](https://www.cnblogs.com/linjinfeng/p/9047335.html)
在tomcat中发布项目时无法添加项目，错误信息：project facet java version 1.7 is not supported，如下图
![](https://images2018.cnblogs.com/blog/1400725/201805/1400725-20180516175946126-627057590.png)
这是由于你的tomcat的jdk版本低于你项目的jdk版本导致的。
解决方案有2个：修改你的tomcat的jdk版本或者降低你的项目的jdk版本
1、修改tomcat的jdk版本
在eclipse中，点击Window->preferences->server->Runtime Environment
选中你的Tomcat，点击Edit，如下图
![](https://images2018.cnblogs.com/blog/1400725/201805/1400725-20180516180035296-1128927322.png)
修改你的JRE版本，点击Finish。
![](https://images2018.cnblogs.com/blog/1400725/201805/1400725-20180516180309643-1028642504.png)
2、降低项目的jdk版本
右键项目->properties->project facets。修改java选项的版本即可，如下图
![](https://images2018.cnblogs.com/blog/1400725/201805/1400725-20180516180156419-1756739379.png)
//=======================================================================================
//备注::
1>我的是通过降低项目的jdk版本才成功的！！！
