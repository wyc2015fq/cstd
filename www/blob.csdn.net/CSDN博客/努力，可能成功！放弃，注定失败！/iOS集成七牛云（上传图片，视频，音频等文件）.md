# iOS集成七牛云（上传图片，视频，音频等文件） - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年12月07日 10:44:08[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：439
## 用的CocoaPods导入SDK
```
platform :ios,'9.0'
    target '项目名' do
    pod 'AFNetworking'
    pod 'Qiniu'
    end
```
## 导入头文件
```
#import<QiniuSDK.h>
#import<AFNetworking.h>
#include<ComminCrypto/CommonCrypto.h>
#import<QN_GTM_Base64.h>
#import<QBEtag.h>
#import<QNConfiguration.h>
```
```
#define kQNinterface @"官网获取外链域名"
```
## 官网获取两个关键
```
static NSString *accessKey = @"官网获取";
  static NSString *secretKey = @"官网获取";
```
## 获取令牌项目中一般是由服务器来生成，这里用于测试
```
+ (NSString *)makeToken:(NSString *)accessKey secretKey:(NSString *)secretKey{
    const char *secretKeyStr = [secretKey UTF8String];
    NSString *policy = [QiniuLoad marshal];
    NSData *policyData = [policy dataUsingEncoding:NSUTF8StringEncoding];
    NSString *encodedPolicy = [QN_GTM_Base64 stringByWebSafeEncodingData:policyData padded:TRUE];
    const char *encodedPolicyStr = [encodedPolicy cStringUsingEncoding:NSUTF8StringEncoding];
    char digestStr[CC_SHA1_DIGEST_LENGTH];
    bzero(digestStr, 0);
    CCHmac(kCCHmacAlgSHA1, secretKeyStr, strlen(secretKeyStr), encodedPolicyStr, strlen(encodedPolicyStr), digestStr);
    NSString *encodedDigest = [QN_GTM_Base64 stringByWebSafeEncodingBytes:digestStr length:CC_SHA1_DIGEST_LENGTH padded:TRUE];
    NSString *token = [NSString stringWithFormat:@"%@:%@:%@",  accessKey, encodedDigest, encodedPolicy];
    return token;//得到了token
    }
```
```
+ (NSString *)marshal{
    NSInteger _expire = 0;
    time_t deadline;
    time(&deadline);//返回当前系统时间
    //@property (nonatomic , assign) int expires; 怎么定义随你...
    deadline += (_expire > 0) ? _expire : 3600; // +3600秒,即默认token保存1小时.
    NSNumber *deadlineNumber = [NSNumber numberWithLongLong:deadline];
    NSMutableDictionary *dic = [NSMutableDictionary dictionary];
    [dic setObject:@"修改成七牛存储空间的名字" forKey:@"scope"];//根据
    [dic setObject:deadlineNumber forKey:@"deadline"];
    NSString *json = [QiniuLoad convertToJsonData:dic ];
    return json;
    }
```
```
+(NSString *)convertToJsonData:(NSDictionary *)dict{
    NSError *error;
    NSData *jsonData = [NSJSONSerialization dataWithJSONObject:dict options:NSJSONWritingPrettyPrinted error:&error];
    NSString *jsonString;
    if (!jsonData) {
    NSLog(@"%@",error);
    }else{
    jsonString = [[NSString alloc]initWithData:jsonData encoding:NSUTF8StringEncoding];
    }
    NSMutableString *mutStr = [NSMutableString stringWithString:jsonString];
    NSRange range = {0,jsonString.length};
    [mutStr replaceOccurrencesOfString:@" " withString:@"" options:NSLiteralSearch range:range];
    NSRange range2 = {0,mutStr.length};
    [mutStr replaceOccurrencesOfString:@"\n" withString:@"" options:NSLiteralSearch range:range2];
    return mutStr;
    }
```
## 上传图片
```
+(void)uploadImageToQNFilePath:(NSArray *)photos success:(QNSuccessBlock)success failure:(QNFailureBlock)failure{
    NSMutableArray *imageAry =[NSMutableArray new];
    NSMutableArray *imageAdd = [NSMutableArray new];
    for (ZLPhotoAssets *status in photos) {
    [imageAry addObject:[status aspectRatioImage]];
    }
    //主要是把图片或者文件转成nsdata类型就可以了
    QNConfiguration *config = [QNConfiguration build:^(QNConfigurationBuilder *builder) {
    builder.zone = [QNZone zone0];}];
    QNUploadManager *upManager = [[QNUploadManager alloc] initWithConfiguration:config];
    QNUploadOption *uploadOption = [[QNUploadOption alloc] initWithMime:nil
    progressHandler:^(NSString *key, float percent) {
    NSLog(@"上传进度 %.2f", percent);
     }
    params:nil
    checkCrc:NO
    cancellationSignal:nil];
    [imageAry enumerateObjectsUsingBlock:^(id  _Nonnull obj, NSUInteger idx, BOOL * _Nonnull stop) {
    NSLog(@"%ld",idx);
    NSData *data;
    if (UIImagePNGRepresentation(obj) == nil){
    data = UIImageJPEGRepresentation(obj, 1);
    } else {
    data = UIImagePNGRepresentation(obj);}
    [upManager putData:data key:[QiniuLoad qnImageFilePatName] token:[QiniuLoad makeToken:accessKey secretKey:secretKey] complete:^(QNResponseInfo *info, NSString *key, NSDictionary *resp) {
    NSLog(@"%@",resp[@"key"]);
    if (info.isOK) {
    [imageAdd addObject:[NSString stringWithFormat:@"%@%@",kQNinterface,resp[@"key"]]];}else{
    [imageAdd addObject:[NSString stringWithFormat:@"%ld",idx]];}
    if (imageAdd.count == imageAry.count) {
    if (success) {
    success([imageAdd componentsJoinedByString:@";"]);}}}
    option:uploadOption];}];}
```
## 上传视频（格式：MP4）
```
+(void)uploadVideoToQNFilePath:(NSURL *)url success:(QNSuccessBlock)success failure:(QNFailureBlock)failure{
    NSMutableArray *imageAdd = [NSMutableArray new];
    NSMutableArray *errors = [NSMutableArray new];
    QNConfiguration *config = [QNConfiguration build:^(QNConfigurationBuilder *builder) {
    builder.zone = [QNZone zone0];}];
    QNUploadManager *upManager = [[QNUploadManager alloc] initWithConfiguration:config];
    QNUploadOption *uploadOption = [[QNUploadOption alloc] initWithMime:nil
    progressHandler:^(NSString *key, float percent) {
    NSLog(@"上传进度 %.2f", percent);}
    params:nil
    checkCrc:NO
    cancellationSignal:nil];
    NSData *myVideoData = [NSData dataWithContentsOfURL:url];
    [upManager putData:myVideoData key:[QiniuLoad qnVideoFilePatName] token:[QiniuLoad makeToken:accessKey secretKey:secretKey] complete:^(QNResponseInfo *info, NSString *key, NSDictionary *resp) {
    if (info.isOK) {
    [imageAdd addObject:[NSString stringWithFormat:@"%@%@",kQNinterface,resp[@"key"]]];
    //                NSLog(@"%@",imageAdd);}else{
    [errors addObject:[NSString stringWithFormat:@"%@", @1]];}
    if (imageAdd.count == 1) {
    if (success) {
    success([imageAdd componentsJoinedByString:@";"]);}}else{
    if (failure) {
    failure([errors componentsJoinedByString:@","]);}}}
    option:uploadOption];}
```
## 上传音频（格式：AMR）
```
+(void)uploadAmrToQNFilePath:(NSString *)url success:(QNSuccessBlock)success failure:(QNFailureBlock)failure{
    NSMutableArray *imageAdd = [NSMutableArray new];
    NSMutableArray *errors = [NSMutableArray new];
    QNConfiguration *config = [QNConfiguration build:^(QNConfigurationBuilder *builder) {
    builder.zone = [QNZone zone0];}];
    QNUploadManager *upManager = [[QNUploadManager alloc] initWithConfiguration:config];
    QNUploadOption *uploadOption = [[QNUploadOption alloc] initWithMime:nil
    progressHandler:^(NSString *key, float percent) {
    NSLog(@"上传进度 %.2f", percent);}
    params:nil
    checkCrc:NO
    cancellationSignal:nil];
    NSData *data = [NSData dataWithContentsOfFile:url];
    [upManager putData:data key:[QiniuLoad qnAmrFilePatName] token:reslut[@"data"] complete:^(QNResponseInfo *info, NSString *key, NSDictionary *resp) {
    if (info.isOK) {
    [imageAdd addObject:[NSString stringWithFormat:@"%@%@",kQNinterface,resp[@"key"]]];
    //                NSLog(@"%@",imageAdd);}else{
    [errors addObject:[NSString stringWithFormat:@"%@", @1]];}
    if (imageAdd.count == 1) {
    if (success) {
    success([imageAdd componentsJoinedByString:@";"]);}}else{
    if (failure) {
    failure([errors componentsJoinedByString:@","]);}}
    } option:uploadOption];}
```
## 文件命名
```
+ (NSString *)qnImageFilePatName{
    NSDateFormatter *formatter = [[NSDateFormatter alloc]init];
    [formatter setDateFormat:@"yyyyMMdd"];
    NSString *nowe = [formatter stringFromDate:[NSDate date]];
     char datax[12];//十六位防重字符
    for (int x=0;x<12;datax[x++] = (char)('A' + (arc4random_uniform(26))));
    NSString *number = [[NSString alloc] initWithBytes:datax length:12 encoding:NSUTF8StringEncoding];
    //当前时间
    NSInteger interval = (NSInteger)[[NSDate date]timeIntervalSince1970];
    NSString *name = [NSString stringWithFormat:@"Picture/%@/%ld%@.jpg",now,interval,number];
    NSLog(@"name__%@",name);
    return name;}
```
简单封装了下直接拖到demo里就能用
百度云链接：[https](https://link.jianshu.com?t=https://pan.baidu.com/s/1mi80azI)：//pan.baidu.com/s/1mi80azI密码：rbcm
作者：搬运工一只自我修养的
链接：HTTPS：//www.jianshu.com/p/3b4b3dee26b5 
来源：书繁简
繁简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
