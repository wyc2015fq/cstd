# VR干货：教大家如何使用Oculus DK2！ - fighting！！！ - CSDN博客
2017年08月13日 15:36:45[dujiahei](https://me.csdn.net/dujiahei)阅读数：903
VR干货：教大家如何使用Oculus DK2！ http://vr.sina.com.cn/news/js/2016-06-13/doc-ifxszmaa1942948.shtml
Oculus官方和论坛新人贴都反反复复强调Oculus Rift Development Kit 2 叫做**开发者版本2代**
　　这篇教程虽不说是给开发者看的（估计开发者也根本不用看），但至少也要求你有基本的理解力和系统操作能力。
　　如果说你就是要即插即用，就是嫌DK2调试复杂、分辨率低，甚至连下面的教程都懒得认真看完的话，那么请把你手中的DK2卖给开发者，也就是真正需要它的人，真的！
**[安装前需知]**
　　DK2对电脑配置的要求较高，如果你想用DK2玩游戏和demo舒服不至于因为延迟而眩晕的话，请遵循以下原则：
**1、 不支持XP！不支持XP！**
　　不要问我为什么，懒得解释！win7、win8、win8.1都可以，至于MAC，教程留给别人去写吧。
**2、 高性能电脑**
　　因为驱动runtime的渲染目前不兼容笔记本的双显卡切换技术（Nvidia的Optimus技术 和AMD的Dual graphics技术）
　　在核显+独显的笔记本上DK2只能调用核心显卡进行渲染处理，未来的驱动是否能兼容双显卡技术依然未知，
　　所以现阶段想流畅的玩DK2，你需要台式机或双独立显卡的高性能笔记本电脑。
**3、 硬件配置越高越好！尤其是显卡！**
　　如果你的机器性能一般，那你只能比较流畅的运行简单的demo，遇到大型游戏或光影特效场景，你就等着吐吧。
　　目前根据群友的试验总结，推荐配置NV：GTX760起步，AMDR9-M280起步。
**4、 尽量不要用Ghost系统**
　　Ghost系统玩DK2出现的各种奇葩故障恕我无力解答！
**[DK2使用指南]**
　　好了，我们正式开始。
　　DK2的设置于运行复杂程度远超DK1，但是只要你能认真阅读并理解以下教程内容，玩转DK2并不是什么难事儿。
**一、驱动（runtime）的下载与安装**
　　当你拆封你的DK2后，不要先急着去点亮它的屏幕。
　　区别于DK1即插即用的方式，DK2是需要在电脑上安装对应的驱动软件才能够正常工作的。
　　确保你有一个好的电脑平台后，首先需要下载DK2的驱动：runtime。
　　Oculus官方在持续更新runtime版本，发布界面链接如下：https：//developer.oculusvr.com/？action=dl&v=21
　　也可以直接访问旮旯网站上的0.4.2驱动下载，免去官方网站注册和缓慢下载的麻烦，下载地址：http://www.galar.cn/app_detail.asp?id=34
![](http://n.sinaimg.cn/sina_vr/crawl/20160613/LLiW-fxszvys0670065.jpg)下载好的文件
　　如果你安装过老版本的runtime，那么请在安装新的runtime前把老版本的先卸掉，否则可能出现各种奇葩问题。
　　右键以管理员身份运行安装程序，建议暂时关闭杀毒软件，因为有部分群友反映杀毒软件会误报。
　　安装过程不再详写，接受许可条款并一路Next，安装路径选择默认路径即可。
![](http://n.sinaimg.cn/sina_vr/crawl/20160613/abpY-fxszmai2212023.jpg)安装路径选择
　　安装完成后提示要重启电脑，那就重启呗。
　　重启完成后，首先检查DK2相关控制软件及服务是否正常启动，先看通知区域图标里是否多出个小眼睛的图标：
![](http://n.sinaimg.cn/sina_vr/crawl/20160613/66dR-fxszmai2212025.jpg)RUNTIME图标
　　然后在任务管理器内检查相关服务进程是否正常启动（如下图）
　　其中32位系统启动的是OVRServer_X86，64位系统启动的是OVRServer_X64。
　　如果没有正常启动，需要你到runtime的安装路径：
　　C：\Program Files（x86）\Oculus\Service 手动启动OVRserviceLaucher。
![](http://n.sinaimg.cn/sina_vr/crawl/20160613/tq-v-fxszmai2212027.jpg)任务管理器内的相关服务进程
　　一切正常的话就可以进入下一步了。
**二、连接DK2**
　　具体连接方法请参照DK2盒子里的说明书。
　　虽说是英文，但是要是你连这种看图操作说明书都看不懂的话，请把DK2转卖给开发者吧…
　　连接好DK2和位置追踪摄像头后，如果你的设备正常，DK2上会亮起黄色的指示灯。
　　若没亮起，请按指示灯边上的开关测试并检查连线是否正常，主板供电是否正常（DK2是靠USB供电的）。
**三、使用OculusConfiguration Utility对DK2进行基础设置  **
超级重点章节，请仔细阅读！！！
　　在连接好DK2后，左击或右击通知区域的小眼睛图标，打开Rift Display Mode
![](http://n.sinaimg.cn/sina_vr/crawl/20160613/s8Io-fxszkzy5197673.jpg)打开Rift
 Display Mode
　　在打开的窗口中点选DK2的工作模式为ExtendDesktop to the HMD，点Apply应用，如图：
![](http://n.sinaimg.cn/sina_vr/crawl/20160613/pa8O-fxszmaa1942797.jpg)选择工作模式
　　这时候你会发现DK2里的屏幕已经点亮，它变成了你电脑的扩展桌面。
　　但是你也会发现，DK2屏幕首次被识别成扩展桌面的时候，屏幕是竖的（要是没装runtime直接连接DK2到电脑的话，会发现的更早）。
　　这时候只需要桌面右键菜单→屏幕分辨率，点击选中DK2的屏幕，然后设置成横向显示就可以了。如下图：
![设置横向显示](http://n.sinaimg.cn/sina_vr/transform/20160613/lbyy-fxszmaa1942729.jpg)设置横向显示
　　吐槽一下：这个应该是最基本的系统操作了吧！稍微动下脑子都能想出来，为什么就天天有人要问？
　　将DK2的屏幕摆正后，先刷新固件。
　　点击小眼睛图标，点击OculusConfiguration Utility，打开DK2设置软件界面。
　　选择Tools→Advanced→UpdateFirmware。
![](http://n.sinaimg.cn/sina_vr/crawl/20160613/HeTw-fxszmaa1942801.jpg)更新固件界面
　　在弹出的选择框中选择固件文件，固件文件在你安装runtime的路径
　　C：\ProgramFiles （x86）\Oculus\Tools\Firmware\DK2中
　　如下图：
![固件文件路径](http://n.sinaimg.cn/sina_vr/transform/20160613/9yrY-fxszvys0669924.jpg)固件文件路径
　　选择好固件后就开始刷新了，期间不要断开DK2的任何连接。
　　你要问断开会咋样？我不会那么作死，你要作死自己去作吧！要是DK2变砖了可以写个经验分享给大家。
　　刷新完成后到C：\ProgramFiles （x86）\Oculus\Tools里重新打开OculusConfiguration Utility。
　　接下来需要设置用户配置文件。
　　在没有创建用户配置文件的时候，OculusConfiguration Utility下的所有选项为灰色不可选。
　　首先你需要点击User下的那个“+”，新增一个用户配置文件，输入你的用户名。
![](http://n.sinaimg.cn/sina_vr/crawl/20160613/EPzM-fxszmaa1942803.jpg)输入新的用户名
　　用户配置文件设置好后，界面里的选项就可以设置了，请参考界面选项的翻译。
![](http://n.sinaimg.cn/sina_vr/crawl/20160613/6VWN-fxszmaa1942805.jpg)主界面翻译
![](http://n.sinaimg.cn/sina_vr/crawl/20160613/dGbg-fxszkzy5197685.jpg)高级设置界面翻译
![输出模式界面翻译](http://n.sinaimg.cn/sina_vr/transform/20160613/W0XQ-fxszvys0669960.jpg)输出模式界面翻译
**四、试运行自带DEMO**
　　当你理解了设置选项并设置了用户配置文件后，运行自带的demo（办公桌）。
　　根据demo中的感受，再做设置上的调整，直到你认为最舒服的状态为止。
![自带的DEMO演示](http://n.sinaimg.cn/sina_vr/transform/20160613/01yv-fxszkzy5197590.jpg)自带的DEMO演示
　　至此，DK2的基本设置算是完成了。
