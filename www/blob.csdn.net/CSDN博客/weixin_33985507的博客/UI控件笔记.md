# UI控件笔记 - weixin_33985507的博客 - CSDN博客
2016年03月30日 14:11:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：19
- UILabel
- UIViewController
- UITextField
- UIScrollView
- UIImage
- UIWindow
- UIView
- UITableView
- UIMenuController
- UINavigation
- UICollectionViewFlowLayout
- UICollectionView
- UITextView
- UILabel
self.textLabel.highlighted = YES;//设置label高亮
self.textLabel.highlightedTextColor = [UIColor grayColor];//label高亮状态下字体颜色
- 
UIViewController
1.automaticallyAdjustsScrollViewInsets
/*automaticallyAdjustsScrollViewInsets根据按所在界面的status bar，navigationbar，与tabbar的高度，自动调整scrollview的 inset,设置为no，不让viewController调整。*/
self.automaticallyAdjustsScrollViewInsets = NO;
```
2.UIViewController的presentedViewController，presentingViewController和parentViewController三个属性
  转自:http://www.th7.cn/Program/IOS/201507/517610.shtml
  presentedViewController：The view controller that is presented by this view controlller(read-only)，被本视图控制器present出来的的视图控制器（只读）
  presentingViewController：The view controller that presented this view controller. (read-only)，present出来本视图控制器的视图控制器（只读）
  parentViewController：The parent view controller of the recipient(容器). (read-only)
  
  有ViewControllerA和ViewControllerB，在A里使用presentViewController:animated:completion: 方法present视图控制器B(如下)：
  ViewControllerB *viewb = [[ViewControllerBalloc] init];
  [selfpresentViewController:viewb animated:YEScompletion:nil];
  NSLog(@"self.presentedViewController：     %@",self.presentedViewController);
  NSLog(@"self.presentingViewController：    %@",self.presentingViewController);
  NSLog(@"self.parentViewController：        %@",self.parentViewController);
  NSLog(@"viewb.presentedViewController：    %@", viewb.presentedViewController);
  NSLog(@"viewb.presentingViewController：   %@", viewb.presentingViewController);
  NSLog(@"viewb.parentViewController：       %@", viewb.parentViewController);
  
  打印结果为：
  2015-07-30 10:38:45.923 PresentDemo[1518:85346] self.presentedViewController：     <ViewControllerB: 0x7ff6c974a440>
  2015-07-30 10:38:45.924 PresentDemo[1518:85346] self.presentingViewController：    (null)
  2015-07-30 10:38:45.924 PresentDemo[1518:85346] self.parentViewController：        (null)
  
  2015-07-30 10:38:45.924 PresentDemo[1518:85346] viewb.presentedViewController：    (null)
  2015-07-30 10:38:45.924 PresentDemo[1518:85346] viewb.presentingViewController：   <ViewController: 0x7ff6c9729e40>
  2015-07-30 10:38:45.924 PresentDemo[1518:85346] viewb.parentViewController：       (null)
  
  视图控制器B被本视图控制器（A）present，所以A的presentedViewController是ViewControllerB，B的presentingViewController是ViewController（即A）
```
- UITextField
// 设置光标颜色
self.tintColor = [UIColor whiteColor];
// 修改占位文字颜色
[self setValue:[UIColor whiteColor] forKeyPath:@"_placeholderLabel.textColor"];
- UIScrollView
// 设置滚动条的内边距等于contentInset
vc.tableView.scrollIndicatorInsets = vc.tableView.contentInset;
- 
UIImage
1.把图片写入相册
-(IBAction)save {
if (self.imageView.image == nil) {
return;
}
// 将图片写入相册
UIImageWriteToSavedPhotosAlbum(self.imageView.image, self, @selector(image:didFinishSavingWithError:contextInfo:), nil);
}
```
-(void)image:(UIImage *)image didFinishSavingWithError:(NSError *)error contextInfo:(void *)contextInfo
    {
       if (error) {
            NSLog(@"保存失败!");
        } else {
            NSLog(@"保存成功!");
        }
   }
 2.图片圆形切图
@implementation UIImage (Extension)
 -(UIImage *)circleImage
{
  // NO代表透明
  UIGraphicsBeginImageContextWithOptions(self.size, NO, 0.0);
  
  // 获得上下文
  CGContextRef ctx = UIGraphicsGetCurrentContext();
  
  // 添加一个圆
  CGRect rect = CGRectMake(0, 0, self.size.width, self.size.height);
  CGContextAddEllipseInRect(ctx, rect);
  
  // 裁剪
  CGContextClip(ctx);
  
  // 将图片画上去
  [self drawInRect:rect];
  
  UIImage *image = UIGraphicsGetImageFromCurrentImageContext();
  
  UIGraphicsEndImageContext();
  
  return image;
}
```
- 
UIWindow
keyWindow:一个窗口当前能接受键盘和非触摸事件时，便被认为是主窗口。而触摸事件则被投递到触摸发生的窗口，没有相应坐标值的事件被投递到主窗口
。同一时刻只有一个窗口是主窗口。
UIWindow还定义了以下通知：
· UIWindowDidBecomeVisibleNotification
· UIWindowDidBecomeHiddenNotification
· UIWindowDidBecomeKeyNotification
· UIWindowDidResignKeyNotification
.每当应用窗口发生编程变化时，UIWindow通知就会被投递。例如，当你的应用显示或者隐藏一个窗口时，UIWindowDidBecomeVisibleNotification和
UIWindowDidBecomeHiddenNotification通知相应地就会被投递。值得注意的是，当应用转移到后台时，这些通知不会被投递：即便应用转到后台时，
窗口不会显示，窗口在应用的上下文中仍然被认为是可见的。
.大多数应用不需要处理UIWindowDidBecomeVisibleNotification和UIWindowDidBecomeHiddenNotification通知，很少有应用拥有一个以上窗口。
.UIWindowDidBecomeKeyNotification 和 UIWindowDidResignKeyNotification能帮助你跟踪应用窗口何时是主窗口，何时不是。
当你通过显示一个输入配件视图来或者用户输入时，你也许需要知道一个窗口是不是主窗口。
```
1.自定义window
  //窗口级别: UIWindowLevelNormal < UIWindowLevelStatusBar < UIWindowLevelAlert
  UIWindow *window;
  window = [[UIWindow alloc] init];
  window.rootViewController = [UIViewController new];//ios需要设置根控制器
  window.frame = CGRectMake(0, 0, 375, 20);
  window.backgroundColor = [UIColor yellowColor];
  window.windowLevel = UIWindowLevelStatusBar;
  //显示窗口
  window.hidden = NO;
  //成为主窗口并显示
  //[window makeKeyAndVisible];
  2.设置状态栏颜色
  //设置info.plist 
  View controller-based status bar appearance = NO//不让控制器控制状态栏
  [UIApplication sharedApplication].statusBarStyle = UIStatusBarStyleLightContent;//设置全局状态栏
  如果设置info.plist View controller-based status bar appearance = YES//让控制器设置状态栏
  /**
   * 让当前控制器对应的状态栏是白色
   */
  -(UIStatusBarStyle)preferredStatusBarStyle
  {
      return UIStatusBarStyleLightContent;
  }
  状态栏由最上面的window的根控制器控制。
 // 更新状态栏(self是控制器)
 [self setNeedsStatusBarAppearanceUpdate];
 3.判断view是否在主窗口显示
 @implementation UIView (XMGExtension)
  - (BOOL)isShowingOnKeyWindow
  {
      // 主窗口
      UIWindow *keyWindow = [UIApplication sharedApplication].keyWindow;
      
      // 以主窗口左上角为坐标原点, 计算self的矩形框
      CGRect newFrame = [keyWindow convertRect:self.frame fromView:self.superview];
      CGRect winBounds = keyWindow.bounds;
      
      // 主窗口的bounds 和 self的矩形框 是否有重叠
      BOOL intersects = CGRectIntersectsRect(newFrame, winBounds);
      
      return !self.isHidden && self.alpha > 0.01 && self.window == keyWindow && intersects;
  }
```
- 
UIView
1.autoresizingMask
//在 UIView 中有一个autoresizingMask的属性，它对应的是一个枚举的值（如下），属性的意思就是自动调整子控件与父控件中间的位置，宽高。
enum {
UIViewAutoresizingNone                 = 0,
UIViewAutoresizingFlexibleLeftMargin   = 1 << 0,
UIViewAutoresizingFlexibleWidth        = 1 << 1,
UIViewAutoresizingFlexibleRightMargin  = 1 << 2,
UIViewAutoresizingFlexibleTopMargin    = 1 << 3,
UIViewAutoresizingFlexibleHeight       = 1 << 4,
UIViewAutoresizingFlexibleBottomMargin = 1 << 5
};
/*
UIViewAutoresizingNone就是不自动调整。
UIViewAutoresizingFlexibleLeftMargin 自动调整与superView左边的距离，保证与superView右边的距离不变。
UIViewAutoresizingFlexibleRightMargin 自动调整与superView的右边距离，保证与superView左边的距离不变。
UIViewAutoresizingFlexibleTopMargin 自动调整与superView顶部的距离，保证与superView底部的距离不变。
UIViewAutoresizingFlexibleBottomMargin 自动调整与superView底部的距离，也就是说，与superView顶部的距离不变。
UIViewAutoresizingFlexibleWidth 自动调整自己的宽度，保证与superView左边和右边的距离不变。
UIViewAutoresizingFlexibleHeight 自动调整自己的高度，保证与superView顶部和底部的距离不变。
UIViewAutoresizingFlexibleLeftMargin  |UIViewAutoresizingFlexibleRightMargin 自动调整与superView左边的距离，保证与左边的距离和右边的距离和原来距左边和右边的距离的比例不变。比如原来距离为20，30，调整后的距离应为68，102，即68/20=102/30。
*/
2.setNeedsDisplay和setNeedsLayout
UIView的setNeedsDisplay和setNeedsLayout方法。首先两个方法都是异步执行的。而setNeedsDisplay会调用 自动调用drawRect方法，
这样可以拿到UIGraphicsGetCurrentContext，就可以画画了。而setNeedsLayout 会默认调用layoutSubViews，就可以处理子视图中的一些数据。
宗上所诉，setNeedsDisplay方便绘图，而layoutSubViews方便出来数据
```
layoutSubviews在以下情况下会被调用：
  1、init初始化不会触发layoutSubviews。
  2、addSubview会触发layoutSubviews。
  3、设置view的Frame会触发layoutSubviews，当然前提是frame的值设置前后发生了变化。
  4、滚动一个UIScrollView会触发layoutSubviews。
  5、旋转Screen会触发父UIView上的layoutSubviews事件。
  6、改变一个UIView大小的时候也会触发父UIView上的layoutSubviews事件。
  7、直接调用setLayoutSubviews。
  
  drawRect在以下情况下会被调用：
  1、如果在UIView初始化时没有设置rect大小，将直接导致drawRect不被自动调用。drawRect调用是在Controller->loadView, Controller->viewDidLoad 两方法之后掉用的.所以不用担心在控制器中,这些View的drawRect就开始画了.这样可以在控制器中设置一些值给View(如果这些View draw的时候需要用到某些变量值).
  2、该方法在调用sizeToFit后被调用，所以可以先调用sizeToFit计算出size。然后系统自动调用drawRect:方法。
  3、通过设置contentMode属性值为UIViewContentModeRedraw。那么将在每次设置或更改frame的时候自动调用drawRect:。
  4、直接调用setNeedsDisplay，或者setNeedsDisplayInRect:触发drawRect:，但是有个前提条件是rect不能为0。
  以上1,2推荐；而3,4不提倡
  
  drawRect方法使用注意点：
  1、若使用UIView绘图，只能在drawRect：方法中获取相应的contextRef并绘图。如果在其他方法中获取将获取到一个 invalidate的ref并且不能用于画图。drawRect：方法不能手动显示调用，必须通过调用setNeedsDisplay 或者 setNeedsDisplayInRect，让系统自动调该方法。
  2、若使用calayer绘图，只能在drawInContext: 中（类似于drawRect）绘制，或者在delegate中的相应方法绘制。同样也是调用setNeedDisplay等间接调用以上方法
  3、若要实时画图，不能使用gestureRecognizer，只能使用touchbegan等方法来掉用setNeedsDisplay实时刷新屏幕
```
- UITableView
1.
// cell的高度设置,cell的xib内容已经确定,ios8以后才支持
self.tableView.estimatedRowHeight = 44;//初始化高度
self.tableView.rowHeight = UITableViewAutomaticDimension;//计算高度
2.
// 去掉分割线
self.tableView.separatorStyle = UITableViewCellSeparatorStyleNone;
3.
// 内边距
self.tableView.contentInset = UIEdgeInsetsMake(0, 0, 0, 0);
4.
cell.accessoryType = UITableViewCellAccessoryNone;//cell没有任何的样式
cell.accessoryType = UITableViewCellAccessoryDisclosureIndicator;//cell的右边有一个小箭头，距离右边有十几像素；
cell.accessoryType = UITableViewCellAccessoryDetailDisclosureButton;//cell右边有一个蓝色的圆形button；
cell.accessoryType = UITableViewCellAccessoryCheckmark;//cell右边的形状是对号；
- UIMenuController
UILabel实现复制剪切功能
![1766523-15260e6902dd66be.png](https://upload-images.jianshu.io/upload_images/1766523-15260e6902dd66be.png)
7DBEF01C-5643-4F95-83A3-B0DBDDE4F9EB.png
```
＃import <UIKit/UIKit.h>
    @interface XXLabel : UILabel
    @end
    @implementation XXLabel
    - (void)awakeFromNib
    {
        [self setup];
    }
    
    - (instancetype)initWithFrame:(CGRect)frame
    {
        if (self = [super initWithFrame:frame]) {
            [self setup];
        }
        return self;
    }
    
    - (void)setup
    {
        self.userInteractionEnabled = YES;
        [self addGestureRecognizer:[[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(labelClick)]];
    }
    
    /**
     * 让label有资格成为第一响应者
     */
    - (BOOL)canBecomeFirstResponder
    {
        return YES;
    }
    
    /**
     * label能执行哪些操作(比如copy, paste等等)
     * @return  YES:支持这种操作
     */
    - (BOOL)canPerformAction:(SEL)action withSender:(id)sender
    {
        if (action == @selector(cut:) || action == @selector(copy:) || action == @selector(paste:)) return YES;
        
        return NO;
    }
    
    - (void)cut:(UIMenuController *)menu
    {
        // 将自己的文字复制到粘贴板
        [self copy:menu];
        
        // 清空文字
        self.text = nil;
    }
    
    - (void)copy:(UIMenuController *)menu
    {
        // 将自己的文字复制到粘贴板
        UIPasteboard *board = [UIPasteboard generalPasteboard];
        board.string = self.text;
    }
    
    - (void)paste:(UIMenuController *)menu
    {
        // 将粘贴板的文字 复制 到自己身上
        UIPasteboard *board = [UIPasteboard generalPasteboard];
        self.text = board.string;
    }
    
    - (void)labelClick
    {
        // 1.label要成为第一响应者(作用是:告诉UIMenuController支持哪些操作, 这些操作如何处理)
        [self becomeFirstResponder];
        
        // 2.显示MenuController
        UIMenuController *menu = [UIMenuController sharedMenuController];
        // targetRect: MenuController需要指向的矩形框
        // targetView: targetRect会以targetView的左上角为坐标原点
        [menu setTargetRect:self.bounds inView:self];
        //    [menu setTargetRect:self.frame inView:self.superview];
        [menu setMenuVisible:YES animated:YES];
    }
    @end
```
UILabel实现弹出自定义功能
![1766523-098b4acb8361269f.png](https://upload-images.jianshu.io/upload_images/1766523-098b4acb8361269f.png)
6D53A983-44A2-44BC-B2C5-A176D17B36F5.png
```
＃import "XXLabel.h"
    @interface XXLabel : UILabel
    @end
    @implementation XXLabel
    - (void)awakeFromNib
    {
        [self setup];
    }
    
    - (instancetype)initWithFrame:(CGRect)frame
    {
        if (self = [super initWithFrame:frame]) {
            [self setup];
        }
        return self;
    }
    
    - (void)setup
    {
        self.userInteractionEnabled = YES;
    }
    
    /**
     * 让label有资格成为第一响应者
     */
    - (BOOL)canBecomeFirstResponder
    {
        return YES;
    }
    
    /**
     * label能执行哪些操作(比如copy, paste等等)
     * @return  YES:支持这种操作
     */
    - (BOOL)canPerformAction:(SEL)action withSender:(id)sender
    {
        return NO;
    }
    @end
    ＃import "ViewController.h"
    ＃import "XXLabel.h"
    @interface ViewController ()
    @property (weak, nonatomic) IBOutlet UIWebView *webView;
    @property (weak, nonatomic) IBOutlet XXLabel *label;
    @end
    
    @implementation ViewController
    
    - (void)viewDidLoad {
        [super viewDidLoad];
        
        //    [self.webView loadRequest:[NSURLRequest requestWithURL:[NSURL URLWithString:@"http://www.baidu.com"]]];
        [self.webView loadHTMLString:@"<div style=\"color:red;\">哈哈哈哈</div>" baseURL:nil];
        
        [self.label addGestureRecognizer:[[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(labelClick)]];
    }
    
    - (void)labelClick
    {
        // 1.label要成为第一响应者(作用是:告诉UIMenuController支持哪些操作, 这些操作如何处理)
        [self.label becomeFirstResponder];
        
        // 2.显示MenuController
        UIMenuController *menu = [UIMenuController sharedMenuController];
        
        // 添加MenuItem, 要添加到控制器
        UIMenuItem *ding = [[UIMenuItem alloc] initWithTitle:@"顶" action:@selector(ding:)];
        UIMenuItem *replay = [[UIMenuItem alloc] initWithTitle:@"回复" action:@selector(replay:)];
        UIMenuItem *report = [[UIMenuItem alloc] initWithTitle:@"举报" action:@selector(report:)];
        menu.menuItems = @[ding, replay, report];
        
        [menu setTargetRect:self.label.bounds inView:self.label];
        [menu setMenuVisible:YES animated:YES];
    }
    
    - (void)ding:(UIMenuController *)menu
    {
        NSLog(@"%s %@", __func__ , menu);
    }
    
    - (void)replay:(UIMenuController *)menu
    {
        NSLog(@"%s %@", __func__ , menu);
    }
    
    - (void)report:(UIMenuController *)menu
    {
        NSLog(@"%s %@", __func__ , menu);
    }
    @end
```
- 
UINavigation
```
+ (id)appearanceWhenContainedIn:(Class <>)ContainerClass,...
  这个方法可设置某个类的改变：例如：设置UIBarButtonItem 在UINavigationBar、UIPopoverController、UITabbar中的效果。就可以这样写
  [[UIBarButtonItem appearanceWhenContainedIn:[UINavigationBar class], [UIPopoverController class],[UITabbar class] nil] setTintColor:myPopoverNavBarColor];
1.设置Navi的item文字属性
  //UIBarButtonItem *item = [UIBarButtonItem appearance];//设置navi全局
  NSMutableDictionary *itemAttrs = [NSMutableDictionary dictionary];
  itemAttrs[NSForegroundColorAttributeName] = [UIColor blackColor];
  itemAttrs[NSFontAttributeName] = [UIFont systemFontOfSize:11];
  itemAttrs[NSForegroundColorAttributeName] = [UIColor greenColor];
  [self.navigationItem.rightBarButtonItem setTitleTextAttributes:itemAttrs forState:UIControlStateNormal];
  
  // 强制刷新
  [self.navigationController.navigationBar layoutIfNeeded];
2.自定义navi左侧按钮,返回侧滑失效
  self.interactivePopGestureRecognizer.delegate = nil;//设置代理为空
```
- 
UICollectionViewFlowLayout
流水中间变大布局
@interface XXXLineLayout : UICollectionViewFlowLayout
@end
@implementation XXXLineLayout
```
- (instancetype)init
  {
      if (self = [super init]) {
      }
      return self;
  }
  
  /**
   * 当collectionView的显示范围发生改变的时候，是否需要重新刷新布局
   * 一旦重新刷新布局，就会重新调用下面的方法：
   1.prepareLayout
   2.layoutAttributesForElementsInRect:方法
   */
  - (BOOL)shouldInvalidateLayoutForBoundsChange:(CGRect)newBounds
  {
      return YES;
  }
  
  /**
   * 用来做布局的初始化操作（不建议在init方法中进行布局的初始化操作）
   */
  - (void)prepareLayout
  {
      [super prepareLayout];
      
      // 水平滚动
      self.scrollDirection = UICollectionViewScrollDirectionHorizontal;
      
      // 设置内边距
      CGFloat inset = (self.collectionView.frame.size.width - self.itemSize.width) * 0.5;
      self.sectionInset = UIEdgeInsetsMake(0, inset, 0, inset);
  }
  
  /**
   UICollectionViewLayoutAttributes *attrs;
   1.一个cell对应一个UICollectionViewLayoutAttributes对象
   2.UICollectionViewLayoutAttributes对象决定了cell的frame
   */
  /**
   * 这个方法的返回值是一个数组（数组里面存放着rect范围内所有元素的布局属性）
   * 这个方法的返回值决定了rect范围内所有元素的排布（frame）
   */
  - (NSArray *)layoutAttributesForElementsInRect:(CGRect)rect
  {
      // 获得super已经计算好的布局属性
      NSArray *array = [super layoutAttributesForElementsInRect:rect];
      
      // 计算collectionView最中心点的x值
      CGFloat centerX = self.collectionView.contentOffset.x + self.collectionView.frame.size.width * 0.5;
      
      // 在原有布局属性的基础上，进行微调
      for (UICollectionViewLayoutAttributes *attrs in array) {
          // cell的中心点x 和 collectionView最中心点的x值 的间距
          CGFloat delta = ABS(attrs.center.x - centerX);
          
          // 根据间距值 计算 cell的缩放比例
          CGFloat scale = 1 - delta / self.collectionView.frame.size.width;
          
          // 设置缩放比例
          attrs.transform = CGAffineTransformMakeScale(scale, scale);
      }
      
      return array;
  }
  
  /**
   * 这个方法的返回值，就决定了collectionView停止滚动时的偏移量
   */
  - (CGPoint)targetContentOffsetForProposedContentOffset:(CGPoint)proposedContentOffset withScrollingVelocity:(CGPoint)velocity
  {
      // 计算出最终显示的矩形框
      CGRect rect;
      rect.origin.y = 0;
      rect.origin.x = proposedContentOffset.x;
      rect.size = self.collectionView.frame.size;
      
      // 获得super已经计算好的布局属性
      NSArray *array = [super layoutAttributesForElementsInRect:rect];
      
      // 计算collectionView最中心点的x值
      CGFloat centerX = proposedContentOffset.x + self.collectionView.frame.size.width * 0.5;
      
      // 存放最小的间距值
      CGFloat minDelta = MAXFLOAT;
      for (UICollectionViewLayoutAttributes *attrs in array) {
          if (ABS(minDelta) > ABS(attrs.center.x - centerX)) {
              minDelta = attrs.center.x - centerX;
          }
      }
      
      // 修改原有的偏移量
      proposedContentOffset.x += minDelta;
      return proposedContentOffset;
  }
  
  @end
```
- 
UICollectionView
1.获取当前现实在屏幕中的最后一个cell
func save() {
//保存图片到本地
```
//获取当前现实在屏幕中的最后一个cell
  let index = collectionView.indexPathsForVisibleItems().last!
  let cell = collectionView.cellForItemAtIndexPath(index) as! YQTableViewCell
  let image = cell.iconView.image
  //第一个参数是图片,第二个参数是当前控制器,第三个参数是当前控制器的didFinishSavingWithError:contextInfo:方法
  UIImageWriteToSavedPhotosAlbum(image!, self, #selector(YQTableViewController.image(_:didFinishSavingWithError:contextInfo:)), nil)
    }
  func image(image:UIImage, didFinishSavingWithError error:NSError?, contextInfo:AnyObject){
  }
  2.跳到第0组的第10个cell
  节选自 http://my.oschina.net/u/2340880/blog/522613
  collectionVeiw.scrollToItemAtIndexPath(NSIndexPath(forItem: 10, inSection: 0),atScrollPosition: UICollectionViewScrollPosition.Left, animated: true)
 //设置选中某一item，并使视图滑动到相应位置，scrollPosition是滑动位置的相关参数，如下：
  public struct UICollectionViewScrollPosition : OptionSetType {
      public init(rawValue: UInt)
      //无
      public static var None: UICollectionViewScrollPosition { get }
      
      // The vertical positions are mutually exclusive to each other, but are bitwise or-able with the horizontal scroll positions.
      // Combining positions from the same grouping (horizontal or vertical) will result in an NSInvalidArgumentException.
      //垂直布局时使用的 对应上中下
      public static var Top: UICollectionViewScrollPosition { get }
      public static var CenteredVertically: UICollectionViewScrollPosition { get }
      public static var Bottom: UICollectionViewScrollPosition { get }
      //水平布局时使用的  对应左中右
      // Likewise, the horizontal positions are mutually exclusive to each other.
      public static var Left: UICollectionViewScrollPosition { get }
      public static var CenteredHorizontally: UICollectionViewScrollPosition { get }
      public static var Right: UICollectionViewScrollPosition { get }
  }
```
- 
UITextView
//1.UITextView插入图片
// 1.创建附件
let attachment = NSTextAttachment()
attachment.image = image
// 设置了附件的大小
attachment.bounds = CGRectMake(0, -4, 20, 20)
```
// 2. 根据附件创建属性字符串
      let imageText = NSAttributedString(attachment: attachment)
      
      // 3.拿到当前所有的内容
      let strM = NSMutableAttributedString(attributedString: self.textView.attributedText)
      
      // 4.插入表情到当前光标所在的位置
      let range = self.textView.selectedRange
      strM.replaceCharactersInRange(range, withAttributedString: imageText)
      // 属性字符串有自己默认的尺寸
      strM.addAttribute(NSFontAttributeName, value: UIFont.systemFontOfSize(19), range: NSMakeRange(range.location, 1))
      
      // 5.将替换后的字符串赋值给UITextView
      self.textView.attributedText = strM
      // 恢复光标所在的位置
      // 两个参数: 第一个是指定光标所在的位置, 第二个参数是选中文本的个数
      self.textView.selectedRange = NSMakeRange(range.location + 1, 0)
```
