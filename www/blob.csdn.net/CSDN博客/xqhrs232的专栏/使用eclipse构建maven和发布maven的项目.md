# 使用eclipse构建maven和发布maven的项目 - xqhrs232的专栏 - CSDN博客
2018年11月30日 14:21:51[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：28
个人分类：[JavaWeb技术](https://blog.csdn.net/xqhrs232/article/category/8231541)
原文地址::[https://www.cnblogs.com/huanglinbin/p/6610839.html](https://www.cnblogs.com/huanglinbin/p/6610839.html)
相关文章
1、[eclipse中安装maven的插件](https://www.cnblogs.com/huanglinbin/p/6610771.html)----[https://www.cnblogs.com/huanglinbin/p/6610771.html](https://www.cnblogs.com/huanglinbin/p/6610771.html)
2、[window下安装和部署maven](https://www.cnblogs.com/huanglinbin/p/6610295.html)----[https://www.cnblogs.com/huanglinbin/p/6610295.html](https://www.cnblogs.com/huanglinbin/p/6610295.html)
对于maven的部署和安装插件不熟的同学可以看一下上两篇文章maven的部署和安装：
此笔记已经集成了maven的插件。
# 一、构建Maven项目
- 用eclipse构建maven项目之前我们需要为eclipse做一些必要的配置
- 
## 一些必要的配置
　　1、点击eclipse菜单栏的Window下的preferences，选择Maven菜单
　　2、选择Installations，选择Add，添加你本地安装的maven安装目录，如下所示。
![](https://images2015.cnblogs.com/blog/1021174/201703/1021174-20170324120218986-1950405402.png)
3、点击Apply应用，然后选择左侧菜单的User Settings，为maven配置全局设置。点击Browse选择你安装maven目录下的conf子目录下的settings.xml文件，其他选项不用管，直接点击Apply应用就可以了。
![](https://images2015.cnblogs.com/blog/1021174/201703/1021174-20170324120307393-595088443.png)
4、然后点击坐车Maven根菜单，进行Maven的自定义配置，是否需要下载源码，是否下载javadoc文件，根据需要勾选应用就可以了。
![](https://images2015.cnblogs.com/blog/1021174/201703/1021174-20170324120330065-31550813.png)
配置完毕！接下来就可以进行maven项目的构建了。
- 
## 开始构建Maven项目
1.在eclipse右键新建一个Maven Project　　
![](https://images2015.cnblogs.com/blog/1021174/201703/1021174-20170324120547580-1864725343.png)
直接点击next下一步：采用默认勾选的就可以![](https://images2015.cnblogs.com/blog/1021174/201703/1021174-20170324120637518-1511110904.png)
2、Filter过滤器选择quickstart，点击Next进行下一步
（这里也可以输入web关键字过滤条件，选择web进行创建项目，他两却别在于创建后的目录结构不一样。需要自己补全目录结构。）
![](https://images2015.cnblogs.com/blog/1021174/201703/1021174-20170324120850596-1967114448.png)
next下一步：
3、在Group Id 填写我们的项目包名，按照规范我们选择域名倒写。Artifact Id 填写我们的项目名称，然后点击Finish完成创建。
![](https://images2015.cnblogs.com/blog/1021174/201703/1021174-20170324120932658-443010860.png)
4、好了，这Maven项目就创建好了。以下是它的目录结构：![](https://images2015.cnblogs.com/blog/1021174/201703/1021174-20170324120957002-919941056.png)
# 二、发布Maven项目
 1、选中项目，然后选择运行按钮下的 Run Configratis...
![](https://images2015.cnblogs.com/blog/1021174/201703/1021174-20170324121039549-1877194444.png)
 2、进入该配置界面后，在Maver Build 上鼠标右键New，然后修改Name名称，点击Browse Workspace选择该项目，然后填写Goals(目标)为package，点击Apply应用就OK了。
![](https://images2015.cnblogs.com/blog/1021174/201703/1021174-20170324121137252-719608814.png)
 3、点击Apply后，看控制台如果出现BUILD SUCCESS就说明发布成功了！
![](https://images2015.cnblogs.com/blog/1021174/201703/1021174-20170324121206565-1874483199.png)
 4、在项目上鼠标右键刷新项目，然后就可以看到新生成的目录结构及jar文件
![](https://images2015.cnblogs.com/blog/1021174/201703/1021174-20170324121229815-1593380563.png)
以上就是我的maven的构建，有问题的请给我留言，一起探讨一起学习。
