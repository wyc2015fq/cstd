# JPA注解详解(五) - z69183787的专栏 - CSDN博客
2014年04月02日 09:35:57[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2437

**@EntityListeners**
可以使用生命周期批注（请参阅[生命周期事件批注](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#LifecycleAnnotations)）指定实体中的方法，这些方法在指定的生命周期事件发生时执行您的逻辑。
使用 @EntityListeners 批注将一个或多个实体监听程序类与 [@Entity](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#Entity)或 [@MappedSuperclass](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#MappedSuperclass)关联，条件是您需要在指定的生命周期事件发生时执行逻辑，以及：
- 不希望在实体 API 中公开生命周期监听程序方法。
- 要在不同的实体类型之间共享生命周期监听程序逻辑。
当实体或子类上发生生命周期事件时，JPA 持续性提供程序将按监听程序定义的顺序通知每个实体监听程序，并调用使用相应的生命周期事件类型进行批注的实体监听程序方法（如果有）。
实体监听程序类具有以下特征：
- 它是一个普通的旧式 Java 对象 (POJO) 类
- 它有一个或多个具有以下签名的回调方法：
public void <MethodName>(Object)
可以指定参数类型 Object ，或实体监听程序将与其关联的实体类的类型。
- 它用一个或多个生命周期事件批注对每个回调方法进行批注。
一个生命周期事件只能与一个回调监听程序方法关联，但某个给定的回调监听程序方法可以与多个生命周期事件关联。
如果使用实体监听程序，则可以管理哪些实体监听程序使用 [@ExcludeDefaultListeners](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#ExcludeDefaultListeners)和[@ExcludeSuperclassListeners](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#ExcludeSuperclassListeners)调用。
[表 1-12](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDJJFJC)列出了此批注的属性。有关更多详细信息，请参阅 [API](http://java.sun.com/javaee/5/docs/api/javax/persistence/EntityListeners.html)。
表 1-12 @EntityListeners 属性
|**属性**|**必需**|**说明**|
|----|----|----|
|value||要为 [@Entity](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#Entity)或 [@MappedSuperclass](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#MappedSuperclass)指定实体监听程序类的列表，请将 value 设置为实体监听程序类的 Class 数组。|
[示例 1-21](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDJBJAG)显示了如何使用此批注将实体监听程序类 EmployeePersistListener （请参阅[示例
 1-22](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDCHCII)）和EmployeeRemoveListener （请参阅[示例 1-23](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDHDGAD)）与实体 Employee 关联。[示例
 1-23](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDHDGAD)显示了您可以将多个生命周期事件与给定的实体监听程序类方法关联，但任何给定的生命周期事件只能在实体监听程序类中出现一次。
示例 1-21 @EntityListeners
@Entity
@EntityListeners(value={EmployeePersistListner.class, EmployeeRemoveListener.class})
public class Employee implements Serializable {
    ...
}
示例 1-22 EmployeePersistListener
public class EmployeePersistListener {
@PrePersist
employeePrePersist(Object employee) {
    ...
    }
    ...
}
示例 1-23 EmployeeRemoveListener
public class EmployeeRemoveListener {
@PreRemove
@PostRemove
employeePreRemove(Object employee) {
    ...
    }
    ...
}
**@EntityResult**
执行 [@NamedNativeQuery](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#NamedNativeQuery)时，它可以返回实体（包括不同类型的实体）、标量值或实体和标量值的组合。
使用 @EntityResult 批注返回实体。
有关详细信息，另请参阅 [@ColumnResult](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#ColumnResult)、[@FieldResult](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#FieldResult)和 [@SqlResultSetMapping](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#SqlResultSetMapping)。
[表 1-8](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDGAGFI)列出了此批注的属性。有关更多详细信息，请参阅 [API](http://java.sun.com/javaee/5/docs/api/javax/persistence/EntityResult.html)。
表 1-13 @EntityResult 属性
|**属性**|**必需**|**说明**|
|----|----|----|
|entityClass||将 entityClass 设置为由 SELECT 语句返回的实体的 Class 。|
|discriminatorColumn||默认值：空 String 。默认情况下，JPA 持续性提供程序假设 SELECT 语句中不包含标识符列（请参阅[@Inheritance](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#Inheritance)）。如果在 SELECT 语句中使用标识符列，请将 discriminatorColumn 设置为所使用的 String 列名。|
|fields||默认值：空 FieldResult 数组。默认情况下，JPA 持续性提供程序假设 SELECT 语句包含与返回的实体的所有字段或属性相对应的所有列，且 SELECT 语句中的列名对应于字段或属性名（未使用 AS 语句）。如果 SELECT 语句只包含某些与返回的实体的字段或属性相对应的列，或 SELECT语句中的列名并不对应于字段或属性名（使用了 AS 语句），请将 fields 设置为[@FieldResult](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#FieldResult)的数组， SELECT 语句中的每一列一个 [@FieldResult](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#FieldResult)。|
[示例 1-24](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDHGEGE)显示了如何使用此批注将 Order 和 Item （请参阅[示例
 1-25](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDHAJBB)）实体包含在结果列表（请参阅[示例 1-26](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDDDHDC)）中。在该示例中，结果列表将为 Object 数组的 List ，如： {[Order,
 Item], [Order, Item], ...} 。
示例 1-24 使用 @EntityResult 的 Order 实体
@SqlResultSetMapping(
name="OrderResults",
entities={
@EntityResult(
entityClass=Order.class, 
fields={
@FieldResult(name="id",       column="order_id"),
@FieldResult(name="quantity", column="order_quantity"),
@FieldResult(name="item",     column="order_item")
            }
        ),
@EntityResult(
entityClass=Item.class,
fields={
@FieldResult(name="id",       column="item_id"),
@FieldResult(name="name",     column="item_name"),
            }
        )
    }
)
@Entity
public class Order {
@Id
protected int id;
protected long quantity;
protected Item item;
    ...
}
示例 1-25 Item 实体
@Entity
public class Item {
@Id
protected int id;
protected String name;
    ...
}
示例 1-26 结合使用 @SqlResultSetMapping 与 @EntityResult 的原生查询
Query q = entityManager.createNativeQuery(
"SELECT o.id       AS order_id, " +
"o.quantity AS order_quantity, " +
"o.item     AS order_item, " + 
"i.id       AS item_id, " +
"i.name     AS item_name, " +
"FROM Order o, Item i " +
"WHERE (order_quantity > 25) AND (order_item = i.id)",
"OrderResults"
);
List resultList = q.getResultList(); 
// List of Object arrays:{[Order, Item], [Order, Item], ...}
**@Enumerated**
默认情况下，JPA 持续性提供程序持久保存枚举常量的序数值。
使用 @Enumerated 批注指定在 String 值适合应用程序要求或与现有数据库模式匹配的情况下，JPA 持续性提供程序是否应持久保存枚举常量的序数值或 String 值。
该批注可以与 [@Basic](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#Basic)一起使用。
[表 1-14](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDFIAEE)列出了此批注的属性。有关更多详细信息，请参阅 [API](http://java.sun.com/javaee/5/docs/api/javax/persistence/Enumerated.html)。
表 1-14 @Enumerated 属性
|**属性**|**必需**|**说明**|
|----|----|----|
|value||默认值： EnumType.ORDINAL 。默认情况下，JPA 持续性提供程序假设对于映射到枚举常量的属性或字段，应持久保存序数值。在[示例 1-28](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDEJBHG)中，当持久保存 Employee 时， EmployeeStatus 的序数值将写入数据库。如果需要持久保存的枚举常量的 String 值，请将 value 设置为 EnumType.STRING 。|
根据[示例 1-27](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDEJJFC)中的枚举常量，[示例
 1-28](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDEJBHG)显示了如何使用此批注指定在持久保存 Employee 时应将 SalaryRate的 String 值写入数据库。默认情况下，会将 EmployeeStatus 的序数值写入数据库。
示例 1-27 枚举常量
public enum EmployeeStatus {FULL_TIME, PART_TIME, CONTRACT}
public enum SalaryRate {JUNIOR, SENIOR, MANAGER, EXECUTIVE}
示例 1-28 @Enumerated
@Entity
public class Employee {
    ...
public EmployeeStatus getStatus() {
    ...
    }
@Enumerated(STRING)
public SalaryRate getPayScale() {
    ...
    }
    ...
}
**@ExcludeDefaultListeners**
默认监听程序是 orm.xml 文件中指定的一个生命周期事件监听程序类，该类应用于持续性单元（请参阅[@PersistenceUnit](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#PersistenceUnit)）中的所有实体。在调用任何其他实体监听程序（请参阅 [@EntityListeners](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#EntityListeners)）之前，JPA 持续性提供程序首先按照 orm.xml 文件中定义的顺序调用默认监听程序（如果有）。
如果默认监听程序行为不适用，请使用 @ExcludeDefaultListeners 批注覆盖（并阻止）针对给定 [@Entity](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#Entity)或[@MappedSuperclass](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#MappedSuperclass)执行的默认监听程序。
此批注没有属性。有关更多详细信息，请参阅 [API](http://java.sun.com/javaee/5/docs/api/javax/persistence/ExcludeDefaultListeners.html)。
[示例 1-29](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDCGBHG)显示了如何使用此批注指定不应对 Employee 实体执行默认监听程序。
示例 1-29 @ExcludeDefaultListeners
@Entity
@ExcludeDefaultListeners
public class Employee implements Serializable {
    ...
}
