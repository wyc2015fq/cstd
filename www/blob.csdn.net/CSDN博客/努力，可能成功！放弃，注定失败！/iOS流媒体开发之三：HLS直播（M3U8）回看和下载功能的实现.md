# iOS流媒体开发之三：HLS直播（M3U8）回看和下载功能的实现 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年10月31日 17:02:56[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：2978
尊重知识，转发请注明出处：[iOS流媒体开发之三：HLS直播（M3U8）回看和下载功能的实现](http://www.jianshu.com/p/b0db841ed6d3)
> 
# 概要
流媒体开发第一篇文章就说要把这些不是随便就可以百度到的知识献给**“简书”**，拖了一个多月了，总算弄完了，深深松了口气，万幸没有食言，否则对不起小伙伴们。
流媒体始终是大众生活娱乐最为重要的一个部分，同时也是技术开发中比较有难度的，尤其是直播，不仅功能是点播无法替代的，开发难度也要比点播大，里约奥运会等重大体育赛事大家只能通过直播观看比赛，体会现场观看的紧张和刺激，点播是无法做到的。
如今我们也会有直播回看和下载的需求，一些APP包括我们自己的项目也已经实现了这些功能，网上讲解这部分技术的知识相对较少，而且有很多都不是很靠谱，我这里抛砖引玉，给大家提供一种思路，仅供参考。所以建议大家理解我的思路，尽量不要直接拿来用在项目里，后面我会详细讲解有哪些地方在应用到项目中需要额外的处理。
`注意：1、本文不适合初级iOS开发者，需要有一定的开发经验，和对流媒体技术的基本概念和开发技术的了解，例如本文不会讲解什么是TS、AAC和M3U8等概念，这些知识网上很多，大家可以自行查阅理解，这里就赘述了；2、直播的回看和下载相对于音视频的播放开发难度要大一些，数据处理的思路也比较复杂，所以为了大家能更快的理解和接受，本文着重核心功能的讲解，以免过多的代码对理解产生干扰，比如我们拿到一个M3U8链接，我们要判断这个链接是否是http或者https的，其次要去除链接中的空白字符，注意空白字符不一定是空格，还有可能是回车、TAB等其他的空白字符，处理起来也比较繁琐，本文不对这些做过多处理，默认M3U8链接是有效的，小伙伴们在实际项目中要对这些地方做处理，避免因此出现bug；3、鉴于HLS直播的回看和下载网上可参考的资料太少，如果观看本文的小伙伴有更好的实现方案，欢迎留言，对本文的实现方案提出建议，感激不尽。`
> 
# 回看
HLS直播的回看功能有2种实现方案，2种方案都需要借助服务器。
1、第一种方案是服务器将实时获取的TS（**AAC音频处理流程一样，后面不赘述**）文件片段存储到指定的路径下，当客户端请求某一时间段的回看节目时，服务器取出相对应的TS，打包这些TS片段生成.M3U8索引文件和播放链接，返回给客户端，这是客户端拿到的播放链接和直播的链接是一样的，播放的处理流程也是一样的，只不过这时的直播只能播放一段时间。
2、第二种方案是服务器将制定节目的直播内容使用FFMPEG转码成MP4和3GP等点播源，生成播放连接返回给客户端播放就可以了。
`注意：由于回看要借助服务器实现，这里就不附上实现的代码了，客户端的实现比较简单，拿到播放源直接播放就可以了，后面要讲的下载和回看的第一种方案是一样的，都是将TS片段下载下来，可以参考后面的内容。`
3、两中方案的优缺点分析：
①第一种方案对于服务器来说处理比较简单，只需要将TS存储并打包即可。对于客户端来说播放很简单，同时HLS的传输效率也要更高一些，播放速度会很快，但是涉及到调整视频进度、截取视频某一帧图片，监听视频播放状态这些就比较麻烦了。回看的内容虽然也是直播的内容，但是在用户看来无所谓点播和直播，这些已经是播放过的节目，自然可以调整进度。这里给出一种调整进度的方案，根据客户端的时间戳向服务器获取相应的TS片段。例如下面这个链接：
`self.playerUrl = @"http://cctv2.vtime.cntv.wscdns.com:8000/live/no/204_/seg0/index.m3u8?begintime=1469509516000";`
这个链接有一个参数：begintime，从命名我们可以看出是要传输一个播放源从哪里开始播放的时间戳，服务器拿到这个参数后会生成对应的数据返回给客户端播放，这里就可以实现精准的进度控制了。
②第二种方案对于服务器来说要繁琐些，多了一步制作点播源的步骤。对于客户端，第二种方案的好处是直接拿到的是点播的播放源，无论是进度调整、获取帧率图和播放状态的控制都很简单，虽然播放速度相对与HLS来说会慢一点，但影响并不大。同时由于服务器已经将每一个节目转码成功，如果用户要下载这些节目观看，客户端的实现也比较简单。这种方案的缺点是不够灵活，用户只能以节目为时间单位进行回看，无法像第一种方案一样，以时间戳为单位回看，精细度不够。
`总结两种回看方案并没有优略之分，具体采用哪一种，要看具体项目的需求，小伙伴们在开发过程中要注意和服务器的联调测试，尤其是第一种方案，M3U8的各种tag设置的不准确也会造成各种播放错误，并没有那么容易实现，当然服务器那边也会有一些第三方库可以直接用，所以对于有些开发经验的服务器工程师还是比较容易实现的。`
> 
# 下载
下载的流程比较复杂，为了让小伙伴更容易理解，我不会按照我的代码一步步讲解，这样只会让人头晕脑胀，意义不大。我这里按照我在学习新知识时比较容易理解知识的经验来讲解。
我们在学习时，如果只是拿来别人的代码一行行看，遇到不会的查阅，然后再下面的，没一会就头晕了，相信大家都有过这种经验，效果非常差，而且作者在写这些代码的时候并不是逐字逐行的写的，而是一次次优化改动得来的，通过代码我们很难明白作者写代码的逻辑和心路历程，自控力强的多看几遍屡清楚思路能看明白，自控力稍差的可能就放弃了，下面讲解下我的讲解思路和学习方法。
###### *学习思路
①首先我会说明HLS下载的实现思路，小伙伴们在看这部分的时候不要把自己当成技术人员，各行各业最有价值的都是解决问题的思想和能力，而不是代码、文字和各种工具等，所以我尽量让一个没有任何开发技术的人明白HLS下载的逻辑，明白了解决问题的逻辑，再看后面的代码就不至于晕头转向了；
②其次我会按照流程逐步讲解，在讲解每一步流程时，每一步也是一个相对独立的子流程，我也会大概的描述下每一步子流程的实现思路，小伙伴们理解起来也会更加简单；
③最后说下小伙伴们在阅读时的一些注意事项。在对核心功能还没有充分理解的前提下，不要太在意一些技术细节，比如这里为什么调用这个方法、这样做性能不太高等等和核心功能无关的。等小伙伴们对核心功能理解了，再来优化和理解一些小的地方，才会得心应手。由于我们写这些代码的时候考虑的也不是很健全，所有会有很多地方写得不完美，也欢迎小伙伴们留言指出来，绝对知错就改，感激不尽。
###### *实现思路
实现思路可以分为4大步：解码、下载、打包、播放。
**解码：**拿到一个M3U8链接后解析出M3U8索引的具体内容，包括每一个TS的下载链接、时长等；
**下载：**拿到每一个TS文件的链接就可以逐个下载了，下载后存储到手机里；
**打包：**将下载的TS数据按照播放顺序打包，供客户端播放；
**播放：**数据打包完成，就可以播放了。
`说明:1、本文借鉴了iOS端M3U8第三方库的处理流程，由于这个第三方库长时间没有维护和更新，并且采用了ASI作为网络请求，直接采用会给项目带来大量的警告和错误，还会导致无法适配各种架构等问题，处理起来很是繁琐和棘手，并且即使配置成功，也是无法直接使用的，还是需要改动第三方库的很多地方，所以我这里模仿M3U8库的部分处理逻辑，同时网络请求使用AFN，当然这里建议大家对AFN做一层封装后再使用，避免AFN升级换代带来不必要的麻烦。2、本文封装了一个名为“ZYLDecodeTool”的工具类，负责调度每一步。`
![](http://upload-images.jianshu.io/upload_images/1347742-e9893ab5203ff2a7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
HLS下载流程
###### *解码
解码这一步就做一件事情，拿到播放链接，读取M3U8索引文件，解析出每一个TS文件的下载地址和时长，封装到Model中，供后面使用。
解码器ZYLM3U8Handler.h文件
```
#import <Foundation/Foundation.h>
#import "M3U8Playlist.h"
@class ZYLM3U8Handler;
@protocol ZYLM3U8HandlerDelegate <NSObject>
/**
 * 解析M3U8连接失败
 */
- (void)praseM3U8Finished:(ZYLM3U8Handler *)handler;
/**
 * 解析M3U8成功
 */
- (void)praseM3U8Failed:(ZYLM3U8Handler *)handler;
@end
@interface ZYLM3U8Handler : NSObject
/**
 * 解码M3U8
 */
- (void)praseUrl:(NSString *)urlStr;
/**
 * 传输成功或者失败的代理
 */
@property (weak, nonatomic)id <ZYLM3U8HandlerDelegate> delegate;
/**
 * 存储TS片段的数组
 */
@property (strong, nonatomic) NSMutableArray *segmentArray;
/**
 * 打包获取的TS片段
 */
@property (strong, nonatomic) M3U8Playlist *playList;
/**
 * 存储原始的M3U8数据
 */
@property (copy, nonatomic) NSString *oriM3U8Str;
@end
```
ZYLM3U8Handler.m文件
```
#import "ZYLM3U8Handler.h"
#import "M3U8SegmentModel.h"
@implementation ZYLM3U8Handler
#pragma mark - 解析M3U8链接
- (void)praseUrl:(NSString *)urlStr {
    //判断是否是HTTP连接
    if (!([urlStr hasPrefix:@"http://"] || [urlStr hasPrefix:@"https://"])) {
        if (self.delegate != nil && [self.delegate respondsToSelector:@selector(praseM3U8Failed:)]) {
            [self.delegate praseM3U8Failed:self];
        }
        return;
    }
    //解析出M3U8
    NSError *error = nil;
    NSStringEncoding encoding;
    NSString *m3u8Str = [[NSString alloc] initWithContentsOfURL:[NSURL URLWithString:urlStr] usedEncoding:&encoding error:&error];//这一步是耗时操作，要在子线程中进行
self.oriM3U8Str = m3u8Str;
    /*注意1、请看代码下方注意1*/
    if (m3u8Str == nil) {
        if (self.delegate != nil && [self.delegate respondsToSelector:@selector(praseM3U8Failed:)]) {
                [self.delegate praseM3U8Failed:self];
        }
        return;
    }
    //解析TS文件
    NSRange segmentRange = [m3u8Str rangeOfString:@"#EXTINF:"];
    if (segmentRange.location == NSNotFound) {
        //M3U8里没有TS文件
        if (self.delegate != nil && [self.delegate respondsToSelector:@selector(praseM3U8Failed:)]) {
                [self.delegate praseM3U8Failed:self];
        }
        return;
    }
if (self.segmentArray.count > 0) {
    [self.segmentArray removeAllObjects];
}
    //逐个解析TS文件，并存储
    while (segmentRange.location != NSNotFound) {
        //声明一个model存储TS文件链接和时长的model
        M3U8SegmentModel *model = [[M3U8SegmentModel alloc] init];
        //读取TS片段时长
        NSRange commaRange = [m3u8Str rangeOfString:@","];
        NSString* value = [m3u8Str substringWithRange:NSMakeRange(segmentRange.location + [@"#EXTINF:" length], commaRange.location -(segmentRange.location + [@"#EXTINF:" length]))];
        model.duration = [value integerValue];
        //截取M3U8
        m3u8Str = [m3u8Str substringFromIndex:commaRange.location];
        //获取TS下载链接,这需要根据具体的M3U8获取链接，可以根据自己公司的需求
        NSRange linkRangeBegin = [m3u8Str rangeOfString:@","];
        NSRange linkRangeEnd = [m3u8Str rangeOfString:@".ts"];
        NSString* linkUrl = [m3u8Str substringWithRange:NSMakeRange(linkRangeBegin.location + 2, (linkRangeEnd.location + 3) - (linkRangeBegin.location + 2))];
        model.locationUrl = linkUrl;
        [self.segmentArray addObject:model];
        m3u8Str = [m3u8Str substringFromIndex:(linkRangeEnd.location + 3)];
        segmentRange = [m3u8Str rangeOfString:@"#EXTINF:"];
    }
    /*注意2、请看代码下方注意2*/
    //已经获取了所有TS片段，继续打包数据
    [self.playList initWithSegmentArray:self.segmentArray];
    self.playList.uuid = @"moive1";
    //到此数据TS解析成功，通过代理发送成功消息
    if (self.delegate != nil && [self.delegate respondsToSelector:@selector(praseM3U8Finished:)]) {
            [self.delegate praseM3U8Finished:self];
    }
}
#pragma mark - getter
- (NSMutableArray *)segmentArray {
if (_segmentArray == nil) {
    _segmentArray = [[NSMutableArray alloc] init];
}
return _segmentArray;
}
- (M3U8Playlist *)playList {
if (_playList == nil) {
    _playList = [[M3U8Playlist alloc] init];
}
return _playList;
}
@end
```
注意：
1、下面就是解析出来的M3U8索引数据，#EXTINF:10表示的是这段TS的时长是10秒，57b3f432.ts这里表示的是每一个TS的文件名，有的M3U8这里直接是一个完成的http链接。前面说到我们要拼接处每一个TS文件的下载链接，这里应该如何拼接呢，在一开始做这里的时候，我也费解了一段时间，查阅了一些资料和博文都不靠谱，所以不建议大家根据这些不靠谱的信息拼接链接，我这里总结出来的经验是，TS文件一般都存储在.M3U8索引文件所在的路径，只需要将TS文件名替换到.M3U8索引即可，当然最靠谱的做法和你们的服务器小伙伴协商好下载路径。
```cpp
```cpp
#EXTM3U
     #EXT-X-VERSION:2
     #EXT-X-MEDIA-SEQUENCE:102
     #EXT-X-TARGETDURATION:12
     #EXTINF:10,
     57b3f432.ts
     #EXTINF:12,
     57b3f43c.ts
     #EXTINF:9,
     57b3f446.ts
```
```
2、M3U8Playlist是一个存储一个M3U8数据的Model，存储的是TS下载链接数组，数组的数量。uuid设置为固定的"moive1"，主要是用来拼接统一的缓存路径。
###### *下载
拿到每一个TS的链接就可以下载了，下载后缓存到本地。
下载器ZYLVideoDownLoader.h文件
```
#import <Foundation/Foundation.h>
#import "M3U8Playlist.h"
    @class ZYLVideoDownLoader;
    @protocol ZYLVideoDownLoaderDelegate <NSObject>
    /**
     * 下载成功
     */
    - (void)videoDownloaderFinished:(ZYLVideoDownLoader *)videoDownloader;
    /**
     * 下载失败
     */
    - (void)videoDownloaderFailed:(ZYLVideoDownLoader *)videoDownloader;
    @end
    @interface ZYLVideoDownLoader : NSObject
    @property (strong, nonatomic) M3U8Playlist *playList;
    /**
     * 记录原始的M3U8
     */
    @property (copy, nonatomic) NSString *oriM3U8Str;
    /**
     * 下载TS数据
     */
    - (void)startDownloadVideo;
    /**
     * 储存正在下载的数组
     */
    @property (strong, nonatomic) NSMutableArray *downLoadArray;
    /**
     * 下载成功或者失败的代理
     */
    @property (weak, nonatomic) id <ZYLVideoDownLoaderDelegate> delegate;
    /**
     * 创建M3U8文件
     */
    - (void)createLocalM3U8file;
    @end
```
下载器ZYLVideoDownLoader.m文件
```
#import "ZYLVideoDownLoader.h"
#import "M3U8SegmentModel.h"
#import "SegmentDownloader.h"
    @interface ZYLVideoDownLoader () <SegmentDownloaderDelegate>
    @property (assign, nonatomic) NSInteger index;//记录一共多少TS文件
    @property (strong, nonatomic) NSMutableArray *downloadUrlArray;//记录所有的下载链接
    @property (assign, nonatomic) NSInteger sIndex;//记录下载成功的文件的数量
    @end
    @implementation ZYLVideoDownLoader
    -(instancetype)init {
        self = [super init];
        if (self) {
            self.index = 0;
            self.sIndex = 0;
        }
        return self;
    }
#pragma mark - 下载TS数据
    - (void)startDownloadVideo {
        //首相检查是否存在路径
        [self checkDirectoryIsCreateM3U8:NO];
        __weak __typeof(self)weakSelf = self;
/*注意1，请看下方注意1*/
        //将解析的数据打包成一个个独立的下载器装进数组
        [self.playList.segmentArray enumerateObjectsUsingBlock:^(M3U8SegmentModel *obj, NSUInteger idx, BOOL * _Nonnull stop) {
            //检查此下载对象是否存在
            __block BOOL isE = NO;
            [weakSelf.downloadUrlArray enumerateObjectsUsingBlock:^(NSString *inObj, NSUInteger inIdx, BOOL * _Nonnull inStop) {
                if ([inObj isEqualToString:obj.locationUrl]) {
                    //已经存在
                    isE = YES;
                    *inStop = YES;
                } else {
                    //不存在
                    isE = NO;
                }
            }];
            if (isE) {
                //存在
            } else {
                //不存在
                NSString *fileName = [NSString stringWithFormat:@"id%ld.ts", (long)weakSelf.index];
                SegmentDownloader *sgDownloader = [[SegmentDownloader alloc] initWithUrl:[@"http://111.206.23.22:55336/tslive/c25_ct_btv2_btvwyHD_smooth_t10/" stringByAppendingString:obj.locationUrl] andFilePath:weakSelf.playList.uuid andFileName:fileName withDuration:obj.duration withIndex:weakSelf.index];
                sgDownloader.delegate = weakSelf;
                [weakSelf.downLoadArray addObject:sgDownloader];
                [weakSelf.downloadUrlArray addObject:obj.locationUrl];
                weakSelf.index++;
            }
        }];
         /*注意2，请看下方注意2*/
        //根据新的数据更改新的playList
        __block NSMutableArray *newPlaylistArray = [[NSMutableArray alloc] init];
        [self.downLoadArray enumerateObjectsUsingBlock:^(SegmentDownloader *obj, NSUInteger idx, BOOL * _Nonnull stop) {
            M3U8SegmentModel *model = [[M3U8SegmentModel alloc] init];
            model.duration = obj.duration;
            model.locationUrl = obj.fileName;
            model.index = obj.index;
            [newPlaylistArray addObject:model];
        }];
        if (newPlaylistArray.count > 0) {
            self.playList.segmentArray = newPlaylistArray;
        }
        //打包完成开始下载
        [self.downLoadArray enumerateObjectsUsingBlock:^(SegmentDownloader *obj, NSUInteger idx, BOOL * _Nonnull stop) {
            obj.flag = YES;
            [obj start];
        }];
    }
#pragma mark - 检查路径
    - (void)checkDirectoryIsCreateM3U8:(BOOL)isC {
        //创建缓存路径
        NSString *pathPrefix = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,NSUserDomainMask,YES) objectAtIndex:0];
        NSString *saveTo = [[pathPrefix stringByAppendingPathComponent:@"Downloads"] stringByAppendingPathComponent:self.playList.uuid];
        NSFileManager *fm = [NSFileManager defaultManager];
        //路径不存在就创建一个
        BOOL isD = [fm fileExistsAtPath:saveTo];
        if (isD) {
            //存在
        } else {
            //不存在
            BOOL isS = [fm createDirectoryAtPath:saveTo withIntermediateDirectories:YES attributes:nil error:nil];
            if (isS) {
                NSLog(@"路径不存在创建成功");
            } else {
                NSLog(@"路径不存在创建失败");
            }
        }
    }
#pragma mark - SegmentDownloaderDelegate
 /*注意3，请看下方注意3*/
#pragma mark - 数据下载成功
    - (void)segmentDownloadFinished:(SegmentDownloader *)downloader {
        //数据下载成功后再数据源中移除当前下载器
        self.sIndex++;
        if (self.sIndex >= 3) {
            //每次下载完成后都要创建M3U8文件
            [self createLocalM3U8file];
            //证明所有的TS已经下载完成
            [self.delegate videoDownloaderFinished:self];
        }
    }
#pragma mark - 数据下载失败
    - (void)segmentDownloadFailed:(SegmentDownloader *)downloader {
        [self.delegate videoDownloaderFailed:self];
    }
#pragma mark - 进度更新
    - (void)segmentProgress:(SegmentDownloader *)downloader TotalUnitCount:(int64_t)totalUnitCount completedUnitCount:(int64_t)completedUnitCount {
        //NSLog(@"下载进度：%f", completedUnitCount * 1.0 / totalUnitCount * 1.0);
    }
 /*注意4，请看下方注意4*/
#pragma mark - 创建M3U8文件
            - (void)createLocalM3U8file {
        [self checkDirectoryIsCreateM3U8:YES];
        //创建M3U8的链接地址
        NSString *path = [[[[NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,NSUserDomainMask,YES) objectAtIndex:0] stringByAppendingPathComponent:@"Downloads"] stringByAppendingPathComponent:self.playList.uuid] stringByAppendingPathComponent:@"movie.m3u8"];
        //拼接M3U8链接的头部具体内容
        //NSString *header = @"#EXTM3U\n#EXT-X-VERSION:2\n#EXT-X-MEDIA-SEQUENCE:371\n#EXT-X-TARGETDURATION:12\n";
        NSString *header = [NSString stringWithFormat:@"#EXTM3U\n#EXT-X-VERSION:3\n#EXT-X-MEDIA-SEQUENCE:0\n#EXT-X-TARGETDURATION:15\n"];
        //填充M3U8数据
        __block NSString *tsStr = [[NSString alloc] init];
        [self.playList.segmentArray enumerateObjectsUsingBlock:^(M3U8SegmentModel *obj, NSUInteger idx, BOOL * _Nonnull stop) {
            //文件名
            NSString *fileName = [NSString stringWithFormat:@"id%ld.ts", obj.index];
            //文件时长
            NSString* length = [NSString stringWithFormat:@"#EXTINF:%ld,\n",obj.duration];
            //拼接M3U8
            tsStr = [tsStr stringByAppendingString:[NSString stringWithFormat:@"%@%@\n", length, fileName]];
        }];
        //M3U8头部和中间拼接,到此我们完成的新的M3U8链接的拼接
        header = [header stringByAppendingString:tsStr];
/*注意5，请看下方注意5*/
        header = [header stringByAppendingString:@"#EXT-X-ENDLIST"];
        //拼接完成，存储到本地
        NSMutableData *writer = [[NSMutableData alloc] init];
        NSFileManager *fm = [NSFileManager defaultManager];
        //判断m3u8是否存在,已经存在的话就不再重新创建
        if ([fm fileExistsAtPath:path isDirectory:nil]) {
            //存在这个链接
            NSLog(@"存在这个链接");
        } else {
            //不存在这个链接
            NSString *saveTo = [[[NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,NSUserDomainMask,YES) objectAtIndex:0] stringByAppendingPathComponent:@"Downloads"] stringByAppendingPathComponent:self.playList.uuid];
            BOOL isS = [fm createDirectoryAtPath:saveTo withIntermediateDirectories:YES attributes:nil error:nil];
            if (isS) {
                NSLog(@"创建目录成功");
            } else {
                NSLog(@"创建目录失败");
            }
        }
        [writer appendData:[header dataUsingEncoding:NSUTF8StringEncoding]];
        BOOL bSucc = [writer writeToFile:path atomically:YES];
        if (bSucc) {
            //成功
            NSLog(@"M3U8数据保存成功");
        } else {
            //失败
            NSLog(@"M3U8数据保存失败");
        }
        NSLog(@"新数据\n%@", header);
    }
#pragma mark - 删除缓存文件
    - (void)deleteCache {
        //获取缓存路径
        NSString *pathPrefix = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,NSUserDomainMask,YES) objectAtIndex:0];
        NSString *saveTo = [[pathPrefix stringByAppendingPathComponent:@"Downloads"] stringByAppendingPathComponent:@"moive1"];
        NSFileManager *fm = [NSFileManager defaultManager];
        //路径不存在就创建一个
        BOOL isD = [fm fileExistsAtPath:saveTo];
        if (isD) {
            //存在
            NSArray *deleteArray = [_downloadUrlArray subarrayWithRange:NSMakeRange(0, _downloadUrlArray.count - 20)];
            //清空当前的M3U8文件
            [deleteArray enumerateObjectsUsingBlock:^(NSString *obj, NSUInteger idx, BOOL * _Nonnull stop) {
                BOOL isS = [fm removeItemAtPath:[saveTo stringByAppendingPathComponent:[NSString stringWithFormat:@"%@", obj]] error:nil];
                if (isS) {
                    NSLog(@"多余路径存在清空成功%@", obj);
                } else {
                    NSLog(@"多余路径存在清空失败%@", obj);
                }
            }];
        }
    }
#pragma mark - getter
    - (NSMutableArray *)downLoadArray {
        if (_downLoadArray == nil) {
            _downLoadArray = [[NSMutableArray alloc] init];
        }
        return _downLoadArray;
    }
    - (NSMutableArray *)downloadUrlArray {
        if (_downloadUrlArray == nil) {
            _downloadUrlArray = [[NSMutableArray alloc] init];
        }
        return _downloadUrlArray;
    }
    @end
```
注意：
1、这里获取到的M3U8数据包含了很多TS文件，并不会在下载器里直接下载，而是要对每一个TS文件再次封装，然后每一个封装好的数据模型单独下载；
2、这里更新playlist的目的是为了后续创建.M3U8索引，可以暂时略过这里，到了创建索引的地方自然就懂了；
3、这是数据下载成功的代理，由于本文使用的测试连接每一个M3U8里有3个TS文件，所以当第一次3个文件全部下载完成后告诉系在工具类下载完成，后续没下载完成一个就告诉下载工具类一次；
4、在第一次3个TS文件下载成功和后续每有一个TS下载成功后，都会更新.M3U8索引文件，保证索引文件的更新；
5、这里要注意，添加了#EXT-X-ENDLIST，表明这个源事HLS的点播源，当播放的时候，HLS会从头开始播放。
###### *TS文件下载器
上面的下载器将每一个TS文件单独封装，单独下载，下面我们来看看每一个TS文件是如何下载的
TS文件下载器 SegmentDownloader.h文件
```
#import <Foundation/Foundation.h>
@class SegmentDownloader;
@protocol SegmentDownloaderDelegate <NSObject>
/**
 * 下载成功
 */
- (void)segmentDownloadFinished:(SegmentDownloader *)downloader;
/**
 * 下载失败
 */
- (void)segmentDownloadFailed:(SegmentDownloader *)downloader;
/**
 * 监听进度
 */
- (void)segmentProgress:(SegmentDownloader *)downloader TotalUnitCount:(int64_t)totalUnitCount completedUnitCount:(int64_t)completedUnitCount;
@end
@interface SegmentDownloader : NSObject
@property (nonatomic, copy) NSString *fileName;
@property (nonatomic, copy) NSString *filePath;
@property (nonatomic, copy) NSString *downloadUrl;
@property (assign, nonatomic) NSInteger duration;
@property (assign, nonatomic) NSInteger index;
/**
 * 标记这个下载器是否正在下载
 */
@property (assign, nonatomic) BOOL flag;
/**
 * 初始化TS下载器
 */
- (instancetype)initWithUrl:(NSString *)url andFilePath:(NSString *)path andFileName:(NSString *)fileName withDuration:(NSInteger)duration withIndex:(NSInteger)index;
/**
 * 传递数据下载成功或者失败的代理
 */
@property (strong, nonatomic) id <SegmentDownloaderDelegate> delegate;
/**
 * 开始下载
 */
- (void)start;
@end
```
TS文件下载器 SegmentDownloader.m文件
```
#import "SegmentDownloader.h"
#import <AFNetworking.h>
@interface SegmentDownloader ()
@property (strong, nonatomic) AFHTTPRequestSerializer *serializer;
@property (strong, nonatomic) AFURLSessionManager *downLoadSession;
@end
@implementation SegmentDownloader
#pragma mark - 初始化TS下载器
- (instancetype)initWithUrl:(NSString *)url andFilePath:(NSString *)path andFileName:(NSString *)fileName withDuration:(NSInteger)duration withIndex:(NSInteger)index {
    self = [super init];
    if (self) {
        self.downloadUrl = url;
        self.filePath = path;
        self.fileName = fileName;
        self.duration = duration;
        self.index = index;
    }
    return self;
}
#pragma mark - 开始下载
- (void)start {
    //首先检查此文件是否已经下载
    if ([self checkIsDownload]) {
        //下载了
        [self.delegate segmentDownloadFinished:self];
        return;
    } else {
        //没下载
    }
    //首先拼接存储数据的路径
    __block NSString *path = [[[[NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,NSUserDomainMask,YES) objectAtIndex:0] stringByAppendingPathComponent:@"Downloads"] stringByAppendingPathComponent:self.filePath] stringByAppendingPathComponent:self.fileName];
    /*注意1，请查看下方注意1*/
    //这里使用AFN下载,并将数据同时存储到沙盒目录制定的目录中
    NSURLRequest *request = [NSURLRequest requestWithURL:[NSURL URLWithString:self.downloadUrl]];
    __block NSProgress *progress = nil;
    NSURLSessionDownloadTask *downloadTask = [self.downLoadSession downloadTaskWithRequest:request progress:&progress destination:^NSURL * _Nonnull(NSURL * _Nonnull targetPath, NSURLResponse * _Nonnull response) {
        //在这里告诉AFN数据存储的路径和文件名
        NSURL *documentsDirectoryURL = [NSURL fileURLWithPath:path isDirectory:NO];
        return documentsDirectoryURL;
    } completionHandler:^(NSURLResponse * _Nonnull response, NSURL * _Nullable filePath, NSError * _Nullable error) {
        if (error == nil) {
            //下载成功
            //NSLog(@"路径%@保存成功", filePath);
            [self.delegate segmentDownloadFinished:self];
        } else {
            //下载失败
            [self.delegate segmentDownloadFailed:self];
        }
        [progress removeObserver:self forKeyPath:@"completedUnitCount"];
    }];
    //添加对进度的监听
    [progress addObserver:self forKeyPath:@"completedUnitCount" options:NSKeyValueObservingOptionNew context:nil];
    //开始下载
    [downloadTask resume];
}
#pragma mark - 检查此文件是否下载过
- (BOOL)checkIsDownload {
    //获取缓存路径
    NSString *pathPrefix = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,NSUserDomainMask,YES) objectAtIndex:0];
    NSString *saveTo = [[pathPrefix stringByAppendingPathComponent:@"Downloads"] stringByAppendingPathComponent:self.filePath];
    NSFileManager *fm = [NSFileManager defaultManager];
    __block BOOL isE = NO;
    //获取缓存路径下的所有的文件名
    NSArray *subFileArray = [fm subpathsAtPath:saveTo];
    [subFileArray enumerateObjectsUsingBlock:^(id  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
        //判断是否已经缓存了此文件
        if ([self.fileName isEqualToString:[NSString stringWithFormat:@"%@", obj]]) {
            //已经下载
            isE = YES;
            *stop = YES;
        } else {
            //没有存在
            isE = NO;
        }
    }];
    return isE;
}
#pragma mark - 监听进度
- (void)observeValueForKeyPath:(NSString *)keyPath ofObject:(NSProgress *)object change:(NSDictionary *)change context:(void *)context
{
    if ([keyPath isEqualToString:@"completedUnitCount"]) {
        [self.delegate segmentProgress:self TotalUnitCount:object.totalUnitCount completedUnitCount:object.completedUnitCount];
    }
}
#pragma mark - getter
- (AFHTTPRequestSerializer *)serializer {
    if (_serializer == nil) {
        _serializer = [AFHTTPRequestSerializer serializer];
    }
    return _serializer;
}
- (AFURLSessionManager *)downLoadSession {
    if (_downLoadSession == nil) {
        NSURLSessionConfiguration *configuration = [NSURLSessionConfiguration defaultSessionConfiguration];
        _downLoadSession = [[AFURLSessionManager alloc] initWithSessionConfiguration:configuration];
    }
    return _downLoadSession;
}
@end
```
注意：
1、这里使用AFN的AFURLSessionManager下载数据并缓存数据到本地，同时可以通过这里获得下载的进度；
2、由于这里是自己下载TS文件，所有若是我们的项目中有直接操作视频数据的需求，就可以在这里获取视频数据进行处理了。具体的下载流程，大家参考代码即可。
3、为了直观的看到TS文件的下载过程，小伙伴们可以在模拟器上运行DEMO，然后进入到沙盒目录下，可以看到数据的实时更新，如下图：
![](http://upload-images.jianshu.io/upload_images/1347742-4c29462bcdb8a00f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/520)
TS文件下载过程
![](http://upload-images.jianshu.io/upload_images/1347742-7f60647d21207225.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/520)
TS文件下载过程
> 
# 播放
TS文件下载完成了，.M3U8索引文件也创建好了，那么如何播放呢，看着一段段零散的TS文件，我们难道要一段段播放给用户看吗？这样显然不合理，这里我们要使用HLS直播播放技术，模拟服务器和客户端的交互的过程，所以我们在本地建立一个http服务器，让HLS访问本地的http服务器就可以播放了，下面看看具体的实现过程
###### *建立本地的http服务器
这里我们使用iOS端很有名也很好用的CocoaHTTPServer第三方库建立http服务器，可以直接cocoaPods导入工程，导入后创建服务器，代码如下：
```
- (void)openServer {
    [DDLog addLogger:[DDTTYLogger sharedInstance]];
    self.httpServer=[[HTTPServer alloc]init];
    [self.httpServer setType:@"_http._tcp."];
    [self.httpServer setPort:9479];
    NSString *pathPrefix = [NSSearchPathForDirectoriesInDomains(NSDocumentDirectory,NSUserDomainMask,YES) objectAtIndex:0];
    NSString *webPath = [pathPrefix stringByAppendingPathComponent:@"Downloads"];
    [self.httpServer setDocumentRoot:webPath];
    NSLog(@"服务器路径：%@", webPath);
    NSError *error;
    if ([self.httpServer start:&error]) {
        NSLog(@"开启HTTP服务器 端口:%hu",[self.httpServer listeningPort]);
    }
    else{
        NSLog(@"服务器启动失败错误为:%@",error);
    }
}
```
注意：
1、[self.httpServer setPort:9479];这里是设置服务器端口，端口号写一个不容易重复的即可，避免用户手机其他APP也建立了端口号一样的服务器，导致服务器建立失败，或者数据混乱，另外用模拟器在本地建立的服务器，是直接建立的mac上的，可以把播放链接直接给vlc打开播放；
2、[self.httpServer setDocumentRoot:webPath];这一步在给服务器设置路径的时候，一定要注意和缓存TS数据的路径一致；
3、解码工具类中使用了一些定时器，小伙伴们在使用的时候，要记得声明一个销毁解码工具类的方法，在这个方法里销毁定时器等，避免页面无法销毁的bug。
###### *播放
服务器页建立好了，那么播放链接是什么呢？懂一些网络技术的小伙伴可能已经猜到了，服务器是建立在本地的，网络里127.0.0.1是本地IP地址，因此播放连接是：@"[http://127.0.0.1:9479/moive1/movie.m3u8"，](http://127.0.0.1:9479/moive1/movie.m3u8%22%EF%BC%8C) 将这个连接直接交给AVPlayer就可以播放了，用VLC打开，不仅可以播放，还可以调整进度。当下载了一些文件后，退出APP，即使在没有网络的情况下打开，也可以正常播放，如图：
![](http://upload-images.jianshu.io/upload_images/1347742-fcab2633f43e971e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/520)
手机播放
![](http://upload-images.jianshu.io/upload_images/1347742-356fefb2f14e33fd.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/520)
VLC播放
> 
# 尾巴
到这里我们已经实现了M3U8直播的回看和下载，DEMO下载地址：[Demo](https://github.com/yueshangfeng/huikan.git)。
本文为小伙伴们提供了一种思路，整个实现过程还是有些复杂的，需要小伙伴们反复理解，当然有一定的音视频开发技术理解起来就简单多了，本文并没有对M3U8做过多技术讲解，这方面的知识可以查阅苹果官方文档：[HLS苹果官方资料](https://developer.apple.com/streaming/)，这里只是挑出一些问题讲解一下，最终能否理解还要靠小伙伴们自己的努力，若在文中发现错误请及时指正，感激不尽。
文／张云龙（简书作者）
原文链接：http://www.jianshu.com/p/b0db841ed6d3
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
