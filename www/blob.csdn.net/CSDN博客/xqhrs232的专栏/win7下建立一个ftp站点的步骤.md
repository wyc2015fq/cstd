# win7下建立一个ftp站点的步骤 - xqhrs232的专栏 - CSDN博客
2017年01月20日 14:24:26[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：410
个人分类：[PC/Windows/Office技术](https://blog.csdn.net/xqhrs232/article/category/906929)
原文地址::[http://blog.csdn.net/u011449588/article/details/41842071](http://blog.csdn.net/u011449588/article/details/41842071)
相关文章
1、win7系统建立ftp服务器图文步骤----[http://blog.sina.com.cn/s/blog_8a095c5b0102vk9l.html](http://blog.sina.com.cn/s/blog_8a095c5b0102vk9l.html)
2、win7系统中ftp服务器搭建方法(多图)----[http://www.jb51.net/os/windows/158410.html](http://www.jb51.net/os/windows/158410.html)
3、win7下如何建立ftp服务器----[http://jingyan.baidu.com/article/574c5219d466c36c8d9dc138.html](http://jingyan.baidu.com/article/574c5219d466c36c8d9dc138.html)
1.首先在控制面板中打开程序和功能
![](https://img-blog.csdn.net/20141210090709138?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTQ0OTU4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
2.然后点击打开或关闭windows功能
![](https://img-blog.csdn.net/20141210090919718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTQ0OTU4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
3.勾选住Ftp服务器
![](https://img-blog.csdn.net/20141210091131983?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTQ0OTU4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
4.打开“我的电脑”->"管理"->“本地用户和组”->右键"用户"->"新用户"
![](https://img-blog.csdn.net/20141210091805665?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTQ0OTU4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
出现以下界面：那个hello是新建的，heyoufei是以前建立的
![](https://img-blog.csdn.net/20141210091847364?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTQ0OTU4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
5.打开控制面板中的管理工具，再打开管理工具里的“Internet 信息服务(IIS)管理器”
![](https://img-blog.csdn.net/20141210092610077?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTQ0OTU4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
6.此时你屏幕左面的“网站“下面是没有ftp的，在“网站”右键”添加ftp站点“，这时要选择一个物理路径，则在你的电脑上的随便哪个盘新建一个文件夹，以后就用这个文件夹上传文件和下载文件，我这里是D:\ftp 文件夹
![](https://img-blog.csdn.net/20141210092713289?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTQ0OTU4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
7：IP地址就是你本地计算机的IP地址，然后点击下一步，端口号默认即可，ssl改为允许，然后下一步
![](https://img-blog.csdn.net/20141210093103810?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTQ0OTU4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
8：身份验证改成基本，允许访问改为所有用户，权限你可以自己选择了，然后进行下一步完成
![](https://img-blog.csdn.net/20141210093328703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTQ0OTU4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
9：验证ftp服务器是否创建成功，
在别的计算机上登入ftp:10.22.128.166试试吧，这里你要先关闭本地计算机的防火墙，不然别的计算机不能访问，然后输入账号和密码
![](https://img-blog.csdn.net/20141210094031870?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTQ0OTU4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
登入成功：
![](https://img-blog.csdn.net/20141210094104453?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTQ0OTU4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)，
当然你可以再我的电脑中输入ftp://10.22.128.166,可以试试在ftp上上传和下载文件了。
![](https://img-blog.csdn.net/20141210094335703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTQ0OTU4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
最后说明一点，ftp上下载和上传文件都可以在命令行下执行，有兴趣可以一试
![](https://img-blog.csdn.net/20141210094916890?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTQ0OTU4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
查看ftp里的内容，用ls或dir命令
![](https://img-blog.csdn.net/20141210095014110?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTQ0OTU4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
用get命令下载ftp到本地计算机，这里要主要下载的文件有默认的文件夹，详情见图片
![](https://img-blog.csdn.net/20141210095227984?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTQ0OTU4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
用put命令上传文件：
![](https://img-blog.csdn.net/20141210095319687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTQ0OTU4OA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
最后你可以查看在IP地址为10.22.128.166中新上传的文件，最后退出ftp用bye命令或者quit命令
其实也可以不用终端，用第三方软件也可以实现的，这里我就不介绍了
