# 提高java反射速度的方法method.setAccessible(true) - z69183787的专栏 - CSDN博客
2013年11月21日 16:56:45[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1272
个人分类：[反射-机制](https://blog.csdn.net/z69183787/article/category/2175689)
java代码中，常常将一个类的成员变量置为private
在类的外面获取此类的私有成员变量的value时，需要注意:
测试类：
- publicclass AccessibleTest {  
- 
- privateint id;  
- private String name;  
- 
- public AccessibleTest() {  
- 
-     }  
- 
- publicint getId() {  
- return id;  
-     }  
- 
- publicvoid setId(int id) {  
- this.id = id;  
-     }  
- 
- public String getName() {  
- return name;  
-     }  
- 
- publicvoid setName(String name) {  
- this.name = name;  
-     }  
- 
- }  
Main类：
Java代码  ![收藏代码](http://usummer.iteye.com/images/icon_star.png)
- publicclass Main {  
- 
- publicstaticvoid main(String[] args) throws Exception {  
-         Class clazz = Class.forName("com.test.accessible.AccessibleTest");  
-         AccessibleTest at = new AccessibleTest();  
-         at.setId(1);  
-         at.setName("AT");  
- for (Field f : clazz.getDeclaredFields()) {  
-             f.setAccessible(true);//AccessibleTest类中的成员变量为private,故必须进行此操作
-             System.out.println(f.get(at));//获取当前对象中当前Field的value
-         }  
- 
-     }  
- 
- }  
如果没有在获取Field之前调用setAccessible(true)方法，异常：
Java代码  ![收藏代码](http://usummer.iteye.com/images/icon_star.png)
-  java.lang.IllegalAccessException:  
- Class com.test.accessible.Main   
- can not access   
- a member of class com.test.accessible.AccessibleTest  
- with modifiers "private"
当然在AccessibleTest类的内部(AccessibleTest的内部类除外)
进行如上操作则不需要调用setAccesible()方法
明显 Accessible并不是标识方法能否访问的. public的方法 Accessible仍为false 
使用了method.setAccessible(true)后 性能有了20倍的提升
Accessable属性是继承自AccessibleObject 类. 功能是启用或禁用安全检查
JDK API中的解释
引用
AccessibleObject 类是 Field、Method 和 Constructor 对象的基类。它提供了将反射的对象标记为在使用时取消默认 Java 语言访问控制检查的能力。对于公共成员、默认（打包）访问成员、受保护成员和私有成员，在分别使用 Field、Method 或 Constructor 对象来设置或获得字段、调用方法，或者创建和初始化类的新实例的时候，会执行访问检查。
在反射对象中设置 accessible 标志允许具有足够特权的复杂应用程序（比如 Java Object Serialization 或其他持久性机制）以某种通常禁止使用的方式来操作对象。
setAccessible 
public void setAccessible(boolean flag) 
                   throws SecurityException 
将此对象的 accessible 标志设置为指示的布尔值。值为 true 则指示反射的对象在使用时应该取消 Java 语言访问检查。值为 false 则指示反射的对象应该实施 Java 语言访问检查。
实际上setAccessible是启用和禁用访问安全检查的开关,并不是为true就能访问为false就不能访问
