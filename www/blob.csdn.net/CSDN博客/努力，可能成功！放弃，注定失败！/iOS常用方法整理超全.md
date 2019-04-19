# iOS常用方法整理超全 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年11月14日 11:04:48[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：583
一.富文本：
```
/**
*  富文本声明
*/
+ (NSMutableAttributedString *)getLableText:(NSString *)text changeText:(NSString *)changeString Color:(UIColor *)color font:(NSInteger)font;
```
```
/**
*  富文本实现
*/
+ (NSMutableAttributedString *)getLableText:(NSString *)text changeText:(NSString *)changeString Color:(UIColor *)color font:(NSInteger)font{
    
    if ([self isBlankString:changeString] == YES || text == nil) {
        return nil;
        
    }
    if ([text rangeOfString:changeString].location != NSNotFound ) {
        
        NSMutableAttributedString *noteStr = [[NSMutableAttributedString alloc] initWithString:text];
        NSRange range = [text rangeOfString:changeString];
        
        
        NSDictionary *dict = @{NSForegroundColorAttributeName:color,NSFontAttributeName:[UIFont systemFontOfSize:font]};
        [noteStr addAttributes:dict range:range];
        return noteStr;
        
        
    }
    NSMutableAttributedString *noteStr = [[NSMutableAttributedString alloc] initWithString:text];
    return noteStr;
}
```
二.判断字符串是否为空:
```
/**
 *  判断字符串是否为空声明
 */
+ (BOOL) isBlankString:(NSString *)string;
```
```
/**
 *  判断字符串是否为空实现
 */
+ (BOOL) isBlankString:(NSString *)string {
    if (string == nil || string == NULL) {
        return YES;
    }
    if ([string isKindOfClass:[NSNull class]]) {
        return YES;
    }
    if ([string isEqualToString:@"(null)"]) {
        return YES;
    }
    if ([string isEqualToString:@"<null>"]) {
        return YES;
    }
    if ([[string stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]] length]==0) {
        return YES;
    }
    return NO;
}
```
三.字符串转UTF8：
```
/**
 *  字符串转UTF8声明
 */
+(NSString *)strByUIF8String:(NSString *)str;
```
```
/**
 *  字符串转UTF8实现
 */
+(NSString *)strByUIF8String:(NSString *)str{
    return [str stringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding];
}
```
四.毫秒转时间:
```
/**
 *  毫秒转时间声明
 */
+ (NSString *)ConvertStrToTime:(NSString *)timeStr;
```
```
/**
 *  毫秒转时间实现
 */
   long long time=[timeStr longLongValue];
    
    NSDate *d = [[NSDate alloc]initWithTimeIntervalSince1970:time/1000.0];
    
    NSDateFormatter *formatter = [[NSDateFormatter alloc]init];
    
    [formatter setDateFormat:@"yyyy-MM-dd HH:mm:ss"];
    
    NSString*timeString=[formatter stringFromDate:d];
    
    return timeString;
```
五.车牌正则表达式：
```
/**
 *  车牌格式声明
 */
+ (BOOL) carNumber:(NSString *)carNumber;
```
```
/**
 *  车牌格式实现
 */
+ (BOOL) carNumber:(NSString *)carNumber;
{
    NSString *emailCheck = @"^[京津沪渝冀豫云辽黑湘皖鲁新苏浙赣鄂桂甘晋蒙陕吉闽贵粤青藏川宁琼使领A-Z]{1}[A-Z]{1}[A-Z0-9]{4,5}[A-Z0-9挂学警港澳]{1}$";
    NSPredicate *emailTest = [NSPredicate predicateWithFormat:@"SELF MATCHES%@",emailCheck];
    return [emailTest evaluateWithObject:carNumber];
}
```
六.字符串每四位加横杠：
```
/**
 *  字符串每四位加一个横杠声明
 */
+(NSString *)dealWithString:(NSString *)number;
```
```
/**
 *  字符串每四位加一个横杠实现
 */
+ (NSString *)dealWithString:(NSString *)number
{
    NSString *doneTitle = @"";
    int count = 0;
    for (int i = 0; i < number.length; i++) {
        
        count++;
        doneTitle = [doneTitle stringByAppendingString:[number substringWithRange:NSMakeRange(i, 1)]];
        if (count == 4) {
            doneTitle = [NSString stringWithFormat:@"%@-", doneTitle];
            count = 0;
        }
    }
    NSLog(@"%@", doneTitle);
    return doneTitle;
}
```
七.获取当天时间：
```
/**
 *  获取当天时间声明
 */
+ (NSString *)getCurrentTime;
```
```
/**
 *  获取当天时间实现
 */
+ (NSString *)getCurrentTime {
    NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
    [formatter setDateFormat:@"yyyy-MM-dd"];
    NSString *dateTime = [formatter stringFromDate:[NSDate date]];
    return dateTime;
}
```
八.银行卡号的正则表达式：
```
/**
 *  判断是否是正确的银行卡号声明
 */
+(BOOL) IsBankCard:(NSString *)cardNumber;
```
```
/**
 *  判断是否是正确的银行卡号实现
 */
+ (BOOL) IsBankCard:(NSString *)cardNumber
{
    if(cardNumber.length==0)
    {
        return NO;
    }
    NSString *digitsOnly = @"";
    char c;
    for (int i = 0; i < cardNumber.length; i++)
    {
        c = [cardNumber characterAtIndex:i];
        if (isdigit(c))
        {
            digitsOnly =[digitsOnly stringByAppendingFormat:@"%c",c];
        }
    }
    int sum = 0;
    int digit = 0;
    int addend = 0;
    BOOL timesTwo = false;
    for (NSInteger i = digitsOnly.length - 1; i >= 0; i--)
    {
        digit = [digitsOnly characterAtIndex:i] - '0';
        if (timesTwo)
        {
            addend = digit * 2;
            if (addend > 9) {
                addend -= 9;
            }
        }
        else {
            addend = digit;
        }
        sum += addend;
        timesTwo = !timesTwo;
    }
    int modulus = sum % 10;
    return modulus == 0;
}
```
九.字典转字符串:
```
/**
 * 字典 转换为NSString的json字符串声明
 */
+ (NSString*)convertToStrJson:(id)infoDict;
```
```
/**
 * 字典转换为NSString的json字符串实现
 */
+ (NSString*)convertToStrJson:(id)infoDict
{
    NSError *error;
    NSData *jsonData = [NSJSONSerialization dataWithJSONObject:infoDict
                                                       options:0//NSJSONWritingPrettyPrinted // Pass 0 if you don't care about the readability of the generated string
                                                         error:&error];
    
    NSString *jsonString = @"";
    
    if (! jsonData)
    {
        NSLog(@"Got an error: %@", error);
    }else
    {
        jsonString = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
    }
    
    jsonString = [jsonString stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];  //去除掉首尾的空白字符和换行字符
    
    [jsonString stringByReplacingOccurrencesOfString:@"\n" withString:@""];
    
    return jsonString;
}
```
十.json转字典：
```
/**
 *  转换strJson为字典声明
 */
+ (NSMutableDictionary*)convertToDic:(id)infoStr;
```
```
/**
 *  转换strJson为字典实现
 */
+ (NSMutableDictionary*)convertToDic:(id)infoStr
{
    if (infoStr == nil) {
        return nil;
    }
    
    NSError *error;
    
    NSData  *jsonData = [infoStr dataUsingEncoding:NSUTF8StringEncoding];
    
    NSMutableDictionary    *dic = [NSJSONSerialization JSONObjectWithData:jsonData options:NSJSONReadingMutableContainers error:nil];
    
    if (error) {
        NSLog(@"strJson convert dic flaut");
        return nil;
    }
    return dic;
}
```
十一.比较两个时间的大小：
```
/**
 *  比较两个时间的大小声明
 *
 *  @param oneDay     时间1
 *  @param anotherDay 时间2
 *
 *  @return 0:两个时间相等 1:时间1在前 2:时间1在后
 */
+ (NSInteger)compareOneDay:(NSDate *)oneDay withAnotherDay:(NSDate *)anotherDay;
```
```
/**
 *  比较两个时间的大小实现
 */
+ (NSInteger)compareOneDay:(NSDate *)oneDay withAnotherDay:(NSDate *)anotherDay
{
    NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
    [dateFormatter setDateFormat:@"dd-MM-yyyy"];
    NSString *oneDayStr = [dateFormatter stringFromDate:oneDay];
    NSString *anotherDayStr = [dateFormatter stringFromDate:anotherDay];
    NSDate *dateA = [dateFormatter dateFromString:oneDayStr];
    NSDate *dateB = [dateFormatter dateFromString:anotherDayStr];
    NSComparisonResult result = [dateA compare:dateB];
//    MyLog(@"date1 : %@, date2 : %@", oneDay, anotherDay);
    if (result == NSOrderedDescending) {
        //MyLog(@"Date1  is in the future");
        return 1;
    }
    else if (result == NSOrderedAscending){
        //MyLog(@"Date1 is in the past");
        return -1;
    }
    //MyLog(@"Both dates are the same");
    return 0;
    
}
```
十二.展示报错信息：
```
/**
 *  展示报错信息声明
 */
+ (void)showFailAlert:(NSString *)strAlert;
```
```
/**
 *  展示报错信息实现
 */
+ (void)showFailAlert:(NSString *)strAlert
{
    UIViewController *rootViewController = [[[UIApplication sharedApplication] keyWindow] rootViewController];
    
    if ([SFCommon IsUpIOS9])
    {
        UIAlertController *alert = [UIAlertController alertControllerWithTitle:@"提示"
                                                                       message:strAlert
                                                                preferredStyle:UIAlertControllerStyleAlert];
        
        [alert addAction:[UIAlertAction actionWithTitle:@"确定" style:UIAlertActionStyleDefault handler:^(UIAlertAction * _Nonnull action) {
            
        }]];
        
        [rootViewController presentViewController:alert animated:true completion:nil];
    }
    else
    {
        UIAlertView * alert = [[UIAlertView alloc]initWithTitle:@"提示"
                                                        message:strAlert
                                                       delegate:self
                                              cancelButtonTitle:@"确定"
                                              otherButtonTitles:nil];
        
        alert.tag = 1;
        [alert show];
    }
}
```
十三.MD5签名：
```
/**
 *  MD5签名声明
 */
+ (NSString *)md5 : (NSString *)strInput;
```
```
/**
 *  MD5签名实现
 */
+ (NSString *)md5 : (NSString *)strInput
{
    const char *cStr = [strInput UTF8String];
    
    unsigned    char    digest[CC_MD5_DIGEST_LENGTH];
    
    CC_MD5(cStr, (CC_LONG)strlen(cStr), digest);
    
    NSMutableString *strOutput = [NSMutableString stringWithCapacity:CC_MD5_DIGEST_LENGTH * 2];
    
    for(int i = 0; i < CC_MD5_DIGEST_LENGTH; i++)
    {
        [strOutput appendFormat:@"%02x", digest[i]];
    }
    
    return  strOutput;
}
```
十四.：快速创建UIImageView:
```
/**
 *  快速构建UIimageView声明
 */
+ (UIImageView *)createImageView:(NSString *)imageName tag:(NSInteger)tag click:(void(^)(UIImageView *imageView))imageClick;
```
```
/**
 *  快速构建UIimageView实现
 */
+ (UIImageView *)createImageView:(NSString *)imageName tag:(NSInteger)tag click:(void(^)(UIImageView *imageView))imageClick;
```
```
+ (UIImageView *)createImageView:(NSString *)imageName tag:(NSInteger)tag click:(void(^)(UIImageView *imageView))imageClick{
    UIImageView *imageView = [[UIImageView alloc] initWithImage:[UIImage imageNamed:imageName]];
    if (tag != 0) {
         imageView.tag = tag;
    }
    if (imageClick != nil) {
        [imageView addTapClick:^(UIView *view) {
            __weak UIImageView *imageView1 = (UIImageView *)view;
            if (imageClick) {
                imageClick(imageView1);
            }
        }];
    }
    
    return imageView;
}
```
十五.快速创建UIView:
```
/**
 *  快速构建UIView声明
 */
+ (UIView *)createView:(UIColor *)bgColor tag:(NSInteger)tag click:(void(^)(UIView *view))viewClick;
```
```
/**
 *  快速构建UIView实现
 */
+ (UIView *)createView:(UIColor *)bgColor tag:(NSInteger)tag click:(void(^)(UIView *view))viewClick{
    UIView *view = [[UIView alloc]init];
    if (tag != 0) {
        view.tag = tag;
    }
    view.backgroundColor = bgColor;
    if (viewClick != nil) {
        [view addTapClick:^(UIView *view) {
            if (viewClick) {
                viewClick(view);
            }
        }];
    }
    return view;
}
```
十六：快速创建UIButton:
```
/**
 *  快速构建button声明
 */
+(UIButton *)createBtnFrame:(CGRect)frame title:(NSString *)title bgColor:(UIColor *)color textColor:(UIColor *)textColor textFont:(float)font target:(id)target action:(SEL)action;
+(UIButton *)createBtnFrame:(CGRect)frame title:(NSString *)title bgColor:(UIColor *)color textColor:(UIColor *)textColor textFont:(float)font click:(void(^)(UIButton *button))btnblock;
```
```
/**
 *  快速构建button实现
 */
+(UIButton *)createBtnFrame:(CGRect)frame title:(NSString *)title bgColor:(UIColor *)color textColor:(UIColor *)textColor textFont:(float)font target:(id)target action:(SEL)action
{
    UIButton *btn = [UIButton buttonWithType:UIButtonTypeSystem];
    
    btn.frame = frame;
    
    [btn setTitle:title forState:UIControlStateNormal];
    btn.titleLabel.font = [UIFont systemFontOfSize:font];
    
    if (color) {
        [btn setBackgroundColor:color];
    }
    if (textColor) {
        [btn setTitleColor:textColor forState:(UIControlStateNormal)];
    }
    if (target && action) {
        [btn addTarget:target action:action forControlEvents:UIControlEventTouchUpInside];
    }
    
    return btn;
    
}
+(UIButton *)createBtnFrame:(CGRect)frame title:(NSString *)title bgColor:(UIColor *)color textColor:(UIColor *)textColor textFont:(float)font click:(void(^)(UIButton *button))btnblock;
{
    UIButton *btn = [UIButton buttonWithType:UIButtonTypeSystem];
    
    btn.frame = frame;
    
    [btn setTitle:title forState:UIControlStateNormal];
    btn.titleLabel.font = [UIFont systemFontOfSize:font];
    
    if (color) {
        [btn setBackgroundColor:color];
    }
    if (textColor) {
        [btn setTitleColor:textColor forState:(UIControlStateNormal)];
    }
    [btn handleClickCallBack:^(UIButton *button) {
        if (btnblock) {
            btnblock(button);
        }
    }];
    
    return btn;
    
}
```
十七.快速创建UILabel:
```
/**
 *  快速构建label声明
 */
+(UILabel *)createLabelFrame:(CGRect)frame title:(NSString *)title textColor:(UIColor *)color font:(UIFont *)font textAlignment:(NSTextAlignment)textAlignment numberOfLine:(NSInteger)numberOfLines;
```
```
/**
 *  快速构建label实现
 */
+(UILabel *)createLabelFrame:(CGRect)frame title:(NSString *)title textColor:(UIColor *)color font:(UIFont *)font textAlignment:(NSTextAlignment)textAlignment numberOfLine:(NSInteger)numberOfLines;
```
十八:UITextfield输入字数限制：
```
/**
 *  输入字数限制声明
 */
+ (void)limitInputNumberOfWordsWithMaxLength:(NSUInteger)maxLength  inputView:(id<UITextInput>)inputView inputString:(NSString *)string;
```
```
/**
 *  输入字数限制实现
 */
+ (void)limitInputNumberOfWordsWithMaxLength:(NSUInteger)maxLength  inputView:(id<UITextInput>)inputView inputString:(NSString *)string 
{
    //获取高亮部分
    UITextRange *selectedRange = [inputView markedTextRange];
    UITextPosition *position = [inputView positionFromPosition:selectedRange.start offset:0];
    
    if (!position || !selectedRange) {
        if (string.length > maxLength) {
            
            NSRange range1 = [string rangeOfComposedCharacterSequenceAtIndex:maxLength];
            if (range1.length == 1) {
                if ([inputView isKindOfClass:[UITextField class]]) {//textfield
                    UITextField *field = (UITextField *)inputView;
                    field.text = [string substringToIndex:maxLength];
                }else if([inputView isKindOfClass:[UITextView class]]){
                    UITextView *textView = (UITextView *)inputView;
                    textView.text = [string substringToIndex:maxLength];
                }
                
            }else
            {
                NSRange range2 = [string rangeOfComposedCharacterSequencesForRange:NSMakeRange(0, maxLength)];
                if ([inputView isKindOfClass:[UITextField class]]) {//textfield
                    UITextField *field = (UITextField *)inputView;
                    field.text = [string substringWithRange:range2];
                }else if([inputView isKindOfClass:[UITextView class]]){
                    UITextView *textView = (UITextView *)inputView;
                    textView.text = [string substringWithRange:range2];
                }
                
            }
            
        }
    }
    
}
```
十九.计算文字高度：
```
/**
 *  计算文字的高度声明
 */
+(CGFloat)heightForViewWithString:(NSString*)string FontSize:(NSInteger)fontSize width:(CGFloat)width;
```
```
/**
 *  计算文字的高度实现
 */
+(CGFloat)heightForViewWithString:(NSString*)string FontSize:(NSInteger)fontSize width:(CGFloat)width{
    CGRect frame=[string boundingRectWithSize:CGSizeMake(width,10000000) options:NSStringDrawingUsesLineFragmentOrigin attributes:@{NSFontAttributeName:[UIFont systemFontOfSize:fontSize]} context:nil];
    return frame.size.height;
}
```
二十.字符串生成二维码：
```
/**
 *  生成二维码声明
 */
+(UIImage *)imageCodeString:(NSString *)string;
```
```
/**
 *  生成二维码实现
 */
+(UIImage *)imageCodeString:(NSString *)string{
    CIFilter *filter = [CIFilter filterWithName:@"CIQRCodeGenerator"];
    [filter setDefaults];
    NSString *dataString = string;
    NSData *data = [dataString dataUsingEncoding:NSUTF8StringEncoding];
    [filter setValue:data forKeyPath:@"inputMessage"];
    CIImage *outputImage = [filter outputImage];
    return   [self createNonInterpolatedUIImageFormCIImage:outputImage withSize:200];
}
```
二十一.生成UUID：
```
/**
 *  UUID获取声明
 */
+ (NSString *)getUniqueStrByUUID;
```
```
/**
 *  UUID获取实现
 */
+ (NSString *)getUniqueStrByUUID{
CFUUIDRef    uuidObj = CFUUIDCreate(nil);//create a new UUID
//get the string representation of the UUID
NSString *uuidString = (__bridge_transfer NSString *)CFUUIDCreateString(nil, uuidObj);
CFRelease(uuidObj);
return uuidString;
}
```
二十二.判断字符串中是否有空格
```
/**
 *  判断字符串中是否有空格声明
 */
+ (BOOL)isHaveSpaceInString:(NSString *)string;
```
```
/**
 *  判断字符串中是否有空格实现
 */
+ (BOOL)isHaveSpaceInString:(NSString *)string{
NSRange _range = [string rangeOfString:@" "];
if (_range.location != NSNotFound) {
return YES;
}else {
return NO;
          }
}
```
二十三.判断字符串中是否有中文：
```
/**
 *  判断字符串中是否有中文声明
 */
+ (BOOL)isHaveChineseInString:(NSString *)string;
```
```
/**
 *  判断字符串中是否有中文实现
 */
+ (BOOL)isHaveChineseInString:(NSString *)string{
for(NSInteger i = 0; i < [string length]; i++){
int a = [string characterAtIndex:i];
if (a > 0x4e00 && a < 0x9fff) {
return YES;
}
}
return NO;
}
```
二十四.根据日期计算是星期几：
```
/**
 *  根据日期计算今天是星期几声明
 */
+ (NSString*)weekdayStringFromDate:(NSDate*)inputDate;
```
```
/**
 *  根据日期计算今天是星期几实现
 */
+ (NSString*)weekdayStringFromDate:(NSDate*)inputDate{
NSArray *weekdays = [NSArray arrayWithObjects: [NSNull null], @"周日", @"周一", @"周二", @"周三", @"周四", @"周五", @"周六", nil];
NSCalendar *calendar = [[NSCalendar alloc] initWithCalendarIdentifier:NSGregorianCalendar];
NSTimeZone *timeZone = [[NSTimeZone alloc] initWithName:@"Asia/Shanghai"];
[calendar setTimeZone: timeZone];
NSCalendarUnit calendarUnit = NSWeekdayCalendarUnit;
NSDateComponents *theComponents = [calendar components:calendarUnit fromDate:inputDate];
return [weekdays objectAtIndex:theComponents.weekday];
}
```
二十五.获取文件的大小：
```
/**
 *  获取文件的大小声明
 */
+ (long long)fileSizeAtPath:(NSString *)filePath;
```
```
/**
 *  获取文件的大小实现
 */
+ (long long)fileSizeAtPath:(NSString *)filePath{
NSFileManager *fileManager = [NSFileManager defaultManager];
if (![fileManager fileExistsAtPath:filePath]) return 0;
return [[fileManager attributesOfItemAtPath:filePath error:nil] fileSize];
}
```
二十六.对图片进行模糊处理:
```
/**
 *  对图片进行模糊处理声明
 */
+ (UIImage *)blurWithOriginalImage:(UIImage *)image blurName:(NSString *)name radius:(NSInteger)radius;
```
```
/**
 *  对图片进行模糊处理实现
 */
#pragma mark - 对图片进行模糊处理
// CIGaussianBlur ---> 高斯模糊
// CIBoxBlur      ---> 均值模糊(Available in iOS 9.0 and later)
// CIDiscBlur     ---> 环形卷积模糊(Available in iOS 9.0 and later)
// CIMedianFilter ---> 中值模糊, 用于消除图像噪点, 无需设置radius(Available in iOS 9.0 and later)
// CIMotionBlur   ---> 运动模糊, 用于模拟相机移动拍摄时的扫尾效果(Available in iOS 9.0 and later)
+ (UIImage *)blurWithOriginalImage:(UIImage *)image blurName:(NSString *)name radius:(NSInteger)radius{
CIContext *context = [CIContext contextWithOptions:nil];
CIImage *inputImage = [[CIImage alloc] initWithImage:image];
CIFilter *filter;
if (name.length != 0) {
filter = [CIFilter filterWithName:name];
[filter setValue:inputImage forKey:kCIInputImageKey];
if (![name isEqualToString:@"CIMedianFilter"]) {
[filter setValue:@(radius) forKey:@"inputRadius"];
}
CIImage *result = [filter valueForKey:kCIOutputImageKey];
CGImageRef cgImage = [context createCGImage:result fromRect:[result extent]];
UIImage *resultImage = [UIImage imageWithCGImage:cgImage];
CGImageRelease(cgImage);
return resultImage;
}else{
return nil;
}
}
```
二十七.对图片进行过滤镜处理：
```
/**
 *  对图片进行过滤镜处理声明
 */
+ (UIImage *)filterWithOriginalImage:(UIImage *)image filterName:(NSString *)name;
```
```
/**
 *  对图片进行过滤镜处理实现
 */
#pragma mark - 对图片进行滤镜处理
// 怀旧 --> CIPhotoEffectInstant                         单色 --> CIPhotoEffectMono
// 黑白 --> CIPhotoEffectNoir                            褪色 --> CIPhotoEffectFade
// 色调 --> CIPhotoEffectTonal                           冲印 --> CIPhotoEffectProcess
// 岁月 --> CIPhotoEffectTransfer                        铬黄 --> CIPhotoEffectChrome
// CILinearToSRGBToneCurve, CISRGBToneCurveToLinear, CIGaussianBlur, CIBoxBlur, CIDiscBlur, CISepiaTone, CIDepthOfField
+ (UIImage *)filterWithOriginalImage:(UIImage *)image filterName:(NSString *)name{
CIContext *context = [CIContext contextWithOptions:nil];
CIImage *inputImage = [[CIImage alloc] initWithImage:image];
CIFilter *filter = [CIFilter filterWithName:name];
[filter setValue:inputImage forKey:kCIInputImageKey];
CIImage *result = [filter valueForKey:kCIOutputImageKey];
CGImageRef cgImage = [context createCGImage:result fromRect:[result extent]];
UIImage *resultImage = [UIImage imageWithCGImage:cgImage];
CGImageRelease(cgImage);
return resultImage;
}
```
二十八.计算一个时间距离现在时间有多久
```
/**
 *  计算上一个时间距离现在多久声明
 */
+ (NSString *)timeIntervalFromLastTime:(NSString *)lastTime
lastTimeFormat:(NSString *)format1
ToCurrentTime:(NSString *)currentTime
currentTimeFormat:(NSString *)format2;
+ (NSString *)timeIntervalFromLastTime:(NSDate *)lastTime ToCurrentTime:(NSDate *)currentTime;
```
```
/**
 *  计算上一个时间距离现在多久实现
 */
/**
*  计算上次日期距离现在多久
*
*  @param lastTime    上次日期(需要和格式对应)
*  @param format1     上次日期格式
*  @param currentTime 最近日期(需要和格式对应)
*  @param format2     最近日期格式
*
*  @return xx分钟前、xx小时前、xx天前
*/
+ (NSString *)timeIntervalFromLastTime:(NSString *)lastTime
lastTimeFormat:(NSString *)format1
ToCurrentTime:(NSString *)currentTime
currentTimeFormat:(NSString *)format2{
//上次时间
NSDateFormatter *dateFormatter1 = [[NSDateFormatter alloc]init];
dateFormatter1.dateFormat = format1;
NSDate *lastDate = [dateFormatter1 dateFromString:lastTime];
//当前时间
NSDateFormatter *dateFormatter2 = [[NSDateFormatter alloc]init];
dateFormatter2.dateFormat = format2;
NSDate *currentDate = [dateFormatter2 dateFromString:currentTime];
return [Utility timeIntervalFromLastTime:lastDate ToCurrentTime:currentDate];
}
+ (NSString *)timeIntervalFromLastTime:(NSDate *)lastTime ToCurrentTime:(NSDate *)currentTime{
NSTimeZone *timeZone = [NSTimeZone systemTimeZone];
//上次时间
NSDate *lastDate = [lastTime dateByAddingTimeInterval:[timeZone secondsFromGMTForDate:lastTime]];
//当前时间
NSDate *currentDate = [currentTime dateByAddingTimeInterval:[timeZone secondsFromGMTForDate:currentTime]];
//时间间隔
NSInteger intevalTime = [currentDate timeIntervalSinceReferenceDate] - [lastDate timeIntervalSinceReferenceDate];
//秒、分、小时、天、月、年
NSInteger minutes = intevalTime / 60;
NSInteger hours = intevalTime / 60 / 60;
NSInteger day = intevalTime / 60 / 60 / 24;
NSInteger month = intevalTime / 60 / 60 / 24 / 30;
NSInteger yers = intevalTime / 60 / 60 / 24 / 365;
if (minutes <= 10) {
return  @"刚刚";
}else if (minutes < 60){
return [NSString stringWithFormat: @"%ld分钟前",(long)minutes];
}else if (hours < 24){
return [NSString stringWithFormat: @"%ld小时前",(long)hours];
}else if (day < 30){
return [NSString stringWithFormat: @"%ld天前",(long)day];
}else if (month < 12){
NSDateFormatter * df =[[NSDateFormatter alloc]init];
df.dateFormat = @"M月d日";
NSString * time = [df stringFromDate:lastDate];
return time;
}else if (yers >= 1){
NSDateFormatter * df =[[NSDateFormatter alloc]init];
df.dateFormat = @"yyyy年M月d日";
NSString * time = [df stringFromDate:lastDate];
return time;
}
return @"";
}
```
二十九.RGB转UIColor
```
/**
 * 16进制颜色 转成UIColor声明
 */
+(UIColor *)getColor:(NSString *) hexColor;
```
```
/**
 * 16进制颜色 转成UIColor实现
 */
+(UIColor *)getColor:(NSString *) hexColor{
unsigned int red,green,blue;
NSRange range;
range.length = 2;
range.location = 0;
[[NSScanner scannerWithString:[hexColor substringWithRange:range]]scanHexInt:&red];
range.location = 2;
[[NSScanner scannerWithString:[hexColor substringWithRange:range]]scanHexInt:&green];
range.location = 4;
[[NSScanner scannerWithString:[hexColor substringWithRange:range]]scanHexInt:&blue];
return [UIColor colorWithRed:(float)(red/255.0f)green:(float)(green / 255.0f) blue:(float)(blue / 255.0f)alpha:1.0f];
}
```
三十.获取APP的名字：
```
/**
 * 获取App的名字声明
 */
+(NSString*) getAppName;
```
```
/**
 * 获取App的名字实现
 */
+(NSString*) getAppName
{
return [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleDisplayName"];
}
```
三十一.获取本地版本号:
```
/**
 * 获取本地版本号声明
 */
+(NSString*) getLocalAppVersion;
```
```
/**
 * 获取本地版本号实现
 */
+(NSString*) getLocalAppVersion
{
return [[[NSBundle mainBundle] infoDictionary] objectForKey:@"CFBundleShortVersionString"];
}
```
三十二.去掉字符串中的空格：
```
/**
 * 去掉字符串中的空格声明
 */
+ (NSString *)deleteSpacesInString:(NSString *)string;
```
```
/**
 * 去掉字符串中的空格实现
 */
/**
 *  将字符串中的空格去掉再输出
 *
 *  @param chinese 目标字符串
 *
 *  @return 转换后的字符串
 */
+ (NSString *)deleteSpacesInString:(NSString *)string{
    if ([string containsString:@" "]) {
        return [string stringByReplacingOccurrencesOfString:@" " withString:@""];
    } else {
        return string;
    }
}
```
三十三.将汉字转成拼音:
```
/**
 * 将汉字转成拼音声明
 */
+ (NSString *)transform:(NSString *)chinese;
```
```
/**
 * 将汉字转成拼音实现
 */
/**
 *  将汉字转化成拼音
 *
 *  @param chinese 中文
 *
 *  @return 小写拼音
 */
+ (NSString *)transform:(NSString *)chinese{
    NSMutableString *pinyin = [chinese mutableCopy];
    CFStringTransform((__bridge CFMutableStringRef)pinyin, NULL, kCFStringTransformMandarinLatin, NO);
    CFStringTransform((__bridge CFMutableStringRef)pinyin, NULL, kCFStringTransformStripCombiningMarks, NO);
    return pinyin;
}
```
三十四.字典数据保存到沙盒
```
/**
 *  加入缓存 value/key声明
 */
+(void)cacheKeyValues:(NSDictionary*)value forKey:(NSString*)key;
/**
 *  取出key对应的缓存声明
 */
+(NSDictionary*)getValuesForKey:(NSString*)key;
```
```
/**
 *  加入缓存 value/key实现
 */
+(void)cacheKeyValues:(NSDictionary*)value forKey:(NSString*)key
{
    [[NSUserDefaults standardUserDefaults] setObject:value forKey:key];
    [[NSUserDefaults standardUserDefaults] synchronize];
}
/**
 *  取出key对应的缓存实现
 */
+(NSDictionary*)getValuesForKey:(NSString*)key
{
    return [[NSUserDefaults standardUserDefaults] objectForKey:key];
}
```
三十五.字符串数据保存到沙盒
```
/**
 *  字符串缓存声明
 */
+(void)cacheString:(NSString*)value forKey:(NSString*)key;
/**
 *  获取缓存中的字符串声明
 */
+(NSString*)getStringForKey:(NSString*)key;
```
```
/**
 *  字符串缓存实现
 */
+(void)cacheString:(NSString*)value forKey:(NSString*)key
{
    [[NSUserDefaults standardUserDefaults] setObject:value forKey:key];
    [[NSUserDefaults standardUserDefaults] synchronize];
}
/**
 *  获取缓存中的字符串实现
 */
+(NSString*)getStringForKey:(NSString*)key
{
    return [[NSUserDefaults standardUserDefaults] objectForKey:key];
    
}
```
三十六.NSInteger数据保存到沙盒
```
/**
 *  NSInteger缓存声明
 */
+(void)cacheIntger:(NSInteger )value forKey:(NSString*)key;
/**
 *  获取缓存中的NSInteger声明
 */
+(NSInteger)getIntgerForKey:(NSString*)key;
```
```
/**
 *  NSInteger缓存实现
 */
+(void)cacheIntger:(NSInteger )value forKey:(NSString*)key{
    [[NSUserDefaults standardUserDefaults]setInteger:value forKey:key];
    [[NSUserDefaults standardUserDefaults]synchronize];
}
/**
 *  获取缓存中的NSInteger实现
 */
+(NSInteger)getIntgerForKey:(NSString*)key{
    return [[NSUserDefaults standardUserDefaults]integerForKey:key];
}
```
三十七.数组数据保存到沙盒
```
/**
 *  动态列表缓存声明
 */
+(void)cacheMuTableArray:(NSMutableArray*)value forKey:(NSString*)key;
/**
 *  获取缓存中的动态列表声明
 */
+(NSMutableArray*)getMuTableArrayForKey:(NSString*)key;
```
```
/**
 *  动态列表缓存实现
 */
+(void)cacheMuTableArray:(NSMutableArray*)value forKey:(NSString*)key
{
    [[NSUserDefaults standardUserDefaults] setObject:value forKey:key];
    [[NSUserDefaults standardUserDefaults] synchronize];
}
/**
 *  获取缓存中的动态列表实现
 */
+(NSMutableArray*)getMuTableArrayForKey:(NSString*)key
{
    return [[NSUserDefaults standardUserDefaults] objectForKey:key];
    
}
```
三十八.删除沙盒中的数据
```
/**
 *  删除沙盒中的数据声明
 */
+(void)removeCacheDicForKey:(NSString*)key;
```
```
/**
 *  删除沙盒中的数据实现
 */
+(void)removeCacheDicForKey:(NSString*)key
{
    [[NSUserDefaults standardUserDefaults] setObject:nil forKey:key];
    [[NSUserDefaults standardUserDefaults] synchronize];
}
```
三十九.判断是否开启相机权限：
```
/**
 *  是否开启相机权限声明
 */
+ (BOOL)isAgreeMediaAVAuthor;
```
```
/**
 *  是否开启相机权限实现
 */
+ (BOOL)isAgreeMediaAVAuthor
{
    AVAuthorizationStatus status = [AVCaptureDevice authorizationStatusForMediaType:AVMediaTypeVideo];
    
    if (status == AVAuthorizationStatusDenied || status == AVAuthorizationStatusRestricted) {
        
        return NO;
        
    } else {
        
        return YES;
    }
}
```
四十.判断手机系统 是否在于iOS9
```
/**
 *  判断系统是否大于iOS9声明
 */
+ (BOOL)IsUpIOS9;
```
```
/**
 *  判断系统是否大于iOS9实现
 */
+ (BOOL)IsUpIOS9
{
    float current = [[[UIDevice currentDevice] systemVersion] floatValue];
    
    if (current < 9.0)
    {
        return NO;
    }
    else
    {
        return YES;
    }
}
```
四十一.获取一个时间距离现在是几个月
```
/**
 *  获取几个月前声明
 *  monthAgo,几个月前
 */
+ (NSString *)getAgoTime :(int) monthAgo;
```
```
/**
 *  判断几个月前实现
 */
+ (NSString *)getAgoTime : (int) monthAgo
{
    NSDateFormatter *format = [[NSDateFormatter alloc]init];
    [format setDateFormat:@"yyyyMM"];
    NSDateComponents    *compt = [[NSDateComponents alloc] init];
    [compt setMonth: - monthAgo];
    NSCalendar  *calendar = [NSCalendar currentCalendar];
    NSDate  *dateNow = [calendar dateByAddingComponents:compt toDate:[NSDate date] options:0];
    NSString    *strAgoTime = [format stringFromDate:dateNow];
    
    return strAgoTime;
    
}
```
好了，今天就整理这么多，后续发现有好用的方法会在文章中补上，大家如果有好用的方法可以在留言版给我留言哦，谢谢！
iOS开发交流群:529052159，iOS公众号：iOS开发精髓，每天都会发布精美文章。动动你的小手扫码关注吧！
![image](http://upload-images.jianshu.io/upload_images/1617037-c2fae95f3b4a25cc.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
