# mac OS 下超详细搭建 Appiumm 自动化测试环境 - zhusongziye的博客 - CSDN博客





2018年11月09日 21:08:35[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：246








Appium 在 mac OS 环境搭建

Appium 一年之前相比，有了很大的改变

1、iOS 9 之前一直以 instruments 下的 UIAutomation 为驱动底层技术（弊端由于 instruments 的限制，单台 mac 只能对应单台设备）；

2、iOS 9.3 时代推出 XCUITest 工具，用以替代 UIAutomation；

3、iOS 10 时代苹果直接废弃了 UIAutomation、Facebook 推出 WebDriverAgent（实现的 server 能够支持单台 mac 对应多个设备）；

4、Appium 在iOS 9.3 后全面采用 WebDriverAgent 的方案。



首先，我们要知道环境搭建所需工具：

![](https://img-blog.csdnimg.cn/20181109210345842.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

其次，重中之重：版本很重要，xcode9.3 + node10.11.0 + appium1.7.0，如果根据以下配置仍然不能成功运行，那么99%是版本的问题。



下面开始逐个安装

1、安装node

brew install node

通过这种方法安装的node是最新版本，因为我本地需要有node 8.1.1版本，所以使用nvm进行安装。



2、下载Xcode 和 Command Line Tools：需要有个AppleID，并根据自己电脑OS版本。

![](https://img-blog.csdnimg.cn/20181109210418289.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

下载后直接安装即可，注意Xcode保持在application文件夹下只有一个唯一版本。



3、下载安装JDK

![](https://img-blog.csdnimg.cn/20181109210436278.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

下载完成后直接安装即可。



4、安装Android SDK

很多资料都推荐使用brew安装，但是实践后发现SDK文件为空，所以使用Android studio来安装。

如果打开Android studio报错，直接cancel到下一步即可；若使用default方式安装，也可以在configuration里重新下载相关tools。



![](https://img-blog.csdnimg.cn/2018110921045746.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/2018110921051226.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



5、安装appium最新版本

![](https://img-blog.csdnimg.cn/20181109210524230.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

这里不使用npm下载方式是因为下载过程较长，且容易报错。



6、安装appium-doctor

npm install -g appium-doctor



7、配置Android环境变量

在 ~/.bash_profile文件中添加path



export ANDROID_HOME=~/Library/Android/sdk/

export JAVA_HOME=/Library/Java/JavaVirtualMachines/jdk1.8.0_171.jdk/Contents/Home #jdk安装路径  

export PATH=~/bin:$PATH:/usr/local/bin:$ANDROID_HOME/platform-tools/:$JAVA_HOME/bin

export CLASSPATH=.:$JAVA_HOME/lib/dt.jar:$JAVA_HOME/lib/tools.jar

在appium的configurations里设置

![](https://img-blog.csdnimg.cn/20181109210551811.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



8、安装其他依赖

brew install libimobiledevice --HEAD

npm install -g ios-deploy

brew install carthage

libimobiledevice / ideviceinstaller 库：相当于 android 的 adb，是 Appium 底层用到的工具之一，用于获取 iOS 设备信息

ios-deploy：查看设备应用

carthage：类似于 java 的 maven，项目依赖管理，主要是 WebDriverAgent 使用



9、配置WebDriverAgent

（1）首先进入WebDriverAgent所在文件目录下：

/Applications/Appium.app/Contents/Resources/app/node_modules/appium/node_modules/appium-xcuitest-driver/WebDriverAgent

（2）根目录下执行：

./Scripts/bootstrap.sh

（3）打开WebDriverAgent.xcodeproj工程文件

（4）添加自己的account：Xcode -> Preferences -> account -> 左下角 +

![](https://img-blog.csdnimg.cn/20181109210625999.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



（5）更改bundle id：这里的bundle id就相当于一个App的唯一识别号，更改的规则随意，只要确认不会与有重复即可；所有的targets都需要修改。

![](https://img-blog.csdnimg.cn/20181109210639391.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



![](https://img-blog.csdnimg.cn/20181109210648669.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



10、验证是否环境已经安装成功了

（1）终端输入：appium-doctor

如果环境都已配置好，应该是文章最开始图片中的结果。

（2）运行WebDriverAgentRunner

![](https://img-blog.csdnimg.cn/20181109210704826.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)

按照图中，1:选择scheme为WebDriverAgentRunner -> 2：选择运行设备，模拟器和真机都可以 -> 3：选择要构建的方式 ->4：选择Test方式的步骤验证是否能成功运行，WebDriverAgentRunner

如果真机运行后弹出需要在设置中信任证书。

Verify the Developer App certificate for your account is trusted on your device. Open Settings on iPhone and navigate to General -> Device Management, then select your Developer App certificate to trust it.

按照步骤操作后，重新运行就可以了。

![](https://img-blog.csdnimg.cn/20181109210723571.png)



成功运行结果图

![](https://img-blog.csdnimg.cn/20181109210734852.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



11、遇到的问题总结

1）appium版本不支持xcode版本（需要调用XCUITest)，重新下载可匹配的Xcode版本。

Xcode9.3 + Appium1.6.4时，运行WebDriverAgentRunner报错。

Early unexpected exit, operation never finished bootstrapping - no restart will be attempted

![](https://img-blog.csdnimg.cn/20181109210751776.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



解决方法：更新Appium版本到1.7.0



2）appium无法调起webdriverrunner（webdriveragent更新），查看是否因为webdriveragent证书签名、依赖framework、serverURL抛出错误信息。

安装依赖framework时报错

Falling back to building from the source

A shell task (/usr/bin/xcrun xcodebuild -project /Applications/Appium.app/Contents/Resources/app/node_modules/appium/node_modules/appium-xcuitest-driver/WebDriverAgent/Carthage/Checkouts/CocoaAsyncSocket/CocoaAsyncSocket.xcodeproj CODE_SIGNING_REQUIRED=NO CODE_SIGN_IDENTITY= CARTHAGE=YES -list) failed with exit code 72:

xcrun: error: unable to find utility "xcodebuild", not a developer tool or in PATH

解决方法：选择Command Line Tools

![](https://img-blog.csdnimg.cn/20181109210810484.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll,size_16,color_FFFFFF,t_70)



Xcode -> Preferences -> Locations -> Command Line Tools。



3）长时间测试，server中断，这个无法解决，appium自身问题。



4）webdriveragent启动失败，status、inspector无法正常查看，查看版本。



5）appium脚本启动失败，查看脚本中的配置



