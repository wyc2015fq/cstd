# 移动 App 云测试平台的对比与分析 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [ThoughtWorks](http://www.jobbole.com/members/thoughtworkschina) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
文章作者/配图来自ThoughtWorks：黄勇，未经允许，谢绝转载。
我们都知道在测试移动app时最耗时的是在各种测试设备进行测试， 因为不论是安卓还是iOS都已经碎片化了。而云测试看似是解决这一问题的有效途径。因此选择哪种云测试平台来协助测试人员进行各种测试就成为首要问题。
### 我们先来看看云测试平台通常都提供哪些功能和服务。
主流的云测试平台都支持对原生native，混合hybrid和Web app的测试，这些测试包括：
- 兼容测试。通过在多种测试设备上安装／卸载和运行被测app，遍历app的每个界面，主要检查app是否会报错或者崩溃。有些云测试平台还会对每个页面进行截图并进行对比。
- 脚本测试通过运行云测试平台工具进行录制的或者使用自动化测试框架编写的自动化脚本，实现模拟用户操作的目的，并且减少手动测试时间。
- 性能监控和分析利用Android SDK提供的借口，云测试平台可以检测移动app的耗电量，CPU等资源占用率，使用的流量等信息。有些云测试平台还提供自己的SDK，整合在app中可以提供更为准确的性能指标和信息，包括线上app的性能信息以及崩溃信息等。
- 手动测试和人工测试云测试平台的手动测试是指租用云测试平台的特定设备，测试人员手动登录设备进行测试。而人工测试则是将测试需求告知云测试平台的专业测试人员，雇佣他们临时作为自己的测试人员进行测试。
- 持续集成不少提供脚本测试的云测试平台都同时提供对持续集成（Continuous Integration）环境的支持。
此外不少国内云测试平台还提供以下功能：
- 安全测试
- 内测托管分发
- 众包测试
### 我们再来看看各种云测试平台对于上述功能和服务的支持情况。
由于国内外的云测试平台使用环境等因素的不同，我们分别对国内外主流的几个云测试平台进行对比。
##### 国外主流的云测试平台：
- Xamarin Test Cloud (https://xamarin.com/test-cloud/)
- TestDroid (http://testdroid.com/)
- Sauce Labs (https://saucelabs.com/mobile/)
- Google Cloud Test Cloud (https://developers.google.com/cloud-test-lab/)
- AWS Device Farm (https://aws.amazon.com/device-farm/)
![TestCloud-Foreign](http://ww4.sinaimg.cn/large/7cc829d3jw1f4ln3zeuwqj21kw0vhwm8.jpg)图1 － 国外主流的云测试平台对比
从上图我们可以看到一些特点：
- 在测试设备的数量上，Xamarin Test Cloud和Sauce Labs都是非常有优势的，虽然Xamarin Test Cloud统计的是测试设备的数量，而Sauce Labs是平台的数量；
- 亚马逊自己的FireOS只被自己的云测试平台支持，在国内我们也能看到类似的例子；
- 所有的云测试平台都支持app测试，但是只有TestDroid支持游戏测试；
- 对于国内云测试平台提供的人工测试，安全测试，内测分发和众包测试，国外这些云测试平台都是不支持的，需要结合别的工具和框架进行使用。不过对于手动测试，Sauce Labs和Perfecto这两个云测试平台支持租用测试设备进行手动测试；
- 对于云测试基础功能的兼容测试，以及脚本测试，崩溃分析和持续集成，这些云测试平台都是支持的；
- 只有Xamarin Test Cloud，TestDroid和AWS Device Farm支持性能监控；
- 对于脚本测试所使用的移动app自动化测试框架，每个平台都不甚相同：
- Xamarin Test Cloud支持Calabash（iOS和Android）和自己的Xamarin.UITest;
- TestDroid支持很多框架，包括支持iOS的Calabash，appium，UI Automation和 Jasmine，以及支持Android的Calabash，appium，Espresso，Robotium和uiautomator；
- Sauce Labs支持自己的开源框架appium；
- Google Cloud Test Lab则支持Espresso，Robotium和Robo test；
- AWS Device Farm也支持很多框架，包括支持iOS的Calabash，appium，UIAutomation和XCTest，以及支持Android的Calabash，appium，JUnit，Espresso，Robotium和uiautomator。
- Xamarin Test Cloud，TestDroid和Sauce Labs都有自己的移动app测试脚本录制工具，分别是：Xamarin Test Recorder，TestDroid Recorder和appium inspector。
**综合来看，对于国外的云测试平台，如果侧重的是测试设备的覆盖程度，选择Xamarin Test Cloud和Sauce Labs会更合适；如果需要测试FireOS设备，那就选择AWS Device Farm；如果侧重的是脚本测试中支持的语言和框架，那就可以选择TestDroid和AWS Device Farm；如果是进行游戏测试，只能选择TestDroid；如果要远程连接测试设备进行手动测试，那就需要选择Sauce Labs和Perfecto；如果在测试过程中需要同步监测性能，就不能选择Sauce Labs和Google Cloud Test Lab。**
##### 国内主流的云测试平台：
- Testin云测 (http://www.testin.cn/)
- 百度MTC (http://mtc.baidu.com/)
- 腾讯优测 (http://utest.qq.com/)
- 阿里MQC (http://mqc.aliyun.com/)
![TestCloud-Domastic](http://ww2.sinaimg.cn/large/7cc829d3jw1f4ln40hy38j20sg0ir413.jpg)图2 － 国内主流的云测试平台对比
从上图我们也可以看到一些特点：
- Testin云测支持的测试设备数量最多，达到了600部Android和70部iOS终端的数量；但是和Xamarin Test Cloud以及Sauce Labs支持的设备数量还是有不少差距的；
- 和亚马逊类似，阿里的YunOS也只有阿里MQC才能支持；
- 和国外的云测试平台很类似，这四个国内云测试平台也都支持app的云测试，而不支持游戏测试；只有Testin云测支持游戏测试；
- 对于云测试基础功能的兼容测试，国内主流云测试平台都是支持的；
- 这四个国内云测试平台也都支持崩溃分析，不过对于性能监控，却只有百度MTC支持，而且百度MTC的深度性能测试中还可以做竞品app的性能对比；
- Testin云测和百度MTC不支持手动测试；
- 只有阿里MQC不支持人工测试；
- 只有Testin云测不支持安全测试；对于支持安全测试的云测试平台，也没有公布是如何进行安全测试的；
- Testin云测支持内测分发和众包测试，阿里MQC支持众包测试，其它两个云测试平台对于内测分发和众包测试都不支持；
- 对于脚本测试，只有腾讯优测不支持；而对于测试工具和框架，各个平台的支持也不相同：
- Testin云测支持Robotium，JUnit，淘宝的Athrun和Testin SDK，其中只有Testin SDK支持iOS和Android，其他框架都只支持Android；
- 百度MTC只支持通过自己的测试脚本录制工具录制的脚本；
- 阿里MQC支持Robotium和增强后的appium，其中appium可以支持iOS和Android；
- Testin云测，百度MTC和阿里MQC都提供了自己的测试脚本录制工具，分别是itestin录制回放工具，百度MTC录制回放工具和易测；
- 国内云测试平台都没有提及持续集成，不过从笔者的了解看来，Testin云测和阿里MQC应该是都支持的。
**对于国内云测试平台，如果需要覆盖更多的测试设备或者需要测试游戏亦或需要内测分发，只能选择Testin云测；如果需要测试YunOS设备，那就需要选择阿里MQC；如果需要进行性能监控和竞品对比，那就选择百度MTC；如果要远程连接测试设备进行手动测试，那就需要选择腾讯优测和阿里MQC；如果需要雇佣云测试平台的专业测试人员，就不能选择阿里MQC；如果需要进行安全测试，就不能选择Testin云测；如果需要进行众包测试，那就选择Testin云测和阿里MQC；如果要进行脚本测试，就不能选择腾讯优测，对于百度MTC也不推荐。**
相信通过对比这些云测试平台提供的功能和服务，以及它们各自的特点，读者在选用云测试平台时有了更多的依据。希望大家在使用这些信息作为依据时，综合考虑这些云测试平台的特点，同时可以使用它们提供的免费试用进行尝试，以便验证是否真的适合自己的app。
**P.S.以上云测试平台提供的功能及服务，截止于2016年3月20日。**
