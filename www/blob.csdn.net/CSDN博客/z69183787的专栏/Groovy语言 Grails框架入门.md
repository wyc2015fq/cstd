# Groovy语言 Grails框架入门 - z69183787的专栏 - CSDN博客
2015年01月05日 19:01:52[OkidoGreen](https://me.csdn.net/z69183787)阅读数：762
个人分类：[Groovy-概述](https://blog.csdn.net/z69183787/article/category/2816515)
掌握基本的 Grails 开发技术，并能够进一步独立学习 Grails 的高级功能。 
Grails 是搭建在动态语言 Groovy 之上的一个开源 MVC Web 开发框架，Grails 的一个显著特点在于“快”，这主要得益于 Groovy 脚本语言的动态特性，也得益于其“一栈式”的设计。与传统的 Java/C# Web 开发框架相比，它可以使用极少的代码和配置实现诸如表单提交、数据库读写等常见的
 Web 应用功能，实现了开发效率的成倍提高。 
与它的老师 Ruby on Rails 相比，Grails 的优势在于 Groovy 语言与 Java 平台的紧密集成。一方面 Groovy 在语法上与 Java 十分相似，降低了众多的 Java 程序员学习 Grails 的门槛。另一方面，Groovy 本身运行于 JVM 之上，可以直接调用 Java 代码，实现了与
 Java 平台的完美整合，因此可以将 Grails Web 应用运行在成熟可靠的 Java EE 应用服务器之上。 
Grails 是一个遵循 MVC 设计模式的 Web 开发框架。它分别用 Domain Class、View、Controller 对应于 MVC 的模型、视图和控制器。可以简单的把一个 Domain Class 理解为数据库中的一张表，Grails 在底层通过 Hibernate 将 Domain Class 和数据库的表进行了映射。View
 在 Grails 中指的是 GSP 页面（注意是 GSP 不是 JSP），它用于显示用户界面。GSP 与 JSP 类似，既可以编写静态网页的元素，也可以嵌入 Groovy 代码实现动态内容。Controller 在 Grails 中充当着重要的角色：它可以响应用户的请求，可以接收用户提交的表单，还可以访问模型的数据，并传递给 View。Grails 在 MVC 三方面的具体内容，将在后面的例子中做详细介绍。 
学习Grails应该具备的预备知识： 
1:    熟练使用常见的 HTML 标签； 
2:    有一定的 Web 开发基础（理解表单提交 POST/GET 、理解会话）； 
3:    Java 语言或者 Groovy 语言的基础； 
4:    Hibernate（非必需）； 
1:首先从[http://www.grails.org/](http://www.grails.org/) 
 上下载 Grails 安装程序（目前最新版本是 grails-1.3.7），将压缩包解压到硬盘上，并设置环境变量 GRAILS_HOME 为解压的路径。 
2:设置 GRAILS_HOME 环境变量,然后把 %GRAIS_HOME%\bin 加到 System 的 Path 变量中。 
3:在控制台下输入 grails help，如果能看到 Grails 的命令列表，Grails 的安装配置就算成功了。 
Groovy 语言的一些特点： 
1:   无需“;”结尾（有“;”也不会报错）； 
2:   简单数据类型，与 Java 相同； 
Example Eg:
1:创建一个 Grails 应用程序命名为：Contactsyc 
Grails 会为我们创建一个名叫“Contactsyc”文件夹，并在其中生成一个应用程序框架，生成的应用程序目录结构如下： 
表 1. Contactsyc 应用文件目录结构 
Contactsyc 　 
+ grails-app 　 
　 + conf 存放配置信息，包含数据源、应用程序启动时自动执行的类 ApplicationBootStrap.groovy, Url 映射配置 
　 + controller 存放控制器(“MVC”的“C”) 
　 + domain 存放域类(“MVC”的“M“) 
　 + i18n 存放国际化资源文件 
　 + services 存放 service 类 
　 + taglib 存放标签库类 
　 + views 存放视图模版(“MVC”的 V，每个控制器对应一个文件夹并存放在 Views 中，每个文件夹中会有多个 GSP 页面) 
　 　 + layouts 存放布局模板 
+ grails-tests 存放测试代码 
+ hibernate 存放可选的其它 Hibernate 配置文件 
+ lib 存放其它 Jar 包（如 JDBC 驱动等） 
+ spring 存放可选的 Spring 配置文件 
+ src 　 
　 + java 存放 Java 源程序 
　 + groovy 存放 Groovy 源程序 
+ web-app 　 
　 + css 存放 CSS 样式表 
　 + images 存放图片文件 
　 + js 存放 JavaScript 文件 
　 + WEB-INF 存放部署相关的文件 
　 + index.gsp 应用程序的首页 
2:添加 Domain Class (Team.groovy) 
前面说过，Domain Class 实质上是数据库表映射的实体类。 
domain 文件夹中的 Team.groovy，加入如下内容： 
class Team { 
    String teamName 
    Date foundDate 
} 
这两行代码描述了 Team 所包含的属性，从代码中可以看出 Groovy 语言的一些特点： 
    无需“;”结尾（有“;”也不会报错）； 
    简单数据类型，与 Java 相同； 
3:创建 Controller 
接下来为 Domain Class Team 类创建 Controller，在 grails-app/controller 中创建一个名为 TeamController 的类。 
加入如下代码： 
class TeamController { 
    def scaffold = Team 
} 
4:不要小看这仅有的一行代码，它使 Team 表有了相应的 CRUD（增、删、查、改）功能的页面。 运行contractsyc，就可以看到效果。 
练习过程中可能会有如下的几个疑问： 
    1:    数据库在哪？ 
    2:    如何修改页面的外观？ 
    3:    URL 与 Controller 的对应关系是什么？ 
    4:    表单能做验证吗？ 
下面，将对这几个问题加以解答，并给应用程序加入一些新的功能。 
配置数据库: 
实现了 CRUD，为什么没有配置数据库呢？ 
Grails 为用户提供的一个内置的轻量级数据库 hsqldb，虽然在性能和功能上难以满足要求，但是对于演示 scaffold 还是绰绰有余的。当然，我们可以用 MySQL 去替换它，下面就来演示这个过程（如果读者不想使用 MySQL 去替换 hsqldb，可以跳过下面的内容）。 
首先修改 grails-app\conf\DataSource.groovy： 
修改前（默认）： 
dataSource { 
    pooled = true 
    driverClassName = "org.hsqldb.jdbcDriver" 
    username = "sa" 
    password = "" 
} 
hibernate { 
    cache.use_second_level_cache = true 
    cache.use_query_cache = true 
    cache.provider_class = 'net.sf.ehcache.hibernate.EhCacheProvider' 
} 
// environment specific settings 
environments { 
    development { 
        dataSource { 
            dbCreate = "create-drop" // one of 'create', 'create-drop','update' 
            url = "jdbc:hsqldb:mem:devDB" 
        } 
    } 
    test { 
        dataSource { 
            dbCreate = "update" 
            url = "jdbc:hsqldb:mem:testDb" 
        } 
    } 
    production { 
        dataSource { 
            dbCreate = "update" 
            url = "jdbc:hsqldb:file:prodDb;shutdown=true" 
        } 
    } 
} 
修改后（mysql）： 
dataSource { 
    pooled = false 
    driverClassName = "com.mysql.jdbc.Driver" 
    username = "root" 
    password = "***" 
} 
hibernate { 
    cache.use_second_level_cache=true 
    cache.use_query_cache=true 
    cache.provider_class='org.hibernate.cache.EhCacheProvider' 
} 
// environment specific settings 
environments { 
    development { 
        dataSource { 
            dbCreate = "update" // one of 'create', 'create-drop','update' 
            url = "jdbc:mysql://localhost:3306/Contact_dev" 
        } 
    } 
    test { 
        dataSource { 
            dbCreate = "update" 
            url = "jdbc:mysql://localhost:3306/Contact_test" 
        } 
    } 
    production { 
        dataSource { 
            dbCreate = "update" 
            url = "jdbc:mysql://localhost:3306/Contact_prod" 
        } 
    } 
} 
Grails 默认要求提供三个数据库，分别用于开发、测试和产品环境。修改连接的 Driver 名称、用户名密码以及三个数据库的URL，保存。接下来分别创建上述三个数据库，并把 MySQL 的 JDBC 驱动程序拷贝到 lib 文件夹中。 
接下来，重新运行项目（grails run-app），Grails 会自动为 Domain Class 创建相应的数据库表。至此，Team 的 CRUD 程序就运行在 MySQL 之上了。 
修改页面外观： 
使用 scaffold 实现的 CRUD 实际上并没有创建 GSP 页面，所以无法定制 View 内容。在 Grails 中，可以为指定的 Domain Class 创建实现 CRUD 操作的 Controller 和 View。 
在 grails-app\views\ 目录中创建一个名叫 team 的文件夹，文件夹的名称刚好和 controller 一一对应。team 文件夹中包含了 4 个 GSP 文件，分别是 create.gsp、list.gsp、edit.gsp 和 show.gsp。这几个页面和之前 scaffold 实现的功能是一致的。修改这几个页面的代码，即可以定制页面的外观。 
URL 与 Controller 的命名约定: 
1:打开 generate-all 命令产生的 Controller 即 TeamController.groovy 文件，可以看到如下内容： 
class TeamController { 
    def index = { redirect(action:list,params:params) } 
    // the delete, save and update actions only accept POST requests 
    def allowedMethods = [delete:'POST', save:'POST', update:'POST'] 
    def list = { 
        if(!params.max) params.max = 10 
        [ teamList: Team.list( params ) ] 
    } 
    def show = { 
        [ team : Team.get( params.id ) ] 
    } 
… 
其中 def xxx = {xxx} 的结构在 Groovy 语言中叫闭包（Closure），在 Controller 中，每个闭包对应为一个 Action，即处理一个特定的 Web 请求。然后看看 Grails URL 的命名原则： 
http(s)://host:pot/ProjectName/ControllerName/ActionName/Parameters 
如果请求的 URL 是 http://localhost:8080/Contact/team/list，即调用了 team 控制器的 list Action，对应将会触发 TeamController 的 list 闭包的执行。每个 Action 被执行完毕后，会默认跳转去执行它在 View 中同名的 GSP 页面。并把
 Action 返回的数据传递给 GSP。比如这个例子中，list Action 执行完毕后，会跳转到 grails-app\views\team\list.gsp 视图，并把 Team.list(params) 的结果传递给 list.gsp，在 list.gsp 中可以通过 teamList 访问到 Controller 传递过来的数据。 
表单的验证: 
Grails 对表单的验证提供了强大的支持，可以使用 Domain Class 的验证来实现表单的验证。修改 Team.groovy 文件： 
class Team { 
    String teamName 
    Date foundDate 
    static constraints = { 
        teamName(size:3..50,blank:false,unique:true) 
    } 
} 
在 Team 类中加入个名叫 constraints 的静态 Closure，并在其中描述验证逻辑：team 的字符串长度在 3~50 之间、不能为空、而且是唯一的。（注意早期版本的 Grails 使用 length 限定字符串的长度，在 V0.5 以后改为了 size 
保存 Team.groovy，重新运行 Contact，会发现验证逻辑已经可以工作了. 
但验证失败后报出的错误信息对用户来说并不友好。想要修改这些内容，需要编辑 grails-app\i18n 中的资源文件，这在后面还会做详细介绍。 

    List result = null; 
        List paramList = new ArrayList(); 
        StringBuffer sql = new StringBuffer(); 
        String overallQuery = generateQuery(productList.get(0)); 
        sql.append(overallQuery); 
        sql.append("(" + NEWLINE); 
URL 与 Controller 的命名约定： 
Controller 即 TeamController.groovy 文件，可以看到如下内容： 
class TeamController { 
    def index = { redirect(action:list,params:params) } 
    // the delete, save and update actions only accept POST requests 
    def allowedMethods = [delete:'POST', save:'POST', update:'POST'] 
    def list = { 
        if(!params.max) params.max = 10 
        [ teamList: Team.list( params ) ] 
    } 
    def show = { 
        [ team : Team.get( params.id ) ] 
    } 
… 
其中 def xxx = {xxx} 的结构在 Groovy 语言中叫闭包（Closure），在 Controller 中，每个闭包对应为一个 Action，即处理一个特定的 Web 请求。然后看看 Grails URL 的命名原则： 
http(s)://host:pot/ProjectName/ControllerName/ActionName/Parameters 
如果请求的 URL 是 http://localhost:8080/Contact/team/list，即调用了 team 控制器的 list Action，对应将会触发 TeamController 的 list 闭包的执行。每个 Action 被执行完毕后，会默认跳转去执行它在 View 中同名的 GSP 页面。并把
 Action 返回的数据传递给 GSP。比如这个例子中，list Action 执行完毕后，会跳转到 grails-app\views\team\list.gsp 视图，并把 Team.list(params) 的结果传递给 list.gsp，在 list.gsp 中可以通过 teamList 访问到 Controller 传递过来的数据。 
