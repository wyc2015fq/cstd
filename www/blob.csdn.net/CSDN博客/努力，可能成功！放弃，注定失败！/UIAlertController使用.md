# UIAlertController使用 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年03月10日 10:37:13[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：348

昨天苹果正式推送的iOS9。当天在网上就看到了很过开发朋友就在说可以放弃iOS7了（当然还要支持iOS6的朋友们不要哭）。我们基本遵守支持最新的2-3个iOS版本。如今iOS9正式推送后，估计大部分开发朋友在不久的将来也要最低支持iOS8了。那么，上面的标题种UIAlertController就是在iOS8推出的新类。
UIAlertController基本可以简单的理解为UIAlertController == UIAlertView + UIActionSheet，相比后面两个100%的开发朋友使用过.先简单看下UIAlertView和UIActionSheet的使用和效果
## 1.UIAlertView使用：
UIAlertView*alert = [[UIAlertView alloc]initWithTitle:@"提示"message:@"我是最牛逼的开发"delegate:nilcancelButtonTitle:@"不是"otherButtonTitles:@"是",nil];
alert.alertViewStyle=UIAlertViewStyleLoginAndPasswordInput;
[alert show];
![](http://upload-images.jianshu.io/upload_images/606414-9a8fe55b29891095.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
UIAlertView
## 2.UIActionSheet使用
UIActionSheet*sheet = [[UIActionSheetalloc]initWithTitle:@"我是最牛逼的开发"delegate:nilcancelButtonTitle:@"是"destructiveButtonTitle:@"是" otherButtonTitles:@"关闭",nil];
[sheet showInView:self.view];
![](http://upload-images.jianshu.io/upload_images/606414-a27223f8dab83f43.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
UIActionSheet
以上的使用，在熟悉不过了，就不多说了。那么说说UIAlertController吧，直接先看看使用，然后再说说区别
## 3.UIAlertController使用
UIAlertController *alert = [UIAlertController alertControllerWithTitle:@"提示"message:@"我是最牛逼的发"preferredStyle:UIAlertControllerStyleAlert];
当你想 show出来的时候，进入UIAlertController.h文件发现该类没有 show方法，但是看到@interface UIAlertController :UIViewController  从这就看出来该类是一个控制器。那么我们就可以试着想普通展示一个UIViewController的方法试一试，在这我使用的是model
[selfpresentViewController:alertanimated:YEScompletion:nil];  运行看效果：
![](http://upload-images.jianshu.io/upload_images/606414-09a082909b528c80.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
UIAlertController
这个样子，傻眼了，它挡住了整个屏幕，又没有关闭按钮，导致无法关闭。那么如何添加像UIAlertView一样添加一些取消或者其他按钮。UIAlertController是通过- (void)addAction:(UIAlertAction*)action; 这样添加一个action就是添加一个按钮事件，就像这样：
[alertaddAction [UIAlertActionactionWithTitle:@"定"style:UIAlertActionStyleDestructivehandler:^(UIAlertAction*action) {  NSLog(@"点击了确定按钮"); }]];
[alertaddAction:[UIAlertActionactionWithTitle:@"消"style:UIAlertActionStyleCancelhandler:^(UIAlertAction*action) {  NSLog(@"点击了取消按钮");  }]];
![](http://upload-images.jianshu.io/upload_images/606414-412ccae0390d4897.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
UIAlertController
我们看到，它看起来正常工作了，需要注意的是，action事件的处理在添加是采用了Block方式添加进去，个人觉得这样更加出色。当在同一个类中创建多个UIAlertController对象时，还要像之前一样在代理种区分具体对象，或者采用runtime发发为每一个UIAlertController对象，通过下面的方法
objc_setAssociatedObject 以及 objc_getAssociatedObject
添加自己的具体操作，触发时，取出具体操作。另外在添加了按钮后，那么如何添加UITextField文本框。UIAlertController是采用
[alertaddTextFieldWithConfigurationHandler:^(UITextField*textField) { textField.textColor= [UIColorredColor];
textField.text=@"iOS8";
[textFieldaddTarget:selfaction:@selector(usernameDidChange:)forControlEvents:UIControlEventEditingChanged];
}];
这样，就可以添加输入文本框了，就像这样
![](http://upload-images.jianshu.io/upload_images/606414-08d3b588cbf37762.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
UIAlertController
从上面的代码，我在添加时，还对textField进行了一些设置，比如默认文字以及textColor。并且对textField添加了值改变的监听事件。各种处理，我们都可以脑洞大开。
在一开始，说了基本可以认为UIAlertController == UIAlertView + UIActionSheet 上面演示的都是Alert，那么ActionSheet的方式呢，如下，只需要在创建代码
UIAlertController *alert = [UIAlertController alertControllerWithTitle:@"提示"message:@"我是最牛逼的发"preferredStyle:UIAlertControllerStyleActionSheet];
创建时将UIAlertControllerStyleAlert  改为UIAlertControllerStyleActionSheet 。alert就可以在底部弹出了，效果各位可以自己运行一下看看。需要注意的是：在UIAlertControllerStyleActionSheet样式下，是不可以添加UITextField。否则就会报错。
## 4.简单的区别
1.UIAlertView  iOS系统为了保证UIAlertView在所有界面之上，它会临时创建一个新的UIWindow，通过将UIWindow的UIWindowLevel设置的更高，让UIAlertView盖在所有应用的界面之上          
2.UIAlertController  是继承自UIViewController，它采用一个UIPopoverPresentationController类进行管理，UIPopoverPresentationController又继承自 UIPresentationController，其中的presentingViewController属性表示展示之前的Controller，presentedViewController属性表示被展示的Controller。另外这种方式，也统一了iPhone和iPad的使用方式
