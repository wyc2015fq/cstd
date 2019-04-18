# JPA注解详解(二) - z69183787的专栏 - CSDN博客
2014年04月02日 09:35:02[OkidoGreen](https://me.csdn.net/z69183787)阅读数：753
**@Transient**
默认情况下，JPA 持续性提供程序假设实体的所有字段均为持久字段。
使用 @Transient 批注指定实体的非持久字段或属性，例如，一个在运行时使用但并非实体状态一部分的字段或属性。
JPA 持续性提供程序不会对批注为 @Transient 的属性或字段持久保存（或创建数据库模式）。
该批注可以与 [@Entity](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#Entity)、[@MappedSuperclass](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#MappedSuperclass)和 [@Embeddable](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#Embeddable)一起使用。
该批注没有属性。有关更多详细信息，请参阅 [API](http://java.sun.com/javaee/5/docs/api/javax/persistence/Transient.html)。
[示例 1-89](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDIHFCF)显示了如何使用此批注将 Employee 字段 currentSession 指定为非持久字段。JPA 持续性提供程序将不持久保存该字段。
示例 1-89 @Transient
@Entitypublic class Employee {    @Id int id;    @Transient Session currentSession;    ...}
**@UniqueConstraint**
默认情况下，JPA 持续性提供程序假设所有列均可以包含重复值。
使用 @UniqueConstraint 批注指定将在为主表或辅助表生成的 DDL 中包含一个唯一约束。或者，您可以在列级别指定唯一约束（请参阅 [@Column](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#Column)）。
[表 1-48](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDFDEEA)列出了此批注的属性。有关更多详细信息，请参阅 [API](http://java.sun.com/javaee/5/docs/api/javax/persistence/UniqueConstraint.html)。
表 1-48 @UniqueConstraint 属性
|**属性**|**必需**|**说明**|
|----|----|----|
|columnNames||如果任何列均包含唯一约束，请将 columnNames 设置为 String 列名的数组。|
[示例 1-90](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDDBCAF)显示了如何使用此批注对主表 EMP 中的列 EMP_ID 和 EMP_NAME 指定一个唯一约束。
示例 1-90 使用唯一约束的 @Table
@Entity
@Table(
name="EMP",
uniqueConstraints={@UniqueConstraint(columnNames={"EMP_ID", "EMP_NAME"})}
)
public class Employee implements Serializable {
    ...
}
**@Version**
默认情况下，JPA 持续性提供程序假设应用程序负责数据一致性。
使用 @Version 批注通过指定用作其乐观锁定值的实体类的版本字段或属性来启用 JPA 管理的乐观锁定（推荐做法）。
选择版本字段或属性时，确保：
·    每个实体只有一个版本字段或属性
·    选择一个持久保存到主表的属性或字段（请参阅 [@Table](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#Table)）
·    您的应用程序不修改版本属性或字段
此批注没有属性。有关更多详细信息，请参阅 [API](http://java.sun.com/javaee/5/docs/api/javax/persistence/Version.html)。
[示例 1-91](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDCIBDJ)显示了如何使用此批注将属性 getVersionNum 指定为乐观锁定值。在该示例中，该属性的列名设置为OPTLOCK （请参阅 [@Column](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#Column)），而非属性的默认列名。
示例 1-91 @Version
@Entity
public class Employee implements Serializable {
    ...
@Version
@Column(name="OPTLOCK")
protected int getVersionNum() {
return versionNum;
    }
    ...
}
**@Column**
默认情况下，JPA 持续性提供程序假设每个实体的持久字段存储在其名称与持久字段的名称相匹配的数据库表列中。
使用 @Column 批注：
- 将持久字段与其他名称关联（如果默认列名难于处理、与事先存在的数据模型不兼容或作为数据库中的列名无效）
- 将持久字段与辅助表中的列关联（请参阅 [@SecondaryTable](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#SecondaryTable)）
- 微调数据库中列的特征
[表 1-7](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDBAHAG)列出了此批注的属性。有关更多详细信息，请参阅 [API](http://java.sun.com/javaee/5/docs/api/javax/persistence/Column.html)。
表 1-7 @Column 属性
|**属性**|**必需**|**说明**|
|----|----|----|
|columnDefinition||默认值：空 String 。默认情况下，JPA 使用最少量 SQL 创建一个数据库表列。如果需要使用更多指定选项创建的列，请将 columnDefinition 设置为在针对列生成DDL 时希望 JPA 使用的 SQL 片断。注意：捕获批注中的 DDL 信息时，某些 JPA 持续性提供程序可以在生成数据库模式时使用此 DDL 。例如，请参阅[“用于 Java2DB 模式生成的 TopLink JPA 扩展”](http://www.oracle.com/technology/products/ias/toplink/jpa/resources/toplink-jpa-extensions.html#Java2DBSchemaGen)。|
|insertable||默认值： true 。默认情况下，JPA 持续性提供程序假设所有列始终包含在 SQL INSERT 语句中。如果该列不应包含在这些语句中，请将 insertable 设置为 false 。|
|length||默认值： 255默认情况下，JPA 持续性提供程序假设所有列在用于保存 String 值时的最大长度为 255个字符。如果该列不适合于您的应用程序或数据库，请将 length 设置为适合于您的数据库列的int 值。|
|name||默认值：JPA 持续性提供程序假设实体的每个持久字段都存储在其名称与持久字段或属性的名称相匹配的数据库表列中。要指定其他列名，请将 name 设置为所需的 String 列名。|
|nullable||默认值： true 。默认情况下，JPA 持续性提供程序假设允许所有列包含空值。如果不允许该列包含空值，请将 nullable 设置为 false 。|
|precision||默认值： 0.默认情况下，JPA 持续性提供程序假设所有列在用于保存十进制（精确数字）值时的精度为 0 。如果该精度不适合于您的应用程序或数据库，请将 precision 设置为相应的 int 精度。|
|scale||默认值： 0.默认情况下，JPA 持续性提供程序假设所有列在用于保存十进制（精确数字）值时的伸缩度为 0 。如果该伸缩度不适合于您的应用程序或数据库，请将 scale 设置为相应的 int 精度。|
|table||默认值：JPA 持续性提供程序假设实体的所有持久字段都存储到一个其名称为实体名称的数据库表中（请参阅 [@Table](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#Table)）。如果该列与辅助表关联（请参阅 [@SecondaryTable](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#SecondaryTable)），请将 name 设置为相应辅助表名称的 String 名称，如[示例 1-8](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDCEHDA)所示。|
|unique||默认值： false 。默认情况下，JPA 持续性提供程序假设允许所有列包含重复值。如果不允许该列包含重复值，请将 unique 设置为 true 。设置为 true 时，这相当于在表级别使用 [@UniqueConstraint](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#UniqueConstraint)。|
|updatable||默认值： true 。默认情况下，JPA 持续性提供程序假设列始终包含在 SQL UPDATE 语句中。如果该列不应包含在这些语句中，请将 updatable 设置为 false 。|
[示例 1-8](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDCEHDA)显示了如何使用此批注使 JPA 将 empId 持久保存到辅助表 EMP_HR 中的列 EMP_NUM 。默认情况下，JPA 将 empName 持久保存到主表 Employee 中的列 empName 。
示例 1-8 @Column
@Entity
@SecondaryTable(name="EMP_HR")
public class Employee implements Serializable {
    ...
@Column(name="EMP_NUM", table="EMP_HR")
private Long empId;
private String empName;
    ...
}
