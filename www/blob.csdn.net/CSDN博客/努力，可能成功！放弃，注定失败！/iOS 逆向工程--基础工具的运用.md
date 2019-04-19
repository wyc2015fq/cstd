# iOS 逆向工程--基础工具的运用 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年10月19日 10:57:42[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：95
IOS 逆向工程 APP破解
## 提要 ：
经过一个月断断续续对**APP逆向工程**的研究，这篇文章算是对逆向工程知识迈出了第一步，如果你对**iOS逆向工程**感兴趣的话，建议你还是按照这篇文章的思路，在你的越狱手机上完整的实践一遍，这样你就会对那些基础的越狱工具有了一个浅显的了解。
如果不喜欢这个排版，可以访问我的另一个博客网址
[作业部落博客地址](https://www.zybuluo.com/CoDancer/note/1105833)
## 准备工作
> - **越狱手机（例如我的：iPhone 5s，系统8.3）**
- **越狱手机上的Cydia中安装Cycript，OpenSSH**
- **电脑端上安装iFunBox，xCode，Hopper Disassembler,Sublime Text**
- **电脑终端命令行中安装Class Dump,Theos,Dumpdecrypted**
## 工具作用介绍
### OpenSSH：
```
Mac连接手机，通过Mac的终端命令操作手机。
```
### Cycript
```
获取手机中某一个运行的进程，并可以获取APP进程后用oc代码获取反编译APP的Documents目录路径。
```
### iFunBox
```
Mac端的连接手机后的图形界面，比较方便的把砸壳后的文件直接导出到Mac上。
```
### xCode
```
这个就不作说明了
```
### Hopper Disassembler
```
这个是进行反汇编的工具，可以反汇编、反编译并且调试你的应用，我们可以通过这个工具查看伪代码的方法名和变量，还原函数体的具体逻辑。
```
### Class Dump
```
对APP可执行文件获取其头文件，这样我们可以根据头文件中暴露的方法名去猜测其APP的编写逻辑。然后利用上述的反编译工具对某一文件进行函数的具体编写逻辑。
```
### Theos
```
创建一个Tweak工程，通过定制工程文件，再通过指定的bundle id，编译+打包+安装，从而向手机上的APP注入代码，进而实现对某个APP中的功能的破解（例如会员去广告功能，抢红包功能，过滤好友列表功能）
```
### Dumpdecrypted
```
俗称：“砸壳”，dumpdecrypted是个出色的app脱壳开源工具，它的原理是：将应用程序运行起来（iOS系统会先解密程序再启动），然后将内存中的解密结果dump写入文件中，得到一个新的可执行程序文件。
```
### Sublime Text
```
打开后面要用到的Makefile文件
```
说了这些工具的用途之后，我们接下来开始我们的破解工作。
## 需要掌握的命令行
> - 查看当前xcode的sdk路径：xcrun --sdk iphoneos --show-sdk-path
- 修改当前xcode的默认sdk： sudo xcode-select -s /Applications/Xcode.app/Contents/Developer
- 文件夹中显示和隐藏文件：defaults write com.apple.finder AppleShowAllFiles -bool false/true之后得重新启动Finder
- ps -e 获取手机所有进程
- ps -A|grep mobile 抓取手机上运行的APP进程
- cycript -p WeChat
## 实现目标
我们打开微信APP后（在App Store中下载的软件），自动弹出一个警告框。
![](https://upload-images.jianshu.io/upload_images/1376750-5279119f117a778e.PNG?imageMogr2/auto-orient/strip%7CimageView2/2/w/320/format/webp)
IMG_0014.PNG
当然这只是一个很简单的一步，我之前也是参考前人的文章。
[参考文章iOS逆向工程](https://pandara.xyz/2016/08/13/fake_wechat_location/)
不过这篇文章是16年的，在实现的过程中还是碰到了几个坑。
> - 获取可执行程序文件WeChat.decrypted出错
- 在创建 Theos 工程后编译（make）操作总是提示
**ld: framework not found CydiaSubstrate**
- 在make成功后 make package时提示：
**Can't locate IO/Compress/[Lzma.pm](http://Lzma.pm) in @INC (you may need to install the IO::Compress::Lzma module) (@INC contains: /Library/Perl/5.18/darwin-thread-multi-2level /Library/Perl/5.18 /Network/Library/Perl/5.18/darwin-thread-multi-2level /Network/Library/Perl/5.18 /Library/Perl/Updates/5.18.2 /System/Library/Perl/5.18/darwin-thread-multi-2level /System/Library/Perl/5.18 /System/Library/Perl/Extras/5.18/darwin-thread-multi-2level /System/Library/Perl/Extras/5.18 .) at /opt/theos/bin/[dm.pl](http://dm.pl) line 12.	BEGIN failed--compilation aborted at /opt/theos/bin/dm.pl line 12.	make: [internal-package] Error 2**
## 实现流程
### 1.生成砸壳用的dumpdecrypted.dylib
```
OpenSSH命令的使用：
前提：保证手机和Mac在同一个网段上。
登录成功后就可以使用终端命行操作iPhone
退出登录命令是exit
```
![](https://upload-images.jianshu.io/upload_images/1376750-ec1f053685767521.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/714/format/webp)
4F102D71-8902-447F-8852-EC1480146C13.png
![](https://upload-images.jianshu.io/upload_images/1376750-8975c7780044c254.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
1AB41217-5D90-4AE2-B698-B5B17C0CB552.png
![](https://upload-images.jianshu.io/upload_images/1376750-73c0961596d371fe.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
C20F0342-9CA8-42FA-9A5D-19C9D4D182B7.png
```
ps -e 获取手机中所有的当前运行的进程
ps -A|grep mobile 抓取手机上运行的APP进程
```
![](https://upload-images.jianshu.io/upload_images/1376750-97b49a044273864e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/990/format/webp)
1CB6FE71-B7B9-468A-8041-39D2B95530E3.png
```
这儿有两个路径
/var/mobile/Containers/Bundle/Application/下的是我们要找的可执行文件
/var/mobile/Containers/Data/Application
得到Documents路径,也就是APP所在的沙盒路径，而我们之后要生成解密后文件的命令行得需要在APP真正的路径下执行。
Cycript命令行：
沙盒路径的获取：  [[NSFileManager defaultManager] URLsForDirectory:NSDocumentDirectory inDomains:NSUserDomainMask][0]
```
![](https://upload-images.jianshu.io/upload_images/1376750-1232577ecd11d172.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
18084829-010C-4AF6-A70F-60AEF896EFFD.png
```
踩坑点：当命令行中输入cycript -p WeChat后并没有出现下面的cy#这一行时，你最好用数据线连接手机（这样你会发现输入命令不卡），之后打开微信程序后，这时候cy#这一行就出现了，同样获取沙盒路径也得打开程序，不然不会出现你想要的结果,退出cyscript命令环境control+z。
这一阶段的目的：制作砸壳的“锤头”，因为不同iOS系统下的dumpdecrypted.dylib（锤头）是不同的。
```
**如果你的手机系统是8.0的可以直接下载我上传GitHub上的文件，那么你可不用看生成“锤头”的部分。**
[dumpdecrypted.dylib github地址](https://github.com/CoDancer/dumpdecrypted.dylib)
开始生成“锤头”
[dumpdecrypted源码下载地址](https://link.jianshu.com/?t=https://github.com/stefanesser/dumpdecrypted/archive/master.zip)
```
用Sublime Text打开下载的dumpdecrypted文件夹里面的Makefile，配置里面的参数
GCC_UNIVERSAL=$(GCC_BASE) -arch armv7 -arch armv7s -arch arm64
如果是7.0以下的设备我们还需要改些东西，我们要将上图的第二行修改为
GCC_UNIVERSAL=$(GCC_BASE) -arch armv7 -arch armv7s
还要将dumpdecrypted文件夹里的dumpdecrypted.c文件的第76行
if (lc->cmd == LC_ENCRYPTION_INFO || lc->cmd ==LC_ENCRYPTION_INFO_64)
改为
if(lc->cmd == LC_ENCRYPTION_INFO)
到相应的目录下（cd dirc）,
```
**执行命令行：make**
```
完成后dumpdecrypted.dylib文件出现在当前目录下，这个就是我们砸壳用的锤头。
踩坑点：如果你的手机是ios8.3的系统，就需要下载xcode6.3的版本，这儿是下载链接，并且需要切换你的默认xcode路径，首先看看你默认的xcode是iosSDK是多少的。
命令行：xcrun --sdk iphoneos --show-sdk-path  //查看iosSDK版本路径
sudo xcode-select -s /Applications/Xcode.app/Contents/Developer
修改默认的iosSDK
```
![](https://upload-images.jianshu.io/upload_images/1376750-ee13039ac714fd62.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
63B4B96A-B164-430D-B4D3-714E208F3767.png
[xcode版本下载](https://link.jianshu.com/?t=https://developer.apple.com/downloads/index.action)
获取“锤头”完成
**砸壳获取解密后的可执行文件**
> - 将dumpdecrypted.dylib拷贝到Documents路径下
> 
1 命令行：scp `dumpdecrypted.dylib路径` root@设备IP:`Documents路径`;
2 利用iFunBox
- cd Documents路径 //到APP的Documents路径下执行命令
- 
DYLD_INSERT_LIBRARIES=dumpdecrypted.dylib 可执行文件路径
	建议用第二种方法
![](https://upload-images.jianshu.io/upload_images/1376750-824dcd7ca83caf70.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
DE6B36FA-2578-4C5F-BA36-15DCC046B206.png
```
利用iFunBox到APP的Documents路径
```
![](https://upload-images.jianshu.io/upload_images/1376750-4b56aaa9c6285893.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
4B935D43-042B-402C-900C-27E072DD3E47.png
```
命令行成功，APP的Documents路径会生成APP的解密后的可执行文件
```
![](https://upload-images.jianshu.io/upload_images/1376750-69c01cfb6e1a4521.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
55AC1559-2006-4C70-9E2C-3187C53E72CB.png
```
最后我们把.decrypted文件拷到Mac上
```
### 2.反编译静态分析（Hopper Disassembler登场）
```
把.decrypted丢进去，你会看到很炫酷的界面。至于这个软件的下载，
```
[Hopper Disassembler的破解地址](http://www.sdifen.com/hopperdisassembler408.html)
```
具体的操作你可以看看这篇文章，这个是在比较小的demo上去介绍Hopper Disassembler的原理的。
```
[Hopper Disassembler的反编译的基础demo](https://www.jianshu.com/p/33e40af6e328)
```
如果你把上篇简书中的文章照着实现了一遍的话，你就会对Hopper Disassembler有了自己的了解了。
```
### 3.Tweak与Theos
```
简介：tweak的实质就是ios平台的动态库。IOS平台上有两种形势的动态库，dylib与framework。Framework这种开发者用的比较多，而dylib这种就相对比较少一点，比如libsqlite.dylib，libz.dylib等。而tweak用的正是dylib这种形势的动态库。我们可以在/Library/MobileSubstrate/DynamicLibraries目录下查看手机上存在着的所有tweak。这个目录下除dylib外还存在着plist与bundle两种格式的文件，plist文件是用来标识该tweak的作用范围，而bundle是tweak所用到的资源文件。
    与正常的APP开发使用xcode不同，Tweak的开发环境是theos或者iosopendev。iosopendev是在theos的基础上实现的基于xcode的开发环境。而theos是一种命令行式的开发编译环境，与c/c++的命令行编译形式很相象。
    这里的概述摘自简友的这篇文章，感兴趣的可以看看，个人认为总结的还是很全面的。
```
[Tweak与Theos的基础介绍](https://www.jianshu.com/p/b3113ff64688)
```
Mac的环境下载，网上有很多关于Theos的下载及配置的博客，但有些是很早的资料，这里我是参考这篇文章的。
```
[Theos的下载及安装](https://blog.sunnyyoung.net/theos-an-zhuang-yu-pei-zhi/)
```
创建theos工程(package name 只能包含小写的字母还有数字)
```
![](https://upload-images.jianshu.io/upload_images/1376750-c69b2f94394f5165.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
7A038052-223E-4E3D-BD84-2F9BBC97A4A0.png
```
踩坑点：nic.pl命令行并没有反应，这是你没有配置环境路径
export THEOS=/opt/theos
export PATH=/opt/theos/bin/:$PATH
```
**主要需要注意的地方**
> - Theos 要求我们输入 MobileSubstrate Bundle filter，也就是 tweak 作用对象的 bundle identifier，这里我们填上微信的 id(com.tencent.xin)，可以在解压后的 ipa 包中的 plist 文件中找到。
- 最后我们需要输入指定 tweak 安装完成之后需要重启的应用，以 bundle identifier 来表示，这里还是填上微信。
到这里工程便创建完成了，会在当前目录下生成工程文件夹。
接下来就是定制工程文件了
编辑 MakeFile
![](https://upload-images.jianshu.io/upload_images/1376750-6dcbc7cb4c561baf.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
E828DE69-F14C-42FF-9183-72028E97A8A1.png
编写 tweak 源码
![](https://upload-images.jianshu.io/upload_images/1376750-e940620f6ea7e49f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
C0C8E875-9B75-4D5A-8F27-0DE7C3B1384E.png
```
方便懒人
%hook MicroMessengerAppDelegate
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
UIAlertView *alertView = [[UIAlertView alloc] initWithTitle:@"成功注入代码" message:@"IOS 逆向工程" delegate:nil cancelButtonTitle:@"Cancel" otherButtonTitles:nil];
[alertView show];
#pragma clang diagnostic pop
return %orig;
}
%end
```
定制完毕
接下来就是激动人心的make操作了
![](https://upload-images.jianshu.io/upload_images/1376750-58737c168bda2289.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
90878361-B368-41AA-B4D0-C5D1153936A7.png
```
这是说找不到ios8.3的SDK，那么这时候你得切换默认的iosSDK路径了。
sudo xcode-select -s /Applications/Xcode6.3.app/Contents/Developer
```
![](https://upload-images.jianshu.io/upload_images/1376750-0cedfcf2d30fb053.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
C2D8F829-CB8D-4828-8D9F-7CED7DDEBE17.png
```
这是什么👻！！！怎么办，很慌。
找各种帖子资料
```
[资料1](https://github.com/theos/theos/issues/210)
[资料2](https://github.com/theos/theos/issues/167)
最后的解决办法(MakeFile文件中配置)
**TARGET = iphone::9.2:8.3**
```
Clang actually supports it as of Xcode 7.0 (iOS SDK 9.0), but enable it only if you're using iOS SDK 8.4+. I wasn't too sure how it would work out when I implemented it so I restrained it to the latest SDK at the time. It certainly could be dropped back to 9.0+.
就是说Clang在ios9.0上才支持，所以我们的TARGET是包含9.0的，当然你手机如果是9.0的系统就不会报错了。（哦咪头佛）
```
make一下(双手合十，赐予我力量吧！！！)
![](https://upload-images.jianshu.io/upload_images/1376750-0a0800a4eee94486.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
F32D321E-4C31-4F41-96C0-979169066F26.png
这样我们离成功又进了一步了
```
题外话：笔者在解决这个问题时，可谓生不如死啊，查阅各种资料，建议各位还是在Google下查阅资料，中间因为这个问题还搁置了对逆向工程的研究。
```
之后make package
```
Can't locate IO/Compress/Lzma.pm in @INC (you may need to install the IO::Compress::Lzma module)
可能出现这个问题，这时候执行
sudo cpan -I IO::Compress::Lzma
```
![](https://upload-images.jianshu.io/upload_images/1376750-5850dfc50ec5a022.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
A98354BE-F338-41C2-AE99-99BDE97F3D71.png
**安装**
```
最后，要将这个 deb 文件安装到 iOS 中去，采用了命令行的安装方法。
```
![](https://upload-images.jianshu.io/upload_images/1376750-da0720bfe388d054.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
05AB59E5-4DA0-4707-99B9-6CF29A3BA68E.png
```
这个错误是因为我误将 Makefile 中需要 kill 的进程名称填成了 bundle identifier，将它改成 WeChat 就好了：killall -9 WeChat
```
![](https://upload-images.jianshu.io/upload_images/1376750-ce2722ffa177f81e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
BF29F32E-239F-44B8-BEDC-43CF7FF2E8D2.png
```
打开手机的微信后，你会看到这个效果。
```
![](https://upload-images.jianshu.io/upload_images/1376750-5279119f117a778e.PNG?imageMogr2/auto-orient/strip%7CimageView2/2/w/320/format/webp)
IMG_0014.PNG
文章到此结束！！！
**特别说明：写这篇博客个人参考的前人博客**
> - [OpenSSH的基础用法](https://www.jianshu.com/p/5241669baa74)
- [iOS逆向工程介绍与实现](https://www.jianshu.com/p/4da57be23275)
- [如何制作“砸壳”的锤头](https://www.jianshu.com/p/14db1ac34bf1)
- [iOS逆向工程以及demo的实现](https://pandara.xyz/2016/08/13/fake_wechat_location/)
- [Hopper Disassembler的介绍](https://www.jianshu.com/p/33e40af6e328)
- [Tweak与Theos的介绍](https://www.jianshu.com/p/b3113ff64688)
作者：怀心逝水
链接：https://www.jianshu.com/p/38261a775997
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
