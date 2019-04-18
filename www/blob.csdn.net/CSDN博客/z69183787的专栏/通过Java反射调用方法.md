# 通过Java反射调用方法 - z69183787的专栏 - CSDN博客
2014年04月16日 14:32:00[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1298
 通过Java反射调用方法
这是个测试用的例子，通过反射调用对象的方法。
TestRef.java
import java.lang.reflect.Method; 
import java.lang.reflect.InvocationTargetException;
/** 
*/ 
public class TestRef {
    public static void main(String args[]) throws NoSuchMethodException, IllegalAccessException, InvocationTargetException { 
        Foo foo = new Foo("这个一个Foo对象！"); 
        Class clazz = foo.getClass(); 
        Method m1 = clazz.getDeclaredMethod("outInfo"); 
        Method m2 = clazz.getDeclaredMethod("setMsg", String.class); 
        Method m3 = clazz.getDeclaredMethod("getMsg"); 
        m1.invoke(foo); 
        m2.invoke(foo, "重新设置msg信息！"); 
        String msg = (String) m3.invoke(foo); 
        System.out.println(msg); 
    } 
}
class Foo { 
    private String msg;
    public Foo(String msg) { 
        this.msg = msg; 
    }
    public void setMsg(String msg) { 
        this.msg = msg; 
    }
    public String getMsg() { 
        return msg; 
    }
    public void outInfo() { 
        System.out.println("这是测试Java反射的测试类"); 
    } 
}
控制台输出结果：
这是测试Java反射的测试类 
重新设置msg信息！
----------------------------------------------------------
**Java反射方法调用 **反射方法
/**
  * @param methodObject  方法所在的对象
  * @param methodName    方法名
  * @param args          方法名参数数组
*/
public Object invokeMethod(Object methodObject, String methodName, Object[] args)
   throws Exception {
  Class ownerClass = methodObject.getClass();
  Class[] argsClass = new Class[args.length];
  for (int i = 0, j = args.length; i < j; i++) {
   argsClass[i] = args[i].getClass();
  }
  Method method = ownerClass.getMethod(methodName, argsClass);
  return method.invoke(methodObject, args);
 }
调用方式：
        String[] args={,,,};
        invokeMethod(new MyObject(),getName,args);
