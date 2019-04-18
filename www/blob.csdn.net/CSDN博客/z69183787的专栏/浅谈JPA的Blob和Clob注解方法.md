# 浅谈JPA的Blob和Clob注解方法 - z69183787的专栏 - CSDN博客
2014年04月04日 16:15:41[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5904
**@Lob 注解属性将被持久化为 Blog 或 Clob 类型。**
- Clob（Character Large Ojects）类型是长字符串类型，具体的java.sql.Clob, Character[], char[] 和 java.lang.String 将被持久化为 Clob 类型。
- Blob（Binary Large Objects）类型是字节类型，具体的java.sql.Blob, Byte[], byte[] 和 serializable type 将被持久化为 Blob 类型。
- @Lob 持久化为Blob或者Clob类型,根据get方法的返回值不同,自动进行Clob和Blob的转换。
- 因为这两种类型的数据一般占用的内存空间比较大，所以通常使用延迟加载的方式，与@Basic标记同时使用，设置加载方式为FetchType.LAZY。
例：
@Lob
public String getFullText() {
   return fullText;  // clob type
}
@Lob
public byte[] getFullCode() {
  return fullCode;  // blog type
}
***@Lob 通常与@Basic同时使用，提高访问速度。 例：***
@Lob 
@Basic(fetch = FetchType.LAZY) 
@Column(name=" content", columnDefinition="CLOB", nullable=true) 
public String getContent() {
    return content;
}
***@Basic ***
实体Bean中所有的**非Static 非transient**的属性都可以被持久化，没有定义注解属性的等价于在其上添加了@Basic注解。
通过@Basic注解可以声明属性的获取策略(lazy与否)，默认的是即时获取(early fetch)。
这里又讨论到了延迟关联获取和延迟属性获取,通常不需要对简单属性设置延迟获取，如需要定义@Basic(fetch=FetchType.LAZY)
***MySQL中TinyText，Text，Mediumtext，Longtext的设置***
在MySQL中没有Clob，用Text代替了，而且分为了tinytext， text，mediumtext，longtext。Blob也按这种方式分成了四种。
那如果使用hbm2ddl的方式自动生成table时，怎么通过注解分别配置这几种类型呢？默认都是生成longtext和longblob类型。
跟Oracle等通用的方法目前还没有找到，经过测试，可以通过@Column(columnDefinition = "mediumtext")这样的方式实现。
如果自己写sql脚本生成表，然后通过JPA来映射的话，以上四种类型的数据库字段在不配置columnDefinition时都可以正常工作。
