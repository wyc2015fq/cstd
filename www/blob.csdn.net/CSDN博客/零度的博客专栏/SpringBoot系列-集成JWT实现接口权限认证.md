# SpringBoot系列-集成JWT实现接口权限认证 - 零度的博客专栏 - CSDN博客
2018年06月27日 10:43:01[零度anngle](https://me.csdn.net/zmx729618)阅读数：4241

原文出处：http://ju.outofmemory.cn/entry/341269
RESTful API认证方式 
 一般来讲，对于RESTful API都会有认证(Authentication)和授权(Authorization)过程，保证API的安全性。 
**  Authentication vs. Authorization  **
 Authentication指的是确定这个用户的身份，Authorization是确定该用户拥有什么操作权限。 
 认证方式一般有三种 
*  Basic Authentication  *
 这种方式是直接将用户名和密码放到Header中，使用  ` Authorization: Basic Zm9vOmJhcg== ` ，使用最简单但是最不安全。 
*  TOKEN认证  *
 这种方式也是再HTTP头中，使用  ` Authorization: Bearer <token> ` ，使用最广泛的TOKEN是JWT，通过签名过的TOKEN。 
*  OAuth2.0  *
 这种方式安全等级最高，但是也是最复杂的。如果不是大型API平台或者需要给第三方APP使用的，没必要整这么复杂。 
 一般项目中的RESTful API使用JWT来做认证就足够了。 
##  什么是JWT 
 Json web token (JWT), 是为了在网络应用环境间传递声明而执行的一种基于JSON的开放标准（(RFC 7519).该token被设计为紧凑且安全的，特别适用于分布式站点的单点登录（SSO）场景。JWT的声明一般被用来在身份提供者和服务提供者间传递被认证的用户身份信息，以便于从资源服务器获取资源，也可以增加一些额外的其它业务逻辑所必须的声明信息，该token也可直接被用于认证，也可被加密。 
 JWT官网：  [https://jwt.io/](https://jwt.io/)
 JWT是由三段信息构成的，将这三段信息文本用.链接一起就构成了Jwt字符串。就像这样:  
```
eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiYWRtaW4iOnRydWV9.TJVA95OrM7E2cBab30RMHrHDcEfxjoYZgeFONFh7HgQ
```
###  JWT的构成 
 第一部分我们称它为头部（header),第二部分我们称其为载荷（payload, 类似于飞机上承载的物品)，第三部分是签证（signature)。 
*  header  *
 jwt的头部承载两部分信息： 
-   声明类型，这里是jwt  
-   声明加密的算法 通常直接使用 HMAC SHA256  
 这里的加密算法是单向函数散列算法，常见的有MD5、SHA、HAMC。这里使用基于密钥的Hash算法HMAC生成散列值。 
-   MD5 message-digest algorithm 5 （信息-摘要算法）缩写，广泛用于加密和解密技术，常用于文件校验。校验？不管文件多大，经过MD5后都能生成唯一的MD5值  
-   SHA (Secure Hash Algorithm，安全散列算法），数字签名等密码学应用中重要的工具，安全性高于MD5  
-   HMAC (Hash Message Authentication Code，散列消息鉴别码，基于密钥的Hash算法的认证协议。用公开函数和密钥产生一个固定长度的值作为认证标识，用这个标识鉴别消息的完整性。常用于接口签名验证  
 完整的头部就像下面这样的JSON：  
```
{
  &apos;typ&apos;: &apos;JWT&apos;,
  &apos;alg&apos;: &apos;HS256&apos;
}
```
 然后将头部进行base64加密（该加密是可以对称解密的),构成了第一部分  
```
eyJ0eXAiOiJKV1QiLCJhbGciOiJIUzI1NiJ9
```
*  playload  *
 载荷就是存放有效信息的地方，这些有效信息包含三个部分： 
-   标准中注册的声明  
-   公共的声明  
-   私有的声明  
 标准中注册的声明 (建议但不强制使用) ： 
-   iss: jwt签发者  
-   sub: jwt所面向的用户  
-   aud: 接收jwt的一方  
-   exp: jwt的过期时间，这个过期时间必须要大于签发时间  
-   nbf: 定义在什么时间之前，该jwt都是不可用的.  
-   iat: jwt的签发时间  
-   jti: jwt的唯一身份标识，主要用来作为一次性token,从而回避重放攻击。  
 公共的声明： 
 公共的声明可以添加任何的信息，一般添加用户的相关信息或其他业务需要的必要信息.但不建议添加敏感信息，因为该部分在客户端可解密 
 私有的声明： 
 私有声明是提供者和消费者所共同定义的声明，一般不建议存放敏感信息，因为base64是对称解密的，意味着该部分信息可以归类为明文信息。 
 定义一个payload:  
```
{
  "sub": "1234567890",
  "name": "John Doe",
  "admin": true
}
```
 然后将其进行base64加密，得到Jwt的第二部分：  
```
eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiYWRtaW4iOnRydWV9
```
*  signature  *
 jwt的第三部分是一个签证信息，这个签证信息由三部分组成： 
```
header (base64后的)
payload (base64后的)
secret
```
 这个部分需要base64加密后的header和base64加密后的payload使用.连接组成的字符串，然后通过header中声明的加密方式进行加盐secret组合加密，然后就构成了jwt的第三部分。 
```
// javascript
var encodedString = base64UrlEncode(header) + '.' + base64UrlEncode(payload);
var signature = HMACSHA256(encodedString, 'secret'); // TJVA95OrM7E2cBab30RMHrHDcEfxjoYZgeFONFh7HgQ
```
 将这三部分用.连接成一个完整的字符串,构成了最终的jwt:  
```
eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJzdWIiOiIxMjM0NTY3ODkwIiwibmFtZSI6IkpvaG4gRG9lIiwiYWRtaW4iOnRydWV9.TJVA95OrM7E2cBab30RMHrHDcEfxjoYZgeFONFh7HgQ
```
 注意：secret是保存在服务器端的，jwt的签发生成也是在服务器端的，secret就是用来进行jwt的签发和jwt的验证，所以，它就是你服务端的私钥，在任何场景都不应该流露出去。一旦客户端得知这个secret, 那就意味着客户端是可以自我签发jwt了。 
###  如何应用 
 一般是在请求头里加入Authorization，并加上Bearer标注：  
```
fetch('api/user/1', {
  headers: {
    'Authorization': 'Bearer ' + token
  }
})
```
 服务器负责解析这个HTTP头来做用户认证和授权处理。大致流程如下： 
###  安全相关 
 JWT协议本身不具备安全传输功能，所以必须借助于SSL/TLS的安全通道，所以建议如下： 
-   不应该在jwt的payload部分存放敏感信息，因为该部分是客户端可解密的部分。  
-   保护好secret私钥，该私钥非常重要。  
-   如果可以，请使用https协议  
##  和SpringBoot集成 
 简要的说明下我们为什么要用JWT，因为我们要实现完全的前后端分离，所以不可能使用session，cookie的方式进行鉴权，所以JWT就被派上了用场，可以通过一个加密密钥来进行前后端的鉴权。 
 程序逻辑: 
-   我们POST用户名与密码到/login进行登入，如果成功返回一个加密token，失败的话直接返回401错误。  
-   之后用户访问每一个需要权限的网址请求必须在header中添加Authorization字段，例如Authorization: token，token为密钥。  
-   后台会进行token的校验，如果不通过直接返回401。  
 这里我讲一下如何在SpringBoot中使用JWT来做接口权限认证，安全框架依旧使用Shiro，JWT的实现使用  [jjwt](https://github.com/jwtk/jjwt)
###  添加Maven依赖 
```
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-web</artifactId>
    <exclusions>
        <exclusion>
            <groupId>org.springframework.boot</groupId>
            <artifactId>spring-boot-starter-tomcat</artifactId>
        </exclusion>
    </exclusions>
</dependency>
<dependency>
    <groupId>org.springframework.boot</groupId>
    <artifactId>spring-boot-starter-jetty</artifactId>
</dependency>
<dependency>
    <groupId>com.auth0</groupId>
    <artifactId>java-jwt</artifactId>
    <version>3.3.0</version>
</dependency>
<!-- shiro 权限控制 -->
<dependency>
    <groupId>org.apache.shiro</groupId>
    <artifactId>shiro-spring</artifactId>
    <version>1.4.0</version>
    <exclusions>
        <exclusion>
            <artifactId>slf4j-api</artifactId>
            <groupId>org.slf4j</groupId>
        </exclusion>
    </exclusions>
</dependency>
<!-- shiro ehcache (shiro缓存)-->
<dependency>
    <groupId>org.apache.shiro</groupId>
    <artifactId>shiro-ehcache</artifactId>
    <version>1.4.0</version>
    <exclusions>
        <exclusion>
            <artifactId>slf4j-api</artifactId>
            <groupId>org.slf4j</groupId>
        </exclusion>
    </exclusions>
</dependency>
```
###  创建用户Service 
 这个在shiro一节讲过如果创建角色权限表，添加用户Service来执行查找用户操作，这里就不多讲具体实现了，只列出关键代码： 
```
/**
 * 通过名称查找用户
 *
 * @param username
 * @return
 */
public ManagerInfo findByUsername(String username) {
    ManagerInfo managerInfo = managerInfoDao.findByUsername(username);
    if (managerInfo == null) {
        throw new UnknownAccountException();
    }
    return managerInfo;
}
```
 用户信息类：  
```
public class ManagerInfo implements Serializable {
    private static final long serialVersionUID = 1L;
    /**
     * 主键ID
     */
    private Integer id;
    /**
     * 账号
     */
    private String username;
    /**
     * 密码
     */
    private String password;
    /**
     * md5密码盐
     */
    private String salt;
    /**
     * 一个管理员具有多个角色
     */
    private List<SysRole> roles;
```
###  JWT工具类 
 我们写一个简单的JWT加密，校验工具，并且使用用户自己的密码充当加密密钥，这样保证了token 即使被他人截获也无法破解。并且我们在token中附带了username信息，并且设置密钥5分钟就会过期。 
```
public class JWTUtil {
    // 过期时间5分钟
    private static final long EXPIRE_TIME = 5 * 60 * 1000;
    /**
     * 校验token是否正确
     *
     * @param token  密钥
     * @param secret 用户的密码
     * @return 是否正确
     */
    public static boolean verify(String token, String username, String secret) {
        try {
            Algorithm algorithm = Algorithm.HMAC256(secret);
            JWTVerifier verifier = JWT.require(algorithm)
                    .withClaim("username", username)
                    .build();
            DecodedJWT jwt = verifier.verify(token);
            return true;
        } catch (Exception exception) {
            return false;
        }
    }
    /**
     * 获得token中的信息无需secret解密也能获得
     *
     * @return token中包含的用户名
     */
    public static String getUsername(String token) {
        try {
            DecodedJWT jwt = JWT.decode(token);
            return jwt.getClaim("username").asString();
        } catch (JWTDecodeException e) {
            return null;
        }
    }
    /**
     * 生成签名,5min后过期
     *
     * @param username 用户名
     * @param secret   用户的密码
     * @return 加密的token
     */
    public static String sign(String username, String secret) {
        try {
            Date date = new Date(System.currentTimeMillis() + EXPIRE_TIME);
            Algorithm algorithm = Algorithm.HMAC256(secret);
            // 附带username信息
            return JWT.create()
                    .withClaim("username", username)
                    .withExpiresAt(date)
                    .sign(algorithm);
        } catch (UnsupportedEncodingException e) {
            return null;
        }
    }
}
```
###  编写登录接口 
 为了让用户登录的时候获取到正确的JWT Token，需要实现登录接口，这里我编写一个  ` LoginController.java ` ： 
```
@RestController
public class LoginController {
    @Resource
    private ManagerInfoService managerInfoService;
    private static final Logger _logger = LoggerFactory.getLogger(LoginController.class);
    @PostMapping("/login")
    public BaseResponse login(@RequestParam("username") String username,
                              @RequestParam("password") String password) {
        ManagerInfo user = managerInfoService.findByUsername(username);
        //盐（用户名+随机数）
        String salt = user.getSalt();
        //原密码
        String encodedPassword = ShiroKit.md5(password, username + salt);
        if (user.getPassword().equals(encodedPassword)) {
            return new BaseResponse(true, "Login success", JWTUtil.sign(username, encodedPassword));
        } else {
            throw new UnauthorizedException();
        }
    }
    @RequestMapping(path = "/401")
    @ResponseStatus(HttpStatus.UNAUTHORIZED)
    public BaseResponse unauthorized() {
        return new BaseResponse(false, "Unauthorized", null);
    }
}
```
 注意上面登录的时候，我会从数据库中把这个用户取出来，密码加盐算MD5值比较，通过之后再用密码作为密钥来签名生成JWT。 
###  编写RESTful接口 
 先编写一个通用的接口返回类： 
```
/**
 * API接口的基础返回类
 *
 * @author XiongNeng
 * @version 1.0
 * @since 2018/1/7
 */
public class BaseResponse<T> {
    /**
     * 是否成功
     */
    private boolean success;
    /**
     * 说明
     */
    private String msg;
    /**
     * 返回数据
     */
    private T data;
    public BaseResponse() {
    }
    public BaseResponse(boolean success, String msg, T data) {
        this.success = success;
        this.msg = msg;
        this.data = data;
    }
}
```
 通过SpringMVC实现RESTful接口，这里我只写一个示例方法： 
```
@RestController
@RequestMapping(value = "/api/v1")
public class PublicController {
    @Resource
    private ApiService apiService;
    /**
     * 请求入网接口
     *
     * @return 处理结果
     */
    @RequestMapping(value = "/join", method = RequestMethod.POST)
    @RequiresRoles("admin")
    public ResponseEntity<BaseResponse> doJoin(@RequestBody PosParam posParam) {
        _logger.info("请求入网接口 start....");
        BaseResponse result = new BaseResponse();
        // imei码约束检查
        if (StringUtils.isEmpty(posParam.getImei()) || posParam.getImei().length() > 32) {
            result.setSuccess(false);
            result.setMsg("IMEI码长度不是1-32位，入网失败。");
            return new ResponseEntity<>(result, HttpStatus.OK);
        }
        Pos pos = new Pos();
        Date now = new Date();
        pos.setJointime(now);
        pos.setBindtime(now);
        BeanUtils.copyProperties(posParam, pos);
        // 插入一条新纪录
        pos.setProjectId(project.getId());
        int insert = apiService.insertPos(pos);
        if (insert > 0) {
            result.setSuccess(true);
            result.setMsg("入网成功");
            return new ResponseEntity<>(result, HttpStatus.CREATED);
        } else {
            result.setSuccess(false);
            result.setMsg("入网失败，请联系管理员。");
            return new ResponseEntity<>(result, HttpStatus.OK);
        }
    }
}
```
###  自定义异常 
 为了实现我自己能够手动抛出异常，我自己写了一个  ` UnauthorizedException.java `
```
public class UnauthorizedException extends RuntimeException {
    public UnauthorizedException(String msg) {
        super(msg);
    }
    public UnauthorizedException() {
        super();
    }
}
```
###  处理框架异常 
 之前说过restful要统一返回的格式，所以我们也要全局处理Spring Boot的抛出异常。利用@RestControllerAdvice能很好的实现。注意这个统一异常处理器只对认证过的用户调用接口中的异常有作用，对AuthenticationException没有用 
```
@RestControllerAdvice
public class ExceptionController {
    // 捕捉shiro的异常
    @ResponseStatus(HttpStatus.UNAUTHORIZED)
    @ExceptionHandler(ShiroException.class)
    public BaseResponse handle401(ShiroException e) {
        return new BaseResponse(false, "shiro的异常", null);
    }
    
    // 捕捉UnauthorizedException
    @ResponseStatus(HttpStatus.UNAUTHORIZED)
    @ExceptionHandler(UnauthorizedException.class)
    public BaseResponse handle401() {
        return new BaseResponse(false, "UnauthorizedException", null);
    }
    // 捕捉其他所有异常
    @ExceptionHandler(Exception.class)
    @ResponseStatus(HttpStatus.BAD_REQUEST)
    public BaseResponse globalException(HttpServletRequest request, Throwable ex) {
        return new BaseResponse(false, "其他异常", null);
    }
    private HttpStatus getStatus(HttpServletRequest request) {
        Integer statusCode = (Integer) request.getAttribute("javax.servlet.error.status_code");
        if (statusCode == null) {
            return HttpStatus.INTERNAL_SERVER_ERROR;
        }
        return HttpStatus.valueOf(statusCode);
    }
}
```
###  配置Shiro 
 大家可以先看下官方的  [Spring-Shiro](http://shiro.apache.org/spring.html) 整合教程，有个初步的了解。不过既然我们用了SpringBoot，那我们肯定要争取零配置文件。 
*  实现JWTToken  *
 JWTToken差不多就是Shiro用户名密码的载体。因为我们是前后端分离，服务器无需保存用户状态，所以不需要RememberMe这类功能，我们简单的实现下AuthenticationToken接口即可。因为token自己已经包含了用户名等信息，所以这里我就弄了一个字段。如果你喜欢钻研，可以看看官方的UsernamePasswordToken是如何实现的。 
```
public class JWTToken implements AuthenticationToken {
    // 密钥
    private String token;
    public JWTToken(String token) {
        this.token = token;
    }
    @Override
    public Object getPrincipal() {
        return token;
    }
    @Override
    public Object getCredentials() {
        return token;
    }
}
```
*  实现Realm  *
 realm的用于处理用户是否合法的这一块，需要我们自己实现。 
```
/**
 * Description  : 身份校验核心类
 */
public class MyShiroRealm extends AuthorizingRealm {
    private static final Logger _logger = LoggerFactory.getLogger(MyShiroRealm.class);
    @Autowired
    ManagerInfoService managerInfoService;
    /**
     * JWT签名密钥，这里没用。我使用的是用户的MD5密码作为签名密钥
     */
    public static final String SECRET = "9281e268b77b7c439a20b46fd1483b9a";
    /**
     * 必须重写此方法，不然Shiro会报错
     */
    @Override
    public boolean supports(AuthenticationToken token) {
        return token instanceof JWTToken;
    }
    /**
     * 认证信息(身份验证)
     * Authentication 是用来验证用户身份
     *
     * @param auth
     * @return
     * @throws AuthenticationException
     */
    @Override
    protected AuthenticationInfo doGetAuthenticationInfo(AuthenticationToken auth)
            throws AuthenticationException {
        _logger.info("MyShiroRealm.doGetAuthenticationInfo()");
        String token = (String) auth.getCredentials();
        // 解密获得username，用于和数据库进行对比
        String username = JWTUtil.getUsername(token);
        if (username == null) {
            throw new AuthenticationException("token invalid");
        }
        //通过username从数据库中查找 ManagerInfo对象
        //实际项目中，这里可以根据实际情况做缓存，如果不做，Shiro自己也是有时间间隔机制，2分钟内不会重复执行该方法
        ManagerInfo managerInfo = managerInfoService.findByUsername(username);
        if (managerInfo == null) {
            throw new AuthenticationException("User didn't existed!");
        }
        if (!JWTUtil.verify(token, username, managerInfo.getPassword())) {
            throw new AuthenticationException("Username or password error");
        }
        return new SimpleAuthenticationInfo(token, token, "my_realm");
    }
    /**
     * 此方法调用hasRole,hasPermission的时候才会进行回调.
     * <p>
     * 权限信息.(授权):
     * 1、如果用户正常退出，缓存自动清空；
     * 2、如果用户非正常退出，缓存自动清空；
     * 3、如果我们修改了用户的权限，而用户不退出系统，修改的权限无法立即生效。
     * （需要手动编程进行实现；放在service进行调用）
     * 在权限修改后调用realm中的方法，realm已经由spring管理，所以从spring中获取realm实例，调用clearCached方法；
     * :Authorization 是授权访问控制，用于对用户进行的操作授权，证明该用户是否允许进行当前操作，如访问某个链接，某个资源文件等。
     *
     * @param principals
     * @return
     */
    @Override
    protected AuthorizationInfo doGetAuthorizationInfo(PrincipalCollection principals) {
        /*
         * 当没有使用缓存的时候，不断刷新页面的话，这个代码会不断执行，
         * 当其实没有必要每次都重新设置权限信息，所以我们需要放到缓存中进行管理；
         * 当放到缓存中时，这样的话，doGetAuthorizationInfo就只会执行一次了，
         * 缓存过期之后会再次执行。
         */
        _logger.info("权限配置-->MyShiroRealm.doGetAuthorizationInfo()");
        String username = JWTUtil.getUsername(principals.toString());
        // 下面的可以使用缓存提升速度
        ManagerInfo managerInfo = managerInfoService.findByUsername(username);
        SimpleAuthorizationInfo authorizationInfo = new SimpleAuthorizationInfo();
        //设置相应角色的权限信息
        for (SysRole role : managerInfo.getRoles()) {
            //设置角色
            authorizationInfo.addRole(role.getRole());
            for (Permission p : role.getPermissions()) {
                //设置权限
                authorizationInfo.addStringPermission(p.getPermission());
            }
        }
        return authorizationInfo;
    }
}
```
 在  ` doGetAuthenticationInfo ` 中用户可以自定义抛出很多异常，详情见文档。 
*  重写Filter  *
 所有的请求都会先经过Filter，所以我们继承官方的  ` BasicHttpAuthenticationFilter ` ，并且重写鉴权的方法，另外通过重写preHandle，实现跨越访问。 
 代码的执行流程  ` preHandle->isAccessAllowed->isLoginAttempt->executeLogin `
```
public class JWTFilter extends BasicHttpAuthenticationFilter {
    private Logger LOGGER = LoggerFactory.getLogger(this.getClass());
    /**
     * 判断用户是否想要登入。
     * 检测header里面是否包含Authorization字段即可
     */
    @Override
    protected boolean isLoginAttempt(ServletRequest request, ServletResponse response) {
        HttpServletRequest req = (HttpServletRequest) request;
        String authorization = req.getHeader("Authorization");
        return authorization != null;
    }
    /**
     *
     */
    @Override
    protected boolean executeLogin(ServletRequest request, ServletResponse response) throws Exception {
        HttpServletRequest httpServletRequest = (HttpServletRequest) request;
        String authorization = httpServletRequest.getHeader("Authorization");
        JWTToken token = new JWTToken(authorization);
        // 提交给realm进行登入，如果错误他会抛出异常并被捕获
        getSubject(request, response).login(token);
        // 如果没有抛出异常则代表登入成功，返回true
        return true;
    }
    /**
     * 这里我们详细说明下为什么最终返回的都是true，即允许访问
     * 例如我们提供一个地址 GET /article
     * 登入用户和游客看到的内容是不同的
     * 如果在这里返回了false，请求会被直接拦截，用户看不到任何东西
     * 所以我们在这里返回true，Controller中可以通过 subject.isAuthenticated() 来判断用户是否登入
     * 如果有些资源只有登入用户才能访问，我们只需要在方法上面加上 @RequiresAuthentication 注解即可
     * 但是这样做有一个缺点，就是不能够对GET,POST等请求进行分别过滤鉴权(因为我们重写了官方的方法)，但实际上对应用影响不大
     */
    @Override
    protected boolean isAccessAllowed(ServletRequest request, ServletResponse response, Object mappedValue) {
        if (isLoginAttempt(request, response)) {
            try {
                executeLogin(request, response);
            } catch (Exception e) {
                response401(request, response);
            }
        }
        return true;
    }
    /**
     * 对跨域提供支持
     */
    @Override
    protected boolean preHandle(ServletRequest request, ServletResponse response) throws Exception {
        HttpServletRequest httpServletRequest = (HttpServletRequest) request;
        HttpServletResponse httpServletResponse = (HttpServletResponse) response;
        httpServletResponse.setHeader("Access-control-Allow-Origin", httpServletRequest.getHeader("Origin"));
        httpServletResponse.setHeader("Access-Control-Allow-Methods", "GET,POST,OPTIONS,PUT,DELETE");
        httpServletResponse.setHeader("Access-Control-Allow-Headers", httpServletRequest.getHeader("Access-Control-Request-Headers"));
        // 跨域时会首先发送一个option请求，这里我们给option请求直接返回正常状态
        if (httpServletRequest.getMethod().equals(RequestMethod.OPTIONS.name())) {
            httpServletResponse.setStatus(HttpStatus.OK.value());
            return false;
        }
        return super.preHandle(request, response);
    }
    /**
     * 将非法请求跳转到 /401
     */
    private void response401(ServletRequest req, ServletResponse resp) {
        try {
            HttpServletResponse httpServletResponse = (HttpServletResponse) resp;
            httpServletResponse.sendRedirect("/401");
        } catch (IOException e) {
            LOGGER.error(e.getMessage());
        }
    }
}
```
*  编写ShiroConfig配置类  *
 这里我还增加了EhCache缓存管理支持，不需要每次都调用数据库做授权。 
```
@Configuration
@Order(1)
public class ShiroConfig {
    /**
     * ShiroFilterFactoryBean 处理拦截资源文件问题。
     * 注意：单独一个ShiroFilterFactoryBean配置是或报错的，以为在
     * 初始化ShiroFilterFactoryBean的时候需要注入：SecurityManager Filter Chain定义说明
     * 1、一个URL可以配置多个Filter，使用逗号分隔
     * 2、当设置多个过滤器时，全部验证通过，才视为通过
     * 3、部分过滤器可指定参数，如perms，roles
     */
    @Bean
    public ShiroFilterFactoryBean shirFilter(SecurityManager securityManager) {
        ShiroFilterFactoryBean shiroFilterFactoryBean = new ShiroFilterFactoryBean();
        // 必须设置 SecurityManager
        shiroFilterFactoryBean.setSecurityManager(securityManager);
        //验证码过滤器
        Map<String, Filter> filtersMap = shiroFilterFactoryBean.getFilters();
        filtersMap.put("jwt", new JWTFilter());
        shiroFilterFactoryBean.setFilters(filtersMap);
        // 拦截器
        Map<String, String> filterChainDefinitionMap = new LinkedHashMap<String, String>();
        // 其他的
        filterChainDefinitionMap.put("/**", "jwt");
        // 访问401和404页面不通过我们的Filter
        filterChainDefinitionMap.put("/401", "anon");
        filterChainDefinitionMap.put("/404", "anon");
        shiroFilterFactoryBean.setFilterChainDefinitionMap(filterChainDefinitionMap);
        return shiroFilterFactoryBean;
    }
    @Bean
    public SecurityManager securityManager() {
        DefaultWebSecurityManager securityManager = new DefaultWebSecurityManager();
        // 设置realm.
        securityManager.setRealm(myShiroRealm());
        //注入缓存管理器
        securityManager.setCacheManager(ehCacheManager());
        // 关闭shiro自带的session
        DefaultSubjectDAO subjectDAO = new DefaultSubjectDAO();
        DefaultSessionStorageEvaluator defaultSessionStorageEvaluator = new DefaultSessionStorageEvaluator();
        defaultSessionStorageEvaluator.setSessionStorageEnabled(false);
        subjectDAO.setSessionStorageEvaluator(defaultSessionStorageEvaluator);
        securityManager.setSubjectDAO(subjectDAO);
        return securityManager;
    }
    /**
     * 身份认证realm; (这个需要自己写，账号密码校验；权限等)
     */
    @Bean
    public MyShiroRealm myShiroRealm() {
        MyShiroRealm myShiroRealm = new MyShiroRealm();
        return myShiroRealm;
    }
    /**
     * 开启shiro aop注解支持. 使用代理方式; 所以需要开启代码支持;
     *
     * @param securityManager 安全管理器
     * @return 授权Advisor
     */
    @Bean
    public AuthorizationAttributeSourceAdvisor authorizationAttributeSourceAdvisor(SecurityManager securityManager) {
        AuthorizationAttributeSourceAdvisor authorizationAttributeSourceAdvisor = new AuthorizationAttributeSourceAdvisor();
        authorizationAttributeSourceAdvisor.setSecurityManager(securityManager);
        return authorizationAttributeSourceAdvisor;
    }
    /**
     * shiro缓存管理器;
     * 需要注入对应的其它的实体类中：
     * 1、安全管理器：securityManager
     * 可见securityManager是整个shiro的核心；
     *
     * @return
     */
    @Bean
    public EhCacheManager ehCacheManager() {
        EhCacheManager cacheManager = new EhCacheManager();
        cacheManager.setCacheManagerConfigFile("classpath:ehcache.xml");
        return cacheManager;
    }
}
```
 里面URL规则自己参考文档  [http://shiro.apache.org/web.html](http://shiro.apache.org/web.html) ，这个在shiro那篇说的很清楚了。 
##  运行验证 
 最后是将代码跑起来验证这一切是否正常。 
 启动SpringBoot后，先通过POST请求登录拿到token 
 然后在调用入网接口的时候在header中带上这个token认证： 
 如果token认证不正确会报异常： 
 如果使用普通用户登录，认证正确但是授权访问接口失败，会返回如下的未授权结果： 
##  参考文章 
- [RESTful API Authentication Basics](http://blog.restcase.com/restful-api-authentication-basics/)
- [什么是 JWT – JSON WEB TOKEN](https://www.jianshu.com/p/576dbf44b2ae)
- [Shiro+JWT+Spring Boot Restful简易教程](https://juejin.im/post/59f1b2766fb9a0450e755993)
##  RESTful API认证方式 
 一般来讲，对于RESTful API都会有认证(Authentication)和授权(Authorization)过程，保证API的安全性。 
**  Authentication vs. Authorization  **
 Authentication指的是确定这个用户的身份，Authorization是确定该用户拥有什么操作权限。 
 认证方式一般有三种 
*  Basic Authentication  *
 这种方式是直接将用户名和密码放到Header中，使用  ` Authorization: Basic Zm9vOmJhcg== ` ，使用最简单但是最不安全。 
*  TOKEN认证  *
 这种方式也是再HTTP头中，使用  ` Authorization: Bearer <token> ` ，使用最广泛的TOKEN是JWT，通过签名过的TOKEN。 
*  OAuth2.0  *
 这种方式安全等级最高，但是也是最复杂的。如果不是大型API平台或者需要给第三方APP使用的，没必要整这么复杂。 
 一般项目中的RESTful API使用JWT来做认证就足够了。 
