# Spring Data JPA 全局DAO的扩展 - z69183787的专栏 - CSDN博客
2014年04月03日 06:21:04[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1935
前几天看了springside4的mini-web代码发现确实有不少新的东东，咱这次单说说Spring Data JPA吧。 
引用springside4的 wiki关于对Spring Data JPA的简介 
Spring Data JPA在JPA上又做了一层封装，只要编写接口就够了，不用写一行实现代码，CRUD方法啦，分页啦，自动将findByLoginName()的方法定义翻译成适当的QL啦都由它包了： 
public interface UserDao extends PagingAndSortingRepository<User, Long> { 
User findByLoginName(String loginName); 
} 
使用上很简单，快速浏览一下下面的资料就够了。 
只有一个坑爹的地方，如果要为UserDao扩展方法(而不是接口)，要新增一个UserDaoCustom接口，这时候，实现类的名字必须是UserDaoImpl，而不是UserDaoCustomImpl。 
另外，除了智能地翻译连Less,Not,And,Or都支持的方法名，它当然也可以直接用@Query在方法上标注复杂的查询语句。 
资料 
•[官方文档](http://static.springsource.org/spring-data/data-jpa/docs/current/reference/html/)
•[使用 Spring Data JPA 简化 JPA 开发 IBM DW上的中文版教程.](http://www.ibm.com/developerworks/cn/opensource/os-cn-spring-jpa/index.html)
 如果看完上面的资料 也许你对Spring Data JPA有了初步的认识，动动手你就知道他的强大，但是对DAO的扩展上有点麻烦，上面红字部分是对单个dao进行扩展的方法。下面我们来说一下对全局DAO的扩展，创建你自己的CustomRepository。
首先咱们要建一个自己的扩展接口类MyRepository
Java代码  ![收藏代码](http://haolx.iteye.com/images/icon_star.png)
- import java.io.Serializable;  
- 
- import org.springframework.data.jpa.repository.JpaRepository;  
- 
- publicinterface MyRepository<T, ID extends Serializable>   
- extends JpaRepository<T, ID> {  
- 
- String  sharedCustomMethod();  
- }  
其中sharedCustomMethod是全局的共享自定义方法。
然后在建一个实现类MyCustomRepository
Java代码  ![收藏代码](http://haolx.iteye.com/images/icon_star.png)
- import java.io.Serializable;  
- 
- import javax.persistence.EntityManager;  
- 
- import org.springframework.data.jpa.repository.support.JpaEntityInformation;  
- import org.springframework.data.jpa.repository.support.SimpleJpaRepository;  
- import org.springframework.data.repository.NoRepositoryBean;  
- 
- @NoRepositoryBean
- publicclass MyCustomRepository<T, ID extends Serializable>   
- extends SimpleJpaRepository<T, ID> implements MyRepository<T, ID> {  
- 
- 
- privatefinal EntityManager entityManager;  
- 
- public MyCustomRepository(Class<T> domainClass, EntityManager em) {  
- super(domainClass, em);  
- // TODO Auto-generated constructor stub
- 
-         entityManager=em;  
-     }  
- 
- public MyCustomRepository(final JpaEntityInformation<T, ?> entityInformation, final EntityManager entityManager) {  
- super(entityInformation, entityManager);  
- this.entityManager = entityManager;  
- }  
- 
- 
- public String sharedCustomMethod() {  
- return"hello sharedCustomMethod";  
- // implementation goes here
- }   
- }  
 注意 @NoRepositoryBean一定要有的，还有全局的扩展实现类不要用Imp作为后缀名，不然会报异常的（目前还没搞清楚报异常的具体原因，个人猜测可能是和局部的扩展有冲突吧）。
然后在定义MyRepositoryFactory
Java代码  ![收藏代码](http://haolx.iteye.com/images/icon_star.png)
- import java.io.Serializable;  
- 
- importstatic org.mockito.Mockito.*;  
- 
- import javax.persistence.EntityManager;  
- 
- import org.springframework.data.jpa.repository.JpaRepository;  
- import org.springframework.data.jpa.repository.support.JpaEntityInformation;  
- import org.springframework.data.jpa.repository.support.JpaRepositoryFactory;  
- import org.springframework.data.repository.core.RepositoryMetadata;  
- 
- 
- publicclass MyRepositoryFactory extends JpaRepositoryFactory {  
- 
- public MyRepositoryFactory(EntityManager entityManager) {  
- super(entityManager);  
- // TODO Auto-generated constructor stub
-     }  
- @Override
- @SuppressWarnings("unchecked")  
- protected JpaRepository<?, ?> getTargetRepository(RepositoryMetadata metadata, EntityManager em) {  
- 
-         JpaEntityInformation<Object, Serializable> entityMetadata = mock(JpaEntityInformation.class);  
-         when(entityMetadata.getJavaType()).thenReturn((Class<Object>) metadata.getDomainType());  
- returnnew MyCustomRepository<Object, Serializable>(entityMetadata, em);  
-     }  
- 
- /*
-      * (non-Javadoc)
-      * 
-      * @see
-      * org.springframework.data.repository.support.RepositoryFactorySupport#
-      * getRepositoryBaseClass()
-      */
- @Override
- protected Class<?> getRepositoryBaseClass(RepositoryMetadata metadata) {  
- 
- return MyCustomRepository.class;  
-     }  
- }  
接着在建一个MyRepositoryFactoryBean
Java代码  ![收藏代码](http://haolx.iteye.com/images/icon_star.png)
- import java.io.Serializable;  
- 
- import javax.persistence.EntityManager;  
- 
- import org.springframework.data.jpa.repository.JpaRepository;  
- import org.springframework.data.jpa.repository.support.JpaRepositoryFactoryBean;  
- import org.springframework.data.repository.core.support.RepositoryFactorySupport;  
- 
- publicclass MyRepositoryFactoryBean<T extends JpaRepository<Object, Serializable>> extends JpaRepositoryFactoryBean<T, Object, Serializable> {  
- 
- 
- @Override
- protected RepositoryFactorySupport createRepositoryFactory(EntityManager em) {  
- 
- returnnew MyRepositoryFactory(em);  
-     }  
- }  
最后在配置文件里需要定义
<jpa:repositories base-package="org.springside.examples.miniweb"  transaction-manager-ref="transactionManager" 
 factory-class="org.springside.examples.miniweb.dao.account.MyRepositoryFactoryBean"
entity-manager-factory-ref="entityManagerFactory"/>
 其实这里还有个repository-impl-postfix=" "个人理解他是来定义局部扩展库的实现类的后缀的默认是Imp。
呵呵 这样你就可以使用全局的自定义扩展库了。
Java代码  ![收藏代码](http://haolx.iteye.com/images/icon_star.png)
- publicinterface UserDao extends MyRepository<User, Long> ,JpaSpecificationExecutor<User>{  
- 
- }  
