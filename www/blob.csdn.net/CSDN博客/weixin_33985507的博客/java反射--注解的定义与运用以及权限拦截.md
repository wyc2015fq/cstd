# java反射--注解的定义与运用以及权限拦截 - weixin_33985507的博客 - CSDN博客
2013年06月19日 20:43:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
**自定义注解类编写的一些规则:**
**1. Annotation型定义为@interface, 所有的Annotation会自动继承java.lang.Annotation这一接口,并且不能再去继承别的类或是接口.**
**2. 参数成员只能用public或默认(default)这两个访问权修饰**
**3. 参数成员只能用基本类型byte,short,char,int,long,float,double,boolean八种基本数据类型和String、Enum、Class、annotations等数据类型,以及这一些类型的数组.**
**4. 要获取类方法和字段的注解信息，必须通过Java的反射技术来获取 Annotation对象,因为你除此之外没有别的获取注解对象的方法**
**5. 注解也可以没有定义成员, 不过这样注解就没啥用了**
**自定义注解类时, 可以指定目标 (类、方法、字段, 构造函数等) , 注解的生命周期(运行时,class文件或者源码中有效), 是否将注解包含在javadoc中及是否允许子类继承父类中的注解, 具体如下:**
**1. @Target 表示该注解目标,可能的 ElemenetType 参数包括:**
> 
> 
**ElemenetType.CONSTRUCTOR 构造器声明 ElemenetType.FIELD 域声明(包括 enum 实例)  ElemenetType.LOCAL_VARIABLE 局部变量声明  ElemenetType.METHOD 方法声明  ElemenetType.PACKAGE 包声明  ElemenetType.PARAMETER 参数声明  ElemenetType.TYPE 类，接口(包括注解类型)或enum声明**
> 
**2. @Retention 表示该注解的生命周期,可选的 RetentionPolicy 参数包括**
> 
> **RetentionPolicy.SOURCE 注解将被编译器丢弃  RetentionPolicy.CLASS 注解在class文件中可用，但会被VM丢弃  RetentionPolicy.RUNTIME VM将在运行期也保留注释，因此可以通过反射机制读取注解的信息**
> 
**3. @Documented 指示将此注解包含在 javadoc 中**
**4.  @Inherited 指示允许子类继承父类中的注解**
**类注解的定义：**
```java
import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;
/**
 * 注解类
 * @author Owner
 */
@Retention(RetentionPolicy.RUNTIME)
@Target(ElementType.TYPE)
public @interface MyClassAnnotation {
	String uri();
	String desc();
}
```
**构造方法注解定义：**
```java
import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;
/**
 * 构造方法注解
 * @author Owner
 *
 */
@Retention(RetentionPolicy.RUNTIME)   
@Target(ElementType.CONSTRUCTOR) 
public @interface MyConstructorAnnotation {
	String uri();
	String desc();
}
```
 方法注解定义：
```java
import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;
/**
 * 我的方法注解
 * @author Owner
 *
 */
@Retention(RetentionPolicy.RUNTIME)   
@Target(ElementType.METHOD)
public @interface MyMethodAnnotation {
	String uri();
	String desc();
}
```
**字段注解定义：**
```java
import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;
/**
 * 字段注解定义
 * @author Owner
 *
 */
@Retention(RetentionPolicy.RUNTIME)   
@Target(ElementType.FIELD) 
public @interface MyFieldAnnotation {
	String uri();
	String desc();
}
```
**最后定义一个测试类**
```java
import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
@MyClassAnnotation(desc="The class name", uri="com.annotation.MySample")
public class MyTest {
	@MyFieldAnnotation(desc="The class field", uri="com.annotation.MySample#id")
	private int id;
	
	@MyConstructorAnnotation(desc="The class constructor", uri="com.annotation.MySample#MySample")
	public MyTest(){}
	public int getId() {
		return id;
	}
	@MyMethodAnnotation(desc="The class method", uri="com.annotation.MySample#setId")
	public void setId(int id) {
		this.id = id;
	}
	
	
	public static void main(String[] args) throws Exception {
		
		Class<MyTest> clazz = MyTest.class;
		
		//得到类注解
		MyClassAnnotation myClassAnnotation = clazz.getAnnotation(MyClassAnnotation.class);
		
		System.out.println(myClassAnnotation.desc()+" "+myClassAnnotation.uri());
		
		//得到构造方法注解
		Constructor<MyTest> cons = clazz.getConstructor(new Class[]{});
		
		MyConstructorAnnotation myConstructorAnnotation = cons.getAnnotation(MyConstructorAnnotation.class);
		
		System.out.println(myConstructorAnnotation.desc()+" "+myConstructorAnnotation.uri());
		
		//获取方法注解
		Method method = clazz.getMethod("setId", new Class[]{int.class});
		
		MyMethodAnnotation myMethodAnnotation = method.getAnnotation(MyMethodAnnotation.class);
		
		System.out.println(myMethodAnnotation.desc()+" "+myMethodAnnotation.uri());
		//获取字段注解
		Field field = clazz.getDeclaredField("id");
		
		MyFieldAnnotation myFieldAnnotation = field.getAnnotation(MyFieldAnnotation.class);
		
		System.out.println(myFieldAnnotation.desc()+" "+myFieldAnnotation.uri() );
	}
	
}
```
**输出：**
**The class name com.annotation.MySample The class constructor com.annotation.MySample#MySample The class method com.annotation.MySample#setId The class field com.annotation.MySample#id**
**好了，上面是基本学习，我们在实际的项目中用在什么地方呢？我想我们都做过关于细粒度权限拦截的问题，在Struts2中可以根据登录用户所具有的的权限进行任一一个action方法的拦截，可以定义一个自定义方法注解，例如**
```java
@Retention(RetentionPolicy.RUNTIME)//代表Permission注解保留在的阶段
@Target(ElementType.METHOD)//标注在方法上面
public @interface Permission {
	/** 模块 */
	String module();
	/** 权限值 */
	String privilege();
	
}
```
**比如有一个部门action，Department.action,有一个方法public String departmentlistUI(){}**
**可以这样定义方法**
```java
@Permission(module="department",privilege="view")
	public String departmentlistUI(){
}
```
**然后自定定义一个权限拦截器PrivilegeInterceptor.java并在struts.xml中注册，**
**在实现interceptor接口后，实现方法public String intercept(ActionInvocation invocation) throws Exception {}**
**在这里调用任一个action方法都会经过该拦截方法，通过invocation可以获取当前调用的action的名字，以及调用的action的哪个方法，**
**通过这段代码可以获取action名字和方法名**
```java
String  actionName=invocation.getProxy().getActionName();
	    String  methodName=invocation.getProxy().getMethod();
	    
	    System.out.println("拦截到：action的名字："+actionName+"方法名："+methodName);
```
**然后通过反射技术，获取该方法上的自定义权限注解，获取当前登录的用户（从session中），遍历当前用户的所拥有的权限组，并且遍历任一个权限组下的所有的权限，看是否包括该方法上注解所需的权限。这样就可以完成细粒度的action方法权限拦截了。**
**这只是个大体的思路**
**下面看一下，拦截器的具体实现该功能的代码**
```java
private boolean validate(ActionInvocation invocation) throws SecurityException, NoSuchMethodException {
		
		String  methodName=invocation.getProxy().getMethod();
		
		Method currentMethod = invocation.getAction().getClass().getMethod(methodName);
		
		if(currentMethod != null && currentMethod.isAnnotationPresent(Permission.class)){
			//得到方法上的注解
			Permission permission = currentMethod.getAnnotation(Permission.class);
			//该方法上的所需要的权限
			SystemPrivilege methodPrivilege = new SystemPrivilege(new SystemPrivilegePK(permission.module(), permission.privilege()));
			//得到当前登录的用户
			Employee e = (Employee) ActionContext.getContext().getSession().get("loginUser");
			//遍历当前用户下的所有的权限组
			for(PrivilegeGroup group : e.getGroups()){
				//如果该权限组下包含，要访问该方法所需要的权限，就放行
				if(group.getPrivileges().contains(methodPrivilege)){
					return true;
				}
				
			}
			//说明遍历的该用户所有的权限组，没有发现该权限，说明没有该权限
			return false;
			 
		}
		//没有标注注解，表示谁都可以调用该方法
		return true;
	}
```

