# hibernate的 HQL、QBE、QBC - 苝花向暖丨楠枝向寒 - CSDN博客

2018年05月22日 22:56:32[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：72


一：HQL语句  -- 面向对象的思想，字段用pojo的变量名代替，表名用pojo类名代替，有几种参数的表示方法，这里不举例，网上很多。

没有select 的 hql查询语句：查询该表所有字段。调用list()方法，返回的是list<pojo>

```java
String hql = "from EmpVo e where e.empname=:empname";

    Query createQuery = session.createQuery(hql);

    List<EmpVo> emp = createQuery .list();
```

有select的查询语句，查询部分字段：调用list()方法，返回的是list<Object[]>，如果想让返回的是list<pojo>，则可以将hql

```java
String hql = "select empname,cardno,sex from EmpVo e where e.empname=:empname";
改为   

    String hql = "select new EmpVo(empname,cardno,sex) from EmpVo e where e.empname=:empname";
```

同时 在pojo中 生成 对应的 有参数构造方法    如  

```java
public EmpVo(String empname,String cardno,String sex){

                this.empname = empname;
                this.cardno = cardno;

                this.sex = sex;

            }
```

但这种方式很麻烦，刚好hibernate有个Transformers类，他可以将返回的结果进行转换，主要就是来解决返回的结果是list<Object[]> ，不好遍历，也不方便取值。  下面介绍用法：

```java
String hql ="select empname as name,cardno as cardno,sex as sex from EmpVo";
Query createQuery = session.createQuery(hql);
list=createQuery.setResultTransformer(Transformers.ALIAS_TO_ENTITY_MAP).list();
```

注意 ： 如果用的是 hql查询，我们的hql语句必须 有别名，否则转换出来的数据为空

原因：  查询出的数据，做类型转换，会自动调用下面这个方法，tuple是查出来的数据，aliases是别名。

```java
@Override
	public Object transformTuple(Object[] tuple, String[] aliases) {
		Map result = new HashMap(tuple.length);
		for ( int i=0; i<tuple.length; i++ ) {
			String alias = aliases[i];
			if ( alias!=null ) {
				result.put( alias, tuple[i] );
			}
		}
		return result;
	}
```

可以看出 如果alias == null，就会返回一个空map，即map这个对象存在，但里面并没有存任何数据。

但是： 还有另外 一个方法，我个人认为比较好用 

```java
String sql1 = "select empname,cardno,sex from employee";
	NativeQuery createSQLQuery = session.createSQLQuery(sql1);
	createSQLQuery.setResultTransformer(Transformers.ALIAS_TO_ENTITY_MAP).list();
```
直接用sql语句来查询，并且调用的是
```java
session.createSQLQuery(sql1);
```

当执行transfromTuple的时候，会将表的字段名，作为别名，所以用这种方式，就不需要自己去为字段设置别名了。

```java
@Override
	public Object transformTuple(Object[] tuple, String[] aliases) {
		Map result = new HashMap(tuple.length);
		for ( int i=0; i<tuple.length; i++ ) {
			String alias = aliases[i];
			if ( alias!=null ) {
				result.put( alias, tuple[i] );
			}
		}
		return result;
	}
```

二、QBE--在线查询 

```java
// QBE 的查询 方式 更加 体现面向对象的 思想 --QBE 返回的list 可以 直接打印出来 -HQL不可以
	public List findEmpByQbc(EmpVo emp) {
		List list = null;
		Session session = HibernateSessionFactory.getSession();
		Transaction tx = session.getTransaction();
		try {
			tx.begin();

			Criteria criteria = session.createCriteria(EmpVo.class);
			criteria.add(Restrictions.like("empname", "张", MatchMode.ANYWHERE));
			list = criteria.list();
			tx.commit();
			session.close();
		} catch (Exception e) {
			tx.rollback();
			e.printStackTrace();
		}
		return list;

	}
```

Restrictions 就相当于 sql 里面的 where ，可以加 一些条件来限制。 

MatchMode就是 限制的作用域： 如例子中 要求empname 中必须包含张这个字，至于张这个字出现在哪里都可以，只要包含了就行，这就是 MatchMode.ANYWHERE 的作用。

三、QBC 查询   --- 离线查询 。

QBE 查询的 弊端 就是 通用性查，我现在查询emp表，需要写一个方法，当时这个方法当我要查询 dep表的时候就不适用了。

因为表与表的字段不同，约束条件也会不同。

所以产生了 QBC 查询， QBC查询实现了 将  限制的条件 与 session 分离， 我们在调用这个查询方法前，通过 DetachedCriteria 对象，来设置一些约束条件，然后 再调用 方法，将 DetachedCriteria 与 session 绑定。 

下面 展示 代码

```java
// 离线查询 --- 分页查询
	public List findByRow(DetachedCriteria dc, int startrow, int pagesize) {
		List list = null;
		HibernateSessionFactory.getSession().clear();
		Session session = HibernateSessionFactory.getSession();
		Transaction tx = session.getTransaction();
		try {
			tx.begin();
			// 绑定数据库连接
			Criteria cr = dc.getExecutableCriteria(session);
			// 去重复
			cr.setResultTransformer(cr.DISTINCT_ROOT_ENTITY);
			cr.setMaxResults(pagesize); // 每页显示记录数
			cr.setFirstResult(startrow); // 设置起始位置
			list = cr.list();
			tx.commit();
			session.close();

		} catch (Exception e) {
			tx.rollback();
			e.printStackTrace();
		}
		return list;
	}
```

调用

```java
public static void main6(String[] args) {
		
		HBaseDao base = new HBaseDao();
		//离线查询	
		DetachedCriteria dc = DetachedCriteria.forClass(EmpVo.class);  //查emp这个表
		dc.add(Restrictions.eq("empname", "张三));   //设置条件，empname = "张三"
		find = base.findByRow(dc, 0, 1);      //调用方法，传一个DetachedCriteria对象，分页的起始条数，每页显示数据数目
		System.out.println("离线查询，分页查询 ：" +find);

	}
```

