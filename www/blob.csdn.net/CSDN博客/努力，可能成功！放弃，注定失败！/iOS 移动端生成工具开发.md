# iOS 移动端生成工具开发 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年08月22日 13:38:22[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：176
> 
[本文来自简书，原文地址:http://www.jianshu.com/p/cb36b36f90dd](http://www.jianshu.com/p/cb36b36f90dd)
> 
上个月的一篇[移动端面向文档开发](http://www.jianshu.com/p/b35d06cf189a)一文久违的被编辑推荐到了首页, 也引来了饿了么大神的关注, 虽然最后实力不济未被录用, 但也指明了我前进的方向. 从4月到现在从一个想法到一步步的实现, 感觉自己还是有充足的长进的, 今天我们继续来完善这个项目.
之前有写过一个python脚本的生成工具, 通过读取json文件进行代码的生成, 实际操作中部分使用者反映, json文件的可读性不是很好(明显是不熟悉json语法导致的), 以及需要生成多平台代码, 所以python并不是很熟练的我, 考虑再三还是写一个oc项目一劳永逸. 本文会涉及到一些平时开发并不会特别关注的知识点, 以及该生成工具的代码设计和使用方法.
#### 选择项目类型
第一个脑中闪现的想法就是使用Command Line Tool命令行工具了, 不就是代码生成吗, 还要开启模拟器? 这不是逗咩...
![](https://img-blog.csdn.net/20170822134012743?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzA1MTM0ODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
但我们需要读取Template模板文件, 由于模板文件是不可编译的文件, 所以我们并不能直接将其拖入项目中来, 然后我想那就通过相对路径的方法来读取文件吧, 可惜的是我找了一圈都没有获取当前路径的api, 只有sandbox, 沙盒...
我想命令行工具还有沙盒这玩意? 好吧, 我试试通过Bundle进行获取吧, 试了很久获取到的都是null, 无语... 到Stack Overflow上一问, 命令行工具不支持自制Bundle的读取, 好吧, 我认了. 但我感觉这只是一个链接的问题, 应该是可以读取的, 可惜找不到可执行的方法, 无奈只有放弃.
![](https://img-blog.csdn.net/20170822134018162?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzA1MTM0ODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
被迫我们只能够选择Single View Application了, 只是万般无奈啊, 好吧, 对世事的妥协不也是生活中的一部分嘛...
#### 选择配置文件
在配置文件上的选择其实比较简单, 既然是oc的项目, 选择读取最为简单的plist是最好的选择. 同时也比较容易配置.
![](https://img-blog.csdn.net/20170822134024449?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzA1MTM0ODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20170822134030087?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzA1MTM0ODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
进行了一些简单的配置, 这部分不是很清晰的同学可以前往 [http://www.jianshu.com/p/47d565bf200e](http://www.jianshu.com/p/47d565bf200e) 进行了解.
配置完plist后我们将之前的模板一起放入bundle并引入工程.
![](https://img-blog.csdn.net/20170822134036809?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzA1MTM0ODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
知识点: 放入bundle的文件并不会进行编译, 仅作为资源文件存储.
#### 选择设计模式
对于设计模式来说, 确实是看过不少相关类型的书籍, 一些常用的设计模式也是手到擒来, 这不, 对于代码生成工具, 立马能够想到生成器模式, 哈哈, 是工厂模式和装饰模式.
其实对于这些设计模式的命名其实感觉挺误导人的, 什么责任链咯, 原型什么的, 其实看一遍代码就懂得东西被吹的神神道道, 也是醉了.
通过设计模式的选择后我们创建了`SQBuilder`和`SQFileParser`, 前者为工厂模式, 后者为装饰模式.
```
@interface SQBuilder : NSObject
@property (nonatomic,copy) NSString * user;
@property (nonatomic,copy) NSString * prefix;
@property (nonatomic,copy) NSString * module;
@property (nonatomic,strong) NSDictionary * parameter;
@property (nonatomic,strong) NSDictionary * dataList;
@property (nonatomic,strong) NSArray * actionList;
+ (void)runWithFileParser:(NSDictionary *)config success:(void (^)())success failure:(void (^)())failure;
- (void)build;
@end
```
```
+ (SQBuilder *)builderWithType:(SQBuilderType)builderType {
    switch (builderType) {
        case SQBuilderTypeiOS:
            return [SQBuilder_iOS new];
            break;
        case SQBuilderTypeAndroid:
            return [SQBuilder_Android new];
            break;
        case SQBuilderTypeNone:
            return [SQBuilder new];
            break;
        default:
            break;
    }
}
```
SQBuilder的头文件暴露了和plist中需要配置的属性以及加载SQFileParser文件解析类的方法. 通过工厂模式可以分配创建不同的类.
```
@interface SQFileParser : NSObject
+ (NSDictionary *)parser_plist_r;
+ (void)parser_rw:(NSString *)path code:(NSString *)code filename:(NSString *)filename header:(NSString *)header parameter:(NSMutableArray *)parameter;
@end
```
```
[SQBuilder runWithFileParser:[SQFileParser parser_plist_r] success:^{
    title = @"Build Finished"; message = @"please view the floder on the desktop";
} failure:^{
    title = @"Build Error!!!"; message = @"please enter the right builder type!!";
}];
```
SQFileParser的头文件暴露了读取不同文件类型的接口, 后续读取Excel表格类型仅需添加新的类型接口即可.
#### 创建文件夹 Api
```
NSFileManager * manager = [NSFileManager defaultManager];
NSString * path = [NSString stringWithFormat:@"/Users/%@/Desktop/%@", self.user, self.module];
[manager createDirectoryAtPath:path withIntermediateDirectories:NO attributes:nil error:nil];
```
oc中创建文件夹的方式, 能找到的就只有这个了, 而且只能读取绝对路径, 但又不能读取当前文件路径, mac机的user每台机器都不一样, 真是坑啊, 像文件读写这种东西还是python写最为稳妥.
#### SQFileParser 代码分析
```
+ (NSDictionary *)parser_plist_r {
    
    NSBundle * bundle = [NSBundle bundleWithPath:[[NSBundle mainBundle] pathForResource:@"builder.bundle" ofType:nil]];
    return [NSDictionary dictionaryWithContentsOfFile:[bundle pathForResource:@"config/config.plist" ofType:nil]];
}
```
通过子bundle进行读取/config.plist配置文件, 知识点: 当读取bundle深层结构时需要添加文件路径.
```
+ (NSString *)parser_r:(NSString *)filename code:(NSString *)code {
    
    NSBundle * bundle = [NSBundle bundleWithPath:[[NSBundle mainBundle] pathForResource:@"builder.bundle" ofType:nil]];
    return [NSMutableString stringWithContentsOfFile:[bundle pathForResource:[NSString stringWithFormat:@"template/%@/%@", code, filename] ofType:nil] encoding:NSUTF8StringEncoding error:nil];
}
```
Templete文件的读取, 和上面的读取plist相同, 并没有什么不同之处.
```
static NSString * code;
+ (NSString *)replaceThougth:(NSString *)templete parameter:(NSMutableArray *)parameter {
    
    __block NSString * temp = templete;
    [[parameter firstObject] enumerateKeysAndObjectsUsingBlock:^(NSString *  _Nonnull key, NSString *  _Nonnull obj, BOOL * _Nonnull stop) {
        temp = [templete stringByReplacingOccurrencesOfString:key withString:obj];
    }];
    [parameter removeObjectAtIndex:0];
    if (parameter.count) {
        [SQFileParser replaceThougth:temp parameter:parameter];
    } else {
        code = temp;
    }
    return code;
}
```
写入新文件, 这个就要好好讲讲了, NSString的`stringByReplacingOccurrencesOfString`方法是会返回一个新的更改后的字符串但并不会改变原指针. 但如果需要替换很多处的话就需要链式调用多次, 真是丑, 一个字概括了所有的观点, 所以我在想有什么方法能够避免这个问题呢? 马上想的就是上述递归调用的方法啦, 哈哈.
```
+ (void)parser_rw:(NSString *)path code:(NSString *)code filename:(NSString *)filename header:(NSString *)header parameter:(NSMutableArray *)parameter {
    NSString * arch = [[filename componentsSeparatedByString:@"."]firstObject];
    NSString * suffix = [[filename componentsSeparatedByString:@"."]lastObject];
    NSString * filename_r = [NSString stringWithFormat:@"%@Template.%@", arch,suffix];
    NSString * filename_w = [NSString stringWithFormat:@"%@/%@%@.%@", path,header,arch,suffix];
    NSString * template =  [SQFileParser parser_r:filename_r code:[code lowercaseString]];
    [[SQFileParser replaceThougth:template parameter:parameter] writeToFile:filename_w atomically:YES encoding:NSUTF8StringEncoding error:nil];
}
```
这个方法是读写一体的, 根据拿到的配置参数, 拼接出输入文件名和输出文件名, 并进行文件的读取和写入.
#### SQBuilder
这个其实就没什么好讲的了, 自己去看代码即可, 仅是读取配置参数并动态生成方法的逻辑代码了.
#### 如何使用?
![](https://img-blog.csdn.net/20170822134042406?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzA1MTM0ODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20170822134048172?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzA1MTM0ODM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
打开终端输入pwd获取user信息,并填入plist配置文件, 并输入其他参数, 进行`Run`即可将代码生成至桌面. 以后开发的时候只需要关注View层, 其他的都不需要操心啦~~
具体说明可以[下载SQTemplate](https://github.com/coderZsq/coderZsq.project.oc)来进行共同探讨.
作者：Castie1
链接：http://www.jianshu.com/p/cb36b36f90dd
來源：简书
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
