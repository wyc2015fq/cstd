# SPRING-DATA-JPA 全局DAO配置 - z69183787的专栏 - CSDN博客
2015年01月08日 19:20:03[OkidoGreen](https://me.csdn.net/z69183787)阅读数：11696
配置文件：spring-jpa.xml
```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
	xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:p="http://www.springframework.org/schema/p"
	xmlns:c="http://www.springframework.org/schema/c" xmlns:util="http://www.springframework.org/schema/util"
	xmlns:aop="http://www.springframework.org/schema/aop" xmlns:tx="http://www.springframework.org/schema/tx"
	xmlns:jpa="http://www.springframework.org/schema/data/jpa"
	xsi:schemaLocation="
		http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-3.1.xsd
		http://www.springframework.org/schema/util http://www.springframework.org/schema/util/spring-util-3.1.xsd
		http://www.springframework.org/schema/tx http://www.springframework.org/schema/tx/spring-tx-3.1.xsd
		http://www.springframework.org/schema/data/jpa http://www.springframework.org/schema/data/jpa/spring-jpa.xsd
		http://www.springframework.org/schema/aop http://www.springframework.org/schema/aop/spring-aop-3.1.xsd">
	<!--
		BoneCP教程见http://www.cnblogs.com/zhwl/archive/2013/07/23/3208550.html
	-->
	
	<!-- JPA实体管理器工厂 -->
	<bean id="entityManagerFactory"
		class="org.springframework.orm.jpa.LocalContainerEntityManagerFactoryBean">
		<property name="dataSource" ref="${springJPA.dataSource}" />
		<property name="persistenceProvider" ref="persistenceProvider" />
		<property name="jpaVendorAdapter" ref="jpaVendorAdapter" />
		<property name="jpaDialect" ref="jpaDialect" />
		<property name="packagesToScan" value="${springJPA.packagesToScan}" />
		<property name="jpaProperties">
			<props>
				<prop key="hibernate.dialect">org.hibernate.dialect.Oracle9Dialect</prop>
				<prop key="hibernate.connection.driver_class">oracle.jdbc.OracleDriver</prop>
				<prop key="hibernate.max_fetch_depth">3</prop>
				<prop key="hibernate.jdbc.fetch_size">18</prop>
				<prop key="hibernate.jdbc.batch_size">10</prop>
				<prop key="hibernate.hbm2ddl.auto">${springJPA.jpaProperties.hibernate.hbm2ddl.auto}</prop>
				<prop key="hibernate.show_sql">false</prop>
				<prop key="hibernate.format_sql">false</prop>
				<prop key="javax.persistence.validation.mode">none</prop>
				<!-- 查询缓存 -->
				<prop key="hibernate.cache.use_query_cache">true</prop> 
				<prop key="hibernate.cache.region.factory_class">org.hibernate.cache.ehcache.EhCacheRegionFactory</prop>
				<!--若为ALL，则默认全部开启二级缓存 ，实体上的@cacheable都不用打-->
				<prop key="javax.persistence.sharedCache.mode">ENABLE_SELECTIVE</prop>
				<prop key="hibernate.generate_statistics">true</prop>
			</props>
		</property>
	</bean>
	
	<!-- 用于指定持久化实现厂商类 -->
	<bean id="persistenceProvider" class="org.hibernate.ejb.HibernatePersistence" />
	
	<!-- 用于设置JPA实现厂商的特定属性 -->
	<bean id="jpaVendorAdapter" class="org.springframework.orm.jpa.vendor.HibernateJpaVendorAdapter">
		<property name="database" value="${springJPA.jpaVendorAdapter.database}" />
	</bean>
	
	<!-- 用于指定一些高级特性 -->
	<bean id="jpaDialect" class="org.springframework.orm.jpa.vendor.HibernateJpaDialect" />
	
	<!-- 事务管理器 -->
	<bean id="transactionManager" class="org.springframework.orm.jpa.JpaTransactionManager">
		<property name="entityManagerFactory" ref="entityManagerFactory" />
	</bean>
	
	<!-- 启用 annotation事务-->
	<tx:annotation-driven />
	<!-- 配置Spring Data JPA扫描目录-->
	<jpa:repositories base-package="com.wonders.**.dao"
		entity-manager-factory-ref="entityManagerFactory"
		transaction-manager-ref="transactionManager"
		repository-impl-postfix="Impl"
		factory-class="com.wonders.frame.core.dao.DefaultRepositoryFactoryBean">
	</jpa:repositories>
</beans>
```
工厂bean：DefaultRepositoryFactoryBean
```java
/**
 * 
 */
package com.wonders.frame.core.dao;
import java.io.Serializable;
import javax.persistence.EntityManager;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.support.JpaRepositoryFactoryBean;
import org.springframework.data.repository.core.support.RepositoryFactorySupport;
/**
 *
 *
 */
public class DefaultRepositoryFactoryBean<T extends JpaRepository<S, ID>, S, ID extends Serializable>
		extends JpaRepositoryFactoryBean<T, S, ID> {
	/**
     * Returns a {@link RepositoryFactorySupport}.
     *
     * @param entityManager
     * @return
     */
	protected RepositoryFactorySupport createRepositoryFactory(EntityManager entityManager) {
 
    	return new DefaultRepositoryFactory(entityManager);
    }
}
```
工厂类：DefaultRepositoryFactory
```java
package com.wonders.frame.core.dao;
import java.io.Serializable;
import javax.persistence.EntityManager;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.support.JpaEntityInformation;
import org.springframework.data.jpa.repository.support.JpaRepositoryFactory;
import org.springframework.data.querydsl.QueryDslPredicateExecutor;
import org.springframework.data.querydsl.QueryDslUtils;
import org.springframework.data.repository.core.RepositoryMetadata;
import org.springframework.util.Assert;
import com.wonders.frame.core.dao.impl.GenericRepositoryImpl;
/**
 * 
 *
 */
public class DefaultRepositoryFactory extends JpaRepositoryFactory {
	private final EntityManager entityManager;
    
	public DefaultRepositoryFactory(EntityManager entityManager) {
		super(entityManager);
		Assert.notNull(entityManager);
		this.entityManager = entityManager;
		
	}
	
	@Override
	protected <T, ID extends Serializable> JpaRepository<?, ?> getTargetRepository(RepositoryMetadata metadata, EntityManager entityManager) {
		//TODO
		JpaEntityInformation<?, Serializable> entityInformation = getEntityInformation(metadata.getDomainClass());
		return new GenericRepositoryImpl(entityInformation, entityManager); // custom implementation
	}
  
    @Override
    protected Class<?> getRepositoryBaseClass(RepositoryMetadata metadata) {
 
    	return GenericRepositoryImpl.class;
    }
}
```
自定义扩展接口：GenericRepository
```java
package com.wonders.frame.core.dao;
import java.io.Serializable;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.List;
import javax.persistence.Query;
import org.springframework.data.domain.Pageable;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.JpaSpecificationExecutor;
import org.springframework.data.repository.CrudRepository;
import org.springframework.data.repository.NoRepositoryBean;
import org.springframework.transaction.annotation.Transactional;
import com.wonders.frame.core.model.vo.SimplePage;
/**
 * @
 * 针对spring data jpa所提供的接口{@link JpaRepository}再次扩展
 * @NoRepositoryBean是必须的
 */
@NoRepositoryBean
public interface GenericRepository<T, ID extends Serializable> 
	extends JpaRepository<T, ID>,JpaSpecificationExecutor<T> {
	
	List<T> findAll(HashMap<String,Object> queryParams,
			LinkedHashMap<String, String> orderby);
	SimplePage<T> findByPage(HashMap<String,Object> queryParams,
			LinkedHashMap<String, String> orderby, Integer pageSize,Integer pageNum);
	
	SimplePage<T> findByPageWithSql(String sql,
			HashMap<String, Object> queryParams, Integer pageSize,
			Integer pageNum);
	SimplePage<T> findByPageWithWhereHql(String whereHql,
			HashMap<String, Object> queryParams, Integer pageSize,
			Integer pageNum);
	SimplePage<T> findByPage(HashMap<String, Object> queryParams,
			String orderby, Integer pageSize, Integer pageNum);
	/**
	 * 
	 * 根据sql语句查询，结果是实体的集合
	 * 
	 * @param sql
	 * 
	 * @param entityClass
	 * 
	 * @return
	 */
	public List<T> findAllBySql(Class<T> entityClass, String sql);
	
	public String getUniqueResultBySql(String sql,HashMap<String,Object> queryParams);
	
	//public List<T> findByAttachIds(String[] ids);
	
//	@Transactional
//	public void updateModelNameAndModelId(String modelName, Integer modelId, String[] ids);
}
```
自定义扩展实现类：GenericRepositoryImpl ，其中的其他方法都是全局共享方法，即每个继承自定义扩展接口的用户接口均可使用这些函数
```java
package com.wonders.frame.core.dao.impl;
import java.io.Serializable;
import java.util.HashMap;
import java.util.LinkedHashMap;
import java.util.List;
import javax.persistence.EntityManager;
import javax.persistence.Query;
import org.apache.log4j.Logger;
import org.springframework.data.jpa.repository.support.JpaEntityInformation;
import org.springframework.data.jpa.repository.support.JpaEntityInformationSupport;
import org.springframework.data.jpa.repository.support.SimpleJpaRepository;
import org.springframework.data.repository.NoRepositoryBean;
import org.springframework.data.repository.query.Param;
import org.springframework.transaction.annotation.Transactional;
import com.wonders.frame.core.dao.GenericRepository;
import com.wonders.frame.core.model.vo.PageInfo;
import com.wonders.frame.core.model.vo.SimplePage;
import com.wonders.frame.core.utils.ReflectUtil;
/**
 * {@link GenericRepository}接口实现类，并在{@link SimpleJpaRepository}基础上扩展。
 * @param <T> ORM对象
 * @param <ID> 主键ID	
 */
@NoRepositoryBean   // 必须的
public class GenericRepositoryImpl<T, ID extends Serializable> extends
		SimpleJpaRepository<T, ID> implements GenericRepository<T, ID> {
	static Logger logger = Logger.getLogger(GenericRepositoryImpl.class);
    private final EntityManager em;
    private final Class<T> entityClass;
    private final String entityName;
    
	/**
	 * 构造函数
     * @param domainClass
     * @param em
     */
	public GenericRepositoryImpl(final JpaEntityInformation<T, ?> entityInformation, EntityManager entityManager) {
		
		super(entityInformation, entityManager);
		this.em = entityManager;
		this.entityClass=entityInformation.getJavaType();
		this.entityName=entityInformation.getEntityName();
	}
	
	/**
	 * 构造函数
	 * @param domainClass
	 * @param em
	 */
	public GenericRepositoryImpl(Class<T> domainClass, EntityManager em) {
        this(JpaEntityInformationSupport.getMetadata(domainClass, em), em); 
    }
	
	@Override
    public List<T> findAll(HashMap<String,Object> queryParams,
    		LinkedHashMap<String, String> orderby){        
		 
		String whereHql=buildWhereQuery(queryParams);
		String orderHql=buildOrderby(orderby);
	
		String hql = "select o from "+entityName+" o where o.removed=0"; 
		Query query=createQuery(hql+whereHql+orderHql,queryParams);
		List<T> list=(List<T>)query.getResultList();
		return list;    
	}
	
    
    @Override
	public SimplePage<T> findByPage(HashMap<String,Object> queryParams,
			LinkedHashMap<String, String> orderby, Integer pageSize,Integer pageNum) {
    	
    	return findByPage(queryParams,buildOrderby(orderby), pageSize, pageNum);
	}
    @Override
    public SimplePage<T> findByPage(HashMap<String,Object> queryParams,
			String orderby, Integer pageSize,Integer pageNum) {
               		 
		String whereHql=buildWhereQuery(queryParams);
		String orderHql=orderby;
		
		String hql="select count(*) from "+ entityName+ " o where o.removed=0 "; 
		Query query=createQuery(hql+whereHql+orderHql,queryParams);
		PageInfo pageInfo=new PageInfo(((Long)query.getSingleResult()).intValue(),pageSize);
		pageInfo.refresh(pageNum);
        
        hql = "select o from "+ entityName+ " o where o.removed=0 ";
        query=createQuery(hql+whereHql+orderHql,queryParams);
        query.setFirstResult(pageInfo.getStartRecord()).setMaxResults(pageInfo.getPageSize());  
        return new SimplePage<T>(pageInfo,query.getResultList());
	}
    
    private Query createQuery(String hql,HashMap<String,Object> queryParams){
    	Query query = em.createQuery(hql);
		setQueryParams(query, queryParams);
		return query;
    }
	@Override
	public SimplePage<T> findByPageWithWhereHql(String whereHql, HashMap<String,Object> queryParams, Integer pageSize,Integer pageNum) {
        
		String hql="select count(*) from "+ entityName+ " o where o.removed=0 ";  
		if(whereHql==null){
			whereHql="";
		}
		Query query = em.createQuery(hql+whereHql);
		setQueryParams(query, queryParams);
		PageInfo pageInfo=new PageInfo(((Long)query.getSingleResult()).intValue(),pageSize);
		pageInfo.refresh(pageNum);
        
        hql = "select o from "+ entityName+ " o where o.removed=0 ";
        query = em.createQuery(hql+whereHql);        
        setQueryParams(query, queryParams);
        query.setFirstResult(pageInfo.getStartRecord()).setMaxResults(pageInfo.getPageSize());  
        return new SimplePage<T>(pageInfo,query.getResultList());
	}
	
	@Override
	public SimplePage<T> findByPageWithSql(String sql, HashMap<String,Object> queryParams, Integer pageSize,Integer pageNum) {
		Query query = em.createNativeQuery("select count(*) from ("+sql+")");  
		setQueryParams(query, queryParams);
		
		PageInfo pageInfo=new PageInfo(((Long)query.getSingleResult()).intValue(),pageSize);
		pageInfo.refresh(pageNum);
		
		query = em.createNativeQuery(sql);
		setQueryParams(query, queryParams);
        query.setFirstResult(pageInfo.getStartRecord()).setMaxResults(pageInfo.getPageSize());  
		return new SimplePage<T>(pageInfo,query.getResultList());
	}
	
	private void setQueryParams(Query query, HashMap<String, Object> queryParams){
		if(queryParams!=null && queryParams.size()>0){
			for(String key : queryParams.keySet()){
				Class clazz=ReflectUtil.getFieldType(this.entityClass, key.replaceAll("_s","").replaceAll("_e", "").replaceAll("_in", ""));
				if(clazz!=null && clazz.equals(String.class) && !key.endsWith("_in")){
					query.setParameter(key, '%'+queryParams.get(key).toString()+'%');
				}else{
					query.setParameter(key, queryParams.get(key));
				}
			}
		}
	}
	private String buildOrderby(LinkedHashMap<String, String> orderby) {
		// TODO Auto-generated method stub
		StringBuffer orderbyql = new StringBuffer("");
		if(orderby!=null && orderby.size()>0){
			orderbyql.append(" order by ");
			for(String key : orderby.keySet()){
				orderbyql.append("o.").append(key).append(" ").append(orderby.get(key)).append(",");
			}
			orderbyql.deleteCharAt(orderbyql.length()-1);
		}
		
		return orderbyql.toString();
	}
	
	private String buildWhereQuery(HashMap<String, Object> queryParams) {
		// TODO Auto-generated method stub
		StringBuffer whereQueryHql = new StringBuffer("");
		if(queryParams!=null && queryParams.size()>0){
			for(String key : queryParams.keySet()){
//				if(key.equalsIgnoreCase("id")){
//					whereQueryHql.append(" and ").append("o.").append(key).append(" in(:").append(key).append(")");
//				}else 
				if(key.endsWith("_s")){
					whereQueryHql.append(" and ").append("o.").append(key.replace("_s", "")).append(" >=:").append(key);
				}else if(key.endsWith("_e")){
					whereQueryHql.append(" and ").append("o.").append(key.replace("_e", "")).append(" <=:").append(key);
				}else if(key.endsWith("_in")){
					whereQueryHql.append(" and ").append("o.").append(key.replace("_in", "")).append(" in:").append(key);
				}else{
					Class clazz=ReflectUtil.getFieldType(this.entityClass, key);
					if(clazz!=null && clazz.equals(String.class)){
						whereQueryHql.append(" and ").append("o.").append(key).append(" like :").append(key);
					}else{
						whereQueryHql.append(" and ").append("o.").append(key).append(" =:").append(key);
					}
					
				}
			}
		}
		
		return whereQueryHql.toString();
	}
	@Override
	public List<T> findAllBySql(Class<T> entityClass, String sql) {
          //创建原生SQL查询QUERY实例,指定了返回的实体类型
          Query query =  em.createNativeQuery(sql,entityClass);
          //执行查询，返回的是实体列表,
          List<T> EntityList = query.getResultList();
		return EntityList;
	}
	public String getUniqueResultBySql(String sql,HashMap<String,Object> queryParams){
		Query query =  em.createNativeQuery(sql);
		for(String key : queryParams.keySet()){
			query.setParameter(key, queryParams.get(key));
		}
        //执行查询，返回的是实体列表,
        String result = (String)query.getSingleResult();
       return result;
	}
//	public List<T> findByAttachIds(String[] ids){
//		String idsString ="";
//		if(ids==null || ids.length<1) return null;
//		for(int i=0; i<ids.length; i++){
//			try {
//				Long.valueOf(ids[i]);
//			} catch (NumberFormatException e) {
//				return null;
//			}
//			idsString += ids[i] +",";
//		}
//		idsString = idsString.substring(0,idsString.length()-1);
//		String hql ="from Attach t where t.removed='0' and t.id in ("+idsString+")";
//		Query query=createQuery(hql, null);
//		List<T> list=(List<T>)query.getResultList();
//		return list;    
//	}
//	
//	@Transactional
//	public void updateModelNameAndModelId(String modelName, Integer modelId, String[] ids){
//		String idsString ="";
//		if(ids==null || ids.length<1) return;
//		for(int i=0; i<ids.length; i++){
//			try {
//				Long.valueOf(ids[i]);
//			} catch (NumberFormatException e) {
//				return;
//			}
//			idsString += ids[i] +",";
//		}
//		idsString = idsString.substring(0,idsString.length()-1);
//		String hql ="update Attach a set a.modelName ="+modelName+", a.modelId ="+modelId+" where a.id in ("+idsString+")";
//		createQuery(hql, null).executeUpdate();
//	}
}
```
使用范例：自定义接口继承 扩展接口
```java
package com.wonders.frame.cpq.dao;
import java.util.List;
import javax.persistence.QueryHint;
import org.springframework.data.domain.Page;
import org.springframework.data.domain.Pageable;
import org.springframework.data.jpa.repository.Modifying;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.jpa.repository.QueryHints;
import org.springframework.data.repository.query.Param;
import org.springframework.transaction.annotation.Transactional;
import com.wonders.frame.core.dao.GenericRepository;
import com.wonders.frame.cpq.model.bo.CpqMain;
public interface CpqMainDao extends GenericRepository<CpqMain, Integer>{
	public Page<CpqMain> findAll(Pageable pageable);
		
	@Query("select a from CpqMain a where a.removed = 0 and a.id =?")
	public CpqMain findById(Integer id);
	
	@Transactional  
	@Modifying 
	@Query("update CpqMain a set a.removed = 1 where a.id =:id")
	public Integer removeById(@Param("id")Integer id);
	
	
    @QueryHints({ @QueryHint(name = "org.hibernate.cacheable", value ="true") })  
    public List<CpqMain> findAll();  
    
}
```
