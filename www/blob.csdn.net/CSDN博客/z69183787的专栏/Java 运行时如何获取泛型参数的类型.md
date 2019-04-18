# Java 运行时如何获取泛型参数的类型 - z69183787的专栏 - CSDN博客
2018年04月05日 18:46:18[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1041
[https://blog.csdn.net/hj7jay/article/details/54889717](https://blog.csdn.net/hj7jay/article/details/54889717)
https://blog.csdn.net/xiaozaq/article/details/52329321
在 Java 中对于下面最简单的泛型类
[java][view plain](https://blog.csdn.net/hj7jay/article/details/54889717#)[copy](https://blog.csdn.net/hj7jay/article/details/54889717#)
- class A<T> {  
- publicvoid foo() {  
- //如何在此处获得运行时 T 的具体类型呢？
-     }  
- }  
设想我们使用时
[java][view plain](https://blog.csdn.net/hj7jay/article/details/54889717#)[copy](https://blog.csdn.net/hj7jay/article/details/54889717#)
- new A<String>().foo();  
是否能在 foo() 方法中获得当前的类型是 String 呢？答案是否定的，不能。在 foo() 方法中 this 引用给不出类型信息， this.getClass() 就更不可能了，因为 Java 的泛型不等同于 C++ 的模板类， this.getClass() 实例例是被所有的不同具体类型的 A 实例(new A<String>(), new A<Integer>() 等) 共享的，所以在字节码中类型会被擦除到上限。
我们可以在 IDE 的调试时看到这个泛型类的签名
![](https://img-blog.csdn.net/20170206091914391?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGo3amF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
或者用 javap -v cc.unmi.A 可以查看到类 A 的泛型签名
Signature: #17                          // <T:Ljava/lang/Object;>Ljava/lang/Object;
为什么说是擦除到上限呢？并不是泛型在字节码中都表示为 Object , 看下面的例子，假如 A 声明如下
class A<T extends Number> {
}
再用 javap -v cc.unmi.A 来看泛型签名
Signature: #18                          // <T:Ljava/lang/Number;>Ljava/lang/Object;
也就是说在上面的 foo() 方法中无法获得当前的类型，我们必须给它加个参数 T
public void foo(T t) {
t.getClass();
}
了解了 Java 泛型机制是如何擦除类型的，我们接下来的问题就是如何通过反射获得泛型签名中的类型，一般会在继承或实现泛型接口时会用到它。
### 继承一个泛型基类
[java][view plain](https://blog.csdn.net/hj7jay/article/details/54889717#)[copy](https://blog.csdn.net/hj7jay/article/details/54889717#)
- class A<T, ID> {  
- }  
- 
- class B extends A<String, Integer> {  
- }  
- 
- publicclass Generic {  
- 
- publicstaticvoid main(String[] args) {  
-         System.out.println(B.class.getGenericSuperclass());  
-     }  
- }  
上面的代码输出是
cc.unmi.A<java.lang.String, java.lang.Integer>
所以要获得这两个类型是可行的，设置了断点
![](https://img-blog.csdn.net/20170206092231780?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGo3amF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
这张图可以看到 B.class.getGenericSuperclass() 得到的实际类型是 ParameterizedTypeImpl 通过它就可以获得 actualTypeArguments 了。代码就是
[java][view plain](https://blog.csdn.net/hj7jay/article/details/54889717#)[copy](https://blog.csdn.net/hj7jay/article/details/54889717#)
- ParameterizedType parameterizedType = (ParameterizedType) B.class.getGenericSuperclass();  
- Type[] actualTypeArguments = parameterizedType.getActualTypeArguments();  
- for(Type actualTypeArgument: actualTypeArguments) {  
-     System.out.println(actualTypeArgument);  
- }  
上面的代码输出
class java.lang.String
class java.lang.Integer
我们不妨用 javap -v cc.unmi.B 的泛型签名
Signature: #12                          // Lcc/unmi/A<Ljava/lang/String;Ljava/lang/Integer;>;
### 实现一个泛型接口
这时与继承一个泛型基类的情况略有不同，如下关系，A 是一个泛型接口
[java][view plain](https://blog.csdn.net/hj7jay/article/details/54889717#)[copy](https://blog.csdn.net/hj7jay/article/details/54889717#)
- interface A<T, ID> {  
- }  
- 
- class B implements A<String, Integer> {  
- }  
该如何反射获得 B 的参数类型呢，用上面的方法已不可行， B.class.getGenericSuperclass() 已不是一个 ParameterizedTypeImpl 而是一个 Object 类型。现在需要另一个方法 getGenericInterfaces(): Type[] 它得到一个 Type 数组，代表类实现的多个接口，因为我们这儿只实现了一个接口，所以取第一个元素，它的类型是我们已见过的 ParameterizedTypeImpl ,
![](https://img-blog.csdn.net/20170206092153717?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGo3amF5/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
因此我们用来获得实现接口而来的泛型参数的代码就是
[java][view plain](https://blog.csdn.net/hj7jay/article/details/54889717#)[copy](https://blog.csdn.net/hj7jay/article/details/54889717#)
- ParameterizedType parameterizedType = (ParameterizedType) B.class.getGenericInterfaces()[0];  
- Type[] actualTypeArguments = parameterizedType.getActualTypeArguments();  
- for (Type actualTypeArgument : actualTypeArguments) {  
-     System.out.println(actualTypeArgument);  
- }  
同样能得到上面的一样的结果。
### 总结一下
- 如果是继承基类而来的泛型，就用 getGenericSuperclass() , 转型为 ParameterizedType 来获得实际类型
- 如果是实现接口而来的泛型，就用 getGenericInterfaces() , 针对其中的元素转型为 ParameterizedType 来获得实际类型
- 我们所说的 Java 泛型在字节码中会被擦除，并不总是擦除为 Object 类型，而是擦除到上限类型
- 能否获得想要的类型可以在 IDE 中，或用 javap -v <your_class>   来查看泛型签名来找到线索
example：
获取几种形式泛型的Class类型：
ClassA类：
**[java]**[view plain](https://blog.csdn.net/xiaozaq/article/details/52329321#)[copy](https://blog.csdn.net/xiaozaq/article/details/52329321#)
- import java.lang.reflect.ParameterizedType;  
- import java.lang.reflect.Type;  
- 
- publicclass  ClassA <T>{         
- private T obj;         
- publicvoid setObject(T obj) {      this.obj = obj;  }       
- public T getObject() {    return obj;   }     
- 
- /**
-      * 获取T的实际类型
-      */
- publicvoid testClassA() throws NoSuchFieldException, SecurityException {  
-         System.out.print("getSuperclass:");  
-         System.out.println(this.getClass().getSuperclass().getName());  
-         System.out.print("getGenericSuperclass:");  
-         Type t = this.getClass().getGenericSuperclass();  
-         System.out.println(t);  
- if (ParameterizedType.class.isAssignableFrom(t.getClass())) {  
-             System.out.print("getActualTypeArguments:");  
- for (Type t1 : ((ParameterizedType) t).getActualTypeArguments()) {  
-                 System.out.print(t1 + ",");  
-             }  
-             System.out.println();  
-         }  
-     }  
- }      
Test类：
**[java]**[view plain](https://blog.csdn.net/xiaozaq/article/details/52329321#)[copy](https://blog.csdn.net/xiaozaq/article/details/52329321#)
- import java.lang.reflect.Type;  
- import java.util.List;  
- import java.util.Map;  
- import java.lang.reflect.ParameterizedType;  
- 
- publicclass Test extends ClassA<String> {  
- private List<String> list;  
- private Map<String, Object> map;  
- 
- /***
-      * 获取List中的泛型
-      */
- publicstaticvoid testList() throws NoSuchFieldException, SecurityException {  
-         Type t = Test.class.getDeclaredField("list").getGenericType();  
- if (ParameterizedType.class.isAssignableFrom(t.getClass())) {  
- for (Type t1 : ((ParameterizedType) t).getActualTypeArguments()) {  
-                 System.out.print(t1 + ",");  
-             }  
-             System.out.println();  
-         }  
-     }  
- 
- /***
-      * 获取Map中的泛型
-      */
- publicstaticvoid testMap() throws NoSuchFieldException, SecurityException {  
-         Type t = Test.class.getDeclaredField("map").getGenericType();  
- if (ParameterizedType.class.isAssignableFrom(t.getClass())) {  
- for (Type t1 : ((ParameterizedType) t).getActualTypeArguments()) {  
-                 System.out.print(t1 + ",");  
-             }  
-             System.out.println();  
-         }  
-     }  
- 
- publicstaticvoid main(String args[]) throws Exception {  
-         System.out.println(">>>>>>>>>>>testList>>>>>>>>>>>");  
-         testList();  
-         System.out.println("<<<<<<<<<<<testList<<<<<<<<<<<\n");  
-         System.out.println(">>>>>>>>>>>testMap>>>>>>>>>>>");  
-         testMap();  
-         System.out.println("<<<<<<<<<<<testMap<<<<<<<<<<<\n");  
-         System.out.println(">>>>>>>>>>>testClassA>>>>>>>>>>>");  
- new Test().testClassA();  
-         System.out.println("<<<<<<<<<<<testClassA<<<<<<<<<<<");  
-     }  
- 
- }  
结果：
**[plain]**[view plain](https://blog.csdn.net/xiaozaq/article/details/52329321#)[copy](https://blog.csdn.net/xiaozaq/article/details/52329321#)
- >>>>>>>>>>>testList>>>>>>>>>>>  
- class java.lang.String,  
- <<<<<<<<<<<testList<<<<<<<<<<<  
- 
- >>>>>>>>>>>testMap>>>>>>>>>>>  
- class java.lang.String,class java.lang.Object,  
- <<<<<<<<<<<testMap<<<<<<<<<<<  
- 
- >>>>>>>>>>>testClassA>>>>>>>>>>>  
- getSuperclass:com.pelin.util.ClassA  
- getGenericSuperclass:com.pelin.util.ClassA<java.lang.String>  
- getActualTypeArguments:class java.lang.String,  
- <<<<<<<<<<<testClassA<<<<<<<<<<<  
