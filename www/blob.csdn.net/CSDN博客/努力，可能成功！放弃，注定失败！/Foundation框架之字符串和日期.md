# Foundation框架之字符串和日期 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年10月08日 14:59:14[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：306
# 字符串
## NSString
```php
```php
- (NSString *)substringFromIndex:(NSUInteger)from;//截取字符串从from到[string length]位置
- (NSString *)substringToIndex:(NSUInteger)to; //截取字符串从0到to位置
- (NSString *)substringWithRange:(NSRange)range;//按照range范围截取字符串
string = [string uppercaseString];  // 将string转成大写
string = [string lowercaseString];  // 将string转成小写
BOOL ret1 = [string hasPrefix:@"He”]; // 判断string 是否以@“He”开头
BOOL ret2 = [string hasSuffix:@"hhh"];  // 判断string 是否以@“hhh”结尾
NSRange pos = [string rangeOfString:@"iOS"]; // 获取字符串@"iOS"首次出现的位置
- (unichar)characterAtIndex:(NSUInteger)index;// 指定索引位置的字符。
```
```
### 常见开发需求
- 
判断字符串为`nil`或者为`@“"`
```bash
//当字符串为nil的时候，执行[testStr length] 时同样返回0
if ([testStr length] == 0) {
}
```
【思考】当服务端返回`testStr = @“ ”;`时，上面判断就有问题了。
解决方案：先trim去空格然后再判断。
```
NSString *test3 = [testStr stringByTrimmingCharactersInSet:[NSCharacterSetwhitespaceCharacterSet] ];
if ([test3 length] == 0) {
      NSLog(@"判断成功");
  }
```
- 判断字符串1中包含字符串2
```
NSString *string = @"http://upload-images.jianshu.io/upload_images/783355-c0fbbdc98cbe6c99.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240";
if ([string containsString:@"http:"]) { 
   NSLog(@"这个是网络图片");}
else {  NSLog(@"这个是本地图片");
}
```
【注意】`containsString`出现在`iOS8`以后，所以在`iOS7`系统上程序必定崩溃
通常解决方法
```
if ([string rangeOfString:@"http:"].location == NSNotFound) {   
   NSLog(@"这个是本地图片");} 
else {  
  NSLog(@"这个是网络图片");
}
```
- NSString转换成NSData对象
`NSString* str = @"teststring";NSData* data = [str dataUsingEncoding:NSUTF8StringEncoding];`
- 
```
NSString *appName = [infoDictionary objectForKey:@"CFBundleName"];
  NSString *jsonPath = [[NSBundle mainBundle] pathForResource:appName ofType:@"json"];
  NSDictionary *jsonDict = @{};
  if(jsonPath){
      NSData *jsonData = [NSData dataWithContentsOfFile:jsonPath];
      NSError *error;
      jsonDict = [NSJSONSerialization JSONObjectWithData:jsonData options:kNilOptions error:&error];
  }
```
## NSMutableString
```
[str appendString:@",appendingStr!"];
[str appendFormat:@"%@。",@"appendingStr!"];
[str insertString:  atIndex:]; //插入字符串到指定索引
deleteCharactersInRange:NSMakeRange //删除范围内字符串
replaceCharactersInRange:NSMakeRange //替代字符串。
```
## copy和mutableCopy的区别
![](http://upload-images.jianshu.io/upload_images/783355-7d0b0cadf148b3fc.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
mutableCopy和copy的区别
# 日期与时间
## 1.地区时间和时区时间
```
#地区时间
NSLocale* locale = [NSLocale currentLocale];//当前地区
NSString* str = [date1 descriptionWithLocale:locale];//根据local获取当前时间字符串。
//指定地区
 NSLocale* locales[] = {[[NSLocale alloc]initWithLocaleIdentifier:@"zh_CN"],
        [[NSLocale alloc]initWithLocaleIdentifier:@"en_US"]};
例：
 NSString *str1 = [date description];
 NSString *str2 = [date descriptionWithLocale:locale];//根据时区获
  NSLog(@"description:%@,descriptionWithLocale:%@",str1,str2);
//手机语言为英文
//description:2016-04-06 05:36:08 +0000,descriptionWithLocale:Wednesday, April 6, 2016 at 1:36:08 PM China Standard Time
//手机语言为中文的结果
//description:2016-04-06 05:38:30 +0000,descriptionWithLocale:2016年4月6日 星期三 中国标准时间 下午1:38:30
#时区
NSTimeZone *timeZone = [NSTimeZone systemTimeZone];//系统所在时区
NSTimeZone *timeZone = [NSTimeZone timeZoneForSecondsFromGMT:0 * 3600];//直接指定时区
```
【附】地区识别码
`zh-cn 简体中文``zh-tw 繁体中文``da-dk 丹麦语``nl-nl 荷兰语`
`en-us 英语``fi-fi 芬兰语``fr-fr 法语``de-de 德语``it-it 意大利`
`ja-jp 日语``ko-kr 朝鲜语``nb-no 挪威语``pt-br 葡萄牙语``es-es 西班牙语``es-us 西班牙语（美国）``sv-se 瑞典语`
## 2. 初始化和时间差
```
#- (instancetype)initWithTimeInterval:(NSTimeInterval)secsToBeAdded sinceDate:(NSDate *)date;
//[NSDate date]获取当前时间的date对象
//获取从当前时间开始，一天之后的日期
NSDate* date2 =[[NSDate alloc]initWithTimeInterval:3600*24
 sinceDate:[NSDate date]];NSLog(@"%@",date2);//2016-04-07 05:36:08 +0000
//[注]：用date对象调用+ (instancetype)dateWithTimeInterval:(NSTimeInterval)secsToBeAdded sinceDate:(NSDate *)date;
#- (instancetype)initWithTimeIntervalSinceNow:(NSTimeInterval)secs;
NSDate* date2 =[[NSDate alloc]initWithTimeIntervalSinceNow:3600*24];
NSLog(@"%@",date2);//2016-11-18 09:38:57 +0000
//[注]：用date对象调用+ (instancetype)dateWithTimeIntervalSinceNow:(NSTimeInterval)secs;
#- (instancetype)initWithTimeIntervalSince1970:(NSTimeInterval)secs;
//获取从1970年1月1日开始，20年后的日期
NSDate* date4 = [[NSDate alloc]initWithTimeIntervalSince1970:3600*24*366*20];
NSLog(@"%@",date4);//1990-01-16 00:00:00 +0000
//[注]：用date对象调用+ (instancetype)dateWithTimeIntervalSince1970:(NSTimeInterval)secs;
#- (id) initWithTimeIntervalSinceReferenceDate:(NSTimeInterval) secs; 
//从2001年1月1日0时0分0秒开始secs秒后的日期。
//[注]：用date对象调用+ (instancetype)dateWithTimeIntervalSinceReferenceDate:(NSTimeInterval)ti;
#- (NSTimeInterval)timeIntervalSinceDate:(NSDate *)anotherDate;
```
## 3. 时间的比较
```
- (NSDate *)earlierDate:(NSDate *)anotherDate;//返回更早的那个时间
     - (NSDate *)laterDate:(NSDate *)anotherDate;//返回相对较晚的时间
     - (NSComparisonResult)compare:(NSDate *)other;
      //  ==                         NSOrderedSame           0
      //  >  anotherDate is earlier, NSOrderedDescending     1
      //  <  anotherDate is later,    NSOrderedAscending.   -1
     - (BOOL)isEqualToDate:(NSDate *)otherDate;
```
## 4.格式化时间
```
NSDateFormatter是NSFormatter的子类，另，NSFormatter的用途是“将数据在字符串与特定类型的对象之间转换”，目前NSFormatter只有两个子类NSNumberFormatter和NSDateFormatter。
主要作用是NSDate和NSString之间进行转换。
```
## 开发常见需求
- 1.将时间戳转化为指定格式的时间字符串。
```
//*时间戳*是指格林威治时间1970年01月01日00时00分00秒(北京时间1970年01月01日08时00分00秒)起至现在的总秒数。
NSDate *date = [[NSDate alloc]initWithTimeIntervalSince1970:[bookDate longLongValue]/1000.0];
NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init] ;
dateFormatter.dateFormat = @"yyyy-MM-dd HH:mm:ss";//注意MM表示月份mm表示时间，HH：0~23时，hh：1~12小时
NSString *currentDateStr = [dateFormatter stringFromDate:date];
```
- 2.NSString转化为NSDate。
```
NSString *str = @"2014年03月11日 04:32:33 +0000";
 NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
 formatter.dateFormat = @"yyyy年MM月dd日 hh:mm:ss ZZZ";
 NSDate *newDate = [formatter dateFromString:str];
 NSLog(@"newDate:%@",newDate);
```
【注意】对于转换过程中出现结果不正确的问题注意以下几点
1.formatter.dateFormat = @"yyyy年MM月dd日 hh:mm:ss ZZZ";自定义的格式化字符串必须与给定的字符串str格式相同
2.得到小时不正确就是时区的问题所以字符串末尾带上时区+0000(GMT)格式化规则字符串末尾带上 ZZZ
- 3.从NSDate对象中获取年、月、日、时、分、秒 ······。
```
NSDateComponents *components = [[NSCalendar currentCalendar] components:NSCalendarUnitDay | NSCalendarUnitMonth | NSCalendarUnitYear 
fromDate:[NSDate date]];
NSInteger day = [components day];
NSInteger month = [components month];
NSInteger year = [components year];
```
【附录】上面例子中- (NSDateComponents *)components:(NSCalendarUnit)unitFlags fromDate:(NSDate*)date;
`NSCalendarUnit`对象包括 ：纪元、年、月、日、时、分、秒、星期等
如下：
`NSCalendarUnitEra``NSCalendarUnitYear``NSCalendarUnitMonth``NSCalendarUnitDay``NSCalendarUnitHour``NSCalendarUnitMinute``NSCalendarUnitSecond``NSCalendarUnitWeekday``NSCalendarUnitWeekdayOrdinal`等
文／BrandonYum（简书作者）
原文链接：http://www.jianshu.com/p/8742f143e3d0
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
