# struts2配置 - zj360202的专栏 - CSDN博客





2009年11月06日 11:07:00[zj360202](https://me.csdn.net/zj360202)阅读数：418标签：[struts																[interceptor																[action																[class																[spring																[validation](https://so.csdn.net/so/search/s.do?q=validation&t=blog)






### Struts 2 环境

下载类库:[http://192.168.16.240/source/struts-2.0.11.1-lib.zip](http://192.168.16.240/source/struts-2.0.11.1-lib.zip)



#### 完整类库列表
antlr-2.7.2.jar
commons-beanutils-1.6.jar
commons-chain-1.1.jar
commons-logging-1.0.4.jar
commons-logging-api-1.1.jar
commons-validator-1.3.0.jar
freemarker-2.3.8.jar
ognl-2.6.11.jar
oro-2.0.8.jar
struts-core-1.3.5.jar
struts2-codebehind-plugin-2.0.11.1.jar
struts2-config-browser-plugin-2.0.11.1.jar
struts2-core-2.0.11.1.jar
struts2-jasperreports-plugin-2.0.11.1.jar
struts2-jfreechart-plugin-2.0.11.1.jar
struts2-jsf-plugin-2.0.11.1.jar
struts2-pell-multipart-plugin-2.0.11.1.jar
struts2-plexus-plugin-2.0.11.1.jar
struts2-sitegraph-plugin-2.0.11.1.jar
struts2-sitemesh-plugin-2.0.11.1.jar
struts2-spring-plugin-2.0.11.1.jar
struts2-struts1-plugin-2.0.11.1.jar
struts2-tiles-plugin-2.0.11.1.jar
tiles-api-2.0.4.jar
tiles-core-2.0.4.jar
tiles-jsp-2.0.4.jar
xwork-2.0.4.jar


#### 基础类库列表
commons-logging-1.0.4.jar
freemarker-2.3.8.jar
ognl-2.6.11.jar
struts2-core-2.0.11.jar
xwork-2.0.4.jar


#### 基础配置
- WEB.XML 
<filter>
        <filter-name>struts2</filter-name>
        <filter-class>org.apache.struts2.dispatcher.FilterDispatcher</filter-class>
    </filter>
    <filter-mapping>
        <filter-name>struts2</filter-name>
        <url-pattern>/*</url-pattern>
    </filter-mapping>- Struts.xml 
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE struts PUBLIC   "-//Apache Software Foundation//DTD Struts Configuration 2.0//EN"   "http://struts.apache.org/dtds/struts-2.0.dtd">
<struts>
    <constant name="struts.enable.DynamicMethodInvocation" value="false" />
    <constant name="struts.devMode" value="false" />
    <include file="example.xml"/>
    <!-- Add packages here -->
</struts>


### 配置文件

struts.xml 位置：classes下 配置 Action
- package 
struts对配置的组织结构
         类似于java package
         属性：
           *  name  :   包的名字，唯一标示
              extends：  继承
              namespace：命名空间
              abstract:  抽象申明- namespace 
1 default namespace   ""
    2 root namespace "/"
    3 action 搜索的顺序
            当前namespace ---一级一级向上搜索
            Web应用的请求路径被视为namespace
    4 访问 /aaa.action的时候的搜索的顺序
          "/"    --->   ""
    5  namespace 没有目录信息
    6  package 包含0 - N 个 action           
<pre>
;Action:<pre>
    name: 唯一标示   name.action ---> action name
    class：实现类    继承actionsupport的类
    method:调用方法  动态调用的方法名称- Result 
name: 用来匹配action的返回值
    type: 是result-type 进行预定义的/自定义的 常用dispatcher,转发
          <result-types>
              <result-type name="dispatcher" class =""  default="true" ></result-type>
          </result-types>
特殊位置：
<global-results>
   <result name=""></result>
           ....
</global-results>

特殊参数：
<result name="next_opertor" >${var_name}</result>
      var_name 必须是action里面的一个私有属性，并且有get/set方法
               必须符合bean的命名规则： 小写开头
                                       set/get大写开头的属性- include 
只能放在struts 标签的下一级
  用来引入其他的struts配置文件
   file = " "
   被他引入xml文件是一个标准的struts配置
interceptors = > 拦截器
<interceptors>
     <interceptor name="" class=""/>
     <intercptor-stack name="">
      <interceptor-ref name=""/>
      <interceptor-ref name=""/>
     </interceptor-stack>
<interceptors>
<default-interceptor-ref name="">

<action。。。。。。>
    <interceptor-ref name="" />- constants 
<constant name="" value="" />
可以完成
struts.properties 所完成的工作
可以完成
    constant 所完成的工作
struts.properties
配置struts特性（动态方法调用，虚拟机）
struts-default.xml struts...jar里面，----〉零配置 struts-default.vm 模板





### Action



### I18N/I10N



### 拦截器
- 拦截器在struts 2接收到请求之后，抵达Action之前 以及离开Action之后 发挥作用 - 抵达Action之前，可以处理参数（文件上传，struts2有内置拦截器），或者拦截请求 
- 离开Action之后，可以处理Action返回的Result（嵌套模板的拦截器） 

- 配置拦截器 - struts自己提供了缺省的拦截器， 
- 拦截器的使用的第一步 ，配置、可以理解为登记 
- 拦截器的使用的第二步 ，配置、定义缺省interceptor-stack 、或者 直接指明 某个Action使用的拦截器 
- 使用拦截器的时候以Stack为单位 
- 拦截器可以被覆盖，采取就近配置的拦截器：： namespace范围 或者 action范围内部 
- 拦截器之行的先后顺序：根据配置文件内容，由上至下（抵达Action之前），由下至上（离开Action之后）
注册Interceptors:

 <interceptors>
            <interceptor name="name_1" 
                   class="com.opensymphony.xwork2.interceptor.AliasInterceptor"/>
            .......
 </interceptors>

注册Interceptor-stack:

 <interceptor-stack name="stack_name_1">
                <interceptor-ref name="name_1"/>
                 .......
</interceptor-stack>

使用Interceptor-stack

在package中定义 ：
<default-interceptor-ref name="stack_name_1"/>

在Action中定义         
    <action .......>
         <interceptor-ref name="stack_name_1"/>
    </action>
<pre>

##系统自带拦截器
###Alias Interceptor
####转换参数名,内容不变<pre>
name="alias" 
class="com.opensymphony.xwork2.interceptor.AliasInterceptor"- 实现参数读取的拦截器 Parameters Interceptor - 把请求中的参数放置到Action中 ： 参数 ---》 Action中的 Property(attribute) 
- 放置在 default stack ,在覆盖的情况下可以直接使用
name="params" 
class="com.opensymphony.xwork2.interceptor.ParametersInterceptor"

- 实现国际化的拦截器 I18n Interceptor - 提供读取国际化 properties文件键值的方法 
- 放置在 default stack ,在覆盖的情况下可以直接使用
name="i18n" 
class="com.opensymphony.xwork2.interceptor.I18nInterceptor"

- 实现cookie的读写和配置的拦截器 cookie Interceptor - 提供Action中操作Cookie的方法 
- 放置在 default stack ,在覆盖的情况下可以直接使用
name="cookie" class="org.apache.struts2.interceptor.CookieInterceptor"

- 实现struts2校验功能的拦截器 validation Interceptor - 放置在 default stack ,在覆盖的情况下可以直接使用 
- 在Action执行缺省或者配置的方法之前进行调用validation方法
name="validation" class="org.apache.struts2.interceptor.validation.AnnotationValidationInterceptor"

- 实现Struts中数据（参数）前后共享的 Chainning Interceptor - Action前后可以读取上下文的参数 （比如前一个 Action的属性） 
- 把所有使用到的Action 的属性放置在上下文的attribute中 

- Create Session Interceptor 自动创建 Session - 提供 其他基于Session的Interceptor/Action提供session
name="createSession"
class="org.apache.struts2.interceptor.CreateSessionInterceptor"

- 异常拦截器 Exception Interceptor - 把异常定位到一个确切的页面
name="exception" class="com.opensymphony.xwork2.interceptor.ExceptionMappingInterceptor"

- 提供Action操作 Session 与 Application（Servlet context ）的方法Scope Interceptor - Action直接操作session/application的方法
name="scope" 
class="org.apache.struts2.interceptor.ScopeInterceptor"

- 文件上传的拦截器 fileuploader interceptor - 提供基于struts2的文件上传功能
name="fileUpload" 
class="org.apache.struts2.interceptor.FileUploadInterceptor"

- Logger Interceptor 输出Action的名字 





#### 要点



##### 在Struts 2中实现文件上传
- 所需要的 jar文件
基本的 struts2的 類庫 （commons-logging-1.0.4.jar日志記錄 ,freemarker-2.3.8.jar 模板實現 ,ognl-2.6.11.jar onnl語言支持  ,struts2-core-2.0.11.jar struts2的核心類庫,xwork-2.0.4.jar struts2的基礎類庫）
apache upload 組件 （commons-fileupload-x.xxxx.jar）
apache io 組件 （commons-io-x.xx.xx.jar）
- 所需要的配置文件
;struts.properties :
     在該文件中需要修改臨時文件存儲路徑，文件的大小
     struts.multipart.saveDir= /upload   保存目录，值得是realpath
     struts.multipart.maxSize=209715200  上传文件的大小限制
     struts.multipart.parser=jakarta     解析上传文件信息（multipart/form-data，
上传文件时的form数据类型一定是这种情况）
;struts.xml :
      在该文件中配置文件类型，以参数的形式放置在fileUpload拦截器引用中
      <interceptor-ref name="fileUpload">
      <param name="allowedTypes">
      image/png,application/pdf
      </param>
     </interceptor-ref>
- 所需要的操作
在Action 中，直接使用  MultiPartRequestWrapper 来获取上传的文件及参数 （文件上传的时候 ，
通过正常的参数获取渠道---params拦截器无法的到我们需要的参数，因为该拦截器无法解析multipart
类型的请求）
   这个时候 我们通过ServletActionContext得到的Request本身就是一个MultiPartRequestWrapper的实
例，我们通过前置类型转换的到该对象。  
      multipartInfo.getParameter("") 得到我们需要的普通参数
      multipartInfo.getFiles("");     得到我们需要的文件对象（但我们得到该文件的时候，文件
已经被上传，并且保存在struts.properties中所定义的目录中，该 Request被返回给客户端的时候，
临时文件会被删除）



##### 在Struts 2.0中国际化(i18n)应用程序
- 使用struts 的 Text标签可以读取在struts.properties文件中定义的消息文件中的消息文本
struts.custom.i18n.resources 定义文件名称（包含路径信息,包含路径信息的时候,路径的分隔符用.）
struts.locale=zh-cn    定义缺省的Location
实际读取的文件： 文件名称+Locale的值+.properties   
如果该文件找不到，会读取   文件名称+.properties   
如果该文件依旧找不到，显示key的值
在所有的struts ui tag（用于界面显示的标签 都有一个key属性，可以直接读取消息文件中key对对应的值   

当出现非ASCII CODE的时候,我们需要 先使用本地语言编码编写文字内容,然后使用jdk提供的
native2ascii工具转码,成为iso8859-1编码的文本文件,否则会出现乱码,该操作与IDE(Eclipse)所设
定的缺省字符集,文件字符集等 无关
- 使用struts标签中以及Action中可以使用getText的方式的到消息文件中的消息文本
在struts中的Text标签也是调用该方法
         在Action中的父类ActionSupport中已经定义了该方法，可以直接使用
         在Action中（不包含jsp页面、模板页面），不需要定义package.properties文件，但是可以直接读取该文件中的key-value对应信息，
         package.properties             局部消息文件
         struts.custom.i18n.resources   全局消息文件
         定义了局部消息文件后，局部消息文件会首先被读取。
         文件名的定义不变   filename_local.properties
- struts2中使用资源文件的local可以通过参数传递来得到
参数是 java.util.Locale的实例
         设置方式： ActionContext.getContext().setLocale(locale参数)
         可以通过其它方式来设定locale,或者自我实现，但都不是Struts2的实现方式。
- struts2 中的国际化有可能出现乱码，出现乱码后的解决方式
引用文件的字符集 （jsp有三个：  jsp自己 两个：contentType中的charset和
pageEncoding，  html 一个： meta 中的contentType中的charset，模板文件根据模板实现不同，但是与jsp方式类似）,预定义不符合，或者三个字符集不一致。
         服务器返回给客户端的数据编码与客户端请求不符合
         消息文件本身是乱码：不一定是编写的乱码，而是读取的时候使用的字符集与文件本省的字符
集不一直，缺省情况下读取的时候使用操作系统自己的缺省字符集读取，单独xp本身就有四种（其中三种是unicode,unicode的不同类型互补兼容--ASCII例外）
         解决方式：
             1、转码或过滤器，可以注册使用apache提供的字符集过滤器（）
             2、更新本地消息文件字符集、编码内容，（另存为新的字符集文件 转成新的字符集编码的文件 或者使用 java提供的 native2ascii转换成iso8859-1的字符集）
             3、修改jsp/模板文件中的有关字符集定义，如果jsp自己定义的两个charset不一直，编译本身就会出错，只需要修改一致，如果修改一致后依旧是乱码，可以一起替换成新的字符集
事实上，会涉及字符集只有：utf-8 / iso8859-1 / gbk 
       CJK字符集 ： 中日韩 字符集 JPN-xxxx;外包项目开发的时候才会遇到



##### 在Struts 2.0中实现表单数据校验（Validation）
- 通过拦截器来实现，在Action中的方法被调用之前，首先调用validation方法或者其他验证方法 
- 验证结束后，如果验证失败，返回结果(result)为固定的input，返回内容中会包含错误信息。
返回的错误集合以 key为 "fieldErrors"的一个数据对象保存在request中,如果使用模板技术,
该错误信息会显示在该控件的下部(具体位置由模板控制),如果不使用模板技术,我们可以使用Struts2 Tag
读取"fieldErrors"的值进行显示.
- Struts的校验功能为服务器端校验
校验代码: 
    配置文件struts.xml中的Action必须要有<result name="input"> </result>
    校验代码中需要有 addfieldError方法(尽量在该方法中使用I18N代码,以方便国际化显示,因为这段代码是硬编码,不便于修改)
     需要添加
       @Override
 public void validate() {
           addfieldError(....);
        }
       方法到Action中,所有的验证操作在该方法中实现.
- 如果验证成功，调用Action定义的方法，返回逻辑上应该得到的结果。 



##### 在Struts 2中数据类型转换
Struts
    field user.name
          user.password
    ----> 在action中寻找一个属性，名称为user
                    if find
                       set user.name to user.name's value
                       set user.passwordto user.name's password
                    else
                       DEBUG:打印消息 
    -----> After Find it：
           数据类型转换：
                    得到属性 user的实例 （如果为null,通过class创建实例，注意field对应的类、bean需要包含无参构造方法）
                    调用set方法，给该对象赋值   【在这个地方完成数据类型转换】
                    
    【在这个地方完成数据类型转换】                
  基本数据类型      <---->   String    
  引用数据类型      <---->   String
  比如：
      在数据类型转换中，Date需要单独处理，原因是格式的问题：
      Date 请求的 Locale 对应的 DateFormat 的 short 格式 
手动转换类型(日期)：
     转换方式，
          1    重写Struts的 StrutsTypeConverter                            
          2    注册在Struts中
                  ActionName-conversion.properties  [针对Action]
                  xwork-conversion.properties       [针对Struts App]
          3   非 Struts 类型自定义转换 @interface，就是jdk5之后的注释
在程序中注册过的日期类型可以自动被调用，无需干涉。


##### 在Struts 2中实现IoC
- 在Struts 2中实现IoC 
- IoC 
Inversion Of Control 反转of控制,我们自己定义的类交给容器（运行上下文环境）去控制，并非在类的内部进行控制，依赖注入 （Dependency Injection） 与Ioc实际是一回事
class A   implement x
   class B   implement x
   interface x
   xxxFactory   
   xxxFactory.get(parameter);通过这种方式可以 动态的调用 某一个已知接口的实现类、或者是某一个抽象类的子类。 
在调用之前，被调用的类是未知的。
这种方式被称为接口驱动（Interface Driver Design） 
在这种方式的基础上 使用接口实现的形式对已经实现的类进行非浸入方式的注入，从而达到不修改实现类的情况下完成比原有代码更多的工作，直接后果就是延缓接口的实现，在接口实现之前可以添加代码。
这种行为被称为依赖注入（Dependency Injection） === Inversion Of Control - 配置 
把谁注入给谁，如果没有定义的话，依旧是一个典型的factory模式的代码不能完成真正的Ioc操作 ，通过 framework 进行配置和管理 - Struts2中使用 Ioc; 
加入spring-xxx.jar 【提供spring支持，spring是一个轻量级的Ioc容器,如果没有完整的jar文件 ，可以使用必须的四个包：spring-context-2.5.4.jar，spring-web-2.5.4.jar，spring-core-2.5.4.jar，spring-beans-2.5.4.jar】 加入struts2-spring-plugin-2.0.11.1.jar【用于提供在Struts中使用spring Ioc控制的链接】 - 加入配置文件 
ApplicationContext.xml 【配置Ioc中的Java Bean ( POJO,非 Ejb 的 JavaBean ) 】
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE beans PUBLIC "-//SPRING//DTD BEAN 2.0//EN"
    "http://www.springframework.org/dtd/spring-beans-2.0.dtd">
<beans>
      <bean id="" class=""> 
                <property name ="Field_Name" >
              <ref local ="another_bean_id" />
         </property >
                <property name ="Field_Name" >
                 </propertie>
      </bean> 
</beans>- find action 
- search action with classname in applicationcontext.xml 
- use a action class's instance defined by applicationcontext.xml 



##### 完成一個Struts2 with Spring的IOC程序的步驟
- 定義 Struts2中的Action 【使用 applicationContext.xml中的bean_id作為我們的Action的class】
<action name="CatchIocDataMethod"
   class="com.baoding.Action.CatchIocDataMethod">
      成為：
      <action name="CatchIocDataMethod"
   class="ActionClassId in ApplicationContext">
- 定義Spring - 拷貝jar：spring-context-2.5.4.jar，spring-web-2.5.4.jar，spring-core-2.5.4.jar，spring-beans-2.5.4.jar,struts2-spring-plugin-2.0.11.1.jar 5個文件到/WEB-INFO/lib 
- 建立文件 applicationContext.xml 到 /WEB-INFO/目錄 【如果在web.xml中定義了spring配置文件的路徑，另當別論】 

- WEB.xml中路進的配置方式 
<context-param>
          <param-name>contextConfigLocation</param-name>
          <param-value>/WEB-INF/classes/applicationContext-*.xml,classpath*:applicationContext-*.xml</param-value>
     </context-param>- - 修改struts.properties文件或者struts.xml文件，定義struts中ObjectFactory使用的容器為Spring
【struts.properties】
        struts.objectFactory = spring //去掉改行注釋
        【struts.xml】
        <constant name="struts.objectFactory" value="spring" /> 
        所有的在struts.properties文件中出現的常量都可以使用這種方式在struts.xml中進行覆蓋

- 提示信息
Initializing Struts-Spring integration  **  struts2-spring-plugin 被使用后的提示信息
Bean factory for application context .....  Spring Ioc容器成功初始化后的信](https://so.csdn.net/so/search/s.do?q=spring&t=blog)](https://so.csdn.net/so/search/s.do?q=class&t=blog)](https://so.csdn.net/so/search/s.do?q=action&t=blog)](https://so.csdn.net/so/search/s.do?q=interceptor&t=blog)](https://so.csdn.net/so/search/s.do?q=struts&t=blog)




