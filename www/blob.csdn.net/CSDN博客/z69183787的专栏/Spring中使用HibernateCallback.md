# Spring中使用HibernateCallback - z69183787的专栏 - CSDN博客
2013年12月27日 14:34:18[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1026
HibernateTemplate还提供一种更加灵活的方式来操作数据库，通过这种方式可以完全使用Hibernate的操作方式。这种灵活方式主要是通过如下两个方法完成的： 
（1）
Java代码 ![复制代码](http://bolan392.iteye.com/images/icon_copy.gif)![收藏代码](http://bolan392.iteye.com/images/icon_star.png)
- Object execute(HibernateCallback action)  
Java代码  ![收藏代码](http://bolan392.iteye.com/images/icon_star.png)
- Object execute(HibernateCallback action)  
（2）
Java代码 ![复制代码](http://bolan392.iteye.com/images/icon_copy.gif)![收藏代码](http://bolan392.iteye.com/images/icon_star.png)
- List executeFind(HibernateCallback action)  
Java代码  ![收藏代码](http://bolan392.iteye.com/images/icon_star.png)
- List executeFind(HibernateCallback action)  
HibernateCallback实例可在任何有效的Hibernate数据访问中使用。程序开发者通过HibernateCallBack，可以完全使用Hibernate灵活的方式来访问数据库，解决Spring封装Hibernate后灵活性不足的缺陷。 
HibernateCallback是个接口，该接口包含一个方法doInHibernate(org.hibernate.Session session)，该方法只有一个参数Session。 
注意： 
在doInHibernate方法内可访问Session，该Session对象是绑定到该线程的Session实例。在该方法内的持久层操作，与不使用Spring时的持久化操作完全相同。这保证了对于复杂的持久化层访问，依然可以使用Hibernate的访问方式。 
下面是有一个HibernateDaoSupport扩展类（增加了三个分页查询的方法）： 
Java代码 ![复制代码](http://bolan392.iteye.com/images/icon_copy.gif)![收藏代码](http://bolan392.iteye.com/images/icon_star.png)
- //============================================== 
- publicclass ExtendHibernateDaoSupport extends HibernateDaoSupport{  
   
- 
- /**       
-         使用hql进行分页查询       
-         @param hql 需要查询的hql语句       
-         @param offset 第一条记录索引       
-         @param pageSize 当前需要显示的记录数       
-         @return 当前页的所有记录   */
- public List findByPage(final String hql, finalint offset, finalint pageSize){
   
- //通过一个HibernateCallback对象来执行查询         
-                 List list = getHibernateTemplate().executeFind(   
- new HibernateCallback(){//实现HibernateCallback接口必须实现的方法 
- public Object doInHibernate(Session session)throws HibernateException, SQLException{//执行Hibernate分页查询 
-                                 List result = session.createQuery(hql)   
-                                 .setFirstResult(offset)   
-                                 .setMaxResults(pageSize)   
-                                 .list();   
- return result;   
-                                 }   
-                             });   
- return list;          
-         }          
- /**       
-         使用hql进行分页查询       
-         @param hql 需要查询的hql语句       
-         @param value 如果hql有一个参数需要传入，value就是传入hql语句的参数       
-         @param offset 第一条记录索引       
-         @param pageSize 当前需要显示的记录数       
-         @return 当前页的所有记录       
-         */
- 
- public List findByPage(final String hql, final Object value,finalint offset, finalint pageSize){
   
- //通过一个HibernateCallback对象来执行查询        
-                 List list = getHibernateTemplate().executeFind(new HibernateCallback(){//实现HibernateCallback接口必须实现的方法  
- public Object doInHibernate(Session session)throws HibernateException, SQLException{//执行Hibernate分页查询  
-                         List result = session.createQuery(hql)//为hql语句传入参数  
-                         .setParameter(0,value)   
-                         .setFirstResult(offset)   
-                         .setMaxResults(pageSize)   
-                         .list();   
- return result;   
-                         }   
-                     });   
- return list;          
-         }         
- /**  
- 
-         使用hql进行分页查询      
-         @param hql 需要查询的hql语句       
-         @param values 如果hql有多个参数需要传入，values就是传入hql语句的参数数组       
-         @param offset 第一条记录索引       
-         @param pageSize 当前需要显示的记录数       
-         @return 当前页的所有记录       
-         */
- 
- public List findByPage(final String hql, final Object[] values,finalint offset, finalint pageSize){
   
- //通过一个HibernateCallback对象来执行查询        
-                 List list = getHibernateTemplate().executeFind(new HibernateCallback(){           
- //实现HibernateCallback接口必须实现的方法 
- public Object doInHibernate(Session session)throws HibernateException, SQLException{       
   
- //执行Hibernate分页查询 
-                         Query query = session.createQuery(hql);          
- //为hql语句传入参数  
- for(int i=0; i<values.length; i++){
   
-                             query.setParamter(i,values[i]);   
-                             }   
-                         List result = query.setFirstResult(offset)   
-                         .setMaxResults(pageSize)   
-                         .list();   
- return result;   
-                         }   
-                     });   
- return list;         
-         }     
Java代码  ![收藏代码](http://bolan392.iteye.com/images/icon_star.png)
- //==============================================
- publicclass ExtendHibernateDaoSupport extends HibernateDaoSupport{    
- 
- /**      
-         使用hql进行分页查询      
-         @param hql 需要查询的hql语句      
-         @param offset 第一条记录索引      
-         @param pageSize 当前需要显示的记录数      
-         @return 当前页的所有记录   */
- public List findByPage(final String hql, finalint offset, finalint pageSize){  
- //通过一个HibernateCallback对象来执行查询        
-                 List list = getHibernateTemplate().executeFind(  
- new HibernateCallback(){//实现HibernateCallback接口必须实现的方法
- public Object doInHibernate(Session session)throws HibernateException, SQLException{//执行Hibernate分页查询
-                                 List result = session.createQuery(hql)  
-                                 .setFirstResult(offset)  
-                                 .setMaxResults(pageSize)  
-                                 .list();  
- return result;  
-                                 }  
-                             });  
- return list;         
-         }         
- /**      
-         使用hql进行分页查询      
-         @param hql 需要查询的hql语句      
-         @param value 如果hql有一个参数需要传入，value就是传入hql语句的参数      
-         @param offset 第一条记录索引      
-         @param pageSize 当前需要显示的记录数      
-         @return 当前页的所有记录      
-         */
- 
- public List findByPage(final String hql, final Object value,finalint offset, finalint pageSize){  
- //通过一个HibernateCallback对象来执行查询       
-                 List list = getHibernateTemplate().executeFind(new HibernateCallback(){//实现HibernateCallback接口必须实现的方法 
- public Object doInHibernate(Session session)throws HibernateException, SQLException{//执行Hibernate分页查询 
-                         List result = session.createQuery(hql)//为hql语句传入参数 
-                         .setParameter(0,value)  
-                         .setFirstResult(offset)  
-                         .setMaxResults(pageSize)  
-                         .list();  
- return result;  
-                         }  
-                     });  
- return list;         
-         }        
- /** 
- 
-         使用hql进行分页查询     
-         @param hql 需要查询的hql语句      
-         @param values 如果hql有多个参数需要传入，values就是传入hql语句的参数数组      
-         @param offset 第一条记录索引      
-         @param pageSize 当前需要显示的记录数      
-         @return 当前页的所有记录      
-         */
- 
- public List findByPage(final String hql, final Object[] values,finalint offset, finalint pageSize){  
- //通过一个HibernateCallback对象来执行查询       
-                 List list = getHibernateTemplate().executeFind(new HibernateCallback(){          
- //实现HibernateCallback接口必须实现的方法
- public Object doInHibernate(Session session)throws HibernateException, SQLException{         
- //执行Hibernate分页查询
-                         Query query = session.createQuery(hql);         
- //为hql语句传入参数 
- for(int i=0; i<values.length; i++){  
-                             query.setParamter(i,values[i]);  
-                             }  
-                         List result = query.setFirstResult(offset)  
-                         .setMaxResults(pageSize)  
-                         .list();  
- return result;  
-                         }  
-                     });  
- return list;        
-         }     
注意：Spring提供的XxxTemplate和XxxCallBack互为补充，XxxTemplate对通用操作进行封装，而XxxCallBack解决了封装后灵活性不足的缺陷。
