# shiro安全三部曲 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2012年11月28日 11:29:21[boonya](https://me.csdn.net/boonya)阅读数：14098








**第一部分 Shiro简介及项目目录结构**


最新官方示例下载：[http://shiro.apache.org/](http://shiro.apache.org/)
- Shiro是Apache从JSecret项目演变而来的，该框架实现了：用户登录、认证、授权和权限管理操作的完整控制流程。Shiro最早的名字是JSecurity，后来更名为Shiro并成为Apache的孵化项目。这次改名也同样影响了Grails Shiro Plugin。它最早在Shiro还未改名之前就已经存在了，后来因为Shiro的名字变更，也就一道跟着“改名换姓”。由于Grails Shiro Plugin中已经包含了Shiro相关的Jar，因此对于插件的使用者而言，不必专门下载Shiro。JSecurity是一个强大，灵活的java安全框架。用于简洁地处理身份验证，授权，企业多个系统会话管理，加密服务等。
- Shiro主要对用户的权限和Session进行特别的封装，并且支持跨平台的登录权限认证，EJB等项目中也可以实现Session的共享。
- Shiro主要使用对象

Subject      当前操作的程序的对象相当于用户User，对应操作视图

SecurityManager  Shiro框架的心脏，确保框架正常运行。


Authenticator    执行身份验证（登录）尝试负责组件。

AuthenticationStrategy   协调Realm用户数据访问策略


Authorizer       负责应用程序中决定用户访问控制的组件。

SessionManager   如何创建及管理用户的session生命周期，提供良好的session体验。

SessionDAO      支持Session CRUD数据操作。

CacheManager    创建并管理Shiro组件执行的Cache实例的生命周期。

Cryptography     Shiro企业安全组件的补充。

Realms             担当Shiro与应用程序安全数据的"桥梁"或"连接器"。
- 框架关系图如下所示：

![](https://img-my.csdn.net/uploads/201211/28/1354074014_9933.png)
- Shiro配置：{以Spring-Hibernate 项目为例}

Shiro.ini初始化文件，可以将用户登录的权限设置在这里

[users]   users=user:*

[roles]   roles=role:*等等，shiro.ini在实际项目中可以不要，它仅仅是一种数据访问配置的手段

ehcache.xml  文件配置对象缓存示例的个数等

log4j.properties   日志配置文件

hibernate.cfg.xml  配置实体隐射关系[Hibernate整合]

applicationContext.xml  配置Spring 数据源访问和对象注入

applicationContext-shiro.xml  Shiro配置文件

Sprhib-servlet.xml      servlet控制页面跳转路径的配置

 配置文件目录如下：

![](https://img-my.csdn.net/uploads/201211/28/1354074069_4622.png)

-  创建规范的项目目录结构如下：
![](https://img-my.csdn.net/uploads/201211/28/1354074116_9658.png)

注：具体文件配置官方示例有。

**第二部分   Shiro与Hibernate的使用配置**



web.xml中的配置：



<!-- Shiro Filter is defined in the spring application context: -->

<filter>

<filter-name>shiroFilter</filter-name>

<filter-class>org.springframework.web.filter.DelegatingFilterProxy</filter-class>

</filter>

<filter-mapping>

   <filter-name>shiroFilter</filter-name>

   <url-pattern>/*</url-pattern>

</filter-mapping>

applicationContext-shiro.xml配置：实现认证和授权

<!-- shiro start -->

<bean id="lifecycleBeanPostProcessor" class="org.apache.shiro.spring.LifecycleBeanPostProcessor" />

<bean id="cacheManager" class="org.apache.shiro.cache.ehcache.EhCacheManager">

      <property name="cacheManagerConfigFile"value="classpath:ehcache.xml" />

</bean>

<bean id="credentialsMatcher"class="org.apache.shiro.authc.credential.HashedCredentialsMatcher">

      <property name="hashAlgorithmName" value="SHA-256" />

</bean>

<bean id="iniRealm" class="com.boonya.shiro.security.CurrentIniRealm">

      <constructor-arg type="java.lang.String" value="classpath:shiro.ini" />

      <property name="credentialsMatcher" ref="credentialsMatcher" />

</bean>

<bean id="userRealm" class="com.boonya.shiro.security.UserRealm" />

<bean id="securityManager" class="org.apache.shiro.web.mgt.DefaultWebSecurityManager">

       <property name="realms">

             <list>

               <ref bean="iniRealm" /> 

               <ref bean="userRealm" />

           </list>

      </property>

      <property name="cacheManager" ref="cacheManager" />

</bean>

<bean id="shiroFilter" class="org.apache.shiro.spring.web.ShiroFilterFactoryBean">

       <property name="securityManager" ref="securityManager" />

       <property name="loginUrl" value="/login" />

       <property name="successUrl" value="/maps/main.html"></property>

       <property name="unauthorizedUrl" value="/unauthorized"></property>

       <property name="filters">

         <util:map>

           <entry key="anAlias">

               <beanclass="org.apache.shiro.web.filter.authc.PassThruAuthenticationFilter" />

           </entry>

         </util:map>

      </property>

      <property name="filterChainDefinitions">

         <value><!-- 权限字符过滤 -->

             /unauthorized=anon

             /validate/code*=anon

             /login/**=anon

             /image/**=anon 

             /js/**=anon

            /css/**=anon

            /common/**=anon

            /index.htm* = anon

           /maps/**=authc

        </value>

    </property>

</bean>

<!-- shiro end -->

**有关必要说明：**

securityManager：这个属性是必须的。

loginUrl ：没有登录的用户请求需要登录的页面时自动跳转到登录页面，不是必须的属性，不输入地址的话会自动寻找项目web项目的根目录下的”/login.jsp”页面。

successUrl ：登录成功默认跳转页面，不配置则跳转至”/”。如果登陆前点击的一个需要登录的页面，则在登录自动跳转到那个需要登录的页面。不跳转到此。

unauthorizedUrl ：没有权限默认跳转的页面。

其权限过滤器及配置释义：

anon   org.apache.shiro.web.filter.authc.AnonymousFilter

authc  org.apache.shiro.web.filter.authc.FormAuthenticationFilter

authcBasic   org.apache.shiro.web.filter.authc.BasicHttpAuthenticationFilter

perms   org.apache.shiro.web.filter.authz.PermissionsAuthorizationFilter

port    org.apache.shiro.web.filter.authz.PortFilter

rest    org.apache.shiro.web.filter.authz.HttpMethodPermissionFilter

roles   org.apache.shiro.web.filter.authz.RolesAuthorizationFilter

ssl     org.apache.shiro.web.filter.authz.SslFilter

user    org.apache.shiro.web.filter.authc.UserFilter

logout  org.apache.shiro.web.filter.authc.LogoutFilter

anon:例子/admins/**=anon 没有参数，表示可以匿名使用。

authc:例如/admins/user/**=authc表示需要认证(登录)才能使用，没有参数

roles：例子/admins/user/**=roles[admin],参数可以写多个，多个时必须加上引号，并且参数之间用逗号分割，当有多个参数时，例如admins/user/**=roles["admin,guest"],每个参数通过才算通过，相当于hasAllRoles()方法。

perms：例子/admins/user/**=perms[user:add:*],参数可以写多个，多个时必须加上引号，并且参数之间用逗号分割，例如/admins/user/**=perms["user:add:*,user:modify:*"]，当有多个参数时必须每个参数都通过才通过，想当于isPermitedAll()方法。

rest：例子/admins/user/**=rest[user],根据请求的方法，相当于/admins/user/**=perms[user:method] ,其中method为post，get，delete等。

port：例子/admins/user/**=port[8081],当请求的url的端口不是8081是跳转到schemal://serverName:8081?queryString,其中schmal是协议http或https等，serverName是你访问的host,8081是url配置里port的端口，queryString

是你访问的url里的？后面的参数。

authcBasic：例如/admins/user/**=authcBasic没有参数表示httpBasic认证

ssl:例子/admins/user/**=ssl没有参数，表示安全的url请求，协议为https

user:例如/admins/user/**=user没有参数表示必须存在用户，当登入操作时不做检查

注：anon，authcBasic，auchc，user是认证过滤器，

perms，roles，ssl，rest，port是授权过滤器
**第三部分  Shiro****代码分析**- 
利用分层思想创建清晰的项目目录结构




---dao                         
数据访问层

---model                    
业务实体

---service                   
业务层

---security/realm       
数据桥接,决定访问那个数据源

---web/controller       控制层/视图展现层

- 
代码分析阶段




[略] dao和model是我们所熟悉的因此这里没有必要再讲述。

[述]

**service  让接口决定业务实现**

我们可以编写业务类的接口并作实现，不需要对外提供的方法不必在接口中定义。

好处：功能明确，结构清晰，使目有全牛，而不是盲目的开始编码。

一个接口对应一个实现。

Java代码：

接口：

public interface UserService {

    User getCurrentUser();

    void createUser(String username, String email, String password);

    List<User> getAllUsers();

    User getUser(Long userId);

    void deleteUser(Long userId);

    void updateUser(User user);

}

实现：

@Service("userService")

public class DefaultUserService implements UserService {

    private UserDAO userDAO;

    @Autowired

    public void setUserDAO(UserDAO userDAO) {

        this.userDAO = userDAO;

    }

    public User getCurrentUser() {

        final Long currentUserId = (Long) SecurityUtils.getSubject().getPrincipal();

        if( currentUserId != null ) {

            return getUser(currentUserId);

        } else {

            return null;

        }

}

.....................

}

**security 衔接程序与数据源的中间组件realm**

一般只提供登录验证的realm即可，当用户进入系统之后还是跟以前的实现模式一样，只是登录的时候必须做安全验证。如果不登录系统，就不能看到具有安全保护的数据页面展示，如果没有普通guest(来宾)访问页面，以致只能看到登录界面<都是通过配置过滤器来实现>。、

实现一个用户自定义的realm必须继承自AuthorizingRealm 。

Java代码：

@Component

public class SampleRealm extends AuthorizingRealm {

    protected UserDAO userDAO = null;

    @SuppressWarnings("deprecation")

public SampleRealm() {

        setName("SampleRealm"); //This name must match the name in the User class's getPrincipals() method

        setCredentialsMatcher(new Sha256CredentialsMatcher());

    }

    @Autowired

    public void setUserDAO(UserDAO userDAO) {

        this.userDAO = userDAO;

}

//用户登录认证

protected AuthenticationInfo doGetAuthenticationInfo(AuthenticationToken authcToken) throws AuthenticationException {

        //认证前调用

        UsernamePasswordToken token = (UsernamePasswordToken) authcToken;

        User user = userDAO.findUser(token.getUsername());

        if( user != null ) {

            //认证后返回认证信息

            return new SimpleAuthenticationInfo(user.getId(), user.getPassword(), getName());

        } else {

            return null;

        }

    }

//获取认证后信息：用户的角色，享有的权限

    protected AuthorizationInfo doGetAuthorizationInfo(PrincipalCollection principals) {

        Long userId = (Long) principals.fromRealm(getName()).iterator().next();

        User user = userDAO.getUser(userId);

        if( user != null ) {

            SimpleAuthorizationInfo info = new SimpleAuthorizationInfo();

            for( Role role : user.getRoles() ) {

                info.addRole(role.getName());

                info.addStringPermissions( role.getPermissions() );

            }

            return info;

        } else {

            return null;

        }

    }

}



**web/controller  决定页面跳转到那个页面或返回相应的页面数据**

用户登入系统后决定用户去向，属于权限控制字符过滤的范围，是否用户有权限访问该URL路径。这里以签到为例：

1、创建绑定操作命令实体

public class SignupCommand {

    private String username;

    private String email;

private String password;

//getter setter 略

}

2、一般需要对操作的数据进行验证的可以创建一个验证器实现Validator接口

public class SignupValidator implements Validator {

    //邮箱验证正则表达式

    private static final String SIMPLE_EMAIL_REGEX = "[A-Z0-9._%+-]+@[A-Z0-9.-]+\\.[A-Z]{2,4}";

    @SuppressWarnings("rawtypes")

public boolean supports(Class aClass) {

        return SignupCommand.class.isAssignableFrom(aClass);

    }

    //执行验证

    public void validate(Object o, Errors errors) {

        SignupCommand command = (SignupCommand)o;

        ValidationUtils.rejectIfEmptyOrWhitespace(errors, "username", "error.username.empty", "Please specify a username.");

        ValidationUtils.rejectIfEmptyOrWhitespace(errors, "email", "error.email.empty", "Please specify an email address.");

        if( StringUtils.hasText( command.getEmail() ) && !Pattern.matches( SIMPLE_EMAIL_REGEX, command.getEmail().toUpperCase() ) ) {

            errors.rejectValue( "email", "error.email.invalid", "Please enter a valid email address." );

        }

        ValidationUtils.rejectIfEmptyOrWhitespace(errors, "password", "error.password.empty", "Please specify a password.");

    }

}


3、编写控制器

@Controller

public class SignupController {

    private SignupValidator signupValidator = new SignupValidator();

    private UserService userService;

    @Autowired

    public void setUserService(UserService userService) {

        this.userService = userService;

    }

    @RequestMapping(value="/signup",method= RequestMethod.GET)

    public String showSignupForm(Model model, @ModelAttribute SignupCommand command) {

return"signup";

    }

    @RequestMapping(value="/signup",method= RequestMethod.POST)

public String showSignupForm(Model model, @ModelAttribute SignupCommand command, BindingResult errors) {

        //数据验证是否合法

        signupValidator.validate(command, errors);

        if( errors.hasErrors() ) {

            return showSignupForm(model, command);

        }

        // Create the user

        userService.createUser( command.getUsername(), command.getEmail(), command.getPassword() );

        // Login the newly created user

        SecurityUtils.getSubject().login(new UsernamePasswordToken(command.getUsername(), command.getPassword()));

        return "redirect:/s/home";

    }

}







