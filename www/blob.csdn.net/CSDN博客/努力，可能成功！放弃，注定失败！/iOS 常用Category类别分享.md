# iOS 常用Category类别分享 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年07月29日 20:34:31[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：3664
# iOS 常用Category类别分享
字数1340
阅读86评论2喜欢8
# 前言
     各位都知道，类别是一种为现有的类添加新方法的方式，利用Objective-C的动态运行时分配机制，可以为现有的类添加新方法，这种为现有的类添加新方法的方式称为类别catagory，他可以为任何类添加新的方法，包括那些没有源代码的类。类别使得无需创建对象类的子类就能完成同样的工作。
     有许多小技术点靠类别直接实现，从而节约时间将精力花在更重要的开发任务上，现在我来分享一下我从事iOS开发以来积累的Category！
# 第一部分 NSString、NSAttributedString
### NSString包括：
![](http://upload-images.jianshu.io/upload_images/2153139-2acd6a077f6f6fda.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
1.png
## 1.NSString+Category：
一些NSString常规内容
```
//电话号码中间4位****显示
+ (NSString*) getSecrectStringWithPhoneNumber:(NSString*)phoneNum;
//银行卡号中间8位显示
+ (NSString*) getSecrectStringWithAccountNo:(NSString*)accountNo;
//计算文字高度
- (CGFloat   ) heightWithFontSize:(CGFloat)fontSize width:(CGFloat)width;
/**抹除运费小数末尾的0*/
- (NSString *) removeUnwantedZero;
//去掉前后空格
- (NSString *) trimmedString;
//Data类型转换为Base64
+ (NSString *)base64StringFromData:(NSData *)data length:(NSUInteger)length;
//电话格式
+ (NSString*) stringMobileFormat:(NSString*)mobile;
//数组中文格式（几万）可自行添加
+ (NSString*) stringChineseFormat:(double)value;
```
## 2.NSString+Predicate：
常用的正则表达式判断
```
//有效的电话号码
- (BOOL) isValidMobileNumber;
//有效的真实姓名
- (BOOL) isValidRealName;
//是否只有中文
- (BOOL) isOnlyChinese;
//有效的验证码(根据自家的验证码位数进行修改)
- (BOOL) isValidVerifyCode;
//有效的银行卡号
- (BOOL) isValidBankCardNumber;
//有效的邮箱
- (BOOL) isValidEmail;
//有效的字母数字密码
- (BOOL) isValidAlphaNumberPassword;
//检测有效身份证
//15位
- (BOOL) isValidIdentifyFifteen;
//18位
- (BOOL) isValidIdentifyEighteen;
//限制只能输入数字
- (BOOL) isOnlyNumber;
```
## 3.NSString+DisplayTime：
从时间戳转为显示时间
```
//通过时间戳计算时间差（几小时前、几天前）
+ (NSString *) compareCurrentTime:(NSTimeInterval) compareDate;
//通过时间戳得出显示时间
+ (NSString *) getDateStringWithTimestamp:(NSTimeInterval)timestamp;
//通过时间戳和格式显示时间
+ (NSString *) getStringWithTimestamp:(NSTimeInterval)timestamp formatter:(NSString*)formatter;
```
### NSAttributedString包括：
![](http://upload-images.jianshu.io/upload_images/2153139-ec4a8dd9d63bfb76.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
2.png
富文本高度、文字与图片混合的富文本
```
//由于系统计算富文本的高度不正确，自己写了方法
- (CGFloat)heightWithContainWidth:(CGFloat)width;
//直接返回NSAttributedString
+ (NSAttributedString *)attributeStringWithPrefixString:(NSString *)prefixString
                                           suffixString:(NSString *)suffixString;
/**
 *  直接返回NSAttributedString
 *
 *  @param prefixString 前面的string
 *  @param prefixFont   字体大小
 *  @param prefixColor  字体颜色
 *  @param suffixString 后面拼接的string
 *  @param suffixFont   字体大小
 *  @param suffixColor  字体颜色
 *
 *  @return 直接返回NSAttributedString
 */
+ (NSAttributedString *)attributeStringWithPrefixString:(NSString *)prefixString
                                             prefixFont:(CGFloat)prefixFont
                                            prefixColor:(UInt32)prefixColor
                                           suffixString:(NSString *)suffixString
                                             suffixFont:(CGFloat)suffixFont
                                            suffixColor:(UInt32)suffixColor;
// string在前 图片在后
+ (NSMutableAttributedString *)attributeStringWithPrefixString:(NSString *)prefixString
                                              subffixImageName:(NSString *)subffixImageName;
//图片在前 string在后
+ (NSMutableAttributedString *)attributeStringWithSubffixString:(NSString *)subffixString
                                                prefixImageName:(NSString *)prefixImageName;
```
# 第二部分 NSArray,NSDictionary
这一块内容主要是防止nil而发生闪退的情况,可变和不可变的各种遇到nil而闪退的情况都有实现
### NSArray、NSMutableArray
```
@interface NSArray (Category)
//以下写法均防止闪退
+ (instancetype)safeArrayWithObject:(id)object;
- (id)safeObjectAtIndex:(NSUInteger)index;
- (NSArray *)safeSubarrayWithRange:(NSRange)range;
- (NSUInteger)safeIndexOfObject:(id)anObject;
//通过Plist名取到Plist文件中的数组
+ (NSArray *)arrayNamed:(NSString *)name;
// 数组转成json 字符串
- (NSString *)toJSONStringForArray;
@end
@interface NSMutableArray (Category)
//以下写法均防止闪退
- (void)safeAddObject:(id)object;
- (void)safeInsertObject:(id)object atIndex:(NSUInteger)index;
- (void)safeInsertObjects:(NSArray *)objects atIndexes:(NSIndexSet *)indexs;
- (void)safeRemoveObjectAtIndex:(NSUInteger)index;
- (void)safeRemoveObjectsInRange:(NSRange)range;
@end
```
### NSDictionary、NSMutableDictionary
```
@interface NSDictionary (Category)
//用于数据解析，返回对象为字符串或值类型，数组和字典不要用此方法
- (id)safeObjectForKey:(NSString *)key;
//设置键值对 针对对象为空处理
- (void)safeSetObject:(id)object forKey:(id)key;
- (id)objectForKeyCustom:(id)aKey;
- (id)safeKeyForValue:(id)value;
/**
 *  字段转成json的字符串
 *
 *  @return json 字符串
 */
- (NSString *)toJSONStringForDictionary;
@end
@interface NSMutableDictionary (Category)
- (void)safeSetObject:(id)aObj forKey:(id<NSCopying>)aKey;
- (id)safeObjectForKey:(id<NSCopying>)aKey;
@end
```
# 第三部分 UIView、UIImage、UIButton
### UIView
```
//UIMotionEffect和Home页背景视差效果
- (void) addCenterMotionEffectsXYWithOffset:(CGFloat)offset;
//把View加在Window上
- (void) addToWindow;
//View截图
- (UIImage*) screenshot;
//ScrollView截图 contentOffset
- (UIImage*) screenshotForScrollViewWithContentOffset:(CGPoint)contentOffset;
//View按Rect截图
- (UIImage*) screenshotInFrame:(CGRect)frame;
```
### UIImage,这里内容很多我只列出一部分，具体的大家可以down或者clone下来之后仔细看
```
//由颜色生成图片
+ (UIImage *) imageWithColor:(UIColor*)color;
//将图片剪裁至目标尺寸
+ (UIImage *) imageByScalingAndCroppingForSourceImage:(UIImage *)sourceImage targetSize:(CGSize)targetSize;
//图片旋转角度
- (UIImage *) imageRotatedByDegrees:(CGFloat)degrees;
//拉伸图片UIEdgeInsets
- (UIImage *) resizableImage:(UIEdgeInsets)insets;
//拉伸图片CGFloat
- (UIImage *) imageByResizeToScale:(CGFloat)scale;
//放大图片CGSize
- (UIImage *) imageByResizeWithMaxSize:(CGSize)size;
//小样图图片CGSize
- (UIImage *) imageWithThumbnailForSize:(CGSize)size;
//通过Rect剪裁图片
- (UIImage *) imageByCropToRect:(CGRect)rect;
```
### UIButton,扩大点击范围，非常有用的一个方法，大力推荐
`- (void)setEnlargeEdgeWithTop:(CGFloat)top right:(CGFloat)right bottom:(CGFloat)bottom left:(CGFloat)left;`
# 总结
     这边把一些常用的类别给大家列举了一些，其实在GitHub上的项目里还有很多其他类别，大家可以仔细研究下，毕竟App有很多功能是共同，基本每个方法都是我因为某些需求写的、还有部分是网上找的。如果不会使用可以来询问我，第一次写分享的文章，不喜勿喷。谢谢
     以后我还会持续更新的！最后放出传送门。。麻烦下载的同学给点个星或者点个赞 ~(≧▽≦)/~ 谢谢啦！
# 下载地址
[Git下载地址](https://github.com/kazamihayato/iOS-Category)
