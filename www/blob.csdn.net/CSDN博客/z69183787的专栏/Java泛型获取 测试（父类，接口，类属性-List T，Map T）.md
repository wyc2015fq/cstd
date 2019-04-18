# Java泛型获取 测试（父类，接口，类属性-List T，Map T） - z69183787的专栏 - CSDN博客
2017年01月10日 13:59:25[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1057
```java
package generic;
import org.apache.poi.ss.formula.functions.T;
import java.lang.reflect.ParameterizedType;
import java.lang.reflect.Type;
import java.lang.reflect.TypeVariable;
import java.lang.reflect.WildcardType;
import java.util.ArrayList;
import java.util.List;
/**
 * Created by Administrator on 2017/1/10.
 */
class Parent<T>{
    public Class<T> instance;
    public Parent(){
    }
}
interface GenericInterface<T> {
    final int b =1;
    static final int c =43;
    static double d=2;
    //abstract void a();
}
interface WildInterface<T extends Number> {
}
public class GenericTest extends Parent<Integer> implements GenericInterface<Double>,WildInterface<Integer>{
    public List<String> real = new ArrayList<>();
    public List<? extends Number> real2 = new ArrayList<>();
    public List real3 = new ArrayList();
    public static void main(String[] args) throws NoSuchFieldException {
        GenericTest test = new GenericTest();
        Type type = test.getClass().getGenericSuperclass();
        System.out.println(type.toString());
        System.out.println("parent class ----------------------------------------------");
        if(type instanceof ParameterizedType){
            for(Type t : ((ParameterizedType) type).getActualTypeArguments()){
                System.out.println(t.toString());
                System.out.println(((Class)t).getCanonicalName());
            }
        }
        System.out.println();
        System.out.println("interface class ----------------------------------------------");
        //interface
        for (Type t : test.getClass().getGenericInterfaces()) {
            Type[] temp = ((ParameterizedType) t).getActualTypeArguments();
            for (Type c : temp) {
                if(c instanceof WildcardType){
                    System.out.println("wild：" +((WildcardType) c).getUpperBounds().toString());
                }
                System.out.println(c);
                System.out.println(((Class) c).getName());
            }
        }
        //这个list只能是成员变量或方法变量中才能获得他的泛型。
        //can not get
        System.out.println();
        System.out.println("list class ----------------------------------------------");
        List<String> list = new ArrayList<String>();
        Type lType = list.getClass().getGenericSuperclass();
        System.out.println(lType.toString());
        if(lType instanceof ParameterizedType){
            for(Type t : ((ParameterizedType) lType).getActualTypeArguments()){
                System.out.println(t.toString());
                //System.out.println(((Class)t).getCanonicalName());
            }
        }
        //get
        System.out.println();
        System.out.println("list field ----------------------------------------------");
        lType = test.getClass().getField("real").getGenericType();
        System.out.println(lType.toString());
        if(lType instanceof ParameterizedType){
            for(Type t : ((ParameterizedType) lType).getActualTypeArguments()){
                System.out.println(t.toString());
                //System.out.println(((Class)t).getCanonicalName());
            }
        }
        System.out.println();
        System.out.println("list field WildcardType ----------------------------------------------");
        lType = test.getClass().getField("real2").getGenericType();
        System.out.println(lType.toString());
        if(lType instanceof ParameterizedType){
            for(Type t : ((ParameterizedType) lType).getActualTypeArguments()){
                System.out.println(t.toString());
                System.out.println(((WildcardType)t).getUpperBounds()[0]);
                //System.out.println(((Class)t).getCanonicalName());
            }
        }
        System.out.println();
        System.out.println("class TypeVariable ----------------------------------------------");
        TypeVariable[] v  = test.getClass().getSuperclass().getTypeParameters();
        for (TypeVariable vv : v) {
            System.out.println(vv.getName());
            Type[] tt = vv.getBounds();
            for (Type c : tt) {
                System.out.println(((Class) c).getName());
            }
        }
    }
}
```
```java
输出：
```
generic.Parent<java.lang.Integer>
parent class ----------------------------------------------
class java.lang.Integer
java.lang.Integer
interface class ----------------------------------------------
class java.lang.Double
java.lang.Double
class java.lang.Integer
java.lang.Integer
list class ----------------------------------------------
java.util.AbstractList<E>
E
list field ----------------------------------------------
java.util.List<java.lang.String>
class java.lang.String
list field WildcardType ----------------------------------------------
java.util.List<? extends java.lang.Number>
? extends java.lang.Number
class java.lang.Number
class TypeVariable ----------------------------------------------
T
java.lang.Object
