# Hibernate ORM映射中关clob及blob类型对应bo类 注解写法 - z69183787的专栏 - CSDN博客
2013年04月03日 10:50:35[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2300
**1)、映射日期与时间类型，指定时间精度**
  Annotation：
  用法：@Temporal(参数) 
  说明：参数有3种 只显示时间,只显示日期,时间日期都显示
  举例：
  @Temporal(TemporalType.DATE)  只显示日期
  @Temporal(TemporalType.TIME)  只显示时间
  @Temporal(TemporalType.TIMESTAMP)  显示日期与时间
  如果时间字段上不加@Temporal，则默认是日期与时间类型，数据库会创建（datetime）类型
@Temporal(TemporalType.TIMESTAMP)
@Column(name = "UPDDATE")
public Date getUpddate() {
return upddate;
}
**2)、clob 及 blob 对象**
/*
@Lob 
@Basic(fetch = FetchType.LAZY)  clob blob 一般使用懒加载
@Column(name = "CONTENT", columnDefinition = "BLOB",nullable=true) 
public byte[] getContent() { 
return this.content; 
} 
*/
@Lob
    @Basic(fetch=FetchType.EAGER)
    @Column(name = "ACCIDENT_REMARK",columnDefinition = "CLOB")
    // @Column(name="ISPUBLIC" ,nullable=false,columnDefinition="INT default 0")  默认值
    //@Column(columnDefinition="varchar2(2) default '11'")
/*@Column(name = "ACCIDENT_REMARK", nullable = true, length = 4000)*/
public String getAccidentRemark() {
return accidentRemark;
}

