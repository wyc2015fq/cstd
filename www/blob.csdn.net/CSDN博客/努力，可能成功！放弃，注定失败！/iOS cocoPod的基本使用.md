# iOS  cocoPod的基本使用 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年01月19日 11:55:03[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1639

  虽然网上关于CocoaPods安装教程多不胜数,但是我在安装的过程中还是出现了很多错误,所以大家可以照下来步骤装一下,我相信会很好用.
前言
在iOS项目中使用第三方类库可以说是非常常见的事，但是要正确地配置他们有时候是非常繁琐的事情，幸运的是CocoaPods是一个很好的解决方案。
什么是CocoaPods
CocoaPods是OS
 X和iOS下的一个第三类库管理工具，通过CocoaPods工具我们可以为项目添加被称为“Pods”的依赖库（这些类库必须是CocoaPods本身所支持的），并且可以轻松管理其版本。
Cocoapods意义体现在两个方面。第一，在引入第三方库时它可以自动为我们完成各种各样的配置，包括配置编译阶段、[连接器](http://cpro.baidu.com/cpro/ui/uijs.php?adclass=0&app_id=0&c=news&cf=1001&ch=0&di=128&fv=20&is_app=0&jk=8686810bd6887756&k=%C1%AC%BD%D3%C6%F7&k0=%C1%AC%BD%D3%C6%F7&kdi0=0&luki=4&mcpm=0&n=10&p=baidu&q=65035100_cpr&rb=0&rs=1&seller_id=1&sid=567788d6b818686&ssp2=1&stid=9&t=tpclicked3_hc&td=1836545&tu=u1836545&u=http%3A%2F%2Fwww.bubuko.com%2Finfodetail-425274.html&urlid=0)选项、甚至是ARC环境下的-fno-objc-[arc](http://cpro.baidu.com/cpro/ui/uijs.php?adclass=0&app_id=0&c=news&cf=1001&ch=0&di=128&fv=20&is_app=0&jk=8686810bd6887756&k=arc&k0=arc&kdi0=0&luki=3&mcpm=0&n=10&p=baidu&q=65035100_cpr&rb=0&rs=1&seller_id=1&sid=567788d6b818686&ssp2=1&stid=9&t=tpclicked3_hc&td=1836545&tu=u1836545&u=http%3A%2F%2Fwww.bubuko.com%2Finfodetail-425274.html&urlid=0)配置等。第二，使用CocoaPods可以很方便地查找新的第三方库，这些类库是比较“标准的”，而不是网上随便找到的，这样可以让我们找到真正好用的类库。
接下来我们将介绍CocoaPods的使用。
CocoaPods的核心组件
CocoaPods是用Ruby写的，并划分成了若干个Gem包。
CocoaPods在解析执行过程中最重要的几个包的路径分别是：CocoaPods/CocoaPods、CocoaPods/Core和 CocoaPods/Xcodeproj。
CocoaPods / CocoaPod：这是面向用户的组件，每当执行一个pod命令时，这个[组件](http://cpro.baidu.com/cpro/ui/uijs.php?adclass=0&app_id=0&c=news&cf=1001&ch=0&di=128&fv=20&is_app=0&jk=8686810bd6887756&k=%D7%E9%BC%FE&k0=%D7%E9%BC%FE&kdi0=0&luki=10&mcpm=0&n=10&p=baidu&q=65035100_cpr&rb=0&rs=1&seller_id=1&sid=567788d6b818686&ssp2=1&stid=9&t=tpclicked3_hc&td=1836545&tu=u1836545&u=http%3A%2F%2Fwww.bubuko.com%2Finfodetail-425274.html&urlid=0)将被激活。它包括了所有实用CocoaPods的功能，并且还能调用其他gem包来执行任务。
CocoaPods / Core：Core
 gem提供了与CocoaPods相关的文件（主要是podfile和podspecs）的处理。
Podfile：该文件用于配置项目所需要的第三方库，它可以被高度定制。本文中我们主要在这里做动作。
Podspec：该文件描述了一个库将怎样被添加进工程中。.podspec文件可以标识该第三方库所需要的[源码](http://cpro.baidu.com/cpro/ui/uijs.php?adclass=0&app_id=0&c=news&cf=1001&ch=0&di=128&fv=20&is_app=0&jk=8686810bd6887756&k=%D4%B4%C2%EB&k0=%D4%B4%C2%EB&kdi0=0&luki=6&mcpm=0&n=10&p=baidu&q=65035100_cpr&rb=0&rs=1&seller_id=1&sid=567788d6b818686&ssp2=1&stid=9&t=tpclicked3_hc&td=1836545&tu=u1836545&u=http%3A%2F%2Fwww.bubuko.com%2Finfodetail-425274.html&urlid=0)文件、依赖库、编译选项，以及其他第三方库需要的配置。
CocoaPods / Xcodeproj：这个包负责处理工程文件，它能创建以及修改.xcodeproj文件和.xcworkspace文件。它也可以作为一个独立的包使用，当你要编写修改项目文件的脚本时，可以考虑使用CocoaPods/Xcodeproj。
Cocoapods安装步骤
1、升级Ruby环境
终端输入：$gem update --system
此时会出现
ERROR:  While executing gem ... (Gem::FilePermissionError)
    You don‘t have write permissions for the /Library/Ruby/Gems/2.0.0 directory.
zijingMacBook-Pro:~ shiyunlei$ sudo gem update —system
这个是因为你没有权限去更新Ruby
这个时候你可以在终端使用：$sudo gem update —system来进行升级
![bubuko.com,布布扣](https://img-blog.csdn.net/20141022164653840?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenVveW91MTMxNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这个时候表示Ruby升级完成。
2、安装CocoaPods时我们要访问cocoapods.org，该网站可能被我们大天朝墙了，这样下载安装可能会是龟速，幸好我们可以用淘宝的Ruby镜像来访问该网站，方法如下：
1、gem sources --remove[https://rubygems.org/](https://rubygems.org/)
2、gem sources -a[http://ruby.taobao.org/](http://ruby.taobao.org/)
可以使用下面的命令(红色部分)查看是否更换镜像成功（如下图所示）：
$ gem sources -l
![bubuko.com,布布扣](https://img-blog.csdn.net/20141022164511875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenVveW91MTMxNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
3、安装Cocoapods，在终端输入命令
$sudo gem install cocoapods
这个时候会提示你输入密码，注：在终端输入密码不会有提示，[光标](http://cpro.baidu.com/cpro/ui/uijs.php?adclass=0&app_id=0&c=news&cf=1001&ch=0&di=128&fv=20&is_app=0&jk=8686810bd6887756&k=%B9%E2%B1%EA&k0=%B9%E2%B1%EA&kdi0=0&luki=1&mcpm=0&n=10&p=baidu&q=65035100_cpr&rb=0&rs=1&seller_id=1&sid=567788d6b818686&ssp2=1&stid=9&t=tpclicked3_hc&td=1836545&tu=u1836545&u=http%3A%2F%2Fwww.bubuko.com%2Finfodetail-425274.html&urlid=0)也不会移动，一定要注意
开始安装，如下图：PS:这个过程可能花费时间比较长
![bubuko.com,布布扣](https://img-blog.csdn.net/20141022164750734?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenVveW91MTMxNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
安装成功如下：
![bubuko.com,布布扣](https://img-blog.csdn.net/20141022165250144?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenVveW91MTMxNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
4、使用search命令搜索类库名： 
$pod se[arc](http://cpro.baidu.com/cpro/ui/uijs.php?adclass=0&app_id=0&c=news&cf=1001&ch=0&di=128&fv=20&is_app=0&jk=8686810bd6887756&k=arc&k0=arc&kdi0=0&luki=3&mcpm=0&n=10&p=baidu&q=65035100_cpr&rb=0&rs=1&seller_id=1&sid=567788d6b818686&ssp2=1&stid=9&t=tpclicked3_hc&td=1836545&tu=u1836545&u=http%3A%2F%2Fwww.bubuko.com%2Finfodetail-425274.html&urlid=0)h
 AFNetworking
如果CocoaPods支持，将会输出搜索到的所有类库版本和信息，以及在Podfile中配置的写法，终端返回信息如下：
-> AFNetworking (2.3.1)
   A delightful iOS and OS X networking framework.
pod ‘AFNetworking‘, ‘~> 2.3.1’（这个语句是需要在编辑Podfile的时候输入的命令，即是如何安装下载相关类库的指令）
   - Homepage: https://github.com/AFNetworking/AFNetworking
   - Source:   https://github.com/AFNetworking/AFNetworking.git
   - Versions: 2.3.1, 2.3.0, 2.2.4, 2.2.3, 2.2.2, 2.2.1, 2.2.0, 2.1.0,
   2.0.3, 2.0.2, 2.0.1, 2.0.0, 2.0.0-RC3, 2.0.0-RC2, 2.0.0-RC1, 1.3.4, 1.3.3,
   1.3.2, 1.3.1, 1.3.0, 1.2.1, 1.2.0, 1.1.0, 1.0.1, 1.0, 1.0RC3, 1.0RC2, 1.0RC1,
   0.10.1, 0.10.0, 0.9.2, 0.9.1, 0.9.0, 0.7.0, 0.5.1 [[master](http://cpro.baidu.com/cpro/ui/uijs.php?adclass=0&app_id=0&c=news&cf=1001&ch=0&di=128&fv=20&is_app=0&jk=8686810bd6887756&k=master&k0=master&kdi0=0&luki=8&mcpm=0&n=10&p=baidu&q=65035100_cpr&rb=0&rs=1&seller_id=1&sid=567788d6b818686&ssp2=1&stid=9&t=tpclicked3_hc&td=1836545&tu=u1836545&u=http%3A%2F%2Fwww.bubuko.com%2Finfodetail-425274.html&urlid=0) repo]
   - Sub specs:   - AFNetworking/Serialization (2.3.1)   -
   AFNetworking/Security (2.3.1)   - AFNetworking/Reachability (2.3.1)   -
   AFNetworking/NSURLConnection (2.3.1)   - AFNetworking/NSURLSession (2.3.1)
   - AFNetworking/UIKit (2.3.1)
-> AFNetworking+AutoRetry (0.0.5)
   Auto Retries for AFNetworking requests
   pod ‘AFNetworking+AutoRetry‘, ‘~> 0.0.5‘
   - Homepage: https://github.com/shaioz/AFNetworking-AutoRetry
   - Source:   https://github.com/shaioz/AFNetworking-AutoRetry.git
   - Versions: 0.0.5, 0.0.4, 0.0.3, 0.0.2, 0.0.1 [[master](http://cpro.baidu.com/cpro/ui/uijs.php?adclass=0&app_id=0&c=news&cf=1001&ch=0&di=128&fv=20&is_app=0&jk=8686810bd6887756&k=master&k0=master&kdi0=0&luki=8&mcpm=0&n=10&p=baidu&q=65035100_cpr&rb=0&rs=1&seller_id=1&sid=567788d6b818686&ssp2=1&stid=9&t=tpclicked3_hc&td=1836545&tu=u1836545&u=http%3A%2F%2Fwww.bubuko.com%2Finfodetail-425274.html&urlid=0) repo]
5、需要在工程中创建一个Podfile文件，
使用命令：
$cd /Users/shiyunlei/Desktop/CocoapodsSample(进入工程目录，cd后面的是工程的路径)
$[touch](http://cpro.baidu.com/cpro/ui/uijs.php?adclass=0&app_id=0&c=news&cf=1001&ch=0&di=128&fv=20&is_app=0&jk=8686810bd6887756&k=touch&k0=touch&kdi0=0&luki=2&mcpm=0&n=10&p=baidu&q=65035100_cpr&rb=0&rs=1&seller_id=1&sid=567788d6b818686&ssp2=1&stid=9&t=tpclicked3_hc&td=1836545&tu=u1836545&u=http%3A%2F%2Fwww.bubuko.com%2Finfodetail-425274.html&urlid=0) Podfile（创建Podfile文件）
![bubuko.com,布布扣](https://img-blog.csdn.net/20141022165405477?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenVveW91MTMxNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后使用vim编辑Podfile文件，使用命令：
$[vim](http://cpro.baidu.com/cpro/ui/uijs.php?adclass=0&app_id=0&c=news&cf=1001&ch=0&di=128&fv=20&is_app=0&jk=8686810bd6887756&k=vim&k0=vim&kdi0=0&luki=7&mcpm=0&n=10&p=baidu&q=65035100_cpr&rb=0&rs=1&seller_id=1&sid=567788d6b818686&ssp2=1&stid=9&t=tpclicked3_hc&td=1836545&tu=u1836545&u=http%3A%2F%2Fwww.bubuko.com%2Finfodetail-425274.html&urlid=0) Podfile
进入如下界面
![bubuko.com,布布扣](https://img-blog.csdn.net/20141022165127171?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenVveW91MTMxNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后紧接着按键盘上的“i”键，变成如下状态，这个时候可以对Podfile文件进行编辑：
$platform:ios, ‘7.0‘
$ pod ‘AFNetworking‘, ‘~> 2.3.1‘
在编辑完成后按“esc”，
![bubuko.com,布布扣](https://img-blog.csdn.net/20141022165317765?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenVveW91MTMxNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
在按“:”,这个时候输入wq,点击回车，保存并退出。
![bubuko.com,布布扣](https://img-blog.csdn.net/20141023092652824?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenVveW91MTMxNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这个时候打开Podfile文件就会看到里面添加了刚才在终端输入的一行语句，如下图
![bubuko.com,布布扣](https://img-blog.csdn.net/20141023092856673?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenVveW91MTMxNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后在终端输入命令安装相应的第三方类库
$pod install
![bubuko.com,布布扣](https://img-blog.csdn.net/20141023092656546?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenVveW91MTMxNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
因为这个过程需要下载相应的类库，所以时间和网速有关。
安装成功之后截图如下：
![bubuko.com,布布扣](https://img-blog.csdn.net/20141023093149131?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenVveW91MTMxNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这个时候会看到之后打开工程都需要从类型为工程名.xcworkspace文件打开。
![bubuko.com,布布扣](https://img-blog.csdn.net/20141023093313340?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvenVveW91MTMxNA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
