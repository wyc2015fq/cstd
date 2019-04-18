# Hibernate的cascade属性  特别是 cascadeType.all的 作用 - z69183787的专栏 - CSDN博客
2014年03月27日 22:31:50[OkidoGreen](https://me.csdn.net/z69183787)阅读数：51017
**1.JPA中的CascadeType.ALL并不等于{CascadeType.PESIST,CascadeType.REMOVE,CascadeType.MERGE,CascadeType.REFRESH}**
在Hibernate中调用session.save() or session.update()并不能触发 {CascadeType.PESIST,CascadeType.REMOVE,CascadeType.MERGE,CascadeType.REFRESH} 的级联操作，而能触发CascadeType.ALL的级联。如不希望用CascadeType.ALL，需要使用Hibernate自身对 cascade的注解
 @Cascade(value=org.hibernate.annotations.CascadeType.SAVE_UPDATE)
**2.共享主键的OneToOne方式不能通过cascade来自动生成标识符** ，即使声明了@GenericGenerator(name="foreign_key",strategy="foreign",parameters={@Parameter(name="property",value="person")})
@EntitypublicclassPersonimplementsSerializable{@Id@GeneratedValue(generator ="g_id")publicString getId(){return id;}@OneToOne(cascade ={CascadeType.PERSIST ,CascadeType.ALL})@JoinColumn(name ="profile_id")@PrimaryKeyJoinColumn(name="id")publicProfile getProfile(){return profile;}}@EntitypublicclassProfileimplementsSerializable{@Id@GenericGenerator(name ="foreign_key", strategy ="foreign", parameters ={@Parameter(name ="property", value ="person")})@GeneratedValue(generator ="foreign_key")publicString getId(){return id;}@OneToOne(mappedBy ="profile")publicPerson getPerson(){return person;}publicvoid setPerson(Person person){this.person = person;}}
**3.delete-orphan只应用在OneToMany中，能级联删除Many集合中的子元素。**
   NOTE:JPA并不支持delete-orphan，但并不明白hibernate reference中为什么可以在OneToMany中添加属性orphanRemoval=true的属性。事实上，要用hibernate独有的注解 @Cascade(value=@CascadeType.DELETE_ORPHAN)来达到此目的。
--------------------------------------------------------------------------------
cascade属性的可能值有     
all: 所有情况下均进行关联操作，即save-update和delete。     
none: 所有情况下均不进行关联操作。这是默认值。      
save-update: 在执行save/update/saveOrUpdate时进行关联操作。      
delete: 在执行delete 时进行关联操作。      
all-delete-orphan: 当一个节点在对象图中成为孤儿节点时，删除该节点。
比如在一个一对多的关系中，Student包含多个book，当在对象关系中删除一个book时，
此book即成为孤儿节点。
