# IntelliJ IDEA创建JavaWeb工程及配置Tomcat部署 - 零度的博客专栏 - CSDN博客
2017年05月09日 09:40:53[零度anngle](https://me.csdn.net/zmx729618)阅读数：20834
步骤：
![](https://img-blog.csdn.net/20160327230418551?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160327230554216?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160327230620208?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
在WEB-INF 下创建classes 和 lib 两个文件夹
![](https://img-blog.csdn.net/20160327230644208?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
右上角一个蓝色的按钮...
![](https://img-blog.csdn.net/20160327230726515?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
Modules选项卡,Paths下的配置...输出路径选择classes
![](https://img-blog.csdn.net/20160327230808141?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
Dependencies选项卡下,"+"加号 jars or dire...那个按钮...
![](https://img-blog.csdn.net/20160327230913678?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
选中WEB-INF 下的lib文件夹
![](https://img-blog.csdn.net/20160327231036413?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
选择Jar Directory
![](https://img-blog.csdn.net/20160327231058772?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
配置好,再勾选上lib...
然后配置Tomcat
![](https://img-blog.csdn.net/20160327231124504?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160327231211865?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
Name随便写
点击 Configure.. 添加配置你本地的Tomcat路径.....
After launch 去掉那个勾
HTTP port 是端口号
![](https://img-blog.csdn.net/20160327231230976?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
选中 Deployment选项卡
右手边的"+"加号
选中Artifact...
![](https://img-blog.csdn.net/20160327231300444?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20160327231412820?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后将 On"Update "action 和 On frame deactivation 选为update classes and resources 
选中这两个以后 就能够热部署了....
![](https://img-blog.csdn.net/20160327231432211?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后在index.jsp 中 加一个HelloWorld,然后就可以运行了.....
![](https://img-blog.csdn.net/20160327231449680?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
后来留意看了下,发现编译后的文件,输出到了out文件夹中,
少了一步配置....
在Artiacts选项卡中,将 Outputdirectory设置为WEB-INF 下的classes 文件夹.....
然后就行了
![](https://img-blog.csdn.net/20160328094959712?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
解释的比较少，不过对照图上的配置信息应该足够了，运行成功！
