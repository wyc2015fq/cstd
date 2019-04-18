# iOS开发拓展篇—音频处理（音乐播放器2） - weixin_33985507的博客 - CSDN博客
2016年07月23日 21:41:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
**iOS开发拓展篇—音频处理（音乐播放器2）**
说明：该文主要介绍音乐播放界面的搭建。
**一、跳转**
**1.跳转到音乐播放界面的方法选择**
　　（1）使用模态跳转（又分为手动的和自动的）
　　（2）使用xib并设置跳转
**2.两种方法的分析**
　　可以使用模态的方法，添加一个控制器，让这个控制器和音乐播放控制器类进行关联，脱线，设置标识符且在cell的点击事件中执行segue即可。
　　步骤说明：
　　（1）在storyboard中新拖入一个控制器，然后设置和playing控制器类相关联。
![](https://images0.cnblogs.com/i/450136/201408/131733150454274.png)
　　（2）设置手动跳转
![](https://images0.cnblogs.com/i/450136/201408/131719588894710.png)
　　（3）设置segue的标识符
![](https://images0.cnblogs.com/i/450136/201408/131721273115690.png)
　　（3）跳转代码处理
![](https://images0.cnblogs.com/i/450136/201408/131721342485110.png)
　　不推荐使用模态的原因如下：
　　　　当选中一首音乐跳转到播放界面进行播放后，如果要跳回到音乐列表界面，那么最常见的做法是在音乐播放控制器上添加一个按钮。
　　　　当点击的时候，销毁这个控制器（dismissed）。但是，控制器销毁了那么正在播放的音乐也就随之不在了。
　　　　且由于播放界面控制器的布局是固定的，因此这里选择的方法是使用xib进行创建。
**3.选择的方法**
　　新建一个xib，对应于音乐播放控制器。
　　xib的结构如下图所示：
![](https://images0.cnblogs.com/i/450136/201408/131723400142032.png)
　细节：控制器只需要创建一次，因此建议使用懒加载，当然也可是把播放器设置为单例
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
 1 //
 2 //  YYMusicsViewController.m
 3 //
 4 
 5 #import "YYMusicsViewController.h"
 6 #import "YYMusicModel.h"
 7 #import "MJExtension.h"
 8 #import "YYMusicCell.h"
 9 #import "YYPlayingViewController.h"
10 
11 @interface YYMusicsViewController ()
12 @property(nonatomic,strong)NSArray *musics;
13 @property(nonatomic,strong)YYPlayingViewController *playingViewController;
14 @end
15 
16 @implementation YYMusicsViewController
17 #pragma mark-懒加载
18 -(NSArray *)musics
19 {
20     if (_musics==nil) {
21         _musics=[YYMusicModel objectArrayWithFilename:@"Musics.plist"];
22     }
23     return _musics;
24 }
25 -(YYPlayingViewController *)playingViewController
26 {
27     if (_playingViewController==nil) {
28         _playingViewController=[[YYPlayingViewController alloc]init];
29     }
30     return _playingViewController;
31 }
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
**4.xib的内部细节：**
（1）已经实现了约束，用于适配ios6和ios7。
（2）设置音乐名称和歌手的View设置为半透明的，设置方法如下：
![](https://images0.cnblogs.com/i/450136/201408/131726558734735.png)
　　设置为30%
![](https://images0.cnblogs.com/i/450136/201408/131727228113897.png)
　　注意：不要再storyboard中控件的属性面板上设置透明度（这样的话，这个控件中的子控件也是同样的透明度）。
　　　　不推荐的做法：
![](https://images0.cnblogs.com/i/450136/201408/131728225457914.png)
（3）按钮点击发光
![](https://images0.cnblogs.com/i/450136/201408/131728578892511.png)
（4）设置view隐藏能够节省一些性能。（参考代码）
（5）在切换控制器的过程中，设置窗口不能点击（这样做是为了防止用户多次连续的点击歌曲名会出现的问题）。
5.补充：
　　项目代码中拖入了UIView的分类，以方便计算frame
**二、涉及到的代码**
在播放控制器的.h文件中提供一个公共对象方法接口
YYPlayingViewController.h文件
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
1 //  YYPlayingViewController.h
2 
3 #import <UIKit/UIKit.h>
4 
5 @interface YYPlayingViewController : UIViewController
6 //显示控制器
7 -(void)show;
8 @end
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
YYPlayingViewController.m文件
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
 1 //
 2 //  YYPlayingViewController.m
 3 //
 4 
 5 #import "YYPlayingViewController.h"
 6 
 7 @interface YYPlayingViewController ()
 8 - (IBAction)exit;
 9 
10 @end
11 
12 @implementation YYPlayingViewController
13 #pragma mark-公共方法
14 -(void)show
15 {
16     //1.禁用整个app的点击事件
17     UIWindow *window=[UIApplication sharedApplication].keyWindow;
18     window.userInteractionEnabled=NO;
19     
20     //2.添加播放界面
21     //设置View的大小为覆盖整个窗口
22     self.view.frame=window.bounds;
23     //设置view显示
24     self.view.hidden=NO;
25     //把View添加到窗口上
26     [window addSubview:self.view];
27     
28     //3.使用动画让View显示
29     self.view.y=self.view.height;
30     [UIView animateWithDuration:0.25 animations:^{
31         self.view.y=0;
32     } completion:^(BOOL finished) {
33         window.userInteractionEnabled=YES;
34     }];
35 }
36 #pragma mark-内部的按钮监听方法
37 //返回按钮
38 - (IBAction)exit {
39     //1.禁用整个app的点击事件
40     UIWindow *window=[UIApplication sharedApplication].keyWindow;
41     window.userInteractionEnabled=NO;
42     
43     //2.动画隐藏View
44     [UIView animateWithDuration:0.25 animations:^{
45         self.view.y=window.height;
46     } completion:^(BOOL finished) {
47         window.userInteractionEnabled=YES;
48         //设置view隐藏能够节省一些性能
49         self.view.hidden=YES;
50     }];
51 }
52 @end
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
 cell的点击事件中的处理代码：
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
 1 /**
 2  *  cell的点击事件
 3  */
 4 -(void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath
 5 {
 6     //取消选中被点击的这行
 7     [tableView deselectRowAtIndexPath:indexPath animated:YES];
 8     
 9     //调用公共方法
10     [self.playingViewController show];
11     
12 //    //执行segue跳转
13 //    [self performSegueWithIdentifier:@"music2playing" sender:nil];
14 }
```
