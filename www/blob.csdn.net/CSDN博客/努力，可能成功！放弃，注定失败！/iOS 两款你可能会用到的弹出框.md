# iOS 两款你可能会用到的弹出框 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年06月19日 16:20:05[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：675
###### 前言
好久没写博客了……最近拿到了一版原型图，各种弹框，简直快把老爷给弹死了……因为实现功能是其次的，最主要还得把这些东西给封装一下，方便同事的调用。于是乎，我就开始了爬坑的过程。经过两天的耕耘，出了两款风格迥异的弹框，这里给大家分享一下。。。同时也祭奠一下，我老去的容颜……
###### 效果图
***底部PickerView弹框***（这个东西还是蛮常见的）
![](https://upload-images.jianshu.io/upload_images/1678515-28dee0ff81c2a75a.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/315)
view.gif
***中间TextView弹框***（这个东西真不常见，Alert支持的是单行输入，也就是textField）
![](https://upload-images.jianshu.io/upload_images/1678515-d38bb0d6afd0846b.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/305)
vie.gif
###### 底部PickerView弹框（LHEditPickerView）
首先生成了一个LHEditPickView这个就是大家看到的那个弹出框的实体view
代码如下：
LHEditPickView.h
```
#import <UIKit/UIKit.h>
@interface LHEditPickView : UIView
/**
 *  背景图（半透明全屏）
 */
@property (nonatomic,weak)UIView *blackBgView;
/**
 *  下部弹出框的ToolBar
 */
@property (nonatomic,weak)UIToolbar *toolBarView;
/**
 *  弹出框主题背景
 */
@property (nonatomic,weak)UIView *mainBgView;
/**
 *  部门选择PickView
 */
@property (nonatomic,weak)UIPickerView *pickerView;
/**
 *  取消按钮
 */
@property (nonatomic,weak)UIButton *cancelBtn;
/**
 *  确定按钮
 */
@property (nonatomic,weak)UIButton *sureBtn;
@property (nonatomic,strong)NSArray *data;
@property (nonatomic,copy) void (^refreshUserInterface)(NSString *);
@property (nonatomic,copy) void (^dropEditPickerView)();
@end'
```
LHEditPickView.m
```
#import "LHEditPickView.h"
@interface LHEditPickView()<UIPickerViewDelegate,UIPickerViewDataSource>{
//按钮的宽度
CGFloat _btnWidth;
NSString *_result;
}
@end
@implementation LHEditPickView
-(instancetype)initWithFrame:(CGRect)frame{
self=[super initWithFrame:frame];
if(self){
    //主背景图
    UIView *mainBgView=[[UIView alloc]initWithFrame:CGRectMake(0, 0, frame.size.width, frame.size.height)];
    self.mainBgView=mainBgView;
    mainBgView.backgroundColor=[UIColor whiteColor];
    [self addSubview:mainBgView];
    
    //ToolBar
    UIToolbar *toolView=[[UIToolbar alloc]initWithFrame:CGRectMake(0, 0, frame.size.width, frame.size.height/6)];
    [toolView setBackgroundImage:[UIImage imageNamed:@"daohangtiao"] forToolbarPosition:UIBarPositionAny barMetrics:UIBarMetricsDefault];
    self.toolBarView=toolView;
    toolView.backgroundColor=[UIColor blueColor];
    [mainBgView addSubview:toolView];
    
    //取消，确定按钮
    _btnWidth=100.0;
    UIButton *cancelbtn=[UIButton buttonWithType:UIButtonTypeCustom];
    cancelbtn.frame=CGRectMake(0, 0, _btnWidth, CGRectGetHeight(toolView.frame));
    [cancelbtn setTitle:@"取消" forState:UIControlStateNormal];
    [cancelbtn addTarget:self action:@selector(onclickCancel:) forControlEvents:UIControlEventTouchUpInside];
    self.cancelBtn=cancelbtn;
    [toolView addSubview:cancelbtn];
    
    _btnWidth=100.0;
    UIButton *sureBtn=[UIButton buttonWithType:UIButtonTypeCustom];
    sureBtn.frame=CGRectMake(frame.size.width-_btnWidth, 0, _btnWidth, CGRectGetHeight(toolView.frame));
    [sureBtn setTitle:@"确定" forState:UIControlStateNormal];
    self.sureBtn=sureBtn;
    [self.sureBtn addTarget:self action:@selector(onclickSure:) forControlEvents:UIControlEventTouchUpInside];
    [toolView addSubview:sureBtn];
    
    
    
    //UIPickerView
    UIPickerView *picker=[[UIPickerView alloc]initWithFrame:CGRectMake(0, CGRectGetMaxY(toolView.frame), frame.size.width, (frame.size.height/6)*5)];
    self.pickerView=picker;
    picker.showsSelectionIndicator=YES;
    picker.delegate=self;
    picker.dataSource=self;
    [mainBgView addSubview:picker];
    
}
return self;
}
//设置data并且设设置_result的初始值
-(void)setData:(NSArray *)data{
if(_data!=data){
    _data=data;
    _result=data[0];
}
//刷新所有元素
[self.pickerView reloadAllComponents];
}
#pragma mark -ButtonClick
-(void)onclickCancel:(id)sender{
if(self.dropEditPickerView){
    self.dropEditPickerView();
}
}
//确定按钮,block传值
-(void)onclickSure:(id)sender{
if(self.refreshUserInterface){
    self.refreshUserInterface(_result);
}
if(self.dropEditPickerView){
    self.dropEditPickerView();
}
}
#pragma mark -PickerViewDelegate
//有多少行
-(NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component{
return self.data.count;
}
//有多少列
-(NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView{
return 1;
}
//设置每一行的内容
-(NSString *)pickerView:(UIPickerView *)pickerView titleForRow:(NSInteger)row forComponent:(NSInteger)component{
return self.data[row];
}
//设置选中结果
-(void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component{
_result=self.data[row];
}
@end
```
由于本人习惯于代码布局，所以很low……呵呵哒
现在主体显示的部分已经有了，现在就该操心怎么将它显示出来了，于是我又创建了一个类——LHBottomPickerView（显示LHEditPickView)废话不多说，上代码：
LHBottomPickerView.h
```
#import <UIKit/UIKit.h>
#import "LHEditPickView.h"
@interface LHBottomPickerView : UIView
@property (nonatomic,weak)UIView *grayBgView;
@property (nonatomic,weak)LHEditPickView *editView;
@property (nonatomic,weak)UIViewController *controller;
@property (nonatomic,copy)void (^bottomResultPresent)(NSString *);
@property (nonatomic,strong)UITapGestureRecognizer *recognizer;
+(instancetype)showWithController:(UIViewController *)controller andData:(NSArray *)data;
-(instancetype)initWithController:(UIViewController *)controller;
+(void)showEditPickerViewWithController:(UIViewController *)controller andData:(NSArray *)data andBlock:(void (^)(NSString *temp))bottomResultPresent;
@end
```
LHBottomPickerView.m
```
#import "LHBottomPickerView.h"
#import "AppDelegate.h"
@interface LHBottomPickerView()<UIGestureRecognizerDelegate>
@end
@implementation LHBottomPickerView
-(instancetype)initWithController:(UIViewController *)controller{
self=[super init];
if(self){
    //黑色半透明背景
    AppDelegate *app=(AppDelegate *)[UIApplication sharedApplication].delegate;
    UIView *grayBgView=[[UIView alloc]initWithFrame:[UIScreen mainScreen].bounds];
    grayBgView.backgroundColor=[UIColor colorWithRed:0 green:0 blue:0 alpha:0.5];
    [app.window.rootViewController.view addSubview:grayBgView];
    grayBgView.hidden=YES;
    self.grayBgView=grayBgView;
    
    //为grayBgView添加点击手势
    
    UITapGestureRecognizer *tapGestureRecognizer = [[UITapGestureRecognizer alloc] init];
    tapGestureRecognizer.numberOfTapsRequired = 1; // 设置单击几次才触发方法
    [self.grayBgView setUserInteractionEnabled:YES];
    tapGestureRecognizer.delegate=self;
    [tapGestureRecognizer addTarget:self action:@selector(popAndPushPickerView)]; // 添加点击手势的方法
    tapGestureRecognizer.cancelsTouchesInView = NO;
    [self.grayBgView addGestureRecognizer:tapGestureRecognizer];
    self.recognizer=tapGestureRecognizer;
    
    
    //LHEditPickerView
    LHEditPickView *editView=[[LHEditPickView alloc]initWithFrame:CGRectMake(0, controller.view.bounds.size.height, controller.view.bounds.size.width, controller.view.bounds.size.height/5*2)];
    self.editView=editView;
    self.editView.refreshUserInterface=^(NSString * result){
        if(self.bottomResultPresent){
            self.bottomResultPresent(result);
        }
    };
    self.editView.dropEditPickerView=^(){
        [self popAndPushPickerView];
    };
    [self.grayBgView addSubview:editView];
    
}
return self;
}
+(instancetype)showWithController:(UIViewController *)controller andData:(NSArray *)data{
LHBottomPickerView *bottom=[[self alloc]initWithController:controller];
bottom.controller=controller;
bottom.editView.data=data;
[controller.view addSubview:bottom];
[bottom popAndPushPickerView];
return  bottom;
}
-(void)popAndPushPickerView{
if(self.grayBgView.hidden){
    [UIView animateWithDuration:0.5 animations:^{
        self.grayBgView.hidden=NO;
        self.editView.frame=CGRectMake(0, self.controller.view.bounds.size.height/5*3, self.controller.view.bounds.size.width, self.controller.view.bounds.size.height/5*2);
    }];
    [self.grayBgView setUserInteractionEnabled:YES];
}else{
    [UIView animateWithDuration:0.5 animations:^{
        self.editView.frame=CGRectMake(0, self.controller.view.bounds.size.height, self.controller.view.bounds.size.width, self.controller.view.bounds.size.height/5*2);
    } completion:^(BOOL finished) {
        [UIView animateWithDuration:0.5 animations:^{
            self.grayBgView.hidden=YES;
        }];
    }];
    
}
}
-(BOOL)gestureRecognizer:(UIGestureRecognizer *)gestureRecognizer shouldReceiveTouch:(UITouch *)touch{
if ( [touch.view isKindOfClass:[UIButton class]])
{
    return NO;
}
//由于LHEdiPickView中的toolView也受到了手势的影响，所以在这里将这这个ToolView屏蔽掉
if([touch.view isKindOfClass:[UIToolbar class]]){
    return NO;
}
return YES;
}
+(void)showEditPickerViewWithController:(UIViewController *)controller andData:(NSArray *)data andBlock:(void (^)(NSString *temp))bottomResultPresent{
LHBottomPickerView *bottom=[LHBottomPickerView showWithController:controller andData:data];
bottom.bottomResultPresent=bottomResultPresent;
}
@end
```
下面就是最神奇的地方，调用竟然会如此简单，还是上代码：
ViewController.h
```
#import "ViewController.h"
#import "LHBottomPickerView.h"
@interface ViewController ()
@property (nonatomic,weak)UIView *grayBgView;
@property (nonatomic,weak)LHEditPickView *editView;
@property (nonatomic,weak)UILabel *label;
@property (nonatomic,weak)UIButton *button;
@end
@implementation ViewController
- (void)viewDidLoad {
[super viewDidLoad];
UIButton *button=[UIButton buttonWithType:UIButtonTypeCustom];
button.frame=CGRectMake(0, 0, 100, 100);
button.backgroundColor=[UIColor redColor];
[self.view addSubview:button];
self.button=button;
[button addTarget:self action:@selector(clickBtn) forControlEvents:UIControlEventTouchUpInside];
}
-(void)clickBtn{
//**看到没，就是这么简单**
[LHBottomPickerView showEditPickerViewWithController:self andData:@[@"早晨",@"中午",@"下午"] andBlock:^(NSString *temp) {
    [self.button setTitle:temp forState:UIControlStateNormal];
}];
}
@end
```
**总结一下知识点：**
- block使用：block属性、block参数、block传递、block回调
- 手势冲突
###### 弹出TextView弹框（LHPopTextView）
这个弹框，当时写的时候就想去用AlertController或者AlertView,但是发现alertView和AlertController自带的是一个TextField,并且还是单行的，显然满足不了我的需求，真心不想自己写这一部分东西，但是最后，还是重写吧，因为没有，所以得造。
- 这个与上一个效果略有不同，就是那个灰色的遮盖，第一个是用了UIView然后添加了手势，但是添加手势以后，会出现手势冲突，所以这个我把背景遮盖给换成了一个UIButton，这样子省了不少的功夫。代码如下：
LHTopTextView.h(弹出框的视图)
```
#import <UIKit/UIKit.h>
@interface LHTopTextView : UIView
@property (nonatomic,weak)UITextView *textView;
@property (nonatomic,weak)UIButton *submitBtn;
@property (nonatomic,weak)UIButton *cancelBtn;
@property (nonatomic,weak)UILabel *titleLabel;
@property (nonatomic,copy) void(^submitBlock)(NSString * text);
@property (nonatomic,copy) void(^closeBlock)();
@end
```
LHTopTextView.m
```
#import "LHTopTextView.h"
@interface LHTopTextView()<UITextViewDelegate>{
CGFloat _space;
NSString *_text;
CGFloat _margin;
}
@end
@implementation LHTopTextView
-(instancetype)initWithFrame:(CGRect)frame{
self=[super initWithFrame:frame];
if(self){
    //设置两个控件之间的间距
    _space=10.0;
    
    //设置与边框的间距
    _margin=15.0;
    
    //设置圆角
    self.layer.cornerRadius=5;
    [self.layer setMasksToBounds:YES];
    
    //设置背景色
    self.backgroundColor=[UIColor whiteColor];
    
    //驳回申诉
    UILabel *titleLabel=[[UILabel alloc]initWithFrame:CGRectMake((frame.size.width-2*_margin)/3+_margin, _margin,(frame.size.width-2*_margin)/3, (frame.size.height-_margin*2-_space)/7)];
    self.titleLabel=titleLabel;
    [self addSubview:titleLabel];
    [titleLabel setFont:[UIFont systemFontOfSize:20]];
    titleLabel.textAlignment=NSTextAlignmentCenter;
    [titleLabel setText:@"驳回申诉"];
    //输入框
    UITextView *textView=[[UITextView alloc]initWithFrame:CGRectMake(_margin, CGRectGetMaxY(titleLabel.frame)+_space, frame.size.width-2*_margin, CGRectGetHeight(titleLabel.frame)*4)];
    textView.backgroundColor=[UIColor colorWithRed:0.5 green:0.5 blue:0.5 alpha:0.739140070921986];
    self.textView=textView;
    textView.font=[UIFont systemFontOfSize:15];
    NSString *str=@"请您输入您对处理结果的评价，最多128个字";
    textView.textColor=[UIColor whiteColor];
    textView.text=str;
    textView.returnKeyType=UIReturnKeyDone;
    textView.delegate=self;
    [self addSubview:textView];
    
    //seperateLine
    UIView *lineView=[[UIView alloc]initWithFrame:CGRectMake(0, CGRectGetMaxY(textView.frame)+_margin, frame.size.width, 1)];
    lineView.backgroundColor=[UIColor grayColor];
    [self addSubview:lineView];
    
    //取消按钮
    UIButton *cancelBtn=[UIButton buttonWithType:UIButtonTypeCustom];
    cancelBtn.frame=CGRectMake(0, CGRectGetMaxY(lineView.frame), frame.size.width/2, CGRectGetHeight(titleLabel.frame)*2);
    [cancelBtn setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    [cancelBtn setTitle:@"关闭" forState:UIControlStateNormal];
    self.cancelBtn=cancelBtn;
    [cancelBtn addTarget:self action:@selector(clickCancel:) forControlEvents:UIControlEventTouchUpInside];
    [self addSubview:cancelBtn];
    
    //按钮分隔线
    UIView *seperateLine=[[UIView alloc]initWithFrame:CGRectMake(CGRectGetMaxX(cancelBtn.frame), CGRectGetMinY(cancelBtn.frame), 1, CGRectGetHeight(cancelBtn.frame))];
    seperateLine.backgroundColor=[UIColor grayColor];
    [self addSubview:seperateLine];
    //确定按钮
    UIButton *sureBtn=[UIButton buttonWithType:UIButtonTypeCustom];
    sureBtn.frame=CGRectMake(CGRectGetMaxX(seperateLine.frame), CGRectGetMinY(cancelBtn.frame), CGRectGetWidth(cancelBtn.frame), CGRectGetHeight(cancelBtn.frame));
    self.submitBtn=sureBtn;
    [sureBtn setTitle:@"提交" forState:UIControlStateNormal];
    [sureBtn setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    [sureBtn addTarget:self action:@selector(clickSubmit:) forControlEvents:UIControlEventTouchUpInside];
    [self addSubview:sureBtn];
    
}
return self;
}
-(void)textViewDidBeginEditing:(UITextView *)textView{
textView.textColor=[UIColor blackColor];
textView.text=nil;
}
/**
 * 通过代理方法去设置不能超过128个字，但是可编辑
 */
#pragma mark -UITextViewDelegate
-(void)textViewDidChange:(UITextView *)textView{
if(textView.text.length>=128){
   textView.text=[textView.text substringToIndex:127];
}
}
#pragma mark -return键弹回键盘
-(BOOL)textView:(UITextView *)textView shouldChangeTextInRange:(NSRange)range replacementText:(NSString *)text
```
{
```
if ([text isEqualToString:@"\n"]) {
    
    [textView resignFirstResponder];
    
    return NO;
    
}
return YES;    
}
#pragma mark -处理确定点击事件
-(void)clickSubmit:(id)sender{
if(self.textView.editable){
    [self.textView resignFirstResponder];
}
if(self.textView.text.length>0){
    if([self.textView.textColor isEqual:[UIColor redColor]]||[self.textView.textColor isEqual:[UIColor whiteColor]]){
        [self.textView becomeFirstResponder];
    }else{
        if(self.submitBlock){
            self.submitBlock(self.textView.text);
        }
    }
}else{
    self.textView.textColor=[UIColor redColor];
    self.textView.text=@"您输入的内容不能为空，请您输入内容";
}
}
#pragma mark -处理取消点击事件
-(void)clickCancel:(id)sender{
if(self.closeBlock){
    self.closeBlock();
}
}
@end
```
LHEditTextView.h(显示弹出框和遮盖的视图)
```
#import <UIKit/UIKit.h>
@interface LHEditTextView : UIView
@property (nonatomic,weak)UIButton *grayBgView;
@property (nonatomic,copy)void (^requestDataBlock)(NSString *text);
+(instancetype)showWithController:(UIViewController *)controller;
+(void)showWithController:(UIViewController *)controller andRequestDataBlock:(void(^)(NSString *))requestDataBlock;
@end
```
LHEditTextView.m
```
#import "LHEditTextView.h"
#import "AppDelegate.h"
#import "LHTopTextView.h"
@interface LHEditTextView()<UIGestureRecognizerDelegate>
@property (nonatomic,weak)LHTopTextView *topTextView;
@property (nonatomic,weak)UIViewController *controller;
@end
@implementation LHEditTextView
-(instancetype)initWithController:(UIViewController *)controller{
self=[super init];
if(self){
    //黑色半透明背景
    AppDelegate *app=(AppDelegate *)[UIApplication sharedApplication].delegate;
    UIButton *grayBgView=[UIButton buttonWithType:UIButtonTypeCustom];
    grayBgView.frame=[UIScreen mainScreen].bounds;
    grayBgView.backgroundColor=[UIColor colorWithRed:0 green:0 blue:0 alpha:0.3];
    [app.window.rootViewController.view addSubview:grayBgView];
    grayBgView.hidden=YES;
    self.grayBgView=grayBgView;
    
    
    [grayBgView addTarget:self action:@selector(popAndPushPickerView) forControlEvents:UIControlEventTouchUpInside];
    
    LHTopTextView *topTextView=[[LHTopTextView alloc]initWithFrame:CGRectMake(15, controller.view.bounds.size.height/3, controller.view.bounds.size.width-30, controller.view.bounds.size.height/3)];
    self.topTextView=topTextView;
    topTextView.submitBlock=^(NSString *text){
        [self popAndPushPickerView];
        if(self.requestDataBlock){
            self.requestDataBlock(text);
        }
    };
    topTextView.closeBlock=^(){
        [self popAndPushPickerView];
    };
    [self.grayBgView addSubview:topTextView];
    
}
return self;
}
+(instancetype)showWithController:(UIViewController *)controller{
LHEditTextView *editTextView=[[self alloc]initWithController:controller];
editTextView.controller=controller;
[controller.view addSubview:editTextView];
[editTextView popAndPushPickerView];
return  editTextView;
}
+(void)showWithController:(UIViewController *)controller andRequestDataBlock:(void(^)(NSString *text))requestDataBlock{
LHEditTextView *edit=[LHEditTextView showWithController:controller];
edit.requestDataBlock=requestDataBlock;
}
-(void)popAndPushPickerView{
if(self.grayBgView.hidden){
    [UIView animateWithDuration:0.5 animations:^{
        self.grayBgView.hidden=NO;
        self.topTextView.hidden=NO;
    }];
    [self.grayBgView setUserInteractionEnabled:YES];
}else{
    [UIView animateWithDuration:0.5 animations:^{
        self.topTextView.hidden=YES;
    } completion:^(BOOL finished) {
        [UIView animateWithDuration:0.5 animations:^{
            self.grayBgView.hidden=YES;
        }];
    }];
    
}
}
@end
```
调用还是十分简单：ViewController.m
```
#import "ViewController.h"
#import "LHEditTextView.h"
@interface ViewController ()
@end
@implementation ViewController
- (void)viewDidLoad {
[super viewDidLoad];
}
-(void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event{
[LHEditTextView showWithController:self andRequestDataBlock:^(NSString *text) {
    NSLog(@"这里面去实现数据的回调");
}];
}
- (void)didReceiveMemoryWarning {
[super didReceiveMemoryWarning];
// Dispose of any resources that can be recreated.
}
@end
```
这是这两天写的小demo,其实有心的人会把这两个demo封装到一起的，这样子今后用这也方便。。。。降龙十八掌，打完收工！
有想要demo的，可以加我QQ：635326856
最近我发现好多人问我要代码，但是我整天不开电脑，给你们说，第一种样式其实很简单，就是将上面的代码给复制下来生成对应名字的文件（.h和.m文件）就可以了。

作者：Peak_One
链接：https://www.jianshu.com/p/328789026dba
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
