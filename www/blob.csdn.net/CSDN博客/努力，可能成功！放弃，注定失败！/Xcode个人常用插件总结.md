# Xcode个人常用插件总结 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年03月31日 11:47:56[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1327
Xcode在本地存放的路径 /Users/用户名/Library/Application Support/Developer/Shared/Xcode/Plug-ins
个人常用插件如下：
###### 1.Alcatraz
Alcatraz是管理 Xcode 所有插件的插件，安装方法参照[项目官网](http://alcatraz.io)或者[gitHib地址](https://github.com/alcatraz/Alcatraz)
安装完成后，重启Xcode加载该插件后再Xcode的window菜单下多处一个Package Manager的菜单项（快捷键：shift + command + 9）。
如下图所示：
![](http://upload-images.jianshu.io/upload_images/1101711-94222610c0eee44d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Package Manager.png
然后搜索你想要的插件，直接点击INSTALL进行安装即可。
插件地址：[https://github.com/alcatraz/Alcatraz](https://github.com/alcatraz/Alcatraz)
###### 2.CleanHeaders
清除重复导入的头文件插件，并按照字母排序。快捷键（command+|）和Xcode本身的快捷键可能有冲突。
![](http://upload-images.jianshu.io/upload_images/1101711-d237b77c8c51b689.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
CleanHeaders
插件地址：[https://github.com/insanoid/CleanHeaders-Xcode](https://github.com/insanoid/CleanHeaders-Xcode)
###### 3.CocoaPods
大名鼎鼎，不解释了。
![](https://github.com/kattrali/cocoadocs-xcode-plugin/raw/master/menu.png)
插件地址：[https://github.com/kattrali/cocoapods-xcode-plugin](https://github.com/kattrali/cocoapods-xcode-plugin)
###### 4.DerivedData Exterminator
缓存目录的插件，调试程序是很有用。
![](https://github.com/kattrali/deriveddata-exterminator/raw/master/docs/menu.png)
插件地址：[https://github.com/kattrali/deriveddata-exterminator](https://github.com/kattrali/deriveddata-exterminator)
###### 5.FuzzyAutocomplete
代码提交建议插件
![](https://github.com/FuzzyAutocomplete/FuzzyAutocompletePlugin/raw/master/demo.gif)
插件地址：[https://github.com/FuzzyAutocomplete/FuzzyAutocompletePlugin](https://github.com/FuzzyAutocomplete/FuzzyAutocompletePlugin)
###### 6.HighlightSelectedString
选中的单词高亮插件
![](https://github.com/keepyounger/HighlightSelectedString/raw/master/demo.png)
插件地址：[https://github.com/keepyounger/HighlightSelectedString](https://github.com/keepyounger/HighlightSelectedString)
###### 7.HOStringSense
完美的正则表达式编辑插件，支持多行文本，内嵌HTML，还提供字符串长度的快速反馈。
![](https://github.com/holtwick/HOStringSense-for-Xcode/raw/master/StringDemoAnimation.gif)
插件地址：[https://github.com/holtwick/HOStringSense-for-Xcode](https://github.com/holtwick/HOStringSense-for-Xcode)
###### 8.OMColorSense
颜色输入辅助插件，在编写UIColor代码时，实时预览相应的颜色
![](http://upload-images.jianshu.io/upload_images/1101711-f51a8e7ca1b923af.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
插件地址：[https://github.com/omz/ColorSense-for-Xcode](https://github.com/omz/ColorSense-for-Xcode)
###### 9.Peckham
快速导入头文件，快捷键: ⌘ + ctrl + P
![](https://github.com/markohlebar/Peckham/raw/master/Misc/Peckham.gif)
插件地址：[https://github.com/markohlebar/Peckham](https://github.com/markohlebar/Peckham)
###### 10.VVDocumenter-Xcode
自动生成代码注释的工具，可以方便地将函数的参数名和返回值提取出来，生成注释文档的时候很有用。快捷键: ///
![](https://camo.githubusercontent.com/ca5518c9872e15b8a95b9d8c5f44bc331977d710/68747470733a2f2f7261772e6769746875622e636f6d2f6f6e65766361742f5656446f63756d656e7465722d58636f64652f6d61737465722f53637265656e53686f742e676966)
插件地址：[https://github.com/onevcat/VVDocumenter-Xcode](https://github.com/onevcat/VVDocumenter-Xcode)
###### 11.XAlign
代码对齐插件，强迫症者首选神器。快捷键：shift + command + X
![](https://camo.githubusercontent.com/7973c0e352b1f91e3efe5b3550cff5df97f4589a/687474703a2f2f7166692e73682f58416c69676e2f696d616765732f657175616c2e676966)
插件地址：[https://github.com/qfish/XAlign](https://github.com/qfish/XAlign)
###### 12.XcodeColors
在控制器打印出彩色的log。和CocoaLumberjack配合据说是日志输出神器，没怎么研究过。
![](http://upload-images.jianshu.io/upload_images/1101711-38e70a5e79899d99.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
插件地址：[https://github.com/robbiehanson/XcodeColors](https://github.com/robbiehanson/XcodeColors)
###### 其他
- 
1.KSImageNamed用过一段时间，个人感觉不怎么好用，如果图片很多的话，根本就是个悲剧。有时候你需要对图片名称进行拼接，不需要他自动提示出来。
![](https://camo.githubusercontent.com/c354bf04524df86daeabe7a6d2b9926fac790f85/68747470733a2f2f7261772e6769746875622e636f6d2f6b7375746865722f4b53496d6167654e616d65642d58636f64652f6d61737465722f73637265656e73686f742e676966)
插件地址：[https://github.com/ksuther/KSImageNamed-Xcode](https://github.com/ksuther/KSImageNamed-Xcode)
- 
2.XToDo
查找项目中所有的带有 TODO, FIXME, ???, !!! 标记的注释
![](https://github.com/trawor/XToDo/raw/master/screenshots/2.png)
插件地址：[https://github.com/trawor/XToDo](https://github.com/trawor/XToDo)
- 
3.SCXcodeSwitchExpander
快速填充枚举值
![](https://camo.githubusercontent.com/d4ab3ba45af70951557adbf17a9d0deab47e519f/68747470733a2f2f646c2e64726f70626f7875736572636f6e74656e742e636f6d2f752f31323734383230312f534358636f6465537769746368457870616e6465722f534358636f6465537769746368457870616e646572312e676966)
插件地址：[https://github.com/stefanceriu/SCXcodeSwitchExpander](https://github.com/stefanceriu/SCXcodeSwitchExpander)
