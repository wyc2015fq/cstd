# java自定义注解 - 零度的博客专栏 - CSDN博客
2016年08月08日 10:22:00[零度anngle](https://me.csdn.net/zmx729618)阅读数：299标签：[Java注解](https://so.csdn.net/so/search/s.do?q=Java注解&t=blog)
个人分类：[Java注解](https://blog.csdn.net/zmx729618/article/category/6354188)
                
 java注解是附加在代码中的一些元信息，用于一些工具在编译、运行时进行解析和使用，起到说明、配置的功能。
注解不会也不能影响代码的实际逻辑，仅仅起到辅助性的作用。包含在 java.lang.annotation 包中。
1、元注解
元注解是指注解的注解。包括  @Retention @Target @Document @Inherited四种。
1.1、@Retention: 定义注解的保留策略
@Retention(RetentionPolicy.SOURCE)   
//注解仅存在于源码中，在class字节码文件中不包含
@Retention(RetentionPolicy.CLASS) // 默认的保留策略，注解会在class字节码文件中存在，但运行时无法获得，
@Retention(RetentionPolicy.RUNTIME)  // 注解会在class字节码文件中存在，在运行时可以通过反射获取到
1.2、@Target：定义注解的作用目标
其定义的源码为： 
**[java]**[view plain](http://blog.csdn.net/yixiaogang109/article/details/7328466#)[copy](http://blog.csdn.net/yixiaogang109/article/details/7328466#)[print](http://blog.csdn.net/yixiaogang109/article/details/7328466#)[?](http://blog.csdn.net/yixiaogang109/article/details/7328466#)
- @Documented
- @Retention(RetentionPolicy.RUNTIME)  
- @Target(ElementType.ANNOTATION_TYPE)  
- public@interface Target {  
-     ElementType[] value();  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
@Documented
@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.ANNOTATION_TYPE)
public @interface Target {
    ElementType[] value();
}
```
@Target(ElementType.TYPE)   //接口、类、枚举、注解
@Target(ElementType.FIELD) //字段、枚举的常量
@Target(ElementType.METHOD) //方法
@Target(ElementType.PARAMETER) //方法参数
@Target(ElementType.CONSTRUCTOR)  //构造函数
@Target(ElementType.LOCAL_VARIABLE)//局部变量
@Target(ElementType.ANNOTATION_TYPE)//注解
@Target(ElementType.PACKAGE) ///包
 由以上的源码可以知道，他的elementType 可以有多个，一个注解可以为类的，方法的，字段的等等
1.3、@Document：说明该注解将被包含在javadoc中
1.4、@Inherited：说明子类可以继承父类中的该注解
2、java 注解的自定义
      下面是自定义注解的一个例子
**[java]**[view plain](http://blog.csdn.net/yixiaogang109/article/details/7328466#)[copy](http://blog.csdn.net/yixiaogang109/article/details/7328466#)[print](http://blog.csdn.net/yixiaogang109/article/details/7328466#)[?](http://blog.csdn.net/yixiaogang109/article/details/7328466#)
- @Documented
- @Target({ElementType.TYPE,ElementType.METHOD})  
- @Retention(RetentionPolicy.RUNTIME)  
- public@interface Yts {  
- publicenum YtsType{util,entity,service,model}  
- 
- public YtsType classType() default YtsType.util;  
- }  
- 
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
@Documented
@Target({ElementType.TYPE,ElementType.METHOD})
@Retention(RetentionPolicy.RUNTIME)
public @interface Yts {
   public enum YtsType{util,entity,service,model}
   
   public YtsType classType() default YtsType.util;
}
```
**[java]**[view plain](http://blog.csdn.net/yixiaogang109/article/details/7328466#)[copy](http://blog.csdn.net/yixiaogang109/article/details/7328466#)[print](http://blog.csdn.net/yixiaogang109/article/details/7328466#)[?](http://blog.csdn.net/yixiaogang109/article/details/7328466#)
- @Documented
- @Retention(RetentionPolicy.RUNTIME)  
- @Target(ElementType.METHOD)  
- @Inherited
- public@interface HelloWorld {  
- public String name()default"";  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
@Documented
@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.METHOD)
@Inherited
public @interface HelloWorld {
    public String name()default "";
}
```
@Retention(RetentionPolicy.RUNTIME)
定义的这个注解是注解会在class字节码文件中存在，在运行时可以通过反射获取到。
@Target({ElementType.TYPE,ElementType.METHOD})
因此这个注解可以是类注解，也可以是方法的注解
这样一个注解就自定义好了，当然注解里面的成员可以为基本的数据类型，也可以为数据，Object等等
3 注解是定义好了，那么怎么来得到，解析注解呢？
java的反射机制可以帮助，得到注解，代码如下：
**[java]**[view plain](http://blog.csdn.net/yixiaogang109/article/details/7328466#)[copy](http://blog.csdn.net/yixiaogang109/article/details/7328466#)[print](http://blog.csdn.net/yixiaogang109/article/details/7328466#)[?](http://blog.csdn.net/yixiaogang109/article/details/7328466#)
- publicclass ParseAnnotation {  
- 
- publicvoid parseMethod(Class clazz) throws IllegalArgumentException, IllegalAccessException, InvocationTargetException, SecurityException, NoSuchMethodException, InstantiationException{  
-   Object obj = clazz.getConstructor(new Class[]{}).newInstance(new Object[]{});  
- for(Method method : clazz.getDeclaredMethods()){  
-         HelloWorld say = method.getAnnotation(HelloWorld.class);  
-         String name = "";  
- if(say != null){  
-            name = say.name();  
-            method.invoke(obj, name);  
-         }  
-        Yts yts = (Yts)method.getAnnotation(Yts.class);  
- if(yts != null){  
- if(YtsType.util.equals(yts.classType())){  
-           System.out.println("this is a util method");  
-         }else{  
-             System.out.println("this is a other method");  
-             }  
-         }  
-       }  
-     }  
- @SuppressWarnings("unchecked")  
- publicvoid parseType(Class clazz) throws IllegalArgumentException, IllegalAccessException, InvocationTargetException{  
-         Yts yts = (Yts) clazz.getAnnotation(Yts.class);  
- if(yts != null){  
- if(YtsType.util.equals(yts.classType())){  
-                 System.out.println("this is a util class");  
-             }else{  
-                 System.out.println("this is a other class");  
-             }  
-         }  
-     }  
- 
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
public class ParseAnnotation {
	 public void parseMethod(Class clazz) throws IllegalArgumentException, IllegalAccessException, InvocationTargetException, SecurityException, NoSuchMethodException, InstantiationException{
  Object obj = clazz.getConstructor(new Class[]{}).newInstance(new Object[]{});
    for(Method method : clazz.getDeclaredMethods()){
        HelloWorld say = method.getAnnotation(HelloWorld.class);
        String name = "";
        if(say != null){
           name = say.name();
           method.invoke(obj, name);
        }
       Yts yts = (Yts)method.getAnnotation(Yts.class);
       if(yts != null){
          if(YtsType.util.equals(yts.classType())){
          System.out.println("this is a util method");
        }else{
            System.out.println("this is a other method");
            }
        }
      }
    }
	@SuppressWarnings("unchecked")
	public void parseType(Class clazz) throws IllegalArgumentException, IllegalAccessException, InvocationTargetException{
		Yts yts = (Yts) clazz.getAnnotation(Yts.class);
		if(yts != null){
			if(YtsType.util.equals(yts.classType())){
				System.out.println("this is a util class");
			}else{
				System.out.println("this is a other class");
			}
		}
	}
	
}
```
前一个方法是解析得到方法注解的，后一个方法是得到类注解的
以下是测试方法类
**[java]**[view plain](http://blog.csdn.net/yixiaogang109/article/details/7328466#)[copy](http://blog.csdn.net/yixiaogang109/article/details/7328466#)[print](http://blog.csdn.net/yixiaogang109/article/details/7328466#)[?](http://blog.csdn.net/yixiaogang109/article/details/7328466#)
- @Yts(classType =YtsType.util)  
- publicclass SayHell {  
- 
- @HelloWorld(name = " 小明 ")  
- @Yts
- publicvoid sayHello(String name){  
- if(name == null || name.equals("")){  
-             System.out.println("hello world!");  
-         }else{  
-             System.out.println(name + "say hello world!");  
-         }  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
```java
@Yts(classType =YtsType.util)
public class SayHell {
	@HelloWorld(name = " 小明 ")
	@Yts
	public void sayHello(String name){
		if(name == null || name.equals("")){
			System.out.println("hello world!");
		}else{
			System.out.println(name + "say hello world!");
		}
	}
}
```
**[html]**[view plain](http://blog.csdn.net/yixiaogang109/article/details/7328466#)[copy](http://blog.csdn.net/yixiaogang109/article/details/7328466#)[print](http://blog.csdn.net/yixiaogang109/article/details/7328466#)[?](http://blog.csdn.net/yixiaogang109/article/details/7328466#)
- public static void main(String[] args) throws IllegalArgumentException, IllegalAccessException, InvocationTargetException, SecurityException, NoSuchMethodException, InstantiationException {  
-         ParseAnnotation parse = new ParseAnnotation();  
-         parse.parseMethod(SayHell.class);  
-         parse.parseType(SayHell.class);  
-     }  
