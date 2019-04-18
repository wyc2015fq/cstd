# spring-boot 加载本地静态资源文件路径配置 - z69183787的专栏 - CSDN博客
2015年06月16日 17:13:34[OkidoGreen](https://me.csdn.net/z69183787)阅读数：40739
1.spring boot默认加载文件的路径是 
/META-INF/resources/ 
/resources/ 
/static/ 
/public/ 
这些目录下面, 当然我们也可以从spring boot源码也可以看到 
Java代码  ![收藏代码](http://lihao312.iteye.com/images/icon_star.png)
- privatestaticfinal String[] CLASSPATH_RESOURCE_LOCATIONS = {  
- "classpath:/META-INF/resources/", "classpath:/resources/",  
- "classpath:/static/", "classpath:/public/" };  
所有本地的静态资源都配置在了classpath下面了, 而非在webapp下了 
2. 
Html代码  ![收藏代码](http://lihao312.iteye.com/images/icon_star.png)
- <%@ page language="java"contentType="text/html; charset=utf-8"
- pageEncoding="utf-8"%>
- <!DOCTYPE html>
- <%@ taglib prefix="c"uri="http://java.sun.com/jstl/core"%>
- <htmllang="zh-cn">
- <head>
- <metacharset="utf-8">
- <metahttp-equiv="X-UA-Compatible"content="IE=edge">
- <metaname="viewport"content="width=device-width, initial-scale=1.0">
- <metaname="description"content="">
- <metaname="author"content="">
- 
- <title>用户登录</title>
- </head>
- <!-- CSS件 -->
- <linkrel="stylesheet"href="/css/bootstrap.min.css">
- <linkrel="stylesheet"href="/css/login.css">
- <body>
- <divclass="container">
- <formclass="form-signin">
- <h2class="form-signin-heading">Please sign in</h2>
- <inputtype="text"class="form-control"placeholder="Email address" required autofocus>
- <inputtype="password"class="form-control"placeholder="Password" required>
- <buttonclass="btn btn-lg btn-primary btn-block"type="submit">Sign in</button>
- </form>
- </div>
- 
- <scriptsrc="http://cdn.bootcss.com/jquery/1.10.2/jquery.min.js"></script>
- <scriptsrc="/js/bootstrap.min.js"></script>
- </body>
- </html>
3.目录结构详见附件 
项目地址[https://github.com/leelance/spring-boot-all](https://github.com/leelance/spring-boot-all)
