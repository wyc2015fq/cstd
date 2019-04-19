# Runtime全方位装逼指南 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年01月05日 10:43:10[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：242标签：[runtime																[iOS 资源大全																[iOS常用方法																[iOS学习资料](https://so.csdn.net/so/search/s.do?q=iOS学习资料&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS常用方法&t=blog)](https://so.csdn.net/so/search/s.do?q=iOS 资源大全&t=blog)](https://so.csdn.net/so/search/s.do?q=runtime&t=blog)
个人分类：[iOS开发](https://blog.csdn.net/qq_30513483/article/category/6072895)
本文来源于简书,原文地址：[http://www.jianshu.com/p/efeb33712445](http://www.jianshu.com/p/efeb33712445)
楔子
[Runtime](http://opensource.apple.com//source/objc4/)是什么？见名知意，其概念无非就是“因为 Objective-C
 是一门动态语言，所以它需要一个运行时系统……这就是 Runtime 系统”云云。对博主这种菜鸟而言，Runtime 在实际开发中，其实就是一组C语言的函数。胡适说：“多研究些问题，少谈些主义”，云山雾罩的概念听多了总是容易头晕，接下来我们直接从代码入手学习 Runtime。
1、由objc_msgSend说开去：
Objective-C 中的方法调用，不是简单的方法调用，而是发送消息，也就是说，其实 [receiver message] 会被编译器转化为: objc_msgSend(receiver, selector)，何以证明？新建一个类 MyClass，其.m文件如下：
```
#import "MyClass.h"
@implementation MyClass
-(instancetype)init{
    if (self = [super init]) {
        [self showUserName];
    }
    return self;
}
-(void)showUserName{
    NSLog(@"Dave Ping");
}
```
使用 clang 重写命令:
`$ clang -rewrite-objc MyClass.m`
然后在同一目录下会多出一个 MyClass.cpp 文件，双击打开，可以看到 init 方法已经被编译器转化为下面这样：
```
static instancetype _I_MyClass_init(MyClass * self, SEL _cmd) {
    if (self = ((MyClass *(*)(__rw_objc_super *, SEL))(void *)objc_msgSendSuper)((__rw_objc_super){(id)self, (id)class_getSuperclass(objc_getClass("MyClass"))}, sel_registerName("init"))) {
        ((void (*)(id, SEL))(void *)objc_msgSend)((id)self, sel_registerName("showUserName"));
    }
    return self;
}
```
我们要找的就是它：
`((void (*)(id, SEL))(void *)objc_msgSend)((id)self, sel_registerName("showUserName"))`
objc_msgSend 函数被定义在 objc/message.h 目录下，其函数原型是酱紫滴：
`OBJC_EXPORT void objc_msgSend(void /* id self, SEL op, ... */ )`
该函数有两个参数，一个 id 类型，一个 SEL 类型。
2、SEL
SEL 被定义在 objc/objc.h 目录下：
```cpp
```cpp
typedef struct objc_selector *SEL;
```
```
其实它就是个映射到方法的C字符串，你可以用 Objective-C 编译器命令 @selector() 或者 Runtime 系统的 sel_registerName 函数来获得一个 SEL 类型的方法选择器。
3、id
与 SEL 一样，id 也被定义在 objc/objc.h 目录下：
`typedef struct objc_object *id;`
id 是一个结构体指针类型，它可以指向 Objective-C 中的任何对象。objc_object 结构体定义如下：
`struct objc_object { Class isa OBJC_ISA_AVAILABILITY;};`
我们通常所说的对象，就长这个样子，这个结构体只有一个成员变量 isa，对象可以通过 isa 指针找到其所属的类。isa 是一个 Class 类型的成员变量，那么 Class 又是什么呢？
4、Class
Class 也是一个结构体指针类型：
```cpp
```cpp
typedef struct objc_class *Class;
```
```
objc_class 结构体是酱紫滴：
```
struct objc_class {
    Class isa  OBJC_ISA_AVAILABILITY;
#if !__OBJC2__
    Class super_class                                        OBJC2_UNAVAILABLE;
    const char *name                                         OBJC2_UNAVAILABLE;
    long version                                             OBJC2_UNAVAILABLE;
    long info                                                OBJC2_UNAVAILABLE;
    long instance_size                                       OBJC2_UNAVAILABLE;
    struct objc_ivar_list *ivars                             OBJC2_UNAVAILABLE;
    struct objc_method_list **methodLists                    OBJC2_UNAVAILABLE;
    struct objc_cache *cache                                 OBJC2_UNAVAILABLE;
    struct objc_protocol_list *protocols                     OBJC2_UNAVAILABLE;
#endif
} OBJC2_UNAVAILABLE;
```
我们通常说的类就长这样子：
·Class 也有一个 isa 指针，指向其所属的元类（meta）.
·super_class：指向其超类.
·name：是类名.
·version：是类的版本信息.
·info：是类的详情.
·instance_size：是该类的实例对象的大小.
·ivars：指向该类的成员变量列表.
·methodLists：指向该类的实例方法列表，它将方法选择器和方法实现地址联系起来。methodLists 是指向 ·objc_method_list 指针的指针，也就是说可以动态修改 *methodLists 的值来添加成员方法，这也是 Category 实现的原理，同样解释了 Category 不能添加属性的原因.
·cache：Runtime 系统会把被调用的方法存到 cache 中（理论上讲一个方法如果被调用，那么它有可能今后还会被调用），下次查找的时候效率更高.
·protocols：指向该类的协议列表.
说到这里有点乱了，我们来捋一下，当我们调用一个方法时，其运行过程大致如下：
首先，Runtime 系统会把方法调用转化为消息发送，即 objc_msgSend，并且把方法的调用者，和方法选择器，当做参数传递过去.
此时，方法的调用者会通过 isa 指针来找到其所属的类，然后在 cache 或者 methodLists 中查找该方法，找得到就跳到对应的方法去执行.
如果在类中没有找到该方法，则通过 super_class 往上一级超类查找（如果一直找到 NSObject 都没有找到该方法的话，这种情况，我们放到后面消息转发的时候再说）.
前面我们说 methodLists 指向该类的实例方法列表，实例方法即-方法，那么类方法（+方法）存储在哪儿呢？类方法被存储在元类中，Class
 通过 isa 指针即可找到其所属的元类.
![](http://upload-images.jianshu.io/upload_images/1396900-1b94ff9a3905ba68?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
上图实线是 super_class 指针，虚线是 isa 指针。根元类的超类是NSObject，而 isa 指向了自己。NSObject 的超类为 nil，也就是它没有超类。
5、使用objc_msgSend
前面我们使用 clang 重写命令，看到 Runtime 是如何将方法调用转化为消息发送的。我们也可以依样画葫芦，来学习使用一下 objc_msgSend。新建一个类 TestClass，添加如下方法：
```
-(void)showAge{
    NSLog(@"24");
}
-(void)showName:(NSString *)aName{
    NSLog(@"name is %@",aName);
}
-(void)showSizeWithWidth:(float)aWidth andHeight:(float)aHeight{
    NSLog(@"size is %.2f * %.2f",aWidth, aHeight);
}
-(float)getHeight{
    return 187.5f;
}
-(NSString *)getInfo{
    return @"Hi, my name is Dave Ping, I'm twenty-four years old in the year, I like apple, nice to meet you.";
}
```
我们可以像下面这样，使用 objc_msgSend 依次调用这些方法：
```
TestClass *objct = [[TestClass alloc] init];
    ((void (*) (id, SEL)) objc_msgSend) (objct, sel_registerName("showAge"));
    ((void (*) (id, SEL, NSString *)) objc_msgSend) (objct, sel_registerName("showName:"), @"Dave Ping");
    ((void (*) (id, SEL, float, float)) objc_msgSend) (objct, sel_registerName("showSizeWithWidth:andHeight:"), 110.5f, 200.0f);
    float f = ((float (*) (id, SEL)) objc_msgSend_fpret) (objct, sel_registerName("getHeight"));
    NSLog(@"height is %.2f",f);
    NSString *info = ((NSString* (*) (id, SEL)) objc_msgSend) (objct, sel_registerName("getInfo"));
    NSLog(@"%@",info);
```
也许你已经注意到，objc_msgSend 在使用时都被强制转换了一下，这是因为 objc_msgSend 这个函数至少要有两个参数，一个id消息接受者，一个SEL消息名称。后面三个点代表参数，是变参。也就是说方法携带的参数，可以没有，可以有多个。如果我们把调用 showAge 方法改成这样：
`objc_msgSend(objct, sel_registerName("showAge"));`
Xcode 就会报错：
`Too many arguments to function call, expected 0, have 2.`
完整的 objc_msgSend 使用代码在[这里](https://github.com/daiweiping/RuntimeLearn)。
6、objc_msgSendSuper
编译器会根据情况在 objc_msgSend，objc_msgSend_stret，objc_msgSendSuper，objc_msgSendSuper_stret 或 objc_msgSend_fpret 五个方法中选择一个来调用。如果消息是传递给超类，那么会调用 objc_msgSendSuper 方法，如果消息返回值是数据结构，就会调用 objc_msgSendSuper_stret 方法，如果返回值是浮点数，则调用 objc_msgSend_fpret 方法。
这里我们重点说一下 objc_msgSendSuper，objc_msgSendSuper 函数原型如下：
`OBJC_EXPORT void objc_msgSendSuper(void /* struct objc_super *super, SEL op, ... */ )`
当我们调用 [super selector] 时，Runtime 会调用 objc_msgSendSuper 方法，objc_msgSendSuper 方法有两个参数，super 和 op，Runtime 会把 selector 方法选择器赋值给 op。而 super 是一个 objc_super 结构体指针，objc_super 结构体定义如下：
```
struct objc_super {
    /// Specifies an instance of a class.
    __unsafe_unretained id receiver;
    /// Specifies the particular superclass of the instance to message. 
#if !defined(__cplusplus)  &&  !__OBJC2__
    /* For compatibility with old objc-runtime.h header */
    __unsafe_unretained Class class;
#else
    __unsafe_unretained Class super_class;
#endif
    /* super_class is the first class to search */
};
```
Runtime 会创建一个 objc_spuer 结构体变量，将其地址作为参数（super）传递给 objc_msgSendSuper，并且将 self 赋值给 receiver：super—>receiver=self.
举个栗子，问下面的代码输出什么：
```
@implementation Son : Father
- (id)init
{
    self = [super init];
    if (self)
    {
        NSLog(@"%@", NSStringFromClass([self class]));
        NSLog(@"%@", NSStringFromClass([super class]));
    }
    return self;
}
@end
```
答案是全部输出 Son.
使用 clang 重写命令，发现上述代码被转化为:
```
NSLog((NSString *)&__NSConstantStringImpl__var_folders_gm_0jk35cwn1d3326x0061qym280000gn_T_main_a5cecc_mi_0, NSStringFromClass(((Class (*)(id, SEL))(void *)objc_msgSend)((id)self, sel_registerName("class"))));
NSLog((NSString *)&__NSConstantStringImpl__var_folders_gm_0jk35cwn1d3326x0061qym280000gn_T_main_a5cecc_mi_1, NSStringFromClass(((Class (*)(__rw_objc_super *, SEL))(void *)objc_msgSendSuper)((__rw_objc_super){ (id)self, (id)class_getSuperclass(objc_getClass("Son")) }, sel_registerName("class"))));
```
当调用 [super class] 时，会转换成 objc_msgSendSuper 函数：
第一步先构造 objc_super 结构体，结构体第一个成员就是 self。第二个成员是 (id)class_getSuperclass(objc_getClass(“Son”)).
第二步是去 Father 这个类里去找 - (Class)class，没有，然后去 NSObject 类去找，找到了。最后内部是使用 objc_msgSend(objc_super->receiver, @selector(class)) 去调用，此时已经和 [self class] 调用相同了，所以两个输出结果都是 Son。
7、对象关联
对象关联允许开发者对已经存在的类在 Category 中添加自定义的属性：
`OBJC_EXPORT void objc_setAssociatedObject(id object, const void *key, id value, objc_AssociationPolicy policy) __OSX_AVAILABLE_STARTING(__MAC_10_6, __IPHONE_3_1);`
·object 是源对象.
·value 是被关联的对象.
·key 是关联的键，objc_getAssociatedObject 方法通过不同的 key 即可取出对应的被关联对象.
·policy 是一个枚举值，表示关联对象的行为，从命名就能看出各个枚举值的含义：
```
typedef OBJC_ENUM(uintptr_t, objc_AssociationPolicy) {
    OBJC_ASSOCIATION_ASSIGN = 0,           /**< Specifies a weak reference to the associated object. */
    OBJC_ASSOCIATION_RETAIN_NONATOMIC = 1, /**< Specifies a strong reference to the associated object. 
                                            *   The association is not made atomically. */
    OBJC_ASSOCIATION_COPY_NONATOMIC = 3,   /**< Specifies that the associated object is copied. 
                                            *   The association is not made atomically. */
    OBJC_ASSOCIATION_RETAIN = 01401,       /**< Specifies a strong reference to the associated object.
                                            *   The association is made atomically. */
    OBJC_ASSOCIATION_COPY = 01403          /**< Specifies that the associated object is copied.
                                            *   The association is made atomically. */
};
```
要取出被关联的对象使用 objc_getAssociatedObject 方法即可，要删除一个被关联的对象，使用 objc_setAssociatedObject 方法将对应的 key 设置成 nil 即可：
`objc_setAssociatedObject(self, associatedKey, nil, OBJC_ASSOCIATION_COPY_NONATOMIC);`
objc_removeAssociatedObjects 方法将会移除源对象中所有的关联对象.
举个栗子，假如我们要给 UIButton 添加一个监听单击事件的 block 属性，新建 UIButton 的 Category，其.m文件如下：
```
#import "UIButton+ClickBlock.h"
#import <objc/runtime.h>
static const void *associatedKey = "associatedKey";
@implementation UIButton (ClickBlock)
//Category中的属性，只会生成setter和getter方法，不会生成成员变量
-(void)setClick:(clickBlock)click{
    objc_setAssociatedObject(self, associatedKey, click, OBJC_ASSOCIATION_COPY_NONATOMIC);
    [self removeTarget:self action:@selector(buttonClick) forControlEvents:UIControlEventTouchUpInside];
    if (click) {
        [self addTarget:self action:@selector(buttonClick) forControlEvents:UIControlEventTouchUpInside];
    }
}
-(clickBlock)click{
    return objc_getAssociatedObject(self, associatedKey);
}
-(void)buttonClick{
    if (self.click) {
        self.click();
    }
}
@end
```
然后在代码中，就可以使用 UIButton 的属性来监听单击事件了：
```
UIButton *button = [UIButton buttonWithType:UIButtonTypeCustom];
    button.frame = self.view.bounds;
    [self.view addSubview:button];
    button.click = ^{
        NSLog(@"buttonClicked");
    };
```
完整的对象关联代码点[这里](https://github.com/daiweiping/RuntimeLearn)
8、自动归档
博主在学习 Runtime 之前，归档的时候是酱紫写的：
```
- (void)encodeWithCoder:(NSCoder *)aCoder{
    [aCoder encodeObject:self.name forKey:@"name"];
    [aCoder encodeObject:self.ID forKey:@"ID"];
}
- (id)initWithCoder:(NSCoder *)aDecoder{
    if (self = [super init]) {
        self.ID = [aDecoder decodeObjectForKey:@"ID"];
        self.name = [aDecoder decodeObjectForKey:@"name"];
    }
    return self;
}
```
那么问题来了，如果当前 Model 有100个属性的话，就需要写100行这种代码：
`[aCoder encodeObject:self.name forKey:@"name"];`
想想都头疼，通过 Runtime 我们就可以轻松解决这个问题：
1.使用 class_copyIvarList 方法获取当前 Model 的所有成员变量.
2.使用 ivar_getName 方法获取成员变量的名称.
3.通过 KVC 来读取 Model 的属性值（encodeWithCoder:），以及给 Model 的属性赋值（initWithCoder:）.
举个栗子，新建一个 Model 类，其.m文件如下：
```
#import "TestModel.h"
#import <objc/runtime.h>
#import <objc/message.h>
@implementation TestModel
- (void)encodeWithCoder:(NSCoder *)aCoder{
    unsigned int outCount = 0;
    Ivar *vars = class_copyIvarList([self class], &outCount);
    for (int i = 0; i < outCount; i ++) {
        Ivar var = vars[i];
        const char *name = ivar_getName(var);
        NSString *key = [NSString stringWithUTF8String:name];
        // 注意kvc的特性是，如果能找到key这个属性的setter方法，则调用setter方法
        // 如果找不到setter方法，则查找成员变量key或者成员变量_key，并且为其赋值
        // 所以这里不需要再另外处理成员变量名称的“_”前缀
        id value = [self valueForKey:key];
        [aCoder encodeObject:value forKey:key];
    }
}
- (nullable instancetype)initWithCoder:(NSCoder *)aDecoder{
    if (self = [super init]) {
        unsigned int outCount = 0;
        Ivar *vars = class_copyIvarList([self class], &outCount);
        for (int i = 0; i < outCount; i ++) {
            Ivar var = vars[i];
            const char *name = ivar_getName(var);
            NSString *key = [NSString stringWithUTF8String:name];
            id value = [aDecoder decodeObjectForKey:key];
            [self setValue:value forKey:key];
        }
    }
    return self;
}
@end
```
完整的自动归档代码在[这里](https://github.com/daiweiping/RuntimeLearn)
9、字典与模型互转
最开始博主是这样用字典给 Model 赋值的：
```
-(instancetype)initWithDictionary:(NSDictionary *)dict{
    if (self = [super init]) {
        self.age = dict[@"age"];
        self.name = dict[@"name"];
    }
    return self;
}
```
可想而知，遇到的问题跟归档时候一样（后来使用[MJExtension](https://github.com/CoderMJLee/MJExtension)），这里我们稍微来学习一下其中原理，字典转模型的时候：
1.根据字典的 key 生成 setter 方法.
2.使用 objc_msgSend 调用 setter 方法为 Model 的属性赋值（或者 KVC）.
模型转字典的时候：
1.调用 class_copyPropertyList 方法获取当前 Model 的所有属性.
2.调用 property_getName 获取属性名称.
3.根据属性名称生成 getter 方法.
4.使用 objc_msgSend 调用 getter 方法获取属性值（或者 KVC）.
代码如下：
```
#import "NSObject+KeyValues.h"
#import <objc/runtime.h>
#import <objc/message.h>
@implementation NSObject (KeyValues)
//字典转模型
+(id)objectWithKeyValues:(NSDictionary *)aDictionary{
    id objc = [[self alloc] init];
    for (NSString *key in aDictionary.allKeys) {
        id value = aDictionary[key];
        /*判断当前属性是不是Model*/
        objc_property_t property = class_getProperty(self, key.UTF8String);
        unsigned int outCount = 0;
        objc_property_attribute_t *attributeList = property_copyAttributeList(property, &outCount);
        objc_property_attribute_t attribute = attributeList[0];
        NSString *typeString = [NSString stringWithUTF8String:attribute.value];
        if ([typeString isEqualToString:@"@\"TestModel\""]) {
            value = [self objectWithKeyValues:value];
        }
        /**********************/
        //生成setter方法，并用objc_msgSend调用
        NSString *methodName = [NSString stringWithFormat:@"set%@%@:",[key substringToIndex:1].uppercaseString,[key substringFromIndex:1]];
        SEL setter = sel_registerName(methodName.UTF8String);
        if ([objc respondsToSelector:setter]) {
            ((void (*) (id,SEL,id)) objc_msgSend) (objc,setter,value);
        }
    }
    return objc;
}
//模型转字典
-(NSDictionary *)keyValuesWithObject{
    unsigned int outCount = 0;
    objc_property_t *propertyList = class_copyPropertyList([self class], &outCount);
    NSMutableDictionary *dict = [NSMutableDictionary dictionary];
    for (int i = 0; i < outCount; i ++) {
        objc_property_t property = propertyList[i];
        //生成getter方法，并用objc_msgSend调用
        const char *propertyName = property_getName(property);
        SEL getter = sel_registerName(propertyName);
        if ([self respondsToSelector:getter]) {
            id value = ((id (*) (id,SEL)) objc_msgSend) (self,getter);
            /*判断当前属性是不是Model*/
            if ([value isKindOfClass:[self class]] && value) {
                value = [value keyValuesWithObject];
            }
            /**********************/
            if (value) {
                NSString *key = [NSString stringWithUTF8String:propertyName];
                [dict setObject:value forKey:key];
            }
        }
    }
    return dict;
}
@end
```
完整代码在[这里](https://github.com/daiweiping/RuntimeLearn)
10、动态方法解析
前面我们留下了一点东西没说，那就是如果某个对象调用了不存在的方法时会怎么样，一般情况下程序会crash，错误信息类似下面这样：
`unrecognized selector sent to instance 0x7fd0a141afd0`
但是在程序crash之前，Runtime 会给我们动态方法解析的机会，消息发送的步骤大致如下：
1.检测这个 selector 是不是要忽略的。比如 Mac OS X 开发，有了垃圾回收就不理会 retain，release 这些函数了.
2.检测这个 target 是不是 nil 对象。ObjC 的特性是允许对一个 nil 对象执行任何一个方法不会 Crash，因为会被忽略掉.
3.如果上面两个都过了，那就开始查找这个类的 IMP，先从 cache 里面找，完了找得到就跳到对应的函数去执行.
如果 cache 找不到就找一下方法分发表.
4.如果分发表找不到就到超类的分发表去找，一直找，直到找到NSObject类为止.
如果还找不到就要开始进入消息转发了，消息转发的大致过程如图：
![](http://upload-images.jianshu.io/upload_images/1396900-19c5eb4913f49e33?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
这里写图片描述
1.进入 resolveInstanceMethod: 方法，指定是否动态添加方法。若返回NO，则进入下一步，若返回YES，则通过 class_addMethod 函数动态地添加方法，消息得到处理，此流程完毕.
2.resolveInstanceMethod: 方法返回 NO 时，就会进入 forwardingTargetForSelector: 方法，这是 Runtime 给我们的第二次机会，用于指定哪个对象响应这个 selector。返回nil，进入下一步，返回某个对象，则会调用该对象的方法.
3.若 forwardingTargetForSelector: 返回的是nil，则我们首先要通过 methodSignatureForSelector: 来指定方法签名，返回nil，表示不处理，若返回方法签名，则会进入下一步.
4当第 methodSignatureForSelector: 方法返回方法签名后，就会调用 forwardInvocation: 方法，我们可以通过 anInvocation 对象做很多处理，比如修改实现方法，修改响应对象等.
如果到最后，消息还是没有得到响应，程序就会crash，详细代码在[这里](https://github.com/daiweiping/RuntimeLearn)。
[](http://yulingtianxia.com/blog/2014/11/05/objective-c-runtime/)[Objective-C
 Runtime](http://yulingtianxia.com/blog/2014/11/05/objective-c-runtime/)
