# 代码Crash -- 规避 - weixin_33985507的博客 - CSDN博客
2016年12月22日 11:36:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：7
一、向容器添加对象
主要指NSMutableArray、NSMutabledictionary。向这两个容器添加nil，会引起崩溃。
分为两种模式 1.传入   2.获取
所以在传入前，需要对传入的内容进行非错判断
1.传入数据的规避措施
```
NSString *addString = nil;
NSMutableArray *mArray = [NSMutableArray array];
if (addString != nil)  {
    [mArray addObject:addString];
} else {
    NSLog(@"xx类xx方法试图向数组mArray传入一个nil");
}
```
2.从网上获取的规避措施
```
// 直接替换字典空值为@""
+ (NSDictionary *)deleteAllNullValueWithDictionary:(NSDictionary *)dict {
    NSMutableDictionary *mutableDic = [NSMutableDictionary dictionary];
    for (NSString *keyStr in dict.allKeys) {
          if ([[dict objectForKey:keyStr] isEqual:[NSNull null]]) {
                [mutableDic setObject:@"" forKey:keyStr];
          } else {
              [mutableDic setObject:[dict objectForKey:keyStr] forKey:keyStr];
          }
    }
  return mutableDic;
}
```
二、从容器取数据
主要指NSArray、NSMutableArray。从array里取数据，要注意防止下标数组越界。
一般要读取某个动态生成的容器里的内容，需要先对这个容器进行检查
NSArray *array = 动态生成;
if (array != nil && array.count > 0) {    //读取里面的内容    }else {    //输出错误信息}
```
NSArray *array = @[@"a", @"b", @"c"];
 int index = 5;
 if (index < array.count) {
       NSString *testString = array[index];
 } else {
       NSLog(@"xx类xx方法试图从数组mArray取一个超出count的对象");
 }
```
三、调用其他类的方法或不知道要调用的方法当前SDK版本是否支持。需要先判断一下这个方法是否存在。
```
if ([self.delegate respondsToSelector:@selector(要调用的方法)]) {
    [self.delegate 要调用的方法];
}
```
