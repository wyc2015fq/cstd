# OC字符串的常用方法 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年03月07日 10:17:14[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：329

     网上写的关于字符串常用方法的博客很多，这里我简单做了下总结！不喜勿喷哦！
一.创建字符串
#import <Foundation/Foundation.h>
//NSString
//创建不可变字符串对象的类
//NSMutableString
//创建可变字符串对象的类
//OC语言完全兼容C语言
//OC字符串与C语言字符串区别
//1.OC字符串是一个字符串对象,字符串常量需要用@""包含
//2.C语言字符串用""包含
//3.C语言的字符串以字符的ASCII码形式存储
//4.OC中的字符串以uicode编码(万国码)形式存储
//UTF-8(多字节编码)
//5.打印OC字符串用%@,打印C语言字符串用%s

int main(int argc,constchar * argv[]) {
@autoreleasepool {
        NSString *str1 =@"hello world";//@"hello world"是一个常量字符串对象,存储常量区,不可以被修改
        NSLog(@"str1 = %@", str1);
//格式化创建字符串对象
//- (instancetype)initWithFormat:(NSString *)format, ...
        NSString *str2 = [[NSString alloc] initWithFormat:@"%s%d%@","hello",123,
@"world" ];
        NSLog(@"str2 = %@", str2);
//用格式化的类方法创建字符串对象
//+ (instancetype)stringWithFormat:(NSString *)format, ...
        NSString *str3 = [NSString stringWithFormat:@"%s%d%@","qiafdn",456,@"ffds"];
        NSLog(@"str3 = %@", str3);
//用给定的字符串对象创建字符串对象
        NSString *str4=@"中国教育";
        NSString *str5 = [[NSString alloc] initWithString:str4];
        NSLog(@"str5 = %@", str5);
//用C的字符串创建OC的字符串对象
        NSString *str6 = [[NSString alloc] initWithUTF8String:"qifdfdg中国jiaoyu"];
        NSLog(@"str6 = %@", str6);
        NSString *str7 = [[NSString alloc] initWithCString:"我的qifdfg" encoding:NSUTF8StringEncoding];
        NSLog(@"str7 = %@", str7);
//创建一个空的字符串对象 @""
        NSString *str8 = [NSString string];
        NSLog(@"str8 = %@", str8);
        NSString *str9 = [[NSString alloc] init];
        NSLog(@"str9 = %@", str9);
//跟initWithString相对应
        NSString *str10 = [NSString stringWithString:str7];
        NSLog(@"str10 = %@", str10);
//跟initWithUTF8String相对应
        NSString *str11 = [NSString stringWithUTF8String:"hello world中国"];
        NSLog(@"str11 = %@", str11);
//跟initWithCString相对应
        NSString *str12 = [NSString stringWithCString:"zhongguo" encoding:NSUTF8StringEncoding];
        NSLog(@"str12 = %@", str12);
    }
return0;
}
二.NSString的常用方法
#import <Foundation/Foundation.h>
//NSString
int main(int argc,constchar * argv[]) {
@autoreleasepool {
        NSString *str1 =@"hello world
中国";
//求字符串长度
        NSUInteger len = [str1 length];
        NSLog(@"len = %li", len);
//获取字符串指定位置的字符
        unichar ch = [str1 characterAtIndex:13];
        NSLog(@"ch = %C", ch);//%C打印unichar字符
 %c打印ASCII字符
//字符串提取
//从传入下标位置提取子串到字符串结束
        NSString *subStr1 = [str1 substringFromIndex:4];
        NSLog(@"subStr1 = %@", subStr1);
//提取子串到指定位置(不包含下标位置字符)
        NSString *subStr2 = [str1 substringToIndex:7];
        NSLog(@"subStr2 = %@",subStr2);
//提取指定范围内的字符串
        NSRange range = {6,5};
        NSString *subStr3 = [str1 substringWithRange:range];
        NSLog(@"subStr3 = %@",  subStr3);
//NSMakeRange();//构建NSRange变量
        NSString *subStr4 = [str1 substringWithRange:NSMakeRange(2,6)];
        NSLog(@"subStr4 = %@", subStr4);
//字符串比较
        NSString *str2 = [NSString stringWithCString:"hallo world中国" encoding:NSUTF8StringEncoding];
        NSString *str3 = [NSString stringWithUTF8String:"hello world中国"];
        NSComparisonResult result = [str2 compare:str3];
if (result == NSOrderedAscending) {//递增
            NSLog(@"str2 < str3");
        }
elseif(result == NSOrderedDescending)//递减
        {
            NSLog(@"str2 > str3");
        }
else
        {
            NSLog(@"str2 == str3");
        }
//以大小写不敏感方式比较字符串
//[str2 caseInsensitiveCompare:str3];
//判断两个字符串是否相等
//- (BOOL)isEqualToString:(NSString *)aString;
BOOL ret = [str2 isEqualTo:str3];
if (ret==YES) {
            NSLog(@"str2 == str3");
        }
else
        {
            NSLog(@"str2 != str3");
        }
//判断前缀子串
//- (BOOL)hasPrefix:(NSString *)aString;
BOOL ret1 = [@"www.baidu.com" hasPrefix:@"www."];
        NSLog(@"ret1 = %d", ret1);
//判断后缀子串
//- (BOOL)hasSuffix:(NSString *)aString;
BOOL ret2 = [@"www.hao123.com" hasSuffix:@"com"];
        NSLog(@"ret2 = %d", ret2);
//判断是否包含子串(10.10macos)
BOOL ret3 = [@"hao123" containsString:@"hao"];
        NSLog(@"ret3 = %d", ret3);
//查找子串
        NSString *str4 = [[NSString alloc] initWithFormat:@"%s","hello world qidfafddnworldfedffsng"];
        NSRange range1 =[str4 rangeOfString:@"world"];
if (range1.location == NSNotFound) {//不能查找对应的子串,返回long类型最大值
            NSLog(@"没有查找到字串 notfound = %lu", NSNotFound);
        }
else
        {
            NSLog(@"location = %lu length = %lu", range1.location, range1.length);
        }
//倒序查找子串
        NSRange range2 = [str4 rangeOfString:@"world" options:NSBackwardsSearch];
        NSLog(@"location = %li length = %li", range2.location, range2.length);
//字符串追加
//并不是直接在原字符串的末尾追加字符串,而是利用传入的字符串及原字符串创建一个新的字符串
        NSString *str5 =@"hello";
        NSLog(@"%p", str5);
        str5 = [str5 stringByAppendingString:@"world"];
        NSLog(@"str5 = %@", str5);
        NSLog(@"%p", str5);
//格式化追加字符串
        NSString *str6 =@"qfdfdng";
        str6 = [str6 stringByAppendingFormat:@"%d%s",123,"helloworld"];
        NSLog(@"str6 = %@", str6);
//把字符换串对象转换成整型浮点型
int a = [@"12345" intValue];
float f = [@"3.14" floatValue];
        NSLog(@"a = %d f = %.2f", a, f);
//返回公共前缀子串
        NSString *str7 = [@"www.baidu.com" commonPrefixWithString:@"www.hao123.com" options:NSLiteralSearch];
        NSLog(@"str7 = %@", str7);
//大小写转换
//把小写字母转换成大写字母
        NSString *str8 = [@"baidu中国" uppercaseString];
        NSLog(@"str8 = %@", str8);
//把大写字母转换成小写字母
        NSString *str9 = [@"BaiDU" lowercaseString];
        NSLog(@"str9 = %@", str9);
//把每个单词的首字母大写
        NSString *str10= [@"bai du qian feng" capitalizedString];
        NSLog(@"str10 = %@", str10);
//字符串替换
//- (NSString *)stringByReplacingOccurrencesOfString:(NSString *)target withString:(NSString *)replacement
        NSString *str11=@"hello world qiafdfnfdg hello world hello hell qdfdfnfdg";
        str11 = [str11 stringByReplacingOccurrencesOfString:@"hello" withString:@"welcome"];
        NSLog(@"str11 = %@", str11);
//替换指定范围内的字符
//- (NSString *)stringByReplacingCharactersInRange:(NSRange)range withString:(NSString *)replacement
        NSString *str12 =@"hello world qianfeng";
        str12 = [str12 stringByReplacingCharactersInRange:NSMakeRange(12,8) withString:@"welcome"];
        NSLog(@"str12 = %@", str12);
//把OC的字符串对象转换成C字符串
        NSLog(@"%s", [@"hello world" UTF8String]);
//用网址的内容生成OC字符串对像
//- (instancetype)initWithContentsOfURL:(NSURL *)url encoding:(NSStringEncoding)enc error:(NSError **)error;
//+ (instancetype)stringWithContentsOfURL:(NSURL *)url encoding:(NSStringEncoding)enc error:(NSError **)error;
        NSURL *url= [[NSURL alloc] initWithString:@"http://www.baidu.com"];
        NSString *urlContent = [[NSString alloc] initWithContentsOfURL:url encoding:NSUTF8StringEncoding error:nil];//nil空指针
        NSLog(@"urlContent = %@", urlContent);
//用文件的内容生成字符串
//- (instancetype)initWithContentsOfFile:(NSString *)path encoding:(NSStringEncoding)enc error:(NSError **)error;
//+ (instancetype)stringWithContentsOfFile:(NSString *)path encoding:(NSStringEncoding)enc error:(NSError **)error;
        NSString *fileContent = [NSString stringWithContentsOfFile:@"/Users/zhangxueming/Desktop/json.txt" encoding:NSUTF8StringEncoding error:nil];
        NSLog(@"fileContent = %@", fileContent);
    }
return0;
}
三.NSSMUtableString的常用方法
#import <Foundation/Foundation.h>
//NSMutableString 继承与NSString
//所有NSString类的方法NSMutableString都可以使用
int main(int argc,constchar * argv[]) {
@autoreleasepool {
//创建指定容量大小的可变字符串对象
//+ (NSMutableString *)stringWithCapacity:(NSUInteger)capacity;
        NSMutableString *mulStr1 = [[NSMutableString alloc] initWithCapacity:20];
        NSLog(@"mulStr1 = %@", mulStr1);
//替换指定范围内的字符
//- (void)replaceCharactersInRange:(NSRange)range withString:(NSString *)aString;
        NSMutableString *mulStr2 = [[NSMutableString alloc] initWithString:@"hello world qianfdfsfefdg"];
        [mulStr2 replaceCharactersInRange:NSMakeRange(6,5) withString:@"welcome"];
        NSLog(@"mulStr2 = %@", mulStr2);
//在指定位置增加字符串
        NSMutableString *mulStr3 = [[NSMutableString alloc] initWithFormat:@"梦想中国"];
        [mulStr3 insertString:@"hello world" atIndex:2];
        NSLog(@"mulStr3 = %@", mulStr3);
//删除指定范围内的字符
        NSMutableString *mulStr4 = [NSMutableString stringWithUTF8String:"热爱hello world中国"];
        [mulStr4 deleteCharactersInRange:NSMakeRange(2,11)];
        NSLog(@"mulStr4 = %@", mulStr4);
//追加字符串
        NSMutableString *mulStr5 = [NSMutableString stringWithString:@"helloworld"];
        [mulStr5 appendString:@"qidffdf"];
        NSLog(@"mulStr5 = %@", mulStr5);
//格式化追加字符串
        NSMutableString *mulStr6 = [NSMutableString stringWithFormat:@"%s%d","hello",
12345];
        [mulStr6 appendFormat:@"%.2f%@",3.14,
@"world"];
        NSLog(@"mulStr6 = %@", mulStr6);
//修改字符串
        NSMutableString *mulStr7 = [[NSMutableString alloc] initWithString:@"hello world"];
        [mulStr7 setString:@"qifdfdng"];
        NSLog(@"mulStr7 = %@", mulStr7);
    }
return0;
}
