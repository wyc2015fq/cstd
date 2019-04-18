# hibernate之级联cascade和关系维持inverse - z69183787的专栏 - CSDN博客
2014年02月27日 10:32:59[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1425
hibernate的关联关系，重点在理解级联cascade和inverse
1、cascade一般用在级联保存，级联更新，级联删除上
   1.1cascade注解有两种，一种是基于hibernate注解
org.hibernate.annotations.Cascade
org.hibernate.annotations.CascadeType
支持一下级联
ALL, PERSIST//级联持久化，调用session.persist()时会触发级联事件 MERGE//级联保存或者更新，jpa规范 hibernate为了支持jsr220 后面添加的，调用session.merge()时触发级联 REMOVE,//级联删除，jpa规范同上，调用session.delete()时触发
 REFRESH, DELETE,//级联删除，session.delete()触发 SAVE_UPDATE,//级联保存或者更新session.save(),session.update(),session.saveOrUpdate(); REPLICATE,
    配置示例
Java代码 ![复制代码](http://418684644-qq-com.iteye.com/images/icon_copy.gif)![收藏代码](http://418684644-qq-com.iteye.com/images/icon_star.png)![](http://418684644-qq-com.iteye.com/images/spinner.gif)
- @Cascade(value={org.hibernate.annotations.CascadeType.ALL})
   
- private StudentInfo studentInfo ;  
**[java]**[view
 plain](http://blog.csdn.net/m13666368773/article/details/7410893#)[copy](http://blog.csdn.net/m13666368773/article/details/7410893#)[print](http://blog.csdn.net/m13666368773/article/details/7410893#)[?](http://blog.csdn.net/m13666368773/article/details/7410893#)
- @Cascade(value={org.hibernate.annotations.CascadeType.ALL})  
- private StudentInfo studentInfo ;  
   1.2第二种注解是基于jpa规范，也就是apache jsr220规范，也是ejb3的持久层规范
javax.persistence.CascadeType
ALL,
PERSIST,调用session.persist()时触发 MERGE，调用session.merge()触发REMOVE,调用session.delete()触发REFRESH,  DETACH
    配置示例
Java代码 ![复制代码](http://418684644-qq-com.iteye.com/images/icon_copy.gif)![收藏代码](http://418684644-qq-com.iteye.com/images/icon_star.png)![](http://418684644-qq-com.iteye.com/images/spinner.gif)
- @ManyToOne(cascade={CascadeType.MERGE})
   
- @JoinColumn(name = "teacher_id")
   
- private Teacher teacher;  
**[java]**[view
 plain](http://blog.csdn.net/m13666368773/article/details/7410893#)[copy](http://blog.csdn.net/m13666368773/article/details/7410893#)[print](http://blog.csdn.net/m13666368773/article/details/7410893#)[?](http://blog.csdn.net/m13666368773/article/details/7410893#)
- @ManyToOne(cascade={CascadeType.MERGE})  
- @JoinColumn(name = "teacher_id")  
- private Teacher teacher;  
  1.3级联一般用在OneToOne和OneToMany上，这也是hibernate官方的推荐，有时候我们在开发中也用在            ManyToOne,ManyToMany上，只是想在测试上少写点代码而已，在持久化一个实体的时候级联持久其他关联实体，
  如下：teacher和student是ManyToMany,加上了cascade注解，便于测试
Java代码 ![复制代码](http://418684644-qq-com.iteye.com/images/icon_copy.gif)![收藏代码](http://418684644-qq-com.iteye.com/images/icon_star.png)![](http://418684644-qq-com.iteye.com/images/spinner.gif)
- @Test
- publicvoid addStudentToTeacher(){
   
-         Student student1 = new Student("张三",20,20072733L);
   
-         Student student2 = new Student("李四",20,20072734L);
   
-         Student student3 = new Student("王五",20,20072735L);
   
- 
-         Teacher teacher = new Teacher("张老师");
   
-         teacher.getStudents().add(student3);   
-         teacher.getStudents().add(student2);   
-         teacher.getStudents().add(student1);   
- this.teacherDao.save(teacher);
   
-     }  
**[java]**[view
 plain](http://blog.csdn.net/m13666368773/article/details/7410893#)[copy](http://blog.csdn.net/m13666368773/article/details/7410893#)[print](http://blog.csdn.net/m13666368773/article/details/7410893#)[?](http://blog.csdn.net/m13666368773/article/details/7410893#)
- @Test
- publicvoid addStudentToTeacher(){  
-         Student student1 = new Student("张三",20,20072733L);  
-         Student student2 = new Student("李四",20,20072734L);  
-         Student student3 = new Student("王五",20,20072735L);  
- 
-         Teacher teacher = new Teacher("张老师");  
-         teacher.getStudents().add(student3);  
-         teacher.getStudents().add(student2);  
-         teacher.getStudents().add(student1);  
- this.teacherDao.save(teacher);  
-     }  
2、inverse，英文意思是反向，反转。在这里可以理解为控制反转，也就是说实体间的关系由谁控制，所以inverse用在实体关联上。如OneToOne，OneToMany，ManyToMany 
     在OneToMany中，如果不指定inverse，那么hibernate会去找默认的表来维持关系。
     例如用老师和课程两个实体来说明，假设teacher和course是OneToMany的关系，
     配置如下：
//通过外键teacher_id关联teacher，inverse通过mappedBy来设置
Java代码 ![复制代码](http://418684644-qq-com.iteye.com/images/icon_copy.gif)![收藏代码](http://418684644-qq-com.iteye.com/images/icon_star.png)![](http://418684644-qq-com.iteye.com/images/spinner.gif)
- @ManyToOne(cascade={CascadeType.MERGE})
   
- @JoinColumn(name = "teacher_id")
   
- private Teacher teacher;
   
- 
- @OneToMany(mappedBy="teacher",fetch=FetchType.LAZY,cascade={CascadeType.MERGE,CascadeType.REMOVE})
   
- private Set<Course> courses = new HashSet<Course>();  
**[java]**[view
 plain](http://blog.csdn.net/m13666368773/article/details/7410893#)[copy](http://blog.csdn.net/m13666368773/article/details/7410893#)[print](http://blog.csdn.net/m13666368773/article/details/7410893#)[?](http://blog.csdn.net/m13666368773/article/details/7410893#)
- @ManyToOne(cascade={CascadeType.MERGE})  
- @JoinColumn(name = "teacher_id")  
- private Teacher teacher;  
- 
- @OneToMany(mappedBy="teacher",fetch=FetchType.LAZY,cascade={CascadeType.MERGE,CascadeType.REMOVE})  
- private Set<Course> courses = new HashSet<Course>();  
   在这里指定teacher的属性courses的关系由关联实体Course的teacher属性维持，如果不配置，则会去找关联表teacher_course来维持关系。OneToOne和ManyToMany大同小异，这里不再累述！
