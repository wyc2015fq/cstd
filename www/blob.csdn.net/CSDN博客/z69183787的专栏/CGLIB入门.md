# CGLIB入门 - z69183787的专栏 - CSDN博客
2016年10月27日 06:45:07[OkidoGreen](https://me.csdn.net/z69183787)阅读数：417
个人分类：[反射-Cglib](https://blog.csdn.net/z69183787/article/category/6484334)
1. Enhancer和MethodInterceptor
Enhancer可以用来动态的生成一个类，这个类可以继承指定的一个类，实现指定的一些接口。
同时,Enhancer在生成一个类之前需要指定一个Callback，当类方法调用时，方法的执行被分配给这个Callback
MethodInterceptor是一个使用比较多的继承自Callback的接口，它只有一个方法声明
Java代码  
- publicinterface MethodInterceptor extends Callback {
- public Object intercept(Object obj, java.lang.reflect.Method method, Object[] args,MethodProxy proxy) throws Throwable;
  
- }  
- 
我们看一下 JDK自带的Invocationhandler接口中的方法声明
Java代码  ![收藏代码](http://isuifengfei.iteye.com/images/icon_star.png)
- publicinterface InvocationHandler {  
- public Object invoke(Object proxy, Method method, Object[] args) throws Throwable;  
- }  
- 
 从参数构成上，methodInterceptor的输入参数比Invocationhandler多1个，其实前3个参数对象的含义与Invocationhandler的含义是相同的
第一个参数表示调用方法来自哪个对象；
第二个参数表示调用方法的Method对象；
第三个参数表示此次调用的输入参数列表；
多出来的参数是MethodProxy 类型的，它应该是cglib生成用来代替Method对象的一个对象，使用MethodProxy比调用JDK自身的Method直接执行方法效率会有提升
代码示例：
Java代码  ![收藏代码](http://isuifengfei.iteye.com/images/icon_star.png)
- import java.lang.reflect.Method;  
- 
- import net.sf.cglib.proxy.Enhancer;  
- import net.sf.cglib.proxy.MethodInterceptor;  
- import net.sf.cglib.proxy.MethodProxy;  
- 
- 
- publicclass MyMethodInterceptor implements MethodInterceptor {  
- 
- // 接口1
- staticinterface Inter1{  
- publicvoid fun1();  
-     }  
- // 接口2
- staticinterface Inter2{  
- public String fun2(String arg0);  
-     }  
- 
- // 内部方法
- public String myFun1(String arg0){  
- return"hello," + arg0 ;  
-     }  
- 
- public Object intercept(Object obj, Method method, Object[] args, MethodProxy proxy) throws Throwable {  
-         String methodName = method.getName();  
- 
- if( "fun1" .equals(methodName) ){  
-             System. out .println( "[intercept] fun1 invoked" );  
- returnnull;  
-         } elseif ( "fun2" .equals(methodName) ){  
-             System. out .println( "[intercept] fun2 invoked before" );  
-             String result = (String)args[0] + "..." ;  
-             System. out .println( result );  
-             System. out .println( "[intercept] fun2 invoked after" );  
- return result;  
-         } elseif ( "myFun1" .equals(methodName) ){  
-             System. out .println( "[intercept] myFun1 invoked before" );  
-             Object result = proxy. invokeSuper(obj, args);  
-             System. out .println( result );  
-             System. out .println( "[intercept] myFun1 invoked after" );  
- return result;  
-         }  
- 
- returnnull;  
-     }  
- 
- public Object createProxy(){  
-         Enhancer enhancer = new Enhancer();  
-         enhancer.setSuperclass(MyMethodInterceptor. class );  
-         enhancer.setInterfaces( new Class[]{Inter1. class,Inter2. class});  
-         enhancer.setCallback( this );  
- return enhancer.create();  
-     }  
- 
- 
- publicstaticvoid main(String[] args) {  
-         MyMethodInterceptor ss = new MyMethodInterceptor();  
-         Object proxy = ss.createProxy();  
- 
- // 接口
-         Inter1 inter1 = (Inter1)proxy;  
-         inter1.fun1();  
- 
-         Inter2 inter2 = (Inter2)proxy;  
-         inter2.fun2( "code generate library" );  
- 
- // 类
-         MyMethodInterceptor c1 = (MyMethodInterceptor)proxy;  
-         c1.myFun1( "cglib" );  
- 
-     }  
- 
- }  
- 
 执行结果：
[intercept] fun1 invoked
[intercept] fun2 invoked before
code generate library...
[intercept] fun2 invoked after
[intercept] myFun1 invoked before
hello,cglib
[intercept] myFun1 invoked after
我们看到，在intercept()方法中，对调用函数名进行了判断，并进行了不同处理。可以再方法执行之前、之后做一些我们想做的事情，甚至是修改输入参数、输出参数。
MethodProxy在对执行函数的时候，提供了2个方法
Java代码  ![收藏代码](http://isuifengfei.iteye.com/images/icon_star.png)
- public Object invoke (Object obj, Object[] args) throws Throwable  
- public Object invokeSuper(Object obj, Object[] args) throws Throwable
  
我们看到，在例子中使用的是invokeSuper()方法，因为动态生成的类是子类或者是实现类，因此invokeSuper就是执行父类中方法的意思。
那么invoke()方法是做什么的。javadoc上说这个方法可以用于 相同类中的其他对象的方法执行，也就是说这个方法中的obj需要传入相同一个类的另一个对象，否则会进入无限递归循环。
通过这个简单的例子，我们可以看到cglib可以动态的生成一个代理，而且这种方法比JDK的动态代理更强大，因为JDK的动态代理，需要代理的类实现某个接口，而cglib没有这个要求，因为它可以直接生成指定类的子类，同时支持实现接口的方式。cglib提供的MethodProxy的执行效率高于JDK自带的反射。
2. CallbackFilter
一个Enhancer生成类可以指定多个Callback,这样对于每次调用有哪个Callback，就需要指定一个CallbackFilter的策略
Java代码  ![收藏代码](http://isuifengfei.iteye.com/images/icon_star.png)
- import java.lang.reflect.Method;  
- import net.sf.cglib.proxy.Callback;  
- import net.sf.cglib.proxy.CallbackFilter;  
- import net.sf.cglib.proxy.Enhancer;  
- import net.sf.cglib.proxy.MethodInterceptor;  
- import net.sf.cglib.proxy.MethodProxy;  
- 
- publicclass CallbackTest {  
- 
- publicvoid fun1(){  
-         System. out .println( "fun1 invoekd" );  
-     }  
- 
- publicvoid fun2(){  
-         System. out .println( "fun2 invoekd" );  
-     }  
- 
- staticclass ClassA implements MethodInterceptor{  
- public Object intercept(Object obj, Method method, Object[] args, MethodProxy proxy) throws Throwable {  
-             System. out .println( "ClassA intercept invoked..." );  
- return proxy.invokeSuper(obj, args);  
-         }  
-     }  
- 
- staticclass ClassB implements MethodInterceptor{  
- public Object intercept(Object obj, Method method, Object[] args, MethodProxy proxy) throws Throwable {  
-             System. out .println( "ClassB intercept invoked..." );  
- return proxy.invokeSuper(obj, args);  
-         }  
-     }  
- 
- public Object createProxy(){  
-         Enhancer enhancer = new Enhancer();  
-         enhancer.setSuperclass(getClass());  
-         enhancer.setCallbacks( new Callback[]{ new ClassA(), new ClassB() });  
-         enhancer.setCallbackFilter( new CallbackFilter() {  
- 
- publicint accept(Method method) {  
-                 String methodName = method.getName();  
- if( "fun1" .equals(methodName) ){  
- return0;  
-                 } else {  
- return1;  
-                 }  
-             }  
-         });  
- return enhancer.create();  
-     }  
- 
- publicstaticvoid main(String[] args) {  
-         CallbackTest test = new CallbackTest();  
- 
-         CallbackTest obj = (CallbackTest)test.createProxy();  
- 
-         obj.fun1();  
-         obj.fun2();  
- 
-     }  
- 
- }  
- 
输出结果：
ClassA intercept invoked...
fun1 invoekd
ClassB intercept invoked...
fun2 invoekd
我们可以看到，CallbackFilter类的accept()方法的返回值是int类型的，它用来指示此次指派的Callback的次序，从0开始，注意这个返回值必须小于当前指定的Callback的总个数。
3. Mixin
Mixin可以对多个对象进行代理，需要同时指定多个接口和者多个接口对应的代理对象
Java代码  ![收藏代码](http://isuifengfei.iteye.com/images/icon_star.png)
- <span style="font-size: xx-small;">import net.sf.cglib.proxy.Mixin;  
- 
- publicclass MixinTest {  
- 
- staticinterface Inter1 {  
- 
- void fun1(String arg0);  
-     }  
- 
- staticinterface Inter2 {  
- void fun1(String arg0);  
- void fun2(int arg0);  
-     }  
- 
- publicstaticvoid main(String[] args) {  
-         Mixin mixin = Mixin. create( new Class[]{Inter1. class ,Inter2.class },  
- new Object[]{  
- new Inter1() {  
- publicvoid fun1(String arg0) {  
-                                             System.out .println("Inter1 - " + arg0);  
-                                         }  
-                                     },  
- new Inter2() {  
- publicvoid fun1(String arg0) {  
-                                             System.out .println("Inter1 - " + arg0);  
-                                         }  
- publicvoid fun2( int arg0) {  
-                                             System.out .println("Inter2 - " + arg0);  
-                                         }  
-                                     }  
-                                    });  
- 
-         Inter1 inter1 = (Inter1) mixin;  
-         inter1.fun1( "hello" );  
- 
-         Inter2 inter2 = (Inter2) mixin;  
-         inter2.fun1( "world" );  
-         inter2.fun2(999);  
- 
-     }  
- }  
- </span>  
 输出结果：
Inter1 - hello
Inter1 - world
Inter2 - 999
 我们查看一下classpath下编译之后的class文件
![](http://dl.iteye.com/upload/attachment/0074/1158/9ad03486-b683-3ea4-840d-994802af6655.png)
其中，Mixintest$inter1.class和Mixintest$inter2.class是2个内部接口
生成的2个类是MixinTest$1.class和MixinTest$2.class，也就是说其实CGLIB没有为这2个代理对象生成1个类，而是生成了2个类
反编译一下这2个类，我们可以看到生成的Mixin对象其实是引用了这两个类对象
Java代码  ![收藏代码](http://isuifengfei.iteye.com/images/icon_star.png)
- <span style="font-size: xx-small;">class MixinTest$1
- implements MixinTest.Inter1  
- {  
- publicvoid fun1(String arg0)  
-   {  
-     System.out.println("Inter1 - " + arg0);  
-   }  
- }  
- 
- class MixinTest$2
- implements MixinTest.Inter2  
- {  
- publicvoid fun1(String arg0)  
-   {  
-     System.out.println("Inter1 - " + arg0);  
-   }  
- publicvoid fun2(int arg0) {  
-     System.out.println("Inter2 - " + arg0);  
-   }  
- }  
- </span>  
4. BeanCopier
BeanCopier可以实现Bean之间的属性同名属性拷贝
Java代码  ![收藏代码](http://isuifengfei.iteye.com/images/icon_star.png)
- <span style="font-size: xx-small;">import java.util.Arrays;  
- import java.util.List;  
- import net.sf.cglib.beans.BeanCopier;  
- import org.apache.commons.lang.builder.ToStringBuilder;  
- import org.apache.commons.lang.builder.ToStringStyle;  
- 
- publicclass BeanCopierTest {  
- 
- staticclass ClassA{  
- private String username ;  
- private String password ;  
- private String score ;  
- 
- private List<String> list ;  
- 
- public String getUsername() {  
- return username ;  
-         }  
- publicvoid setUsername(String username) {  
- this .username = username;  
-         }  
- public String getPassword() {  
- return password ;  
-         }  
- publicvoid setPassword(String password) {  
- this .password = password;  
-         }  
- public String getScore() {  
- return score ;  
-         }  
- publicvoid setScore(String score) {  
- this .score = score;  
-         }  
- public List<String> getList() {  
- return list ;  
-         }  
- publicvoid setList(List<String> list) {  
- this .list = list;  
-         }  
- @Override
- public String toString() {  
- return ToStringBuilder.reflectionToString( this , ToStringStyle.MULTI_LINE_STYLE );  
-         }  
-     }  
- 
- staticclass ClassB{  
- private String username ;  
- private String password ;  
- private String address ;  
- private List<Integer> list ;  
- 
- public String getUsername() {  
- return username ;  
-         }  
- publicvoid setUsername(String username) {  
- this .username = username;  
-         }  
- public String getPassword() {  
- return password ;  
-         }  
- publicvoid setPassword(String password) {  
- this .password = password;  
-         }  
- public String getAddress() {  
- return address ;  
-         }  
- publicvoid setAddress(String address) {  
- this .address = address;  
-         }  
- public List<Integer> getList() {  
- return list ;  
-         }  
- publicvoid setList(List<Integer> list) {  
- this .list = list;  
-         }  
- @Override
- public String toString() {  
- return ToStringBuilder.reflectionToString( this , ToStringStyle.MULTI_LINE_STYLE );  
-         }  
-     }  
- 
- publicstaticvoid main(String[] args) {  
- 
-         BeanCopier beanCopier = BeanCopier.create(ClassA. class,ClassB. class ,false );  
- 
-         List<String> list = Arrays. asList( new String[]{ "a" ,"b" ,"c" } );  
- 
-         ClassA a = new ClassA();  
-         a.setUsername( "hello" );  
-         a.setPassword( "world" );  
-         a.setScore( "99" );  
-         a.setList(list);  
- 
-         ClassB b = new ClassB();  
-         b.setUsername( "hello" );  
-         b.setPassword( "world" );  
-         b.setAddress( "beijing" );  
- 
-         beanCopier.copy(a, b, null );  
- 
-         System. out .println( a );  
-         System. out .println( b );  
- 
-     }  
- 
- }  
- </span>  
我们可以看到，对于2个对象中的同名属性 username 和 password进行了拷贝，并且对内部的符合属性List a 也进行了复制，但是，通过debug我们可以发现，内部的符合属性其实并没有实现copy
因为他们在内存中实现上是同一个对象，对于内置复合对象的拷贝，需要寻找其他途径。而且，虽然ClassA中是List<String>,ClassB中是List<Integer>，在赋值的时候并没有抛出异常，也就是仅仅是内存上的赋值成功，并没有进行繁星检查，这也充分说明Java的泛型为伪泛型，在运行时会“泛型”会消失。但是如果此后在调用赋值之后的泛型不支持的方法时，就可能会遇到运行时异常。这会是一个安全隐患。
![](https://img-blog.csdn.net/20141226135859812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZG9uZ25hbjU5MTE3MjExMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

