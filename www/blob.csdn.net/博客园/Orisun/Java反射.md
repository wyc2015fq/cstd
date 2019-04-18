# Java反射 - Orisun - 博客园







# [Java反射](https://www.cnblogs.com/zhangchaoyang/articles/5095393.html)





## 获取数组类型

```
int[] arr=new int[3];
System.out.println(arr.getClass());
```

输出的是：

class [I

```
package test;

import java.lang.reflect.Method;

import org.junit.Assert;

public class ReflectArray {

    static class A {
        int get(int[] arr) {
            int rect = 0;
            for (int i = 0; i < arr.length; i++) {
                rect += arr[i] ^ 2;
            }
            return rect;
        }
    }

    public static Class<?> getClass(String className)
            throws ClassNotFoundException {
        // switch表达式中使用String是Java1.7的新特性，老版本的Java需要使用if-else结构。switch比if-else更高效
        switch (className) {
            case "int":
                return int.class;
            case "String":
                return String.class;
            case "double":
                return double.class;
            case "float":
                return float.class;
            case "byte":
                return byte.class;
            case "short":
                return short.class;
            case "Integer":
                return Integer.class;
            case "Double":
                return Double.class;
            case "Float":
                return Float.class;
            case "Byte":
                return Byte.class;
            case "Short":
                return Short.class;
            // 数组类型
            case "int[]":
                return Class.forName("[I");
            case "String[]":
                return Class.forName("[Ljava.lang.String;");
            case "double[]":
                return Class.forName("[D");
            case "float[]":
                return Class.forName("[F");
            case "byte[]":
                return Class.forName("[B");
            case "short[]":
                return Class.forName("[S");
            case "Integer[]":
                return Class.forName("[Ljava.lang.Integer;");
            case "Double[]":
                return Class.forName("[Ljava.lang.Double;");
            case "Float[]":
                return Class.forName("[Ljava.lang.Float;");
            case "Byte[]":
                return Class.forName("[Ljava.lang.Byte;");
            case "Short[]":
                return Class.forName("[Ljava.lang.Short;");
            default:
                return Class.forName(className);
        }
    }

    public static void main(String[] args) throws ClassNotFoundException,
            SecurityException, NoSuchMethodException {
        // 等价于Class<?> clazz=ReflectArray.A.class;
        Class<?> clazz = Class.forName("test.ReflectArray$A");
        Method m = clazz.getDeclaredMethod("get", getClass("int[]"));
        Class<?> r = m.getReturnType();
        Assert.assertEquals(r, getClass("int"));
    }
}
```



## 获取泛型类型

```
package test;

import java.lang.reflect.Field;
import java.lang.reflect.Method;
import java.lang.reflect.ParameterizedType;
import java.lang.reflect.Type;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public class Generic {

    static interface Iface1 {}

    static interface Iface2<V> {}

    static class Parent<K, V> {}

    static class Son<K, V> extends Parent<Integer, String> implements Iface1,
            Iface2<String> {
        Map<Integer, String> map;
        Map<Integer, String> get(List<Byte> list) {
            Map<Integer, String> map = new HashMap<Integer, String>();
            map.put(1, "abc");
            return map;
        }
    }

    public static void printGenericType(Type type) {
        if (type instanceof ParameterizedType) {
            ParameterizedType ptype = (ParameterizedType) type;
            Type[] typeArgs = ptype.getActualTypeArguments();
            for (Type typeArg : typeArgs) {
                Class<?> typeCls = (Class<?>) typeArg; // Class是Type的子类
                System.out.println(typeCls);
            }
        } else {
            System.out.println(type + "(不带泛型参数)");
        }
    }

    public static void main(String[] args) throws ClassNotFoundException,
            NoSuchFieldException, SecurityException, NoSuchMethodException {
        Class<?> clazz = Class.forName("test.Generic$Son");
        Type parentType = clazz.getGenericSuperclass(); // 获取父类
        System.out.println("父类泛型");
        printGenericType(parentType);
        System.out.println("");

        Type[] interfaceTypes = clazz.getGenericInterfaces(); // 获取所有继承的接口
        for (Type interfaceType : interfaceTypes) {
            System.out.println("接口泛型");
            printGenericType(interfaceType);
            System.out.println("");
        }

        Field field = clazz.getDeclaredField("map"); // 获取成员变量
        Type fieldType = field.getGenericType(); // 获取成员变量的类型
        System.out.println("成员变量泛型");
        printGenericType(fieldType);
        System.out.println("");

        Method method = clazz.getDeclaredMethod("get", List.class); // 获取成员方法
        Type[] parameterTypes = method.getGenericParameterTypes(); // 获取成员方法的参数类型
        for (Type parameterType : parameterTypes) {
            System.out.println("方法参数泛型");
            printGenericType(parameterType);
            System.out.println("");
        }

        Type returnType = method.getGenericReturnType(); // 获取成员方法的返回类型
        System.out.println("方法返回类型泛型");
        printGenericType(returnType);
    }
}
```

输出：

父类泛型

class java.lang.Integer

class java.lang.String



接口泛型

interface test.Generic$Iface1(不带泛型参数)



接口泛型

class java.lang.String



成员变量泛型

class java.lang.Integer

class java.lang.String



方法参数泛型

class java.lang.Byte



方法返回类型泛型

class java.lang.Integer

class java.lang.String












