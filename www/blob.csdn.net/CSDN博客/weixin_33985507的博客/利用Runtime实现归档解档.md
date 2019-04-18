# 利用Runtime实现归档解档 - weixin_33985507的博客 - CSDN博客
2018年04月30日 23:45:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：9
简易Demo, 示例只包含一些基本属性用于展示实现, 并不包含嵌套关系:
```
#import "Person.h"
#import "objc/runtime.h"
@implementation Person
- (instancetype)initWithCoder:(NSCoder *)aDecoder {
    self = [super init];
    if (self) {
        
        // 获取所有的实例变量
        unsigned int count = 0;
        Ivar *ivars = class_copyIvarList([Person class], &count);
        
        // 遍历
        for (int i = 0; i < count; i ++) {
            Ivar ivar = ivars[i];
            const char *name = ivar_getName(ivar);
            NSString *key = [NSString stringWithUTF8String:name];
            // kvc
            id value = [aDecoder decodeObjectForKey:key];
            // 解码
            [self setValue:value forKey:key];
        }
        
        free(ivars);
        
    }
    return self;
}
- (void)encodeWithCoder:(NSCoder *)aCoder {
    // 获取所有的实例变量
    unsigned int count = 0;
    Ivar *ivars = class_copyIvarList([Person class], &count);
    
    // 遍历
    for (int i = 0; i < count; i ++) {
        Ivar ivar = ivars[i];
        const char *name = ivar_getName(ivar);
        NSString *key = [NSString stringWithUTF8String:name];
        // kvc
        /***
            valueForKey (总体规则 先找相关方法 再找相关变量)
         1. 先找相关方法, 如果相关方法找不到(getName name)
         2. 根据accessInstanceVariablesDirectly类方法来判断
         3. 如果判断是NO,直接执行KVC的valueForUndefineKey(系统跑出一个异常,未定义key)
         4. 如果是YES,继续找相关变量(_name _isName,naame,isName)
         ***/
        id value = [self valueForKey:key];
        // 编码
        [aCoder encodeObject:value forKey:key];
    }
    
    free(ivars);
}
@end
```
