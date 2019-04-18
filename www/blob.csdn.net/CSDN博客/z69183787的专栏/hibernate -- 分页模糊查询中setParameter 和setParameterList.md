# hibernate -- 分页模糊查询中setParameter 和setParameterList - z69183787的专栏 - CSDN博客
2014年12月11日 17:02:18[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3995
个人分类：[Spring-模板类																[对象映射框架-Hibernate](https://blog.csdn.net/z69183787/article/category/2175489)](https://blog.csdn.net/z69183787/article/category/2176815)
在分页模糊查询中碰到setParameter 和setParameterList这两个方法
setParameter
以前就只会用setParameter(int arg,String str),我用到了from table A  where 1=1 and ... like ?  
还可以用另外一种方法： setParameter(String arg0, Object arg1)
但是这种方法在hql时注意要这样写如：
**[java]**[view plain](http://blog.csdn.net/snn1410/article/details/8879239#)[copy](http://blog.csdn.net/snn1410/article/details/8879239#)
- from User u where u.name=:username and u.yearold=:yearold  
- 
- 这就是一个常见的简单值的占位符式的查询，通过这样的方式就可以把值注入到参数中：  
- 
- query.setParameter(“username”,”bluedavy”);  
- 
- query.setParameter(“yearold”,25);   
格式： “ ：设置的参数名 ”-- username,yearold
setParameterList
一般用于in查询，可以是int数组，string 数组
**[java]**[view plain](http://blog.csdn.net/snn1410/article/details/8879239#)[copy](http://blog.csdn.net/snn1410/article/details/8879239#)
-    in 查询  
- 
- in 查询也是经常被使用到的一种查询，在 Hibernate 中表现出来会稍有不同，不过如果按照对象观点去看就很容易理解了，例如下面这句：  
- 
- from User u where u.name in (:usernameList)  
- 
- 在 Hibernate 中通过这样的方式将值注入到这个参数中：  
- 
- List list=new ArrayList();  
- 
- list.add(“jerry”);  
- 
- list.add(“bluedavy”);  
- 
- query.setParameterList(“usernameList”,list);   
详细参照：
[http://zhaoyari.i.sohu.com/blog/view/56828595.htm](http://zhaoyari.i.sohu.com/blog/view/56828595.htm)
[http://www.linuxso.com/sql/14043.html](http://www.linuxso.com/sql/14043.html)
