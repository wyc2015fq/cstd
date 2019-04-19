# iOS时间格式化输出技巧 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年04月17日 10:02:32[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：587
![](http://upload-images.jianshu.io/upload_images/2229730-c5461b52892079ee.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
[时间格式化输出](http://www.jianshu.com/p/8f68fe8be4f4)
## [本文来自简书，原文地址:http://www.jianshu.com/p/8f68fe8be4f4](http://www.jianshu.com/p/8f68fe8be4f4)
## 一.前言
最近项目开发过程中用到了大量的关于时间的处理,将后台返回的时间字符串转换为指定的格式时间再显示在UI上.
例如: 将后台返回的时间字符串`2017-04-16 13:08:06`转换为:`2017年04月16日`、`2017年04月`、`04月16日`、`2017-04-16`、`2017-04`、`04-16`、`13:08`、`星期几`等等.
项目是多人开发,由于前期没有统一处理时间转换的问题,后期发现项目中好多关于时间转换的代码,大部分都是通过(- : 等字符)截取成字符串数组再取相应时间拼接成指定格式,输出在UI显示的地方,代码非常的臃肿,并且这种方式非常不可取.
原因:后台返回的时间字符串 并不都是 `2017-04-16 13:08:06`这种格式,还有`2017-04-16`这种格式,截取前需要长度格式等校验,多了很多校验代码.非常不可取.
既然是时间,我们便要通过时间的思维来完成转换问题,不要通过截取字符串的方式
于是我便写了一个类,来统一处理转换问题.
代码地址:见片尾
## 二.效果
![](http://upload-images.jianshu.io/upload_images/2229730-e2d5192debec537c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Demo.png
具体怎么操作:
## 三.将时间字符串->NSDate
首先我们要将`2017-04-16 13:08:06`或`2017-04-16`这种格式时间字符串转换为NSDate
我们新建一个NSDate的Category,笔者取名为NSDate+XHCategory,写一个时间字符串->NSDate方法,代码如下:
```
+(NSDate*)xh_dateWithFormat_yyyy_MM_dd_HH_mm_ss_string:(NSString *)string
{
    NSDateFormatter* dateFormat = [[NSDateFormatter alloc] init];
    [dateFormat setDateFormat:@"yyyy-MM-dd HH:mm:ss"];
    NSDate *date =[dateFormat dateFromString:string];
    return date;
}
```
为了兼用其他格式时间字符串,我们把可能的情况都写上,如下
```
+(NSDate *)xh_dateWithFormat_yyyy_MM_dd_HH_mm_string:(NSString *)string
{
    NSDateFormatter* dateFormat = [[NSDateFormatter alloc] init];
    [dateFormat setDateFormat:@"yyyy-MM-dd HH:mm"];
    NSDate *date =[dateFormat dateFromString:string];
    return date;
}
+(NSDate *)xh_dateWithFormat_yyyy_MM_dd_HH_string:(NSString *)string
{
    NSDateFormatter* dateFormat = [[NSDateFormatter alloc] init];
    [dateFormat setDateFormat:@"yyyy-MM-dd HH"];
    NSDate *date =[dateFormat dateFromString:string];
    return date;
}
+(NSDate *)xh_dateWithFormat_yyyy_MM_dd_string:(NSString *)string
{
    NSDateFormatter* dateFormat = [[NSDateFormatter alloc] init];
    [dateFormat setDateFormat:@"yyyy-MM-dd"];
    NSDate *date =[dateFormat dateFromString:string];
    return date;
}
+(NSDate *)xh_dateWithFormat_yyyy_MM_string:(NSString *)string
{
    NSDateFormatter* dateFormat = [[NSDateFormatter alloc] init];
    [dateFormat setDateFormat:@"yyyy-MM"];
    NSDate *date =[dateFormat dateFromString:string];
    return date;
}
```
再写一个统一转换时间字符串为 NSDate的方法,如下:
```
+(NSDate *)xh_dateWithDateString:(NSString *)dateString
{
    NSDate *date = nil;
    date = [self xh_dateWithFormat_yyyy_MM_dd_HH_mm_ss_string:dateString];
    if(date) return date;
    date = [self xh_dateWithFormat_yyyy_MM_dd_HH_mm_string:dateString];
    if(date) return date;
    date = [self xh_dateWithFormat_yyyy_MM_dd_HH_string:dateString];
    if(date) return date;
    date = [self xh_dateWithFormat_yyyy_MM_dd_string:dateString];
    if(date) return date;
    date = [self xh_dateWithFormat_yyyy_MM_string:dateString];
    if(date) return date;
    return nil;
}
```
## 四.将NSDate -> NSDateFormatter
为什么要再转换为NSDateFormatter,有些人可能已经明白了,我们点开NSDateFormatter可以看到NSDateFormatter有以下属性
```
@property (nullable, copy) NSCalendar *calendar NS_AVAILABLE(10_7, 4_0);
@property (nullable, copy) NSTimeZone *timeZone NS_AVAILABLE(10_7, 4_0);
@property NSInteger era;
@property NSInteger year;
@property NSInteger month;
@property NSInteger day;
@property NSInteger hour;
@property NSInteger minute;
@property NSInteger second;
@property NSInteger nanosecond NS_AVAILABLE(10_7, 5_0);
@property NSInteger weekday;
@property NSInteger weekdayOrdinal;
@property NSInteger quarter NS_AVAILABLE(10_6, 4_0);
@property NSInteger weekOfMonth NS_AVAILABLE(10_7, 5_0);
@property NSInteger weekOfYear NS_AVAILABLE(10_7, 5_0);
@property NSInteger yearForWeekOfYear NS_AVAILABLE(10_7, 5_0);
@property (getter=isLeapMonth) BOOL leapMonth NS_AVAILABLE(10_8, 6_0);
@property (nullable, readonly, copy) NSDate *date NS_AVAILABLE(10_7, 4_0);
@end
```
我们新建一个NSDateComponents 的Category,笔者取名NSDateComponents+XHCategory,并实现如下方法:
```
+(NSDateComponents *)xh_dateComponentsFromDate:(NSDate *)date
{
    NSDateComponents *components = [[NSCalendar currentCalendar] components:NSCalendarUnitYear| NSCalendarUnitMonth | NSCalendarUnitDay | NSCalendarUnitWeekOfYear |  NSCalendarUnitHour | NSCalendarUnitMinute | NSCalendarUnitSecond | NSCalendarUnitWeekday | NSCalendarUnitWeekdayOrdinal fromDate:date];
    return components;
}
```
接着我们就可以进行转换操作了,我们新建一个NSString的Category,笔者取名NSString+XHDateFormat
在NSString+XHDateFormat.h文件中写上需要转换的类型如下:
```
```
/**
 *  x年x月x日
 */
@property(nonatomic,copy,readonly)NSString *xh_formatNianYueRi;
/**
 *  x年x月
 */
@property(nonatomic,copy,readonly)NSString *xh_formatNianYue;
/**
 *  x月x日
 */
@property(nonatomic,copy,readonly)NSString *xh_formatYueRi;
/**
 *  x年
 */
@property(nonatomic,copy,readonly)NSString *xh_formatNian;
/**
 *  x时x分x秒
 */
@property(nonatomic,copy,readonly)NSString *xh_formatShiFenMiao;
/**
 *  x时x分
 */
@property(nonatomic,copy,readonly)NSString *xh_formatShiFen;
/**
 *  x分x秒
 */
@property(nonatomic,copy,readonly)NSString *xh_formatFenMiao;
/**
 *  yyyy-MM-dd
 */
@property(nonatomic,copy,readonly)NSString *xh_format_yyyy_MM_dd;
/**
 *  yyyy-MM
 */
@property(nonatomic,copy,readonly)NSString *xh_format_yyyy_MM;
/**
 *  MM-dd
 */
@property(nonatomic,copy,readonly)NSString *xh_format_MM_dd;
/**
 *  yyyy
 */
@property(nonatomic,copy,readonly)NSString *xh_format_yyyy;
/**
 *  HH:mm:ss
 */
@property(nonatomic,copy,readonly)NSString *xh_format_HH_mm_ss;
/**
 *  HH:mm
 */
@property(nonatomic,copy,readonly)NSString *xh_format_HH_mm;
/**
 *  mm:ss
 */
@property(nonatomic,copy,readonly)NSString *xh_format_mm_ss;
#pragma mark - 转换为星期几
@property(nonatomic,copy,readonly)NSString *xh_formatWeekDay;
```
```
在 NSString+XHDateFormat.m 实现如下:
```
-(NSString *)xh_formatNianYueRi
{
    NSDate *date = [NSDate xh_dateWithDateString:self];
    return [NSString stringWithFormat:@"%ld年%02ld月%02ld日",date.year,date.month,date.day];
}
-(NSString *)xh_formatNianYue
{
    NSDate *date = [NSDate xh_dateWithDateString:self];
    return [NSString stringWithFormat:@"%ld年%02ld月",date.year,date.month];
}
-(NSString *)xh_formatYueRi
{
    NSDate *date = [NSDate xh_dateWithDateString:self];
    return [NSString stringWithFormat:@"%02ld月%02ld月",date.month,date.day];
}
-(NSString *)xh_formatNian
{
    NSDate *date = [NSDate xh_dateWithDateString:self];
    return [NSString stringWithFormat:@"%ld年",date.year];
}
-(NSString *)xh_formatShiFenMiao
{
    NSDate *date = [NSDate xh_dateWithDateString:self];
    return [NSString stringWithFormat:@"%ld时%02ld分%02ld秒",date.hour,date.minute,date.seconds];
}
-(NSString *)xh_formatShiFen
{
    NSDate *date = [NSDate xh_dateWithDateString:self];
    return [NSString stringWithFormat:@"%ld时%02ld分",date.hour,date.minute];
}
-(NSString *)xh_formatFenMiao
{
    NSDate *date = [NSDate xh_dateWithDateString:self];
    return [NSString stringWithFormat:@"%02ld分%02ld秒",date.minute,date.seconds];
}
-(NSString *)xh_format_yyyy_MM_dd
{
   NSDate *date = [NSDate xh_dateWithDateString:self];
   return [NSString stringWithFormat:@"%ld-%02ld-%02ld",date.year,date.month,date.day];
}
-(NSString *)xh_format_yyyy_MM
{
    NSDate *date = [NSDate xh_dateWithDateString:self];
    return [NSString stringWithFormat:@"%ld-%02ld",date.year,date.month];
}
-(NSString *)xh_format_MM_dd
{
    NSDate *date = [NSDate xh_dateWithDateString:self];
    return [NSString stringWithFormat:@"%02ld-%02ld",date.month,date.day];
}
-(NSString *)xh_format_yyyy
{
    NSDate *date = [NSDate xh_dateWithDateString:self];
    return [NSString stringWithFormat:@"%ld",date.year];
}
-(NSString *)xh_format_HH_mm_ss
{
    NSDate *date = [NSDate xh_dateWithDateString:self];
    return [NSString stringWithFormat:@"%02ld:%02ld:%02ld",date.hour,date.minute,date.seconds];
}
-(NSString *)xh_format_HH_mm
{
    NSDate *date = [NSDate xh_dateWithDateString:self];
    return [NSString stringWithFormat:@"%02ld:%02ld",date.hour,date.minute];
}
-(NSString *)xh_format_mm_ss
{
    NSDate *date = [NSDate xh_dateWithDateString:self];
    return [NSString stringWithFormat:@"%02ld:%02ld",date.minute,date.seconds];
}
-(NSString *)xh_formatWeekDay
{
    NSString *weekStr=nil;
    NSDate *date = [NSDate xh_dateWithDateString:self];
    switch (date.weekday) {
        case 2:
            weekStr = @"星期一";
            break;
        case 3:
            weekStr = @"星期二";
            break;
        case 4:
            weekStr = @"星期三";
            break;
        case 5:
            weekStr = @"星期四";
            break;
        case 6:
            weekStr = @"星期五";
            break;
        case 7:
            weekStr = @"星期六";
            break;
        case 1:
            weekStr = @"星期天";
            break;
        default:
            break;
    }
    return weekStr;
}
```
## 五.调用:
```
self.timeString = @"2017-04-16 13:08:06";
    //星期
    NSString *time0 = self.timeString.xh_formatWeekDay;
    //2017年04月16日
    NSString *time1 = self.timeString.xh_formatNianYueRi;
    //2017年04月
    NSString *time2 = self.timeString.xh_formatNianYue;
    //04月16日
    NSString *time3 = self.timeString.xh_formatYueRi;
    //2017年
    NSString *time4 = self.timeString.xh_formatNian;
    //13时08分01秒
    NSString *time5 = self.timeString.xh_formatShiFenMiao;
    //13时08分
    NSString *time6 = self.timeString.xh_formatShiFen;
    //08分01秒
    NSString *time7 = self.timeString.xh_formatFenMiao;
    //2017-04-16
    NSString *time8 = self.timeString.xh_format_yyyy_MM_dd;
    //2017-04
    NSString *time9 = self.timeString.xh_format_yyyy_MM;
    //04-16
    NSString *time10 = self.timeString.xh_format_MM_dd;
    //2017
    NSString *time11 = self.timeString.xh_format_yyyy;
    //13:08:06
    NSString *time12 = self.timeString.xh_format_HH_mm_ss;
    //13:08
    NSString *time13 = self.timeString.xh_format_HH_mm;
    //08:06
    NSString *time14 = self.timeString.xh_format_mm_ss;
```
代码地址:[https://github.com/CoderZhuXH/XHDate](https://github.com/CoderZhuXH/XHDate)
