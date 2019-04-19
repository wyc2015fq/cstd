# iOS逆向-免费送你百度网盘超级会员 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2018年09月26日 10:02:32[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：687
![](https://upload-images.jianshu.io/upload_images/2264397-d6ffdacde16e8f60.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/800/format/webp)
iOS逆向
基于百度网盘最新版本8.9.1，学习逆向，增加技能经验。
学习过程中遇到的问题总结一下。
### 如何找到block的实现
在[Block-ABI](http://clang.llvm.org/docs/Block-ABI-Apple.html)中定义的block结构体为
```
struct Block_literal_1 {
    void *isa; // initialized to &_NSConcreteStackBlock or &_NSConcreteGlobalBlock
    int flags;
    int reserved;
    void (*invoke)(void *, ...);
    struct Block_descriptor_1 {
    unsigned long int reserved;         // NULL
        unsigned long int size;         // sizeof(struct Block_literal_1)
        // optional helper functions
        void (*copy_helper)(void *dst, void *src);     // IFF (1<<25)
        void (*dispose_helper)(void *src);             // IFF (1<<25)
        // required ABI.2010.3.16
        const char *signature;                         // IFF (1<<30)
    } *descriptor;
    // imported variables
};
```
- isa是x24寄存器，指向_NSConcreteStackBlock或_NSConcreteGlobalBlock的地址。占8个字节。
- flags和reserved个占4个字节。
- invoke就是指向block实现的指针。在block真实地址偏移16个字节处。
- Block_descriptor_1为block的描述，包括block的大小及签名等。
比如以下方法的参数arg5就是一个block
```
typedef void(^CDUnknownBlockType)(void);
@interface UIManager
- (void)openZipListFileMeta:(id)arg1 isLocal:(_Bool)arg2 navigationContrloller:(id)arg3 zipfileListDoneBlock:(CDUnknownBlockType)arg4 guideBuyVipDoneBlock:(CDUnknownBlockType)arg5;
@end
%hook UIManager
- (void)openZipListFileMeta:(id)arg1 isLocal:(_Bool)arg2 navigationContrloller:(id)arg3 zipfileListDoneBlock:(CDUnknownBlockType)arg4 guideBuyVipDoneBlock:(CDUnknownBlockType)arg5{
    %orig;
}
%end
```
想看block的实现，通过llvm下断点，断到以该block为参数的方法上。
首先在ida中找到该方法的偏移地址为0x1002C5F00：
![](https://upload-images.jianshu.io/upload_images/2264397-17c00048cc2340bc.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
获取偏移地址
然后通过`im li -o -f`获得ASLR偏移(地址空间布局随机化)，为0x68000：
![](https://upload-images.jianshu.io/upload_images/2264397-166d89b3fae0cb82.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
ASLR偏移
最后下断点`br s -a 0x0000000000068000+0x1002C5F00`（也可直接在xcode中下断点更方便）
查看断点处的汇编代码比较与ida中的是否一致：
![](https://upload-images.jianshu.io/upload_images/2264397-1ba35b46b2ff2bea.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
image.png
按c继续。当断点触发后，`po (char *)$x1`,再次验证断点处的方法名，发现断点下正确。
查看参数arg5：`po $x6`,则0x102aee0c0即为参数arg5的地址，也就是需要查找的block的地址。
查找block实现的地址：`memory read --size 8 --format x 0x102aee0c0`，block的地址偏移16位即block的实现的地址：0x000000010058dbec。
此时即可查看该block实现的汇编代码了`dis -a 0x000000010058dbec`。
![](https://upload-images.jianshu.io/upload_images/2264397-e01d0a8316351afc.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
查找block的实现
在xcode中不好分析，可去ida中分析block的实现。0x000000010058dbec-0x68000=0x100525BEC
打开ida，按G跳转到0x100525BEC，发现和xcode中的汇编代码一致。查找成功。
![](https://upload-images.jianshu.io/upload_images/2264397-59ee3c170bc79eca.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
image.png
### 如何hook block
按fn+f5,将汇编代码转换为伪代码，分析block的参数和返回值。
![](https://upload-images.jianshu.io/upload_images/2264397-f843813d86e2a4ad.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
伪代码
发现参数和返回值都为空。即可自定义一个block，再将自定义block当做参数传入原始方法
```
- (void)openZipListFileMeta:(id)arg1 isLocal:(_Bool)arg2 navigationContrloller:(id)arg3 zipfileListDoneBlock:(CDUnknownBlockType)arg4 guideBuyVipDoneBlock:(CDUnknownBlockType)arg5{
    
    void (^customBlock)(void) = ^{
        NSLog(@"hook block");
    };
    
    %orig(arg1, arg2, arg3, arg4, customBlock);
}
```
### 如何调用其它类
- 使用Logos语法
	%c ：该指令的作用等同于objc_getClass或NSClassFromString,即动态获取一个类的定义，在%hook或％ctor内使用 。
```
NSString *userId = [%c(BDUser) userId];
```
- 使用CaptainHook
它既不需要语法转换，也不依赖CydiaSubstrate动态库，其利用的是OC的runtime特性。可以参考[wiki](https://github.com/rpetrich/CaptainHook/wiki)。
```
NSString *userId = ((id (*)(id, SEL))objc_msgSend)(objc_getClass("BDUser"),@selector(userId));
```
### 最终效果如下
- 
极速下载
![](https://upload-images.jianshu.io/upload_images/2264397-4b432d6fb2c97eb2.jpeg?imageMogr2/auto-orient/strip%7CimageView2/2/w/750/format/webp)
极速下载
- 
倍速播放
![](https://upload-images.jianshu.io/upload_images/2264397-49642507c3c1abc2.jpeg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1000/format/webp)
倍速播放
- 
尊贵标识
![](https://upload-images.jianshu.io/upload_images/2264397-26ac82a34269f819.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/562/format/webp)
尊贵标识
### 送你超级会员
百度网盘iPhone客户端，享有超级会员特权，可以极速下载，倍速播放，尊贵标识，无法在线解压.
代码已提交[LTBaiduNetDiskiPhonePod](https://github.com/AloneMonkey/MonkeyDevSpecs)，快去集成试试吧。
[源码在这](https://github.com/1061961387/LTBaiduNetDiskiPhone)喜欢的小伙伴可以关注下。
作者：iOS_Latte
链接：https://www.jianshu.com/p/2bac0f91595f
來源：简书
简书著作权归作者所有，任何形式的转载都请联系作者获得授权并注明出处。
