# hibernate 彻底理解get与load使用时的区别以及lazy属性的作用 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年05月17日 20:32:44[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：128


说明：本文部分图片来源于百度经验，但其对内容的理解存在问题，感兴趣的可以看看。

附上链接：[点击打开链接](https://jingyan.baidu.com/article/4853e1e5164a091909f726c1.html)

get()和load()：

1、调用 get方法的时候会直接发出sql语句，去数据库查询你传的参数即pojo实体对应的表。

![](https://img-blog.csdn.net/20180517201328774?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180517201355418?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

2、调用 load方法 的时候 并不会发出sql 语句 ， 而是 当你程序中用到这个pojo的时候，它才会发出sql语句去查询对应的表。

![](https://img-blog.csdn.net/20180517201943389?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180517202006346?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

lazy属性 ：

1、 这个属性 是针对于 一对多（多对一） 表关联的情况，一对一中如果在配置文件中 设置lazy="true" 配置文件直接报错。一         一对一的关联关系中，是一定会查询关联表的。

![](https://img-blog.csdn.net/20180517202102192?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180517202119930?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

2、lazy=“true” 代表当我们去查询一个表的时候，它发出的sql语句并没有去查询所关联的表。仅仅查询主表，即你调用             load()或get()方法所传入的pojo对应的表。 当你去调用从表在 主表中 对应的全局变量这个属性的时候，它才会发出sql语         句去查询这个从表。

部门表：

![](https://img-blog.csdn.net/20180517202215974?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

员工表：

![](https://img-blog.csdn.net/20180517202255114?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

部门（一） ---- 员工（多）

当 lazy = "ture" 时 ，测试 如下

![](https://img-blog.csdn.net/20180517202509565?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDI0NzI2Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

```
Hibernate: 
    select
        depvo0_.depid as depid1_1_0_,
        depvo0_.depname as depname2_1_0_,
        depvo0_.parentid as parentid3_1_0_ 
    from
        dep depvo0_ 
    where
        depvo0_.depid=?
部门名称 ： 智能制造部                     --------  请注意 这行输出语句以及 sql的位置 关系，输出的时候仅仅查询了主表
Hibernate:                               --------   下面的sql语句 是depVo.getEmployee() 产生的，也就是你用的时候它才会查询
    select
        employee0_.depid as depid7_2_0_,
        employee0_.empid as empid1_2_0_,
        employee0_.empid as empid1_2_1_,
        employee0_.empname as empname2_2_1_,
        employee0_.cardno as cardno3_2_1_,
        employee0_.sex as sex4_2_1_,
        employee0_.age as age5_2_1_,
        employee0_.nation as nation6_2_1_,
        employee0_.depid as depid7_2_1_,
        accountvo1_.empid as empid1_0_2_,
        accountvo1_.username as username2_0_2_,
        accountvo1_.password as password3_0_2_,
        accountvo1_.status as status4_0_2_,
        empvo2_.empid as empid1_2_3_,
        empvo2_.empname as empname2_2_3_,
        empvo2_.cardno as cardno3_2_3_,
        empvo2_.sex as sex4_2_3_,
        empvo2_.age as age5_2_3_,
        empvo2_.nation as nation6_2_3_,
        empvo2_.depid as depid7_2_3_ 
    from
        employee employee0_ 
    left outer join
        account accountvo1_ 
            on employee0_.empid=accountvo1_.empid 
    left outer join
        employee empvo2_ 
            on accountvo1_.empid=empvo2_.empid 
    where
        employee0_.depid=?
------------------
员工姓名 ： rtrt
------------------
员工姓名 ： 哈哈哈
------------------
员工姓名 ： eaewaa
------------------
员工姓名 ： eaew
------------------
员工姓名 ： ewqewq
------------------
员工姓名 ： dsadsa
------------------
员工姓名 ： eaew
```

当 lazy=“false” 时，测试如下    --  测试代码还一样，只贴出console 的结果

```
Hibernate: 
    select
        depvo0_.depid as depid1_1_0_,
        depvo0_.depname as depname2_1_0_,
        depvo0_.parentid as parentid3_1_0_ 
    from
        dep depvo0_ 
    where
        depvo0_.depid=?
Hibernate: 
    select
        employee0_.depid as depid7_2_0_,
        employee0_.empid as empid1_2_0_,
        employee0_.empid as empid1_2_1_,
        employee0_.empname as empname2_2_1_,
        employee0_.cardno as cardno3_2_1_,
        employee0_.sex as sex4_2_1_,
        employee0_.age as age5_2_1_,
        employee0_.nation as nation6_2_1_,
        employee0_.depid as depid7_2_1_,
        accountvo1_.empid as empid1_0_2_,
        accountvo1_.username as username2_0_2_,
        accountvo1_.password as password3_0_2_,
        accountvo1_.status as status4_0_2_,
        empvo2_.empid as empid1_2_3_,
        empvo2_.empname as empname2_2_3_,
        empvo2_.cardno as cardno3_2_3_,
        empvo2_.sex as sex4_2_3_,
        empvo2_.age as age5_2_3_,
        empvo2_.nation as nation6_2_3_,
        empvo2_.depid as depid7_2_3_ 
    from
        employee employee0_ 
    left outer join
        account accountvo1_ 
            on employee0_.empid=accountvo1_.empid 
    left outer join
        employee empvo2_ 
            on accountvo1_.empid=empvo2_.empid 
    where
        employee0_.depid=?
部门名称 ： 智能制造部                     -----  注意这行的位置，这里当调用get方法的时候，就发出了两条sql，分别查询了主表和从表
------------------
员工姓名 ： rtrt
------------------
员工姓名 ： 哈哈哈
------------------
员工姓名 ： eaewaa
------------------
员工姓名 ： eaew
------------------
员工姓名 ： ewqewq
------------------
员工姓名 ： dsadsa
------------------
员工姓名 ： eaew
```

总结：

get()和load()的区别是在于查询主表的时候延迟加载还是不延迟加载，get()直接发出sql、load()用的时候发出sql，这个不涉及从表的事 儿，也就是说如果如果你没设置lazy或lazy=“false”，无论是get()还是load()方法，当它发出sql语句的时候，你会发现都会去查询所关联的表。

而lazy这个属性，主要是针对查询从表的时候延迟加载还是不延迟加载。

lazy="true" 的时候，你查询主表，调用get()方法，你会发现它的sql，仅仅查询的主表，而当我们调用了主表pojo中对应的从表pojo的全局变量的时候，它又会发出一条sql，这时在查询从表。

lazy="false"的时候，你查询主表，调用get()方法，你会发现它直接发出两条sql，一个是查询的主表，一个是查询的从表。这里多提一嘴，lazy和fetch可以配合使用，如果设置了fetch=“join” ，它会将这两条sql，通过外链接合并成一条sql从而提高效率。



