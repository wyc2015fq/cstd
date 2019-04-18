# 一对多关联的CRUD__@ManyToOne(cascade=(CascadeType.ALL)) - z69183787的专栏 - CSDN博客
2014年03月27日 22:21:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3254
## 一：Group和Users两个类
假定一个组里有n多用户，但是一个用户只对应一个用户组。
1.所以Group对于Users是“一对多”的关联关系@OneToMany
Users对于Group是“多对一”@ManyToOne
2.CRUD时候，希望是能从具体用户Users查到其对应的Group，反过来也能通过Group查到具体Users，所以是双向关联
（所以要用mappedBy去除冗余信息）
**[java]**[view
 plain](http://blog.csdn.net/null____/article/details/8159497#)[copy](http://blog.csdn.net/null____/article/details/8159497#)
- @Entity
- @Table(name="t_Group")//指定一个表名
- publicclass Group   
- {  
- privateint id;  
- private String name;  
- private Set<Users> users = new HashSet<Users>();  
- 
- @Id
- @GeneratedValue//主键用自增序列
- publicint getId() {  
- return id;  
-     }  
- @OneToMany(mappedBy="group",cascade=(CascadeType.ALL))//以“多”一方为主导管理，级联用ALL
- public Set<Users> getUsers() {  
- return users;  
-     }  
**[java]**[view
 plain](http://blog.csdn.net/null____/article/details/8159497#)[copy](http://blog.csdn.net/null____/article/details/8159497#)
- @Entity
- @Table(name="t_Users")  
- publicclass Users   
- {  
- privateint id;  
- private String name;  
- private Group group;  
- 
- @Id
- @GeneratedValue
- publicint getId() {  
- return id;  
-     }  
- @ManyToOne(fetch=FetchType.LAZY,cascade=(CascadeType.ALL))//解决1+N,级联用ALL
- @JoinColumn(name="groupId")//指定外键名称，不指定的默认值是group_Id
- public Group getGroup() {  
- return group;  
-     }  

## 二：C增
cascade：级联，只影响cud，不影响r
（all全都级联，persist存储时级联，remove删除时级联）
如果没有设置cascade，默认需要save（Group)和save(users)，两个都要存，设置级联之后，只存一个就行了
级联依赖于这句：@ManyToOne(cascade=(CascadeType.ALL))//需要依赖于其他的东西时候
设置好正反向之后，多个有级联关系的对象就一起被保存了
**[java]**[view
 plain](http://blog.csdn.net/null____/article/details/8159497#)[copy](http://blog.csdn.net/null____/article/details/8159497#)
- @Test
- publicvoid testC() {     
-     Session session = HibernateUtil.getSessionFactory().getCurrentSession();  
-        session.beginTransaction();  
- 
-        Users u1 = new Users();  
-        Users u2 = new Users();  
-        u1.setName("u1");  
-        u2.setName("u2");  
- //u1和u2的id自增
- 
-        Group g = new Group();  
-        g.setName("g1");  
- //g的id自增
- 
-        g.getUsers().add(u1);//正向
-        g.getUsers().add(u2);  
- 
-        u1.setGroup(g);//反向
-        u2.setGroup(g);//不然u1和u2中的group信息为空
- 
-        session.save(g);//因为设置级联，所以存储g时候也把u1和u2存上了。
- //不设置级联的话，还要存储u1和u2
- 
-        session.getTransaction().commit();  
-        HibernateUtil.getSessionFactory().close();           
- }  

## 三：R查
默认会这样处理（平时管用的思路也是这样）：
1.取“多”的时候，把“一”取出来
2.取“一”时，不取“多”的，用到时候再去取（看user信息时候一般看组名，看group时候user信息太多不必看）
fetch管读取，cascade管增删改
@OneToMany(mappedBy="group",cascade=(CascadeType.ALL),fetch=FetchType.EAGER)
@OneToMany默认的是LAZY，@ManyToOne默认是EAGER
User u = (User)s.get(User.class,1);
//取user时候，把group也取出来，
**[java]**[view
 plain](http://blog.csdn.net/null____/article/details/8159497#)[copy](http://blog.csdn.net/null____/article/details/8159497#)
- Users u = (Users)session.get(Users.class,11);//取id为11号的u
- //hibernate产生的语句里也把group拿出来了：group1_.id as id0_0_,和group1_.name as name0_0_ 
**[sql]**[view
 plain](http://blog.csdn.net/null____/article/details/8159497#)[copy](http://blog.csdn.net/null____/article/details/8159497#)
- Hibernate:   
- select
-         users0_.id as id1_1_,  
-         users0_.groupId as groupId1_1_,  
-         users0_.nameas name1_1_,  
-         group1_.id as id0_0_,  
-         group1_.nameas name0_0_   
- from
-         t_Users users0_   
- leftouterjoin
-         t_Group group1_   
- on users0_.groupId=group1_.id   
- where
-         users0_.id=?  
只取出Group的话，不会去查询里边的user
**[java]**[view
 plain](http://blog.csdn.net/null____/article/details/8159497#)[copy](http://blog.csdn.net/null____/article/details/8159497#)
- //只取出Group的话，不会去查询里边的user
- Group group = (Group)session.get(Group.class,11);  
**[sql]**[view
 plain](http://blog.csdn.net/null____/article/details/8159497#)[copy](http://blog.csdn.net/null____/article/details/8159497#)
- Hibernate:   
- select
-         group0_.id as id0_0_,  
-         group0_.nameas name0_0_   
- from
-         t_Group group0_   
- where
-         group0_.id=?  

## 四：U更新
注意：fetch影响两者读取顺序（两边都设成EAGER要多取出一次），反正都要取，具体再试
@OneToMany,@ManyToOne都写cascade=(CascadeType.ALL)
update时候自动关联更新
**[java]**[view
 plain](http://blog.csdn.net/null____/article/details/8159497#)[copy](http://blog.csdn.net/null____/article/details/8159497#)
- //因为cascade=(CascadeType.ALL)，所以自动关联更新
- Users u = (Users)session.load(Users.class,11);//取id为11号的u
- u.setName("u250");  
- u.getGroup().setName("gp01");  

## 五：D删
删多：实测只删掉目的项目，不关联其他
先load（就是select）一下，确认有之后，再删
没有遇到：不能直接s.delete(u)，因为u和group有级联，group和所有users都有级联，一下就够给删掉了的情况
不过严谨起见，还是设置一个u.setGroup(null);比较好
**[java]**[view
 plain](http://blog.csdn.net/null____/article/details/8159497#)[copy](http://blog.csdn.net/null____/article/details/8159497#)
- Users u1 = new Users();  
- u1.setId(18);  
- u1.setGroup(null);//严谨起见，应该先让俩表脱离关联
- session.delete(u1);  
HQL对应的语句（作用是一样的）：s.createQuery("delete from User u where u.id = 1").executeUpdate();//User是类名
删一：如果有子项目，报错不让删除
Group g = (Group)s.load(Group.class,1);
s.delete(g);
**[sql]**[view
 plain](http://blog.csdn.net/null____/article/details/8159497#)[copy](http://blog.csdn.net/null____/article/details/8159497#)
- Hibernate:   
- delete
- from
-         t_Group   
- where
-         id=?  
- 22:56:20,691  WARN SqlExceptionHelper:143 - SQL Error: 2292, SQLState: 23000  
- 22:56:20,692 ERROR SqlExceptionHelper:144 - ORA-02292: 违反完整约束条件 (SCOTT.FK9F52DA5D4E678049) - 已找到子记录  
- 
