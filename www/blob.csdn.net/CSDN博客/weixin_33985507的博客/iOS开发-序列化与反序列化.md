# iOS开发-序列化与反序列化 - weixin_33985507的博客 - CSDN博客
2018年12月22日 15:38:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：5
写这篇文章的目的是由浅入深的记录一下序列化在iOS开发中的作用及其使用方法。
##### 1.什么是序列化？
> 
有时我们会有这样的需求，将程序中使用的多个对象及其属性值，以及它们的相互关系保存在文件（硬盘）中，或者发送给另外的进程。为了实现此功能，Foundation框架中，可以把相互关联的多个对象归档为二进制文件，而且还能将对象的关系从二进制文件中还原出来，这种将对象打包成二进制文件的过程就称为序列化（及开发中常说的归档）。逆变换则称为反序列化或者解档。
##### 2.NSCoding协议
> 
所有可以序列化的对象都必须要遵守NSCoding协议。此协议在Foundation/NSObject.h中定义，NSObject本身并不采用该协议。NSString、NSDictionary等Foundation的主要类都遵守了NSCoding协议。
NSCoding在Foundation中的声明:
```
@protocol NSCoding
//序列化数据，读取实例对象的数据，并把这些数据写到coder中去。NSCoder会在第3点中解释
- (void)encodeWithCoder:(NSCoder *)aCoder;
//反序列化数据，从coder中读取数据，并保存到相应的对象中。
- (nullable instancetype)initWithCoder:(NSCoder *)aDecoder;
@end
```
##### 3.抽象类NSCoder
> 
NSCoder是archivie字节流的抽象类.它提供了用于存储和读取对象数据的接口，由于其实抽象类，所以不能直接使用它来创建对象        通过其子类NSKeyedarchiver将对象写入到字节流，通过子类NSKeyedUnarchiver从字节流中读取数据。也就是可以通过这两个子类完成序列化和反序列化操作。
说了这么多概念的东西，大家应该已经烦了，下面就写一个小demo实现一个简单的需求--将邻居家的狗困到笼子里再放出来。
neighborDog类.h文件
```
@interface NeighborDog : NSObject<NSCoding> //注意遵守NSCoding协议
@property(nonatomic,strong)NSString *dogRope;//狗绳
@property(nonatomic,strong)NSString *dogBowl;//狗碗
@end
```
neighborDog类.m文件
```
@implementation NeighborDog
@synthesize dogRope;
@synthesize dogBowl;
- (void)encodeWithCoder:(NSCoder *)dogCoder
{
    [dogCoder encodeObject:dogRope forKey:@"dogRope"];
    [dogCoder encodeObject:dogBowl forKey:@"dogBowl"];
}
- (instancetype)initWithCoder:(NSCoder *)aDecoder{
    if (self = [super init]) {
        self.dogRope = [aDecoder decodeObjectForKey:@"dogRope"];
        self.dogBowl = [aDecoder decodeObjectForKey:@"dogBowl"];
    }
    return self;
}
@end
```
写一个类person，这个person负责具体的操作，在person中实例化一个neighbordog。
```
//把狗装进笼子里
-(void)putTheDogInCage{
    NSData *wData = [NSKeyedArchiver archivedDataWithRootObject:neighbordogModel];//neighbordogModel是创建的狗对象
    [[NSUserDefaults standardUserDefaults]setObject:wData forKey:@"STORAGE_GESTURE_MODEL"];
    [[NSUserDefaults standardUserDefaults]synchronize];
}
//放狗
-(void)releaseDog{
    NSData *data = [[NSUserDefaults standardUserDefaults]objectForKey:@"STORAGE_GESTURE_MODEL"];
    NSArray *arrt = [NSKeyedUnarchiver unarchiveObjectWithData:data];
}
//当然可以创建很多只狗放在一个数组里面，同时进行归档解档
```
看到这里有些iOSer会说这么简单的东西拿出来不觉得尴尬吗，好吧，以上内容仅供初学者参考。
下面结合iOS的黑魔法讨论一下序列化和反序列化的应用。
##### 4：利用运行时将对象序列化与反序列化
首先讲一个需求：
> 
为了给用户极致的体验，我们打算序列化对象的三个特定属性供用户在特定条件下使用，此时你开心的用上面简单写的归档方法写好了。后来产品同学发现这样远远不够用，于是需求改变了，要动态的序列化对象的属性。由于运行时中是可以动态获取类属性的，所以可以尝试用runtime实现。
将上面的例子中的NSCoding协议方法实现做如下改动：
```
- (void)encodeWithCoder:(NSCoder *)dogCoder
{
    unsigned int ivarCount = 0;
    unsigned int propertyVarCount = 0;
    unsigned int sharedVarCount = 0;
    
    Class selfclass = [self class];
    while (selfclass != [NSObject class]) {
        //当序列化对象不是直接继承NSObject时会统计不到父类的属性，所以需要对NSObject的上层序列对象的类属性进行编解码，这时注意用class_copyIvarList获取超类属性的时候kvc会发生崩溃，超类属性的获取需要用class_copyPropertyList.
        BOOL isSelfClass_bool = (selfclass == [self class]);
        Ivar *ivarArr = isSelfClass_bool ? class_copyIvarList([selfclass class], &ivarCount) : nil;//创建一个Ivar类型的指针数组，用于描述类声明的实例变量。
        objc_property_t *propertyArr = isSelfClass_bool ? nil : class_copyPropertyList(selfclass, &propertyVarCount);//描述属性的objc_property_t类型的指针数组，由类声明。不包括由超类声明的任何属性。
        sharedVarCount = isSelfClass_bool ? ivarCount : propertyVarCount;
        for (int i = 0; i < sharedVarCount; i++) {
            const char *varName = isSelfClass_bool ? ivar_getName(*(ivarArr + i)) : property_getName(*(propertyArr + i));
            NSString *key = [NSString stringWithUTF8String:varName];
            //kvc可以获取本类所有变量以及父类的属性，但是不包含父类的私有变量，如果直接使用会发生崩溃
            id varValue = [self valueForKey:key];
            NSArray *filters = @[@"superclass", @"description", @"debugDescription", @"hash"];
            if (varValue && (![filters containsObject:key])) {
                [dogCoder encodeObject:varValue forKey:key];
            }
        }
        free(ivarArr);
        free(propertyArr);
        selfclass = class_getSuperclass(selfclass);
    }
}
- (instancetype)initWithCoder:(NSCoder *)aDecoder{
    unsigned int ivarCount = 0;
    unsigned int propertyVarCount = 0;
    unsigned int sharedVarCount = 0;
    
    Class selfClass = [self class];
    while (selfClass != [NSObject class]) {
        BOOL isSelfClass_bool = (selfClass == [self class]);
        
        Ivar *ivarArr = isSelfClass_bool ? class_copyIvarList([selfClass class], &ivarCount) : nil;//创建一个Ivar类型的指针数组，用于描述类声明的实例变量。
        objc_property_t *propertyArr = isSelfClass_bool ? nil : class_copyPropertyList(selfClass, &propertyVarCount);//描述属性的objc_property_t类型的指针数组，由类声明。不包括由超类声明的任何属性。
        sharedVarCount = isSelfClass_bool ? ivarCount : propertyVarCount;
        
        for (int i = 0; i < sharedVarCount; i++) {
            const char *varName = isSelfClass_bool ? ivar_getName(*(ivarArr + i)) : property_getName(*(propertyArr + i));
            NSString *key = [NSString stringWithUTF8String:varName];
            id varValue = [aDecoder decodeObjectForKey:key];
            NSArray *filters = @[@"superclass", @"description", @"debugDescription", @"hash"];
            if (varValue && [filters containsObject:key] == NO) {
                [self setValue:varValue forKey:key];
            }
        }
        free(ivarArr);
        free(propertyArr);
        selfClass = class_getSuperclass(selfClass);
    }
    return self;
}
```
这些代码可以直接再用宏定义封装一层，然后在需要序列化的对象中直接调用即可。
eg:宏定义封装 CODER_DECODER_runtime()
```
@implementation ViewController
CODER_DECODER_runtime()
- (void)viewDidLoad {
    [super viewDidLoad];
}
```
作者简介:就职于甜橙金融信息技术部,负责翼支付iOS客户端开发工作,喜欢研究iOS底层及新的技术，望结交天下码友，共同进步，共同学习。
