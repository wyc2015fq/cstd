# Hibernate和JPA的主键生成策略 - 零度的博客专栏 - CSDN博客
2016年04月29日 16:33:43[零度anngle](https://me.csdn.net/zmx729618)阅读数：593
主键是关系数据库中的一个基本概念，它用来保证记录的唯一性。简单来说，就是同一张数据库表中，不允许存在多条相同主键的记录。主键生成策略，就是当向数据库表中插入记录的时候，这个记录的主键该如何生成。绝大部分情况下，主键都是没有业务含义的，所以开发者不会、也不需要，显示地设置实体对象的主键值。但是对于数据库来说，主键是必须的，显然这个责任，落在了hibernate这个持久层框架上。对于初学hibernate的人来说，往往会被一堆的主键生成策略弄晕，本文介绍下一些容易混淆的概念，希望能帮助大家了解hibernate主键生成策略。
       Hibernate内置了很多主键生成策略，这些策略可以分为两类：一类是JPA标准的主键生成策略，一类是Hibernate框架特有的主键生成策略。JPA标准策略有4种：auto策略、table策略、sequence策略、identity策略；余下的都是hibernate自己的策略，包括我们常用的native、uuid、assigned、sequence等。
## 1.JPA和Hibernate的区别
       JPA全称Java Persistence API，是sun公司针对ORM技术提出的技术规范，用来将POJO按照标准的方式进行持久化，很类似于JDBC规范。Hibernate最早是以ORM框架形式出现的，用来解决JDBC存在的问题。随着JPA标准的发展和完善，hibernate到后来也开始支持JPA规范，并且能够完全兼容JPA规范。也就说，hibernate是JPA标准的一个实现，还在此基础上增加了一些自己特有的功能。这就是我们常说的：JPA是hibernate的一个子集，hibernate是JPA的超集。
## 2.JPA的4种策略介绍
- **AUTO策略**
 auto策略是JPA默认的策略，在hibernate的代码 *GenerationType.AUTO *进行定义。使用 AUTO 策略就是将主键生成的策略交给持久化引擎 (persistence   engine) 来决定，由它自己从 Table 策略，Sequence 策略和 Identity策略三种策略中选择最合适的。不同的持久化引擎
 、不同的数据库一般策略不同。比如oracle最常用的就 是sequence，[MySQL](http://lib.csdn.net/base/14)最常用的就是identify，因为oracle数据库支持sequence，而mysql数据库则 是只支持 auto_increment 。
- **Sequence策略**
对应Hibernate代码 *GenerationType.SEQUENCE *。一些数据库，如oracle就会内置"序列生成器"。为了使用序列，我们需要使用JPA的sequence策略。
- **Identity策略**
对应Hibernate代码 *GenerationType.IDENTITY* 。 这个很适合像mysql这样的数据库，提供了对自增主键的支持。
- **Table策略**
对应Hibernate代码 ***GenerationType.TABLE***。 使用一张特殊的数据库表，保存插入记录的时，需要的主键值。
## 3.注解风格使用JPA主键生成策略
   配置实体主键生成策略的时候，需要用到@GeneratedValue主键
- **使用AUTO策略**
对应的java代码如下：
publicclassTeacher{
privateint id;
private String title;
@Id
@GeneratedValue(strategy = GenerationType.AUTO)
publicint getId(){
return id;
}
}
我使用的是mysql对应的数据库表如下：
![](http://img0.tuicool.com/eym2ua.jpg)
注意：
1、如果是mysql数据库，一定要将主键列设置成自增长的，否则使用AUTO策略的时候，会报错：
org.hibernate.exception.GenericJDBCException: Field 'id' doesn't have a default value 
2、如果是oracle数据库，那么会使用hibernate_sequence，这个名称是固定的，不能更改。
- **使用sequence策略**
**publicclassTeacher{**
**privateint id;**
**private String title;**
**@Id**
**@GeneratedValue(strategy = GenerationType.SEQUENCE,generator="mySeqGenerator")**
**@SequenceGenerator(name = "mySeqGenerator", sequenceName = "t_teacher_sequence", initialValue = 1000, allocationSize = 50)**
**publicint getId(){**
**return id;**
**}**
** }**
  这里需要配合使用@SequenceGenerator，用来指定序列的相关信息。
name：序列生成器的名称，会在@GeneratedValue中进行引用
sequenceName：oracle数据库中的序列生成器名称
initialValue：主键的初始值
allocationSize：主键每次增长值的大小 
注意：如果底层数据库不执行序列，会报错：
org.hibernate.MappingException: org.hibernate.dialect.MySQLDialect does not support sequences
- 使用Identity策略
     publicclassTeacher{
private
int id;
private String title;
@Id
@GeneratedValue(strategy = GenerationType.IDENTITY)
public
int getId(){
return id;
                }
}
     使用比较简单，这里不需要做详细介绍了
- **使用Table策略**
publicclassTeacher{
private
int id;
private String title;
@Id
@GeneratedValue(strategy = GenerationType.TABLE,generator="myTableGenerator")
@TableGenerator(name = 
"myTableGenerator", table = "hibernateNeedTable", pkColumnName = "pk_key", valueColumnName =
"pk_value", pkColumnValue = 
"teacherId", initialValue =
100, allocationSize = 
1000)
public
int getId(){
return id;
}
    }
   不知道怎么用合适的语言来描述这些属性的含义。直接上图和执行的sql语句，读者只需要简单理解下，就能明白这些属性的含义了。
1、我们数据库中多了一张表
![](http://img1.tuicool.com/uymmAj.jpg)
2、实际发出的sql语句如下：
```
<span style="font-family:Arial;font-size:12px;">Hibernate: 
  select
    pk_value 
  from
    hibernateNeedTable 
  where
    pk_key = 'teacherId' for update
      
Hibernate: 
  update
    hibernateNeedTable 
  set
    pk_value = ? 
  where
    pk_value = ? 
    and pk_key = 'teacherId'
Hibernate: 
  insert 
  into
    Teacher
    (title, id) 
  values
    (?, ?)</span>
```
    看到这里，大家应该明白了这些属性的用法了。值得一提的是，这个表可以给无数的表作为主键表，只是添加一条记录而以(需要保证table、pkColumnName、valueColumnName三个属性值唯一就可以了。
