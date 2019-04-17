# Stripes视图框架内置Spring集成的使用 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2013年11月06日 00:51:10[boonya](https://me.csdn.net/boonya)阅读数：2274








  Stripes视图框架内置Spring集成了的，在项目中也遇到需要导入spring支持的jar包，这足以证明Stripes与Spring有着非同寻常的关系。

**Spring注入Java Bean**

  我们需要将Spring beans或业务对象自动地注入到需要进行处理或引用的对象上。在Stripes框架中，实现这一点并不要求外部配置—除了你的Spring上下文配置之外。上下文配置如下：



` <context:component-scan base-package="com.boonya.stripes.service" />`**Java 代码中实现Spring Bean 注入**




```java
@SpringBean
  private transient AccountService accountService;
  @SpringBean
  private transient CatalogService catalogService;
```

注：@SpringBean一般在*ActionBean中使用，而其他的Java Bean的对象注入用@Resource或@Autowired。













