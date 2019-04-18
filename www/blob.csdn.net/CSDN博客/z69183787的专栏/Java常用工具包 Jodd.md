# Java常用工具包 Jodd - z69183787的专栏 - CSDN博客
2016年12月20日 18:35:41[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2388
  下文摘抄自：[开源中国](http://www.oschina.net/p/jodd)[Demo](https://github.com/oblac/jodd)[jar下载](http://jodd.org/download/index.html)[官方文档](http://jodd.org/)[JoddAPI](http://jodd.org/api/index.html)
Jodd 是一个 [Java](http://lib.csdn.net/base/javase) 的工具包和一个微型框架。Jodd
 工具增强了 JDK 提供很多强大的功能，帮助你实现日常的一些开发任务，让代码更加可靠和健壮。而 Jodd 框架是一个轻量级的应用框架，紧缩但强大。设计遵循 CoC、DRY 和 SCS 原则，让开发更加简单。
 Jodd = tools + ioc + mvc + db + aop + tx + html < 1.3M
你可以把 Jodd 想象成 Java 的"瑞士军刀",不仅小，锋利而且包含许多便利的功能。
Jodd 提供的功能有：
1. 提供操作[Java](http://lib.csdn.net/base/java)bean，
2. 可以从各种数据源加载 Bean,
3. 简化 JDBC 的接连与代码，
4. 剖析 SQL 查询，
5. 处理时间与日期，
6. 操作与格式化 String,
7. 搜索本地硬盘上的文件，
8. 帮助处理 Servlet 请求等。
除此之外还包含一个很小，但实用的基于 JSP 的 MVC 框架。
**Jodd 不仅仅是个工具包：**
**Jodd 工具包**
> 
高性能的工具集合
- **TypeConverter** 一个强大的类型转换工具
- **BeanUtil** 高效的 Bean 工具，支持嵌套的属性，以及标准的集合类
- **Base64/Base32**
- **JDateTime** 增强的时间类
- **IO** 快速高性能的各种 Buffer、Writer、OutputStream
- **Wildcard** 通配符工具
- **Servlet** Servlet 工具，JSP 的标签、函数扩展
- **FindFile/ClassFinder** 强大的文件/类搜索工具，支持通配符&正则表达式匹配
- **Cache** 简单易用的 LRU、LRU、FIFO 缓存
- **StringUtil** 强大的字符串处理工具
- 等等
**Madvoc**
> 
轻量级快速开发MVC框架
- 自动扫描 Actions & Results
- 参数自动注入，支持嵌套的属性注入，支持标准集合参数的注入
- 自动导出需要输出的参数
- 支持通配符/正则表达式路由地址
- 支持 REST 风格的 URL
- 支持通过配置文件设置 Action 的拦截器
- 开放的 API，可轻松扩展
**HTTP**
> 
轻量级Http客户端
- 基于socket
- 支持 cookies
- 支持上传文件
- 支持自定义 HTTP 头
- 支持 gzip
- 支持 Basic authentication
**Props**
> 
增强的properties
- 支持 UTF-8 编码
- 支持插值操作
- 支持 ini 类型的区段
- 支持使用 `+=` 附加值
- 支持多行模式
**Email**
> 
易用的 Email 接收发送工具，基于 javax.mail
- 支持 SSL
- 支持添加附件
- 支持 POP3、IMAP
- IMAP 接受模式支持定义过滤器
- 支持解析 EML 文件
**Petite**
> 
轻量级 IoC 容器
**Db & DbOom**
> 
轻量级 ORM 框架
**Lagarto**
> 
高性能的 xml/html 解析框架
**Jerry**
> 
HTML 解析框架，Java 中的 [jQuery](http://lib.csdn.net/base/jquery)，基于 Lagarto
**VTor**
> 
基于注解的字段验证框架
- 可设置多配置
- 易扩展
**Proxetta**
> 
高性能的代理生成器
更多参考文章：[http://hechuanzhen.iteye.com/blog/1775836](http://hechuanzhen.iteye.com/blog/1775836)
