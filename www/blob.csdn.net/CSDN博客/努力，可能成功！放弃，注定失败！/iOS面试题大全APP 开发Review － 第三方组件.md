# iOS面试题大全APP 开发Review － 第三方组件 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年09月19日 08:26:35[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：770
Apple改进了Review流程之后，真的很迅速，昨天下午提交的应用，晚上就Ready for Sale了。大家可以在这里下载到了：[iOS面试题大全-点亮您的iOS技能树](http://itunes.apple.com/app/id1099674518)
为什么要写这个应用？是因为自己学iOS一年多了，很想对自己的知识进行一个梳理，而最近又陆陆续续看到了不少iOS面试题，所以想整理下这些问题，因此有了这个应用。
APP搜集了300+的常会被问到的iOS面试题，按iOS技能树的形式整理，从编码规范，版本控制，开发工具，APP Store配置，Object-C，Swift，iOS，数据结构与算法，架构，设计模式，第三方组建，UI设计，综合题等方面来归纳总结，即可以作为面试前的准备，也可以用来梳理自己的iOS技能，查漏补缺。
APP还提供了模拟面试的功能，会随机抽取30道面试题，让用户回答并录音。用户可以以此来模拟面试环境和准备面试。
此外，用户还可以自己定制面试问卷，作为一个面试官向面试者出题。
希望大家用的愉快，下面是应用的截图：
![](http://upload-images.jianshu.io/upload_images/1771779-ab477413f72f8eb5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
app preview.png
今天主要谈谈我在这个应用中用到的第三方组件，基本上现在都是站在牛人的肩膀上开发，找找各种第三方框架，然后把应用搭建起来。：）下面是我的Cocoapods文件：
```bash
source 'https://github.com/CocoaPods/Specs.git'
platform:ios, '8.0'
use_frameworks!
target "IOSSkillTree" do
pod 'ChameleonFramework'
pod 'Google-Mobile-Ads-SDK'
pod 'AVOSCloud'
pod 'AVOSCloudCrashReporting'
pod 'MBProgressHUD'
pod 'RATreeView'
pod 'ActionSheetPicker-3.0'
pod 'TAPromotee'
pod 'DZNEmptyDataSet'
pod 'SlideMenuControllerOC'
pod 'PulsingHalo'
end
```
简单介绍下每一个的框架吧。
[1. Chameleon](https://github.com/ViccAlexander/Chameleon)
把他最为第一个，是因为这个东西非常非常非常(不是风怒了，是重要的事情说三遍)适合一个独立开发的程序员！因为他就是你的设计师，如果你想要应用美观的话，一定要把这个库加到项目里。(当然，你配有高大上的设计和美工，当我没说)。
下面是他提供的功能，详细的大家可以去github上看。
![](http://upload-images.jianshu.io/upload_images/1771779-6947a0a4c91e24e1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Chameleon功能.png
说说我自己喜欢的几个功能：
- 就是他定义了24中扁平化的颜色.
![](http://upload-images.jianshu.io/upload_images/1771779-57725395a39df46f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
flat24.png
并定义了宏，所以直接用，比如：
`self.view.backgroundColor = FlatGreen;`
- 取反色，这样基本上就不用担心背景色和文字颜色如何调配了。
`ContrastColor(backgroundColor, isFlat);`
- 主题，一行代码就可以让你朴实的应用鲜活起来
`[Chameleon setGlobalThemeUsingPrimaryColor:(UIColor *)color withContentStyle:(UIContentStyle)contentStyle];`
![](http://upload-images.jianshu.io/upload_images/1771779-24f3d42c1aaf50d1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
theme.png
作为一个程序员，快试试吧，你会喜欢他的。
[2. admob](https://apps.admob.com/)
人总是要有梦想的，所以admob也是一定要加的。当然还有其他很多广告平台，大家可以自由选择。我个人还是比较信赖google的。admob也一直在改进，之前主要有两种形式：
- Banner：也就是我们最常见的，顶部或者底部那种横幅广告
- Interstitial:全屏弹出的广告。
近两年又追加了两种新类型：
- Rewarded video：带奖励性质的视频广告，比如看完视频后奖励两个游戏币。其实google并没有这类广告，不过他提供了这个借口，你可以通过他在admob内配置其他第三方的视频广告。我有集成过AdColony (Rewarded)，不过在国内测试，总会遇到取不到广告的情况。
- Native：这个是今年才新加入的，可以让我们自己定制广告的设计和风格。
上个月，Google又启用Firebase Analytics来替代Google Analytics，用来分析应用。在Parse被facebook收购之后，并宣布开源并停止服务之后，反倒谷歌让Firebase转型为面向移动开发者的一体化平台。在Parse停用后，我就用了国内的LeanCloud，所以暂时还没有用过Firebase，不过以后应该会尝试。
[3. LeanCloud](https://leancloud.cn/)
是国内领先的针对移动应用的一站式云端服务，BaaS 领域的先行者，专注于为应用开发者提供一流的工具和平台。(摘抄自他们家的自我介绍，别说是我夸的)。
因为我之前用的是Parse，前面提到过，Parse被Facebook收购之后就瞬间停止服务并开源（好吧，被收购的公司总是这么风雨飘零，如然让我想起了BOBJ），而LeanCloud几乎是Parse的翻版，甚至连API都是一样的（只是打头的两个字母不一样），所以就直接拿来用了。个人感觉还不错，有些功能比Parse有过之而无不及，而且更贴近国内，价格嘛，不好说，反正我先免费的用着：）。
为什么需要BaaS？人的精力有限吧，独立开发者已经承担了多重角色，如果还要再自己搞服务器写后台程序，那么一个应用开发的成本和周期就会非常非常的昂贵了。 Parse， LeanCloud，Firebase等等这类BaaS在起步阶段是一个非常好的平台。
鉴于Parse开源了，这边推荐[微博@iOS程序犭袁](http://weibo.com/luohanchenyilong/)大神的[Facebook开源的Parse源码分析【系列】](https://github.com/ChenYilong/ParseSourceCodeStudy)
[4. MBProgressHUD](https://github.com/jdg/MBProgressHUD)
等待指示器,这个翻译怪怪得，反正大家懂的。官网的说明非常清晰，所以我就不多说了，很好用：）看图，各种指示器，满足您的需求。
![](http://upload-images.jianshu.io/upload_images/1771779-61ba81b201fae932.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
MBProgressHUD.png
[5. RATreeView](https://github.com/Augustyniak/RATreeView)
因为这次需要用到树层结构来展示，所以用到了这个库。这是他的一个展示。
![](https://raw.github.com/Augustyniak/RATreeView/master/Screens/animation.gif)
tree table.gif
我项目中的树形结构, 去除了他的编辑功能，右边的按钮做了一些变化，在点击展开子树的时候，会有一些动态效果，用的是这个类库[FRDLivelyButton](https://github.com/sebastienwindal/FRDLivelyButton)，大家可以参考。
![](http://upload-images.jianshu.io/upload_images/1771779-5d41c1abc5945ed3.gif?imageMogr2/auto-orient/strip)
FRDLivelyButton.gif
[6. ActionSheetPicker-3.0](https://github.com/skywinder/ActionSheetPicker-3.0)
我也很喜欢这个项目，因为他能让你可能需要做半天的事情，10分钟就搞定了。
**iPhone/iPod ActionSheetPicker** = ActionSheetPicker = A Picker + UIActionSheet
**iPad ActionSheetPicker** = A Picker + UIPopoverController
![](http://upload-images.jianshu.io/upload_images/1771779-0bbf5f789a638cdc.gif?imageMogr2/auto-orient/strip)
ActionSheetPicker.gif
[7. TAPromotee](https://github.com/JanC/TAPromotee)
有了这个，你就可以在自己的应用里做交叉推广了。在自己的应用里推广自己的其他应用，是不是很酷：）而且是他用起来非常简单，以至于你都不可能不用－将TAPromotee加入你的podfile中，免费配置与享受更多下载吧。
![](http://upload-images.jianshu.io/upload_images/1771779-fc8369c63e52fcc2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
TAPromotee.png
[8. DZNEmptyDataSet](https://github.com/dzenbot/DZNEmptyDataSet)
如果你的table view和collection view为空，屏幕就为空，那你会怎么办？就空着？这样一来用户体验并不理想。
使用这个库，你只需遵守一些协议，iOS就会自动将collection view处理完善，并将用户消息以合适美观的方式显示出来。每个iOS项目都可以自动处理，不用再一一费神。
![](http://upload-images.jianshu.io/upload_images/1771779-71f6547a859190c5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
[9. SlideMenuControllerOC](https://github.com/Pluto-Y/SlideMenuControllerOC)
因为应用中需要一个侧滑菜单，所以用到了这个，找了几个，后来还是觉得这个好用一点。
![](https://raw.githubusercontent.com/Pluto-Y/SlideMenuControllerOC/master/Screenshots/SlideMenuControllerSwift3.gif)
SlideMenuControllerOC.png
[10. PulsingHalo](https://github.com/shu223/PulsingHalo)
应用中的模拟面试，会用到录音并且播放录音的内容，所以想找一个类似脉冲效果的动画，展示正在录音或者播放。所以用到了这个：
![](https://raw.githubusercontent.com/shu223/PulsingHalo/master/demo_.gif)
PulsingHalo.png
下面几个包含在CocoaPods中，不过依旧很好用：
[11. iRate](https://github.com/nicklockwood/iRate)
这是是用来让用户对你的应用做评价的。
以前我都是怎么做的？就是在设置页面中加一览类似让用户评价的按钮，用户点击之后就跳去对应的APP Review页面。 非常被动，其实就是根本没有人点。
那么稍微好一点的做法就是创建定制的应用内置alert，当满足一定的条件之后，就弹出对话框引导用户去做评价。就是我们经常看到的
![](http://upload-images.jianshu.io/upload_images/1771779-044a1fb5126bcfb5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
这个就是iRata——一个小型库——你可以把它放入项目中，把问卷调查什么的都忘记吧，iRate会在恰当的时候为你解决这个问题。 仅仅两行代码
```
[iRate sharedInstance].daysUntilPrompt = 1;
    [iRate sharedInstance].usesUntilPrompt = 1;
```
只要你指定了这两个条件，当满足时，就会自动弹出引导用户去做评价的对话框。 
[12. ShareSDK For iOS](http://www.mob.com/#/downloadDetail/ShareSDK/ios)
![](http://upload-images.jianshu.io/upload_images/1771779-0b53a6cbf0c12e7e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
ShareSDK.png
社会化分享也是必备的，我现在仅仅用到分享到微信和QQ，暂时用的是ShareSDK，不知道友盟怎么样。可能以后也会尝试一下。
这就是我在[iOS面试题大全-点亮您的iOS技能树](http://itunes.apple.com/app/id1099674518)用到的第三方组件，大家有看的上的拿去直接用哦。
最后感谢这些大神的分享。今天就到这了，明天接着和大家分享别的内容。
文／启发禅悟（简书作者）
原文链接：http://www.jianshu.com/p/dc4548770853
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
