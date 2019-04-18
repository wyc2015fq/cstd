# Spring MVC 视图解析 Resolving views - z69183787的专栏 - CSDN博客
2014年12月03日 09:51:33[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4895
## 17.5 Spring MVC Resolving views
所有WEB应用程序的 MVC 框架都提供了决策视图的方式。 Spring MVC 提供了视图 resolvers，可以使你无需绑定特定的视图技术就可以呈现model到浏览器中的。  Spring 允许你使用 JSPs， Velocity templates 和 XSLT 视图。请参阅 [Chapter 18, *View technologies*](http://static.springsource.org/spring/docs/3.2.x/spring-framework-reference/html/view.html) 有关如何集成和使用多个不同视图技术的讨论。
Spring处理视图的两种重要方式是 ViewResolver 和 View。 ViewResolver 提供了视图名称和真实视图之间的映射。 View 接口处理请求的准备并将请求移交给一种视图技术。
## 17.5.1 使用 ViewResolver interface 接口解析视图
正如[Section 17.3, “Implementing Controllers”](http://static.springsource.org/spring/docs/3.2.x/spring-framework-reference/html/mvc.html#mvc-controller)讨论的， Spring  MVC 控制器中的所有handler方法必须解析为一个逻辑视图的名称，
 要么显式地 (例如, 通过返回一个 String， View， 或者是 ModelAndView) 要么隐式地 (即， 基于公约)。 Spring 中的视图由一个逻辑视图名称决定，并由一个视图解析器解析。 Spring 附带了许多视图解析器。 下面的表格展示了其中的大部分； 下面是几个例子：
Table 17.3. View resolvers
ViewResolver                                                             Description
AbstractCachingViewResolver     抽象的视图解析函数，它可以缓存视图。视图经常需要在他们可用之前做一些准备工作； 扩展此视图解析器来提供缓存功能。
XmlViewResolver        ViewResolver 的实现，它接受一个具有与Spring’s XML bean factories一样DTD文件的XML配置文件。默认的配置文件是 /WEB-INF/views.xml。
ResourceBundleViewResolver      ViewResolver 的实现，它使用ResourceBundle中定义的 bean，由 bundle 的基类名指定。通常，你需要再classpath中的properties文件中定义  bundle 。默认的文件名是 views.properties。
UrlBasedViewResolver      简单实现了 ViewResolver 接口，它使逻辑视图名称到URL的解析直接生效，  而不用显式映射定义。 如果你的逻辑名称和你的视图资源名称以直接的方式匹配，而不需要任意映射的话就非常适用。
InternalResourceViewResolver      UrlBasedViewResolver 的便利子类，支持 InternalResourceView (影响 Servlets 和 JSPs) 和 JstlView 、 TilesView等子类。可以通过setViewClass(..)指定通过该解析器产生的所有视图的视图类。详细内容请参阅  UrlBasedViewResolver 类的Javadocs。
VelocityViewResolver / FreeMarkerViewResolver       UrlBasedViewResolver 便利的子类，分别支持 VelocityView (实际上， Velocity 模板) 或 FreeMarkerView ，和他们的自定义子类。
ContentNegotiatingViewResolver      实现了 ViewResolver 接口，它解析基于请求文件名或者Accept  header的视图。请参阅[Section 17.5.4,
 “`ContentNegotiatingViewResolver`”](http://static.springsource.org/spring/docs/3.2.x/spring-framework-reference/html/mvc.html#mvc-multiple-representations) 。
作为一个例子，使用JSP作为视图技术，你可以使用 UrlBasedViewResolver。 该视图解析器将视图名转换为URL，并且将请求移交给 RequestDispatcher 来呈现视图。
<bean id="viewResolver"
      class="org.springframework.web.servlet.view.UrlBasedViewResolver">
    <property name="viewClass" value="org.springframework.web.servlet.view.JstlView"/>
    <property name="prefix" value="/WEB-INF/jsp/"/>
    <property name="suffix" value=".jsp"/>
</bean>
当返回test作为一个逻辑视图名称时， 此视图解析器将请求转发到 RequestDispatcher ，然后将请求转发到 /WEB-INF/jsp/test.jsp。
当在一个WEB应用中组合使用不同视图技术时，你可以使用 ResourceBundleViewResolver：
<bean id="viewResolver"
      class="org.springframework.web.servlet.view.ResourceBundleViewResolver">
    <property name="basename" value="views"/>
    <property name="defaultParentView" value="parentView"/>
</bean>
ResourceBundleViewResolver 根据basename来区分检查 ResourceBundle ， 并逐个解析支持的视图，它使用 [viewname].(class) 属性的值作为视图类， [viewname].url 属性的值作为视图 url。 可以在下一章涉及视图技术的章节中找到例子。如你所视，你可以标识一个父视图，properties文件中所有视图继承 “extend”它。 通过这种方式你可以指定一个默认视图类， 例如：
`AbstractCachingViewResolver` 的子类缓存他们解析的视图实例。缓存可以提升某些视图技术的性能。如果将cache属性设置为false，就可以关闭缓存。另外，如果你一定要在运行时刷新某些视图 (例如 当 Velocity template 修改时)， 你可以使用 `removeFromCache(String viewName, Locale loc)` 方法。
## 17.5.2 Chaining ViewResolvers
Spring 支持多视图解析器。因此你可以采用链式解析器，例如， 特定环境下重写特定的视图。链式视图解析器通过增加多个解析器到 application context 中来实现，如果需要可以通过制定 order 属性来指定顺序。请记住，order属性的值越高，解析器在链中的位置越靠后。
在下面的例子中，解析器链中含有两个解析器，一个 InternalResourceViewResolver， 它总是自动定位为链中最后一个解析器， 一个 XmlViewResolver 用于指定的 Excel 视图， InternalResourceViewResolver 不支持Excel 视图。
<bean id="jspViewResolver" class="org.springframework.web.servlet.view.InternalResourceViewResolver">
  <property name="viewClass" value="org.springframework.web.servlet.view.JstlView"/>
  <property name="prefix" value="/WEB-INF/jsp/"/>
  <property name="suffix" value=".jsp"/>
</bean>
<bean id="excelViewResolver" class="org.springframework.web.servlet.view.XmlViewResolver">
  <property name="order" value="1"/>
  <property name="location" value="/WEB-INF/views.xml"/>
</bean>
*<!-- in views.xml -->*
<beans>
  <bean name="report" class="org.springframework.example.ReportExcelView"/>
</beans>
如果特定的视图解析器不产生一个视图， Spring 检查 context 查找其他视图解析器。如果存在另外的视图解析器， Spring 继续进行检查直到视图被解析。 如果没有视图解析器能返回视图， Spring 抛出一个 ServletException。
视图解析器的约定指定：一个视图解析器可以返回null表明找不到该视图。并不是所有的视图解析器都这么做，但是 ，因为在某些情况下，解析器不能检测视图是否存在。例如， InternalResourceViewResolver 内部使用 RequestDispatcher ， dispatching 是确认JSP是否存在的唯一方式，但是，此操作只能执行一次。  同样适用于VelocityViewResolver 和其他一些解析器。请参阅视图解析器的 Javadoc 看是否报告不存在的视图。因此，将 InternalResourceViewResolver
 放到链中的最后位置，链中的结果不会被全部检查，因为 InternalResourceViewResolver 总会返回一个视图！
## 17.5.3 Redirecting to views
如前文所述，控制器通常返回一个逻辑视图名称，视图解析器解析到一种特定的视图技术。 对于像JSP这样的视图技术，是由Servlet或者JSP引擎处理的， 这种解析通常由 InternalResourceViewResolver 和 InternalResourceView的组合完成， 它产生一个内部 forward 或 include ，通过 Servlet API 的 RequestDispatcher.forward(..) 方法或 RequestDispatcher.include() 方法。对于其他视图技术，例如
 Velocity，XSLT等，该视图自己将内容写入 response stream。
在视图呈现之前，有时候最好向客户发出HTTP重定向。 这是可取的，例如，当以个控制器被 POST的数据调用时候， response 实际上代表了另外一个控制器 (例如，表单成功提交时)。在这种情况下， 普通的内部 forward 意味着另外的控制器还可以看到相同的 POST 数据，如果把它和其他预期数据混为一谈，可能会有问题。显示结果之前执行重定向的原因是消除用户多次提交表单信息的可能性。 这种场景下，浏览器首先发送一个初始化的 POST；它将收到一个 response 来重定向到其他URL；最后，浏览器将执行后续的
 重定向response的URL中的 GET 。因此，从浏览器的角度，当前页面并不反映POST的结果，而是GET的结果。 最终的效果是，用户无法通过执行刷新操作意外的重复提交相同数据。 。 刷新强制结果页的GET，而不是重新发送初始的POST数据。
**RedirectView**
作为一个控制器响应的结果强制重定向的一种方法，是为控制器创建和返回一个一个 Spring RedirectView 的实例。 这种情况下， DispatcherServlet 不使用普通视图解析机制。 而是因为它已具有给定视图 (重定向) ，  DispatcherServlet 只是指示视图做它的工作。
RedirectView 发出 HttpServletResponse.sendRedirect() 调用返回到客户端浏览器的结果作为一个HTTP重定向。默认情况下，所有的model属性都被理解为，重定向URL中作为URI模板变量公开。 其余的属性是基本类型或者是基本类型的 collections/arrays ，将自动被作为query parameters附加。
追加基本类型属性作为 query parameters 可能是所需的结果，如果一个 model 实例准备专门用于重定向。可是，在使用注解的控制器中， model 可能含有为了呈现目的添加的额外属性 (例如 下拉字段值)。 为了避免这种属性出现在 URL 中的可能性， 使用注解的控制器可以定义一个RedirectAttributes类型参数，使用它 指定要向 RedirectView 提供的确切属性。如果控制器方法决定要重定向，使用 RedirectAttributes 的内容。否则使用 model 的内容。
***注意：*** 来自当前请求的 URI template variables 自动变得可用，当扩展一个重定向URL不需要显式增加，既不通过Model也不通过 RedirectAttributes。 例如：
*@RequestMapping(value = "/files/{path}", method = RequestMethod.POST)*
public String upload(...) {
    // ...
    return "redirect:files/{path}";
}
如果使用 RedirectView 并且视图由控制器自己创建，推荐将你配置的重定向URL注入到控制器中，这样就不是写死到控制器，而是在上下文中和视图名称一起配置。下一节讨论了这一进程。
**The redirect: prefix **
虽然使用的 RedirectView 工作正常，如果控制器自己创建 RedirectView，控制器意识到重定向正在发生是不可避免的事实。 这真的不理想，两件事情联系太紧密了。 控制器不应该真正关心 response 被如何处理。 一般它应该只操作已被注入的视图名称。
特殊的 redirect: 前缀使您可以完成此任务。如果返回的视图名称含有 redirect:前缀； UrlBasedViewResolver (和所有子类) 将认定它是有重定向需要的特别说明。视图名称的其余部分将被视为重定向 URL。
如果控制返回的是RedirectView，实际效果相同，但是现在控制本身只是可以操作逻辑视图名称。 类似redirect:/myapp/some/resource的逻辑视图名称，将相对于当前 Servlet context 重定向，而类似  redirect:http://myhost.com/some/arbitrary/path 的名称将重定向到一个绝对URL。
**The forward: prefix **
你也可以为视图名称使用特殊的 forward: 前缀，由 UrlBasedViewResolver 和子类最终解决。这将创建一个 InternalResourceView (最终执行一个 RequestDispatcher.forward()) 包围视图名称的其余部分， 这被视为一个 URL。 因此，这个前缀与 InternalResourceViewResolver  InternalResourceView (以JSP为例)一起使用无效。 但是当你主要使用其他视图技术时候，该前缀将非常有用， 但是仍然想要强制
 某一资源的 forward 由 Servlet/JSP 引擎处理。 (注意，相反的，你也可能链住多视图解析器)
与 redirect: 前缀一样，如果视图名称含有 forward: 前缀被注入到控制器，控制器不检测 处理response时发生的任何特殊事情。
## 17.5.4 ContentNegotiatingViewResolver
ContentNegotiatingViewResolver 本身并不解析视图，而是委托给其他视图解析器， 选择类似于客户端请求这样表示形式的视图。客户端从服务器请求的表现形式有两种策略：
- 对于每个资源，使用一个不同的 URI，通常通过在 URI 中使用不同的文件扩展名。例如， URI http://www.example.com/users/fred.pdf 请求一个 PDF 形式的文件，而 http://www.example.com/users/fred.xml 请求一个 XML 形式文件。
- 客户端使用同一的 URI 来定位的资源，但是设置 Accept HTTP 请求头来列出它能理解的 media types。例如，一个 HTTP 请求 http://www.example.com/users/fred 使用 Accept header 设置为 application/pdf 则，请求一个 PDF 作为user fred的展示形式，而 http://www.example.com/users/fred 使用一个 Accept header 设置为 text/xml 请求一个XML形式文件。 这种策略被称为内容协商。
***注意：***  Accept header 有一个问题，就是它不可能设置在web浏览器的 HTML中。
例如：， in Firefox,它被固定为： Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
由于这个原因，开发基于浏览器的web应用时，为每种展示形式使用不同URI还是比较常见。
为了支持的一种资源，多种表示形式， Spring 提供了 ContentNegotiatingViewResolver 来解决视图基于文件扩展或者是HTTP请求的 Accept header 。 ContentNegotiatingViewResolver 本身不执行视图解析，而是委托给您通过Bean属性ViewResolvers指定的一系列视图解决方案。
ContentNegotiatingViewResolver 通过比较请求的媒体类型（request media type）和View关联的每个不同ViewResolver支持的媒体类型，来选择合适的View来处理请求。 列表中的第一个视图有一个兼容 Content-Type 返回展现形式到客户端。 如果不能由 ViewResolver 链提供兼容视图， 那么通过 DefaultViews 属性指定视图的列表将会被咨询（consulted）。 后一种选项适合于单视图， 可以呈现当前资源合适的展现形式，而无论逻辑视图名称。
  Accept header 可能包含通配符， 例如 text/*，在这种情况下其 Content-Type是 text/xml 的视图将兼容匹配。
为了支持基于文件扩展名的视图解析， 使用 ContentNegotiatingViewResolver 的bean 属性 mediaTypes 来指定媒体类型的文件扩展名的映射。关于用来决策请求媒体类型的算法的更多信息， 请参阅  ContentNegotiatingViewResolver的API documentation 。
下面是使用 ContentNegotiatingViewResolver配置的例子：
<bean class="org.springframework.web.servlet.view.ContentNegotiatingViewResolver">
  <property name="mediaTypes">
    <map>
      <entry key="atom" value="application/atom+xml"/>
      <entry key="html" value="text/html"/>
      <entry key="json" value="application/json"/>
    </map>
  </property>
  <property name="viewResolvers">
    <list>
      <bean class="org.springframework.web.servlet.view.BeanNameViewResolver"/>
      <bean class="org.springframework.web.servlet.view.InternalResourceViewResolver">
        <property name="prefix" value="/WEB-INF/jsp/"/>
        <property name="suffix" value=".jsp"/>
      </bean>
    </list>
  </property>
  <property name="defaultViews">
    <list>
      <bean class="org.springframework.web.servlet.view.json.MappingJackson2JsonView" />
    </list>
  </property>
</bean>
<bean id="content" class="com.springsource.samples.rest.SampleContentAtomView"/>
InternalResourceViewResolver 处理视图名称和JSP页面的转换，而 BeanNameViewResolver 返回一个基于bean name的视图。(请参阅  “[Resolving
 views with the ViewResolver interface](http://static.springsource.org/spring/docs/3.2.x/spring-framework-reference/html/mvc.html#mvc-viewresolver-resolver)” ) 这个例子中，该内容的bean是继承自 AbstractAtomFeedView 的类，它返回 Atom RSS feed。
在上面的配置中，如果请求的是.html扩展名，视图解析器搜索与媒体类型 text/html 相匹配的视图。 InternalResourceViewResolver 提供了匹配text/html的视图。如果请求的是.atom的文件扩展名，视图解决方案搜索与媒体类型 application/atom+xml 匹配的视图。 该视图由 BeanNameViewResolver 映射的 SampleContentAtomView 如果视图名称返回的是 content. 如果请求的是 .json， 将从DefaultViews列表中选定MappingJackson2JsonViewinstance
 ，无论视图名称是什么。 或者，客户端请求可以不用指定文件扩展名，而是指定 Accept header 设置首选的 media-type， 两者效果一致。
如果 `ContentNegotiatingViewResolver` 的ViewResolvers列表没有显式配置，他自动使用任何应用程序中定义的 ViewResolvers 。
相应的控制器代码为URI 以 http://localhost/content.atom 或 http://localhost/content 形式返回一个Atom RSS feed，且具有application/atom+xml的 Accept header 的示例代码如下。
*@Controller*publicclass ContentController {
    private List<SampleContent> contentList = new ArrayList<SampleContent>();
    *@RequestMapping(value="/content", method=RequestMethod.GET)*public ModelAndView getContent() {
        ModelAndView mav = new ModelAndView();
        mav.setViewName("content");
        mav.addObject("sampleContentList", contentList);
        return mav;
    }
}
