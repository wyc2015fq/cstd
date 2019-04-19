# CGLIB使用入门 - 零度的博客专栏 - CSDN博客
2017年01月18日 09:33:19[零度anngle](https://me.csdn.net/zmx729618)阅读数：554
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
![](http://static.blog.csdn.net/images/save_snippets.png)
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
![](http://static.blog.csdn.net/images/save_snippets.png)
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
  
![](http://static.blog.csdn.net/images/save_snippets_01.png)
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
![](http://static.blog.csdn.net/images/save_snippets.png)
输出结果：
ClassA intercept invoked...
fun1 invoekd
ClassB intercept invoked...
fun2 invoekd
我们可以看到，CallbackFilter类的accept()方法的返回值是int类型的，它用来指示此次指派的Callback的次序，从0开始，注意这个返回值必须小于当前指定的Callback的总个数
