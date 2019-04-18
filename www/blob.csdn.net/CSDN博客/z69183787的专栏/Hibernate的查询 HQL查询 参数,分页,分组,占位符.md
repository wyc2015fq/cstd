# Hibernate的查询 HQL查询 参数,分页,分组,占位符 - z69183787的专栏 - CSDN博客
2014年04月15日 12:47:15[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2860
增加参数为分两种，一中是占位符方式：
Java代码  ![收藏代码](http://cuisuqiang.iteye.com/images/icon_star.png)
- /**
-  * 占位符参数
-  */
- @SuppressWarnings("unchecked")  
- publicstaticvoid test4(){  
-     Session session = HibernateSessionFactory.currentSession();  
-     String hql = "from User u where u.id=?";  
-     Query query = session.createQuery(hql);  
-     query.setParameter(0, 2);  
-     List<User> user = query.list();  
-     System.out.println("------------SQL执行完毕---------------");  
- for (User us : user) {  
-         System.out.println(us.getName());  
-     }  
- }  
注意是从 0 开始，如果你的 id 是整型，那么必须设置一个整数，如果你设置参数是 "2" 是不行的！
也可以使用命名参数：
Java代码  ![收藏代码](http://cuisuqiang.iteye.com/images/icon_star.png)
- /**
-  * 命名参数
-  */
- @SuppressWarnings("unchecked")  
- publicstaticvoid test5(){  
-     Session session = HibernateSessionFactory.currentSession();  
-     String hql = "from User u where u.id=:id";  
-     Query query = session.createQuery(hql);  
-     query.setInteger("id", 2);  
-     List<User> user = query.list();  
-     System.out.println("------------SQL执行完毕---------------");  
- for (User us : user) {  
-         System.out.println(us.getName());  
-     }  
- }  
使用命名参数感觉上是比较直观的
分页的话比较简单，设置两个参数即可：
Java代码  ![收藏代码](http://cuisuqiang.iteye.com/images/icon_star.png)
- /**
-  * 分页
-  */
- @SuppressWarnings("unchecked")  
- publicstaticvoid test6(){  
-     Session session = HibernateSessionFactory.currentSession();  
-     String hql = "from User u";  
-     Query query = session.createQuery(hql);  
-     query.setFirstResult(0);  
-     query.setMaxResults(5);  
-     List<User> user = query.list();  
-     System.out.println("------------SQL执行完毕---------------");  
- for (User us : user) {  
-         System.out.println(us.getName());  
-     }  
- }  
至于分组和排序的话，和你写SQL是一样的，只是这里写的是实体的属性名字：
Java代码  ![收藏代码](http://cuisuqiang.iteye.com/images/icon_star.png)
- /**
-  * 分组排序
-  */
- @SuppressWarnings("unchecked")  
- publicstaticvoid test7(){  
-     Session session = HibernateSessionFactory.currentSession();  
-     String hql = "from User u group by name order by id desc";  
-     Query query = session.createQuery(hql);  
-     List<User> user = query.list();  
-     System.out.println("------------SQL执行完毕---------------");  
- for (User us : user) {  
-         System.out.println(us.getName());  
-     }  
- }  
