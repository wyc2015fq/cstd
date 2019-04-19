# ios内存管理 - LC900730的博客 - CSDN博客
2017年06月26日 23:55:59[lc900730](https://me.csdn.net/LC900730)阅读数：145标签：[ios](https://so.csdn.net/so/search/s.do?q=ios&t=blog)
个人分类：[oc](https://blog.csdn.net/LC900730/article/category/6990658)
## 1.引用计数器
1、每个OC对象都有自己的引用计数器； 
2、它是一个整数； 
3、从字面上看，可以理解为“对象被引用的次数”，也可理解为多少个指针指向它； 
4、当引用计数器为0，对象占用的内存就会被系统回收； 
5、当使用alloc、new或者copy创建一个对象时候，对象的引用计数器默认是1；
## 2.操作引用计数器
retain：使得对象引用计数器+1； 
release：使得对象引用计数器-1； 
retainCount：查看当前对象的引用计数器的个数；(不准确)
## 3.dealloc方法
对象被销毁之前，系统会自动调用； 
一般是重写dealloc方法，在这里释放相关资源，一旦重写daelloc方法，就必须在最后调用[super daelloc],并且在最后调用。 
不能直接调用daelloc方法； 
一旦释放，占用的内存不可使用，使用可能会出现野指针错误；
## ARC:
automatic reference counting:自动引用计数； 
不需要程序员管理内容，编译器会在适当地方自动给我们添加release/retain等代码;(java中的垃圾回收机制不一样，java中回收是系统干的) 
MRC:程序员需要手动管理，自己编写retain/release等代码；
```
Person *p=[Person new];
//Wife遵守了这个协议，编译器不会报警告。
Wife<WifeCondition> *w=[Wife new];
//如果想明确通知人类必须接受一个遵守协议的wife，那么Person.h文件中（类型限定，类型限定是写在数据类型的右边）
@property (nonatomic,strong)Wife<WifeCondition>*wife;
//虽然在接受一个对象的时候，对这个对象进行了类型限定，但是并不意味着这个对象就真正的实现了这个方法，所以在调用对象的协议方法时应该进行一次安全验证
if[self.wife respondsToSelector:@selector(cooking)]){
    [self.wife cooking];
}
p.wife=w;
```
## 4.协议的应用：代理设计模式
当对象A发生了一些行为，想告知对象B(B成为A的代理)； 
对象B想监听对象A的一些行为 
当对象A无法处理某些行为的时候，想让B帮忙处理；
使用id类型来接受保姆，如果将来换保姆了，婴儿类不用修改代码； 
但是学生类没有喂东西、哄小孩的能力，因此需要实现协议 
id  nanny; 
下次招聘保姆的时候只需要实现协议即可；
学生找房子，不具备找房子的能力，所以找一个代理; 
编写规范： 
1.一般情况下：当前协议属于谁，我们就将协议定义到谁的头文件中； 
2.协议的名称一般以它属于的那个类的类名开头，后面跟上Protocol或者delegate； 
3.协议中的方法名称一般以协议的名称protocol之前的作为开头； 
4.一般情况下协议中的方法会触发该协议的对象传递出去(即哪一个类触发就将哪一个类传递) 
5.一般情况下一个类中的代理属于的名称叫做delegate; 
6.当某一个类要成为另一个类的代理的时候，一般情况下.h中用@protocol协议名称，告诉当前类这是一个协议；在 
.m中用#import真正的导入一个类；
