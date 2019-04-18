# YYImage - weixin_33985507的博客 - CSDN博客
2017年04月11日 10:42:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
# YYImage特性
- 支持以下类型动画图像的播放/编码/解码:
WebP, APNG, GIF。
- 支持以下类型静态图像的显示/编码/解码:
WebP, PNG, GIF, JPEG, JP2, TIFF, BMP, ICO, ICNS。
- 支持以下类型图片的渐进式/逐行扫描/隔行扫描解码:
PNG, GIF, JPEG, BMP。
- 支持多张图片构成的帧动画播放，支持单张图片的 sprite sheet 动画。
- 完全兼容 UIImage 和 UIImageView，使用方便。
- 保留可扩展的接口，以支持自定义动画。
- 高效的动态内存缓存管理，以保证高性能低内存的动画播放。
# 用法
### 显示动画类型的图片
```
// 文件: mygif@3x.gif
UIImage *image = [YYImage imageNamed:@"mygif.gif"];
UIImageView *imageView = [[YYAnimatedImageView alloc] initWithImage:image];
[self.view addSubView:imageView];
```
### 播放帧动画
```
// 文件: frame1.png, frame2.png, frame3.png
NSArray *paths = @[@"/ani/frame1.png", @"/ani/frame2.png", @"/ani/frame3.png"];
NSArray *times = @[@0.1, @0.2, @0.1];
UIImage *image = [YYFrameImage alloc] initWithImagePaths:paths frameDurations:times repeats:YES];
UIImageView *imageView = [YYAnimatedImageView alloc] initWithImage:image];
[self.view addSubView:imageView];
```
### 播放sprite sheet动画
```
// 8 * 12 sprites in a single sheet image
UIImage *spriteSheet = [UIImage imageNamed:@"sprite-sheet"];
NSMutableArray *contentRects = [NSMutableArray new];
NSMutableArray *durations = [NSMutableArray new];
for (int j = 0; j < 12; j++) {
   for (int i = 0; i < 8; i++) {
       CGRect rect;
       rect.size = CGSizeMake(img.size.width / 8, img.size.height / 12);
       rect.origin.x = img.size.width / 8 * i;
       rect.origin.y = img.size.height / 12 * j;
       [contentRects addObject:[NSValue valueWithCGRect:rect]];
       [durations addObject:@(1 / 60.0)];
   }
}
YYSpriteSheetImage *sprite;
sprite = [[YYSpriteSheetImage alloc] initWithSpriteSheetImage:img
                                                contentRects:contentRects
                                              frameDurations:durations
                                                   loopCount:0];
YYAnimatedImageView *imageView = [YYAnimatedImageView new];
imageView.size = CGSizeMake(img.size.width / 8, img.size.height / 12);
imageView.image = sprite;
[self.view addSubView:imageView];
```
### 动画播放控制
```
YYAnimatedImageView *imageView = ...;
// 暂停:
[imageView stopAnimating];
// 播放:
[imageView startAnimating];
// 设置播放进度:
imageView.currentAnimatedImageIndex = 12;
// 获取播放状态:
image.currentIsPlayingAnimation;
//上面两个属性都支持 KVO。
```
### 图片解码
```
// 解码单帧图片:
NSData *data = [NSData dataWithContentOfFile:@"/tmp/image.webp"];
YYImageDecoder *decoder = [YYImageDecoder decoderWithData:data scale:2.0];
UIImage image = [decoder frameAtIndex:0 decodeForDisplay:YES].image;
// 渐进式图片解码 (可用于图片下载显示):
NSMutableData *data = [NSMutableData new];
YYImageDecoder *decoder = [[YYImageDecoder alloc] initWithScale:2.0];
while(newDataArrived) {
   [data appendData:newData];
   [decoder updateData:data final:NO];
   if (decoder.frameCount > 0) {
       UIImage image = [decoder frameAtIndex:0 decodeForDisplay:YES].image;
       // progressive display...
   }
}
[decoder updateData:data final:YES];
UIImage image = [decoder frameAtIndex:0 decodeForDisplay:YES].image;
// final display...
```
### 图片编码
```
// 编码静态图 (支持各种常见图片格式):
YYImageEncoder *jpegEncoder = [[YYImageEncoder alloc] initWithType:YYImageTypeJPEG];
jpegEncoder.quality = 0.9;
[jpegEncoder addImage:image duration:0];
NSData jpegData = [jpegEncoder encode];
// 编码动态图 (支持 GIF/APNG/WebP):
YYImageEncoder *webpEncoder = [[YYImageEncoder alloc] initWithType:YYImageTypeWebP];
webpEncoder.loopCount = 5;
[webpEncoder addImage:image0 duration:0.1];
[webpEncoder addImage:image1 duration:0.15];
[webpEncoder addImage:image2 duration:0.2];
NSData webpData = [webpEncoder encode];
```
### 图片类型检测
```
// 获取图片类型
YYImageType type = YYImageDetectType(data); 
if (type == YYImageTypePNG) ...
```
