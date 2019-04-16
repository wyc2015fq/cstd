# Java深入 - 深入 Java自定义注解 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2014年07月05日 11:41:49[initphp](https://me.csdn.net/initphp)阅读数：17387
个人分类：[Java深入系列](https://blog.csdn.net/initphp/article/category/2332145)

所属专栏：[Java深入](https://blog.csdn.net/column/details/myjava.html)









我们在使用Spring框架的时候，会经常使用类似：@Autowired 这样的注解。我们也可以自己定义一些注解。Java的注解主要在包：java.lang.annotation中实现。

### 1. 元注解

什么是元注解？你可以这样理解，元注解是自定义注解的注解。元注解主要包含4个。他们主要在java.lang.annotation中可以找到。我们自己要创建注解的时候必须要用到这些元注解。所以必须彻底理解这四个元注解的含义。

1. @Documented

2. @Inherited

3. @Retention

4. @Target

例如：



```java
package com.test.www;

import java.lang.annotation.Documented;
import java.lang.annotation.ElementType;
import java.lang.annotation.Inherited;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;



/**
 * 定义一个用户名的自定义注解
 * @author zhuli
 * @date 2014-7-5
 */
@Documented
@Retention(RetentionPolicy.RUNTIME)
@Target({ ElementType.TYPE, ElementType.METHOD})
@Inherited
public @interface UserNameAnnotations {

    public String value() default "";

}
```



#### 1. @Documented

****@****Documented用于描述其它类型的annotation应该被作为被标注的程序成员的公共API，因此可以被例如javadoc此类的工具文档化。Documented是一个标记注解，没有成员。

#### 2. @Inherited

@Inherited 元注解是一个标记注解，@Inherited阐述了某个被标注的类型是被继承的。如果一个使用了@Inherited修饰的annotation类型被用于一个class，则这个annotation将被用于该class的子类。

#### 3. @Target

@Target说明了Annotation所修饰的对象范围：Annotation可被用于 packages、types（类、接口、枚举、Annotation类型）、类型成员（方法、构造方法、成员变量、枚举值）、方法参数和本地变量（如循环变量、catch参数）。

ElementType.CONSTRUCTOR  作用于构造器

ElementType.FIELD  作用于域/属性

ElementType.LOCAL_VARIABLE  用于描述局部变量

ElementType.METHOD  作用于方法

ElementType.PACKAGE   用于描述包

ElementType.PARAMETER   用于描述参数

ElementType.TYPE   用于描述类、接口(包括注解类型) 或enum声明，最常用


单个修饰对象的范围：



```java
@Target(ElementType.TYPE)
```


多个：





```java
@Target({ ElementType.TYPE, ElementType.METHOD})
```

#### 4. Retention

定义了该Annotation被保留的时间长短：某些Annotation仅出现在源代码中，而被编译器丢弃；而另一些却被编译在class文件中；编译在class文件中的Annotation可能会被虚拟机忽略，而另一些在class被装载时将被读取（请注意并不影响class的执行，因为Annotation与class在使用上是被分离的）。使用这个meta-Annotation可以对 Annotation的“生命周期”限制。

RetentionPolicy.RUNTIME 注解会在class字节码文件中存在，在运行时可以通过反射获取到

RetentionPolicy.CLASS 默认的保留策略，注解会在class字节码文件中存在，但运行时无法获得

RetentionPolicy.SOURCE 注解仅存在于源码中，在class字节码文件中不包含




### 2. 创建一个自定义注解 - 作用于类

1. 创建一个注解类



```java
package com.test.www;

import java.lang.annotation.Documented;
import java.lang.annotation.ElementType;
import java.lang.annotation.Inherited;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;



/**
 * 定义一个用户名的自定义注解
 * @author zhuli
 * @date 2014-7-5
 */
@Documented //文档
@Retention(RetentionPolicy.RUNTIME) //在运行时可以获取
@Target({ ElementType.TYPE, ElementType.METHOD}) //作用到类，方法，接口上等
@Inherited //子类会继承
public @interface UserNameAnnotations {
    
    public String value() default ""; //使用的时候 @UserNameAnnotations(value="xxx")

}
```


2. 创建一个Test类





```java
package com.test.www;


/**
 * 一个注解的测试类
 * @author zhuli
 * @date 2014-7-5
 */
//注入注解作用于类上面
//可以通过反射 获取类的信息之后 获取得到这个注解的值
@UserNameAnnotations(value = "initphp") 
public class Test {

    private String userName;

    public String getUserName() {
        return userName;
    }

    public void setUserName(String userName) {
        this.userName = userName;
    }

}
```



3. 测试类





```java
package com.test.www;


public class mainTest {

    public static void main(String[] args) {
        Class<Test> testClass = Test.class;
        //因为注解是作用于类上面的，所以可以通过isAnnotationPresent来判断是否是一个
        //有UserNameAnnotations注解的类
        if (testClass.isAnnotationPresent(UserNameAnnotations.class)) {
            System.out.println("this is a Annotations class");
            //通过getAnnotation可以获取注解对象
            UserNameAnnotations userNameAnnotations = (UserNameAnnotations) testClass.
getAnnotation(UserNameAnnotations.class);
            if (userNameAnnotations != null) {
                System.out.println("value:" + userNameAnnotations.value());
            } else {
                System.out.println("null");
            }
        } else {
            System.out.println("this is not Annotations class");
        }

    }
}
```


4. 输出：





```
this is a Annotations class
value:initphp
```



### 3. 创建一个自定义注解 - 作用于方法

1. 自定义注解类



```java
package com.test.www;


import java.lang.annotation.Documented;
import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

/**
 * 定义一个作用到方法的注解
 * @author zhuli.zhul
 * @date 2014-7-5
 */
@Documented//文档
@Retention(RetentionPolicy.RUNTIME)//在运行时可以获取
@Target({ ElementType.TYPE, ElementType.METHOD })//作用到类，方法，接口上等
public @interface MethodType {

    //枚举类型
    public enum MethodTypeEnum {
        TYPE1, TYPE2
    }

    //实际的值
    public MethodTypeEnum methodType() default MethodTypeEnum.TYPE1;
}
```


2. 创建一个使用注解的类





```java
package com.test.www;

import com.test.www.MethodType.MethodTypeEnum;


/**
 * 一个注解的测试类
 * @author zhuli
 * @date 2014-7-5
 */
//注入注解作用于类上面
//可以通过反射 获取类的信息之后 获取得到这个注解的值
@UserNameAnnotations(value = "initphp") 
public class Test {

    private String userName;

    //注解到
    @MethodType(methodType=MethodTypeEnum.TYPE2)
    public String getUserName() {
        return userName;
    }

    public void setUserName(String userName) {
        this.userName = userName;
    }

}
```


3. 创建main入口



```java
package com.test.www;


import java.lang.reflect.Method;

import com.test.www.MethodType.MethodTypeEnum;

public class mainTest {

    public static void main(String[] args) {
        Class<Test> testClass = Test.class;
        try {
            //因为是注解到method上的，所以首先要获取这个方法
            Method method = testClass.getDeclaredMethod("getUserName");
            
            //判断这个方法上是否有这个注解
            if (method.isAnnotationPresent(MethodType.class)) {
                System.out.println("this is a method Annotation");
                
                //如果有这个注解，则获取注解类
                MethodType methodType = (MethodType) method.getAnnotation(MethodType.class);
                if (methodType != null) {
                    if (MethodTypeEnum.TYPE1.equals(methodType.methodType())) {
                        System.out.println("this is TYPE1");
                    } else {
                        System.out.println("this is TYPE2");
                    }
                }
            } else {
                System.out.println("this is not  a method Annotation");
            }

        } catch (Exception e) {
        }

    }
}
```


4. 输出：



```
this is a method Annotation
this is TYPE2
```





### 4. 创建一个自定义注解 - 作用于域



1. 创建一个自定义注解



```java
package com.test.www;


import java.lang.annotation.Documented;
import java.lang.annotation.ElementType;
import java.lang.annotation.Retention;
import java.lang.annotation.RetentionPolicy;
import java.lang.annotation.Target;

/**
 * 定义一个作用到域上的自定义注解
 * @author zhuli
 * @date 2014-7-5
 */
@Documented//文档
@Retention(RetentionPolicy.RUNTIME)//在运行时可以获取
@Target({ ElementType.FIELD })//作用到类的域上面
public @interface FieldAnnotations {

    public String value() default ""; //使用的时候 @FieldAnnotations(value="xxx")

}
```


2. 创建一个使用注解的类





```java
package com.test.www;

import com.test.www.MethodType.MethodTypeEnum;


/**
 * 一个注解的测试类
 * @author zhuli
 * @date 2014-7-5
 */
//注入注解作用于类上面
//可以通过反射 获取类的信息之后 获取得到这个注解的值
@UserNameAnnotations(value = "initphp") 
public class Test {

    @FieldAnnotations(value="zhuli")
    private String userName;

    //注解到
    @MethodType(methodType=MethodTypeEnum.TYPE2)
    public String getUserName() {
        return userName;
    }

    public void setUserName(String userName) {
        this.userName = userName;
    }

}
```


3. 创建main入口类





```java
package com.test.www;


import java.lang.reflect.Field;

public class mainTest {

    public static void main(String[] args) {
        Test test = new Test();
        Class<Test> testClass = Test.class;
        try {
            //因为是注解到Field上的，所以首先要获取这个字段
            Field field = testClass.getDeclaredField("userName");

            //判断这个Field上是否有这个注解
            if (field.isAnnotationPresent(FieldAnnotations.class)) {
                System.out.println("this is a field Annotation");

                //如果有这个注解，则获取注解类
                FieldAnnotations fieldAnnotations = (FieldAnnotations) field.getAnnotation(FieldAnnotations.class);
                if (fieldAnnotations != null) {
                    //通过反射给私有变量赋值
                    field.setAccessible(true);
                    field.set(test, fieldAnnotations.value());
                    System.out.println("value:" + test.getUserName());
                }
            } else {
                System.out.println("this is not  a field Annotation");
            }

        } catch (Exception e) {
        }

    }
}
```


4. 输出：





```java
this is a field Annotation
value:zhuli
```










