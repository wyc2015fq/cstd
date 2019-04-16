# 编译运行Red5源代码 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月09日 23:52:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：40
个人分类：[RTMP](https://blog.csdn.net/leixiaohua1020/article/category/8432890)










**1.安装Eclipse3.3.2**
 a.[**点这里从官方网站下载**](http://www.eclipse.org/downloads/download.php?file=/eclipse/downloads/drops/R-3.3.2-200802211800/eclipse-SDK-3.3.2-win32.zip&url=http://download.actuatechina.com/eclipse/eclipse/downloads/drops/R-3.3.2-200802211800/eclipse-SDK-3.3.2-win32.zip&mirror_id=385)
b.解压缩到D:\red5
 c.进入D:\red5\eclipse，双击eclipse.exe
 d.选择工程存放目录，设置在D:\red5\code，勾选"Use this as the default and do not ask again"。

**2.安装Subclipse(Eclipse上的SVN插件)**
 a.[**点这里从官方网站下载**](http://subclipse.tigris.org/files/documents/906/43484/site-1.4.4.zip)
 b.选择Eclipse主菜单中Help->Software Updates->Find and Install...
 c.选择Search for new features toinstall,单击next，进入插件安装界面
 d.单击New Archived Site...打开文件选取界面，找到刚才下载的site-1.4.4.zip双击。弹出Edit Local Site界面，单击OK
 e.回到c画面，单击Finish
 f.进入Updates界面，勾选"JavaHL Adapter(recommended)"，"Subclipse(required)"，单击next


![](https://img-blog.csdn.net/20131009235010687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



g.进入Install界面，选中"I accept the terms in the license agreements"，单击next
 h.单击Finish，弹出新界面单击Install All，再弹出新界面单击Yes，Eclipse重新启动，此时Subclipse安装完毕。

**3.从red5的svn上取出源代码**
 a.选择Eclipse主菜单中Window->Open Perspective->Other，弹出"Open Perspective"界面，选择"SVN资源库研究"，单击OK

![](https://img-blog.csdn.net/20131009235015187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

b.在Eclipse左侧，单击右键弹出菜单，选择"新建"->"资源库位置..."
 c.进入"添加SVN资源库"界面，在URL中填入：http://red5.googlecode.com/svn/java/server/tags/0_9rc1/，单击Finish
 d.在Eclipse左侧，出现一个条目"http://red5.googlecode.com/svn/java/server/tags/0_9rc1/"，右键点击，弹出菜单选择"检出为..."

![](https://img-blog.csdn.net/20131009235011156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




e.等待片刻，弹出"从SVN检出"界面，选中"做为工作空间中的项目检出"，单击Finish
 f.弹出"SVN检出"界面，此时正在从远程SVN服务器取回源代码，等待1～N分钟（视你的网速）
 g.下载完毕，选择Eclipse主菜单中Window->Open Perspective->Java，进入Java编辑视图

**4.编译red5源代码**
 a.打开red5_server工程，找到build.xml，右键单击，选择Run As->Ant Build
 b.在Ant引导的编译过程中会利用Ivy下载相关依赖的jar文件(入门推荐学习[这篇文章](http://www.ibm.com/developerworks/cn/java/j-ap05068/?S_TACT=105AGX52&S_CMP=NL&ca=dnl-cn-06112008)及[官方教程](http://ant.apache.org/ivy))
 c.看到BUILD SUCCESSFUL。打开lib文件夹，所有依赖的jar文件都放在里边了。打开工程属性，选择Java Build Path，打开Libraries选项卡，单击Add JARs，选取lib文件夹里所有的jar，单击OK

**5.运行red5 Standalone**
 a.找到org.red5.server.Standalone，运行之。
 b.日志系统使用[LogBack](http://logback.qos.ch/)






