# JSONModel使用 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2016年05月26日 13:12:25[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：5058
### 准备:
[JSONModel_下载](https://github.com/icanzilb/JSONModel)
![](http://upload-images.jianshu.io/upload_images/1519620-ddb13211f06f6796.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
JSONModel
> 
快速的解析数据为Model模型,支持层级嵌套Model模型解析，指定类型自动转化,无需手动解析,一行代码搞定!
配合ESJsonFormat插件效果更佳!
[ESJSONFormatter_下载](https://github.com/EnjoySR/ESJsonFormat-Xcode)
![](http://upload-images.jianshu.io/upload_images/1519620-37c959a7240a01da.gif?imageMogr2/auto-orient/strip)
配合ESJsonFormat效果更佳
# 基本使用
> 
假设你的 JSON 串像下面这样子:
```
{
"id":"10", 
"country":"Germany", 
"dialCode": 49, 
"isInEurope":true
}
```
> 
创建一个你自己的类,并继承至 JSONModel
在你的头文件里面进行声明你所需要的 JSON key值
```
#import "JSONModel.h"
@interface CountryModel : JSONModel
@property (assign, nonatomic) int id;
@property (strong, nonatomic) NSString* country;
@property (strong, nonatomic) NSString* dialCode;
@property (assign, nonatomic) BOOL isInEurope;
@end
```
> 
.m文件中你不需要做其他的事情了.
初始化你的 model ,如下所示:
```
#import "CountryModel.h"
...
NSString* json = (fetch here JSON from Internet) ... 
NSError* err = nil;
CountryModel* country = [[CountryModel alloc] initWithString:json error:&err];
```
> 
如果传过来的 JSON 合法,你所定义的所有的属性都会与该 JSON 值相匹配,并且 JSONModel 也会尝试尽可能的转换成你所想要的数据,就像上面的例子:
- 转化 "id",从字符串转换成 int 型
- 拷贝 country 属性的值
- 转换 dialCode ,从NSNumber 转换为 NSString 值
- 最后一个呢是将 isInEurope 转换成 BOOL 的属性
所以，你需要做的就是定义出你期望的属性就行了。
# 例子
> 
例子中所有Model模型都要继承JSONModel
#### 1.命名自动匹配
> - Model属性名和服务器返回数据字段一致
![](http://upload-images.jianshu.io/upload_images/1519620-79dc044e1792d3d8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
命名自动匹配
#### 2.Model关联(model含有其他model)
> - Model属性名和服务器返回数据字段一致
- 关联其他Model的属性需要指定自身类型(例如: **ProductModel***)
![](http://upload-images.jianshu.io/upload_images/1519620-80cd94e3cf6c2bc3.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Model关联
#### 3.Model集合(model含有其他model的集合)
> 
**注意:NSArry后的就<>中包含一个协议,这并不是Objective-C的新语法,他们不会冲突,使用JSONModel必须实现这个协议!**
- 层级嵌套,Model中包含其他Model集合,必须实现该协议
- 包含其他Model集合的属性需要指定层级类型和自身类型(例如: **NSArray<ProductModel>***)
![](http://upload-images.jianshu.io/upload_images/1519620-f0cbf8b6367f129c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Model集合
#### 4.键映射
> - 在一个Model中获取服务器返回数据不同层级的数据
- 例如:order_id和name不在同一层级
- 采用KVC的方式来取值,并赋值给Model属性(order_details.name ---> productName order_details.price.usd ---> price)
![](http://upload-images.jianshu.io/upload_images/1519620-aad7a58b5f0903d7.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
键映射
#### 5.设置全局键映射(应用于所有model)
![](http://upload-images.jianshu.io/upload_images/1519620-ba3ab31f472bd78c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
全局键映射
#### 6.设置下划线自动转驼峰
> - 自定义把下划线字段解析为驼峰命名属性
- 场景:服务器数据返回下划线命名字段可为Model中以驼峰命名的属性相应的赋值
- mapperFromUpperCaseToLowerCase 大写转小写
![](http://upload-images.jianshu.io/upload_images/1519620-3a4893d63be7249d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
下划线自动转驼峰
#### 7.可选属性(属性值可以为空或null)
> - 某些属性值可以为空
- 防止由于服务器数据返回空导致JSONModel异常(程序崩溃)
![](http://upload-images.jianshu.io/upload_images/1519620-fe80a87058b50c6b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
可选属性
#### 8.忽略属性(属性值可以完全忽略)
> - 解析时会完全忽略它
- 场景: 该属性值不需要从服务器数据中获取
![](http://upload-images.jianshu.io/upload_images/1519620-39d68686c44d3e4d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
忽略属性
#### 9.设置所有属性可选(所有属性值可以为空)
> - Model的所有属性值都可以为空
- 防止由于服务器数据返回空导致JSONModel异常(程序崩溃)
- 官方建议尽量避免使用该方法
![](http://upload-images.jianshu.io/upload_images/1519620-ab3bedecf31fcf6a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
所有属性可选
#### 10.使用内置的HTTP链接
```
//添加额外的头
[[JSONHTTPClient requestHeaders] setValue:@"MySecret" forKey:@"AuthorizationToken"];
//设置GET,POST请求
[JSONHTTPClient postJSONFromURLWithString:@"http://mydomain.com/api"
                                   params:@{@"postParam1":@"value1"}
                               completion:^(id json, JSONModelError *err) {
                                   //检查错误,处理JSON
}];
```
#### 11.将Model导出字典或JOSN字符串
> - 快速导出Model中所有属性和属性值
```
ProductModel* pm = [[ProductModel alloc] initWithString:jsonString error:nil];
pm.name = @"Changed Name";
//以字典形式导出
NSDictionary* dict = [pm toDictionary];
//以字符串形式导出
NSString* string = [pm toJSONString];
```
#### 12.自定义数据处理
> - 添加NSDate转换:根据后台返回时间戳格式进行相应处理
- 场景:内嵌转换,不能满足需求,需要自定义
- JSONModel支持类型: 
![](http://upload-images.jianshu.io/upload_images/1519620-c70e68647ae5e8cf.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
json支持类型
```
@implementation JSONValueTransformer (CustomTransformer)
//时间戳转NSDate
- (NSDate *)NSDateFromNSString:(NSString*)string {
    NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
    [formatter setDateFormat:APIDateFormat];
    return [formatter dateFromString:string];
}
//NSDate转时间戳
- (NSString *)JSONObjectFromNSDate:(NSDate *)date {
    NSDateFormatter *formatter = [[NSDateFormatter alloc] init];
    [formatter setDateFormat:APIDateFormat];
    return [formatter stringFromDate:date];
}
@end
```
#### 13.自定义处理特殊属性
```
@interface ProductModel : JSONModel
@property (assign, nonatomic) int id;
@property (strong, nonatomic) NSString* name;
@property (assign, nonatomic) float price;
@property (strong, nonatomic) NSLocale *locale;
@end
@implementation ProductModel
//处理本地化标识后给locale赋值
- (void)setLocaleWithNSString:(NSString*)string {
    self.locale = [NSLocale localeWithLocaleIdentifier:string];
}
- (NSString *)JSONObjectForLocale {
    return self.locale.localeIdentifier;
}
@end
```
#### 14.自定义JSON验证
```
@interface ProductModel : JSONModel
@property (assign, nonatomic) int id;
@property (strong, nonatomic) NSString* name;
@property (assign, nonatomic) float price;
@property (strong, nonatomic) NSLocale *locale;
@property (strong, nonatomic) NSNumber <Ignore> *minNameLength;
@end
@implementation ProductModel
- (BOOL)validate:(NSError *__autoreleasing *)error {
    BOOL valid = [super validate:error];
    if (self.name.length < self.minNameLength.integerValue) {
        *error = [NSError errorWithDomain:@"me.mycompany.com" code:1 userInfo:nil];
        valid = NO;
    }
    return valid;
}
@end
```
文／LuisX（简书作者）
原文链接：http://www.jianshu.com/p/3c49e5eb1602
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
