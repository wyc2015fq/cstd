# 人手一份核武器 - Hacking Team 泄露（开源）资料导览手册 - =朝晖= - 博客园
# [人手一份核武器 - Hacking Team 泄露（开源）资料导览手册](https://www.cnblogs.com/dhcn/p/7906435.html)
https://zhuanlan.zhihu.com/p/20102713
author:蒸米
# 0x00 序
事先声明本人并不是全栈安全工程师，仅仅是移动安全小菜一枚，所以对泄漏资料的分析难免会有疏忽或着错误，望各位围观的大侠手下留情。
首先来看安全界两大元老对Hacking Team(以下简称HT)被黑这个事件的看法：
@tombkeeper: Stuxnet 让公众知道：“原来真有这种事”，Snowden 让公众知道：“原来这种事这么多”，Hacking Team 让公众知道：“原来这种事都正经当买卖干了”。
@赵武在路上: 2011年的时候，HBGray被黑，很多人没有意识到意味着什么，因为跟国家安全相关。这两天Hacking team被黑，大家也没有意识到意味着什么。这次包括了客户清单和0day，但我更关注的是RCS的代码，以前行业内都是粗糙到不行的demo，工程化公开的很少，这次会让行业内的技术往前推进几年，尤其是黑产。
可以说这次事件和斯诺登事件的影响力是不相上下的，但HT被黑不光光是让公众知道有这回事，随之而来还有整整415G的泄漏资料！里面有Flash 0day, Windows字体0day, iOS enterprise backdoor app, Android selinux exploit, WP8 trojan等等核武级的漏洞和工具。那么废话不多说，我们这就开始导览之旅。
![](https://pic2.zhimg.com/50/39a7100a8a7608c5c5a136d032ed86f5_hd.jpg)
# 0x01 总览
因为所有文件加起来的大小整整有415.77G。光下载就得好久好久。还好有人把整个镜像放了一份在网上。有兴趣的同学可以直接去查看：[http://ht.transparencytoolkit.org/](http://link.zhihu.com/?target=http%3A//ht.transparencytoolkit.org/)。据说之所以这么大是因为里面有很多的邮件。但你不用担心你的小水管，有好人整理了一个只有1.3G的精华版。在这里我也提供一个百度网盘下载：[HackingTeam_免费高速下载](http://link.zhihu.com/?target=http%3A//pan.baidu.com/s/1i3lHQRF) 。在下载完完整版之前，我们就先拿精华版解解馋吧。
里面的数据大概是这样的：
![](https://pic3.zhimg.com/50/d8a92a97f21b60bcbe8e61db0ac91e6a_hd.jpg)
“HACKING TEAM PASSWORDS AND TWEETS.pdf” 里主要保存了Christian Pozzi这个人经常去的网站的账号以及密码以及twitter的截图。估计他就是那个被黑了电脑的人了，因为这个人的电脑被黑，连带着HT内网git服务器，知识库，邮件服务器的数据全部都被dump下来了。
Hacking Team Saudi Arabia Training.pdf里面数据貌似不全，通过提纲来开看主要是介绍了如何安装和使用RSC (Remote Control System)系统。不得不说HT最牛的东西就是他们的RCS系统了，他们公司实现了全平台的RSC系统（包括windows phone）。我们可以看一下他们系统的截图：
![](https://pic4.zhimg.com/50/0570b826ae82ad8150a15df4406636f7_hd.jpg)
各种监控信息详细到令人发指。不知道有多少人被这样监控着。
gitosis-admin-master.zip里保存了git服务器上成员的public key以及每个人负责的项目，通过”gitosis.conf”就可以大概知道哪个项目是谁在做了。比如placidi这个成员主要做android相关的项目。Naga, daniele, zeno, diego, ivan这几个人组要做fuzzer。Matteo, zeno, daniele这几个主要做病毒查杀检测。
![](https://pic2.zhimg.com/50/d675edde4214127fadb15ea8b7c56321_hd.jpg)
所以接下来的章节我们也会按照他们的分组来进行分别讲解。
# 0x02 Android
1 core-android-audiocapture-master.zip主要是利用Collin Mulliner提供的hook框架来hook mediaserver从而进行语音和通话监听。”Pack”文件夹里保存了最后编译完成的程序。而”references”文件夹里几乎都是Collin Mulliner论文的ppt，并且整个项目就是在[crmulliner/adbi · GitHub](http://link.zhihu.com/?target=https%3A//github.com/crmulliner/adbi)这个项目上改的。截取下来的音频并不是 wav格式，还需要使用”decoder”文件夹下的工具进行解密，看样子作者除了电话监听，还成功测试了wechat, whatsapp, skype等应用的语音截获。
![](https://pic4.zhimg.com/50/db04830bc5200461a134bdb220907cbf_hd.jpg)
2 core-android-market-master.zip应该是用来向Google Play上传监控应用的项目。虽然说Google Play检测系统，但对于这种用于APT攻击的malware是毫无作用的。在\core-android-market-master\doc\readme.txt中还保存HT开发者账号的用户名和密码。但是当笔者准备尝试登录的时候，发现密码已经在几个小时前被修改了。
![](https://pic2.zhimg.com/50/277ab60e98f9b42bce60f6a530d3ec65_hd.jpg)
3 core-android-master.zip就是HT的RSC系统源码了。除去一些编译用的gradle文件，所有的源码都保存在” \core-android-master\RCSAndroid”目录下，通过这个RSC app除了可以做到基本信息监控外，还可以获取所有主流社交应用的信息。
![](https://pic2.zhimg.com/50/fa58edb637ccff7545b222f706003891_hd.jpg)
在应用加固方面，这个RSC app除了使用了DexGuard进行混淆，还有虚拟机检测功能。根据开发日志，这个项目貌似还使用很多0day的trick对应用进行混淆。非常值得以后慢慢研究。 接下来就是重头戏root了，主要代码都在\core-android-master\RCSAndroid\jni 这个目录下， 上来就能看到” exploit_list.c”这个霸气的文件，里面可以调用各种exp来获取root权限：
![](https://pic3.zhimg.com/50/5d1e1e2c4c9ea68505178c48d2b0e626_hd.jpg)
除此之外，core-android-master\RCSAndroid\jni\selinux_exploit还有绕过 selinux enforcing模式的exploit。
4 core-android-native-master.zip中有更加详细的root项目代码和说明，在”legacy_native”文件夹中： Suidext中包含了所有的shell。Local2root中包含了<=4.1版本的root exp。在”selinux_native”文件夹中，”Put_user_exploit”: 包含了 put_user calls的exp。”Kernel_waiter_exploit”包含了towelroot的exp。Suidext包含了新的shell。使用” build.sh”编译完后的exp都在”bin”目录下（这些exp是可以干掉android 5.0 selinux的）。其他的文件请参考目录下的README.txt。因为是意大利语，请使用Google自行翻译一下。
# 0x03 iOS & Mac OS
1 “core-ios-master.zip”里面的” core”文件夹中保存了RCS的主要代码。主要是利用dylib注入对用户输入，GPS，屏幕等信息进行监控。
![](https://pic4.zhimg.com/50/deb28f0312bfed30c3f7ce4cf0966aa3_hd.jpg)
”ios-newsstand-app”文件夹应该是另一个ios应用的源码。看代码大概是替换ios系统的输入法，然后进行键盘记录，大概是用来攻击没有越狱的机器吧。”Keybreak”文件夹是用来破解手机锁屏密码的，里面有lockdownd remote exploit的源码。”ios-install-win32”和” ios-install-osx”文件夹里是windows和mac os下来给iPhone或者iPad装应用的工具。此外HT还拥有一个iOS enterprise帐号可以用来发布enpublic app: “UID=DE9J4B8GTF, CN=iPhone Distribution: HT srl, OU=DE9J4B8GTF, O=HT srl, C=IT”。关于enpublic app的危害，可以参考我之前的文章或论文。
2 “vector-ipa-master.zip”里面应该是另一个ios木马的源码，这个木马并不是应用，貌似是一个底层网络代理，可以用来监控或者控制系统的网络流量。
3 “core-macos-master.zip”的”core-macos-master\core”的文件夹中保存了mac os RCS的源码，其实就是mac os木马了，和windows的木马非常相似。
# 0x04 Windows Phone & symbian & blackberry
1 core-winphone-master.zip是Windows Phone的RCS木马。据说在WP设备上实现“激活追踪”是利用了系统中的一个0day，允许第三方代码程序像受信任程序一样执行。该RCS还可以获取联系人、日历、通话、地理位置、短信、传感器状态状态等信息。程序ID为：11B69356-6C6D-475D-8655-D29B240D96C8。
![](https://pic3.zhimg.com/50/9426b726144b47626ec6e8ce7794f6ca_hd.jpg)
2 core-blackberry-master.zip和core-symbian-master.zip分别是黑莓和塞班的RCS系统。
# 0x05 Fuzzer
- 
fuzzer-windows-master.zip主要保存了windows下的fuzzer源码。里面有针对IE和字体的Fuzzer测试系统。
- 
fuzzer-windows-master.zip主要保存了android下的fuzzer源码。里面有针对jpg，sms和system call的Fuzzer测试系统。Trinity主要是用来做system call fuzzer的，比如说binder使用的ioctl()系统调用。
![](https://pic4.zhimg.com/50/3c4340b8abe2594e9f3035ab47a98c9b_hd.jpg)
# 0x06病毒查杀检测
test-av-master.zip是第一代产品。test-av2-master.zip是第二代产品。HT给他们起名叫AVMonitor。这个系统主要使用来做查杀检测，用来保证自己的产品可以通过检测。test-av2-master.zip\test-av2-master\doc\AVTEST Box.xlsx保存了他们使用的杀毒软件的列表和序列号。
![](https://pic3.zhimg.com/50/845c95353d3b2849d645de1089476a5e_hd.jpg)
在”test-av2-master\doc\whiteboard”文件夹中甚至有他们开会的白板照。
![](https://pic4.zhimg.com/50/9691283966b4bb49d09a8f0f7961c4a3_hd.jpg)
# 0x07 Exploit & 0day
vector-exploit-master.zip文件又是第二波高潮的开始，首先在里面你可以找到两个flash的exp: 一个是Flash的0day : ActionScript ByteArray Buffer Use After Free，另一个是Nicolas Joly在Pwn2Own 2015大赛中使用的CVE-2015-0349。为了在IE和Chrome上绕过其沙盒机制完全控制用户系统，Hacking Team还利用了一个Windows中的内核驱动： Adobe Font Driver(atmfd.dll)中存在的一处字体0day漏洞，实现权限提升并绕过沙盒机制。该0day漏洞可以用于WindowsXP~Windows 8.1系统，X86和X64平台都受影响。数字公司已经在很多人种子还没下完的时候就写出了分析报告：[Hacking Team攻击代码分析](http://link.zhihu.com/?target=http%3A//drops.wooyun.org/papers/6968)，有兴趣的读者可以前去围观。
![](https://pic3.zhimg.com/50/078e140d665f3126422f103f14c4415e_hd.jpg)
除了flash的两个exp和font 0day外，在vector-exploit-master\src\ht-webkit-Android4-src目录下还有一个Android Browser exploit，在用android brower浏览一个网页后就可以在目标机器上安装上目标apk。该漏洞会影响Android 4.0.*到4.3.*版本的手机。粗略看了一下源码，利用过程十分复杂，exp的利用至少有四个stage，还用到了information leak，heap spray等技术。PS:在vector-exploit-master\src\ht-webkit-Android4-src\docs中有公司开会的时候拍的exp图解。
![](https://pic1.zhimg.com/50/ca0d4c1c12371e4a9e55387e3afafe08_hd.jpg)![](https://pic1.zhimg.com/50/00626933a4a97e1b0266a72a739d49cc_hd.jpg)
# 0x08 其他
- GeoTrust-master Signing Keys.zip 保存了HT的GeoTrust证书。
- [http://ht.transparencytoolkit.org/audio/](http://link.zhihu.com/?target=http%3A//ht.transparencytoolkit.org/audio/) 里有大量的录音。
- HT在自己家的产品中留下了SQL后门，方便他们随时查询。[http://ht.transparencytoolkit.org/rcs-dev%5cshare/HOME/ALoR/htdocs/conf.php](http://link.zhihu.com/?target=http%3A//ht.transparencytoolkit.org/rcs-dev%255cshare/HOME/ALoR/htdocs/conf.php)
- 
虚拟机保护壳VMProtect Professional的很多正版key泄漏
[https://ht.transparencytoolkit.org/rcs-dev%5cshare/HOME/ALoR/VMProtect.key](http://link.zhihu.com/?target=https%3A//ht.transparencytoolkit.org/rcs-dev%255cshare/HOME/ALoR/VMProtect.key)[https://ht.transparencytoolkit.org/rcs-dev%5cshare/HOME/Ivan/vmprotect/](http://link.zhihu.com/?target=https%3A//ht.transparencytoolkit.org/rcs-dev%255cshare/HOME/Ivan/vmprotect/)
# 0x09 八卦
1 Phineas Fisher号称自己黑了gamma和HT。HT的twitter还转发了这条消息。。。
![](https://pic3.zhimg.com/50/4f48f389465e7f3572649ab36fe44fba_hd.jpg)
2 HT的密码都特别简单，不被黑才怪。
![](https://pic4.zhimg.com/50/0e6da1a2c4c56ea0ef4058bdceb5b5f7_hd.jpg)
3 [http://ht.transparencytoolkit.org/c.pozzi/Desktop/you.txt](http://link.zhihu.com/?target=http%3A//ht.transparencytoolkit.org/c.pozzi/Desktop/you.txt) 你懂的。。。(from @youstar)
![](https://pic1.zhimg.com/50/86f8d72260ad04f8ea2c8a96e293085c_hd.jpg)
# 0x10 未完待续
由于泄漏的资料实在太过庞大，本文还有很多的内容没有覆盖到。因此我们在随后的几天还会继续跟进这个事件，并更新我们的文章，欢迎大家回来继续阅读。

