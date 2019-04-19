# getGenericSuperclass()和getSuperclass() - 零度的博客专栏 - CSDN博客
2016年06月28日 16:49:10[零度anngle](https://me.csdn.net/zmx729618)阅读数：1164
                
一.说明
1.Class<? super T> getSuperclass():返回本类的父类
2.Type getGenericSuperclass():返回本类的父类,并且包含泛型参数信息
二.例子
1.ClassA.java
```
package zmx.test;
public class ClassA<T> {
	
	  private T obj;  
	    
	  public void setObject(T obj) {  
	    
          this.obj = obj;  
	    
	  }  
	    
	      
	   
	  public T getObject() {  
	    
	      return obj;  
	    
	    } 
}
```
2.Test.java
```
package zmx.test;
import java.lang.reflect.ParameterizedType;
import java.lang.reflect.Type;
public class Test <T> extends ClassA<T>{  
	
	
	public static void main(String args[]) throws Exception{  
		  
		    System.out.println("======getSuperclass======:");    
		    System.out.println(Test.class.getSuperclass().getName());            
		    System.out.println("======getGenericSuperclass======:");    
		    Type t = Test.class.getGenericSuperclass();    
		    System.out.println(t);    
		    if (ParameterizedType.class.isAssignableFrom(t.getClass())) {   
			      System.out.print("----------->getActualTypeArguments:");    
			      for (Type t1:((ParameterizedType)t).getActualTypeArguments()) {    
			        System.out.print(t1 + ",");    
			      }    
			      System.out.println();    
	        }  
	}
}
```
输出结果:
======getSuperclass======:
ClassA
======getGenericSuperclass======:
ClassA<T>
----------->getActualTypeArguments:T,
