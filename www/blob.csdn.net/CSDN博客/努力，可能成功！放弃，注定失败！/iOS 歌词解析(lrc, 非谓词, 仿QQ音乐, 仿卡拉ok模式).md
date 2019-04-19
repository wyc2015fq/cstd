# iOS 歌词解析(lrc, 非谓词, 仿QQ音乐, 仿卡拉ok模式) - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2019年03月20日 10:53:28[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：136
## 前言
要解析 `lrc` 格式的歌词, 首先需要知道什么是 `lrc` 歌词, 还需要知道 `lrc` 歌词的规范. 在这里先放出一个`百度百科`的链接地址, 仅供大家参考: [百度百科: lrc](https://link.jianshu.com?t=http://baike.baidu.com/link?url=e3K8w3X2WM4zmaYXsKq9qq2VCY1ZDqeC0moO-LZoPrVpJayhZRDd8YzVsqBbGQePZ4JUqkQRnNQTFUsBIs6pMK)
## 关于本文
本文的歌词解析部分, 仅仅针对标准的 `lrc` 格式歌词进行解析, 像我们常用的 QQ音乐 的 `qrc` 等歌词并未进行解析. `lrc` 的标准格式也不仅有一种(例如时间标签: `[00:01.02]`、`[0:1:12]`、`[00:02]`), 本文对所有标准样式的 `lrc` 歌词都进行了解析.
本文的歌词解析部分, 由于 `Demo` 中用不到, 所以针对 `[ar:艺人名]`、`[ti:曲名]`、`[al:专辑名]` 等这类的 `识别标签` 做了过滤的处理, 并未予以解析.
本文的歌词展示部分, 采用的是模拟 `拉卡OK` 的播放样式(类似 QQ音乐), 也就是说歌词会一步一步的高亮显示, 而不是直接正行的高亮显示. 为什么说是模拟 `卡拉OK`的播放样式呢? 原因在于解析的是标准 `lrc` 歌词, 它不包含每一个字的毫秒值, 所以也就无法做到歌曲唱到哪个字, 哪个字就高亮显示.
## 关于Demo
要 `Demo` 的小伙伴在留言处留下你的邮箱吧, 我有点懒得上传 `Github` 了. 抱歉!
## 效果
俗话说得好, 千言万语不如上几张动图来的实在. 先来看看效果.
普通播放音乐
![](https://upload-images.jianshu.io/upload_images/2452150-4ff1237550e07d6c.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/375/format/webp)
普通播放.gif
切换歌曲
![](https://upload-images.jianshu.io/upload_images/2452150-49921842e0e60c9f.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/375/format/webp)
切换音乐.gif
歌词定位
![](https://upload-images.jianshu.io/upload_images/2452150-7b7a6cebb53f6cde.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/375/format/webp)
歌词定位.gif
进度条定位
![](https://upload-images.jianshu.io/upload_images/2452150-13ecbe382854895a.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/375/format/webp)
进度条定位.gif
## 正文
好了, 效果看完了, 现在来简单描述一下歌词解析的部分. 先来看看 `.h` 里面公开的方法, 如下:
```
/** 解析歌词. 默认: 解析 .lrc 歌词, Bundle 为 MainBundle */
+ (NSArray<NSDictionary *> *)ml_parseLyricWithFileName:(NSString *)fileName;
+ (NSArray<NSDictionary *> *)ml_parseLyricWithFileName:(NSString *)fileName 
                                                  type:(NSString *)type;
+ (NSArray<NSDictionary *> *)ml_parseLyricWithFileName:(NSString *)fileName 
                                                  type:(NSString *)type
                                              inBundle:(NSBundle *)bundle;
/** 通过歌词字符串, 解析歌词 */
+ (NSArray<NSDictionary *> *)ml_parseLyricWithLyricString:(NSString *)lyricString;
```
前三个方法是用来加载和解析本地歌词的, 第四个方法主要是考虑后台直接返回歌词字符串的情况下, 直接可以使用该方法进行解析.
返回值是一个由 `NSDictionary` 组成的数组, 其中每一个 `NSDictionary` 元素包含的 `Key` 如下所示:
```
/** 歌词解析返回 NSDictionary 的 Key */
static NSString *const kMLLyricScrollView_LyricParse_Key_Time = @"MLParseLyricKey_Time";
static NSString *const kMLLyricScrollView_LyricParse_Key_TimeString = @"MLParseLyricKey_TimeString";
static NSString *const kMLLyricScrollView_LyricParse_Key_TimeInterval = @"MLParseLyricKey_TimeInterval";
static NSString *const kMLLyricScrollView_LyricParse_Key_LyricContent = @"MLParseLyricKey_LyricContent";
```
具体这个 `NSDictionary` 数组怎么使用, 下文中会详细进行介绍.
拿到 `Demo` 的同学, 从代码中不难看出, 其实主要的代码逻辑都集中在 `+ (NSArray<NSDictionary *> *)ml_parseLyricWithLyricString:(NSString *)lyricString;` 方法中(上方代码块中的第四个方法), 所以在这里, 我就先从这个方法入手, 来说说解析歌词的代码逻辑.
在直接进入这个方法之前, 我先上一段 `lrc`, 这更有助于记忆和理解.
```
Lyric Demo
[ti:刚好遇见你]
[ar:李玉刚]
[al:]
[by:黎起铮]
匹配时间为: 03 分 20 秒 的歌曲
[0:0.0]
[0:2]刚好遇见你
[0:3.65]作词：高进
[0:4.61]作曲：高进
[0:5.65]演唱：李玉刚
[0:6.66]编曲：关天天
[0:13.35][1:14:75]我们哭了
[0:16.12][1:17:53]我们笑着
[0:19.21][1:20:71]我们抬头望天空
[0:22.6][1:23:54]星星还亮着几颗
[0:25.29][1:26:56]我们唱着
[0:28.23][1:29:73]时间的歌
[0:31.45][1:33:4]才懂得相互拥抱
[0:34.47][1:35:83]到底是为了什么
[00:37.48][01:38.96][02:31.50]因为我刚好遇见你
[00:41.3][01:42.36][2:7.36][02:34.74]留下足迹才美丽
[00:44.3][01:45.44][02:10.14][02:37.80]风吹花落泪如雨
[00:47.16][01:48.53][02:13.11][02:40.76]因为不想分离
[00:50.32][01:51.65][02:4.72][02:16.22][02:43.95]因为刚好遇见你
[00:53.26][01:54.69][02:19.27][02:46.97]留下十年的期许
[00:56.25][01:57.72][02:22.32][02:49.96]如果再相遇
[00:59.68][02:01.5][02:25.79][02:53.28]我想我会记得你
[02:57]
[03:14]Made By Liguoan
[03:20]
```
其实上面这段歌词是我自己编辑的, 说实在的, 它应该算是标准 `lrc` 歌词里面最不标准的一种了. 主要是为了代码的测试, 所以刻意把它写的很不标准(在标准 `lrc` 歌词的范围内). 首先可以看到, 这段歌词中包含 `Lyric Demo`、`匹配时间为: 03 分 20 秒 的歌曲` 这样的注释性质的文字, 其次, 仔细观察时间标签, 五花八门, 什么样子的都有, 例如: `[0:2]`、`[0:6.66]`、`[1:17:53]`、`[02:31.50]` 等. 最后, 一句歌词, 不同的触发时间, 写在同一行中, 这样的歌词解析出来最大的问题就是无序(歌词无序, 也在 `lrc` 标准范围内). 所以**基本上**, 如果上面这段 `lrc` 歌词解析成功的话, 那其它在网上找的 `lrc` 也好, 后台给的 `lrc` 也罢, 只要它在标准 `lrc` 歌词范围内, 那我们的 `App` 应该都可以完美的解析.
好了, `lrc` 歌词也了解了, 来看看解析歌词的代码吧.
Step 1
用 `\n` 字符, 将歌词字符串分割成为一个字符串数组, 声明一个 `result` 变量用来保存最后的结果, 声明一个 `arrFilter` 变量, 用来过滤掉所有的 `识别标签`:
```
// 解析歌词, 先通过 "\n" 回车字符, 将 lyric 字符串分割成字符串数组
    NSMutableArray<NSMutableDictionary *> *result = [NSMutableArray array];
    NSArray *arrLycComponents = [lyricString componentsSeparatedByString: @"\n"];
    
    // 设置标签过滤数组. Note: 本段代码中, 只解析了时间标签, 其他标记标签没解析
    NSArray<NSString *> *arrFilter = @[@"[ar", @"[ti", @"[al", @"[by", @"[of", @"t_"];
```
Step 2
`For...in` 循环, 遍历歌词字符串数组中的所有内容, 并对每一条遍历出来的内容进行处理 (`Step 2` 代码逻辑全在 `For..in` 循环中).
Step 2.1
```
// 解析歌词部分
    for (NSString *lyric in arrLycComponents) {
        // 空句直接进入下一次循环.
        if (!lyric.length) continue;
        
        // 歌词不以 "[" 开头为非法格式, 直接进入下一次循环
        if (![lyric hasPrefix: @"["]) continue;
        
        // 过滤标签, 如果是标签则直接进入下一次循环.
        BOOL needFilter = NO;
        for (NSString *filter in arrFilter) {
            if ([lyric hasPrefix: filter]) {
                needFilter = YES;
                break;
            }
        }
        if (needFilter) continue;
        ...
    }
```
判断是否为空句, 对歌词中的空句不予以解析.
标准 `lrc` 歌词, 都应以标签作为开头, 所以过滤掉所有不合法的 `lrc` 歌词语句.
由于本文中, 对 `识别标签` 不予以处理, 所以会通过 `arrFilter` 数组, 将所有的 `识别标签` 进行过滤.
Step 2.2
```
// 解析歌词部分
    for (NSString *lyric in arrLycComponents) {
        ...
        if (![lyric containsString: @"]"]) continue;
        NSMutableArray<NSString *> *lrcComponents = [NSMutableArray arrayWithArray: [lyric componentsSeparatedByString: @"]"]];
        ...
    }
```
通过 `]` 字符分割字符串, 是为了解决一句歌词有很多触发时间点的情况.
分割后的 `lrcComponents` 有以下几种情况:
歌词1: `[00:53.00][01:43.88][02:11.23]虽然无所谓写在脸上`
分割后的样子例如: `@[@"[00:53.00", @"[01:43.88", @"[02:11.23", @"虽然无所谓写在脸上"]`
歌词2: `[00:34.57]怎么慢它停也停不了`
分割后的样子例如: `@[@"[00:34.57", @"怎么慢它停也停不了"]`
歌词3: `[00:48.50][01:29.73]`
分割后的样子例如: `@[@"[00:48.50", @"[01:29.73"]`
Step 2.3
```
// 解析歌词部分
    for (NSString *lyric in arrLycComponents) {
        ...
        NSString *lastComponent = [lrcComponents lastObject];
        if ([lastComponent hasPrefix: @"["]) { // 最后一部分应该是内容部分, 不应该是 "[" 字符开头.
            [lrcComponents addObject: @""];
        }
        ...
    }
```
由于有 `Step2.2` 中 `歌词3` 这种类型的歌词, 也就是有时间点, 但是没有内容的歌词(可能是间奏), 我们需要专门处理这种类型的歌词. `lrcComponents` 这个数组的最后一部分, 应该为歌词的内容, 所以如果最后一部分不是歌词内容的话, 需要手动添加一个空字符串, 作为歌词的内容.
处理完的 lrcComponents 的样子应该如下(最后一部分, 永远是歌词部分):
`@[@"[00:53.00", @"[01:43.88", @"[02:11.23", @"虽然无所谓写在脸上"]`
`@[@"[00:34.57", @"怎么慢它停也停不了"]`
`@[@"[00:48.50", @"[01:29.73", @""]`
Step 2.4
```
// 解析歌词部分
    for (NSString *lyric in arrLycComponents) {
        ...
        for (NSInteger index=0; index<lrcComponents.count-1; index++) {
            lrcComponents[index] = [lrcComponents[index] stringByReplacingOccurrencesOfString: @"["
                                                                                   withString: @""];
        }
        ...
    }
```
在 `Step 2.4` 中, 我们需要将 `lrcComponents` 字符串数组中每个元素的 `[` 字符去除.
处理完的 lrcComponents 的样子应该如下:
`@[@"00:53.00", @"01:43.88", @"02:11.23", @"虽然无所谓写在脸上"]`
`@[@"00:34.57", @"怎么慢它停也停不了"]`
`@[@"00:48.50", @"01:29.73", @""]`
可以看到, 到目前为止, 我们已经将歌词基本解析出来了, 不必要的字符都也已经删除掉了. 接下来, 就是将字符串数组中的歌词, 拼接成字典, 保存到 `result` 这个数组中.
Step 2.5
```
// 解析歌词部分
    for (NSString *lyric in arrLycComponents) {
        ...
        for (NSInteger index=0; index<lrcComponents.count-1; index++) {
            
            NSMutableDictionary *lyricDict = [self ml_lyricDictWithContent: [lrcComponents lastObject]
                                                         triggerTimeString: lrcComponents[index]];
            if (!lyricDict) continue;
            ...
        }
        ...
    }
```
由于一句歌词, 可能对应不同的时间点, 所以在 `Step 2.5` 中, 使用 `For` 循环, 遍历每一个时间点, 然后进行字典的拼接. 如果字典拼接失败(也就是 `lyricDict` 为空的时候), 说明该歌词语句存在不合法的情况, 则直接跳过本次循环, 进入下一句歌词的解析.
可以看到, `Step 2.5` 中有一个方法 `+ (NSMutableDictionary *)ml_lyricDictWithContent:(NSString *)lyricContent triggerTimeString:(NSString *)triggerTimeString`, 该方法用来拼接字典, 代码逻辑的实现如下:
```
+ (NSMutableDictionary *)ml_lyricDictWithContent:(NSString *)lyricContent triggerTimeString:(NSString *)triggerTimeString {
    
    // 尝试获取时间. 如果时间不存在或格式不符合 直接返回 nil
    // Note: 歌词格式支持如下几种(lrc 标准格式均支持)
    // 1. [mm:ss.ff]
    // 2. [m:s.f]
    // 3. [mm:ss:f]
    // 4. [mm:ss:ff]
    // 5. [mm:ss]
    
    // 通过 ":" 分割, 如果分割后的数组元素个数小于2或者大于3, 说明格式有误, 返回 nil
    NSMutableArray *timeComponent = [NSMutableArray arrayWithArray: [triggerTimeString componentsSeparatedByString: @":"]];
    if (timeComponent.count<2 || timeComponent.count>3) return nil;
    
    // 如果 timeCompoent 数量等于2, 说明可能为上面的 第1、第2、第5种情况, 所以需要尝试用 "." 进行毫秒分割
    if (timeComponent.count == 2) {
        NSString *secondComponent = [timeComponent lastObject];
        NSArray *subComponent = [secondComponent componentsSeparatedByString: @"."];
        if (subComponent.count>2) return nil;
        if (subComponent.count == 2) {
            [timeComponent removeObject: secondComponent];
            [timeComponent addObjectsFromArray: subComponent];
        } else {
            [timeComponent addObject: @"00"];
        }
    }
    
    // 如果 timeCompoent 元素数量不等于3, 说明解析有误, 直接进入下一次循环
    if (timeComponent.count != 3) return nil;
    
    // 分别获取分、秒、毫秒
    NSString *min = [timeComponent firstObject];
    NSString *sec = timeComponent[1];
    NSString *mm  = [timeComponent lastObject];
    if (![self ml_isValidTimeString: min] ||
        ![self ml_isValidTimeString: sec] ||
        ![self ml_isValidTimeString: mm ]) return nil; // 处理 [0x0C:-34.50] 这种非法情况
    if (min.length == 1) min = [NSString stringWithFormat:@"0%@", min];
    if (sec.length == 1) sec = [NSString stringWithFormat:@"0%@", sec];
    if (mm.length  == 1) mm  = [NSString stringWithFormat:@"0%@",  mm];
    
    // 获取 歌词触发时间字符串、 歌词触发时间、 歌词内容
    triggerTimeString = [NSString stringWithFormat: @"%@:%@.%@", min, sec, mm]; // 该句歌词的时间字符串.
    NSTimeInterval time = [min integerValue] * 60 + [sec integerValue] + [mm integerValue]/100.0f; // 该句歌词的时间.
    
    return [NSMutableDictionary dictionaryWithDictionary:@{
                                                           kMLLyricScrollView_LyricParse_Key_TimeString:triggerTimeString,
                                                           kMLLyricScrollView_LyricParse_Key_Time:@(time),
                                                           kMLLyricScrollView_LyricParse_Key_LyricContent:lyricContent
                                                           }];
}
```
这部分代码块, 看起来比较多, 不过注释都已经非常清晰了, 在这里就不做过多的赘述了, 大家看代码就好了, 其实很简单. 不过要说一下的是, 代码块中包含了一个 `+ (BOOL)ml_isValidTimeString:(NSString *)timeString` 方法, 主要是用来判断是否是正确的时间格式, 通过这个方法来过滤掉 `[0x0C:-34.50]` 这种非法的时间语句.
Step 2.6
```
// 解析歌词部分
    for (NSString *lyric in arrLycComponents) {
        ...
        for (NSInteger index=0; index<lrcComponents.count-1; index++) {
            ...
            // 在将歌词添加到数组之前, 需要考虑下面这种情况 ⤵️
            // 有些歌词第一句没内容, 所以在这里如果第一句没内容, 则不添加到数组中. (result.count 为0, 则代表第一句)
            // 如果第一句有内容, 为了歌词的显示效果, 第一句的时间应该为 [00:00:00].
            if (!result.count) {
                if (![lrcComponents lastObject].length) continue;
                
                // 修改触发时间
                lyricDict[kMLLyricScrollView_LyricParse_Key_TimeString] = @"00:00.00";
                lyricDict[kMLLyricScrollView_LyricParse_Key_Time] = @(0);
            }
            
            // 保存在数组中
            [result addObject: lyricDict];
        }
        ...
    }
```
Step 3
```
// 解析歌词部分
        ...
    // 如果歌词解析失败, 直接返回空
    if (!result.count) return nil;
    
    // 由于处理了一句歌词多个触发时间的歌词类型, 所以数组中的内容并非有序, 需要做排序操作.
    [result sortUsingComparator:^NSComparisonResult(NSMutableDictionary *_Nonnull obj1, NSMutableDictionary *_Nonnull obj2) {
        return [obj1[kMLLyricScrollView_LyricParse_Key_Time] integerValue] > [obj2[kMLLyricScrollView_LyricParse_Key_Time] integerValue];
    }];
        ...
```
Step 4
```
// 解析歌词部分
      ...
    // 计算每一句歌词的播放时长
    NSMutableDictionary *lastLyricDict = nil;
    for (NSMutableDictionary *lyricDict in result) {
        
        if (lastLyricDict) {
            NSTimeInterval timeInterval = [lyricDict[kMLLyricScrollView_LyricParse_Key_Time] integerValue] - [lastLyricDict[kMLLyricScrollView_LyricParse_Key_Time] integerValue];
            lastLyricDict[kMLLyricScrollView_LyricParse_Key_TimeInterval] = @(timeInterval);
        }
        lastLyricDict = lyricDict;
    }
    
    // 最后一句歌词如果没有歌词内容, 则删除掉最后一句
    if (![lastLyricDict[kMLLyricScrollView_LyricParse_Key_LyricContent] length]) {
        [result removeLastObject];
    }
```
截止到 `Step 4`, 歌词解析部分基本上已经完毕了. 接下来就是将该方法返回的 `NSDictionary` 数组转换成数据模型的操作. 然后显示歌词, `Demo` 中显示歌词使用的是 `MLLyricScrollView.h` 类, 想要成为这个歌词视图的数据源, 需要遵守以下协议, 协议内容如下:
```
/** 作为 MLLyricScrollView 数据源的数据模型, 必须遵守本协议 */
@protocol MLLyricScrollViewDataSourceProtocol <NSObject>
/** 返回歌词内容 */
- (NSString *)ml_lyricContent;
/** 当前这句歌词的播放时长 */
- (NSTimeInterval)ml_timeInterval;
/** 当前这句歌词的触发时间 */
- (NSTimeInterval)ml_triggerTime;
@end
```
任何一个遵守 `MLLyricScrollViewDataSourceProtocol` 协议的数据模型, 都可以作为 `MLLyricScrollView` 的数据源. 具体的代码逻辑的实现部分, 在本文中就不做赘述了, 感兴趣的小伙伴留下自己的邮箱, 我会将 `Demo` 发给你.
## 结语
文章写得比较匆忙, 难免会有表述不清或者偏差的地方, 有什么问题请帮我指出, 我会尽快修改, 谢谢!
**Lemon龙说**:
> 
**如果您在文章中看到了错误 或 误导大家的地方, 请您帮我指出, 我会尽快更改**
> 
**如果您有什么疑问或者不懂的地方, 请留言给我, 我会尽快回复您**
> 
**如果您觉得本文对您有所帮助, 您的喜欢是对我最大的鼓励**
> 
**如果您有好的文章, 可以[投稿](https://www.jianshu.com/collection/e84a7722d673)给我, 让更多的 iOS Developer 在简书这个平台能够更快速的成长**
作者：李国安
链接：https://www.jianshu.com/p/27d6b937e592
来源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
