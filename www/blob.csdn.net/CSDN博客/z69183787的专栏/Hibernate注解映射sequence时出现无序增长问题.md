# Hibernate注解映射sequence时出现无序增长问题 - z69183787的专栏 - CSDN博客
2013年12月17日 11:22:10[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1323
    通过Hibernate注解的方式映射oracel数据库的sequence主键生成器时，出现增加数据时sequence无序增长的问题，配置如下： 
Java代码  ![收藏代码](http://zhouyangchenrui.iteye.com/images/icon_star.png)
- @SequenceGenerator(name = "SEQ_DM_SERVICE_MODEL", sequenceName = SEQ_DM_SERVICE_MODEL")  
- @Id
- @GeneratedValue(strategy = SEQUENCE, generator = "SEQ_DM_SERVICE_MODEL")  
- @Column(name = "SERVICE_MODEL_ID", unique = true, nullable = false, precision = 10, scale = 0)  
- public Long getId() {  
- returnthis.id;  
- }  
此时主键id的增长是按照hibernate自动处理的方式，而并非数据库中定义的sequence来处理。必须加allocationSize=1,initialValue=1这两项配置才可以解决上述问题。如下： 
Java代码  ![收藏代码](http://zhouyangchenrui.iteye.com/images/icon_star.png)
- @SequenceGenerator(name = "generator",allocationSize=1,initialValue=1, sequenceName = "SEQ_DM_SERVICE_MODEL")
  
