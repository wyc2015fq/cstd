# iOS 开发 之 网络编程 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年03月05日 21:49:02[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：393标签：[iOS](https://so.csdn.net/so/search/s.do?q=iOS&t=blog)
个人分类：[iOS开发																[iOS网络编程](https://blog.csdn.net/qq_30513483/article/category/6120589)](https://blog.csdn.net/qq_30513483/article/category/6072895)
如果此文帮助了您,请点击喜欢或评论,如果您喜欢我的文章请关注我,您的支持永远都是我前行的动力.
> 
# 注意
1.发送请求
url : = 网络协议([http://今后可能是https](http://xn--https-1f2hj12achw50mt97c/)) + 文件路径? + 参数1 & 参数2 ...
2.接收响应
3.创建连接对象传输数据
xcode7 之后若想要使用http,需要修改info.plist文件
NSAppTransportSecucurity NSDictionary
NSAllowsArbitraryLoads BOOL YES;
> 
# 第一种get 同步
`NSURL *url = [NSURL URLWithString:BASE_URL];//1.创建URL对象`
`NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];//2.创建请求对象`
`[request setHTTPMethod:@"get"];//2.1 创建请求方式(默认是get,这一步可以不写)`
`NSURLResponse *response = nil;NSError *error = nil;//3.创建响应对象(有时会出错)`
```
NSData *data = [NSURLConnection sendSynchronousRequest:request
 returningResponse:&response error:&error];//4.创建连接对象
```
```
NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:data
 options:NSJSONReadingAllowFragments error:nil];//解析
```
```
self.dataArray = [NSMutableArray arrayWithCapacity:5];for
 (NSDictionary *dict in dic[@"news"]) {//开辟空间News *news = [News new];[news setValuesForKeysWithDictionary:dict]; [_dataArray addObject:news];}for (News *news in _dataArray) { NSLog(@"%@",news);}
```
> 
# 第二种 post 同步
注:浏览器只能模拟get
`NSURL *url = [NSURL URLWithString:URL_PORT1];//1.创建url对象`
`NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];//2.创建请求对象`
`[request setHTTPMethod:@"post"];//2.1(必须写) 创建请求方式`
```
NSData *tempData = [URL_PORT2 dataUsingEncoding:NSUTF8StringEncoding];[request
 setHTTPBody:tempData];//3.设置请求参数
```
`NSURLResponse *response = nil;NSError *error = nil;//4.创建响应对象`
```
NSData *data =[NSURLConnection sendSynchronousRequest:request
 returningResponse:&response error:&error];//5.创建连接对象
```
```
NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:data
 options:NSJSONReadingAllowFragments error:&error];//6.解析NSLog(@"%@",dic);self.dataArray = [NSMutableArray arrayWithCapacity:5]; for (NSDictionary *dicq in dic[@"news"]) { News *news = [News new]; [news setValuesForKeysWithDictionary:dicq]; [_dataArray addObject:news];
 } for (News *ee in _dataArray) { NSLog(@"%@",ee); }
```
> 
# 第三种 GET异步 -- BLOCK
`NSURL *url = [NSURL URLWithString:BASE_URL];//1. 创建URL对象`
`NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];//2.创建请求对象`
```
[NSOperationQueue mainQueue]主队列,多线程内容.__weak typeof (self)temp
 = self;//3.创建连接对象
```
```
[NSURLConnection sendAsynchronousRequest:request queue:[NSOperationQueue
 mainQueue]completionHandler:^(NSURLResponse * _Nullable response, NSData * _Nullable data, NSError * _Nullable connectionError) {
```
```
NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:data
 options:NSJSONReadingAllowFragments error:nil]; NSLog(@"get异步---%d---",[[NSThread currentThread]isMainThread]); temp.dataArray = [NSMutableArray arrayWithCapacity:5]; }];NSLog(@"get异步---%d---",[[NSThread currentThread]isMainThread]);}
```
> 
# 第四种 POST异步 -- BLOCK
`NSURL *url = [NSURL URLWithString:URL_PORT1];//1.创建url对象`
`NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];//2.创建请求对象`
`[request setHTTPMethod:@"post"];//2.1(必须写) 创建请求方式`
```
NSData *tempData = [URL_PORT2 dataUsingEncoding:NSUTF8StringEncoding];
 [request setHTTPBody:tempData];//3.设置请求参数
```
```
__weak typeof (self)ttemp = self;//4.设置创建连接对象[NSURLConnection
 sendAsynchronousRequest:request queue:[NSOperationQueue mainQueue] completionHandler:^(NSURLResponse * _Nullable response, NSData * _Nullable data, NSError * _Nullable connectionError) {if (nil != data) { NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:data
 options:NSJSONReadingAllowFragments error:nil]; NSLog(@"%@",dic); } ttemp.dataArray = [NSMutableArray arrayWithCapacity:5]; }];
```
> 
# 第五种 get异步 -- 代理"
`NSURL *url = [NSURL URLWithString:BASE_URL];//1.创建URL对象`
`NSMutableURLRequest *request = [NSMutableURLRequest requestWithURL:url];//2.创建请求对象`
```
NSURLConnection * connection = [NSURLConnection connectionWithRequest:request
 delegate:self];//3.创建连接对象,并实现代理
```
`[connection start];//4.开始执行`
> 
# get异步代理
NSURLConnectionDataDelegate
```
- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse
 *)response{ //1.接收到服务器的响应
```
`self.tempData = [NSMutableData data];//准备数据接收}`
```
- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData
 *)data{//2.接收请求的数据
```
`[_tempData appendData:data];//将每次新接收到的数据拼接到原有数据包的后面;}`
```
- (void)connectionDidFinishLoading:(NSURLConnection *)connection{//3.加载完毕,开始解析NSDictionary
 *dic = [NSJSONSerialization JSONObjectWithData:_tempData options:NSJSONReadingAllowFragments error:nil];NSLog(@"%@",dic);}
```
```
- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError
 *)error{ //打印失败信息 NSLog(@"文件链接出现了error:%@",error);}
```
> 
# 第六种 post异步 -- 代理
```
NSURL *url = [NSURL URLWithString:URL_PORT1];NSMutableURLRequest
 *request = [NSMutableURLRequest requestWithURL:url];[request setHTTPMethod:@"post"];NSData *tempData = [URL_PORT2 dataUsingEncoding:NSUTF8StringEncoding];[request setHTTPBody:tempData];NSURLConnection *connertion = [NSURLConnection connectionWithRequest:request
 delegate:self];[connertion start];
```
```
- (void)connection:(NSURLConnection *)connection didReceiveResponse:(NSURLResponse
 *)response{//1.接收到服务器的响应
```
`self.tempData = [NSMutableData data]; //准备数据接收}`
```
- (void)connection:(NSURLConnection *)connection didReceiveData:(NSData
 *)data{//2.接收请求的数据
```
`[_tempData appendData:data]; //将每次新接收到的数据拼接到原有数据包的后面;}`
```
- (void)connectionDidFinishLoading:(NSURLConnection *)connection{//3.加载完毕,开始解析NSDictionary
 *dic = [NSJSONSerialization JSONObjectWithData:_tempData options:NSJSONReadingAllowFragments error:nil];NSLog(@"%@",dic);}
```
```
- (void)connection:(NSURLConnection *)connection didFailWithError:(NSError
 *)error{ //打印失败信息NSLog(@"文件链接出现了error:%@",error);}
```
> 
# post异步session
`NSURL *url = [NSURL URLWithString:URL_PORT1];//1.创建URL对象`
```
NSMutableURLRequest *request = [[NSMutableURLRequest alloc]
 initWithURL:url];2.创建请求对象
```
`[request setHTTPMethod:@"post"];//2.1`
`NSData *tempData = [URL_PORT2 dataUsingEncoding:NSUTF8StringEncoding];//参数转化`
`[request setHTTPBody:tempData];`
`NSURLSession *session = [NSURLSession sharedSession];//3.建立会话`
session支持三种类型的任务
NSURLSessionDataTask;//(加载数据)
NSURLSessionDownloadTask;//(下载)
NSURLSessionUploadTask;//(上传)
```
NSLog(@"%d",[[NSThread currentThread]isMainThread]); __weak
 typeof (self)temp = self;//判断是否是子线程 1是子线程,0不是主线程 NSURLSessionDataTask *task = [session dataTaskWithRequest:request completionHandler:^(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error) {
```
```
NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:data
 options:NSJSONReadingAllowFragments error:nil]; //解析 NSLog(@"%@",dic); NSLog(@"---%d---",[[NSThread currentThread]isMainThread]);
```
`dispatch_async(dispatch_get_main_queue(), ^{//回到主线程(刷新数据)`
`[temp.tableView reloadData]; //相当于tableView的刷新数据 }); }];`
`[task resume];//4.启动任务}`
> 
# get异步session
`NSURL *url = [NSURL URLWithString:BASE_URL];//1.创建url对象`
`NSURLSession *session = [NSURLSession sharedSession];//2.创建session对象`
```
NSURLSessionDataTask *task = [session dataTaskWithURL:url
 completionHandler:^(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error) {//3.创建task (该方法内部做了处理,默认使用get,直接传递Url即可);NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:data options:NSJSONReadingAllowFragments error:nil];
```
`dispatch_async(dispatch_get_main_queue(), ^{});`
`[task resume];}`
