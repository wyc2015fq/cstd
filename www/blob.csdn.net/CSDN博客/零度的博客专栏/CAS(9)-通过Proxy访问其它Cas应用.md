# CAS(9)-通过Proxy访问其它Cas应用 - 零度的博客专栏 - CSDN博客
2016年05月25日 15:20:06[零度anngle](https://me.csdn.net/zmx729618)阅读数：3053
                
       考虑这样一种场景：有两个应用App1和App2，它们都是受Cas Server保护的，即请求它们时都需要通过Cas Server的认证。现需要在App1中通过Http请求访问App2，显然该请求将会被App2配置的Cas的AuthenticationFilter拦截并转向Cas Server，Cas Server将引导用户进行登录认证，这样我们也就不能真正的访问到App2了。针对这种应用场景，Cas也提供了对应的支持。
## 1.1     原理 
       Cas Proxy可以让我们轻松的通过App1访问App2时通过Cas Server的认证，从而访问到App2。其主要原理是这样的，App1先通过Cas Server的认证，然后向Cas Server申请一个针对于App2的proxy ticket，之后在访问App2时把申请到的针对于App2的proxy ticket以参数ticket传递过去。App2的AuthenticationFilter将拦截到该请求，发现该请求携带了ticket参数后将放行交由后续的Ticket Validation
 Filter处理。Ticket Validation Filter将会传递该ticket到Cas Server进行认证，显然该ticket是由Cas Server针对于App2发行的，App2在申请校验时是可以校验通过的，这样我们就可以正常的访问到App2了。针对Cas Proxy的原理，官网有一张图很能说明问题，如下所示。
![](http://dl2.iteye.com/upload/attachment/0102/2261/7f6e09a2-2234-3d79-9f4a-06feed9837ac.jpg)
## 1.2     配置 
       Cas Proxy实现的核心是Cas20ProxyReceivingTicketValidationFilter，该Filter是Ticket Validation Filter的一种。使用Cas Proxy时我们需要使用Cas20ProxyReceivingTicketValidationFilter作为我们的Ticket Validation Filter，而且对于代理端而言该Filter需要放置在AuthenticationFilter之前。对于上述应用场景而言，App1就是我们的代理端，而App2就是我们的被代理端。Cas20ProxyReceivingTicketValidationFilter在代理端与被代理端的配置是不一样的。我们先来看一下在代理端的配置。
### 1.2.1代理端
       既然Cas20ProxyReceivingTicketValidationFilter是一个Ticket Validation Filter，所以之前我们介绍的Ticket Validation Filter需要配置的参数，在这里也需要配置，Ticket Validation Filter可以配置的参数这里也可以配置。所不同的是对于代理端的Cas20ProxyReceivingTicketValidationFilter必须指定另外的两个参数，proxyCallbackUrl和proxyReceptorUrl。
l  **proxyCallbackUrl**：用于指定一个回调地址，在代理端通过Cas Server校验ticket成功后，Cas Server将回调该地址以传递pgtId和pgtIou，Cas20ProxyReceivingTicketValidationFilter在接收到对应的响应后会将它们保存在内部持有的ProxyGrantingTicketStorage中。之后在对传递过来的ticket进行validate的时候又会根据pgtIou从ProxyGrantingTicketStorage中获取对应的pgtId，用以保存在AttributePrincipal中，而AttributePrincipal又会保存在Assertion中。proxyCallbackUrl因为是指定Cas
 Server回调的地址，所以其必须是一个可以供外部访问的绝对地址。此外，因为Cas Server默认只回调使用安全通道协议https进行通信的地址，所以我们的proxyCallbackUrl需要是一个使用https协议访问的地址。
l  **proxyReceptorUrl**：该地址是proxyCallbackUrl相对于代理端的一个地址，Cas20ProxyReceivingTicketValidationFilter将根据该地址来决定请求是否来自Cas Server的回调。
       下面是一个Cas20ProxyReceivingTicketValidationFilter在代理端配置的示例，需要注意的是该Filter需要配置在AuthenticationFilter之前，所以完整配置如下：
<context-param>
<param-name>serverName</param-name>
<param-value>https://elim:8043</param-value>
</context-param>
<filter>
<filter-name>proxyValidationFilter</filter-name>
<filter-class>org.jasig.cas.client.validation.Cas20ProxyReceivingTicketValidationFilter</filter-class>
<init-param>
<param-name>casServerUrlPrefix</param-name>
<param-value>https://elim:8443/cas</param-value>
</init-param>
<init-param>
<param-name>proxyCallbackUrl</param-name>
<param-value>https://elim:8043/app1/proxyCallback</param-value>
</init-param>
<init-param>
<param-name>proxyReceptorUrl</param-name>
<param-value>/proxyCallback</param-value>
</init-param>
</filter>
<filter-mapping>
<filter-name>proxyValidationFilter</filter-name>
<url-pattern>/*</url-pattern>
</filter-mapping>
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
<filter>
<filter-name>casHttpServletRequestWrapperFilter</filter-name>
<filter-class>org.jasig.cas.client.util.HttpServletRequestWrapperFilter</filter-class>
</filter>
<filter-mapping>
<filter-name>casHttpServletRequestWrapperFilter</filter-name>
<url-pattern>/*</url-pattern>
</filter-mapping>
<filter>
<filter-name>casAssertionThreadLocalFilter</filter-name>
<filter-class>org.jasig.cas.client.util.AssertionThreadLocalFilter</filter-class>
</filter>
<filter-mapping>
<filter-name>casAssertionThreadLocalFilter</filter-name>
<url-pattern>/*</url-pattern>
</filter-mapping>
### 1.2.2被代理端
       在被代理端Cas20ProxyReceivingTicketValidationFilter是扮演Ticket Validation Filter的角色，它可以验证正常通过Cas Server登录认证成功后返回的ticket，也可以认证来自其它代理端传递过来的proxy ticket，当然，最终的认证都是通过Cas Server来完成的。既然Cas20ProxyReceivingTicketValidationFilter在被代理端是作为Ticket Validation Filter来使用的，所以Ticket
 Validation Filter可以有的参数其都可以配置。在被代理端需要配置一个参数用以表示接受来自哪些应用的代理，这个参数可以是acceptAnyProxy，也可以是allowedProxyChains。acceptAnyProxy表示接受所有的，其对应的参数值是true或者false；而allowedProxyChains则用以指定具体接受哪些应用的代理，多个应用就写多行，allowedProxyChains的值对应的是代理端提供给Cas Server的回调地址，如果使用前文示例的代理端配置，我们就可以指定被代理端的allowedProxyChains为“https://elim:8043/app1/proxyCallback”，这样当app1作为代理端来访问该被代理端时就能通过验证，得到正确的响应。下面是一个被代理端配置Cas20ProxyReceivingTicketValidationFilter的完整配置示例。
<context-param>
<param-name>serverName</param-name>
<param-value>http://elim:8081</param-value>
</context-param>
<filter>
<filter-name>casSingleSignOutFilter</filter-name>
<filter-class>org.jasig.cas.client.session.SingleSignOutFilter</filter-class>
</filter>
<filter-mapping>
<filter-name>casSingleSignOutFilter</filter-name>
<url-pattern>/*</url-pattern>
</filter-mapping>
<filter>
<filter-name>casAuthenticationFilter</filter-name>
<filter-class>org.jasig.cas.client.authentication.AuthenticationFilter</filter-class>
<init-param>
<param-name>casServerLoginUrl</param-name>
<param-value>https:// elim:8443/cas/login</param-value>
</init-param>
</filter>
<filter-mapping>
<filter-name>casAuthenticationFilter</filter-name>
<url-pattern>/*</url-pattern>
</filter-mapping>
<filter>
<filter-name>proxyValidationFilter</filter-name>
<filter-class>org.jasig.cas.client.validation.Cas20ProxyReceivingTicketValidationFilter</filter-class>
<init-param>
<param-name>casServerUrlPrefix</param-name>
<param-value>https://elim:8443/cas</param-value>
</init-param>
<init-param>
<param-name>acceptAnyProxy</param-name>
<param-value>true</param-value>
</init-param>
</filter>
<filter-mapping>
<filter-name>proxyValidationFilter</filter-name>
<url-pattern>/*</url-pattern>
</filter-mapping>
<filter>
<filter-name>casHttpServletRequestWrapperFilter</filter-name>
<filter-class>org.jasig.cas.client.util.HttpServletRequestWrapperFilter</filter-class>
</filter>
<filter-mapping>
<filter-name>casHttpServletRequestWrapperFilter</filter-name>
<url-pattern>/*</url-pattern>
</filter-mapping>
<filter>
<filter-name>casAssertionThreadLocalFilter</filter-name>
<filter-class>org.jasig.cas.client.util.AssertionThreadLocalFilter</filter-class>
</filter>
<filter-mapping>
<filter-name>casAssertionThreadLocalFilter</filter-name>
<url-pattern>/*</url-pattern>
</filter-mapping>
## 1.3     请求示例 
       配置好以后接下来将展示一个app1作为代理端访问app2的应用示例。该示例的重点在于app1的请求发起，对于需要请求的app2端的内容我们假设就是一个简单的jsp文件，其简单的输出一些文本。对于代理端而言，其请求的发起通常需要经过如下步骤：
       1、获取到当前的AttributePrincipal对象，如果当前可以获取到request对象并且使用了HttpServletRequestWrapperFilter，我们则可以直接从request中获取。
   AttributePrincipal principal = (AttributePrincipal) req.getUserPrincipal();
       当然，如果使用了AssertionThreadLocalFilter，我们也可以从AssertionHolder中获取Assertion，进而获取到对应的AttributePrincipal对象。
   AttributePrincipal principal = AssertionHolder.*getAssertion*().getPrincipal();
       2、通过AttributePrincipal获取针对于被代理端对应的proxy ticket，该操作将促使AttributePrincipal向Cas Server发起请求，从而获取到对应的proxy ticket。针对同一URL每次从Cas Server请求获取到的proxy ticket都是不一样的。以下是一个获取针对于“http://elim:8081/app2/getData.jsp”的proxy ticket的示例：
   String proxyTicket = principal.getProxyTicketFor("http://elim:8081/app2/getData.jsp");
       3、在请求被代理端时将获取到的proxy ticket以参数ticket一起传递过去，如：
   URL url = **new** URL("http://elim:8081/app2/getData.jsp?ticket=" + proxyTicket);
       完整的示例代码如下所示：
@WebServlet(name="casProxyTest", urlPatterns="/cas/proxy/test")
**public class** CasProxyTestServlet
**extends** HttpServlet {
**protected void** doGet(HttpServletRequest req, HttpServletResponse resp)
**throws** ServletException, IOException {
      doPost(req, resp);
   }
**protected void** doPost(HttpServletRequest req, HttpServletResponse resp)
**throws** ServletException, IOException {
//1、获取到AttributePrincipal对象
      AttributePrincipal principal = AssertionHolder.*getAssertion*().getPrincipal();
//2、获取对应的proxy ticket
      String proxyTicket = principal.getProxyTicketFor("http://elim:8081/app/getData.jsp");
//3、请求被代理应用时将获取到的proxy ticket以参数ticket进行传递
      URL url = **new** URL("http://elim:8081/app/getData.jsp?ticket=" + URLEncoder.*encode*(proxyTicket,
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
      resp.getWriter().write(content.toString());
   }
}
（注：本文是基于Cas Server3.5.2和Cas Client3.1.11所写，原文地址：[http://haohaoxuexi.iteye.com/blog/2145751](http://haohaoxuexi.iteye.com/blog/2145751)）
