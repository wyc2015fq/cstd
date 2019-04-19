# Xcode方便开发的插件推荐 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年08月29日 08:49:08[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：6672
Xcode的插件可以使我们开发更方便，效率更高。你没有理由拒绝哈哈，下面推荐一些插件：
**Alcatraz**
[https://github.com/alcatraz/Alcatraz](https://github.com/alcatraz/Alcatraz)
Alcatraz 是一个帮你管理 Xcode 插件、模版以及颜色配置的工具。它可以直接集成到 Xcode 的图形界面中，让你感觉就像在使用 Xcode 自带的功能一样。
使用方法推荐巧哥的这篇文章：
[http://blog.devtang.com/2014/03/05/use-alcatraz-to-manage-xcode-plugins/](http://blog.devtang.com/2014/03/05/use-alcatraz-to-manage-xcode-plugins/)
![](http://upload-images.jianshu.io/upload_images/1608265-b181647f65b1f930.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
image.png
下面的这些插件都带有链接地址，安装方法可以使用Alcatraz，也可以在GitHub把工程下载下来，编译一遍，然后完全退出Xcode，重启Xcode，如果弹出选择菜单就选择Load Bundle.
**KSImageNamed**
一款方便填写图片文件名称的插件。支持NSImage和UIImage，当你写到[UIImage imaged:的时候，插件会把项目中的图片名称提示出来，并且还有预览功能，对经常用代码生成图片视图的童鞋十分方便。
![](http://upload-images.jianshu.io/upload_images/1608265-a27a59a339cd31bf.gif?imageMogr2/auto-orient/strip)
image.gif
**FuzzyAutocomplete**
[https://github.com/FuzzyAutocomplete/FuzzyAutocompletePlugin](https://github.com/FuzzyAutocomplete/FuzzyAutocompletePlugin)
强大的模糊匹配输入 让你写代码的时候再也不用费脑子去记住名字那么长的对象或者函数名了 好用到让你想哭，但是Xcode8出来后应该就不用了吧。
![](http://upload-images.jianshu.io/upload_images/1608265-972d83909db6b2a9.gif?imageMogr2/auto-orient/strip)
demo.gif
**FKConsole**
[https://github.com/Forkong/FKConsole](https://github.com/Forkong/FKConsole)
在控制台直接将UTF8编码转化为中文!
![](http://upload-images.jianshu.io/upload_images/1608265-0cc04be495958e08.gif?imageMogr2/auto-orient/strip)
image.gif
**ESTranslate**
[https://github.com/EnjoySR/ESTranslate-Xcode](https://github.com/EnjoySR/ESTranslate-Xcode)
快速翻译Xcode中代码单词以及句子!
![](http://upload-images.jianshu.io/upload_images/1608265-e004973a7b38c92a.gif?imageMogr2/auto-orient/strip)
image.gif
**VVDocumenter**
[https://github.com/onevcat/VVDocumenter-Xcode](https://github.com/onevcat/VVDocumenter-Xcode)
喵大的又一力作 能够识别当前函数的参数和返回类型 帮助你快速编写符合规范的注释(目前是以Javadoc为标准)
![](http://upload-images.jianshu.io/upload_images/1608265-29db9ad72c6772c7.gif?imageMogr2/auto-orient/strip)
image.gif
**RSImageOptimPlugin**
[https://github.com/yeahdongcn/RSImageOptimPlugin](https://github.com/yeahdongcn/RSImageOptimPlugin)
可使用 ImageOptim 对项目里的图片资源进行优化，减少安装包的体积。
![](http://upload-images.jianshu.io/upload_images/1608265-a505d35ab369ded0.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
image.png
**deriveddata**
[https://github.com/kattrali/deriveddata-exterminator](https://github.com/kattrali/deriveddata-exterminator)
如果你老是遇到Xcode抽风，显示一些编译的错误或警告，或者有些时候Xcode会出各种奇怪的问题，但是编译又能通过，清除Xcode缓存就可以解决这类问题。
![](http://upload-images.jianshu.io/upload_images/1608265-d94b08800c98278a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
image.png
**HOStringSense**
[https://github.com/holtwick/HOStringSense-for-Xcode](https://github.com/holtwick/HOStringSense-for-Xcode)
在编辑字符串的时候，它会在代码上方生成一个字数统计的提示框。当你需要编辑多行文本时，可以在弹出的文本框中编辑字符串内容，插件会自动进行字符转义的工作，并实时在代码中更新。
![](http://upload-images.jianshu.io/upload_images/1608265-6629ca861b3f719b.gif?imageMogr2/auto-orient/strip)
image.gif
**OMColorSense**
[https://github.com/omz/ColorSense-for-Xcode](https://github.com/omz/ColorSense-for-Xcode)
一个简洁的可视化颜色插件。在你编辑颜色的RGB值的时候，会在代码上方出现一个小的色块，显示对应的颜色。点击色块还会弹出一个颜色编辑器，在编辑器中得到的色值会自动填入colorWithRed:green:blue:alpha:方法中，贴心！
![](http://upload-images.jianshu.io/upload_images/1608265-b4e9633761de122c.gif?imageMogr2/auto-orient/strip)
image.gif
**SCXcodeSwitchExpander**
[https://github.com/stefanceriu/SCXcodeSwitchExpander](https://github.com/stefanceriu/SCXcodeSwitchExpander)
Switch语句相当繁琐。虽然它清楚地列出了在每种情况下应该处理的方式，但一个个填入枚举值那真是浪费时间。SCXcodeSwitchExpander插件可以帮我们完成这个工作，它可以自动获得所有的枚举值填入相应的位置，而我们只需在每个枚举值下面填入对应的逻辑即可，方便极了！
![](http://upload-images.jianshu.io/upload_images/1608265-e9e795f75b318cd9.gif?imageMogr2/auto-orient/strip)
image.gif
**XcodeBoost**
[https://github.com/fortinmike/XcodeBoost](https://github.com/fortinmike/XcodeBoost)
通过配置，我们可以使用光标或者不精确的选择就可以剪切或者拷贝代码行，可以在粘贴代码的时候不触发代码格式化，还可以通过在.m文件中拷贝方法，粘贴进.h文件的时候就可以得到自动格式成的方法声明，还有好些功能都可以实现。
![](http://upload-images.jianshu.io/upload_images/1608265-60120420aa9998af.gif?imageMogr2/auto-orient/strip)
image.gif
![](http://upload-images.jianshu.io/upload_images/1608265-2c26b115ceb88f65.gif?imageMogr2/auto-orient/strip)
image.gif
**Xcode-Quick-Localization**
[https://github.com/tappollo/Xcode-Quick-Localization](https://github.com/tappollo/Xcode-Quick-Localization)
你是否已经厌倦了反复写NSLocalizedString？如果是，那么QuickLocalization插件可以助你一臂之力。当你在编辑字符串的时候，只需要按下option+shift+d，就可以将@“Any String”转换成NSLocalizedString(@"Any String", nil)。而且还可以通过修改配置来定义要出现在comment字段中的内容。
![](http://upload-images.jianshu.io/upload_images/1608265-235bcc4782960995.gif?imageMogr2/auto-orient/strip)
image.gif
**XAlign**
[https://github.com/qfish/XAlign](https://github.com/qfish/XAlign)
一个格式化代码的插件，作为有洁癖的码农 看到不对齐的代码一定是不能忍的 XAlign可以轻松解决你的烦恼。
![](http://upload-images.jianshu.io/upload_images/1608265-c526e15dd0f42998.gif?imageMogr2/auto-orient/strip)
image.gif
![](http://upload-images.jianshu.io/upload_images/1608265-ffea31ede7ac4b82.gif?imageMogr2/auto-orient/strip)
image.gif
**ClangFormat**
[https://github.com/travisjeffery/ClangFormat-Xcode](https://github.com/travisjeffery/ClangFormat-Xcode)
ClangFormat也是一款格式化代码的插件，开发者不仅可以实现对代码的自动或批量格式化，还可以进行自定义配置。
![](http://upload-images.jianshu.io/upload_images/1608265-19a2bac09a928cb0.gif?imageMogr2/auto-orient/strip)
image.gif
**CodePilot**
[https://github.com/macoscope/CodePilot](https://github.com/macoscope/CodePilot)
你要找的是文件？是文件夹？是代码？Never Mind，CMD+SHIFT+X调出CodePilot，输入任何你想到搜的东西吧！想搜appFinishLaunchingWithOptions？忘记咋拼了？没关系强大的代码搜索能力，appflaun一样也可以找到！超级强大的正则匹配，匹配任何你所想！
![](http://upload-images.jianshu.io/upload_images/1608265-ad9f3ad42ecf1f8e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
image.png
**CocoaPods**
[https://github.com/CocoaPods/CocoaPods](https://github.com/CocoaPods/CocoaPods)
可以很方便的在Xcode通过pods安装各种objective-c第三方库，省去以前还要手动去跑pods命令行的麻烦。
**RTImageAssets**
[https://github.com/rickytan/RTImageAssets](https://github.com/rickytan/RTImageAssets)
用来生成 @3x 的图片资源对应的 @2x 和 @1x 版本，只要拖拽高清图到 @3x 的位置上，然后按 Ctrl+Shift+A
即可自动生成两张低清的补全空位。当然，如果你对图片质量要求不高的话，你也可以从 @2x 的图生成 @3x 版本。
![](http://upload-images.jianshu.io/upload_images/1608265-7d1c3404d2d40d7f.gif?imageMogr2/auto-orient/strip)
image.gif
![](http://upload-images.jianshu.io/upload_images/1608265-b3ffc83ad76c95cb.gif?imageMogr2/auto-orient/strip)
image.gif
**JKBlockCommenter**
[https://github.com/Johnykutty/JKBlockCommenter](https://github.com/Johnykutty/JKBlockCommenter)
选中一段代码，按下⌘⌥/就可把这段代码包含在/**/之间，方便注释。
![](http://upload-images.jianshu.io/upload_images/1608265-9947012caf2643b7.gif?imageMogr2/auto-orient/strip)
image.gif
**Auto-Importer**
[https://github.com/citrusbyte/Auto-Importer-for-Xcode](https://github.com/citrusbyte/Auto-Importer-for-Xcode)
可以搜索和自动导入头文件的一款Xcode插件。
![](http://upload-images.jianshu.io/upload_images/1608265-3147e0b7f02b11f5.gif?imageMogr2/auto-orient/strip)
image.gif
**ZLGotoSandboxPlugin**
[https://github.com/MakeZL/ZLGotoSandboxPlugin](https://github.com/MakeZL/ZLGotoSandboxPlugin)
一个看应用沙盒的插件。也可以用SimPholders，看 [这篇文章](http://www.jianshu.com/p/b304515d225b)
![](http://upload-images.jianshu.io/upload_images/1608265-9626bf84f45a9cb4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
ZLGotoSandboxPlugin.png
**FKRealGroup**
[https://github.com/Forkong/FKRealGroup](https://github.com/Forkong/FKRealGroup)
FKRealGroup是文件夹创建删除增强插件，会在编辑菜单中添加”New Real Group”和”Delete Real Group”两个选项。
Xcode本身的”New Group”选项只会创建一个虚拟文件夹，并不会在本地磁盘创建真实文件夹。
FKRealGroup可以解决这个问题。”New Real Group”选项会在相应磁盘目录创建一个真实的文件夹。
我们在Xcode中如果使用”Delete”选项去删除文件夹，如果此文件夹为真实文件夹，那么一般情况下，Xcode只会删除此真实文件夹内的文件，而文件夹却依然存在。
”Delete Real Group”可以解决这个问题。”Delete Real Group”会默认删除真实文件夹，移动到废纸篓里面。
![](http://upload-images.jianshu.io/upload_images/1608265-72085141b75ebe9d.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
image.jpg
**ActivatePowerMode**
[https://github.com/poboke/ActivatePowerMode](https://github.com/poboke/ActivatePowerMode)
一个装逼插件，打字的时候有一些特效。
![](http://upload-images.jianshu.io/upload_images/1608265-2fcacbdb8f87c496.gif?imageMogr2/auto-orient/strip)
image.gif
文／iOS_啼血无痕（简书作者）
原文链接：http://www.jianshu.com/p/51635869b027
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
