# iOS 集成银联支付 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年03月17日 13:13:50[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：603
> 
当初集成支付宝的时候 觉得见了这么丑的代码 加上这么难找的下载地址 在配上几乎为零的文档 寒哥就要吐血了
下午去集成银联 才知道 血吐的早了
- 下载地址 
> 
[https://open.unionpay.com/upload/download/Development_kit85427986.rar](https://open.unionpay.com/upload/download/Development_kit85427986.rar)
其实我找了半个小时 也不知道怎么就下载好了 这个我在Chrome的下载记录里找到的 
解压之后会有这样的目录结构
![](http://upload-images.jianshu.io/upload_images/239184-ebeabb025e019b86.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
文档在下面 sdk在upmp_iPhone里面
![](http://upload-images.jianshu.io/upload_images/239184-7ff3b248547406c6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
# 1、 SDK 说明
SDK 分为以下两个版本:1 支持纯无卡交易静态库,以下简称UPPayPlugin,包含文件:
`UPPayPlugin.h``UPPayPluginDelegate.h``libUPPayPlugin.a`
# 2 支持纯无卡交易和 VIPOS 音频口支付静态库,以下简称 UPPayPluginPro,
包含文件:
`UPPayPluginPro.h`
`UPPayPluginDelegate.h``libUPPayPluginPro.a`
# 3、 添加 SDK 包
- 
a) 根据商户选择的SDK版本,将sdk/inc目录和sdk/libs目录下对应版本的三个文件添加到 UPPayDemo 工程中;
- 
b) 如 果 你 选 择 的 是 UPPayPlugin 版 本 , 添 加 QuartzCore.framework 、Security.framework 到工程中;
- 
c) 如 果 你 选 择 的 是 UPPayPluginPro 版 本 , 添 加 QuartzCore.framework 、AudioToolbox.framework, CoreAudio.framework 、 MediaPlayer.framework,AVFoundation.framework 和 Security.framework 到工程中;
- 
d) 在工程的 Build Settings 中找到 Other Linker Flags 中添加-ObjC 宏;（这个可能是遗留的问题 我就没写 不报错）
![](http://upload-images.jianshu.io/upload_images/239184-6764e25cd0a71413.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
# 4、 调用插件
- 
在需要调用支付控件的源文件内引用头文件 UPPayPlugin.h 或 UPPayPluginPro.h(注意:如果工程的 compile source as 选项的值不是 Objective–C++,则引用此头文件的文件类型都要改为.mm)
- 
通过调用
- (BOOL)startPay:(NSString*)tn mode:(NSString*)mode
viewController:(UIViewController*)viewController delegate:(id<UPPayPluginDelegate>)delegate;
实现控件的调用
- 
处理支付结果
银联手机支付控件有三个支付状态返回值:success、fail、cancel,分别代表:支付成功、支付失败、用户取消支付。这三个返回状态值以字符串的形式作为回调函数参数(NSString*)result 返回。通过在工程中添加头文件“UPPayPluginDelegate.h”,在处理交易结果的界面,实现 UPPayPluginDelegate接口,根据该头文件中的回调函数:-(void)UPPayPluginResult:(NSString*)result来实现回调方法,从而可以根据支付结果的不同进行相关的处理。
> 
1.将 xcode 工程中 Build Setting -> Apple LLVM compiler Language 标签下的 C++ Language Dialect 和 C++ Standard Library 属性值修改为 Compiler Default
2.将涉及到引用 UPPayPlugin.h 的源文件的后缀名都改为.mm;
3.参照demo，检查工程设置 Search Paths 里的 Framework Search Paths、Header Search Paths、Library Search Paths 的路径设置,看设置路径是否正确
- 添加 QuartzCore.framework、AudioToolbox.framework, CoreAudio.framework、 MediaPlayer.framework, AVFoundation.framework 和 Security.framework
5.工程的 Build Settings 中找到 Other Linker Flags 中添加-ObjC 宏;
上面提到的几点网上都可以找到，如果已经正确设置，还是编译不通过，那么赶紧检查下 项目targets 对应的sdk版本，选择7.0以下的，目前银联的sdk貌似还不支持7.0，
## 注
> 
银联支付目测只需要2个参数 1 tn 其实就是订单号 2 mode 是测试环境还是线上环境
和支付宝不同的是 支付宝要求本地把订单的信息书写完毕 调起sdk支付即可 银联是我把一些生成订单的信息 如 付款方 收款方 订单描述 金额等信息传递到服务器 由服务器生成一个订单号 然后客户端调起sdk进入支付
mode 竟然是个死的字符串 真是醉了 `"00"` 表示线上环境`"01"`表示测试环境
返回的代理结果 更是坑爹 :success、fail、cancel,分别代表:支付成功、支付失败、用户取消支付
测试环境 账号 
提供测试使用卡号、手机号信息(此类信息仅供测试,不会发生正式交易)招商银行预付费卡:卡号:6226 4401 2345 6785密码:111101
