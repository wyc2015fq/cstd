# iOS音频篇：AVPlayer的缓存实现 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年07月04日 14:11:10[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：9533
　　在上一篇文章[《使用AVPlayer播放网络音乐》](http://www.jianshu.com/p/32b932f44c9b)介绍了AVPlayer的基本使用，下面介绍如何通过AVAssetResourceLoader实现AVPlayer的缓存
## 需求梳理
　　没有任何工具能适用于所有的场景，在使用AVPlayer的过程中，我们会发现它有很多局限性，比如播放网络音乐时，往往不能控制其内部播放逻辑，比如我们会发现播放时seek会失败，数据加载完毕后不能获取到数据文件进行其他操作，因此我们需要寻找弥补其不足之处的方法，这里我们选择了AVAssetResourceLoader。
**AVAssetResourceLoader的作用**：让我们自行掌握AVPlayer数据的加载，包括获取AVPlayer需要的数据的信息，以及可以决定传递多少数据给AVPlayer。
**AVAssetResourceLoader在AVPlayer中的位置**如下：*
![](http://upload-images.jianshu.io/upload_images/1644426-0bc9c532e86a476e.jpeg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Location.jpeg
## 实现核心
　　使用AVAssetResourceLoader需要实现AVAssetResourceLoaderDelegate的方法：
```
- (BOOL)resourceLoader:(AVAssetResourceLoader *)resourceLoader 
shouldWaitForLoadingOfRequestedResource:(AVAssetResourceLoadingRequest *)loadingRequest;
```
**要求加载资源的代理方法**，这时我们需要保存loadingRequest并对其所指定的数据进行读取或下载操作，当数据读取或下载完成，我们可以对loadingRequest进行完成操作。
```
- (void)resourceLoader:(AVAssetResourceLoader *)resourceLoader 
didCancelLoadingRequest:(AVAssetResourceLoadingRequest *)loadingRequest;
```
**取消加载资源的代理方法**，这时我们需要取消loadingRequest所指定的数据的读取或下载操作。
## 实现策略
　　通过AVAssetResourceLoader实现缓存的策略有多种，没有绝对的优与劣，只要符合我们的实际需求就可以了。
**下面我们以模仿企鹅音乐的来演示AVAssetResourceLoader实现缓存的过程为例子。**
　　先观察并猜测企鹅音乐的缓存策略（当然它不是用AVPlayer播放）：
　　1、开始播放，同时开始下载完整的文件，当文件下载完成时，保存到缓存文件夹中；
　　2、当seek时
　　　（1）如果seek到已下载到的部分，直接seek成功；（如下载进度60%，seek进度50%）
　　　（2）如果seek到未下载到的部分，则开始新的下载（如下载进度60%，seek进度70%）
　　　　　　PS1：此时文件下载的范围是70%-100%
　　　　　　PS2：之前已下载的部分就被删除了
　　　　　　PS3：如果有别的seek操作则重复步骤2，如果此时再seek到进度40%，则会开始新的下载（范围40%-100%）
　　3、当开始新的下载之后，由于文件不完整，下载完成之后不会保存到缓存文件夹中；
　　4、下次再播放同一歌曲时，如果在缓存文件夹中存在，则直接播放缓存文件；
## 实现流程
##### 流程示意图：
![](http://upload-images.jianshu.io/upload_images/1644426-a2dd9a776cafbd5f.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Process.png
##### 1、通过自定义scheme来创建avplayer，并给AVURLAsset指定代理（SUPlayer对象）
```
AVURLAsset * asset = [AVURLAsset URLAssetWithURL:[self.url customSchemeURL] options:nil];            
[asset.resourceLoader setDelegate:self.resourceLoader queue:dispatch_get_main_queue()];
self.currentItem = [AVPlayerItem playerItemWithAsset:asset];
self.player = [AVPlayer playerWithPlayerItem:self.currentItem];
```
##### 2、代理实现AVAssetResourceLoader的代理方法（SUResourceLoader对象）
```
- (BOOL)resourceLoader:(AVAssetResourceLoader *)resourceLoader shouldWaitForLoadingOfRequestedResource:(AVAssetResourceLoadingRequest *)loadingRequest {
    [self addLoadingRequest:loadingRequest];
    return YES;
}
- (void)resourceLoader:(AVAssetResourceLoader *)resourceLoader didCancelLoadingRequest:(AVAssetResourceLoadingRequest *)loadingRequest {
    [self removeLoadingRequest:loadingRequest];
}
```
##### 3、对loadingRequest的处理（addLoadingRequest方法）
　　（1）将其加入到requestList中
`[self.requestList addObject:loadingRequest];`
　　（2）如果还没开始下载，则开始请求数据，否则静待数据的下载
`[self newTaskWithLoadingRequest:loadingRequest cache:YES];`
　　（3）如果是seek之后的loadingRequest，判断请求开始的位置，如果已经缓冲到，则直接读取数据
```
if (loadingRequest.dataRequest.requestedOffset >= self.requestTask.requestOffset &&
    loadingRequest.dataRequest.requestedOffset <= self.requestTask.requestOffset + self.requestTask.cacheLength) {
    [self processRequestList];
}
```
###### 　　3.4如果还没缓冲到，则重新请求
```
if (self.seekRequired) {
    [self newTaskWithLoadingRequest:loadingRequest cache:NO];
}
```
##### 4、数据请求的处理（newTaskWithLoadingRequest方法）
　　（1）先判断是否已经有下载任务，如果有，则先取消该任务
```
if (self.requestTask) {
    fileLength = self.requestTask.fileLength;
    self.requestTask.cancel = YES;
}
```
　　（2）建立新的请求，设置代理
```
self.requestTask = [[SURequestTask alloc]init];
    self.requestTask.requestURL = loadingRequest.request.URL;
    self.requestTask.requestOffset = loadingRequest.dataRequest.requestedOffset;
    self.requestTask.cache = cache;
    if (fileLength > 0) {
        self.requestTask.fileLength = fileLength;
    }
    self.requestTask.delegate = self;
    [self.requestTask start];
    self.seekRequired = NO;
```
##### 5、数据响应的处理（processRequestList方法）
**　　对requestList里面的loadingRequest填充响应数据，如果已完全响应，则将其从requestList中移除**
```
- (void)processRequestList {
    NSMutableArray * finishRequestList = [NSMutableArray array];
    for (AVAssetResourceLoadingRequest * loadingRequest in self.requestList) {
        if ([self finishLoadingWithLoadingRequest:loadingRequest]) {
            [finishRequestList addObject:loadingRequest];
        }
    }
    [self.requestList removeObjectsInArray:finishRequestList];
}
```
**　　填充响应数据的过程如下：**
（1）填写 contentInformationRequest的信息，注意contentLength需要填写下载的文件的总长度，contentType需要转换
```
CFStringRef contentType = UTTypeCreatePreferredIdentifierForTag(kUTTagClassMIMEType, (__bridge CFStringRef)(MimeType), NULL);
    loadingRequest.contentInformationRequest.contentType = CFBridgingRelease(contentType);
    loadingRequest.contentInformationRequest.byteRangeAccessSupported = YES;
    loadingRequest.contentInformationRequest.contentLength = self.requestTask.fileLength;
```
（2）计算可以响应的数据长度，注意数据读取的起始位置是当前avplayer当前播放的位置，结束位置是loadingRequest的结束位置或者目前文件下载到的位置
```
NSUInteger cacheLength = self.requestTask.cacheLength;
    NSUInteger requestedOffset = loadingRequest.dataRequest.requestedOffset;
    if (loadingRequest.dataRequest.currentOffset != 0) {
        requestedOffset = loadingRequest.dataRequest.currentOffset;
    }
    NSUInteger canReadLength = cacheLength - (requestedOffset - self.requestTask.requestOffset);
    NSUInteger respondLength = MIN(canReadLength, loadingRequest.dataRequest.requestedLength);
```
（3）读取数据并填充到loadingRequest
`    [loadingRequest.dataRequest respondWithData:[SUFileHandle readTempFileDataWithOffset:requestedOffset - self.requestTask.requestOffset length:respondLength]];`
（4） 如果完全响应了所需要的数据，则完成loadingRequest，注意判断的依据是 响应数据结束的位置 >= loadingRequest结束的位置
```
NSUInteger nowendOffset = requestedOffset + canReadLength;
    NSUInteger reqEndOffset = loadingRequest.dataRequest.requestedOffset + loadingRequest.dataRequest.requestedLength;
    if (nowendOffset >= reqEndOffset) {
        [loadingRequest finishLoading];
        return YES;
    }
    return NO;
```
##### 6、处理requestList的时机
当有新的loadingRequest或者文件下载进度更新时，都需要处理requestList
##### 7、新的请求任务实现的过程（SURequestTask对象）
（1）初始化时，需要删除旧的临时文件，并创建新的空白临时文件
```
- (instancetype)init {
    if (self = [super init]) {
        [SUFileHandle createTempFile];
    }
    return self;
}
```
（2）建立新的连接，如果是seek后的请求，则指定其请求内容的范围
```
- (void)start {
    NSMutableURLRequest * request = [NSMutableURLRequest requestWithURL:[self.requestURL originalSchemeURL] cachePolicy:NSURLRequestReloadIgnoringCacheData timeoutInterval:RequestTimeout];
    if (self.requestOffset > 0) {
        [request addValue:[NSString stringWithFormat:@"bytes=%ld-%ld", self.requestOffset, self.fileLength - 1] forHTTPHeaderField:@"Range"];
    }
    self.session = [NSURLSession sessionWithConfiguration:[NSURLSessionConfiguration defaultSessionConfiguration] delegate:self delegateQueue:[NSOperationQueue mainQueue]];
    self.task = [self.session dataTaskWithRequest:request];
    [self.task resume];
}
```
（3）当收到数据时，将数据写入临时文件，更新下载进度，同时通知代理处理requestList
```
- (void)URLSession:(NSURLSession *)session dataTask:(NSURLSessionDataTask *)dataTask didReceiveData:(NSData *)data {
    if (self.cancel) return;
    [SUFileHandle writeTempFileData:data];
    self.cacheLength += data.length;
    if (self.delegate && [self.delegate respondsToSelector:@selector(requestTaskDidUpdateCache)]) {
        [self.delegate requestTaskDidUpdateCache];
    }
}
```
（4）当下载完成时，如果满足缓存的条件，则将临时文件拷贝到缓存文件夹中
```
if (self.cache) {
    [SUFileHandle cacheTempFileWithFileName:[NSString fileNameWithURL:self.requestURL]];
}
if (self.delegate && [self.delegate respondsToSelector:@selector(requestTaskDidFinishLoadingWithCache:)]) {
    [self.delegate requestTaskDidFinishLoadingWithCache:self.cache];
}
```
## 示例Demo
　　以上就是总体的实现流程，当然每个人的思路都不同，你可以在对其理解得足够深刻之后使用更高效更安全的方式去实现。
###### 　　本文的demo在我的github上可以下载：[GitHub : SUCacheLoader](https://github.com/DaMingShen/SUCacheLoader)
**本demo是以缓存豆瓣FM的歌曲（MP4格式）为例写的，如果你追求更完美的效果，可以从以下几方面入手：**
　　1、对缓存格式支持的处理：并不是所有文件格式都支持的哦，对于不支持的格式，你应该不使用缓存功能；
　　2、对缓存过程中各种错误的处理：比如下载超时、连接失败、读取数据错误等等的处理；
　　3、缓存文件的命名处理，如果缓存文件没有后缀（如.mp4），可能会导致播放失败；
　　4、AVPlayer播放状态的处理，要做到完美的播放体验，在这方面要下点功夫；
## Next：
　　接下来将带来AudioFileStream + AudioQueue 播放本地文件、网络文件、缓存实现的讲解
文／明仔Su（简书作者）
原文链接：http://www.jianshu.com/p/93ce1748ea57
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
