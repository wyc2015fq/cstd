# BeanUtils.copyProperties方法遇到null值问题 - z69183787的专栏 - CSDN博客
2014年07月01日 20:54:57[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6966
转自:http://simen-net.iteye.com/blog/644801
在MVC的开发模式中经常需要将model与pojo的数据绑定，apache和spring的工具包中都有BeanUtils，使用其中的copyProperties方法可以非常方便的进行这些工作，但在实际应用中发现，对于null的处理不太符合个人的需要，例如在进行修改操作中只需要对model中某一项进行修改，那么一般我们在页面上只提交model的ID及需要修改项的值，这个时候使用BeanUtils.copyProperties会将其他的null绑定到pojo中去。为解决这个问题我重写了部分spring BeanUtils的代码如下
**[java]**[view
 plain](http://blog.csdn.net/thc1987/article/details/7546109#)[copy](http://blog.csdn.net/thc1987/article/details/7546109#)
- publicabstractclass BeanUtils extends org.springframework.beans.BeanUtils {  
- 
- publicstaticvoid copyProperties(Object source, Object target) throws BeansException {  
-     Assert.notNull(source, "Source must not be null");  
-     Assert.notNull(target, "Target must not be null");  
-     Class<?> actualEditable = target.getClass();  
-     PropertyDescriptor[] targetPds = getPropertyDescriptors(actualEditable);  
- for (PropertyDescriptor targetPd : targetPds) {  
- if (targetPd.getWriteMethod() != null) {  
-         PropertyDescriptor sourcePd = getPropertyDescriptor(source.getClass(), targetPd.getName());  
- if (sourcePd != null && sourcePd.getReadMethod() != null) {  
- try {  
-             Method readMethod = sourcePd.getReadMethod();  
- if (!Modifier.isPublic(readMethod.getDeclaringClass().getModifiers())) {  
-               readMethod.setAccessible(true);  
-             }  
-             Object value = readMethod.invoke(source);  
- // 这里判断以下value是否为空 当然这里也能进行一些特殊要求的处理 例如绑定时格式转换等等
- if (value != null) {  
-               Method writeMethod = targetPd.getWriteMethod();  
- if (!Modifier.isPublic(writeMethod.getDeclaringClass().getModifiers())) {  
-                 writeMethod.setAccessible(true);  
-               }  
-               writeMethod.invoke(target, value);  
-             }  
-           } catch (Throwable ex) {  
- thrownew FatalBeanException("Could not copy properties from source to target", ex);  
-           }  
-         }  
-       }  
-     }  
-   }  
- }  
- 
