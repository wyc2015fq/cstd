# Java List 复制 - z69183787的专栏 - CSDN博客
2013年03月28日 11:06:12[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1054
个人分类：[Java-知识](https://blog.csdn.net/z69183787/article/category/2175279)

开发时, 很多时候我们需要实现 List 的复制, 如果使用For循环复制,  需要写好几行代码, 也不能复用 .
其实我们可以借助泛型写一个通用的方法, 以后都可使用这个方法实现List 的复制 .
Java代码  ![收藏代码](http://jxdwuao.iteye.com/images/icon_star.png)
- /**
-  * 复制集合
-  * @param <E>
-  * @param source
-  * @param destinationClass
-  * @return
-  * @throws InstantiationException 
-  * @throws InvocationTargetException 
-  * @throws IllegalAccessException 
-  */
- publicstatic <E> List<E> copyTo(List<?> source, Class<E> destinationClass) throws IllegalAccessException, InvocationTargetException, InstantiationException{  
- if (source.size()==0) return Collections.emptyList();  
-     List<E> res = new ArrayList<E>(source.size());  
- for (Object o : source) {  
-         E e = destinationClass.newInstance();  
-         BeanUtils.copyProperties(e, o);  
-         res.add(e);  
-     }  
- return res;  
- }  
