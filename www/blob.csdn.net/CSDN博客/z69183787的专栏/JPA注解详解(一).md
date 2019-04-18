# JPA注解详解(一) - z69183787的专栏 - CSDN博客
2014年04月02日 09:34:34[OkidoGreen](https://me.csdn.net/z69183787)阅读数：867
**@Table**
默认情况下，JPA 持续性提供程序假设实体的所有持久字段均存储到一个名称为实体名称的数据库表中（请参阅[@Entity](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#Entity)）。
在以下条件下，使用 @Table 批注指定与实体关联的主表：
·    实体名称难于处理、是一个保留字、与预先存在的数据模型不兼容或作为数据库中的表名无效
·    需要控制表所属的目录或模式
如果希望 JPA 将某些字段持久保存到主表，而将其他字段持久保存到一个或多个辅助表，请参阅[@SecondaryTable](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#SecondaryTable)。
[表 1-45](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDGEAJB)列出了此批注的属性。有关更多详细信息，请参阅 [API](http://java.sun.com/javaee/5/docs/api/javax/persistence/Table.html)。
表 1-45 @Table 属性
|**属性**|**必需**|**说明**|
|----|----|----|
|catalog||默认值：JPA 持续性提供程序使用任何适用于数据库的默认目录。如果默认目录不适合于应用程序，请将 catalog 设置为要使用的 String 目录名。|
|name||默认值：JPA 持续性提供程序假设实体的数据库表与实体类同名。在[示例 1-86](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDCGHGA)中，默认 name 为 Employee 。如果实体类名难以处理、是一个保留字或与预先存在的数据模型不兼容，请将 name设置为相应的数据表名称。在[示例 1-86](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDCGHGA)中，JPA 将实体类 Employee 持久保存到名为 EMP 的数据库表中。|
|schema||默认值：JPA 持续性提供程序使用任何适用于数据库的默认模式。如果默认模式不适合于应用程序，请将 schema 设置为要使用的 String 模式名。|
|uniqueConstraints||默认值：JPA 持续性提供程序假设实体的数据库表中的任何列均没有唯一约束。如果唯一约束应用于该表中的一列或多列，请将 uniqueContraints 设置为一个或多个UniqueConstraint 实例的数组。有关详细信息，请参阅 [@UniqueConstraint](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#UniqueConstraint)。|
[示例 1-86](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDCGHGA)显示了如何使用此批注指定主表名。
示例 1-86 @Table
@Entity
@Table(name="EMP")
public class Employee implements Serializable {
    ...
}
**@TableGenerator**
如果使用 [@GeneratedValue](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#GeneratedValue)批注指定一个 TABLE 类型的主键生成器，可以使用 @TableGenerator 批注微调该主键生成器以：
·    由于名称难于处理、是一个保留字、与预先存在的数据模型不兼容或作为数据库中的表名无效而更改主键生成器的表名称
·    更改分配大小以匹配应用程序要求或数据库性能参数
·    更改初始值以匹配现有的数据模型（例如，如果基于已经为其分配或保留了一组主键值的现有数据集构建）
·    使用特定目录或模式配置主键生成器的表
·    在主键生成器表的一列或多列商配置一个唯一的约束
[表 1-46](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDJDBGJ)列出了此批注的属性。有关更多详细信息，请参阅 [API](http://java.sun.com/javaee/5/docs/api/javax/persistence/TableGenerator.html)。
表 1-46 @TableGenerator 属性
|**属性**|**必需**|**说明**|
|----|----|----|
|name||SequenceGenerator 的名称必须匹配其 startegy 设置为 startegy 的GeneratedValue 的名称。生成器名称的作用域对持续性单元是全局的（跨所有生成器类型）。|
|allocationSize||默认值： 50.默认情况下，JPA 持续性提供程序使用的分配大小为 50 。如果此分配大小与应用程序要求或数据库性能参数不匹配，请将 allocationSize 设置为所需的 int 值。|
|catalog||默认值：JPA 持续性提供程序使用任何适用于数据库的默认目录。如果默认目录不适合于应用程序，请将 catalog 设置为要使用的 String 目录名。|
|initialValue||默认值： 0.默认情况下，JPA 持续性提供程序将所有主键值的起始值设置为 0 。如果这与现有数据模型不匹配，请将 initialValue 设置为所需的 int 值。|
|pkColumnName||默认值：JPA 持续性提供程序为生成器表中的主键列提供名称。如果该名称不适合于应用程序，请将 pkColumnName 设置为所需的 String 名称。|
|pkColumnValue||默认值：JPA 持续性提供程序为生成器表中的主键列提供一个合适的主键值。如果该值不适合于应用程序，请将 pkColumnValue 设置为所需的 String 值。|
|schema||默认值：JPA 持续性提供程序使用任何适用于数据库的默认模式。如果默认模式不适合于应用程序，请将 schema 设置为要使用的 String 模式名。|
|table||默认值：JPA 持续性提供程序为存储生成的 ID 值的表提供了一个合适的名称。如果默认表名不适合于应用程序，请将 table 设置为所需的 String 表名。|
|uniqueConstraints||默认值：JPA 持续性提供程序假设主键生成器表中的任何列均没有唯一约束。如果唯一约束应用于该表中的一列或多列，则将 uniqueContraints 设置为一个或多个 UniqueConstraint 实例的数组。有关详细信息，请参阅 [@UniqueConstraint](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#UniqueConstraint)。|
|valueColumnName||默认值：JPA 持续性提供程序为存储生成的 ID 值的列提供了一个合适的名称。如果默认列名不适合于应用程序，请将 valueColumnName 设置为所需的 String 列名。|
[示例 1-87](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDCIBFC)显示了如何使用此批注为名为 empGen 的 TABLE 主键生成器指定分配大小。
示例 1-87 @TableGenerator
@Entity
public class Employee implements Serializable {
    ...
@Id
@TableGenerator(
name="empGen",
allocationSize=1
    )
@GeneratedValue(strategy=TABLE, generator="empGen")
@Column(name="CUST_ID")
public Long getId() {
return id;
    }
    ...
**@Temporal**
使用 @Temporal 批注指定 JPA 持续性提供程序应只为 java.util.Date 和 java.util.Calendar 类型的字段或属性持久保存的数据库类型。
该批注可以与 [@Basic](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#Basic)一起使用。
[表 1-14](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDFIAEE)列出了此批注的属性。有关更多详细信息，请参阅 [API](http://java.sun.com/javaee/5/docs/api/javax/persistence/Temporal.html)。
表 1-47 @Temporal 属性
|**属性**|**必需**|**说明**|
|----|----|----|
|value||将 value 设置为与希望 JPA 持续性提供程序使用的数据库类型相对应的 TemporalType ：- DATE - 等于 java.sql.Date- TIME - 等于 java.sql.Time- TIMESTAMP - 等于 java.sql.Timestamp|
[示例 1-88](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDBJJCD)显示了如何使用此批注指定 JPA 持续性提供程序应将 java.util.Date 字段 startDate 持久保存为DATE ( java.sql.Date ) 数据库类型。
示例 1-88 @Temporal
@Entity
public class Employee {
    ...
@Temporal(DATE)    protected java.util.Date startDate;
    ...
}
