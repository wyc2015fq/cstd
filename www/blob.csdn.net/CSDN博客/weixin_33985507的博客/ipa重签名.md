# ipa重签名 - weixin_33985507的博客 - CSDN博客
2018年04月25日 17:30:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
**为什么要研究重签名问题？**
将程序打包成ipa包后，ipa包中会包含Provisioning Profile和_CodeSignature等文件，里面包含了对整个ipa的签名信息。
一旦改动ipa中的不论什么内容，比如添加一个头文件，签名就会失效，导致ipa无法安装。（貌似改变Info.plist是不影响的）
因此，假设要通过改动ipa包中的内容来篡改信息或注入代码（比如加一个动态库进去）的话。就须要解决ipa包的重签名问题。
**准备**
在重签名之前要做一些准备工作。
1.首先要下载并安装Xcode，里面有个codesign_allocate程序，在安装时/usr/bin中会多一个codesign的程序，该程序用于ipa包的签名。
2.企业证书的Certificate文件，私钥文件，Provisioning Profile文件等。注意企业证书一定要是iOS Distribution的类型，不能是iOS Development。
其他具体设置。如In-House等參见http://beyondvincent.com/blog/2014/07/30/provision-ios-ipa-app-for-in-house-enterprise-distribution/
**重签名步骤**
**1.解压ipa**
解压ipa得到Payload/[App name].bundle。打开bundle。
**2.删除旧的ipa签名信息**
删除bundle中的_CodeSignature文件夹。也就是删除之前ipa的签名信息。
**3.更换证书**
将bundle中旧的Provisioning Profile删除。用我们自己的企业证书的Provisioning Profile替代。
注意，要将ipa包中我们的企业证书改名为embedded.mobileprovision。
**4.改动bundle内容**
此时我们能够尽情地改动bundle中的内容。比如加入一个动态库。删除一张图片等。
注意：假设要往bundle中加入动态库，必须要先将其压缩成zip文件，在程序启动时将其解压到Documents文件夹下再dlopen。否则程序在启动时载入动态库文件会卡死在那里。
**5.又一次签名**
在完毕bundle内容的改动后。须要通过/usr/bin/codesign程序又一次签名。命令例如以下：
`/usr/bin/codesign -f -s "iPhone Distribution: YourCompanyName" --resource-rules Payload/ MYAPP.app/ResourceRules.plist Payload/ MYAPP.app`当中iPhone Distribution能够从**钥匙串訪问**中获取，就是企业证书的名字，比如以下的"iPhone Distribution:XXXXXXXXXX"：![](https://img-blog.csdn.net/20140820234224737)
具体的命令演示样例：
`codesign -f -s "iPhone Developer: XXXXXXXXXX" --resource-rules Payload/NdCP_Game_Demo.app/ResourceRules.plist Payload/NdCP_Game_Demo.app`这条命令的意思是：使用keychain中"iPhone Developer: XXXXXXXXXX"标识的证书。依据Payload/NdCP_Game_Demo.app/ResourceRules.plist文件里的规则进行签名，假设ipa包中已经存在签名，那么将被替换，该过程生成的_CodeSignature文件夹将存放在Payload/NdCP_Game_Demo.app文件夹下。
在命令运行后，在bundle中会生成一个新的_CodeSignature文件夹，里面包含了新的签名信息。
**6.又一次打包成ipa**
在完毕上述工作后。能够生成新的ipa进行公布。演示样例命令例如以下：
```
zip -r ResignTest.ipa Payload
# 注意Payload后面不能有“/”
```
**7.公布**
经过打包公布測试。得出结论：对改动了ipa包内容和当中的签名后，重签名的ipa文件能够在非越狱设备和越狱设备上安装成功。注意，是通过带有SSL证书的站点和Apple系统安装的（企业内部分发程序）。不是通过PP助手那样的第三方工具安装的。
**參考资料**
http://www.cocoachina.com/bbs/simple/?t181236.html
http://www.169it.com/article/15588355475122308384.html
**附录**
**1.codesign**
在帮助文档中：
**codesign -- Create and manipulate code signatures**
codesign的选项：
```
-f, --force     When signing, causes codesign to replace any existing signature on the path(s) given. Without this option, existing signatures will not be replaced, and the signing operation fails.
-s, --sign identity     Sign the code at the path(s) given using this identity. See SIGNING IDENTITIES below.
--resource-rules filename   During signing, this option overrides the default rules for identifying and collecting bundle resources and nested code to be sealed into the signature. The argument is the path to a property list (plist) file containing scanning and qualification instructions. See the code signing documentation for details.
```
其用法例如以下：
```
Usage: codesign -s identity [-fv*] [-o flags] [-r reqs] [-i ident] path ... # sign
       codesign -v [-v*] [-R testreq] path|[+]pid ... # verify
       codesign -d [options] path ... # display contents
       codesign -h pid ... # display hosting paths
```
注意：CocoaChina论坛中当中一个帖子说重签名后，会影响到keychain的使用，这一步有待验证，后面会补充说明。
