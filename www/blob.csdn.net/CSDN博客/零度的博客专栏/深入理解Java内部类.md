# 深入理解Java内部类 - 零度的博客专栏 - CSDN博客
2017年11月23日 14:39:34[零度anngle](https://me.csdn.net/zmx729618)阅读数：340
## 一、什么是内部类？
　　内部类是指在一个外部类的内部再定义一个类。内部类作为外部类的一个成员，并且依附于外部类而存在的。内部类可为静态，可用protected和private修饰（而外部类只能使用public和缺省的包访问权限）。内部类主要有以下几类：成员内部类、局部内部类、静态内部类、匿名内部类
## 二、内部类的共性
(1)、内部类仍然是一个独立的类，在编译之后内部类会被编译成独立的.class文件，但是前面冠以外部类的类名和$符号 。
(2)、内部类不能用普通的方式访问。
(3)、内部类声明成静态的，就不能随便的访问外部类的成员变量了，此时内部类只能访问外部类的静态成员变量 。
(4)、外部类不能直接访问内部类的的成员，但可以通过内部类对象来访问
　　内部类是外部类的一个成员，因此内部类可以自由地访问外部类的成员变量，无论是否是private的。
　　因为当某个外围类的对象创建内部类的对象时，此内部类会捕获一个隐式引用，它引用了实例化该内部对象的外围类对象。通过这个指针，可以访问外围类对象的全部状态。
通过反编译内部类的字节码，分析之后主要是通过以下几步做到的： 
　　1 编译器自动为内部类添加一个成员变量， 这个成员变量的类型和外部类的类型相同， 这个成员变量就是指向外部类对象的引用； 
　　2 编译器自动为内部类的构造方法添加一个参数， 参数的类型是外部类的类型， 在构造方法内部使用这个参数为1中添加的成员变量赋值； 
　　3 在调用内部类的构造函数初始化内部类对象时， 会默认传入外部类的引用。
## 三、为什么需要内部类？
其主要原因有以下几点：
- 
内部类方法可以访问该类定义所在的作用域的数据，包括私有的数据
- 
内部类可以对同一个包中的其他类隐藏起来,一般的非内部类，是不允许有 private 与protected权限的，但内部类可以
- 
可以实现多重继承
- 
当想要定义一个回调函数且不想编写大量代码时，使用匿名内部类比较便捷
使用内部类最吸引人的原因是：
　　每个内部类都能独立地继承自一个（接口的）实现，所以无论外围类是否已经继承了某个（接口的）实现，对于内部类都没有影响。大家都知道Java只能继承一个类，它的多重继承在我们没有学习内部类之前是用接口来实现的。但使用接口有时候有很多不方便的地方。比如我们实现一个接口就必须实现它里面的所有方法。而有了内部类就不一样了。它可以使我们的类继承多个具体类或抽象类。
大家看下面的例子:
```
public class Example1 {
    public String name(){
       return "liutao";
   }
}
public class Example2 {
    public int age(){
        return 25;
    }
}
public class MainExample {
   private class test1 extends Example1{
        public String name(){
          return super.name();
        }
    }
    private class test2 extends Example2 {
       public int age(){
         return super.age();
       }
    }
   public String name(){
    return new test1().name(); 
   }
   public int age(){
       return new test2().age();
   }
   public static void main(String args[]){
       MainExample mi=new MainExample();
       System.out.println("姓名:"+mi.name());
       System.out.println("年龄:"+mi.age());
   }
}
```
## 四、成员内部类：
　　即在一个类中直接定义的内部类， 成员内部类与普通的成员没什么区别，可以与普通成员一样进行修饰和限制。成员内部类不能含有static的变量和方法。
publicclass Outer {
    privatestaticint i = 1;
    privateint j = 10;
    privateint k = 20;
    publicstaticvoid outer_f1() {}
    publicvoid outer_f2() {}
    // 成员内部类中，不能定义静态成员
    // 成员内部类中，可以访问外部类的所有成员class Inner {
        // static int inner_i = 100;//内部类中不允许定义静态变量int j = 100; // 内部类和外部类的实例变量可以共存int inner_i = 1;
        void inner_f1() {
            System.out.println(i);
            // 在内部类中访问内部类自己的变量直接用变量名            System.out.println(j);
            // 在内部类中访问内部类自己的变量也可以用this.变量名
            System.out.println(this.j);
            // 在内部类中访问外部类中与内部类同名的实例变量用外部类名.this.变量名
            System.out.println(Outer.this.j);
            // 如果内部类中没有与外部类同名的变量，则可以直接用变量名访问外部类变量            System.out.println(k);
            outer_f1();
            outer_f2();
        }
    }
    // 外部类的非静态方法访问成员内部类publicvoid outer_f3() {
        Inner inner = new Inner();
        inner.inner_f1();
    }
    // 外部类的静态方法访问成员内部类，与在外部类外部访问成员内部类一样publicstaticvoid outer_f4() {
        // step1 建立外部类对象
        Outer out = new Outer();
        // step2 根据外部类对象建立内部类对象
        Inner inner = out.new Inner();
        // step3 访问内部类的方法        inner.inner_f1();
    }
    publicstaticvoid main(String[] args) {
        //outer_f4();//该语句的输出结果和下面三条语句的输出结果一样
        // 如果要直接创建内部类的对象，不能想当然地认为只需加上外围类Outer的名字，
        // 就可以按照通常的样子生成内部类的对象，而是必须使用此外围类的一个对象来
        // 创建其内部类的一个对象：
        // Outer.Inner outin = out.new Inner()
        // 因此，除非你已经有了外围类的一个对象，否则不可能生成内部类的对象。因为此
        // 内部类的对象会悄悄地链接到创建它的外围类的对象。如果你用的是静态的内部类，
        // 那就不需要对其外围类对象的引用。
        Outer out = new Outer();
        Outer.Inner outin = out.new Inner();
        outin.inner_f1();
    }
}
## 六、局部内部类：
　　在方法中定义的内部类称为局部内部类。与局部变量类似，局部内部类不能有访问说明符，因为它不是外围类的一部分，但是它可以访问当前代码块内的常量，和此外围类所有的成员。
需要注意的是：
　　(1)、局部内部类只能在定义该内部类的方法内实例化，不可以在此方法外对其实例化。
　　(2)、局部内部类对象不能使用该内部类所在方法的非final局部变量。
具体原因等下再说
publicclass Outer {
    privateint s = 100;
    privateint out_i = 1;
    publicvoid f(finalint k) {
        finalint s = 200;
        int i = 1;
        finalint j = 10;
        // 定义在方法内部class Inner {
            int s = 300;// 可以定义与外部类同名的变量
            // static int m = 20;//不可以定义静态变量
            Inner(int k) {
                inner_f(k);
            }
            int inner_i = 100;
            void inner_f(int k) {
                // 如果内部类没有与外部类同名的变量，在内部类中可以直接访问外部类的实例变量                System.out.println(out_i);
                // 可以访问外部类的局部变量(即方法内的变量)，但是变量必须是final的                System.out.println(j);
                // System.out.println(i);
                // 如果内部类中有与外部类同名的变量，直接用变量名访问的是内部类的变量                System.out.println(s);
                // 用this.变量名访问的也是内部类变量
                System.out.println(this.s);
                // 用外部类名.this.内部类变量名访问的是外部类变量
                System.out.println(Outer.this.s);
            }
        }
        new Inner(k);
    }
    publicstaticvoid main(String[] args) {
        // 访问局部内部类必须先有外部类对象
        Outer out = new Outer();
        out.f(3);
    }
}
## 七、静态内部类(嵌套类)：
　　如果你不需要内部类对象与其外围类对象之间有联系，那你可以将内部类声明为static。这通常称为嵌套类（nested class）。想要理解static应用于内部类时的含义，你就必须记住，普通的内部类对象隐含地保存了一个引用，指向创建它的外围类对象。然而，当内部类是static的时，就不是这样了。嵌套类意味着：
　　1. 要创建嵌套类的对象，并不需要其外围类的对象。
　　2. 不能从嵌套类的对象中访问非静态的外围类对象。
publicclass Outer {
    privatestaticint i = 1;
    privateint j = 10;
    publicstaticvoid outer_f1() {}
    publicvoid outer_f2() {}
    // 静态内部类可以用public,protected,private修饰
    // 静态内部类中可以定义静态或者非静态的成员privatestaticclass Inner {
        staticint inner_i = 100;
        int inner_j = 200;
        staticvoid inner_f1() {
            // 静态内部类只能访问外部类的静态成员(包括静态变量和静态方法)
            System.out.println("Outer.i" + i);
            outer_f1();
        }
        void inner_f2() {
            // 静态内部类不能访问外部类的非静态成员(包括非静态变量和非静态方法)
            // System.out.println("Outer.i"+j);
            // outer_f2();        }
    }
    publicvoid outer_f3() {
        // 外部类访问内部类的静态成员：内部类.静态成员        System.out.println(Inner.inner_i);
        Inner.inner_f1();
        // 外部类访问内部类的非静态成员:实例化内部类即可
        Inner inner = new Inner();
        inner.inner_f2();
    }
    publicstaticvoid main(String[] args) {
        new Outer().outer_f3();
    }
}
生成一个静态内部类不需要外部类成员：这是静态内部类和成员内部类的区别。静态内部类的对象可以直接生成：Outer.Inner in = new Outer.Inner();而不需要通过生成外部类对象来生成。这样实际上使静态内部类成为了一个顶级类(正常情况下，你不能在接口内部放置任何代码，但嵌套类可以作为接口的一部分，因为它是static
 的。只是将嵌套类置于接口的命名空间内，这并不违反接口的规则）
## 八、匿名内部类：
简单地说：匿名内部类就是没有名字的内部类。什么情况下需要使用匿名内部类？如果满足下面的一些条件，使用匿名内部类是比较合适的：
- 只用到类的一个实例。
- 类在定义后马上用到。
- 类非常小（SUN推荐是在4行代码以下）
- 给类命名并不会导致你的代码更容易被理解。
在使用匿名内部类时，要记住以下几个原则：
- 
　　匿名内部类不能有构造方法。
- 
　　匿名内部类不能定义任何静态成员、方法和类。
- 
　　匿名内部类不能是public,protected,private,static。
- 
　　只能创建匿名内部类的一个实例。
- 
   一个匿名内部类一定是在new的后面，用其隐含实现一个接口或实现一个类。
- 
　　因匿名内部类为局部内部类，所以局部内部类的所有限制都对其生效。
下面的代码展示的是，如果你的基类需要一个有参数的构造器，应该怎么办：
```
public class Parcel7 {
    public Wrapping wrap(int x) {
        // Base constructor call:
        return new Wrapping(x) { // Pass constructor argument.
            public int value() {
                return super.value() * 47;
            }
        }; // Semicolon required
    }
    public static void main(String[] args) {
        Parcel7 p = new Parcel7();
        Wrapping w = p.wrap(10);
    }
}
```
只需简单地传递合适的参数给基类的构造器即可，这里是将x 传进new Wrapping(x)。在匿名内部类末尾的分号，并不是用来标记此内部类结束（C++中是那样）。实际上，它标记的是表达式的结束，只不过这个表达式正巧包含了内部类罢了。因此，这与别的地方使用的分号是一致的。
如果在匿名类中定义成员变量或者使用带参数的构造函数，你同样能够对其执行初始化操作：
```
public class Parcel8 {
    // Argument must be final to use inside
    // anonymous inner class:
    public Destination dest(final String name, String city) {
        return new Destination(name, city) {
            private String label = name;
            public String getName() {
                return label;
            }
        };
    }
    public static void main(String[] args) {
        Parcel8 p = new Parcel8();
        Destination d = p.dest("Tanzania", "gz");
    }
    abstract class Destination {
        Destination(String name, String city) {
            System.out.println(city);
        }
        abstract String getName();
    }
}
```
注意这里的形参city，由于它没有被匿名内部类直接使用，而是被抽象类Inner的构造函数所使用，所以不必定义为final。
## 九、内部类的重载问题
　　如果你创建了一个内部类，然后继承其外围类并重新定义此内部类时，会发生什么呢？也就是说，内部类可以被重载吗？这看起来似乎是个很有用的点子，但是“重载”内部类就好像它是外围类的一个方法，其实并不起什么作用：
```
class Egg {
       private Yolk y;
 
       protected class Yolk {
              public Yolk() {
                     System.out.println("Egg.Yolk()");
              }
       }
 
       public Egg() {
              System.out.println("New Egg()");
              y = new Yolk();
       }
}
 
public class BigEgg extends Egg {
       public class Yolk {
              public Yolk() {
                     System.out.println("BigEgg.Yolk()");
              }
       }
 
       public static void main(String[] args) {
              new BigEgg();
       }
}
```
输出结果为：
New Egg()
Egg.Yolk()
缺省的构造器是编译器自动生成的，这里是调用基类的缺省构造器。你可能认为既然创建了BigEgg 的对象，那么所使用的应该是被“重载”过的Yolk，但你可以从输出中看到实际情况并不是这样的。
这个例子说明，当你继承了某个外围类的时候，内部类并没有发生什么特别神奇的变化。这两个内部类是完全独立的两个实体，各自在自己的命名空间内。当然，明确地继承某个内部类也是可以的：
```
class Egg2 {
       protected class Yolk {
              public Yolk() {
                     System.out.println("Egg2.Yolk()");
              }
 
              public void f() {
                     System.out.println("Egg2.Yolk.f()");
              }
       }
 
       private Yolk y = new Yolk();
 
       public Egg2() {
              System.out.println("New Egg2()");
       }
 
       public void insertYolk(Yolk yy) {
              y = yy;
       }
 
       public void g() {
              y.f();
       }
}
 
public class BigEgg2 extends Egg2 {
       public class Yolk extends Egg2.Yolk {
              public Yolk() {
                     System.out.println("BigEgg2.Yolk()");
              }
 
              public void f() {
                     System.out.println("BigEgg2.Yolk.f()");
              }
       }
 
       public BigEgg2() {
              insertYolk(new Yolk());
       }
 
       public static void main(String[] args) {
              Egg2 e2 = new BigEgg2();
              e2.g();
       }
}
```
输出结果为：
Egg2.Yolk()
New Egg2()
Egg2.Yolk()
BigEgg2.Yolk()
BigEgg2.Yolk.f()
现在BigEgg2.Yolk 通过extends Egg2.Yolk 明确地继承了此内部类，并且重载了其中的方法。Egg2 的insertYolk()方法使得BigEgg2 将它自己的Yolk 对象向上转型，然后传递给引用y。所以当g()调用y.f()时，重载后的新版的f()被执行。第二次调用Egg2.Yolk()是BigEgg2.Yolk
 的构造器调用了其基类的构造器。可以看到在调用g()的时候，新版的f()被调用了。
## 十、内部类的继承问题
　　因为内部类的构造器要用到其外围类对象的引用，所以在你继承一个内部类的时候，事情变得有点复杂。问题在于，那个“秘密的”外围类对象的引用必须被初始化，而在被继承的类中并不存在要联接的缺省对象。要解决这个问题，需使用专门的语法来明确说清它们之间的关联：
```
class WithInner {
        class Inner {
                Inner(){
                        System.out.println("this is a constructor in WithInner.Inner");
                };
        }
}
 
public class InheritInner extends WithInner.Inner {
        // ! InheritInner() {} // Won't compile
        InheritInner(WithInner wi) {
                wi.super();
                System.out.println("this is a constructor in InheritInner");
        }
 
        public static void main(String[] args) {
                WithInner wi = new WithInner();
                InheritInner ii = new InheritInner(wi);
        }
}
```
输出结果为：
this is a constructor in WithInner.Inner
this is a constructor in InheritInner
可以看到，InheritInner 只继承自内部类，而不是外围类。但是当要生成一个构造器时，缺省的构造器并不算好，而且你不能只是传递一个指向外围类对象的引用。此外，你必须在构造器内使用如下语法：
enclosingClassReference.super();
这样才提供了必要的引用，然后程序才能编译通过。
## 为什么非静态内部类中不能有static修饰的属性，但却可以有常量？
如：
publicclass InnerClassDemo{
    int x;
    class A{
        staticint a = 0;//这样写是不合法的.staticfinalint b=0;//这样写是合法的
     }
}
定义一个静态的域或者方法，要求在静态环境或者顶层环境，即如果加上 static class A变成静态内部类就ok非静态内部类 依赖于一个外部类对象，而静态域/方法是不依赖与对象——仅与类相关（细说了，就是加载静态域时，根本没有外部类对象）因此，非静态内部类中不能定义静态域/方法，编译过不了。
而常量之所以可以（不论有无static），因为java在编译期就确定所有常量，放到所谓的常量池当中。常量的机制和普通变量不一样
