# iOS中使用Fastlane实现自动化打包和发布 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年05月23日 13:14:19[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：503标签：[iOS 资源大全																[iOS学习资料																[iOS自动打包和发布](https://so.csdn.net/so/search/s.do?q=iOS自动打包和发布&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS学习资料&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS 资源大全&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
##### 本文来自简书，原文地址：[http://www.jianshu.com/p/192c09cc8e27](http://www.jianshu.com/p/192c09cc8e27)
### 简介
Fastlane是一套使用Ruby写的自动化工具集，用于iOS和Android的自动化打包、发布等工作，可以节省大量的时间。
![](http://upload-images.jianshu.io/upload_images/1608265-f63702702cfa790f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Fastlane
Github：[https://github.com/fastlane/fastlane](https://github.com/fastlane/fastlane)
官网：[https://fastlane.tools/](https://fastlane.tools/)
文档：[https://docs.fastlane.tools/](https://docs.fastlane.tools/)
在写这篇文章的时候，他在Github上已经有15000+ star。
### 安装
1、首先要安装正确的 Ruby 版本。在终端窗口中用下列命令来确认:
`ruby -v`
2、然后检查 Xcode 命令行工具是否安装。在终端窗口中输入命令：
`xcode-select --install`
如果未安装，终端会开始安装，如果报错误：
```
command line tools are already installed,
 use "Software Update" to install updates.
```
代表已经安装。
3、以上依赖配置好之后就可以通过 rubygem 进行安装了：
`$ sudo gem install fastlane`
安心等待一会，fastlane就安装完成了。
### 初始化
打开终端，cd到你的工程目录，然后执行`fastlane init`：
```
$ cd to/your/ios/project 
$ fastlane init
[14:21:43]: Detected iOS/Mac project in current directory...
[14:21:43]: This setup will help you get up and running in no time.
[14:21:43]: fastlane will check what tools you're already using and set up
[14:21:43]: the tool automatically for you. Have fun! 
[14:21:43]: Created new folder './fastlane'.
[14:21:43]: $ xcodebuild -showBuildSettings -project ./xxx.xcodeproj
[14:21:48]: Your Apple ID (e.g. fastlane@krausefx.com): xxx@xxx.xom
[14:21:54]: Verifying that app is available on the Apple Developer Portal and iTunes Connect...
[14:21:54]: Starting login with user 'xxx@xxx.com'
+----------------+--------------------------------------+
|                    Detected Values                    |
+----------------+--------------------------------------+
| Apple ID       | xxx@xxx.com                    |
| App Name       | xxx                            |
| App Identifier | com.xxx.xxx                |
| Project        | /Users/lisong/Desktop/xxx/x  |
|                | xx.xcodeproj                     |
+----------------+--------------------------------------+
[14:22:06]: Please confirm the above values (y/n)
y
[14:22:09]: Created new file './fastlane/Appfile'. Edit it to manage your preferred app metadata information.
[14:22:09]: Loading up 'deliver', this might take a few seconds
[14:22:09]: Login to iTunes Connect (xxx@xxx.com)
[14:22:13]: Login successful
+-----------------------+------------------------+
|             deliver 2.30.1 Summary             |
+-----------------------+------------------------+
| screenshots_path      | ./fastlane/screenshots |
| metadata_path         | ./fastlane/metadata    |
| username              | xxx@xxx.com      |
| app_identifier        | com.xxx.xxx  |
| edit_live             | false                  |
| platform              | ios                    |
| skip_binary_upload    | false                  |
| skip_screenshots      | false                  |
| skip_metadata         | false                  |
| force                 | false                  |
| submit_for_review     | false                  |
| automatic_release     | false                  |
| dev_portal_team_id    | WKR87TTKML             |
| overwrite_screenshots | false                  |
+-----------------------+------------------------+
[14:22:21]: Writing to 'fastlane/metadata/zh-Hans/description.txt'
...
[14:22:21]: Writing to 'fastlane/metadata/review_information/notes.txt'
[14:22:21]: Successfully created new configuration files.
[14:22:22]: Successfully downloaded large app icon
[14:22:22]: Downloading all existing screenshots...
[14:22:27]: Downloading existing screenshot '1_iphone4_1.1.jpg' for language 'zh-Hans'
···
[14:22:34]: Downloading existing screenshot '5_iphone6_5.5.jpg' for language 'zh-Hans'
[14:22:34]: Successfully downloaded all existing screenshots
[14:22:34]: Successfully created new Deliverfile at path 'fastlane/Deliverfile'
[14:22:34]: $ xcodebuild -list -project ./xxx.xcodeproj
[14:22:35]: 'snapshot' not enabled.
[14:22:35]: 'cocoapods' enabled.
[14:22:35]: 'carthage' not enabled.
[14:22:35]: Created new file './fastlane/Fastfile'. Edit it to manage your own deployment lanes.
[14:22:35]: fastlane will collect the number of errors for each action to detect integration issues
[14:22:35]: No sensitive/private information will be uploaded
[14:22:35]: Learn more at https://github.com/fastlane/fastlane#metrics
[14:22:35]: Successfully finished setting up fastlane
```
在 "Your Apple ID" 这一步输入苹果开发者账号。在“Please confirm the above values”这一步，确认信息，没问题输入 y。然后，fastlane 会进行一系列的初始化操作，包括下载 App Store 上的元数据和截屏文件。
等待初始化完成之后，工程目录下就多了一个 fastlane目录，其内容如下：
![](http://upload-images.jianshu.io/upload_images/1608265-812d184b57793510.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
fastlane目录
咱们来看两个主要的，Appfile和Fastfile。
##### Appfile
Appfile用来存放app_identifier，apple_id和team_id。 [了解详情](https://github.com/fastlane/fastlane/blob/master/fastlane/docs/Appfile.md)，它的格式是这样的：
```
app_identifier "com.xxx.xxx" # app的bundle identifier
apple_id "xxx@xxx.com" # 你的Apple ID
team_id "XXXXXXXXXX" # Team ID
···
```
你也可以为每个lane(后面会讲到)提供不同的 app_identifier, apple_id 和 team_id，例如：
```
app_identifier "com.aaa.aaa"
apple_id "aaa@aaa.com"
team_id "AAAAAAAAAA"
for_lane :inhouse do
  app_identifier "com.bbb.bbb"
  apple_id "bbb@bbb.com"
  team_id "AAAAAAAAAA"
end
```
这里就是为Fastfile中定义的:inhouse设置单独的信息。
##### Fastfile
Fastfile管理你所创建的 lane， [了解详情](https://github.com/fastlane/fastlane/tree/master/fastlane/docs#fastfile)。它的格式是这样的：
```
```
···
# 自动更新fastlane 工具
# update_fastlane
#需要的fastlane的最小版本，在每次执行之后会检查是否有新版本，如果有会在最后末尾追加新版本提醒
fastlane_version "2.30.1"
#默认使用平台是 ios，也就是说文件可以定义多个平台
default_platform :ios
platform :ios do
  before_all do
    # ENV["SLACK_URL"] = "https://hooks.slack.com/services/..."
    cocoapods
  end
  desc "Runs all the tests"
  lane :test do
    scan
  end
  desc "提交一个新的Beta版本到 Apple TestFlight"
  desc "This will also make sure the profile is up to date"
  lane :beta do
    # match(type: "appstore") # more information: https://codesigning.guide
    gym(scheme: "Docment") # Build your app - more options available
    pilot
    # sh "your_script.sh"
  end
  desc "部署一个新版本到App Store"
  lane :release do
    # match(type: "appstore")
    # snapshot
    gym(scheme: "Docment") # Build your app - more options available
    deliver(force: true)
    # frameit
  end
  # 你可以定义自己的lane
  #执行lane成功后的回调
  after_all do |lane|
    # slack(
    #   message: "Successfully deployed new App Update."
    # )
  end
  # 如果流程发生异常会走这里并终止
  error do |lane, exception|
    # slack(
    #   message: exception.message,
    #   success: false
    # )
  end
end
```
```
我们也可以定义一个自己的lane：
```
desc "企业版"
  lane :inHouse do
  gym(scheme: "XXX",
      export_method:"enterprise",
      output_directory "./build", # 打包后的 ipa 文件存放的目录
      output_name "XXX"  # ipa 文件名
   )
  end
```
其中一个lane就是一个任务，里面是一个个的action组成的工作流。
利用目前支持的工具可以做所有包含自动化和可持续化构建的每个环节，例如：
> 
[scan](https://github.com/fastlane/fastlane/tree/master/scan) 自动化测试工具，很好的封装了 Unit Test
[sigh](https://github.com/fastlane/fastlane/tree/master/sigh) 针对于 iOS 项目开发证书和 Provision file 的下载工具
[match](https://github.com/fastlane/fastlane/tree/master/match) 同步团队每个人的证书和 Provision file 的超赞工具
[gym](https://github.com/fastlane/fastlane/tree/master/gym) 针对于 iOS 编译打包生成 ipa 文件
[deliver](https://github.com/fastlane/fastlane/tree/master/deliver) 用于上传应用的二进制代码，应用截屏和元数据到 App Store
[snapshot](https://github.com/fastlane/fastlane/tree/master/snapshot) 可以自动化iOS应用在每个设备上的本地化截屏过程
### 执行
定义完lane之后怎么执行呢？打开终端，切换到项目的根目录：执行`fastlane lane'name`就可以了。成功之后会在相应的路径下生成ipa文件，如果报错的话就根据错误信息好好查看[文档](https://docs.fastlane.tools/)。
### 其他
1、这里是官方提供的一些[例子](https://github.com/fastlane/examples)。
2、想了解fastlane命令的话可以执行`$ fastlane --help`
3、查看可用任务的列表，可以执行命令`$ fastlane lanes`
4、fastlane也提供了很多[插件](https://docs.fastlane.tools/actions/#plugins)方便我们使用，例如pgyer(发布app到蒲公英)。我们也可以打完包直接传到蒲公英上，具体的可以看蒲公英提供的[文档](https://www.pgyer.com/doc/view/fastlane)。
如果你感觉有些插件不符合自己的情况，你甚至可以[自定义插件](https://docs.fastlane.tools/plugins/create-plugin/)
5、多个 lane 的话实际上是可以相互调用的，这个其实特别实用。例如：
```
```
default_platform :ios
platform :ios do
  lane :prepare do
    cocoapods
    match
  end
  desc 'fastlane build'   'fastlane build type:adhoc'
  lane :build do |options|
    # 调用上面的 prepare 任务
    prepare
    case options[:type]
    when 'adhoc'
      adhoc
    else
      appstore
    end
  end
  lane : adhoc do
  ···
  end
  lane : appstore do
  ···
  end
end
```
```
6、我们可以在 Fastfile 文件中添加一个函数来设置version号和build号。
```
```
default_platform :ios
def prepare_version(options)
    increment_version_number(
        version_number: options[:version]
    )
    increment_build_number(
        build_number: options[:build]
    )
end
```
```
然后可以在一个lane中使用这个函数：
```
lane :appstore do |options|
   ···
    prepare_version(options)
   ···
end
```
然后执行这个lane的时候：
`$ fastlane appstore version:2.4.0 build:2.0`
好啦，先说到这里吧，Fastlane能做的事情还有很多，大家可以去好好看看[文档](https://docs.fastlane.tools/)，研究一些高级的用法吧！
