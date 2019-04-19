# iOS实现断点续传 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年11月22日 09:10:42[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：56
网络下载是我们在项目中经常要用到的功能，如果是小文件的下载，比如图片和文字之类的，我们可以直接请求源地址，然后一次下载完毕。但是如果是下载较大的音频和视频文件，不可能一次下载完毕，用户可能下载一段时间，关闭程序，回家接着下载。这个时候，就需要实现断点续传的功能。让用户可以随时暂停下载，下次开始下载，还能接着上次的下载的进度。
今天我们来看看如何自己简单的封装一个断点续传的类，实现如下功能。
- 使用者只需要调用一个接口即可以下载，同时可以获取下载的进度。
- 下载成功，可以获取文件存储的位置
- 下载失败，给出失败的原因
- 可以暂停下载，下次开始下载，接着上次的进度继续下载
### 原理讲解
要实现断点续传的功能，通常都需要客户端记录下当前的下载进度，并在需要续传的时候通知服务端本次需要下载的内容片段。
在HTTP1.1协议（RFC2616）中定义了断点续传相关的HTTP头的Range和Content-Range字段，一个最简单的断点续传实现大概如下：
- 客户端下载一个1024K的文件，已经下载了其中512K
- 网络中断，客户端请求续传，因此需要在HTTP头中申明本次需要续传的片段：
	Range:bytes=512000-
	这个头通知服务端从文件的512K位置开始传输文件
- 服务端收到断点续传请求，从文件的512K位置开始传输，并且在HTTP头中增加：
	Content-Range:bytes 512000-/1024000
	并且此时服务端返回的HTTP状态码应该是206，而不是200。
### 难点说明
1. 客户端如何获取已经下载的文件字节数
客户端这边，我们需要记录下每次用户每次下载的文件大小，然后实现原理讲解中步骤1的功能。
那么如何记载呢？
其实我们可以直接获取指定路径下文件的大小，iOS已经提供了相关的功能，实现代码如下，
```
[[[NSFileManager defaultManager] attributesOfItemAtPath: FileStorePath error:nil][NSFileSize] integerValue]
```
2.如何获取被下载文件的总字节数
上一步，我们获取了已经下载文件的字节数，这里我们需要获取被下载文件的总字节数，有了这两个值，我们就可以算出下载进度了。
那么如何获取呢？这里我们需要用到http 头部的conten-length字段，先来看看该字段的含义
> 
Content-Length用于描述HTTP消息实体的传输长度the transfer-length of the message-body。在HTTP协议中，消息实体长度和消息实体的传输长度是有区别，比如说gzip压缩下，消息实体长度是压缩前的长度，消息实体的传输长度是gzip压缩后的长度。
简单点说，content-length表示被下载文件的字节数。
对比原理讲解的第三步，我们可以看到如果要计算出文件的总字节数，那么必须把已经下载的字节数 加上 content-length。
我们需要把每个被下载文件的总字节数存储起来，这里我们选择使用plist文件来记载，plist文件包含一个字典。设置文件名为键值，已经下载的文件字节数为值。
文件名为了防止重复，这里我们设置文件名为下载url的hash值，可以保证不重重。
实现代码如下：
```
- (void)URLSession:(NSURLSession *)session dataTask:(NSURLSessionDataTask *)dataTask didReceiveResponse:(NSHTTPURLResponse *)response completionHandler:(void (^)(NSURLSessionResponseDisposition))completionHandler
{
    self.totalLength = [response.allHeaderFields[@"Content-Length"] integerValue] +  DownloadLength;
    NSMutableDictionary *dict = [NSMutableDictionary dictionaryWithContentsOfFile: TotalLengthPlist];
    
    if (dict == nil) dict = [NSMutableDictionary dictionary];
    dict[ Filename] = @(self.totalLength);
    
    [dict writeToFile: TotalLengthPlist atomically:YES];
}
```
上述NSSessionDelegate方法会在请求收到回应的时候调用一次，我们可以在该方法中获取回应信息，取出content-length字段。
3.封装一个方法，实现下载进度，成功，失败提示
我们可以模仿AFNetwork，把下载封装到一个方法，然后使用不同的block来实现下载进度，成功，失败后的回调。
定义如下：
```
-(void)downLoadWithURL:(NSString *)URL
              progress:(progressBlock)progressBlock
               success:(successBlock)successBlock
                 faile:(faileBlock)faileBlock
{
    self.successBlock = successBlock;
    self.failedBlock = faileBlock;
    self.progressBlock = progressBlock;
    self.downLoadUrl = URL;
    [self.task resume];
}
```
上面的三个block都采用宏定义方式，这样看起来比较简洁，具体代码参考下面的完整代码。
然后我们可以在NSURLSessionDataDelegate的对应的代理方法中去实现三个block的调用，然后传入相应的参数。这样当其他人调用我们的方法，就可以在相应的block中实现回调。具体代码参考下面的完整代码
### 完整代码实现
下面是完整的代码实现
```
#import <Foundation/Foundation.h>
typedef void (^successBlock) (NSString *fileStorePath);
typedef void (^faileBlock) (NSError *error);
typedef void (^progressBlock) (float progress);
@interface DownLoadManager : NSObject <NSURLSessionDataDelegate>
@property (copy) successBlock  successBlock;
@property (copy) faileBlock      failedBlock;
@property (copy) progressBlock    progressBlock;
-(void)downLoadWithURL:(NSString *)URL
              progress:(progressBlock)progressBlock
               success:(successBlock)successBlock
                 faile:(faileBlock)faileBlock;
+ (instancetype)sharedInstance;
-(void)stopTask;
@end
```
```
//
//  DownLoadManager.m
//  test1
//
//  Created by Mia on 16/7/30.
//  Copyright © 2016年 Mia. All rights reserved.
//
#import "DownLoadManager.h"
#import "NSString+Hash.h"
@interface DownLoadManager ()
/** 下载任务 */
@property (nonatomic, strong) NSURLSessionDataTask *task;
/** session */
@property (nonatomic, strong) NSURLSession *session;
/** 写文件的流对象 */
@property (nonatomic, strong) NSOutputStream *stream;
/** 文件的总大小 */
@property (nonatomic, assign) NSInteger totalLength;
@property(nonatomic,strong)NSString *downLoadUrl;
@end
// 文件名（沙盒中的文件名），使用md5哈希url生成的，这样就能保证文件名唯一
#define  Filename  self.downLoadUrl.md5String
// 文件的存放路径（caches）
#define  FileStorePath [[NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES) lastObject] stringByAppendingPathComponent: Filename]
// 使用plist文件存储文件的总字节数
#define  TotalLengthPlist [[NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES) lastObject] stringByAppendingPathComponent:@"totalLength.plist"]
// 文件的已被下载的大小
#define  DownloadLength [[[NSFileManager defaultManager] attributesOfItemAtPath: FileStorePath error:nil][NSFileSize] integerValue]
@implementation DownLoadManager
#pragma mark  - 创建单例
static id _instance;
+ (instancetype)allocWithZone:(struct _NSZone *)zone
{
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _instance = [super allocWithZone:zone];
    });
    return _instance;
}
+ (instancetype)sharedInstance
{
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _instance = [[self alloc] init];
    });
    return _instance;
}
- (id)copyWithZone:(NSZone *)zone
{
    return _instance;
}
- (id)mutableCopyWithZone:(NSZone *)zone {
    return _instance;
}
#pragma mark - 公开方法
-(void)downLoadWithURL:(NSString *)URL
              progress:(progressBlock)progressBlock
               success:(successBlock)successBlock
                 faile:(faileBlock)faileBlock
{
    self.successBlock = successBlock;
    self.failedBlock = faileBlock;
    self.progressBlock = progressBlock;
    self.downLoadUrl = URL;
    [self.task resume];
    
    NSLog(@"%p----%p",self,[DownLoadManager sharedInstance]);
}
-(void)stopTask{
    [self.task suspend ];
}
#pragma mark  - getter方法
- (NSURLSession *)session
{
    if (!_session) {
        _session = [NSURLSession sessionWithConfiguration:[NSURLSessionConfiguration defaultSessionConfiguration] delegate:self delegateQueue:[[NSOperationQueue alloc] init]];
    }
    return _session;
}
- (NSOutputStream *)stream
{
    if (!_stream) {
        _stream = [NSOutputStream outputStreamToFileAtPath: FileStorePath append:YES];
    }
    return _stream;
}
- (NSURLSessionDataTask *)task
{
    if (!_task) {
        NSInteger totalLength = [[NSDictionary dictionaryWithContentsOfFile: TotalLengthPlist][ Filename] integerValue];
        
        if (totalLength &&  DownloadLength == totalLength) {
            NSLog(@"######文件已经下载过了");
            return nil;
        }
        
        // 创建请求
        NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:[NSURL URLWithString: self.downLoadUrl]];
        
        // 设置请求头
        // Range : bytes=xxx-xxx，从已经下载的长度开始到文件总长度的最后都要下载
        NSString *range = [NSString stringWithFormat:@"bytes=%zd-",  DownloadLength];
        [request setValue:range forHTTPHeaderField:@"Range"];
        
        // 创建一个Data任务
        _task = [self.session dataTaskWithRequest:request];
    }
    return _task;
}
#pragma mark - <NSURLSessionDataDelegate>
/**
 * 1.接收到响应
 */
- (void)URLSession:(NSURLSession *)session dataTask:(NSURLSessionDataTask *)dataTask didReceiveResponse:(NSHTTPURLResponse *)response completionHandler:(void (^)(NSURLSessionResponseDisposition))completionHandler
{
    // 打开流
    [self.stream open];
    
    /*
     （Content-Length字段返回的是服务器对每次客户端请求要下载文件的大小）
     比如首次客户端请求下载文件A，大小为1000byte，那么第一次服务器返回的Content-Length = 1000，
     客户端下载到500byte，突然中断，再次请求的range为 “bytes=500-”，那么此时服务器返回的Content-Length为500
     所以对于单个文件进行多次下载的情况（断点续传），计算文件的总大小，必须把服务器返回的content-length加上本地存储的已经下载的文件大小
     */
    self.totalLength = [response.allHeaderFields[@"Content-Length"] integerValue] +  DownloadLength;
    
    // 把此次已经下载的文件大小存储在plist文件
    NSMutableDictionary *dict = [NSMutableDictionary dictionaryWithContentsOfFile: TotalLengthPlist];
    if (dict == nil) dict = [NSMutableDictionary dictionary];
    dict[ Filename] = @(self.totalLength);
    [dict writeToFile: TotalLengthPlist atomically:YES];
    
    // 接收这个请求，允许接收服务器的数据
    completionHandler(NSURLSessionResponseAllow);
}
/**
 * 2.接收到服务器返回的数据（这个方法可能会被调用N次）
 */
- (void)URLSession:(NSURLSession *)session dataTask:(NSURLSessionDataTask *)dataTask didReceiveData:(NSData *)data
{
    // 写入数据
    [self.stream write:data.bytes maxLength:data.length];
    
    float progress = 1.0 *  DownloadLength / self.totalLength;
    if (self.progressBlock) {
        self.progressBlock(progress);
    }
    
    // 下载进度
}
/**
 * 3.请求完毕（成功\失败）
 */
- (void)URLSession:(NSURLSession *)session task:(NSURLSessionTask *)task didCompleteWithError:(NSError *)error
{
    if (error) {
        if (self.failedBlock) {
            self.failedBlock(error);
            self.successBlock = nil;
            self.progressBlock = nil;
            self.failedBlock = nil;
        }
        self.stream = nil;
        self.task = nil;
        
    }else{
        if (self.successBlock) {
            self.successBlock(FileStorePath);
            self.successBlock = nil;
            self.progressBlock = nil;
            self.failedBlock = nil;
        }
        // 关闭流
        [self.stream close];
        self.stream = nil;
        // 清除任务
        self.task = nil;
    }
}
@end
```
### 如何调用
```
@interface ViewController ()
@end
@implementation ViewController
/**
 * 开始下载
 */
- (IBAction)start:(id)sender {
    // 启动任务
    NSString * downLoadUrl =  @"http://audio.xmcdn.com/group11/M01/93/AF/wKgDa1dzzJLBL0gCAPUzeJqK84Y539.m4a";
    [[DownLoadManager sharedInstance]downLoadWithURL:downLoadUrl progress:^(float progress) {
        NSLog(@"###%f",progress);
        
    } success:^(NSString *fileStorePath) {
        NSLog(@"###%@",fileStorePath);
        
    } faile:^(NSError *error) {
        NSLog(@"###%@",error.userInfo[NSLocalizedDescriptionKey]);
    }];
}
/**
 * 暂停下载
 */
- (IBAction)pause:(id)sender {
    [[DownLoadManager sharedInstance]stopTask];
}
@end
```
### 总结
这里只能实现单个任务下载，大家可以自己想想办法，看如何实现多任务下载，并且实现断点续传功能。
并且为了更加便于操作，建议把存储信息换成使用数据库存储。
[Demo下载地址](https://link.jianshu.com?t=http://7xslqw.com1.z0.glb.clouddn.com/%20breakpoint%20Transmissio.zip)
作者：西木柚子
链接：https://www.jianshu.com/p/0e6deea7de87
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
