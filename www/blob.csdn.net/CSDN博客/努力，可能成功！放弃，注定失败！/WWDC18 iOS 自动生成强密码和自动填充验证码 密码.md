# WWDC18 iOS 自动生成强密码和自动填充验证码/密码 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年10月19日 11:01:26[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：349
## 概述
> 
本文将介绍[WWDC18 Automatic Strong Passwords and Security Code Autofill](https://developer.apple.com/videos/play/wwdc2018/204)和[WWDC17 Introducing Password AutoFill for Apps](https://developer.apple.com/videos/play/wwdc2017/206)中提到的几个功能: 密码自动填充/自动生成的强密码/验证码自动填充等.
> 
如果需要更好的实现此功能,网站需要`支持HTTPS`,开发Demo可以用GitHub Pages.
> 
在iOS11中,Apple已经做了启发式功能,可以让开发者在无感知的情况下,支持密码的快速填充,但是为了更好的用户体验和降低未来版本中可能出现的bug,建议对不同的功能做一些额外的步骤配置. 默认效果是这样的,QuickType Bar上没有关联域的账户密码可选
![](https://upload-images.jianshu.io/upload_images/1455933-e4421ac287d0cd84.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
默认支持的效果
## 功能一: iOS11密码自动填充功能,账户密码在QuickBar中自动显示;iOS12自动保存密码到iCloud钥匙串
> 
Infer login scenario
Check eligibility based on associated domains Find user name and password fields
Detect sign-in action
Prompt to save or update password
### 效果图
![](https://upload-images.jianshu.io/upload_images/1455933-dbff117b8e17d9df.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
密码自动填充
### 工作原理
- iOS会自动推断登录场景
- 检查关联的域是否有效合格,从iCloud 钥匙串中筛选出符合的账户和密码显示在QuickType Bar
- 根据设定输入框的textContentType类别,找到用户名和密码输入框,在用户点击QuickType Bar上的账户密码是填充到对应位置
- 检测登录操作,在登录页面要从视图层次结构中移除的时候,提示是否保存/更新账户密码
### 开发人员需要配置的步骤
> 
Associated Domains
设置UITextField的textContentType为对应类型
1.在[苹果开发者后台](https://developer.apple.com/account/)开启APP的Associated Domains服务
![](https://upload-images.jianshu.io/upload_images/1455933-9d0ac693d9e3a85e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
苹果开发者后台APP配置
2.记录Prefix和app的BundleID,生成apple-app-site-association文件,放到关联域网站服务器的.well-known目录下面或者根目录下 . `注意:`网站需要支持HTTPS,Demo中为了演示,可以把项目和个人的Github Pages关联,文件放到自己的Github Pages.
```
{
    "webcredentials":{
        "apps":["XW5558TH45.com.beike.testapp"] 
    }
}
```
![](https://upload-images.jianshu.io/upload_images/1455933-170dc995082d2f6e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
网站设置:文件放到关联域的服务器
官方示例存放目录:
![](https://upload-images.jianshu.io/upload_images/1455933-c342cf0eb0e014c5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
官方示例文件存放
3.Xcode中项目配置,开启Domains,点击"+"添加一项, webcredentials:后面的"[coderxllau.github.io](http://coderxllau.github.io)"改为自己关联的域,例如webcredentials:www.baidu.com等
![](https://upload-images.jianshu.io/upload_images/1455933-0eb7f05ef8aed6cd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
Xcode中项目配置
4.Xcode中除了开启并关联域,还需要更改输入框textContentType为指定类别
> 
iOS 11和iOS 12中分别新增了新的Type.
```
UIKIT_EXTERN UITextContentType const UITextContentTypeUsername                  NS_AVAILABLE_IOS(11_0);
UIKIT_EXTERN UITextContentType const UITextContentTypePassword                  NS_AVAILABLE_IOS(11_0);
UIKIT_EXTERN UITextContentType const UITextContentTypeNewPassword               NS_AVAILABLE_IOS(12_0);
UIKIT_EXTERN UITextContentType const UITextContentTypeOneTimeCode               NS_AVAILABLE_IOS(12_0);
```
```
self.userNameField.textContentType = UITextContentTypeUsername;
self.passwordField.textContentType = UITextContentTypePassword;
```
## 功能二: iOS12 自动生成建议的用户和强密码
### 效果图
![](https://upload-images.jianshu.io/upload_images/1455933-28bc863ee44f746c.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
自动生成强密码
### 工作原理/设置步骤
和功能一Password Autofill基本一样,不同的地方是在iOS 12中新增UITextContentTypeNewPassword,用这个字段标记新密码输入框即可在用户点击的时候自动生成强密码填充
```
self.nameField.textContentType = UITextContentTypeUsername;
if (@available(iOS 12.0, *)) {
    self.passwordField.textContentType = UITextContentTypeNewPassword;
    self.passwordField.passwordRules = [UITextInputPasswordRules passwordRulesWithDescriptor:@"required: lower; required: upper; allowe: digit; required: [-]; minlength: 5;"];
} else {
    self.passwordField.textContentType = UITextContentTypePassword;
}
```
![](https://upload-images.jianshu.io/upload_images/1455933-7c356e3c3f275316.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/814/format/webp)
image.png
### 密码的格式
自动生成的密码,我们可以通过一些方法指定生成的密码格式和规则. 可以在官方提供的密码格式工具[Password Rules Validation Tool](https://developer.apple.com/password-rules/)
上面调试自己的密码格式,将生成的密码格式描述复制下来,设置给UITextField的passwordRules属性.
```
if (@available(iOS 12.0, *)) {
    self.passwordField.textContentType = UITextContentTypeNewPassword;
    self.passwordField.passwordRules = [UITextInputPasswordRules passwordRulesWithDescriptor:@"required: lower; required: upper; allowe: digit; required: [-]; minlength: 5;"];
}
```
![](https://upload-images.jianshu.io/upload_images/1455933-e75dac63f9511f63.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
密码生成格式规则
![](https://upload-images.jianshu.io/upload_images/1455933-78b8e10eb78da25c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/288/format/webp)
IB中设置密码生成格式
## 功能三: iOS12 自动填充验证码
### 效果图 , 从短信中获取到验证码后自动显示到QuickType Bar上
![](https://upload-images.jianshu.io/upload_images/1455933-8b6a744e7272356a.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
验证码自动识别
工作原理
在输入框成慰第一响应者的时候,使用数据检测器启发式来推断传入消息携带安全代码,把检测到验证码显示到QuickType Bar上,省去用户的操作成本
缺陷:在demo中可以看到,自如发来的验证码也可以被demo获取,对于验证码来源没有做到很好的隔离
开发步骤:
开发步骤和功能一基本一致,在完成功能一的基础上面,设置输入框的输入类别为UITextContentTypeOneTimeCode即可.
## 注意点
- 您应避免在视图层次结构中构建定制键盘UI，或在控件中设置自定义输入视图。因为如果您这样做，则会阻止iOS显示必要的UI或注入相应的事件以代表您的用户键入代码。
- 关联的网站要支持HTTPS
- 标记textContentType的输入框独立开,不要用一个输入框标记两个类别
- 验证码来源没有做到很好的隔离
## 参考资料
[WWDC18 Automatic Strong Passwords and Security Code Autofill](https://developer.apple.com/videos/play/wwdc2018/204)
[WWDC17 Introducing Password AutoFill for Apps](https://developer.apple.com/videos/play/wwdc2017/206);
作者：shannoon
链接：https://www.jianshu.com/p/7e812c43b08f
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
