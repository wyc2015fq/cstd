# Hibernate解决原生SQL映射为对象的问题探究 - 零度的博客专栏 - CSDN博客
2016年11月17日 16:54:14[零度anngle](https://me.csdn.net/zmx729618)阅读数：1586
                
       如何将使用本地原生SQL查询出来的结果映射为值对象的问题，这里就Hibernate中提供的方法做个结论。前提，这里没有使用属性的延迟加载技术。
　　假设有个值对像，如下：Java代码　
```
package　test;　
　
public　class　Person　{　
　　private　Long　id;　
　　private　String　name;　
　　private　Long　age;　
　　private　Long　phone;　
　　private　String　address;　
　
　　public　Person(Long　id,　String　name,　Long　age,　Long　phone,　String　address)　{　
　　　　this.id　=　id;　
　　　　this.name　=　name;　
　　　　this.age　=　age;　
　　　　this.phone　=　phone;　
　　　　this.address　=　address;　
　　}　
　
　　public　Long　getId()　{　
　　　　return　id;　
　　}　
　
　　public　void　setId(Long　id)　{　
　　　　this.id　=　id;　
　　}　
　
　　public　String　getName()　{　
　　　　return　name;　
　　}　
　
　　public　void　setName(String　name)　{　
　　　　this.name　=　name;　
　　}　
　
　　public　Long　getAge()　{　
　　　　return　age;　
　　}　
　
　　public　void　setAge(Long　age)　{　
　　　　this.age　=　age;　
　　}　
　
　　public　Long　getPhone()　{　
　　　　return　phone;　
　　}　
　
　　public　void　setPhone(Long　phone)　{　
　　　　this.phone　=　phone;　
　　}　
　
　　public　String　getAddress()　{　
　　　　return　address;　
　　}　
　
　　public　void　setAddress(String　address)　{　
　　　　this.address　=　address;　
　　}　
}
```
　　 如果查询全部五列记录的话，那么只要：
        Java代码：　　
`List　list　=　getHibernateTemplate().loadAll(Person.class);`
　　 如果只需要查询id,name,age三列记录呢？那么就要新增构造方法了：
　　Java代码：　　　
```
public　Person(Long　id,　String　name,　Long　age)　{　
　      this.id　=　id;　
　      this.name　=　name;　
　      this.age　=　age;　
　}
```
　　 然后，就可以通过HQL来进行查询。
　　 Java代码：　　
`List　list　=　getHibernateTemplate().find("select　new　test.Person(id,name,age)　from　Person");`
　     这个方法通常可以满足需要了。但是，如果还需要查询id,name,phone三列记录的话，还需要新增构造方法，不行了，会出现构造方法冲突了。有个办法：
　　  Java代码：　　
```
List　list　=　getSession().createQuery("select　id,name,phone　from　person")　
　　　.addScalar("id",Hibernate.Long).addScalar("name").addScalar("phone",Hibernate.Long)　
　　　.addEntity(Person.class);
```
　　但是，这个只适用于存在Person实体的，如果Hibernate中没有进行Person映射就不可行，如系统中只存在一个JavaBean，可通过下面方法：
　　Java代码　　
```
List　list　=　getSession().createSQLQuery("select　id　\"id\",name　\"name\",phone　\"phone\"　from　person")　
　　　.addScalar("id",Hibernate.Long).addScalar("name").addScalar("phone",Hibernate.Long)　
　　　.setResultTransformer(Transformers.aliasToBean(Person.class)));
```
　　那么Hibernate就会自动将查出来的三列内容组装到VO对象中去，只是代码量有点大，而且名称都需要重新定义为小写的(需set方法属性一致)，在Oracle中查出来的列都默认为大写的（不知道其它数据库怎么样）
　　这个办法就不依赖于构造方法了，只需要定义私有变量，设置getter/setter方法就行了。
　　不过如果更猛点的，根本就没有JavaBean对象可以填充怎么办，Hibernate可以将查出来的列组装到集合类中去。如Map。
　　Java代码：　　
```
List　list　=　getSession().createSQLQuery("select　*　from　person")　
　　　　　　　　　　.setResultTransformer(Transformers.ALIAS_TO_ENTITY_MAP);
```
　　除了这个Transformers还可以将列转化为List。
```
Java代码　　
List　list　=　getSession().createSQLQuery("select　*　from　person")　
　　　　　　　　　　.setResultTransformer(Transformers.T0_LIST);
```
　　到此，还可以通过继承Transformers将结果映射为其它对象，不累述了，基本功能够用了。
