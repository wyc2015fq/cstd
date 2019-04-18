# 通过PropertyDescriptor反映射调用set和get方法 - z69183787的专栏 - CSDN博客
2012年12月27日 15:20:55[OkidoGreen](https://me.csdn.net/z69183787)阅读数：27947
```java
package com.zhoushun;
import java.lang.reflect.Method;
import java.lang.reflect.Field;
import java.beans.PropertyDescriptor;
public class PropertyUtil {
	@SuppressWarnings("unchecked")
	public static PropertyDescriptor getPropertyDescriptor(Class clazz, String propertyName) {
		StringBuffer sb = new StringBuffer();//构建一个可变字符串用来构建方法名称
		Method setMethod = null;
		Method getMethod = null;
		PropertyDescriptor pd = null;
		try {
			Field f = clazz.getDeclaredField(propertyName);//根据字段名来获取字段
			if (f!= null) {
				//构建方法的后缀
			   String methodEnd = propertyName.substring(0, 1).toUpperCase() + propertyName.substring(1);
			   sb.append("set" + methodEnd);//构建set方法
			   setMethod = clazz.getDeclaredMethod(sb.toString(), new Class[]{ f.getType() });
			   sb.delete(0, sb.length());//清空整个可变字符串
			   sb.append("get" + methodEnd);//构建get方法
			   //构建get 方法
			   getMethod = clazz.getDeclaredMethod(sb.toString(), new Class[]{ });
			   //构建一个属性描述器 把对应属性 propertyName 的 get 和 set 方法保存到属性描述器中
			   pd = new PropertyDescriptor(propertyName, getMethod, setMethod);
			}
		} catch (Exception ex) {
				ex.printStackTrace();
		}
	
		return pd;
	}
	
	@SuppressWarnings("unchecked")
	public static void setProperty(Object obj,String propertyName,Object value){
		Class clazz = obj.getClass();//获取对象的类型
		PropertyDescriptor pd = getPropertyDescriptor(clazz,propertyName);//获取 clazz 类型中的 propertyName 的属性描述器
		Method setMethod = pd.getWriteMethod();//从属性描述器中获取 set 方法
		try {
			setMethod.invoke(obj, new Object[]{value});//调用 set 方法将传入的value值保存属性中去
		}catch (Exception e){
			e.printStackTrace();
		}
	}
	
	@SuppressWarnings("unchecked")
	public static Object getProperty(Object obj, String propertyName){
	   Class clazz = obj.getClass();//获取对象的类型
	   PropertyDescriptor pd = getPropertyDescriptor(clazz,propertyName);//获取 clazz 类型中的 propertyName 的属性描述器
	   Method getMethod = pd.getReadMethod();//从属性描述器中获取 get 方法
	   Object value =null ;
	   try {
		   value = getMethod.invoke(clazz, new Object[]{});//调用方法获取方法的返回值
	   } catch (Exception e) {
		   e.printStackTrace();
	   }
	   return value;//返回值
	}
}
```
2
```java
public boolean setValue(Object objSet, Object objGet)
        throws IllegalArgumentException, SecurityException, InstantiationException, IllegalAccessException, InvocationTargetException, NoSuchMethodException, IntrospectionException
    {
        boolean flag = true;
        Field fields[] = objSet.getClass().getDeclaredFields();
        String value = "";
        String fieldNameGet = "";
        List list = new ArrayList();
        Field afield[];
        int j = (afield = fields).length;
        for(int i = 0; i < j; i++)
        {
            Field field = afield[i];
            String fieldName = field.getName();
            fieldNameGet = xmlToModel(fieldName);
            if(!"error".equals(fieldNameGet))
            {
                PropertyDescriptor pd = new PropertyDescriptor(fieldNameGet, objGet.getClass());
                Method getMethod = pd.getReadMethod();
                value = String.valueOf(getMethod.invoke(objGet, new Object[0]));
                boolean checkResult = returnMessage.checkValue(value, fieldNameGet);
                if(checkResult)
                {
                    PropertyDescriptor pd1 = new PropertyDescriptor(fieldName, objSet.getClass());
                    Method setMethod = pd1.getWriteMethod();
                    setMethod.invoke(objSet, new Object[] {
                        field.getType().getConstructor(new Class[] {
                            java/lang/String
                        }).newInstance(new Object[] {
                            value
                        })
                    });
                } else
                {
                    flag = checkResult;
                }
            }
        }
        return flag;
    }
```
3
```java
import java.beans.PropertyDescriptor;
import java.lang.reflect.Field;
import java.lang.reflect.Method;
public class ReflectTest {
 
 public static void main(String[] args) throws Exception {
  Class clazz = Class.forName("TaskProvidePropsList");//这里的类名是全名。。有包的话要加上包名
  Object obj = clazz.newInstance();
  Field[] fields = clazz.getDeclaredFields();
  //写数据
  for(Field f : fields) {
   PropertyDescriptor pd = new PropertyDescriptor(f.getName(), clazz);
   Method wM = pd.getWriteMethod();//获得写方法
   wM.invoke(obj, 2);//因为知道是int类型的属性，所以传个int过去就是了。。实际情况中需要判断下他的参数类型
  }
  //读数据
  for(Field f : fields) {
   PropertyDescriptor pd = new PropertyDescriptor(f.getName(), clazz);
   Method rM = pd.getReadMethod();//获得读方法
   Integer num = (Integer) rM.invoke(obj);//因为知道是int类型的属性,所以转换成integer就是了。。也可以不转换直接打印
   System.out.println(num);
  }
 }
}
```
