# OC 自动生成分类属性方法 - 月若无涯的专栏 - CSDN博客
2016年08月23日 16:46:50[月若无涯](https://me.csdn.net/u010124617)阅读数：1107
好文，转自：[http://nathanli.cn/2015/12/14/objective-c-元编程实践-分类动态属性/](http://nathanli.cn/2015/12/14/objective-c-元编程实践-分类动态属性/)
## 背景
　　分类，在 iOS 开发中，是常常需要用到的。在分类里添加属性也是常有的事，但分类中无法添加实例变量，编译器也无法为提供分类中属性的 `getter`和 `setter` 方法了。一般而言，需要手动来实现这两个方法，如果只是用来存储变量的话，关联对象很容易做到这一点：

Objective-C
```
```
|123456789101112131415|@interfaceNSObject(db_sqlite)@property(nonatomic,assign)intdb_rowid;@end@implementationNSObject(db_sqlite)-(int)db_rowid{return[objc_getAssociatedObject(self,_cmd)intValue];}-(void)setDb_rowid:(int)db_rowid{objc_setAssociatedObject(self,@selector(db_rowid),@(db_rowid),OBJC_ASSOCIATION_RETAIN_NONATOMIC);}@end|
　　这是很常见的实现方式。
　　要是再给这个分类多加几个属性，也就得再多加几个这样的 `getter`、`setter` 方法，无法也就是方法名字、关联参数不一样罢了，这可真是个体力活呀！要是能像普通类属性那样就好了，自动给生成这两个方法，想想就爽。
　　要想做到自动生成这两个方法，可以从两个方面入手：
　　1、编码期
　　2、运行期
　　编码期。在写代码的时候要做到自动生成方法，可以写一个 XCode 插件，一按某些快捷键，相应代码就自动生成了，这有点类似于 Eclipse。插件的讨论不在本文范围内。
　　运行期。在编码阶段只需少量代码，具体的方法则在运行期动态生成。本文研究怎么在运行期动态生成这些所需要的方法。本文最终生成的代码在：[https://github.com/NathanLi/iOSCategoryPropertyDynamicSupport](https://github.com/NathanLi/iOSCategoryPropertyDynamicSupport)
### 需求
　　简单点说，就是在运行时能生成分类中属性相应的 `getter``setter` 方法，也就是模仿类中普通 @property 定义的属性。这样的需求太泛，咱们先来细化一下：
　　1、只生成分类中的，我想要的 `getter` 和 `setter`方法体；
　　2、属性类型：支持基本数据类型、对象、结构体，且自动存取；
　　3、支持 `@property` 定义中的 `assign`、`strong`、`copy`、`weak`；
　　4、支持 `@property` 中的自定义的方法名；
　　5、支持 `KVC`；
　　6、支持 `KVO`；
　　7、本条不是需求，而是简单的设定：不支持原子即 atomic，只支持 nonatomic。
## 实现
### 1、确定要动态生成方法的属性
　　这里根据属性的名字来确定是否需要动态生成方法，就以 `nl_` 为前辍就好了：

Objective-C
``
|12|@property(nonatomic,strong)idnl_object;|
　　由于是在分类中，且没有定义相应的方法，所以会有警告：

Objective-C
```
```
|123|Property'nl_object'requires method'nl_object'to be defined-use@dynamicor provideamethod implementation inthis categoryProperty'nl_object'requires method'setNl_object:'to be defined-use@dynamicor provideamethod implementation inthis category|
　　在分类实现里加个 `@dynamic` 就好了：

Objective-C
``
|12|@dynamicnl_double;|
### 2、消息
`@dynamic` 告诉编译器，这两个方法有没有实现你都不用管，就当作它们存在就行了。那么问题来了，这两个方法明明没有实现，却依然能够调用呢？
　　这根消息发送机制有关。在 Objective-C中，消息不会与方法实现绑定，而是在运行时才关联起来的。
　　编译器会把所有消息转换为一个函数调用：`objc_msgSend`。这个函数总得知道是 `谁` 发送了 `哪条` 消息吧，所以它最少也有两个参数——消息的接收者
 和 消息名（即选择子 `SEL`），比如下面这个方法：

Objective-C
```
```
|12|[receiver message]|
　　编译器就会把它变成这个样子：

Objective-C
``
|12|objc_msgSend(receiver,message)|
　　如果消息有参数的话，就会直接传这个函数，所以这个函数的参数个数不定：

Objective-C
```
```
|12|objc_msgSend(receiver,selector,arg1,arg2,...)|
`objc_msgSend` 的工作就是消息的动态绑定：
　　1、根据 `selecotr` 和 `receiver` 找到对应的函数实现（也就是函数地址）。不同的类可以有相同的方法，但是它们所对应的函数地址是不一样的。
　　2、调用找到的函数，并传入相应的参数：`receiver`、`selector`、`arg1`…。
　　3、返回调用的函数的返回值。
　　来看下实例：

Objective-C
``
|123456789101112131415|@implementationNLPerson-(instancetype)init{if(self=[superinit]){[self setName:@"name"];}returnself;}-(void)setName:(NSString*)name{_name=name;}@end|
　　所对应的函数代码是这样的：

Objective-C
```
```
|1234567891011|staticinstancetype _I_NLPerson_init(NLPerson*self,SEL_cmd){if(self...){objc_msgSend((id)self,sel_registerName("setName:"),"name");}returnself;}staticvoid_I_NLPerson_setName_(NLPerson*self,SEL_cmd,NSString*name){...}|
　　可以看到， `[self setName:@"name"]`，最后变成了 `objc_msgSend` 函数调用。这个函数最终会根据 `self` 和 `setName:` 找到函数 `_I_NLPerson_setName_` 并调用。被调用的函数包含三个参数，分别是调用者、`SEL`（`_cmd`）和方法参数。
　　正如上那个函数看到的，每个方法都有一个选择子这个参数：`_cmd`，所以才能这么打印方法名：

Objective-C
``
|12345|-(void)setName:(NSString*)name{NSLog(@"%s",sel_getName(_cmd));_name=name;}|
`SEL` 实际上就是一个字符串：
```
cahr
 *
```
，所以咱们将 `SEL` 简单理解为方法名也并无不可。刚刚说到了，`objc_msgSend` 会根据 `SEL` 找到对应的函数地址，来看看它是怎么找的。
　　实际上，OC 中的所有对象和类，最后都被处理为结构体。对象结构体中，会有一个 `isa` 指针，指向自己的类结构体。而类结构体有很多类信息，其中两个：
　　1、指向 `superclass` 的指针。
　　2、类分发表。这个表里存储了方法名 `selector` 与所对应的函数地址 `address`。
　　如上面的 `NLPeson` 类中的分发表：
|selector|addrss|
|----|----|
|init|_I_NLPerson_init|
|setName:|_I_NLPerson_setName_|
|…|…|
| | |
　　消息传递框架图：
![方法查找图](https://developer.apple.com/library/ios/documentation/Cocoa/Conceptual/ObjCRuntimeGuide/Art/messaging1.gif)
　　当发送一个消息给一个对象时，首先会去这个对象的 `isa` 所指向的类结构体里的分发表中寻找 `selector`，如果找不到的话，`objc_msgSend` 会根据 `superclass` 指针找到下一个结构体里寻找 `selector`，直到 `NSObject`。只要它找到了 `selector`，就会调用相应的函数了。意思就是说，先通过消息名，找到函数地址，再调用。这就是所谓的消息运行时动态绑定。
### 3、动态增加方法
　　如果给对象发送了一个未知的消息，如果这个对象无法响应或转发的话，就会调用 `doesNotRecognizeSelector:` 方法，这个方法会抛出 `NSInvalidArgumentException` 异常。如果你不想让一个让别人调用你的类的 `copy` 或 `init` 方法的话，可以这么做：

Objective-C
```
```
|1234|-(id)copy{[self doesNotRecognizeSelector:_cmd];}|
　　但在调用这个方法之前，系统还是给了我们处理的机会。实现 `resolveInstanceMethod:` 方法，能动态地给实例方法和类方法添加一个实现。
　　Objective-C 中的方法所对应的函数最少有两个参数：`self` 和 `_cmd`。如下所示：

Objective-C
``
|1234|voiddynamicMethodIMP(idself,SEL_cmd){// implementation ....}|
　　可以用 C 函数 `class_addMethod` 将其作为一个方法动态加到一个类中：

Objective-C
```
```
|1234567891011|@implementationMyClass+(BOOL)resolveInstanceMethod:(SEL)aSEL{if(aSEL==@selector(resolveThisMethodDynamically)){class_addMethod([selfclass],aSEL,(IMP)dynamicMethodIMP,"v@:");returnYES;}return[super resolveInstanceMethod:aSEL];}@end|
### 4、属性元数据、类型编码（Type Encodings）
　　要能动态生成属性的方法，首先得知道属性的一些基本信息：类型、方法名、是 `weak` 还是 `strong` 等。这些数据都可以在运行时获得到。要用到的技术是：类型编码(Type Encdoings)。
　　类型编码是 runtime 的辅助工具。编译器会将类型用字符串来表示。可以用 `@encode` 得到这个字符串：

Objective-C
``
|1234|char*buf1=@encode(int);// buf1 --> "i"char*buf2=@encode(longlong);// buf2 --> "q"char*buf3=@encode(unsignedint);// buf2 --> "I"|
　　编码表如下：
![此处输入图片的描述](https://raw.githubusercontent.com/NathanLi/_private_images_repository/master/images/Objective-C_type_encodings.png)
　　这是描述类型的数据。那描述属性的呢？
　　编译器会将类、分类和协议中的属性以元数据信息存起来。有一系列的 C 函数来访问这些数据。
　　属性元数据是用结构体 `Property` 来描述的：

Objective-C
```
```
|12|typedefstructobjc_property *Property;|
　　可以用 `class_copyPropertyList` 和 `protocol_copyPropertyList` 来分别获取类（包含分类）中和协议中的属性：

Objective-C
``
|123|objc_property_t*class_copyPropertyList(Classcls,unsignedint*outCount)objc_property_t*protocol_copyPropertyList(Protocol*proto,unsignedint*outCount)|
　　比如下面声明的这个类：

Objective-C
```
```
|12345678|@interface Person : NSObject@property(nonatomic,assign)floatage;@end...@dynamicage;|
　　可以这么来获取它的属性列表：

Objective-C
``
|1234|idPersonClass=objc_getClass("Person");unsignedintoutCount;objc_property_t*properties=class_copyPropertyList(PersonClass,&outCount);|
　　除了一次性获得所有属性列表外，还有方法 `class_getProperty` 和 `protocol_getProperty` 可以通过属性名获取单个属性：

Objective-C
```
```
|123|objc_property_t class_getProperty(Classcls,constchar*name)objc_property_t protocol_getProperty(Protocol*proto,constchar*name,BOOLisRequiredProperty,BOOLisInstanceProperty)|
　　获取到属性结构体后，就可以拿到这个属性的名字和元信息：

Objective-C
``
|123|constchar*property_getName(objc_property_tproperty)// 获取属性的名字constchar*property_getAttributes(objc_property_tproperty)// 获取属性的元信息|
`property_getAttributes` 能获取到属性的很多信息，包括刚看到的类型编码、`getter`和`setter` 方法名、对应的实例变量名等等。打印所有属性的元信息例子：

Objective-C
```
```
|123456789|idPersonClass=objc_getClass("Person");unsignedintoutCount,i;objc_property_t *properties=class_copyPropertyList(PersonClass,&outCount);for(i=0;i<outCount;i++){objc_property_t property=properties[i];fprintf(stdout,"%s %s\n",property_getName(property),property_getAttributes(property));// 输出：age Tf,D,N}|
`property_getAttributes` 获取到的 `Tf,D,N` 是什么意思呢？`Tf`，是以 `T`开头，后面的字符串 `f` 表示类型编码；`D` 表示 `@dynamic`；`N` 表示 `nonatomic`。这些都是属性本身的信息，以 `,` 分割。这些字符串的规则是这样的：
|Code|意义|
|----|----|
|R|readonly|
|C|copy|
|&|assigned (retain).|
|N|nonatomic|
|G`name`|以 G 开头是的自定义的 Getter 方法名。(如：GcustomGetter 名字是:customGetter).|
|S`name`|以 S 开头是的自定义的 Setter 方法名。(如：ScustoSetter: 名字是: ScustoSetter:).|
|D|@dynamic|
|W|__weak|
　　来看看下面这个例子，你就全理解了：
![此处输入图片的描述](https://raw.githubusercontent.com/NathanLi/_private_images_repository/master/images/property_getAttributes_explams.png)
### 5、属性解析
　　直接使用属性的元数据可不太好用，用一个对象来描述它会好很多。

Objective-C
``
|12345678910111213141516171819202122232425262728293031323334353637383940414243444546|typedefNS_ENUM(NSUInteger,NLPropertyPolicy){NLPropertyPolicyAssign,NLPropertyPolicyStrong,NLPropertyPolicyCopy,NLPropertyPolicyWeak,};@interface NLPropertyDescriptor : NSObject/***  @brief 属性名*/@property(nonatomic,copy,readonly)NSString*name;/***  @brief getter 方法名*/@property(nonatomic,copy,readonly)NSString*getterName;/***  @brief setter 方法名*/@property(nonatomic,copy,readonly)NSString*setterName;/***  @brief 变量名*/@property(nonatomic,copy,readonly)NSString*variableName;/***  @brief 属性类型编码*/@property(nonatomic,copy,readonly)NSString*typeEncoding;/***  @brief 属性类型*/@property(nonatomic,assign,readonly)NLPropertyPolicypropertyPolicy;/***  @brief 初始化*/-(instancetype)initWithObjcProperty:(objc_property_t)objcProperty;@end|
　　将属性的各项特性都存起来，想要的时候直接拿就好了，这就比 `objc_property_t` 好用多了。下面是初始化方法：

Objective-C
```
```
|12345678910111213141516171819202122232425262728293031323334353637383940414243444546474849505152535455|-(instancetype)initWithObjcProperty:(objc_property_t)objcProperty{if(self=[superinit]){_propertyPolicy=NLPropertyPolicyAssign;constchar*cPropertyName=property_getName(objcProperty);_name=[[NSString stringWithCString:cPropertyName encoding:NSUTF8StringEncoding] copy];_getterName=[_name copy];_variableName=[@"_" stringByAppendingString:_name];({// default setter name.NSString*firstChar=[[_name substringToIndex:1] uppercaseString];NSString*subjectName=[_name substringFromIndex:1]?:@"";subjectName=[subjectName stringByAppendingString:@":"];_setterName=[[NSString stringWithFormat:@"set%@%@",firstChar,subjectName] copy];});constchar*cPropertyAttributes=property_getAttributes(objcProperty);NSString*sPropertyAttributes=[NSString stringWithCString:cPropertyAttributes encoding:NSUTF8StringEncoding];NSArray*attributes=[sPropertyAttributes componentsSeparatedByString:@","];[attributes enumerateObjectsUsingBlock:^(NSString*_Nonnull obj,NSUIntegeridx,BOOL*_Nonnull stop){if(idx==0){// 第一个一定是类型编码_typeEncoding=[obj copy];}if([obj hasPrefix:@"G"]){// getter 方法名NSString*getterName=[obj substringFromIndex:1];_getterName=[getterName copy];}elseif([obj hasPrefix:@"S"]){// setter 方法名NSString*setterName=[obj substringFromIndex:1];_setterName=[setterName copy];}elseif([obj hasPrefix:@"V"]){// 变量名NSString*variableName=[obj substringFromIndex:1];_variableName=[variableName copy];}elseif([obj isEqualToString:@"&"]){_propertyPolicy=NLPropertyPolicyStrong;}elseif([obj isEqualToString:@"C"]){_propertyPolicy=NLPropertyPolicyCopy;}elseif([obj isEqualToString:@"W"]){_propertyPolicy=NLPropertyPolicyWeak;}elseif([obj isEqualToString:@"R"]){// readonly_setterName=nil;}}];}returnself;}|
　　可以通过 `class_copyPropertyList` 获取到一个类中的所有属性结构体，也就能拿到所有属性的元数据。但大部分属性咱们是不感兴趣的，只对 `@dynamic` 以及以 `nl_` 为前辍的属性感兴趣。那就写一个分类方法，用来获取对咱们有用的所有属性数据：

Objective-C
``
|123456789101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657|@interfaceNSObject(nl_dynamicPropertyStore)/***  @brief 判断是否应该自动生成方法的属性*/+(BOOL)nl_validDynamicProperty:(_Nonnullobjc_property_t)objProperty;/***  @brief  所有需要动态增加 getter、setter 方法的属性描述器*/+(NSArray<NLPropertyDescriptor*>*_Nullable)nl_dynamicPropertyDescriptors;@end@implementationNSObject(nl_dynamicPropertyStore)+(BOOL)nl_validDynamicProperty:(objc_property_t)objProperty{constchar*propertyAttributes=property_getAttributes(objProperty);// 必须是 @dynamicstaticchar*conststaticDynamicAttribute=",D,";if(strstr(propertyAttributes,staticDynamicAttribute)==NULL){returnNO;}// 名字得以 “nl_” 为前辍constchar*propertyName=property_getName(objProperty);staticchar*conststaticPropertyNamePrefix="nl_";if(strstr(propertyName,staticPropertyNamePrefix)!=propertyName){returnNO;}returnYES;}+(NSArray*)nl_dynamicPropertyDescriptors{NSMutableArray*descriptors=objc_getAssociatedObject(self,_cmd);if(nil==descriptors){unsignedintoutCount,index;descriptors=[NSMutableArray arrayWithCapacity:outCount];objc_setAssociatedObject(self,_cmd,descriptors,OBJC_ASSOCIATION_RETAIN_NONATOMIC);// 获取到本类所有的属性结构体，并转换为属性描述器objc_property_t*properties=class_copyPropertyList([selfclass],&outCount);for(index=0;index<outCount;++index){objc_property_tproperty=properties[index];if([self nl_validDynamicProperty:property]){NLPropertyDescriptor*descriptor=[[NLPropertyDescriptoralloc] initWithObjcProperty:property];[descriptors addObject:descriptor];}}free(properties);}returndescriptors;}@end|
`getter` 和 `setter` 方法里的数据总得存储在某个地方吧，用字典来存储是比较理想的做法。就在 `nl_dynamicPropertyStore` 这个分类里定义：

Objective-C
```
```
|123456789101112131415161718|@interfaceNSObject(nl_dynamicPropertyStore)/** *  @brief  用来存储自动生成的 `getter`、`setter` 操作的数据 */@property(nonatomic,strong,readonly)NSMutableDictionary*_Nullable nl_dynamicPropertyDictionary;@end@implementationNSObject(nl_dynamicPropertyStore)-(NSMutableDictionary*)nl_dynamicPropertyDictionary{NSMutableDictionary*dynamicProperties=objc_getAssociatedObject(self,_cmd);if(!dynamicProperties){dynamicProperties=[NSMutableDictionary dictionaryWithCapacity:2];objc_setAssociatedObject(self,_cmd,dynamicProperties,OBJC_ASSOCIATION_RETAIN_NONATOMIC);}returndynamicProperties;}@end|
### 6、自动生成 `getter`、`setter` 方法
　　要用到的知识都已经介绍完了，接着就看看怎么来自动生成方法了。
　　前面介绍过，当发送了一个没有实现过的消息时，我们在 `resolveInstanceMethod:` 方法中为其添加实现。这个方法在 `NSObject` 类中定义，在这里，不可能继承它来实现我们想要的功能。我们可以在 `NSObject`的分类中写一个新的方法来替代原有的这个方法实现，这叫“方法调配”(method swizzling)，这常常用于给原有方法增加新的功能。
> 
方法是动态绑定的，只有在运行时经过查找 后，才知道这条消息所对应的函数。方法，也就是一个名字，加上一个与之关联的函数。所谓方法调配，也就是将两个方法各自关联的函数互相交换一行而已。比如，nameA–>funcationA（nameA 是方法名，funcationA 是关联的方法实现函数）, nameB–>funcationB，经过方法调配后，nameA–>funcationB，nameB–>funcationA。那么此时 [obj nameA] 这个消息，实现上调用的是 funcationB。
　　方法调配的核心函数是 `method_exchangeImplementations`，它就是交换两个方法的实现的，代码：

Objective-C
``
|12345678910111213141516171819|@interfaceNSObject(nl_dynamicPropertySupport)@end@implementationNSObject(nl_dynamicSupport)+(void)load{MethodresolveInstanceMethod=class_getClassMethod(self,@selector(resolveInstanceMethod:));Methodnl_resolveInstanceMethod=class_getClassMethod(self,@selector(nl_resolveInstanceMethod:));if(resolveInstanceMethod&&nl_resolveInstanceMethod){// method swizzlingmethod_exchangeImplementations(resolveInstanceMethod,nl_resolveInstanceMethod);}}#pragma mark - swizzle +resolveInstanceMethod+(BOOL)nl_resolveInstanceMethod:(SEL)sel{// 最后记得调用原有的实现return[self nl_resolveInstanceMethod:sel];}|
　　经过调配之后，原本调用 `resolveInstanceMethod` 最后执行的是 `nl_resolveInstanceMethod` 方法体。由于是给 `resolveInstanceMethod` 增加新的功能，所以在自定义的方法实现了自己的逻辑后，再调用原有的实现。那接下来就将增加方法的逻辑放在这里。
　　要添加方法，得先把这些方法所对应的函数定义出来。由于 `getter`、`setter` 方法的参数个数和返回值个数都是一致的，所以它们对应的函数并不与属性名相关。而且所有属性的方法都有一个共同的参数：`SEL`，我们可以用这个参数来对数据进行存储。这里以对象、int、CGRect类型为例：

Objective-C
```
```
|12345678910111213141516171819202122232425262728293031323334353637383940414243444546474849505152535455|@interfaceNSObject(nl_dynamicPropertyStore)/** * 获取该选择子对应的属性名 */+(NSString*)nl_dynamicPropertyNameWithSelctor:(SEL)selector{return[[self nl_descriptorWithSelector:selector] name];}/** * 获取该选择子对应的属性描述器 */+(NLPropertyDescriptor*)nl_descriptorWithSelector:(SEL)selector{for(NLPropertyDescriptor*descriptorin[selfnl_dynamicPropertyDescriptors]){NSString*selectorName=NSStringFromSelector(selector);if([descriptor.getterName isEqualToString:selectorName]||[descriptor.setterName isEqualToString:selectorName]){returndescriptor;}}returnnil;}@end// 对象类型的属性的 setter 方法的实现void__NL__object_dynamicSetterIMP(idself,SEL_cmd,idarg){NSString*propertyName=[[selfclass] nl_dynamicPropertyNameWithSelctor:_cmd];[[selfnl_dynamicPropertyDictionary] setObject:arg forKey:propertyName];}// 对象类型的属性的 getter 方法的实现id__NL__object_dynamicGetterIMP(idself,SEL_cmd){NSString*propertyName=[[selfclass] nl_dynamicPropertyNameWithSelctor:_cmd];return[[selfnl_dynamicPropertyDictionary] objectForKey:propertyName];}// int类型的属性的 setter 方法的实现void__NL__int_dynamicSetterIMP(idself,SEL_cmd,intarg){NSString*propertyName=[[selfclass] nl_dynamicPropertyNameWithSelctor:_cmd];[[selfnl_dynamicPropertyDictionary] setObject:@(arg) forKey:propertyName];}// int类型的属性的 getter 方法的实现int__NL__int_dynamicGetterIMP(idself,SEL_cmd){NSString*propertyName=[[selfclass] nl_dynamicPropertyNameWithSelctor:_cmd];return[[[selfnl_dynamicPropertyDictionary] objectForKey:propertyName] intValue];}// CGRect类型的属性的 setter 方法的实现void__NL__cgrect_dynamicSetterIMP(idself,SEL_cmd,CGRectarg){NSString*propertyName=[[selfclass] nl_dynamicPropertyNameWithSelctor:_cmd];[[selfnl_dynamicPropertyDictionary] setObject:[NSValue valueWithCGRect:arg] forKey:propertyName];}// CGRect类型的属性的 getter 方法的实现CGRect__NL__cgrect_dynamicGetterIMP(idself,SEL_cmd){NSString*propertyName=[[selfclass] nl_dynamicPropertyNameWithSelctor:_cmd];return[[[selfnl_dynamicPropertyDictionary] objectForKey:propertyName] CGRectValue];}|
　　方法的各个实现都有了，接下来的工作根据未实现的方法名，找到对应的函数，再把这个函数加到方法中去：

Objective-C
``
|12345678910111213141516171819202122232425262728293031323334353637383940414243444546474849505152535455565758596061626364656667686970717273747576777879|+(BOOL)nl_resolveInstanceMethod:(SEL)sel{NSArray<NLPropertyDescriptor*>*propertyDescriptors=[selfnl_dynamicPropertyDescriptors];for(NLPropertyDescriptor*propertyDescriptorinpropertyDescriptors){BOOLdidAddMethod=[self nl_addMethodWithDescriptor:propertyDescriptor selector:sel];if(didAddMethod){returnYES;}}// 最后记得调用原有的实现return[self nl_resolveInstanceMethod:sel];}+(BOOL)nl_addMethodWithDescriptor:(NLPropertyDescriptor*)desciptor selector:(SEL)sel{NSString*selName=NSStringFromSelector(sel);if([desciptor.setterName isEqualToString:selName]){BOOLaddedSetter=[self nl_addSetterMethodWithDescriptor:desciptor];returnaddedSetter;}if([desciptor.getterName isEqualToString:selName]){BOOLaddedGetter=[self nl_addGetterMethodWithDescriptor:desciptor];returnaddedGetter;}returnNO;}// 添加 setter 方法实现+(BOOL)nl_addSetterMethodWithDescriptor:(NLPropertyDescriptor*)desciptor{IMPsetterIMP=NULL;if([desciptorisIntType]){setterIMP=(IMP)__NL__int_dynamicSetterIMP;}if([desciptorisObjectType]){setterIMP=(IMP)__NL__object_dynamicSetterIMP;}if([desciptorisRectType]){setterIMP=(IMP)__NL__cgrect_dynamicSetterIMP;}if(setterIMP!=NULL){class_addMethod(self,NSSelectorFromString(desciptor.setterName),setterIMP,"v@:");returnYES;}returnNO;}// 添加 getter 方法实现+(BOOL)nl_addGetterMethodWithDescriptor:(NLPropertyDescriptor*)desciptor{SELselector=NSSelectorFromString(desciptor.getterName);if([desciptorisIntType]){class_addMethod(self,selector,(IMP)__NL__int_dynamicGetterIMP,"i@:");returnYES;}NSString*typeEncoding=[desciptortypeEncoding];if([typeEncoding hasPrefix:@"T"]){typeEncoding=[typeEncoding substringFromIndex:1];}constchar*cFuncationTypes=[[typeEncoding stringByAppendingString:@"@:"] cStringUsingEncoding:NSUTF8StringEncoding];if([desciptorisObjectType]){{class_addMethod(self,selector,(IMP)__NL__object_dynamicGetterIMP,cFuncationTypes);returnYES;}if([desciptorisRectType]){class_addMethod(self,selector,(IMP)__NL__cgrect_dynamicGetterIMP,cFuncationTypes);returnYES;}returnNO;}|
> 
`class_addMethod` 函数声明：
```
BOOL
 class_addMethod(Class cls, SEL name, IMP imp, const char *types)
```
。`cls` 是要添加方法的类；`name`是要添加方法实现的名字；`imp`是要添加方法对应的实现，`types`是用类型编码描述该方法参数的字符串，而方法的函数必定会有参数：`self`（对象，类型编码是`@`）和`_cmd`（选择子，类型编码是`:`），所以这个 `type` 字符串中必定包含
 “@:” 子串，这个子串前的字符是这个方法的返回值，其后面的字符是该方法的其它参数。
　　实验一把：


Objective-C
```
```
|123456789101112131415161718192021|@interfaceViewController(nl_ex)@property(nonatomic,assign)intnl_int;@property(nonatomic,strong)idnl_object;@end@implementationViewController(nl_ex)@dynamicnl_object;@dynamicnl_int;@end-(void)viewDidLoad{[superviewDidLoad];self.nl_int=20;self.nl_object=[UIViewnew];fprintf(stdout,"nl_int = %d\n",self.nl_int);fprintf(stdout,"nl_object = %s\n",[[self.nl_object description] cStringUsingEncoding:NSUTF8StringEncoding]);// 输出： nl_int = 20//       nl_object = new nl_object string}|
　　完全没问题，奖励自己一把先。
### 7、添加 KVO 支持
　　KVO 还不简单，在 `setter` 实现里加上 `willChangeValueForKey:` 和 `didChangeValueForKey:` 就好了：

Objective-C
``
|1234567|void__NL__object_dynamicSetterIMP(idself,SEL_cmd,idarg){NSString*propertyName=[[selfclass] nl_dynamicPropertyNameWithSelctor:_cmd];[self willChangeValueForKey:propertyName];[[selfnl_dynamicPropertyDictionary] setObject:arg forKey:propertyName];[self didChangeValueForKey:propertyName];}|
　　再来验证一把：

Objective-C
```
```
|12345678910111213|-(void)observeValueForKeyPath:(NSString*)keyPath ofObject:(id)object change:(NSDictionary<NSString*,id>*)change context:(void*)context{idvalue=[object valueForKeyPath:keyPath];fprintf(stdout,"observe %s = %s\n",[keyPath cStringUsingEncoding:NSUTF8StringEncoding],[[value description] cStringUsingEncoding:NSUTF8StringEncoding]);}-(void)viewDidLoad{[superviewDidLoad];[self addObserver:self forKeyPath:@"nl_object" options:NSKeyValueObservingOptionNew context:nil];self.nl_object=[UIViewnew];fprintf(stdout,"nl_object = %s\n",[[self.nl_object description] cStringUsingEncoding:NSUTF8StringEncoding]);}|
　　会打印出什么？
　　可惜，什么也不会打印，而会崩溃：

Objective-C
``
|123456789101112|2015-12-1400:10:48.700CategoryPropertyDynamicSupport[1707:100735]***Terminatingappduetouncaughtexception'NSInvalidArgumentException',reason:'*** setObjectForKey: key cannot be nil'***Firstthrowcallstack:(0CoreFoundation0x00000001063cce65__exceptionPreprocess+1651libobjc.A.dylib0x0000000105e45debobjc_exception_throw+482CoreFoundation0x00000001062ca6e2-[__NSDictionaryMsetObject:forKey:]+10423CategoryPropertyDynamicSupport0x0000000105579e44__NL__object_dynamicSetterIMP+2604CategoryPropertyDynamicSupport0x0000000105582689-[ViewControllerviewDidLoad]+15615UIKit0x0000000106fdef98-[UIViewControllerloadViewIfRequired]+11986UIKit0x0000000106fdf2e7-[UIViewControllerview]+27...|
　　log 显示 `__NL__object_dynamicSetterIMP` 函数里的 
```
[[self
 nl_dynamicPropertyDictionary] setObject:arg forKey:propertyName];
```
 崩溃，原因是 `propertyName` 等于 `nil`。`propertyName` 不是选择子所对应的属性名吗，这个属性明明存在的呀，怎么为会空呢？
　　看看下面的代码：

Objective-C
```
```
|12345678|-(void)viewDidLoad{[superviewDidLoad];[self addObserver:self forKeyPath:@"nl_object" options:NSKeyValueObservingOptionNew context:nil];Classclass=[selfclass];// ViewControllerClasskvoClass=object_getClass(self);// NSKVONotifying_ViewControllerClasskvoSuperClass=class_getSuperclass(kvoClass)// ViewController|
　　原因就在这里，在 `addObserver:...` 后，咱们这个对象所属的类就已经不是原来的那个类了，而是原来的类的子类了。系统不过重写了 `-class` 方法，让人看起来还是原来的类的样子。咱们之前的 `nl_dynamicPropertyDescriptors` 只包含了当前类的属性，显然不对。这里把父类的属性也加进去：

Objective-C
``
|1234567891011121314151617181920212223242526272829|+(NSArray*)nl_dynamicPropertyDescriptors{NSMutableArray*descriptors=objc_getAssociatedObject(self,_cmd);if(nil==descriptors){unsignedintoutCount,index;descriptors=[NSMutableArray arrayWithCapacity:outCount];objc_setAssociatedObject(self,_cmd,descriptors,OBJC_ASSOCIATION_RETAIN_NONATOMIC);// 获取到本类所有的属性结构体，并转换为属性描述器objc_property_t*properties=class_copyPropertyList([selfclass],&outCount);for(index=0;index<outCount;++index){objc_property_tproperty=properties[index];if([self nl_validDynamicProperty:property]){NLPropertyDescriptor*descriptor=[[NLPropertyDescriptoralloc] initWithObjcProperty:property];[descriptors addObject:descriptor];}}free(properties);if(self!=[NSObjectclass]){// 加上父类的属性描述器[descriptors addObjectsFromArray:[class_getSuperclass(self)nl_dynamicPropertyDescriptors]];}}returndescriptors;}|
　　再来验证一下：

Objective-C
```
```
|12345678910111213141516|-(void)observeValueForKeyPath:(NSString*)keyPath ofObject:(id)object change:(NSDictionary<NSString*,id>*)change context:(void*)context{idvalue=[object valueForKeyPath:keyPath];fprintf(stdout,"observe %s = %s\n",[keyPath cStringUsingEncoding:NSUTF8StringEncoding],[[valuedescription] cStringUsingEncoding:NSUTF8StringEncoding]);}-(void)viewDidLoad{[superviewDidLoad];[self addObserver:self forKeyPath:@"nl_object" options:NSKeyValueObservingOptionNew context:nil];self.nl_object=[UIViewnew];fprintf(stdout,"nl_object = %s\n",[[self.nl_objectdescription] cStringUsingEncoding:NSUTF8StringEncoding]);// 输出：observe nl_object = <UIView: 0x7f8a6b82e820; frame = (0 0; 0 0); layer = <CALayer: 0x7f8a6b82e6b0>//      nl_object = <UIView: 0x7f8a6b82e820; frame = (0 0; 0 0); layer = <CALayer: 0x7f8a6b82e6b0>>}|
　　验证通过。
### 8、结束
　　代码过多，KVC 和 weak 的支持属于细枝末节，这里就不一一介绍了，想看看完整的代码的话，这里：[https://github.com/NathanLi/iOSCategoryPropertyDynamicSupport](https://github.com/NathanLi/iOSCategoryPropertyDynamicSupport)。
　　虽然现在 Objective-C 在 Swift 面前已经显得过时，但这 runtime 知识此时了解却也还是有些价值的。这里只是简单的介绍了一个属性相关的知识，实际上可玩的东西很多，比如 ORM （如 LKDBHelper） 等等。
