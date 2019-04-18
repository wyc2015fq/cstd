# Morphia快速入门 - z69183787的专栏 - CSDN博客
2016年10月26日 18:14:17[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2764
# 使用Morphia
下面的代码说明了如何去初始化一个Morphia的实例。使用这个实例，你可以通过Morphia去映射你的实体。
```
Morphia morphia = new Morphia();
// 告诉Morphia在哪里找到你的类
// 可以为不同的包或者类进行多次的调用
morphia.mapPackage("org.mongodb.morphia.example");
//创建datastore，并连接到指定数据库
//datastore有两个参数，第一个用来连接到MongoDB，第二个是数据库的名字。
final Datastore datastore = morphia.createDatastore(new MongoClient(), "morphia_example");
datastore.ensureIndexes();
```
通过这种方法配置Morphia，我们可以通过创建多个Datastore实例来连接到多个数据库。
第二行代码，我们告诉Morphia去指定的package中寻找所有标记了`@Entity`的类，以及所有在类中的映射元数据。（@Id,@Reference等等）。
## 映射选项
当你拥有了Morphia实例，以可以通过`MappingOption`类去配置多种映射选项。当创建Morphia时，可以指定`Mapper`，但大部分用户会使用默认的`Mapper`。两种最常用的配置参数是`storeEmpties`和`storeNulls`。默认情况下，Morphia不会向MongoDB中存储空的表或者映射值，也不会存储空值。如果你需要去存储这些值，需要把这两个参数设置为`true`。其它的一些设置这里不一一赘述。
# 映射类
Morphia有两种方法控制你的类：作为顶级的实体或者潜入在其它实体内。
所有被注释为`@Entity`的类都会被当作一个顶级的文档直接存储在一个集合中。所有被注释为`@Entity`的类都必须有一个字段注释为`@Id`，这会被用来当作MongoDB中的`_id`字段。`@Embedded`表明这个类会被作为一个文档里的子文档。`@Embedded`类不需要有`@Id`字段。
```
@Entity("employees")
@Indexes(
    @Index(value = "salary", fields = @Field("salary"))
)
class Employee {
    @Id
    private ObjectId id;
    private String name;
    @Reference
    private Employee manager;
    @Reference
    private List<Employee> directReports;
    @Property("wage")
    private Double salary;
}
```
这里我们举了一个例子。这个类使用了`@Entity`注释，所以它是一个顶层的文档。默认情况下，Morphia会使用类名作为集合名，如果你在之前指定了，如`"employees"`，它会使用这个值作为集合名。在这个例子中，所有的Employee实例都会被保存在`"employees"`集合中。
注释`@Indexes`列出了Morphia应该创建的索引。在这个实例中，我们定义了一个名为`salary`的索引，它对salary做了升序排序。
我们标记了`id`字段作为主键。这个例子中我们使用了Java驱动中的`ObjectId`类型作为`id`的类型。这个ID可以是任何类型。除了transient和static的字段，Morphia试图去保存任何没有被注释的字段。
对于注释`Property`，这个注释是可选的。如果不使用这个注释，Morphia会使用Java中的字段名作为对应文档中的字段名。如果有时你想要改变文档中字段的名字，你可以使用注释`Property`，它会将你指定的名字传递过去。
剩下的一个注释是`@Reference`这个注释告诉Morphia这个字段引用了其它的映射实体。这样Morphia会存储MongoDB中被称为`DBRef`，即集合的名字和一些键值对。这些引用实体必须已经被保存或者至少被分配了ID，否则Morphia会抛出一个异常。
## 保存数据
大多数情况下，你可以像对待Java对象一样，如果你准备像数据库中写入数据，可以这样： 
```
final Employee elmer = new Employee("Elmer Fudd", 50000.0);
datastore.save(elmer);
```
再进一步，我们可以定义一些关系并保存下来： 
```
final Employee daffy = new Employee("Daffy Duck", 40000.0);
datastore.save(daffy);
final Employee pepe = new Employee("Pepé Le Pew", 25000.0);
datastore.save(pepe);
elmer.getDirectReports().add(daffy);
elmer.getDirectReports().add(pepe);
datastore.save(elmer);
```
我们只需要创建和保存其它的雇员，然后我们就可以将他们添加到报告列表中并保存。想要更新MongoDB中的数据，可以更新Java中的对象然后再次调用`datastore.save()`。对于块更新，这不是一种有效的更新方式。我们可以直接在数据库中进行更新，而不是从每个文档中取出来，转化为Java对象，更新，又转化为文档，再写入MongoDB。为了告诉你怎么这么做，我们需要先学习查询。
## 查询
Morphia尝试使你的查询类型安全。Morphia将会控制你的数据的所有转换细节，只有很少的一部分需要你去做额外的工作。 
```
final Query<Employee> query = datastore.createQuery(Employee.class);
final List<Employee> employees = query.asList();
```
这是一个基本的Morphia查询。我们告诉`Datastore`去创建一个参数为`Employee`的查询。我们将所有取到的`Employee`放在一个`List`中。对于一个非常大的查询结果，对内存来说可能会吃不消。在这个简单的例子中，使用`asList()`是可以的，但实际上，`fetch()`通常是更好的选择。
大多数查询都希望使用某种方法去过滤数据，有两种方法： 
```
underpaid = datastore.createQuery(Employee.class)
                     .field("salary").lessThanOrEq(30000)
                     .asList();
```
这里使用了`field()`方法来对指定字段进行过滤，返回一个许多方法的接口的实例来建立一次查询。总的来说，查询的建立可以被很安全的完成。
另一种办法是使用`filter()`方法，这种方法比`field()`形式更自由简洁。这里我们可以在查询字符串中嵌入特定的操作。虽然这比另一种更简洁，但它在字符串中留下了更多的东西去验证，会有可能出错。
```cpp
```cpp
List<Employee> underpaid = datastore.createQuery(Employee.class)
                                    .filter("salary <=", 30000)
                                    .asList();
```
```
两种方法的选择在大多数情况下是喜好的问题。每种方法，Morphia都回去验证`Employee`中被叫做`salary`的字段。如果数据库中的字段和Java字段不匹配，Morphia会使用任一种形式，对任一个名字进行验证。
## 更新
更新由两部分组成：一个查询和和一组更新操作符。这个例子中，我们将找到所有薪水低的员工并给他们涨薪10000.第一步是创建一个查询去找到所有的低薪员工： 
```
final Query<Employee> underPaidQuery = datastore.createQuery(Employee.class)
                                             .filter("salary <=", 30000);
```
为了更新找到的文档，我们创建了一个`UpdateOperations`的实例。 
```
final UpdateOperations<Employee> updateOperations = datastore.createUpdateOperations(Employee.class)
                                                   .inc("salary", 10000);
```
这里我们将`salary`字段增加了10000，对应的操作符是`$inc`。最后一步是： 
`final UpdateResults results = datastore.update(underPaidQuery, updateOperations);`
这行执行了更新数据库。`UpdateResults`的实例将包含有关更新操作的各种统计数据。 
## 删除
删除操作只需要进行查询，然后告诉`Datastore`去删除它。
```cpp
```cpp
final Query<Employee> overPaidQuery = datastore.createQuery(Employee.class)
                                                .filter("salary >", 100000);
datastore.delete(overPaidQuery);
```
```
`delete()`方法有多种不同的形式，但这是做常用的。
文／KAIKIS（简书作者）
原文链接：http://www.jianshu.com/p/fb8f0b46a03a
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
