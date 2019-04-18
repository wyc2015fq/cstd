# Java静态内部类 - z69183787的专栏 - CSDN博客
2014年06月06日 11:14:56[OkidoGreen](https://me.csdn.net/z69183787)阅读数：592
个人分类：[面试](https://blog.csdn.net/z69183787/article/category/2185959)
**两种内部类**
Java的内部类有两种，一种是静态内部类，另一种是普通内部类，普通内部类可以获得外部对象的引用，
所以在普通内部类能够访问外部对象的成员变量 ，也就能够使用外部类的资源，可以说普通内部类依赖
于外部类，普通内部类与外部类是共生共死的，创建普通内部类的对象之前，必须先创建外部类的对象。
创建普通内部类的代码如下：
Outer o = new Outer();
Outer.Inner inner = o.new Inner();
Outer.Inner inner = new o.Inner();         /* 错误 */
Outer.Inner inner = new Outer.Inner();  /* 错误 */
静态内部类没有外部对象的引用，所以它无法获得外部对象的资源，当然好处是，静态内部类无需依赖于
外部类，它可以独立于外部对象而存在。创建静态内部类的代码如下：
Outer.Inner inner = new Outer.Inner();
**静态类的使用场景**
在以下场合可以使用静态内部类:
(1) 外部类需要使用内部类，而内部类无需使用外部类的资源
(2) 内部类可以独立外部类创建对象
使用静态内部类的好处是加强了代码的封装性以及提高了代码的可读性，举个例子：
Java代码  ![收藏代码](http://kenby.iteye.com/images/icon_star.png)
- publicclass Person{    
- //姓名  
- private String name;    
- //家庭  
- private Home home;    
- //构造函数设置属性值  
- public Person(String _name){    
-           name = _name;    
-      }    
- /* home、name的getter/setter方法省略 */
- 
- publicstaticclass Home{    
- //家庭地址  
- private String address;    
- //家庭电话  
- private String tel;    
- 
- public Home(String _address,String _tel){    
-             address = _address;    
-             tel = _tel;    
-           }    
- /* address、tel的getter/setter方法省略 */
-      }    
- }   
把Home放在Person内部，一看便知道Home是Person的一个属性。使用的时候也很方便，如下代码：
Java代码  ![收藏代码](http://kenby.iteye.com/images/icon_star.png)
- publicstaticvoid main(String[] args) {    
-     Home home = new Person.Home("上海", "021");  
-     Person p1 = new Person("张三");    
-     Person p2 = new Person("李四");    
-     p1.setHome(home);    
-     p2.setHome(home);    
- }   
这里创建一个home对象，p1和p2都使用这个home对象，p1和p2共享同一个home对象。
如果把Home换成普通内部类，情况就不同了，这时代码变成：
Java代码  ![收藏代码](http://kenby.iteye.com/images/icon_star.png)
- publicstaticvoid main(String[] args) {    
-     Person p1 = new Person("张三");    
-     Home home = p1.new Home("上海", "021");  
-     p1.setHome(home);    
-     Person p2 = new Person("李四");    
-     p2.setHome(home);    
- }   
这里p1和p2依然共享同一个home对象，但此时home对象和p1是同生共死的，如果p1对象消亡，那么p2就没有
家了，这对p2来说实在不公平，而如果为p1和p2都创建一个Home对象，又浪费资源。所以在这个例子中，
使用静态内部类比普通内部类要合适。
**静态内部类和普通内部类的区别**
（1）普通内部类不能声明static的方法和变量
普通内部类不能声明static的方法和变量，注意这里说的是变量，常量（也就是final static修饰的属性）
还是可以的，而静态内部类形似外部类，没有任何限制。
（2）使用静态内部类，多个外部类的对象可以共享同一个内部类的对象。
使用普通内部类，每个外部类的对象都有自己的内部类对象，外部对象之间不能共享内部类的对象
