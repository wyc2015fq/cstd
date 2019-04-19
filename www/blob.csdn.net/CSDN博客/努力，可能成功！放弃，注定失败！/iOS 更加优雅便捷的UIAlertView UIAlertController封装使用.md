# iOS 更加优雅便捷的UIAlertView/UIAlertController封装使用 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年12月30日 22:25:10[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1470
之前做过一套关于UIAlertView/UIAlertController的混合封装，详见：
> 
[iOS （封装）一句话调用系统的alertView和alertController](http://www.jianshu.com/p/056809125cbe)
这个是将alertView和alertController做了版本适配封装在一起的，提供了变参和数组两种方式，不过现在看来，虽然是“一句话”调用，但并不是很优雅的方式。
这次，改变了方案，将UIAlertView和UIAlertController分开进行了处理，整体代码也轻量了很多。
- 基于UIAlertView封装的JXTAlertView，这个是将之前写Demo时搞的一套快捷使用alertView的工具抽离整理出来的，并提供了C函数直接调用，像这样：
`jxt_showAlertTitle(@"简易调试使用alert，单按钮，标题默认为“确定”");`
就可以直接显示出一个alertView。
- 基于UIAlertController封装的JXTAlertController，支持iOS8及以上。调用方式为UIViewController的扩展分类方法，支持使用链式语法的方式配置alert的按钮及样式，相对于变参或者数组，更加简洁。
代码及Demo见GitHub:
> 
[JXTAlertManager](https://github.com/kukumaluCN/JXTAlertManager)
![](http://upload-images.jianshu.io/upload_images/1468630-5faebd1f6f2a3ccf.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
JXTAlertManager大体结构
# 1. JXTAlertView 便捷调试工具
之所以叫做快捷调试工具，就是因为这套代码是之前写Demo时搞出来的。所以，如果不是要适配iOS7及以下版本的话，这套代码还是建议只用在平时Demo测试。也因此，并没有针对UIActionSheet再进行封装，说白了是因为懒……
平时写一些Demo代码时，总会用到alert、toast、HUD这些工具，如果没有一套简便的工具，会麻烦很多，所以便从轻量便捷角度出发，基于UIAlertView，封装实现了alert、toast、HUD这些常用工具。
![](http://upload-images.jianshu.io/upload_images/1468630-e99f5311021af24a.gif?imageMogr2/auto-orient/strip)
JXTAlertView大致效果演示
## 1.1.快捷使用alertView
如果只是简单的一个提示，可以这样使用（这里只是一个示例，详细用法见源码）：
`jxt_showAlertTitle(@"简易调试使用alert，单按钮，标题默认为“确定”");`
其实现是基于：
```
[JXTAlertView showAlertViewWithTitle:title 
                             message:message 
                   cancelButtonTitle:cancelButtonTitle 
                    otherButtonTitle:otherButtonTitle
                   cancelButtonBlock:cancelBlock 
                    otherButtonBlock:otherBlock];
```
这是常用的两个以内的按钮的alertView，也可以这样使用：
```
jxt_showAlertTwoButton(@"title", @"message", @"cancel", ^(NSInteger buttonIndex) {
    NSLog(@"cancel");
}, @"other", ^(NSInteger buttonIndex) {
    NSLog(@"other");
});
```
针对于复杂的多按钮的alertView，还是使用变参方式，按钮响应，根据添加的按钮标题的index号依序区分：
```
[JXTAlertView showAlertViewWithTitle:@"title"
                             message:@"message"
                   cancelButtonTitle:@"cancel"
                    buttonIndexBlock:^(NSInteger buttonIndex) {
    if (buttonIndex == 0) {
        NSLog(@"cancel");
    }
    else if (buttonIndex == 1) {
        NSLog(@"按钮1");
    }
    else if (buttonIndex == 2) {
        NSLog(@"按钮2");
    }
    else if (buttonIndex == 3) {
        NSLog(@"按钮3");
    }
    else if (buttonIndex == 4) {
        NSLog(@"按钮4");
    }
    else if (buttonIndex == 5) {
        NSLog(@"按钮5");
    }
} otherButtonTitles:@"按钮1", @"按钮2", @"按钮3", @"按钮4", @"按钮5", nil];
```
## 1.2.简单的toast
这里的toast提示，有别于传统意义上的toast，因为其是基于alertView实现的，是一个没有按钮的alertView。可自定义展示延时时间，支持关闭回调的配置。
```
[JXTAlertView showToastViewWithTitle:@"title"
                             message:@"message"
                            duration:2
                   dismissCompletion:^(NSInteger buttonIndex) {
    NSLog(@"关闭");
}];
```
## 1.3.三种HUD的实现
这里的HUD区别于toast之处在于，其关闭时机可控，并不是单纯的一个延时展示。
三种HUD是指单纯的文字型、带风火轮（菊花）的加载窗、带进度条的加载窗。
后两者用KVC的方式访问了alertView的私有属性`accessoryView`实现，这样做可能没有太大问题，不过还是不建议线上开发使用，而且利用这种方式访问私有属性本来就是不太安全的，一旦key（私有属性名）改变了，不做容错处理，会崩溃，源码实现中做了一定的容错，但是，一旦对应key变化，也就导致对应功能失效了。
- 示例代码（C函数方式）：
```
jxt_showLoadingHUDTitleMessage(@"title", @"message");
dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(3 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
  jxt_dismissHUD();
});
```
HUD还有对应的简易的显示加载成功失败状态的方法，以及刷新进度条进度值的方法，详情见Demo。
# 2. JXTAlertController（iOS8）（链式语法的“隐患”）
JXTAlertController是基于系统的UIAlertController封装的，因此也只能支持iOS8及以上系统版本。
虽然源码中的`JXTAlertManagerHeader.h`做了一个版本适配，但是，其意义更多在于提示，很可能因此出错，所以，如果要适配iOS7，对应方法还是需要自行适配。
下面都以alert举例，actionSheet同理。
![](http://upload-images.jianshu.io/upload_images/1468630-b1e71fb54160106c.gif?imageMogr2/auto-orient/strip)
JXTAlertController大致效果演示
## 2.1.结构说明
```java
```java
/**
 JXTAlertController: show-alert(iOS8)
 @param title             title
 @param message           message
 @param appearanceProcess alert配置过程
 @param actionBlock       alert点击响应回调
 */
- (void)jxt_showAlertWithTitle:(nullable NSString *)title
                       message:(nullable NSString *)message
             appearanceProcess:(JXTAlertAppearanceProcess)appearanceProcess
                  actionsBlock:(nullable JXTAlertActionBlock)actionBlock NS_AVAILABLE_IOS(8_0);
```
```
上述方法是针对`UIViewController`做的分类扩展，详见源码。
也就是在某个VC中使用时，可直接用`self`指针调用。
`JXTAlertAppearanceProcess`是配置块，`JXTAlertActionBlock`是按钮响应回调块。
## 2.2.链式语法添加按钮
```
[self jxt_showActionSheetWithTitle:@"title"
                           message:@"message"
                 appearanceProcess:^(JXTAlertController * _Nonnull alertMaker) {
    alertMaker.
    addActionCancelTitle(@"cancel").
    addActionDestructiveTitle(@"按钮1").
    addActionDefaultTitle(@"按钮2").
    addActionDefaultTitle(@"按钮3").
    addActionDestructiveTitle(@"按钮4");
} actionsBlock:^(NSInteger buttonIndex, UIAlertAction * _Nonnull action, JXTAlertController * _Nonnull alertSelf) {
    if ([action.title isEqualToString:@"cancel"]) {
        NSLog(@"cancel");
    }
    else if ([action.title isEqualToString:@"按钮1"]) {
        NSLog(@"按钮1");
    }
    else if ([action.title isEqualToString:@"按钮2"]) {
        NSLog(@"按钮2");
    }
    else if ([action.title isEqualToString:@"按钮3"]) {
        NSLog(@"按钮3");
    }
    else if ([action.title isEqualToString:@"按钮4"]) {
        NSLog(@"按钮4");
    }
}];
```
`appearanceProcess`配置块中，`alertMaker`是当前alertController对象，`addActionCancelTitle(@"cancel")`是添加一个按钮，其等效于：
```
[alertController addAction:[UIAlertAction actionWithTitle:@"cancel" style:UIAlertActionStyleCancel handler:^(UIAlertAction * _Nonnull action) {
}]];
```
这里引入了简单的链式语法，可以连续添加系统支持的三类action按钮，当然`UIAlertActionStyleCancel`这个样式的action只能添加一次。这样可以大大简化代码。
`actionsBlock`是action按钮响应回调，可以根据index区分响应，index根据执行add的语法链从0依序增加，cancel类型的action布局位置是固定的，和添加顺序无关，但其index与添加顺序有关。
对于复杂或者特殊的alertController，也可以根据action.title或者action区分响应。
## 2.3.链式语法的“隐患”
用过`Masonry`这个库的，应该都对链式语法不会太陌生。链式语法可以使得代码简化且逻辑清晰化。但是，其也有一定的“隐患”存在。
`Masonry`应该是用的最多的一个自动布局的三方库，类似的还有`SDAutoLayout`（这里只是举例，同样的三方还有很多，这个应该是除了`Masonry`外，用的相对多一些的一个）这样的，同样的链式语法，后者似乎更加简洁优雅。那为什么大名鼎鼎的`Masonry`不这么干呢？我想是因为“安全”。
用`SDAutoLayout`的Demo做一个实验：
![](http://upload-images.jianshu.io/upload_images/1468630-1ab34f91d23640d2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
view为nil导致的崩溃
这里把view0置为nil，之后运行，程序直接崩溃了。。。这类似于执行一个未赋值的空block。
有人可能会认为这样的实验没有意义，为nil了干嘛还要布局呢？其实这是笔者前阵子在封装一个链式库时遇到的问题：实际应用开发中，情况要复杂很多，有些view是动态添加的，甚至是根据接口数据动态创建的，如果在处理这类代码逻辑中稍有不慎，就会造成上述问题，给不存在的view进行布局，直接导致程序崩溃。。。
其实这也是代码书写规范的问题，针对这类动态view，在处理时，本就应该添加`if`条件判断的，不过有时容易忽视，或者他人接手相关代码时，也容易忽略。如果用`Masonry`的块配置布局，就不会发生这类问题，因为这种情况，对于`Masonry`那种写法，就是一个空指针执行一个方法，其结果就是不执行，而像`SDAutoLayout`这类的，不作判空处理，就会导致程序崩溃。这里尤其要注意。
## 2.4.其他配置
针对alert上的输入框，保持系统的添加方式，示例如下：
```
[self jxt_showAlertWithTitle:@"title"
                     message:@"message"
           appearanceProcess:^(JXTAlertController * _Nonnull alertMaker) {
    alertMaker.
    addActionDestructiveTitle(@"获取输入框1").
    addActionDestructiveTitle(@"获取输入框2");
    [alertMaker setAlertDidShown:^{
        [self logMsg:@"alertDidShown"];//不用担心循环引用
    }];
    alertMaker.alertDidDismiss = ^{
        [self logMsg:@"alertDidDismiss"];
    };
    [alertMaker addTextFieldWithConfigurationHandler:^(UITextField * _Nonnull textField) {
        textField.placeholder = @"输入框1-请输入";
    }];
    [alertMaker addTextFieldWithConfigurationHandler:^(UITextField * _Nonnull textField) {
        textField.placeholder = @"输入框2-请输入";
    }];
} actionsBlock:^(NSInteger buttonIndex, UIAlertAction * _Nonnull action, JXTAlertController * _Nonnull alertSelf) {
    if (buttonIndex == 0) {
        UITextField *textField = alertSelf.textFields.firstObject;
        [self logMsg:textField.text];//不用担心循环引用
    }
    else if (buttonIndex == 1) {
        UITextField *textField = alertSelf.textFields.lastObject;
        [self logMsg:textField.text];
    }
}];
```
对于alert展示和关闭的回调，同样支持以block的方式配置。
如果`appearanceProcess`块不进行任何配置操作，即无按钮的alert，同样默认以toast模式处理。可通过设置`toastStyleDuration`属性，配置toast展示延迟时间。
## 2.5.改变alertController的字体颜色
可以通过KVC的方式访问alertController的私有属性，从而进行修改对应的字体的颜色，甚至字体。
对于`UIAlertAction`，可以用下面的方式修改字体颜色：
`[alertAction setValue:[UIColor grayColor] forKey:@"titleTextColor"];`
![](http://upload-images.jianshu.io/upload_images/1468630-2e11fedfd07f701b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
修改UIAlertAction字体颜色的效果
但是就像前面说的，个人并不推荐这类方式，所以源码中没有提供相关的方法。
如果有对应的特殊需求，总体来说，还是自定义alert视图比较灵活，网上相关的开源库也有很多可以直接使用，不必总是纠结于系统的实现方式。
最后，欢迎使用JXTAlertManager，如果遇到任何问题，请及时联系作者。
**参考文章：**
1.[iOS更改UIActionController弹出字体的样式](http://www.jianshu.com/p/f6752f7f8709)
2.[UIAlertController 简单修改title以及按钮的字体颜色](http://www.jianshu.com/p/cecd1b4bbf27#)
3.[How to add subview inside UIAlertView for iOS 7?](http://stackoverflow.com/questions/18759206/how-to-add-subview-inside-uialertview-for-ios-7)
4.[UIAlertView addSubview in iOS7](http://stackoverflow.com/questions/18729220/uialertview-addsubview-in-ios7)
5.[iOS UIAlertView中UIActivityindicatorView风火轮提示加载等待](http://blog.csdn.net/fengshi_sh/article/details/38493829)
文／霖溦（简书作者）
原文链接：http://www.jianshu.com/p/ae336594daf0
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
