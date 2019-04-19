# CAS(10)-spring整合 - 零度的博客专栏 - CSDN博客
2016年05月25日 15:30:58[零度anngle](https://me.csdn.net/zmx729618)阅读数：2645
                
       众所周知，Cas是对单点登录的一种实现。本文假设读者已经了解了Cas的原理及其使用，这些内容在本文将不会讨论。Cas有Server端和Client端，Client端通常对应着我们自己的应用，[spring](http://lib.csdn.net/base/17) Security整合Cas指的就是在Spring Security应用中整合Cas
 Client，已达到使用Cas Server实现单点登录和登出的效果。本文旨在描述如何在Spring Security应用中使用Cas的单点登录。
       首先需要将Spring Security对Cas支持的jar包加入到应用的类路径中。如果我们的应用使用Maven构造的，则可以在应用的pom.xml文件中加上如下依赖。
<dependency>
<groupId>org.springframework.security</groupId>
<artifactId>spring-security-cas</artifactId>
<version>${spring.security.version}</version>
</dependency>
## 1.1     配置登录认证
       加入了spring-security-cas-xxx.jar到Spring Security应用的classpath后，我们便可以开始配置我们的Spring Security应用使用Cas进行单点登录了。
### 1.1.1配置AuthenticationEntryPoint
       首先需要做的是将应用的登录认证入口改为使用CasAuthenticationEntryPoint。所以首先我们需要配置一个CasAuthenticationEntryPoint对应的bean，然后指定需要进行登录认证时使用该AuthenticationEntryPoint。配置CasAuthenticationEntryPoint时需要指定一个ServiceProperties，该对象主要用来描述service（Cas概念）相关的属性，主要是指定在Cas Server认证成功后将要跳转的地址。
<!-- 指定登录入口为casEntryPoint -->
<security:httpentry-point-ref=*"casEntryPoint"*>
      ...
</security:http>
<!-- 认证的入口 -->
<bean
id=*"casEntryPoint"*
class=*"org.springframework.security.cas.web.CasAuthenticationEntryPoint"*>
<!-- Cas Server的登录地址，elim是我的计算机名 -->
<propertyname=*"loginUrl"*value=*"https://elim:8443/cas/login"*/>
<!-- service相关的属性 -->
<propertyname=*"serviceProperties"*ref=*"serviceProperties"*/>
</bean>
<!-- 指定service相关信息 -->
<bean
id=*"serviceProperties"*
class=*"org.springframework.security.cas.ServiceProperties"*>
<!-- Cas Server认证成功后的跳转地址，这里要跳转到我们的Spring Security应用，之后会由CasAuthenticationFilter处理，默认处理地址为/j_spring_cas_security_check
 -->
<propertyname=*"service"*
value=*"http://elim:8080/app/j_spring_cas_security_check"*/>
</bean>
### 1.1.2配置CasAuthenticationFilter
       之后我们需要配置一个CasAuthenticationFilter，并将其放置在Filter链表中CAS_FILTER的位置，以处理Cas Server认证成功后的页面跳转，用以在Spring Security中进行认证。该Filter会将Cas Server传递过来的ticket（Cas概念）封装成一个Authentication（对应UsernamePasswordAuthenticationToken，其中ticket作为该Authentication的password），然后传递给AuthenticationManager进行认证。
<security:httpentry-point-ref=*"casEntryPoint"*>
      ...
<security:custom-filterref=*"casFilter"*position=*"CAS_FILTER"*/>
      ...
</security:http>
<bean
id=*"casFilter"*
class=*"org.springframework.security.cas.web.CasAuthenticationFilter"*>
<propertyname=*"authenticationManager"*ref=*"authenticationManager"*/>
<!-- 指定处理地址，不指定时默认将会是“/j_spring_cas_security_check” -->
<propertyname=*"filterProcessesUrl"*value=*"/j_spring_cas_security_check"*/>
</bean>
### 1.1.3配置AuthenticationManager
       CasAuthenticationFilter会将封装好的包含Cas Server传递过来的ticket的Authentication对象传递给AuthenticationManager进行认证。我们知道默认的AuthenticationManager实现类为ProviderManager，而ProviderManager中真正进行认证的是AuthenticationProvider。所以接下来我们要在AuthenticationManager中配置一个能够处理CasAuthenticationFilter传递过来的Authentication对象的AuthenticationProvider实现，CasAuthenticationProvider。CasAuthenticationProvider首先会利用TicketValidator（Cas概念）对Authentication中包含的ticket信息进行认证。认证通过后将利用持有的AuthenticationUserDetailsService根据认证通过后回传的Assertion对象中拥有的username加载用户对应的UserDetails，即主要是加载用户的相关权限信息GrantedAuthority。然后构造一个CasAuthenticationToken进行返回。之后的逻辑就是正常的Spring
 Security的逻辑了。
<security:authentication-manageralias=*"authenticationManager"*>
<security:authentication-providerref=*"casAuthenticationProvider"*/>
</security:authentication-manager>
<bean
id=*"casAuthenticationProvider"*
class=*"org.springframework.security.cas.authentication.CasAuthenticationProvider"*>
<!-- 通过username来加载UserDetails -->
<propertyname=*"authenticationUserDetailsService"*>
<beanclass=*"org.springframework.security.core.userdetails.UserDetailsByNameServiceWrapper"*>
<!-- 真正加载UserDetails的UserDetailsService实现
 -->
<constructor-argref=*"userDetailsService"*/>
</bean>
</property>
<propertyname=*"serviceProperties"*ref=*"serviceProperties"*/>
<!-- 配置TicketValidator在登录认证成功后验证ticket -->
<propertyname=*"ticketValidator"*>
<beanclass=*"org.jasig.cas.client.validation.Cas20ServiceTicketValidator"*>
<!-- Cas Server访问地址的前缀，即根路径-->
<constructor-argindex=*"0"*value=*"https:// elim:8443/cas"*/>
</bean>
</property>
<propertyname=*"key"*value=*"key4CasAuthenticationProvider"*/>
</bean>
<bean
id=*"userDetailsService"*
class=*"org.springframework.security.core.userdetails.jdbc.JdbcDaoImpl"*>
<propertyname=*"dataSource"*ref=*"dataSource"*/>
</bean>
       经过以上三步配置以后，我们的Spring Security应用就已经跟Cas整合好了，可以在需要登录的时候通过Cas Server进行单点登录了。
## 1.2     单点登出
       Spring Security应用整合Cas Client配置单点登出功能实际和单独使用Cas Client配置单点登出功能一样，其根本都是通过配置一个SingleSignOutFilter响应Cas Server单点登出时的回调，配置一个SingleSignOutHttpSessionListener用于在Session过期时删除SingleSignOutFilter存放的对应信息。SingleSignOutFilter需要配置在Cas 的AuthenticationFilter之前，对于Spring
 Security应用而言，该Filter通常是配置在Spring Security的配置文件中，而且是配置在CAS_FILTER之前。所以我们可以在Spring Security的配置文件中进行如下配置。
<security:httpentry-point-ref=*"casEntryPoint"*>
<!-- SingleSignOutFilter放在CAS_FILTER之前 -->
<security:custom-filterref=*"casLogoutFilter"*before=*"CAS_FILTER"*/>
<security:custom-filterref=*"casFilter"*position=*"CAS_FILTER"*/>
      ...
</security:http>
<bean
id=*"casLogoutFilter"*
class=*"org.jasig.cas.client.session.SingleSignOutFilter"*/>
       然后跟单独使用Cas Client一样，在web.xml文件中配置一个SingleSignOutHttpSessionListener。
<listener>
<listener-class>org.jasig.cas.client.session.SingleSignOutHttpSessionListener</listener-class>
</listener>
       经过以上配置在访问Cas Server的logout地址（如：https:elim:8443/cas/logout）进行登出时，Cas Server登出后将回调其中注册的每一个Service（Cas概念，即client应用），此时在client应用中配置好的SingleSignOutFilter将处理对应Client应用的登出操作。
       虽然以上配置可以满足我们在Spring Security应用中的单点登出要求，但Cas官方文档和Spring Security官方文档都推荐我们在Cas Client应用进行登出操作时，不是直接访问Cas Server的logout，而是先登出本应用，然后告诉用户其当前登出的只是本应用，再提供一个对应Cas Server的链接，使其可以进行真正的单点登出。对此，Spring Security官方文档中给我们提供例子是提供两个LogoutFilter，一个是登出当前Spring Security应用，一个是登出Cas
 Server的。
<security:httpentry-point-ref=*"casEntryPoint"*>
<!-- 请求登出Cas Server的过滤器，放在Spring Security的登出过滤器之前
 -->
<security:custom-filterref=*"requestCasLogoutFilter"*before=*"LOGOUT_FILTER"*/>
<!-- SingleSignOutFilter放在CAS_FILTER之前 -->
<security:custom-filterref=*"casLogoutFilter"*before=*"CAS_FILTER"*/>
<security:custom-filterref=*"casFilter"*position=*"CAS_FILTER"*/>
      ...
</security:http>
<bean
id=*"requestCasLogoutFilter"*
class=*"org.springframework.security.web.authentication.logout.LogoutFilter"*>
<!-- 指定登出成功后需要跳转的地址，这里指向Cas Server的登出URL，以实现单点登出
 -->
<constructor-argvalue=*"https://elim:8443/cas/logout"*/>
<constructor-arg>
<beanclass=*"org.springframework.security.web.authentication.logout.SecurityContextLogoutHandler"*/>
</constructor-arg>
<!-- 该Filter需要处理的地址，默认是Spring Security的默认登出地址“/j_spring_security_logout”-->
<propertyname=*"filterProcessesUrl"*value=*"/j_spring_cas_security_logout"*/>
</bean>
       此外，Spring Security推荐我们在使用Cas Server的单点登出时一起使用CharacterEncodingFilter，以避免SingleSignOutFilter在获取参数时出现编码问题。
## 1.3     使用代理
       关于Cas应用使用代理的基本原理、概念等内容的介绍不在本文讨论范围之内，如需了解请读者参考其它资料，或者**参考我的另一篇博文**。本文旨在描述Spring Security应用在整合Cas后如何通过Cas Proxy访问另一个受Cas包含的应用。
       使用Cas Proxy时有两个主体，代理端和被代理端。而且我们知道代理端和被代理端针对Cas20ProxyReceivingTicketValidationFilter的配置是不一样的，虽然整合Cas的Spring Security应用不再使用Cas20ProxyReceivingTicketValidationFilter了，但其底层的核心机制是一样的。所以Cas整合Spring Security后的应用在作为代理端和被代理端时的配置也是不一样的。接下来将分开讲解Spring Security应用作为代理端和被代理端整合Cas后的配置。
### 1.3.1代理端
       首先需要为CasAuthenticationFilter多指定两个参数，proxyReceptorUrl和proxyGrantingTicketStorage。proxyReceptorUrl用以指定Cas Server在回调代理端传递pgtId和pgtIou时回调地址相对于代理端的路径，如“/proxyCallback”，CasAuthenticationFilter会根据proxyReceptorUrl来确定一个请求是否来自Cas Server针对proxy的回调。如果是则需要接收Cas
 Server传递过来的pgtId和pgtIou，并将它们保存在持有的ProxyGrantingTicketStorage中。CasAuthenticationProvider之后会从ProxyGrantingTicketStorage中获取对应的pgtId，即proxy granting ticket，并将其保存在AttributePrincipal中，而AttributePrincipal又会保存到对应的Assertion中。
<!-- 配置ProxyGrantingTicketStorage，用以保存pgtId和pgtIou
 -->
<bean
id=*"proxyGrantingTicketStorage"*class=*"org.jasig.cas.client.proxy.ProxyGrantingTicketStorageImpl"*/>
<bean
id=*"casFilter"*
class=*"org.springframework.security.cas.web.CasAuthenticationFilter"*>
<propertyname=*"authenticationManager"*ref=*"authenticationManager"*/>
<!-- 指定处理地址，不指定时默认将会是“/j_spring_cas_security_check” -->
<propertyname=*"filterProcessesUrl"*value=*"/j_spring_cas_security_check"*/>
<propertyname=*"proxyGrantingTicketStorage"*ref=*"proxyGrantingTicketStorage"*/>
<propertyname=*"proxyReceptorUrl"*value=*"/proxyCallback"*/>
</bean>
       其次是需要将CasAuthenticationProvider持有的TicketValidator由Cas20ServiceTicketValidator改成Cas20ProxyTicketValidator。其需要配置一个ProxyGrantingTicketStorage用来获取proxy granting ticket，即我们熟知的pgtId。在单独使用Cas Proxy时，Cas20ProxyReceivingTicketValidationFilter内部默认持有一个ProxyGrantingTicketStorage实现，其使用的Cas20ProxyTicketValidator也将使用该ProxyGrantingTicketStorage。整合Spring
 Security之后， Spring Security不使用Cas20ProxyReceivingTicketValidationFilter，而直接由CasAuthenticationFilter获取proxy granting ticket，由CasAuthenticationProvider对ticket进行校验。Cas20ProxyTicketValidator内部没默认的ProxyGrantingTicketStorage，所以在配置Cas20ProxyTicketValidator时我们需要给其指定一个ProxyGrantingTicketStorage实现。此外还需要为Cas20ProxyTicketValidator指定一个proxyCallbackUrl用以指定在Cas20ProxyTicketValidator通过Cas
 Server校验service ticket成功后将回调哪个地址以传递pgtId和pgtIou。proxyCallbackUrl默认情况下必须使用https协议，而应用的其它请求可以用非https协议。其它的配置和Cas20ServiceTicketValidator一样，Cas20ProxyTicketValidator的父类其实就是Cas20ServiceTicketValidator。
<bean
id=*"casAuthenticationProvider"*
class=*"org.springframework.security.cas.authentication.CasAuthenticationProvider"*>
<!-- 通过username来加载UserDetails -->
<propertyname=*"authenticationUserDetailsService"*>
<beanclass=*"org.springframework.security.core.userdetails.UserDetailsByNameServiceWrapper"*>
<!-- 真正加载UserDetails的UserDetailsService实现
 -->
<constructor-argref=*"userDetailsService"*/>
</bean>
</property>
<propertyname=*"serviceProperties"*ref=*"serviceProperties"*/>
<!-- 配置TicketValidator在登录认证成功后验证ticket -->
<propertyname=*"ticketValidator"*>
<beanclass=*"org.jasig.cas.client.validation.Cas20ProxyTicketValidator"*>
<!-- Cas Server访问地址的前缀，即根路径-->
<constructor-argindex=*"0"*value=*"https://elim:8443/cas"*/>
<!-- 指定Cas Server回调传递pgtId和pgtIou的地址，该地址必须使用https协议
 -->
<propertyname=*"proxyCallbackUrl"*value=*"https://elim:8043/app/proxyCallback"*/>
<propertyname=*"proxyGrantingTicketStorage"*ref=*"proxyGrantingTicketStorage"*/>
</bean>
</property>
<propertyname=*"key"*value=*"key4CasAuthenticationProvider"*/>
</bean>
       经过以上步骤后我们整合Cas后的Spring Security应用就可以作为代理端使用Cas proxy访问其它被Cas保护的应用了，当然前提是其它被代理端能够接受我们应用的代理，了解Cas Proxy的人应该都知道这一点，在接下来的Spring Security应用整合Cas作为被代理端中也会讲到这部分内容。这里我们假设现在有一个应用app2能够接受我们应用的代理访问，那么在基于上述配置的应用中我们可以通过如下代码访问app2。
@Controller
@RequestMapping("/cas/test")
**publicclass** CasTestController {
@RequestMapping("/getData")
**publicvoid** getDataFromApp(PrintWriter writer)
**throws** Exception {
//1、从SecurityContextHolder获取到当前的Authentication对象，其是一个CasAuthenticationToken
      CasAuthenticationToken cat = (CasAuthenticationToken)SecurityContextHolder.*getContext*().getAuthentication();
//2、获取到AttributePrincipal对象
      AttributePrincipal principal = cat.getAssertion().getPrincipal();
//3、获取对应的proxy ticket
      String proxyTicket = principal.getProxyTicketFor("http://elim:8081/app2/getData.jsp");
//4、请求被代理应用时将获取到的proxy ticket以参数ticket进行传递
      URL url = **new** URL("http://elim:8081/app2/getData.jsp?ticket=" + URLEncoder.*encode*(proxyTicket,
"UTF-8"));
      HttpURLConnection conn = (HttpURLConnection)url.openConnection();
      BufferedReader br = **new** BufferedReader(**new** InputStreamReader(conn.getInputStream(),
"UTF-8"));
      StringBuffer content = **new** StringBuffer();
      String line = **null**;
**while** ((line=br.readLine()) !=
**null**) {
         content.append(line).append("<br/>");
      }
      writer.write(content.toString());
   }
}
       需要注意的是通过AttributePrincipal的getProxyTicketFor()方法获取proxy ticket时，每调用一次都会获取一个全新的proxy ticket。用户可以根据自己的需要将获取到的proxy ticket按照指定的URL缓存起来，以避免每次都去针对同一个URL获取一个全新的proxy ticket。此外，如果在被代理端认证时根据proxy ticket缓存了Authentication的话也需要我们在代理端保证针对同一URL传递过去的proxy ticket是一样的，否则被代理端针对proxy
 ticket缓存Authentication的功能就没用了。
### 1.3.2被代理端
       Spring Security应用整合Cas使用Cas Proxy作为被代理端时主要需要进行三点修改。
       第一点是通过ServiceProperties指定CasAuthenticationFilter的authenticateAllArtifacts为true，这样CasAuthenticationFilter将会尝试对所有ticket进行认证，而不是只认证来自filterProccessUrl指定地址的请求。这样代理端在请求被代理端的资源时将proxy ticket以参数ticket进行传递时，CasAuthenticationFilter才会让CasAuthenticationProvider对proxy
 ticket进行校验，这样我们的请求才有可能被CasAuthenticationProvider认证成功并请求到真正的资源。
       第二点是指定CasAuthenticationFilter使用的AuthenticationDetailsSource为ServiceAuthenticationDetailsSource。CasAuthenticationFilter默认使用的是WebAuthenticationDetailsSource。ServiceAuthenticationDetailsSource将构建一个ServiceAuthenticationDetails对象作为当前Authentication的details对象。ServiceAuthenticationDetailsSource构建的ServiceAuthenticationDetails对象会将当前请求的地址构建为一个serviceUrl，通过其getServiceUrl()方法可以获取到该serviceUrl地址。之后该Authentication对象传递到CasAuthenticationProvider进行认证时就可以从Authentication的details中获取到对应的serviceUrl，并在通过Cas
 Server对代理端以参数ticket传递过来的proxy ticket进行验证时连同对应的serviceUrl一起传递过去。因为之前代理端申请proxy ticket时就是通过该serviceUrl进行申请的，Cas Server需要对于它们的配对来验证对应的proxy ticket是否有效。
       第三点是将CasAuthenticationProvider的TicketValidator由Cas20ServiceTicketValidator改为Cas20ProxyTicketValidator，因为Cas Proxy被代理端需要调用Cas Server的proxyValidator对代理端传递过来的proxy ticket进行验证。此外需要通过acceptAnyProxy或allowedProxyChains指定将接受哪些代理。acceptAnyProxy用以指定是否接受所有的代理，可选值为true或false。allowedProxyChains则用以指定具体接受哪些代理，其对应的值是代理端在获取pgtId时提供给Cas
 Server的回调地址，如我们需要接受前面示例中代理端的代理，则我们的allowedProxyChains的值应该是“https://elim:8043/app/proxyCallback”。如果需要接受多个代理端的代理，则在指定allowedProxyChains时多个代理端回调地址应各占一行。
       针对以上三点，我们的Spring Security应用整合Cas作为Cas Proxy的被代理端时需要对我们的配置进行如下改造。
<!-- 指定service相关信息 -->
<bean
id=*"serviceProperties"*
class=*"org.springframework.security.cas.ServiceProperties"*>
<!-- Cas Server认证成功后的跳转地址，这里要跳转到我们的Spring Security应用，之后会由CasAuthenticationFilter处理，默认处理地址为/j_spring_cas_security_check
 -->
<propertyname=*"service"*
value=*"http://elim:8083/app2/j_spring_cas_security_check"*/>
<!-- 通过ServiceProperties指定CasAuthenticationFilter的authenticateAllArtifacts为true
 -->
<propertyname=*"authenticateAllArtifacts"*value=*"true"*/>
</bean>
<bean
id=*"casFilter"*
class=*"org.springframework.security.cas.web.CasAuthenticationFilter"*>
<propertyname=*"authenticationManager"*ref=*"authenticationManager"*/>
<!-- 指定处理地址，不指定时默认将会是“/j_spring_cas_security_check” -->
<propertyname=*"filterProcessesUrl"*value=*"/j_spring_cas_security_check"*/>
<!-- 通过ServiceProperties指定CasAuthenticationFilter的authenticateAllArtifacts为true 
 -->
<propertyname=*"serviceProperties"*ref=*"serviceProperties"*/>
<!-- 指定使用的AuthenticationDetailsSource为ServiceAuthenticationDetailsSource -->
<propertyname=*"authenticationDetailsSource"*>
<beanclass=*"org.springframework.security.cas.web.authentication.ServiceAuthenticationDetailsSource"*/>
</property>
</bean>
<bean
id=*"casAuthenticationProvider"*
class=*"org.springframework.security.cas.authentication.CasAuthenticationProvider"*>
<!-- 通过username来加载UserDetails -->
<propertyname=*"authenticationUserDetailsService"*>
<beanclass=*"org.springframework.security.core.userdetails.UserDetailsByNameServiceWrapper"*>
<!-- 真正加载UserDetails的UserDetailsService实现
 -->
<constructor-argref=*"userDetailsService"*/>
</bean>
</property>
<propertyname=*"serviceProperties"*ref=*"serviceProperties"*/>
<!-- 配置TicketValidator在登录认证成功后验证ticket -->
<propertyname=*"ticketValidator"*>
<beanclass=*"org.jasig.cas.client.validation.Cas20ProxyTicketValidator"*>
<!-- Cas Server访问地址的前缀，即根路径-->
<constructor-argindex=*"0"*value=*"https://elim:8443/cas"*/>
<propertyname=*"allowedProxyChains"*>
<value>https://elim:8043/app/proxyCallback</value>
</property>
</bean>
</property>
<propertyname=*"key"*value=*"key4CasAuthenticationProvider"*/>
</bean>
       此外，对于被代理端而言，代理端在对其进行访问时都被认为是无状态的。对于无状态的认证CasAuthenticationProvider将在认证成功后将对应的Authentication对象以proxy tickit为key存放到所持有的StatelessTicketCache中，然后在下次代理端访问时将优先根据代理端传递过来的proxy ticket从StatelessTicketCache中获取Authentication对象，如果存在则不再进行认证，否则将继续进行认证。CasAuthenticationProvider默认持有的StatelessTicketCache为NullStatelessTicketCache，其所有的实现都是空的。所以默认情况下，被代理端在被代理端访问时将每次都对代理端进行认证。如果用户不希望在被代理端每次都对代理端的请求进行认证，则可以为被代理端的CasAuthenticationProvider指定一个StatelessTicketCache。用户可以实现自己的StatelessTicketCache，并指定CasAuthenticationProvider使用的StatelessTicketCache为该StatelessTicketCache。不过也可以使用Spring
 Security为我们提供的EhCacheBasedTicketCache。EhCacheBasedTicketCache是基于Ehcache实现的一个StatelessTicketCache。以下是一个为CasAuthenticationProvider配置EhCacheBasedTicketCache的示例。
<bean
id=*"casAuthenticationProvider"*
class=*"org.springframework.security.cas.authentication.CasAuthenticationProvider"*>
……
<propertyname=*"statelessTicketCache"*>
<beanclass=*"org.springframework.security.cas.authentication.EhCacheBasedTicketCache"*>
<!-- Ehcache对象 -->
<propertyname=*"cache"*ref=*"proxyTicketCache"*/>
</bean>
</property>
……
</bean>
<!-- 定义一个Ehcache -->
<bean
id=*"proxyTicketCache"*
class=*"org.springframework.cache.ehcache.EhCacheFactoryBean"*>
<propertyname=*"cacheName"*value=*"proxyTicketCache"*/>
<propertyname=*"timeToLive"*value=*"600"*/>
</bean>
       需要注意的是在代理端通过AttributePrincipal的getProxyTicketFor()方法获取到的proxy ticket每次都是不一样的，所以在被代理端通过StatelessTicketCache根据proxy ticket缓存认证对象Authentication时只有在同一proxy ticket能够请求多次的情况下才会有用，这也就要求我们在代理端同样能将proxy ticket缓存起来，以在请求同一地址时能使用相同的proxy ticket。
### 1.3.3既为代理端又为被代理端
       Cas Proxy的代理端和被代理端是相互独立的，所以一个Cas应用既可以作为代理端去访问其它Cas应用，也可以作为被代理端被其它应用访问。当Spring Security应用整合Cas后既想作为Cas Proxy的代理端访问其它Cas应用，也想作为被代理端被其它Cas应用访问时只需要将上述作为代理端的配置和作为被代理端的配置整到一起就行了。以下是一段示例代码。
<!-- 指定service相关信息 -->
<bean
id=*"serviceProperties"*
class=*"org.springframework.security.cas.ServiceProperties"*>
<!-- Cas Server认证成功后的跳转地址，这里要跳转到我们的Spring Security应用，之后会由CasAuthenticationFilter处理，默认处理地址为/j_spring_cas_security_check
 -->
<propertyname=*"service"*
value=*"http://elim:8080/app /j_spring_cas_security_check"*/>
<propertyname=*"authenticateAllArtifacts"*value=*"true"*/>
</bean>
<!-- 配置ProxyGrantingTicketStorage，用以保存pgtId和pgtIou
 -->
<bean
id=*"proxyGrantingTicketStorage"*class=*"org.jasig.cas.client.proxy.ProxyGrantingTicketStorageImpl"*/>
<bean
id=*"casFilter"*
class=*"org.springframework.security.cas.web.CasAuthenticationFilter"*>
<propertyname=*"authenticationManager"*ref=*"authenticationManager"*/>
<!-- 指定处理地址，不指定时默认将会是“/j_spring_cas_security_check” -->
<propertyname=*"filterProcessesUrl"*value=*"/j_spring_cas_security_check"*/>
<propertyname=*"proxyGrantingTicketStorage"*ref=*"proxyGrantingTicketStorage"*/>
<propertyname=*"proxyReceptorUrl"*value=*"/proxyCallback"*/>
<!-- 通过ServiceProperties指定CasAuthenticationFilter的authenticateAllArtifacts为true 
 -->
<propertyname=*"serviceProperties"*ref=*"serviceProperties"*/>
<!-- 指定使用的AuthenticationDetailsSource为ServiceAuthenticationDetailsSource -->
<propertyname=*"authenticationDetailsSource"*>
<beanclass=*"org.springframework.security.cas.web.authentication.ServiceAuthenticationDetailsSource"*/>
</property>
</bean>
<bean
id=*"casAuthenticationProvider"*
class=*"org.springframework.security.cas.authentication.CasAuthenticationProvider"*>
<!-- 通过username来加载UserDetails -->
<propertyname=*"authenticationUserDetailsService"*>
<beanclass=*"org.springframework.security.core.userdetails.UserDetailsByNameServiceWrapper"*>
<!-- 真正加载UserDetails的UserDetailsService实现
 -->
<constructor-argref=*"userDetailsService"*/>
</bean>
</property>
<propertyname=*"serviceProperties"*ref=*"serviceProperties"*/>
<!-- 配置TicketValidator在登录认证成功后验证ticket -->
<propertyname=*"ticketValidator"*>
<beanclass=*"org.jasig.cas.client.validation.Cas20ProxyTicketValidator"*>
<!-- Cas Server访问地址的前缀，即根路径-->
<constructor-argindex=*"0"*value=*"https://elim:8443/cas"*/>
<!-- 指定Cas Server回调传递pgtId和pgtIou的地址，该地址必须使用https协议
 -->
<propertyname=*"proxyCallbackUrl"*value=*"https://elim:8043/app/proxyCallback"*/>
<propertyname=*"proxyGrantingTicketStorage"*ref=*"proxyGrantingTicketStorage"*/>
<!-- 作为被代理端时配置接收任何代理 -->
<propertyname=*"acceptAnyProxy"*value=*"true"*/>
</bean>
</property>
<propertyname=*"key"*value=*"key4CasAuthenticationProvider"*/>
</bean>
（注：本文是基于Spring Security3.1.6、Cas Server3.5.2所写，原文地址：[http://haohaoxuexi.iteye.com/blog/2270446](http://haohaoxuexi.iteye.com/blog/2270446)）
