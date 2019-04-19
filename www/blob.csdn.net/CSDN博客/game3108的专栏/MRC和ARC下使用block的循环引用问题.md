# MRC和ARC下使用block的循环引用问题 - game3108的专栏 - CSDN博客
2016年03月30日 14:59:31[game3108](https://me.csdn.net/game3108)阅读数：357
## 摘要
block虽然好用，但是里面也有不少坑，最大的坑莫过于`循环引用`问题。稍不注意，可能就会造成内存泄漏。这节，我将从源码的角度来分析造成`循环引用`问题的根本原因。并解释变量前加`__block`，和`__weak`的区别。
## 一个循环引用问题
下面我们来看下下面的代码，这样写会出什么问题？
```
typedef void (^blk_t)(void);
@interface MyObject : NSObject
{
    blk_t blk_;
} @end
@implementation MyObject
- (id)init
{
    self = [super init];
    blk_ = ^{NSLog(@"self = %@", self);}; 
    return self;
}
- (void)dealloc
{
    NSLog(@"dealloc");
    Block_release(blk_);
    [super dealloc];/*在ARC环境中删除该行代码*/
} @end
int main()
{
    id o = [[MyObject alloc] init];
    NSLog(@"%@", o);
    [o release];/*在ARC环境中删除该行代码*/
    return 0;
}
```
### 在MRC环境下
```
struct __block_impl {
    void *isa;
    int Flags;
    int Reserved;
    void *FuncPtr;
};
struct __MyObject__init_block_impl_0 {
    struct __block_impl impl;
    struct __MyObject__init_block_desc_0* Desc;
    MyObject *self;
    __MyObject__init_block_impl_0(void *fp, struct __MyObject__init_block_desc_0 *desc, MyObject *_self, int flags=0) : self(_self) {
        impl.isa = &_NSConcreteStackBlock;
        impl.Flags = flags;
        impl.FuncPtr = fp;
        Desc = desc;
    }
};
static void __MyObject__init_block_func_0(struct __MyObject__init_block_impl_0 *__cself) {
    MyObject *self = __cself->self; // bound by copy
    NSLog((NSString*)&__NSConstantStringImpl__var_folders_rv_5xtfn15d3nl5g0z1csq3zch80000gn_T_MyObject_80a1b7_mi_0, self);
}
static void __MyObject__init_block_copy_0(struct __MyObject__init_block_impl_0*dst, struct __MyObject__init_block_impl_0*src){
    _Block_object_assign((void*)&dst->self, (void*)src->self, 3/*BLOCK_FIELD_IS_OBJECT*/);
}
static void __MyObject__init_block_dispose_0(struct __MyObject__init_block_impl_0*src) {
    _Block_object_dispose((void*)src->self, 3/*BLOCK_FIELD_IS_OBJECT*/);
}
static struct __MyObject__init_block_desc_0 {
    size_t reserved;
    size_t Block_size;
    void (*copy)(struct __MyObject__init_block_impl_0*, struct __MyObject__init_block_impl_0*);
    void (*dispose)(struct __MyObject__init_block_impl_0*);
} __MyObject__init_block_desc_0_DATA = { 0, sizeof(struct __MyObject__init_block_impl_0), __MyObject__init_block_copy_0, __MyObject__init_block_dispose_0};
static id _I_MyObject_init(MyObject * self, SEL _cmd) {
    self = ((MyObject *(*)(__rw_objc_super *, SEL))(void *)objc_msgSendSuper)((__rw_objc_super){ (id)self, (id)class_getSuperclass(objc_getClass("MyObject")) }, sel_registerName("init"));
    (*(blk_t *)((char *)self + OBJC_IVAR_$_MyObject$blk_)) = (void (*)())&__MyObject__init_block_impl_0((void *)__MyObject__init_block_func_0, &__MyObject__init_block_desc_0_DATA, self, 570425344);
    return self;
}
...
```
[上一节](http://blog.itanbing.com/2015/07/18/block-implement-principle/)研究了block的实现原理，在这就不再一一介绍。有兴趣的朋友可以看下[这部分](http://blog.itanbing.com/2015/07/18/block-implement-principle/)。
首先我们看下MyObject.m中`init`函数中的`blk_ = ^{NSLog(@"self = %@", self);};`。它被转换成了
`(*(blk_t *)((char *)self + OBJC_IVAR_$_MyObject$blk_)) = (void (*)())&__MyObject__init_block_impl_0((void *)__MyObject__init_block_func_0, &__MyObject__init_block_desc_0_DATA, self, 570425344);`
`570425344`为`(1 << 25 | 1 << 29)`，它在[Block_private.h](http://opensource.apple.com/source/clang/clang-137/src/projects/compiler-rt/BlocksRuntime/Block_private.h)声明，为
```
BLOCK_HAS_COPY_DISPOSE
 ｜ BLOCK_HAS_DESCRIPTOR
```
。这个数被赋值给`__block_impl`的`Flags`。给这个是干什么的呢？后面会讲到，先放着不管。
接着我们看下`blk_ = ^{NSLog(@"self = %@", self);};`，它做了什么呢？blk_是MyObject的成员变量，因为MyObject对象在堆上，blk_也在堆上。`^{NSLog(@"self = %@", self);}`这个block在栈上生成，因为赋值给堆上的blk_，所以会被隐式地copy到堆上。没错，它就调用了_Block_copy(...)方法。这有个知识点，什么情况下栈上的block会隐式地进行copy操作。
- block被赋值到堆上的block变量
- 在ARC环境下，block被赋值给__strong属性标记的block变量
- 在ARC环境下，block被当作返回block时
从[上一节](http://blog.itanbing.com/2015/07/18/block-implement-principle/)我们知道，block的copy其实调用的
```
/* Copy, or bump refcount, of a block.  If really copying, call the copy helper if present. */
static void *_Block_copy_internal(const void *arg, const int flags) {
    ...
    if (result->flags & BLOCK_HAS_COPY_DISPOSE) {
        //printf("calling block copy helper %p(%p, %p)...\n", aBlock->descriptor->copy, result, aBlock);
        (*aBlock->descriptor->copy)(result, aBlock); // do fixup
        ...
        return result;
    }
}
```
上面的result就是我们要copy的block，我们知道block的flags被赋值为`BLOCK_HAS_COPY_DISPOSE ｜ BLOCK_HAS_DESCRIPTOR`。来看下copy方法
`(*aBlock->descriptor->copy)(result, aBlock);`
实现
```
static void __MyObject__init_block_copy_0(struct __MyObject__init_block_impl_0*dst, struct __MyObject__init_block_impl_0*src){
    _Block_object_assign((void*)&dst->self, (void*)src->self, 3/*BLOCK_FIELD_IS_OBJECT*/);
}
```
`3`为`BLOCK_FIELD_IS_OBJECT`，这个也可以从[Block_private.h](http://opensource.apple.com/source/clang/clang-137/src/projects/compiler-rt/BlocksRuntime/Block_private.h)看到.
`__MyObject__init_block_copy_0`调用了[runtime.c](http://opensource.apple.com/source/clang/clang-137/src/projects/compiler-rt/BlocksRuntime/runtime.c)中的
```
void _Block_object_assign(void *destAddr, const void *object, const int flags) {
    //printf("_Block_object_assign(*%p, %p, %x)\n", destAddr, object, flags);
    if ((flags & BLOCK_BYREF_CALLER) == BLOCK_BYREF_CALLER) {
        if ((flags & BLOCK_FIELD_IS_WEAK) == BLOCK_FIELD_IS_WEAK) {
            _Block_assign_weak(object, destAddr);
        }
        else {
            // do *not* retain or *copy* __block variables whatever they are
            _Block_assign((void *)object, destAddr);
        }
    }
    else if ((flags & BLOCK_FIELD_IS_BYREF) == BLOCK_FIELD_IS_BYREF)  {
        // copying a __block reference from the stack Block to the heap
        // flags will indicate if it holds a __weak reference and needs a special isa
        _Block_byref_assign_copy(destAddr, object, flags);
    }
    // (this test must be before next one)
    else if ((flags & BLOCK_FIELD_IS_BLOCK) == BLOCK_FIELD_IS_BLOCK) {
        // copying a Block declared variable from the stack Block to the heap
        _Block_assign(_Block_copy_internal(object, flags), destAddr);
    }
    // (this test must be after previous one)
    else if ((flags & BLOCK_FIELD_IS_OBJECT) == BLOCK_FIELD_IS_OBJECT) {
        //printf("retaining object at %p\n", object);
        _Block_retain_object(object);
        //printf("done retaining object at %p\n", object);
        _Block_assign((void *)object, destAddr);
    }
}
```
flags传进来的为`BLOCK_FIELD_IS_OBJECT`，执行了
```
void _Block_object_assign(void *destAddr, const void *object, const int flags) {
    ...
    // (this test must be after previous one)
    else if ((flags & BLOCK_FIELD_IS_OBJECT) == BLOCK_FIELD_IS_OBJECT) {
        //printf("retaining object at %p\n", object);
        _Block_retain_object(object);
        //printf("done retaining object at %p\n", object);
        _Block_assign((void *)object, destAddr);
    }
}
```
我们看到`_Block_retain_object(object);`，它表明被捕获的MyObject对象被retain，也就是被持有了，也就是MyObject对象被持有了两次（一次`init`，一次被blk_变量持有），在MyObject对象调用release时，引用计数－1变为1。所以MyObject对象不会调用`dealloc`方法，而blk_变量是在`dealloc`释放的，也就是，blk_不会被释放，那blk_持有的MyObject对象也不会被释放。这样便造成了内存泄漏。
为了解决这个问题，在变量加个`__block`标记。我们再来看下，当加了block后转换的代码
struct __block_impl {
void *isa;int Flags;int Reserved;void *FuncPtr;
};
```
struct __Block_byref_weakSelf_0 {   /*增加*/
    void *__isa;
    __Block_byref_weakSelf_0 *__forwarding;
    int __flags;
    int __size;
    void (*__Block_byref_id_object_copy)(void*, void*);
    void (*__Block_byref_id_object_dispose)(void*);
    typeof (self) weakSelf;
};
static void __Block_byref_id_object_copy_131(void *dst, void *src) {    /*增加*/
    _Block_object_assign((char*)dst + 40, *(void * *) ((char*)src + 40), 131);
}
static void __Block_byref_id_object_dispose_131(void *src) {    /*增加*/
    _Block_object_dispose(*(void * *) ((char*)src + 40), 131);
}
struct __MyObject__init_block_impl_0 {
    struct __block_impl impl;
    struct __MyObject__init_block_desc_0* Desc;
    __Block_byref_weakSelf_0 *weakSelf; // by ref
    __MyObject__init_block_impl_0(void *fp, struct __MyObject__init_block_desc_0 *desc, __Block_byref_weakSelf_0 *_weakSelf, int flags=0) : weakSelf(_weakSelf->__forwarding) {
        impl.isa = &_NSConcreteStackBlock;
        impl.Flags = flags;
        impl.FuncPtr = fp;
        Desc = desc;
    }
};
static void __MyObject__init_block_func_0(struct __MyObject__init_block_impl_0 *__cself) {
    __Block_byref_weakSelf_0 *weakSelf = __cself->weakSelf; // bound by ref
    NSLog((NSString *)&__NSConstantStringImpl__var_folders_rv_5xtfn15d3nl5g0z1csq3zch80000gn_T_MyObject_2fcc35_mi_0, (weakSelf->__forwarding->weakSelf));
}
static void __MyObject__init_block_copy_0(struct __MyObject__init_block_impl_0*dst, struct __MyObject__init_block_impl_0*src) {
    _Block_object_assign((void*)&dst->weakSelf, (void*)src->weakSelf, 8/*BLOCK_FIELD_IS_BYREF*/);
}
static void __MyObject__init_block_dispose_0(struct __MyObject__init_block_impl_0*src) {
    _Block_object_dispose((void*)src->weakSelf, 8/*BLOCK_FIELD_IS_BYREF*/);
}
static struct __MyObject__init_block_desc_0 {
    size_t reserved;
    size_t Block_size;
    void (*copy)(struct __MyObject__init_block_impl_0*, struct __MyObject__init_block_impl_0*);
    void (*dispose)(struct __MyObject__init_block_impl_0*);
} __MyObject__init_block_desc_0_DATA = { 0, sizeof(struct __MyObject__init_block_impl_0), __MyObject__init_block_copy_0, __MyObject__init_block_dispose_0};
static id _I_MyObject_init(MyObject * self, SEL _cmd) {
    self = ((MyObject *(*)(__rw_objc_super *, SEL))(void *)objc_msgSendSuper)((__rw_objc_super){ (id)self, (id)class_getSuperclass(objc_getClass("MyObject")) }, sel_registerName("init"));
    __attribute__((__blocks__(byref))) __Block_byref_weakSelf_0 weakSelf = {(void*)0,(__Block_byref_weakSelf_0 *)&weakSelf, 33554432, sizeof(__Block_byref_weakSelf_0), __Block_byref_id_object_copy_131, __Block_byref_id_object_dispose_131, self};
    (*(blk_t *)((char *)self + OBJC_IVAR_$_MyObject$blk_)) = (void (*)())&__MyObject__init_block_impl_0((void *)__MyObject__init_block_func_0, &__MyObject__init_block_desc_0_DATA, (__Block_byref_weakSelf_0 *)&weakSelf, 570425344);
    return self;
}
```
我们只看下不同的地方。和不加__block多了以下部分
```cpp
```cpp
struct __Block_byref_weakSelf_0 {
    void *__isa;
    __Block_byref_weakSelf_0 *__forwarding;
    int __flags;
    int __size;
    void (*__Block_byref_id_object_copy)(void*, void*);
    void (*__Block_byref_id_object_dispose)(void*);
    typeof (self) weakSelf;
};
static void __Block_byref_id_object_copy_131(void *dst, void *src) {
    _Block_object_assign((char*)dst + 40, *(void * *) ((char*)src + 40), 131);
}
static void __Block_byref_id_object_dispose_131(void *src) {
    _Block_object_dispose(*(void * *) ((char*)src + 40), 131);
}
```
```
和[持有普通类型变量](http://blog.itanbing.com/2015/07/18/block-implement-principle/)不同，object的copy需要管理引用计数，比[持有普通类型变量](http://blog.itanbing.com/2015/07/18/block-implement-principle/)多了copy，和dispose函数。等下我们再分析，先往下看。
`__block typeof(self) weakSelf = self;`转化的代码为 *attribute*((**blocks**(byref)))
**Block_byref_weakSelf_0 weakSelf = {(void*)0,(**Block_byref_weakSelf_0 *)&weakSelf, 33554432, sizeof(**Block_byref_weakSelf_0),**Block_byref_id_object_copy_131, __Block_byref_id_object_dispose_131, self};
去掉强转代码后
```cpp
```cpp
__Block_byref_weakSelf_0 weakSelf = {(void*)0,&weakSelf, 33554432, sizeof(__Block_byref_weakSelf_0), __Block_byref_id_object_copy_131, __Block_byref_id_object_dispose_131, self};
```
```
- 33554432为`1 << 25`，即`BLOCK_HAS_COPY_DISPOSE`。
- `__Block_byref_id_object_copy`方法被赋值为`__Block_byref_id_object_copy_131`。
我们再从最开始捋一遍。首先block被拷贝到堆上，这时调用__Block_copy函数。block的flags为570425344`BLOCK_HAS_COPY_DISPOSE ｜ BLOCK_HAS_DESCRIPTOR`。
```
/* Copy, or bump refcount, of a block.  If really copying, call the copy helper if present. */
static void *_Block_copy_internal(const void *arg, const int flags) {
    ...
    if (result->flags & BLOCK_HAS_COPY_DISPOSE) {
        //printf("calling block copy helper %p(%p, %p)...\n", aBlock->descriptor->copy, result, aBlock);
        (*aBlock->descriptor->copy)(result, aBlock); // do fixup
        ...
        return result;
    }
}
```
`aBlock->descriptor->copy`调用
```cpp
```cpp
static void __MyObject__init_block_copy_0(struct __MyObject__init_block_impl_0*dst, struct __MyObject__init_block_impl_0*src) {
    _Block_object_assign((void*)&dst->weakSelf, (void*)src->weakSelf, 8/*BLOCK_FIELD_IS_BYREF*/);
}
```
```
标记下，`_Block_object_assign`的参数`src`为含有捕获的对象weakSelf的结构体`__Block_byref_weakSelf_0`。
```
void _Block_object_assign(void *destAddr, const void *object, const int flags) {
    ...
    else if ((flags & BLOCK_FIELD_IS_BYREF) == BLOCK_FIELD_IS_BYREF)  {
        // copying a __block reference from the stack Block to the heap
        // flags will indicate if it holds a __weak reference and needs a special isa
        _Block_byref_assign_copy(destAddr, object, flags);
    }
    ...
}
```
flags值为`BLOCK_HAS_COPY_DISPOSE`。所以
```
static void _Block_byref_assign_copy(void *dest, const void *arg, const int flags) {
    struct Block_byref **destp = (struct Block_byref **)dest;
    struct Block_byref *src = (struct Block_byref *)arg;
    ...
    else if ((src->forwarding->flags & BLOCK_REFCOUNT_MASK) == 0) {
        //printf("making copy\n");
    // src points to stack
        bool isWeak = ((flags & (BLOCK_FIELD_IS_BYREF|BLOCK_FIELD_IS_WEAK)) == (BLOCK_FIELD_IS_BYREF|BLOCK_FIELD_IS_WEAK));
        // if its weak ask for an object (only matters under GC)
        struct Block_byref *copy = (struct Block_byref *)_Block_allocator(src->size, false, isWeak);
        copy->flags = src->flags | _Byref_flag_initial_value; // non-GC one for caller, one for stack
        copy->forwarding = copy; // patch heap copy to point to itself (skip write-barrier)
        src->forwarding = copy;  // patch stack to point to heap copy
        copy->size = src->size;
        if (src->flags & BLOCK_HAS_COPY_DISPOSE) {
            // Trust copy helper to copy everything of interest
            // If more than one field shows up in a byref block this is wrong XXX
            copy->byref_keep = src->byref_keep;
            copy->byref_destroy = src->byref_destroy;
            (*src->byref_keep)(copy, src);
        }
        ...
    }
    ...
}
```
(*src->byref_keep)(copy, src);调用`__Block_byref_weakSelf_0`的`__Block_byref_id_object_copy`方法，即
```cpp
```cpp
static void __Block_byref_id_object_copy_131(void *dst, void *src) {
    _Block_object_assign((char*)dst + 40, *(void * *) ((char*)src + 40), 131);
}
```
```
131为`BLOCK_FIELD_IS_OBJECT (3) |BLOCK_BYREF_CALLER（128）`
```
void _Block_object_assign(void *destAddr, const void *object, const int flags) {
    //printf("_Block_object_assign(*%p, %p, %x)\n", destAddr, object, flags);
    if ((flags & BLOCK_BYREF_CALLER) == BLOCK_BYREF_CALLER) {
        if ((flags & BLOCK_FIELD_IS_WEAK) == BLOCK_FIELD_IS_WEAK) {
            _Block_assign_weak(object, destAddr);
        }
        else {
            // do *not* retain or *copy* __block variables whatever they are
            _Block_assign((void *)object, destAddr);
        }
    }
    ...
}
```
当在MRC环境时，直接复制，并不会retain捕获的对象。所以在MRC环境下，__block可以消除`循环引用`。
### 在ARC环境下
在ARC环境下，在不加**block的情况下，也会出现`循环引用`。但是在加上**block后，仍然无法消除`循环引用`。我们来看下
`__block typeof(self) weakSelf = self`，在ARC下其实为`__block __strong typeof(self) weakSelf = self;`转换后，如下
```
struct __Block_byref_weakSelf_0 {
    void *__isa;
    __Block_byref_weakSelf_0 *__forwarding;
    int __flags;
    int __size;
    void (*__Block_byref_id_object_copy)(void*, void*);
    void (*__Block_byref_id_object_dispose)(void*);
    __strong typeof (self) weakSelf;
};
struct __MyObject__init_block_impl_0 {
    struct __block_impl impl;
    struct __MyObject__init_block_desc_0* Desc;
    __Block_byref_weakSelf_0 *weakSelf; // by ref
    __MyObject__init_block_impl_0(void *fp, struct __MyObject__init_block_desc_0 *desc, __Block_byref_weakSelf_0 *_weakSelf, int flags=0) : weakSelf(_weakSelf->__forwarding) {
        impl.isa = &_NSConcreteStackBlock;
        impl.Flags = flags;
        impl.FuncPtr = fp;
        Desc = desc;
    }
};
```
weakSelf(_weakSelf->__forwarding)，即
`__strong typeof (self) weakSelf ＝ weakSelf;`
`__strong`为强引用，所以即使加了`__block`捕获的对象self仍然会被retain。
解决方法，加上`__weak`标记，即`__weak typeof(self) weakSelf = self`，这样self就不会被持有了。
## 小节
通过源码分析，我们可以了解使用`block`引起`循环引用`的原因，以及解决方案，理解`__block`和`__weak`的作用。
文／没事蹦蹦（简书作者）
原文链接：http://www.jianshu.com/p/1383d56a7ca3
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
