# [翻译] CNPGridMenu - weixin_33985507的博客 - CSDN博客
2014年12月29日 22:00:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
CNPGridMenu
![](https://images0.cnblogs.com/blog/607542/201412/290924310286388.gif)
`CNPGridMenu` is a Mailbox style grid menu with a blurred background for iOS 7 & iOS 8.
CNPGridMenu是一个邮箱网格风格的菜单，调出菜单的同时模糊背景，支持iOS7与iOS8.
![](https://images0.cnblogs.com/blog/607542/201412/290925008416981.gif)
Usage - 使用
You can create items for the grid menu using `CNPGridMenuItem`.
你可以通过CNPGridMenuItem创建网格的items。
`@property (nonatomic, strong) NSString *title;`
`@property (nonatomic, strong) UIImage *icon;`
`@property (nonatomic, copy) SelectionHandler selectionHandler;`
[](https://github.com/carsonperrotti/CNPGridMenu#customization)Customization - 自定义
You can choose the blur style. `UIBlurEffectStyleDark``UIBlurEffectStyleLight or ``UIBlurEffectStyleExtraLight`
你可以使用模糊背景的风格，支持3种，UIBlurEffectStyleDark，UIBlurEffectStyleLight 与 UIBlurEffectStyleExtraLight。
`@property (nonatomic, assign) UIBlurEffectStyle blurEffectStyle;`
[](https://github.com/carsonperrotti/CNPGridMenu#installation)Installation - 安装
Requires iOS 7 or iOS 8
需要iOS7或者iOS8
`Pod "CNPGridMenu"`
Or if you don't want to use Cocoapods `#import "CNPGridMenu.h"`
如果你不想用Cocoapods，你可以直接导入头文件“CNPGridMenu.h”
