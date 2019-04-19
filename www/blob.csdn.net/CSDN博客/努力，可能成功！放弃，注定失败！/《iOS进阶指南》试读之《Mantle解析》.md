# 《iOS进阶指南》试读之《Mantle解析》 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年04月13日 08:51:16[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：1311
# 本文转自简书，原文地址:[http://www.jianshu.com/p/f49ddbf8a2ea](http://www.jianshu.com/p/f49ddbf8a2ea)
# Mantle
阅读一个库的源码，首先要知道，我们为什么需要这一类的库。
## Mantle的目的
Mantle 的诞生是为了更方便的将服务端返回的数据映射为我们的 Model。
简单来说，我们在写 app 的时候，经常需要把服务端返回的数据和我们自己创建 model 关联起来，这样，在和 View 层交互的时候就可以使用 model 而不是直接使用字典。
那么，我们如果不使用 Mantle 的情况下。是如何创建一个 Model 并且把服务端返回的数据填充到这个Model里呢？我们来看看 Mantle 给的例子，一般是这样的。
```
typedef enum : NSUInteger {
    GHIssueStateOpen,
    GHIssueStateClosed
} GHIssueState;
@interface GHIssue : NSObject <NSCoding, NSCopying>
@property (nonatomic, copy, readonly) NSURL *URL;
@property (nonatomic, copy, readonly) NSURL *HTMLURL;
@property (nonatomic, copy, readonly) NSNumber *number;
@property (nonatomic, assign, readonly) GHIssueState state;
@property (nonatomic, copy, readonly) NSString *reporterLogin;
@property (nonatomic, copy, readonly) NSDate *updatedAt;
@property (nonatomic, strong, readonly) GHUser *assignee;
@property (nonatomic, copy, readonly) NSDate *retrievedAt;
@property (nonatomic, copy) NSString *title;
@property (nonatomic, copy) NSString *body;
- (id)initWithDictionary:(NSDictionary *)dictionary;
@end
```
然后 .m 文件里的实现一般是这样的。
```
@implementation GHIssue
+ (NSDateFormatter *)dateFormatter {
    NSDateFormatter *dateFormatter = [[NSDateFormatter alloc] init];
    dateFormatter.locale = [[NSLocale alloc] initWithLocaleIdentifier:@"en_US_POSIX"];
    dateFormatter.dateFormat = @"yyyy-MM-dd'T'HH:mm:ss'Z'";
    return dateFormatter;
}
- (id)initWithDictionary:(NSDictionary *)dictionary {
    self = [self init];
    if (self == nil) return nil;
    _URL = [NSURL URLWithString:dictionary[@"url"]];
    _HTMLURL = [NSURL URLWithString:dictionary[@"html_url"]];
    _number = dictionary[@"number"];
    if ([dictionary[@"state"] isEqualToString:@"open"]) {
        _state = GHIssueStateOpen;
    } else if ([dictionary[@"state"] isEqualToString:@"closed"]) {
        _state = GHIssueStateClosed;
    }
    _title = [dictionary[@"title"] copy];
    _retrievedAt = [NSDate date];
    _body = [dictionary[@"body"] copy];
    _reporterLogin = [dictionary[@"user"][@"login"] copy];
    _assignee = [[GHUser alloc] initWithDictionary:dictionary[@"assignee"]];
    _updatedAt = [self.class.dateFormatter dateFromString:dictionary[@"updated_at"]];
    return self;
}
```
想象一下，如果你的 Model 有几十种，相当于你要写几十次这样重复的代码。因此，为了减少这种重复性的工作，应运而生了 Mantle 这样的库。
## Mantle的功能
了解了Mantle为何诞生。那么，我们就要看看Mantle到底为我们解决了什么样的问题？
- 避免了写重复性的 init 方法，通过服务端返回的数据自动生成 model ，也可以利用model来反序列化生成 JSON 。
- 当model里的某个属性名称和服务端的某个字段的名字不一样的时候，可以利用 
```
+
 (NSDictionary *)JSONKeyPathsByPropertyKey
```
 这个方法来进行字段的匹配。
- 可以通过 `+<key>JSONTransformer` 这个命名方法来声明一个转换方法。举例来说就是，一般服务端返回日期的时候是以时间戳的方式返回的，通常是一个长整形的数字，1464116217
 ,但是你声明的对应的property是一个 `NSDate` 类型，这时候，你就需要进行长整形 -> 日期类型的转换，所以
 Mantle 提供了这种方式，来进行方便的转换。
- 自动的decode和encoding。方便 Model 的归档。
当然，Mantle 远不止这些功能。但是，我们先搞清楚主要的功能，那么一些次要的功能自然就迎刃而解了。
## Mantle如何实现这些功能？
### 如何实现自动将Dictionary的值自动的赋给Model里对应的property？
我们先来尝试一下自己实现自动化的赋值。
熟悉OC的朋友应该知道NSObject有一个方法，叫做 
```
- (void)setValuesForKeysWithDictionary:(NSDictionary<NSString
 *, id> *)keyedValues;
```
这个方法就是利用kvc，直接让 model 调用 
```
- (void)setValue:(nullable id)value
 forKeyPath:(NSString *)keyPath;
```
 方法，通过遍历传入的字典的key，对model进行赋值。
举个简单例子，现在我们有个Model叫做User。它的property和init方法如下。
```
@property (nonatomic, copy) NSString *name;
@property (nonatomic, copy) NSString *address;
- (id)initWithDictionary:(NSDictionary *)dict
{
    if (self = [super init]) {
        [self setValuesForKeysWithDictionary:dict];
    }
    return self;
}
```
然后我们这样给它赋值。
```
NSDictionary *dict = @{@"name":@"zql", @"address":@"beijing"};
 User *user = [[User alloc] initWithDictionary:dict];
 NSLog(@"User name is %@, address is %@", user.name, user.address);
```
打印出来的结果就是
`User name is zql, address is beijing`
但是，这里有可能会出现几个问题。
- 如果 Dictionary 里的某一个 key 的名字和Model里的property的名字不匹配，就会造成`NSUnknownKeyException` 然后直接崩溃。
- 没法进行类型判断，如果你的dictionary里某个key对应的值和你的model里相同的key对应的值得类型不一致，他没有办法自动转换，而且完全不会报错。
当然，你可能会说，第一个问题，可以通过实现 
```
- (void)setValue:(id)value forUndefinedKey:(NSString
 *)key
```
 这个方法来进行检测和修复。
但问题是，如果大量的key不匹配的话，你又回到了原来的问题，需要写大量重复的代码。
这样分析下来，直接使用 `setValuesForKeysWithDictionary` 并不能实现我们的需求。
### Mantle 是如何做的？
显然，Mantle需要解决的第一个问题就是，如何建立起 Model 里的 property 和 Dictionary 里的key 一一对应的关系。
这个显然是需要用户提供的。
因为不同的 App 服务端返回的数据五花八门，命名方式有可能是驼峰命名也有可能不是，那么我们定义的 Model 也一样。如何才能建立起这种关系呢？
Mantle 的做法是，你需要
- 创建的 Model 需要继承自MTLModel
- 必须实现 MTLJSONSerializing 协议。
先看代码。
```
@interface Car : MTLModel<MTLJSONSerializing>
@property (nonatomic, copy) NSString *carName;
@property (nonatomic, copy) NSString *carOwnner;
@end
```
.m 文件中是这样的
```
+ (NSDictionary *)JSONKeyPathsByPropertyKey
{
    return @{@"carName": @"name",
             @"carOwnner" :@"ownner"
             };
}
```
我们来思考一下，为什么这样写就可以建立起 model 和 Dictionary 一一对应的关系。
首先，服务端返回给我们的 Car 信息的JSON文件中，键值对分别是， name 对应的是汽车的名称，ownner 对应的是汽车的拥有者，但是我们在创建 Model 的时候，汽车的名字是 carName，拥有者的名字是carOwnner ，所以，我们需要告诉 Mantle，在利用 kvc 赋值的时候。
```
id value = dictionary[@"carName"];
[self setValue:value forKey:@"carName"];
```
实际上走到这的时候第一步就错了，为什么？
因为dictionary里根本就没有 `carName` 这个key，你拿到的是nil。
所以，应该如何处理这种 model 里的 key 和 字典里 key 不一致的情况？
Mantle 的处理方法是，你需要告诉我，model 的 property 的名字和字典里的 key 存在怎样的一种对应关系。
所以，当 Mantle 在遍历Car这个类的Property列表的时候，，应当先去用户在 `JSONKeyPathsByPropertyKey` 方法中传回的字典里寻找，是否有Property对应的服务端的key，再利用这个对应的key去Dictionary里拿数据，再赋值给我们的property。比如上面这个例子，就应该把`carName`替换为`name`之后再从字典取值，然后再把取得的值赋给`carName`这个property。
需要注意的是，如果 `JSONKeyPathsByPropertyKey` 里没有填写任何对应关系，最新版本Mantle是不会默认
 Model 里的key和字典中的key 相同的，而是直接跳过。但是早些版本的Mantle会默认这种相同的关系，直接赋值。
本着严谨的精神，我去查看了一下 Mantle release 记录，看看到底是哪个版本的 Mantle 取消了这种默认的行为。
在 2.0 版本的ChangeLog里看到了下面一段话。
![](http://upload-images.jianshu.io/upload_images/107041-31cdedf8bb388f2d.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
![](http://upload-images.jianshu.io/upload_images/107041-1a43c73bfc580144.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
所以，在 2.0 版本， Mantle 取消了这种隐式的转换关系。
在这个说明里，作者还提到了一个方法，`+[NSDictionary mtl_identityPropertyMapWithModel:]` ，这个方法如何使用呢？当你的
 Model 里的所有属性的名字和 JSON 里的所有 key 的名字完全相同的时候，你就可以用这个方法直接生成一个 NSDictionary， 直接返回。省掉了自己写。例如.
```
+ (NSDictionary *)JSONKeyPathsByPropertyKey {
    return [NSDictionary mtl_identityPropertyMapWithModel:self];
}
```
### 分析源码
说了这么多。我们就来看看Mantle到底如何实现这么多好用的功能。
先看看 Mantle的目录结构。
![](http://upload-images.jianshu.io/upload_images/107041-4acad35106a7e79a.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
主要模块有：
- Modules 主要负责最基础的功能，包括通过 runtime 获取Class的property，encode 和 decode 功能。
- Adapters 主要负责JSON <-> Model转换的核心逻辑。
- ValueTransform 主要负责某个Property需要进行自定义转换的需求。例如服务端返回的时间戳 -> Model中声明的NSDate类型这种转换。
- libextobjc 将`property_attribute`这种
```
Type
 Encodings
```
过得东西转化为对应的Model。后面会细讲。
#### 基础知识
##### 1.如何利用Runtime获取一个类的所有Property的信息？
之前已经说过了，想要实现ORM的基本条件就是，获取一个类的所有属性的名字。拿不到属性的名字，就没法利用kvc赋值。
怎么拿？代码如下。
```cpp
```cpp
+ (void)printAllPropertiesAndVaules
{
    unsigned int outCount, i;
    objc_property_t *properties =class_copyPropertyList([self class], &outCount);
    for (i = 0; i<outCount; i++)
    {
        objc_property_t property = properties[i];
        const char *propertyName = property_getName(property);
        const char *propertyAttribute = property_getAttributes(property);
        NSLog(@"property name is %@, attributes is %@", @(propertyName), @(propertyAttribute));
    }
    free(properties);
}
```
```
打印的结果如下。
```
property name is name, attributes is T@"NSString",C,N,V_name
property name is address, attributes is T@"NSString",C,N,V_address
```
##### 2.上一个例子里打印出来的property_attribute是什么？
名字好理解，输出的和我们声明的property名字一致，这个没什么问题。但是后面那个attributes是个什么东西？
我们声明的`@property (nonatomic, copy) NSString *name;`通过runtime取出来之后的形式是这样的。
`T@"NSString",C,N,V_name`
关于这样的一段字符串到底是什么意思，苹果官方文档是这么说的
> 
The string starts with a T followed by the @encode type and a comma, and finishes with a V followed by the name of the backing instance variable. Between these, the attributes are specified by the following descriptors, separated by commas:
然后我画了一张图来解释这一段字符串到底是什么意思。
![](http://upload-images.jianshu.io/upload_images/107041-87fd996af0d13291.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
至于每个符号的意思，你们可以从苹果的官方文档中找到。地址在：[https://developer.apple.com/library/ios/documentation/Cocoa/Conceptual/ObjCRuntimeGuide/Articles/ocrtTypeEncodings.html#//apple_ref/doc/uid/TP40008048-CH100-SW1](https://developer.apple.com/library/ios/documentation/Cocoa/Conceptual/ObjCRuntimeGuide/Articles/ocrtTypeEncodings.html#//apple_ref/doc/uid/TP40008048-CH100-SW1)
所以，我们可以通过`property_getName`获取到property的名字，也可以通过`property_getAttributes`获取到property的一些属性。这样，也就迈出了第一步，起码，我们能知道一个类都有那些属性，这些属性叫什么，都是怎么声明的。
##### 3.解析EXTRuntimeExtensions这个类，先看看.h文件
Mantle中对应上述功能的就是这个类。
我们来看看Mantle是如何解析type encoding过的属性的。
```
typedef enum {
    /**
     * The value is assigned.
     */
    mtl_propertyMemoryManagementPolicyAssign = 0,
    /**
     * The value is retained.
     */
    mtl_propertyMemoryManagementPolicyRetain,
    /**
     * The value is copied.
     */
    mtl_propertyMemoryManagementPolicyCopy
} mtl_propertyMemoryManagementPolicy;
/**
 * Describes the attributes and type information of a property.
 */
typedef struct {
    /**
     * Whether this property was declared with the \c readonly attribute.
     */
    BOOL readonly;
    /**
     * Whether this property was declared with the \c nonatomic attribute.
     */
    BOOL nonatomic;
    /**
     * Whether the property is a weak reference.
     */
    BOOL weak;
    /**
     * Whether the property is eligible for garbage collection.
     */
    BOOL canBeCollected;
    /**
     * Whether this property is defined with \c \@dynamic.
     */
    BOOL dynamic;
    /**
     * The memory management policy for this property. This will always be
     * #mtl_propertyMemoryManagementPolicyAssign if #readonly is \c YES.
     */
    mtl_propertyMemoryManagementPolicy memoryManagementPolicy;
    /**
     * The selector for the getter of this property. This will reflect any
     * custom \c getter= attribute provided in the property declaration, or the
     * inferred getter name otherwise.
     */
    SEL getter;
    /**
     * The selector for the setter of this property. This will reflect any
     * custom \c setter= attribute provided in the property declaration, or the
     * inferred setter name otherwise.
     *
     * @note If #readonly is \c YES, this value will represent what the setter
     * \e would be, if the property were writable.
     */
    SEL setter;
    /**
     * The backing instance variable for this property, or \c NULL if \c
     * \c @synthesize was not used, and therefore no instance variable exists. This
     * would also be the case if the property is implemented dynamically.
     */
    const char *ivar;
    /**
     * If this property is defined as being an instance of a specific class,
     * this will be the class object representing it.
     *
     * This will be \c nil if the property was defined as type \c id, if the
     * property is not of an object type, or if the class could not be found at
     * runtime.
     */
    Class objectClass;
    /**
     * The type encoding for the value of this property. This is the type as it
     * would be returned by the \c \@encode() directive.
     */
    char type[];
} mtl_propertyAttributes;
```
首先，Mantle定义了一个枚举类型，用来记录一个property的内存管理方式，枚举中有三种类型。分别对应iOS中的`assign,retain,copy`.
枚举的名字叫做`mtl_propertyMemoryManagementPolicy`.
然后，Mantle声明了一个名为`mtl_propertyAttributes`的struct用来记录一个property的各种属性。
这个struct有下列成员。
`readonly`: 一个Bool值，用来记录这个property是否为readonly.
`nonatomic`: Bool。记录是否为nonatomic
`weak`: Bool。memory manage的方式是否为weak。
`canBeCollected`:Bool。是否可以被垃圾回收机制管理。（实际上iOS并没有自动垃圾回收机制。OSX以前有过，但是在mountain
 lion之后就被禁用了。）
`dynamic`: Bool.是否被声明为dynamic。
`memoryManagementPolicy`：即Mantle自己定义的枚举类型，用来记录property的内存管理方式，如果这个property是readonly的时候，那么这个值永远为`mtl_propertyMemoryManagementPolicyAssign`.
`getter`: SEL。即记录这个property的自定义getter方法。
`setter`: SEL.记录这个property的自定义setter方法
`ivar`: const char 类型。记录这个property对应的成员变量。例如`@property
 (nonatomic, copy) NSString user`,那么对应的成员变量应该为`_user`.`objectClass
```
:
 如果这个property的类型是一个自定义的类。那么这个objectClass用来记录这个类。
```
char type[]
```
:
 用来记录这个property的type encoding之后的值。例如，user这个property的类型是NSString，那么type encoding之后，NSString会被编译器转化成
```
@`这个标记，所以这个type里存储的就是`@`.
头文件中还定义了一个方法。
```
mtl_propertyAttributes *mtl_copyPropertyAttributes (objc_property_t
 property);
```
这个方法是干嘛的呢？
很简单，就是把我们的`objc_property_t`类型转换成我们的`mtl_propertyAttributes`的struct。
接下来，我们来看看.m文件中，Mantle是如何把一个runtime中的`objc_property_t`转换成struct的。
##### 再来看看EXTRuntimeExtensions.m
```cpp
```cpp
mtl_propertyAttributes *mtl_copyPropertyAttributes (objc_property_t property) {
    const char * const attrString = property_getAttributes(property);
    if (!attrString) {
        fprintf(stderr, "ERROR: Could not get attribute string from property %s\n", property_getName(property));
        return NULL;
    }
    if (attrString[0] != 'T') {
        fprintf(stderr, "ERROR: Expected attribute string \"%s\" for property %s to start with 'T'\n", attrString, property_getName(property));
        return NULL;
    }
    const char *typeString = attrString + 1;
    const char *next = NSGetSizeAndAlignment(typeString, NULL, NULL);
    if (!next) {
        fprintf(stderr, "ERROR: Could not read past type in attribute string \"%s\" for property %s\n", attrString, property_getName(property));
        return NULL;
    }
    size_t typeLength = next - typeString;
    if (!typeLength) {
        fprintf(stderr, "ERROR: Invalid type in attribute string \"%s\" for property %s\n", attrString, property_getName(property));
        return NULL;
    }
    // allocate enough space for the structure and the type string (plus a NUL)
    mtl_propertyAttributes *attributes = calloc(1, sizeof(mtl_propertyAttributes) + typeLength + 1);
    if (!attributes) {
        fprintf(stderr, "ERROR: Could not allocate mtl_propertyAttributes structure for attribute string \"%s\" for property %s\n", attrString, property_getName(property));
        return NULL;
    }
    // copy the type string
    strncpy(attributes->type, typeString, typeLength);
    attributes->type[typeLength] = '\0';
    // if this is an object type, and immediately followed by a quoted string...
    if (typeString[0] == *(@encode(id)) && typeString[1] == '"') {
        // we should be able to extract a class name
        const char *className = typeString + 2;
        next = strchr(className, '"');
        if (!next) {
            fprintf(stderr, "ERROR: Could not read class name in attribute string \"%s\" for property %s\n", attrString, property_getName(property));
            return NULL;
        }
        if (className != next) {
            size_t classNameLength = next - className;
            char trimmedName[classNameLength + 1];
            strncpy(trimmedName, className, classNameLength);
            trimmedName[classNameLength] = '\0';
            // attempt to look up the class in the runtime
            attributes->objectClass = objc_getClass(trimmedName);
        }
    }
    if (*next != '\0') {
        // skip past any junk before the first flag
        next = strchr(next, ',');
    }
    while (next && *next == ',') {
        char flag = next[1];
        next += 2;
        switch (flag) {
        case '\0':
            break;
        case 'R':
            attributes->readonly = YES;
            break;
        case 'C':
            attributes->memoryManagementPolicy = mtl_propertyMemoryManagementPolicyCopy;
            break;
        case '&':
            attributes->memoryManagementPolicy = mtl_propertyMemoryManagementPolicyRetain;
            break;
        case 'N':
            attributes->nonatomic = YES;
            break;
        case 'G':
        case 'S':
            {
                const char *nextFlag = strchr(next, ',');
                SEL name = NULL;
                if (!nextFlag) {
                    // assume that the rest of the string is the selector
                    const char *selectorString = next;
                    next = "";
                    name = sel_registerName(selectorString);
                } else {
                    size_t selectorLength = nextFlag - next;
                    if (!selectorLength) {
                        fprintf(stderr, "ERROR: Found zero length selector name in attribute string \"%s\" for property %s\n", attrString, property_getName(property));
                        goto errorOut;
                    }
                    char selectorString[selectorLength + 1];
                    strncpy(selectorString, next, selectorLength);
                    selectorString[selectorLength] = '\0';
                    name = sel_registerName(selectorString);
                    next = nextFlag;
                }
                if (flag == 'G')
                    attributes->getter = name;
                else
                    attributes->setter = name;
            }
            break;
        case 'D':
            attributes->dynamic = YES;
            attributes->ivar = NULL;
            break;
        case 'V':
            // assume that the rest of the string (if present) is the ivar name
            if (*next == '\0') {
                // if there's nothing there, let's assume this is dynamic
                attributes->ivar = NULL;
            } else {
                attributes->ivar = next;
                next = "";
            }
            break;
        case 'W':
            attributes->weak = YES;
            break;
        case 'P':
            attributes->canBeCollected = YES;
            break;
        case 't':
            fprintf(stderr, "ERROR: Old-style type encoding is unsupported in attribute string \"%s\" for property %s\n", attrString, property_getName(property));
            // skip over this type encoding
            while (*next != ',' && *next != '\0')
                ++next;
            break;
        default:
            fprintf(stderr, "ERROR: Unrecognized attribute string flag '%c' in attribute string \"%s\" for property %s\n", flag, attrString, property_getName(property));
        }
    }
    if (next && *next != '\0') {
        fprintf(stderr, "Warning: Unparsed data \"%s\" in attribute string \"%s\" for property %s\n", next, attrString, property_getName(property));
    }
    if (!attributes->getter) {
        // use the property name as the getter by default
        attributes->getter = sel_registerName(property_getName(property));
    }
    if (!attributes->setter) {
        const char *propertyName = property_getName(property);
        size_t propertyNameLength = strlen(propertyName);
        // we want to transform the name to setProperty: style
        size_t setterLength = propertyNameLength + 4;
        char setterName[setterLength + 1];
        strncpy(setterName, "set", 3);
        strncpy(setterName + 3, propertyName, propertyNameLength);
        // capitalize property name for the setter
        setterName[3] = (char)toupper(setterName[3]);
        setterName[setterLength - 1] = ':';
        setterName[setterLength] = '\0';
        attributes->setter = sel_registerName(setterName);
    }
    return attributes;
errorOut:
    free(attributes);
    return NULL;
}
```
```
```cpp
```
