# iOS中字符串的用法,看这里就够了(详细) - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年05月30日 11:04:55[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：3812
### 这篇文章主要详细介绍iOS中字符串的各种用法,如果发现不够完善可以提交意见,主要是方便对字符串的使用的理解 内容比较多但是详细,如果觉得以后用得着的地方可以先收藏着;
#### NSString的基本概念 (NSString是不可变字符串)
什么是NSString?
> 
一个NSString代表一个字符串(文字内容)
一般称NSString为字符串类
NSString的创建方式 (两种)
```
- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
//    [self create1];
    [self create2];
}
/** 第一种创建方式 */
//常量区中的字符串只要内容一致, 不会重复创建
- (void)create1
{
    NSString *str1 = @"lion";
    NSString *str2 = @"lion";
    //输出地址不一样
    NSLog(@"\n %p \n %p",&str1,&str2);
}
/** 第二种创建方式 */
//堆区中得字符串哪怕内容一致, 也会重复创建
- (void)create2
{
//stringWithFormat 可以进行拼接
    NSString *str1 = [NSString stringWithFormat:@"lion"];
    NSString *str2 = [NSString stringWithFormat:@"lion"];
    //输出地址相同
    NSLog(@"\n %p \n %p",str1,str2);
}
```
#### NSString的读写
读取:
```
// 用来保存错误信息
NSError *error = nil;
// 读取文件内容
NSString *str = [NSString stringWithContentsOfFile:@"/Users/LJH/Desktop/Lion.txt" encoding:NSUTF8StringEncoding error:&error];
// 如果有错误信息
if (error) {
    NSLog(@"读取失败, 错误原因是:%@", [error localizedDescription]);
} else { // 如果没有错误信息
    NSLog(@"读取成功, 文件内容是:\n%@", str);
}
```
写入:
```
// writeToFile 写入
NSString *str = @"Lion";
BOOL flag = [str writeToFile:@"/Users/LJH/Desktop/Lion.txt" atomically:YES encoding:NSUTF8StringEncoding error:nil];
if (flag == 1)
{
    NSLog(@"写入成功");
}
//注意: 重复写入同一个文件会覆盖上一个文件
```
#### 字符串的比较
NSString的大小写属性
```
全部字符转为大写字母
- (NSString *)uppercaseString;
全部字符转为小写字母
- (NSString *)lowercaseString
首字母变大写，其他字母都变小写
- (NSString *)capitalizedString
```
NSString的比较
one:
```
- (BOOL)isEqualToString:(NSString *)aString;
两个字符串的内容相同就返回YES, 否则返回NO
    NSString *str1 = @"lion";
    NSString *str2 = [NSString stringWithFormat:@"lion"];
    if ([str1 isEqualToString:str2]) {
        NSLog(@"字符串内容一样");
    }
    if (str1 == str2) {
        NSLog(@"字符串地址一样");
    }
```
two:
```
- (NSComparisonResult)compare:(NSString *)string;
这个方法可以用来比较两个字符串内容的大小
比较方法: 逐个字符地进行比较ASCII值，返回NSComparisonResult作为比较结果
NSComparisonResult是一个枚举，有3个值:
如果左侧 > 右侧,返回NSOrderedDescending,
如果左侧 < 右侧,返回NSOrderedAscending,
如果左侧 == 右侧返回NSOrderedSame
    NSString *str1 = @"abc";
    NSString *str2 = @"abd";
    switch ([str1 compare:str2]) {
        case NSOrderedAscending:
            NSLog(@"后面一个字符串大于前面一个");
            break;
        case NSOrderedDescending:
            NSLog(@"后面一个字符串小于前面一个");
            break;
        case NSOrderedSame:
            NSLog(@"两个字符串一样");
            break;
    }
    输出结果: 后面一个字符串大于前面一个
```
three:
```
```
- (NSComparisonResult) caseInsensitiveCompare:(NSString *)string;
忽略大小写进行比较，返回值与compare:一致
```
```
#### 字符串搜索
```
```
- (BOOL)hasPrefix:(NSString *)aString;
是否以aString开头
- (BOOL)hasSuffix:(NSString *)aString;
是否以aString结尾
- (NSRange)rangeOfString:(NSString *)aString;
用来检查字符串内容中是否包含了aString
如果包含, 就返回aString的范围
如果不包含, NSRange的location为NSNotFound, length为0
```
```
NSRange基本概念
```
NSRange是Foundation框架中比较常用的结构体, 它的定义如下:
typedef struct _NSRange {
    NSUInteger location;
    NSUInteger length;
} NSRange;
// NSUInteger的定义
typedef unsigned int NSUInteger;
NSRange用来表示事物的一个范围,通常是字符串里的字符范围或者数组里的元素范围
NSRange有2个成员
NSUInteger location : 表示该范围的起始位置
NSUInteger length : 表示该范围内的长度
比如@“I Miss You Lion”中的@“Lion”可以用location为11，length为4的范围来表示
```
NSRange的创建
```cpp
```cpp
有3种方式创建一个NSRange变量
方式1
NSRange range;
range.location = 11;
range.length = 4;
方式2
NSRange range = {11, 4};
或者
NSRange range = {.location = 11,.length = 4};
方式3 : 使用NSMakeRange函数
NSRange range = NSMakeRange(11, 4);
```
```
#### 字符串的截取
```
one:
- (NSString *)substringFromIndex:(NSUInteger)from;
从指定位置from开始(包括指定位置的字符)到尾部
- (void)subFrom
{
    //字符从0开始算
    NSString *str = @"<head>会跳舞的狮子</head>";
    str = [str substringFromIndex:6];
    NSLog(@"str = %@", str);
    //打印结果 str = 会跳舞的狮子</head>
}
two:
- (NSString *)substringToIndex:(NSUInteger)to;
从字符串的开头一直截取到指定的位置to，但不包括该位置的字符
- (void)subTo
{
    NSString *str = @"<head>会跳舞的狮子</head>";
    str = [str substringToIndex:12];
    NSLog(@"str = %@", str);
    //打印结果 str = <head>会跳舞的狮子
}
three:
- (NSString *)substringWithRange:(NSRange)range;
按照所给出的NSRange从字符串中截取子串
- (void)subWithRange
{
    NSString *str = @"<head>会跳舞的狮子</head>";
    NSRange range;
    /*
     range.location = 6;
     range.length = 6;
     */
    range.location = [str rangeOfString:@">"].location + 1;
    range.length = [str rangeOfString:@"</"].location - range.location;
    NSString *result = [str substringWithRange:range];
    NSLog(@"result = %@", result);
    //打印结果   会跳舞的狮子
}
```
#### 字符串的替换
```
- (NSString *)stringByReplacingOccurrencesOfString:(NSString *)target withString:(NSString *)replacement;
用replacement替换target
- (void)Replacing
{
    NSString *str = @"http:**www.baidu.com*lion.png";
    NSString *newStr = [str stringByReplacingOccurrencesOfString:@"*" withString:@"/"];
    NSLog(@"newStr = %@", newStr);
}
- (NSString *)stringByTrimmingCharactersInSet:(NSCharacterSet *)set;
去除首尾
- (void)Trimming
{
    NSString *str =  @"   http://baidu.com/lion.png   ";
    //去除首尾的空格
    NSString *newStr = [str stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
    NSLog(@"str =|%@|", str);
    NSLog(@"newStr =|%@|", newStr);
    /*
     打印结果
     str =|   http://baidu.com/lion.png   |
     newStr =|http://baidu.com/lion.png|
     */
}
```
#### 字符串与路径
NSString 与 路径
```
one:
- (BOOL)isAbsolutePath;
是否为绝对路径
- (void)isAbsolutePath
{
    // 其实就是判断是否以/开头
    NSString *str = @"/Users/LJH/Desktop/Lion.txt";
    NSString *str = @"Users/LJH/Desktop/Lion.txt";
    if ([str isAbsolutePath]) {
        NSLog(@"是绝对路径");
    }else
    {
        NSLog(@"不是绝对路径");
    }
}
two:
- (NSString *)lastPathComponent;
获得最后一个目录
- (void)lastPathComponent
{
    // 截取最后一个/后面的内容
    NSString *str = @"/Users/LJH/Desktop/Lion.txt";
    NSString *component = [str lastPathComponent];
    NSLog(@"component = %@", component);
}
three:
- (NSString *)stringByDeletingLastPathComponent;
删除最后一个目录
- (void)stringByDeletingLastPathComponent
{
    // 其实就是上次最后一个/和之后的内容
    NSString *str = @"/Users/LJH/Desktop/Lion.txt";
    NSString *newStr = [str stringByDeletingLastPathComponent];
    NSLog(@"newStr = %@", newStr);
    //打印结果:newStr = /Users/LJH/Desktop
}
four:
- (NSString *)stringByAppendingPathComponent:(NSString *)str;在路径的后面拼接一个目录 (也可以使用stringByAppendingString:或者stringByAppendingFormat:拼接字符串内容)
- (void)stringByAppendingPathComponent
{
    // 其实就是在最后面加上/和要拼接得内容
    // 注意会判断后面有没有/有就不添加了, 没有就添加, 并且如果有多个会替换为1个
    //    NSString *str = @"/Users/NJ-Lee/Desktop";
    NSString *str = @"/Users/LJH/Desktop/";
    NSString *newStr = [str stringByAppendingPathComponent:@"Lion"];
    NSLog(@"newStr = %@", newStr);
    //打印结果:newStr = /Users/LJH/Desktop/Lion
}
```
NSString 与 文件拓展名
```
- (void)pathExtension
{
one:
- (NSString *)pathExtension;
获得拓展名
    // 其实就是从最后面开始截取.之后的内容
    //    NSString *str = @"Lion.txt";
    NSString *str = @"abc.Lion.txt";
    NSString *extension = [str pathExtension];
    NSLog(@"extension = %@", extension);
  //打印结果:extension = txt
}
two:
- (NSString *)stringByDeletingPathExtension;
删除尾部的拓展名
- (void)stringByDeletingPathExtension
{
    // 其实就是上次从最后面开始.之后的内容
    NSString *str = @"love.Lion.txt";
    NSString *newStr = [str stringByDeletingPathExtension];
    NSLog(@"newStr = %@", newStr);
    //打印结果 newStr = love.Lion
}
three:
- (NSString *)stringByAppendingPathExtension:(NSString *)str;
在尾部添加一个拓展名
- (void)stringByAppendingPathExtension
{
    // 其实就是在最后面拼接上.和指定的内容
    NSString *str = @"Lion";
    NSString *newStr = [str stringByAppendingPathExtension:@"png"];
    NSLog(@"newStr = %@", newStr);
    //打印结果:  newStr = Lion.png
}
```
#### 字符串与基本数据类型的转换
```
- (NSUInteger)length;
返回字符串的长度(有多少个文字)
- (unichar)characterAtIndex:(NSUInteger)index;
返回index位置对应的字符
转为基本数据类型
- (double)doubleValue;
- (float)floatValue;
- (int)intValue;
    NSString *str1 = @"22";
    NSString *str2 = @"22";
    int res = str1.intValue + str2.intValue;
    NSLog(@"res = %i", res);
    NSString *str1 = @"22";
    NSString *str2 = @"22.2";
    double res = str1.doubleValue + str2.doubleValue;
    NSLog(@"res = %f", res);
转为C语言中的字符串
- (char *)UTF8String;
    NSString *str = @"love";
    const char *cStr = [str UTF8String];
    NSLog(@"cStr = %s", cStr);
    char *cStr = "love";
    NSString *str = [NSString stringWithUTF8String:cStr];
    NSLog(@"str = %@", str);
```
# NSMutableString的用法 (NSMutableString是可变字符串)
#### NSMutableString的基本概念
> - NSMutableString 类 继承NSString类,那么NSString 
