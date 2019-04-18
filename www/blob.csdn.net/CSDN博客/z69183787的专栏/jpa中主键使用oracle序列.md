# jpa中主键使用oracle序列 - z69183787的专栏 - CSDN博客
2013年12月12日 15:18:24[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4893
实体类配置
@Id
 @GeneratedValue(strategy=GenerationType.SEQUENCE,generator="mseq")
 @SequenceGenerator(name="mseq",sequenceName="metadata_seq",allocationSize=1)
 @Column(name = "ENTID", unique = true, nullable = false, precision = 22, scale = 0)
 public long getEntid() {
  return this.entid;
 }
sequenceName 是自己在oracle中创建的序列。allocationSize要指定为1，否则它会按照默认50个数字增长。
