# oc-32-@property示例 - weixin_33985507的博客 - CSDN博客
2016年03月24日 10:54:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
Goods.h
```
#import <Foundation/Foundation.h>
typedef struct{
    int year;
    int month;
    int day;
} MyDate;
typedef enum{
    GenderFemal,
    Gendermal
} Gender;
@interface Goods : NSObject
//已经做了set的内存管理
@property(nonatomic,assign)float price; // 单价
@property(nonatomic,assign)Gender gender; // 性别
@property(nonatomic,assign)float weight; // 重量
@property(nonatomic,copy)NSString *showImage; // 图片
@property(nonatomic,assign)MyDate produceDate; // 生产日期
@property(nonatomic,assign)MyDate expireDaer;  // 过期日期
// 卖家所卖的商品
@property(nonatomic,retain)Goods *goods;
@end
```
Good.m
```
#import "Goods.h"
@implementation Goods
- (void)dealloc
{
    NSLog(@"商品被释放");
    [super dealloc];
}
@end
```
main.m
```
#import <Foundation/Foundation.h>
#import "Goods.h"
#import "Buyer.h"
#import "Seller.h"
int main(int argc, const char * argv[]) {
    @autoreleasepool {
        // 创建商品
        Goods *g = [[Goods alloc] init];
        // 给商品属性赋值
        g.price = 100;
        g.weight = 100;
        g.showImage = @"/Users/apple/Desktop/1.png";
        g.produceDate = (MyDate){2015,10,15};
        g.expireDaer = (MyDate){2015,10,16};
        
        // 买家
        Buyer *buyer = [[Buyer alloc] init];
        buyer.name = @"小明";
        buyer.gender = GenderFemal;
        buyer.age = 20;
        buyer.height = 141;
        
        // 卖家
        Seller *seller = [[Seller alloc] init];
        seller.name = @"张三丰";
        seller.age = 18;
        seller.height = 137;
        seller.goods = g;
        
        // 释放对象
        [g release];
        [buyer release];
        [seller release];
        
    }
    return 0;
}
```
