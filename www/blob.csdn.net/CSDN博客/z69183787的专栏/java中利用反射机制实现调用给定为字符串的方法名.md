# java中利用反射机制实现调用给定为字符串的方法名 - z69183787的专栏 - CSDN博客
2012年10月27日 10:55:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2237
1、被测试类
package demo.reflect;
/**
 * 被测试类
 * @author wangyanjun
 *
 */
public class Quizzee {
 public void testMethod(String arg1,Integer arg2){
  System.out.println("测试结果：反射方法的调用");
  System.out.println(arg1+"   "+arg2);
  System.out.println("测试成功！");
 }
}
2、测试
package demo.reflect;
import java.lang.reflect.Method;
/**
 * 功能：调用给定为字符串的方法名
 * @author wangyanjun
 *
 */
public class Test {
 /**
  * @param args
  */
 public static void main(String[] args) {
  try {
   //返回给定字符串名的类 Class 对象
   //并创建此 Class 对象所表示的类的一个新实例
   Object object = Class.forName("demo.reflect.Quizzee").newInstance();
   //返回方法名为“testMethod”的一个 Method 对象，后面跟的是该方法参数
   Method method = object.getClass().getMethod("testMethod",
     new Class[] { String.class, Integer.class });
   //执行该方法
   method.invoke(object, new Object[]{new String("数据1"),520});
  } catch (Exception e) {
   e.printStackTrace();
  }
 }
}
3. invoke class 内 static 方法
//统一方法调用
public static String methodInvoke(String methodName,String arg1,String arg2,String arg3){
Class<?> cls = DomainCrossInfo.class;
Method[] methods = cls.getDeclaredMethods();  
String methodsName = "";
String result = "error";
for (Method method : methods) { 
methodsName += method.getName() +",";
}
if(methodsName.indexOf(methodName)>=0){
  //返回方法名为“testMethod”的一个 Method 对象，后面跟的是该方法参数
  Method method;
try {
method = cls.getMethod(methodName,new Class[] { String.class, String.class,String.class });
result = (String) method.invoke(null, new Object[]{arg1,arg2,arg3});
} catch (Exception e) {
// TODO Auto-generated catch block
e.printStackTrace();
} 
}else{
return result;
}
return result;
}
