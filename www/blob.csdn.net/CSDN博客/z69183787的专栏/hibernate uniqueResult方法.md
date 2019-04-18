# hibernate uniqueResult方法 - z69183787的专栏 - CSDN博客
2014年02月11日 16:04:07[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1203
**如果查询返回多个值用list()方法**
**[java]**[view
 plain](http://blog.csdn.net/itmyhome1990/article/details/7104080#)[copy](http://blog.csdn.net/itmyhome1990/article/details/7104080#)
- publicvoid testQuery(){  
-         Configuration config = new Configuration().configure();  
-         SessionFactory factory = config.buildSessionFactory(); //创建SessionFactory
-         Session session = factory.openSession();        //创建Session
-         session.beginTransaction();                     //开始事务
- 
-         Query query = session.createQuery("from Student");  
-         List list = query.list();  // 用list方法返回多个值
- //List list = session.createQuery("from Student").list();
- for(int i=0;i<list.size();i++){  
-             Student student = (Student)list.get(i);  
-             System.out.print(student.getId()+" ");  
-             System.out.print(student.getName()+" ");  
-             System.out.print(student.getSex()+" ");  
-             System.out.print(student.getBirthday());  
-             System.out.println();  
-         }         
- 
-         session.getTransaction().commit();         //提交事务 
-         session.close();                           //关闭Session
-     }  
当确定返回的实例只有一个或者null时 用uniqueResult()方法
**[java]**[view
 plain](http://blog.csdn.net/itmyhome1990/article/details/7104080#)[copy](http://blog.csdn.net/itmyhome1990/article/details/7104080#)
- publicvoid testGet(){  
-         Configuration config = new Configuration().configure();  
-         SessionFactory factory = config.buildSessionFactory();  
-         Session session = factory.openSession();  
-         session.beginTransaction();  
- 
-         Query query = session.createQuery("from Student s where s.id=?");  
-         query.setString(0, "2");  
-         Student student = (Student)query.uniqueResult();  //当确定返回的实例只有一个或者null时 用uniqueResult()方法
- //Student student = (Student)session.createQuery("from Student s where s.id=?").setString(0,"5").uniqueResult();
- 
-         System.out.println(student.getName());  
-         System.out.println(student.getBirthday());  
- 
- 
-         session.getTransaction().commit();  
-         session.close();  
-     }  
- 
