# Morphia 学习一 注解 - z69183787的专栏 - CSDN博客
2016年10月27日 07:12:48[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1373
**可参考：http://blog.csdn.net/small_love/article/details/5943123**
**Morphia** 是一个针对Mongo和Java 对象转换的映射的轻量级ORM类型安全类库。
      1.简单易用，轻量级，一旦每一种类型通过反射获取将被缓存，性能比较好。
      2.Datastore和DAO<T,V>的抽象封装。
      3.快速的查询的支持，在类运行时进行校验。
      4.Mapping是基于注解而不是基于xml。
      5.针对Validation和Log的扩展。
      6.生命周期的控制。
      7.可以和Spring，Guice等DI框架整合。
      8.支持各种功能的扩展。
## **Entity注解**
@Entity      value属性为DBConllection设置名称。必须有一个无参的默认构造方法，可以是public、protected、private等
      noClassnameStored属性默认为存储类名。如果只存储单一的实体对象并且关心数据库大小，不存储类名是安全的。
      保存类名的主要目的是在同一个链接中保存不同的实体对象，但是你想作为他们的基类或超类来读取。如果不在文档中保存类名，Morphia将不能正确的识别创建那个类。如：
- @Entity("animals") abstract class Animal { String name; }   
- @Entity("animals") Cat extends Animal { ... }   
- @Entity("animals") Dog extends Animal { ... }   
- 
- //And then performing the following query...  
- 
List<Animal> animals = ds.createQuery(Animal.class).asList();   
@Id
        @Id将值注解为MongoDB的唯一ID字段，MongoDB必须有一个唯一索引，mongo会自动生成id。如果使用其他类型，需要自己设置。
- @Id  
- private ObjectId id;  
**@Indexed**
    当datastore.ensureIndexes() 方法调用时，mongoDB将该值生成索引。
- @Indexed(value=IndexDirection.ASC, name="upc", unique=true, dropDups=true)   
- private String upcSymbol;  
    value ：指定index的方向。默认ASC。 IndexDirection.ASC (ascending), IndexDirection.DESC (descending), IndexDirection.BOTH (both)
    name ：指定index的名称。默认由mongoDB产生
    unique：是否为唯一索引。默认false。如果为true，插入重复值会报错。 
    dropDups：通知唯一索引删除重复值，只有第一条被保留，默认为false。
**@Indexes&@Index**
    复合indexes可以指定多个字段，该注解是class级别。例如下面代码指定user为默认升序同时date为降序（-表示DESC）
- @Entity // this is require to know where the indexes are to be created  
- @Indexes( @Index("user, -date") )  
- public class ChangeLog{  
- 
 Date date;  
- 
 String user;  
- 
 Record changedRecord;  
- 
} </span>  
如下表示可以找到最近变化的记录，该集合拥有两个复合indexes
@Transient 不将该字段保存到mongoDB@Property("feild_name") 指定该对象的属性映射到mongoDB的字段名称，默认为该对象的属性名。
@Serialized 字段被转换成二进制，并且被存储
@NotSaved 字段不会被保存，但是能被加载，良好的数据迁移
@AlsoLoad 该字段所以提供的名字都能被加载，良好的数据迁移
@Version 为Entity提供一个乐观锁，动态加载，不需要设置值
- @Entity  
- class Myclass {  
- 
   ...  
- @Version Long v;  
- 
}  
@Embedded 创建一个类被嵌套在实体类中的情况下使用，例如，在Hotel类中 可能会有一个Address。Address是Hotel不可分割的一部分，没有ID, 并且不会被存储在分开的collection中。事实上被@Embedded注释的类不允许有@Id
- @Entity  
- public class Hotel {  
- 
- @Id  
- private String id;  
- 
- private String name;  
- private int stars;  
- 
- @Embedded  
- private Address address;  
- 
- // ... getters and setters  
- 
}  
- 
- 
...  
- 
- import com.google.code.morphia.annotations.Embedded;  
- 
- @Embedded  
- public class Address {  
- 
- private String street;  
- private String city;  
- private String postCode;  
- private String country;  
- 
- // ... getters and setters  
- 
}  
@Reference 在数据库中引用另外一个文档，可以在多个Entity中引用同一个文档，注意被引用对象在被引用之前必须已经保存到mongoDB中。
      concreteClass: 指定具体的实体类。 
      ignoreMissing:  忽略任何不能解决的参考。 
      lazy：为参考创建一个代理，这个将在第一次调用时加载(类似Hibernate中的lazy属性) 
      value: 指定在Mongo中存储的属性名。默认使用对象的属性名保存到mongo中 
生命周期方法注解（delete没有生命周期事件）
@PrePersist save之前被调用，它可以返回一个DBObject代替一个空的
@PostPersist save到datastore之后被调用
@PreLoad 在Entity被映射到datastore对象之前被调用，可以对参数DBObject进行add/remove/change
@PostLoad     在Entity被映射之后被调用
@EntityListeners 指定外部生命周期事件实现类
- @EntityListeners(BackAccountWatcher.class)  
- class BankAccount {  
- @Id String id;  
- 
  Date lastUpdated = new Date();  
- 
}  
- 
- class BankAccountWatcher{  
- @PrePersist  
- void PrePersist() {}                  
- @PrePersist  
- void PrePersistWithParam(BankAccount act) {act.lastUpdated = new Date();}  
- @PrePersist  
- 
    DBObject PrePersistWithParamAndReturn(DBObject dbObj) {}  
- 
- @PostPersist  
- void postPersist() {}                  
- @PostPersist  
- void postPersistWithParam(DBObject dbObj) {}  
- 
- @PreLoad  
- void preLoad() {}                  
- @PreLoad  
- void PreLoadWithParam(DBObject dbObj) {}                
- @PreLoad  
- 
    DBObject PreLoadWithParamAndReturn(DBObject dbObj) {}  
- 
- @PostLoad  
- void PostLoad() {}                  
- @PreLoad  
- void PostLoadWithParam(DBObject dbObj)  
1、@Entity 
如果你想通过Morphia把你的对象保存到Mongo中，你首先要做的是使用@Entity注解你的类：
**[java]**[view
 plain](http://blog.csdn.net/withiter/article/details/9341815#)[copy](http://blog.csdn.net/withiter/article/details/9341815#)
[print](http://blog.csdn.net/withiter/article/details/9341815#)[?](http://blog.csdn.net/withiter/article/details/9341815#)
- import com.google.code.morphia.annotations.Entity;   
- 
- @Entity
- 
- publicclass Hotel {   
- ...   
- }   
A、名称和构造方法
你可以为你的MongoDBDBConllection设置名称。必须有一个无参的默认构造方法。
**[java]**[view
 plain](http://blog.csdn.net/withiter/article/details/9341815#)[copy](http://blog.csdn.net/withiter/article/details/9341815#)
[print](http://blog.csdn.net/withiter/article/details/9341815#)[?](http://blog.csdn.net/withiter/article/details/9341815#)
- @Entity("hotels")   
- 
- publicclass Hotel {   
- ...   
- public Hotel() {   
- 
- }   
- ...   
- }    
注意：你的默认构造方法不一定是public，也可以是protected或private。
B、在文档中的类名和怎样废弃使用类名
在@Entity注解中提供了一个可选的参数用来标识是否在文档中保存类名。 
**[java]**[view
 plain](http://blog.csdn.net/withiter/article/details/9341815#)[copy](http://blog.csdn.net/withiter/article/details/9341815#)
[print](http://blog.csdn.net/withiter/article/details/9341815#)[?](http://blog.csdn.net/withiter/article/details/9341815#)
- @Entity(value="hotels", noClassnameStored=true)  
- 
- publicclass Hotel {  
- ...  
- public Hotel() {  
- }  
- ...  
- }  
默认会在文档中保存类名。为什么要使用他？保存类名的主要目的是，当你在同一个链接中保存不同的实体对象，但是你想作为他们的基类或超类来读取。
如：
**[java]**[view
 plain](http://blog.csdn.net/withiter/article/details/9341815#)[copy](http://blog.csdn.net/withiter/article/details/9341815#)
[print](http://blog.csdn.net/withiter/article/details/9341815#)[?](http://blog.csdn.net/withiter/article/details/9341815#)
- @Entity("animals") abstractclass Animal { Stringname; }  
- @Entity("animals") Cat extends Animal { ... }  
- @Entity("animals") Dog extends Animal { ... }  
- 
- //And then performing the following query...
- List<Animal> animals =ds.createQuery(Animal.class).asList();  
**[java]**[view
 plain](http://blog.csdn.net/withiter/article/details/9341815#)[copy](http://blog.csdn.net/withiter/article/details/9341815#)
[print](http://blog.csdn.net/withiter/article/details/9341815#)[?](http://blog.csdn.net/withiter/article/details/9341815#)
- 正如你看到的，如果不在文档中保存类名，Morphia将不能正确的识别创建那个类。  
- 如果你在一个链接中只保存唯一的实体类型并且你还关心你的数据库大小，在你的@Entity注解上加上noClassnameStored=true参数将会是安全的。  
2、@Id
被@Entity注解的类必须有一个唯一的 @Id值；这些值被保存在MongoDB[数据库](http://lib.csdn.net/base/mysql)的“id”属性中，这是一个必须的位索引。像在Hotel类中：
**[java]**[view
 plain](http://blog.csdn.net/withiter/article/details/9341815#)[copy](http://blog.csdn.net/withiter/article/details/9341815#)
[print](http://blog.csdn.net/withiter/article/details/9341815#)[?](http://blog.csdn.net/withiter/article/details/9341815#)
- @Entity
- publicclass Hotel{  
- 
- @Id
- private ObjectId id;  
-    .....  
- }  
如上所示：被@Id注解的ID是ObjectId类型的，这样，你就不用担心Id的生成问题了，Mongo会自动为你生成一个唯一的新的ObjectId对象（就像MySQL中的auto_increment类型的ID）。如果你想使用其他的类型作为ID，那么在你保存数据时就必须手动为ID赋值。
3、@Indexed
此注解是为某个字段申请一个索引。 当datastore.ensureIndexes()方法被调用时 这些索引就会被申请。更多如下：
把@Indexed注解在你想让MongoDB创建索引的属性上。
**[java]**[view
 plain](http://blog.csdn.net/withiter/article/details/9341815#)[copy](http://blog.csdn.net/withiter/article/details/9341815#)
[print](http://blog.csdn.net/withiter/article/details/9341815#)[?](http://blog.csdn.net/withiter/article/details/9341815#)
- @Entity
- publicclass Product {  
- 
- @Id
- private ObjectId id;  
- 
- @Indexed(value=IndexDirection.ASC, name="upc",unique=true, dropDups=true)  
- private StringupcSymbol;  
-     .......  
- }  
参数说明如下：
value：表名这个索引的方向；IndexDirection.ASC(升序)，IndexDirection.DESC(降序), IndexDirection.BOTH(两者)；默认为 升序。
name： 被创建的索引的名称；mongodb默认创建的索引名的格式为(key1_1/-1_key2_1)。
unique：创建一个唯一索引，当创建唯一索引后，当在此字段插入相同的值时将会报错。true:为唯一索引；false：不是唯一索引；默认为：false。
dropDups：此参数表明，当为某个字段创建唯一索引时，删除其他相同值的记录。只保留第一条记录。true:删除重复；false:不删除重复（当有重复值时唯一索引创建失败）；默认为false。
向MongoDB申请创建索引 Datastore.ensureIndexes()需要被调用。这个方法应该在你已经使用Morphia注册了你的实体类后被调用。他将会异步创建你的所有索引。 这个动作也许会在你每次启动你的应用时都被执行。
注意：当在一个存在的系统上，创建已经存在的索引时，将不会花费任何事件(也不会做任何事情)。
**[java]**[view
 plain](http://blog.csdn.net/withiter/article/details/9341815#)[copy](http://blog.csdn.net/withiter/article/details/9341815#)
[print](http://blog.csdn.net/withiter/article/details/9341815#)[?](http://blog.csdn.net/withiter/article/details/9341815#)
- Morphia m =.....  
- Datastore ds =....  
- m.map(Product.class);  
- ds.ensureIndexes();  //为被@Indexed注解的所有字段创建索引。
你可以在 [http://www.mongodb.org/display/DOCS/Indexes](http://www.mongodb.org/display/DOCS/Indexes) 
 了解更多关于MongoDB索引的信息。
 4、@Embedded
你可以创建一个类被嵌套在实体类中，在这种情况下我们可以使用@Embedded注解。例如，在Hotel类中 可能会有一个Address。Address是Hotel不可分割的一部分，没有ID,并且不会被存储在分开的collection中。在这种情况下我们可以使用@Embedded注解Address。
**[java]**[view
 plain](http://blog.csdn.net/withiter/article/details/9341815#)[copy](http://blog.csdn.net/withiter/article/details/9341815#)
[print](http://blog.csdn.net/withiter/article/details/9341815#)[?](http://blog.csdn.net/withiter/article/details/9341815#)
- @Entity
- publicclass Hotel{  
- 
- ...  
- @Id
- private ObjectId id;  
- 
- @Embedded
- private Address address;  
- ...  
- }  
- 
- @Embedded
- publicclass Address{  
- ...  
- }  
正如你所看到的，被@Embedded注解的类没有@Id。 这是因为他们经常被嵌套在其他类中。事实上，被@Embedded注解的类也不允许有@Id。
 5、@Transient和@Property
一般情况下，保存原始和基本类型的属性到Mongo中，你不必为他们添加任何注解：
**[java]**[view
 plain](http://blog.csdn.net/withiter/article/details/9341815#)[copy](http://blog.csdn.net/withiter/article/details/9341815#)
[print](http://blog.csdn.net/withiter/article/details/9341815#)[?](http://blog.csdn.net/withiter/article/details/9341815#)
- privateint myInt;  
- private Date myDate;  
- private List<String> myStrings;  
- private String[] stringArray;  
默认情况下，Morphia将会试着映射所有支持的基本和原始类型到Mongo，包括这些类型的数组。
MongoDB仅仅有以下数据类型：
•Integer(32位有符号值)
•Long(64有符号值) 
•Double(64位IEEE745浮点值)
•String (字符串)
这里有些有价值的转换：
•float  →  double
•byte   →  int
•short  →  int
•char  →  String       
并且，依赖于你的模型中存在的类型和数据库将会试着自动为你转换。我的意思是，因为在MongoDB数据库中只存在三种数字类型(32/64有符号， 64位FP)这将相当容易转换。下面的列表为当前什么样的基本和原始类型可以被保存：
 •enum(在Mongo中作为String保存)
•[Java](http://lib.csdn.net/base/javaee).util.Date(在Mongo中保存为从新纪元UTC的毫秒数)
•java.util.Locale(作为字符串保存)
•com.mongodb.DBRef
•com.mongodb.ObjectId                  
 正如我我们上面的例子所示，Morphia还支持java.util.List,java.util.Set和java.util.Map容器，和任何支持的原始类型的数组。如果你想把一个属性排除映射到Mongo中， 你可以使用@Transient注解：
**[java]**[view
 plain](http://blog.csdn.net/withiter/article/details/9341815#)[copy](http://blog.csdn.net/withiter/article/details/9341815#)
[print](http://blog.csdn.net/withiter/article/details/9341815#)[?](http://blog.csdn.net/withiter/article/details/9341815#)
- import com.google.code.morphia.annotations.Transient;  
- 
- @Transient
- privateint myTransientInt;  
默认情况下，Morphia把属性名作为Mongo中的字段名。这个可以通过@Property注解进行修改，指定一个名称。
**[java]**[view
 plain](http://blog.csdn.net/withiter/article/details/9341815#)[copy](http://blog.csdn.net/withiter/article/details/9341815#)
[print](http://blog.csdn.net/withiter/article/details/9341815#)[?](http://blog.csdn.net/withiter/article/details/9341815#)
- import com.google.code.morphia.annotations.Property;   
- 
- @Property("my_integer")   
- privateint myInt;  //在MongoDB中为my_integer   
6、@Reference
Mongo在同一个数据库引用一个文档(对象)到另一个。考虑以下类：
**[java]**[view
 plain](http://blog.csdn.net/withiter/article/details/9341815#)[copy](http://blog.csdn.net/withiter/article/details/9341815#)
[print](http://blog.csdn.net/withiter/article/details/9341815#)[?](http://blog.csdn.net/withiter/article/details/9341815#)
- publicclass BlogEntry {  
- private Stringtitle;  
- private DatepublishDate;  
- private Stringbody;  
- private Authorauthor;  
- // getters andsetters
- }  
**[java]**[view
 plain](http://blog.csdn.net/withiter/article/details/9341815#)[copy](http://blog.csdn.net/withiter/article/details/9341815#)
[print](http://blog.csdn.net/withiter/article/details/9341815#)[?](http://blog.csdn.net/withiter/article/details/9341815#)
- publicclass Author {  
- private Stringusername;  
- private StringfullName;  
- private StringemailAddress;  
- // getters andsetters
- 
- }         
这里的一个问题就是：我们如果注解BlogEntry中的author属性？当然，我们可以是用@Embedded注解，但是这样就没什么意义了，因为在每个BlogEntry实例中都保存一个Author对象。我们反而想在多个blog实例中引用一个单独的Author文档(对象)在Mongo中。在这种情况下我们使用 @Reference注解
**[java]**[view
 plain](http://blog.csdn.net/withiter/article/details/9341815#)[copy](http://blog.csdn.net/withiter/article/details/9341815#)
[print](http://blog.csdn.net/withiter/article/details/9341815#)[?](http://blog.csdn.net/withiter/article/details/9341815#)
- import com.google.code.morphia.annotations.Entity;  
- import com.google.code.morphia.annotations.Embedded;  
- import com.google.code.morphia.annotations.Id;  
- import com.google.code.morphia.annotations.Reference;  
- import com.google.code.morphia.annotations.Property;  
- 
- @Entity
- publicclass BlogEntry {  
- 
- @Id
- private ObjectIdid;  
- private Stringtitle;  
- private DatepublishDate;  
- private Stringbody;  
- 
- @Reference
- private Authorauthor;  
- // getters andsetters
- 
- }  
**[java]**[view
 plain](http://blog.csdn.net/withiter/article/details/9341815#)[copy](http://blog.csdn.net/withiter/article/details/9341815#)
[print](http://blog.csdn.net/withiter/article/details/9341815#)[?](http://blog.csdn.net/withiter/article/details/9341815#)
- @Entity
- publicclass Author {  
- 
- @Id
- private ObjectIdid;  
- private Stringusername;  
- private StringfullName;  
- private StringemailAddress;  
- // getters andsetters
- 
- }   
当使用引用时有很重要的一点我们必须提及：被引用的对像在被引用之前必须已经保存到了MongoDB数据库中。这个真的的意思是。就像上面的例子，在你创建一个BlogEntry对象之前，一个Author已经被保存到了数据库中。默认情况下，Morphia使用属性名作为在数据库中保存的值。当然这个可以在@Reference注解中指定。
**[java]**[view
 plain](http://blog.csdn.net/withiter/article/details/9341815#)[copy](http://blog.csdn.net/withiter/article/details/9341815#)
[print](http://blog.csdn.net/withiter/article/details/9341815#)[?](http://blog.csdn.net/withiter/article/details/9341815#)
- @Reference("blog_authors")  
- privateList<Author> authors;      
补充：注解使用的参数。
concreteClass：指定具体的实体类。
ignoreMissing：忽略任何不能解决的参考。
lazy：为参考创建一个代理，这个将在第一次调用时加载(类似[hibernate](http://lib.csdn.net/base/javaee)中的lazy属性)
value：指定在Mongo中存储的属性名。
