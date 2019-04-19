# Android兼容性测试框架(CTS)手册 - xqhrs232的专栏 - CSDN博客
2012年03月14日 16:13:58[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：936
原文地址::[http://blog.csdn.net/a345017062/article/details/6408871](http://blog.csdn.net/a345017062/article/details/6408871)
pdf文档::android-cts-manual-r4.pdf----[http://wenku.baidu.com/view/4f4e50b765ce050876321332.html](http://wenku.baidu.com/view/4f4e50b765ce050876321332.html)
本文翻译自Android官方的CTS手册android-cts-manual-r4.pdf
Android兼容性测试框架手册
![图1](http://hi.csdn.net/attachment/201105/10/0_1305003692FL79.gif)
1、为什么需要兼容性测试（以下称CTS）？
1.1、让APP提供更好的用户体验。用户可以选择更多的适合自己设备的APP。让APP更稳定。
1.2、让开发者设计更高质量的APP。
1.3、通过CTS的设备可以运行Android market。
另外，CTS是免费的，而且很简单。
2、怎么样才能让我的设备变成兼容的。
![](http://hi.csdn.net/attachment/201105/10/0_1305003699FuQi.gif)
2.1、与Android Compatibility Definition文档（以下简称ACD）匹配
先看一下与你的系统版本相匹配的ACD，文档中描述了你需要具备的软件和硬件参数。
2.2、通过CTS测试
CTS是开源的测试框架，使用它来测试你的设备是否具备兼容性。
2.3、提交报告
你可以把测试结果发送给cts@android.com。当你提交了一个CTS报告时，你还可以要求访问Android Market.
我们正在准备专门用途测试和认证的网站和相应服务，好了之后会通知大家的。
3、CTS的工作原理
![](http://hi.csdn.net/attachment/201105/10/0_1305003705dX3q.gif)
CTS主要包含两个组件：
运行在PC上的测试框架组件。主要用来管理测试用例（test case）的执行。
运行在设备或模拟器上的测试用例。这些用例用JAVA写成的APK文件。
3.1、工作流程
1、可以从源码编译CTS，也可以从网站下载编译好的CTS
2、安装配置CTS。
3、把设备连接到PC上。
4、运行CTS。CTS会把相应的测试用例（也就是一个APK文件）传送到设备上并通过instrumentation运行，然后记录运行结果，最后删除测试用例。
所有的测试用例执行完毕后，可以参照测试结果重新调整或优化系统。然后继续运行CTS测试。
5、测试通过以后，你可以把CTS生成的结果（就是result下面那个以测试时间命名的.zip文件）提交给cts@android.com。
- - 
测试用例的类型
CTS包含下面三种级别：
1、单元级。测试Android平台上的代码单元。比如，一个java.util.HashMap这样的类。
2、功能级。多个API组合而成的一个更高级的功能。
3、程序级。通过运行一个简单的APP来执行一个API集合和Android运行时服务。
未来版本还会包含下面的类型：
1、强度测试。测试系统在高CPU运算之下的稳定性。
2、效率测试。比如每秒渲染的帧数。
3.3、目前的测试覆盖面
目前，为了确保兼容，测试用例覆盖了下面的这些范围
1、Signature
对每个Android产品，都有一些XML文件来描述所有的公开API。CTS包含了一个工具来检测这些API签名中包含的API是否都在系统中被支持。
2、Platform
测试SDK文档中描述的平台API，比如core libraries,Android Application Framework等。要求这些API可以提供：
正确的类、属性、方法签名、方法行为、错误参数处理方式
3、Dalvik VM
专门针对Dalvik Vm的测试。
4、Platform Data Model
平台通过ContentProvider提供给开发者使用的数据，比如：Contacts,Browser,Settings等。
5、Platform Intents
平台提供的用于核心功能的Intent。
6、Platform permission
平台提供的一些重要APP权限
7、Platform Resources
simple values,drawables,nine-patch,animations,layouts,styles and themes，loading alternate resources等
4、配置和使用CTS
4.1、配置CTS
1.6及以上版本才能运行CTS。
解压ZIP包，编辑android-cts/tools/startcts脚本，修改变量SDK_ROOT来与环境相匹配。
例如：
SDK_ROOT=/home/myuser/android-sdk-linux_x86-1.6_r1
即，指向SDK的根目录。
4.2、配置设备
下面的这些说明很重要，配置不当的话可能导致测试超时或测试失败：
1、下载SDK到机器上。
2、你要测试的设备应该运行的是一个user build。
3、参考这个链接（[http://developer.android.com/guide/developing/device.html](http://developer.android.com/guide/developing/device.html)）来设置一下你的设备。
4、运行CTS之前，确保你的设备已经烧入了一个user build
5、在运行CTS测试之前还需要通过Settings->Speech Synthesis->Install voice data来下载TTS文件。如果没有安装Android
 Market的话，需要手动安装。
6、建议你使用一个专门用来测试的Google账户来登录设备。
7、确保设备有一个SD卡，并且SD卡是空的。因为CTS可能修改／删除SD卡上的数据。
8、在设备上做一次恢复出厂设置（Settings->SD Card & phone Storage->Factory data reset）。注意：这会删除设备上的所有用户数据。
9、确保设备没有处在任何lock pattern之下（取消Settings->Security&location->Require Pattern这个选项）
10、确保“Screen Timeout”被设置为“Never Timeout”（Settings->Sound&Display->Screen Timeout应该被设置为“Never
 Timeout”）
11、确保“Stay Awake”被选中（Settings->Applications->Development->Stay awake）
12、确保Settings->Application->Development->Allow mock locations被设置为true。
13、在运行CTS时，设备停留在桌面上。
14、当设置正在进行测试时，绝不能执行其它任务。
15、CTS运行时不要按任何键或触摸屏幕。
4.3、使用CTS
运行一个test plan需要：
1、至少有一个设备或模拟器连接到PC上。然后运行脚本android-cts/tools/startcts
2、你可以通过执行start –plan CTS来执行默认的test plan。这个test plan包含所有的测试用例。
使用ls -p可以查看repository
中包含的测试用例的列表。
使用ls –plan可以查看repository
包含的test plan列表。
3、还可以运行startcts start –plan <plan_name>来执行一个指定的test plan。
4.4、选择CTS Plan
当前CTS版本中，一共包含下面7个test plan。
1、CTS
包含所有的测试用例，大约会在设备上执行121000个测试。
2、Signature
包含对所有公开API的签名认证。
3、Android
包含对Android平台API的测试。
4、Java
包含对Java核心库API的测试。
5、VM
包含对Dalvik Vm的测试。
6、RefApp
包含对相关应用程序的测试
7、Performance
包含对系统性能的测试。
5、解释一下测试结果
测试结果被放在$CTS_ROOT/repository/results/<start time>.zip
。在这个ZIP文件中，testResult.xml文件包含了真正的测试结果，用浏览器打开这个文件可以看到下面这样的效果
![](http://hi.csdn.net/attachment/201105/10/0_1305003710zTLR.gif)
其中，“device information”部分提供了设备和固件的细节（如make,model,firmware build,platform等）和设备的硬件配置（屏幕参数、键盘、屏幕类型等）。
另外，“test summary”部分提供了test plan的执行细节描述，包含CTS plan name和执行的开始、结束时间。还有测试结果的统计，包含测试通过、失败、超时、不能执行的个数。
下面的这个表格统计了一个包中的测试用例有多少测试通过。
![](http://hi.csdn.net/attachment/201105/10/0_13050037153K13.gif)
这个表格的后面还跟着对执行结果更具体的描述。
这个报告列出了test package,test suite,test case和执行的测试，还有测试执行的结果：通过、失败、超时、未执行。当测试失败时，可以在XML文件中找到stack trace，为了让执行结果更简洁，这些stack
 trace没有被包含进来。用文本编辑器查看XML文件，在里面搜索一下<Test>标签和<StackTrace>标签就可以了。
6、注意事项
CTS在测试时会重启设备，这是正常现象。
CTS一次只能在一个设备上执行。
运行CTS时可能会出现弹出强制关闭对话框，要求用户选择关闭或者等待。重新运行这个测试一般就会好的。
以下有几篇文章，讲得比较简洁，可以先看一下这几篇再阅读本文，那样理解起来会容易一些
http://chenhuawei1234567.blog.163.com/blog/static/194526712010629111638224/
http://blog.csdn.net/zjujoe/archive/2010/06/01/5640461.aspx
