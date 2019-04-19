# ShareSDK 3.4.0 isWXAppInstalled 返回NO - 月若无涯的专栏 - CSDN博客
2016年08月16日 11:08:39[月若无涯](https://me.csdn.net/u010124617)阅读数：7682
    升级到3.4.0版本的ShareSDK之后，发现`[WXApi isWXAppInstalled]`方法一直返回false，无法正常使用。初步怀疑是ShareSDK自己的bug。
    查找资料后发现，解决方案居然是要自己在AppDelegate中的`didFinishLaunchingWithOptions`方法内，调用微信SDK官方api的注册方法：
`[WXApi registerApp:@"wx123123123"];`
即可修复问题。
    问题解决起来虽然很简单，但是这也反应ShareSDK的团队在SDK开发上的一些问题，这种问题都能出现……
20160822更新： 
今天经过沟通后发现，ShareSDK自己做了一套用于判断设备是否安装相应平台的判断，如果判断微信也可以使用下面这种方法
```
#import <ShareSDKExtension/ShareSDKExtension.h>
BOOL isInstallWx = [ShareSDK isClientInstalled:SSDKPlatformTypeWechat];
```
来判断。但如果涉及到微信自己方法的调用的，还是要使用上面那种进行官方的注册。
另外很遗憾，本人在自己工程中测试时，发现了一个必现的bug，第一次打开应用后，微信相关功能无法使用，报错如下：
> 
warning 微信 应用信息有误 不能进行相关操作 请检查本地代码中和服务端的 微信 平台应用配置是否有误 配置 app_id null app_secret null
而第二次打开应用问题消失，一切功能良好，此情况必现。
该问题看起来是微信初次注册不成功，但是由于本人配置未曾变化，使用之前版本的SDK则无问题，因此猜测ShareSDK内部注册有bug。 
与他们技术沟通后，他们未能重现，暂未找到解决方案，因此个人回滚3.3.2版本使用。
不知道是否有其他开发者也遇到此问题，如果有欢迎留言讨论。
