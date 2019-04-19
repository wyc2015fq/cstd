# Spring管理Filter和Servlet - 零度的博客专栏 - CSDN博客
2016年05月13日 11:08:49[零度anngle](https://me.csdn.net/zmx729618)阅读数：1695
        在使用spring容器的web应用中，业务对象间的依赖关系都可以用context.xml文件来配置，并且由spring容器来负责依赖对象  的创建。如果要在filter或者servlet中使用spring容器管理业务对象，通常需要使用WebApplicationContextUtils.getRequiredWebApplicationContext(getServletContext())来获得WebApplicationContext，然后调用WebApplicationContext.getBean(“beanName”)来获得对象的引用，这实际上是使用了依赖查找来获得对象，并且在filter或者servlet代码中硬编码了应用对象的bean名字。为了能在filter或者servlet中感知spring中bean，可采用如下步骤来实现：
        1-  将filter或者servlet作为bean定义在context.xml文件中，和要应用的bean定义放在一起；
        2-  实现一个filter代理或者servlet代理，该代理用WebApplicationContext来获得在context.xml中定义的filter或者servlet的对象，并将任务委托给context.xml中定义的filter或者servlet
       3-  在web.xml中用ContextLoaderListener来初始化spring  的context，同时在filter代理或者servlet代理的定义中用初始化参数来定义context.xml中filter或者servlet的bean名字（或者直接受用代理的名称获得相应的filter或者servlet的名称）。
       4-  在web.xml中定义filter代理或者servlet代理的mapping.
       利用这种方式就将filter或者servlet和业务对象的依赖关系用spring  来进行管理，并且不用在servlet中硬编码要引用的对象名字。具体实例如下：
**1、spring与web配置**
1.1  在applicationContext.xml中定义filter
```
<span style="font-family:Arial;font-size:12px;">       <bean id="springFilter" class="com.netqin.filter.SpringFilter">
              <property name="name">
                  <value>SpringFilter</value>
              </property>
       </bean></span>
```
            说明：com.netqin.filter.SpringFilter为实现了javax.servlet.Filter接口的filter
            实现filter代理 实际上，filter代理不需要我们自己来实现，Spring提供了两种现成的filter代理 org.springframework.security.util.FilterToBeanProxy， org.springframework.web.filter.DelegatingFilterProxy，两者只是在web.xml中的配置上略有不同，下面就让我们一起看看如何在web.xml中进行配置。</span>
1.2. 配置web.xml 
     初始化spring的context ，因为是使用spring来管理，所以在使用filter前先要初始化spring的context，一般来说配置如下：
```
<span style="font-family:Arial;font-size:12px;">    <context-param>
        <param-name>contextConfigLocation</param-name>
        <param-value>
            /WEB-INF/applicationContext.xml
        </param-value>
    </context-param>
    <listener>
        <listener-class>
            org.springframework.web.context.ContextLoaderListener
        </listener-class>
    </listener></span>
```
**  2、Filter配置：**
 2.1 FilterToBeanProxy
```
<span style="font-family:Arial;font-size:12px;">    <filter>
        <filter-name> springFilter </filter-name>
        <filter-class>org.springframework.security.util.FilterToBeanProxy
        </filter-class>
        <init-param>
            <param-name>targetBean</param-name>
            <param-value>springFilter</param-value>
        </init-param>
    </filter></span>
```
       说明：需要为FilterToBeanProxy提供上下文参数，这里我们配置的是targetBean属性，它告诉spring在context中查找的bean名称，所以当请求被过滤器拦截后FilterToBeanProxy会在applicationContext.xml中会查找id为springFilter的bean.我们也可以配置targetClass属性，意思就是查找该类型的bean.
2.2 DelegatingFilterProxy
```
<span style="font-family:Arial;font-size:12px;">    <filter>
        <filter-name>springFilter</filter-name>
        <filter-class>
            org.springframework.web.filter.DelegatingFilterProxy
        </filter-class>
    </filter></span>
```
        说明：使用DelegatingFilterProxy时不需要配置任何参数，spring会根据filter-name的名字来查找bean，所以这里spring会查找id为springFilter的bean.
 2.3  配置filter的mapping
```
<span style="font-family:Arial;font-size:12px;">    <filter-mapping>
        <filter-name>springFilter</filter-name>
        <url-pattern>/*</url-pattern>
    </filter-mapping></span>
```
OK! filter配置完成。推荐使用DelegatingFilterProxy，应为配置上更简单。
**3、Servlet 配置**
   Servlet的配置与Filter的配置十分相似 
 3.1 在applicationContext.xml中定义servlet
```
<span style="font-family:Arial;font-size:12px;">       <bean id="springServlet" class="com.netqin.servlet.SpringServlet">
              <property name="name">
                  <value>SpringServlet</value>
              </property>
       </bean></span>
```
 说明：com.netqin.servlet.SpringServlet继承自 javax.servlet.http.HttpServlet
3.2   实现servlet代理，与filter不同，spring没有为servlet提供代理实现，需要我们自己来创建，不过放心，创建一个servlet代理十分简单，一个具体的实现如下：
```
<span style="font-family:Arial;font-size:12px;">import java.io.IOException;
import javax.servlet.GenericServlet;
import javax.servlet.Servlet;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import org.springframework.web.context.WebApplicationContext;
import org.springframework.web.context.support.WebApplicationContextUtils;
public class ServletToBeanProxy extends GenericServlet {
    private String targetBean;
    private Servlet proxy;
    public void init() throws ServletException {
        <span style="background-color: rgb(255, 255, 51);">this.targetBean = getInitParameter("targetBean");
</span>        getServletBean();
        proxy.init(getServletConfig());
    }
    public void service(ServletRequest req, ServletResponse res)
            throws ServletException, IOException {
        proxy.service(req, res);
    }
    private void getServletBean() {
        WebApplicationContext wac = WebApplicationContextUtils
                .getRequiredWebApplicationContext(getServletContext());
        this.proxy = (Servlet) wac.getBean(targetBean);
    }
}</span>
```
       说明：相信看了代码就明白了，它利用targetBean属性在spring中查找相应的servlet，这很像FilterToBeanProxy的方式，所以我为其取名为ServletToBeanProxy。当然，我们也可以使用类似于DelegatingFilterProxy的方式，只需要将上述代码中标记为黄色的部分修改为this.targetBean=this.getServletName();即可，我们相应的命名为DelegatingServletProxy。
        配置web.xml和初始化spring的context 与filter中的说明一致，不再赘述。         
 3.3  ServletToBeanProxy
```
<span style="font-family:Arial;font-size:12px;">    <servlet>
        <servlet-name>springServlet</servlet-name>
        <servlet-class>
            com.netqin.servlet.proxy.ServletToBeanProxy
        </servlet-class>
        <init-param>
            <param-name>targetBean</param-name>
            <param-value>springServlet</param-value>
        </init-param>
        <load-on-startup>1</load-on-startup>
    </servlet></span>
```
3.4DelegatingServletProxy
```
<span style="font-family:Arial;font-size:12px;">    <servlet>
        <servlet-name>springServlet</servlet-name>
        <servlet-class>
            com.netqin.servlet.proxy.DelegatingServletProxy
        </servlet-class>
        <load-on-startup>1</load-on-startup>
    </servlet></span>
```
3.5  配置servlet的mapping
```
<span style="font-family:Arial;font-size:12px;">    <filter-mapping>
        <filter-name>springServlet</filter-name>
        <url-pattern>/servlet/*</url-pattern>
    </filter-mapping></span>
```
        OK! servlet的配置完成。推荐使用DelegatingServletProxy，应为配置上更简单。
