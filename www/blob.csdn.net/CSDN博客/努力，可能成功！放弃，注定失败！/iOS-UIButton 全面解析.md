# iOS-UIButton 全面解析 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年06月20日 10:21:32[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1578
# UIButton 的全面解析
`建议收藏，用到的时候来这里一查就都明白了`
```
//初始化Button 不用alloca init 的方法 用便利构造器初始化
UIButton *button = [UIButton buttonWithType:UIButtonTypeSystem];
typedef NS_ENUM(NSInteger, UIButtonType) {
    UIButtonTypeCustom = 0,       -自定义风格
    UIButtonTypeSystem NS_ENUM_AVAILABLE_IOS(7_0),
    UIButtonTypeDetailDisclosure, -蓝色小箭头按钮，主要做详细说明
    UIButtonTypeInfoLight,        -亮色感叹号
    UIButtonTypeInfoDark,         -暗色感叹号
    UIButtonTypeContactAdd,       -十字加号按钮
    UIButtonTypeRoundedRect = UIButtonTypeSystem,  -圆角矩形
};
//设置button frmae 
button.frame = CGRectMake(100, 100, 100, 100);
//设置button 背景颜色
button.backgroundColor = [UIColor orangeColor];
//添加button的标题
[button setTitle:@"登陆" forState:UIControlStateNormal];
//设置标题的颜色
[button setTitleColor:[UIColor redColor] forState:UIControlStateNormal];
//设置标题字体的大小
button.titleLabel.font = [UIFont systemFontOfSize:20];
//设置button的背景图片
[button setBackgroundImage:[UIImage imageNamed:@"11.png"] forState:UIControlStateNormal];
//获取指定状态下的背景图片
UIImage *tempImage = [button imageForState:UIControlStateNormal];
//设置前景图片  前景图片必须是镂空图，或者是线条勾勒的图片
[button setImage:[UIImage imageNamed:@"7.png"] forState:UIControlStateNormal];
//设置阴影颜色
[button setTitleShadowColor:[UIColor purpleColor] forState:UIControlStateNormal];
//button 一些其他属性
@property(nonatomic)  UIEdgeInsets contentEdgeInsets; 内容内距离
@property(nonatomic)  UIEdgeInsets titleEdgeInsets;   标题内距离
@property(nonatomic)  BOOL  reversesTitleShadowWhenHighlighted; 标题的阴影改变时，按钮是否高亮显示。默认为NO
@property(nonatomic)  UIEdgeInsets imageEdgeInsets;   图片内边距
@property(nonatomic)  BOOL adjustsImageWhenHighlighted;按钮高亮的情况下，图像的颜色是否要加深一点。默认是YES
@property(nonatomic)  BOOL adjustsImageWhenDisabled; 按钮禁用的情况下，图像的颜色是否要加深一点。默认是YES
@property(nonatomic)  BOOL showsTouchWhenHighlighted; 按下按钮是否会发光 默认是NO
@property(nonatomic,readonly) UIButtonType buttonType; button的类型
设置button某个状态的标题
- (void)setTitle:(nullable NSString *)title forState:(UIControlState)state;
例： [button setTitle:@"开灯" forState:UIControlStateNormal];
设置button某个状态的标题颜色
- (void)setTitleColor:(nullable UIColor *)color forState:(UIControlState)state
例：[button setTitleColor:[UIColor blueColor] forState:UIControlStateNormal];
设置button某个状态阴影的标题颜色
- (void)setTitleShadowColor:(nullable UIColor *)color forState:(UIControlState)state
例：[button setTitleShadowColor:[UIColor purpleColor] forState:UIControlStateNormal];
设置button某个状态图片
- (void)setImage:(nullable UIImage *)image forState:(UIControlState)state;
设置button 某个状态背景图片
- (void)setBackgroundImage:(nullable UIImage *)image forState:(UIControlState)state UI_APPEARANCE_SELECTOR; // default is nil
设置button 某个状态下的富文本标题
- (void)setAttributedTitle:(nullable NSAttributedString *)title forState:(UIControlState)state NS_AVAILABLE_IOS(6_0); // default is nil. title is assumed to be single line
返回button 某个状态下的标题
- (nullable NSString *)titleForState:(UIControlState)state;
返回button 某个状态下的标题颜色
- (nullable UIColor *)titleColorForState:(UIControlState)state;
返回button 某个状态下的阴影标题颜色
- (nullable UIColor *)titleShadowColorForState:(UIControlState)state;
返回button 某个状态下的图片
- (nullable UIImage *)imageForState:(UIControlState)state;
返回button 某个状态下的背景图片
- (nullable UIImage *)backgroundImageForState:(UIControlState)state;
返回button 某个状态下的富文本标题
- (nullable NSAttributedString *)attributedTitleForState:(UIControlState)state NS_AVAILABLE_IOS(6_0);
/* forState: 这个参数的作用是定义按钮的文字或图片在何种状态下才会显现*/
以下是几种状态
 enum {
UIControlStateNormal = 0, 常规状态显现
UIControlStateHighlighted = 1 << 0, 高亮状态显现
UIControlStateDisabled = 1 << 1, 禁用的状态才会显现
UIControlStateSelected = 1 << 2, 选中状态
UIControlStateApplication = 0x00FF0000, 当应用程序标志时
UIControlStateReserved = 0xFF000000 为内部框架预留，可以不管他
 };
获取按钮当前标题
@property(nullable, nonatomic,readonly,strong) NSString *currentTitle;
获取按钮当前标题颜色
@property(nonatomic,readonly,strong) UIColor  *currentTitleColor;
获取按钮当前阴影标题颜色
@property(nullable, nonatomic,readonly,strong) UIColor *currentTitleShadowColor;
获取按钮当前按钮内图像
@property(nullable, nonatomic,readonly,strong) UIImage  *currentImage;
获取按钮当前标题背景图片
@property(nullable, nonatomic,readonly,strong) UIImage  *currentBackgroundImage;
获取按钮当前标题富文本
@property(nullable, nonatomic,readonly,strong) NSAttributedString *currentAttributedTitle
@property(nullable, nonatomic,readonly,strong) UILabel    *titleLabel NS_AVAILABLE_IOS(3_0);
@property(nullable, nonatomic,readonly,strong) UIImageView *imageView  NS_AVAILABLE_IOS(3_0);
指定背景边界
- (CGRect)backgroundRectForBounds:(CGRect)bounds;
指定内容边界
- (CGRect)contentRectForBounds:(CGRect)bounds;
指定标题边界
- (CGRect)titleRectForContentRect:(CGRect)contentRect;
指定图片边界
- (CGRect)imageRectForContentRect:(CGRect)contentRect;
示例：
- (CGRect)imageRectForContentRect:(CGRect)bounds{
        return CGRectMake(0.0, 0.0, 44, 44);
    }
@end
//给按钮添加点击事件
[button addTarget:self action:@selector(action:) forControlEvents:UIControlEventTouchUpInside];
枚举值：
    UIControlEventTouchDown             // 单点触摸按下事件：用户点触屏幕，或者又有新手指落下的时候。
    UIControlEventTouchDownRepeat       // 多点触摸按下事件，点触计数大于1：用户按下第二、三、或第四根手指的时候。
    UIControlEventTouchDragInside       // 当一次触摸在控件窗口内拖动时。
    UIControlEventTouchDragOutside      // 当一次触摸在控件窗口之外拖动时。
    UIControlEventTouchDragEnter        // 当一次触摸从控件窗口之外拖动到内部时
    UIControlEventTouchDragExit         // 当一次触摸从控件窗口内部拖动到外部时。
    UIControlEventTouchUpInside         // 所有在控件之内触摸抬起事件
    UIControlEventTouchUpOutside        // 所有在控件之外触摸抬起事件(点触必须开始与控件内部才会发送通知)。
    UIControlEventTouchCancel           // 所有触摸取消事件，即一次触摸因为放上了太多手指而被取消，或者被上锁或者电话呼叫打断。
    UIControlEventValueChanged          // 当控件的值发生改变时，发送通知。用于滑块、分段控件、以及其他取值的控件。你可以配置滑块控件何时发送通知，在滑块被放下时发送，或者在被拖动时发送。
    UIControlEventEditingDidBegin       // 当文本控件中开始编辑时发送通知
    UIControlEventEditingChanged        // 当文本控件中的文本被改变时发送通知。
    UIControlEventEditingDidEnd         // 当文本控件中编辑结束时发送通知。
    UIControlEventEditingDidEndOnExit   // 当文本控件内通过按下回车键（或等价行为）结束编辑时，发送通知。
    UIControlEventAllTouchEvents        // 通知所有触摸事件。
    UIControlEventAllEditingEvents      // 通知所有关于文本编辑的事件。
    UIControlEventApplicationReserved   // range available for application use
    UIControlEventSystemReserved        // range reserved for internal framework use
    UIControlEventAllEvents             // 通知所有事件
```
`如果有错误的地方请大家多多指正，欢迎评论交流，共同学习。`
文／xx_cc（简书作者）
原文链接：http://www.jianshu.com/p/cc9655d82045
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
