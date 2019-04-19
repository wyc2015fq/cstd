# 一步一步实现iOS微信自动抢红包(非越狱) - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年03月25日 09:25:00[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1372
![](http://upload-images.jianshu.io/upload_images/691288-c7e714dcf4f87a75.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
微信红包
> 
前言：最近笔者在研究iOS逆向工程，顺便拿微信来练手，在非越狱手机上实现了微信自动抢红包的功能。
**题外话：此教程是一篇严肃的学术探讨类文章，仅仅用于学习研究，也请读者不要用于商业或其他非法途径上，笔者一概不负责哟~~**
好了，接下来可以进入正题了！
#### 此教程所需要的工具/文件
- [yololib](https://github.com/KJCracks/yololib)
- [class-dump](http://stevenygard.com/projects/*class-dump*/)
- [dumpdecrypted](https://github.com/stefanesser/dumpdecrypted)
- [iOSOpenDev](http://iosopendev.com/download/)
- [iTools](http://www.itools.cn/)
- OpenSSH(Cydia)
- iFile(Cydia)
- Cycript(Cydia)
- Command Line Tools
- Xcode
- 苹果开发者证书或企业证书
- 一台越狱的iPhone
是的，想要实现在非越狱iPhone上达到自动抢红包的目的，工具用的可能是有点多（**工欲善其事必先利其器**^_^）。不过，没关系，大家可以按照教程的步骤一步一步来执行，不清楚的步骤可以重复实验，毕竟天上不会掉馅饼嘛。
## 解密微信可执行文件(Mach-O)
因为从Appstore下载安装的应用都是加密过的，所以我们需要用一些工具来为下载的App解密，俗称砸壳。这样才能便于后面分析App的代码结构。
首先我们需要一台已经越狱的iPhone手机(现在市面上越狱已经很成熟，具体越狱方法这里就不介绍了)。然后进入Cydia，安装**OpenSHH**、**Cycript**、**iFile**(调试程序时可以方便地查看日志文件)这三款软件。
> 
PS：笔者的手机是iPhone 6Plus，系统版本为iOS9.1。
在电脑上用iTunes上下载一个最新的微信，笔者当时下载的微信版本为6.3.13。下载完后，iTunes上会显示出已下载的app。
![](http://upload-images.jianshu.io/upload_images/691288-2830c17dd8f98e92.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
iTunes
连上iPhone，用iTunes装上刚刚下载的微信应用。
打开Mac的终端，用ssh进入连上的iPhone**(确保iPhone和Mac在同一个网段，笔者iPhone的IP地址为192.168.8.54)**。OpenSSH的root密码默认为*alpine*。
![](http://upload-images.jianshu.io/upload_images/691288-efbcbbadadb0ea51.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
ssh
接下来就是需要找到微信的Bundle id了，，这里笔者有一个小技巧，我们可以把iPhone上的所有App都关掉，唯独保留微信，然后输入命令
`ps -e`
![](http://upload-images.jianshu.io/upload_images/691288-ea9fc8c6ca4dfd63.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
微信bundle id
这样我们就找到了微信的可执行文件Wechat的具体路径了。接下来我们需要用Cycript找出微信的Documents的路径，输入命令`cycript -p WeChat`
![](http://upload-images.jianshu.io/upload_images/691288-0e731253d5adee7c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
cycript
- 编译dumpdecrypted
先记下刚刚我们获取到的两个路径(Bundle和Documents)，这时候我们就要开始用dumpdecrypted来为微信二进制文件(WeChat)砸壳了。
确保我们从Github上下载了最新的dumpdecrypted源码，进入dumpdecrypted源码的目录，编译dumpdecrypted.dylib，命令如下:
![](http://upload-images.jianshu.io/upload_images/691288-a444fac57bafe7f5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
dumpdecrypted.dylib
这样我们可以看到dumpdecrypted目录下生成了一个dumpdecrypted.dylib的文件。
- scp
拷贝dumpdecrypted.dylib到iPhone上，这里我们用到scp命令.
`scp 源文件路径 目标文件路径` 。具体如下：
![](http://upload-images.jianshu.io/upload_images/691288-50aec0e15d68b229.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
scp
- 开始砸壳
dumpdecrypted.dylib的具体用法是：`DYLD_INSERT_LIBRARIES=/PathFrom/dumpdecrypted.dylib /PathTo`
![](http://upload-images.jianshu.io/upload_images/691288-29352a27fd1d0bb1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
dumpdecrypted
这样就代表砸壳成功了，当前目录下会生成砸壳后的文件，即WeChat.decrypted。同样用scp命令把WeChat.decrypted文件拷贝到电脑上,接下来我们要正式的dump微信的可执行文件了。
## dump微信可执行文件
- 
从Github上下载最新的class-dump源代码，然后用Xcode编译即可生成class-dump(这里比较简单，笔者就不详细说明了)。
- 
导出微信的头文件
使用class-dump命令,把刚刚砸壳后的WeChat.decrypted,导出其中的头文件。`./class-dump -s -S -H ./WeChat.decrypted -o ./header6.3-arm64`
![](http://upload-images.jianshu.io/upload_images/691288-1aacfbf80ef82f78.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
导出的头文件
这里我们可以新建一个Xcode项目，把刚刚导出的头文件加到新建的项目中，这样便于查找微信的相关代码。
![](http://upload-images.jianshu.io/upload_images/691288-eaf3744cdfd9390d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
微信的头文件
找到**CMessageMgr.h**和**WCRedEnvelopesLogicMgr.h**这两文件，其中我们注意到有这两个方法：`- (void)AsyncOnAddMsg:(id)arg1 MsgWrap:(id)arg2;` ，`- (void)OpenRedEnvelopesRequest:(id)arg1;`。没错，接下来我们就是要利用这两个方法来实现微信自动抢红包功能。其实现原理是，通过hook微信的新消息函数，我们判断是否为红包消息，如果是，我们就调用微信的打开红包方法。这样就能达到自动抢红包的目的了。哈哈，是不是很简单，我们一起来看看具体是怎么实现的吧。
- 新建一个dylib工程，因为Xcode默认不支持生成dylib，所以我们需要下载iOSOpenDev，安装完成后(Xcode7环境会提示安装iOSOpenDev失败，请参考[iOSOpenDev安装问题](http://www.tqcto.com/article/software/14553.html))，重新打开Xcode，在新建项目的选项中即可看到iOSOpenDev选项了。
![](http://upload-images.jianshu.io/upload_images/691288-ccbcb59d4e6eea3a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
iOSOpenDev
- 
dylib代码
选择Cocoa Touch Library，这样我们就新建了一个dylib工程了，我们命名为autoGetRedEnv。
删除autoGetRedEnv.h文件，修改autoGetRedEnv.m为autoGetRedEnv.mm，然后在项目中加入[CaptainHook.h](https://github.com/rpetrich/CaptainHook)
因为微信不会主动来加载我们的hook代码，所以我们需要把hook逻辑写到构造函数中。
```
__attribute__((constructor)) static void entry()
{
  //具体hook方法
}
```
hook微信的AsyncOnAddMsg: MsgWrap:方法，实现方法如下：
```
//声明CMessageMgr类
CHDeclareClass(CMessageMgr);
CHMethod(2, void, CMessageMgr, AsyncOnAddMsg, id, arg1, MsgWrap, id, arg2)
{
  //调用原来的AsyncOnAddMsg:MsgWrap:方法
  CHSuper(2, CMessageMgr, AsyncOnAddMsg, arg1, MsgWrap, arg2);
  //具体抢红包逻辑
  //...
  //调用原生的打开红包的方法
  //注意这里必须为给objc_msgSend的第三个参数声明为NSMutableDictionary,不然调用objc_msgSend时，不会触发打开红包的方法
  ((void (*)(id, SEL, NSMutableDictionary*))objc_msgSend)(logicMgr, @selector(OpenRedEnvelopesRequest:), params);
}
__attribute__((constructor)) static void entry()
{
  //加载CMessageMgr类
  CHLoadLateClass(CMessageMgr);
  //hook AsyncOnAddMsg:MsgWrap:方法
  CHClassHook(2, CMessageMgr, AsyncOnAddMsg, MsgWrap);
}
```
> 
项目的全部代码，笔者已放入[Github](https://github.com/east520/AutoGetRedEnv)中。
完成好具体实现逻辑后，就可以顺利生成dylib了。
## 重新打包微信App
- 
为微信可执行文件注入dylib
要想微信应用运行后，能执行我们的代码，首先需要微信加入我们的dylib，这里我们用到一个dylib注入神器:[yololib](https://github.com/KJCracks/yololib)，从网上下载源代码，编译后得到yololib。
使用yololib简单的执行下面一句就可以成功完成注入。注入之前我们先把之前保存的WeChat.decrypted重命名为WeChat，即已砸完壳的可执行文件。
`./yololib 目标可执行文件 需注入的dylib`
注入成功后即可见到如下信息：
![](http://upload-images.jianshu.io/upload_images/691288-7319139328678dd9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
dylib注入
- 
新建Entitlements.plist
```xml
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
  <key>application-identifier</key>
  <string>123456.com.autogetredenv.demo</string>
  <key>com.apple.developer.team-identifier</key>
  <string>123456</string>
  <key>get-task-allow</key>
  <true/>
  <key>keychain-access-groups</key>
  <array>
      <string>123456.com.autogetredenv.demo</string>
  </array>
</dict>
</plist>
```
这里大家也许不清楚自己的证书Teamid及其他信息，没关系，笔者这里有一个小窍门，大家可以找到之前用开发者证书或企业证书打包过的App(例如叫Demo)，然后在终端中输入以下命令即可找到相关信息，命令如下：
`./ldid -e ./Demo.app/demo`
- 
给微信重新签名
接下来把我们生成的**dylib(libautoGetRedEnv.dylib)**、刚刚注入dylib的**WeChat**、以及**embedded.mobileprovision**文件(可以在之前打包过的App中找到)拷贝到**WeChat.app**中。
命令格式：`codesign -f -s 证书名字 目标文件`
> 
PS:证书名字可以在钥匙串中找到
分别用codesign命令来为微信中的相关文件签名,具体实现如下：
![](http://upload-images.jianshu.io/upload_images/691288-93795a412c283610.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
重新签名
- 
打包成ipa
给微信重新签名后，我们就可以用xcrun来生成ipa了，具体实现如下：
`xcrun -sdk iphoneos PackageApplication -v WeChat.app -o ~/WeChat.ipa`
## 安装拥有抢红包功能的微信
以上步骤如果都成功实现的话，那么真的就是万事俱备，只欠东风了~~~
我们可以使用iTools工具，来为iPhone(此iPhone Device id需加入证书中)安装改良过的微信了。
![](http://upload-images.jianshu.io/upload_images/691288-5b04b16681fb37ec.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
iTools
## 大工告成！！
好了，我们可以看看hook过的微信抢红包效果了~
![](http://upload-images.jianshu.io/upload_images/691288-cf9d5c6e1ebce029.gif?imageMogr2/auto-orient/strip)
自动抢红包
哈哈，是不是觉得很爽啊，"妈妈再也不用担心我抢红包了。"。大家如果有兴趣可以继续hook微信的其他函数，这样既加强了学习，又满足了自己的特(zhuang)殊(bi)需求嘛。
**教程中所涉及到的工具及源代码笔者都上传到Github上。[Github地址](https://github.com/east520/AutoGetRedEnv)**
**特别鸣谢:**
1.[iOS冰与火之歌(作者:蒸米)](http://drops.wooyun.org/papers/12803)
2.[iOS应用逆向工程](http://www.amazon.cn/iOS%E5%BA%94%E7%94%A8%E9%80%86%E5%90%91%E5%B7%A5%E7%A8%8B-%E6%B2%99%E6%A2%93%E7%A4%BE/dp/B00VFDVY7E/ref=sr_1_1?s=books&ie=UTF8&qid=1458733194&sr=1-1&keywords=ios%E5%BA%94%E7%94%A8%E9%80%86%E5%90%91%E5%B7%A5%E7%A8%8B)
