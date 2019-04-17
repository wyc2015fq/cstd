# 【GM+Fiddler】安卓VM搭建实现谷歌服务 _一时兴起的自我挑战 - CD's Coding - CSDN博客





2015年04月29日 21:36:53[糖果天王](https://me.csdn.net/okcd00)阅读数：1545








## 前言

近些日子猛然发现自己新技能Get的太少了……

随着这个念头的出现，便开始了又一次的技能发掘。

这次给自己的任务是：使用安卓虚拟机，完成翻墙，搭建谷歌框架，实现虚拟机下的谷歌服务的完整实现。

P.S. 不要流口水啦，Ingress无法在VM下跑起的哟，在很早的更新中Google的猩猩大大们早就把这个解决掉啦~

（在我进入这个游戏之前这个漏洞就被修复了到底算是好事还是不好呢……）

从头到尾都是自己一个人倒腾出来的，没有依赖询问任何人哦~~ 得瑟得瑟~~o(^▽^)o




嘛不管啦，毕竟花了我10个小时左右，做个成果汇总，流程处理记录一下也作为今后备忘吧——

### 结果展示：

![](https://img-blog.csdn.net/20150429180037749?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





## 本次流程中所使用的物品清单：



### 重要物品：

》梯子 此处使用的是SS-Shadowsocks 

》Wiki 类情报搜集能力

### 所需程序：

》[Genymotion](https://www.genymotion.com/#!/download) //速度极快的PC端安卓模拟器


》[Oracle VM VirtualBox](https://www.virtualbox.org/wiki/Downloads) //

》[Fiddler Web Debugger](http://www.telerik.com/download/fiddler) //

》[Genymotion-ARM-Translation.zip](http://forum.xda-developers.com/showthread.php?t=2528952) //

》[com.muzhiwan.gsfinstaller.apk](http://www.muzhiwan.com/com.muzhiwan.gsfinstaller.html) //

不同的环境下可能会用到的程序：

》[NetworkLocation.apk](https://github.com/microg/NetworkLocation/releases)

》[Genymotion-Uninstall](http://www.how-to-uninstall-program.com/uninstaller/uninstall-genymotion-genymotion-removal-genymotion-windows-uninstaller.html)



》[Gapp](http://forum.xda-developers.com/showthread.php?t=2528952)

### 本次试验所使用环境：

》Win8.1 Professional x64

》Google Galaxy Nexus - 4.1.1 - API16 - 720x1280 （VM配置）







## 图文流程及坑点解说

![](https://img-blog.csdn.net/20150429210330075?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


最初就是这仨兄弟了，他们是可以一起获得的，在[Genymotion官网](https://www.genymotion.com/#!/)上注册一个账号，然后在[下载页面](https://www.genymotion.com/#!/download)下载，选择自己的操作系统环境，以及为了获得VM的VirtualBox，我们选择稍微大一点的那个 Get
 Genymotion(117.47MB)

![](https://img-blog.csdn.net/20150429210552737?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


下载安装完成之后打开Genymotion，在Add里可以选择你想要的模拟的安卓机类型，这里就需要刚刚我们用到的那个账户，登录之后，这里就有好多机型供你选择啦，我随手选了个GoogleGalaxy，大家按理说可以随意选择的呆胶布~~ 啊对了，要记住你选择的安卓版本哦~

![](https://img-blog.csdn.net/20150429211122772?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20150429211120293?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


安卓机跑起来之后呢，在文件夹里把事先下载好的 Genymotion-ARM-Translation.zip 拖动到窗口中，会出现上述的【正在文件传输】以及【要不要安装】（抱歉我英语不太好），点击ok，如果显示Success，那么关掉虚拟机重启即可，如果出现Failed（这里我Failed了快几万次了……），那么请下载个新的试试，我装国内论坛里给的百度网盘下载出来的就都Failed，然后搬了个梯子在墙外下了个就正常了我也是无奈呀……

P.S. 但是无论墙内墙外，“gapps-jb-20121011-signed”和“gapps-jb-20130813-signed”都是万年Failed，所以我才放弃了通过拖动zip的方法来获得谷歌框架，那么我们该怎么弄呢？别着急，看下一步~

![](https://img-blog.csdn.net/20150429211553840?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


我们可以通过可以安装的apk来帮助我们进行安装，我的第一反应是使用和我的Note一样用小米的谷歌安装器，就是红色图标的那个，但无奈于总是说apk包有问题不让装，于是这里我用了那个拇指玩的谷歌安装器，在我的环境下，这个是成功可行的，拖动上来之后就扔给他我不管了……这一过程中似乎有需要架梯（fan）子（qiang）的部分，所以在这一步之前，我就先把墙的问题解决了，具体如下

![](https://img-blog.csdn.net/20150429211926078?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


这个软件叫做Fiddler，是一个较为常用的代理用软件，与我们日常所见到的不同，这个的好处是可以将自己电脑的网络共享给虚拟机中使用，注意这点:虚拟机同时共享电脑上正在使用的VPN，可以访问墙外。事先将电脑使用Shadowsocks进行架梯，由于现在似乎不让详细说关于架梯子的事情这里就一笔带过，总之就是无论什么方法先让电脑翻过去，然后打开fiddler。PC端这边设置好了，虚拟机那里自然也是要设置的。

![](https://img-blog.csdn.net/20150429212225603?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


虚拟机中的网络是通过NAT（网络地址转换）经由ＰＣ的网络进行供给的，所以时刻处于链接在“WiredSSID“下，我们需要做的是长按，修改设置，将代理设置为手动，服务器主机名为10.0.3.2，端口随意，什么你问我为什么？我要模仿我看到这条情报的时候，那个帖子主所说的话来回复你们”这是固定搭配，懂吗？固定搭配！“

网络设置好的话，按理说安装器们会尽职尽责的帮你弄好Market的，在所有任务全都完成之后，点击Market，输入你的GoogleID，此处如果使用我刚刚所说的方法架梯出墙，是可以很快获得回应并成功登上的。没有架梯的情况下……我等了一段时间没有回复就没有继续等了，也不知道能不能好……

<! --- if you are a ingresser:

如果有玩Ingress的孩纸们看到这里我觉得可能会蠢蠢欲动想用这个做点什么，友情告诉你们哟，在VM下，商店中不提供Ingress的下载~ 什么？你说你拆了可用的包强行装进去？哈哈，还是不行哦，VM下根本无法打开呢~ 什么？你还想用VM原生的GPS组件替换掉NetworkLocation，哈哈哈，依然不行哦，这里并不是谷歌服务的问题，文章开头就给你们展示了可以经由谷歌服务下通过VM更改GPS读入数据，以更改终端当前所在位置GPS，但是，游戏依然是不会让进的哟~ 和Permission无关，所以去改rw-r--r--没有意义，由于和GPS来源无关，所以替换/修改/改名
 Location的APK没有意义，是在游戏包内部的代码中有所判断，所以不重写重新打包都是没用的哟~啊对了，重新打包的时候如果你没有NIA的数字签名也是无法游戏的，(●'◡'●)　所以～咱们公平友善地游戏吧~ 别想歪门邪道咯~ 啊对了上面这段如果有哪里不对请评论OR私信我哟~若有判断不对或者语句不对的地方我会仔细倾听详细地理解然后立马更正的~~ 欢迎点评指教~ >




![](https://img-blog.csdn.net/20150429175305848?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)










