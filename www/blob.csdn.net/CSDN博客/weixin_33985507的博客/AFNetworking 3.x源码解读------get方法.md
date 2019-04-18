# AFNetworking 3.x源码解读------get方法 - weixin_33985507的博客 - CSDN博客
2017年02月25日 15:34:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
一直想阅读AFNetworking源码，中间肯定会遇到很多疑惑，也想试着解开这些疑惑，拨开迷雾。如有不正确，不准确，欢迎指正。
在下面的篇幅中主要做两件事:
1> 用系统原生NSURLSession发送get方法
2> 用AFN发送get方法
一.用系统原生NSURLSession发送get方法
> 
NSString *userInfoStr = @"https://xxxxxx/user/info?token=1c0944fb2cecc7e79a1e44bf153659c2";
NSURLSession *session = [NSURLSession sharedSession];
NSURLSessionDataTask *dataTask = [session dataTaskWithURL:[NSURL URLWithString:userInfoStr] completionHandler:^(NSData * _Nullable data, NSURLResponse * _Nullable response, NSError * _Nullable error) {
NSDictionary *dict = [NSJSONSerialization JSONObjectWithData:data options:NSJSONReadingMutableLeaves error:nil];
NSLog(@"%@",dict);
}];
[dataTask resume];
从上面的例子可以看出，用系统NSURLSession发送get请求很简单
（1）先获取NSURLSession单例对象
（2）传递url和参数给session
（3）注意调用任务的resume方法。任务启动，服务端就会返回数据给客户端。我们直接解析就可以了。
二.用AFN发送get请求
比着系统的更简单些。我们只需要传递url和参数给AFHTTPSessionManager对象。它就会把解析好的数据返回给客户端。
> 
NSString *userInfoStr = @"https://xxxxx/user/info?token=8955c13fe371df3354aaf3aa924bc9e5";
AFHTTPSessionManager *manager = [AFHTTPSessionManager manager];
[manager GET:userInfoStr parameters:nil progress:^(NSProgress * _Nonnull downloadProgress) {
} success:^(NSURLSessionDataTask * _Nonnull task, id  _Nullable responseObject) {
NSLog(@"responseObject----%@",responseObject);
} failure:^(NSURLSessionDataTask * _Nullable task, NSError * _Nonnull error) {
NSLog(@"请求失败");
}];
