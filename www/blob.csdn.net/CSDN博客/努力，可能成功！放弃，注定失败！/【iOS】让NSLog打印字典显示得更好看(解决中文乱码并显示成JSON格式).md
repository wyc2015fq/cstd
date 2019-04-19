# 【iOS】让NSLog打印字典显示得更好看(解决中文乱码并显示成JSON格式) - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年09月26日 09:55:05[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：272
前言
文章的初衷很简单，是为了能够正常显示打印出字典里面的中文。因为默认情况下，直接打印字典的话，在Xcode控制台上，中文会是乱码的，需要Unicode转码才能看到中文。
比如打印下面的一个字典
```
NSDictionary *dict = @{
                       @"ArticleTitle":@"【iOS开发】打开另一个APP（URL Scheme与openURL）",
                       @"ArticleUrl":@"https://www.jianshu.com/p/0811ccd6a65d",
                       @"author":@{
                               @"nickName":@"谦言忘语",
                               @"blog":@"https://www.jianshu.com/u/cc2cf725ac0c",
                               @"work":@"iOS工程师"
                               }
                       };
NSLog(@"打印出的字典：%@",dict);
```
Xcode控制台上显示的是这样子的：
![](https://upload-images.jianshu.io/upload_images/1818095-8b3219e5575d961c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
默认情况下Xcode打印字典，中文会显示乱码
WTF!谁能告诉我，这坨东西是什么玩意儿？！！！
> 
其实还是可以知道这些Unicode编码是什么意思的。平常我遇到这种情况会复制那堆Unicode的代码到[在线网站](http://tool.chinaz.com/tools/unicode.aspx)上进行转码查看。但是依然觉得不太方便。
![](https://upload-images.jianshu.io/upload_images/1818095-706c7ef46987b834.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
使用在线网站进行Unicode转码
先看看结果
我终于无法忍受这么坑爹的中文显示了，查找一些资料、经过一系列尝试之后，终于找到一个比较满意的解决方案了。先看结果：
![](https://upload-images.jianshu.io/upload_images/1818095-34744ece938cf873.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/984/format/webp)
最终结果
```
2018-09-03 15:43:10.046 PrintBeautifulLog[4446:1265987] 打印出的字典：{
  "ArticleTitle" : "【iOS开发】打开另一个APP（URL Scheme与openURL）",
  "ArticleUrl" : "https:\/\/www.jianshu.com\/p\/0811ccd6a65d",
  "author" : {
    "work" : "iOS工程师",
    "blog" : "https:\/\/www.jianshu.com\/u\/cc2cf725ac0c",
    "nickName" : "谦言忘语"
  }
}
```
是不是顿时觉得神清气爽？中文出来了，而且格式也很好看，层次分明。
对了，是不是觉得这个格式似曾相似？
嘿嘿，没错，这个就是JSON格式。不信？我们拿去[JSON在线格式化网站](https://www.json.cn/)上验证下？
![](https://upload-images.jianshu.io/upload_images/1818095-27876a76d204e8da.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
JSON格式验证
另外，使用po命令调试打印的时候也是一样的。
![](https://upload-images.jianshu.io/upload_images/1818095-76f4966d4b4f6274.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
po命令调试时也能打印打印出JSON格式的Log
直接将文件拖入到工程中即可使用
这么神奇的效果？怎么做到的？嗯，很简单，直接将[github仓库](https://github.com/shixueqian/PrintBeautifulLog)上的这两个分类拉入到工程中就可以了。什么代码都不用写。
![](https://upload-images.jianshu.io/upload_images/1818095-57bb0e383920592f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
直接将这两个分类拉入到工程中即可使用
怎么做到的？
其实代码很简单，简单到难以想象。分类里面就只有10多行代码。
```
//NSDictionry分类实现文件代码
#import "NSDictionary+Log.h"
@implementation NSDictionary (Log)
#ifdef DEBUG
//打印到控制台时会调用该方法
- (NSString *)descriptionWithLocale:(id)locale{
    return self.debugDescription;
}
//有些时候不走上面的方法，而是走这个方法
- (NSString *)descriptionWithLocale:(id)locale indent:(NSUInteger)level{
    return self.debugDescription;
}
//用po打印调试信息时会调用该方法
- (NSString *)debugDescription{
    NSError *error = nil;
    //字典转成json
    NSData *jsonData = [NSJSONSerialization dataWithJSONObject:self options:NSJSONWritingPrettyPrinted  error:&error];
    //如果报错了就按原先的格式输出
    if (error) {
        return [super debugDescription];
    }
    NSString *jsonString = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
    return jsonString;
}
#endif
@end
```
接下来解释下这段代码：
- NSLog打印字典(NSDictionary)和数组(NSArray)的时候的时候会走`- (NSString *)descriptionWithLocale:(id)locale`来决定打印的字符串。打印其他对象(比如NSString类型)的时候会走`- (NSString *)description`方法。所以现在我们需要重写NSDictionary的`- (NSString *)descriptionWithLocale:(id)locale`方法来得到我们想要的结果。
- 在使用po命令调试的时候，会走`- (NSString *)debugDescription`方法，所以我们需要覆盖该方法来显示出我们想要的结果。
- 在`- (NSString *)descriptionWithLocale:(id)locale`和`- (NSString *)debugDescription`方法里面将字典转化为JSON字符串输入，就能同时在代码调试打印和使用po命令调试打印时都能得到我们想要的结果。
```
NSError *error = nil;
    //字典转成json格式字符串
    NSData *jsonData = [NSJSONSerialization dataWithJSONObject:self options:NSJSONWritingPrettyPrinted  error:&error];
    NSString *jsonString = [[NSString alloc] initWithData:jsonData encoding:NSUTF8StringEncoding];
    return jsonString;
```
- 字典转化成字符串有可能会失败，所以失败的时候我们就以默认的格式输出。
```
if (error) {
    return [super debugDescription];
}
```
- 在分类里面做了DEBUG预编译判断，只有在DEBUG模式下才会调用该方法，线上包（线上包采用Release模式）不会受到影响。
```
#ifdef DEBUG
//分类中的代码
#endif
```
嗯，NSArray分类里面的代码也是一毛一样的。所以打印NSArray也能像NSDictionary一样使用JSON格式输出，并且可以正常显示中文。不多说了。
> 
除了 `- (NSString *)descriptionWithLocale:(id)locale`方法之外，还有一个`- (NSString *)descriptionWithLocale:(id)locale indent:(NSUInteger)level`方法。这两个方法功能是一样的，后者多了一个indent（缩进）参数。我测试过这两个方法的优先级，发现前后测试的结果有点矛盾，所以就懒得理，两个都实现了。
再看下其他解决NSLog打印字典时中文显示乱码的方式
还有其他的方式也能解决NSLog打印字典时显示乱码的问题。方法是一样的，增加字典和数组的分类，重写`- (NSString *)descriptionWithLocale:(id)locale`和`- (NSString *)debugDescription`方法，修改Xcode输出字符串。不同之处在于输出字符串的处理方式。先看看常用的方式。
```
//NSDictionary分类实现文件代码
- (NSString *)descriptionWithLocale:(id)locale{
    return self.debugDescription;
}
- (NSString *)debugDescription {
    NSMutableString *strM = [NSMutableString stringWithString:@"{\n"];
    [self enumerateKeysAndObjectsUsingBlock:^(id key,id obj,BOOL *stop) {
        [strM appendFormat:@"\t%@ = %@;\n", key, obj];
    }];
    [strM appendString:@"}\n"];
    return strM;
}
```
```
//NSArray分类实现文件代码
- (NSString *)descriptionWithLocale:(id)locale{
    return self.debugDescription;
}
- (NSString *)debugDescription
{
    NSMutableString *strM = [NSMutableString stringWithString:@"(\n"];
    [self enumerateObjectsUsingBlock:^(id obj, NSUInteger idx,BOOL *stop) {
        [strM appendFormat:@"\t%@,\n", obj];
    }];
    [strM appendString:@")"];
    return strM;
}
```
这种方式是直接遍历字典中的key和value，中间加一个`=`拼接起来。然后所有的key/value对拼接成一个字符串。每个key/value对后面都加入一个换行符`\n`。最后在前后加上大括号`{}`括起来。这种方式可以解决中文显示乱码的问题，但是有一个比较不好的地方，就是缩进格式没有了（Xcode默认的格式是有缩进格式的）。不管里面有多少层嵌套，前面都是一样的间隔。在多层嵌套的时候看起来会不太爽。
![](https://upload-images.jianshu.io/upload_images/1818095-a32d21df8ee82a41.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
遍历key/value对，重新拼接输出字符串
上面的方式无法处理缩进格式问题，我们之前提过，使用`- (NSString *)descriptionWithLocale:(id)locale indent:(NSUInteger)level`方法是有缩进参数的，所以可以使用这个方法可以将缩进格式搞出来。看了下感觉还不错。但是有个小缺点，使用po参数调试的时候就没有办法了。两个方法分写是在NSArray分类和NSDictionary分类里面实现的。代码如下：
```
//NSArray
- (NSString *)descriptionWithLocale:(nullable id)locale indent:(NSUInteger)level{
    
    NSMutableString *mStr = [NSMutableString string];
    NSMutableString *tab = [NSMutableString stringWithString:@""];
    for (int i = 0; i < level; i++) {
        [tab appendString:@"\t"];
    }
    [mStr appendString:@"(\n"];
    for (int i = 0; i < self.count; i++) {
        NSString *lastSymbol = (self.count == i + 1) ? @"":@",";
        id value = self[i];
        if ([value respondsToSelector:@selector(descriptionWithLocale:indent:)]) {
            [mStr appendFormat:@"\t%@%@%@\n",tab,[value descriptionWithLocale:locale indent:level + 1],lastSymbol];
        } else {
            [mStr appendFormat:@"\t%@%@%@\n",tab,value,lastSymbol];
        }
    }
    [mStr appendFormat:@"%@)",tab];
    return mStr;
}
//NSDictionary
- (NSString *)descriptionWithLocale:(id)locale indent:(NSUInteger)level
{
    NSMutableString *mStr = [NSMutableString string];
    NSMutableString *tab = [NSMutableString stringWithString:@""];
    for (int i = 0; i < level; i++) {
        [tab appendString:@"\t"];
    }
    [mStr appendString:@"{\n"];
    NSArray *allKey = self.allKeys;
    for (int i = 0; i < allKey.count; i++) {
        id value = self[allKey[i]];
        NSString *lastSymbol = (allKey.count == i + 1) ? @"":@";";
        if ([value respondsToSelector:@selector(descriptionWithLocale:indent:)]) {
            [mStr appendFormat:@"\t%@%@ = %@%@\n",tab,allKey[i],[value descriptionWithLocale:locale indent:level + 1],lastSymbol];
        } else {
[mStr appendFormat:@"\t%@%@ = %@%@\n",tab,allKey[i],value,lastSymbol];
        }
    }
    [mStr appendFormat:@"%@}",tab];
    return mStr;
}
```
还有另外一种方式，这种方式的思想是，上面第一种方式没有缩进格式，看起来很不爽，但是系统默认的实现方式是有缩进格式的。只是中文显示有问题而已。那我直接把默认方式中要输出的字符串进行Unicode转化，将其转化为中文不就可以了？
具体代码就不贴了，有兴趣可以看下[这篇文章](https://www.jianshu.com/p/040293327e18)
这种方式确实可行，跟原先的输出的唯一不同就是将Unicode字符串转化为了中文字符串显示。但是有一个缺点，那就是在将默认方式的Unicode字符串转化为中文字符串显示的时候，容易出问题。因为转码之前是需要暴力替换的，这个替换过程是很容易出问题的。比如如果字典的value字符串里面本来就有`" "`符号，那转码就出问题了。
更新（20180914）
之前的方式遇到字典数组里面有模型的情况容易出问题。
于是在将字典/数组转换成JSON字符串之前，先判断其是否能转换成JSON格式字符串，如果不能，就调用系统的原始实现。
由于要调用系统的原始实现，所以还使用了method swizzle交换了上面说的3个系统方法。具体可查看github代码。
参考
[代码已放在github上](https://github.com/shixueqian/PrintBeautifulLog)
[iOS JSON数据NSLog小技巧](https://www.jianshu.com/p/b6bb983e39da)
[iOS 打印中文字典,数组,控制台输出中文,并保持缩进格式](https://www.jianshu.com/p/040293327e18)
[iOS description方法和descriptionWithLocale:方法 解决中文现问题](https://blog.csdn.net/u011146511/article/details/51234462)
[xcode8控制台打印出字典和数组中的中文字符 解决中文乱码](https://www.jianshu.com/p/294debd013d4)
[iOS开发实战tips--让Xcode的控制台支持NSArray和NSDictionary的中文输出](https://blog.csdn.net/biggercoffee/article/details/50094967)
[从NSDictionary打印不出中文开始](https://www.jianshu.com/p/f14b4cb1435b)
作者：谦言忘语
链接：https://www.jianshu.com/p/79cd2476287d
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
