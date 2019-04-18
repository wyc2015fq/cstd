# J​A​V​A​中​c​l​o​n​e​方​法​详​解 - z69183787的专栏 - CSDN博客
2014年05月19日 21:10:05[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1537
**预备知识** @@@@@@@@@@@@@@@@@@@@@@
为了理解java的clone，有必要先温习以下的知识。 
java的类型，java的类型分为两大类，一类为primitive，如int，另一类为引用类型,如String,Object等等。 
java引用类型的存储，java的**引用类型都是存储在堆上的**。 
**Java代码 **
1.  **publicclass** B {  
2.  **int** a;  
3.      String b;  
4.  
5.  **public** B(**int** a, String b) {  
6.  **super**();  
7.  **this**.a = a;  
8.  **this**.b = b;  
9.      }  
10. }  
对这样一个引用类型的实例，我们可以推测，在堆上它的内存存储形式（除去指向class的引用，锁的管理等等内务事务所占内存），应该有一个int值表示a,以及一个引用，该引用指向b在堆上的存储空间。 
**为什么要clone**
恩，因为需要。废话。 
有名的GoF设计模式里有一个模式为原型模式，用原型实例指定创建对象的种类,并且通过拷贝这些原型创建新的对象. 
简单的说就是clone一个对象实例。使得clone出来的copy和原有的对象一模一样。 
插一个简单使用clone的例子，如果一个对象内部有可变对象实例的话，public API不应该直接返回该对象的引用，以防调用方的code改变该对象的内部状态。这个时候可以返回该对象的clone。 
问题来了，什么叫一模一样。 
一般来说，有 
x.clone()!= x 
x.clone().getClass()== x.getClass() 
x.clone().equals(x) 
但是这些都不是强制的。 
我们需要什么样的clone就搞出什么样的clone好了。 
一般而言，我们要的clone应该是这样的。copy和原型的内容一样，但是又是彼此隔离的。即在clone之后，改变其中一个不影响另外一个。 
**Object的clone以及为什么如此实现**
Object的clone的行为是最简单的。以堆上的内存存储解释的话（不计内务内存），对一个对象a的clone就是在堆上分配一个和a在堆上所占存储空间一样大的一块地方，然后把a的堆上内存的内容复制到这个新分配的内存空间上。 
看例子。 
**Java代码 **
1.  **class** User {  
2.      String name;  
3.  **int** age;  
4.  }  
5.  
6.  **class** Account **implementsCloneable** {  
7.      User user;  
8.  **long** balance;  
9.  
10. @Override
11. **public** Object clone()**throws** CloneNotSupportedException {  
12. **returnsuper**.clone();  
13.     }  
14. }  
**Java代码 **
1.  // user.
2.  User user = **new** User();  
3.  user.name = "user";  
4.  user.age = 20;  
5.  // account.
6.  Account account = **new** Account();  
7.  account.user = user;  
8.  account.balance = 10000;  
9.  // copy.
10. Account copy = (Account) **account.clone()**;  
11. 
12. // balance因为是**基本类型，所以****copy和原型是相等且独立的**。
13. Assert.assertEquals(copy.balance, account.balance);  
14. copy.balance = 20000;  
15. // 改变copy不影响原型。
16. Assert.assertTrue(copy.balance != account.balance);  
17. 
18. // user因为是**引用类型，所以****copy和原型的引用是同一的**。
19. Assert.assertTrue(copy.user == account.user);  
20. copy.user.name = "newName";  
21. // 改变的是同一个东西，**原形也跟着改变了**。
22. Assert.assertEquals("newName", account.user.name);  
恩，默认实现是帮了我们一些忙，但是不是全部。 
primitive的确做到了相等且隔离。 
**引用类型仅仅是复制了一下引用，copy和原型引用的东西是一样的**。 
这个就是所谓的**浅copy**了。 
要实现深copy，**即复制原型中对象的内存copy**，**而不仅仅是一个引用**。只有自己动手了。 
等等，是不是所有的引用类型都需要深copy呢？ 
不是！ 
我们之所以要深copy，是因为默认的实现提供的浅copy不是隔离的，换言之，改变copy的东西，会影响到原型的内部。比如例子中，改变copy的user的name，影响了原型。 
如果我们要copy的类是不可变的呢，如String，没有方法可以改变它的内部状态呢。 
**Java代码 **
1.  **class** User **implements** Cloneable {  
2.      String name;  
3.  **int** age;  
4.  
5.  @Override
6.  **public** Object clone()**throws** CloneNotSupportedException {  
7.  **returnsuper**.clone();  
8.      }  
9.  }  
**Java代码 **
1.  // user.
2.  User user = **new** User();  
3.  user.name = "user";  
4.  user.age = 20;  
5.  
6.  // copy
7.  User copy = (User) user.clone();  
8.  
9.  // age因为是primitive，所以copy和原型是相等且独立的。
10. Assert.assertEquals(copy.age, user.age);  
11. copy.age = 30;  
12. // 改变copy不影响原型。
13. Assert.assertTrue(copy.age != user.age);  
14. 
15. // **name****因为是引用类型，所以copy和原型的引用是同一的**。
16. Assert.assertTrue(copy.name == user.name);  
**17. **//** String****为不可变类。没有办法可以通过对copy.name的字符串的操作改变这个字符串。**
**18. // 改变引用新的对象不会影响原型。**
19. copy.name = "newname";  
20. Assert.assertEquals("newname", copy.name);  
21. Assert.assertEquals("user", user.name);  
可见，在考虑clone时，**primitive**和**不可变对象类型是可以同等对待的（都不会影响原形）**。 
java为什么如此实现clone呢？ 
也许有以下考虑。 
1 效率和简单性，简单的copy一个对象在堆上的的内存比遍历一个对象网然后内存深copy明显效率高并且简单。 
2 不给别的类强加意义。如果A（例一中的account）实现了Cloneable，**同时有一个引用指向B（例一中的user）**，如果直接复制内存进行深copy的话，**意味着B在意义上也是支持Clone的**，但是这个是在使用B的A中做的，B甚至都不知道。破坏了B原有的接口。 
3 有可能破坏语义。如果A实现了Cloneable，同时有一个引用指向B，该B实现为单例模式，如果直接复制内存进行深copy的话，破坏了B的单例模式。
4 方便且更灵活，如果A引用一个不可变对象，则内存deep copy是一种浪费。Shadow copy给了程序员更好的灵活性。 
**如何clone**
clone三部曲。 
1 声明实现Cloneable接口。 
2 调用super.clone拿到一个对象，如果父类的clone实现没有问题的话，在该对象的内存存储中，所有父类定义的field都已经clone好了，该类中的primitive和不可变类型引用也克隆好了，可变类型引用都是浅copy。 
3 把浅copy的引用指向原型对象新的克隆体。 
给个例子。 
**Java代码 **
1.  **class** User **implements** Cloneable {  
2.      String name;  
3.  **int** age;  
4.  
5.  @Override
6.  **public** User clone()**throws** CloneNotSupportedException {  
7.  **return** (User) **super**.clone();  
8.      }  
9.  }  
10. 
11. **class** Account **implements** Cloneable {  
12.     User user;  
13. **long** balance;  
14. 
15. @Override
16. **public** Account clone()**throws** CloneNotSupportedException {  
17.         Account account = **null**;  
18. 
19.         account = (Account) **super**.clone();  
20. **if** (user != **null**) { //分离了对user的引用 
21.             account.user = user.clone();  
22.         }  
23. 
24. **return** account;  
25.     }  
26. }  
**对clone的态度**
clone嘛，我觉得是个好东西，毕竟系统默认实现已经帮我们做了很多事情了。 
但是它也是有缺点的。 
1 手工维护clone的调用链。这个问题不大，程序员有责任做好。 
2 如果class的field是个final的可变类，就不行了。三部曲的第三步没有办法做了。 
考虑一个类对clone的态度，有如下几种。 
1 公开支持：好吧，按照clone三部曲实现吧。前提是父类支持（公开或者默默）。 
2 默默支持：不实现Cloneable接口，但是在类里面有正确的protected的clone实现，这样，该类不支持clone，但是它的子类如果想支持的话也不妨碍。 
3 不支持：好吧，为了明确该目的，提供一个抛CloneNotSupportedException 异常的protected的clone实现。 
4 看情况支持：该类内部可以保存其他类的实例，如果其他类支持则该类支持，如果其他类不支持，该类没有办法，只有不支持。 
**其他的选择**
可以用原型构造函数，或者静态copy方法来手工制作一个对象的copy。 
好处是即使class的field为final，也不会影响该方法的使用。不好的地方是所有的primitive赋值都得自己维护。 
**和Serializable的比较**
使用Serializable同样可以做到对象的clone。但是： 
Cloneable本身就是为clone设计的，虽然有一些缺点，但是如果它可以clone的话无疑用它来做clone比较合适。如果不行的话用原型构造函数，或者静态copy方法也可以。 
Serializable制作clone的话，添加了太多其它的东西，增加了复杂性。 
1 所有的相关的类都得支持Serializable。这个相比支持Cloneable只会工作量更大 
2Serializable添加了更多的意义，除了提供一个方法用Serializable制作Clone，该类等于也添加了其它的public API，如果一个类实现了Serializable，等于它的2进制形式就已经是其API的一部分了，不便于该类以后内部的改动。 
3 当类用Serializable来实现clone时，用户如果保存了一个老版本的对象2进制，该类升级，用户用新版本的类反系列化该对象，再调用该对象用Serializable实现的clone。这里为了一个clone的方法又引入了类版本兼容性的问题。不划算。 
**性能**
不可否认，JVM越来越快了。 
但是系统默认的native实现还是挺快的。 
clone一个有100个元素的int数组，用系统默认的clone比静态copy方法快2倍左右。
**1．浅复制与深复制概念 **
⑴浅复制（浅克隆） 
被复制对象的所有变量都含有与原来的对象相同的值，而所有的对其他对象的引用仍然指向 原来的对象。换言之，浅复制仅仅复制所考虑的对象，而不复制它所引用的对象。 
⑵深复制（深克隆） 
被复制对象的所有变量都含有与原来的对象相同的值，除去那些引用其他对象的变量。那些 引用其他对象的变量将指向被复制过的新对象，而不再是原有的那些被引用的对象。换言之，深复制把要复制的对象所引用的对象都复制了一遍。
**2．Java的clone（）方法 **
⑴clone方法将对象复制了一份并返回给调用者。一般而言，clone（）方法满足： 
①对任何的对象x，都有x.clone() !=x//克隆对象与原对象不是同一个对象 
②对任何的对象x，都有x.clone().getClass()= =x.getClass()//克隆对象与原对象的类型一样 
③如果对象x的equals()方法定义恰当，那么x.clone().equals(x)应该成立。 
⑵Java中对象的克隆 
①为了获取对象的一份拷贝，我们可以利用Object类的clone()方法。 
②在派生类中覆盖基类的clone()方法，并声明为public。 
③在派生类的clone()方法中，调用super.clone()。 
④在派生类中实现Cloneable接口。 
请看如下代码： 
class Student implements Cloneable 
{ 
    String name; 
    int age; 
    Student(String name,int age) 
    { 
        this.name=name; 
        this.age=age; 
    } 
    public Object clone() 
    { 
        Object o=null; 
        try 
        { 
        o=(Student)super.clone();//Object中的clone()识别出你要复制的是哪一个对象。 
        } 
        catch(CloneNotSupportedException e) 
        { 
           System.out.println(e.toString()); 
        } 
        return o; 
    } 
} 
public static void main(String[] args) 
    { 
      Student s1=new Student("zhangsan",18);
      Student s2=(Student)s1.clone(); 
      s2.name="lisi"; 
     s2.age=20; 
System.out.println("name="+s1.name+","+"age="+s1.age);//修改学生2后，不影响学生1的值。 
   } 
说明： 
①为什么我们在派生类中覆盖Object的clone()方法时，一定要调用super.clone()呢？在运行时 刻，Object中的clone()识别出你要复制的是哪一个对象，然后为此对象分配空间，并进行对象的复制，将原始对象的内容一一复制到新对象的存 储空间中。
②继承自java.lang.Object类的clone()方法是浅复制。以下代码可以证明之。 
class Professor 
{ 
    String name; 
    int age; 
    Professor(String name,int age) 
    { 
        this.name=name; 
        this.age=age; 
    } 
} 
class Student implements Cloneable 
{ 
    String name;//常量对象。 
    int age; 
    Professor p;//学生1和学生2的引用值都是一样的。 
    Student(String name,int age,Professor p) 
    { 
        this.name=name; 
        this.age=age; 
        this.p=p; 
    } 
    public Object clone() 
    { 
        Student o=null; 
        try 
        { 
           o=(Student)super.clone(); 
        } 
        catch(CloneNotSupportedException e) 
        { 
           System.out.println(e.toString()); 
        } 
        o.p=(Professor)p.clone(); 
        return o; 
    } 
} 
public static void main(String[] args) 
    { 
      Professor p=newProfessor("wangwu",50); 
      Student s1=newStudent("zhangsan",18,p); 
      Student s2=(Student)s1.clone(); 
      s2.p.name="lisi"; 
     s2.p.age=30; 
System.out.println("name="+s1.p.name+","+"age="+s1.p.age);//学生1的教授成为lisi,age为30。 
} 
那应该如何实现深层次的克隆，即修改s2的教授不会影响s1的教授？代码改进如下。 
改进使学生1的Professor不改变（深层次的克隆） 
class Professor implements Cloneable 
{ 
    String name; 
    int age; 
    Professor(String name,int age) 
    { 
        this.name=name; 
        this.age=age; 
    } 
    public Object clone() 
    { 
        Object o=null; 
        try 
        { 
           o=super.clone(); 
        } 
        catch(CloneNotSupportedException e) 
        { 
           System.out.println(e.toString()); 
        } 
        return o; 
    } 
} 
class Student implements Cloneable 
{ 
    String name; 
    int age; 
    Professor p; 
    Student(String name,int age,Professor p) 
    { 
        this.name=name; 
        this.age=age; 
        this.p=p; 
    } 
    public Object clone() 
    { 
        Student o=null; 
        try 
        { 
           o=(Student)super.clone(); 
        } 
        catch(CloneNotSupportedException e) 
        { 
           System.out.println(e.toString()); 
        } 
        o.p=(Professor)p.clone(); 
        return o; 
    } 
} 
public static void main(String[] args) 
    { 
      Professor p=newProfessor("wangwu",50); 
      Student s1=newStudent("zhangsan",18,p); 
      Student s2=(Student)s1.clone(); 
      s2.p.name="lisi"; 
     s2.p.age=30; 
System.out.println("name="+s1.p.name+","+"age="+s1.p.age);//学生1的教授不改变。 
} 
3．利用串行化来做深复制 
把对象写到流里的过程是串行化（Serilization）过程，但是在Java程序师圈子里又非常形象地称为“冷冻”或者“腌咸菜（picking）”过程；而把对象从流中读出来的并行化（Deserialization）过程则叫做 “解冻”或者“回鲜(depicking)”过程。应当指出的是，写在流里的是对象的一个拷贝，而原对象仍然存在于JVM里面，因此“腌成咸菜”的只是对 象的一个拷贝，Java咸菜还可以回鲜。
在Java语言里深复制一个对象，常常可以先使对象实现Serializable接 口，然后把对象（实际上只是对象的一个拷贝）写到一个流里（腌成咸菜），再从流里读出来（把咸菜回鲜），便可以重建对象。
如下为深复制源代码。 
public Object deepClone() 
{ 
//将对象写到流里 
ByteArrayOutoutStream bo=new ByteArrayOutputStream(); 
ObjectOutputStream oo=new ObjectOutputStream(bo); 
oo.writeObject(this); 
//从流里读出来 
ByteArrayInputStream bi=new ByteArrayInputStream(bo.toByteArray()); 
ObjectInputStream oi=new ObjectInputStream(bi); 
return(oi.readObject()); 
} 
这样做的前提是对象以及对象内部所有引用到的对象都是可串行化的，否则，就需要仔细考 察那些不可串行化的对象可否设成transient，从而将之排除在复制过程之外。上例代码改进如下。
class Professor implements Serializable 
{ 
    String name; 
    int age; 
    Professor(String name,int age) 
    { 
        this.name=name; 
        this.age=age; 
    } 
} 
class Student implements Serializable 
{ 
    String name;//常量对象。 
    int age; 
    Professor p;//学生1和学生2的引用值都是一样的。 
    Student(String name,int age,Professor p) 
    { 
        this.name=name; 
        this.age=age; 
        this.p=p; 
    } 
    public Object deepClone() throws IOException, 
OptionalDataException,ClassNotFoundException 
{ 
//将对象写到流里 
ByteArrayOutoutStream bo=new ByteArrayOutputStream(); 
ObjectOutputStream oo=new ObjectOutputStream(bo); 
oo.writeObject(this); 
//从流里读出来 
ByteArrayInputStream bi=new ByteArrayInputStream(bo.toByteArray()); 
ObjectInputStream oi=new ObjectInputStream(bi); 
return(oi.readObject()); 
} 
} 
public static void main(String[] args) 
    { 
      Professor p=newProfessor("wangwu",50); 
      Student s1=newStudent("zhangsan",18,p); 
      Student s2=(Student)s1.deepClone(); 
      s2.p.name="lisi"; 
     s2.p.age=30; 
System.out.println("name="+s1.p.name+","+"age="+s1.p.age);//学生1的教授不改变。 
}
