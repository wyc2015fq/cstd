# iOS多语言功能开发小结 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年04月08日 17:33:28[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：552
> 
当前xcode版本7.2 手机端 iOS 9.3
       我们的项目已经非常庞大了，也上线了，老板突然说要加一个多语言功能，在他眼里程序员就是个魔术师，点一个按钮然后项目所有的中文字就变成英文的了。听得我都醉了，于是这两天我调研了一下，写了一个Demo来演示如何办到多语言之间的切换。
## Demo地址
       Demo上传到了github上，有兴趣的童鞋可以[下载](https://github.com/iOSKesai/languageDemo.git)去看看。
## 实现方法和原理解释
       基本上所有的App对于多语言这块无非2种需求，一个是软件根据手机当前操作系统的语言来显示对应的语言。另外一个种是，不论当前手机设置的是什么语言，用户可以通过设置（点击按钮，就像老板说的一样变魔术）改变当前软件的语言。
        第一种需求的童鞋们可以参考这篇[文章](http://www.jianshu.com/p/25ee1f211a30)，应该说的很详细了，而且也比较简单。这里我主要说明一下手动改变语言设置的实现方法。
- xCode创建工程，起名languageChangeDemo，然后创建资源CustomLocalizable.strings
![](http://upload-images.jianshu.io/upload_images/1538778-009703e62ef5edcd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
1.创建.strings资源文件
2.添加多语言，这里我添加了三种语言英文、简体中文、和繁体中文（台湾）。
![](http://upload-images.jianshu.io/upload_images/1538778-0b998e4b6fe4b94c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
2.添加语言
![](http://upload-images.jianshu.io/upload_images/1538778-c4956c93ab85155c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
3.默认就选个英文先添加上
![](http://upload-images.jianshu.io/upload_images/1538778-43ab3f6f068b5f3c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
4.添加语言
![](http://upload-images.jianshu.io/upload_images/1538778-ca6ea4b862d4871d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
点击.strings文件把语言勾选上
       这样就变成了第一张图里面三个.strings 后缀的文件，相当于每个文件代表了一种语言。这样基本上就算是配置好了，接下来就看怎么用了。
3.点击这三个.strings文件，在里面分别写入`"showText" = "this is a English Text";`英文的`"showText" = "这是一个简体中文";`中文的`"showText" = "這是壹個臺灣繁體";`繁体的。
![](http://upload-images.jianshu.io/upload_images/1538778-b1335bb67dbcc5cf.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
键值
这里实际上就是说，以后你要给哪个label显示文字不要用具体的字了，用这里面的Key来替代，然后在不同的语言环境下，会使用后面的值来替代。
4.代码实现起来的思路就是你告诉你的工程去文字的时候从哪个.strings文件中去取。然后调用`[bundle localizedStringForKey:(key) value:@"" table:(tbl)]`来设置具体的字符串，通常网上说的都会把这些方法封装成宏来实现。
![](http://upload-images.jianshu.io/upload_images/1538778-6abf2161d27569e8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
xib视图
我创建了3个按钮，每个按钮事件底下写了:
```
- (IBAction)englishBtn:(UIButton *)sender {
    //这句话告诉项目是从哪个.string文件中取字体。
    _appDelegate.lan = @"en";
   //这个是给上面的label赋值，不管上面语言都赋值那个Key，但显示的时候回事Value
    _StrtLabel.text = CustomStr(@"showText");
}
- (IBAction)chineseSimpleBtn:(UIButton *)sender {
    _appDelegate.lan =@"zh-Hans";
    _StrtLabel.text = CustomStr(@"showText");
}
- (IBAction)chineseTWBtn:(UIButton *)sender {
    _appDelegate.lan =@"zh-Hant-TW";
    _StrtLabel.text = CustomStr(@"showText");
}
```
       这里的`@"en"``@"zh-Hans"``@"zh-Hant-TW"`你可能会问，从哪来的？实际上当你前面按照我说的配置好了以后，你的工程文件夹下面会有几个文件夹。这样就知道该写什么了吧？
![](http://upload-images.jianshu.io/upload_images/1538778-cdd500ae5b70b35b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Snip20160408_15.png
       我这个Demo的思路是把这个字符串存到appDelegate的lan属性中(我自己创建了一个)。然后剩下一些工作我都在appDelegate里面封装了一下，包括宏的定义，目的是让程序中其他地方设置字的时候只用调用一个简单的宏就行了，例如:`_StrtLabel.text = CustomStr(@"showText");`这样，下面看一下appDelegate中的一些方法：
AppDelegate.h
```
#import <UIKit/UIKit.h>
//封装了一个宏 用来方便输入文字--实际是文字对应的key
#define CustomStr(key) [(AppDelegate *)[[UIApplication sharedApplication] delegate] showText:(key)];
@interface AppDelegate : UIResponder <UIApplicationDelegate>
@property (strong, nonatomic) UIWindow *window;
@property(strong,nonatomic) NSString *lan; //用来保存选择的语言
-(NSString *)showText:(NSString *)key;  //用来替代以往的 NSString 方法
@end
```
AppDelegate.m
```
-(NSString *)showText:(NSString *)key
{
    NSString *path = [[NSBundle mainBundle] pathForResource:_lan ofType:@"lproj"];
    return [[NSBundle bundleWithPath:path] localizedStringForKey:key value:nil table:@"CustomLocalizable"];
}
```
       AppDelegate.m中`@"lproj"`就是刚才那些存放语言的几个文件夹的后缀名`_lan`就是当前语言的类型了。然后这个方法就是返回了那些语言包里Key对应的Value值。
       AppDelegate.h中，我用了一个宏封装了一下`-(NSString *)showText:(NSString *)key;`这个方法的调用。这样的好处就是比如好几个小伙伴开发，他们用直接用这个宏就行了，很简单。
## 总结
       如果你的工程一开始就考虑多语言化，那么像我这样配置好，以后碰到给label赋值、等等的文字显示操作都用`CustomStr(key)`这个宏来搞定就行了。如果你的工程项目都上线好几版了，才想起来这个问题，那么按照我说的办法配置好，让你的小伙伴们，一个一个地方改吧，如果你的项目不是那么大，这个办法还是挺好的。当然，如果你的项目像我们一样，超级庞大，那么还有一个更高大上的更改办法，前面的这些工作都基本一样，然后思路就是用正则表达式查找你工程中所有的中文，然后替换，然后有一些编译不过的地方，再人工修改，具体的参考[这篇文章](http://www.jianshu.com/p/e97584e7ca13)吧。目前我的老板还没有逼我，赶紧改。所以这方法我还没有试，等哪天实在没办法了，我再去试试吧，祝我好运！
