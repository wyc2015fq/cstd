# 使用终端命令进行xcode项目打包 - xqhrs232的专栏 - CSDN博客
2019年02月22日 16:31:14[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：74
原文地址::[https://blog.csdn.net/ycf03211230/article/details/53940852](https://blog.csdn.net/ycf03211230/article/details/53940852)
前言： 
一般项目都有开发环境和生产环境，甚至还有其他的环境，每次要更新了，都是先测试环境，再生产环境，而且每次一修改，都要重复打包，一会一个，好烦心呀～～～于是乎想着方便点的打包方式。 
1、先进入你项目所在的文件下下，比如我用在桌面上的一个测试项目TestQBImageVC
cd /Desktop/TestQBImageVC/
1
2、使用如下命令clean工程:
xcodebuild clean -project TestQBImageVC.xcodeproj -scheme TestQBImageVC -configuration Release
1
在执行这部分时，出现了如下错误
Failed to locate a valid instance of CoreSimulatorService in the bootstrap.  Adding it now.
2016-12-30 10:11:19.153 xcodebuild[5724:71896] *** Assertion failure in -[SimServiceContext reloadServiceIfMovedOrAbortIfWeAreInvalid], /BuildRoot/Library/Caches/com.apple.xbs/Sources/CoreSimulator/CoreSimulator-209.19/CoreSimulator/SimServiceContext.m:536
** INTERNAL ERROR: Uncaught exception **
Uncaught Exception: The loaded com.apple.CoreSimulator.CoreSimulatorService job does not match our expectations: pathOfLoadedJob: /Users/youchunfen/Downloads/Xcode.app/Contents/Developer/Library/PrivateFrameworks/CoreSimulator.framework/Versions/A/XPCServices/com.apple.CoreSimulator.CoreSimulatorService.xpc/Contents/MacOS/com.apple.CoreSimulator.CoreSimulatorService, our frameworkPath: /Applications/Xcode.app/Contents/Developer/Library/PrivateFrameworks/CoreSimulator.framework...........
1
2
3
4
5
由于我mac上安装了很多个版本的Xcode，解决方案如下，在终端输入如下命令：
launchctl remove com.apple.CoreSimulator.CoreSimulatorService || true
1
然后在执行如下命令，就会看到成功的提示。
xcodebuild clean -project TestQBImageVC.xcodeproj -scheme TestQBImageVC -configuration Release
1
注意：若是使用cocoapods安装了第三方，则项目中会多了个xxx.xcworkspace.所以打开应该是这个工作区间，用如下命令clean工程。
xcodebuild clean -workspace TestQBImageVC.xcworkspace -scheme TestQBImageVC -configuration Release
1
上面的命令： 
-project TestQBImageVC.xcodeproj：编译项目名称 
-workspace TestQBImageVC.xcworkspace:编译工作空间名称 
-scheme TestQBImageVC：scheme名称(与你的项目名称相同) 
configuration Release:(Debug/Release) 发布的选择Release
3、archive，导出.xcarchive文件,我是在当前目录下生成了一个TestQBImageVC.xcarchive
xcodebuild archive -project TestQBImageVC.xcodeproj -scheme TestQBImageVC -archivePath TestQBImageVC.xcarchive
1
2
执行成功的提示： 
然后在当前目下看到TestQBImageVC.xcarchive文件 
4、将.xcarchive导出ipa包，如下命令：
 xcodebuild -exportArchive -archivePath TestQBImageVC.xcarchive -exportPath TestQBImageVC -exportFormat ipa -exportProvisioningProfile "XXXXX"
1
执行成功提示： 
在当前文件下出现ipa
上面的命令： 
-archivePath ：是将刚才我们生成TestQBImageVC.xcarchive，注意它的路径 
-exportPath TestQBImageVC ：将要导出ipa文件的目录和文件名，我是存在当前文件夹的，文件名是TestQBImageVC 
-exportFormat ipa:导出ipa文件，格式是.ipa 
-exportProvisioningProfile “XXXX”:你配置证书的profile文件名称，一定要正确，否则报错
这样到处的ipa，可以放在第三方平台上（蒲公英等），当然也可以放在公司的服务器上，生成下载链接或者二维码，给测试人员测试。
参考： 
http://www.jianshu.com/p/2247f76404eb
