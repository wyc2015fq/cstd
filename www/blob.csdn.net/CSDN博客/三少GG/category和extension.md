# category和extension - 三少GG - CSDN博客
2016年02月04日 00:36:55[三少GG](https://me.csdn.net/scut1135)阅读数：770
[http://tech.meituan.com/DiveIntoCategory.html](http://tech.meituan.com/DiveIntoCategory.html)
# 2、连类比事-category和extension
extension看起来很像一个匿名的category，但是extension和有名字的category几乎完全是两个东西。 extension在编译期决议，它就是类的一部分，在编译期和头文件里的@interface以及实现文件里的@implement一起形成一个完整的类，它伴随类的产生而产生，亦随之一起消亡。**extension一般用来隐藏类的私有信息，你必须有一个类的源码才能为一个类添加extension，所以你无法为系统的类比如NSString添加extension。**（详见[2](https://developer.apple.com/library/mac/documentation/Cocoa/Conceptual/ProgrammingWithObjectiveC/CustomizingExistingClasses/CustomizingExistingClasses.html)）
但是category则完全不一样，它是在运行期决议的。
就category和extension的区别来看，我们可以推导出一个明显的事实，**extension可以添加实例变量，而category是无法添加实例变量的（因为在运行期，对象的内存布局已经确定，如果添加实例变量就会破坏类的内部布局，这对编译型语言来说是灾难性的）**。
# iOS学习笔记：Category与Extension的区别
- [iOS学习笔记](http://www.dejohndong.com/tags/iOS%E5%AD%A6%E4%B9%A0%E7%AC%94%E8%AE%B0/)
## 前言
作为一个有4年以上iOS开发经验的开者者来说，使用Category和Extension的场景应该是数不胜数的，也不知道从XCode的哪个版本开始，创建一个新的UIViewController类会默认添加一个该类的Extension，而我也习惯于在这个Extension上添加一些不公开的属性。
## 我的分析
仔细查阅了相关资料并且结合自己的实践得出以下结论：
Category: 字面意思是类别。
- 
主要用来扩展方法，并且适用于subclass;
- 
只能添加readonly的属性, 如果要添加readwrite属性必须在runtime过程中用objc_setAssociatedObject()和objc_getAssociatedObject()方法来实现属性的get与set方法;
Extension:字面意思是扩展。
- 
同样可以扩展方法和属性，但局限于原始类；
- 
声明的方法必须在@implemention中实现，不然编译器会报warning;
在extension中可以定义可写的属性，公有可读、私有可写的属性(Publicly-Readable, Privately-Writeable Properties)一般这样实现。
## 总结
综上所述，我们通常要封装一些公共方法的时候我们可以考虑使用Category的方式。
如果我们想在原始类上面增加一些不公开的方法、属性（私有方法、属性）时可以新建一个Extension来解决问题。
- 

