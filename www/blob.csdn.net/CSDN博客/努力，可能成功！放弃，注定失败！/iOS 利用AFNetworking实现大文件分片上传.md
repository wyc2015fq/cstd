# iOS 利用AFNetworking实现大文件分片上传 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年09月17日 10:09:54[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：218
概述
一说到`文件上传`，想必大家都并不陌生，更何况是利用[AFNetworking](https://github.com/AFNetworking/AFNetworking)（PS：后期统称AF）来做，那更是小菜一碟。比如开发中常见的场景:`头像上传`，`九宫格图片上传`...等等，这些场景无一不使用到文件上传的功能。如果利用`AF`来实现，无非就是客户端调用`AF`提供的文件上传接口即可，`API`如下所示：
```
- (nullable NSURLSessionDataTask *)POST:(NSString *)URLString
                             parameters:(nullable id)parameters
              constructingBodyWithBlock:(nullable void (^)(id <AFMultipartFormData> formData))block
                               progress:(nullable void (^)(NSProgress *uploadProgress))uploadProgress
                                success:(nullable void (^)(NSURLSessionDataTask *task, id _Nullable responseObject))success
                                failure:(nullable void (^)(NSURLSessionDataTask * _Nullable task, NSError *error))failure;
```
上面这种场景，主要是针对一些小资源文件的上传，上传过程耗时较短，用户可以接受。但是一旦资源文件过大（比如1G以上），则必须要考虑上传过程网络中断的情况。试想我们还是采用上述方案，一口气把这整个1G的资源文件上传到服务器，这显然是不现实的，就算服务器答应，用户也不答应的。考虑到网络使用中断或服务器上传异常...等场景，那么我们恢复网络后又得重新从头开始上传，那之前已经上传完成的部分资源岂不作废，这种耗时耗力的工作，显然是不符合常理的。为了解决大文件上传的存在如此鸡肋的问题，从而诞生了一个叫：**分片上传（断点续上传）**
**分片上传（断点续上传）** 主要是为了保证在网络中断后1G的资源文件已上传的那部分在下次网络连接时不必再重传。所以我们本地在上传的时候，要将大文件进行切割分片，比如分成`1024*1024B`，即将大文件分成`1M`的片进行上传，服务器在接收后，再将这些片合并成原始文件，这就是 **分片** 的基本原理。断点续传要求本地要记录每一片的上传的状态，我通过三个状态进行了标记`（waiting loading finish）`，当网络中断，再次连接后，从断点处进行上传。服务器通过文件名、总片数判断该文件是否已全部上传完成。
弄懂了**分片上传（断点续上传）** 的基本原理，其核心就是**分片**，然后将分割出来的的每一**片**，按照类似上传头像的方式上传到服务器即可，全部上传完后再在服务端将这些小数据片合并成为一个资源。
**分片上传**引入了两个概念：**块（block）**和**片（fragment）**。每个块由一到多个片组成，而一个资源则由一到多个块组成。他们之间的关系可以用下图表述：
![](https://upload-images.jianshu.io/upload_images/1874977-dd5b366b90878448.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
文件资源组成关系.png
本文笔者将着重分析**分片上传**实现的具体过程以及细节处理，争取把里面的所有涵盖的知识点以及细节处理分析透彻。希望为大家提供一点思路，少走一些弯路，填补一些细坑。文章仅供大家参考，若有不妥之处，还望不吝赐教，欢迎批评指正。
效果图如下：
![](https://upload-images.jianshu.io/upload_images/1874977-b61430f455ccc52d.gif?imageMogr2/auto-orient/strip%7CimageView2/2/w/381/format/webp)
FileUpload.gif
知识点
虽然**分片上传**的原理看似非常简单，但是落实到具体的实现，其中还是具有非常多的细节分析和逻辑处理，而且都是我们开发中不常用到的知识点，这里笔者就总结了一下**分片上传**所用到的知识点和使用场景，以及借助一些第三方框架，来达到**分片上传**的目的。
- 
**图片和视频资源的获取**
	所谓文件上传，前提必须得有文件，而文件一般是本地文件，本地文件的获取来源一般是系统相册获取，关于如何从系统相册中获取图片或视频资源，这里笔者采用[TZImagePickerController](https://github.com/banchichen/TZImagePickerController)一个支持多选、选原图和视频的图片选择器，同时有预览、裁剪功能，支持iOS6+第三方框架。根据`TZImagePickerControllerDelegate`返回的资源（图片、视频）数据，然后利用`TZImageMananger`提供的API，获取到原始图片和视频资源。关键API如下：具体使用请参照`TZImagePickerController`提供Demo。
```
/// 获取原图
  - (void)getOriginalPhotoDataWithAsset:(id)asset completion:(void (^)(NSData *data,NSDictionary *info,BOOL isDegraded))completion;
  - (void)getOriginalPhotoDataWithAsset:(id)asset progressHandler:(void (^)(double progress, NSError *error, BOOL *stop, NSDictionary *info))progressHandler completion:(void (^)(NSData *data,NSDictionary *info,BOOL isDegraded))completion;
 /// 获得视频
  - (void)getVideoWithAsset:(id)asset completion:(void (^)(AVPlayerItem * playerItem, NSDictionary * info))completion;
  - (void)getVideoWithAsset:(id)asset progressHandler:(void (^)(double progress, NSError *error, BOOL *stop, NSDictionary *info))progressHandler completion:(void (^)(AVPlayerItem *, NSDictionary *))completion;
```
- 
**文件读写和剪切**
	文件写入一般用于从相册中获取到图片的原图`data`，然后将其写入到指定的文件夹中，一般调用`NSData`提供的方法。
```
- (BOOL)writeToFile:(NSString *)path atomically:(BOOL)useAuxiliaryFile;
```
文件剪切一般用于从相册中获取到视频资源，其视频格式是`mov`格式的，需要我们视频压缩转成`mp4`格式，压缩成功后一般将其导入到APP沙盒文件的`tmp`目录下，总所周知，`tmp`里面一般存放一些临时文件，所以需要将其导入到`Cache`文件夹中去，这里用文件移动（剪切）再好不过了，而且不需要读取到内存中去。 直接调用 `NSFileManager`的提供的API即可:
```
- (BOOL)moveItemAtPath:(NSString *)srcPath toPath:(NSString *)dstPath error:(NSError **)error
```
文件读取一般主要用于读取每一个文件`片`的大小，需要利用`NSFileHandle`来处理，调用其如下API来完成。
```
- (NSData *)readDataOfLength:(NSUInteger)length;
- (void)seekToFileOffset:(unsigned long long)offset;
+ (nullable instancetype)fileHandleForReadingAtPath:(NSString *)path;
```
综上所述：`NSData`，`NSFileManager`，`NSFileHandle`的API的常规使用得比较熟练。
- 
**视频压缩**
	系统的录制视频导出的格式是`mov`，所以一般的做法就是压缩转化成`mp4`格式，这样就得用到系统的视频压缩方法，大家可以自行百度`AVAssetExportSession`的使用。这里笔者采用`TZImagePickerController`提供的API来做的，具体请参照`TZImageManager`提供的方法，大家可以看看其实现。
```
/// Export video 导出视频 presetName: 预设名字，默认值是AVAssetExportPreset640x480
- (void)getVideoOutputPathWithAsset:(id)asset success:(void (^)(NSString *outputPath))success failure:(void (^)(NSString *errorMessage, NSError *error))failure;
- (void)getVideoOutputPathWithAsset:(id)asset presetName:(NSString *)presetName success:(void (^)(NSString *outputPath))success failure:(void (^)(NSString *errorMessage, NSError *error))failure;
```
- 
**资源缓存**
	所谓资源缓存，就是一般从系统相册中获取到的资源（图片、视频），我们会将资源另存到在`/Library/Caches/Upload`目录下，然后把资源存放的相对路径给缓存起来，下次从系统相册中选取相同的资源，如果已经存在于`/Library/Caches/Upload`目录下，则不需要去获取原始图片，或者压缩视频了。这里笔者采用的是：[YYCache](https://github.com/ibireme/YYCache) 来做内存缓存和磁盘缓存。具体使用，还请自行百度。
- 
**数据库**
	数据库主要用于，保存新建资源，保存上传资源，保存文件片...等等，利用数据库的`增`，`删`，`改`，`查`等功能，更加方便快捷的处理文件片的上传状态，上传进度，获取或删除草稿数据...等等一些列的操作，大大提供了开发的效率。这里笔者采用的是基于[FMDB](https://github.com/ccgus/fmdb)封装的[BGFMDB](https://github.com/huangzhibiao/BGFMDB)框架，`BGFMDB`是对`FMDB`面相对象层的封装，且几乎支持存储iOS所有基本的自带数据类型，让数据的`增`，`删`，`改`，`查`分别只需要一行代码即可。具体使用，还请查看`BGFMDB`提供的Demo。
- 
**多线程**
	多线程的使用主要用在，① 从系统相册获取到资源（图片、视频），对资源进行处理（比如，获取原图，压缩视频等等）; ② 文件分片上传。其实现实开发中，我们使用多线程的的场景并不多，但反观使用多线程最多的场景就是--**面试**。多线程其实是iOS中非常重要的知识点，但是由于平时疏于练习和使用，脑子里面可能只有少许多线程的相关知识。此次笔者在项目中做`大文件分片上传`功能，也让笔者重拾了多线程的相关知识，而且运用到实际开发中去，也是一个不小的收获。这里笔者就讲讲本模块中用到了哪些多线程的知识，当然具体的理论知识和实践操作，大家可以参照下面笔者分享的网址去针对性的学习和实践多线程的相关知识。具体如下：
- 
[iOS多线程：『GCD』详尽总结](https://www.jianshu.com/p/2d57c72016c6)
**特别提醒：** ① 必须掌握`GCD 队列组：dispatch_group`。合理使用`dispatch_group_enter`、`dispatch_group_leave` 和 `dispatch_group_notify`的配套使用。
		② 必须掌握`GCD 信号量：dispatch_semaphore`。熟练使用`dispatch_semaphore_create`、`dispatch_semaphore_signal`和`dispatch_semaphore_wait`的配套使用，利用`dispatch_semaphore`保持线程同步，将异步执行任务转换为同步执行任务以及保证线程安全，为线程加锁。
- 
[iOS多线程：『NSOperation、NSOperationQueue』详尽总结](https://www.jianshu.com/p/4b1d77054b35)
模块
关于笔者在Demo中提供的`文件分片上传`的示例程序，虽然不够华丽，但麻雀虽小，五脏俱全，大家凑合着看咯。但总的来说，可以简单分为以下几个模块：
- 
**资源新建：** 系统相册获取资源文件（图片、视频）；获取原图或视频压缩，并导入到沙盒指定的文件夹；资源缓存。
- 
**后台接口：** 考虑到示例程序中部分业务逻辑是按照后台提供的API设计的，所以有必要分享一下后台提供了哪些API，以及具体的使用的场景。
- 
**文件分片：** 将新建资源，转化为上传资源，将资源中存放的每一个文件块，按照`512k`的大小分成若干个文件片。涉及到新建资源存储数据库，上传资源存储数据库，以及每个文件片存储数据库。
- 
**草稿存储：** 草稿列表的数据来源主要分为`手动存草稿`和`自动存草稿`。`手动存草稿`一般是指用户手动点击存草稿按钮保存草稿，此草稿数据可以进行二次编辑；`自动存草稿`一般是指用户点击提交按钮上传资源文件，由于一时半会不会上传到服务器上去，所以需要报存草稿，此草稿数据可以显示上传进度和上传状态，用户可以点击暂停/开始上传此草稿，但不允许二次编辑。当然，草稿数据都是可以手动删除的。
- 
**分片上传<核心>：** 将上传资源中所有分好的文件片，上传到服务器中去，当网络中断或程序异常都可以支持断点续传，保证在网络中断后该上传资源中已上传的那部分文件片在下次网络连接时或程序启动后不必再重传。涉及到更新资源进度，更新资源状态，以及每一个文件片的上传状态。
资源新建
资源新建模块的UI搭建，笔者这里就不过多赘述，这里更多讨论的是功能逻辑和细节处理。具体内容还请查看`CMHCreateSourceController.h/m`
- 
设置`TZImagePickerController`导出图片宽度
	默认情况下，`TZImagePickerController` (PS：后期统称`TZ`) 默认导出的图片宽度为`828px`，具体请查看`TZ`提供的`photoWidth`属性。考虑到手动存草稿可以是二次编辑，所以有必要把`TZ`返回的图片储存到数据库中，所以我们只需要存储`缩略图`即可，何况新建资源模块本身页面也只展示小图，完全没必要导出宽度为`828px`的图片，这样会导致数据存储和数据读取都异常缓慢，解决方案如下：
```
/// CoderMikeHe Fixed Bug : 这里新建模块只需要展示，小图，所以导出图片不需要太大，
/// 而且导出的图片需要存入数据库，所以尽量尺寸适量即可，否则会导致存储数据库和读取数据库异常的慢
  imagePickerVc.photoWidth = ceil(MH_SCREEN_WIDTH / 4);
```
- 
`PHAsset` 保存数据库
	默认情况下，`TZ`是支持本地图片预览的，需要我们提供一组`selectedAssets`，里面装着`PHAsset`对象，如果我们处于新建资源页面时，这完全没有问题；一旦我们手动存草稿，进行二次编辑时，就会出现问题，原因就是`PHAsset`不遵守`NSCoding`协议，无法进行归档。解决方案其实就是`储存PHAsset的localIdentifier`即可。通过`localIdentifier`获取`PHAsset`代码如下：
```
/// 获取PHAsset
PHFetchResult *fetchResult = [PHAsset fetchAssetsWithLocalIdentifiers:@[file.localIdentifier] options:nil];
PHAsset *asset = fetchResult.firstObject;
if (!asset) {
  // 这种场景就是这张照片储存完PHAsset以后，但用户在手机上把这张照片删除
}
```
- 
资源（图片，视频）处理
	常规逻辑：第一步，通过`TZ`从系统相册中获取一组资源（图片、视频）文件，第二步，遍历资源列表根据`PHAsset`去获取原图数据或压缩视频，第三步将处理过的资源保存到`Cache/Upload`文件夹中。看起来该方案看似稳如藏獒，但是实际情况第二步、第三步操作，其实是非常耗内存的，而且每次获取系统相册中同一个的资源（`PHAsset`），第二步、第三步处理过后都是一样的，如果该资源（`PHAsset`）之前已经通过第二步、第三步处理过，那么后面在使用到该资源是不是完全没有必要进行第二步和第三步操作，所以这里就必须用到`数据缓存（磁盘缓存+内存缓存）`。 最终方案如下：
![](https://upload-images.jianshu.io/upload_images/1874977-613ca798a6bf76e6.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
资源处理逻辑.png
从上图👆明显可知，只有两种场景才会去执行第二步、第三步处理，且都是由于不存在磁盘中导致的。这里有一个比较细节的地方：`缓存相对路径`。千万不要缓存绝对路径，因为随着APP的更新或重装，都会导致应用的沙盒的绝对路径是会改变的。
实现代码如下：
```
/// 完成图片选中
- (void)_finishPickingPhotos:(NSArray<UIImage *> *)photos sourceAssets:(NSArray *)assets isSelectOriginalPhoto:(BOOL)isSelectOriginalPhoto infos:(NSArray<NSDictionary *> *)infos{
  
  /// 选中的相片以及Asset
  self.selectedPhotos = [NSMutableArray arrayWithArray:photos];
  self.selectedAssets = [NSMutableArray arrayWithArray:assets];
  /// 记录一下是否上传原图
  self.source.selectOriginalPhoto = isSelectOriginalPhoto;
  
  /// 生成资源文件
  __block NSMutableArray *files = [NSMutableArray array];
  /// 记录之前的源文件
  NSMutableArray *srcFiles = [NSMutableArray arrayWithArray:self.source.files];
  
  NSInteger count = MIN(photos.count, assets.count);
  /// 处理资源
  /// CoderMikeHe Fixed Bug : 这里可能会涉及到选中多个视频的情况，且需要压缩视频的情况
  [MBProgressHUD mh_showProgressHUD:@"正在处理资源..." addedToView:self.view];
  
  NSLog(@"Compress Source Complete Before %@ !!!!" , [NSDate date]);
  
  /// 获取队列组
  dispatch_group_t group = dispatch_group_create();
  /// 创建信号量 用于线程同步
  dispatch_semaphore_t semaphore = dispatch_semaphore_create(0);
  
  for (NSInteger i = 0; i < count; i ++ ) {
      dispatch_group_enter(group);
      dispatch_async(_compressQueue, ^{ // 异步追加任务
          /// 设置文件类型
          PHAsset *asset = assets[i];
          /// 图片或资源 唯一id
          NSString *localIdentifier = [[TZImageManager manager] getAssetIdentifier:asset];
          UIImage *thumbImage = photos[i];
          
          /// 这里要去遍历已经获取已经存在资源的文件 内存中
          BOOL isExistMemory = NO;
          for (CMHFile *f in srcFiles.reverseObjectEnumerator) {
              /// 判断是否已经存在路径和文件
              if ([f.localIdentifier isEqualToString:localIdentifier] && MHStringIsNotEmpty(f.filePath)) {
                  [files addObject:f];
                  [srcFiles removeObject:f];
                  isExistMemory = YES;
                  break;
              }
          }
          if (isExistMemory) {
              NSLog(@"++++ 💕文件已经存在内存中💕 ++++");
              dispatch_group_leave(group);
          }else{
              //// 视频和图片，需要缓存，这样会明显减缓，应用的内存压力
              /// 是否已经缓存在沙盒
              BOOL isExistCache = NO;
              
              /// 1. 先去缓存里面去取
              NSString *filePath = (NSString *)[[YYCache sharedCache] objectForKey:localIdentifier];
              /// 这里必须的判断一下filePath是否为空! 以免拼接起来出现问题
              if (MHStringIsNotEmpty(filePath)) {
                  /// 2. 该路径的本地资源是否存在， 拼接绝对路径，filePath是相对路径
                  NSString * absolutePath = [[CMHFileManager cachesDir] stringByAppendingPathComponent:filePath];
                  if ([CMHFileManager isExistsAtPath:absolutePath]) {
                      /// 3. 文件存在沙盒中，不需要获取了
                      isExistCache = YES;
                      
                      /// 创建文件模型
                      CMHFile *file = [[CMHFile alloc] init];
                      file.thumbImage = thumbImage;
                      file.localIdentifier = localIdentifier;
                      /// 设置文件类型
                      file.fileType = (asset.mediaType == PHAssetMediaTypeVideo)? CMHFileTypeVideo : CMHFileTypePicture;
                      file.filePath = filePath;
                      [files addObject:file];
                  }
              }
              
              
              if (isExistCache) {
                  NSLog(@"++++ 💕文件已经存在磁盘中💕 ++++");
                  dispatch_group_leave(group);
              }else{
                  
                  /// 重新获取
                  if (asset.mediaType == PHAssetMediaTypeVideo) {  /// 视频
                      /// 获取视频文件
                      [[TZImageManager manager] getVideoOutputPathWithAsset:asset presetName:AVAssetExportPresetMediumQuality success:^(NSString *outputPath) {
                          NSLog(@"+++ 视频导出到本地完成,沙盒路径为:%@ %@",outputPath,[NSThread currentThread]);
                          /// Export completed, send video here, send by outputPath or NSData
                          /// 导出完成，在这里写上传代码，通过路径或者通过NSData上传
                          /// CoderMikeHe Fixed Bug :如果这样写[NSData dataWithContentsOfURL:xxxx]; 文件过大，会导致内存吃紧而闪退
                          /// 解决办法，直接移动文件到指定目录《类似剪切》
                          NSString *relativePath = [CMHFile moveVideoFileAtPath:outputPath];
                          if (MHStringIsNotEmpty(relativePath)) {
                              CMHFile *file = [[CMHFile alloc] init];
                              file.thumbImage = thumbImage;
                              file.localIdentifier = localIdentifier;
                              /// 设置文件类型
                              file.fileType =  CMHFileTypeVideo;
                              file.filePath = relativePath;
                              [files addObject:file];
                              
                              /// 缓存路径
                              [[YYCache sharedCache] setObject:file.filePath forKey:localIdentifier];
                          }
                          
                          dispatch_group_leave(group);
                          /// 信号量+1 向下运行
                          dispatch_semaphore_signal(semaphore);
                          
                      } failure:^(NSString *errorMessage, NSError *error) {
                          NSLog(@"😭😭😭++++ Video Export ErrorMessage ++++😭😭😭 is %@" , errorMessage);
                          dispatch_group_leave(group);
                          /// 信号量+1 向下运行
                          dispatch_semaphore_signal(semaphore);
                      }];
                  }else{  /// 图片
                      [[TZImageManager manager] getOriginalPhotoDataWithAsset:asset completion:^(NSData *data, NSDictionary *info, BOOL isDegraded) {
                          NSString* relativePath = [CMHFile writePictureFileToDisk:data];
                          if (MHStringIsNotEmpty(relativePath)) {
                              CMHFile *file = [[CMHFile alloc] init];
                              file.thumbImage = thumbImage;
                              file.localIdentifier = localIdentifier;
                              /// 设置文件类型
                              file.fileType =  CMHFileTypePicture;
                              file.filePath = relativePath;
                              [files addObject:file];
                              
                              /// 缓存路径
                              [[YYCache sharedCache] setObject:file.filePath forKey:localIdentifier];
                          }
                          dispatch_group_leave(group);
                          /// 信号量+1 向下运行
                          dispatch_semaphore_signal(semaphore);
                      }];
                  }
                  /// 等待
                  dispatch_semaphore_wait(semaphore, DISPATCH_TIME_FOREVER);
              }
          }
      });
  }
  
  /// 所有任务完成
  dispatch_group_notify(group, dispatch_get_main_queue(), ^{
      NSLog(@"Compress Source Complete After %@ !!!!" , [NSDate date]);
      ///
      [MBProgressHUD mh_hideHUDForView:self.view];
      /// 这里是所有任务完成
      self.source.files = files.copy;
      [self.tableView reloadData];
  });
}
```
后台接口
这里分享一下笔者在实际项目中用到的后台提供断点续传的接口，因为项目中部分逻辑处理是根据后台提供的数据来的。这里笔者简单分析一下各个接口的使用场景。
- 
预加载获取文件ID（`/fileSection/preLoad.do`）
**使用场景**：根据当次上传的文件数量，预先分配好文件ID，APP终端需要做好保存与文件的对应关系，在续传文件时候作为参数传递。
**请求URL**: `http://uadmin.xxxx.cn/fileSection/preLoad.do` (POST)
![](https://upload-images.jianshu.io/upload_images/1874977-6627cec1004c0305.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
Preload.png
- 
断点续传文件（`/fileSection/upload.do`）
**使用场景**：大文件分片并行上传。
**请求URL**: `http://uadmin.xxxx.cn/fileSection/upload.do` (POST)
![](https://upload-images.jianshu.io/upload_images/1874977-6a496e6cccc12509.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
Upload.png
- 
删除文件（`/fileSection/delete.do`）
**使用场景**：在App手动删除草稿时同时删除已上传到服务器的文件。
**请求URL**: `http://uadmin.xxxx.cn/fileSection/delete.do` (POST)
![](https://upload-images.jianshu.io/upload_images/1874977-cef8887b474ba088.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
Delete.png
- 
检查文件是否上传完毕（`/fileSection/isFinish.do`）
**使用场景**：APP中该上传资源的所有的文件片都上传到服务器，服务器需要检查这些文件片的合成情况。如果服务器合成失败，即`finishStatus = 0`，服务器会把那些合成失败的文件返回给APP，即`failFileIds`。APP需要根据`failFileIds`去回滚本地数据库，然后继续重传失败的文件片。
**请求URL**: `http://uadmin.xxxx.cn/fileSection/isFinish.do` (POST)
![](https://upload-images.jianshu.io/upload_images/1874977-033d323fcfdbcaa2.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
finish.png
文件分片
文件分片的过程主要是在用户点击提交资源的过程。具体内容和细节还请查看`CMHSource.h/m` 、`CMHFile.h/m`、`CMHFileSource.h/m`、`CMHFileBlock.h/m`、`CMHFileFragment.h/m`的实现。
首先，这里需要将新建资源`CMHSource` 转成上传资源`CMHFileSource`，以及将新建资源的文件列表`NSArray <CMHFile *> *files`转成上传资源的文件块列表`NSArray <CMHFileBlock *> *fileBlocks`。
其次，需要根据新建资源的文件列表`NSArray <CMHFile *> *files`的个数，即`files.count`，去调用后台提供的预加载获取文件ID（`/fileSection/preLoad.do`）接口，去获取文件ID列表，从而为文件列表`NSArray <CMHFile *> *files`中每一个文件(`CMHFile`)绑定文件ID，然后将`CMHFile`列表转成`CMHFileBlock`列表，以及将新建资源`CMHSource` 转成上传资源`CMHFileSource`。 关键代码如下：
```
- (void)commitSource:(void (^)(BOOL))complete{
/// 1. 通过要上传的文件个数  去服务器获取对应的文件ID
NSInteger uploadFileCount = self.files.count;
/// 2. 以下通过真实的网络请求去模拟获取 文件ID的场景 https://live.9158.com/Room/GetHotTab?devicetype=2&isEnglish=0&version=1.0.1
/// 类似于实际开发中调用服务器的API:  /fileSection/preLoad.do
/// 1. 配置参数
CMHKeyedSubscript *subscript = [CMHKeyedSubscript subscript];
subscript[@"isEnglish"] = @0;
subscript[@"devicetype"] = @2;
subscript[@"version"] = @"1.0.1";
/// 2. 配置参数模型
CMHURLParameters *paramters = [CMHURLParameters urlParametersWithMethod:CMH_HTTTP_METHOD_GET path:CMH_GET_HOT_TAB parameters:subscript.dictionary];
/// 3. 发起请求
[[CMHHTTPRequest requestWithParameters:paramters] enqueueResultClass:nil parsedResult:YES success:^(NSURLSessionDataTask *task, id responseObject) {
    /// - 如果到这里了就认为获取文件ID成功，这里模拟后台返回的数据 有几个上传文件 就对应几个上传文件ID
    NSMutableArray *fileIds = [NSMutableArray arrayWithCapacity:uploadFileCount];
    for (NSInteger i = 0; i < uploadFileCount; i++) {
        NSString *fileId = [self _cmh_fileKey];
        [fileIds addObject:fileId];
    }
    /// - 为每个上传文件绑定服务器返回的文件ID,获取要上传的文件块列表
    /// 将服务器文件ID列表转换为，转成json字符串，后期需要存数据库，这个fileIdsStr很重要
    NSString *fileIdsStr = fileIds.yy_modelToJSONString;
    /// 要上传的文件块列表
    NSMutableArray *fileBlocks = [NSMutableArray arrayWithCapacity:uploadFileCount];
    /// 生成上传文件以及绑定文件ID
    for (NSInteger i = 0; i < uploadFileCount; i++) {
        CMHFile *file = self.files[i];
        NSString *fileId = fileIds[i];
        
        /// 资源中的文件绑定文件ID
        file.fileId = fileId;
        
        /// 文件块
        CMHFileBlock *fileBlcok = [[CMHFileBlock alloc] initFileBlcokAtPath:file.filePath fileId:fileId sourceId:self.sourceId];
        [fileBlocks addObject:fileBlcok];
    }
    /// 生成上传文件资源
    CMHFileSource *fileSource = [[CMHFileSource alloc] init];
    fileSource.sourceId = self.sourceId;
    fileSource.fileIds = fileIdsStr;
    fileSource.fileBlocks = fileBlocks.copy;
    /// 保存文件和资源
    /// 非手动存草稿
    self.manualSaveDraft = NO;
    
    /// CoderMikeHe Fixed Bug : 这里必须记录必须强引用上传资源
    self.fileSource = fileSource;
    
    /// 先保存资源
    @weakify(self);
    [self saveSourceToDB:^(BOOL isSuccess) {
        if (!isSuccess) {
            !complete ? : complete(isSuccess);
            [MBProgressHUD mh_showTips:@"保存资源失败！！！"];
            return ;
        }
        @strongify(self);
        /// CoderMikeHe Fixed Bug : 这里必须用self.fileSource 而不是 fileSource ,因为这是异步，会导致 fileSource == nil;
        /// 保存上传资源
        @weakify(self);
        [self.fileSource saveFileSourceToDB:^(BOOL rst) {
            !complete ? : complete(rst);
            @strongify(self);
            /// 这里需要开始上传
            if (rst) {
                [[CMHFileUploadManager sharedManager] uploadSource:self.sourceId];
            }else{
                [MBProgressHUD mh_showTips:@"保存上传资源失败！！！"];
            }
        }];
    }];
} failure:^(NSURLSessionDataTask * _Nullable task, NSError *error) {
    /// 回调错误
    !complete ? : complete(NO);
    /// show error
    [MBProgressHUD mh_showErrorTips:error];
}];
}
```
然后，我们需要将文件块`CMHFileBlock`按照`512k`的大小切割成多个文件片`CMHFileFragment`，这里的代码实现和属性生成都是参照这篇文章👉[HTTP断点续传与断点上传之 -- 文件流操作](https://blog.csdn.net/wuyoujian0313/article/details/51850540)来实现的。关键代码如下:
```
// 切分文件片段
- (void)_cutFileForFragments {
  
  NSUInteger offset = CMHFileFragmentMaxSize;
  // 总片数
  NSUInteger totalFileFragment = (self.totalFileSize%offset==0)?(self.totalFileSize/offset):(self.totalFileSize/(offset) + 1);
  self.totalFileFragment = totalFileFragment;
  NSMutableArray<CMHFileFragment *> *fragments = [[NSMutableArray alloc] initWithCapacity:0];
  for (NSUInteger i = 0; i < totalFileFragment; i ++) {
      
      CMHFileFragment *fFragment = [[CMHFileFragment alloc] init];
      fFragment.fragmentIndex = i;
      fFragment.uploadStatus = CMHFileUploadStatusWaiting;
      fFragment.fragmentOffset = i * offset;
      if (i != totalFileFragment - 1) {
          fFragment.fragmentSize = offset;
      } else {
          fFragment.fragmentSize = self.totalFileSize - fFragment.fragmentOffset;
      }
      
      /// 关联属性
      fFragment.fileId = self.fileId;
      fFragment.sourceId = self.sourceId;
      fFragment.filePath = self.filePath;
      fFragment.totalFileFragment = self.totalFileFragment ;
      fFragment.totalFileSize = self.totalFileSize;
      
      fFragment.fileType = self.fileType;
      fFragment.fileName = [NSString stringWithFormat:@"%@-%ld.%@",self.fileId , (long)i , self.fileName.pathExtension];
      
      
      [fragments addObject:fFragment];
  }
  self.fileFragments = fragments.copy;
}
```
最后，我们知道`一份上传资源`由`多个文件块`组成，而`一个文件块`由`多个文件片`组成。所以我们是不是可以这样理解：`一份上传资源`由`多个文件片`组成。前提是要保证每一个文件片，必须含有两个属性`sourceId`和`fileId`。
`sourceId` : 代表这个文件片所属于哪个资源。
`fileId` : 代表这个文件片所属于哪个文件块。
`一份上传资源`由`多个文件片`组成的代码实现，无非就是重写`CMHFileSource`的`setFileBlocks`即可。关键代码如下：
```
- (void)setFileBlocks:(NSArray<CMHFileBlock *> *)fileBlocks{
  _fileBlocks = fileBlocks.copy;
  
  NSMutableArray *fileFragments = [NSMutableArray array];
  
  for (CMHFileBlock *fileBlock in fileBlocks) {
      [fileFragments addObjectsFromArray:fileBlock.fileFragments];
      self.totalFileFragment = self.totalFileFragment + fileBlock.totalFileFragment;
      self.totalFileSize = self.totalFileSize + fileBlock.totalFileSize;
  }
  self.fileFragments = fileFragments.copy;
}
```
当然，我们需要将`CMHSource`、`CMHFileSource`、`CMHFileFragment`保存到数据库即可。
分片上传
分片上传是本Demo中一个比较重要的功能点，但其实功能点并不难，主要复杂的还是业务逻辑以及数据库处理。分片上传，其原理还是文件上传，某个文件片的上传和我们平时上传头像的逻辑一模一样，不同点无非就是我们需要利用数据库去记录每一片的上传状态罢了。详情请参考：`CMHFileUploadManager.h/m`
这里笔者以`CMHFileUploadManager`上传某个资源为例，具体讲讲其中的逻辑以及细节处理。具体的代码实现请参考:`- (void)uploadSource:(NSString *)sourceId;`的实现。**注意：笔者提供的Demo，一次只能上传一个资源**。关于具体的业务逻辑分析，笔者已经写在写在代码注释里面了，这里就不再赘述，还请结合代码注释去理解具体的业务逻辑和场景。关键代码如下：
```
/// 上传资源 <核心方法>
- (void)uploadSource:(NSString *)sourceId{
    
    if (!MHStringIsNotEmpty(sourceId)) { return; }
    
    /// CoderMikeHe Fixed Bug : 解决初次加载的问题,不需要验证网络
    if (self.isLoaded) {
        if (![AFNetworkReachabilityManager sharedManager].isReachable) { /// 没有网络
            [self postFileUploadStatusDidChangedNotification:sourceId];
            return;
        }
    }
    self.loaded = YES;
    
    
    /// - 获取该资源下所有未上传完成的文件片
    NSArray *uploadFileFragments = [CMHFileFragment fetchAllWaitingForUploadFileFragment:sourceId];
    
    if (uploadFileFragments.count == 0) {
        
        /// 没有要上传的文件片
        
        /// 获取上传资源
        CMHFileSource *fileSource = [CMHFileSource fetchFileSource:sourceId];
        /// 获取资源
        CMHSource *source = [CMHSource fetchSource:sourceId];
        
        if (MHObjectIsNil(source)) {
            
            /// 提交下一个资源
            [self _autoUploadSource:sourceId reUpload:NO];
            
            /// 没有资源，则🈶何须上传资源，将数据库里面清掉
            [CMHFileSource removeFileSourceFromDB:sourceId complete:NULL];
            /// 通知草稿页 删除词条数据
            [[NSNotificationCenter defaultCenter] postNotificationName:CMHFileUploadDidFinishedNotification object:nil userInfo:@{CMHFileUploadSourceIdKey : sourceId}];
            return;
        }
        
        if (MHObjectIsNil(fileSource)) {
            
            /// 提交资源
            [self _autoUploadSource:sourceId reUpload:NO];
            
            /// 没有上传资源 ，则直接提交
            [[CMHFileUploadManager sharedManager] postFileUploadDisableStatusNotification:sourceId fileUploadDisabled:YES];
            [self _commitSource:sourceId];
            return;
        }
        
        if (fileSource.totalFileFragment <= 0) {
            
            /// 提交资源
            [self _autoUploadSource:sourceId reUpload:NO];
            
            /// 没有上传文件片
            [[CMHFileUploadManager sharedManager] postFileUploadDisableStatusNotification:sourceId fileUploadDisabled:YES];
            [self _commitSource:sourceId];
            return;
        }
        
        /// 倒了这里 ， 证明 fileSource,source 有值，且 fileSource.totalFileFragment > 0
        CMHFileUploadStatus uploadStatus = [CMHFileSource fetchFileUploadStatus:sourceId];
        if (uploadStatus == CMHFileUploadStatusFinished) {
            // 文件全部上传成
            dispatch_time_t delayTime = dispatch_time(DISPATCH_TIME_NOW, (int64_t)(.25/*延迟执行时间*/ * NSEC_PER_SEC));
            dispatch_after(delayTime, dispatch_get_main_queue(), ^{
                /// 检查服务器的文件上传合成状态
                [self _checkFileFragmentSynthetiseStatusFromService:sourceId];
            });
        }else{
            /// 到了这里，则证明这个草稿永远都不会上传成功了，这里很遗憾则需要将其从数据库中移除
            /// 提交资源
            [self _autoUploadSource:sourceId reUpload:NO];
            
            [CMHSource removeSourceFromDB:sourceId complete:NULL];
            /// 通知草稿页 删除这条数据
            [[NSNotificationCenter defaultCenter] postNotificationName:CMHFileUploadDidFinishedNotification object:nil userInfo:@{CMHFileUploadSourceIdKey : sourceId}];
        }
        return;
    }
    
    
    /// 0. 这里一定会新建一个新的上传队列，一定会开启一个新的任务
    /// - 看是否存在于上传数组中
    NSString *findSid = nil;
    /// - 是否有文件正在上传
    BOOL isUploading = NO;
    
    for (NSString *sid in self.uploadFileArray) {
        /// 上传资源里面已经存在了，findSid
        if ([sid isEqualToString:sourceId]) {
            findSid = sid;
        }
        /// 查看当前是否有上传任务正在上传
        CMHFileUploadQueue *queue = [self.uploadFileQueueDict objectForKey:sid];
        if (queue && !queue.isSuspended) {
            isUploading = YES;
        }
    }
    
    /// 2. 检查状态，插入数据，
    if (findSid) { /// 已经存在了，那就先删除，后插入到第0个元素
        [self.uploadFileArray removeObject:findSid];
        [self.uploadFileArray insertObject:sourceId atIndex:0];
    }else{ /// 不存在上传资源数组中，直接插入到第0个元素
        [self.uploadFileArray insertObject:sourceId atIndex:0];
    }
    
    /// 3. 检查是否已经有上传任务了
    if (isUploading) { /// 已经有正在上传任务了，则不需要开启队列了,就请继续等待
        /// 发送通知
        [self postFileUploadStatusDidChangedNotification:sourceId];
        return;
    }
    /// 4. 如果没有上传任务，你就创建队里开启任务即可
    /// 更新这个上传文件的状态 为 `正在上传的状态`
    [self updateUpLoadStatus:CMHFileUploadStatusUploading sourceId:sourceId];
    
    /// 创建信号量 用于线程同步
    dispatch_semaphore_t semaphore = dispatch_semaphore_create(0);
    /// 创建一个队列组
    dispatch_group_t group = dispatch_group_create();
    /// 操作数
    NSMutableArray *operations = [NSMutableArray array];
    
    /// 这里采用串行队列且串行请求的方式处理每一片的上传
    for (CMHFileFragment *ff in uploadFileFragments) {
        /// 进组
        dispatch_group_enter(group);
        // 创建对象，封装操作
        NSBlockOperation *operation = [NSBlockOperation blockOperationWithBlock:^{
            
            /// 切记：任务(网络请求)是串行执行的 ，但网络请求结果回调是异步的、
            [self _uploadFileFragment:ff
                             progress:^(NSProgress *progress) {
                                 NSLog(@" \n上传文件ID👉【%@】\n上传文件片👉 【%ld】\n上传进度为👉【%@】",ff.fileId, (long)ff.fragmentIndex, progress.localizedDescription);
                             }
                              success:^(id responseObject) {
                                  /// 处理成功的文件片
                                  [self _handleUploadFileFragment:ff];
                                  /// 退组
                                  dispatch_group_leave(group);
                                  /// 信号量+1 向下运行
                                  dispatch_semaphore_signal(semaphore);
                              } failure:^(NSError *error) {
                                  /// 更新数据
                                  /// 某片上传失败
                                  [ff updateFileFragmentUploadStatus:CMHFileUploadStatusWaiting];
                                  /// 退组
                                  dispatch_group_leave(group);
                                  /// 信号量+1 向下运行
                                  dispatch_semaphore_signal(semaphore);
                                  
                              }];
            /// 等待
            dispatch_semaphore_wait(semaphore, DISPATCH_TIME_FOREVER);
        }];
        /// 添加操作数组
        [operations addObject:operation];
    }
    /// 创建NSOperationQueue
    CMHFileUploadQueue * uploadFileQueue = [[CMHFileUploadQueue alloc] init];
    /// 存起来
    [self.uploadFileQueueDict setObject:uploadFileQueue forKey:sourceId];
    /// 把操作添加到队列中 不需要设置为等待
    [uploadFileQueue addOperations:operations waitUntilFinished:NO];
    
    /// 队列组的操作全部完成
    dispatch_group_notify(group, dispatch_get_main_queue(), ^{
        NSLog(@"😁😁😁+++dispatch_group_notify+++😁😁😁");
        /// 0. 如果运行到这，证明此`Queue`里面的所有操作都已经全部完成了，你如果再使用 [queue setSuspended:YES/NO];将没有任何意义，所以你必须将其移除掉
        [self.uploadFileQueueDict removeObjectForKey:sourceId];
        /// 1. 队列完毕了，清除掉当前的资源，开启下一个资源
        [self _removeSourceFromUploadFileArray:sourceId];
        /// CoderMikeHe: 这里先不更新草稿页的状态，等提交完表格再去发送通知
        /// 检查一下资源上传
        [self _uploadSourceEnd:sourceId];
    });
    
    //// 告知外界其资源状态改过了
    [self postFileUploadStatusDidChangedNotification:sourceId];
}
```
这里对上传资源下的需要上传的文件片做了循环的上传，由于网络请求是一个异步的操作，同时也考虑到太多并发（当然系统对于网络请求开辟的线程个数也有限制）对于手机性能的影响，因此利用GCD信号量等待这种功能特性`让一个片段上传完之后再进行下一个片段的上传`。
文件上传核心代码如下：
```
/// 上传某一片文件 这里用作测试
- (void)_uploadFileFragment:(CMHFileFragment *)fileFragment
                   progress:(nullable void (^)(NSProgress * _Nonnull))uploadProgress
                    success:(void (^)(id responseObject))success
                    failure:(void (^)(NSError *error))failure{
    /// 获取上传参数
    NSDictionary *parameters = [fileFragment fetchUploadParamsInfo];
    /// 获取上传数据
    NSData *fileData = [fileFragment fetchFileFragmentData];
    
    /// 资源文件找不到，则直接修改数据库，无论如何也得让用户把资源提交上去，而不是让其永远卡在草稿页里，这样太影响用户体验了
    if (fileData == nil) {
        /// CoderMikeHe Fixed Bug : V1.6.7之前 修复文件丢失的情况
        /// 1. 获取该片所处的资源
        CMHFileSource *uploadSource = [CMHFileSource fetchFileSource:fileFragment.sourceId];
        /// 取出fileID
        NSMutableArray *fileIds = [NSMutableArray arrayWithArray:uploadSource.fileIds.yy_modelToJSONObject];
        
        NSLog(@"😭😭😭😭 Before -- 文件<%@>未找到个数 %ld <%@> 😭😭😭😭",fileFragment.fileId , fileIds.count, fileIds);
        if ([fileIds containsObject:fileFragment.fileId]) {
            /// 数据库包含
            [fileIds removeObject:fileFragment.fileId];
            uploadSource.fileIds = fileIds.yy_modelToJSONString;
            /// 更新数据库
            [uploadSource saveOrUpdate];
        }
        NSLog(@"😭😭😭😭 After -- 文件<%@>未找到个数 %ld <%@> 😭😭😭😭",fileFragment.fileId , fileIds.count, fileIds);
        
        /// 一定要回调为成功，让用户误以为正在上传，而不是直接卡死在草稿页
        NSDictionary *responseObj = @{@"code" : @200};
        !success ? : success(responseObj);
        return;
    }
    
    /// 这里笔者只是模拟一下网络情况哈，不要在乎这些细节 ，
    /// 类似于实际开发中调用服务器的API:  /fileSection/upload.do
    /// 2. 以下通过真实的网络请求去模拟获取 文件ID的场景 https://live.9158.com/Room/GetHotTab?devicetype=2&isEnglish=0&version=1.0.1
    /// 1. 配置参数
    CMHKeyedSubscript *subscript = [CMHKeyedSubscript subscript];
    subscript[@"isEnglish"] = @0;
    subscript[@"devicetype"] = @2;
    subscript[@"version"] = @"1.0.1";
    
    /// 2. 配置参数模型
    CMHURLParameters *paramters = [CMHURLParameters urlParametersWithMethod:CMH_HTTTP_METHOD_GET path:CMH_GET_HOT_TAB parameters:subscript.dictionary];
    /// 3. 发起请求
    [[CMHHTTPRequest requestWithParameters:paramters] enqueueResultClass:nil parsedResult:YES success:^(NSURLSessionDataTask *task, id  _Nullable responseObject) {
#warning CMH TODO 稍微延迟一下，模拟现实情况下的上传进度
        NSInteger randomNum = [NSObject mh_randomNumber:0 to:5];
        [NSThread sleepForTimeInterval:0.1 * randomNum];
        
        !success ? : success(responseObject);
    } failure:^(NSURLSessionDataTask * _Nullable task, NSError *error) {
        !failure ? : failure(error);
    }];
#if 0
    /// 这个是真实上传，请根据自身实际项目出发  /fileSection/upload.do
    [self _uploadFileFragmentWithParameters:parameters
                                   fileType:fileFragment.fileType
                                   fileData:fileData
                                   fileName:fileFragment.fileName
                                   progress:uploadProgress
                                    success:success
                                    failure:failure];
#endif
    
}
/// 实际开发项目中上传每一片文件，这里请结合自身项目开发去设计
- (NSURLSessionDataTask *)_uploadFileFragmentWithParameters:(NSDictionary *)parameters
                                                   fileType:(CMHFileType)fileType
                                                   fileData:(NSData *)fileData
                                                   fileName:(NSString *)fileName
                                                   progress:(void (^)(NSProgress *))uploadProgress
                                                    success:(void (^)(id responseObject))success
                                                    failure:(void (^)(NSError *error))failure{
    /// 配置成服务器想要的样式
    NSMutableArray *paramsArray = [NSMutableArray array];
    [paramsArray addObject:parameters];
    
    /// 生成jsonString
    NSString *jsonStr = [paramsArray yy_modelToJSONString];
    
    /// 设置TTPHeaderField
    [self.uploadService.requestSerializer setValue:jsonStr forHTTPHeaderField:@"file_block"];
    /// 开启文件任务上传
    /// PS : 着了完全可以看成，我们平常上传头像给服务器一样的处理方式
    NSURLSessionDataTask *uploadTask = [self.uploadService POST:@"/fileSection/upload.do" parameters:nil/** 一般这里传的是基本参数 */ constructingBodyWithBlock:^(id<AFMultipartFormData>  _Nonnull formData) {
        
        /// 拼接mimeType
        NSString *mimeType = [NSString stringWithFormat:@"%@/%@",(fileType == CMHFileTypePicture) ? @"image":@"video",[[fileName componentsSeparatedByString:@"."] lastObject]];
        
        /// 拼接数据
        [formData appendPartWithFileData:fileData name:@"sectionFile" fileName:fileName mimeType:mimeType];
        
    } progress:^(NSProgress * progress) {
        !uploadProgress ? : uploadProgress(progress);
    } success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject) {
        !success ? : success(responseObject);
    } failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error) {
        !failure ? : failure(error);
    }];
    return uploadTask;
}
```
检查服务器文件上传合成情况的核心代码如下:
```
/// 检查服务器文件片合成情况
- (void)_checkFileFragmentSynthetiseStatusFromService:(NSString *)sourceId{
    
    /// 这里调用服务器的接口检查文件上传状态，以这个为标准
    CMHFileSource *uploadSource = [CMHFileSource fetchFileSource:sourceId];
    /// 没意义
    if (uploadSource == nil) { return; }
    
    /// 如果这里进来了，则证明准备验证文件片和提交表单，则草稿里面的这块表单，你不能在让用户去点击了
    [self postFileUploadDisableStatusNotification:sourceId fileUploadDisabled:YES];
    
    /// V1.6.5之前的接口老数据
    if (!MHStringIsNotEmpty(uploadSource.fileIds)) {
        /// 这里可能是老数据，直接认为成功，就不要去跟服务器打交道了
        /// 成功
        [self _commitSource:sourceId];
        /// 上传下一个
        [self _autoUploadSource:sourceId reUpload:NO];
        return;
    }
    /// 这里笔者只是模拟一下网络情况哈，不要在乎这些细节，
    /// 类似于实际开发中调用服务器的API:  /fileSection/isFinish.do
    /// 2. 以下通过真实的网络请求去模拟获取 文件ID的场景 https://live.9158.com/Room/GetHotTab?devicetype=2&isEnglish=0&version=1.0.1
    /// 1. 配置参数
    CMHKeyedSubscript *subscript = [CMHKeyedSubscript subscript];
    subscript[@"isEnglish"] = @0;
    subscript[@"devicetype"] = @2;
    subscript[@"version"] = @"1.0.1";
    
    /// 2. 配置参数模型
    CMHURLParameters *paramters = [CMHURLParameters urlParametersWithMethod:CMH_HTTTP_METHOD_GET path:CMH_GET_HOT_TAB parameters:subscript.dictionary];
    
    /// 3. 发起请求
    [[CMHHTTPRequest requestWithParameters:paramters] enqueueResultClass:nil parsedResult:YES success:^(NSURLSessionDataTask *task, id  _Nullable responseObject) {
        
        /// 模拟后台返回的合成结果
        CMHFileSynthetise *fs = [[CMHFileSynthetise alloc] init];
        NSInteger randomNum = [NSObject mh_randomNumber:0 to:20];
        fs.finishStatus = (randomNum > 0) ? 1 : 0;  /// 模拟服务器合成失败的场景，毕竟合成失败的几率很低
        
        if (fs.finishStatus>0) {
            /// 服务器合成资源文件成功
            /// 成功
            [self _commitSource:sourceId];
            /// 上传下一个
            [self _autoUploadSource:sourceId reUpload:NO];
            return ;
        }
        
        /// 服务器合成资源文件失败， 服务器会把合成失败的 fileId 返回出来
        /// 也就是 "failFileIds" : "fileId0,fileId1,..."的格式返回出来
        /// 这里模拟后台返回合成错误的文件ID, 这里只是演习！！这里只是演习！！
        /// 取出fileID
        NSMutableArray *fileIds = [NSMutableArray arrayWithArray:uploadSource.fileIds.yy_modelToJSONObject];
        /// 模拟只有一个文件ID合成失败
        NSString *failFileIds = fileIds.firstObject;
        fs.failFileIds = failFileIds;
        
        /// 这里才是模拟真实的网络情况
        if (MHStringIsNotEmpty(fs.failFileIds)) {
            /// 1. 回滚数据
            [uploadSource rollbackFailureFile:fs.failureFileIds];
            /// 2. 获取进度
            CGFloat progress = [CMHFileSource fetchUploadProgress:sourceId];
            /// 3. 发送通知
            [MHNotificationCenter postNotificationName:CMHFileUploadProgressDidChangedNotification object:nil userInfo:@{CMHFileUploadSourceIdKey : sourceId , CMHFileUploadProgressDidChangedKey : @(progress)}];
            /// 4. 重新设置回滚数据的经度
            [CMHSource updateSourceProgress:progress sourceId:sourceId];
        }else{
            /// 无需回滚，修改状态即可
            [self postFileUploadStatusDidChangedNotification:sourceId];
        }
        
        /// 合成失败，继续重传失败的片，允许用户点击草稿页的资源
        [self postFileUploadDisableStatusNotification:sourceId fileUploadDisabled:NO];
        /// 重传该资源
        [self _autoUploadSource:sourceId reUpload:YES];
        
    } failure:^(NSURLSessionDataTask * _Nullable task, NSError *error) {
        /// 1. 服务器报错不重传
        [MBProgressHUD mh_showErrorTips:error];
        
        /// 更新资源状态
        [self updateUpLoadStatus:CMHFileUploadStatusWaiting sourceId:sourceId];
        
        /// 更新状态
        [self postFileUploadStatusDidChangedNotification:sourceId];
        /// 文件片合成失败，允许点击
        [self postFileUploadDisableStatusNotification:sourceId fileUploadDisabled:NO];
    }];
}
```
总之，文件分片上传逻辑不止上面这一点点内容，还有存在许多逻辑处理和细节注意，比如暂停上传资源；开始上传资源；取消上传资源；取消所有上传资源；服务器合成某些文件失败，客户端回滚数据库，重传失败的文件片；某个资源上传后自动重传下个资源....等等。大家有兴趣可以查看`CMHFileUploadManager.h`提供的API的具体实现。 `CMHFileUploadManager.h`的所有内容如下：
```
/// 某资源的所有片数据上传，完成也就是提交资源到服务器成功。
FOUNDATION_EXTERN NSString *const CMHFileUploadDidFinishedNotification;
/// 资源文件上传状态改变的通知
FOUNDATION_EXTERN NSString *const CMHFileUploadStatusDidChangedNotification;
/// 草稿上传文件状态 disable 是否不能点击 如果为YES 不要修改草稿页表单的上传状态 主需要让用户不允许点击上传按钮
FOUNDATION_EXTERN NSString *const CMHFileUploadDisableStatusKey;
FOUNDATION_EXTERN NSString *const CMHFileUploadDisableStatusNotification;
/// 某资源中的某片数据上传完成
FOUNDATION_EXTERN NSString *const CMHFileUploadProgressDidChangedNotification;
/// 某资源的id
FOUNDATION_EXTERN NSString *const CMHFileUploadSourceIdKey;
/// 某资源的进度
FOUNDATION_EXTERN NSString *const CMHFileUploadProgressDidChangedKey;
@interface CMHFileUploadManager : NSObject
/// 存放操作队列的字典
@property (nonatomic , readonly , strong) NSMutableDictionary *uploadFileQueueDict;
/// 声明单例
+ (instancetype)sharedManager;
/// 销毁单例
+ (void)deallocManager;
/// 基础配置，主要是后台上传草稿数据  一般这个方法会放在 程序启动后切换到主页时调用
- (void)configure;
/// 上传资源
/// sourceId:文件组Id
- (void)uploadSource:(NSString *)sourceId;
/// 暂停上传 -- 用户操作
/// sourceId: 资源Id
- (void)suspendUpload:(NSString *)sourceId;
/// 继续上传 -- 用户操作
/// sourceId: 资源Id
- (void)resumeUpload:(NSString *)sourceId;
/// 取消掉上传 -- 用户操作
/// sourceId: 资源Id
- (void)cancelUpload:(NSString *)sourceId;
/// 取消掉所有上传 一般这个方法会放在 程序启动后切换到登录页时调用
- (void)cancelAllUpload;
/// 删除当前用户无效的资源
- (void)clearInvalidDiskCache;
//// 以下方法跟服务器交互，只管调用即可，无需回调，
/// 清除掉已经上传到服务器的文件片 fileSection
- (void)deleteUploadedFile:(NSString *)sourceId;
/// 告知草稿页，某个资源的上传状态改变
/// sourceId -- 资源ID
- (void)postFileUploadStatusDidChangedNotification:(NSString *)sourceId;
/// 告知草稿页，某个资源不允许点击
- (void)postFileUploadDisableStatusNotification:(NSString *)sourceId fileUploadDisabled:(BOOL)fileUploadDisabled;
/// 更新资源的状态
/// uploadStatus -- 上传状态
/// sourceId -- 资源ID
- (void)updateUpLoadStatus:(CMHFileUploadStatus)uploadStatus sourceId:(NSString *)sourceId;
@end
```
总结
以上内容，就是笔者在做`大文件分片上传`的过程中的心得体会。看似简单的`文件分片上传`功能，但其中涵盖的知识面还是比较广的，结合笔者前面谈及的必备知识点，大家业余时间可以系统去学习和掌握，最后笔者还是建议大家把`多线程`的相关知识恶补一下和实践起来。当然这其中肯定还有一些细小的逻辑和细节问题还未暴露出来，如果大家在使用和查看过程中发现问题或者不理解的地方，以及如果有好的建议或意见都可以在底部👇评论区指出。
期待
- 文章若对您有点帮助，请给个喜欢❤️，毕竟码字不易；若对您没啥帮助，请给点建议💗，切记学无止境。
- 针对文章所述内容，阅读期间任何疑问；请在文章底部批评指正，我会火速解决和修正问题。
- GitHub地址：[https://github.com/CoderMikeHe](https://github.com/CoderMikeHe)
- 源码地址：
[MHDevelopExample目录中的Architecture/Contacts/FileUpload文件夹中](https://github.com/CoderMikeHe/MHDevelopExample_Objective_C) <**特别强调：** 使用前请全局搜索 `CMHDEBUG` 字段并将该**宏**置为 `1`即可，默认是`0` >
作者：CoderMikeHe
链接：https://www.jianshu.com/p/7919c620967e
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
