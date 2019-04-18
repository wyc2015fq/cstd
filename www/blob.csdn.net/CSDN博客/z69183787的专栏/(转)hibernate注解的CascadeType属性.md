# (转)hibernate注解的CascadeType属性 - z69183787的专栏 - CSDN博客
2014年03月27日 22:25:30[OkidoGreen](https://me.csdn.net/z69183787)阅读数：12913
cascade表示级联操作   
   CascadeType.MERGE级联更新：若items属性修改了那么order对象保存时同时修改items里的对象。对应EntityManager的merge方法 
   CascadeType.PERSIST级联刷新：获取order对象里也同时也重新获取最新的items时的对象。对应EntityManager的refresh(object)方法有效。即会重新查询数据库里的最新数据   
  CascadeType.REFRESH级联保存：对order对象保存时也对items里的对象也会保存。对应EntityManager的presist方法   
  CascadeType.REMOVE级联删除：对order对象删除也对items里的对象也会删除。对应EntityManager的remove方法
CascadeType.PERSIST只有A类新增时，会级联B对象新增。若B对象在数据库存（跟新）在则抛异常（让B变为持久态）
CascadeType.MERGE指A类新增或者变化，会级联B对象（新增或者变化）
CascadeType.REMOVE只有A类删除时，会级联删除B类；
CascadeType.ALL包含所有；
CascadeType.REFRESH没用过。
综上：大多数情况用CascadeType.MERGE就能达到级联跟新又不报错，用CascadeType.ALL时要斟酌下CascadeType.REMOVE
@Fetch:
定义了加载关联关系的获取策略. FetchMode 可以是
SELECT (在需要加载关联的时候触发select操作), SUBSELECT(只对集合有效,使用了子查询策略,详情参考Hibernate参考文档)
JOIN (在加载主实体(owner entity)的时候使用SQL JOIN来加载关联关系).
JOIN 将覆写任何延迟属性 (通过 JOIN策略加载的关联将不再具有延迟性).
