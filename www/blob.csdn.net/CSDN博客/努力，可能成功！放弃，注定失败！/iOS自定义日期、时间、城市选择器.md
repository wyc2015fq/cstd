# iOS自定义日期、时间、城市选择器 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年05月16日 14:45:33[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：424
本文来自简书，原文地址:[http://www.jianshu.com/p/c7fb8d7dba6d](http://www.jianshu.com/p/c7fb8d7dba6d)
选择器，我想大家都不陌生，当需要用户去选择某些范围值内的一个固定值时，我们会采用选择器的方式。选择器可以直观的提示用户选择的值范围、统一信息的填写格式，同时也方便用户快速的进行选择，比如对于性别，正常情况下就只有男女两种情况，那这时候用一个选择器给用户进行选择的话，可以避免错误数据的输入，也更方便用户去填写。再比如需要获取用户的生日信息时，采用选择器的方式可以统一生日的格式，如果让用户自行输入的话，可能会出现各种各样的生日信息格式，不利于数据的存储，但是采用选择器的方式的话，用户可找到对应的日期进行选择即可。
在iOS有专门的一个选择器类`UIPickerView`，进入UIPickerView的头文件，我们可以发现
 UIPickerView直接继承了UIView，其事件处理通过代理方法完成，所以创建UIPickerView的时候记得签它的代理`UIPickerViewDataSource`和`UIPickerViewDelegate`。其实它和UITbleView有点像，不过UIPickerView还多了个列属性，UITbleView我们都很熟了，所以可以对比UITbleView的相关属性和代理方法来学习记忆UIPickerView。
### 一． UIPickerView的代理方法
（1）UIPickerViewDataSource对应的代理方法有（其代理方法必须要实现）：
###### 返回显示的列数
```
- (NSInteger)numberOfComponentsInPickerView:(UIPickerView
 *)pickerView;
```
###### 返回每一列中需要显示的行数
`- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component;`
（2）UIPickerViewDelegate对应的代理方法（其代理方法根据需求进行选择性实现）：
###### 返回每一列的宽度
`- (CGFloat)pickerView:(UIPickerView *)pickerView widthForComponent:(NSInteger)component;`
###### 返回每一列的高度
```
- (CGFloat)pickerView:(UIPickerView *)pickerView rowHeightForComponent:(NSInteger)component
 ;
```
###### 返回UIPickerView控件中指定列的列表项的要显示的内容
```
- (nullable NSString *)pickerView:(UIPickerView *)pickerView
 titleForRow:(NSInteger)row forComponent:(NSInteger)component ;
```
###### 返回UIView，作为该UIPickerView控件中指定列的指定行的显示视图
```
- (UIView *)pickerView:(UIPickerView *)pickerView viewForRow:(NSInteger)row
 forComponent:(NSInteger)component reusingView:(nullable UIView *)view ;
```
###### 选择指定列中的指定行
```
- (void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row
 inComponent:(NSInteger)component ;
```
### 二、UIPickerView和UIDatePicker。
UIDatePicker是系统帮我们封装好的一个时间日期选择器，继承于UIControl，UIDatePicker有一定的局限性，因为它只有四种显示样式：
```
UIDatePickerModeTime,          
UIDatePickerModeDate,          
UIDatePickerModeDateAndTime, 
UIDatePickerModeCountDownTimer,
```
如果需求和这四种样式都不符合的话，那么就没办法使用UIDatePicker，比如当只需要显示年、月信息的时候，显然UIDatePicker没办法满足我们的需求，那这时我们只能通过UIPickerView来自定义自己想要的选择器。
### 三、UIPickerView的自定义使用
###### （1）创建基类继承于UIView的WXZBasePickView。
我们常见的选择器的样式是一个带透明背景色的view，底部是内容的选择器，有确定和取消按钮，大致如图：
![](http://upload-images.jianshu.io/upload_images/972702-f6721aef8a8578c9.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/320)
选择器
所以我们创建一个基类view，这个view的样式如图所示样式，之后根据内容的差别创建基于该view的选择器。
在.h中声明各个属性及方法
```
#import <UIKit/UIKit.h>
@interface WXZBasePickView : UIView
@property (nonatomic, strong) UIView *contentView;
//选择器
@property (nonatomic, strong)UIPickerView *pickerView;
//取消按钮
@property (nonatomic, strong)UIButton *cancelButton;
 //确定按钮
@property (nonatomic, strong)UIButton *confirmButton;
//选择器每一列的高度
@property (nonatomic, assign)CGFloat pickerViewHeight;
/**
 *  创建视图,初始化视图时初始数据
 */
- (void)initPickView;
/**
 *  确认按钮的点击事件
 */
- (void)clickConfirmButton;
/**
 *  pickerView的显示
 */
- (void)show;
/**
 *  移除pickerView
 */
- (void)disMiss;
@end
```
在.m中实现相关方法
```
#import "WXZBasePickView.h"
#define ScreenWidth [UIScreen mainScreen].bounds.size.width
#define ScreenHeight [UIScreen mainScreen].bounds.size.height
@implementation WXZBasePickView
- (instancetype)init
{
    self = [super init];
    if (self) {
        _pickerViewHeight      = 250;
        self.bounds = [UIScreen mainScreen].bounds;
        self.backgroundColor = [UIColor colorWithRed:0 green:0 blue:0 alpha:0.64];
        self.layer.opacity = 0.0;
        UITapGestureRecognizer *tap=[[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(disMiss)];
        self.userInteractionEnabled = YES;
        [self addGestureRecognizer:tap];
        [self addSubview:self.contentView];
        [self.contentView addSubview:self.pickerView];
        [self.contentView addSubview:self.cancelButton];
        [self.contentView addSubview:self.confirmButton];
        [self initPickView];
     }
    return self;
}
//初始化选择器内容，创建子类时需实现该父类方法
-(void)initPickView{
}
//点击确定按钮
- (void)clickConfirmButton
{
    [self disMiss];
}
//点击取消按钮
- (void) clickCancelButton
{
    [self disMiss];
}
//显示选择器
 - (void)show
{
    [[UIApplication sharedApplication].keyWindow addSubview:self];
    [self setCenter:[UIApplication sharedApplication].keyWindow.center];
    [[UIApplication sharedApplication].keyWindow bringSubviewToFront:self];
        CGRect frame =  self.contentView.frame;
       frame.origin.y -= self.contentView.frame.size.height;
        [UIView animateWithDuration:0.4 delay:0 options:UIViewAnimationOptionCurveEaseOut animations:^{
            [self.layer setOpacity:1.0];
            self.contentView.frame = frame;
        } completion:^(BOOL finished) {
        }];
    }
//移除选择器
- (void)disMiss
{
        CGRect frame =  self.contentView.frame;
        frame.origin.y += self.contentView.frame.size.height;
        [UIView animateWithDuration:0.4 delay:0 options:UIViewAnimationOptionCurveEaseOut animations:^{
            [self.layer setOpacity:0.0];
            self.contentView.frame = frame;
        } completion:^(BOOL finished) {
            [self removeFromSuperview];
        }];
}
//设置选择器的高度
- (void)setPickerViewHeight:(CGFloat)pickerViewHeight
{
    _pickerViewHeight = pickerViewHeight;
    self.contentView.frame = CGRectMake(self.contentView.frame.origin.x, self.contentView.frame.origin.y, self.contentView.frame.size.width, pickerViewHeight);
}
- (UIView *)contentView
{
    if (!_contentView) {
        _contentView = [[UIView alloc]initWithFrame:CGRectMake(0, ScreenHeight, ScreenWidth, self.pickerViewHeight)];
        [_contentView setBackgroundColor:[UIColor whiteColor]];
    }
    return _contentView;
}
- (UIPickerView *)pickerView
{
    if (!_pickerView) {
        _pickerView = [[UIPickerView alloc]initWithFrame:CGRectMake(0,  0, self.contentView.frame.size.width, self.contentView.frame.size.height)];
        [_pickerView setBackgroundColor:[UIColor whiteColor]];
    }
    return _pickerView;
}
- (UIButton *)cancelButton
{
    if (!_cancelButton) {
        _cancelButton = [[UIButton alloc]initWithFrame:CGRectMake(16, 0, 44, 44)];
        [_cancelButton setTitle:@"取消" forState:UIControlStateNormal];
        [_cancelButton setTitleColor:[UIColor grayColor] forState:UIControlStateNormal];
        [_cancelButton.titleLabel setFont:[UIFont systemFontOfSize:16]];
        [_cancelButton addTarget:self action:@selector(clickCancelButton) forControlEvents:UIControlEventTouchUpInside];
    }
    return _cancelButton;
}
- (UIButton *)confirmButton
{
    if (!_confirmButton) {
        _confirmButton = [[UIButton alloc]initWithFrame:CGRectMake(self.contentView.frame.size.width - self.cancelButton.frame.size.width - self.cancelButton.frame.origin.x, self.cancelButton.frame.origin.y, self.cancelButton.frame.size.width, self.cancelButton.frame.size.height)];
        [_confirmButton setTitle:@"确定" forState:UIControlStateNormal];
        [_confirmButton setTitleColor:[UIColor blueColor] forState:UIControlStateNormal];
        [_confirmButton.titleLabel setFont:[UIFont systemFontOfSize:16]];
        [_confirmButton addTarget:self action:@selector(clickConfirmButton) forControlEvents:UIControlEventTouchUpInside];
    }
    return _confirmButton;
}
@end
```
###### (2)、自定义一个日期选择器，可选择显示年、月或者年、月、日以及是否显示“至今”选项。
创建继承于我们自定义好的WXZBasePickView的日期选择器WXZPickDateView
.h声明相关属性和方法
```
#import "WXZBasePickView.h"
@class WXZBasePickView;
//选择器的选择代理方法
@protocol  PickerDateViewDelegate<NSObject>
- (void)pickerDateView:(WXZBasePickView *)pickerDateView selectYear:(NSInteger)year selectMonth:(NSInteger)month selectDay:(NSInteger)day;
@end
@interface WXZPickDateView : WXZBasePickView
@property(nonatomic, weak)id <PickerDateViewDelegate>delegate ;
@property(nonatomic, assign)BOOL isAddYetSelect;//是否增加至今的选项
@property(nonatomic, assign)BOOL isShowDay;//是否显示日
//设置默认显示的值
-(void)setDefaultTSelectYear:(NSInteger)defaultSelectYear defaultSelectMonth:(NSInteger)defaultSelectMonth defaultSelectDay:(NSInteger)defaultSelectDay;
@end
```
.m实现相关方法
```
#import "WXZPickDateView.h"
@interface WXZPickDateView()<UIPickerViewDataSource, UIPickerViewDelegate>
/** 选择的年 */
@property (nonatomic, assign)NSInteger selectYear;
/** 选择的月 */
@property (nonatomic, assign)NSInteger selectMonth;
/** 选择的日 */
@property (nonatomic, assign)NSInteger selectDay;
@property (nonatomic, assign)NSInteger currentYear;
@property (nonatomic, assign)NSInteger currentMonth;
@property (nonatomic, assign)NSInteger currentDay;
@property (nonatomic, assign)NSInteger defaultYear;
@property (nonatomic, assign)NSInteger defaultMonth;
@property (nonatomic, assign)NSInteger defaultDay;
@property (nonatomic, assign)NSInteger minShowYear;
@property (nonatomic, assign)NSInteger yearSum;
@end
@implementation WXZPickDateView
- (void)initPickView
{
    [super initPickView];
    _minShowYear = 1940;//最小年份
    NSCalendar *gregorian = [[NSCalendar alloc]initWithCalendarIdentifier:NSCalendarIdentifierGregorian];
    // 获取当前日期
    NSDate* dt = [NSDate date];
    // 指定获取指定年、月、日、时、分、秒的信息
    unsigned unitFlags = NSCalendarUnitYear |
    NSCalendarUnitMonth |  NSCalendarUnitDay |
    NSCalendarUnitHour |  NSCalendarUnitMinute |
    NSCalendarUnitSecond | NSCalendarUnitWeekday;
    // 获取不同时间字段的信息
    NSDateComponents* comp = [gregorian components: unitFlags fromDate:dt];
    _yearSum = comp.year-_minShowYear+1;
    _currentYear=comp.year;
    _currentMonth=comp.month;
    _currentDay=comp.day;
    _selectYear  = comp.year;
    _selectMonth = comp.month;
    _selectDay   = comp.day;
    _defaultYear = comp.year;
    _defaultMonth = comp.month;
    _defaultDay=comp.day;
    [self.pickerView setDelegate:self];
    [self.pickerView setDataSource:self];
}
-(void)setDefaultTSelectYear:(NSInteger)defaultSelectYear defaultSelectMonth:(NSInteger)defaultSelectMonth defaultSelectDay:(NSInteger)defaultSelectDay{
    if (defaultSelectYear!=0) {
     _defaultYear=defaultSelectYear;
    }
    if (defaultSelectMonth!=0) {
        _defaultMonth = defaultSelectMonth;
    }
    if (defaultSelectDay!=0) {
         _defaultDay=defaultSelectDay;
    }
    if (defaultSelectYear==-1) {
        _defaultYear=_currentYear+1;
        _defaultMonth=1;
        _defaultDay=1;
    }
    [self.pickerView selectRow:(_defaultYear - _minShowYear) inComponent:0 animated:NO];
    [self.pickerView reloadComponent:1];
    [self.pickerView selectRow:(_defaultMonth - 1) inComponent:1 animated:NO];
    if (_isShowDay==YES) {
        [self.pickerView reloadComponent:2];
        [self.pickerView selectRow:_defaultDay inComponent:1 animated:NO];
    }
    [self refreshPickViewData];
}
-(void)setIsAddYetSelect:(BOOL)isAddYetSelect{
    _isAddYetSelect = isAddYetSelect;
}
- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView
{
//判断是否要显示日，如果显示则返回3列，反之返回2列
    if (_isShowDay==YES) {
        return 3;
    }else{
        return 2;
    }
}
- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component
{
    if (component == 0) {
        if (_isAddYetSelect==YES) {
            //显示至今选项的话，需要比总共要显示的年份多返回一行
            return self.yearSum+1;
        }else{
           return self.yearSum;
        }
    }else if(component == 1) {
        NSInteger yearSelected = [pickerView selectedRowInComponent:0] + self.minShowYear;
        if (yearSelected==_currentYear+1) {
            //至今选项的时候月份信息不返回
            return 0;
        }else{
            return 12;
        }
    }else {
        NSInteger yearSelected = [pickerView selectedRowInComponent:0] + self.minShowYear;
        if (yearSelected==_currentYear+1) {
             //至今选项的时候日信息不返回
            return 0;
        }else{
        NSInteger yearSelected = [pickerView selectedRowInComponent:0] + self.minShowYear;
        NSInteger monthSelected = [pickerView selectedRowInComponent:1] + 1;
        return  [self getDaysWithYear:yearSelected month:monthSelected];
        }
    }
}
//根据年、月判断日期天数
- (NSInteger)getDaysWithYear:(NSInteger)year
                       month:(NSInteger)month
{
    switch (month) {
        case 1:
            return 31;
            break;
        case 2:
            if (year%400==0 || (year%100!=0 && year%4 == 0)) {
                return 29;
            }else{
                return 28;
            }
            break;
        case 3:
            return 31;
            break;
        case 4:
            return 30;
            break;
        case 5:
            return 31;
            break;
        case 6:
            return 30;
            break;
        case 7:
            return 31;
            break;
        case 8:
            return 31;
            break;
        case 9:
            return 30;
            break;
        case 10:
            return 31;
            break;
        case 11:
            return 30;
            break;
        case 12:
            return 31;
            break;
        default:
            return 0;
            break;
    }
}
- (CGFloat)pickerView:(UIPickerView *)pickerView rowHeightForComponent:(NSInteger)component
{
    //每一行的高度
    return 36;
}
- (void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component
{
    NSInteger selectYear;
    NSInteger selectMonth;
    switch (component) {
        case 0:
            [pickerView reloadComponent:1];
            if (_isAddYetSelect==YES) {
                selectYear = row+_minShowYear+1;
            }else{
                selectYear = row+_minShowYear;
            }
            if (_isShowDay==YES) {
                [pickerView reloadComponent:2];
            }
            break;
        case 1:
            selectMonth = row+1;
            if (_isShowDay==YES) {
                [pickerView reloadComponent:2];
            }
        default:
            break;
    }
    [self refreshPickViewData];
}
- (UIView *)pickerView:(UIPickerView *)pickerView viewForRow:(NSInteger)row forComponent:(NSInteger)component reusingView:(nullable UIView *)view
{
    NSString *text;
    if (component == 0) {
        if (_isAddYetSelect==YES) {
            if (row+_minShowYear==_currentYear+1) {
                text=@"至今";
            }else{
                text =  [NSString stringWithFormat:@"%zd年", row + _minShowYear];
            }
        }else{
            text =  [NSString stringWithFormat:@"%zd年", row + _minShowYear];
        }
    }else if (component == 1){
        if (_isAddYetSelect==YES) {
            NSInteger yearSelected = [pickerView selectedRowInComponent:0] + self.minShowYear;
            if (yearSelected==_currentYear+1) {
                text =  [NSString stringWithFormat:@""];
            }else{
                text =  [NSString stringWithFormat:@"%zd月", row + 1];
            }
        }else{
            text =  [NSString stringWithFormat:@"%zd月", row + 1];
        }
    }else{
        text = [NSString stringWithFormat:@"%zd日", row + 1];
    }
    UILabel *label = [[UILabel alloc]init];
    label.textAlignment = 1;
    label.font = [UIFont systemFontOfSize:16];
    label.text = text;
    return label;
}
- (void)clickConfirmButton
{
    if ([self.delegate respondsToSelector:@selector(pickerDateView:selectYear:selectMonth:selectDay:)]) {
        [self.delegate pickerDateView:self selectYear:self.selectYear selectMonth:self.selectMonth selectDay:self.selectDay];
    }
    [super clickConfirmButton];
}
- (void)refreshPickViewData
{
    self.selectYear  = [self.pickerView selectedRowInComponent:0] + self.minShowYear;
    self.selectMonth = [self.pickerView selectedRowInComponent:1] + 1;
    if (_isShowDay==YES) {
          self.selectDay   = [self.pickerView selectedRowInComponent:2] + 1;
    }
}
- (void)setYearLeast:(NSInteger)yearLeast
{
    _minShowYear = yearLeast;
}
- (void)setYearSum:(NSInteger)yearSum
{
    _yearSum = yearSum;
}
-(void)setIsShowDay:(BOOL)isShowDay{
    _isShowDay=isShowDay;
}
@end
```
###### （3） 创建继承于WXZBasePickView的时间选择器WXZPickTimeView。
.h声明属性及相关方法
```
#import "WXZBasePickView.h"
@class WXZPickTimeView;
@protocol  PickTimeViewDelegate<NSObject>
- (void)pickerTimeView:(WXZPickTimeView *)pickerTimeView selectHour:(NSInteger)hour selectMinute:(NSInteger)minute ;
@end
@interface WXZPickTimeView : WXZBasePickView
@property(nonatomic, weak)id <PickTimeViewDelegate>delegate ;
-(void)setDefaultHour:(NSInteger)hour defaultMinute:(NSInteger)minute  ;
@end
```
.m实现相关方法
```
#import "WXZPickTimeView.h"
@interface WXZPickTimeView () <UIPickerViewDataSource, UIPickerViewDelegate>{
    UIDatePicker    *_datePicker;
}
@property (nonatomic, assign)NSInteger selectHour;
@property (nonatomic, assign)NSInteger selectMinute;
@property (nonatomic, assign)NSInteger day;
@property (nonatomic, assign)NSInteger defaultHour;
@property (nonatomic, assign)NSInteger defaultMinute;
@end
@implementation WXZPickTimeView
 - (void)initPickView
{
    [super initPickView];
    NSCalendar *gregorian = [[NSCalendar alloc]
                             initWithCalendarIdentifier:NSCalendarIdentifierGregorian];
    // 获取当前日期
    NSDate* dt = [NSDate date];
    // 定义一个时间字段的旗标，指定将会获取指定年、月、日、时、分、秒的信息
    unsigned unitFlags = NSCalendarUnitYear |
    NSCalendarUnitMonth |  NSCalendarUnitDay |
    NSCalendarUnitHour |  NSCalendarUnitMinute |
    NSCalendarUnitSecond | NSCalendarUnitWeekday;
    // 获取不同时间字段的信息
    NSDateComponents* comp = [gregorian components: unitFlags
                                          fromDate:dt];
    _selectHour=comp.hour;
    _selectMinute=comp.minute;
    [self.pickerView setDelegate:self];
    [self.pickerView setDataSource:self];
    [self.pickerView selectRow:_selectHour inComponent:0 animated:NO];
    [self.pickerView selectRow:_selectMinute inComponent:1 animated:NO];
}
-(void)setDefaultHour:(NSInteger)hour defaultMinute:(NSInteger)minute{
    if (hour!=0) {
     _defaultHour=hour;
    }else{
        _defaultHour=_selectHour;
    }
    if (minute!=0) {
        _defaultMinute = minute;
    }else{
        _defaultMinute=_selectMinute;
    }
    [self.pickerView selectRow:_defaultHour inComponent:0 animated:NO];
    [self.pickerView reloadComponent:1];
    [self.pickerView selectRow:_defaultMinute inComponent:1 animated:NO];
    [self refreshPickTimeViewData];
}
- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView
{
    return 2;
}
- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component
{
    if (component == 0) {
        return 24;
    }else {
        return 60;
    }
}
- (CGFloat)pickerView:(UIPickerView *)pickerView rowHeightForComponent:(NSInteger)component
{
    return 36;
}
- (void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component
{
    switch (component) {
        case 0:
            [pickerView reloadComponent:1];
            break;
        case 1:
        default:
            break;
    }
    [self refreshPickTimeViewData];
}
- (UIView *)pickerView:(UIPickerView *)pickerView viewForRow:(NSInteger)row forComponent:(NSInteger)component reusingView:(nullable UIView *)view
{
    NSString *text;
    if (component == 0) {
        if (row<10) {
            text =  [NSString stringWithFormat:@"0%zd时", row];
        }else{
            text =  [NSString stringWithFormat:@"%zd时", row];
        }
    }else if (component == 1){
        if (row<10) {
            text =  [NSString stringWithFormat:@"0%zd分", row ];
        }else{
            text =  [NSString stringWithFormat:@"%zd分", row ];
        }
    }else{
    }
    UILabel *label = [[UILabel alloc]init];
    label.textAlignment = 1;
    label.font = [UIFont systemFontOfSize:16];
    label.text = text;
    return label;
}
- (void)clickConfirmButton
{
    if ([self.delegate respondsToSelector:@selector(pickerTimeView:selectHour:selectMinute:)]) {
        [self.delegate pickerTimeView:self selectHour:self.selectHour selectMinute:self.selectMinute];
    }
    [super clickConfirmButton];
}
- (void)refreshPickTimeViewData
{
    self.selectHour  = [self.pickerView selectedRowInComponent:0];
    self.selectMinute = [self.pickerView selectedRowInComponent:1];
}
@end
```
###### （4）创建继承于WXZBasePickView的城市选择器WXZPickCityView。
.h声明相关属性和方法
```
#import "WXZBasePickView.h"
@class WXZPickCityView;
@protocol  PickerCityViewDelegate<NSObject>
- (void)pickerArea:(WXZPickCityView *)pickerArea selectProvince:(NSString *)province selectCity:(NSString *)city;
@end
@interface WXZPickCityView : WXZBasePickView
@property(nonatomic, weak)id <PickerCityViewDelegate>delegate ;
/**设置默认的省市*/
-(void)setDefaultCity:(NSString *)defaultCity  DefaultProvience:(NSString *)defaultProvience;
@end
```
.m实现相关方法
```
#import "WXZPickCityView.h"
@interface WXZPickCityView()<UIPickerViewDataSource, UIPickerViewDelegate>
@property (nonatomic, strong, nullable)NSArray *areaDataSource;
@property (nonatomic, strong, nullable)NSMutableArray *provinceArray;
@property (nonatomic, strong, nullable)NSMutableArray *cityArray;
@property (nonatomic, strong, nullable)NSMutableArray *selectedArray;//当前选中的数组
@property (nonatomic, strong, nullable)NSString *selectProvince;
@property (nonatomic, strong, nullable)NSString *selectCity;
@end
@implementation WXZPickCityView
- (void)initPickView
{
    [super initPickView];
    for (NSDictionary *dic in self.areaDataSource) {
        [self.provinceArray addObject:dic[@"state"]];
    }
    NSMutableArray *citysArr = [NSMutableArray arrayWithArray:[self.areaDataSource firstObject][@"cities"]];
    for (NSDictionary *dic in citysArr) {
        [self.cityArray addObject:dic[@"city"]];
    }
    self.selectProvince = self.provinceArray[0];
    self.selectCity = self.cityArray[0];
    [self.pickerView setDelegate:self];
    [self.pickerView setDataSource:self];
}
//设置默认显示的省市
-(void)setDefaultCity:(NSString *)defaultCity DefaultProvience:(NSString *)defaultProvience{
    for (NSInteger i = 0; i<_provinceArray.count; i++) {
        if ([_provinceArray[i] isEqualToString:defaultProvience]) {
            [self.pickerView selectRow:i inComponent:0 animated:NO];
            self.selectedArray = self.areaDataSource[i][@"cities"];
            [self.cityArray removeAllObjects];
            [self.selectedArray enumerateObjectsUsingBlock:^(NSDictionary *obj, NSUInteger idx, BOOL * _Nonnull stop) {
                [self.cityArray addObject:obj[@"city"]];
            }];
            for (NSInteger j= 0; j<_cityArray.count; j++) {
                if ([defaultCity isEqualToString:_cityArray[j]]) {
                    [self.pickerView selectRow:i inComponent:0 animated:NO];
                    [self.pickerView reloadComponent:1];
                    [self.pickerView selectRow:j inComponent:1 animated:NO];
                    [self refreshSelectAreaData];
                }
            }
        }
    }
}
- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView
{
    return 2;
}
- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component
{
    if (component == 0) {
        return self.provinceArray.count;
    }else  {
        return self.cityArray.count;
    }
}
- (CGFloat)pickerView:(UIPickerView *)pickerView rowHeightForComponent:(NSInteger)component
{
    return 36;
}
- (void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component
{
    if (component == 0) {
        self.selectedArray = self.areaDataSource[row][@"cities"];
        [self.cityArray removeAllObjects];
        [self.selectedArray enumerateObjectsUsingBlock:^(NSDictionary *obj, NSUInteger idx, BOOL * _Nonnull stop) {
            [self.cityArray addObject:obj[@"city"]];
        }];
        [pickerView reloadComponent:1];
        [pickerView selectRow:0 inComponent:1 animated:YES];
    }else if (component == 1) {
        if (self.selectedArray.count == 0) {
            self.selectedArray = [self.areaDataSource firstObject][@"cities"];
        }
    }else{
    }
    [self refreshSelectAreaData];
}
- (UIView *)pickerView:(UIPickerView *)pickerView viewForRow:(NSInteger)row forComponent:(NSInteger)component reusingView:(nullable UIView *)view
{
    for(UIView *singleLine in pickerView.subviews)
    {
        if (singleLine.frame.size.height < 1)
        {
            singleLine.backgroundColor =[UIColor grayColor];
        }
    }
    NSString *text;
    if (component == 0) {
        text =  self.provinceArray[row];
    }else if (component == 1){
        text =  self.cityArray[row];
    }else{
    }
    UILabel *label = [[UILabel alloc]init];
    label.textAlignment = 1;
    label.font = [UIFont systemFontOfSize:16];
    label.text = text;
    return label;
}
- (void)clickConfirmButton
{
    [self.delegate pickerArea:self selectProvince:self.selectProvince selectCity:self.selectCity];
    [super clickConfirmButton];
}
- (void)refreshSelectAreaData
{
    NSInteger provienceIndex = [self.pickerView selectedRowInComponent:0];
    NSInteger cityIndex = [self.pickerView selectedRowInComponent:1];
    self.selectProvince = self.provinceArray[provienceIndex];
    self.selectCity = self.cityArray[cityIndex];
}
- (NSArray *)areaDataSource
{
    if (!_areaDataSource) {
        NSString *path = [[NSBundle bundleForClass:[WXZPickCityView class]] pathForResource:@"area" ofType:@"plist"];
        _areaDataSource = [[NSArray alloc]initWithContentsOfFile:path];
    }
    return _areaDataSource;
}
- (NSMutableArray *)provinceArray
{
    if (!_provinceArray) {
        _provinceArray = [NSMutableArray array];
    }
    return _provinceArray;
}
- (NSMutableArray *)cityArray
{
    if (!_cityArray) {
        _cityArray = [NSMutableArray array];
    }
    return _cityArray;
}
- (NSMutableArray *)selectedArray
{
    if (!_selectedArray) {
        _selectedArray = [NSMutableArray array];
    }
    return _selectedArray;
}
@end
```
###### （5）创建继承于WXZBasePickView的单列选择器WXZCustomPickView，可根据传进来的数组显示相关的选择内容
.h声明相关属性和方法
```
#import "WXZBasePickView.h"
@class WXZCustomPickView;
@protocol  CustomPickViewDelegate<NSObject>
- (void)customPickView:(WXZCustomPickView *)customPickView selectedTitle:(NSString *)selectedTitle;
@end
@interface WXZCustomPickView :WXZBasePickView
@property (nonatomic, strong)NSMutableArray *dataArray;
@property(nonatomic, copy)NSString *defalutSelectRowStr;
@property(nonatomic, weak)id <CustomPickViewDelegate>delegate;
@end
```
.m实现相关方法
```
#import "WXZCustomPickView.h"
@interface WXZCustomPickView()<UIPickerViewDataSource, UIPickerViewDelegate>
/** 1.选中的字符串 */
@property (nonatomic, strong, nullable)NSString *selectedTitle;
@end
@implementation WXZCustomPickView
- (void)initPickView
{
    [super initPickView];
    _dataArray=[NSMutableArray mutableCopy];
    [self.pickerView setDelegate:self];
    [self.pickerView setDataSource:self];
}
-(void)setDefalutSelectRowStr:(NSString *)defalutSelectRowStr{
    _defalutSelectRowStr=defalutSelectRowStr;
    for (NSInteger i = 0; i<_dataArray.count; i++) {
        if ([_dataArray[i] isEqualToString:_defalutSelectRowStr]) {
            [self.pickerView reloadAllComponents];
            [self.pickerView selectRow:i inComponent:0 animated:NO];
        }
    }
}
- (NSInteger)numberOfComponentsInPickerView:(UIPickerView *)pickerView
{
    return 1;
}
- (NSInteger)pickerView:(UIPickerView *)pickerView numberOfRowsInComponent:(NSInteger)component
{
    return self.dataArray.count;
}
- (CGFloat)pickerView:(UIPickerView *)pickerView rowHeightForComponent:(NSInteger)component
{
    return 36;
}
- (CGFloat)pickerView:(UIPickerView *)pickerView widthForComponent:(NSInteger)component
{
    return self.frame.size.width;
}
- (void)pickerView:(UIPickerView *)pickerView didSelectRow:(NSInteger)row inComponent:(NSInteger)component
{
    self.selectedTitle = self.dataArray[row];
}
- (UIView *)pickerView:(UIPickerView *)pickerView viewForRow:(NSInteger)row forComponent:(NSInteger)component reusingView:(nullable UIView *)view
{
    for(UIView *singleLine in pickerView.subviews)
    {
        if (singleLine.frame.size.height < 1)
        {
            singleLine.backgroundColor =[UIColor grayColor];
        }
    }
    UILabel *label = [[UILabel alloc]init];
    label.font=[UIFont systemFontOfSize:16];
    label.textAlignment = 1;
    label.text=self.dataArray[row];
        return label;
}
#pragma mark -点击确定按钮
- (void)clickConfirmButton
{
    [self.delegate customPickView:self selectedTitle:self.selectedTitle];
    [super clickConfirmButton];
}
- (void)setDataArray:(NSMutableArray *)dataArray
{
    _dataArray = dataArray;
    _selectedTitle = dataArray.firstObject;
    [self.pickerView reloadAllComponents];
}
@end
```
这样，几种类型的选择器我们都定义好了，在需要选择器的地方，根据需求创建相应的选择器即可。
### 四、如何使用已创建好的自定义选择器
引入自定义好的选择器，声明相关属性，签相关协议
```
#import "ViewController.h"
#import "WXZPickDateView.h"
#import "WXZPickAreaView.h"
#import "WXZPickTimeView.h"
#import "WXZCustomPickView.h"
@interface ViewController ()<PickerDateViewDelegate,PickerAreaViewDelegate,PickTimeViewDelegate,CustomPickViewDelegate>{
    UIButton *dateButton;
    UIButton *selectAreabutton;
    UIButton *selectTimeButton;
    UIButton *singlePickViewSelectButton;
    BOOL isShowDay;//是否显示日信息
}
@end
```
创建相应的按钮，触发相应的选择器
```
- (void)viewDidLoad {
    [super viewDidLoad];
    dateButton=[UIButton buttonWithType:UIButtonTypeCustom];
    dateButton.frame=CGRectMake(100, 100, 100, 50);
    [dateButton setTitle:@"选择日期" forState:UIControlStateNormal];
    [self.view addSubview:dateButton];
    [dateButton setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    [dateButton addTarget:self action:@selector(pickViewSelect:) forControlEvents:UIControlEventTouchUpInside];
    dateButton.tag = 1000;
      selectAreabutton=[UIButton buttonWithType:UIButtonTypeCustom];
    selectAreabutton.frame=CGRectMake(100, 200, 100, 50);
    [selectAreabutton setTitle:@"选择地区" forState:UIControlStateNormal];
    [self.view addSubview:selectAreabutton];
    [selectAreabutton setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    [selectAreabutton addTarget:self action:@selector(pickViewSelect:) forControlEvents:UIControlEventTouchUpInside];
    selectAreabutton.tag = 1001;
    selectTimeButton=[UIButton buttonWithType:UIButtonTypeCustom];
    selectTimeButton.frame=CGRectMake(100, 300, 100, 50);
    [selectTimeButton setTitle:@"选择时间" forState:UIControlStateNormal];
    [self.view addSubview:selectTimeButton];
    [selectTimeButton setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    [selectTimeButton addTarget:self action:@selector(pickViewSelect:) forControlEvents:UIControlEventTouchUpInside];
    selectTimeButton.tag = 1002;
    singlePickViewSelectButton=[UIButton buttonWithType:UIButtonTypeCustom];
    singlePickViewSelectButton.frame=CGRectMake(100, 400, 100, 50);
    [singlePickViewSelectButton setTitle:@"单个数据选择器" forState:UIControlStateNormal];
    [self.view addSubview:singlePickViewSelectButton];
    [singlePickViewSelectButton setTitleColor:[UIColor blackColor] forState:UIControlStateNormal];
    [singlePickViewSelectButton addTarget:self action:@selector(pickViewSelect:) forControlEvents:UIControlEventTouchUpInside];
    singlePickViewSelectButton.tag = 1003;
}
```
按钮的点击事件：
```
-(void)pickViewSelect:(UIButton *)button{
    if (button.tag==1000) {
           WXZPickDateView *pickerDate = [[WXZPickDateView alloc]init];
            [pickerDate setIsAddYetSelect:YES];//是否显示至今选项
            [pickerDate setIsShowDay:YES];//是否显示日信息
            [pickerDate setDefaultTSelectYear:2007 defaultSelectMonth:4 defaultSelectDay:1];//设定默认显示的日期
            [pickerDate setValidTime:2010];
            [pickerDate setDelegate:self];
           [pickerDate show];
    }else if (button.tag==1001){
        WXZPickAreaView *pickerArea = [[WXZPickAreaView alloc]init];
        [pickerArea setDelegate:self];
        [pickerArea setDefaultCity:@"上海" DefaultProvience:@"上海"];
        [pickerArea show];
        [self.view endEditing:YES];
    }else if (button.tag==1002){
        WXZPickTimeView *pickerArea = [[WXZPickTimeView alloc]init];
        [pickerArea setDelegate:self];
        [pickerArea setDefaultHour:14 defaultMinute:20];
        [pickerArea show];
        [self.view endEditing:YES];
    }else{
        NSMutableArray *arrayData = [NSMutableArray arrayWithObjects:@"2k以下",@"2k-5k",@"5k-10k",@"10k-15k",@"15k-25k",@"25k-50k",@"50k以上", nil];
        WXZCustomPickView *pickerSingle = [[WXZCustomPickView alloc]init];
        [pickerSingle setDataArray:arrayData];
        [pickerSingle setDefalutSelectRowStr:arrayData[0]];
        [pickerSingle setDelegate:self];
        [pickerSingle show];
        [self.view endEditing:YES];
    }
}
```
选择器的代理方法
```
-(void)pickerDateView:(WXZBasePickView *)pickerDateView selectYear:(NSInteger)year selectMonth:(NSInteger)month selectDay:(NSInteger)day{
    NSLog(@"选择的日期是：%ld %ld %ld",year,month,day);
    if (isShowDay==YES) {
        [dateButton setTitle:[NSString stringWithFormat:@"%ld年 %ld月 %ld日",year,month,day] forState:UIControlStateNormal];
    }else{
        [dateButton setTitle:[NSString stringWithFormat:@"%ld年 %ld月",year,month] forState:UIControlStateNormal];
    }
}
-(void)pickerArea:(WXZPickAreaView *)pickerArea selectProvince:(NSString *)province selectCity:(NSString *)city{
    NSLog(@"省市的选择%@ %@",province,city);
    [selectAreabutton setTitle:[NSString stringWithFormat:@"%@%@",province,city] forState:UIControlStateNormal];
}
-(void)customPickView:(WXZCustomPickView *)customPickView selectedTitle:(NSString *)selectedTitle{
    NSLog(@"选择%@",selectedTitle);
    [singlePickViewSelectButton setTitle:selectedTitle forState:UIControlStateNormal];
}
-(void)pickerTimeView:(WXZPickTimeView *)pickerTimeView selectHour:(NSInteger)hour selectMinute:(NSInteger)minute{
    NSLog(@"选择的时间：%ld %ld",hour,minute);
    [selectTimeButton setTitle:[NSString stringWithFormat:@"%ld时 %ld分",hour,minute] forState:UIControlStateNormal];
}
```
最后，看看效果图：
![](http://upload-images.jianshu.io/upload_images/972702-44b941b588bc4261.gif?imageMogr2/auto-orient/strip)
选择器demo效果图
有需要的可以下载demo看看
# [demo地址](https://github.com/wxzwork/CustomPickView)
