# java基础三大特性——封装、继承与多态 - z69183787的专栏 - CSDN博客
2013年12月03日 20:44:11[OkidoGreen](https://me.csdn.net/z69183787)阅读数：829
个人分类：[面试](https://blog.csdn.net/z69183787/article/category/2185959)
**最近上课刚学习到java面向对象三大特性，巩固一下基础，对学习的内容做下总结，加深印象，主要介绍如何注意三大特性的特点和使用，这里主要收藏了学习使用的课件和个人总结，希望能为初学者带来帮助。**
**面向对象的三大特性：**
**封装：**主要实现了隐藏细节，对用户提供访问接口，无需关心方法的具体实现。
**继承：**实现了代码的复用，提高了编程效率。
**多态：**程序的可扩展性及可维护性增强。
**继承**是指在父类的基础上扩展功能，继承中分为子类和父类。
类有两种重要成员：成员变量和方法。子类的成员中可以有自己声明定义的变量，也有从父类继承的。
1.子类继承父类的成员变量，子类拥有父类的成员变量。
                       例如：父类中有姓名、年龄，子类不用定义变量也可以定义自己的姓名、年龄
2.子类继承父类的方法，可实现方法重写，子类拥有自己独特的特点。
                       例如：父 public  String getType(){ return "父类"}
                             子 public String  getType(){return "子类"}
 (1) Java只支持单一继承（只有一个父类），利用**extends**关键字继承；
class A{            }
class B{            }
class C extends A,B{    //错误，同时继承了两个父类          }
(2)Java中允许多层继承
class A{         }
class B extends A{          }
class C extends B{          }
(3)子类和父类有相同的方法(即返回类型、方法名、参数列表完全相同时，仅方法体不一样)，称为**方法重写**(OverRide)；
1.**子类重写了父类的方法，运行时系统调用子类重写的方法实现；**未重写，子类对象调用父类的方法实现****
父类F：public  void  print(){System.out.println(“你好，我是父类！");}
子类A：public  void  print(){System.out.println(“你好，我是子类！");} //重写父类方法
A a=new A();   a.print(); 执行结果是：**你好，我是子类****!**
A中没有重写print()方法，则A a=new A();    a.print(); 结果是：**你好，我是父类！**
**2重写父类方法时不可以降低方法的访问权限**
父类：public  String  getName(){  ***** }
子类：protected String getName(){ *****  }    //重写错误
**3重写了父类方法后使用父类的被重写方法M，可以在子类的方法中使用super.M（）;**
父类：public void  show( ){ *********}        
子类：public void  show( ){super.show() ; ******** }
**4子类的构造方法调用父类的构造方法**
父类：public F( ){     }           //无参构造方法为例
子类：public A( ){    super() ; //可有可无 }
**5.带参构造方法使用，子类使用父类定义的属性可以调用父类带参构造方法**
父类：public F(String a,int b){ this.a=a;    this.b=b;}         
子类：public A(String a,int b,String c){super(a,b) ;    this.c=c;  }
**抽象类(Abstract Class)**
抽象类提供一个类型的部分实现，可以有实例变量，构造方法，抽象方法(Abstract Method 即不能有方法的实现，没有**{}**)和具体方法。一个**抽象类不会有实例**；它是用来继承的。**它反映了一种一般/特殊化的关系**。
用关键字abstract修饰的类称为abstract类（抽象类）。
abstract class A
{ …       }**abstract类特点:1.**abstract类可以有abstract方法。
            对于abstract方法，**只允许声明，不允许实现**，不允许使用**final**修饰。
abstract class A
{     abstract  int min(int x,int y);
      int max(int x,int y){  return x>y?x:y; }
}
2.abstract类不能用new运算创建对象：需产生其子类，由子类创建对象，如果一个类是abstract类的子类，它必须具体实现父类的abstract方法，这就是不允许使用final修饰的原因。
        一个abstract类只关心它的子类是否具有某种功能，并不关心功能的具体行为，功能的具体行为由子类负责实现，抽象类中的抽象方法可以强制子类必须给出这些方法的具体实现。
**多态**
多态性就是指父类的某个方法被其子类重写时，可以各自产生自己的功能行为。简单来说，多态是具有表现多种形态的能力的特征。
当一个类有很多子类时，并且这些子类都重写了父类中的某个方法。那么当我们把子类创建的对象的引用放到一个父类的对象中时，就得到了该对象的一个上转型对象，那么这个上转的对象在调用这个方法时就可能具有多种形态.
** 多态特点**
1.   对于一个引用类型的变量，**Java编译器按照它的声明的类型来处理**
2.   对于一个引用类型的变量，**运行时Java虚拟机按照它的实际引用的对象来处理**
               1、子类重写父类的方法
               2、编写方法时，调用父类定义的方法
               3、运行时，根据实际创建的对象类型动态决定使用哪个方法
例如：让父类引用指向子类对象；如**Person p1 = new Student();**//引用的类型是Person，指向子类对象Student
**接口**
**接口声明**
接口与类属于同一层次，实际上，接口是一种特殊的抽象类。在Java中接口定义同类的定义很相似，分为接口声明和接口体两部分。接口通过interface关键字声明，接口体中只能包含全局常量和抽象方法，接口的变量自动是public
 static final属性，接口中的方法自动是public abstract属性，并没有方法体，以分号结尾。
格式:** [public] interface 接口名 [extends 父接口名表]{ 接口体；}**
接口体包含常量定义和方法定义，
**例如:**
interface IA{
public static finalint MAX=100; //全局常量
   public void print();  //抽象方法
   public abstract int sum();    //抽象方法
}
**注：**
**1)public interface：公开接口，与类相似，一个文件只能有一个public接口，且与文件名相同。**
**2)在一个文件中不可同时定义一个public接口和一个public类。**
**3)接口和接口之间可以定义继承关系，并且接口之间允许实现多重继承。****例：interface IC extends IA,IB{   };**
**接口使用**
**1.一个类通过关键字implements实现接口：**
**格式：**[修饰符] class  类名 [extends 父类] [implements 接口]{}
     非抽象类，必须实现接口的所有方法;
   抽象类的非抽象子类中对其父类所实现的接口中的所有抽象方法都必须实现。
** 2**.**一个类除继承另外一个类，还可以实现接口：**
class IAImpl extends java.util.Arrylist implement IA，IB{}
1)       一个类实现了某个接口，则必须实现接口中的所有方法，如果没有实现接口中的所有方法，则此类一定是抽象类。因为一个类实现接口，相当于它继承一个抽象类。
2)       接口中方法可不写public，但在子类中实现接口的过程中public不可省。（如果省去public则在编译的时候提示出错：对象无法从接口中实现方法，相当于降低了重写方法的权限。）
3)       如果父类实现了某个接口，子类不需要使用implements关键字声明就可以使用此接口。
4) 接口也可以用于定义对象
**                        IA iA=new IAImpl();**//类似于上转型对象
**上转型对象特点：**
           n  上转对象不能操作子类新增的成员变量（失掉了这部分属性）；不能使用子类新增的方法（失掉了一些功能）。
           n  上转型对象可以操作子类继承或隐藏成员变量，也可以使用子类继承的或重写的方法。
           n  上转型对象操作子类继承或重写的方法时，就是通知对应的子类对象去调用这些方法。因此，如果子类重写了父类的某个方法后，对象的上转型对象调用这个方法时，一定是调用了这个重写的方法。
           n  可以将对象的上转型对象再强制转换到一个子类对象，这时，该子类对象又具备了子类所有属性和功能。
**接口使用案例： **
**[java]**[view
 plain](http://blog.csdn.net/luran_fighting/article/details/13505579#)[copy](http://blog.csdn.net/luran_fighting/article/details/13505579#)[print](http://blog.csdn.net/luran_fighting/article/details/13505579#)[?](http://blog.csdn.net/luran_fighting/article/details/13505579#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/88382/fork)
- <span style="font-family:Arial;">interface IDetail{  
- public String detail();  
- }  
- 
- class Teacher implements IDetail {  
- private String name;  
- privateint age;  
- private String type;  
- public Teacher(String name, int age, String type) {  
- this.name = name;  
- this.age = age;  
- this.type = type;  
-        }  
- // 返回教师详细信息
- public String detail() {  
- return"姓名：" + name + ",年龄：" + age + "，教师类型：" + type;  
-        }  
- }  
- 
- class Printer {  
- publicvoid print(String s) {  
-               System.out.println(s);  
-        }  
- }  
- 
- class MedicalInspection implements IDetail{  
- 
-        Printer printer = new Printer();  
- // 返回体检中心信息
- public String detail() {  
- return"这里是滨州市体检中心，欢迎您的到来！";  
-        }  
- 
- // 打印detail信息
- publicvoid printDetail(IDetail d) {  
-               printer.print(d.detail());  
-        }  
- }  
- 
- publicclass ExDemon03 {  
- 
- publicstaticvoid main(String[] args) {  
- 
-               MedicalInspection mi=new MedicalInspection();  
-               IDetail t = new Teacher("李四", 33, "Java");  
- 
-               mi.printDetail();// 打印教师信息
-                 mi.printDetail(mi);// 打印中心信息
-        }  
- }  
- </span>  
