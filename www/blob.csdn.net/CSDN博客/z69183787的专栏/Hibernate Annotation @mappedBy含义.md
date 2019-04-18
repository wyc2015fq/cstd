# Hibernate Annotation @mappedBy含义 - z69183787的专栏 - CSDN博客
2014年01月21日 16:40:57[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1921
#### 原帖请见：[http://revolver77.spaces.live.com/blog/cns!583C0A9A80FC1137!209.entry](http://revolver77.spaces.live.com/blog/cns!583C0A9A80FC1137!209.entry)
#### EJB3使用心得(1)-mappedBy的含义
相关示例：[http://blog.csdn.net/z69183787/article/details/8197106](http://blog.csdn.net/z69183787/article/details/8197106)
**1、关于mappedBy的含义**：
a) 只有OneToOne,OneToMany,ManyToMany上才有mappedBy属性，ManyToOne不存在该属性；
b) mappedBy标签一定是定义在the owned side(被拥有方的)，他指向the owning side(拥有方)；
c) mappedBy的含义，应该理解为，拥有方能够自动维护跟被拥有方的关系；
    当然，如果从被拥有方，通过手工强行来维护拥有方的关系也是可以做到的。
    例如：
Person.java 
@OneToOne(optional = true, cascade = CascadeType.ALL, mappedBy = "person")
 public IDCard getIdcard() {
  return idcard;
 }
IDCard.java
  @OneToOne(optional = false, cascade = CascadeType.ALL)
  @JoinColumn(name = "Person_ID", referencedColumnName = "personid",unique= true)
  public Person getPerson() {
  return person;
  }
OneToOneDAOBean.java
/*上面例子中IDCard为拥有方，这里强行使用Person作为主控方,手工维护两个实体的关系*/
    public void insertPerson(String name, boolean sex,short age, Date birthday,String cardID) {
        Person person = new Person();
        person.setName(name);
        person.setSex(sex);
        person.setAge(Short.valueOf(age));
        person.setBirthday(birthday);
        IDCard idcard = new IDCard(cardID);
        idcard.setPerson(person);
        person.setIdcard(idcard);
        em.persist(person);        
    }
/*其实既然IDCard为拥有方，那么将IDCard视为主控方的话，他自动维护了两个实体的关系*/
    public void insertPerson1(String name, boolean sex,short age, Date birthday,String cardID) {
        Person person = new Person();
        person.setName(name);
        person.setSex(sex);
        person.setAge(Short.valueOf(age));
        person.setBirthday(birthday);
        IDCard idcard = new IDCard(cardID);
        idcard.setPerson(person);
        //person.setIdcard(idcard);
        //em.persist(person);
        em.persist(idcard);
    }
d) mappedBy跟JoinColumn/JoinTable总是处于互斥的一方，可以理解为正是由于拥有方的关联被拥有方的字段存在，拥有方才拥有了被拥有方。mappedBy这方定义的JoinColumn/JoinTable总是失效的，不会建立对应的字段或者表；
e) 标准的关系映射，定义在ejb-3_0-fr-spec-persistence.pdf 的2.1.8 Relationship Mapping Defaults 章节中：
The following rules apply to bidirectional relationships:
• The inverse side of a bidirectional relationship must refer to its owning side by use of the
mappedBy element of the OneToOne, OneToMany, or ManyToMany annotation. The
mappedBy element designates the property or field in the entity that is the owner of the relationship.
• The many side of one-to-many / many-to-one bidirectional relationships must be the owning
side, hence the mappedBy element cannot be specified on the ManyToOne annotation.
• For one-to-one bidirectional relationships, the owning side corresponds to the side that contains
the corresponding foreign key.
• For many-to-many bidirectional relationships either side may be the owning side.
