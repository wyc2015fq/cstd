# APP安全性测试 - zhusongziye的博客 - CSDN博客





2017年11月21日 21:34:44[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：400








**前言:**

　　随着互联网发展，APP应用的盛行，最近了解到手机APP相关的安全性测试，以webview为主体的app，站在入侵或者攻击的角度来讲，安全隐患在于http抓包，逆向工程。

　　目前大部分app还是走的http或者https，所以防http抓包泄露用户信息以及系统自身漏洞是必要的，通过抓包当你查看一个陌生用户信息时，一些手机号，qq等信息页面上应该不显示的，但这些信息不显示并不代表服务器没有下发，好多都是客户端限制的，通过抓包，完全可以查看到陌生用户的app。再如好多发帖，push消息的应用，如果没有消息有效性的验证，抓到包之后篡改消息，服务器一点反应都没，这就会留有极大的隐患。逆向工程对于android就很好理解了，反编译，修改或者插入自己的代码，以达到相应目的。


**安全性测试策略**


　　1. 用户隐私

　　· 检查是否在本地保存用户密码，无论加密与否

　　· 检查敏感的隐私信息，如聊天记录、关系链、银行账号等是否进行加密

　　· 检查是否将系统文件、配置文件明文保存在外部设备上

　　· 部分需要存储到外部设备的信息，需要每次使用前都判断信息是否被篡改

　　备注：本地存储数据可以查看看应用的SharedPreferences文件和数据库文件中的数据（root后在应用安装目录内，或者查看外部存储中有没有写入敏感数据）。

　　2. 文件权限

　　· 检查App所在的目录，其权限必须为不允许其他组成员读写

　　3. 网络传输

　　· 检查敏感信息在网络传输中是否做了加密处理，重要数据要采用TLS或者SSL

　　备注：http请求默认是明文的，如果安全验证和加密机制很烂，通过网络嗅探扫描，很容易被猜到和模拟请求，也可能被注入。

　　比如：允许一个虚假的HTML形式被注入攻击者利用来诱骗用户输入他们的用户名和密码，然后把他们的凭据发送到恶意网站。

　　4. 运行时解释保护

　　· 对于嵌有解释器的软件，检查是否存在XSS、SQL注入漏洞

　　· 使用webiew的App，检查是否存在URL欺骗漏洞

　　5. Android组件权限保护

　　· 禁止App内部组件被任意第三方程序调用。

　　· 若需要供外部调用的组件，应检查对调用者是否做了签名限制

　　6. 升级

　　· 检查是否对升级包的完整性、合法性进行了校验，避免升级包被劫持

　　7.应用自身安全性

　　· 对某个应用进行逆向，看反编译后的代码有没有敏感信息暴露。反编译后对代码修改，插入劫持代码后重新打包，如果存在这种漏洞，对用户和开发者都构成极大的威胁。

　　备注：要求对应用进行加密，防止静态破解，盗取源码，然后嵌入恶意病毒、广告等行为再利用工具打包、签名，形成二次打包应用

8.界面截取

　　· 通过adb shell命令或第三方软件获取root权限，在手机界面截取用户填写的隐私信息，随后进行恶意行为。

**安全性测试方法**


　　如下为转载 Seay FreeBuf 的文章

　　移动app大多通过web api服务的方式跟服务端交互，这种模式把移动安全跟web安全绑在一起。移动app以web服务的方式跟服务端交互，服务器端也是一个展示信息的网站，常见的web漏洞在这也存在，比如说SQL注入、文件上传、中间件/server漏洞等，但是由于部分app不是直接嵌入网页在app中，而是使用的api接口返回josn数据，导致扫描器爬虫无法爬取链接。

　　下图是抓的糗事百科糗事列表

![](https://img-blog.csdn.net/20171121212354698?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




那么我尝试去找app服务端的漏洞，目前想到的两种方法：

　　1.反编译APP

　　2.http[s]代理抓包

　　那么有人应该会提出问题，这两种方式拿到的链接都是零零散散的，也不好找漏洞啊，我这边的利用方式是把所有抓取的链接直接提交任务到多引擎web漏洞扫描器，扫描器可以批量扫SQL注入等等，其实除了这些漏洞，还有很多可以利用的信息。

　　一、反编译APP

　　有两种反编译方式，dex2jar和apktool，两个工具反编译的效果是不一样的，dex2jar反编译出java源代码，apktool反编译出来的是java汇编代码。

　　1. dex2jar反编译

　　工具：dex2jar+jdgui

　　方法：

　　a. 修改apk为zip扩展名
![](https://img-blog.csdn.net/20171121212440042?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


       b. 解压出classes.dex文件

　　c.使用dex2jar反编译(dex2jar.bat classes.dex)
![](https://img-blog.csdn.net/20171121212456701?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


最后反编译出来的源码如下图。虽然部分类被配置proguard.cfg 混淆了，但是还是可以利用的。
![](https://img-blog.csdn.net/20171121212540754?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


2. apktool反编译

　　工具：apktool

　　这个工具比较简单，直接(apktool d apkfile)就可以反编译apk文件，反编译出来的东西为smali反汇编代码、res资源文件、assets配置文件、lib库文件，我们可以直接搜索smali文件和资源文件来查找链接等。
![](https://img-blog.csdn.net/20171121212600212?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


**利用app查找网站真实IP**



　　除了app服务端的漏洞，还有一个比较好玩的利用方式，通过收集app里面的子域名ip来寻找目标网站的真实IP，根据经验，大多app的接口都没有使用cdn等服务。
![](https://img-blog.csdn.net/20171121212622733?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


糗事百科真实IP
![](https://img-blog.csdn.net/20171121213249684?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**二、http[s]代理抓包**

　　这个方法利用在移动设备上设置代理，通过人工操作使app与服务端交互

　　步骤：

　　a. 在抓包机器上开启代理，测试可以用burp，需要自动化提交扫描任务可以自己写一个代理程序，移动设备设置代理服务器。

![](https://img-blog.csdn.net/20171121212815699?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



　b. 在移动设备上操作app，代理端抓取如下。
![](https://img-blog.csdn.net/20171121212925614?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


**总结：**

　　整个思路已经很清晰，那么其实要做的就是让这个过程自动化，反编译之后有一个问题，url不一定完整，很多URL都是拼接起来的，我尝试写一套分析引擎，自动化反编译，然后通过对源码的分析，拼接完整的api url，再进行漏洞扫描。

　　下图是一个dome，后面准备用python来写，放到服务器上。
![](https://img-blog.csdn.net/20171121213045233?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**案例分享**



　　1、任意用户登录

　　某次测试一个app，RP比较好，发现一任意用户登录漏洞。在本地的配置文件中有登录用户的帐号和密码，APP设计比较奇葩，只是验证了用户登录邮箱，没有 验证密码，导致通过修改本地的配置文件就可以实现任意用户登录，登录之后能够查看别人的订单等数据。在android虚拟机中安装的app都在/data /data目录下，大概的目录结构如下：
![](https://img-blog.csdn.net/20171121213111995?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


      app安装目录下的结构都是差不多的，主要有缓存文件、数据库目录、本地文件、配置文件等。比较重要的目录有databases、shared_prefs。分别保存了数据库文件和配置文件。

　　言归正传，查看了安装app的shared_prefs目录，发现其中一个文件内容如下：
![](https://img-blog.csdn.net/20171121213134398?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvemh1c29uZ3ppeWU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


     可以看到有用户的登录邮箱和密码，将邮箱修改成存在的用户邮箱，密码随意输入，然后adb shell之后，用linux命令删除android虚拟机上已经存在的配置文件，再用adb push将修改后的文件发送到android虚拟机，再打开app发现已经用其它用户成功登录了。
            


