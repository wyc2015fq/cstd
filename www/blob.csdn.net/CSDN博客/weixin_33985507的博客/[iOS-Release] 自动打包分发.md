# [iOS-Release] 自动打包分发 - weixin_33985507的博客 - CSDN博客
2017年01月09日 10:20:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8
在 iOS 的日常开发中，需要不断的向测试人员提供最新的测试版本。这一过程的手动操作分为3步：
- 对应用 target 执行 archive 操作。
- 用 archive 产生的内容导出 Ad Hoc 包。
- 将导出的.ipa包上传至应用分发平台，以供测试人员安装。
这是一个重复性的且很耗时的操作，另外，通常都需要提供不止一种环境的测试版本，例如开发环境和生产环境等，这样就成倍增加了打包分发的时间。如果在 Xcode 中已设置好对应的签名文件后，使用脚本自动执行整个过程，则可以节省大量时间，提高工作效率。
整个操作过程用到两部分命令。一个是苹果公司提供的自动构建命令 [xcodebuild](https://link.jianshu.com?t=https://developer.apple.com/legacy/library/documentation/Darwin/Reference/ManPages/man1/xcodebuild.1.html)，使用该命令来进行 archive 与导出.ipa包的操作。另一部分是通过`curl`命令调用 API 来上传导出的.ipa包，具体的 API 使用方式需要查看你所使用的分发平台的 API 文档。
#### Archive 操作
因为大部分的应用都是在 workspace 中搭建的，所以以对 workspace 中的应用执行 archive 操作的命令为例，对于其他的构建方式，可以参考手册进行修改。
```
xcodebuild archive -workspace ApplicationName.xcworkspace -scheme ApplicationName -archivePath ./build/ApplicationName.xcarchive
```
命令中使用 -archivePath 将产生的 .xcarchive 文件指定到项目目录的 build 的路径下，以便后续的导出.ipa包的操作。另外，如果是对 project 中的应用打包，要在 ProjectName.xcodeproj 所在的目录执行命令，如果该目录下有多个 .xcodeproj，那么需要使用 -project 指定。
#### 导出 Ad Hoc 包
使用`xcodebuild -exportArchive`命令从上一步产生的 .xcarchive 文件中导出.ipa包，完整的命令如下：
```
xcodebuild -exportArchive -archivePath ./build/ApplicationName.xcarchive -exportPath ./build -exportOptionsPlist ad_hoc_options.plist
```
`-archivePath`指明了上一步产生的 .xcarchive 文件所在目录，`-exportPath`则是代表了导出的.ipa包存放的路径，同样放在项目目录的 build 的路径下，方便最后的清理删除。注意最后一个参数`-exportOptionsPlist`，指明了一个 .plist 文件的路径，该文件设置了导出.ipa包相关的一些参数，如 method=ad-hoc 指明了导出的.ipa包是通过Ad-Hoc方式进行分发的，可配置的参数可使用`xcodebuild --help`命令查看。ad_hoc_options.plist 文件中的内容如：
```
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE plist PUBLIC "-//Apple//DTD PLIST 1.0//EN" "http://www.apple.com/DTDs/PropertyList-1.0.dtd">
<plist version="1.0">
<dict>
    <key>method</key>
    <string>ad-hoc</string>
</dict>
</plist>
```
#### 上传.ipa包至分发平台
以蒲公英平台为例，根据[文档](https://link.jianshu.com?t=https://www.pgyer.com/doc/api#uploadApp)，可直接使用`curl`命令将导出的.ipa包上传至分发平台：
```
curl -F "file=@./build/ApplicationName.ipa" -F "uKey=your_user_key" -F "_api_key=your_api_key" -F "updateDescription=your_description" http://www.pgyer.com/apiv1/app/upload
```
#### 处理多个测试环境
通常情况下，开发中都有不止一种的测试环境，针对不同环境，可能有不同的域名或其他区别，最简单的可以分为开发和生产环境。对于不同的环境，我们可以对应设置不同的 Configuration，比如 Debug 对应开发环境，Release 对应生产环境。因为开始的 archive 操作是针对 scheme 的，所以我推荐针对每一种 Configuration 建立一个 scheme，这样就相当于每一种开发环境都有一个对应的 scheme。之后在脚本中初始化所有环境的 scheme，然后循环执行相关操作，就可以完成所有测试环境的打包分发工作了。
```
schemes=(ApplicationName-debug ApplicationName-release)
for scheme in ${schemes[@]}; do
# do something
done
```
#### 脚本
最终的 shell 脚本如下，配合[构建号的自动修改](https://www.jianshu.com/p/7cd81bf921bb)，可以很好的完成自动打包分发的工作。
```
cd ~/ApplicationName
schemes=(ApplicationName-debug ApplicationName-release)
for scheme in ${schemes[@]}; do
    xcodebuild archive -workspace ApplicationName.xcworkspace -scheme ${scheme} -archivePath ./build/${scheme}.xcarchive
    if [ $? -ne 0 ]; then
        continue
    fi
    xcodebuild -exportArchive -archivePath ./build/${scheme}.xcarchive -exportPath ./build -exportOptionsPlist ad_hoc_options.plist
    
    if [ $? -ne 0 ]; then
        continue
    fi
    if [ ${scheme} = "ApplicationName-debug" ]; then
        description="Develop Environment"
    elif [ ${scheme} = "ApplicationName-release" ]; then
        description="Production Environment"
    fi
    curl -F "file=@./build/${scheme}.ipa" -F "uKey=your_user_key" -F "_api_key=your_api_key" -F "updateDescription=${description}" http://www.pgyer.com/apiv1/app/upload
done
rm -rf ./build
```
#### ToDo
因为目前在工作中是一个人单独负责应用开发，且没有多余的机器作为 CI 的服务器，所以这种半自动化的方式已经满足需求了，后续再研究持续集成的内容。
