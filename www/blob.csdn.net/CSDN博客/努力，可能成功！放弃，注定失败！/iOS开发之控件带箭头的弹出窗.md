# iOS开发之控件带箭头的弹出窗 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年05月20日 17:44:51[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：919
写了个带箭头的弹出视图的封装类，界面小伙伴们可以自己进行优化，承载视图已经暴露出来了应该是很容易修改的，个人感觉常用的效果都已经出来了^_^，有十二个箭头的位置可自由设置希望对大家有所帮助...
**1、源码下载地址[WBPopOverView](https://github.com/ulovebin/WBPopView)**
**2、效果图**
![](http://upload-images.jianshu.io/upload_images/1728983-dc6c9f10a980178f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
箭头在上中效果图.png
![](http://upload-images.jianshu.io/upload_images/1728983-f307378acef03b80.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
箭头在上左效果图.png
**3、打开工程找到类库的文件，可以直接拖到自己工程使用**
￼
![](http://upload-images.jianshu.io/upload_images/1728983-158472ee54e74255.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
类库文件.png
**4、在这里可以看代码实例**
![](http://upload-images.jianshu.io/upload_images/1728983-0c4a301a27477474.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
实例文件.png
**5、代码讲解**
- 代码实例
```
#import "ViewController.h"
#import "WBPopOverView.h"
#define Swith self.view.frame.size.width
#define Sheight self.view.frame.size.height
@interface ViewController ()
@property(strong,readwrite,nonatomic)UIButton *button;
@property(strong,readwrite,nonatomic)NSMutableArray *testArr;
@end
@implementation ViewController
- (void)viewDidLoad {
    [super viewDidLoad];
    _button=[[UIButton alloc]initWithFrame:CGRectMake(Swith/2-50, 100, 100, 30)];
    [_button setTitle:@"测试" forState:UIControlStateNormal];
    [_button setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];
    [_button addTarget:self action:@selector(btnclick) forControlEvents:UIControlEventTouchUpInside];
    _button.backgroundColor=[UIColor lightGrayColor];
    [self.view addSubview:_button];
}
-(void)btnclick
{
    CGPoint point=CGPointMake(_button.frame.origin.x+_button.frame.size.width/2, _button.frame.origin.y+_button.frame.size.height);//箭头点的位置
    WBPopOverView *view=[[WBPopOverView alloc]initWithOrigin:point Width:200 Height:300 Direction:WBArrowDirectionUp2];//初始化弹出视图的箭头顶点位置point，展示视图的宽度Width，高度Height，Direction以及展示的方向
    UILabel *lable=[[UILabel alloc]initWithFrame:CGRectMake(10, 10, 100, 30)];
    lable.text=@"测试内容";
    lable.textColor=[UIColor whiteColor];
    [view.backView addSubview:lable];
    [view popView];
}
@end
```
- 我们初始化弹出视图时需要弹出视图**“箭头的顶点位置”**、**“视图的宽高”**、**“箭头的指向”**
- 我们向弹出视图中添加子视图可以通过.backView来进行操作
![](http://upload-images.jianshu.io/upload_images/1728983-c5b142ba5b4cdc02.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
添加子视图到弹窗.png
- 我们可以在初始化弹出视图时设置箭头位置方向，这里共定义了十二个位置
![](http://upload-images.jianshu.io/upload_images/1728983-3e67e7ec175d70ee.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
箭头方向.png
文／逆袭的小菜鸟（简书作者）
原文链接：http://www.jianshu.com/p/c762bf00d825
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
