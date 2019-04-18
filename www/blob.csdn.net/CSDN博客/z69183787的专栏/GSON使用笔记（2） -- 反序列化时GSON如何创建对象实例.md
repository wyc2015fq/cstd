# GSON使用笔记（2） -- 反序列化时GSON如何创建对象实例 - z69183787的专栏 - CSDN博客
2016年09月16日 17:14:28[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1270
## 从一个问题开始
假设有这么一个类：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/21527709#)[copy](http://blog.csdn.net/zxhoo/article/details/21527709#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/244716/fork)
- class MyObj {  
- 
- publicfinalint x;  
- 
- public MyObj(int x) {  
- this.x = x;  
-     }  
- 
- }  
和下面的测试代码：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/21527709#)[copy](http://blog.csdn.net/zxhoo/article/details/21527709#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/244716/fork)
- @Test
- publicvoid gson() {  
-     MyObj obj = new Gson().fromJson("{\"x\":1}", MyObj.class);  
-     Assert.assertEquals(1, obj.x);  
- }  
那么GSON是通过什么样的方式创建MyObj对象的呢？答案可能会出乎你的意料（至少出乎了我的意料）。
## InstanceCreator和ObjectConstructor
经过断点调试或者阅读源代码不难发现，GSON是使用**ObjectConstructor**来创建对象实例的，这点从代码注释里也能看的出来：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/21527709#)[copy](http://blog.csdn.net/zxhoo/article/details/21527709#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/244716/fork)
- /**
-  * Defines a generic object construction factory.  The purpose of this class
-  * is to construct a default instance of a class that can be used for object
-  * navigation while deserialization from its JSON representation.
-  *
-  * @author Inderjeet Singh
-  * @author Joel Leitch
-  */
- publicinterface ObjectConstructor<T> {  
- 
- /**
-    * Returns a new instance.
-    */
- public T construct();  
- }  
那么ObjectConstructor从何而来呢？答案在ConstructorConstructor里：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/21527709#)[copy](http://blog.csdn.net/zxhoo/article/details/21527709#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/244716/fork)
- public <T> ObjectConstructor<T> get(TypeToken<T> typeToken) {  
- final Type type = typeToken.getType();  
- final Class<? super T> rawType = typeToken.getRawType();  
- 
- // first try an instance creator
- 
- @SuppressWarnings("unchecked") // types must agree
- final InstanceCreator<T> typeCreator = (InstanceCreator<T>) instanceCreators.get(type);  
- if (typeCreator != null) {  
- returnnew ObjectConstructor<T>() {  
- public T construct() {  
- return typeCreator.createInstance(type);  
-       }  
-     };  
-   }  
- 
- // Next try raw type match for instance creators
- @SuppressWarnings("unchecked") // types must agree
- final InstanceCreator<T> rawTypeCreator =  
-       (InstanceCreator<T>) instanceCreators.get(rawType);  
- if (rawTypeCreator != null) {  
- returnnew ObjectConstructor<T>() {  
- public T construct() {  
- return rawTypeCreator.createInstance(type);  
-       }  
-     };  
-   }  
- 
-   ObjectConstructor<T> defaultConstructor = newDefaultConstructor(rawType);  
- if (defaultConstructor != null) {  
- return defaultConstructor;  
-   }  
- 
-   ObjectConstructor<T> defaultImplementation = newDefaultImplementationConstructor(type, rawType);  
- if (defaultImplementation != null) {  
- return defaultImplementation;  
-   }  
- 
- // finally try unsafe
- return newUnsafeAllocator(type, rawType);  
- }  
代码看起来很复杂，但实际上井然有序：
- 如果我们（通过GsonBuilder）注册过**InstanceCreator**，则交给InstanceCreator来创建实例
- 如果类有**默认构造函数**，则通过反射调用默认构造函数创建实例
- 如果想要创建**List**或**Map**等接口的实例，则走这里
- 否则交给神秘的**UnsafeAllocator**来收场
第一和第三种情况暂不考虑，下面来分析第二和第四种情况。
## 有默认构造函数的情况
按照前面的分析，这种情况GSON是通过调用默认构造函数来创建对象实例的，让我们证明这一点：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/21527709#)[copy](http://blog.csdn.net/zxhoo/article/details/21527709#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/244716/fork)
- class MyObj {  
- 
- publicfinalint x;  
- 
- public MyObj() {  
- thrownew RuntimeException("!!!"); // <---
-     }  
- 
- }  
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/21527709#)[copy](http://blog.csdn.net/zxhoo/article/details/21527709#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/244716/fork)
- @Test(expected = RuntimeException.class) // <---
- publicvoid gson() {  
- new Gson().fromJson("{\"x\":1}", MyObj.class);  
- }  
测试通过！
## 没有默认构造函数的情况
还是通过代码来证明这一点：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/21527709#)[copy](http://blog.csdn.net/zxhoo/article/details/21527709#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/244716/fork)
- class MyObj {  
- 
- publicfinalint x;  
- 
- public MyObj(int x) { // <---
- thrownew RuntimeException("!!!");  
-     }  
- 
- }  
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/21527709#)[copy](http://blog.csdn.net/zxhoo/article/details/21527709#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/244716/fork)
- @Test
- publicvoid gson() {  
-     MyObj obj = new Gson().fromJson("{\"x\":1}", MyObj.class);  
-     Assert.assertEquals(1, obj.x);  
- }  
测试通过！
## UnsafeAllocator
现在让我们一睹**UnsafeAllocator**的风采：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/21527709#)[copy](http://blog.csdn.net/zxhoo/article/details/21527709#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/244716/fork)
- /**
-  * Do sneaky things to allocate objects without invoking their constructors.
-  *
-  * @author Joel Leitch
-  * @author Jesse Wilson
-  */
- publicabstractclass UnsafeAllocator {  
- publicabstract <T> T newInstance(Class<T> c) throws Exception;  
- 
- publicstatic UnsafeAllocator create() {  
- // try JVM
- // public class Unsafe {
- //   public Object allocateInstance(Class<?> type);
- // }
- try {  
-       Class<?> unsafeClass = Class.forName("sun.misc.Unsafe");  
-       Field f = unsafeClass.getDeclaredField("theUnsafe");  
-       f.setAccessible(true);  
- final Object unsafe = f.get(null);  
- final Method allocateInstance = unsafeClass.getMethod("allocateInstance", Class.class);  
- returnnew UnsafeAllocator() {  
- @Override
- @SuppressWarnings("unchecked")  
- public <T> T newInstance(Class<T> c) throws Exception {  
- return (T) allocateInstance.invoke(unsafe, c);  
-         }  
-       };  
-     } catch (Exception ignored) {  
-     }  
- 
-     ...  
- 
- // give up
- returnnew UnsafeAllocator() {  
- @Override
- public <T> T newInstance(Class<T> c) {  
- thrownew UnsupportedOperationException("Cannot allocate " + c);  
-       }  
-     };  
-   }  
- }  
去掉反射后，代码看起来大概是这样：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/21527709#)[copy](http://blog.csdn.net/zxhoo/article/details/21527709#)
- publicabstractclass UnsafeAllocator {    
- publicabstract <T> T newInstance(Class<T> c) throws Exception;    
- 
- publicstatic UnsafeAllocator create() {  
- returnnew UnsafeAllocator() {    
- @Override
- @SuppressWarnings("unchecked")    
- public <T> T newInstance(Class<T> c) throws Exception {    
-           Unsafe unsafe = sun.misc.Unsafe.theUnsafe; // <--
- return (T) unsafe.allocateInstance(c); // <--
-         }    
-       };   
-   }  
- }  
## 那么final字段是怎么处理的？
答案是，通过**反射**。详细情况可以参考[这个问题](http://stackoverflow.com/questions/8357491/gson-deserialization-set-final-field)，下面我们仅通过代码来证明这一点：
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/21527709#)[copy](http://blog.csdn.net/zxhoo/article/details/21527709#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/244716/fork)
- class MyObj {  
- 
- publicfinalint x;  
- 
- public MyObj(int x) { // <---
- this.x = x;  
-     }  
- 
- }  
**[java]**[view
 plain](http://blog.csdn.net/zxhoo/article/details/21527709#)[copy](http://blog.csdn.net/zxhoo/article/details/21527709#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/244716/fork)
- @Test
- publicvoid setFinal() throws Exception {  
-     MyObj obj = new MyObj(1);  
-     Assert.assertEquals(1, obj.x);  
- 
-     Field f = obj.getClass().getField("x");  
-     f.setAccessible(true); // <---
-     f.set(obj, 2);  
-     Assert.assertEquals(2, obj.x);  
- }  
测试通过！
## 结论
反序列化时，如果一个类**没有默认构造函数**，那么GSON是通过**JDK内部API**来创建对象实例的，并且通过**反射**给**final**字段赋值。
这种做法通常是很危险的，所以**非专业人士请勿效仿**！
