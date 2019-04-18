# Java使用反射机制使用get set 方法 - z69183787的专栏 - CSDN博客
2013年11月26日 17:14:20[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4417
```java
import java.beans.PropertyDescriptor;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
/**
 * 
 */
/** 
 * @ClassName: ReflectTest 
 * @Description: TODO(这里用一句话描述这个类的作用) 
 * @author zhoushun 
 * @date 2013年11月26日 下午5:11:35 
 *  
 */
public class ReflectTest {
	public static void main() throws Exception{
		ReflectTest bo = new ReflectTest();
		Field[] fields = bo.getClass().getDeclaredFields();  
       // 写数据  
      for(Field ff : fields) {  
    	  String name = ff.getName();    //获取属性的名字  
    	  String type = ff.getGenericType().toString();    //获取属性的类型
          ff.setAccessible(true);  
       PropertyDescriptor pd = new PropertyDescriptor(ff.getName(), bo.getClass());  
       Method rM = pd.getReadMethod();//获得读方法  
       Object num = (Object) rM.invoke(bo);  
       if(num == null) {  
           Method wM = pd.getWriteMethod();//获得写方法  
           if(type.equals("class java.lang.Integer")){       
              
           }  
           if(type.equals("class java.lang.Short")){       
              
           }         
           if(type.equals("class java.lang.Double")){       
              
           }                    
           if(type.equals("class java.lang.Boolean")){  
              
           }  
           if(type.equals("class java.util.Date")){  
             
           }   
           wM.invoke(bo, "1");//因为知道是int类型的属性，所以传个int过去就是了。。实际情况中需要判断下他的参数类型  
       }  
      }  
	}
}
```
