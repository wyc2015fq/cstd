# UIGestureRecognizer  手势识别 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年03月14日 10:18:23[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：5259
# 一、概念介绍
- 
UIGestureRecognizer是一个抽象类，定义了所有手势的基本行为，使用它的子类才能处理具体的手势
> 
UITapGestureRecognizer（轻触，点按）
UILongPressGestureRecognizer（长按）
UISwipeGestureRecognizer（轻扫手势）
UIRotationGestureRecognizer（旋转手势）
UIPanGestureRecognizer（拖拽手势）
UIPinchGestureRecognizer（捏合手势，缩放用）
- 
使用步骤：
（1）创建手势识别对象
（2）设置手势识别属性，例如手指数量，方向等
（3）将手势识别附加到指定的视图之上
（4）编写手势触发监听方法
# 二、UIView 的分类
方法：
1、添加一个手势
``- (void)addGestureRecognizer:(UIGestureRecognizer*)gestureRecognizer;``
2、移除一个手势
``- (void)removeGestureRecognizer:(UIGestureRecognizer*)gestureRecognizer;``
3、是否接收一个手势触摸事件，默认为YES，返回NO为不接收
``- (BOOL)gestureRecognizerShouldBegin:(UIGestureRecognizer *)gestureRecognizer;``
# 三、UIGestureRecognizer 抽象类
#### 属性：
1、获取当前手势状态
`@property(nonatomic,readonly) UIGestureRecognizerState state;`
> 
UIGestureRecognizerState 枚举：
UIGestureRecognizerStatePossible 尚未识别是何种手势操作（但可能已经触发了触摸事件），默认状态
UIGestureRecognizerStateBegan 手势已经开始，此时已经被识别，但是这个过程中可能发生变化，手势操作尚未完成
UIGestureRecognizerStateChanged 手势状态发生改变
UIGestureRecognizerStateEnded 手势识别操作完成（此时已经松开手指）
UIGestureRecognizerStateCancelled 手势被取消，恢复到默认状态
UIGestureRecognizerStateFailed 手势识别失败，恢复到默认状态
UIGestureRecognizerStateRecognized = UIGestureRecognizerStateEnded 手势识别完成，同end
2、手势识别是否可用
`@property(nonatomic, getter=isEnabled) BOOL enabled;`
3、获取手势触摸的View视图
`@property(nullable, nonatomic,readonly) UIView *view;`
4、是否取消触摸控件的响应
默认为YES，这种情况下当手势识别器识别到触摸之后，会发送touchesCancelled给触摸到的控件以取消控件view对touch的响应，这个时候只有手势识别器响应touch，当设置成NO时，手势识别器识别到触摸之后不会发送touchesCancelled给控件，这个时候手势识别器和控件view均响应touch。
注意：手势识别和触摸事件是同时存在的，只是因为touchesCancelled导致触摸事件失效、
`@property(nonatomic) BOOL cancelsTouchesInView;`
5、是否延迟发送触摸事件给触摸到的控件
默认是NO，这种情况下当发生一个触摸时，手势识别器先捕捉到到触摸，然后发给触摸到的控件，两者各自做出响应。如果设置为YES，手势识别器在识别的过程中（注意是识别过程），不会将触摸发给触摸到的控件，即控件不会有任何触摸事件。只有在识别失败之后才会将触摸事件发给触摸到的控件，这种情况下控件view的响应会延迟约0.15ms。
`@property(nonatomic) BOOL delaysTouchesBegan;`
6、如果触摸识别失败是否立即结束本次手势识别的触摸事件（让触摸控件去识别触摸事件）
默认为YES，这种情况下发生一个触摸时，在手势识别成功后，发送给touchesCancelled消息给触摸控件view，手势识别失败时，会延迟大概0.15ms，期间没有接收到别的触摸才会发送touchesEnded触摸结束方法，如果设置为NO，则不会延迟，即会立即发送touchesEnded以结束当前触摸。
`@property(nonatomic) BOOL delaysTouchesEnded;`
7、 UIGestureRecognizerDelegate 代理
`@property(nullable,nonatomic,weak) id <UIGestureRecognizerDelegate> delegate;`
#### 方法：
1、创建一个手势对象并添加触发事件
``- (instancetype)initWithTarget:(nullableid)target action:(nullableSEL)action;``
2、给一个手势对象添加监听事件
``- (void)addTarget:(id)target action:(SEL)action;``
3、移除一个手势的监听事件
``- (void)removeTarget:(nullable id)target action:(nullable SEL)action;``
4、指定一个手势需要另一个手势执行失败才会执行，同时触发多个手势使用其中一个手势的解决办法
有时手势是相关联的，如单机和双击，点击和长按，点下去瞬间可能只会识别到单击无法识别其他，该方法可以指定某一个 手势，即便自己已经满足条件了，也不会立刻触发，会等到该指定的手势确定失败之后才触发
``- (void)requireGestureRecognizerToFail:(UIGestureRecognizer *)otherGestureRecognizer;``
> 
例子：
```
// 关键在这一行，如果双击确定偵測失败才會触发单击
[singleRecognizer requireGestureRecognizerToFail:doubleRecognizer];
```
5、获取当前触摸在指定视图上的点
``- (CGPoint)locationInView:(nullable UIView*)view;``
6、获取触摸手指数
`- (NSUInteger)numberOfTouches;`
7、多指触摸的触摸点相对于指定视图的位置
`- (CGPoint)locationOfTouch:(NSUInteger)touchIndex inView:(nullable UIView*)view;`
# 四、UIGestureRecognizerDelegate 代理方法
处理一些同时进行的手势操作：
1、 开始进行手势识别时调用的方法，返回NO则结束识别，不再触发手势，用处：可以在控件指定的位置使用手势识别
``- (BOOL)gestureRecognizerShouldBegin:(UIGestureRecognizer *)gestureRecognizer;``
2、手指触摸屏幕后回调的方法，返回NO则不再进行手势识别，方法触发等
此方法在window对象在有触摸事件发生时，调用gesture recognizer的touchesBegan:withEvent:方法之前调用，如果返回NO,则gesture recognizer不会看到此触摸事件。(默认情况下为YES)
`- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldReceiveTouch:(UITouch *)touch;`
> 
例子：UITapGestureRecognizer和UIButton的点击事件冲突的解决办法
```
- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldReceiveTouch:(UITouch *)touch{    
if ([touch.view isKindOfClass:[UIButton class]])    { 
       return NO;    }   
       return YES;
}
```
3、手指按压屏幕后回调的方法，返回NO则不再进行手势识别，方法触发等
`- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldReceivePress:(UIPress *)press;`
4、 是否支持多手势触发，返回YES，则可以多个手势一起触发方法，返回NO则为互斥
是否允许多个手势识别器共同识别，一个控件的手势识别后是否阻断手势识别继续向下传播，默认返回NO；如果为YES，响应者链上层对象触发手势识别后，如果下层对象也添加了手势并成功识别也会继续执行，否则上层对象识别后则不再继续传播
`- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldRecognizeSimultaneouslyWithGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer;`
5、下面这个两个方法也是用来控制手势的互斥执行的
（1） 这个方法返回YES，第一个手势和第二个互斥时，第一个会失效
`- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldRequireFailureOfGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer;`
（2） 这个方法返回YES，第一个和第二个互斥时，第二个会失效
`- (BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldBeRequiredToFailByGestureRecognizer:(UIGestureRecognizer *)otherGestureRecognizer;`
# 五、UITapGestureRecognizer（轻触，点按）
1、 设置能识别到手势的最少的轻触次数（默认为1）
`@property (nonatomic) NSUInteger  numberOfTapsRequired;`
2、 设置能识别到手势的最少的手指的个数（默认为1）
`@property (nonatomic) NSUInteger  numberOfTouchesRequired;`
> 
例子：
```
// 创建一个手势对象
UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(tapAction:)];
// 设置能识别到手势的最少的轻触次数
tap.numberOfTapsRequired = 3;
// 设置能识别到手势的最少的手指的个数
tap.numberOfTouchesRequired = 2;
//把手势对象添加到对应的控件中
[self.imgView addGestureRecognizer:tap];
```
# 六、UILongPressGestureRecognizer（长按手势）
1、最少触摸次数，默认为0
`@property (nonatomic) NSUInteger numberOfTapsRequired;`
2、最少触摸手指个数，默认为1
`@property (nonatomic) NSUInteger numberOfTouchesRequired;、`
3、 设置能识别到长按手势的最短的长按时间，单位：秒，默认为0.5
`@property (nonatomic) CFTimeInterval minimumPressDuration;`
4、设置长按时允许移动的最大距离，单位：像素，默认为10像素
`@property (nonatomic) CGFloat allowableMovement;`
> 
例子：
```
UILongPressGestureRecognizer *longPress = [[UILongPressGestureRecognizer alloc] initWithTarget:self action:@selector(longPressAction:)];
// 设置能识别到长按手势的最小的长按时间
longPress.minimumPressDuration = 0.5;
// "容错的范围"
longPress.allowableMovement  = 10;
// 把长按手势添加到对应的控件中
[self.imgView addGestureRecognizer:longPress];
```
# 七、UISwipeGestureRecognizer（轻扫手势）
1、最少触摸手指个数，默认为1
`@property(nonatomic) NSUInteger numberOfTouchesRequired;`
2、设置轻扫手势支持的方向，默认为向右滑
`@property(nonatomic) UISwipeGestureRecognizerDirection direction;`
> 
枚举值：
UISwipeGestureRecognizerDirectionRight 向右滑
UISwipeGestureRecognizerDirectionLeft 向左滑
UISwipeGestureRecognizerDirectionUp 向上滑
UISwipeGestureRecognizerDirectionDown 向下滑
例子：
```
UISwipeGestureRecognizer *swipeLeft = [[UISwipeGestureRecognizer alloc] initWithTarget:self action:@selector(swipeAction:)];
swipeLeft.direction = UISwipeGestureRecognizerDirectionLeft;
[self.imgView addGestureRecognizer:swipeLeft];
```
# 八、UIRotationGestureRecognizer（旋转手势）
1、旋转的角度
`@property (nonatomic) CGFloat rotation;`
2、旋转速度，单位：度/秒、
`@property (nonatomic,readonly) CGFloat velocity;`
> 
例子：
```
//为图片框添加一个旋转手势
UIRotationGestureRecognizer *rotation = [[UIRotationGestureRecognizer alloc] initWithTarget:self action:@selector(rotateAction:)];rotation.delegate = self;
[self.imgView addGestureRecognizer:rotation];
// 旋转手势的监听方法
- (void)rotateAction:(UIRotationGestureRecognizer *)recognizer {
// 在原来的基础上, 累加多少度
recognizer.view.transform = CGAffineTransformRotate(recognizer.view.transform, recognizer.rotation);
// 每次旋转完毕后将rotation的值, 恢复到0的位置.recognizer.rotation = 0;
}
```
# 九、UIPanGestureRecognizer（拖拽手势）
1、设置触发拖拽最少手指数，默认为1
`@property (nonatomic) NSUInteger minimumNumberOfTouches;`
2、设置触发拖拽最多手指数，默认为 UINT_MAX 无限大
`@property (nonatomic) NSUInteger maximumNumberOfTouches;`
3、获取当前拖拽位置
``- (CGPoint)translationInView:(nullable UIView *)view;``
4、设置当前拖拽位置
`- (void)setTranslation:(CGPoint)translation inView:(nullable UIView *)view;`
5、设置拖拽速度，单位：像素/秒
``- (CGPoint)velocityInView:(nullable UIView *)view;``
> 
例子：
```
UIPanGestureRecognizer *pan = [[UIPanGestureRecognizer alloc] initWithTarget:self action:@selector(panAction:)];
[self.imgView addGestureRecognizer:pan];
// 拖拽手势的监听方法
- (void)panAction:(UIPanGestureRecognizer *)recognizer {
// 1. 获取手指拖拽的时候, 平移的值
CGPoint translation = [recognizer translationInView:recognizer.view];
// 2. 让当前控件做响应的平移
recognizer.view.transform = CGAffineTransformTranslate(recognizer.view.transform, translation.x, translation.y);
// 3. 每次平移手势识别完毕后, 让平移的值不要累加
[recognizer setTranslation:CGPointZero inView:recognizer.view];
}
```
# 十、UIPinchGestureRecognizer（捏合手势，缩放用）
1、设置缩放比例
`@property (nonatomic) CGFloat scale;`
2、获取捏合速度，单位：缩放比/秒
`@property (nonatomic,readonly) CGFloat velocity;`
> 
例子：
```
UIPinchGestureRecognizer *pinch = [[UIPinchGestureRecognizer alloc] initWithTarget:self action:@selector(pinchAction:)];pinch.delegate = self;
[self.imgView addGestureRecognizer:pinch];
// 捏合手势监听方法
- (void)pinchAction:(UIPinchGestureRecognizer *)recognizer {
recognizer.view.transform = CGAffineTransformScale(recognizer.view.transform, recognizer.scale, recognizer.scale);
recognizer.scale = 1.0;
}
```
