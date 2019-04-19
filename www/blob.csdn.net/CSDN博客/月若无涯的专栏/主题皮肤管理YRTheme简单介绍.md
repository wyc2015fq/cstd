# 主题皮肤管理YRTheme简单介绍 - 月若无涯的专栏 - CSDN博客
2016年06月16日 17:21:58[月若无涯](https://me.csdn.net/u010124617)阅读数：799
> 
开发中常常遇到切换主题，夜间模式什么的，一般的做法都是变了新主题就判断一下重新设置新值，代码量较多还不够清晰。 
  本库将主题素材与代码进行了分离，力求一次代码绑定，后续只需要更改素材即可实现主题样式更改  
#### YRTheme地址 : [https://github.com/YueRuo/YRTheme](https://github.com/YueRuo/YRTheme)
#### 特性如下
- 使用 YRThemeManager 对界面上的常用控件进行颜色名称和图片名称的绑定, 当主题变化时 , 新的主题皮肤会自动应用到之前绑定的各个控件上  
- 切换主题只需一行代码:[[YRTheme shared]setCurrentThemeName:@”Dark”]  
- 使用弱引用对象进行控件存储和查找匹配，无需担心内存问题
- 可根据不同开发者需求，自行定制主题和不同主题下元素的获取功能，扩展性更强
#### 具体使用步骤如下
- 在不同的主题皮肤配置文件中设置好相同键名的颜色、图片等等。在例子中，主题名称为Default的，对应的配置文件为DefaultColor.plist和DefaultImage.plist，当然，你可以根据自己需求重新定制主题和各素材名称匹配获取方式，例子中我实现了的主题素材获取处理类为ThemeHepler。
- 
在程序用对常用控件进行主题绑定，绑定代码基本如下  
```
_manager = [[YRThemeManager alloc]init];
//绑定各种控件数据
[_manager bindView:self.view bgColorName:@"commonBgColor"];
[_manager bindImageView:self.imageView imageName:@"testImage1"];
[_manager bindLabel:self.label titleColorName:@"labelTitleColor"];
[_manager bindButton:self.button titleColorName:@"labelTitleColor" state:UIControlStateNormal];
[_manager bindButton:self.button titleColorName:@"buttonHighlight" state:UIControlStateHighlighted];
```
- 
变更主题皮肤
```
// 全局主题更改
[[YRTheme shared]setCurrentThemeName:@"Dark"];
// 仅当前manager管理的主题更改
[_manager updateToTheme:@"Dark"];
```
- 
如果遇到无法绑定的属性（比如layer的颜色什么的），那只能老老实实通过监听kYRThemeChangeNotification来处理主题变更的功能了。
#### 关于内部实现
    简单说起来是使用了一个字典，记录了所有绑定过的控件，当然是弱引用的，所以无需担心内存问题 
PS：关于如何在NSDictionary中弱引用存储一个对象，一般我们自己实现的使用如下方式：
```
//新建一个类似这样的对象
@interface YRThemeSafeObject : NSObject
@property (weak, nonatomic) id obj;
@end
@implementation YRThemeSafeObject
@end
```
    后续存储时先用该对象包装一下，再存储字典中等等。 
    当然也有使用`NSHashTable`类的，有兴趣的可以自行查阅文档。
    当收到主题变更时，检查存储的所有控件，赋值新的素材和样式。说起来就这么简单，实现还是有点小麻烦的，具体请看[demo](https://github.com/YueRuo/YRTheme)。
