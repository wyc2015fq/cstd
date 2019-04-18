# iOS 开发之NSDecimalNumber 的简单应用 - weixin_33985507的博客 - CSDN博客
2017年08月30日 14:57:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：17
## NSDecimalNumber
NSDecimalNumber,不可变NSNumber 的子类,提供了一个面向对象的十进制运算包装器 。实例可以代表任意数值,表示为(mantissa)尾数x 10 ^(exponent)指数,(mantissa)尾数是一个十进制整数到38位长,(exponent)指数是一个整数从-128年到127。
![1623468-d128e5d3df530241.png](https://upload-images.jianshu.io/upload_images/1623468-d128e5d3df530241.png)
实例化方法如下，mantissa：长整形；exponent：指数；flag：正负数。
```
+ (NSDecimalNumber *)decimalNumberWithMantissa:(unsigned long long)mantissa exponent:(short)exponent isNegative:(BOOL)flag;
```
###### Example:
参数表示 一个number的科学记数法 ,要求(mantisa)尾数是一个整数。例如,如果表示的数是-15.99,它是表示为1599 x10 ^ 2   -mantissa 是12345;(exponent)指数是2;和isNegative(一个布尔值,用于指定number 的(sign)符号是负的)是YES
```
NSDecimalNumber *number = [NSDecimalNumber decimalNumberWithMantissa:1599
                                           exponent:-2
                                           isNegative:YES];
```
```
+ (NSDecimalNumber *)decimalNumberWithDecimal:(NSDecimal)decimal
```
给定一个NSDecimal[数据结构](https://link.jianshu.com?t=http://lib.csdn.net/base/datastructure),创建并返回一个等价的 NSDecimalNumber对象
```
+ (NSDecimalNumber *)decimalNumberWithString:(NSString *)numericString
```
创建并返回一个NSDecimalNumber对象值相当于个给定的数字字符串。
numericString:除了数字字符串,numericString可以包括一个“+”或“-”,一个“E”或“E”,用科学记数法表示一个数的指数,和一个NSLocaleDecimalSeparator划分的部分数的有效组成部分。
```
+ (NSDecimalNumber *)decimalNumberWithString:(NSString *)numericString  locale:(id)locale
```
locale:一个字典定义语言环境(特别是NSLocaleDecimalSeparator),在进行解析numericString的时候
```
NSDictionary * locale = @{NSLocaleDecimalSeparator:@","];    //以","当做分隔符格式
NSDecimalNumber * discountAmount = [NSDecimalNumber decimalNumberWithString:@"123,40" locale:locale];    //123.4
```
以下是numericString的一些正确示例:
```
“2500.6” (or “2500,6”, 取决于 locale)
“–2500.6” (or “–2500.6”)
“–2.5006e3” (or “–2,5006e3”)
“–2.5006E3” (or “–2,5006E3”)
```
以下是numericString的一些错误示例:
```
“2,500.6”
“2500 3/5”
“2.5006x10e3”
“two thousand five hundred and six tenths”
```
一个 等同于 1.0的NSDecimalNumber对象
```
+ (NSDecimalNumber *)one
```
一个 等同于 0.0的NSDecimalNumber对象
```
+ (NSDecimalNumber *)zero
```
没有指定数值的NSDecimalNumber对象
常用于对比，比如：[[NSDecimalNumber notANumber] isEqualToNumber:myNumber];
```
+ (NSDecimalNumber *)notANumber
```
###### 初始化Decimal Number
一个NSDecimalNumber使用给定decimal对象初始化
```
- (instancetype)initWithDecimal:(NSDecimal)decimal
```
使用(mantissa)尾数, (exponent)指数 number 的(sign)符号,来初始化NSDecimalNumber
```
- (instancetype)initWithMantissa:(unsigned long long)mantissa  exponent:(short)exponent  isNegative:(BOOL)flag
```
创建并返回一个NSDecimalNumber对象值相当于个给定的numericString数字字符串。
```
- (instancetype)initWithString:(NSString *)numericString
```
以numericString,locale初始化NSDecimalNumber对象
locale:一个字典定义语言环境(特别是NSLocaleDecimalSeparator),在进行解析numericString的时候
```
- (instancetype)initWithString:(NSString *)numericString  locale:(id)locale
```
###### 加法运算
```
-(NSDecimalNumber *)decimalNumberByAdding:(NSDecimalNumber *)decimalNumber;
-(NSDecimalNumber *)decimalNumberByAdding:(NSDecimalNumber *)decimalNumber withBehavior:(nullable id <NSDecimalNumberBehaviors>)behavior;
```
###### 减法运算
```
-(NSDecimalNumber *)decimalNumberBySubtracting:(NSDecimalNumber *)decimalNumber;
- (NSDecimalNumber *)decimalNumberBySubtracting:(NSDecimalNumber *)decimalNumber withBehavior:(id<NSDecimalNumberBehaviors>)behavior
```
###### 乘法运算
```
-(NSDecimalNumber *)decimalNumberByMultiplyingBy:(NSDecimalNumber *)decimalNumber;
-(NSDecimalNumber *)decimalNumberByMultiplyingBy:(NSDecimalNumber *)decimalNumber withBehavior:(nullable id <NSDecimalNumberBehaviors>)behavior;
```
###### 除法运算
```
-(NSDecimalNumber *)decimalNumberByDividingBy:(NSDecimalNumber *)decimalNumber;
-(NSDecimalNumber *)decimalNumberByDividingBy:(NSDecimalNumber *)decimalNumber withBehavior:(nullable id <NSDecimalNumberBehaviors>)behavior;
```
###### 幂运算
```
-(NSDecimalNumber *)decimalNumberByRaisingToPower:(NSUInteger)power;
-(NSDecimalNumber *)decimalNumberByRaisingToPower:(NSUInteger)power withBehavior:(nullable id <NSDecimalNumberBehaviors>)behavior;
```
###### 指数运算
```
-(NSDecimalNumber *)decimalNumberByMultiplyingByPowerOf10:(short)power;
-(NSDecimalNumber *)decimalNumberByMultiplyingByPowerOf10:(short)power 
withBehavior:(nullable id <NSDecimalNumberBehaviors>)behavior;
```
###### 四舍五入运算
```
-(NSDecimalNumber *)decimalNumberByRoundingAccordingToBehavior:(nullable id <NSDecimalNumberBehaviors>)behavior;
```
###### 比较大小
```
-(NSComparisonResult)compare:(NSNumber *)decimalNumber;
简单应用
NSDecimalNumber *discount1 = [NSDecimalNumber decimalNumberWithString:@"2.1"];
  NSDecimalNumber *discount2 = [NSDecimalNumber decimalNumberWithString:@"2.2"];
  NSComparisonResult result = [discount1 compare:discount2];
  if (result == NSOrderedAscending) {
      NSLog(@"discount1 <discount2");
  } else if (result == NSOrderedSame) {
      NSLog(@"discount1 == discount2");
  } else if (result == NSOrderedDescending) {
      NSLog(@"discount1 > discount2");
  }
```
###### 特别鸣谢
[如何使用NSDecimalNumber？](https://link.jianshu.com?t=https://gxnotes.com/article/101442.html)
[‘NSDecimalNumber–十进制数’使用方法](https://www.jianshu.com/p/4703d704c953)
[OC : NSDecimalNumber](https://link.jianshu.com?t=http://blog.csdn.net/wiki_su/article/details/77648853)
