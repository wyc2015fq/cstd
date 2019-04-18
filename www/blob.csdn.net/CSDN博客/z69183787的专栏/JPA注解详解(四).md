# JPA注解详解(四) - z69183787的专栏 - CSDN博客
2014年04月02日 09:35:35[OkidoGreen](https://me.csdn.net/z69183787)阅读数：698
**@Embedded**
默认情况下，JPA 持续性提供程序假设每个实体均持久保存到它自己的数据库表。
使用 @Embedded 批注指定一个持久字段，该字段的 [@Embeddable](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#Embeddable)类型可以存储为拥有实体的固有部分，并共享该实体的身份。嵌入对象的每个持久属性或字段均映射到拥有实体的数据库表。
可以结合使用 @Embedded 和 [@Embeddable](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#Embeddable)以建立严格所有权关系的模型，以便在删除了拥有对象的情况下还将删除被拥有的对象。
嵌入的对象不应映射到多个表。
默认情况下，[@Embeddable](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDIDJHG)类中指定的列定义（请参阅 [@Column](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDCEHDA)）适用于 @Embedded 类。如果要覆盖这些列定义，请使用 [@AttributeOverride](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#AttributeOverride)。
此批注没有属性。有关更多详细信息，请参阅 [API](http://java.sun.com/javaee/5/docs/api/javax/persistence/Embedded.html)。
[示例 1-17](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDIFFDE)显示了如何使用该批注指定：[@Embeddable](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#Embeddable)类 EmploymentPeriod （请参阅[示例
 1-16](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDIDJHG)）可以使用指定的属性覆盖（请参阅 [@AttributeOverride](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#AttributeOverride)）嵌入到实体类中。如果不需要属性覆盖，则可以完全忽略@Embedded 批注：JPA 持续性提供程序将推断出 EmploymentPeriod 是从它的 [@Embeddable](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#Embeddable)批注进行嵌套。
示例 1-17 @Embedded
@Entity
public class Employee implements Serializable {
    ...
@Embedded
@AttributeOverrides({
@AttributeOverride(name="startDate", column=@Column("EMP_START")),
@AttributeOverride(name="endDate", column=@Column("EMP_END"))
    )
public EmploymentPeriod getEmploymentPeriod() { 
        ... 
    }
    ...
}
**@EmbeddedId**
使用 @EmbeddedId 批注指定一个由实体拥有的可嵌入复合主键类（通常由两个或更多基元类型或 JDK 对象类型组成）。从原有数据库映射时（此时数据库键由多列组成），通常将出现复合主键。
复合主键类具有下列特征：
- 它是一个普通的旧式 Java 对象 (POJO) 类。
- 它必须为 public ，并且必须有一个 public 无参数构造函数。
- 如果使用基于属性的访问，则主键类的属性必须为 public 或 protected 。
- 它必须是可序列化的。
- 它必须定义 equals 和 hashCode 方法。
这些方法的值相等性的语义必须与键映射到的数据库类型的数据库相等性一致。
或者，您可以使复合主键类成为非嵌入类（请参阅 [@IdClass](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#IdClass)）。
此批注没有属性。有关更多详细信息，请参阅 [API](http://java.sun.com/javaee/5/docs/api/javax/persistence/EmbeddedId.html)。
[示例 1-18](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CEGCJDJF)显示了一个批注为 [@Embeddable](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#Embeddable)的典型复合主键类。[示例1-19](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CEGBCBEF)显示了如何使用可嵌入的复合主键类（使用 @EmbeddedId 批注）配置一个实体。
示例 1-18 可嵌入复合主键类
@Embeddable
public class EmployeePK implements Serializable
{
private String name;
private long id;
public EmployeePK()
    {
    }
public String getName()
    {
return name;
    }
public void setName(String name)
    {
this.name = name;
    }
public long getId()
    {
return id;
    }
public void setId(long id)
    {
this.id = id;
    }
public int hashCode()
    {
return (int) name.hashCode() + id;
    }
public boolean equals(Object obj)
    {
if (obj == this) return true;
if (!(obj instanceof EmployeePK)) return false;
if (obj == null) return false;
EmployeePK pk = (EmployeePK) obj;
return pk.id == id && pk.name.equals(name);
    }
}
示例 1-19 @EmbeddedId
@Entity
public class Employee implements Serializable
{
EmployeePK primaryKey;
public Employee()
    {
    }
@EmbeddedId
public EmployeePK getPrimaryKey()
    {
return primaryKey;
    }
public void setPrimaryKey(EmployeePK pk)
     {
primaryKey = pk;
    }
    ...
}
**@Entity**
使用 @Entity 批注将普通的旧式 Java 对象 (POJO) 类指定为实体，并使其可用于 JPA 服务。必须将 POJO 类指定为实体，然后才可以使用任何其他 JPA 批注。
[表 1-11](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDJBGAB)列出了此批注的属性。有关更多详细信息，请参阅 [API](http://java.sun.com/javaee/5/docs/api/javax/persistence/Entity.html)。
表 1-11 @Entity 属性
|**属性**|**必需**|**说明**|
|----|----|----|
|name||默认值：JPA 持续性提供程序假设实体名称是实体类的名称。在[示例 1-20](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDHIDBI)中，默认 name 为“Employee ” 。如果实体类名难于处理、是一个保留字、与事先存在的数据模型不兼容或作为数据库中的表名无效，请将 name 设置为其他 String 值。|
[示例 1-20](http://www.oracle.com/technology/global/cn/products/ias/toplink/jpa/resources/toplink-jpa-annotations.html#CHDHIDBI)显示了该批注的用法。
示例 1-20 @Entity
@Entity
public class Employee implements Serializable {
    ...
}
