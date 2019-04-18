# hibernate jpa 注解 @Temporal(TemporalType.DATE) 格式化时间日期，页面直接得到格式化类型的值 - z69183787的专栏 - CSDN博客
2013年02月17日 10:35:16[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4262
1.日期：
@Temporal(TemporalType.DATE)
 @Column(name = "applyDate", nullable = false, length = 10)
 public Date getApplyDate() {
  return applyDate;
 }
在页面端取值：2011-04-12
 2.时间：
@Temporal(TemporalType.TIME)
在页面端取值：22:50:30
 3.日期和时间(默认)：
@Temporal(TemporalType.TIMESTAMP) 
在页面端取值：2011-04-12 22:51:34.0
bo.setApplyDate(new Date());
