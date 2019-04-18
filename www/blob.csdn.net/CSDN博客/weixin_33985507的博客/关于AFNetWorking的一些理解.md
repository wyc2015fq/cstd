# 关于AFNetWorking的一些理解 - weixin_33985507的博客 - CSDN博客
2016年11月14日 15:34:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
AFNetwork是一个轻量级的网络请求api类库。是以NSURLConnection, NSOperation和其他方法为基础的。
下面这个例子是用来处理json请求的：
NSURL *url = [NSURL URLWithString:@"https://alpha-api.app.net/stream/0/posts/stream/global"];
NSURLRequest *request = [NSURLRequest requestWithURL:url];
AFJSONRequestOperation *operation = [AFJSONRequestOperation JSONRequestOperationWithRequest:request success:^(NSURLRequest *request, NSHTTPURLResponse *response, id JSON) {
NSLog(@"App.net Global Stream: %@", JSON);
} failure:nil];
[operation start];
使用方法：
1.下载AFNetwork，点击下载
2.将文件夹名称为AFNetworking拖入到你的工程项目即可
常见问题：
1. AFNetworking作用都有哪些？
NSURLConnection提供了+sendAsynchronousRequest:queue:completionHandler:和+sendAsynchronousRequest:queue:completionHandler: ，但是AFNetworking提供了更好的功能
＊AFURLConnectionOperation和它的子类继承NSOperation的，允许请求被取消，暂停/恢复和由NSOperationQueue进行管理。
＊AFURLConnectionOperation也可以让你轻松得完成上传和下载，处理验证，监控上传和下载进度，控制的缓存。
＊AFHTTPRequestOperation和它得子类可以基于http状态和内容列下来区分是否成功请求了
＊AFNetworking可以将远程媒体数据类型（NSData）转化为可用的格式，比如如JSON，XML，图像和plist。
＊AFHTTPClient提供了一个方便的网络交互接口，包括默认头，身份验证，是否连接到网络，批量处理操作，查询字符串参数序列化，已经多种表单请求
＊的UIImageView+ AFNetworking增加了一个方便的方法来异步加载图像。
2. AFNetworking是否支持缓存？
可以，NSURLCache及其子类提供了很多高级接口用于处理缓存
如果你想将缓存存储再磁盘，推荐使用SDURLCache
3.如何使用AFNetworking上传一个文件？
NSData *imageData = UIImagePNGRepresentation(image);
NSURLRequest *request = [client multipartFormRequestWithMethod:@"POST" path:@"/upload" parameters:nil constructingBodyWithBlock: ^(id  formData) {
[formData appendPartWithFileData:imageData mimeType:@"image/png" name:@"avatar"];
}];
4.如何使用AFNetworking下载一个文件？
先创建一个AFURLConnectionOperation对象，然后再使用它的属性outputStream进行处理
operation.outputStream = [NSOutputStream outputStreamToFileAtPath:@"download.zip" append:NO];
5.如何解决：SystemConfiguration framework not found in project
请导入：
#import
#import
6.当应用程序退出时，如何保持持续的请求？
AFURLConnectionOperation有一个叫setShouldExecuteAsBackgroundTaskWithExpirationHandler:的方法用于处理在应用程序进入后台后，进行持续的请求
[self setShouldExecuteAsBackgroundTaskWithExpirationHandler:^{
}];
一些实例：
1.XML 请求
NSURLRequest *request = [NSURLRequest requestWithURL:[NSURL URLWithString:@"http://api.flickr.com/services/rest/?method=flickr.groups.browse&api_key=b6300e17ad3c506e706cb0072175d047&cat_id=34427469792@N01&format=rest"]];
AFXMLRequestOperation *operation = [AFXMLRequestOperation XMLParserRequestOperationWithRequest:request success:^(NSURLRequest *request, NSHTTPURLResponse *response, NSXMLParser *XMLParser) {
XMLParser.delegate = self;
[XMLParser parse];
} failure:nil];
[operation start];
2.图片请求：
UIImageView *imageView = [[UIImageView alloc] initWithFrame:CGRectMake(0.0f, 0.0f, 100.0f, 100.0f)];
[imageView setImageWithURL:[NSURL URLWithString:@"http://i.imgur.com/r4uwx.jpg"] placeholderImage:[UIImage imageNamed:@"placeholder-avatar"]];
3.图片上传处理，监测上传状态：
didiwei  17:28:57
NSURL *url = [NSURL URLWithString:@"http://api-base-url.com"];
AFHTTPClient *httpClient = [[AFHTTPClient alloc] initWithBaseURL:url];
NSData *imageData = UIImageJPEGRepresentation([UIImage imageNamed:@"avatar.jpg"], 0.5);
NSMutableURLRequest *request = [httpClient multipartFormRequestWithMethod:@"POST" path:@"/upload" parameters:nil constructingBodyWithBlock: ^(id formData) {
[formData appendPartWithFileData:imageData name:@"avatar" fileName:@"avatar.jpg" mimeType:@"image/jpeg"];
}];
AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
[operation setUploadProgressBlock:^(NSUInteger bytesWritten, long long totalBytesWritten, long long totalBytesExpectedToWrite) {
NSLog(@"Sent %lld of %lld bytes", totalBytesWritten, totalBytesExpectedToWrite);
}];
[operation start];
4.在线流媒体请求
NSURLRequest *request = [NSURLRequest requestWithURL:[NSURL URLWithString:@"http://localhost:8080/encode"]];
AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc] initWithRequest:request];
operation.inputStream = [NSInputStream inputStreamWithFileAtPath:[[NSBundle mainBundle] pathForResource:@"large-image" ofType:@"tiff"]];
operation.outputStream = [NSOutputStream outputStreamToMemory];
[operation start];
![2989565-2a29251c2deb972f.png](https://upload-images.jianshu.io/upload_images/2989565-2a29251c2deb972f.png)
这几天在评论页面的时候发现get请求后加入2个以上的汉字会出现BadURL提示，断点调试发现是多中文字符惹的祸，所以在URL使用以前先转码成UTF-8即可
这里顺便说明下自己使用AFNetworking 的常用方法。
目前我只是用到
#import “AFHTTPClient.h”;
#import “AFHTTPRequestOperation.h”
这两个类
发起一个请求
NSString *URLTmp =@”http://www.coneboy.com”;
NSString *URLTmp1 = [URLTmpstringByAddingPercentEscapesUsingEncoding:NSUTF8StringEncoding]; //转码成UTF-8  否则可能会出现错误
URLTmp = URLTmp1;
NSURLRequest *request = [NSURLRequest requestWithURL:[NSURLURLWithString: URLTmp]];
AFHTTPRequestOperation *operation = [[AFHTTPRequestOperation alloc]initWithRequest:request];
[operation setCompletionBlockWithSuccess:^(AFHTTPRequestOperation*operation, id responseObject) {
NSLog(@"Success: %@", operation.responseString);
NSString *requestTmp = [NSStringstringWithString:operation.responseString];
NSData *resData = [[NSData alloc] initWithData:[requestTmpdataUsingEncoding:NSUTF8StringEncoding]];
//系统自带JSON解析
NSDictionary *resultDic = [NSJSONSerializationJSONObjectWithData:resData options:NSJSONReadingMutableLeaveserror:nil];
} failure:^(AFHTTPRequestOperation *operation, NSError *error){
NSLog(@”Failure: %@”, error);
[SVProgressHUD dismissWithError:@"提交失败，请重试"];
}];
[operation start];
}
AFNetworking 下载图片
AFNETWorking 下载图片是异步并且可缓存到cache里面非常好用不会阻塞主线程。用过的都说好！而且使用也简单！
#import “UIImageView+AFNetworking.h”
下面有关于自己的一些看法，AFNetWorking和NSData的区别一点是：
AFNetWorking是将远程媒体数据类型（NSData）转化为可用的格式，比如如JSON，XML，图像和plist。
利用NSData 相关方法可以直接获取到远程媒体数据类型。不需要利用block回调拿取数据。
