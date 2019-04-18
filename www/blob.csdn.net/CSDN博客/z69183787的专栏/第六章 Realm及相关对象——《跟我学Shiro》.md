# 第六章 Realm及相关对象——《跟我学Shiro》 - z69183787的专栏 - CSDN博客
2016年07月11日 17:44:34[OkidoGreen](https://me.csdn.net/z69183787)阅读数：805
ection；然后通过其getPrimaryPrincipal获取PrimaryPrincipal。
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- Set<String> realmNames = princialCollection.getRealmNames();  
获取所有身份验证成功的Realm名字。      
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- Set<Object> principals = princialCollection.asSet(); //asList和asSet的结果一样
将身份信息转换为Set/List，即使转换为List，也是先转换为Set再完成的。
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- Collection<User> users = princialCollection.fromRealm("c");  
根据Realm名字获取身份，因为Realm名字可以重复，所以可能多个身份，建议Realm名字尽量不要重复。
## 6.4 AuthorizationInfo
![](http://dl2.iteye.com/upload/attachment/0094/1345/c8868cea-6b01-34cc-b7be-45ee244bd217.png)
AuthorizationInfo用于聚合授权信息的：
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- publicinterface AuthorizationInfo extends Serializable {  
-     Collection<String> getRoles(); //获取角色字符串信息
-     Collection<String> getStringPermissions(); //获取权限字符串信息
-     Collection<Permission> getObjectPermissions(); //获取Permission对象信息
- }   
当我们使用AuthorizingRealm时，如果身份验证成功，在进行授权时就通过doGetAuthorizationInfo方法获取角色/权限信息用于授权验证。
Shiro提供了一个实现SimpleAuthorizationInfo，大多数时候使用这个即可。
对于Account及SimpleAccount，之前的【6.3 AuthenticationInfo】已经介绍过了，用于SimpleAccountRealm子类，实现动态角色/权限维护的。
## 6.5 Subject
![](http://dl2.iteye.com/upload/attachment/0094/1347/131b0c95-9aed-36b0-9c09-0a6f9c6b3605.png)
Subject是Shiro的核心对象，基本所有身份验证、授权都是通过Subject完成。
**1、身份信息获取**
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- Object getPrincipal(); //Primary Principal
- PrincipalCollection getPrincipals(); // PrincipalCollection 
**2、身份验证**
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- void login(AuthenticationToken token) throws AuthenticationException;  
- boolean isAuthenticated();  
- boolean isRemembered();  
通过login登录，如果登录失败将抛出相应的AuthenticationException，如果登录成功调用isAuthenticated就会返回true，即已经通过身份验证；如果isRemembered返回true，表示是通过记住我功能登录的而不是调用login方法登录的。isAuthenticated/isRemembered是互斥的，即如果其中一个返回true，另一个返回false。
**3、角色授权验证**
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- boolean hasRole(String roleIdentifier);  
- boolean[] hasRoles(List<String> roleIdentifiers);  
- boolean hasAllRoles(Collection<String> roleIdentifiers);  
- void checkRole(String roleIdentifier) throws AuthorizationException;  
- void checkRoles(Collection<String> roleIdentifiers) throws AuthorizationException;  
- void checkRoles(String... roleIdentifiers) throws AuthorizationException;   
hasRole*进行角色验证，验证后返回true/false；而checkRole*验证失败时抛出AuthorizationException异常。 
**4、权限授权验证**
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- boolean isPermitted(String permission);  
- boolean isPermitted(Permission permission);  
- boolean[] isPermitted(String... permissions);  
- boolean[] isPermitted(List<Permission> permissions);  
- boolean isPermittedAll(String... permissions);  
- boolean isPermittedAll(Collection<Permission> permissions);  
- void checkPermission(String permission) throws AuthorizationException;  
- void checkPermission(Permission permission) throws AuthorizationException;  
- void checkPermissions(String... permissions) throws AuthorizationException;  
- void checkPermissions(Collection<Permission> permissions) throws AuthorizationException;  
isPermitted*进行权限验证，验证后返回true/false；而checkPermission*验证失败时抛出AuthorizationException。
**5、会话**
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- Session getSession(); //相当于getSession(true)
- Session getSession(boolean create);    
类似于Web中的会话。如果登录成功就相当于建立了会话，接着可以使用getSession获取；如果create=false如果没有会话将返回null，而create=true如果没有会话会强制创建一个。
**6、退出**
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- void logout();  
**7、RunAs**
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- void runAs(PrincipalCollection principals) throws NullPointerException, IllegalStateException;  
- boolean isRunAs();  
- PrincipalCollection getPreviousPrincipals();  
- PrincipalCollection releaseRunAs();   
RunAs即实现“允许A假设为B身份进行访问”；通过调用subject.runAs(b)进行访问；接着调用subject.getPrincipals将获取到B的身份；此时调用isRunAs将返回true；而a的身份需要通过subject. getPreviousPrincipals获取；如果不需要RunAs了调用subject. releaseRunAs即可。
**8、多线程**
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- <V> V execute(Callable<V> callable) throws ExecutionException;  
- void execute(Runnable runnable);  
- <V> Callable<V> associateWith(Callable<V> callable);  
- Runnable associateWith(Runnable runnable);   
实现线程之间的Subject传播，因为Subject是线程绑定的；因此在多线程执行中需要传播到相应的线程才能获取到相应的Subject。最简单的办法就是通过execute(runnable/callable实例)直接调用；或者通过associateWith(runnable/callable实例)得到一个包装后的实例；它们都是通过：1、把当前线程的Subject绑定过去；2、在线程执行结束后自动释放。
Subject自己不会实现相应的身份验证/授权逻辑，而是通过DelegatingSubject委托给SecurityManager实现；及可以理解为Subject是一个面门。
对于Subject的构建一般没必要我们去创建；一般通过SecurityUtils.getSubject()获取：
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- publicstatic Subject getSubject() {  
-     Subject subject = ThreadContext.getSubject();  
- if (subject == null) {  
-         subject = (new Subject.Builder()).buildSubject();  
-         ThreadContext.bind(subject);  
-     }  
- return subject;  
- }   
即首先查看当前线程是否绑定了Subject，如果没有通过Subject.Builder构建一个然后绑定到现场返回。
如果想自定义创建，可以通过：
Java代码  ![收藏代码](http://jinnianshilongnian.iteye.com/images/icon_star.png)
- new Subject.Builder().principals(身份).authenticated(true/false).buildSubject()  
这种可以创建相应的Subject实例了，然后自己绑定到线程即可。在new Builder()时如果没有传入SecurityManager，自动调用SecurityUtils.getSecurityManager获取；也可以自己传入一个实例。
对于Subject我们一般这么使用：
**1、身份验证（login）**
**2、授权（hasRole*/isPermitted*或checkRole*/checkPermission*）**
**3、将相应的数据存储到会话（Session）**
**4、切换身份（RunAs）/多线程身份传播**
**5、退出**
而我们必须的功能就是1、2、5。到目前为止我们就可以使用Shiro进行应用程序的安全控制了，但是还是缺少如对Web验证、Java方法验证等的一些简化实现。    
示例源代码：[https://github.com/zhangkaitao/shiro-example](https://github.com/zhangkaitao/shiro-example)；可加群134755960探讨Spring/Shiro技术。
