# iOS逆向Reveal查看任意app 的界面 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年07月04日 14:09:40[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：4344
> 
***参考书籍:iOS应用逆向工程第二版[调试其他应用界面](https://hurui.gitbooks.io/reveal-debug/content/viewOtherApp.html),很详细的教程本文是在[调试其他应用界面](https://hurui.gitbooks.io/reveal-debug/content/viewOtherApp.html)上补充的***
## 上面的资料两者结合看,结合看才会更容易看懂
# 一. 准备工作
通过前几章节的学习我们已经能够通过Reveal查看自己开发程序界面信息，但是从Xcode6开始苹果已经将Reveal查看自己开发程序界面信息的功能集成到了Xcode中， 只需要在程序运行起来之后点击Debug View hierarch按钮即可。
![](https://hurui.gitbooks.io/reveal-debug/content/images/Snip20150711_32.png)
在企业级开发中其实我们更多的是利用Reveal来学习其它应用程序的界面效果。要想使用Reveal查看其它应用程序的界面效果必须具备一个条件那就是**必须拥有一台越狱设备**。
1.越狱你的iOS设备：可以利用盘古、PP助手、太极等三方工具一键越狱。（笔者使用的就iOS版本就是iOS8.4，使用太极越狱，非广告……关于越狱的教程，太极的官网上已经贴出，无比简单，傻瓜式教程）,***iOS程序员越狱对手机无任何影响开发调试照常***
[PP助手](http://pro.25pp.com)
[盘古](http://www.pangu.io)
[太极](http://www.taig.com)
2.iOS设备越狱完成后，需要使用Cydia
安装两个软件：**OpenSSH**和**CydiaSubstrate**
安装OpenSSH，打开越狱后的设备上的Cydia程序，点击搜索输入OpenSSH，然后点击安装。（下面的配图是已经安装成功后搜索的显示画面）
![](https://hurui.gitbooks.io/reveal-debug/content/images/Snip20150711_33.png)
SSH是一种可以保证用户加密远程登录到系统的协议，OpenSSH是一个SSH的连接传输工具。
安装Cydia Substrate，打开越狱后的设备上的Cydia程序，点击搜索输入Cydia Substrate，然后点击安装。只有安装了CydiaSubstrate之后才会有MobileSubstrate目录（此目录必须存在才可以继续下面的步骤）。（下面的配图是已经安装成功后搜索的显示画面）
![](https://hurui.gitbooks.io/reveal-debug/content/images/Snip20150711_34.png)
MobileSubstrate是一个公共库，可以用来动态替换内存中的代码、数据等。基本上越狱机下比较有用的系统工具都需要这个库，是Cydia同一个作者维护的，可以放心装。
3.将libReveal.dylib上传到越狱后的设备的/Library/MobileSubstrate/DinamicLibraries
下检查工作：测试OpenSSH和Cydia Substrate是否安装成功
》上传libReveal.dylib文件必须使用到上面安装的OpenSSH工具。如果想要使用OpenSSH工具，**那么就必须将Reveal软件所在的mac设备与越狱后的iOS设备置于同一个局域网内**。
》首先来测试OpenSSH是否安装成功。获取越狱后的设备的局域网内的IP地址，打开“设置”->“WLAN”，连上局域网，然后再点击连接上的局域网查看iOS设备的IP地址。
![](https://hurui.gitbooks.io/reveal-debug/content/images/Snip20150711_37.png)
》打开mac上的终端工具(Terminal)，输入如下命令：ssh root@**iOS设备IP地址**。输入完成后回车，等待连接iOS设备。
![](https://hurui.gitbooks.io/reveal-debug/content/images/Snip20150711_39.png)
**iOS设备的IP地址**指的是越狱后的设备（iphone、ipad）的IP地址，也就是上个步骤中获取的IP地址，后面的教程中出现的地址仅仅是代表笔者测试时iOS设备的IP地址(192.168.2.2)，读者请一定要查看自己的iOS设备地址，并进行替换。
》接下来会提示你输入root用户的密码，iOS设备root用户的默认密码是**alpine**，在终端中输入密码时，终端不会显示你输入的字符，所以只需要输入**alpine**字符后回车就可以请求连接iOS设备了。（**这一步请保证你的iOS设备屏幕已经解锁，不然不会出现输入密码的提示！**）
![](https://hurui.gitbooks.io/reveal-debug/content/images/Snip20150711_40.png)
》如果出现~ root#字符就表示连接成功了。
![](https://hurui.gitbooks.io/reveal-debug/content/images/Snip20150711_41.png)
接下来测试Cydia Substrate 是否安装成功，上一步已经连接上iOS设备的终端中输入以下命令：cd /Library/MobleSubstrate/DinamicLibraries
，回车查看返回结果。如果出现-sh: cd: /Library/MobileSubstrate/DynamicLibraries: No such file or directory
~~~提示那就说明Cydia Substrate安装**不成功**，不成功请重新进入iOS设备上下载安装Cydia Substrate软件。~~**非也,有可能是安装成功的**
# 此处有重大更新更新内容
`cd: /Library/MobileSubstrate/DynamicLibraries: No such file or directory` 并不一定就是没安装好Cydia Substrate软件,使用`ls -l /Library`命令查看里面是否有**RHRevealLoader**
```cpp
```cpp
shujinokinakenkou:~ root# ls -l /Library
total 24
drwxrwxr-x 6 root admin  204 Feb 26  2015 Application Support
drwxrwxr-x 5 root admin  170 Feb 26  2015 Audio
drwxrwxrwt 2 root admin   68 Nov  8  2014 Caches
drwxrwxr-x 2 root admin   68 Nov  8  2014 Filesystems
drwxr-xr-x 3 root admin  102 Jul  2 01:21 Frameworks
drwxrwxr-x 2 root admin   68 Nov  8  2014 Internet Plug-Ins
lrwxr-xr-x 1 root admin   24 Feb 26  2015 Keychains -> ../private/var/Keychains
drwxr-xr-x 2 root wheel   68 Nov  8  2014 LaunchAgents
drwxr-xr-x 2 root wheel 9350 Jul  2 16:00 LaunchDaemons
lrwxr-xr-x 1 root admin   19 Feb 26  2015 Logs -> ../private/var/logs
drwxrwxr-x 2 root admin  102 Nov  8  2014 Managed Preferences
lrwxr-xr-x 1 root admin   27 Feb 26  2015 MobileDevice -> ../private/var/MobileDevice
drwxr-xr-x 3 root wheel  136 Jul  2 18:00 MobileSubstrate
drwxr-xr-x 4 root admin  170 Sep 17  2015 MusicUISupport
drwxr-xr-x 2 root wheel   68 Oct 16  2015 PreferenceBundles
drwxr-xr-x 3 root wheel  102 Jul  2 16:00 PreferenceLoader
lrwxr-xr-x 1 root admin   26 Feb 26  2015 Preferences -> ../private/var/preferences
drwxrwxr-x 2 root admin   68 Nov  8  2014 Printers
drwxr-xr-x 2 root admin  102 Jul  2 16:00 RHRevealLoader
lrwxr-xr-x 1 root admin   29 Jul  1 21:05 Ringtones -> /var/stash/_.3W5Eov/Ringtones
drwxrwxr-x 2 root admin   68 Nov  8  2014 Updates
```
```
有**RHRevealLoader**说明安装是正确的,再使用`root# ls -l /Library/RHRevealLoader` ,里面有**libReveal.dylib**
```cpp
```cpp
shujinokinakenkou:~ root# ls -l /Library/RHRevealLoader
total 4284
-rw-r--r-- 1 root admin 4384608 Jul  2 16:00 libReveal.dylib
shujinokinakenkou:~ root#
```
```
3.1)**下一步配置Reveal(三步,很简单)**
![](http://upload-images.jianshu.io/upload_images/1194882-e87d74ce526e9ec5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Snip20160702_14.png
3.2)**效果图分别为淘宝和appstore的**(***淘宝的轮播是UIScrollView做的,背景view是大大的collectionView***)
![](http://upload-images.jianshu.io/upload_images/1194882-cc72330684940ebc.gif?imageMogr2/auto-orient/strip)
taobao.gif
![](http://upload-images.jianshu.io/upload_images/1194882-5c9ef8b2df7c7613.gif?imageMogr2/auto-orient/strip)
appstore.gif
### 如果上面缺少ibReveal.dylib 继续4.步
4.上传libReveal.dylib。如果上一个步骤的检查工作全部完成：OpenSSH和Cydia Substrate成功安装。
重新打开mac上的终端工具(Terminal)，使用如下命令进入Reveal软件的动态库所在目录：cd /Applications/Reveal.app/Contents/SharedSupport/iOS-Libraries
![](https://hurui.gitbooks.io/reveal-debug/content/images/Snip20150711_43.png)
使用如下命令将libReveal.dylib上传到iOS设备的/Library/MobileSubstrate/DynamicLibraries目录下：scp libReveal.dylib root@iOS设备IP地址:/Library/MobileSubstrate/DynamicLibraries/
，请替换@之后的“iOS设备的IP地址”为你的iOS设备的**真实地址**，回车执行命令后会让你输入iOS设备的root的密码，默认是alpine，上传成功后会显示%100字样。
![](https://hurui.gitbooks.io/reveal-debug/content/images/Snip20150711_44.png)
5.编写并上传一份libReveal.plist
到iOS设备的/Library/MobileSubstrate/DynamicLibraries
目录中。这个文件的作用是：在Reveal中监控指定的App，不上传这个文件也是可以的，那么Reveal就会监控所有的App，只是这样速度会慢一点点。编写libReveal.plist文件，最简单的方法就是新建一个plist文件，它的内容是
{ Filter = { Bundles = ( "你要查看的app的bundle Id1","你要查看的app的bundle Id2" ); }; }
xcode下打开这个libReveal.plist文件可以看到它的结构示例（笔者已经添加了两个Bundle ID）：
![](https://hurui.gitbooks.io/reveal-debug/content/images/Snip20150711_47.png)
**关于获取iOS App应用的Bundle ID**：打开PP助手、iTools、itunes等工具，备份手机上安装到的程序到电脑上(或直接下载需要查看程序ipa文件)，将.ipa修改成.zip，解压后按如下路径进入info.plist文件所在目录，打开info.plist其中便有我们需要的bundle Id。
![](https://hurui.gitbooks.io/reveal-debug/content/images/Snip20150711_49.png)
![](https://hurui.gitbooks.io/reveal-debug/content/images/Snip20150711_54.png)
6.上传libReveal.plist
到iOS设备的/Library/MobileSubstrate/DynamicLibraries
目录中。打开终端进入libReveal.plist所在的目录，使用如下命令上传：scp libReveal.plist root@iOS设备的IP地址:/Library/MobileSubstrate/DynamicLibraries/
，请替换@之后的“iOS设备的IP地址”为你的iOS设备的**真实地址**，回车执行命令后会让你输入iOS设备的root的密码，默认是alpine，上传成功后会显示%100字样。
![](https://hurui.gitbooks.io/reveal-debug/content/images/Snip20150711_55.png)
7.接下来在重启你的iOS设备，连上mac电脑所在的局域网，上打开你需要在Reveal中观察的程序，此时在mac电脑上的Reveal中选择你要观察的程序。
![](https://hurui.gitbooks.io/reveal-debug/content/images/Snip20150711_57.png)
![](https://hurui.gitbooks.io/reveal-debug/content/images/Snip20150711_58.png)
提示：如果此时在mac上的Reveal左上方没有出现你想要调试的程序，这里给出几个检查的建议：
1.手机连接的wifi是否与mac处于同一局域网（如果你在执行步骤7时是在正确执行步骤5和6之后，那就忽略这个检查，这里是针对手机重启了或者是切换了手机网络等情况）。
2.查看你编写的libReveal.plist文件中写的BundleID是否与你在iOS设备上打开的应用是否匹配。甚至，请检查你的pist文件结构是否正确！
3.iOS设备上想要观察的App应用必须处于前台，进入后台即使打开了，在Reveal的左上角“选择连接的应用”框中也不会显示。
文／haojingxue_iOS（简书作者）
原文链接：http://www.jianshu.com/p/060745d5ecc2
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
