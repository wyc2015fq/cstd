# iOS Jenkins自动化打包 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2019年04月10日 11:22:33[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：8
> 
最近测试打包实在烦的不要不要的，一天打N多个包。这样之后，让测试小哥哥小姐姐自己操作就行了。
# Jenkins
```
houjianan:~> brew install Jenkins  
Updating Homebrew...
jenkins: Java 1.8 is required to install this formula.
JavaRequirement unsatisfied!
You can install with Homebrew Cask:
 brew cask install homebrew/cask-versions/java8
You can download from:
 [https://www.oracle.com/technetwork/java/javase/downloads/index.html](https://www.oracle.com/technetwork/java/javase/downloads/index.html)
Error:An unsatisfied requirement failed this build.
```
[java下载地址](https://www.oracle.com/technetwork/java/javase/downloads/index.html)
上述错误是因为没有安装Java 点击链接地址 下载Java即可
再执行
```
houjianan:~> brew install Jenkins  
==>Downloading [http://mirrors.jenkins.io/war/2.156/jenkins.war](http://mirrors.jenkins.io/war/2.156/jenkins.war)
==>Downloading from [http://mirrors.tuna.tsinghua.edu.cn/jenkins/war/2.156/jenkins.war](http://mirrors.tuna.tsinghua.edu.cn/jenkins/war/2.156/jenkins.war)
######################################################################## 100.0%
==>jar xvf jenkins.war
==>Caveats
Note: When using launchctl the port will be 8080.
To have launchd start jenkins now and restart at login:
  brew services start jenkins
Or, if you don't want/need a background service you can just run:
  jenkins
==>Summary
🍺  /usr/local/Cellar/jenkins/2.156: 7 files, 76MB, built in 22 seconds
```
### 安装插件：
- Keychans and Provisioninng Profiles Management （管理本地的keychain和iOS证书的插件
- Xcode integration （用于xcode构建）
- Publish Over SSH （上传服务器使用）
系统管理-插件管理-可选插件-过滤 （因为我安装过了）
![](https://upload-images.jianshu.io/upload_images/1024878-b57ba616e0bf0da6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
BED4B18C-C96A-472B-9D5A-670AF86DBEEE.png
![](https://upload-images.jianshu.io/upload_images/1024878-21193618f813ce88.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
B6A2DB7C-970E-4673-A694-A5B5BAA0B3D2.png
![](https://upload-images.jianshu.io/upload_images/1024878-933ce6ba343e9c13.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
9F7698C2-C4CD-4D5E-811E-861CB52382B5.png
Keychans and Provisioninng Profiles Management
**步骤：**
- *login.keychain*
	终端 cd ~/Library/Keychains
	将login.keychain-db复制出来（放到桌面或者其他目录），并且将名字后面的-db删除（有-db就删除）。
	login.keychain文件准备就绪
- *去苹果爸爸开发者中心下载证书和描述文件到本地*
	证书和描述文件准备就绪
- *上传文件到Jenkins*
	Jenkins-系统管理-Keychans and Provisioninng Profiles Management点击进入下面界面 上传上面修改好的文件
	选择(login.keychain 证书 描述文件 选一个 upload一次)之后 点击Upload 出现如下界面
	
![](https://upload-images.jianshu.io/upload_images/1024878-d2b725f45e243348.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
3A317CE8-20F6-4DE4-8497-A31F542E6F47.png
`注：使用证书和描述文件之前在xcode测试是否可以正常archive`
Jenkins创建任务
![](https://upload-images.jianshu.io/upload_images/1024878-195a6682e0f91473.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
87D45939-4132-48A3-997D-2C9B5ADAD11D.png
输入名称，选择自由风格软件项目，点击确定，如下图
![](https://upload-images.jianshu.io/upload_images/1024878-afbfe3f29b517ae0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
74B686AB-B710-4A98-85AE-0DC41EB87B81.png
Jenkins可以查看到创建的相应任务-点击名称-进入项目
![](https://upload-images.jianshu.io/upload_images/1024878-a1d7bc88c6b6d49b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
27507498-990E-4446-97D9-55CC3371B7C4.png
项目GAJenkins，点击 配置
### 配置
**1、General**
点开高级
![](https://upload-images.jianshu.io/upload_images/1024878-0af199313edc585a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
FC5BDA4B-DC80-4DDD-80E9-F013D9958BD3.png
**2、源码管理**
这里把代码传到gihub上了，为了测试使用。后续配置svn
![](https://upload-images.jianshu.io/upload_images/1024878-8a29cd1c3e073068.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
git和svn都需要添加账号 Jenkins
![](https://upload-images.jianshu.io/upload_images/1024878-066bad599d49c496.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
Respository URL 去github复制
### 构建触发器
![](https://upload-images.jianshu.io/upload_images/1024878-8f2eeb89d1734035.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
[构建触发器]
![](https://upload-images.jianshu.io/upload_images/1024878-e37a47c14a7315cd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/928/format/webp)
获取身份验证令牌-右上角点击名字进入设置
![](https://upload-images.jianshu.io/upload_images/1024878-3ac1523e96641fd9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
在设置界面添加Token 我之前添加过了，所以显示的是创建日期
构建环境
在Keychans and Provisioninng Profiles Management添加过了，这里选择即可
```
Variables added to the build environment. They can be used in other build steps.
${KEYCHAIN_PATH}
Contains the absolute path of the selected copied keychain into the workspace.
${KEYCHAIN_PASSWORD}
Password to unlock the keychain.
${CODE_SIGNING_IDENTITY}
General name of the selected code signing identity.
```![43DB524C-6F4F-4E6A-85BF-40D0472493B6.png](https://upload-images.jianshu.io/upload_images/1024878-90452726e279196c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
```
**构建-执行shell**
`执行shell 需要添加，不需要不添加`
项目使用pod
这里添加执行shell
Pod install
Open GAFastlane.xcworkspace
注意后缀：.xcworkspace
![](https://upload-images.jianshu.io/upload_images/1024878-8d33c8af2043bd1c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
077C3B64-EFB2-4E27-8A87-6F9A1F98F2B3.png
**构建-Xcode**
Xcode integration 在安装工具的时候已经安装好
需要勾选clean before build
Configration: Debug and Release两种
Xcode Schema File 如下图选择的 一般项目名字
![](https://upload-images.jianshu.io/upload_images/1024878-06af65d29304f5d2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
6499C713-77F4-4B65-8F52-D7DECC194F0E.png
![](https://upload-images.jianshu.io/upload_images/1024878-9dc1106804632256.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
AC74AC3E-97B4-44F5-AE14-9458A360E593.png
![](https://upload-images.jianshu.io/upload_images/1024878-309060eb44cc6740.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
4F45F0B5-683B-46EF-9E00-A6A2E78FC291.png
勾选Pack application, build and sign .ipa?
Export method 根据证书和需要填写
.ipa filename pattern
```
${BASE_NAME} 
It is replaced with the base name of the archive 
${VERSION} 
It is replaced with CFBundleVersion in Info.plist 
${SHORT_VERSION} 
It is replaced with CFBundleShortVersionString in Info.plist 
${BUILD_DATE} 
It is replaced with date on which the application was built (yyyy.MM.dd) 
${BASE_NAME}${VERSION}${SHORT_VERSION}${BUILD_DATE} 这样更完整
```
Output directory 导出的ipa放在哪里
${WORKSPACE}/build/ 相当于 /Users/houjianan/.jenkins/workspace/GAJenkins/build/
![](https://upload-images.jianshu.io/upload_images/1024878-df5653bea922dcd6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
DEE3E2E8-FF33-4EA5-BE7B-8BD8BE22B695.png
Keychain path 之前搞得login.keychain绝对路径
Keychain password 电脑登录密码
![](https://upload-images.jianshu.io/upload_images/1024878-6d45e85ae6d55800.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
Code signing & OS X keychain options
### Advanced Xcode build options
Xcode Workspace File（使用了Workspace） ![{WORKSPACE}/GAFastlane Xcode Project Directory（没使用Workspace填这个） Build output directory](https://math.jianshu.com/math?formula=%7BWORKSPACE%7D%2FGAFastlane%20Xcode%20Project%20Directory%EF%BC%88%E6%B2%A1%E4%BD%BF%E7%94%A8Workspace%E5%A1%AB%E8%BF%99%E4%B8%AA%EF%BC%89%20Build%20output%20directory){WORKSPACE}/build 固定写即可
![](https://upload-images.jianshu.io/upload_images/1024878-2a33433804c8ce67.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
1F4CDE6C-46B7-464F-9FF9-A36B3BB10ED6.png
### 构建后操作
Send build artifacts over SSH
`Source files 注意路径别选错了`
![](https://upload-images.jianshu.io/upload_images/1024878-463fbf858eee80f5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
EB45C54A-CF92-4458-8449-5B6F952FE824.png
操作上面之前 先全局配置Publish over SSH
Use password authentication, or use a different key 使用密码登录和私钥密钥登录都可以密钥没做测试
Name随便起，上图会选择这个名字
Hostname 域名 ip
Username 访问地址的用户名
Remote Directory 路径 这里如果配置的路径是最后ipa文件传入的文件夹 上图就就不用配置RemoteDirectory 就是说 下图和上图的RemoteDirectory合并才是最后ipa文件传入的目录
Port 具体多少看情况
Timeout (ms)默认就行 网速特别差就写大点
![](https://upload-images.jianshu.io/upload_images/1024878-c7f3b5a44375e888.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
96612D1E-74B7-4FA8-820C-C052184E64BF.png
![](https://upload-images.jianshu.io/upload_images/1024878-a09fd14f7eee3df2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
DF685A30-A894-4195-AA54-ED84577FEB3B.png
### 去构建
![](https://upload-images.jianshu.io/upload_images/1024878-b066a7d8c95a92fe.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/672/format/webp)
EC7CAD37-3A09-4AB8-8B33-442683C34A8A.png
构建#99次...成功了
```
Archiving dSYM
No dSYM file found in /Users/houjianan/.jenkins/workspace/GAJenkins/build/GAFastlane.xcarchive/dSYMs !
No dSYM file found in /Users/houjianan/.jenkins/workspace/GAJenkins/build/GAFastlane.xcarchive/dSYMs !
SSH: Connecting from host [[houjianandeMacBook-Pro.local](http://houjianandemacbook-pro.local/)]
SSH: Connecting with configuration [10.3.10.33] ...
SSH: Creating session: username [root], hostname [10.3.10.33], port [22]
SSH: Connecting session ...
SSH: Connected
SSH: Opening SFTP channel ...
SSH: SFTP channel open
SSH: Connecting SFTP channel ...
SSH: Connected
SSH: cd [/data/uwsgi/test/]
SSH: OK
SSH: cd [/data/uwsgi/test/]
SSH: OK
SSH: mkdir [build]
SSH: OK
SSH: cd [build]
SSH: OK
SSH: put [GAFastlane.ipa]
SSH: OK
SSH: Disconnecting configuration [10.3.10.33] ...
SSH: Transferred 1 file(s)
Finished: SUCCESS
```
`--------------问题记录-------------------`
`[问题1]`
Source files 路径错误了 报下面错误
Transferred 0 ( 0 + 0 ) file(s)
`[问题2]`
** ARCHIVE FAILED **
The following build commands failed:
CompileSwift normal armv7
CompileSwiftSources normal armv7 [com.apple.xcode.tools.swift.compiler](http://com.apple.xcode.tools.swift.compiler/)
(2 failures)
Build step 'Xcode' marked build as failure
Finished: FAILURE
作者：GA_
链接：https://www.jianshu.com/p/3b38582d1a59
来源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
