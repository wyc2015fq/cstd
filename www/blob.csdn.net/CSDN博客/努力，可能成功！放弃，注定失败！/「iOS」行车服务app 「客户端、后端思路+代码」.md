# 「iOS」行车服务app 「客户端、后端思路+代码」 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年05月23日 13:17:08[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1703
> - 最近开发了一个行车服务项目，iOS客户端采用`Objective-C`编写，
 后端采用`PHP`搭建，部署在`阿里云`，操作系统为
```
Linux
 CentOS 7.3
```
，数据库`MySQL`，服务器为`Apache`，是比较基础的`LAMP`组合。
- iOS端代码部分我会讲述整体的开发思路，一些有意思的功能点也会详细说说。
- 后端代码比较简单，想要自己尝试开发`API`的iOS开发者可以参考。
- 发这篇文的主要目的是对最近的开发做一个整体回顾，同时希望对有需要的同学提供到一些帮助，有很多不尽合理和完善的地方，也请求各位同学不吝赐教，感谢大家！
本文来自简书，原文地址:[http://www.jianshu.com/p/264961e62de7](http://www.jianshu.com/p/264961e62de7)
首先上整体的效果图：
![](http://upload-images.jianshu.io/upload_images/3587644-885a443c5380c5b9.gif?imageMogr2/auto-orient/strip)
show.gif
在POI检索结果页面，地图控件显示为空白，是因为模拟器运行的原因，真机效果良好
##### 这里是 [iOS项目地址](https://github.com/halohily/DriverEpoch)、[后端项目地址](https://github.com/halohily/DriverEpoch-Server) 。如果有帮助，希望点一下`Star`以示鼓励，感谢~
简单介绍：项目UI整体尽量保持了`饿了么`的蓝色风格，其中某些页面参考了`高德地图`、`饿了么`、`Max+`的设计风格。
### 项目功能点
- 账户、用户资料管理
- 参照`饿了么`UI的定位、天气模块
- 基于`高德地图API`开发的`POI`检索，同时界面也加入了一些和`高德地图`app类似的特性
- 自定义交互逻辑的预定及结果通知功能
- 简单参照`Max+`app的资讯模块
- 用户历史足迹、历史事件维护
### 项目使用到的API及第三方库
- `高德地图API`
- `和风天气API`
- `自己搭建的后端相关接口`
- 
```
AFNetworking
 3.0
```
- `SDWebImage`
- `MBProgressHUD`
- 项目内的`Icon`大量使用阿里巴巴的`iconfont`图标，极力推荐
### 项目涉及的技术点
- `高德地图API`的相关使用。包括`地图`、`POI检索`、`导航`等功能。
- `GCD`的使用示例。包括耗时操作的后台执行、UI更新相关操作等。
- `NSUserDefaults`维护账户信息。这里是为使用方便，仅供参考。
- `Core Animation`的使用。由于`TableView`加载时采用`HUD`的用户体验不是很友善，我自己封装了`Loading`页面。
### 开发思路
- 
#### 登录注册页面
![](http://upload-images.jianshu.io/upload_images/3587644-7378e316faa32886.PNG?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
IMG_2008.PNG
- 项目初始化时采用
```
Tabbar
 VC
```
作为`rootController`，如果此时没有账户信息，则采用模态弹出方式弹出登录页面。
- 这部分比较简单，客户端会先对用户输入的信息进行简单的合法判断，然后调用登录接口。
- 有一些关于`TextField`的点击空白和`Return`回收键盘以及呼出键盘时控件移位防止遮挡的代码可以注意一下。
- 在`Tabbar VC`初始化时，第一个tab栏的VC会进行定位操作，如果此时为已登录状态，则直接调用发送历史足迹的接口。在未登录状态下，用户点击登录按钮，登录成功时，会向第一个tab栏VC发送一个调用历史足迹接口的`通知`。
- 
#### 主界面（tabbar栏首页）
![](http://upload-images.jianshu.io/upload_images/3587644-c67333477d29aa18.PNG?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
IMG_2009.PNG
![](http://upload-images.jianshu.io/upload_images/3587644-13b961a650738a34.PNG?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
IMG_2010.PNG
- 
##### 地理位置和天气部分。
- 这部分是单独封装的`View`，UI参考了`饿了么`。地理位置和通知列表按钮的点击事件采用`代理`交由当前VC实现。
- 由于模拟器定位获取的坐标无法使用高德地图的国内API，我对运行环境做了一下判断，如果是模拟器运行，则设置了默认坐标，对应地点大概是`北京市海淀区北京理工大学`。
- 当前VC加载过程中，开启定位，在地理位置反编码的成功回调中调用天气信息的接口。注意地理位置和天气信息的UI更新放在主线程执行。
- 点击地理位置的文字时，会模态弹出重新定位的界面，`重新定位`按钮的点击事件使用代理交由主界面VC实现，避免在此界面再次实例化定位相关的对象。
- 
##### 功能栏
- 使用`UICollectionView`展示。各个`Cell`的`Icon`使用阿里巴巴的`iconfont`图标，将以往的图片素材转为字体来使用，对于素材整理、高清保真、代码方便来说有极大的效率提升。在`Web`、`移动客户端`开发领域已经非常广泛地在使用。
- 
##### 某一功能VC
![](http://upload-images.jianshu.io/upload_images/3587644-14d335ceafe5dd77.PNG?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
IMG_2011.PNG
- 这部分UI参考了`高德地图`的`附近`界面UI以及部分特性。导航栏隐去，最上面是一个地图控件，以及透明的返回按钮。下面是`POI`信息的`tableview`。在`tableview`向下滑动时，地图隐去，`tableview`的`frame`上移，导航栏出现。向上滑动到顶部时，`tableview`下移，地图出现，导航栏隐去。
- 
`tableview`滑动时特性的实现：通过
```
scrollViewDidEndDragging:(UIScrollView
 *)scrollView willDecelerate:(BOOL)decelerate
```
代理方法，判断滑动的方向，从而对`tableview`的`frame`以及导航栏等元素进行操作。
代码如下：
```
//判断滑动手势方向，决定tableview的frame改变
-(void)scrollViewDidEndDragging:(UIScrollView *)scrollView willDecelerate:(BOOL)decelerate
{
CGPoint translation = [scrollView.panGestureRecognizer translationInView:scrollView.superview];
if (translation.y>0) {
    NSLog(@"ssssss");
    {
        NSIndexPath *dic = self.poiList.indexPathsForVisibleRows.firstObject;
        if (self.poiList.frame.origin.y == 64 && dic.row == 0){
            [UIView animateWithDuration:0.2 animations:^{
                NSLog(@"visible:::%@",dic);
                showMap = YES;
                self.navigationController.navigationBar.hidden = YES;
                self.poiList.frame = listFrame;
            }];
        }
    }
}else if(translation.y<0){
    NSLog(@"tttt");
    if (self.poiList.frame.origin.y == 300){
        [UIView animateWithDuration:0.2 animations:^{
            showMap = NO;
            self.navigationController.navigationBar.hidden = NO;
            self.poiList.frame = listFullFrame;
        }];
    }
}
}
```
- 
在界面加载时，使用了自己封装的`loading`页面：
![](http://upload-images.jianshu.io/upload_images/3587644-d2830c5ca67283b3.PNG?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
IMG_2018.PNG
- 在页面view加载的最开始，将loading页的view加入到当前VC的view中。在页面数据加载完成的回调中，调用`tableview`的`reload`方法，而后将`loading`页移除。
- `loading`页使用`basicAnimation`制作了简单的呼吸动画，`keyPath`为logo图标的透明度。
- 
第一个cell的`POI`地点默认可预约，显示预约图标，点击进入预约界面。
![](http://upload-images.jianshu.io/upload_images/3587644-68d79e12dce8c96a.PNG?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
IMG_2012.PNG
- 这个部分的和后端的交互逻辑是自己定义的。一般的预约过程是预约请求发送成功后，一段时间内由店家进行同意或拒绝操作，而后客户端收到结果回执。由于本项目简单模拟此过程，均使用`http`协议进行通信。所以我的实现方式是后端收到预约请求后，直接采用随机数方式模拟店家的接受或拒绝。一定时间后，客户端调用结果查询的接口来获取预约结果。
- 实现方式为，点击预约按钮，若预约接口调用成功，按钮字样变为`预约中`，钟表icon开始转动，并且按钮变为不可点击状态。与此同时，使用`performSelector`方式延时调用获取预约结果的接口。在此网络请求的成功回调中，向主界面发送通知，而后主界面通知按钮右侧显示通知数量的红色图标。如图。
- 这里通知列表的数据是存在`UserDefaults`中的，不同设备之间无法同步。其中涉及到一些`userDefaults`中数据要求的知识，具体可以参考代码。这里是因为需求是临时想出，如果常规实现方式需要开发`获取通知数据`的接口。我为了节省开发时间所以采用这种方式来模拟实现了。这个地方在以后可以完善~
![](http://upload-images.jianshu.io/upload_images/3587644-540e6fbebbd588fc.PNG?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
IMG_2013.PNG
- 
#### 资讯Tab栏
![](http://upload-images.jianshu.io/upload_images/3587644-aa5839919c33dcb4.PNG?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
IMG_2014.PNG
- 这里没有花费过多的精力。数据预置了网易新闻相关数据。界面UI参考了我常常使用的`Max+`iOS客户端的资讯功能。第一个Cell以全图为背景，可以用来突出显示重点内容。
- 资讯详情页是一个简单的`webView`。同样使用了自己封装的`Loading`图。移除时机就简单放在了`webView`的`finishLoad`回调方法中。其实对于`Webview`真正加载完成时机的判断还有很多可以聊的东西，日后补充。
- 
#### 设置页面
![](http://upload-images.jianshu.io/upload_images/3587644-6c33b436a63f7f47.PNG?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
IMG_2015.PNG
- 这个页面显而易见是采用两个`section`的`tableview`实现。在页面的`viewWillAppear`方法中，需要调用获取缓存数据大小的方法，单独更新`清除缓存`cell的内容。因为在每次切换tab的过程中，缓存数据都有可能发生变化。
- 
##### 个人信息
- 页面依然采用`tableview`实现。在信息编辑页面，仿照了`微信`信息更改页面的逻辑。页面出现时，立即弹出键盘。关于使`textField`在页面加载完成时即成为第一响应者的问题，需要重写
```
-
 (BOOL)becomeFirstResponder
```
方法。代码如下：
```
- (BOOL)becomeFirstResponder
{
[super becomeFirstResponder];
return [self.textInput becomeFirstResponder];
}
```
![](http://upload-images.jianshu.io/upload_images/3587644-f9a57d5239608dd0.PNG?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
IMG_2016.PNG
- 
##### 历史足迹、历史预定、积分信息
![](http://upload-images.jianshu.io/upload_images/3587644-8ae5c480a7417417.PNG?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
IMG_2017.PNG
![](http://upload-images.jianshu.io/upload_images/3587644-9746bd57a147bc54.PNG?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
IMG_2019.PNG
- 这部分也依然使用了封装的`loading`页面。如果数据条目为空，则不显示`tableview`，提示`暂无相关信息`，如图。
- 
##### 清除缓存
- 这里其实只使用了`SDImageCache`的相关方法。我们还可以清除本app的缓存数据所在目录来达到真正清除缓存的目的，日后讨论。
- 缓存数据获取的初始单位是`B`，我们需要除以`1024`1次、2次来判断当前数据是在`KB`级别还是`MB`级别，并作正确显示。
- 
##### 退出登录
- 点击此按钮，清除`userDefaults`中相关数据，然后模态弹出登录页面。
- 这里可以注意的一点是，我们在模态弹出登陆页，并且再次登录之后，界面是在退出登录之前的页面的，也就是tab栏的第三个页面。如果想要再次登录之后，界面出现在tab栏的首页，我们可以在模态弹出登录页的完成回调中，对当前tabVC的
```
selected
 index
```
进行设置。代码如下：
```
DELoginViewController *loginVC = [[DELoginViewController alloc] init];
      [self presentViewController:loginVC animated:YES completion:^{
          dispatch_async(dispatch_get_global_queue(DISPATCH_QUEUE_PRIORITY_DEFAULT, 0), ^{
              // 耗时的操作
              dispatch_async(dispatch_get_main_queue(), ^{
                  // 更新界面
                  AppDelegate * appDelegate = (AppDelegate*)[UIApplication sharedApplication].delegate;
                  appDelegate.tabbarVC.selectedIndex = 0;
              });
          });
      }];
```
代码的主要开发思路基本讲述完，具体的细节大家可以和我继续交流~项目中有许多不尽合理的实现方式，小部分是考虑到时间因素。欢迎大家进行讨论、指教。
有相关的问题，欢迎大家进行留言。项目中使用的自有接口，部署在我的阿里云服务器中，可外网访问。请求大家合理使用。
后端项目相关的`sql`代码，如有需要我可以后续贴出。
如果大家有兴趣，以后我可以再讲述一下`server`端的代码，当然，后端我也是初学者的水平，仅供想要入门的同学参考。
我的个人博客网站地址： [Halo的个人博客](http://halohily.com/) ，欢迎大家访问。
代码已开源，地址在文章首部。如果点一下`star`，我真的会非常感谢~
halo
