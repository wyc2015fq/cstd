# 本地音频文件合并，m4a转wav - weixin_33985507的博客 - CSDN博客
2019年01月07日 17:59:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
```
- (void)sourceURLs:(NSArray *)sources composeToURL:(NSURL *)toURL completed:(void (^)(NSError *error)) completed{
    
    NSAssert(sources.count > 1,@"源文件不足两个无需合并");
    
    //  1. 创建`AVMutableComposition `,用于合并所有的音视频文件
    AVMutableComposition* mixComposition = [AVMutableComposition composition];
    
    
    //  2. 给`AVMutableComposition` 添加一个新音频的轨道,并返回音频轨道
    AVMutableCompositionTrack *compositionAudioTrack = [mixComposition addMutableTrackWithMediaType:AVMediaTypeAudio preferredTrackID:kCMPersistentTrackID_Invalid];
    //  3. 循环添加需要的音频资源
    
    //  3.1 音频插入的开始时间,用于记录每次添加音频文件的开始时间
    CMTime beginTime = kCMTimeZero;
    //  3.2 用于记录错误的对象
    NSError *error = nil;
    //  3.3 循环添加音频资源 
    for (NSString *fileName in sources) {
        NSString *name = [NSString stringWithFormat:@"%@.caf",fileName];
        NSURL *sourceURL = [[NSBundle mainBundle] URLForResource:name withExtension:nil];
        //      3.3.1 音频文件资源
        AVURLAsset  *audioAsset = [[AVURLAsset alloc]initWithURL:sourceURL options:nil];
        //      3.3.2 需要合并的音频文件的播放的时间区间
        CMTimeRange audio_timeRange = CMTimeRangeMake(kCMTimeZero, audioAsset.duration);
        //      3.3.3 添加音频文件
        //      参数说明:
        //      insertTimeRange:源录音文件的的区间
        //      ofTrack:插入音频的内容
        //      atTime:源音频插入到目标文件开始时间
        //      error: 插入失败记录错误
        //      返回:YES表示插入成功,`NO`表示插入失败
        BOOL success = [compositionAudioTrack insertTimeRange:audio_timeRange ofTrack:[[audioAsset tracksWithMediaType:AVMediaTypeAudio] objectAtIndex:0] atTime:beginTime error:&error];
        //      3.3.4 如果插入失败,打印插入失败信息
        if (!success) {
            NSLog(@"插入音频失败: %@",error);
            completed(error);
            return;
        }
        //     3.3.5  记录下次音频文件插入的开始时间
        beginTime = CMTimeAdd(beginTime, audioAsset.duration);
    }
    
    //  4. 导出合并的音频文件
    //  4.0 创建一个导入M4A格式的音频的导出对象
    AVAssetExportSession* assetExport = [[AVAssetExportSession alloc] initWithAsset:mixComposition presetName:AVAssetExportPresetAppleM4A];
    //  4.2 设置导入音视频的URL
    assetExport.outputURL = toURL;
    //  导出音视频的文件格式
    assetExport.outputFileType = @"com.apple.m4a-audio";
    //  4.3 导入出
    [assetExport exportAsynchronouslyWithCompletionHandler:^{
        //      4.5 分发到主线程
        dispatch_async(dispatch_get_main_queue(), ^{
            completed(assetExport.error);
        });
    }];
}
- (void)convetM4aToWav:(NSURL *)originalUrl  destUrl:(NSURL *)destUrl {
    
    AVURLAsset *songAsset = [AVURLAsset URLAssetWithURL:originalUrl options:nil];
    
    //读取原始文件信息
    NSError *error = nil;
    AVAssetReader *assetReader = [AVAssetReader assetReaderWithAsset:songAsset error:&error];
    if (error) {
        NSLog (@"error: %@", error);
        return;
    }
    
    AVAssetReaderOutput *assetReaderOutput = [AVAssetReaderAudioMixOutput
                                              assetReaderAudioMixOutputWithAudioTracks:songAsset.tracks
                                              audioSettings: nil];
    if (![assetReader canAddOutput:assetReaderOutput]) {
        NSLog (@"can't add reader output... die!");
        return;
    }
    [assetReader addOutput:assetReaderOutput];
    
    
    AVAssetWriter *assetWriter = [AVAssetWriter assetWriterWithURL:destUrl
                                                          fileType:AVFileTypeCoreAudioFormat
                                                             error:&error];
    if (error) {
        NSLog (@"error: %@", error);
        return;
    }
    AudioChannelLayout channelLayout;
    memset(&channelLayout, 0, sizeof(AudioChannelLayout));
    channelLayout.mChannelLayoutTag = kAudioChannelLayoutTag_Stereo;
    NSDictionary *outputSettings = [NSDictionary dictionaryWithObjectsAndKeys:
                                    [NSNumber numberWithInt:kAudioFormatLinearPCM], AVFormatIDKey,
                                    [NSNumber numberWithFloat:16000.0], AVSampleRateKey,
                                    [NSNumber numberWithInt:2], AVNumberOfChannelsKey,
                                    [NSData dataWithBytes:&channelLayout length:sizeof(AudioChannelLayout)], AVChannelLayoutKey,
                                    [NSNumber numberWithInt:16], AVLinearPCMBitDepthKey,
                                    [NSNumber numberWithBool:NO], AVLinearPCMIsNonInterleaved,
                                    [NSNumber numberWithBool:NO],AVLinearPCMIsFloatKey,
                                    [NSNumber numberWithBool:NO], AVLinearPCMIsBigEndianKey,
                                    nil];
    AVAssetWriterInput *assetWriterInput = [AVAssetWriterInput assetWriterInputWithMediaType:AVMediaTypeAudio
                                                                              outputSettings:outputSettings];
    if ([assetWriter canAddInput:assetWriterInput]) {
        [assetWriter addInput:assetWriterInput];
    } else {
        NSLog (@"can't add asset writer input... die!");
        return;
    }
    
    assetWriterInput.expectsMediaDataInRealTime = NO;
    
    [assetWriter startWriting];
    [assetReader startReading];
    
    AVAssetTrack *soundTrack = [songAsset.tracks objectAtIndex:0];
    CMTime startTime = CMTimeMake (0, soundTrack.naturalTimeScale);
    [assetWriter startSessionAtSourceTime:startTime];
    
    __block UInt64 convertedByteCount = 0;
    
    dispatch_queue_t mediaInputQueue = dispatch_queue_create("mediaInputQueue", NULL);
    [assetWriterInput requestMediaDataWhenReadyOnQueue:mediaInputQueue
                                            usingBlock: ^
     {
         while (assetWriterInput.readyForMoreMediaData) {
             CMSampleBufferRef nextBuffer = [assetReaderOutput copyNextSampleBuffer];
             if (nextBuffer) {
                 // append buffer
                 [assetWriterInput appendSampleBuffer: nextBuffer];
                 NSLog (@"appended a buffer (%zu bytes)",
                        CMSampleBufferGetTotalSampleSize (nextBuffer));
                 convertedByteCount += CMSampleBufferGetTotalSampleSize (nextBuffer);
                 
                 
             } else {
                 [assetWriterInput markAsFinished];
                 [assetWriter finishWritingWithCompletionHandler:^{
                     
                 }];
                 [assetReader cancelReading];
                 NSDictionary *outputFileAttributes = [[NSFileManager defaultManager]
                                                       attributesOfItemAtPath:[destUrl path]
                                                       error:nil];
                 
                 NSLog (@"FlyElephant %lld",[outputFileAttributes fileSize]);
                 break;
             }
         }
         
         
     }];
    
}
```
是转载至不同地方。
