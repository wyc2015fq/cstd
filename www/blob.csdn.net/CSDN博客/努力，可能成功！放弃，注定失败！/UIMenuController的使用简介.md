# UIMenuController的使用简介 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年07月19日 20:25:04[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：2078标签：[iOS学习资料																[iOS 资源大全																[iOS常用方法																[UIMenuController的使用](https://so.csdn.net/so/search/s.do?q=UIMenuController的使用&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS常用方法&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS 资源大全&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS学习资料&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
[UIMenuController苹果官方文档](https://developer.apple.com/library/ios/documentation/iPhone/Reference/UIMenuController_Class/index.html#//apple_ref/doc/uid/TP40008301-CH1-SW7)
[示例代码下载地址](https://github.com/ZYVIP/ZZYMeunControllerDemo)
# 1. UIMenuController简介
在很多应用中，当我们长按一段文字或者图片的时候会弹出一个菜单，我们通过这个菜单可以实现文字等的复制、剪切、删除以及各种操作。
![](http://upload-images.jianshu.io/upload_images/1926594-bc68d2343ad204be.gif?imageMogr2/auto-orient/strip)
示例图片
- 这个菜单就是UIMenuController，系统**默认支持**`UITextField、UITextView、UIWebView`控件的UIMenuController相关操作
- 对于系统不支持UIMenuController操作的控件，我们就要`自定义控件的UIMenuController`来实现相关功能
# 2. UIMenuController相关方法
- 创建一个UIMenuController对象
`+ (UIMenuController *)sharedMenuController`- 显示或者隐藏菜单
```
@property(nonatomic,getter=isMenuVisible) BOOL menuVisible;        // default is NO
//是否通过动画进行设置显示、隐藏
- (void)setMenuVisible:(BOOL)menuVisible animated:(BOOL)animated;
```
> 
**注意 **在显示menu之前，一点要确定为menu设置与其相关的显示位置
- 设置menu显示的位置
```
/**
 *  设置menu显示的位置信息
 *
 *  @param targetRect menu需要显示的矩形区域
 *  @param targetView targetRect会以targetView的左上角为坐标原点进行显示
 */
- (void)setTargetRect:(CGRect)targetRect inView:(UIView *)targetView;
```
> 
**注意**
- targetRect一旦设定以后，矩形范围不会跟随view的移动而移动，如果view移动，必须相应的更新targetRect 。比如tableView 点击cell出现menu，当按住对应的cell，拖动tableView滚动时，menu不会随着对应的cell一起滚动---见示例代码2
- targetRect通常设置为需要弹出menu控件的bounds，targetView设置为对应的控件本身
- 更新menu的显示与对应方法
默认系统在menu显示并且点击menu上的item时，调用该方法。
`- (void)update`- 自定义menuItem
- 该属性默认为空。每一个menu Item都是一个UIMenuItem对象。
- 你可以创建自定义的menu items，每一个item拥有自己的标题和方法，你必须通过menuItems属性添加每一个item。
- 自定义的item在meun中显示在系统item后面
```
@property(nonatomic, copy) NSArray <UIMenuItem *> *menuItems
@interface UIMenuItem : NSObject 
//创建UIMenuItem对象
- (instancetype)initWithTitle:(NSString *)title action:(SEL)action ;
@property(nonatomic,copy) NSString *title;
@property(nonatomic)      SEL       action;
```
- 
数据类型:编辑菜单箭头指向view的位置
默认取决于view在界面的位置
```
typedef enum {
 UIMenuControllerArrowDefault,
 UIMenuControllerArrowUp,
 UIMenuControllerArrowDown,
 UIMenuControllerArrowLeft,
 UIMenuControllerArrowRight,
} UIMenuControllerArrowDirection;
```
- 
munu支持的通知
根据字面意思很容易理解，不再赘述
```
UIMenuControllerWillShowMenuNotification
UIMenuControllerDidShowMenuNotification
UIMenuControllerWillHideMenuNotification
UIMenuControllerDidHideMenuNotification
UIMenuControllerMenuFrameDidChangeNotification
```
# 3.自定义控件的UIMenuController
- 两个重要方法
**PS:**如果自定义的menu显示效果存在问题，一般是这两个方法出现了问题，要多加琢磨
```
//设置控件可以成为第一响应者，注意不是每个控件都可以成为第一响应者
- (BOOL)canBecomeFirstResponder;    // default is NO
/**
 *  设置控件能够执行那些具体操作
 *  @param action 具体操作
 *  @return YES:支持该操作
 */
- (BOOL)canPerformAction:(SEL)action withSender:(nullable id)sender;
// Allows an action to be forwarded to another target. By default checks -canPerformAction:withSender: to either return self, or go up the responder chain.
```
- 
一般步骤：
- 设置控件成为第一响应者
- 创建UIMenuControler
- 创建UIMenuItem(如果需要自定义item)
- 在对应控件重写上述两个方法
- 
UIMenuController按钮点击常见系统方法
```
- (void)cut:(nullable id)sender NS_AVAILABLE_IOS(3_0);
- (void)copy:(nullable id)sender NS_AVAILABLE_IOS(3_0);
- (void)paste:(nullable id)sender NS_AVAILABLE_IOS(3_0);
- (void)select:(nullable id)sender NS_AVAILABLE_IOS(3_0);
- (void)selectAll:(nullable id)sender NS_AVAILABLE_IOS(3_0);
- (void)delete:(nullable id)sender NS_AVAILABLE_IOS(3_2);
- (void)makeTextWritingDirectionLeftToRight:(nullable id)sender NS_AVAILABLE_IOS(5_0);
- (void)makeTextWritingDirectionRightToLeft:(nullable id)sender NS_AVAILABLE_IOS(5_0);
//私有方法
   _promptForReplace:
   _transliterateChinese:
   _showTextStyleOptions:
   _define:
   _addShortcut:
   _accessibilitySpeak:
   _accessibilitySpeakLanguageSelection:
   _accessibilityPauseSpeaking:
   _share:
```
- 示例代码1：自定义Label的UIMenuController
```
#import "ZZYMenuLabel.h"
@implementation ZZYMenuLabel
/**
 *  xib创建label时调用
 */
- (void)awakeFromNib
{
    [self setUp];
}
/**
 *  代码创建label时调用
 */
- (instancetype)initWithFrame:(CGRect)frame
{
    if (self = [super initWithFrame:frame]) {
         [self setUp];
    }
    return self;
}
- (void)setUp
{
    self.userInteractionEnabled = YES;
    [self addGestureRecognizer:[[UILongPressGestureRecognizer alloc]initWithTarget:self action:@selector(longPress)]];
}
- (void)longPress
{
    NSLog(@"%s",__func__);
    //1.设置label为第一响应者
    //通过设置第一响应者UIMenuController可以获得支持哪些操作的信息,操作怎么处理
    [self becomeFirstResponder];
    //2.设置UIMenuController
    UIMenuController * menu = [UIMenuController sharedMenuController];
    //当长按label的时候，这个方法会不断调用，menu就会出现一闪一闪不断显示，需要在此处进行判断
    if (menu.isMenuVisible)return;
    //自定义 UIMenuController
    UIMenuItem * item1 = [[UIMenuItem alloc]initWithTitle:@"剪切" action:@selector(myCut:)];
    UIMenuItem * item2 = [[UIMenuItem alloc]initWithTitle:@"粘贴" action:@selector(myPaste:)];
    menu.menuItems = @[item1,item2];
    [menu setTargetRect:self.bounds inView:self];
//  [menu setTargetRect:self.frame inView:self.superview];
    [menu setMenuVisible:YES animated:YES];
}
#pragma mark - 对控件权限进行设置
/**
 *  设置label可以成为第一响应者
 *
 *  @注意：不是每个控件都有资格成为第一响应者
 */
- (BOOL)canBecomeFirstResponder
{
    return YES;
}
/**
 *  设置label能够执行那些具体操作
 *
 *  @param action 具体操作
 *
 *  @return YES:支持该操作
 */
- (BOOL)canPerformAction:(SEL)action withSender:(id)sender
{
//    NSLog(@"%@",NSStringFromSelector(action));
    if(action == @selector(cut:) || action == @selector(copy:) || action == @selector(myCut:)|| action == @selector(myPaste:)) return YES;
    return NO;
}
#pragma mark - 方法的实现
//- (void)cut:(id)sender
//{
//    
//    NSLog(@"%@",sender);
//    
//}
- (void)myCut:(UIMenuController *) menu
{
    NSLog(@"%s---%@",__func__,menu);
    //复制文字到剪切板
    [self copy:menu];
    //清空文字
    self.text = nil;
}
- (void)cut:(UIMenuController *)menu
{
    //复制文字到剪切板
    [self copy:menu];
    //清空文字
    self.text = nil;
}
- (void)copy:(UIMenuController *)menu
{
    //当没有文字的时候调用这个方法会崩溃
     if (!self.text) return;
    //复制文字到剪切板
    UIPasteboard * paste = [UIPasteboard generalPasteboard];
    paste.string = self.text;
}
- (void)myPaste:(UIMenuController *)menu
{
    //将剪切板文字赋值给label
    UIPasteboard * paste = [UIPasteboard generalPasteboard];
    self.text = paste.string;
}
@end
```
- 示例代码2：UITableViewCell的使用
```
#import "ZZYTableViewCell.h"
@implementation ZZYTableViewCell
- (BOOL)canBecomeFirstResponder
{
    return YES;
}
@end
#import "ZZYTableViewController.h"
#import "ZZYTableViewCell.h"
@interface ZZYTableViewController ()
@property (nonatomic, weak) ZZYTableViewCell * selectCell;
@end
- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
{
    //当menucontroller显示，点击不同的cell时为什么会显示。
//    menuController的显示依赖于第一响应者，当点击另外的cell时，当前cell取消第一响应者状态，menucontroller自动消失
    UIMenuController * menu = [UIMenuController sharedMenuController];
    NSLog(@"%d",menu.isMenuVisible);
    //防止点击多次创建
    if (menu.isMenuVisible)
    {
        [menu setMenuVisible:NO animated:YES];
    }
    else
    {
    ZZYTableViewCell * cell = [tableView cellForRowAtIndexPath:indexPath];
        self.selectCell = cell;
    [cell becomeFirstResponder];
    UIMenuItem * item0 = [[UIMenuItem alloc]initWithTitle:@"分享" action:@selector(share:)];
    UIMenuItem * item1 = [[UIMenuItem alloc]initWithTitle:@"评论" action:@selector(comment:)];
    UIMenuItem * item2 = [[UIMenuItem alloc]initWithTitle:@"点赞" action:@selector(praise:)];
    menu.menuItems = @[item0,item1,item2];
    [menu setTargetRect:CGRectMake(0, cell.frame.size.height * 0.5, cell.frame.size.width, cell.frame.size.height) inView:cell];
    [menu setMenuVisible:YES animated:YES];
    }
}
- (void)share:(UIMenuController *)menu
{
    NSLog(@"%@",self.selectCell.textLabel.text);
}
- (void)comment:(UIMenuController *)menu
{
}
- (void)praise:(UIMenuController *)menu
{
}
//防止拖动tableView时产生的BUG
- (void)scrollViewWillBeginDragging:(UIScrollView *)scrollView
{
    UIMenuController * menu = [UIMenuController sharedMenuController];
    [menu setMenuVisible:NO animated:YES];
}
```
文／断剑（简书作者）
原文链接：http://www.jianshu.com/p/71076f65835d
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
