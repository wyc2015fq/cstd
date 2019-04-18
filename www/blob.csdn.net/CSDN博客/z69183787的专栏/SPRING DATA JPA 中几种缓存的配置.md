# SPRING DATA JPA 中几种缓存的配置 - z69183787的专栏 - CSDN博客
2015年01月07日 07:07:49[OkidoGreen](https://me.csdn.net/z69183787)阅读数：7251
在用spring data jpa的过程中，采用了ehcache 来做缓存, 是否需要二级缓存，一般不需要，这得看业务的需要，因为这东西如果配置不好，反而会导致性能下降，但如果是有些数据，基本不改动，长期不变，很少修改，而且数据量适中，并且外部使用频率高的情况下，还是可以使用的。就目前研究spring data jpa 来说，有三种情况下有可能会使用到缓存
1. spring data jpa 自身的方法
2. 扩展spring data jpa 的方法
3.完全自己用EntityManager 写语句的缓存.
其实现方式，基本如下，用代码表示
publicinterfaceUserDaoextendsCrudRepository,UserCustomer{/**
     * 第一种情况的缓存,处理spring data jpa自身的方法
     * @param id
     * @return
     */@QueryHints({@QueryHint(name ="org.hibernate.cacheable", value ="true")})publicUser findById(long id);/**
     * 第二种情况缓存,处理能根据 spring data jpa 扩展的方法.
     * @return
     */@Query("from User")@QueryHints({@QueryHint(name ="org.hibernate.cacheable", value ="true")})List findAllCached();}
如果是自己写语句的缓存，用如下方式
publicclassUserDaoImplimplementsUserCustomer{@PersistenceContextprivateEntityManager em;/**
     * 第三种缓存,采用 setHint 的方式处理.
     */publicUser findByCustomizeId(){return(User) em.createQuery("from User p where p.id=1").setHint("org.hibernate.cacheable",true).getSingleResult();}
当然在实体类上也要注明，使用缓存
@Cache(usage =CacheConcurrencyStrategy.READ_ONLY,region="mycache")@Cacheable(true)@Entity@Table(name="XUSER")publicclassUser{...........}
至于ehcache 的配置很简单的几行xml 配置，比如
在这几种情况下，测试，一旦缓存之后，是看不到发出sql语句的，说明确实使用了缓存.
