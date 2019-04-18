# SpringMVC开启CORS支持 - z69183787的专栏 - CSDN博客
2016年11月09日 17:27:16[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3791
### **前言**
浏览器出于安全考虑，限制了JS发起跨站请求，使用XHR对象发起请求必须遵循同源策略（SOP：Same Origin Policy），跨站请求会被浏览器阻止，这对开发者来说是很痛苦的一件事，尤其是要开发前后端分离的应用时。
在现代化的Web开发中，不同网络环境下的资源数据共享越来越普遍，同源策略可以说是在一定程度上限制了Web API的发展。
简单的说，CORS就是为了AJAX能够安全跨域而生的。至于CORS的安全性研究，本文不做探讨。
### **目录**
- 
CORS浅述
- 
如何使用？CORS的HTTP头
- 
初始项目准备
- 
CorsFilter: 过滤器阶段的CORS
- 
CorsInterceptor: 拦截器阶段的CORS
- 
@CrossOrigin：Handler阶段的CORS
- 
小结
- 
追求极致的开发体验：整合第三方CORSFilter
- 
示例代码下载
### **CORS浅述**
名词解释：跨域资源共享（Cross-Origin Resource Sharing）
概念：是一种跨域机制、规范、标准，怎么叫都一样，但是这套标准是针对服务端的，而浏览器端只要支持HTML5即可。
作用：可以让服务端决定哪些请求源可以进来拿数据，所以服务端起主导作用（所以出了事找后台程序猿，无关前端^ ^）
常用场景：
- 前后端完全分离的应用，比如Hybrid App
- 开放式只读API，JS能够自由访问，比如地图、天气、时间……
### **如何使用？CORS的HTTP头**
> 
要实现CORS跨域其实非常简单，说白了就是在服务端设置一系列的HTTP头，主要分为请求头和响应头，在请求和响应时加上这些HTTP头即可轻松实现CORS
请求头和响应头信息都是在服务端设置好的，一般在Filter阶段设置，浏览器端不用关心，唯一要设置的地方就是：跨域时是否要携带cookie
- HTTP请求头：
```
#请求域
Origin: ”http://localhost:3000“
#这两个属性只出现在预检请求中，即OPTIONS请求
Access-Control-Request-Method: ”POST“
Access-Control-Request-Headers: ”content-type“
```
- HTTP响应头：
```cpp
```cpp
#允许向该服务器提交请求的URI，*表示全部允许，在SpringMVC中，如果设成*，会自动转成当前请求头中的Origin
Access-Control-Allow-Origin: ”http://localhost:3000“
#允许访问的头信息
Access-Control-Expose-Headers: "Set-Cookie"
#预检请求的缓存时间（秒），即在这个时间段里，对于相同的跨域请求不会再预检了
Access-Control-Max-Age: ”1800”
#允许Cookie跨域，在做登录校验的时候有用
Access-Control-Allow-Credentials: “true”
#允许提交请求的方法，*表示全部允许
Access-Control-Allow-Methods:GET,POST,PUT,DELETE,PATCH
```
```
### **初始项目准备**
- 
补充一下，对于简单跨域和非简单跨域，可以这么理解：
> - 简单跨域就是GET，HEAD和POST请求，但是POST请求的"Content-Type"只能是application/x-www-form-urlencoded, multipart/form-data 或 text/plain
- 反之，就是非简单跨域，此跨域有一个预检机制，说直白点，就是会发两次请求，一次OPTIONS请求，一次真正的请求
- 
首先新建一个静态web项目，定义三种类型的请求：简单跨域请求，非简单跨域请求，带Cookie信息的请求(做登录校验)。代码如下：
```xml
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <title>跨域demo</title>
    <link rel="stylesheet" href="node_modules/amazeui/dist/css/amazeui.min.css">
</head>
<body class="am-container">
<!--简单跨域-->
<button class="am-btn am-btn-primary" onclick="getUsers(this)">
    简单跨域: 获取用户列表
</button>
<p class="am-text-danger"></p>
<!--非简单跨域-->
<button class="am-btn am-btn-primary" onclick="addUser(this)">
    非简单跨域: 添加用户(JSON请求)
</button>
<input type="text" placeholder="用户名">
<p class="am-text-danger"></p>
<!--检查是否登录-->
<button class="am-btn am-btn-primary am-margin-right" onclick="checkLogin(this)">
    登录校验
</button>
<p class="am-text-danger"></p>
<!--登录-->
<button class="am-btn am-btn-primary" onclick="login(this)">
    登录
</button>
<input type="text" placeholder="用户名">
<p class="am-text-danger"></p>
</body>
<script src="node_modules/jquery/dist/jquery.min.js"></script>
<script src="node_modules/amazeui/dist/js/amazeui.js"></script>
<script>
    function getUsers(btn) {
        var $btn = $(btn);
        $.ajax({
            type: 'get',
            url: 'http://localhost:8080/api/users',
            contentType: "application/json;charset=UTF-8"
        }).then(
                function (obj) {
                    $btn.next('p').html(JSON.stringify(obj));
                },
                function () {
                    $btn.next('p').html('error...');
                }
        )
    }
    function addUser(btn) {
        var $btn = $(btn);
        var name = $btn.next('input').val();
        if (!name) {
            $btn.next('input').next('p').html('用户名不能为空');
            return;
        }
        $.ajax({
            type: 'post',
            url: 'http://localhost:8080/api/users',
            contentType: "application/json;charset=UTF-8",
            data: name,
            dataType: 'json'
        }).then(
                function (obj) {
                    $btn.next('input').next('p').html(JSON.stringify(obj));
                },
                function () {
                    $btn.next('input').next('p').html('error...');
                }
        )
    }
    function checkLogin(btn) {
        var $btn = $(btn);
        $.ajax({
            type: 'get',
            url: 'http://localhost:8080/api/user/login',
            contentType: "application/json;charset=UTF-8",
            xhrFields: {
                withCredentials: true
            }
        }).then(
                function (obj) {
                    $btn.next('p').html(JSON.stringify(obj));
                },
                function () {
                    $btn.next('p').html('error...');
                }
        )
    }
    function login(btn) {
        var $btn = $(btn);
        var name = $btn.next('input').val();
        if (!name) {
            $btn.next('input').next('p').html('用户名不能为空');
            return;
        }
        $.ajax({
            type: 'post',
            url: 'http://localhost:8080/api/user/login',
            contentType: "application/json;charset=UTF-8",
            data: name,
            dataType: 'json',
            xhrFields: {
                withCredentials: true
            }
        }).then(
                function (obj) {
                    $btn.next('input').next('p').html(JSON.stringify(obj));
                },
                function () {
                    $btn.next('input').next('p').html('error...');
                }
        )
    }
</script>
</html>
```
- 然后启动web项目（这里推荐一个所见即所得工具：browser-sync）
``browser-sync start --server --files "*.html"``- 
接来下，做服务端的事情，新建一个SpringMVC项目，这里推荐一个自动生成Spring种子项目的网站：[http://start.spring.io/](http://start.spring.io/)
![](http://7xpjn9.com1.z0.glb.clouddn.com/4.53.33.png)
种子项目
- 
项目结构如下：
![](http://7xpjn9.com1.z0.glb.clouddn.com/image/1.57.34.png)
项目结构
- 
在pom.xml中引入lombok和guava
```xml
<dependency>
    <groupId>com.google.guava</groupId>
    <artifactId>guava</artifactId>
    <version>19.0</version>
</dependency>
<dependency>
    <groupId>org.projectlombok</groupId>
    <artifactId>lombok</artifactId>
    <version>1.16.8</version>
</dependency>
```
- 模拟数据源：UserDB
```
public class UserDB {
    public static Cache<String, User> userdb = CacheBuilder.newBuilder().expireAfterWrite(1, TimeUnit.DAYS).build();
    static {
        String id1 = UUID.randomUUID().toString();
        String id2 = UUID.randomUUID().toString();
        String id3 = UUID.randomUUID().toString();
        userdb.put(id1, new User(id1, "jear"));
        userdb.put(id2, new User(id2, "tom"));
        userdb.put(id3, new User(id3, "jack"));
    }
}
```
- 编写示例控制器：UserController
```java
```java
@RestController
@RequestMapping("/users")
public class UserController {
    @RequestMapping(method = RequestMethod.GET)
    List<User> getList() {
        return Lists.newArrayList(userdb.asMap().values());
    }
    @RequestMapping(method = RequestMethod.POST)
    List<String> add(@RequestBody String name) {
        if (userdb.asMap().values().stream().anyMatch(user -> user.getName().equals(name))) {
            return Lists.newArrayList("添加失败, 用户名'" + name + "'已存在");
        }
        String id = UUID.randomUUID().toString();
        userdb.put(id, new User(id, name));
        return Lists.newArrayList("添加成功: " + userdb.getIfPresent(id));
    }
}
```
```
- 编写示例控制器：UserLoginController
```
@RestController
@RequestMapping("/user/login")
public class UserLoginController {
    @RequestMapping(method = RequestMethod.GET)
    Object getInfo(HttpSession session) {
        Object object = session.getAttribute("loginer");
        return object == null ? Lists.newArrayList("未登录") : object;
    }
    @RequestMapping(method = RequestMethod.POST)
    List<String> login(HttpSession session, @RequestBody String name) {
        Optional<User> user = userdb.asMap().values().stream().filter(user1 -> user1.getName().equals(name)).findAny();
        if (user.isPresent()) {
            session.setAttribute("loginer", user.get());
            return Lists.newArrayList("登录成功!");
        }
        return Lists.newArrayList("登录失败, 找不到用户名:" + name);
    }
}
```
- 最后启动服务端项目
```
mvn clean package
debug模式启动Application
```
- 到这里，主要工作都完成了，打开浏览器，访问静态web项目，打开控制台，发现Ajax请求无法获取数据，这就是同源策略的限制
- 下面我们一步步来开启服务端的CORS支持
### **CorsFilter: 过滤器阶段的CORS**
```
@Configuration
public class WebConfig extends WebMvcConfigurerAdapter {
    @Bean
    public FilterRegistrationBean filterRegistrationBean() {
        // 对响应头进行CORS授权
        MyCorsRegistration corsRegistration = new MyCorsRegistration("/**");
        corsRegistration.allowedOrigins(CrossOrigin.DEFAULT_ORIGINS)
                .allowedMethods(HttpMethod.GET.name(), HttpMethod.HEAD.name(), HttpMethod.POST.name(), HttpMethod.PUT.name())
                .allowedHeaders(CrossOrigin.DEFAULT_ALLOWED_HEADERS)
                .exposedHeaders(HttpHeaders.SET_COOKIE)
                .allowCredentials(CrossOrigin.DEFAULT_ALLOW_CREDENTIALS)
                .maxAge(CrossOrigin.DEFAULT_MAX_AGE);
        // 注册CORS过滤器
        UrlBasedCorsConfigurationSource configurationSource = new UrlBasedCorsConfigurationSource();
        configurationSource.registerCorsConfiguration("/**", corsRegistration.getCorsConfiguration());
        CorsFilter corsFilter = new CorsFilter(configurationSource);
        return new FilterRegistrationBean(corsFilter);
    }
}
```
- 现在测试一下“简单跨域”和“非简单跨域”，已经可以正常响应了
![](http://7xpjn9.com1.z0.glb.clouddn.com/image/4.04.42.png)
浏览器图片
- 
再来测试一下 “登录校验” 和 “登录”，看看cookie是否能正常跨域
![](http://7xpjn9.com1.z0.glb.clouddn.com/4.15.04.png)
浏览器图片
- 
如果把服务端的allowCredentials设为false，或者ajax请求中不带{withCredentials: true}，那么登录校验永远都是未登录，因为cookie没有在浏览器和服务器之间传递
### **CorsInterceptor: 拦截器阶段的CORS**
> 
既然已经有了Filter级别的CORS，为什么还要CorsInterceptor呢？因为控制粒度不一样！Filter是任意Servlet的前置过滤器，而Inteceptor只对DispatcherServlet下的请求拦截有效，它是请求进入Handler的最后一道防线，如果再设置一层Inteceptor防线，可以增强安全性和可控性。
关于这个阶段的CORS，不得不吐槽几句，Spring把CorsInteceptor写死在了拦截器链上的最后一个，也就是说如果我有自定义的Interceptor，请求一旦被我自己的拦截器拦截下来，则只能通过CorsFilter授权跨域，压根走不到CorsInterceptor，至于为什么，下面会讲到。
所以说CorsInterceptor是专为授权Handler中的跨域而写的。
废话不多说，直接上代码：
```
@Configuration
public class WebConfig extends WebMvcConfigurerAdapter {
    @Bean
    public FilterRegistrationBean corsFilterRegistrationBean() {
        // 对响应头进行CORS授权
        MyCorsRegistration corsRegistration = new MyCorsRegistration("/**");
        this._configCorsParams(corsRegistration);
        // 注册CORS过滤器
        UrlBasedCorsConfigurationSource configurationSource = new UrlBasedCorsConfigurationSource();
        configurationSource.registerCorsConfiguration("/**", corsRegistration.getCorsConfiguration());
        CorsFilter corsFilter = new CorsFilter(configurationSource);
        return new FilterRegistrationBean(corsFilter);
    }
    @Override
    public void addCorsMappings(CorsRegistry registry) {
        // 配置CorsInterceptor的CORS参数
        this._configCorsParams(registry.addMapping("/**"));
    }
    private void _configCorsParams(CorsRegistration corsRegistration) {
        corsRegistration.allowedOrigins(CrossOrigin.DEFAULT_ORIGINS)
                .allowedMethods(HttpMethod.GET.name(), HttpMethod.HEAD.name(), HttpMethod.POST.name(), HttpMethod.PUT.name())
                .allowedHeaders(CrossOrigin.DEFAULT_ALLOWED_HEADERS)
                .exposedHeaders(HttpHeaders.SET_COOKIE)
                .allowCredentials(CrossOrigin.DEFAULT_ALLOW_CREDENTIALS)
                .maxAge(CrossOrigin.DEFAULT_MAX_AGE);
    }
}
```
- 打开浏览器，效果和上面一样
### **@CrossOrigin：Handler阶段的CORS**
如果把前面的代码认真写一遍，应该已经发现这个注解了，这个注解是用在控制器方法上的，其实Spring在这里用的还是CorsInterceptor，做最后一层拦截，这也就解释了为什么CorsInterceptor永远是最后一个执行的拦截器。
这是最小控制粒度了，可以精确到某个请求的跨域控制
```java
```java
// 先把WebConfig中前两阶段的配置注释掉，再到这里加跨域注解
@CrossOrigin(origins = "http://localhost:3000")
@RequestMapping(method = RequestMethod.GET)
List<User> getList() {
    return Lists.newArrayList(userdb.asMap().values());
}
```
```
- 打开浏览器，发现只有第一个请求可以正常跨域
![](http://7xpjn9.com1.z0.glb.clouddn.com/5.04.50.png)
Handler跨域
### **小结**
三个阶段的CORS配置顺序是后面叠加到前面，而不是后面完全覆盖前面的，所以在设计的时候，每个阶段如何精确控制CORS，还需要在实践中慢慢探索……
### **追求更好的开发体验：整合第三方CORSFilter**
- 
对这个类库的使用和分析将在下一篇展开
- 
官网：[http://software.dzhuvinov.com/cors-filter.html](http://software.dzhuvinov.com/cors-filter.html)
- 
喜欢用这个CORSFilter主要是因为它支持CORS配置文件，能够自动读取classpath下的cors.properties，还有file watching的功能
### **示例代码下载**
- [http://git.oschina.net/jearton/springmvc-cors](http://git.oschina.net/jearton/springmvc-cors)
- [http://git.oschina.net/jearton/web-demo/](http://git.oschina.net/jearton/web-demo/)
文／Jearton（简书作者）
原文链接：http://www.jianshu.com/p/d05303d34222
著作权归作者所有，转载请联系作者获得授权，并标注“简书作者”。
