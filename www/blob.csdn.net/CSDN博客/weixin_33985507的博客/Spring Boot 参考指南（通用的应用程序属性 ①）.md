# Spring Boot 参考指南（通用的应用程序属性 ①） - weixin_33985507的博客 - CSDN博客
2018年06月17日 11:13:06[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
# 第X章. 附录
## 附录A. 通用的应用程序属性
可以在`application.properties`文件，`application.yml`文件，或作为命令行开关，中指定各种属性，本附录提供了一个通用的Spring Boot属性列表和对使用它们的底层类的引用。
> 属性贡献可以来自类路径上的其他jar文件，因此不应该将其视为一个详尽的列表，此外，还可以定义自己的属性。
> 本示例文件仅作为指南，不要将整个内容复制粘贴到应用程序中，相反，只选择你需要的属性。
### 核心属性
```
# ===================================================================
# 通用的 Spring Boot 属性
#
# 这个示例文件是作为指导方针提供的,不要将其全部复制到你自己的应用程序中               
#                             ^^^
# ===================================================================
# ----------------------------------------
# 核心属性
# ----------------------------------------
debug=false # 启用 debug 日志
trace=false # 启用 trace 日志.
```
### 日志记录
```
logging.config= # 日志记录配置文件的位置，例如，classpath:logback.xml为Logback提供 
logging.exception-conversion-word=%wEx # 记录异常时使用的转换字
logging.file= # 日志文件的名字 (例如，`myapp.log`)，名称可以是一个确切的位置或相对于当前目录
logging.file.max-history=0 # 要保存的归档日志文件的最大值，只支持默认的logback设置
logging.file.max-size=10MB # 最大日志文件大小，只支持默认的logback设置
logging.level.*= # 日志级别严重性映射，例如，`logging.level.org.springframework=DEBUG`
logging.path= # 日志文件的位置，例如 /var/log
logging.pattern.console= # 向控制台输出的附加模式，只支持默认的logback设置
logging.pattern.dateformat=yyyy-MM-dd HH:mm:ss.SSS # 日志日期格式的附加模式，只支持默认的logback设置
logging.pattern.file= # 向文件输出的Appender模式，只支持默认的logback设置
logging.pattern.level=%5p # 日志级别的附加模式，只支持默认的logback设置
logging.register-shutdown-hook=false # 当日志系统初始化时，注册一个关闭hook
```
### AOP
```
spring.aop.auto=true # 添加 @EnableAspectJAutoProxy.
spring.aop.proxy-target-class=true # 创建基于子类的(CGLIB)代理(true)，不创建基于标准Java接口的代理(false)
```
### 标识（[ContextIdApplicationContextInitializer](https://github.com/spring-projects/spring-boot/tree/v2.0.2.RELEASE/spring-boot-project/spring-boot/src/main/java/org/springframework/boot/context/ContextIdApplicationContextInitializer.java)）
`spring.application.name= # 应用程序名`
### 管理（[SpringApplicationAdminJmxAutoConfiguration](https://github.com/spring-projects/spring-boot/tree/v2.0.2.RELEASE/spring-boot-project/spring-boot-autoconfigure/src/main/java/org/springframework/boot/autoconfigure/admin/SpringApplicationAdminJmxAutoConfiguration.java)）
```
spring.application.admin.enabled=false # 是否为应用程序启用管理特性
spring.application.admin.jmx-name=org.springframework.boot:type=Admin,name=SpringApplication # 应用程序管理MBean的JMX名称
```
### 自动配置
`spring.autoconfigure.exclude= # 排除的自动配置类`
### 横幅
```
spring.banner.charset=UTF-8 # 横幅文件编码
spring.banner.location=classpath:banner.txt # 横幅文本资源的位置
spring.banner.image.location=classpath:banner.gif # 横幅图像文件位置(jpg或png也可以使用)
spring.banner.image.width=76 # 横幅图片的宽度
spring.banner.image.height= # 横幅图片在字符中的高度(默认基于图片高度)
spring.banner.image.margin=2 # 图片左间距
spring.banner.image.invert=false # 图像是否应该反转为黑暗终端主题
```
### Spring core
`spring.beaninfo.ignore=true # 是否跳过对BeanInfo类的搜索.`
### Spring 缓存（[CacheProperties](https://github.com/spring-projects/spring-boot/tree/v2.0.2.RELEASE/spring-boot-project/spring-boot-autoconfigure/src/main/java/org/springframework/boot/autoconfigure/cache/CacheProperties.java)）
```
spring.cache.cache-names= # 如果底层缓存管理器支持，则创建以逗号分隔的缓存名称列表.
spring.cache.caffeine.spec= # 用于创建缓存的规范，有关规范格式的详细信息，请参阅CaffeineSpec
spring.cache.couchbase.expiration=0ms # 条目过期时间，默认情况下，条目不会过期，注意，这个值最终转换为秒
spring.cache.ehcache.config= # 用于初始化EhCache的配置文件的位置
spring.cache.infinispan.config= # 用于初始化Infinispan的配置文件的位置
spring.cache.jcache.config= # 用于初始化缓存管理器的配置文件的位置
spring.cache.jcache.provider= # 用于检索符合JSR-107的缓存管理器的CachingProvider实现的完全限定名，只有在类路径上有多个JSR-107实现时才需要
spring.cache.redis.cache-null-values=true # 允许缓存null值.
spring.cache.redis.key-prefix= # Key 的前缀
spring.cache.redis.time-to-live=0ms # 条目过期时间，默认情况下，条目不会过期
spring.cache.redis.use-key-prefix=true # 在写入Redis时是否使用 key 前缀
spring.cache.type= # 缓存类型，默认情况下，根据环境自动检测
```
### Spring 配置（[ConfigFileApplicationListener](https://github.com/spring-projects/spring-boot/tree/v2.0.2.RELEASE/spring-boot-project/spring-boot/src/main/java/org/springframework/boot/context/config/ConfigFileApplicationListener.java)）
```
spring.config.additional-location= # 除了默认设置外，还使用配置文件位置
spring.config.location= # 配置文件位置，替换默认值
spring.config.name=application # 配置文件名称
```
### hazelcast（[HazelcastProperties](https://github.com/spring-projects/spring-boot/tree/v2.0.2.RELEASE/spring-boot-project/spring-boot-autoconfigure/src/main/java/org/springframework/boot/autoconfigure/hazelcast/HazelcastProperties.java)）
`spring.hazelcast.config= # 用于初始化Hazelcast的配置文件的位置`
### 工程信息（[ProjectInfoProperties](https://github.com/spring-projects/spring-boot/tree/v2.0.2.RELEASE/spring-boot-project/spring-boot-autoconfigure/src/main/java/org/springframework/boot/autoconfigure/info/ProjectInfoProperties.java)）
```
spring.info.build.location=classpath:META-INF/build-info.properties # 生成的build-info.properties文件的位置
spring.info.git.location=classpath:git.properties # 生成的git.properties文件的位置
```
### JMX
```
spring.jmx.default-domain= # JMX 域名.
spring.jmx.enabled=true # 将管理bean公开给JMX域.
spring.jmx.server=mbeanServer # MBeanServer bean名称
```
### 电子邮件（[MailProperties](https://github.com/spring-projects/spring-boot/tree/v2.0.2.RELEASE/spring-boot-project/spring-boot-autoconfigure/src/main/java/org/springframework/boot/autoconfigure/mail/MailProperties.java)）
```
spring.mail.default-encoding=UTF-8 # 默认MimeMessage编码
spring.mail.host= # SMTP服务器主机，例如 smtp.example.com
spring.mail.jndi-name= # Session JNDI名称，设置时，优先于其他邮件设置
spring.mail.password= # SMTP服务器的登录密码
spring.mail.port= # SMTP服务器端口
spring.mail.properties.*= # 额外的JavaMail session属性
spring.mail.protocol=smtp # SMTP服务器使用的协议
spring.mail.test-connection=false # 是否在启动时测试邮件服务器是否可用
spring.mail.username= # SMTP服务器的登录用户
```
### 应用程序设置（[SpringApplication](https://github.com/spring-projects/spring-boot/tree/v2.0.2.RELEASE/spring-boot-project/spring-boot/src/main/java/org/springframework/boot/SpringApplication.java)）
```
spring.main.banner-mode=console # 模式用于在应用程序运行时显示横幅
spring.main.sources= # 源(类名、包名或XML资源位置)包含在ApplicationContext中
spring.main.web-application-type= # 标记显式地请求特定类型的web应用程序，如果没有设置，则基于类路径自动检测
```
### 文件编码（[FileEncodingApplicationListener](https://github.com/spring-projects/spring-boot/tree/v2.0.2.RELEASE/spring-boot-project/spring-boot/src/main/java/org/springframework/boot/context/FileEncodingApplicationListener.java)）
`spring.mandatory-file-encoding= # 应用程序必须使用的预期字符编码`
### 国际化（[MessageSourceProperties](https://github.com/spring-projects/spring-boot/tree/v2.0.2.RELEASE/spring-boot-project/spring-boot-autoconfigure/src/main/java/org/springframework/boot/autoconfigure/context/MessageSourceProperties.java)）
```
spring.messages.always-use-message-format=false # 是否总是应用MessageFormat规则，甚至解析没有参数的消息
spring.messages.basename=messages # 以逗号分隔的basenames列表(本质上是一个完全限定的类路径位置)，每个都遵循ResourceBundle约定，并宽松地支持基于斜线的位置
spring.messages.cache-duration= # 加载资源包文件缓存持续时间，当没有设置时，绑定包将永远被缓存，如果没有指定持续时间后缀，则将使用秒
spring.messages.encoding=UTF-8 # 消息包编码
spring.messages.fallback-to-system-locale=true # 如果没有找到特定地区的文件，是否返回到系统区域
spring.messages.use-code-as-default-message=false # 是否使用消息代码作为默认消息，而不是抛出“NoSuchMessageException”，建议仅在开发过程中。
```
### 输出
`spring.output.ansi.enabled=detect # 配置ANSI输出`
### PID文件（[ApplicationPidFileWriter](https://github.com/spring-projects/spring-boot/tree/v2.0.2.RELEASE/spring-boot-project/spring-boot/src/main/java/org/springframework/boot/context/ApplicationPidFileWriter.java)）
```
spring.pid.fail-on-write-error= # 如果使用ApplicationPidFileWriter，但它不能编写PID文件，则将失败
spring.pid.file= # 要写入的PID文件的位置(如果使用ApplicationPidFileWriter)
```
### 配置文件
```
spring.profiles.active= # 活动配置文件的逗号分隔的列表，可以被命令行开关覆盖
spring.profiles.include= # 无条件地激活指定的以逗号分隔的配置文件列表(或者使用YAML的配置文件列表)
```
### quartz scheduler（[QuartzProperties](https://github.com/spring-projects/spring-boot/tree/v2.0.2.RELEASE/spring-boot-project/spring-boot-autoconfigure/src/main/java/org/springframework/boot/autoconfigure/quartz/QuartzProperties.java)）
```
spring.quartz.jdbc.initialize-schema=embedded # 数据库schema初始化模式
spring.quartz.jdbc.schema=classpath:org/quartz/impl/jdbcjobstore/tables_@@platform@@.sql # 用于初始化数据库schema的SQL文件的路径
spring.quartz.job-store-type=memory # Quartz job 存储类型
spring.quartz.properties.*= # 附加的Quartz Scheduler属性
```
### 反应堆（[ReactorCoreProperties](https://github.com/spring-projects/spring-boot/tree/v2.0.2.RELEASE/spring-boot-project/spring-boot-autoconfigure/src/main/java/org/springframework/boot/autoconfigure/reactor/core/ReactorCoreProperties.java)）
`spring.reactor.stacktrace-mode.enabled=false # 反应堆是否应该在运行时收集堆栈信息`
