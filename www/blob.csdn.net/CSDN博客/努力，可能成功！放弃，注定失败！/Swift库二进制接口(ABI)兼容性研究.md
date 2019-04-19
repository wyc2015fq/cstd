# Swift库二进制接口(ABI)兼容性研究 - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年05月09日 11:26:31[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：843
## 本文来自简书，原文地址:[http://www.jianshu.com/p/5860f5542f21](http://www.jianshu.com/p/5860f5542f21)
## 前言
阿里云APP组件化过程中，我们拆分出了若干基础组件库和业务代码库，由于代码是采用Swift编写的，所以这些库都是动态库形式。在上一个正式版本，组件化达到了完全形态，主工程只剩下一个壳，所有代码都以pod组件的形式引入，最后交付的是一个主工程的壳二进制程序，和十几个动态库Framework。这些动态库都是运行时才链接到主程序中的。
开发中，我们在某个组件上工作时，其他组件也都是以动态库二进制形式引入的，节省Rebuild源码成本。但开发过程中我们遇到了一些问题。
## 遇到问题
组件拆分出来以后，无可避免，组件之间会存在一些非扁平的依赖关系，如A依赖B依赖C，同时A依赖C。开发A过程中，可能有需要改动C的情况，于是我们将C更新、打包、发布，而B保持不变(只有A用到C加的特性)，A依赖C的新版本继续开发。但我们发现，有一些改动会导致APP闪退，只有在B重新依赖C的新版本也编译、打包、发布新版本之后，才恢复正常。我们注意到，出现这种情况时，一般是我们改动了A中某些类的属性。显而易见，我们遇到的是[二进制兼容性问题](https://zh.wikipedia.org/wiki/%E5%BA%94%E7%94%A8%E4%BA%8C%E8%BF%9B%E5%88%B6%E6%8E%A5%E5%8F%A3)。
理论上ObjC使用静态库也会遇到这种问题的，但是ObjC2.0引入了[non-fragile](http://quotation.github.io/objc/2015/05/21/objc-runtime-ivar-access.html)特性，同时runtime中采用消息转发实现方法调用，规避了大部分会引起ABI不兼容的情况。所以在ObjC中，只要接口兼容，底层组件改动一般是不需要自下往上重新编译链接的。
而Swift，在我们认识里，它没有ObjC那么“动态”，又不像C/C++那么直接(寻址是基地址+偏移量，内存布局改变引起ABI不兼容)，那它会存在哪些能引起ABI不兼容的情况呢？上个版本我们没有弄明白，保险起见，底层改动后我们都把上层重新编译连接，非常蛋疼。所以，这两天我尝试把这个问题弄清楚。
## 复现实例
我们准备三个代码文件：
```
// Foo.swift
public class SimpleClass {
    public var x: Int = 0
    public var y: Int = 0
    public init() {
        x = 1
        y = 2
    }
    public func sum() -> Int {
        return x + y
    }
}
```
```
// Bar.swift
import Foo
public func bar() -> Int {
    return SimpleClass().sum()
}
```
```
// main.swift
import Foundation
import Foo
import Bar
print("bar \(bar())")
```
然后分别编译出动态库，链接到main，运行：
```bash
$ swiftc -emit-module -emit-library Foo.swift
$ swiftc -emit-module -emit-library Bar.swift -I. -L. -lFoo
$ swiftc -I. -L. -lFoo -lBar main.swift
$ ./main
bar 3 // output
```
然后我们给`Foo.swift`增加一个字段diff:
```
// Foo.swift
public class SimpleClass {
    public var diff: Int?
    public var x: Int = 0
    public var y: Int = 0
    public init() {
        x = 1
        y = 2
    }
    public func sum() -> Int {
        return x + y
    }
}
```
单独编译`Foo.swift`，`Bar.swift`和`main.swift`不再重新编译:
```bash
$ swiftc -emit-module -emit-library Foo.swift
$ ./main
[1]    52482 segmentation fault  ./main  // output
$ swiftc -I. -L. -lFoo -lBar main.swift
[1]    52645 segmentation fault  ./main  // output
```
可以看到，无论是否重新编译main，只要不重新编译`Bar`，运行main就会闪退。
## 排查
我们没有挑选一些在C/C++里容易理解的ABI不兼容的场景，如直接引用发生变化的对象成员变量，继承关系下父类字段增删等。上述复现实例中，我们在`SimpleClass`这个类中增加了字段，改变了对象的内存布局，但是，按照C/C++的思路，我们并未在`Bar`中直接引用任何成员变量，为什么会出现错误呢？我们也已经知道，Swift方法调用依赖`vtable`，但我们也并未对实例方法做增删。
还是要从内存布局说起。
### 对象内存布局
Swift对象的内存布局我也是这两天慢慢明白过来的，现在放在最前头，后边说起来就容易理解了。在上一次我为了实现[`HandyJSON`](https://github.com/alibaba/handyjson)做这方面调研的时候，这一块资料很少，但现在情况不同了。特别推荐Mike
 Ash的一个[分享视频](https://www.youtube.com/watch?v=ERYNyrfXjlg&feature=youtu.be)，他的思路很棒，主要依赖`mach_vm_read_overwrite`和`dladder`两个系统函数。前者利用系统调用检查一个指针是否合法，后者检查一个内存地址上是否绑定了什么symbol。基于此，从一个对象的起始指针出发，广度遍历一下可见的内存，构造指针继续往下，最后将得到和这个对象相关的所有地址、所有符号，把这些信息生成一张图，最后大概是这个样子(这个是简单情况)：
![](http://upload-images.jianshu.io/upload_images/1564920-e4faaa90d78332c1.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
从这个视频，我们知道，普通Swift对象，即使是纯Swift对象，其实instance的布局也是和ObjC保持一致，大概是这样：
![](http://upload-images.jianshu.io/upload_images/1564920-6471af6323a3f9be.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
我们打开Swift ABI文档。在[Class Metadata](https://github.com/apple/swift/blob/master/docs/ABI.rst#class-metadata)这一章节，我们看到，在ObjC中，`isa`指向一个Class类对象，Swift类似，但指向的是一个与Class类兼容的metadata，这个metadata用
```
Class
 *
```
去解释是没有问题的，同时它尾部追加了Swift中使用的一些字段。比如`vtable`。`vtable`相关的资料，可以参考:[Swift
 Method Dispatching](http://allegro.tech/2014/12/swift-method-dispatching.html)。
我们直接将`Foo.swift`编译为汇编代码(
```
swiftc
 -S Foo.swift
```
)：
```
525 __TWoFC3Foo11SimpleClass3sumfT_Si:
526     .quad   152
527
528     .globl  __TMLC3Foo11SimpleClass
529 .zerofill __DATA,__common,__TMLC3Foo11SimpleClass,8,3
530     .section    __DATA,__data
531     .align  3
532 __TMfC3Foo11SimpleClass:
533     .quad   __TFC3Foo11SimpleClassD
534     .quad   __TWVBo
535     .quad   __TMmC3Foo11SimpleClass  // isa指向的位置
536     .quad   _OBJC_CLASS_$_SwiftObject
537     .quad   __objc_empty_cache
538     .quad   0
539     .quad   l__DATA__TtC3Foo11SimpleClass+1
540     .long   3
541     .long   0
542     .long   32
543     .short  7
544     .short  0
545     .long   176
546     .long   16
547     .quad   __TMnC3Foo11SimpleClass-(__TMfC3Foo11SimpleClass+80)
548     .quad   0
549     .quad   __TFC3Foo11SimpleClassg1xSi  // vTable起点
550     .quad   __TFC3Foo11SimpleClasss1xSi
551     .quad   __TFC3Foo11SimpleClassm1xSi
552     .quad   __TFC3Foo11SimpleClassg1ySi
553     .quad   __TFC3Foo11SimpleClasss1ySi
554     .quad   __TFC3Foo11SimpleClassm1ySi
555     .quad   __TFC3Foo11SimpleClasscfT_S0_
556     .quad   __TFC3Foo11SimpleClass3sumfT_Si  // sum方法
557     .quad   16
558     .quad   24
559
560     .section    __TEXT,__swift3_typeref,regular,no_dead_strip
561     .align  4
562 L___unnamed_7:
563     .asciz  "C3Foo11SimpleClass"
```
然后`demangle`相关的几个符号：
```bash
$ xcrun swift-demangle *
output:
_TMmC3Foo11SimpleClass ---> metaclass for Foo.SimpleClass
_TMfC3Foo11SimpleClass ---> full type metadata for Foo.SimpleClass
_TFC3Foo11SimpleClassD ---> Foo.SimpleClass.__deallocating_deinit
_TMmC3Foo11SimpleClass ---> metaclass for Foo.SimpleClass
_TMnC3Foo11SimpleClass ---> nominal type descriptor for Foo.SimpleClass
_TMfC3Foo11SimpleClass ---> full type metadata for Foo.SimpleClass
_TFC3Foo11SimpleClassg1xSi ---> Foo.SimpleClass.x.getter : Swift.Int
_TFC3Foo11SimpleClasss1xSi ---> Foo.SimpleClass.x.setter : Swift.Int
_TFC3Foo11SimpleClassm1xSi ---> Foo.SimpleClass.x.materializeForSet : Swift.Int
_TFC3Foo11SimpleClass3sumfT_Si ---> Foo.SimpleClass.sum () -> Swift.Int
```
很棒，一切符合预期。
### 调用细节
然后我们看一下`Bar.swift`中发生了什么(
```
swiftc
 -I. -L. -lFoo -S Bar.swift
```
):
```
38 __TF3Bar3barFT_Si:
 39     .cfi_startproc
 40     pushq   %rbp
 41 Ltmp3:
 42     .cfi_def_cfa_offset 16
 43 Ltmp4:
 44     .cfi_offset %rbp, -16
 45     movq    %rsp, %rbp
 46 Ltmp5:
 47     .cfi_def_cfa_register %rbp
 48     subq    $32, %rsp
 49     callq   __TMaC3Foo11SimpleClass
 50     movq    %rax, %rdi
 51     callq   __TFC3Foo11SimpleClassCfT_S0_  // 构造SimpleClass实例
 52     movq    (%rax), %rdi
 53     movq    %rdi, -8(%rbp)
 54     movq    %rax, %rdi
 55     movq    -8(%rbp), %rcx
 56     movq    %rax, -16(%rbp)
 57     callq   *136(%rcx)  // 调用sum方法
 58     movq    -16(%rbp), %rdi
 59     movq    %rax, -24(%rbp)
 60     callq   _rt_swift_release
 61     movq    -24(%rbp), %rax
 62     addq    $32, %rsp
 63     popq    %rbp
 64     retq
 65     .cfi_endproc
```
跳过细节，我们直接看`line 57： callq *136(%rcx)`行，这正是对`sum`方法的调用。`rcx`寄存器存储的值为`SimpleClass.metadata`的起始指针，偏移136字节后，便是上述中
```
_TFC3Foo11SimpleClass3sumfT_Si
 ---> Foo.SimpleClass.sum () -> Swift.Int
```
符号。
### 引起不兼容的原因
这时候我们就可以考虑一下，在复现实例中，我们加入`diff`字段发生的事情了。加入`diff`字段后重新编译`Foo.swift`，可以看到汇编代码中metadata布局变成了：
```
671     .quad   __TMnC3Foo11SimpleClass-(__TMfC3Foo11SimpleClass+80)
672     .quad   0
673     .quad   __TFC3Foo11SimpleClassg4diffGSqSi_
674     .quad   __TFC3Foo11SimpleClasss4diffGSqSi_
675     .quad   __TFC3Foo11SimpleClassm4diffGSqSi_
676     .quad   __TFC3Foo11SimpleClassg1xSi
677     .quad   __TFC3Foo11SimpleClasss1xSi
678     .quad   __TFC3Foo11SimpleClassm1xSi
679     .quad   __TFC3Foo11SimpleClassg1ySi
680     .quad   __TFC3Foo11SimpleClasss1ySi
681     .quad   __TFC3Foo11SimpleClassm1ySi
682     .quad   __TFC3Foo11SimpleClasscfT_S0_
683     .quad   __TFC3Foo11SimpleClass3sumfT_Si
```
也就是说，Swift为新增的字段添加了三个方法，插在`vtable`其他方法的前面。这时候，`__TFC3Foo11SimpleClass3sumfT_Si`的位置就变了，而`Bar`中仍然以
```
callq
 *136(%rcx)
```
调用sum函数，自然就发生了错误，导致闪退。
## 其他情况
上述例子主要是展示了一种研究ABI兼容性的思路。据此，我们可以研究其他情况。
### Struct
Swift中，Struct不允许继承，所以它的方法派发不必要依赖`vtable`实现，而是直接被编译为全局函数(未研究可见修饰符的影响)。那它还会遇到`Class`中遇到的问题吗？测试结果是，仍然会。我们把`SimpleClass`修改为`SimpleStruct`，同时直接在`x`和`y`字段之间增加`diff`字段方便印证逻辑，看下`Bar`中的变化：
```
// Foo.swift节选
public struct SimpleStruct {
    public var x: Int = 0
    public var diff: Int = 0
    public var y: Int = 0
...
}
```
```
// foo.s节选
 63 __TFV3Foo12SimpleStruct3sumfT_Si:
 64     .cfi_startproc
 65     pushq   %rbp
 66 Ltmp6:
 67     .cfi_def_cfa_offset 16
 68 Ltmp7:
 69     .cfi_offset %rbp, -16
 70     movq    %rsp, %rbp
 71 Ltmp8:
 72     .cfi_def_cfa_register %rbp
 73     addq    %rdx, %rdi  // .x + .y
 74     seto    %al
 75     movq    %rsi, -8(%rbp)
 76     movq    %rdi, -16(%rbp)
 77     movb    %al, -17(%rbp)
 78     jo  LBB2_2
 79     movq    -16(%rbp), %rax
 80     popq    %rbp
 81     retq
 82 LBB2_2:
 83     ud2
 84     .cfi_endproc
```
```
// bar.s节选
 46 Ltmp5:
 47     .cfi_def_cfa_register %rbp
 48     callq   __TFV3Foo12SimpleStructCfT_S0_
 49     movq    %rax, %rdi  // .x
 50     movq    %rdx, %rsi
 51     movq    %rcx, %rdx  // .y
 52     callq   __TFV3Foo12SimpleStruct3sumfT_Si
 53     popq    %rbp
 54     retq
 55     .cfi_endproc
```
可以看到，在`bar.s`中，调用sum时直接调用全局符号来完成的，而非Class中以偏移量实现。而且，在调用前，参数并不是以指向struct实例的指针方式提供`self.`，而是直接传值！传值过程依赖于`SimpleStruct`的内存布局，所以，一旦布局改变，这里就会发生错误。
### Protocol
参考[Swift进阶之内存模型和方法调度](http://www.jianshu.com/p/6495a6ce65ed)一文，Swift对于协议类型的采用如下的内存模型 - Existential Container：
![](http://upload-images.jianshu.io/upload_images/1564920-d11e66f0c4e29532.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
> 
Existential Container包括以下三个部分：
- 前三个word：Value buffer。用来存储Inline的值，如果word数大于3，则采用指针的方式，在堆上分配对应需要大小的内存
- 第四个word：Value Witness Table(VWT)。每个类型都对应这样一个表，用来存储值的创建，释放，拷贝等操作函数。
- 第五个word：Protocol Witness Table(PWT)，用来存储协议的函数。
![](http://upload-images.jianshu.io/upload_images/1564920-75c3deceae501733.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
那么，显然，如果属性增删导致属性存储区在栈、堆之间变化，或者类的方法(包括可见属性的getter、setter)增删引起`vtable`变化，都会引起不兼容问题。
### Extension
我们常常在各个地方为类增加扩展，显然它是不会引起不兼容问题的。为`Foo.swift`增加一个扩展，其中实现一个空方法`f0()`，我们从汇编代码可以看到只是增加了一个global方法定义:
```
320     .globl  __TFC3Foo11SimpleClass2f0fT_T_
321     .align  4, 0x90
322 __TFC3Foo11SimpleClass2f0fT_T_:
323     .cfi_startproc
324     pushq   %rbp
325 Ltmp39:
326     .cfi_def_cfa_offset 16
327 Ltmp40:
328     .cfi_offset %rbp, -16
329     movq    %rsp, %rbp
330 Ltmp41:
331     .cfi_def_cfa_register %rbp
332     movq    %rdi, -8(%rbp)
333     popq    %rbp
334     retq
335     .cfi_endproc
```
### 静态方法
静态方法和扩展类似，只是增加了global方法定义，不会影响到类的metadata，所以不会引起不兼容问题。
## 总结
Swift 3出来时，我们曾看到Chris Lattner的邮件: [回顾Swift3，展望Swift4](https://lists.swift.org/pipermail/swift-evolution/Week-of-Mon-20160725/025676.html)。他提到Swift4阶段1的任务时说：
> - Resilience: This provides a way for public APIs to evolve over time, even in the face of ABI stability. For example, we don’t want the C++ “fragile base class" problem to ever exist in Swift. Much
 of the design and implementation work was done in the Swift 3 timeframe, but there are still major missing pieces, including the user-visible part of the model (e.g. new attributes).
现在看起来，Swift的整个设计完全是静态的，"易碎的"(fragile)，这使得
```
Much of the design
 and implementation work was done in the Swift 3 timeframe
```
这句话让我十分费解。我不知道是不是我理解的方向都错了。
但回到我们工程，这个问题已经拦在前面了，我们只能尽量不要频繁在底层组件做非二进制兼容的改动，即使有必要，我们也应该找到一个方案，自动化地完成有时序依赖的构建过程。
更多的情况，我将继续研究，大家有不同的见解，欢迎交流~
## Reference
- [http://quotation.github.io/objc/2015/05/21/objc-runtime-ivar-access.html](http://quotation.github.io/objc/2015/05/21/objc-runtime-ivar-access.html)
- [http://llvm.org/docs/LangRef.html#getelementptr-instruction](http://llvm.org/docs/LangRef.html#getelementptr-instruction)
- [https://github.com/apple/swift/blob/master/docs/ABI.rst#class-metadata](https://github.com/apple/swift/blob/master/docs/ABI.rst#class-metadata)
- [http://www.jianshu.com/p/6495a6ce65ed](http://www.jianshu.com/p/6495a6ce65ed)
- [http://allegro.tech/2014/12/swift-method-dispatching.html](http://allegro.tech/2014/12/swift-method-dispatching.html)
- [https://www.cs.cmu.edu/~fp/courses/15213-s07/misc/asm64-handout.pdf](https://www.cs.cmu.edu/~fp/courses/15213-s07/misc/asm64-handout.pdf)
- [https://www.lri.fr/~filliatr/ens/compil/x86-64.pdf](https://www.lri.fr/~filliatr/ens/compil/x86-64.pdf)
- [http://www.imada.sdu.dk/Courses/DM18/Litteratur/IntelnATT.htm](http://www.imada.sdu.dk/Courses/DM18/Litteratur/IntelnATT.htm)
- [https://www.youtube.com/watch?v=ERYNyrfXjlg&feature=youtu.be](https://www.youtube.com/watch?v=ERYNyrfXjlg&feature=youtu.be)
- [https://developer.apple.com/videos/play/wwdc2014/404/](https://developer.apple.com/videos/play/wwdc2014/404/)
- [https://developer.apple.com/videos/play/wwdc2016/416/](https://developer.apple.com/videos/play/wwdc2016/416/)
- [https://developer.apple.com/videos/play/wwdc2015/409/](https://developer.apple.com/videos/play/wwdc2015/409/)
- [http://www.atatech.org/articles/69335](http://www.atatech.org/articles/69335)
