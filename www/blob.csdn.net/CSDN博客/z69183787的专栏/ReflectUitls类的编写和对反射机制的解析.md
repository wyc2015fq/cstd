# ReflectUitls类的编写和对反射机制的解析 - z69183787的专栏 - CSDN博客
2016年06月13日 14:51:21[OkidoGreen](https://me.csdn.net/z69183787)阅读数：589
## 反射相关的类
　　反射相关的类，最基本的当然是**Class**类。
　　获取了Class对象之后，就可以接着生成对象实例、调用方法、查看字段等等。
　　字段(Field)、方法(Method)和构造方法(Constructor<T>)各有相应的类来表示，它们共同继承于**java.lang.reflect.AccessibleObject**类：
![](http://images.cnitblog.com/i/325852/201405/131414574213505.png)
　　这三个类共同实现的还有**Member**接口：
![](http://images.cnitblog.com/i/325852/201405/131415329686631.png)
　　获取字段、方法和构造方法时，需要调用Class类的**getXXX()**和**getDeclearedXXX()**方法，需要注意二者区别。
　　比如对于方法：
```
getMethods();//获取public (包括父类中的)
getDeclaredMethods();//获取本类声明(包括各种修饰符public、private)
```
　　相关博文：[Java中的反射机制（一）](http://www.cnblogs.com/mengdd/archive/2013/01/26/2877972.html)
　　API 文档：[http://docs.oracle.com/javase/7/docs/api/](http://docs.oracle.com/javase/7/docs/api/)
## 访问权限相关
　　利用反射，可以访问一些SDK、API中的私有方法。
　　相关博文：[Java中的反射机制（四） 利用反射访问私有](http://www.cnblogs.com/mengdd/archive/2013/01/26/2878136.html)
　　访问私有方法有一个关键点：
```
setAccessible(true); // 抑制Java对修饰符的检查
```
　　这个方法是**AccessibleObject**类中的。
![](http://images.cnitblog.com/i/325852/201405/131418035009084.png)
　　前面说过，它是**Field**、**Method**和**Constructor<T>**的共同基类。
正常情况下，调用isAccessible()返回的都是false，无论你的方法是public还是private。
　　这是因为这个accessible的flag是JVM用来限制是否可以直接访问，**默认情况下是需要进行访问修饰符的检查的，所以flag为false**，即不能直接访问。
**当这个flag****设置为true，表明可以直接访问，不经过访问修饰符的检查**。
　　辅助类中用于调用私有方法的接口：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
/**
     * 调用私有方法
     *
     * @param obj
     *            调用类对象
     * @param methodName
     *            方法名
     * @param paramTypes
     *            参数类型
     * @param params
     *            参数
     * @return
     * @throws Exception
     */
    public static Object invokePrivateMethod(Object obj, String methodName,
            Class<?>[] paramTypes, Object[] params) throws Exception {
        Object value = null;
        Class<?> cls = obj.getClass();
        // 注意不要用getMethod(),因为getMethod()返回的都是public方法
        Method method = cls.getDeclaredMethod(methodName, paramTypes);
        method.setAccessible(true);// 抑制Java的访问控制检查
        value = method.invoke(obj, params);
        return value;
    }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
## 访问修饰符
　　对访问修饰符的获取可以通过Member接口的**getModifiers()**方法，该方法返回一个整型值，整型值是一系列的修饰符位操作组合的结果。
　　用**Modifier**类可以解析这个整型值中包含的修饰符信息。
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
    /**
     * 获取修饰符信息
     *
     * @param member
     * @return
     */
    private static String getModifiersInfo(Member member) {
        StringBuilder sBuilder = new StringBuilder();
        int modifiers = member.getModifiers();
        sBuilder.append("\ngetModifiers: " + +modifiers + ", ");// 得到修饰符编码
        sBuilder.append("\nisPublic: " + Modifier.isPublic(modifiers) + ", ");
        sBuilder.append("\nisPrivate: " + Modifier.isPrivate(modifiers) + ", ");
        sBuilder.append("\nisStatic: " + Modifier.isStatic(modifiers) + ", ");
        sBuilder.append("\nisFinal: " + Modifier.isFinal(modifiers) + ", ");
        sBuilder.append("\nisAbstract: " + Modifier.isAbstract(modifiers));
        return sBuilder.toString();
    }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
## 包装类与原生数据类型
　　相关博文： [Java 包装类 自动装箱和拆箱](http://www.cnblogs.com/mengdd/archive/2013/01/21/2869921.html)
　　写了一个调用静态方法的辅助类，我本来是这么写的：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
    /*
     * wrong:
     * public static Object invokePublicStaticMethod(String className,
     * String methodName, Object[] params) throws Exception {
     *
     * Class<?> cls = Class.forName(className);
     * Class<?>[] paramTypes = new Class<?>[params.length];
     * for (int i = 0; i < params.length; ++i) {
     * paramTypes[i] = params[i].getClass();
     *
     * }
     * Method method = cls.getMethod(methodName, paramTypes);
     * Object value = null;
     * if (isPublicStatic(method)) {
     * value = method.invoke(null, params);
     * }
     *
     * return value;
     * }
     */
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　　意图是只传入参数数组，在内部自己根据参数获取类型数组，可以少传点参数，但是这样遇到了问题。
　　在Example类里写了这么三个测试方法：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
    public static void printSomething(String line) {
        System.out.println(line);
    }
    public static int add(int a, int b) {
        return a + b;
    }
    public static double getPi() {
        return 3.14159d;
    }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　　测试的时候发现参数是String类型的时候可以正常执行，但是参数如果是原生数据类型(int类型)，用这个方法调用时就跑出了异常：
java.lang.NoSuchMethodException: com.mengdd.reflect.Example.add(java.lang.Integer, java.lang.Integer)
　　测试了一下：
```
Object[] array = new Object[] { 1, 2 };
System.out.println("getClass(): " + array[0].getClass());
System.out.println("Integer.TYPE: " + Integer.TYPE);
```
　　输出：
```
getClass(): class java.lang.Integer
Integer.TYPE: int
```
　　而那个用于测试的add()方法：
```
getReturnType: int
getParameterTypes: **[**int, int**]**
```
　　可见**Integer**和**int**被认为是两种类型，所以调用方法的时候，类型参数也还是从外部传入比较科学。
　　修改后的调用方法如下：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
    public static Object invokePublicStaticMethod(String className,
            String methodName, Class<?>[] paramTypes, Object[] params)
            throws Exception {
        Class<?> cls = Class.forName(className);
        Method method = cls.getMethod(methodName, paramTypes);
        Object value = null;
        if (isPublicStatic(method)) {
            value = method.invoke(null, params);
        }
        return value;
    }
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　　测试代码：
```
![复制代码](http://common.cnblogs.com/images/copycode.gif)
            Object result1 = ReflectUtils.invokePublicStaticMethod(
                    "com.mengdd.reflect.Example", "add", new Class<?>[] {
                            int.class, Integer.TYPE }, new Object[] { 1, 2 });
            // int.class和Integer.TYPE都行
            Assert.assertEquals(3, result1);
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
## ReflectUtils类
　　ReflectUtils类完整代码如下：
```
![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)
![复制代码](http://common.cnblogs.com/images/copycode.gif)
package com.mengdd.reflect;
import java.lang.reflect.Constructor;
import java.lang.reflect.Field;
import java.lang.reflect.Member;
import java.lang.reflect.Method;
import java.lang.reflect.Modifier;
import java.util.Arrays;
/**
 *
 * @ClassName ReflectUtils
 * @Description Reflection Helper class
 *
 * @author mengdandan
 * @Date 2014年5月13日上午10:40:32
 *
 */
public class ReflectUtils {
    /**
     * 创建类的实例，调用类的无参构造方法
     *
     * @param className
     * @return
     */
    public static Object newInstance(String className) {
        Object instance = null;
        try {
            Class<?> clazz = Class.forName(className);
            instance = clazz.newInstance();
        }
        catch (ClassNotFoundException e) {
            e.printStackTrace();
        }
        catch (InstantiationException e) {
            // if this Class represents an abstract class, an interface, an
            // array class, a primitive type, or void; or if the class has no
            // nullary constructor; or if the instantiation fails for some other
            // reason.
            e.printStackTrace();
        }
        catch (IllegalAccessException e) {
            // if the class or its nullary constructor is not accessible
            e.printStackTrace();
        }
        return instance;
    }
    /**
     * 获取所有的public构造方法的信息
     *
     * @param className
     * @return
     */
    public static String getPublicConstructorInfo(String className) {
        StringBuilder sBuilder = new StringBuilder();
        try {
            Class<?> clazz = Class.forName(className);
            Constructor<?>[] constructors = clazz.getConstructors();
            sBuilder.append(getConstructorInfo(constructors));
        }
        catch (ClassNotFoundException e) {
            e.printStackTrace();
        }
        return sBuilder.toString();
    }
    /**
     * 得到本类内声明的构造方法信息
     *
     * @param className
     * @return
     */
    public static String getDeclearedConstructorInfo(String className) {
        StringBuilder sBuilder = new StringBuilder();
        try {
            Class<?> clazz = Class.forName(className);
            Constructor<?>[] constructors = clazz.getDeclaredConstructors();
            sBuilder.append(getConstructorInfo(constructors));
        }
        catch (ClassNotFoundException e) {
            e.printStackTrace();
        }
        return sBuilder.toString();
    }
    /**
     * 获取public的字段信息
     *
     * @param className
     * @return
     */
    public static String getPublicFieldInfo(String className) {
        StringBuilder sBuilder = new StringBuilder();
        try {
            Class<?> clazz = Class.forName(className);
            Field[] fields = clazz.getFields();
            sBuilder.append(getFieldInfo(fields));
        }
        catch (ClassNotFoundException e) {
            e.printStackTrace();
        }
        return sBuilder.toString();
    }
    /**
     * 获取本类内声明的字段信息
     *
     * @param className
     * @return
     */
    public static String getDecleardFieldInfo(String className) {
        StringBuilder sBuilder = new StringBuilder();
        try {
            Class<?> clazz = Class.forName(className);
            Field[] fields = clazz.getDeclaredFields();
            sBuilder.append(getFieldInfo(fields));
        }
        catch (ClassNotFoundException e) {
            e.printStackTrace();
        }
        return sBuilder.toString();
    }
    /**
     * 得到所有public方法信息
     *
     * @param className
     * @return
     */
    public static String getPublicMethodInfos(String className) {
        StringBuilder sBuilder = new StringBuilder();
        try {
            Class<?> clazz = Class.forName(className);
            Method[] methods = clazz.getMethods();// 得到所有的public方法，包括从基类继承的
            sBuilder.append(getMethodInfo(methods));
        }
        catch (ClassNotFoundException e) {
            e.printStackTrace();
        }
        return sBuilder.toString();
    }
    /**
     * 得到类内声明的方法信息
     *
     * @param className
     * @return
     */
    public static String getDeclaredMethodInfos(String className) {
        StringBuilder sBuilder = new StringBuilder();
        try {
            Class<?> clazz = Class.forName(className);
            Method[] methods = clazz.getDeclaredMethods();// 得到本类声明的所有方法,包括私有方法
            // clazz.getMethods(); 会返回所有public的方法，但是包括基类Object的方法
            sBuilder.append(getMethodInfo(methods));
        }
        catch (ClassNotFoundException e) {
            e.printStackTrace();
        }
        catch (Exception e) {
            e.printStackTrace();
        }
        return sBuilder.toString();
    }
    /**
     * 得到构造器信息
     *
     * @param constructor
     * @return
     */
    private static String getConstructorInfo(Constructor<?> constructor) {
        StringBuilder sBuilder = new StringBuilder();
        sBuilder.append("name: " + constructor.getName());
        sBuilder.append("\ngetParameterTypes: "
                + Arrays.toString(constructor.getParameterTypes()));
        return sBuilder.toString();
    }
    /**
     * 将一组构造器的信息组成一个字符串返回
     *
     * @param constructors
     * @return
     */
    private static String getConstructorInfo(Constructor<?>[] constructors) {
        StringBuilder sBuilder = new StringBuilder();
        int i = 0;
        for (Constructor<?> c : constructors) {
            sBuilder.append("method: " + ++i + " : ");
            sBuilder.append("\n" + getConstructorInfo(c));
            sBuilder.append("\n");
        }
        return sBuilder.toString();
    }
    /**
     * 获取字段信息，组成一个字符串返回
     *
     * @param field
     * @return
     */
    private static String getFieldInfo(Field field) {
        StringBuilder sBuilder = new StringBuilder();
        sBuilder.append("name: " + field.getName());
        sBuilder.append("\ngetType: " + field.getType());
        sBuilder.append(getModifiersInfo(field));
        return sBuilder.toString();
    }
    /**
     * 获取一组字段的信息，返回字符串
     *
     * @param fields
     * @return
     */
    private static String getFieldInfo(Field[] fields) {
        StringBuilder sBuilder = new StringBuilder();
        int i = 0;
        for (Field field : fields) {
            sBuilder.append("field: " + ++i + " : ");
            sBuilder.append("\n" + getFieldInfo(field));
            sBuilder.append("\n");
        }
        return sBuilder.toString();
    }
    /**
     * 获取方法的信息，组成一个字符串返回
     *
     * @param method
     * @return
     */
    private static String getMethodInfo(Method method) {
        StringBuilder sBuilder = new StringBuilder();
        sBuilder.append("name: " + method.getName());
        sBuilder.append("\ngetReturnType: " + method.getReturnType());
        sBuilder.append("\ngetParameterTypes: "
                + Arrays.toString(method.getParameterTypes()));
        sBuilder.append(getModifiersInfo(method));
        return sBuilder.toString();
    }
    /**
     * 获取一组方法的信息，组成一个字符串返回
     *
     * @param methods
     * @return
     */
    private static String getMethodInfo(Method[] methods) {
        StringBuilder sBuilder = new StringBuilder();
        int i = 0;
        for (Method method : methods) {
            sBuilder.append("method: " + ++i + " : ");
            sBuilder.append("\n" + getMethodInfo(method));
            sBuilder.append("\n");
        }
        return sBuilder.toString();
    }
    /**
     * 获取修饰符信息
     *
     * @param member
     * @return
     */
    private static String getModifiersInfo(Member member) {
        StringBuilder sBuilder = new StringBuilder();
        int modifiers = member.getModifiers();
        sBuilder.append("\ngetModifiers: " + +modifiers + ", ");// 得到修饰符编码
        sBuilder.append("\nisPublic: " + Modifier.isPublic(modifiers) + ", ");
        sBuilder.append("\nisPrivate: " + Modifier.isPrivate(modifiers) + ", ");
        sBuilder.append("\nisStatic: " + Modifier.isStatic(modifiers) + ", ");
        sBuilder.append("\nisFinal: " + Modifier.isFinal(modifiers) + ", ");
        sBuilder.append("\nisAbstract: " + Modifier.isAbstract(modifiers));
        return sBuilder.toString();
    }
    /**
     * 是否是公用静态方法
     *
     * @param member
     * @return
     */
    private static boolean isPublicStatic(Member member) {
        boolean isPS = false;
        int mod = member.getModifiers();
        isPS = Modifier.isPublic(mod) && Modifier.isStatic(mod);
        return isPS;
    }
    /**
     * 调用静态方法
     *
     * @param className
     * @param methodName
     * @param paramTypes
     * @param params
     * @return
     * @throws Exception
     */
    public static Object invokePublicStaticMethod(String className,
            String methodName, Class<?>[] paramTypes, Object[] params)
            throws Exception {
        Class<?> cls = Class.forName(className);
        Method method = cls.getMethod(methodName, paramTypes);
        Object value = null;
        if (isPublicStatic(method)) {
            value = method.invoke(null, params);
        }
        return value;
    }
    /*
     * wrong:
     * public static Object invokePublicStaticMethod(String className,
     * String methodName, Object[] params) throws Exception {
     *
     * Class<?> cls = Class.forName(className);
     * Class<?>[] paramTypes = new Class<?>[params.length];
     * for (int i = 0; i < params.length; ++i) {
     * paramTypes[i] = params[i].getClass();
     *
     * }
     * Method method = cls.getMethod(methodName, paramTypes);
     * Object value = null;
     * if (isPublicStatic(method)) {
     * value = method.invoke(null, params);
     * }
     *
     * return value;
     * }
     */
    /**
     * 调用私有方法
     *
     * @param obj
     *            调用类对象
     * @param methodName
     *            方法名
     * @param paramTypes
     *            参数类型
     * @param params
     *            参数
     * @return
     * @throws Exception
     */
    public static Object invokePrivateMethod(Object obj, String methodName,
            Class<?>[] paramTypes, Object[] params) throws Exception {
        Object value = null;
        Class<?> cls = obj.getClass();
        // 注意不要用getMethod(),因为getMethod()返回的都是public方法
        Method method = cls.getDeclaredMethod(methodName, paramTypes);
        method.setAccessible(true);// 抑制Java的访问控制检查
        value = method.invoke(obj, params);
        return value;
    }
}
![复制代码](http://common.cnblogs.com/images/copycode.gif)
```
　　测试类和测试代码：
```
![](http://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)Example.java
```
```
![](http://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)ReflectTest.java
```
　　预计后期还会有进一步更新完善，项目地址等待补充。
