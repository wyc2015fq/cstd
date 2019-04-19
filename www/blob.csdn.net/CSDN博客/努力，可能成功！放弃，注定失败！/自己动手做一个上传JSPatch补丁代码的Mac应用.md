# 自己动手做一个上传JSPatch补丁代码的Mac应用 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年04月21日 08:45:09[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：717
本文来自简书，原文地址:[http://www.jianshu.com/p/efb4bbe72f64](http://www.jianshu.com/p/efb4bbe72f64)
![微笑](http://static.blog.csdn.net/xheditor/xheditor_emot/default/smile.gif)卷首语：自己动手做 JSPatch 补丁代码的上传下发，补丁代码采用 RSA非对称加密 ，安全性有保障。操作简单，只需要将文件拖进来即可。并且无接入数限制，永久免费！
接上篇[文章](http://www.jianshu.com/p/6607fef31de0),我们讲述了如何混淆`JSPatch`框架，以绕过苹果的检测。那么不可避免的我们就要面对一个问题，那就是自己做补丁代码的上传与下发工作。
> 
首先我们看一下[JSPatchPlatform](https://jspatch.com/)这个官方平台的大致样子，如下图。
![](http://upload-images.jianshu.io/upload_images/2200314-235531e2cf343f31.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
JSPatchPlatform平台.png
从图中我们可以看到，其中有几个非常重要的点，一个是`给你APP的哪个版本下发补丁`，另一个是你`下发的补丁代码是什么`，最后一个就是出于安全性考虑对你的补丁代码`进行RSA非对称加密`，因此我们主要围绕这三个功能来开发自己的上传补丁代码的应用。（当然补丁描述与开发预览功能也很实用，只是在这里我们暂时不做这样的功能，以后可能会加上去。。）
其实一开始就有两条路摆在面前，那就是你是做一个网页版的补丁上传入口还是做一个Mac版的应用呢，最后我们选择了自己做Mac应用，一是在没有十足的把握下（因为一开始我们并不确定我们的混淆方案能否通过）不想给其他研发组的人员增加工作量，二是作为一个开发者，我们不应该仅仅局限于开发手机应用，基于苹果OSX这个统一的得天独厚的生态系统，我们可以很容易的去开发一个简单的Mac应用。比如在开发过程中有很多工具是直接从现有项目里拿过来的，并且完全兼容，没有错误。所以最终我们只需要后台提供给我们一个接口就可以了。。。
下面就是我们开发这个Mac应用的大致思路。我们知道`JSPatch`最终解析的是一个`字符串`，所以我们就想把我们写的补丁文件比如`main.js`最终也转变成一个字符串，并且这个字符串是经过`RSA`加密过的，最后在将这个加密后的字符串上传到自己的服务器就可以了，所以最终要做的事情其实很简单：读取文件内容，进行加密，得到一个加密后的字符串，设置版本号，上传服务器。
> 
所以我们的Mac应用是这个样子的（请假想它其实长得并不丑。。）
![](http://upload-images.jianshu.io/upload_images/2200314-5ef41ebdd799c5a7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
JP-Mac应用窗口.png
怎么使用这个应用呢：将你写好的`本地js补丁文件`，拖进那个文件框，点击`开始加密`，然后就会在下方显示出`加密后的结果`和`文件的原始内容`。关于服务器地址的设置，你需要到代码中根据自己的情况进行设置，版本号也需要自己设置，只要你能够对应起来就可以了。
下面简单的介绍一下本文中用到的`非对称加密`，这是用来对补丁代码进行加密的，非对称加密讲究公钥与私钥，先来搞定这两个东西。这里，我们使用的是`OpenSSL`生成的。我们最终需要的是一个`PKCS#12`格式的文件，以及一个自签名证书。因为自己用的是Mac系统，所以就不用再麻烦着安装`OpenSSL`了。
首先，生成私钥与自签名证书，这里，我们将其合并，我们需要的就是这种效果。
打开终端输入以下的命令（注意执行下面命令后会让你输入密码及填写一些基本的信息比如国家城市等等，本demo中的所有密码都是作者的笔名：`ccsundaychina`，而这个密码在demo中的`Security`类中也需要用到，读者到时可以自行进行修改。）
`openssl req -x509 -days 3650 -new -newkey rsa:2048 -keyout priv_key_self_signed_cert.pem -out priv_key_self_signed_cert.pem`
接着，使用上面生成的文件，导出`PKCS#12`格式的文件（后缀通常为`pfx或p12`）及自签名证书（后缀通常为`der、cer、crt`）。
命令如下
`openssl pkcs12 -export -out pkcs.p12 -in priv_key_self_signed_cert.pem``openssl x509 -in priv_key_self_signed_cert.pem -inform PEM -out cert.der -outform DER`
这样我们得到了以下三个文件
![](http://upload-images.jianshu.io/upload_images/2200314-7b710323b73b9cfd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
证书文件.png
将其中的`p12`文件与`cert.der`文件拖入项目即可。
在项目中的`Security`类中用到了这两个文件。这个类的作用就是对输入的`NSData`进行`RSA加解密`的，下面是它的头文件。
```
//
//  Security.h
//  testMacDemo
//
//  Created by ccSunday on 2017/3/17.
//  Copyright © 2017年 ccSunday. All rights reserved.
//
#import <Foundation/Foundation.h>
@interface Security : NSObject
+ (instancetype)sharedSecurity;
// RSA公钥加密，支持长数据加密
- (NSData *)encryptWithPublicKey:(NSData *)plainData;
// RSA私钥解密，支持长数据解密
- (NSData *)decryptWithPrivateKey:(NSData *)cipherData;
@end
```
当点击开始加密按钮的时候，就会对文件路径下的文件进行加密操作了，下面是具体的代码。
```
- (IBAction)parseFiles:(NSButton *)sender {
    NSLog(@"文件路径：%@",_FilesPathTf.stringValue);
    //获得原始数据
    NSData *baseData = [NSData dataWithContentsOfFile:_FilesPathTf.stringValue];
    //RSA加密
    NSData *encryptData = [[Security sharedSecurity]encryptWithPublicKey:baseData];
    //获得RAS加密后的字符串,将此字符串上传服务器。服务器需要返回该字符串。
   NSString * base64Encoded = [encryptData base64EncodedStringWithOptions:0];
    _resultTextField.stringValue = base64Encoded;
    // NSData from the Base64 encoded str
    NSData *nsdataFromBase64String = [[NSData alloc]
                                      initWithBase64EncodedString:base64Encoded options:0];
    //私钥解密
    //    NSData *
    NSData *decryptData = [[Security sharedSecurity]decryptWithPrivateKey:nsdataFromBase64String];
    NSString *result = [[NSString alloc] initWithData:decryptData  encoding:NSUTF8StringEncoding];
    _originalContentTF.stringValue = result;
}
```
需要注意的一点是，由于加密后返回的是`NSData类型`的数据，而上传服务器的时候需要转为`NSString`,所以需要对它再进行一层转换，这里我们采用的是`base64`获得所对应的加密字符串,读者也可以根据自己的情况进行转换。只要做好前后端的统一即可，而这个`base64Encoded`就是我们需要上传到服务器的主要东西了。一般APP端都会有自己的加密方式，那么在上传这个`base64Encoded`的时候，要记得对其按照APP端统一的网络请求的加密方式再做一层加密。
本文demo可以到这里[下载](https://github.com/sunday1990/JSPatch-BackgroundOperation) ，下载下来后解压`JSPatch后台.zip`，就可以得到一个Mac桌面应用了，使用的时候记得添加信任。
另外运行demo时候，如果需要输入密码的话，输入`ccsundaychina`即可。 打包的方式跟手机app的打包方式是一样的。
