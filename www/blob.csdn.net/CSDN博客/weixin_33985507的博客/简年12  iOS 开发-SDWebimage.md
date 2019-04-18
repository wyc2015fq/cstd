# 简年12:  iOS 开发-SDWebimage - weixin_33985507的博客 - CSDN博客
2017年01月17日 22:29:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
写在前面：观看了关于SDWebimage的视频介绍，讲得很好，分享一下。
- 代理中的内存警告操作
```
-(void)applicationDidReceiveMemoryWarning:(UIApplication *)application
{
    //1.清空缓存
    //clear:直接删除缓存目录下面的文件,然后重新创建空的缓存文件
    //clean:清除过期缓存,计算当前缓存的大小,和设置的最大缓存数量比较,如果超出那么会继续删除(按照文件了创建的先后顺序)
    //过期时间:7天
    [[SDWebImageManager sharedManager].imageCache clearMemory];
    
    //2.取消当前所有的操作
    [[SDWebImageManager sharedManager] cancelAll];
    
    //3.最大并发数量 == 6
    //4.缓存文件的保存名称如何处理? 拿到图片的URL路径,对该路径进行MD5加密
    //5.该框架内部对内存警告的处理方式? 内部通过监听通知的方式请你缓存
    //6.该框架进行缓存处理的方式:可变字典--->NSCache
    //7.如何判断图片的类型: 在判断图片类型的时候，只匹配第一个字节
    //8.队列中任务的处理方式:FIFO（队列，先进先出）
    //9.如何下载图片的? 发送网络请求下载图片,NSURLConnection
    //10.请求超时的时间 15秒
    
    //[NSData dataWithContentsOfURL:<#(nonnull NSURL *)#>]
}
```
2.基本使用
```
#import "ViewController.h"
#import "UIImageView+WebCache.h"
#import "SDWebImageManager.h"
#import "SDWebImageDownloader.h"
#import "UIImage+GIF.h"
#import "NSData+ImageContentType.h"
@interface ViewController ()
@property (weak, nonatomic) IBOutlet UIImageView *imageView;
@end
@implementation ViewController
-(void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event
{
    [self download];
}
//1.下载图片且需要获取下载进度
//内存缓存&磁盘缓存
-(void)download
{
    [self.imageView sd_setImageWithURL:[NSURL URLWithString:@"http://img4.duitang.com/uploads/blog/201310/18/20131018213446_smUw4.thumb.600_0.jpeg"] placeholderImage:[UIImage imageNamed:@"Snip20160221_306"] options:SDWebImageCacheMemoryOnly | SDWebImageProgressiveDownload progress:^(NSInteger receivedSize, NSInteger expectedSize) {
        
    } completed:^(UIImage *image, NSError *error, SDImageCacheType cacheType, NSURL *imageURL) {
        
        switch (cacheType) {
            case SDImageCacheTypeNone:
                NSLog(@"直接下载");
                break;
            case SDImageCacheTypeDisk:
                NSLog(@"磁盘缓存");
                break;
            case SDImageCacheTypeMemory:
                NSLog(@"内存缓存");
                break;
            default:
                break;
        }
    }];
    
    NSLog(@"%@",[NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES) lastObject]);
    
}
//2.只需要简单获得一张图片,不设置
//内存缓存&磁盘缓存
-(void)download2
{
    [[SDWebImageManager sharedManager] downloadImageWithURL:[NSURL URLWithString:@"http://img4.duitang.com/uploads/blog/201310/18/20131018213446_smUw4.thumb.600_0.jpeg"] options:0 progress:^(NSInteger receivedSize, NSInteger expectedSize) {
        NSLog(@"%f",1.0 * receivedSize / expectedSize);
        
    } completed:^(UIImage *image, NSError *error, SDImageCacheType cacheType, BOOL finished, NSURL *imageURL) {
        
        //得到图片
        self.imageView.image = image;
    }];
}
//3.不需要任何的缓存处理
//没有做任何缓存处理|
-(void)download3
{
    //data:图片的二进制数据
    [[SDWebImageDownloader sharedDownloader] downloadImageWithURL:[NSURL URLWithString:@"http://img4.duitang.com/uploads/blog/201310/18/20131018213446_smUw4.thumb.600_0.jpeg"] options:0 progress:^(NSInteger receivedSize, NSInteger expectedSize) {
        
    } completed:^(UIImage *image, NSData *data, NSError *error, BOOL finished) {
        
        [[NSOperationQueue mainQueue]addOperationWithBlock:^{
             self.imageView.image = image;
        }];
       
    }];
}
//4.播放Gif图片
-(void)gif
{
    NSLog(@"%s",__func__);
    //self.imageView.image = [UIImage imageNamed:@"39e805d5ad6eddc4f80259d23bdbb6fd536633ca"];
    
    UIImage *image = [UIImage sd_animatedGIFNamed:@"39e805d5ad6eddc4f80259d23bdbb6fd536633ca"];
    self.imageView.image = image;
}
-(void)type
{
    NSData *imageData = [NSData dataWithContentsOfFile:@"/Users/xiaomage/Desktop/Snip20160221_306.png"];
    NSString *typeStr = [NSData sd_contentTypeForImageData:imageData];
    NSLog(@"%@",typeStr);
}
@end
```
SDWebImage注解： [https://pan.baidu.com/s/1c1G0bHA](https://link.jianshu.com?t=https://pan.baidu.com/s/1c1G0bHA)
