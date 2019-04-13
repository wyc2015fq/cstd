
# Fiddler抓包工具使用 - 高科的专栏 - CSDN博客

2019年03月01日 10:52:57[高科](https://me.csdn.net/pbymw8iwm)阅读数：36


先下载Fiddler
电脑最好是笔记本，这样能和手机保持统一局域网内；其他不多说，直接说步骤了。
**一.对PC（笔记本）参数进行配置**
**1. 配置fiddler允许监听到https（fiddler默认只抓取http格式的）**
打开Fiddler菜单项**Tools->TelerikFiddler Options->HTTPS**，
勾选**CaptureHTTPS CONNECTs**,点击Actions，
勾选**Decrypt HTTPS traffic**和**Ignore servercertificate errors**两项,点击OK（首次点击会弹出**是否信任fiddler证书和安全提示，直接点击yes**就行），见图：
![](https://img-blog.csdn.net/20160919184428770?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**2.配置fiddler允许远程连接**
上一步窗口中点击Tools->TelerikFiddler Options->**Connections，勾选allow remote computers to connect**，默认监听端口为8888（下图Fiddler listens on port就是端口号），若端口被占用可以设置成其他的，配置好后要重新启动fiddler，如下图：
![](https://img-blog.csdn.net/20160919184445956?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**点击完成之后，PC端配置完成**，接下来开始配置手机端，以安卓为例，iOS方法同安卓；
**二.对手机端进行参数配置**
首先要保证手机和电脑都处于同一个网络；
然后要知道电脑的ip地址（当然这个电脑是安装fiddler的电脑），电脑ip地址可通过cmd，输入ipconfig查询，或网络共享中ipv4找到，我的是192.168.103.53，
再者要知道fiddler的端口号，Tools->TelerikFiddler Options->Connections，port中值就是端口号，一般默认为8888；接下来开始操作手机；
**1.第一步：**手机和电脑连接同一个网络，打开手机浏览器，输入**http://ip:端口号**，点击前往；见下图：
![](https://img-blog.csdn.net/20160919184507911?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**2.第二步：**前往之后会跳转到证书下载页，见下图：
点击FiddlerRootcertificate下载证书；
![](https://img-blog.csdn.net/20160919184529474?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**3.第三步**：点击下载之后，安装证书并起个名字，随便写就行，点击确定；
![](https://img-blog.csdn.net/20160919184538255?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**4.第四步：**要求设置一个手机密码，自己设置一个，记住密码就行，最后不用了去系统-安全-密码中去掉即可；
![](https://img-blog.csdn.net/20160919184547786?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**5.第五步：**更改手机无线网的代理
手机系统设置-无线网-点击高级，代理选择手动，主机名就是fiddler的电脑ip地址，端口号，就是fiddler的端口号，与浏览器中ip：端口号保持一致，点击确定即可，
![](https://img-blog.csdn.net/20160919184559302?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**6.操作手机**，电脑fiddler中会显示一些http信息，成功；

**三.其他**
**1.停止电脑对手机的网络监控**
系统设置-wifi，找到代理，去掉即可；
**2.删除手机中证书**
安卓系统设置—安全—收信任的凭证—用户，点击证书删除即可；
**3.删除手机上密码**
手机系统—安全—密码，删除系统密码即可；
**4.根证书安装不成功**
打开cmd命令行
进入Fiddler的安装目录下。我的是：D:\fiddler
输入命令：
makecert.exe -r -ss my -n "CN=DO_NOT_TRUST_FiddlerRoot, O=DO_NOT_TRUST, OU=Created by http://www.fiddler2.com" -sky signature -eku 1.3.6.1.5.5.7.3.1 -h 1 -cy authority -a sha1 -m 120 -b 10/12/2020
注意，最后的日期需要超过你安装软件的日期
1、首先确保安装的 Fiddler 是较新的版本，比较老的版本可能会出现安装不上fiddler证书生成器的问题。
2、关闭Fiddler。
3、下载并安装Fiddler证书生成器。点击打开链接[http://www.telerik.com/docs/default-source/fiddler/addons/fiddlercertmaker.exe?sfvrsn=2](http://www.telerik.com/docs/default-source/fiddler/addons/fiddlercertmaker.exe?sfvrsn=2)

4、打开Fiddler，点击工具栏中的Tools——>Fiddler Options…
![](https://img-blog.csdn.net/20171023190233005?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbDEzMzYwMzc2ODY=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

5、切换到 HTTPS 选项卡，勾选 Capture HTTPS CONNECTs，勾选 Decrypt HTTPS trafic，会弹出安装证书的提示。一路点是或确定安装即可。
6、重启fiddler，依然停留在 HTTPS 选项卡，点击右侧Actions，能看一个下拉菜单，点击 Export Root Certificate
to Desktop，此时证书会生成到桌面上，名为 FiddlerRoot.cer，点OK保存


