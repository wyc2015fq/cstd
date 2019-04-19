# iOS学习之基础控件添加和事件绑定 - xqhrs232的专栏 - CSDN博客
2018年12月13日 17:30:57[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：97
原文地址::[https://www.jianshu.com/p/6eb302a62956](https://www.jianshu.com/p/6eb302a62956)
# 使用环境
XCode 7.1 beta
![](https://upload-images.jianshu.io/upload_images/1616478-34854adaef3046fb.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/532/format/webp)
Paste_Image.png
XCode界面
![](https://upload-images.jianshu.io/upload_images/1616478-29694ae5d4090d09.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
Paste_Image.png
> 
在Xcode中很多地方只要用鼠标单击就可以了
XCode的各个模块功能说明，可以参照这里的一个文章。
[http://blog.csdn.net/shulianghan/article/details/38424965](https://link.jianshu.com?t=http://blog.csdn.net/shulianghan/article/details/38424965)
# 学习目标
- 学会添加控件
- 学会设置控件的基本属性
- 学会绑定控件和事件
- 学会控件和属性绑定
# 创建项目
新建项目，进行设置项目名称等，保存目录。
![](https://upload-images.jianshu.io/upload_images/1616478-afb20c191b91113d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/731/format/webp)
Paste_Image.png
左上角设置设备类型，然后点击【运行】按钮
![](https://upload-images.jianshu.io/upload_images/1616478-46c83fac992e1585.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/283/format/webp)
Paste_Image.png
效果，是一个空白的界面，那就对了，因为我们没有放置任何的控件。
# 增加控件
点击【Main.storyboard】文件，这里就可以放置控件了。
> 
StoryBoard 的本质是一个 XML 文件，描述了若干窗体、组件、Auto Layout 约束等关键信息。你可以用文本工具打开来看看。
![](https://upload-images.jianshu.io/upload_images/1616478-f52fb27d2811b674.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/943/format/webp)
Paste_Image.png
我们可以从右下角的【库面板】上，把控件拖到View界面上
![](https://upload-images.jianshu.io/upload_images/1616478-bfba337cab17b5ba.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/365/format/webp)
Paste_Image.png
左下角按钮可以切换视图显示效果。
- 
拖动一个Label到View上
![](https://upload-images.jianshu.io/upload_images/1616478-35f18a90d096b44c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
Paste_Image.png
- 
设置Label属性
	点击右侧的【属性检测器】，可以设置名称，字体颜色，字体大小，等。
![](https://upload-images.jianshu.io/upload_images/1616478-6d1093f233457bc9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/760/format/webp)
Paste_Image.png
- 登录界面
	在往View上面拖Label,Button,Text,调整布局，最后效果如下：
![](https://upload-images.jianshu.io/upload_images/1616478-0e287010939f2804.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
Paste_Image.png
【Cammand + R 】运行效果：
![](https://upload-images.jianshu.io/upload_images/1616478-10897063f6787764.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/332/format/webp)
Paste_Image.png
此时，基础界面已经搭建完成了。接下来，我们会把控件和ViewController进行一些关联，实现登录按钮的事件。
# 控件和事件绑定
在ViewController.h里面，定义userLogin方法：
```
@interface ViewController : UIViewController
- (IBAction) userLogin:(id)obj;
@end
```
在ViewController.m里面，实现userLogin方法：
```
@implementation ViewController
- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
}
- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}
- (IBAction)userLogin:(id)obj {
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Information" message:@"Hellworld" delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
    [alert show];
}
@end
```
接下来，我们把登录按钮和这个事件进行绑定。
回到【Main.storyboard】上，点击登录按钮，
按住【Control】键，这时候会出现带箭头的线，把箭头指向【View Controller】，然后放开。
![](https://upload-images.jianshu.io/upload_images/1616478-be4045e39e2e599e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/688/format/webp)
Paste_Image.png
选择我们刚才写好的userLogin事件。
我们会在登录按钮的【连接检测器】上看到关联：
![](https://upload-images.jianshu.io/upload_images/1616478-3fedc7353fc2d6d4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/690/format/webp)
Paste_Image.png
如果想去掉关联，点击那个小小的关闭按钮即可。
我们，再来【Command+R】运行下看看，点击登录按钮：
![](https://upload-images.jianshu.io/upload_images/1616478-5234c191437c46a3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/319/format/webp)
Paste_Image.png
出现了，一个Hello world弹窗了。
这还不够，我们想把文本框里面的用户名和密码显示出来。
# 控件和字段绑定
我们再ViewController.h里面，添加字段，并相应设置字段的属性。
```
#import <UIKit/UIKit.h>
@interface ViewController : UIViewController{
    UITextField *tfUserName;
    UITextField *tfPWD;
}
@property (nonatomic, retain) IBOutlet UITextField *tfUserName;
@property (nonatomic, retain) IBOutlet UITextField *tfPWD;
- (IBAction) userLogin:(id)obj;
@end
```
再回到【Main.storyboard】,选择用户名的文本框，点击【右键】
![](https://upload-images.jianshu.io/upload_images/1616478-cba84b88d061cfaf.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/324/format/webp)
Paste_Image.png
鼠标移到【New Referencing Outlet】后面的小圆圈上，此时会出现一个加号，然后点击加号，拖动到【View Controller】然后放开鼠标，就会看到我们刚才定义的两个属性了。
![](https://upload-images.jianshu.io/upload_images/1616478-41506d9089b22082.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/661/format/webp)
Paste_Image.png
选择tfUserName。
同样我们可以在文本框的【连接检测器】上看到关联。
![](https://upload-images.jianshu.io/upload_images/1616478-6bc3ec4a23b41b0b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
Paste_Image.png
同样我们关联密码的文本框和tfPWD.
关联好后，我们就可以在【ViewController】里面，通过tfUserName和tfPWD来获取文本框的内容了。
# 实现显示文本框内容
在ViewController.m上，增加获取文本框内容和显示的代码如下：
```
@implementation ViewController
@synthesize tfUserName;
@synthesize tfPWD;
- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view, typically from a nib.
}
- (void)didReceiveMemoryWarning {
    [super didReceiveMemoryWarning];
    // Dispose of any resources that can be recreated.
}
- (IBAction)userLogin:(id)obj {
    NSString *username = [tfUserName text];
    NSString *pwd = [tfPWD text];
    NSString *message = [NSString stringWithFormat:@"用户名：%@ 密码：%@", username, pwd ];
    UIAlertView *alert = [[UIAlertView alloc] initWithTitle:@"Information" message:message delegate:nil cancelButtonTitle:@"OK" otherButtonTitles:nil];
    [alert show];
}
@end
```
> 
为什么要增加两个@synthesize,因为@synthesize和@property是配对的。
让编译好器自动编写一个与数据成员同名的方法声明来省去读写方法的声明。
运行效果：
![](https://upload-images.jianshu.io/upload_images/1616478-a10ddb8f5827c503.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/321/format/webp)
Paste_Image.png
# 写在最后
通过本文，对iOS界面开发有了一些的认识，当然这仅仅是第一步，还需要在深入学习。这里只是做一个学习的记录。
根据代码写完后，需要提出一些思考才会提升。
比如：
- IBOutlet和IBAction，只有定义这两个才可以关联，原理是什么。
- @property和@synthesize的用法，@property还有那些用法。
- 需要进一步学习OC的类的概念。
