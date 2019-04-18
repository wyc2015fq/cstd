# java 获取 泛型类型（接口及超类） - z69183787的专栏 - CSDN博客
2014年12月25日 10:30:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：12768
```java
package generic.portal;
/**
 * Created by Administrator on 2017/1/10.
 */
public interface GenericInterface<T> {
}
```
```java
package generic.portal;
import java.lang.reflect.ParameterizedType;
import java.lang.reflect.Type;
import java.lang.reflect.TypeVariable;
/**
 * Created by Administrator on 2017/1/10.
 */
public class GenericParent<T,K> {
    public Class<T> entityClass;
    public GenericParent(){
        TypeVariable[] tValue = GenericParent.class.getTypeParameters();
        System.out.println(tValue[0].getName());
        System.out.println(getClass().getName());
        Type t = getClass().getGenericSuperclass();
        System.out.println(t);
        System.out.println(t.getClass().getName());
        entityClass = (Class<T>)((ParameterizedType)t).getActualTypeArguments()[0];
        System.out.println(entityClass.getName());
    }
    public static void main(String[] args){
        GenericParent<String,Integer> obj = new GenericParent<String,Integer>();
    }
}
```
```java
package generic.portal;
import com.google.gson.GsonBuilder;
import java.lang.reflect.*;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
/**
 * Created by Administrator on 2017/1/10.
 */
public class GenericTest extends GenericParent<String, Integer> implements GenericInterface<Double> {
    private static Class getClass(Type type, int i) {
        if (type instanceof ParameterizedType) { // 处理泛型类型
            System.out.println("1111111");
            return getGenericClass((ParameterizedType) type, i);
        } else if (type instanceof TypeVariable) {
            System.out.println("--------" + ((Class) getClass(((TypeVariable) type).getBounds()[0], 0)).getName());
            return (Class) getClass(((TypeVariable) type).getBounds()[0], 0); // 处理泛型擦拭对象
        } else {// class本身也是type，强制转型
            return (Class) type;
        }
    }
    private static Class getGenericClass(ParameterizedType parameterizedType, int i) {
        Object genericClass = parameterizedType.getActualTypeArguments()[i];
        if (genericClass instanceof ParameterizedType) { // 处理多级泛型
            System.out.println("111111");
            return (Class) ((ParameterizedType) genericClass).getRawType();
        } else if (genericClass instanceof GenericArrayType) { // 处理数组泛型
            return (Class) ((GenericArrayType) genericClass).getGenericComponentType();
        } else if (genericClass instanceof TypeVariable) { // 处理泛型擦拭对象
            System.out.println("33333333");
            return (Class) getClass(((TypeVariable) genericClass).getBounds()[0], 0);
        } else {
            System.out.println("444444");
            return (Class) genericClass;
        }
    }
    public String[] getPayments(String[] payments,Integer[] sss, List<TodoInfo> products){
        return payments;
    }
    public static void main(String[] args) {
        GenericTest obj = new GenericTest();
        System.out.println(obj.getClass());
        Type[] a = obj.getClass().getGenericInterfaces();
        Type b = obj.getClass().getGenericSuperclass();
        System.out.println("//zhoushun  ");
        //interface
        for (Type t : a) {
            Type[] temp = ((ParameterizedType) t).getActualTypeArguments();
            for (Type c : temp) {
                System.out.println();
                System.out.println(((Class) c).getName());
            }
        }
        TypeVariable[] v = obj.getClass().getSuperclass().getTypeParameters();
        for (TypeVariable vv : v) {
            System.out.println(vv.getName());
            Type[] tt = vv.getBounds();
            for (Type c : tt) {
                System.out.println(((Class) c).getName());
            }
        }
        //super
        Type[] temp = ((ParameterizedType) b).getActualTypeArguments();
        for (Type c : temp) {
            System.out.println(((Class) c).getName());
        }
        Map map = new HashMap();
        map.put("a",null);
        map.put(1,"");
        map.put("das","asd");
        map.put("dasdsa","<a href='dasdsa'>");
        System.out.println(new GsonBuilder().serializeNulls().disableHtmlEscaping().create().toJson(map));
//        List<?>[] lsa = new List<?>[10]; // OK, array of unbounded wildcard type.
//        Object o = lsa;
//        Object[] oa = (Object[]) o;
//        List<Integer> li = new ArrayList<Integer>();
//        li.add(new Integer(3));
//        oa[1] = li; // Correct.
//        String s = (String) lsa[1].get(0); // Run time error, but cast is explicit.
//        System.out.println(s);
        Method getPayments = null;
        try {
            getPayments = GenericTest.class.getMethod("getPayments", new Class[]{String[].class,Integer[].class, List.class});
        } catch (NoSuchMethodException e) {
            e.printStackTrace();
        }
        Type[] types = getPayments.getGenericParameterTypes();
        System.out.println("The first parameter of this method is GenericArrayType."+ types[0].getClass());
        ParameterizedType gType = (ParameterizedType)types[2];
        System.out.println("The GenericArrayType's component is String." +((Class)gType.getActualTypeArguments()[0]).getName());
    }
}
```
```java
package generic.portal;
/**
 * Created by Administrator on 2017/1/10.
 */
public class TodoInfo {
}
```
输出：
Connected to the target VM, address: '127.0.0.1:53345', transport: 'socket'
T
generic.portal.GenericTest
generic.portal.GenericParent<java.lang.String, java.lang.Integer>
sun.reflect.generics.reflectiveObjects.ParameterizedTypeImpl
java.lang.String
class generic.portal.GenericTest
//zhoushun  
java.lang.Double
T
java.lang.Object
K
java.lang.Object
java.lang.String
java.lang.Integer
{"das":"asd","1":"","a":null,"dasdsa":"<a href='dasdsa'>"}
The first parameter of this method is GenericArrayType.class java.lang.Class
The GenericArrayType's component is String.generic.portal.TodoInfo
Disconnected from the target VM, address: '127.0.0.1:53345', transport: 'socket'
Process finished with exit code 0
