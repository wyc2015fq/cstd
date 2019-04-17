# Jenkins 集成和使用JIRA Bug跟踪管理工具 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年09月07日 19:01:58[boonya](https://me.csdn.net/boonya)阅读数：9693








首先解答一个疑问：是Jira集成Jenkins还是Jenkins集成Jira？Jenkins可以 集成Jira ，Jira也可以集成Jenkins。这两个工具都提供了相关的插件实现。本篇文章就来介绍下Jenkins和Jira互相集成需要做哪些配置，记住插件是少不了的，并且要保证Jira和Jenkins之间能够正常访问。

## Jenkins Jira插件安装

这里Jenkins主要安装的有：
- Jira Issue Updater
- JIRA plugin
- JIRA Trigger Plugin


![](https://img-blog.csdn.net/20170907184026074)





注：上面JIRA Plugin for Jenkins插件是Jira需要的，需要下载手动上传。下载地址： [https://repository.marvelution.com/content/repositories/releases/com/marvelution/jira/plugins/jenkins-jira-plugin/](https://repository.marvelution.com/content/repositories/releases/com/marvelution/jira/plugins/jenkins-jira-plugin/)

![](https://img-blog.csdn.net/20170907184419874)

注：尽量下载比较靠后的版本。


## Jenkins 系统配置Jira配置

![](https://img-blog.csdn.net/20170907184904008)

注：这里的配置都是根据插件需要而配置的，有些是没有必要配置的。


## Jenkins Project配置构建后Jira操作

![](https://img-blog.csdn.net/20170907185151098)


注：还有其它的JIRA配置项，下来再仔细研究下。


## Jira 安装Jenkins Integration for JIRA插件

![](https://img-blog.csdn.net/20170907185501046)





## Jira配置applications和Jenkins websites

添加应用：

![](https://img-blog.csdn.net/20170907185602896)


添加Jenkins：


![](https://img-blog.csdn.net/20170907185530682)

添加后：


![](https://img-blog.csdn.net/20170907185642506)


主要这里报了一个异常警告：

![](https://img-blog.csdn.net/20170907185932973)

这里就用到了前面所说的JIRA Plugin for Jenkins插件，安装上传后就没有了，勾选前面的checkbox框就可以操作了：

![](https://img-blog.csdn.net/20170907190109024)

## Jenkins构建部署错误测试


此测试为验证jenkins构建部署失败时能否自动往Jira上新增一条问题记录。构建过程中发现Jenkins for Jira插件还存在一定的问题，报如下错误：

![](https://img-blog.csdn.net/20170907203730487)


此问题在Jenkins网站问题中有记录：[https://issues.jenkins-ci.org/browse/JENKINS-38293](https://issues.jenkins-ci.org/browse/JENKINS-38293)可以参考。修改Jenkins 构建后Jira的配置：

![](https://img-blog.csdn.net/20170907211531396)


报错问题解决，但是仍然不明白Jira上面为何没有创建一个新的issue，很有可能少了点配置。

## Jira BUG状态改变触发Jenkins构建

原理：Jira利用webhooks连接Jenkins，Jenkins通过jira-trigger-plugin获取触发构建事件。

### jira-trigger-plugin说明配置

首先我们来看看jira-trigger-plugin,GItbub:[https://github.com/jenkinsci/jira-trigger-plugin](https://github.com/jenkinsci/jira-trigger-plugin)我们从中大致可以知道有4个步骤：

**第一步：了解Jira触发器插件的特性**


![](https://img-blog.csdn.net/20170908100620167)


**第二步：配置Jira webhooks**


![](https://img-blog.csdn.net/20170908100733132)


**第三步：Jenkins全局配置之JIRA触发器配置**

![](https://img-blog.csdn.net/20170908100813098)


**第四步：配置Jenkins Job Project构建触发器**


![](https://img-blog.csdn.net/20170908100938647)


### jira-trigger-plugin实例配置

下面我们就配置一个issue Updated的触发构建webhook.

**第一步：在jira>System>Advance中找到webhooks 并创建**

![](https://img-blog.csdn.net/20170908101503875)


创建后的视图如下：

![](https://img-blog.csdn.net/20170908101532887)

**第二步：配置Jenkins全局Jira触发器**

![](https://img-blog.csdn.net/20170908101705837)

**第三步：修改Jenkins Job project的构建触发器**

![](https://img-blog.csdn.net/20170908101846996)

此处配置的BUG状态由In Progress变为Done的都触发Jenkins构建。

**第四步：修改BUG状态验证是否触发构建**

以上操作保存后，修改任意bug状态由In Progress变为Done，Jenkins发起了新的构建任务，如下控制台输出：

![](https://img-blog.csdn.net/20170908102214276)

关于修改的BUG触发记录：

![](https://img-blog.csdn.net/20170908103152044)




