# 3D Touch-对TableView里的具体控件操作,类似微博。 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年09月01日 22:30:58[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：516标签：[3D Touch																[iOS 资源大全																[iOS学习资料																[iOS常用方法](https://so.csdn.net/so/search/s.do?q=iOS常用方法&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS学习资料&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS 资源大全&t=blog)](https://so.csdn.net/so/search/s.do?q=3D Touch&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
去年6S上市之后，立马入了新机，然后简单看了下3D Touch的API。当时只写了对Tableview的每个cell进行了操作的简单demo，还有就是对图标加了3D TOUCH操作。
最近的一个需求，就是TableView里的控件进行3D Touch操作。类似新浪微博那样，重按评论按钮或者重按点赞按钮，或者重按cell里的图片。
对整个Cell的3D TOUCH操作很简单，但是对立面的具体控件，一开始没思路...Google了很久，居然没找到类似。最后还是自己研究出来了...
这篇简书总体分为2部分：
一、对图标的3D TOUCH，直接代码添加，而且每次进入App后，关闭App，下次再对图标进行3D TOUCH，显示的title会改变。如下图，易车App的图标3D TOUCH的条目每次会记录你上次浏览的车型。
![](http://upload-images.jianshu.io/upload_images/2677297-d8285a65b72bdada.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
FullSizeRender.jpg
可以用NSUserDefaults或者其他数据持久化，来保存数据。
```
#import "AppDelegate.h"
#import "ViewController.h"
@interface AppDelegate ()
@end
@implementation AppDelegate
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    [self set3DTouch];
    self.window=[[UIWindow alloc]initWithFrame:[[UIScreen mainScreen]bounds]];
    self.window.backgroundColor=[UIColor whiteColor];
    self.window.rootViewController=[[UINavigationController alloc]initWithRootViewController:[ViewController new]];
    [self.window makeKeyAndVisible];
    return YES;
}
-(void)set3DTouch
{
    //每次进入app，count+1
    //这里是为了演示动态加载 3D TOUCH，使用NSUserDefaults存储数据。
    NSString * photocount = @"";
    if(![[NSUserDefaults standardUserDefaults]objectForKey:@"SharePhoto"])
    {
        [[NSUserDefaults standardUserDefaults]setObject:@"0" forKey:@"SharePhoto"];
        [[NSUserDefaults standardUserDefaults]synchronize];
         count=0;
        photocount  = @"分享照片";
    }
    else
    {
        NSString * s =(NSString*)[[NSUserDefaults standardUserDefaults]objectForKey:@"SharePhoto"];
        count=[s integerValue]+1;
        [[NSUserDefaults standardUserDefaults]setObject:[NSString stringWithFormat:@"%ld",count] forKey:@"SharePhoto"];
        [[NSUserDefaults standardUserDefaults]synchronize];
        photocount  = [NSString stringWithFormat:@"分享照片+%ld",(long)count];
    }
    UIApplicationShortcutIcon *icon1 = [UIApplicationShortcutIcon iconWithType:UIApplicationShortcutIconTypeCompose];
    UIApplicationShortcutIcon *icon2 = [UIApplicationShortcutIcon iconWithType:UIApplicationShortcutIconTypeSearch];
    UIApplicationShortcutIcon *icon3 = [UIApplicationShortcutIcon iconWithType:UIApplicationShortcutIconTypeCapturePhoto];
    UIMutableApplicationShortcutItem *item1 = [[UIMutableApplicationShortcutItem alloc]initWithType:@"0" localizedTitle:@"创建活动" localizedSubtitle:nil icon:icon1 userInfo:nil];
    UIMutableApplicationShortcutItem *item2 = [[UIMutableApplicationShortcutItem alloc]initWithType:@"1" localizedTitle:@"查找店铺" localizedSubtitle:nil icon:icon2 userInfo:nil];
    UIMutableApplicationShortcutItem *item3 = [[UIMutableApplicationShortcutItem alloc]initWithType:@"2" localizedTitle:photocount localizedSubtitle:nil icon:icon3 userInfo:nil];
    NSArray *items = @[item1, item2, item3];
    [UIApplication sharedApplication].shortcutItems = items;
}
- (void)application:(UIApplication *)application performActionForShortcutItem:(UIApplicationShortcutItem *)shortcutItem completionHandler:(void (^)(BOOL))completionHandler
{
        NSString * type = shortcutItem.type;
        NSLog(@"type=%@",type);
    //在这里处理页面跳转   type 1、2、3就是3D touch的3个按钮。
}
```
二、自定义cell，如下图，有头像、评论按钮和点赞按钮。
![](http://upload-images.jianshu.io/upload_images/2677297-615493bcdcd669d4.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
QQ20160901-0@2x.png
新建ViewController，集成自UITableViewController。
具体注释看下面代码。
```
#import "ViewController.h"
#import "DetailViewController.h"
#import "TableViewCell.h"
#define ScreenWidth self.view.frame.size.width
#define ScreenHeight self.view.frame.size.height
@interface ViewController ()<UITableViewDelegate,UITableViewDataSource,UIViewControllerPreviewingDelegate>
@end
@implementation ViewController
- (void)viewDidLoad {
    [super viewDidLoad];
    self.tableView.tableFooterView=[UIView new];
    [self registerForPreviewingWithDelegate:self sourceView:self.tableView];  //注册代理
}
-(NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section
{
    return 20;
}
-(CGFloat)tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath
{
    return 70;
}
-(UITableViewCell*)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath
{
    TableViewCell * cell = [tableView dequeueReusableCellWithIdentifier:@"cell"];
    if(!cell)
    {
        cell=[[TableViewCell alloc]initWithStyle:UITableViewCellStyleDefault reuseIdentifier:@"cell"];
        cell.selectionStyle=UITableViewCellSelectionStyleNone;
    }
    return cell;
}
- (UIViewController *)previewingContext:(id <UIViewControllerPreviewing>)previewingContext viewControllerForLocation:(CGPoint)location
{
    NSIndexPath * IndexPath = [self.tableView indexPathForRowAtPoint:location];
    TableViewCell * cell = [self.tableView cellForRowAtIndexPath:IndexPath];  //获取touch的那个cell
    NSInteger index=3;
    NSMutableArray * array =[NSMutableArray array];
    [array addObject:cell.imageview];
    [array addObject:cell.button1];
    [array addObject:cell.button2];
    for(UIView * view in array)
    {
        if([self touchedView:view and:location])
        {
            if([view isKindOfClass:[UIButton class]]) //判断touch是不是在button上
            {
                UIButton*btn = (UIButton*)view;
                index=[array indexOfObject:btn]; // index为1或者2
            }
            else if([view isKindOfClass:[UIImageView class]])  //判断touch是不是在imageview上
            {
                index=0;
            }
        }
    }
    /*
       只有 设置了 previewingContext.sourceRect，你touch的那个控件才会突出显示，其他区域变模糊。
     */
    DetailViewController * vc=[[DetailViewController alloc]init];
    if(index==3)  //touch在button1 button2 imageview 之外的区域  即cell上
    {
        vc.string=[NSString stringWithFormat:@"3D Touched 第%ld行",IndexPath.row];
        previewingContext.sourceRect =  cell.frame;
    }
    else if(index==0)  //touch 头像
    {
        vc.string=[NSString stringWithFormat:@"3D Touched 第%ld行的头像",IndexPath.row];
        previewingContext.sourceRect = [self.view convertRect:cell.imageview.frame fromView:cell];
    }
    else if(index==1)  //touch 评论
    {
       vc.string=[NSString stringWithFormat:@"3D Touched 第%ld行的评论",IndexPath.row];
        previewingContext.sourceRect =  [self.view convertRect:cell.button1.frame fromView:cell];
    }
    else if(index==2)   //touch 赞
    {
       vc.string=[NSString stringWithFormat:@"3D Touched 第%ld行的赞",IndexPath.row];
        previewingContext.sourceRect =  [self.view convertRect:cell.button2.frame fromView:cell];
    }
    return vc;
}
-(void)previewingContext:(id<UIViewControllerPreviewing>)previewingContext commitViewController:(UIViewController *)viewControllerToCommit
{
    [self showViewController:viewControllerToCommit sender:nil];
}
-(BOOL)touchedView:(UIView*)view and:(CGPoint)location
{
    CGPoint point = [view convertPoint:location fromView:self.tableView];
    return CGRectContainsPoint(view.bounds,point);
}
```
效果截图：
按下按钮瞬间的效果
![](http://upload-images.jianshu.io/upload_images/2677297-0c8054f8641db038.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
QQ20160901-1@2x.png
![](http://upload-images.jianshu.io/upload_images/2677297-3a418c00f8589d2a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
QQ20160901-0@2x.png
![](http://upload-images.jianshu.io/upload_images/2677297-336ef013843b8295.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
QQ20160901-0@2x.png
Peek 、Pop效果
![](http://upload-images.jianshu.io/upload_images/2677297-e79a0210e1708c61.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Simulator Screen Shot 2016年9月1日 11.51.39.png
![](http://upload-images.jianshu.io/upload_images/2677297-750e81714fdcdc58.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Simulator Screen Shot 2016年9月1日 11.51.46.png
![](http://upload-images.jianshu.io/upload_images/2677297-24f4bb1250806e9e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Simulator Screen Shot 2016年9月1日 11.51.30.png
Demo下载地址：[https://github.com/Karl0n/3D-Touch----TableView-](https://github.com/Karl0n/3D-Touch----TableView-)
觉得还行的话,请给个...star。
有更好的实现，请留言，互相学习，3Q。

文／Karl0n（简书作者）
原文链接：http://www.jianshu.com/p/00e142f72686
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
