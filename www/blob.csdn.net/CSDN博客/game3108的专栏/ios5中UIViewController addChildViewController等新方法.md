# ios5中UIViewController addChildViewController等新方法 - game3108的专栏 - CSDN博客
2015年03月10日 15:25:24[game3108](https://me.csdn.net/game3108)阅读数：420标签：[ios](https://so.csdn.net/so/search/s.do?q=ios&t=blog)
个人分类：[转载](https://blog.csdn.net/game3108/article/category/2926393)
### 情况：在项目过程中，修改原来的rootViewController,发现其中使用了addChildViewController。目的是使得RootViewController能够进行多个view的切换。
实际上，项目的实现是使用了一个scrollview，contentvSize设置成N个屏幕大小，将每个view加载到相应的位置，通过pageEnabled进行切换。所以虽然代码中含有addChildViewController，但实际上没有使用，所以转载一篇写的不错的文章进行学习。
个人理解：
使用scrollview的方式：比较容易理解和操作，在一开始初始化rootviewcontroller的情况下，就可以把所有的view一起加载好。但也因此加载会比较缓慢。
使用addChildViewController的方式：代码分离，使用view的时候再加载。
### 原博客地址：http://justsee.iteye.com/blog/1829687
### [ios5中UIViewController addChildViewController等新方法](http://justsee.iteye.com/blog/1829687)
**博客分类：**[ios](http://justsee.iteye.com/category/223124)[objective-c](http://justsee.iteye.com/category/223123)[ios-course](http://justsee.iteye.com/category/264972)
ios4还是3，以前基于在vviewcontroller上的切换，基本上就2中方式，导航控制器上的push，或者普通控制器上modal，现在终于多了一个这种玩意儿了。在ios5中，ViewController中新添加了下面几个方法：
> 
addChildViewController: 
removeFromParentViewController 
transitionFromViewController:toViewController:duration:options:animations:completion: 
willMoveToParentViewController: 
didMoveToParentViewController:
下面详细介绍一下addChildViewController，一个ViewController可以添加多个子ViewController，但是这些子ViewControlle要想显示出来，需要把子controller的视图添加到父视图的相应位置r，可以通过transitionFromViewController:toViewController:duration:options:animations:completion:这个方法转换显示的子视图。同时加入相应的动画。下面以一个例子来说明这几个新方法：
下面详细介绍一下上述效果的实现：
- 创建项目，changeViewController。
- 添加相应的viewController，MainViewController、FirstViewController、SecondViewController、ThirdViewController。
3.把MainViewController添加到window中。
> 
- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions 
{ 
    self.window = [[UIWindow alloc] initWithFrame:[[UIScreen mainScreen] bounds]]; 
    // Override point for customization after application launch. 
    MainViewController *mainViewController=[[MainViewController alloc] initWithNibName:@"MainViewController" bundle:nil]; 
    self.window.rootViewController=mainViewController; 
    [self.window makeKeyAndVisible]; 
    return YES; 
}
4.在MainViewController中添加三个按钮，并且连接onClickbutton方法。
5.在MainViewController中添加三个子controller
> 
#pragma mark – View lifecycle
- (void)viewDidLoad 
{ 
    [super viewDidLoad]; 
    // Do any additional setup after loading the view from its nib. 
    FirstViewController *firstViewController=[[FirstViewController alloc] initWithNibName:@"FirstViewController" bundle:nil]; 
    [self addChildViewController:firstViewController]; 
    SecondViewController *secondViewController=[[SecondViewController alloc] initWithNibName:@"SecondViewController" bundle:nil]; 
    [self addChildViewController:secondViewController]; 
    ThirdViewController *thirdViewController=[[ThirdViewController alloc] initWithNibName:@"ThirdViewController" bundle:nil]; 
    [self addChildViewController:thirdViewController]; 
**    [contentView addSubview:thirdViewController.view]; **
    currentViewController=thirdViewController; 
}
其中要把其中的一个子controller的view添加到根视图中，这样才能显示出相应的视图。
6.点击按钮，切换视图。
> 
-(IBAction)onClickbutton:(id)sender 
{ 
    FirstViewController *firstViewController=[self.childViewControllers objectAtIndex:0]; 
    ThirdViewController *thirdViewController=[self.childViewControllers objectAtIndex:2]; 
    SecondViewController *secondViewController=[self.childViewControllers objectAtIndex:1]; 
    if ((currentViewController==firstViewController&&[sender tag]==1)||(currentViewController==secondViewController&&[sender tag]==2) ||(currentViewController==thirdViewController&&[sender tag]==3) ) { 
        return; 
    } 
[UIViewController](http://wangjun.easymorse.com/?tag=uiviewcontroller) *oldViewController=currentViewController; 
    switch ([sender tag]) { 
        case 1: 
        { 
            NSLog(@"留言及回复"); 
            [self transitionFromViewController:currentViewController toViewController:firstViewController duration:4 options:UIViewAnimationOptionTransitionFlipFromLeft animations:^{ 
            }  completion:^(BOOL finished) { 
                if (finished) { 
                    currentViewController=firstViewController; 
                }else{ 
                    currentViewController=oldViewController; 
                } 
            }]; 
} 
            break; 
        case 2: 
        { 
            NSLog(@"生日提醒"); 
            [self transitionFromViewController:currentViewController toViewController:secondViewController duration:1 options:UIViewAnimationOptionTransitionFlipFromTop animations:^{ 
            }  completion:^(BOOL finished) { 
                if (finished) { 
                  currentViewController=secondViewController; 
                }else{ 
                    currentViewController=oldViewController; 
                } 
            }]; 
        } 
            break; 
        case 3: 
        { 
            NSLog(@"好友申请"); 
            [self transitionFromViewController:currentViewController toViewController:thirdViewController duration:1 options:UIViewAnimationOptionTransitionFlipFromBottom animations:^{ 
            }  completion:^(BOOL finished) { 
                if (finished) { 
                     currentViewController=thirdViewController; 
                }else{ 
                    currentViewController=oldViewController; 
                } 
            }]; 
        } 
            break; 
        default: 
            break; 
    } 
}
其中我把按钮设置成不同的tag了。
这时候点击按钮，就可以切换子视图了。
可以看到，这些view在没有使用时，是不会被load的，并且当有Memory Warning时，当前没有显示的view自动被unload掉了。
这样写的好处：
- 多个UIViewController之间切换可以添加动画
- 当内存警告的时候，可以把当前不是激活状态的ViewController内存释放。所以新的方法确实能有效地节省内存，也能方便地处理内存不足时的资源回收
- 可以把代码更好分开
thx google！
