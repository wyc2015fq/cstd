# iOS流媒体开发之二：滑动手势控制音量、亮度和进度 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年07月12日 23:03:26[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：4183
尊重知识，转发请注明出处：[iOS流媒体开发之二：滑动手势控制音量、亮度和进度](http://www.jianshu.com/p/5396866e3256)
# 概要
看到文章的标题，小伙伴们大概会有两种反应：①这和流媒体技术没关系吧②网上有很多这个功能的实现方案。
***1、***对于第一种反应,从开发者的角度看这个确实不属于流媒体技术范畴，但是对于用户来讲这个已经是看视频时理所当然应该有的功能，密不可分，鉴于用户就是上帝，所以这个绝对属于流媒体范畴.
***2、***第二种反应确实是事实，但是能真正完美实现的不多。有6s Plus的小伙伴可以打开爱奇艺或者新浪微博，随便打开一个视频，手势增大音量出现音量无法调节到最大的bug，会剩余4格音量，只能先调节到很小的音量再次增大才可以到最大音量，当然这个是系统在6s上的一个bug，但并非无法解决，即使是爱奇艺和新浪也没有完美实现（您看到此文章的时候有可能已经修复此bug）。还有一点一般的实现方案是添加**UIPanGestureRecognizer**手势，这个方案如果只是简单的视频播放器或者写一个Demo完全没问题，可是放在一个稍微复杂点的视频播放器会引起很多问题，后面再说，因此我这里提供另一种方案。
![](http://upload-images.jianshu.io/upload_images/1347742-4096c402a2a5f809.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/300)
新浪微博手势调节音量bug.png
# 思考
这个功能对于有一些开发经验的人来说并不难，只要勤快点，别遇到点难题没怎么思考就去找第三方库，都可以靠自己弄个八九不离十。我这里想通过这个功能向大家介绍我平时解决一些问题的心路历程，希望能帮到大家的不仅仅是技术本身，还有一种解决问题的能力。对于喜欢“废话少说，直接上代码”的小伙伴，这篇文章可能会让你有些不舒服，因为在我看来代码一文不值，值钱的是我们的思想，别急，咱们慢慢来。
- 分析问题，抽离本质
当我们拿到这个需求的时候，首先不要想我用哪些类可以实现，我要添加在哪个view上，我们首先要分析出这个问题的本质，对于手势调节这个问题的本质很简单，就是用户左右、上下滑动时根据手指移动的方向和偏移量改变音量等设置。
- 捋顺逻辑，列出方案
分析出问题的本质后，依然不要去想和代码有关的事情，接着我们要思考处理问题的逻辑并给出一个目前看来可行的方案。我平时会找一张白纸，画一些草图帮助记忆和理解，当然曾几度想用xmind等思维导图，但是发现还是朴实的白纸适合我，小伙伴们可以根据自身选择，只要能帮助捋顺思路就可以了。
```
第一步我们要检测到手指在屏幕上的滑动，可以使用UIPanGestureRecognizer手势，前面说了这个会引起很多问题，比如一个视频播放器不仅仅有滑动手势还是单击、双击、左滑退出UINavigationController，各种手势会冲突，其次我们有时会在拖动前或者结束后处理一些事情在再执行拖动，虽然UIPanGestureRecognizer有UIGestureRecognizerStateBegan、UIGestureRecognizerStateEnded这些状态，可是手指点击屏幕没有滑动的时候无法触发，因此我最后弃用这个方案，改为使用-
 (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(nullable UIEvent *)event；系列方法，具体实现看后面代码；第二步要判断用户向哪个方向移动了，以便调用相应的设置，这可以根据手指在上下左右的偏移量来计算，我的方案是设置一个初始偏移量的数值，比如30，上下左右哪个方向偏移量先到达30，手势就设定为对应的方向；第三步就是调用对应的方法去改变音量、亮度和进度了，具体实现看后面。
```
# 行动
逻辑通了，方案有了，现在开始行动吧，首先你要有个可以播放视频demo，最好是点播视频，可参考上篇博文：[iOS流媒体开发之一：总结系统提供的接口](http://www.jianshu.com/p/6c6b59a875c1)，这里不赘述。
- 自定义一个UIButton
我们需要在视频上添加一个透明的button来捕捉和响应用户的点击滑动屏幕的事件，同时在.h文件中声明一个代理，向相应的页面传递自定义button的响应事件，代码如下：
.h文件
```
#import <UIKit/UIKit.h>
    @protocol ZYLButtonDelegate <NSObject>
    /**
     * 开始触摸
     */
    - (void)touchesBeganWithPoint:(CGPoint)point;
    /**
     * 结束触摸
     */
    - (void)touchesEndWithPoint:(CGPoint)point;
    /**
     * 移动手指
     */
    - (void)touchesMoveWithPoint:(CGPoint)point;
    @end
    @interface ZYLButton : UIButton
    /**
     * 传递点击事件的代理
     */
    @property (weak, nonatomic) id <ZYLButtonDelegate> touchDelegate;
    @end
```
.m文件
```
#import "ZYLButton.h"
    @implementation ZYLButton
    //触摸开始
    - (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
        [super touchesBegan:touches withEvent:event];
        //获取触摸开始的坐标
        UITouch *touch = [touches anyObject];
        CGPoint currentP = [touch locationInView:self];
        [self.touchDelegate touchesBeganWithPoint:currentP];
    }
    //触摸结束
    - (void)touchesEnded:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
        [super touchesEnded:touches withEvent:event];
        UITouch *touch = [touches anyObject];
        CGPoint currentP = [touch locationInView:self];
        [self.touchDelegate touchesEndWithPoint:currentP];
    }
    //移动
    - (void)touchesMoved:(NSSet<UITouch *> *)touches withEvent:(nullable UIEvent *)event {
        UITouch *touch = [touches anyObject];
        CGPoint currentP = [touch locationInView:self];
        [self.touchDelegate touchesMoveWithPoint:currentP];
    }
    @end
```
`注意：之所以选择UIButton有2点原因：1、不用手动开启userInteractionEnabled用户交互2、同时可以很方便的为UIButton添加Target，不需要像UIView那样在再定义一个UITapGestureRecognizer，当然UIButton添加各种状态的背景颜色各背景图也要比UIView方便得多。`
- 
将自定义的Button添加到视频上
```
//添加自定义的Button到视频画面上
      self.button = [[ZYLButton alloc] initWithFrame:playerLayer.frame];
      self.button.touchDelegate = self;
      [playerView addSubview:self.button];
```
- 
在代理方法中改变定音量、亮度和进度
首先定义个一枚举表示上下左右，这里只需要判断手指是上下还是左右滑动
```
typedef NS_ENUM(NSUInteger, Direction) {
      DirectionLeftOrRight,
      DirectionUpOrDown,
      DirectionNone
  };
```
同时声明一个表示方向的变量、一个记录用户触摸视频时的坐标变量、一个记录用户触摸视频时的亮度和音量大小的变量和一个记录用户触摸屏幕是视频进度的变量
```
@property (assign, nonatomic) Direction direction;
  @property (assign, nonatomic) CGPoint startPoint;
  @property (assign, nonatomic) CGFloat startVB;
  @property (assign, nonatomic) CGFloat startVideoRate;
```
- 
刚开始触摸视频的代理
当用户首次触摸视频时，记录首次触摸的坐标、当前音量或者亮度、当前视频的进度，为了获取当前音量要首先定义MPVolumeView、 UISlider
```
@property (strong, nonatomic) MPVolumeView *volumeView;//控制音量的view
  @property (strong, nonatomic) UISlider* volumeViewSlider;//控制音量
```
```
```
//设置self.volumeView的frame
    self.volumeView.frame = CGRectMake(0, 0, self.view.frame.size.width, self.view.frame.size.width * 9.0 / 16.0);
```
```
在getter方法中初始化：
```
- (MPVolumeView *)volumeView {
        if (_volumeView == nil) {
            _volumeView  = [[MPVolumeView alloc] init];
            [_volumeView sizeToFit];
            for (UIView *view in [_volumeView subviews]){
                if ([view.class.description isEqualToString:@"MPVolumeSlider"]){
                    self.volumeViewSlider = (UISlider*)view;
                    break;
                }
            }
        }
        return _volumeView;
    }
```
然后在开始触摸的代理里记录
```
#pragma mark - 开始触摸
    - (void)touchesBeganWithPoint:(CGPoint)point {
        //记录首次触摸坐标
        self.startPoint = point;
        //检测用户是触摸屏幕的左边还是右边，以此判断用户是要调节音量还是亮度，左边是亮度，右边是音量
        if (self.startPoint.x <= self.button.frame.size.width / 2.0) {
            //亮度
            self.startVB = [UIScreen mainScreen].brightness;
        } else {
            //音量
            self.startVB = self.volumeViewSlider.value;
        }
    }
    CMTime ctime = self.avPlayer.currentTime;
    self.startVideoRate = ctime.value / ctime.timescale / self.total;
```
- 
接着在拖动的代理方法里改变音量、亮度和进度
```
#pragma mark - 拖动 
  - (void)touchesMoveWithPoint:(CGPoint)point {
      //得出手指在Button上移动的距离
      CGPoint panPoint = CGPointMake(point.x - self.startPoint.x, point.y - self.startPoint.y);
      //分析出用户滑动的方向
       if (self.direction == DirectionNone) {
          if (panPoint.x >= 30 || panPoint.x <= -30) {
              //进度
              self.direction = DirectionLeftOrRight;
          } else if (panPoint.y >= 30 || panPoint.y <= -30) {
              //音量和亮度
              self.direction = DirectionUpOrDown;
          }
      }
      if (self.direction == DirectionNone) {
          return;
      } else if (self.direction == DirectionUpOrDown) {
          //音量和亮度
          if (self.startPoint.x <= self.button.frame.size.width / 2.0) {
      //调节亮度
              if (panPoint.y < 0) {
                  //增加亮度
                  [[UIScreen mainScreen] setBrightness:self.startVB + (-panPoint.y / 30.0 / 10)];
              } else {
                  //减少亮度
                  [[UIScreen mainScreen] setBrightness:self.startVB - (panPoint.y / 30.0 / 10)];
              }
          } else {
              //音量
              if (panPoint.y < 0) {
                   //增大音量
                  [self.volumeViewSlider setValue:self.startVB + (-panPoint.y / 30.0 / 10) animated:YES];
                  if (self.startVB + (-panPoint.y / 30 / 10) - self.volumeViewSlider.value >= 0.1) {
                      [self.volumeViewSlider setValue:0.1 animated:NO];
                      [self.volumeViewSlider setValue:self.startVB + (-panPoint.y / 30.0 / 10) animated:YES];
                  }
              } else {
                  //减少音量
                  [self.volumeViewSlider setValue:self.startVB - (panPoint.y / 30.0 / 10) animated:YES];
              }
          }
      } else if (self.direction == DirectionLeftOrRight ) {
          //进度
          CGFloat rate = self.startVideoRate + (panPoint.x / 30.0 / 20.0);
          if (rate > 1) {
              rate = 1;
          } else if (rate < 0) {
              rate = 0;
          }
      }
}
```
`注意：1、前面提到一个增大音量的bug，我这里的解决办法是如果检测到用户设置的音量比系统当前的音量大于0.1，表示此时设置的音量已经无效了，然后把音量设置为0.1后再设置为我们设置无效的那个音量就可以了，具体做法参考代码；2、在修改视频播放进度的时候，最好不在移动的方法中实时修改视频播放进度，一方面会造成卡顿的现象，一方面没有必要这么做，所以这里只是记录了用户想要调整的进度，然后在触摸结束的方法中设置进度;3、这改变音量会调用系统自己的UI显示音量大小，但是在设置亮度的时候是系统没有提供相应的UI，需要我们自己设置，这个小伙伴们按照项目的需求自行添加一个UI效果就好了。`
- 触摸结束的代理
```
#pragma mark - 结束触摸
  - (void)touchesEndWithPoint:(CGPoint)point {
      if (self.direction == DirectionLeftOrRight) {
          [self.avPlayer seekToTime:CMTimeMakeWithSeconds(self.total * self.currentRate, 1) completionHandler:^(BOOL finished) {
              //在这里处理进度设置成功后的事情
          }];
      }
  }
```
真机运行效果如图
![](http://upload-images.jianshu.io/upload_images/1347742-9d2bfe55df88b346.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/300)
Demo.png
# 尾巴
到这里我们就完成了手指滑动视频控制音量、亮度和进度的需求，大家在自己的项目中实际应用时还需要根据项目需要添加适当的UI，处理其他一些逻辑和业务流程。希望大家通过这个小小的示例可以有自己的解决问题的思路和流程，力争将一个即使很简单的功能做到尽量完美。Demo可以在这里下载：[示例Demo](https://github.com/yueshangfeng/iOS-1-)
文／张云龙（简书作者）
原文链接：http://www.jianshu.com/p/5396866e3256
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
