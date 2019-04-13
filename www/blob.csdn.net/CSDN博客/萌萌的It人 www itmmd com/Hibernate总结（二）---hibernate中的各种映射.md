
# Hibernate总结（二）---hibernate中的各种映射 - 萌萌的It人 www.itmmd.com - CSDN博客


2014年01月06日 15:00:15[Jlins](https://me.csdn.net/dyllove98)阅读数：2050


知识的准备：
（参考：汤阳光Hibernate3.6(基础)）
一、浅谈javaBean之间的关系
存在的关系：
1.关联（一对一、一对多、多对一、多对多）
2.聚合/组合（最后考虑）
3.继承
所以，映射主要分为关联映射、继承映射、其它映射
二、hibernate映射研究的就是持久化对象那一整流程
![](https://img-blog.csdn.net/20140105193119156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2FveWlob21l/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
通过配置文件来控制映射
三个实体中，只要确定两个，就能得到第三个；
此文，主要是从javaBean+配置---------------自动生成-------------table（）
需要自动生成，在开发阶段最好在hibernate.cfg.xml中配

```python
<!-- bean自动生成table -->
  <property name="hbm2ddl.auto">create-drop</property>
```
```python
<!-- 打印sql语句 -->
  <property name="show_sql">true</property>
```

整理思路：
java实体由两部分组成：数据和关系；同理，数据库中的表也一样。
数据部分在不同的系统中，存储大同小异，只需要关系编码问题就OK了
但关系部分，在不同的系统中，有很大的差别；而hibernate就是解决，javaObject与DBTable之间的映射，
那么，我们的明白，在javaObject与DBTable具体对于关系是怎么存储的；
javaObject中数据以对象为基本单位，
主要有两种类型：一对一，对象之间相互通过以对方对象为属性来建立联系；
一对多，建立集合，对象的集合。
Db table中关系是通过主外键来建立联系的
![](https://img-blog.csdn.net/20140105195739875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2FveWlob21l/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
两者之间的映射，就是问题所在
正式研究关系
在hibernate中，要进行object与table之间的转换，对object我们不需要进行任何处理，而仅仅只需要对改变xml的配置文件即可。
而我们研究的重点也就是配置文件如何将不同的关系进行配置。

三、关系配置

1.最基本的配置，（假设对象之间没有任何关系，其它都为默认设置）
参见下面代码
在实体类包目录下建立，如本例
首先：
1）在com.tjcu.domain下建User

```python
public class User {
	
	private int id;
	private String name;
	private Date  birthday;
	
	//必须的
	public User() {
		
	}
	//get/set略
```
2）在com.tjcu.domain目录下建立User.hbm.xml文件

```python
<!-- 创建object与table之间的映射 -->
  <!-- 第一步，导入包命 -->
  <hibernate-mapping package="com.tjcu.domain">  
  	<!-- 第二步，建立class与table之间的映射 --> 
     <class name="User" table="user">
     	     
       <!-- 第三步，建立主键映射 ，-->
       <id name="id" column="id" >  
       	<!-- 注意了，更具不同的需求，选择不同的主键生成器，参考文档 -->           
         <generator class="native" />   <!-- "native"一种主键生成器，使用hibernate内置id生成 -->      
       </id>
       <!-- 第四步，将所有的属性都建立映射 -->         
       <property name="name" column="name" />         
       <property name="birthday" column="birthday" /> 
                 
     </class>   
  </hibernate-mapping>
```
3）在hibernate.cfg.xml文件中添加

```python
<mapping resource="com/tjcu/domain/User.hbm.xml"/>
```
以上就是一个最基本的配置
其中有一个难点：就是主键的生成方式

{
1.为什么有这样奇葩的主键生成器啊？
因为java中判断同一class而不同的object是通过内存地址来判断的，而table中是以id来判断的。
2.具体的主键生成器
native: 对于 oracle 采用 Sequence 方式，对于MySQL 和 SQL Server 采用identity（自增主键生成机制），native就是将主键的生成工作交由数据库完成，hibernate不管                       （很常用）。
identity: 使用SQL Server 和 MySQL 的自增字段，这个方法不能放到 Oracle 中，Oracle 不支持自增字段，要设定sequence（MySQL 和 SQL Server 中很常用）。
sequence: 调用底层数据库的序列来生成主键，要设定序列名，不然hibernate无法找到。
uuid: 采用128位的uuid算法生成主键，uuid被编码为一个32位16进制数字的字符串。占用空间大（字符串类型）。
hilo: 使用hilo生成策略，要在数据库中建立一张额外的表，默认表名为hibernate_unique_key,默认字段为integer类型，名称是next_hi（比较少用）。
foreign: 使用另外一个相关联的对象的主键。通常和<one-to-one>联合起来使用
我就用过这几个，还有别的，根据不同的需求，来查API；一般没有限制，我都采样native。
}
2.关联配置
单项关联：仅仅建立从Order到Customer的多对一关联，即仅仅在Order类中定义customer属性。或者仅仅建立从Customer到Order的一对多关联，即仅仅在Customer类中定义orders集合。
双项关联：既建立从Order到Customer的多对一关联，又建立从Customer到Order的一对多关联
1）一对多/多对一
many-to-one属性:
*name:设定待映射的持久化类的名字。
*column:设定和持久化类的属性对应的表的外键。
*class:设定持久化类的属性的类型。
*not-null:是否允许为空。

2）一对一
必须的区分主次
对于基于外键的1-1关联，其外键可以存放在任意一边，**在需要存放外键一端，增加 many-to-one**元素。为 many-to-one元素增加 unique=“true” 属性来表示为1-1关联，并用name属性来指定关联属性的属性名

```python
<many-to-one name="depart" column="depart_id" />
```

另一端需要使用one-to-one元素，该元素使用 property-ref(可以不加) 属性指定使用被关联实体主键以外的字段作为关联字段
```python
<!-- 一对一 -->
       <one-to-one name="person" constrained="true" />
```

3）继承
**Hibernate支持三种继承映射策略：**
¨      每个具体类一张表(table per concrete class) 将域模型中的每一个实体对象映射到一个独立的表中，也就是说不用在关系数据模型中考虑域模型中的继承关系和多态。
¨      每个类分层结构一张表(table per class hierarchy) 对于继承关系中的子类使用同一个表，这就需要在数据库表中增加额外的区分子类类型的字段。
¨      每个子类一张表(table per subclass) 域模型中的每个类映射到一个表，通过关系数据模型中的外键来描述表之间的继承关系。这也就相当于按照域模型的结构来建立数据库中的表，并通过外键来建立表之间的继承关系。

比较：
![](https://img-blog.csdn.net/20140106113028859?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvY2FveWlob21l/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
具体实现细节略了……

4）组成关系（Component）映射
组件属性的意思是持久化类的属性既不是基本数据类型，也不是 String 字符串，而是某个组件变量，该组件属性的类型可以是**自定义类**。
¨      显然无法直接用 property 映射 name 属性。为了映射组件属性， Hibernate 提供了**component**元素。
¨      每个 component 元素映射一个组件属性，组件属性必须指定该属性的类型，component 元素中的**class**属性用于确定组件的类型。


