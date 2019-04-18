# Spring Boot中CrudRepository与JpaRepository Dao中JpaRepository和JpaSpecificationExecutor查询 - weixin_33985507的博客 - CSDN博客
2018年10月18日 18:34:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
原文地址  https://blog.csdn.net/xuemengrui12/article/details/80525227?utm_source=blogxgwz0
 https://www.imooc.com/article/16983
先看下两者的接口代码：
- 
@NoRepositoryBean
- 
publicinterfaceCrudRepository<T, IDextendsSerializable> extendsRepository<T, ID> {
- 
<S extends T> S save(S var1);
- 
<S extends T> Iterable<S> save(Iterable<S> var1);
- 
T findOne(ID var1);
- 
booleanexists(ID var1);
- 
Iterable<T> findAll();
- 
Iterable<T> findAll(Iterable<ID> var1);
- 
longcount();
- 
voiddelete(ID var1);
- 
voiddelete(T var1);
- 
voiddelete(Iterable<? extends T> var1);
- 
voiddeleteAll();
- 
}
- 
@NoRepositoryBean
- 
publicinterfaceJpaRepository<T, IDextendsSerializable> extendsPagingAndSortingRepository<T, ID>, QueryByExampleExecutor<T> {
- 
List<T> findAll();
- 
List<T> findAll(Sort var1);
- 
List<T> findAll(Iterable<ID> var1);
- 
<S extends T> List<S> save(Iterable<S> var1);
- 
voidflush();
- 
<S extends T> S saveAndFlush(S var1);
- 
voiddeleteInBatch(Iterable<T> var1);
- 
voiddeleteAllInBatch();
- 
T getOne(ID var1);
- 
<S extends T> List<S> findAll(Example<S> var1);
- 
<S extends T> List<S> findAll(Example<S> var1, Sort var2);
- 
}
看一下他们的继承关系
![](https://img-blog.csdn.net/20180531153721977)
注意下两者的save方法的不同，JpaRepository 中的save方法实现源码：
- 
@Transactional
- 
public <S extends T> List<S> save(Iterable<S> entities){
- 
        List<S> result = new ArrayList<S>();
- 
if (entities == null) {
- 
return result;
- 
        }
- 
for (S entity : entities) {
- 
            result.add(save(entity));
- 
        }
- 
return result;
- 
    }
CrudRepository 中的save方法源代码
- 
@Transactional
- 
public <S extends T> S save(S entity){
- 
if (entityInformation.isNew(entity)) {
- 
            em.persist(entity);//是新的就插入
- 
return entity;
- 
        } else {
- 
return em.merge(entity); //不是新的merge
- 
        }
- 
    }
由源码可知CrudRepository 中的save方法是相当于merge+save ，它会先判断记录是否存在，如果存在则更新，不存在则插入记录。唉，还是需要多看源码啊
参考：
[https://blog.csdn.net/zgf19930504/article/details/50537222](https://blog.csdn.net/zgf19930504/article/details/50537222)
[https://blog.csdn.net/strive_peter/article/details/76276758](https://blog.csdn.net/strive_peter/article/details/76276758)
[https://blog.csdn.net/hikeboy/article/details/58597053](https://blog.csdn.net/hikeboy/article/details/58597053)
[http://makaidong.com/youhan26/1/976_10156939.html](http://makaidong.com/youhan26/1/976_10156939.html)
一、JpaRepository支持接口规范方法名查询。意思是如果在接口中定义的查询方法符合它的命名规则，就可以不用写实现。使用命名化参数，使用@Param即可，比如：
`@Query(value="select o from UserModel o where o.name like %:nn") public List<UserModel> findByUuidOrAge(@Param("nn") String name);`
二、spring data jpa 通过创建方法名来做查询，只能做简单的查询，那如果我们要做复杂一些的查询呢，多条件分页怎么办，这里，spring data jpa为我们提供了JpaSpecificationExecutor接口，只要简单实现toPredicate方法就可以实现复杂的查询。JpaSpecificationExecutor提供了以下接口
```
public interface JpaSpecificationExecutor<T> {
    T findOne(Specification<T> spec);
    List<T> findAll(Specification<T> spec);
    Page<T> findAll(Specification<T> spec, Pageable pageable);
    List<T> findAll(Specification<T> spec, Sort sort);
    long count(Specification<T> spec);
}
```
