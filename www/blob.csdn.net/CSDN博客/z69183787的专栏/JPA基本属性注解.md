# JPA基本属性注解 - z69183787的专栏 - CSDN博客
2014年04月02日 09:31:04[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1440
JPA(Java Persistence API)是Sun官方提出的Java持久化规范。它为Java开发人员提供了一种对象/关系映射工具来管理Java应用中的关系数据
JPA规范要求在类路径的META-INF目录下放置persistence.xml
JPA 中将一个类注解成实体类(entity class)有两种不同的注解方式：基于属性(property-based)和基于字段(field-based)的注解
基于字段的注解, 就是直接将注解放置在实体类的字段的前面
基于属性的注解, 就是直接将注解放置在实体类相应的getter方法前面(这一点和Spring正好相反),但是同一个实体类中必须并且只能使用其中一种注解方式
Entity、Table、Id、GeneratedValue、Basic、Column、Temporal、Transient、Lob、Transient、SecondaryTable、Embeddable、Embedded
JPA注解
(1)Entity
@javax.persistence.Entity(name="xxx") 
name指定实体Bean的名称,默认值为 bean class 的非限定类名,select o from xxx o where o.id=?1
(2)Table
@javax.persistence.Table(catalog="xx",name="xx",schema="xx",uniqueConstraints={ @UniqueConstraint(columnNames={"xx","xx"})})
name:指定表的名称
catalog:指定数据库名称
schema:指定数据库的用户名
uniqueConstraints:指定唯一性字段约束,如为personid 和name 字段指定唯一性约束
uniqueConstraints={ @UniqueConstraint(columnNames={"personid", "name"})}
(3)Id
@javax.persistence.Id()
映射到数据库表的主键的属性,一个实体只能有一个属性被映射为主键.
(4)GeneratedValue
@javax.persistence.GeneratedValue(generator="xxx",strategy=GenerationType.AUTO)
strategy:表示主键生成策略,有AUTO,INDENTITY,SEQUENCE 和 TABLE 4种
分别表示让ORM框架自动选择,根据数据库的Identity字段生成,根据数据库表的Sequence字段生成,以有根据一个额外的表生成主键,默认为AUTO 
generator:表示主键生成器的名称,这个属性通常和ORM框架相关,例如,Hibernate可以指定uuid等主键生成方式. 
Hibernate UUID
@Id @GeneratedValue(generator="system-uuid")
@GenericGenerator(name="system-uuid",strategy = "uuid")
(5)Basic
@javax.persistence.Basic(fetch=FetchType.LAZY,optional=true)
fetch:抓取策略,延时加载与立即加载
optional:指定在生成数据库结构时字段是否允许为 null
(6)Column
@javax.persistence.Column(length=15,nullable=false,columnDefinition="",insertable=true,scale=10,table="",updatable=true)
@Column注解指定字段的详细定义
name:字段的名称,默认与属性名称一致 
nullable:是否允许为null,默认为true
unique:是否唯一,默认为false 
length:字段的长度,仅对String类型的字段有效 
columnDefinition:表示该字段在数据库中的实际类型
通常ORM框架可以根据属性类型自动判断数据库中字段的类型,
但是对于Date类型仍无法确定数据库中字段类型究竟是DATE,TIME还是TIMESTAMP,
此外,String的默认映射类型为VARCHAR,如果要将String类型映射到特定数据库的BLOB或TEXT字段类型,该属性非常有用
如: @Column(name="BIRTH",nullable="false",columnDefinition="DATE") 
insertable:默认情况下,JPA持续性提供程序假设所有列始终包含在 SQL INSERT 语句中。
如果该列不应包含在这些语句中，请将 insertable 设置为 false 
updatable：列始终包含在 SQL UPDATE 语句中。如果该列不应包含在这些语句中，请将 updatable 设置为 false 
table:实体的所有持久字段都存储到一个其名称为实体名称的数据库表中,如果该列与 @SecondaryTable表关联
需将 name 设置为相应辅助表名称的String名称
(7)Temporal
@javax.persistence.Temporal(TemporalType.DATE)
value:TemporalType.DATE,TemporalType.TIME,TemporalType.TIMESTAMP时间类型格式
(8)Enumerated
@javax.persistence.Enumerated(EnumType.STRING)
value:EnumType.STRING,EnumType.ORDINAL
枚举类型成员属性映射,EnumType.STRING指定属性映射为字符串,EnumType.ORDINAL指定属性映射为数据序
(9)Lob
@javax.persistence.Lob
用于标注字段类型为Clob和Blob类型
Clob(Character Large Ojects)类型是长字符串类型,实体的类型可为char[]、Character[]、或者String类型
Blob(Binary Large Objects)类型是字节类型,实体的类型可为byte[]、Byte[]、或者实现了Serializable接口的类。
通常使用惰性加载的方式, @Basic(fetch=FetchType.LAZY)
(10)Transient
@javax.persistence.Transient
@Transient表示该属性并非一个到数据库表的字段的映射,ORM框架将忽略该属性
(11)SecondaryTable 
@javax.persistence.SecondaryTable
将一个实体映射到多个数据库表中
如：
@Entity
@SecondaryTables({ 
@SecondaryTable(name = "Address"), 
    @SecondaryTable(name = "Comments") 
})
public class Forum implements Serializable {
@Column(table = "Address", length = 100) 
private String street; 
@Column(table = "Address", nullable = false) 
private String city; 
@Column(table = "Address") 
private String conutry; 
@Column(table = "Comments") 
private String title; 
@Column(table = "Comments") 
private String Comments; 
@Column(table = "Comments") 
}
table属性的值指定字段存储的表名称
没有用 @Column 注解改变属性默认的字段将会存在于 Forum 表
(12)@Embeddable
@javax.persistence.Embeddable
嵌套映射,在被嵌套的类中使用Embeddable注解,说明这个就是一个可被嵌套的类,使用 @Embedded
当同一个类被不同的注解方式的类嵌套时，可能会出现一些错误，使用 @Access(AccessType. FIELD)设定被嵌套类的注解方式 
================================================================================================
(1)
@Entity注解定义
@Target(TYPE) @Retention(RUNTIME)
public @interface Entity{
    String name() default "";//实体bean的名称
}
(2)
@Table注解定义
@Target(value = {ElementType.TYPE}) 
@Retention(value = RetentionPolicy.RUNTIME) 
public @interface Table { 
public String name() default ""; //表的名称
public String catalog() default ""; //数据库名称
public String schema() default ""; //数据库用户名
public UniqueConstraint[] uniqueConstraints() default {}; //指定多个字段唯一性约束 
}
(3)
@UniqueConstraint注解定义
public @interface UniqueConstraint{
    String[] columnNames( ); //唯一字段属性名称
}
(4)
@Id注解定义
@Target({METHOD, FIELD}) @Retention(RUNTIME) 
public @interface Id{ }
(5)
@注解GeneratedValue定义
@Target({METHOD, FIELD}) @Retention(RUNTIME)
public @interface GeneratedValue{
    GenerationType strategy() default AUTO; //主键生成策略
    String generator() default "";
}
(6)
@Column注解定义
@Target(value = {ElementType.METHOD, ElementType.FIELD}) 
@Retention(value = RetentionPolicy.RUNTIME) 
public @interface Column { 
public String name() default ""; //数据库中的列名
public boolean unique() default false; //该列是否唯一
public boolean nullable() default true; //是否可以为空
public boolean insertable() default true; 
public boolean updatable() default true; 
public String columnDefinition() default ""; 
public String table() default ""; 
public int length() default 255; //该列的最大长度
public int precision() default 0; 
public int scale() default 0; 
}
(7)
@Temporal注解定义
public enum TemporalType{
    DATE, //代表 date类型 java.sql.Date 2008-08-08 
    TIME, //代表时间类型  java.sql.Time 20:00:00
    TIMESTAMP //代表时间 java.sql.Timestamp 2008-08-08 20:00:00.000000001
}
public enum TemporalType{
    DATE, //代表 date类型 //java.sql.Date 2008-08-08 
    TIME, //代表时间类型   //java.sql.Time 20:00:00
    TIMESTAMP //代表时间 //java.sql.Timestamp 2008-08-08 20:00:00.000000001
}
