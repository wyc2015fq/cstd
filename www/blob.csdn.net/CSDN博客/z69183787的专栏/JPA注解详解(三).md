# JPA注解详解(三) - z69183787的专栏 - CSDN博客
2014年04月02日 09:35:19[OkidoGreen](https://me.csdn.net/z69183787)阅读数：877
**@ColumnResult**
执行 [@NamedNativeQuery](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#NamedNativeQuery)时，它可以返回实体（包括不同类型的实体）、标量值或实体和标量值的组合。
使用 @ColumnResult 批注返回标量值。标量类型由您在 @ColumnResult 中标识的列类型确定。
有关详细信息，另请参阅 [@EntityResult](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#EntityResult)、[@FieldResult](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#FieldResult)和 [@SqlResultSetMapping](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#SqlResultSetMapping)。
[表 1-8](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDGAGFI)列出了此批注的属性。有关更多详细信息，请参阅 [API](http://java.sun.com/javaee/5/docs/api/javax/persistence/ColumnResult.html)。
表 1-8 @ColumnResult 属性
|**属性**|**必需**|**说明**|
|----|----|----|
|name||在原生 SQL 查询的 SELECT 语句中将 name 设置为列名的 String 等效形式。如果在 SELECT中使用列别名（ AS 语句），则将 name 设置为列别名。|
[示例 1-9](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDEGDDF)显示了如何使用此批注将 Item （请参阅[示例
 1-10](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDIDFBJ)）标量 name 包含在结果列表（请参阅[示例 1-11](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDIDIAI)）中。在该示例中，结果列表将为 Object 数组的 List ，如： {[Order,
 "Shoes"], [Order, "Socks"], ...} 。
示例 1-9 使用 @ColumnResult 的 Order 实体
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
        )
    },
columns={
@ColumnResult(
name="item_name"
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
示例 1-10 Item 实体
@Entity
public class Item {
@Id
protected int id;
protected String name;
    ...
}
示例 1-11 结合使用 @SqlResultSetMapping 与 @ColumnResult 的原生查询
Query q = entityManager.createNativeQuery(
"SELECT o.id       AS order_id, " +
"o.quantity AS order_quantity, " +
"o.item     AS order_item, " + 
"i.name     AS item_name, " +
"FROM Order o, Item i " +
"WHERE (order_quantity > 25) AND (order_item = i.id)",
"OrderResults"
);
List resultList = q.getResultList(); 
// List of Object arrays:{[Order, "Shoes"], [Order, "Socks"], ...}
**@DiscriminatorColumn**
默认情况下，当 [@Inheritance](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#Inheritance)属性策略为 InheritanceType.SINGLE_TABLE 或 JOINED 时，JPA 持续性提供程序将创建一个名为 DTYPE 的标识符列以区分继承层次中的类。
使用 @DiscriminatorColumn 批注：
- 指定一个标识符列名（如果数据模型中的列名不是默认列名 DTYPE ）。
- 指定一个适用于应用程序或事先存在的数据模型的标识符列长度
- 微调数据库中的标识符列的特征
[表 1-9](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDGDIEG)列出了此批注的属性。有关更多详细信息，请参阅 [API](http://java.sun.com/javaee/5/docs/api/javax/persistence/DiscriminatorColumn.html)。
表 1-9 @DiscriminatorColumn 属性
|**属性**|**必需**|**说明**|
|----|----|----|
|columnDefinition||默认值：空 String 。默认情况下，JPA 持续性提供程序使用最少量 SQL 创建一个数据库表列。如果需要使用更多指定选项创建的列，请将 columnDefinition 设置为在针对列生成DDL 时希望 JPA 使用的 SQL 片断。|
|discriminatorType||默认值： DiscriminatorType.STRING 。默认情况下，JPA 持续性提供程序假设标识符类型为 String 。如果要使用其他类型，请将 discriminatorType 设置为 DiscriminatorType.CHAR或 DiscriminatorType.INTEGER 。您的 [@DiscriminatorValue](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#DiscriminatorValue)必须符合此类型。|
|length||默认值： 31默认情况下，JPA 持续性提供程序假设标识符列在用于保存 String 值时的最大长度为255 个字符。如果该列不适合于您的应用程序或数据库，请将 length 设置为适合于您的数据库列的int 值。您的 [@DiscriminatorValue](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#DiscriminatorValue)必须符合此长度。|
|name||默认值：JPA 持续性提供程序假设标识符列名为“ DTYPE ” 。要指定其他列名，请将 name 设置为所需的 String 列名。|
[示例 1-12](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDFJAHI)显示了如何使用此批注指定一个名为 DISC 、类型为 STRING 、长度为 20 的标识符列。在本示例中，该类的 [@DiscriminatorValue](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#DiscriminatorValue)指定为 CUST 。[示例
 1-13](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDEFCAC)中的子类将它自己的 [@DiscriminatorValue](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#DiscriminatorValue)指定为VIP 。在 Customer 和 ValuedCustomer 中，[@DiscriminatorValue](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#DiscriminatorValue)的值必须可以转换为由[@DiscriminatorColumn](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#DiscriminatorColumn)属性 discriminatorType 指定的类型，并且必须符合 [@DiscriminatorColumn](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#DiscriminatorColumn)属性length 。
示例 1-12 @DiscriminatorColumn 和 @DiscriminatorValue — 根类
@Entity
@Table(name="CUST")
@Inheritance(strategy=SINGLE_TABLE)
@DiscriminatorColumn(name="DISC", discriminatorType=STRING, length=20)
@DiscriminatorValue(value-"CUST")
public class Customer {
    ... 
}
示例 1-13 @DiscriminatorValue — 子类
@Entity
@DiscriminatorValue(value="VIP")
public class ValuedCustomer extends Customer { 
    ... 
}
**@DiscriminatorValue**
默认情况下，当 [@Inheritance](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#Inheritance)属性策略为 InheritanceType.SINGLE_TABLE 或 JOINED 时，JPA 持续性提供程序使用 [@DiscriminatorColumn](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#DiscriminatorColumn)按实体名称区分继承层次中的类（请参阅 [@Entity](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#Entity)）。
使用 @DiscriminatorValue 批注指定用于区分此继承层次中的实体的标识符值：
- 如果实体名称不适合于此应用程序
- 匹配现有的数据库模式
[表 1-10](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDDIEHH)列出了此批注的属性。有关更多详细信息，请参阅 [API](http://java.sun.com/javaee/5/docs/api/javax/persistence/DiscriminatorValue.html)。
表 1-10 @DiscriminatorValue 属性
|**属性**|**必需**|**说明**|
|----|----|----|
|value||将 value 设置为符合 [@DiscriminatorColumn](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#DiscriminatorColumn)属性 discriminatorType 和 length 的标识符值的String 等效形式。|
[示例 1-14](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDIDEIC)显示了如何使用此批注指定一个名为 DISC 、类型为 STRING 、长度为 20 的标识符列。在本示例中，该类的 [@DiscriminatorValue](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#DiscriminatorValue)指定为 CUST 。[示例
 1-15](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDJICCD)中的子类将它自己的 [@DiscriminatorValue](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#DiscriminatorValue)指定为VIP 。在 Customer 和 ValuedCustomer 中，[@DiscriminatorValue](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#DiscriminatorValue)的值必须可以转换为由[@DiscriminatorColumn](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#DiscriminatorColumn)属性 discriminatorType 指定的类型，并且必须符合 [@DiscriminatorColumn](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#DiscriminatorColumn)属性length 。
示例 1-14 @DiscriminatorColumn 和 @DiscriminatorValue — 根类
@Entity
@Table(name="CUST")
@Inheritance(strategy=SINGLE_TABLE)
@DiscriminatorColumn(name="DISC", discriminatorType=STRING, length=20)
@DiscriminatorValue(value-"CUST")
public class Customer {
    ... 
}
示例 1-15 @DiscriminatorValue — 子类
@Entity
@DiscriminatorValue(value="VIP")
public class ValuedCustomer extends Customer { 
    ... 
}
**@Embeddable**
默认情况下，JPA 持续性提供程序假设每个实体均持久保存到它自己的数据库表。
使用 @Embeddable 批注指定一个类，该类的实例存储为拥有实体的固有部分并共享该实体的身份。嵌入对象的每个持久属性或字段都将映射到实体的数据库表。
此批注没有属性。有关更多详细信息，请参阅 [API](http://java.sun.com/javaee/5/docs/api/javax/persistence/Embeddable.html)。
[示例 1-16](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDIDJHG)显示了如何使用此批注指定：类 EmploymentPeriod 在用作批注为 [@Embedded](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#Embedded)的持久字段的类型时可以嵌套到实体中（请参阅[示例
 1-17](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDIFFDE)）
示例 1-16 @Embeddable
@Embeddable
public class EmploymentPeriod {
java.util.Date startDate;
java.util.Date endDate;
    ...
}
