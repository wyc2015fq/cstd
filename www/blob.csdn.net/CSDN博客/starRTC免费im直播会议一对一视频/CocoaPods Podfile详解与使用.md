# CocoaPods Podfile详解与使用 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年07月10日 13:52:50[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：197
**1、为什么需要CocoaPods**
在进行[**iOS**](http://lib.csdn.net/base/ios)开发的时候，总免不了使用第三方的开源库，比如SBJson、AFNetworking、Reachability等等。使用这些库的时候通常需要：
- 下载开源库的源代码并引入工程
- 向工程中添加开源库使用到的framework
- 解决开源库和开源库以及开源库和工程之间的依赖关系、检查重复添加的framework等问题
- 如果开源库有更新的时候，还需要将工程中使用的开源库删除，重新执行前面的三个步骤，顿时头都大了。。。
自从有了CocoaPods以后，这些繁杂的工作就不再需要我们亲力亲为了，只需要我们做好少量的配置工作，CocoaPods会为我们做好一切！
**2、什么是CocoaPods**
CocoaPods是一个用来帮助我们管理第三方依赖库的工具。它可以解决库与库之间的依赖关系，下载库的源代码，同时通过创建一个Xcode的workspace来将这些第三方库和我们的工程连接起来，供我们开发使用。
使用CocoaPods的目的是让我们能自动化的、集中的、直观的管理第三方开源库。
安装
CocoaPods是用Ruby实现的， CocoaPods是以Ruby gem包的形式被安装的。
 sudo gem install cocoapods  
 pod setup  
查看源：
gem sources -l  
替换源
gem是管理Ruby库和程序的标准包
- $ sudo gem update --system  
- gem -v
- $ gem sources --add https://gems.ruby-china.org/ --remove https://rubygems.org/$ gem sources -lhttps://gems.ruby-china.org# 确保只有 gems.ruby-china.org
进入工程根目录，然后执行pod install命令，CocoaPods就开始为我们做下载源码、配置依赖关系、引入需要的framework等一些列工作


The Podfile is a specification that describes the dependencies of the targets of one or more Xcode projects.即Podfile文件用来配置第三方库与一或多个Xcode项目之间的依赖信息。
Podfile
platform:ios, '8.0'
CocoaPods就是用于管理第三方依赖的。我们通过Podfile文件来指定工程中的每个target之间与第三方之间的依赖。
有以下三个命令来管理依赖：
- pod 指定特定依赖。比如指定依赖AFNetwroking
- podspec 提供简单的API来创建podspec
- target 通过target指定工程所依赖的范围。
pod命令用于指定工程所依赖的第三方及第三方库的版本范围。
**永远使用最新版本**
pod'HYBMasonryAutoCellHeight'
当我们永远使用远程仓库中的最新版本时，我们只需要指定仓库名即可。当有新的版本发布时，执行pod update命令，会更新至最新的版本。因为版本之间可能会存在很大的差异，因此我们不应该采用这种方式，而是指定版本范围或者指定特定版本。
**指定版本范围**
pod'HYBUnicodeReadable','~>1.1.0'
在工程中见到最多的就是这种写法。
意思是：HYBUnicodeReadable的版本可以是1.1.0到2.0.0，但是不包括2.0.0
**使用固定版本**
pod'HYBLoopScrollView','2.0'
经常使用CocoaPods来管理iOS项目中的第三方库，但是我们要使用CocoaPods来管理第三方库，前提是要写好Podfile文件，通过这个文件来配置第三方库与项目之间的依赖、版本等信息。
可以看到，工程的根目录下多了三个东西：CocoaPodsTest.xcworkspace、Podfile.lock文件和Pods目录。
执行完pod install命令打印出来的内容的最后一行：
**[objc]**[view
 plain](http://blog.csdn.net/wzzvictory/article/details/18737437#)[copy](http://blog.csdn.net/wzzvictory/article/details/18737437#)
- [!] From now on use `CocoaPodsTest.xcworkspace`.  
提示我们从现在起，我们需要使用CocoaPodsTest.xcworkspace文件来开发。
对于工程发生的变化，有几点需要说明：
- 第三方库会被编译成静态库供我们正真的工程使用
CocoaPods会将所有的第三方库以target的方式组成一个名为Pods的工程，该工程就放在刚才新生成的Pods目录下。整个第三方库工程会生成一个名称为libPods.a的静态库提供给我们自己的CocoaPodsTest工程使用。
- 我们的工程和第三方库所在的工程会由一个新生成的workspace管理
为了方便我们直观的管理工程和第三方库，CocoaPodsTest工程和Pods工程会被以workspace的形式组织和管理，也就是我们刚才看到的CocoaPodsTest.xcworkspace文件。
原来的工程设置已经被更改了，这时候我们直接打开原来的工程文件去编译就会报错，只能使用新生成的workspace来进行项目管理。
打开CocoaPodsTest.xcworkspace

执行完pod install之后，会生成一个Podfile.lock文件。这个文件看起来跟我们关系不大，实际上绝对不应该忽略它。该文件用于保存已经安装的Pods依赖库的版本
CocoaPods是用ruby实现的，因此Podfile文件的语法就是ruby的语法。
**CocoaPods常用命令**
**1、pod install**
根据Podfile文件指定的内容，安装依赖库，如果有Podfile.lock文件而且对应的Podfile文件未被修改，则会根据Podfile.lock文件指定的版本安装。
每次更新了Podfile文件时，都需要重新执行该命令，以便重新安装Pods依赖库。
错误：
pod command not found
sudo gem install cocoapods
ERROR: SSL verification error at depth 1: unable to get local issuer certificate (20)
ERROR: You must add /O=Digital Signature Trust Co./CN=DST Root CA X3 to your local trusted store
ERROR: SSL verification error at depth 2: self signed certificate in certificate chain (19)
ERROR: Root certificate is not trusted (/C=US/O=GeoTrust Inc./CN=GeoTrust Global CA)
ERROR: Could not find a valid gem 'xcodeproj' (< 2.0, >= 1.4.4) in any repository
ERROR: Possible alternatives: xcodeproj
解决：
0.首先要安装[Homebrew](http://brew.sh/)终端输入这条命令即可/usr/bin/ruby
 -e "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/master/install)"
1.安装rvm$
 curl -L get.rvm.io | bash -s stable
2.装载rvm$
 source ~/.rvm/scripts/rvm
3.安装2.3.0版本ruby$
 rvm install 2.3.0
4.将2.3.0设为默认$
 rvm use 2.3.0 --default
然后重新执行安装，第一次如果报错，就再运行一次。
[http://blog.csdn.net/blackwolfsky/article/details/51217251](http://blog.csdn.net/blackwolfsky/article/details/51217251)
[http://www.tuicool.com/articles/6ZBvQbY](http://www.tuicool.com/articles/6ZBvQbY)
[http://blog.csdn.net/wzzvictory/article/details/18737437](http://blog.csdn.net/wzzvictory/article/details/18737437)
[http://blog.csdn.net/wzzvictory/article/details/19178709](http://blog.csdn.net/wzzvictory/article/details/19178709)
[http://blog.csdn.net/wzzvictory/article/details/20067595](http://blog.csdn.net/wzzvictory/article/details/20067595)
[http://www.jianshu.com/p/17043faee2e6](http://www.jianshu.com/p/17043faee2e6)
