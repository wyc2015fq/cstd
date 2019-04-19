# CAS(7)-建立使用Cas进行单点登录的应用 - 零度的博客专栏 - CSDN博客
2016年05月25日 14:55:56[零度anngle](https://me.csdn.net/zmx729618)阅读数：1630
                
      根据之前的描述我们知道，Cas由两部分组成，Cas Server和Cas Client。Cas Server是Cas自己的服务端，而Cas Client是Cas客户端，其需要与我们自己的应用进行集成。
## 1.1     加入cas-client-core-xxx.jar到classpath
       在我们下载的Cas Client压缩包的modules目录下可以找到一个名为cas-client-core-xxx.jar的jar文件，首先需要将该jar包加入我们应用的类路径下，笔者这里使用的是cas-client-core-3.1.11.jar。如果用户的应用是使用Maven构造的，则可以在应用的pom.xml文件中加入如下依赖。
<dependency>
<groupId>org.jasig.cas.client</groupId>
<artifactId>cas-client-core</artifactId>
<version>3.1.11</version>
</dependency>
## 1.2     配置Filter 
       然后需要我们在应用的web.xml文件中配置四个Filter，这四个Filter必须按照固定的顺序来进行配置，而且它们必须配置在应用的其它Filter之前。它们的先后顺序要求如下：
l  AuthenticationFilter
l  TicketValidationFilter
l  HttpServletRequestWrapperFilter
l  AssertionThreadLocalFilter
       这些Filter有的必须指定某些参数，有的可以指定某些参数，这些参数可以通过context-param来指定，也可以通过init-param来指定。Cas Client默认会先从init-param取，没取到则从context-param取，所以当init-param和context-param都指定了某个参数时，init-param指定的将拥有更高的优先级。所以当多个Filter需要共用一个参数时，我们可以把它定义为context-param。
### 1.2.1    AuthenticationFilter
       AuthenticationFilter用来拦截所有的请求，用以判断用户是否需要通过Cas Server进行认证，如果需要则将跳转到Cas Server的登录页面。如果不需要进行登录认证，则请求会继续往下执行。
     AuthenticationFilter有两个用户必须指定的参数，一个是用来指定Cas Server登录地址的**casServerLoginUrl**，另一个是用来指定认证成功后需要跳转地址的**serverName**或**service**。service和serverName只需要指定一个就可以了。当两者都指定了，参数service将具有更高的优先级，即将以service指定的参数值为准。service和serverName的区别在于service指定的是一个确定的URL，认证成功后就会确切的跳转到service指定的URL；而serverName则是用来指定主机名，其格式为{protocol}:{hostName}:{port}，如：https://localhost:8443，当指定的是serverName时，AuthenticationFilter将会把它附加上当前请求的URI，以及对应的查询参数来构造一个确定的URL，如指定serverName为“http://localhost”，而当前请求的URI为“/app”，查询参数为“a=b&b=c”，则对应认证成功后的跳转地址将为“http://localhost/app?a=b&b=c”。
       除了上述必须指定的参数外，AuthenticationFilter还可以指定如下可选参数：
l  **renew**：当指定renew为true时，在请Cas Server时将带上参数“renew=true”，默认为false。
l  **gateway**：指定gateway为true时，在请求Cas Server时将带上参数“gateway=true”，默认为false。
l  **artifactParameterName**：指定ticket对应的请求参数名称，默认为ticket。
l  **serviceParameterName**：指定service对应的请求参数名称，默认为service。
       如下是一个配置AuthenticationFilter的示例，serverName由于在接下来配置的Filter中还要用，所以利用context-param将其配置为一个公用的参数。“elim”对应我的电脑名。
<context-param>
<param-name>serverName</param-name>
<param-value>http://elim:8080</param-value>
</context-param>
<filter>
<filter-name>casAuthenticationFilter</filter-name>
<filter-class>org.jasig.cas.client.authentication.AuthenticationFilter</filter-class>
<init-param>
<param-name>casServerLoginUrl</param-name>
<param-value>https://elim:8443/cas/login</param-value>
</init-param>
</filter>
<filter-mapping>
<filter-name>casAuthenticationFilter</filter-name>
<url-pattern>/*</url-pattern>
</filter-mapping>
### 1.2.2    TicketValidationFilter
       在请求通过AuthenticationFilter的认证之后，如果请求中携带了参数ticket则将会由TicketValidationFilter来对携带的ticket进行校验。TicketValidationFilter只是对验证ticket的这一类Filter的统称，其并不对应Cas Client中的一个具体类型。Cas Client中有多种验证ticket的Filter，都继承自AbstractTicketValidationFilter，它们的验证逻辑都是一致的，都有AbstractTicketValidationFilter实现，所不同的是使用的TicketValidator不一样。笔者这里将以Cas10TicketValidationFilter为例，其它还有Cas20ProxyReceivingTicketValidationFilter和Saml11TicketValidationFilter。
<filter>
<filter-name>casTicketValidationFilter</filter-name>
<filter-class>org.jasig.cas.client.validation.Cas10TicketValidationFilter</filter-class>
<init-param>
<param-name>casServerUrlPrefix</param-name>
<param-value>https://elim:8443/cas</param-value>
</init-param>
</filter>
<filter-mapping>
<filter-name>casTicketValidationFilter</filter-name>
<url-pattern>/*</url-pattern>
</filter-mapping>
       必须指定的参数：
l  **casServerUrlPrefix**：用来指定Cas Server对应URL地址的前缀，如上面示例的“https://elim:8443/cas”。
l  **serverName或service**：语义跟前面介绍的一致。
       可选参数：
l  **redirectAfterValidation**：表示是否验证通过后重新跳转到该URL，但是不带参数ticket，默认为true。
l  **useSession**：在验证ticket成功后会生成一个Assertion对象，如果useSession为true，则会将该对象存放到Session中。如果为false，则要求每次请求都需要携带ticket进行验证，显然useSession为false跟redirectAfterValidation为true是冲突的。默认为true。
l  exceptionOnValidationFailure ：表示ticket验证失败后是否需要抛出异常，默认为true。
l  **renew**：当值为true时将发送“renew=true”到Cas
 Server，默认为false。
### 1.2.3    HttpServletRequestWrapperFilter
       HttpServletRequestWrapperFilter用于将每一个请求对应的HttpServletRequest封装为其内部定义的CasHttpServletRequestWrapper，该封装类将利用之前保存在Session或request中的Assertion对象重写HttpServletRequest的getUserPrincipal()、getRemoteUser()和isUserInRole()方法。这样在我们的应用中就可以非常方便的从HttpServletRequest中获取到用户的相关信息。以下是一个配置HttpServletRequestWrapperFilter的示例：
<filter>
<filter-name>casHttpServletRequestWrapperFilter</filter-name>
<filter-class>org.jasig.cas.client.util.HttpServletRequestWrapperFilter</filter-class>
</filter>
<filter-mapping>
<filter-name>casHttpServletRequestWrapperFilter</filter-name>
<url-pattern>/*</url-pattern>
</filter-mapping>
### 1.2.4    AssertionThreadLocalFilter
       AssertionThreadLocalFilter是为了方便用户在应用的其它地方获取Assertion对象，其会将当前的Assertion对象存放到当前的线程变量中，那么以后用户在程序的任何地方都可以从线程变量中获取当前Assertion，无需再从Session或request中进行解析。该线程变量是由AssertionHolder持有的，我们在获取当前的Assertion时也只需要通过AssertionHolder的getAssertion()方法获取即可，如：
   Assertion assertion = AssertionHolder.*getAssertion*();
       像AssertionThreadLocalFilter这种设计理念是非常好的，实际应用中使用的也比较多，Spring Security中也有用到这种理念。为了便于大家了解，特贴出AssertionHolder的源码如下：
**public class** AssertionHolder {
/**
     * ThreadLocal to hold the Assertion for Threads to access.
     */
**private static final** ThreadLocal
*threadLocal* = **new** ThreadLocal();
/**
     * Retrieve the assertion from the ThreadLocal.
     */
**public static** Assertion getAssertion() {
**return** (Assertion) *threadLocal*.get();
    }
/**
     * Add the Assertion to the ThreadLocal.
     */
**public static void** setAssertion(**final** Assertion assertion) {
*threadLocal*.set(assertion);
    }
/**
     * Clear the ThreadLocal.
     */
**public static void** clear() {
*threadLocal*.set(**null**);
    }
}
       以下是配置AssertionThreadLocalFilter的示例：
<filter>
<filter-name>casAssertionThreadLocalFilter</filter-name>
<filter-class>org.jasig.cas.client.util.AssertionThreadLocalFilter</filter-class>
</filter>
<filter-mapping>
<filter-name>casAssertionThreadLocalFilter</filter-name>
<url-pattern>/*</url-pattern>
</filter-mapping>
### 1.2.5    基于Spring的Filter配置
       使用Cas单点登录的应用需要我们在应用的web.xml文件中配置上述介绍的四个Filter，但如果用户的应用是使用Spring开发的，则我们可以只在web.xml文件中配置四个Spring的DelegatingFilterProxy用来代理需要配置的四个Filter，对应的Filter名称对应我们需要代理的Spring ApplicationContext中bean的名称，此时我们需要将对应的Filter配置为Spring ApplicationContext中的一个bean对象。所以此时对应的web.xml文件的定义应该是这样的：
<filter>
<filter-name>casAuthenticationFilter</filter-name>
<filter-class>org.springframework.web.filter.DelegatingFilterProxy</filter-class>
</filter>
<filter-mapping>
<filter-name>casAuthenticationFilter</filter-name>
<url-pattern>/*</url-pattern>
</filter-mapping>
<filter>
<filter-name>casTicketValidationFilter</filter-name>
<filter-class>org.springframework.web.filter.DelegatingFilterProxy</filter-class>
</filter>
<filter-mapping>
<filter-name>casTicketValidationFilter</filter-name>
<url-pattern>/*</url-pattern>
</filter-mapping>
<filter>
<filter-name>casHttpServletRequestWrapperFilter</filter-name>
<filter-class>org.springframework.web.filter.DelegatingFilterProxy</filter-class>
</filter>
<filter-mapping>
<filter-name>casHttpServletRequestWrapperFilter</filter-name>
<url-pattern>/*</url-pattern>
</filter-mapping>
<filter>
<filter-name>casAssertionThreadLocalFilter</filter-name>
<filter-class>org.springframework.web.filter.DelegatingFilterProxy</filter-class>
</filter>
<filter-mapping>
<filter-name>casAssertionThreadLocalFilter</filter-name>
<url-pattern>/*</url-pattern>
</filter-mapping>
       而对应的Filter应该都以对应的名称定义为Spring ApplicationContext中的一个bean。
<bean
name=*"casAuthenticationFilter"*
class=*"org.jasig.cas.client.authentication.AuthenticationFilter"*
p:casServerLoginUrl=*"https://elim:8443/cas/login"*p:renew=*"false"*
p:gateway=*"false"*p:serverName=*"http://elim:8080"*/>
<bean
name=*"casTicketValidationFilter"*
class=*"org.jasig.cas.client.validation.Cas10TicketValidationFilter"*
p:serverName=*"http://elim:8080"*p:redirectAfterValidation=*"true"*>
<propertyname=*"ticketValidator"*>
<beanclass=*"org.jasig.cas.client.validation.Cas10TicketValidator"*>
<!-- 对应于casServerUrlPrefix -->
<constructor-argindex=*"0"*value=*"https://elim:8443/cas"*/>
</bean>
</property>
</bean>
<bean
id=*"casHttpServletRequestWrapperFilter"*class=*"org.jasig.cas.client.util.HttpServletRequestWrapperFilter"*/>
<bean
id=*"casAssertionThreadLocalFilter"*class=*"org.jasig.cas.client.util.AssertionThreadLocalFilter"*/>
## 1.3     添加证书到信任库 
       在ticket验证成功后，还需要验证证书，这需要我们将之前建立的证书导出并添加到当前JRE的证书信任库中，否则将验证失败。JRE在寻找证书时将根据当前使用的host来寻找，且会用该host匹配之前创建证书时指定的用户名称，如果匹配则表示找到。这也就意味着我们在**创建证书时指定的用户名称需要是我们的host**。我的机器名称为“elim”，我就把它作为我的host，那么对应的证书应该这样创建。
keytool -genkey -keyalg RSA -alias tomcat -dname "cn=elim" -storepass changeit
       该语句是对我们之前介绍的keytool -genkey -alias tomcat -keyalg RSA的精写，它已经通过相应的参数指定了对应的参数值，而不需要再与用户交互了。如果还用之前的语句生成证书的话，那么对应的值应该这样填：
![](http://dl2.iteye.com/upload/attachment/0101/9804/2b87930f-9e75-34bf-a225-088580045ce7.png)
       之后会在用户的对应目录下生成一个.keystore文件。之后需要将该文件导出为一个证书到%JAVA_HOME%/jre/lib/security目录下，对应指令为：
keytool -export -alias tomcat -file %JAVA_HOME%/jre/lib/security/tomcat.crt -storepass changeit
       之后需要将导出的tomcat.crt证书添加到运行时使用的JRE的受信任证书库中，此时如果出现异常可将原本%JAVA_HOME%/jre/lib/security目录下的cacerts删除后继续执行以下指令。
keytool -import -alias tomcat -file %JAVA_HOME%/jre/lib/security/tomcat.crt -keystore %JAVA_HOME%/jre/lib/security/cacerts -storepass changeit
       经过以上几步后就可以启用我们自己的Cas Client应用了，然后初次访问该应用时就会跳转到Cas Server进行登录认证。认证成功后将跳转到我们自己的Client应用进行ticket的验证，验证通过后就可以自由的访问我们的Client应用了。
（注：本文是基于Cas Server3.5.2和Cas Client3.1.11所写，原文地址：[http://haohaoxuexi.iteye.com/blog/2142631](http://haohaoxuexi.iteye.com/blog/2142631)）
