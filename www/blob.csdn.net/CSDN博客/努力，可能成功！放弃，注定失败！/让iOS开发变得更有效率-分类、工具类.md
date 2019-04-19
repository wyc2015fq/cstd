# 让iOS开发变得更有效率-分类、工具类 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年06月27日 17:00:48[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1306
在工作中整理的一些分类与工具类，分享给大家。这些工具类可以减少项目中的代码量，让代码变得更简洁，可以大大的提升项目的效率，直接拖到项目中使用即可。下载地址：[https://github.com/leeSmile/tools/tree/master/textTools](https://github.com/leeSmile/tools/tree/master/textTools) 欢迎start
## 分类
### NSString+LXExtension
1.根据文件名计算文件大小
2.快速生成缓存/文档/临时目录路径
3.根据文字返回文本占用的高度/宽度
4.MD5加密
```
/**
 *  md5加密
 */
+ (NSString*)md5HexDigest:(NSString*)input;
/**
 *  根据文件名计算出文件大小
 */
- (unsigned long long)lx_fileSize;
/**
 *  生成缓存目录全路径
 */
- (instancetype)cacheDir;
/**
 *  生成文档目录全路径
 */
- (instancetype)docDir;
/**
 *  生成临时目录全路径
 */
- (instancetype)tmpDir;
/**
 *  @brief 根据字数的不同,返回UILabel中的text文字需要占用多少Size
 *  @param size 约束的尺寸
 *  @param font 文本字体
 *  @return 文本的实际尺寸
 */
- (CGSize)textSizeWithContentSize:(CGSize)size font:(UIFont *)font;
/**
 *  @brief  根据文本字数/文本宽度约束/文本字体 求得text的Size
 *  @param width 宽度约束
 *  @param font  文本字体
 *  @return 文本的实际高度
 */
- (CGFloat)textHeightWithContentWidth:(CGFloat)width font:(UIFont *)font;
/**
 *  @brief  根据文本字数/文本宽度约束/文本字体 求得text的Size
 *  @param height 宽度约束
 *  @param font  文本字体
 *  @return 文本的实际长度
 */
- (CGFloat)textWidthWithContentHeight:(CGFloat)height font:(UIFont *)font;
```
### UIImage+LXExtension
1.快速生成圆形图片
2.给定一个不要渲染的图片名称，生成一个最原始的图片
3.模糊效果
4.固定宽高
5.剪切图片某一部分
6.将自身填充到指定的size
```
- (instancetype)lx_circleImage;
// 生成一个圆形图片
+ (instancetype)lx_circleImageNamed:(NSString *)name;
// 给定一个不要渲染的图片名称,生成一个最原始的图片
+ (UIImage *)imageWithOriginalImageName:(NSString *)imageName;
// 模糊效果(渲染很耗时间,建议在子线程中渲染)
- (UIImage *)blurImage;
- (UIImage *)blurImageWithMask:(UIImage *)maskImage;
- (UIImage *)blurImageWithRadius:(CGFloat)radius;
- (UIImage *)blurImageAtFrame:(CGRect)frame;
// 灰度效果
- (UIImage *)grayScale;
// 固定宽度与固定高度
- (UIImage *)scaleWithFixedWidth:(CGFloat)width;
- (UIImage *)scaleWithFixedHeight:(CGFloat)height;
// 平均的颜色
- (UIColor *)averageColor;
// 裁剪图片的一部分
- (UIImage *)croppedImageAtFrame:(CGRect)frame;
// 将自身填充到指定的size
- (UIImage *)fillClipSize:(CGSize)size;
```
### UIView+LXExtension
1.快速设置控件frame
2.快速根据xib生成View
3.判断两个view是否重叠
```
/**
 *  快速设置控件的位置
 */
@property (nonatomic, assign) CGSize lx_size;
@property (nonatomic, assign) CGFloat lx_width;
@property (nonatomic, assign) CGFloat lx_height;
@property (nonatomic, assign) CGFloat lx_x;
@property (nonatomic, assign) CGFloat lx_y;
@property (nonatomic, assign) CGFloat lx_centerX;
@property (nonatomic, assign) CGFloat lx_centerY;
/**
 *  快速根据xib创建View
 */
+ (instancetype)lx_viewFromXib;
/**
 *  判断self和view是否重叠
 */
- (BOOL)lx_intersectsWithView:(UIView *)view;
```
### UITextField+LXExtension
1.设置textField的占位文字颜色
```
/** 占位文字颜色 */
@property (nonatomic, strong) UIColor *lx_placeholderColor;
```
### UIBarButtonItem+LXExtension
1.快速自定义导航栏上的按钮
`+ (instancetype)lx_itemWithImage:(NSString *)image highImage:(NSString *)highImage target:(id)target action:(SEL)action;`
### NSDate+LXExtension
1.两个时间之间的时间间隔
2.是否为今天，昨天，明天
3.当前是周几
```
@interface LXDateItem : NSObject
@property (nonatomic, assign) NSInteger day;
@property (nonatomic, assign) NSInteger hour;
@property (nonatomic, assign) NSInteger minute;
@property (nonatomic, assign) NSInteger second;
@end
@interface NSDate (LXExtension)
- (LXDateItem *)xmg_timeIntervalSinceDate:(NSDate *)anotherDate;
- (BOOL)lx_isToday;
- (BOOL)lx_isYesterday;
- (BOOL)lx_isTomorrow;
- (BOOL)lx_isThisYear;
//获取今天周几
- (NSInteger)getNowWeekday;
@end
```
### NSDictionary+PropertyCode
1.根据字典快速生成Property属性
使用场景：根据网络请求返回的字典数据，写对应的模型。当属性多时，用手写很费功夫，可用这个类快速打印出所有的模型属性，直接粘贴即可
```
// 生成所需要的属性代码
- (void)propertyCode;
```
### NSObject+JSON
1.字典或对象转成JSON字符串数据
```
/**
 *  字典或对象转成JSON字符串数据
 */
@property (nonatomic, copy, readonly) NSString *JSONString;
```
## 工具类
### Single：快速创建一个单例
```
#ifndef Single_h
#define Single_h
#ifdef __OBJC__
#pragma mark - 单例模式 .h文件内容
#define SingleInterface(name) +(instancetype)share##name;
#pragma mark - 单例模式 .m文件内容
#if __has_feature(objc_arc)
#define SingleImplementation(name) +(instancetype)share##name {return [[self alloc]init];} \
+ (instancetype)allocWithZone:(struct _NSZone *)zone { \
static id instance; \
static dispatch_once_t onceToken; \
dispatch_once(&onceToken, ^{ \
instance = [super allocWithZone:zone]; \
}); \
return instance; \
} \
- (id)copyWithZone:(NSZone *)zone{return self;} \
- (id)mutableCopyWithZone:(NSZone *)zone {return self;}
#else
#define SingleImplementation(name) +(instancetype)share##name {return [[self alloc]init];} \
+ (instancetype)allocWithZone:(struct _NSZone *)zone { \
static id instance; \
static dispatch_once_t onceToken; \
dispatch_once(&onceToken, ^{ \
instance = [super allocWithZone:zone]; \
}); \
return instance; \
} \
- (id)copyWithZone:(NSZone *)zone{return self;} \
- (id)mutableCopyWithZone:(NSZone *)zone {return self;} \
- (instancetype)retain {return self;} \
- (instancetype)autorelease {return self;} \
- (oneway void)release {} \
- (NSUInteger)retainCount {return MAXFLOAT;} \
#endif
#endif
#endif /* Single_h */
```
### HelperUtil：常用的正则表达式
```
```
#pragma 正则匹配邮箱号
+ (BOOL)checkMailInput:(NSString *)mail;
#pragma 正则匹配手机号
+ (BOOL)checkTelNumber:(NSString *) telNumber;
#pragma 正则匹配用户密码6-18位数字和字母组合
+ (BOOL)checkPassword:(NSString *) password;
#pragma 正则匹配用户姓名,20位的中文或英文
+ (BOOL)checkUserName : (NSString *) userName;
#pragma 正则匹配用户身份证号
+ (BOOL)checkUserIdCard: (NSString *) idCard;
#pragma 正则匹员工号,12位的数字
+ (BOOL)checkEmployeeNumber : (NSString *) number;
#pragma 正则匹配URL
+ (BOOL)checkURL : (NSString *) url;
#pragma 正则匹配昵称
+ (BOOL) checkNickname:(NSString *) nickname;
#pragma 正则匹配以C开头的18位字符
+ (BOOL) checkCtooNumberTo18:(NSString *) nickNumber;
#pragma 正则匹配以C开头字符
+ (BOOL) checkCtooNumber:(NSString *) nickNumber;
#pragma 正则匹配银行卡号是否正确
+ (BOOL) checkBankNumber:(NSString *) bankNumber;
#pragma 正则匹配17位车架号
+ (BOOL) checkCheJiaNumber:(NSString *) CheJiaNumber;
#pragma 正则只能输入数字和字母
+ (BOOL) checkTeshuZifuNumber:(NSString *) CheJiaNumber;
#pragma 车牌号验证
+ (BOOL) checkCarNumber:(NSString *) CarNumber;
```
```
### LXFileManagerp:将对象取归档到沙盒，读取存储删除沙盒及偏好设置的数据
```
/// 文件管理类
@interface LXFileManager : NSObject
/// 把对象归档存到沙盒里
+(void)saveObject:(id)object byFileName:(NSString*)fileName;
/// 通过文件名从沙盒中找到归档的对象
+(id)getObjectByFileName:(NSString*)fileName;
/// 根据文件名删除沙盒中的 plist 文件
+(void)removeFileByFileName:(NSString*)fileName;
/// 存储用户偏好设置 到 NSUserDefults
+(void)saveUserData:(id)data forKey:(NSString*)key;
/// 读取用户偏好设置
+(id)readUserDataForKey:(NSString*)key;
/// 删除用户偏好设置
+(void)removeUserDataForkey:(NSString*)key;
```
### LXMacro：便利宏，便捷定义@property属性
```
/** copy */
//NSString
#define String_(name) \
    zzn_copy_property(NSString*,name)
//NSArray
#define Array_(name) \
    zzn_copy_property(NSArray*,name)
//NSDictionary
#define Dictionary_(name) \
    zzn_copy_property(NSDictionary*,name)
//NSNumber,它没用对应的不可变类，其实用copy或strong没有区别
#define Number_(name) \
    zzn_copy_property(NSNumber*,name)
//NSData
#define Data_(name) \
    zzn_copy_property(NSData*,name)
//NSSet
#define Set_(name) \
    zzn_copy_property(NSSet*,name)
//NSIndexSet
#define IndexSet_(name) \
    zzn_copy_property(NSIndexSet*,name)
//代码块，名称和传参，没有传参就不填
#define Block_(name,...) \
    zzn_set_block(void,name,__VA_ARGS__)
//有返回值的代码块
#define BlockReturn_(name,returnClass,...) \
    zzn_set_block(returnClass,name,__VA_ARGS__)
………………………………
```
### LocalPushCenter：快速使用本地通知
```
+ (NSDate *)fireDateWithWeek:(NSInteger)week
                        hour:(NSInteger)hour
                      minute:(NSInteger)minute
                      second:(NSInteger)second;
//本地发送推送（先取消上一个 再push现在的）
+ (void)localPushForDate:(NSDate *)fireDate
                  forKey:(NSString *)key
               alertBody:(NSString *)alertBody
             alertAction:(NSString *)alertAction
               soundName:(NSString *)soundName
             launchImage:(NSString *)launchImage
                userInfo:(NSDictionary *)userInfo
              badgeCount:(NSUInteger)badgeCount
          repeatInterval:(NSCalendarUnit)repeatInterval;
#pragma mark - 退出
+ (void)cancelAllLocalPhsh;
+ (void)cancleLocalPushWithKey:(NSString *)key;
```
文／xiao公子（简书作者）
原文链接：http://www.jianshu.com/p/3e476777c00b
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
