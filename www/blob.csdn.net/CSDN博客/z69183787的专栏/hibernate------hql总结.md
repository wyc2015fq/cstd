# hibernate------hql总结 - z69183787的专栏 - CSDN博客
2014年01月15日 18:51:19[OkidoGreen](https://me.csdn.net/z69183787)阅读数：958
1. 查询整个映射对象所有字段 
Java代码  ![收藏代码](http://kuangbaoxu.iteye.com/images/icon_star.png)
- //直接from查询出来的是一个映射对象，即：查询整个映射对象所有字段   
-         String hql = "from Users";     
-         Query query = session.createQuery(hql);     
- 
-         List<Users> users = query.list();     
- for(Users user : users){     
-             System.out.println(user.getName() + " : " + user.getPasswd() + " : " + user.getId());     
-         }     
- 
-  输出结果为：     
- name1 : password1 : 1
- name2 : password2 : 2
- name3 : password3 : 3
2.查询单个字段  
Java代码  ![收藏代码](http://kuangbaoxu.iteye.com/images/icon_star.png)
- //查询单个字段   
-         String hql = " select name from Users";     
-         Query query = session.createQuery(hql);     
- 
-         List<String> list = query.list();     
- for(String str : list){     
-             System.out.println(str);     
-         }     
- 输出结果为：     
- name1     
- name2     
- name3    
3.查询其中几个字段 
Java代码  ![收藏代码](http://kuangbaoxu.iteye.com/images/icon_star.png)
- //查询其中几个字段   
-         String hql = " select name,passwd from Users";     
-         Query query = session.createQuery(hql);     
- //默认查询出来的list里存放的是一个Object数组   
-         List<Object[]> list = query.list();     
- for(Object[] object : list){     
-             String name = (String)object[0];     
-             String passwd = (String)object[1];     
- 
-             System.out.println(name + " : " + passwd);     
-         }     
- 输出结果为：     
- name1 : password1     
- name2 : password2     
- name3 : password3    
4.修改默认查询结果(query.list())不以Object[]数组形式返回，以List形式返回 
引用
//查询其中几个字段,添加new list(),注意list里的l是小写的。也不需要导入包，这样通过query.list()出来的list里存放的不再是默认的Object数组了，而是List集合了   
        String hql = " select new list(name,passwd) from Users";   
        Query query = session.createQuery(hql);   
        //默认查询出来的list里存放的是一个Object数组，但是在这里list里存放的不再是默认的Object数组了，而是List集合了   
        List<List> list = query.list();   
        for(List user : list){   
            String name = (String)user.get(0);   
            String passwd = (String)user.get(1);   
            System.out.println(name + " : " + passwd);   
        }   
        /**  
        输出结果为：  
         name1 : password1  
        name2 : password2  
        name3 : password3  
         */ 
5.修改默认查询结果(query.list())不以Object[]数组形式返回，以Map形式返回 
Java代码  ![收藏代码](http://kuangbaoxu.iteye.com/images/icon_star.png)
- //查询其中几个字段,添加new map(),注意map里的m是小写的。也不需要导入包，这样通过query.list()出来的list里存放的不再是默认的Object数组了，而是map集合了   
-         String hql = " select new map(name,passwd) from Users";     
-         Query query = session.createQuery(hql);     
- //默认查询出来的list里存放的是一个Object数组，但是在这里list里存放的不再是默认的Object数组了，而是Map集合了   
-         List<Map> list = query.list();     
- for(Map user : list){     
- //一条记录里所有的字段值都是map里的一个元素,key是字符串0,1,2,3....，value是字段值   
- //如果将hql改为：String hql = " select new map(name as username,passwd as password) from Users";,那么key将不是字符串0,1,2...了，而是"username","password"了   
-             String name = (String)user.get("0");//get("0");是get(key),注意:0,1,2...是字符串，而不是整形   
-             String passwd = (String)user.get("1");     
- 
-             System.out.println(name + " : " + passwd);     
-         }     
- /**  
-         输出结果为：  
-          name1 : password1  
-         name2 : password2  
-         name3 : password3  
-          */
6.修改默认查询结果(query.list())不以Object[]数组形式返回，以Set形式返回,但是因为Set里是不允许有重复的元素，所以:username和password的值不能相同。只需将hql改为：String hql = " select new set(name,passwd) from Users"; 
7.修改默认查询结果(query.list())不以Object[]数组形式返回，以自定义类型返回 
自定义类： 
Java代码  ![收藏代码](http://kuangbaoxu.iteye.com/images/icon_star.png)
- package com.domain;     
- 
- publicclass MyUser {     
- 
- private String username;     
- private String password;     
- //因为：String hql = " select new  com.domain.MyUser(name,passwd) from Users";所以必须要有接受2个参数的构造函数   
- public MyUser(String username,String password){     
- this.username = username;     
- this.password = password;     
-     }     
- 
- public String getUsername() {     
- return username;     
-     }     
- publicvoid setUsername(String username) {     
- this.username = username;     
-     }     
- public String getPassword() {     
- return password;     
-     }     
- publicvoid setPassword(String password) {     
- this.password = password;     
-     }     
- 
- 
- }    
Java代码  ![收藏代码](http://kuangbaoxu.iteye.com/images/icon_star.png)
- //通过query.list()出来的list里存放的不再是默认的Object数组了,而是自定义的类MyUser,必须加包名，String hql = "from Users";中的Users类也是必须加包名的，但是因为再Users.hbm.xml里<hibernate-mapping auto-import="true"> auto-import默认值为true（所以auto-import属性也可以不写）,自动导入了   
-         String hql = " select new  com.domain.MyUser(name,passwd) from Users";     
-         Query query = session.createQuery(hql);     
- //默认查询出来的list里存放的是一个Object数组，但是在这里list里存放的不再是默认的Object数组了，而是MyUser对象了   
-         List<MyUser> myUsers = query.list();     
- for(MyUser myUser : myUsers){     
-             String name = myUser.getUsername();     
-             String passwd = myUser.getPassword();     
-             System.out.println(name + " : " + passwd);     
-         }     
- /**  
-         输出结果为：  
-         name1 : password1  
-         name2 : password2  
-         name3 : password3  
-          */
8：条件查询 
Java代码  ![收藏代码](http://kuangbaoxu.iteye.com/images/icon_star.png)
- //条件查询，参数索引值从0开始，索引位置。通过setString,setParameter设置参数   
-         String hql = "from Users where name=? and passwd=?";     
-         Query query = session.createQuery(hql);     
- //第1种方式   
- //      query.setString(0, "name1");   
- //      query.setString(1, "password1");   
- //第2种方式   
-         query.setParameter(0, "name1",Hibernate.STRING);     
-         query.setParameter(1, "password1",Hibernate.STRING);     
-         List<Users> list = query.list();     
- for(Users users : list){     
-             System.out.println(users.getId());     
-         }     
Java代码  ![收藏代码](http://kuangbaoxu.iteye.com/images/icon_star.png)
- //条件查询，自定义索引名(参数名):username,:password.通过setString,setParameter设置参数   
-         String hql = "from Users where name=:username and passwd=:password";     
-         Query query = session.createQuery(hql);     
- //第1种方式   
- //      query.setString("username", "name1");   
- //      query.setString("password", "password1");   
- //第2种方式,第3个参数确定类型   
-         query.setParameter("username", "name1",Hibernate.STRING);     
-         query.setParameter("password", "password1",Hibernate.STRING);     
-         List<Users> list = query.list();     
- for(Users users : list){     
-             System.out.println(users.getId());     
-         }    
Java代码  ![收藏代码](http://kuangbaoxu.iteye.com/images/icon_star.png)
- //条件查询,通过setProperties设置参数   
-         String hql = "from Users where name=:username and passwd=:password";     
-         Query query = session.createQuery(hql);     
- //MyUser类的2个属性必须和:username和:password对应   
-         MyUser myUser = new MyUser("name1","password1");     
-         query.setProperties(myUser);     
-         List<Users> list = query.list();     
- for(Users users : list){     
-             System.out.println(users.getId());     
-         }    
