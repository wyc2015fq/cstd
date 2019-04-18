# 泛型T的类型获取 - z69183787的专栏 - CSDN博客
2016年09月16日 17:30:41[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2558
T.getClass()或者T.class都是非法的，因为T是泛型变量。
由于一个类的类型是什么是在编译期处理的，故不能在运行时直接在Base里得到T的实际类型。
**[java]**[view
 plain](http://blog.csdn.net/running_snail_/article/details/7077947#)[copy](http://blog.csdn.net/running_snail_/article/details/7077947#)
- /**
-  *@author Joshua
-  *@version 2011-12-15 下午02:27:43
-  */
- /**
-  * 可以在service层直接调用,也可以在DAO层扩展调用
-  */
- publicclass BaseDaoImpl<T, PK extends Serializable> implements BaseDao<T, PK>{  
...
**[java]**[view
 plain](http://blog.csdn.net/running_snail_/article/details/7077947#)[copy](http://blog.csdn.net/running_snail_/article/details/7077947#)
- private Class<T> persistentClass;  
- /**
-     * 用于Dao层子类使用的构造函数. 通过子类的泛型定义取得对象类型
-     */
- 
- @SuppressWarnings("unchecked")  
- public BaseDaoImpl(){  
- //getClass() 返回表示此 Class 所表示的实体（类、接口、基本类型或 void）的超类的 Class。
- this.persistentClass=(Class<T>)getSuperClassGenricType(getClass(), 0);  
- }  
...
**[java]**[view
 plain](http://blog.csdn.net/running_snail_/article/details/7077947#)[copy](http://blog.csdn.net/running_snail_/article/details/7077947#)
- /**
-      * 通过反射, 获得定义Class时声明的父类的泛型参数的类型. 如无法找到, 返回Object.class.
-      * 
-      *@param clazz
-      *            clazz The class to introspect
-      * @param index
-      *            the Index of the generic ddeclaration,start from 0.
-      * @return the index generic declaration, or Object.class if cannot be
-      *         determined
-      */
- @SuppressWarnings("unchecked")  
- publicstatic Class<Object> getSuperClassGenricType(final Class clazz, finalint index) {  
- 
- //返回表示此 Class 所表示的实体（类、接口、基本类型或 void）的直接超类的 Type。
-         Type genType = clazz.getGenericSuperclass();  
- 
- if (!(genType instanceof ParameterizedType)) {  
- return Object.class;  
-         }  
- //返回表示此类型实际类型参数的 Type 对象的数组。
-         Type[] params = ((ParameterizedType) genType).getActualTypeArguments();  
- 
- if (index >= params.length || index < 0) {  
- return Object.class;  
-         }  
- if (!(params[index] instanceof Class)) {  
- return Object.class;  
-         }  
- 
- return (Class) params[index];  
-     }  
