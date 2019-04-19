# Xcode6+iPhone6 搭建iOS开发真机测试环境 - xqhrs232的专栏 - CSDN博客
2018年12月05日 11:33:17[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：50
个人分类：[Mac OS/i OS技术](https://blog.csdn.net/xqhrs232/article/category/1252162)
原文地址::[https://blog.csdn.net/fly601845/article/details/43406701](https://blog.csdn.net/fly601845/article/details/43406701)
相关文章
1、[iOS开发教程之：iPhone开发环境搭建](https://www.cnblogs.com/wqsbk/p/3401043.html)----[https://www.cnblogs.com/wqsbk/p/3401043.html](https://www.cnblogs.com/wqsbk/p/3401043.html)
 开始条件如下：
            1.已经交钱的开发者账号公司和个人都可以。如果没有的话请申请。公司用户有个特殊的地方，就是邓白氏编码，这个直接网络申请，不要钱的，要等一到两周时间。
            2.Mac电脑一台并安装好Xcode 6
            3.测试手机一台，我的是iPhone6。确保他已经连上了Mac机器
       如下流程，在苹果的开发者网站选择"Certificates, Identifiers & Profiles"，然后，依次添加
- [Certificates](https://developer.apple.com/account/ios/certificate/certificateList.action)
- [Identifiers](https://developer.apple.com/account/ios/identifiers/bundle/bundleList.action)
- [Devices](https://developer.apple.com/account/ios/device/deviceList.action)
- [Provisioning Profiles](https://developer.apple.com/account/ios/profile/profileList.action)
       然后把生成的Certificates和Provisioning Profiles下载安装到开发的Mac机器上，再打开Xcode6，新建或打开项目，做一些简单设置，最后设定项目要调试的运行设备为我的iPhone6，点击启动后，就OK了。
        详情如下：
        一、在苹果网站点击"Certificates, Identifiers & Profiles"进入之后，点击iOS Certificates右边的“+”，开始添加。
![](https://img-blog.csdn.net/20150107170908250?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWlzaGVuZ2hlcWl1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后选择“iOS App Development”，
![](https://img-blog.csdn.net/20150107171431218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWlzaGVuZ2hlcWl1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
进入上传证书页面
![](https://img-blog.csdn.net/20150107171606812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWlzaGVuZ2hlcWl1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
此时需要制作一个证书签名请求文件，制作好之后上传。制作流程如下：Finder=>应用程序=》实用工具=》钥匙串访问，然后如下图：
![](https://img-blog.csdn.net/20150107172247639?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWlzaGVuZ2hlcWl1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
在弹出对话框填写如下内容：
![](https://img-blog.csdn.net/20150107172428119?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWlzaGVuZ2hlcWl1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
点继续后生成证书签名请求文件。
![](https://img-blog.csdn.net/20150107172608302?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWlzaGVuZ2hlcWl1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
记住这个文件的路径和文件名，然后提交到前面转向的苹果网站页面上去。
![](https://img-blog.csdn.net/20150107172725476?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWlzaGVuZ2hlcWl1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
点击Generate后，生成证书文件。
![](https://img-blog.csdn.net/20150107173123125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWlzaGVuZ2hlcWl1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
点击Download，到本机，然后双击安装证书，在下图的窗口中点击“添加”
![](https://img-blog.csdn.net/20150107173954546?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWlzaGVuZ2hlcWl1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
证书安装完毕。
二、申请APPID，点击苹果页面左边菜单的”Identifiers“=》App IDS，然后点右边的“+”号
![](https://img-blog.csdn.net/20150107174557976?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWlzaGVuZ2hlcWl1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20150107174713106?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWlzaGVuZ2hlcWl1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后填写AppID的相关信息，建议如下图的方式填写App ID Suffix，填写完毕后，点击
![](https://blog.csdn.net/fly601845/article/details/43406701)
勾选App需要用到的服务，如下图，
![](https://img-blog.csdn.net/20150107175335322?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWlzaGVuZ2hlcWl1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
勾选完毕点击“Continue”
![](https://img-blog.csdn.net/20150107175640401?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWlzaGVuZ2hlcWl1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
点击“Submit”后，App ID 信息添加完毕。
三、添加Device（添加设备），还是点击苹果网页的坐标菜单“Devices”=》all，然后点击右边的“+”号，如下图：
![](https://img-blog.csdn.net/20150107180429899?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWlzaGVuZ2hlcWl1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
获取UDID方法如下：将手机连接上电脑，打开iTunes，在左边菜单选中设备，点击描述，如图，然后点击序列号，就会在序列号的地方显示UDID
![](https://img-blog.csdn.net/20150107183257665?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWlzaGVuZ2hlcWl1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
将UDID填写到上面的页面，然后点提交，设备就加好了。如下图：
![](https://img-blog.csdn.net/20150107183632322?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWlzaGVuZ2hlcWl1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
四、添加Provisioning Profiles文件，老规矩，点击左边的“Provisioning Profiles”=》“all”，然后点击右边的“+”
![](https://img-blog.csdn.net/20150107184107304?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWlzaGVuZ2hlcWl1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
### 选中“iOS App Development”，然后点击“Conteinue”
![](https://img-blog.csdn.net/20150107184341822?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWlzaGVuZ2hlcWl1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
选择您需要的App ID，然后点击“Continue”
![](https://img-blog.csdn.net/20150107185053443?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWlzaGVuZ2hlcWl1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
选择证书，然后点击“Continue”
![](https://img-blog.csdn.net/20150107185133177?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWlzaGVuZ2hlcWl1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
选择前面添加的设备，然后点击“Continue”
![](https://img-blog.csdn.net/20150107185202895?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWlzaGVuZ2hlcWl1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
输入配置文件名，然后点击“Generate”
![](https://img-blog.csdn.net/20150107185357851?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWlzaGVuZ2hlcWl1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
配置文件生成完毕，下载到本机，然后双击打开安装。
五、项目配置，启动Xcode 6，然后打开选择项目，如下图，指定项目的PP文件(Provisioning Profile)。
![](https://img-blog.csdn.net/20150107191005989?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWlzaGVuZ2hlcWl1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
修改plist文件
![](https://img-blog.csdn.net/20150107200818773?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWlzaGVuZ2hlcWl1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
另外还有个需要注意的地方，就是要选择一下Team，让Xcode自动在Developer Member Center的iOS Provisioning Profiles生成一个iOS Team Provisioning Profile文件
方法如下：添加Xcode的开发者账号，Xcode=>Preferences (适用于Xcode6，Xcode5 是在Organizer中)，
![](https://img-blog.csdn.net/20150107195946327?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWlzaGVuZ2hlcWl1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后，选中项目=》General=》Team，选择你的开发者账号，
![](https://img-blog.csdn.net/20150107200058703?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWlzaGVuZ2hlcWl1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
此时刷新页面，发现Xcode已经为我们生成好了Team Provisioning Profiles
![](https://img-blog.csdn.net/20150107200333562?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWlzaGVuZ2hlcWl1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
最后一步修改项目的目标调试环境为连上电脑的iPhone，
![](https://img-blog.csdn.net/20150107201520738?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWlzaGVuZ2hlcWl1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后点击启动或者调试项目，还会弹出下面的对话框：
![](https://img-blog.csdn.net/20150107201741357?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWlzaGVuZ2hlcWl1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
点击"Always Allow",设置成功，程序在手机中启动了！如下图
![](https://img-blog.csdn.net/20150107202621617?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWlzaGVuZ2hlcWl1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
希望对大家有所帮助！谢谢！
