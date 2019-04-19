# iOS-你真的会用UIMenuController吗?(详细) - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年06月14日 19:48:41[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：2053
## 请认真阅读,因为里面有很多细节!
### UIMenuController的介绍
- 什么是UIMenuController?
- UIMenuController是UIKit里面的控件
- UIMenuController的作用在开发中弹出的菜单栏
- 后面介绍了菜单栏显示中文的设置
![](http://upload-images.jianshu.io/upload_images/1846074-570a04d5177f711a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
菜单栏
### 有哪些控件是自带UIMenuController菜单栏效果的呢?
- UITextField
- UITextView
- UIWebView
![](http://upload-images.jianshu.io/upload_images/1846074-79cc5bce47455bee.gif?imageMogr2/auto-orient/strip)
系统菜单栏效果.gif
有些读者问我这个动图是怎么添加的: 我用的是licecap 这个软件
下面送个几个好用的软件
![](http://upload-images.jianshu.io/upload_images/1846074-2a9a7cda2cc07e9f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
好用的软件
> 
主要介绍两种:
一种是控制器为第一响应者
另一种是当前UI控件为第一响应者 (把方法封装在控件里面)
具体看代码,注释里我会详细讲解,如果实在不懂不妨动手敲一遍
```
@interface ViewController ()
@property (weak, nonatomic) IBOutlet UILabel *label;
@end
@implementation ViewController
- (void)viewDidLoad {
    [super viewDidLoad];
    //首先要允许label可以跟用户交互
    self.label.userInteractionEnabled = YES;
    //给label添加一个敲击手势
    [self.label addGestureRecognizer:[[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(labelClick)]];
}
/** 点击label触发的方法 */
- (void)labelClick
{
    //控制器不需要调用这个方法, 但是其他乱七八糟的UI控件就需要调用这个方法 \
    因为控制器默认是第一响应者
    //[self becomeFirstResponder];
    //显示menu 从来没有让menu跟控制器有关系 , 因为是第一响应者, 所以会调用第一响应者的方法
    //不一定调用控制器的方法, 因为现在控制器是第一响应者
    // 获得菜单
    UIMenuController *menu = [UIMenuController sharedMenuController];
    // 菜单最终显示的位置
    CGRect rect = CGRectMake(100, 100, 100, 100);
    [menu setTargetRect:rect inView:self.label];
    //为什么要设置2个参数  为了通用 \
    一个是矩形框, 一个是在哪个View上面
    //传了矩形框, 要告诉坐标原点在哪, 坐标原点就在view上\
    以tagreView的左上角为坐标原点
    // 苹果设计2个参数 是因为矩形框一旦修改 出现的位置在哪里都是可以的
    /*
     targetRect：menuController指向的矩形框
     targetView：targetRect以targetView的左上角为坐标原点
     */
    // 显示菜单
    [menu setMenuVisible:YES animated:YES];
    /*
     得通过第一响应者，来告诉MenuController它内部应该显示什么内容
     */
}
#pragma mark - 第一响应者 + UIMenuController
/**
 * 说明控制器可以成为第一响应者
 * 因为控制器是因为比较特殊的对象,它找控制器的方法,不找label的方法
 */
- (BOOL)canBecomeFirstResponder
{
    return YES;
}
/**
 * 通过这个方法告诉UIMenuController它内部应该显示什么内容
 * 返回YES，就代表支持action这个操作
 */
- (BOOL)canPerformAction:(SEL)action withSender:(id)sender
{
    //打印, 将一个方法转换成字符串 你就会看到许多方法
    NSLog(@"%@",NSStringFromSelector(action));
       if (action == @selector(cut:)
        || action == @selector(copy:)
        || action == @selector(paste:)) {
        return YES;
    }
    return NO;
}
//监听事情需要对应的方法 冒号之后传入的是UIMenuController
- (void)cut:(UIMenuController *)menu
{
    NSLog(@"%s %@", __func__, menu);
}
- (void)copy:(UIMenuController *)menu
{
    NSLog(@"%s %@", __func__, menu);
}
- (void)paste:(UIMenuController *)menu
{
    NSLog(@"%s %@", __func__, menu);
}
```
### 上面只是简单的介绍了UIMenuController的基本知识,下面正在带你运用到开发中的知识,封装一个自定义控件里面实现, 附上大概效果:
![](http://upload-images.jianshu.io/upload_images/1846074-fd69daaa4e0e8c59.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
进行工程修改添加支持中文的.png
![](http://upload-images.jianshu.io/upload_images/1846074-5c733cbe86626c38.gif?imageMogr2/auto-orient/strip)
会跳舞的狮子.gif
> 
上代码,代码有具体的注释,还有具体的用法,所以认真阅读代码即可,就不附上demo了
```
#import "JHLabel.h"
@implementation JHLabel
/** 不管控件是通过xib stroyboard 还是纯代码  提供两种初始化的操作都调用同一个方法 */
- (instancetype)initWithFrame:(CGRect)frame
{
    if (self = [super initWithFrame:frame]) {
        [self setupTap];
    }
    return self;
}
/** 不管控件是通过xib stroyboard 还是纯代码  提供两种初始化的操作都调用同一个方法 */
- (void)awakeFromNib
{
    [self setupTap];
}
/** 设置敲击手势 */
- (void)setupTap
{
    self.text = @"author:会跳舞的狮子";
    //已经在stroyboard设置了与用户交互,也可以用纯代码设置
//    self.userInteractionEnabled = YES;
    //当前控件是label 所以是给label添加敲击手势
   [self addGestureRecognizer:[[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(labelClick)]];
}
/** 点击label触发的方法 */
- (void)labelClick
{
    // 让label成为第一响应者 \
    一定要写这句话  因为这句话才是主动让label成为第一响应者
    [self becomeFirstResponder];
    // 获得菜单
    UIMenuController *menu = [UIMenuController sharedMenuController];
    // 设置菜单内容 \
    因为menuItems是数组 官方没有给出需要传入什么对象,但是以经验可以判断出需要传入的是UIMenuItem对象 \
    而且显示是按顺序的
    menu.menuItems = @[
                       [[UIMenuItem alloc] initWithTitle:@"顶" action:@selector(ding:)],
                       [[UIMenuItem alloc] initWithTitle:@"回复" action:@selector(reply:)],
                       [[UIMenuItem alloc] initWithTitle:@"举报" action:@selector(warn:)]
                       ];
    // 菜单最终显示的位置 \
    有两种方式: 一种是以自身的bounds  还有一种是以父控件的frame 
    [menu setTargetRect:self.bounds inView:self];
//    [menu setTargetRect:self.frame inView:self.superview];
    // 显示菜单
    [menu setMenuVisible:YES animated:YES];
}
#pragma mark - UIMenuController相关
/**
 * 让Label具备成为第一响应者的资格
 */
- (BOOL)canBecomeFirstResponder
{
    return YES;
}
/**
 * 通过第一响应者的这个方法告诉UIMenuController可以显示什么内容
 */
- (BOOL)canPerformAction:(SEL)action withSender:(id)sender
{
    if ( (action == @selector(copy:) && self.text) // 需要有文字才能支持复制
        || (action == @selector(cut:) && self.text) // 需要有文字才能支持剪切
        || action == @selector(paste:)
        || action == @selector(ding:)
        || action == @selector(reply:)
        || action == @selector(warn:)) return YES;
    return NO;
}
#pragma mark - 监听MenuItem的点击事件
/** 剪切 */
- (void)cut:(UIMenuController *)menu
{
    //UIPasteboard 是可以在应用程序与应用程序之间共享的 \
    (应用程序:你的app就是一个应用程序 比如你的QQ消息可以剪切到百度查找一样)
    // 将label的文字存储到粘贴板
    [UIPasteboard generalPasteboard].string = self.text;
    // 清空文字
    self.text = nil;
}
/** 赋值 */
- (void)copy:(UIMenuController *)menu
{
    // 将label的文字存储到粘贴板
    [UIPasteboard generalPasteboard].string = self.text;
}
/** 粘贴 */
- (void)paste:(UIMenuController *)menu
{
    // 将粘贴板的文字赋值给label
    self.text = [UIPasteboard generalPasteboard].string;
}
//如果方法不实现,是不会显示出来的
- (void)ding:(UIMenuController *)menu
{
    NSLog(@"%s %@", __func__, menu);
}
- (void)reply:(UIMenuController *)menu
{
    NSLog(@"%s %@", __func__, menu);
}
- (void)warn:(UIMenuController *)menu
{
    NSLog(@"%s %@", __func__, menu);
}
@end
```
### 上面的代码,menu只显示在label的上面 而一般菜单栏我们都是显示在cell的中间, 下面的这幅图详细讲解了显示在cell的中间
![](http://upload-images.jianshu.io/upload_images/1846074-679e49d9e788c245.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
显示在cell内容的中间
```
/** 点击cell的时候调用 */
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    // 取出cell
    UITableViewCell *cell = [tableView cellForRowAtIndexPath:indexPath];
    UIMenuController *menu = [UIMenuController sharedMenuController];
    // 设置菜单内容
    menu.menuItems = @[
                       [[UIMenuItem alloc] initWithTitle:@"顶" action:@selector(ding:)],
                       [[UIMenuItem alloc] initWithTitle:@"回复" action:@selector(reply:)],
                       [[UIMenuItem alloc] initWithTitle:@"举报" action:@selector(warn:)]
                       ];
    // 显示位置
    CGRect rect = CGRectMake(0, cell.height * 0.5, cell.width, 1);
    [menu setTargetRect:rect inView:cell];
    // 显示出来
    [menu setMenuVisible:YES animated:YES];
}
#pragma mark - 获得当前选中的评论
- (JHComment *)selectedComment
{
    // 获得被选中的cell的行号
    NSIndexPath *indexPath = self.tableView.indexPathForSelectedRow;
    NSInteger row = indexPath.row;
    // 获得评论数据
    NSArray *comments = self.shortComments;
    if (indexPath.section == 0 && self.longComments.count) {
        comments = self.longComments;
    }
    return comments[row];
}
#pragma mark - UIMenuController处理
- (BOOL)canBecomeFirstResponder
{
    return YES;
}
- (BOOL)canPerformAction:(SEL)action withSender:(id)sender
{
    if (!self.isFirstResponder) { // 文本框弹出键盘, 文本框才是第一响应者
        if (action == @selector(ding:)
            || action == @selector(reply:)
            || action == @selector(warn:)) return NO;
        //如果是文本框,那么这些显示都不返回
    }
    return [super canPerformAction:action withSender:sender];
}
- (void)ding:(UIMenuController *)menu
{
    NSLog(@"ding - %@ %@",
           self.selectedComment.user.username,
           self.selectedComment.content);
}
- (void)reply:(UIMenuController *)menu
{
    NSLog(@"reply - %@ %@",
           self.selectedComment.user.username,
           self.selectedComment.content);
}
- (void)warn:(UIMenuController *)menu
{
    NSLog(@"warn - %@ %@",
           self.selectedComment.user.username,
           self.selectedComment.content);
}
#注: 这里的self.selectedComent
# (点语法  要么调用set方法要么调用get方法, 这里直接用点语法是没有提示的,我已经进行了声明)
#/** 写方法声明的目的是为了使用点语法提示 */
#- (JHComment *)selectedComment;
```
文／会跳舞的狮子（简书作者）
原文链接：http://www.jianshu.com/p/a504c6a20808
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
