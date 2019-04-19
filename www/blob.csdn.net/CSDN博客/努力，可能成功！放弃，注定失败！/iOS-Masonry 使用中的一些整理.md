# iOS - Masonry 使用中的一些整理 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年07月19日 20:22:06[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：22559
个人喜欢用纯代码写东西,其中用到最多的就是Masonry,我整理一些使用过程中一些点，方便以后使用.(基本的语法就不说了)
首先说几点:
- 我一般将数值类型的约束用`mas_equalTo`，而相对于某个控件，或者某个控件的某个约束，我会使用`equalTo`，如：
`make.size.mas_equalTo(CGSizeMake(100, 100));`
`make.center.equalTo(weakSelf.view);`
- `setNeedsLayout`：告知页面需要更新，但是不会立刻开始更新。执行后会立刻调用layoutSubviews。
`layoutIfNeeded`：告知页面布局立刻更新。所以一般都会和`setNeedsLayout`一起使用。如果希望立刻生成新的frame需要调用此方法，利用这点一般布局动画可以在更新布局后直接使用这个方法让动画生效。
`layoutSubviews`：系统重写布局
`setNeedsUpdateConstraints`：告知需要更新约束，但是不会立刻开始
`updateConstraintsIfNeeded`：告知立刻更新约束
`updateConstraints`：系统更新约束
- `- (void)updateViewConstraints` ViewController的View在更新视图布局时，会先调用ViewController的updateViewConstraints 方法。我们可以通过重写这个方法去更新当前View的内部布局，而不用再继承这个View去重写-updateConstraints方法。我们在重写这个方法时，务必要调用 super 或者 调用当前View的 -updateConstraints 方法。
### 1. 视图居中显示
```
// 防止block中的循环引用
__weak typeof(self) weakSelf = self;
UIView* view         = [UIView new];
view.backgroundColor = [UIColor brownColor];
[self.view addSubview:view];
//使用mas_makeConstraints添加约束
[view mas_makeConstraints:^(MASConstraintMaker *make) {
    // 添加大小约束（make就是要添加约束的控件view）
    make.size.mas_equalTo(CGSizeMake(200, 200));
    // 添加居中约束（居中方式与self相同）
    make.center.equalTo(weakSelf.view);
}];
```
### 2. 两个视图等宽高边距
```
UIView* blackView       = [UIView new];
blackView.backgroundColor = [UIColor blackColor];
[self.view addSubview:blackView];
[blackView mas_makeConstraints:^(MASConstraintMaker *make) {
    //添加约束大小
    make.size.mas_equalTo(CGSizeMake(100, 100));
    //在 左,上 添加约束 (左、上约束都是20）
    make.left.and.top.mas_equalTo(20);
}];
UIView* grayView         = [UIView new];
grayView.backgroundColor = [UIColor lightGrayColor];
[self.view addSubview:grayView];
[grayView mas_makeConstraints:^(MASConstraintMaker *make) {
    // 大小、上边距约束与黑色view相同
    make.size.and.top.equalTo(blackView);
    // 添加右边距约束（这里的间距是有方向性的，左、上边距约束为正数，右、下边距约束为负数）
    make.right.mas_equalTo(-20);
}];
```
### 3. 键盘弹出和收回
```
- (void)dealloc {
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}
- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    __weak typeof(self) weakSelf = self;
    _textField                 = [UITextField new];
    _textField.backgroundColor = [UIColor redColor];
    [self.view addSubview:_textField];
    [_textField mas_makeConstraints:^(MASConstraintMaker *make) {
    //left,right,centerx,y  不能共存只能有其二
    make.left.mas_equalTo(20);
    //        make.right.mas_equalTo(-60);
    make.centerX.equalTo(weakSelf.view);
    make.height.mas_equalTo(40);
    make.bottom.mas_equalTo(0);
    }];
    // 注册键盘通知
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardWillChangeFrameNotification:) name:UIKeyboardWillChangeFrameNotification object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self selector:@selector(keyboardWillHideNotification:) name:UIKeyboardWillHideNotification object:nil];
}
- (void)keyboardWillChangeFrameNotification:(NSNotification *)notification {
    // 获取键盘基本信息（动画时长与键盘高度）
    NSDictionary *userInfo = [notification userInfo];
    CGRect rect = [userInfo[UIKeyboardFrameBeginUserInfoKey] CGRectValue];
    CGFloat keyboardHeight   = CGRectGetHeight(rect);
    CGFloat keyboardDuration = [userInfo[UIKeyboardAnimationDurationUserInfoKey] doubleValue];
    // 修改下边距约束
    [_textField mas_updateConstraints:^(MASConstraintMaker *make) {
        make.bottom.mas_equalTo(-keyboardHeight);
    }];
    // 更新约束
    [UIView animateWithDuration:keyboardDuration animations:^{
    [self.view layoutIfNeeded];
    }];
}
- (void)keyboardWillHideNotification:(NSNotification *)notification {
    // 获得键盘动画时长
    NSDictionary *userInfo   = [notification userInfo];
    CGFloat keyboardDuration = [userInfo[UIKeyboardAnimationDurationUserInfoKey] doubleValue];
    // 修改为以前的约束（距下边距0）
    [_textField mas_updateConstraints:^(MASConstraintMaker *make) {
        make.bottom.mas_equalTo(0);
    }];
    // 更新约束
    [UIView animateWithDuration:keyboardDuration animations:^{
        [self.view layoutIfNeeded];
    }];
}
- (void)touchesBegan:(NSSet *)touches withEvent:(UIEvent *)event {
    [super touchesBegan:touches withEvent:event];
    [self.view endEditing:YES];
}
```
### 4. 三控件等宽间距
##### 方法一:
`array 的 mas_distributeViewsAlongAxis withFixedSpacing` 变化的是控件 长度或宽度
定义一个存放三个控件的数组`NSArray *array;`
`array = @[greenView,redView,blueView];`
直接调用下面的方法:
```cpp
```cpp
- (void)getHorizontalone
{
//方法一,array 的 mas_distributeViewsAlongAxis
/**
 *  多个控件固定间隔的等间隔排列，变化的是控件的长度或者宽度值
 *
 *  @param axisType        轴线方向
 *  @param fixedSpacing    间隔大小
 *  @param leadSpacing     头部间隔
 *  @param tailSpacing     尾部间隔
 */
//    MASAxisTypeHorizontal  水平
//    MASAxisTypeVertical    垂直
[arrayList mas_distributeViewsAlongAxis:MASAxisTypeHorizontal
                       withFixedSpacing:20
                            leadSpacing:5
                            tailSpacing:5];
[arrayList mas_makeConstraints:^(MASConstraintMaker *make) {
    make.top.mas_equalTo(60);
    make.height.mas_equalTo(100);
}];
```
```
}
##### 方法二:
`array de mas_distributeViewsAlongAxis withFixedItemLength` 控件size不变,变化的是间隙
```cpp
```cpp
- (void)getVertical
{
/**
 *  多个固定大小的控件的等间隔排列,变化的是间隔的空隙
 *
 *  @param axisType        轴线方向
 *  @param fixedItemLength 每个控件的固定长度或者宽度值
 *  @param leadSpacing     头部间隔
 *  @param tailSpacing     尾部间隔
 */
[arrayList mas_distributeViewsAlongAxis:MASAxisTypeVertical
                    withFixedItemLength:60
                            leadSpacing:40
                            tailSpacing:10];
[arrayList mas_makeConstraints:^(MASConstraintMaker *make) {
    //        make.top.mas_equalTo(100);
    //        make.height.mas_equalTo(100);
    make.left.mas_equalTo(20);
    make.right.mas_equalTo(-20);
}];
```
```
}
###### 以上俩方法都在`NSArray+MASAdditions`中
##### 方法三:直接设置`multiplier`实现等间距
```
for (NSUInteger i = 0; i < 4; i++) {
    UIView *itemView = [self getItemViewWithIndex:i];
    [_containerView addSubview:itemView];
    [itemView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.width.and.height.equalTo(@(ITEM_SIZE));
        make.centerY.equalTo(_containerView.mas_centerY);
        make.centerX.equalTo(_containerView.mas_right).multipliedBy(((CGFloat)i + 1) / ((CGFloat)ITEM_COUNT + 1));
    }];
}
```
##### 方法四: 利用透明等宽度的SpaceView实现等间距
```
UIView *lastSpaceView       = [UIView new];
lastSpaceView.backgroundColor = [UIColor greenColor];
[_containerView1 addSubview:lastSpaceView];
[lastSpaceView mas_makeConstraints:^(MASConstraintMaker *make) {
    make.left.and.top.and.bottom.equalTo(_containerView1);
}];
for (NSUInteger i = 0; i < ITEM_COUNT; i++) {
    UIView *itemView = [self getItemViewWithIndex:i];
    [_containerView1 addSubview:itemView];
    [itemView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.height.and.width.equalTo(@(ITEM_SIZE));
        make.left.equalTo(lastSpaceView.mas_right);
        make.centerY.equalTo(_containerView1.mas_centerY);
    }];
    UIView *spaceView         = [UIView new];
    spaceView.backgroundColor = [UIColor greenColor];
    [_containerView1 addSubview:spaceView];
    [spaceView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.equalTo(itemView.mas_right).with.priorityHigh(); // 降低优先级，防止宽度不够出现约束冲突
        make.top.and.bottom.equalTo(_containerView1);
        make.width.equalTo(lastSpaceView.mas_width);
    }];
    lastSpaceView = spaceView;
}
[lastSpaceView mas_makeConstraints:^(MASConstraintMaker *make) {
    make.right.equalTo(_containerView1.mas_right);
}];
```
##### 5. 动态改变字体宽度
和面方法4一样,利用spaceView来实现
```
UIView* bgView       = [[UIView alloc]init];
bgView.backgroundColor = [UIColor yellowColor];
[self.view addSubview:bgView];
[bgView mas_makeConstraints:^(MASConstraintMaker *make) {
    make.left.and.right.mas_equalTo(0);
    make.top.mas_equalTo(@100);
    make.height.mas_equalTo(@100);
}];
listText = @[@"北京",@"地大吴波啊",@"你大爷",@"我们的爱哎哎"];
UIView *lastSpaceView = nil;
for(int i = 0 ; i < listText.count;  i ++)
{
    UILabel* label = [UILabel new];
    label.text     = listText[i];
    label.backgroundColor = RANDOMCOLOR;
    [bgView addSubview:label];
    UIView* lineView         = [UIView new];
    lineView.backgroundColor = [UIColor redColor];
    [bgView addSubview:lineView];
    [label mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.bottom.mas_equalTo(0);
        if (lastSpaceView)
        {
            NSLog(@"存在 lastView");
            make.left.equalTo(lastSpaceView.mas_right).mas_offset(@20);
        }else
        {
            NSLog(@"不存在存在 lastView");
            make.left.equalTo(bgView.mas_left);
        }
        make.height.equalTo(bgView);
    }];
    lastSpaceView = label;
    [lineView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.and.bottom.mas_equalTo(0);
        make.width.mas_equalTo(1);
        make.left.mas_equalTo(label.mas_right).mas_offset(@10);
    }];
}
```
##### 效果图:
![](http://upload-images.jianshu.io/upload_images/1503437-23cdea3fdd363703.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
##### 6. 父视图的高度,是里面俩控件高度的和
```
UIView* bgView       = [UIView new];
bgView.backgroundColor = [UIColor purpleColor];
[self.view addSubview:bgView];
UILabel* titleLab        = [UILabel new];
titleLab.backgroundColor = [UIColor redColor];
titleLab.textAlignment   = NSTextAlignmentCenter;
titleLab.font            = [UIFont systemFontOfSize:15.f];
titleLab.text            = @"曹操——《短歌行》";
[bgView addSubview:titleLab];
UILabel* contentLab        = [UILabel new];
contentLab.numberOfLines   = 0 ;
contentLab.textAlignment   = NSTextAlignmentCenter;
contentLab.backgroundColor = [UIColor brownColor];
contentLab.font            = [UIFont systemFontOfSize:13.f];
contentLab.text            = @" 对酒当歌，人生几何？ 譬如朝露，去日苦多。\n 慨当以慷，忧思难忘。 何以解忧？唯有杜康。\n 青青子衿，悠悠我心。 但为君故，沉吟至今。\n 呦呦鹿鸣，食野之苹。 我有嘉宾，鼓瑟吹笙。\n 明明如月，何时可掇？ 忧从中来，不可断绝。\n 越陌度阡，枉用相存。 契阔谈宴，心念旧恩。\n 月明星稀，乌鹊南飞。 绕树三匝，何枝可依？\n 山不厌高，海不厌深。 周公吐哺，天下归心。";
[bgView addSubview:contentLab];
//思路: 父视图的上间距等于title的上间距,父视图的下间距等于content的下间距
__weak typeof(self) weakSelf = self;
[bgView mas_makeConstraints:^(MASConstraintMaker *make) {
    make.left.mas_offset(@30);
    make.right.mas_offset(@-30);
    make.centerY.equalTo(weakSelf.view);
}];
[titleLab mas_makeConstraints:^(MASConstraintMaker *make) {
    make.left.top.right.mas_equalTo(@0);
}];
[contentLab mas_makeConstraints:^(MASConstraintMaker *make) {
    make.left.right.mas_equalTo(@0);
    make.top.equalTo(titleLab.mas_bottom).mas_offset(@10);
    make.bottom.equalTo(bgView);
}];
```
#### 效果图:
![](http://upload-images.jianshu.io/upload_images/1503437-90ff289f1bd5be50.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
###### 以后慢慢更新,记录方便以后使用
文/栋飞
//一些扒的别人的记录
#### 自适应布局允许将宽度或高度设置为固定值.如果你想要给视图一个最小或最大值,你可以这样:
`//width >= 200 && width <= 400make.width.greaterThanOrEqualTo(@200);make.width.lessThanOrEqualTo(@400)`
#### 约束的优先级
`.priority`允许你指定一个精确的优先级,数值越大优先级越高.最高1000.
`.priorityHigh`等价于 `UILayoutPriorityDefaultHigh` .优先级值为 750.
`.priorityMedium`介于高优先级和低优先级之间,优先级值在 250~750之间.
`.priorityLow`等价于 `UILayoutPriorityDefaultLow` , 优先级值为 250.
#### 优先级可以在约束的尾部添加:
`make.left.greaterThanOrEqualTo(label.mas_left).with.priorityLow();`
`make.top.equalTo(label.mas_top).with.priority(600);`
#### center 中心
//使 centerX和 centerY = button1
`make.center.equalTo(button1)`
//使 centerX = superview.centerX - 5, centerY = `superview.centerY + 10make.center.equalTo(superview).centerOffset(CGPointMake(-5, 10))`
#### 指定宽度为父视图的 1/4.
`make.width.equalTo(superview).multipliedBy(0.25);`
文／魏同学（简书作者）
原文链接：http://www.jianshu.com/p/a24dd8638d28
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
