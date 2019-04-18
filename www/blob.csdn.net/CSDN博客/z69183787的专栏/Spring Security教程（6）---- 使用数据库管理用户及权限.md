# Spring Security教程（6）---- 使用数据库管理用户及权限 - z69183787的专栏 - CSDN博客
2014年03月13日 21:23:16[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4894
上一章已经把表结构上传了，今天这部分主要用到的表是
- SYS_USERS
 用户管理表
- SYS_ROLES
 角色管理表
- SYS_AUTHORITIES权限管理表
- SYS_USERS_ROLES用户角色表
- SYS_ROLES_AUTHORITIES角色权限表
要实现使用数据库管理用户，需要自定义用户登录功能，而Spring已经为我们提供了接口UserDetailsService
**[java]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18354599#)[copy](http://blog.csdn.net/jaune161/article/details/18354599#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/159045/fork)
- package org.springframework.security.core.userdetails;  
- 
- publicinterface UserDetailsService {  
- 
- /**
-      * Locates the user based on the username. In the actual implementation, the search may possibly be case
-      * insensitive, or case insensitive depending on how the implementation instance is configured. In this case, the
-      * <code>UserDetails</code> object that comes back may have a username that is of a different case than what was
-      * actually requested..
-      *
-      * @param username the username identifying the user whose data is required.
-      *
-      * @return a fully populated user record (never <code>null</code>)
-      *
-      * @throws UsernameNotFoundException if the user could not be found or the user has no GrantedAuthority
-      */
-     UserDetails loadUserByUsername(String username) throws UsernameNotFoundException;  
- }  
UserDetailsService是一个接口，只有一个方法loadUserByUsername，根据方法名可以看出这个方法是根据用户名来获取用户信息，但是返回的是一个UserDetails对象。而UserDetails也是一个接口
**[java]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18354599#)[copy](http://blog.csdn.net/jaune161/article/details/18354599#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/159045/fork)
- package org.springframework.security.core.userdetails;  
- 
- import org.springframework.security.core.Authentication;  
- import org.springframework.security.core.GrantedAuthority;  
- 
- import java.io.Serializable;  
- import java.util.Collection;  
- //这里省略了Spring的注释，只是我自己对这些方法的简单的注释，如果想了解Spring对这些方法的注释，请查看Spring源码
- publicinterface UserDetails extends Serializable {  
- 
-     Collection<? extends GrantedAuthority> getAuthorities(); //权限集合
- 
-     String getPassword(); //密码
- 
-     String getUsername(); //用户名
- 
- boolean isAccountNonExpired(); //账户没有过期
- 
- boolean isAccountNonLocked();  //账户没有被锁定
- 
- boolean isCredentialsNonExpired(); //证书没有过期
- 
- boolean isEnabled();//账户是否有效
- }  
因此我们的SysUsers这个bean需要实现这个接口
**[html]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18354599#)[copy](http://blog.csdn.net/jaune161/article/details/18354599#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/159045/fork)
- @Entity  
- @DynamicUpdate(true)  
- @DynamicInsert(true)  
- @Table(name = "SYS_USERS", schema = "FYBJ")  
- public class SysUsers implements UserDetails,Serializable {  
- 
-     /**  
-      *   
-      */  
-     private static final long serialVersionUID = -6498309642739707784L;  
- 
-     // Fields  
- 
-     private String userId;  
-     private String username;  
-     private String name;  
-     private String password;  
-     private Date dtCreate;  
-     private Date lastLogin;  
-     private Date deadline;  
-     private String loginIp;  
-     private String VQzjgid;  
-     private String VQzjgmc;  
-     private String depId;  
-     private String depName;  
-     private boolean enabled;  
-     private boolean accountNonExpired;  
-     private boolean accountNonLocked;  
-     private boolean credentialsNonExpired;  
-     @JsonIgnore  
-     private Set<SysUsersRoles>sysUsersRoleses = new HashSet<SysUsersRoles>(0);  
- 
-     private Collection<GrantedAuthority>  authorities;  
- 
-     //.....省略setter,getter.....  
-     //如果属性是boolean（注：不是Boolean）类型的值，在生产getter时会变为isXxx,如enabled生产getter为isEnabled  
- }  
这样写我们的SysUsers只要生产getter和setter方法就实现了UserDetails，同时还可以使用数据库来控制这些属性，两全其美。
在UserDetails中有个属性需要注意下Collection<GrantedAuthority>  authorities，这个属性中存储了这个用户所有的权限。
下面需要先写下SysUsers的DAO层，一个方法是根据用户名获取用户，一个方法是根据用户名获取用户所有的权限，这里我用的是Spring Data Jpa，如果不懂这个请自行从网上查阅资料
**[java]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18354599#)[copy](http://blog.csdn.net/jaune161/article/details/18354599#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/159045/fork)
- publicinterface SysUsersRepository extends JpaRepository<SysUsers, String> {  
- 
- public SysUsers getByUsername(String username);  
- 
- public Collection<GrantedAuthority> loadUserAuthorities(String username);  
- 
- }  
其中getByUsername符合Spring的命名规范，所以这个方法不需要我们来实现，而loadUserAuthorities则需要我们自己动手实现
**[java]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18354599#)[copy](http://blog.csdn.net/jaune161/article/details/18354599#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/159045/fork)
- publicclass SysUsersRepositoryImpl {  
- 
- protected Log logger = LogFactory.getLog(getClass());  
- 
- @PersistenceContext
- private EntityManager entityManager;  
- 
- /**
-      * 根据用户名获取到用户的权限并封装成GrantedAuthority集合
-      * @param username
-      */
- public Collection<GrantedAuthority> loadUserAuthorities(String username){  
-         List<SysAuthorities> list = this.getSysAuthoritiesByUsername(username);  
- 
-         List<GrantedAuthority> auths = new ArrayList<GrantedAuthority>();  
- 
- for (SysAuthorities authority : list) {  
-             GrantedAuthority grantedAuthority = new SimpleGrantedAuthority(authority.getAuthorityMark());  
-             auths.add(grantedAuthority);  
-         }  
- 
- return auths;  
- 
-     }  
- /**
-      * 先根据用户名获取到SysAuthorities集合
-      * @param username
-      * @return
-      */
- @SuppressWarnings("unchecked")  
- private List<SysAuthorities> getSysAuthoritiesByUsername(String username){  
-         String sql = "SELECT * FROM SYS_AUTHORITIES WHERE AUTHORITY_ID IN( "+  
- "SELECT DISTINCT AUTHORITY_ID FROM SYS_ROLES_AUTHORITIES  S1 "+  
- "JOIN SYS_USERS_ROLES S2 ON S1.ROLE_ID = S2.ROLE_ID "+  
- "JOIN SYS_USERS S3 ON S3.USER_ID = S2.USER_ID AND S3.USERNAME=?1)";  
- 
-         Query query = this.entityManager.createNativeQuery(sql, SysAuthorities.class);  
-         query.setParameter(1, username);  
- 
-         List<SysAuthorities> list = query.getResultList();  
- return list;  
-     }  
- }  
不管是用Spring Data Jpa还是普通的方法只要实现这两个方法就可以了
最后也是最重要的一个类UserDetailsService
**[java]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18354599#)[copy](http://blog.csdn.net/jaune161/article/details/18354599#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/159045/fork)
- publicclass DefaultUserDetailsService implements UserDetailsService {  
- 
- protectedfinal Log logger = LogFactory.getLog(getClass());  
- 
- @Autowired
- private SysUsersRepository sysUsersRepository;  
- 
- @Autowired
- private MessageSource messageSource;  
- 
- @Autowired
- private UserCache userCache;  
- 
- @Override
- public UserDetails loadUserByUsername(String username)  
- throws UsernameNotFoundException {  
- 
-         Collection<GrantedAuthority> auths = new ArrayList<GrantedAuthority>();  
-         SysUsers user = (SysUsers) this.userCache.getUserFromCache(username);  
- 
- if(user == null){  
-             user = this.sysUsersRepository.getByUsername(username);  
- if(user == null)  
- thrownew UsernameNotFoundException(this.messageSource.getMessage(  
- "UserDetailsService.userNotFount", new Object[]{username}, null));  
- //得到用户的权限
-             auths = this.sysUsersRepository.loadUserAuthorities( username );  
- 
-             user.setAuthorities(auths);  
-         }  
- 
-         logger.info("*********************"+username+"***********************");  
-         logger.info(user.getAuthorities());  
-         logger.info("********************************************************");  
- 
- this.userCache.putUserInCache(user);  
- 
- return user;  
-     }  
- 
- }  
在loadUserByUsername方法中首先是从缓存中查找用户，如果找到用户就直接用缓存中的用户，如果没有找到就从数据库中获取用户信息。
从数据库中获取用户时先获取User对象，如果用户为空则抛出UsernameNotFoundException，其中UserDetailsService.userNotFount是在property文件中自定义的，如果获取到了user则再获取用户的权限，按照Spring的标准如果没有任何权限也是要抛出这个异常的，在这里我们就不做判断了。
登录后可以看到控制台打印出来以下信息
**[plain]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18354599#)[copy](http://blog.csdn.net/jaune161/article/details/18354599#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/159045/fork)
- *********************admin***********************  
- [AUTH_PASSWORD_MODIFY, AUTH_GG_FBGBGG, AUTH_GG_FBZNGG]  
- ********************************************************  
说明我们登录成功并且已经获取到了权限，但是可能会出现如下页面
![](https://img-blog.csdn.net/20140116141402750?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamF1bmUxNjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这样就是你在数据库中存储的权限跟配置文件中的不对应，或者说访问资源是没有从用户的权限集合中找到这个权限。
