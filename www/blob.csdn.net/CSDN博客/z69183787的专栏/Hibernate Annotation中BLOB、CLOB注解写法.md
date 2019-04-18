# Hibernate Annotation中BLOB、CLOB注解写法 - z69183787的专栏 - CSDN博客
2013年11月22日 13:06:34[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2093
在hibernate Annotation中，实体BLOB、CLOB类型的注解与普通的实体属性有些不同，具体操作如下：BLOB类型，类型声明为byte[]：
**//简单类型 默认 fetchType 为 EAGER**
 private byte[] content；
  注解：
  @Lob
  @Basic(fetch = FetchType.LAZY)
  @Column(name = "CONTENT", columnDefinition = "BLOB",nullable=true)
  public byte[] getContent() {
  return this.content;
  }
  public void setContent(byte[] content) {
  this.content = content;
  }
  CLOB类型，类型声明为String即可：
  private String remark；
  注解：
  @Lob
  @Basic(fetch = FetchType.EAGER)
  @Column(name="REMARK", columnDefinition="CLOB", nullable=true)
  public String getRemark() {
  return this.remark;
  }
  public void setRemark(String recvdocRemark) {
  this.remark = remark;
  }
  按照以上的设置实体类的注解就搞定了。
