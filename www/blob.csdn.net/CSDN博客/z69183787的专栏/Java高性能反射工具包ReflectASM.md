# Java高性能反射工具包ReflectASM - z69183787的专栏 - CSDN博客
2018年04月27日 11:01:58[OkidoGreen](https://me.csdn.net/z69183787)阅读数：677
个人分类：[反射-Asm & ReflectAsm																[反射-机制](https://blog.csdn.net/z69183787/article/category/2175689)](https://blog.csdn.net/z69183787/article/category/6463136)
[https://www.cnblogs.com/juetoushan/p/7724793.html](https://www.cnblogs.com/juetoushan/p/7724793.html)
[ReflectASM](http://code.google.com/p/reflectasm/) 使用字节码生成的方式实现了更为高效的反射机制。执行时会生成一个存取类来 set/get 字段，访问方法或创建实例。一看到 ASM 就能领悟到 ReflectASM 会用字节码生成的方式，而不是依赖于 Java 本身的反射机制来实现的，所以它更快，并且避免了访问原始类型因自动装箱而产生的问题。
pom.xml
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
<project xmlns="http://maven.apache.org/POM/4.0.0" xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://maven.apache.org/POM/4.0.0 http://maven.apache.org/xsd/maven-4.0.0.xsd">
    <modelVersion>4.0.0</modelVersion>
    <groupId>com.jts</groupId>
    <artifactId>t1</artifactId>
    <version>0.0.1-SNAPSHOT</version>
    <packaging>jar</packaging>
    <properties>
        <project.build.sourceEncoding>UTF-8</project.build.sourceEncoding>
    </properties>
    <dependencies>
        <dependency>
            <groupId>junit</groupId>
            <artifactId>junit</artifactId>
            <version>4.10</version>
            <scope>test</scope>
        </dependency>
        <dependency>
            <groupId>com.esotericsoftware</groupId>
            <artifactId>reflectasm</artifactId>
            <version>1.11.0</version>
        </dependency>
    </dependencies>
</project>
![复制代码](https://common.cnblogs.com/images/copycode.gif)
待反射的类：
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
package com.jts.service;
public class UserService {
    
    public int state;
    
    public void update(int id, String username) {
        
    }
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
测试类：
```
![复制代码](https://common.cnblogs.com/images/copycode.gif)
package com.jts.test;
import java.lang.reflect.Method;
import org.junit.Test;
import com.esotericsoftware.reflectasm.ConstructorAccess;
import com.esotericsoftware.reflectasm.FieldAccess;
import com.esotericsoftware.reflectasm.MethodAccess;
import com.jts.service.UserService;
public class AppTest {
    
    public static void main(String[] args) throws Exception{
        AppTest test = new AppTest();
        test.testJdkReflect();
        test.testReflectAsm4Name();
        test.testReflectAsm4Index();
    }
    
    /**
     * JDK反射调用方法
     * @throws Exception
     */
    @Test
    public void testJdkReflect() throws Exception {
        UserService target = new UserService();
        long start = System.currentTimeMillis();
        Method method = target.getClass().getMethod("update", int.class, String.class);
        for (int i = 0; i < 100000000; i++) {
            method.invoke(target, 1, "zhangsan");
        }
        long end = System.currentTimeMillis();
        System.out.println("timeout=" + (end - start));//809 753 880 875 816 
    }
    /**
     * ReflectAsm反射调用方法
     * 用名称定位反射方法
     */
    @Test
    public void testReflectAsm4Name() {
        UserService target = new UserService();
        MethodAccess access = MethodAccess.get(UserService.class);//生成字节码的方式创建UserServiceMethodAccess
        long start = System.currentTimeMillis();
        for (int i = 0; i < 100000000; i++) {
            access.invoke(target, "update", 1, "zhangsan");
        }
        long end = System.currentTimeMillis();
        System.out.println("timeout=" + (end - start));//523 382 415 489 482
    }
    /**
     * ReflectAsm反射调用方法
     * 用方法和字段的索引定位反射方法，性能高
     */
    @Test
    public void testReflectAsm4Index() {
        UserService target = new UserService();
        MethodAccess access = MethodAccess.get(UserService.class);
        int index = access.getIndex("update", int.class, String.class);
        long start = System.currentTimeMillis();
        for (int i = 0; i < 100000000; i++) {
            access.invoke(target, index, 1, "zhangsan");
        }
        long end = System.currentTimeMillis();
        System.out.println("timeout=" + (end - start));//12 15 23 14 24
    }
    /**
     * ReflectAsm反射来set/get字段值
     */
    @Test
    public void testFieldAccess() {
        UserService target = new UserService();
        FieldAccess fieldAccess = FieldAccess.get(target.getClass());
        fieldAccess.set(target, "state", 1);
        int state = (Integer)fieldAccess.get(target, "state");
        System.out.println(state);
        
    }
    /**
     * ReflectAsm反射来调用构造方法
     */
    @Test
    public void testConstructorAccess() {
        ConstructorAccess<UserService> constructorAccess = ConstructorAccess.get(UserService.class);
        UserService userService = constructorAccess.newInstance();
        System.out.println(userService);
    }
    /**
     * 查找方法的索引
     */
    @Test
    public void testIndex() {
        UserService target = new UserService();
        MethodAccess methodAccess = MethodAccess.get(target.getClass());
        int index = methodAccess.getIndex("update", int.class, String.class);
        System.out.println(index);
    }
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
```
```
```
