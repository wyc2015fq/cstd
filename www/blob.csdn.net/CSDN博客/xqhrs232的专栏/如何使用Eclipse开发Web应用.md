# 如何使用Eclipse开发Web应用 - xqhrs232的专栏 - CSDN博客
2018年09月30日 09:29:44[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：59
个人分类：[JavaWeb技术](https://blog.csdn.net/xqhrs232/article/category/8231541)
原文地址::[https://blog.csdn.net/active2489595970/article/details/68957495](https://blog.csdn.net/active2489595970/article/details/68957495)
1：创建项目
（1）：启动Eclipse，进入到Eclipse的开发界面
![](https://img-blog.csdn.net/20170403105333028?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWN0aXZlMjQ4OTU5NTk3MA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
（2）：点击create a new java EE web project
新建的项目名称：first,在Dynamic web module version下拉列表框中选择3.0
![](https://img-blog.csdn.net/20170403105421419?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWN0aXZlMjQ4OTU5NTk3MA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
(3):点击“Next”按钮，打开如图配置java应用的对话框，这里采用默认
![](https://img-blog.csdn.net/20170403105453561?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWN0aXZlMjQ4OTU5NTk3MA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
（4）：点击next，打开Configure web module settings.对话框，将Content directory文本框中的值修改为WebContent
![](https://img-blog.csdn.net/20170403105522435?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWN0aXZlMjQ4OTU5NTk3MA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
(5):
单击"finish"按钮，此时在Eclipse平台的左侧项目资源管理器中，显示项目first
![](https://img-blog.csdn.net/20170403105607232?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWN0aXZlMjQ4OTU5NTk3MA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
2:创建jsp文件
在Eclipse的项目资源管理中，选中first节点下的WebContent节点，并单击鼠标右键，选new/jsp File文件，在对话框File name输入文件名：index.jsp
![](https://img-blog.csdn.net/20170403112406788?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWN0aXZlMjQ4OTU5NTk3MA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
点击“Next"，打开选择JSP模板的对话框，这里采用默认设置
![](https://img-blog.csdn.net/20170403112716090?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWN0aXZlMjQ4OTU5NTk3MA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
单击"Fihsh"按钮，完成JSP文件的创建
此时在项目资源管理器中，自动添加index.jsp的节点，点击它，同时将index.jsp文件中的默认代码改为以下代码：
![](https://img-blog.csdn.net/20170403145401659?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWN0aXZlMjQ4OTU5NTk3MA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
将编辑好的JSP页面保存，就完成了一个简单的JSP程序的创建
3：如何在Eclipse:配置服务器与新项目进行运行
这里以配置Tomcat 6.0为例子，打开Eclipse
![](https://img-blog.csdn.net/20170403121206259?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWN0aXZlMjQ4OTU5NTk3MA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
选择Server下面的Runtime Environments：
![](https://img-blog.csdn.net/20170403121321426?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWN0aXZlMjQ4OTU5NTk3MA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
点击右边的“Add”按钮，进“New Server Runtime Environment”对话框，选择我们要配置的Tomcat版本，这里选用了Tomcat v6.0，注意要与你下载的版本一致。
![](https://img-blog.csdn.net/20170403121509935?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWN0aXZlMjQ4OTU5NTk3MA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
Next后进入下图:
![](https://img-blog.csdn.net/20170403121607061?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWN0aXZlMjQ4OTU5NTk3MA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
在Tomcat installation directory右边的Browse按钮，点击它，进入本地磁盘目录，选择你的Tomcat安装目录，如下：
![](https://img-blog.csdn.net/20170403121733407?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWN0aXZlMjQ4OTU5NTk3MA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
确定后，回到刚才那个对话框，点击JRE右边的Installed JREs按钮，进入下图的界面：
![](https://img-blog.csdn.net/20170403121901334?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWN0aXZlMjQ4OTU5NTk3MA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
如果这里已经配置了JDK,则直接选择即行，如果没有配置的话，就点右边的“Add”,添加一个标准的JRE
![](https://img-blog.csdn.net/20170403122127398?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWN0aXZlMjQ4OTU5NTk3MA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
Next,下图，点右边的Directgry浏览本地文件系统，找到JDK的安装目录。
![](https://img-blog.csdn.net/20170403122328884?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWN0aXZlMjQ4OTU5NTk3MA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
好，确定 – finish，下一步，完成。如下图：
![](https://img-blog.csdn.net/20170403122547291?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWN0aXZlMjQ4OTU5NTk3MA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
项目的新建、添加jar包、运行；一般我们用这个工具来做Web项目，就建一个动态Web工程吧：
![](https://img-blog.csdn.net/20170403123649000?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWN0aXZlMjQ4OTU5NTk3MA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
进入下图界面，在Project name中输入工程名，这里起名为“HelloWorld”，在Target runtime中选择刚才我们配置的Tomcat 9.0服务器。如下图：
![](https://img-blog.csdn.net/20170403123821031?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWN0aXZlMjQ4OTU5NTk3MA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
关于如何把外部的jar包引进我们的项目中来，这个其实很简单，复制那些jar文件，在下图的lib目录下右键粘贴即行。
![](https://img-blog.csdn.net/20170403125839779?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWN0aXZlMjQ4OTU5NTk3MA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
关于如何运行Web项目，Eclipse for JavaEE与MyEclipse的做法不同
![](https://img-blog.csdn.net/20170403151049477?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWN0aXZlMjQ4OTU5NTk3MA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
进入下图，找到我们配置的服务器：
![](https://img-blog.csdn.net/20170403125134788?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWN0aXZlMjQ4OTU5NTk3MA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
Next – > finish即行![](https://img-blog.csdn.net/20170403132031005?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWN0aXZlMjQ4OTU5NTk3MA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
如何发布项目到Tomcat并运行
在项目资源管理器中选择项目名称节点，右击选择Run As/Run on Server,将打开Run on Server对话框，在该对话框选中Always use this server when running this project,单击Finish,即可通过Tomcat运行该项目，如图：
![](https://img-blog.csdn.net/20170403152456467?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWN0aXZlMjQ4OTU5NTk3MA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
--------------------- 本文来自 active2489595970 的CSDN 博客 ，全文地址请点击：https://blog.csdn.net/active2489595970/article/details/68957495?utm_source=copy
