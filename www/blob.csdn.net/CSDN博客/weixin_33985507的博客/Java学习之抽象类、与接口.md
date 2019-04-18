# Java学习之抽象类、与接口 - weixin_33985507的博客 - CSDN博客
2017年05月27日 15:10:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
# 一、抽象类
## 1、抽象类含义的概括：
当多个类出现相同功能时，但功能主体不同，这样可以向上抽取，抽取时只抽取功能定义，而不抽取功能主体。也就是说，我们在从下往上看继承这个体系结构时，位于上层的类要比下层更具有通用性，也就是说更加抽象，即最上层的祖先（即超类）最具通用性。这时只讲上层的类作为遗传（或者说派生）下层类的基本的类，而不考虑特定的实例对象。
## 2、抽象类的特点：
1、抽象方法一定在抽象类中，就是说，一个类中含有抽象方法，这个类也必须是抽象的。
2、抽象方法和抽象类必须被abstract修饰，这是作为抽象（类或方法）的一个标志。
3、抽象类不可用new创建对象，因为调用抽象方法没有意义（抽象方法为类中的成员）。
4、抽象类中的方法要被使用，必须由子类复写所有的抽象方法后，建立子类对象调用，也就是说，如果子类中只覆盖了部分抽象方法，那么这个子类仍为抽象的，是个抽象类。
总结来说，抽象类，是提供功能的，具体实现形式还是需要由子类来实现的，这就强迫了子类复写抽象类中的抽象方法。需要注意的是，抽象类中是可以有非抽象方法（子类可不必对其复写）的。
在此，我个人补充一点关于抽象类可以创建数组的东西。
因为数组也是一种特殊的对象，但是像下面这样就可以用new。
代码如下：
```
//抽象类Person  
abstract class Person   
{  
    private String name;  
    public Person(String name)  
    {  
        this.name = name;  
    }  
    //抽象方法：对人的描述，但对每种人的描述是不清楚的，所以用抽象方法  
    public abstract String getDescription();  
  
    public String getName()  
    {  
        return name;  
    }  
}  
//Student继承父类Person  
class Student extends Person  
{  
    public Student(String name)  
    {  
        super(name);  
    }  
    //复写超类的抽象方法，对学生进行具体描述  
    public String getDescription()  
    {  
        return "I'm a student.";  
    }  
}  
////Worker继承父类Person  
class Worker extends Person  
{  
    public Worker(String name)  
    {  
        super(name);  
    }  
    //复写超类的抽象方法，对工人进行具体描述  
    public String getDescription()  
    {  
        return "I'm a worker.";  
    }  
}  
//测试  
class PersonText  
{  
    public static void main(String[] args)   
    {  
        Person[] p = new Person[2];//用抽象类创建数组  
        //再创建两个对象  
        p[0] = new Student("Sun");  
        p[1] = new Worker("Wu");  
        System.out.println(p[0].getName() + ":" + p[0].getDescription());  
        System.out.println(p[1].getName() + ":" + p[1].getDescription());  
    }  
}
```
至于`Person[] p = new Person[2]；`是如何在内存中进行分配的呢？在这里，`Person[] p = new Person[2]`只是创建了两个数组类型的元素，并不是Person类对象，在这里只是一种引用。而下面的new Student和new Worker才真正创建了两个对象，分别赋值给数组中的两个元素，或者说是两个变量。
## 3、抽象类与一般类无多大区别：
1、描述事物还依然照常描述，只是抽象类中出现了一些看不懂的东西，即功能定义，这些不确定的部分也为事物的功能，需要明确出来，但无法定义主体。通过抽象方法来表示。
2、抽象类比一般类多了抽象函数，类中可以定义抽象方法，但是不必创建主体内容。
3、抽象类不可以实例化，因为抽象方法没意义，无法创建对象
4、特殊之处：抽象类中可以不定义抽象方法，抽象类仅仅是为了不让这个类建立对象。
举例：
```
/* 
假如在开发一个系统时需要对员工进行建模，员工有3个属性： 
姓名、工号以及工资，经理也是员工，除了含有员工的属性外。 
还有一个奖金属性，请使用继承的思想设计出员工类和经理类， 
要求类中提供必要的方法进行属性访问。 
 
分析： 
员工类：name、id、salary 
经理类：继承了员工类，并与自己的bonus。 
*/  
abstract class Employee  
{  
    //本应将变量定义为private，为了测试不加private  
    String name;  
    double salary;  
    public Employee(String name,double salary)  
    {  
        this.name = name;  
        this.salary = salary;  
    }  
    //抽象方法，不可定义功能主体  
    public abstract void work();  
}  
//普通员工继承抽象类雇员  
class Pro extends Employee  
{  
    public Pro(String name,double salary)  
    {  
        super(name,salary);  
    }  
    //复写父类中的抽象方法  
    public void work()  
    {  
        System.out.println("Pro Work");  
    }  
}  
//经理继承抽象类雇员  
class Manager extends Employee  
{  
    private double bonus;  
    public Manager(String name,double salary,double bonus)  
    {  
        //调用超类Employee中的构造器  
        super(name,salary);  
        this.bonus = bonus;  
        this.salary += bonus;//经理获得的最终工资  
    }  
    //复写父类中的抽象方法  
    public void work()  
    {  
        System.out.println("Manager Work");  
    }  
}  
  
class ManagerText  
{  
    public static void main(String [] args)  
    {  
        Manager boss = new Manager("Anna",80000,5000);  
        Pro staff = new Pro("Ben",50000);  
        boss.work();  
        System.out.println("name = " + boss.name + ",salary = " + boss.salary);  
        staff.work();  
        System.out.println("name = " + staff.name + ",salary = " + staff.salary);  
    }  
}
```
# 二、接口
接口：是一种实现关系
## 1、接口：
接口可理解为一种特殊的抽象类（但不是），当抽象类中的方法全为抽象的（即不包含任何非抽象方法），可通过接口表示。----  class用来定义类；而interface定义接口
## 2、定义接口的格式特点：
接口：interface    实现：implements
1、接口中常见定义：常量、抽象方法
2、接口中成员的固定修饰符：
  常量：public static final
  方法：public abstract
注意：
A、接口中的成员全为public，当然那些修饰符是可以省略的，因为接口会自动设为相应的权限，但是还是最好加上。
B、当接口中的常量赋值后，不可再进行第二次赋值操作。
C、接口不可创建对象，因为其中全为抽象方法，需要被子类实现后，对接口中抽象方法全覆盖后，子类才可以实现实例化。
```
interface Inter  
{  
    public static final int NUM = 3;  
    public abstract void show();  
}  
  
class Test implements Inter  
{  
    public void shoe(){}  
}  
class InterfaceDemo  
{  
    public static void main(String [] args)  
    {  
        Test t = new Test();  
        System.out.println(t.NUM);  
        System.out.println(Test.NUM);  
        System.out.println(Inter.NUM);  
        int NUM = 5;//Error  
    }  
}
```
## 3、接口可被类多实现，即将java中的多继承改良成多实现。
1、多继承不可以：
是因为父类中的方法有方法体，若多个父类存在相同方法（而方法体不同），子类如果多继承这些父类的话，那么在运行子类的时候，并不能判断出要运行这些父类中的哪个方法，因此程序会出现异常。所以多继承不可以。
2、多实现可以：
是因为接口中的方法是抽象的，并无方法体，无论这些接口中存在多少个同名的方法，由于无方法体，子类只需要覆盖一次即可，这个方法的具体实现只是通过子类这一个方法实现的。
3、接口之间是可以多继承的：
因为接口中存在的是抽象的方法，接口与接口之间无论是否存在同名函数，这些都是需要子类覆盖的，这样就不会出现无法判断覆盖哪一个的问题了。
## 4、接口的特点：
1、接口是对外暴露的规则
2、接口可以用来多实现
3、接口是程序的扩展功能
4、接口与接口之间可有继承关系
5、接口降低了耦合性
6、类与接口之间是实现关系，且类可以继承一个类的同时实现多个接口
接口关系：like-a；   类关系：has-a
需要注意的是：两个或多个接口中不可有不同返回类型的同名抽象函数。
如下列的A和B中的show是不允许的
```
interface A  
{  
    public abstract int show();  
}  
  
interface B  
{  
    public abstract boolean show();  
}  
  
interface C extends A,B  
{  
    public abstract void Cx();  
}
```
因为在类实现C的时候，是无法复写抽象方法show()的，因为不知道要返回哪种类型。
举例：
```
//抽象类  
abstract class Sporter  
{  
    abstract void play();  
}  
//三个接口  
interface Learn extends Study //学习
{  
    public void learn();  
}  
  
interface Study//研究  
{  
    public void study();  
}  
  
interface Smoking  
{  
    public abstract void smoke();  
}  
//类继承类，类实现接口，接口继承接口  
class WuDong extends Sporter implements Smoking,Learn  
{  
    //分别复写每个抽象方法  
    public void play()  
    {  
        System.out.println("Wu Playing");  
    }  
    public void Smoking()  
    {  
        System.out.println("Wu Smoking");  
    }  
    public void learn()  
    {  
        System.out.println("Wu Learning");  
    }  
    public void study()  
    {  
        System.out.println("Wu Studying");  
    }  
}  
  
class IntfaceDemo  
{  
    public static void main(String[] args)   
    {  
        //......  
    }  
}
```
# 三、抽象类和接口的异同
## 1、概述：
1、抽象类(abstract class)：一般仅用于被子类继承。
当多个类出现相同功能时，但功能主体不同，这样可以向上抽取，抽取时只抽取功能定义，而不抽取功能主体。也就是说，我们在从下往上看继承这个体系结构时，位于上层的类要比下层更具有通用性，也就是说更加抽象，即最上层的祖先（即超类）最具通用性。这时只讲上层的类作为遗传（或者说派生）下层类的基本的类，而不考虑特定的实例对象。
2、接口(interface)：用来建立类与类之间关联的标准
接口可理解为一种特殊的抽象类（但不是），当抽象类中的方法全为抽象的（即不包含任何非抽象方法），通过接口表示。
简单代码示例：
```
//抽象类  
abstract class A{  
    //可用各种修饰符声明  
    int x;  
    public int a;  
    private int b;  
    static int c;  
    final int d;  
    public static final int e;  
    //抽象方法必须有  
    abstract void function();  
    //还可以有非抽象方法  
    void fun(){  
        //.......  
    }  
}  
  
//接口  
interface B{  
    //声明必须加默认修饰符  
    public static final int a;  
    //方法必须是抽象的，不可有非抽象方法  
    abstract void function();  
}  
interface X{}  
interface Y{}  
//继承关系：抽象类和类间  
class C extends A{  
    void function(){  
    //......  
    }  
}  
//实现关系：类与接口间,可多实现  
class D implements B,X,Y{  
    //可直接访问B中的a  
    void function(){  
        //......  
    }  
}  
//多继承关系：接口与接口间  
interface E extends B,X implements Y  
{  
    //......  
}
```
## 2、区别和联系
### 一）区别：
1、与类间关系不同：
抽象类是一种被子类继承(extends)的关系，
而接口和类是一种实现(implements)关系，
接口和接口是继承(extends)关系。
注：
a.子类只能继承一个抽象类。
b.一个类却可以实现多个接口。
c.接口可以继承多个接口。
2、定义特点不同：
a.抽象类可以定义变量、非抽象方法以及抽象方法（必须有一个抽象方法）
变量：private、public、final、static等等修饰符
抽象方法：abstract(必须有)、public、static等等修饰符
b.接口可以定义常量、抽象方法
常量：public static final（都是存在的，如果没有会默认加上），赋值后，不可再次赋值
方法：public abstract
3、权限不同：
a.抽象类可以有私有变量或方法，子类继承抽象父类必须复写全部的抽象方法
b.接口是公开(public)的，里面不能有私有变量或方法，因为接口是对外暴露的，是提供给外界使用的；
实现接口必须重写接口中的全部抽象方法
4、成员不同：
a.抽象类中可以有自己的成员，也可以由非抽象的成员方法。
b.接口中只能有静态的不能被修改的成员变量（即常量），而且所有成员方法皆为抽象的。
5、变量类型不同：
a.抽象类中的变量默认是friendly型的，即包内的任何类都可以访问它，而包外的任何类
都不能访问它(包括包外继承了此类的子类)，其值可以在子类中重新定义，也可重新赋值。
b.接口中定义的变量是默认的public static final，且必须进行初始化即赋初值，并不可改变。
6、设计理念不同：
a.抽象类表示的是："si-a"的关系
b.接口表示的是："like-a"的关系
（组合是"has-a"的关系）
### 二）联系：
1.其实接口是抽象类的延伸，可以将它看做是纯粹的抽象类，就是说接口比抽象类还抽象。
2、抽象类和接口都必须被一个类（子类）复写里面的全部抽象方法。
3、接口和抽象类都不可创建对象，因为其中含有抽象方法，需要被子类实现后，
对接口中抽象方法全覆盖后，子类才可以实现实例化。
补充：
Java接口和Java抽象类有太多相似的地方，又有太多特别的地方，究竟在什么地方，才是它们的最佳位置呢？把它们比较一下，你就可以发现了。
1、Java接口和Java抽象类最大的一个区别，就在于Java抽象类可以提供某些方法的部分实现，而Java接口不可以，这大概就是Java抽象类唯一的优点吧，但这个优点非常有用。如果向一个抽象类里加入一个新的具体方法时，那么它所有的子类都一下子都得到了这个新方法，而Java接口做不到这一点，如果向一个Java接口里加入一个新方法，所有实现这个接口的类就无法成功通过编译了，因为你必须让每一个类都再实现这个方法才行，这显然是Java接口的缺点。
2、一个抽象类的实现只能由这个抽象类的子类给出，也就是说，这个实现处在抽象类所定义出的继承的等级结构中，而由于Java语言的单继承性，所以抽象类作为类型定义工具的效能大打折扣。在这一点上，Java接口的优势就出来了，任何一个实现了一个Java接口所规定的方法的类都可以具有这个接口的类型，而一个类可以实现任意多个Java接口，从而这个类就有了多种类型。
3、从第2点不难看出，Java接口是定义混合类型的理想工具，混合类表明一个类不仅仅具有某个主类型的行为，而且具有其他的次要行为。
4、结合1、2点中抽象类和Java接口的各自优势，具精典的设计模式就出来了：声明类型的工作仍然由Java接口承担，但是同时给出一个Java抽象类，且实现了这个接口，而其他同属于这个抽象类型的具体类可以选择实现这个Java接口，也可以选择继承这个抽象类，也就是说在层次结构中，Java接口在最上面，然后紧跟着抽象类，哈，这下两个的最大优点都能发挥到极至了。这个模式就是“缺省适配模式”。
在Java语言API中用了这种模式，而且全都遵循一定的命名规范：Abstract ＋接口名。
Java接口和Java抽象类的存在就是为了用于具体类的实现和继承的，如果你准备写一个具体类去继承另一个具体类的话，那你的设计就有很大问题了。Java抽象类就是为了继承而存在的，它的抽象方法就是为了强制子类必须去实现的。
使用Java接口和抽象Java类进行变量的类型声明、参数是类型声明、方法的返还类型说明，以及数据类型的转换等。而不要用具体Java类进行变量的类型声明、参数是类型声明、方法的返还类型说明，以及数据类型的转换等。
我想，如果你编的代码里面连一个接口和抽象类都没有的话，也许我可以说你根本没有用到任何设计模式，任何一个设计模式都是和抽象分不开的，而抽象与Java接口和抽象Java类又是分不开的。理解抽象，理解Java接口和抽象Java类，我想就应该是真正开始用面向对象的思想去分析问题，解决问题了吧。
**注：**
1、设计接口的目的就是为了实现C++中的多重继承，不过java团队设计的一样更有趣的东西
来实现这个功能，那就是内部类（inner class）
2、一般的应用里，最顶级的是接口，然后是抽象类实现接口，最后才到具体类实现。
不是很建议具体类直接实现接口的。还有一种设计模式是面向接口编程，而非面向实现编程。
