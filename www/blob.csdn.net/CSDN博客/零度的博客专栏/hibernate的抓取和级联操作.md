# hibernate的抓取和级联操作 - 零度的博客专栏 - CSDN博客
2016年04月29日 17:22:06[零度anngle](https://me.csdn.net/zmx729618)阅读数：577
```
@Entity
@Table(name="orders")
 public class Order {
  //1-m,多的一方为关系维护端，关系维护端负责外键记录的更新，关系被维护端没有权利更新外键记录
 private String orderid;
  private Float amount;
  private Set<OrderItem> items = new HashSet<OrderItem>();
  
  @Id @Column(length=12)
  public String getOrderid() {
   return orderid;
  }
  public void setOrderid(String orderid) {
   this.orderid = orderid;
  }
  @Column(nullable=false)
  public Float getAmount() {
   return amount;
  }
  public void setAmount(Float amount) {
   this.amount = amount;
  }
  @OneToMany(cascade={CascadeType.REFRESH,CascadeType.PERSIST,CascadeType.REMOVE,CascadeType.MERGE},fetch=FetchType.LAZY,mappedBy="order")//出现mapby为被维护端|||默认为延迟加载
 public Set<OrderItem> getItems() {
   return items;
  }
  public void setItems(Set<OrderItem> items) {
   this.items = items;
  }
  public void addOrderItem(OrderItem orderItem){
   orderItem.setOrder(this);
   this.items.add(orderItem);
  }
}
@Entity
 public class OrderItem {
  private Integer id;
  private String productName;
  private Float sellPrice = 0f;
  private Order order;
  
  @Id @GeneratedValue
  public Integer getId() {
   return id;
  }
  public void setId(Integer id) {
   this.id = id;
  }
  @Column(length=40,nullable=false)
  public String getProductName() {
   return productName;
  }
  public void setProductName(String productName) {
   this.productName = productName;
  }
  @Column(nullable=false)
  public Float getSellPrice() {
   return sellPrice;
  }
  public void setSellPrice(Float sellPrice) {
   this.sellPrice = sellPrice;
  }
  @ManyToOne(cascade={CascadeType.MERGE,CascadeType.REFRESH},optional=true)
  @JoinColumn(name="order_id")
  public Order getOrder() {
   return order;
  }
  public void setOrder(Order order) {
   this.order = order;
  }
  
  
}
```
CascadeType.REFRESH：级联刷新，当多个用户同时作操作一个实体，为了用户取到的数据是实时的，在用实体中的数据之前就可以调用一下refresh()方法！
CascadeType.REMOVE：级联删除，当调用remove()方法删除Order实体时会先级联删除OrderItem的相关数据！
CascadeType.MERGE：级联更新，当调用了Merge()方法，如果Order中的数据改变了会相应的更新OrderItem中的数据，
CascadeType.ALL：包含以上所有级联属性。
（注：以上几种级联操作，只能实在满足数据库的约束时才能生效，比如上边的Order和OrderItem存在主外键关联所以执行REMOVE()方法时是不能实现级联删除的）
CascadeType.PERSIST：级联保存，当调用了Persist() 
 方法，会级联保存相应的数据
