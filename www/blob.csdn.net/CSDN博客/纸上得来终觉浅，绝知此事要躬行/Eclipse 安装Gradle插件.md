# Eclipse 安装Gradle插件 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年11月24日 18:04:32[boonya](https://me.csdn.net/boonya)阅读数：4619








前言说明：Gradle也是一种通过依赖配置减少jar下载的方式，类似于Maven的原理。

环境要求：环境jdk一定要是1.7以上，否则插件安装会失败。



项目地址：[https://github.com/eclipse/buildship](https://github.com/eclipse/buildship)


# 1.离线安装方式




第一步：下载Gradle>[http://gradle.org/gradle-download](http://gradle.org/gradle-download/)

第二步：解压gradle-2.5，

配置环境变量：GRADLE_HOME

![](https://img-blog.csdn.net/20150730003250905)


path添加;%GRADLE_HOME%\bin


![](https://img-blog.csdn.net/20150730003424510)




第三步，查看是否安装成功：

以管理员身份运行"命令提示符"，输入：gradle -v
![](https://img-blog.csdn.net/20150730003437614)

第四步：打开eclipse>Help>Install new software>输入http://dist.springsource.com/release/TOOLS/gradle

![](https://img-blog.csdn.net/20150730004257595)



next的过程中，如下图步骤的时候一定要选择第三项，并且全部勾上，然后一直next；

![](http://blog.csdn.net/zxz547388910/article/details/51555330)


![](https://img-blog.csdn.net/20160601140313449)


安装成功后提示重启eclipse；



第五步：eclipse安装gradle插件后，导入gradle项目报错，右键项目-->Gradle-->Refresh All就OK了。

# 2.Eclipse在线更新方式



#### (1)在线安装地址指南：



[https://github.com/eclipse/buildship/blob/master/docs/user/Installation.md](https://github.com/eclipse/buildship/blob/master/docs/user/Installation.md)

找到对应的eclipse版本拷贝链接。

#### (2)打开eclipse，依次按以下步骤操作

![](https://img-blog.csdn.net/20160821163001167?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





![](https://img-blog.csdn.net/20160821163006604?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





![](https://img-blog.csdn.net/20160821163011214?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




![](https://img-blog.csdn.net/20160821163016542?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
# 3.Eclipse Marketplace方式

eclipse中依次打开“Help”–》“Eclipse Marketplace”,在搜索栏中输入“buildship”进行搜索； 
![这里写图片描述](https://img-blog.csdn.net/20151209000207483)

2.点击install，等待安装完成，按照提示重启eclipse即可。

3.安装成功后，可以在Window–>Show View–>Others…中和File–>New–> Other…中看到增加的Gradle选项。







