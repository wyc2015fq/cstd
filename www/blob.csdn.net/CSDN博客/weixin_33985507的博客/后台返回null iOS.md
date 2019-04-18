# 后台返回null iOS - weixin_33985507的博客 - CSDN博客
2018年04月26日 10:52:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
1。第一种解决方案  
就是在每一个 可能传回null 的地方 使用  if([object isEqual：[NSNUll null]]) 去判断
2。第二种解决方案
网上传说老外写了一个Category，叫做NullSafe..只支持到ios9,3  ,实测 并没有解决我的问题..
3。第三种解决方案
AFHTTPSessionManager *manager = [AFHTTPSessionManager manager];
 AFJSONResponseSerializer *serializer = [AFJSONResponseSerializer serializer];
serializer.removesKeysWithNullValues = YES;
[serializer setAcceptableContentTypes:[NSSet setWithObjects:@"application/json", @"text/html",@"text/json",@"text/javascript",@"text/plain", nil]];
manager.responseSerializer=serializer;
manager.requestSerializer = [AFJSONRequestSerializer serializer];
不知道为什么无效，有大神给指导一下吗
4。第四种解决方案
json转model时，每个model里加上
- (id)valueForUndefinedKey:(NSString *)key
{
 return nil;
}
- (void)setValue:(id)value forUndefinedKey:(NSString *)key
{
}
5。第5种解决方案
在数组和字典的category里面写的两个方法. 完美解决..在每次请求回来数据,统一处理.. 
废话不多说,上代码 
字典的类目：
#import "NSDictionary+Extension.h"
#import "NSArray+Extension.h"
@implementation NSDictionary (Extension)
- (NSDictionary *)dictionaryByReplacingNulls {
 const NSMutableDictionary *replaced = [self mutableCopy];
 const id nul = [NSNull null];
 for (NSString *key in self) {
 id object = [self objectForKey:key];
 if (object == nul) [replaced removeObjectForKey:key];
 else if ([object isKindOfClass:[NSDictionary class]]) [replaced setObject:[object dictionaryByReplacingNulls] forKey:key];
 else if ([object isKindOfClass:[NSArray class]]) [replaced setObject:[object arrayByReplacingNulls] forKey:key];
    }
 return [NSDictionary dictionaryWithDictionary:[replaced copy]];
}
@end
数组的类目：
#import "NSArray+Extension.h"
#import "NSDictionary+Extension.h"
@implementation NSArray (Extension)
- (NSArray *)arrayByReplacingNulls  {
 NSMutableArray *replaced = [self mutableCopy];
 const id nul = [NSNull null];
 for (int idx = 0; idx < [replaced count]; idx++) {
 id object = [replaced objectAtIndex:idx];
 if (object == nul) [replaced removeObjectAtIndex:idx];
 else if ([object isKindOfClass:[NSDictionary class]]) [replaced replaceObjectAtIndex:idx withObject:[object dictionaryByReplacingNulls]];
 else if ([object isKindOfClass:[NSArray class]]) [replaced replaceObjectAtIndex:idx withObject:[object arrayByReplacingNulls]];
    }
 return [replaced copy];
}
@end
在封装的网络请求获取到后台数据的地方 将数据处理一下
NSDictionary *dict = [responseObject dictionaryByReplacingNulls];
success(dict);
6。第6种解决方案 
大力推荐：
AvoidCrash再也不怕程序崩溃啦 AvoidCrash的功能不仅仅是数组,还有报错字典等许多功能,具体用法可以参考[https://github.com/chenfanfang/AvoidCrash](https://link.jianshu.com?t=https%3A%2F%2Fgithub.com%2Fchenfanfang%2FAvoidCrash)
转自https://www.cnblogs.com/huangzs/p/7450885.html
