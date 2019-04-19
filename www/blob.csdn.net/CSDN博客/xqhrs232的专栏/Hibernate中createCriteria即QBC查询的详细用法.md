# Hibernate中createCriteria即QBC查询的详细用法 - xqhrs232的专栏 - CSDN博客
2018年11月25日 22:15:04[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：37
个人分类：[JavaWeb技术](https://blog.csdn.net/xqhrs232/article/category/8231541)
原文地址::[https://blog.csdn.net/tjcyjd/article/details/6652111](https://blog.csdn.net/tjcyjd/article/details/6652111)
相关文章
1、[Hibernate createCriteria查询详解](https://www.cnblogs.com/whtydn/p/5001135.html)----[https://www.cnblogs.com/whtydn/p/5001135.html](https://www.cnblogs.com/whtydn/p/5001135.html)
2、Hibernate中Criteria的完整用法----[https://blog.csdn.net/saindy5828/article/details/16893257](https://blog.csdn.net/saindy5828/article/details/16893257)
3、[HIBERNATE之CRITERIA的完整用法](https://www.cnblogs.com/ywx-vashon/p/4857601.html)----[https://www.cnblogs.com/ywx-vashon/p/4857601.html](https://www.cnblogs.com/ywx-vashon/p/4857601.html)
4、[转]hibernate中getsession导致停止响应的问题----[https://blog.csdn.net/billfanggs/article/details/10901631](https://blog.csdn.net/billfanggs/article/details/10901631)
现在假设有一个Student类,内有id,name,age属性
String hql = "from Student s";
按照以前的做法,我们通常是
Query query = session.createQuery(hql);
或者要按照条件检索的话.
String hql = "from Student s where s.name like '王%'"
Query query = session.createQuery(hql);
不用HQL而使用QBC的话,那么代码为:
Criteria criteria =session.createCriteria(Student.class);
Criterion criterion = Expression.like("name","王%");
这样还看不出来.那我们把检索条件加上年龄为22.
HQL:
String hql = "from Student s where s.name like '王%' and s.age= 22 ";
Query query = session.createQuery(hql);
List list = query.list();
QBC:
Criteria criteria =session.createCriteria(Student.class);
Criterion criterion1 = Expression.like("name","王%");
Criterion criterion2 = Expression.eq("age",newInteger(22));
criteria.add(criterion1).add(criterion2);
List list = criteria.list();
看上去烦琐很多.但是做过项目的人都知道,当一个模块业务逻辑发生改变的时候,往往要重写sql,最烦也是最讨厌的就是拿着别人的hql或者sql,两眼瞪的溜园找到底要修改什么地方呢?
如果使用QBC大大的增加了代码的可读性,以及可维护性.
需要注意的是null值
比如我们要查找姓名为null的Student对象时应该这么写
Criteria criteria =session.createCriteria(Student.class);
Criterion criterion = Expression.isNull("name");
criteria.add(criterion).list();
以及使用between...and的时候
Criteria criteria =session.createCriteria(Student.class);
Criterion criterion1 = Expression.ge("age",new Integer(20));//下限
Criterion criterion2 = Expression.le("age",new Integer(25));//上限
//这里也可以把上述两个条件添加到第三个条件里
Criterion criterion3 =Expression.and(criterion1,criterion2);
criteria.add(criterion3).list();
相当于from Student s where s.age between 20 and 25
等同于from Student s where s.age >= 20 and s.age <=25
下面是就HQL和QBC常用的查询条件做的比较
表达式含义                   HQL                   QBC
大于等于                     >=                    Expression.ge()
大于                         >                     Expression.gt()
小于等于                     <=                    Expression.le()
小于                         <                     Expression.lt()
等于                         =                     Expression.eq()
不等于                       <>或者!=              Expression.ne() 
为空                         is null               Expression.isNull()
不为空                       is notnull            Expression.isNotNull()
在指定范围内                 betweenand            Expression.between()
不在指定范围                 not betweenand        Expression.not(Expression.between())
属于某个集合                 in                    Expression.in()
不属于某个集合               notin                 Expression.not(Expression.in())
与                          and                   Expression.and()
或                          or                    Expression.or()
非                          not                   Expression.not()
模糊查询                    like                  Expression.like
**1、创建一个Criteria实例 net.sf.hibernate.Criteria这个接口代表对一个特定的持久化类的查询。Session是用来制造Criteria实例的工厂。**
Criteria crit = sess.createCriteria(Cat.class);
crit.setMaxResults(50);
List cats = crit.list();
**2、缩小结果集范围 一个查询条件(Criterion)是net.sf.hibernate.expression.Criterion接口的一个实例。类net.sf.hibernate.expression.Expression定义了获得一些内置的Criterion类型。**
List cats = sess.createCriteria(Cat.class)
                .add( Expression.like("name", "Fritz%") )
                .add( Expression.between("weight", minWeight, maxWeight))
                .list(); 
表达式（Expressions）可以按照逻辑分组.
 List cats = sess.createCriteria(Cat.class)
                .add( Expression.like("name", "Fritz%") )
                .add( Expression.or( Expression.eq( "age", new Integer(0) ), Expression.isNull("age")))
                .list();
List cats = sess.createCriteria(Cat.class)
                .add( Expression.in( "name",new String[]{"Fritz","Izi","Pk"}))
                .add( Expression.disjunction()
                .add( Expression.isNull("age") )
                .add( Expression.eq("age", new Integer(0) ) )
                .add( Expression.eq("age", new Integer(1) ) )
                .add( Expression.eq("age", new Integer(2) ) ) ) )
                 .list(); 
有很多预制的条件类型（Expression的子类）。有一个特别有用，可以让你直接嵌入SQL。
List cats = sess.createCriteria(Cat.class) 
                .add( Expression.sql("lower($alias.name) like lower(?)", "Fritz%", Hibernate.STRING))
                .list();  
其中的{alias}是一个占位符，它将会被所查询实体的行别名所替代。(原文:The {alias} placeholder with be replaced by the row alias of the queried entity.)
**3、对结果排序 可以使用net.sf.hibernate.expression.Order对结果集排序.**
List cats = sess.createCriteria(Cat.class)
                .add( Expression.like("name", "F%")
                .addOrder( Order.asc("name"))
                .addOrder( Order.desc("age"))
                .setMaxResults(50)
                .list();
**4、关联（Associations） 你可以在关联之间使用createCriteria()，很容易地在存在关系的实体之间指定约束。**
List cats = sess.createCriteria(Cat.class)
                 .add( Expression.like("name", "F%")
                 .createCriteria("kittens")
                 .add( Expression.like("name","F%")
                 .list();
注意，第二个createCriteria()返回一个Criteria的新实例，指向kittens集合类的元素。 下面的替代形式在特定情况下有用。
List cats = sess.createCriteria(Cat.class)
                .createAlias("kittens", "kt")
                .createAlias("mate", "mt")
                .add(Expression.eqProperty("kt.name", "mt.name"))
                .list();
(createAlias())并不会创建一个Criteria的新实例。) 请注意，前面两个查询中Cat实例所持有的kittens集合类并没有通过criteria预先过滤！如果你希望只返回满足条件的kittens,你必须使用returnMaps()。
List cats = sess.createCriteria(Cat.class)
>  .createCriteria("kittens", "kt")
>  .add( Expression.eq("name", "F%") )
>  .returnMaps()
>  .list();
Iterator iter = cats.iterator();
while ( iter.hasNext())
{  
 Map map = (Map) iter.next();
 Cat cat = (Cat) map.get(Criteria.ROOT_ALIAS);
 Cat kitten = (Cat) map.get("kt");
} 
**5、动态关联对象获取（Dynamic association fetching） 可以在运行时通过setFetchMode()来改变关联对象自动获取的策略。**
List cats = sess.createCriteria(Cat.class)
                .add( Expression.like("name", "Fritz%") )
                .setFetchMode("mate", FetchMode.EAGER)
                .list();
这个查询会通过外连接(outer join)同时获得 mate和kittens。 
**6、根据示例查询（Example queries） net.sf.hibernate.expression.Example类许你从指定的实例创造查询条件。**
Cat cat = new Cat();
cat.setSex('F');
cat.setColor(Color.BLACK);
List results = session.createCriteria(Cat.class) 
                    .add( Example.create(cat) ) 
                    .list(); 
版本属性，表示符属性和关联都会被忽略。默认情况下，null值的属性也被排除在外。 You can adjust how the Example is applied. 你可
以调整示例(Example)如何应用。 Example example = Example.create(cat) .excludeZeroes() //exclude zero valued properties
.excludeProperty("color") //exclude the property named "color" .ignoreCase() //perform case insensitive string comparisons
.enableLike(); //use like for string comparisons
List results = session.createCriteria(Cat.class)
                      .add(example)
                      .list(); 
你甚至可以用示例对关联对象建立criteria。 List results = session.createCriteria(Cat.class) .add(Example.create(cat) )
.createCriteria("mate") .add(Example.create(cat.getMate())) .list();
参考代码如下：
- 
`String[] aa = new String[2];`
- 
- 
- 
`List results = getSession().createCriteria(`
- 
`"base.database.entity.Menu")`
- 
`.add(Example.create(instance))`
- 
`.add(Restrictions.between("date",new Date(),new Date()))`
- 
`.add(Restrictions.or(Expression.eq("flg", "S"), Expression.eq("FLG", "D")))`
- 
`.add(Restrictions.in("flg", aa))`
- 
`.addOrder(Order.asc("menuOrder"))`
- 
`.list();`
- 
- 
- 
`add(Projections.groupProperty("color"))`
