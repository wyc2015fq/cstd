# 快速集成App中顶部标题滚动条 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2019年02月01日 14:45:31[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：78标签：[快速集成App中顶部标题滚动条](https://so.csdn.net/so/search/s.do?q=快速集成App中顶部标题滚动条&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
## 前言
目前越来越多的App都需要顶部标题切换界面效果，并且带有非常炫酷的效果，利用点时间给广大的攻城狮们提供了这样一个框架，集成了当前比较主流的几种效果，网易新闻（字体缩放+颜色渐变），今日头条（颜色填充），喜马拉雅，糗事百科（下标），腾讯视频（蒙版）等。如果喜欢我的文章，可以关注我微博:[袁峥Seemygo](https://link.jianshu.com?t=http://weibo.com/2034818060/profile?rightmod=1&wvr=6&mod=personinfo)
## Demo简介:
- 只要修改storyboard中，导航控制器根控制器的类型，就能看到不同的效果。
![](https://upload-images.jianshu.io/upload_images/304825-2097d019c070a75c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
Demo简介.png
## 框架使用教程：
- 用法跟UITabBarController一样，完全模仿UITabBarController写的,只要把YZDisplayViewController拖入自己工程即可。
- 1.自定义控制器继承YZDisplayViewController
- 2.在viewDidLoad中添加标题对应的控制器，并且把标题保存到对应控制器。
- 3.设置想要的标题效果，具体查看YZDisplayViewController.h文件，有哪些效果。
- 4.使用注意：如果需要全屏显示，并且添加的子控制器是tableViewController，当有`导航控制器`或者`UITabBarController`,需要设置tableView额外滚动区域,详情请看`FullChildViewController`
## 实例程序：
### 1.网易新闻:YZWYViewController
![](https://upload-images.jianshu.io/upload_images/304825-4e3ab3a122dc3b25.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/320/format/webp)
网易新闻.gif
- 1.继承YZDisplayViewController
```
@interface YZWYViewController : YZDisplayViewController
@end
```
- 2.在viewDidLoad中添加标题对应的控制器，并且把标题保存到对应控制器。
```
@implementation YZWYViewController
- (void)viewDidLoad
{
    [super viewDidLoad];
    
    // 添加所有子控制器
    [self setUpAllViewController];
    
}
@end
```
- 3.设置想要的标题效果，具体查看YZDisplayViewController.h文件，有哪些效果。
```
// 标题渐变
    // *推荐方式(设置标题渐变)
    [self setUpTitleGradient:^(YZTitleColorGradientStyle *titleColorGradientStyle, UIColor *__autoreleasing *norColor, UIColor *__autoreleasing *selColor) {
        
    }];
    
    // 字体缩放
    // 推荐方式 (设置字体缩放)
    [self setUpTitleScale:^(CGFloat *titleScale) {
        // 字体缩放比例
        *titleScale = 1.3;
    }];
```
### 2.今日头条:YZJRViewController
![](https://upload-images.jianshu.io/upload_images/304825-bc942d4ad2744b11.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/328/format/webp)
今日头条标题渐变.gif
- 1.继承YZDisplayViewController
```
@interface YZJRViewController : YZDisplayViewController
@end
```
- 2.在viewDidLoad中添加标题对应的控制器，并且把标题保存到对应控制器。
```
@implementation YZJRViewController
- (void)viewDidLoad
{
    [super viewDidLoad];
    
    // 添加所有子控制器
    [self setUpAllViewController];
    
}
@end
```
- 3.设置想要的标题效果，具体查看YZDisplayViewController.h文件，有哪些效果。
```
// 模仿网络延迟，0.2秒后，才知道有多少标题
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.2 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
    
        // 移除之前所有子控制器
        [self.childViewControllers makeObjectsPerformSelector:@selector(removeFromParentViewController)];
        
        // 把对应标题保存到控制器中，并且成为子控制器，才能刷新
        // 添加所有新的子控制器
        [self setUpAllViewController];
        
        // 注意：必须先确定子控制器
        [self refreshDisplay];
        
    });
    
    /*  设置标题渐变：标题填充模式 */
    [self setUpTitleGradient:^(YZTitleColorGradientStyle *titleColorGradientStyle, UIColor *__autoreleasing *norColor, UIColor *__autoreleasing *selColor) {
        // 标题填充模式
        *titleColorGradientStyle = YZTitleColorGradientStyleFill;
    }];
```
### 3.喜马拉雅:YZXiMaViewController
![](https://upload-images.jianshu.io/upload_images/304825-7ecfab95809630b5.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/349/format/webp)
喜马拉雅全屏展示.gif
- 1.继承YZDisplayViewController
```
@interface YZXiMaViewController : YZDisplayViewController
@end
```
- 2.在viewDidLoad中添加标题对应的控制器，并且把标题保存到对应控制器。
```
@implementation YZXiMaViewController
- (void)viewDidLoad
{
    [super viewDidLoad];
    
    // 添加所有子控制器
    [self setUpAllViewController];
    
}
@end
```
- 3.设置想要的标题效果，具体查看YZDisplayViewController.h文件，有哪些效果。
```
// 设置标题字体
    // 推荐方式
    [self setUpTitleEffect:^(UIColor *__autoreleasing *titleScrollViewColor, UIColor *__autoreleasing *norColor, UIColor *__autoreleasing *selColor, UIFont *__autoreleasing *titleFont, CGFloat *titleHeight) {
        
            // 设置标题字体
            *titleFont = [UIFont systemFontOfSize:20];
        
    }];
 
    // 推荐方式（设置下标）
    [self setUpUnderLineEffect:^(BOOL *isUnderLineDelayScroll, CGFloat *underLineH, UIColor *__autoreleasing *underLineColor) {
        // 标题填充模式
        *underLineColor = [UIColor redColor];
    }];
    
    // 设置全屏显示
    // 如果有导航控制器或者tabBarController,需要设置tableView额外滚动区域,详情请看FullChildViewController
    self.isfullScreen = YES;
```
- 4.如果添加的子控制器是tableViewController,给tableView设置底部显示区域
```
@implementation FullChildViewController
- (void)viewDidLoad
{
    [super viewDidLoad];
// 如果有导航控制器，顶部需要添加额外滚动区域
  // 添加额外滚动区域   导航条高度 + 标题高度
    if (self.navigationController) {
        CGFloat navBarH = self.navigationController.navigationBar.bounds.size.height;
        // 查看自己标题滚动视图设置的高度，我这里设置为44
        CGFloat titleScrollViewH = 44;
        self.tableView.contentInset = UIEdgeInsetsMake(navBarH + titleScrollViewH, 0, 0, 0);
    }
    
    // 如果有tabBarController，底部需要添加额外滚动区域
//    self.tableView.contentInset = UIEdgeInsetsMake(64 + 44, 0, 49, 0);
}
@end
```
### 4.腾讯视频:YZTXViewController
![](https://upload-images.jianshu.io/upload_images/304825-c0a9311a8a8a21fe.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/330/format/webp)
腾讯视频.gif
- 1.继承YZDisplayViewController
```
@interface YZTXViewController : YZDisplayViewController
@end
```
- 2.在viewDidLoad中添加标题对应的控制器，并且把标题保存到对应控制器。
```
@implementation YZTXViewController
- (void)viewDidLoad
{
    [super viewDidLoad];
    
    // 添加所有子控制器
    [self setUpAllViewController];
    
}
@end
```
- 3.设置想要的标题效果，具体查看YZDisplayViewController.h文件，有哪些效果。
```
CGFloat y = self.navigationController?64:0;
    CGFloat screenW = [UIScreen mainScreen].bounds.size.width;
    CGFloat screenH = [UIScreen mainScreen].bounds.size.height;
    
    // 设置搜索框
    CGFloat searchH = 44;
    UISearchBar *searchBar = [[UISearchBar alloc] initWithFrame:CGRectMake(0, y, screenW, searchH)];
    [self.view addSubview:searchBar];
     // 设置整体内容尺寸（包含标题滚动视图和底部内容滚动视图）
    [self setUpContentViewFrame:^(UIView *contentView) {
       
        CGFloat contentX = 0;
        
        CGFloat contentY = CGRectGetMaxY(searchBar.frame);
        
        CGFloat contentH = screenH - contentY;
        contentView.frame = CGRectMake(contentX, contentY, screenW, contentH);
        
    }];
    
    /****** 标题渐变 ******/
    // 推荐方式(设置标题颜色渐变) // 默认RGB样式
    [self setUpTitleGradient:^(YZTitleColorGradientStyle *titleColorGradientStyle, UIColor *__autoreleasing *norColor, UIColor *__autoreleasing *selColor) {
        *norColor = [UIColor greenColor];
        *selColor = [UIColor redColor];
    }];
    /****** 设置遮盖 ******/
    // *推荐方式(设置遮盖)
    [self setUpCoverEffect:^(UIColor **coverColor, CGFloat *coverCornerRadius) {
        
        // 设置蒙版颜色
        *coverColor = [UIColor colorWithWhite:0.7 alpha:0.4];
        
        // 设置蒙版圆角半径
        *coverCornerRadius = 13;
    }];
```
## 源码
点击这下载[源代码](https://link.jianshu.com?t=https://github.com/iThinkerYZ/YZDisplayViewController)
作者：袁峥
链接：https://www.jianshu.com/p/b45655e23a42
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
