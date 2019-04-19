# Static用法说明 - BlueSky - CSDN博客
2015年03月09日 10:53:06[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：468
个人分类：[java编程](https://blog.csdn.net/ASIA_kobe/article/category/2863525)
1.     静态方法 
通常，在一个类中定义一个方法为static，那就是说，无需本类的对象即可调用此方法
   声明为static的方法有以下几条限制： 
· 它们仅能调用其他的static 方法。 
· 它们只能访问static数据。 
· 它们不能以任何方式引用this 或super。
**class** Simple {
**static****void** go()
 {
       System.*out*.println("Welcome");
    }
}
**public****class** Cal {
**public****static****void** main(String[]
 args) {
       Simple.*go*();
    }
}
调用一个静态方法就是“类名.方法名”,静态方法的使用很简单如上所示。一般来说，静态方法常常为应用程序中的其它类提供一些实用工具所用，在Java的类库中大量的静态方法正是出于此目的而定义的。
2. 静态变量
声明为static的变量实质上就是全局变量。当声明一个对象时，并不产生static变量的拷贝，而是该类所有的实例变量共用同一个static变量。静态变量与静态方法类似。所有此类实例共享此静态变量，也就是说在类装载时，只分配一块存储空间，所有此类的对象都可以操控此块存储空间，当然对于final则另当别论了
**class** Value {
**static****int***c* =
 0;
**static****void** inc()
 {
*c*++;
    }
}
**public****class** Count2 {
**public****static****void** prt(String
 s) {
       System.*out*.print(s);
    }
**public****static****void** main(String[]
 args) {
       Value v1, v2;
       v1 = **new** Value();
       v2 = **new** Value();
*prt*("v1.c=" + v1.*c* + "  v2.c=" +
 v2.*c*);
       v1.*inc*();
*prt*(" v1.c=" + v1.*c* + "  v2.c=" +
 v2.*c*);
    }
}
结果为：v1.c=0  v2.c=0 v1.c=1  v2.c=1
由此可以证明它们共享一块存储区。static变量有点类似于C中的全局变量的概念。
值得探讨的是静态变量的初始化问题。
       如果你需要通过计算来初始化你的static变量，你可以声明一个static块，Static 块仅在该类被加载时执行一次。下面的例子显示的类有一个static方法，一些static变量，以及一个static 初始化块：
**class** Value3 {
**static****int***c* =
 0;
    Value3() {
*c* = 15;
    }
    Value3(**int** i) {
*c* = i;
    }
**static****void** inc()
 {
*c*++;
    }
}
**public****class** Count {
**public****static****void** prt(String
 s) {
       System.*out*.println(s);
    }
    Value3 v = **new** Value3(10);
**static** Value3 *v1*, *v2*;
**static** {//此即为static块
*prt*("v1.c=" + *v1*.*c* + "  v2.c=" + *v2*.*c*);
*v1* = **new** Value3(27);
*prt*("v1.c=" + *v1*.*c* + "  v2.c=" + *v2*.*c*);
*v2* = **new** Value3(15);
*prt*("v1.c=" + *v1*.*c* + "  v2.c=" + *v2*.*c*);
    }
**public****static****void** main(String[]
 args) {
       Count ct = **new** Count();
*prt*("ct.c=" + ct.v.*c*);
*prt*("v1.c=" + *v1*.*c* + "  v2.c=" + *v2*.*c*);
*v1*.*inc*();
*prt*("v1.c=" + *v1*.*c* + "  v2.c=" + *v2*.*c*);
*prt*("ct.c=" + ct.v.*c*);
    }
}
结果为：v1.c=0  v2.c=0
v1.c=27  v2.c=27
v1.c=15  v2.c=15
ct.c=10
v1.c=10  v2.c=10
v1.c=11  v2.c=11
ct.c=11
          这个程序展示了静态初始化的各种特性。如果你初次接触Java，结果可能令你吃惊。可能会对static后加大括号感到困惑。首先要告诉你的是，static定义的变量会优先于任何其它非static变量，不论其出现的顺序如何。正如在程序中所表现的，虽然v出现在v1和v2的前面，但是结果却是v1和v2的初始化在v的前面。在static{后面跟着一段代码，这是用来进行显式的静态变量初始化，这段代码只会初始化一次，且在类被第一次装载时。如果你能读懂并理解这段代码，会帮助你对static关键字的认识。在涉及到继承的时候，会先初始化父类的static变量，然后是子类的，依次类推。
3．静态类
         通常一个普通类不允许声明为静态的，只有一个内部类才可以。这时这个声明为静态的内部类可以直接作为一个普通类来使用，而不需实例一个外部类。
**public****class** StaticCls {
**public****static****void** main(String[]
 args) {
OuterCls.InnerCls oi = **new**OuterCls.InnerCls();
    }
}
**class**OuterCls {
**public****static****class** InnerCls
 {
       InnerCls() {
           System.*out*.println("InnerCls");
       }
    }
} 
结果为：InnerCls
4．static和final一块用表示什么 
static final用来修饰成员变量和成员方法，可简单理解为“全局常量”！ 
对于变量，表示一旦给值就不可修改，并且通过类名可以访问。 
对于方法，表示不可覆盖，并且可以通过类名直接访问。
5.补充：
          static表示“全局”或者“静态”的意思，用来修饰成员变量和成员方法，也可以形成静态static代码块，但是Java语言中没有全局变量的概念。 
　被static修饰的成员变量和成员方法独立于该类的任何对象。也就是说，它不依赖类特定的实例，被类的所有实例共享。只要这个类被加载，Java虚拟机就能根据类名在运行时数据区的方法区内定找到他们。因此，static对象可以在它的任何对象创建之前访问，无需引用任何对象。 
　用public修饰的static成员变量和成员方法本质是全局变量和全局方法，当声明它类的对象时，不生成static变量的副本，而是类的所有实例共享同一个static变量。 
　static 变量前可以有private修饰，表示这个变量可以在类的静态代码块中，或者类的其他静态成员方法中使用（当然也可以在非静态成员方法中使用--废话），但是不能在其他类中通过类名来直接引用，这一点很重要。实际上你需要搞明白，private是访问权限限定，static表示不要实例化就可以使用，这样就容易理解多了。static前面加上其它访问权限关键字的效果也以此类推。 
　static修饰的成员变量和成员方法习惯上称为静态变量和静态方法，可以直接通过类名来访问，访问语法为： 
类名.静态方法名(参数列表...) 
类名.静态变量名 
　用static修饰的代码块表示静态代码块，当Java虚拟机（JVM）加载类时，就会执行该代码块（用处非常大，呵呵）。 
static变量 
　按照是否静态的对类成员变量进行分类可分两种：一种是被static修饰的变量，叫静态变量或类变量；另一种是没有被static修饰的变量，叫实例变量。两者的区别是： 
　对于静态变量在内存中只有一个拷贝（节省内存），JVM只为静态分配一次内存，在加载类的过程中完成静态变量的内存分配，可用类名直接访问（方便），当然也可以通过对象来访问（但是这是不推荐的）。 
　对于实例变量，没创建一个实例，就会为实例变量分配一次内存，实例变量可以在内存中有多个拷贝，互不影响（灵活）。 
static方法 
　静态方法可以直接通过类名调用，任何的实例也都可以调用，因此静态方法中不能用this和super关键字，不能直接访问所属类的实例变量和实例方法 (就是不带static的成员变量和成员成员方法)，只能访问所属类的静态成员变量和成员方法。因为实例成员与特定的对象关联！这个需要去理解，想明白其中的道理，不是记忆！！！ 
　因为static方法独立于任何实例，因此static方法必须被实现，而不能是抽象的abstract。
