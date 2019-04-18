# 编写高质量iOS的有效方法 - weixin_33985507的博客 - CSDN博客
2016年01月21日 13:13:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：10
本文章简括了[《Effective Objective-C 2.0 编写高质量iOS与OS X代码的52个有效方法》](https://link.jianshu.com?t=http://pan.baidu.com/s/1bsdRhc)书中的一些使用规范，具体使用方法可参考书中相应内容，或自行google。
### 熟悉Objective-C对象、消息及运行期
**1）类的头文件中尽量少引入其它头文件**
> 
A）某个类头文件包含另一个类属性，用@class表明该属性是个类即可（向前声明）。
B）如果无法使用向前声明，如声明某个类遵循一项协议，尽量把协议移至分类或单独起文件。
**2）多用字面语法**
> 
A）NSNumber *charNumber = @‘a’ 总比 NSNumber *charNumber = [NSNumber numberWithChar:@‘a’] 简洁的多
B）NSArray/ NSDictionary 也一样。取值可直接使用下标，且如果数为nil时会崩溃，容易排查。（如果使用arrayWith/dictionaryWith 遇到第一个nil值后面的值就会忽略，影响会更大）
**3）多用类型常量，少用＃define预处理指令**
> 
A）例如：static const NSTimeInterval kAnimationDuration = 0.3
规范：实现文件中用k开头，类之外可见用类名为前缀。
注意点：一定要用static 和 const 两个关键字声明。static表明仅在定义此变量的编译单元中可见。如果不加static，编译器会为它创建一个外部符号。此时若是另一个编译单元也声明了同样的变量，则会出错。
B）如果需要对外公开这个变量，可以这样使用：
头文件   ：extern NSString *const MyTestNotification;
实现文件：NSString *const MyTestNotification = @“ThisMyTestNotification”
**4）用枚举表示状态／选项／状态码**
> 
A）可以借助系统的宏(具备向后兼容能力):
NS_ENUM(NSUIntefer, MyState){**, **, **};
NS_OPTIONS(NSUInteger, MyState){** =1<<0, **=1<<1};
注意：用switch语句的枚举，不要使用default分支。此时编译器会警告有的状态未处理。
**5）在对象内部尽量直接访问实例变量**
> 
A）在对象内部读取数据时，应该直接通过实例变量来读，而写入数据时，则应通过属性来写。
B）在初始化init和dealloc方法中，总是直接访问实例变量来读取数据。
**6）在自有类中使用关联对象存放自定义数据**
> 
A）可以通过关联对象机制把两个对象连接起来。
B）只有在其它做法不可行的时候才使用关联对象，因为关联对象容易引起难查的bug
**7）消息转发机制**
![843157-3235fd13f7d26e3a.png](https://upload-images.jianshu.io/upload_images/843157-3235fd13f7d26e3a.png)
**8）方法交换技术method swizzling**
> 
A）一般来说，只有调试程序的时候才需要在运行期间修改方法实现，这种做法不宜滥用。
**9）类对象**
> 
A）尽量使用类型信息查询方法(isKindOfClass)来确定对象类型，而不要直接比较类对象。因为某些对象可能实现了消息转发功能。
![843157-bc2d9b325312208f.png](https://upload-images.jianshu.io/upload_images/843157-bc2d9b325312208f.png)
### 接口与API设计
**1）使用前缀避免命名空间冲突**
> 
A）给类名加上适当的前缀，并且在所有代码中使用这一前缀。
B）若开发的程序中用到了第三方库，则应为其中的名称加上前缀。
**2）提供全能的初始化方法**
> 
A）在类中提供一个全能初始化方法，并于文档里指明。其它初始化方法均应调用此方法。
B）如果全能初始化方法和超类不同，则需要覆写超类中的对应方法。
C）如果超类的初始化方法不适合用于子类，那么应该覆写这个超类方法，并在其中抛出异常。
**3）尽量使用不可变对象**
> 
A）若属性仅可内部修改，则在分类中将readonly改成readwrite属性。
B）不要把可变的集合作为属性公开，而应该提供相关方法，以此修复对象中的可变集合属性。
**4）为私有方法加前缀名**
> 
A）不要单用下划线作为前缀。苹果公司框架内部使用的就是下划线，避免继承框架的类时覆盖超类的方法。
**5）OC的错误处理**
> 
A）只有发生可使整个应用程序崩溃的严重错误时，才应使用异常。
B）在错误不严重的情况下，可以指派代理来处理错误，也可以把错误信息放在NSError对象中，作为输出参数返回给调用者。
**6）理解NSCopying协议**
> 
A）实现copyWithZone:方法， 系统方法以应用程序唯一的“默认区”为参数(以前的程序内存有多个分区)
B）[NSMutableArray copy] => NSArray; [NSArray mutableCopy] => NSMutableArray
C）如果自定义的对象分为可变和不可变版本，需要同时实现NSCopying和NSMutableCopying协议
D）复制对象时需要决定使用深拷贝还是浅拷贝，一般情况下应该尽量执行浅拷贝。
E）如果所写的对象需要深拷贝，可以考虑新增一个专门执行深拷贝的方法。
### 协议与分类
**1）通过委托与数据源进行通信**
> 
A）若有必要，可实现含有位段的结构体，将委托对象是否能响应相关协议方法这一信息缓存至其中(如网络请求中通过代理方法传递数据进度)。
**2）将类的实现代码分散到各个分类中，便于管理**
> 
A）根据回溯信息的分类名称，便于调试。
B）应该将“私有”方法归入名叫Private的分类，以隐藏实现细节。
**3）向第三方类中添加分类时，应给其名称和其中 的方法名加上专用前缀。**
> 
A）为了避免覆盖，出现难以排查的bug。
**4）分类中的属性**
> 
A）把封装数据所用的全部属性都定义在主接口里。
B）分类中不推荐定义属性，因为分类无法合成与属性相关的实例变量。即使可以使用关联对象来解决问题，也不推荐。
**5）用分类隐藏实现细节**
> 
A）新增实例变量／只读扩展为读写／声明私有方法原型／遵守不为外部知道的协议
6）使用匿名对象
内存管理
**1）ARC**
> 
A）以alloc/new/copy/mutableCopy开头的方法，其返回的对像归调用者所有，返回创建对象时不会执行[object release]。
B）CoreFOundation对象不归ARC管理，所以应适当使用CFRetain/CFRelease
**2）dealloc用法**
> 
A）这个方法只用来释放其它对象的应用、取消KVO、通知，不要做其它事。
B）如果对象持有文件描述符等系统资源，则应该专门编写一个方法释放该资源。如和使用者约定，用完资源后必须调用close方法。
C）异步任务不要放在dealloc，正常状态下执行的方法也不要放在dealloc
D）对于开销较大的如套接字、文件描述符、大块内存等，应该单独实现另一个方法，当程序用完这些资源时就调用此方法。因为不能指望dealloc会在某个特定时期调用甚至不会调用，且在等到dealloc执行时，这些资源就会保留过长。
**3）异常**
> 
A）捕获对象时要将try中创立的对象清理干净。
B）默认情况下，ARC不生成处理异常所需要的代码，样板代码多、影响性能等副作用。开启编译标志-fobjc-arc-exceptions后，可以生成这种代码。
**4）保留环**
> 
A）A、B相互作为属性，其中之一应置weak。不建议使用unsafe_unretained，否则继续使用会崩溃。
**5）自动释放池**
> 
A）自动释放池排布在栈中，对象收到autorelease消息后，系统将其放入最顶端。
B）合理运用释放池可以降低应用程序的内存峰值。
C）@autoreleasepool这种新式写法能创建出更为轻便的自动释放池。
**6）僵尸对象**
> 
A）系统回收对象时，可以不将其真的收回，而是转化为僵尸对象。通过环境变量NSZombieEnabled可开启此功能。
B）系统会修改对象的isa指针，令其指向僵尸类(从而变为了僵尸对象)。僵尸类能够响应所有选择子，方式为打印一条包含消息内容及其接受者的消息，并终止程序。
### 块与大中枢派发
**1）块**
> 
A）块也是一个对象，块可以分配在栈、堆、全局的。栈上的离开创建区域之后会被销毁。
**2）用handler块降低代码分散程度**
> 
A）创建对象时，可以使用内联的handler块把相关业务逻辑一起声明。特别是有多个实例需要监控时，用handler可以直接将块与相关对象放在一起。
B）设计API时如果用到了handler块，那么可以增加一个参数，使调用者可通过此参数来决定应该把块安排在哪个队列上执行。
**3）多用派发队列，少用同步锁**
> 
A）派发队列可用来表述同步语意，这种做法比@synchronized块或NSLOck块对象更简单。例如：
   - (NSString *)someString{
            __block NSString *tempStr;
            dispatch_syns(_syncQueue, ^{ tempStr = _someString});
            return tempStr;
    }
B）将同步和异步派发结合起来，可实现与普通加锁机制一样的同步行为，而这么做却不会阻塞执行异步派发的线程。
C）使用同步队列及栅栏块，可以令同步行为更加高效
void dispatch_barrier_async(queue, block); void dispatch_barrier_sync(queue, block);
备注：barrier块必须单独执行，不能与其他block并行。这只对并发队列有意义。并发队列如果发现接下来要执行的block是个barrier block，那么就一直要等到当前所有并发的block都执行完毕，才会单独执行这个barrier block代码块，等到这个barrier block执行完毕，再继续正常处理其他并发block。
**4）多用GCD，少用performSelector系列方法**
> 
A） 在内存管理方面容易疏失。无法确定将要执行的选择子具体是什么，所以ARC无法插入适当的内存管理方法。
B）能处理的选择子过于局限，返回值类型以及参数都收到限制。
C）如果想把任务放在另一个线程上，最好不要用 performSelector，应该封装到块里。
**6）适当选择GCD和操作队列的使用时机**
> 
A）操作队列实现了高层的OC API，能实现一些复杂的功能和操作，如
取消操作、指定操作间的依赖关系、通过键值观测监控NSOperation对象的属性、指定优先级、重用NSOperation对象等。GCD主要针对整个队列间，操作队列主要针对队列中的每个块。
**7）通过dispatch Group机制，根据系统资源状况来执行任务**
> 
A）一系列任务可归入一个dispatch group中，开发者可在这组任务执行完毕获得通知。
相关API：
dispatch_group_create();         // 创建组
dispatch_group_async(group, queue, block);  // 在组中异步执行队列中的block
dispatch_group_enter(group);  // 加入组
dispatch_group_leave(group);   //和enter成对出现，否则改group的任务永远执行不完
dispatch_group_wait(group, timeout);    // 等待组。DISPATCH_TIME_FOREVER 表示永久等待
dispatch_group_notify(group, queue, block);   // 组中任务执行完毕，会执行block
dispatch_apply(count, queue, block(size));   // 重复执行block共count次，是一个持续阻塞函数，直到任务执行完(可在异步dispatch_async中调用该方法)
**8）使用dispatch_once来执行只需要运行一次的线程安全代码**
> 
dispatch_once(token, block);
A）注意标记token应该声明在static或global中。
**9）不要使用dispatch_get_current_queue**
> 
A）该函数的行为常常与开发者所预期不同，因此iOS6.0已废弃，只能做调试使用。
B）该函数用于解决由不可重入的代码所引发的死锁。可以用标记队列dispatch_queue_set_specific、递归锁NSRecursiveLock来解决。
### 熟悉系统框架
**1）多用块枚举，少用for循环**
> 
A）遍历collection有四种方式。最基本的办法是for循环，其次是NSEnumerator遍历法及快速遍历法，最新最先进的是“块枚举法”。
B）“块枚举”本身通过GCD来并发执行遍历操作。
**2）对自定义内存管理语义的collection使用无缝桥接**
> 
A）在CoreFoundation层面创建collection时，可以指定许多回调函数，这些函数表示此collection应该如何处理其函数。运用无缝桥接，可实现特殊内存管理语义。例如可以使OC中的字典实现键值均保留。
**3）构建缓存时选用NSCache，而不是NSDictionary**
> 
A）好处之一是系统资源将要耗尽时，NSCache可以自动删减缓存。且线程安全，保留键值。
B）可以给NSCache对象设置上线，用以限制缓存中的对象个数以及总成本。但这些限制并不可靠，NSCache只是”参考”。
C）将NSPurgeableData(NSMutableData的子类)与NSCache搭配使用，可实现自动清除数据的功能。即当NSPurgeableData对象所占内存为系统丢弃时，该对象自身也可以从缓存中移除。
D）只有那种“重新计算起来很费事”的数据，才值得放入缓存
**4）精简initialize与load的实现代码**
> 
A）运行期的类和分类，必定会调用+load方法，且只调用一次。
B）给定的程序库，无法判断各个类的载入顺序，因此，在+load方法中使用其它类是不安全的，因为不确定这个其它类是否已经载入了。程序执行+load方法时都会阻塞。
C）+load方法不像普通方法，它不遵循继承规则。如果某个类未实现+load方法，那么它的超类不管是否实现了该方法，都不会被调用。类的+load会比分类的+load先执行。
D）+load的真正用途在于调试程序。时下编写OC代码，不需要使用它。
E）+initialize 属于懒加载，首次使用该类之前调用，且只调用一次。在+initialize中可以放心调用任意类的任意方法。如果某个类未实现它，但其超类实现了，那么就会运行超类的实现代码。
F ）无法在编译期设定的全局常量，可以放在initialize方法里初始化。如全局可变数组，单例使用前需要执行某些操作。(PS:使用时，用if(self == [MyClass class])做一下判断，否则可能会执行多变)
**5）别忘了 NSTimer会保留其目标对象**
> 
A）可以扩充NSTimer的功能，用”块”打破保留环。
