# SSH(Struts,Spring,Hibernate)和SSM(SpringMVC,Spring,MyBatis)的区别 - wjheha的博客 - CSDN博客
2017年06月17日 23:15:58[wjheha](https://me.csdn.net/wjheha)阅读数：191标签：[spring																[spring mvc																[mybatis																[ssh](https://so.csdn.net/so/search/s.do?q=ssh&t=blog)](https://so.csdn.net/so/search/s.do?q=mybatis&t=blog)](https://so.csdn.net/so/search/s.do?q=spring mvc&t=blog)](https://so.csdn.net/so/search/s.do?q=spring&t=blog)
个人分类：[Java/Java Web](https://blog.csdn.net/wjheha/article/category/6775424)
原文地址：http://blog.csdn.net/helongzhong/article/details/52848238?locationNum=6&fps=1
## **SSH 和 SSM 的定义**
本人经历过两种技术[架构](http://lib.csdn.net/base/architecture)，分别是常说的 SSH 和 SSM ，SSH 在本科的时候老师就教过。SSM 则是去公司后用的比较多。现在我想将这两大阵营的技术做一下对比。由于本人能力有限，涉及技术较多，我只从具体的应用方面做一些对比。
**SSH** 通常指的是 Struts2 做前端控制器，[spring](http://lib.csdn.net/base/javaee) 管理各层的组件，[hibernate](http://lib.csdn.net/base/javaee) 负责持久化层。
**SSM** 则指的是 SpringMVC 做前端控制器，[spring](http://lib.csdn.net/base/javaee) 管理各层的组件，MyBatis 负责持久化层。
共同之处是都使用了Spring的依赖注入DI来管理各层的组件，使用了面向切面编程AOP来实现日志管理，权限认证，事务等通用功能的切入。
不同之处是 Struts2 和 SpringMVC 做前端控制器的区别，以及 [hibernate](http://lib.csdn.net/base/javaee) 和 MyBatis 做持久化时的区别。但是，Struts2 也可以和 MyBatis
 搭配使用，SpringMVC 也可以和 Hibernate 搭配使用。本文为了简化对比，指定 Struts2 要和 Hibernate 搭配，SpringMVC 要和 MyBatis 搭配。
## **SSH 和 SSM 的实现原理区别**
|所在分层|SSH|SSM|
|----|----|----|
|页面层(View)|JSP|JSP|
|控制器层(Controller)|**Struts2**|**SpringMVC**|
|业务层(Service)|Java|Java|
|持久层(DAO)|**Hibernate**|**MyBatis**|
|数据库层(DB)|MySQL/Oracle|MySQL/Oracle|
|组件管理(Bean)|**Spring**|**Spring**|
**(1) Struts2 的原理**
![这里写图片描述](https://img-blog.csdn.net/20161012124611323)
一个请求在Struts2框架中的处理大概分为以下几个步骤：
1、客户端初始化一个指向Servlet容器（例如Tomcat）的请求
2、这个请求经过一系列的过滤器（Filter）（这些过滤器中有一个叫做ActionContextCleanUp的可选过滤器，这个过滤器对于Struts2和其他框架的集成很有帮助，例如：SiteMesh Plugin）
3、接着FilterDispatcher被调用，FilterDispatcher询问ActionMapper来决定这个请求是否需要调用某个Action
FilterDispatcher是控制器的核心，就是mvc中c控制层的核心。下面粗略的分析下FilterDispatcher工作流程和原理：FilterDispatcher进行初始化并启用核心doFilter。
<code class="hljs java has-numbering">   <span class="hljs-keyword">public</span> <span class="hljs-keyword">void</span> <span class="hljs-title">doFilter</span>(ServletRequest req, ServletResponse res, FilterChain chain) <span class="hljs-keyword">throws</span> IOException, ServletException {
        showDeprecatedWarning();
        HttpServletRequest request = (HttpServletRequest) req;
        HttpServletResponse response = (HttpServletResponse) res;
        ServletContext servletContext = getServletContext();
        String timerKey = <span class="hljs-string">"FilterDispatcher_doFilter: "</span>;
        <span class="hljs-keyword">try</span> {
            <span class="hljs-comment">// FIXME: this should be refactored better to not duplicate work with the action invocation</span>
            ValueStack stack = dispatcher.getContainer().getInstance(ValueStackFactory.class).createValueStack();
            ActionContext ctx = <span class="hljs-keyword">new</span> ActionContext(stack.getContext());
            ActionContext.setContext(ctx);
            UtilTimerStack.push(timerKey);
            request = prepareDispatcherAndWrapRequest(request, response);
            ActionMapping mapping;
            <span class="hljs-keyword">try</span> {
                <span class="hljs-comment">//在这里找到Action的映射器</span>
                mapping = actionMapper.getMapping(request, dispatcher.getConfigurationManager());
            } <span class="hljs-keyword">catch</span> (Exception ex) {
                log.error(<span class="hljs-string">"error getting ActionMapping"</span>, ex);
                dispatcher.sendError(request, response, servletContext, HttpServletResponse.SC_INTERNAL_SERVER_ERROR, ex);
                <span class="hljs-keyword">return</span>;
            }
            <span class="hljs-comment">//没有此Action的话,就去查找静态资源</span>
            <span class="hljs-keyword">if</span> (mapping == <span class="hljs-keyword">null</span>) {
                <span class="hljs-comment">// there is no action in this request, should we look for a static resource?</span>
                String resourcePath = RequestUtils.getServletPath(request);
                <span class="hljs-keyword">if</span> (<span class="hljs-string">""</span>.equals(resourcePath) && <span class="hljs-keyword">null</span> != request.getPathInfo()) {
                    resourcePath = request.getPathInfo();
                }
                <span class="hljs-keyword">if</span> (staticResourceLoader.canHandle(resourcePath)) {
                    staticResourceLoader.findStaticResource(resourcePath, request, response);
                } <span class="hljs-keyword">else</span> {
                    <span class="hljs-comment">// this is a normal request, let it pass through</span>
                    chain.doFilter(request, response);
                }
                <span class="hljs-comment">// The framework did its job here</span>
                <span class="hljs-keyword">return</span>;
            }
            <span class="hljs-comment">//有此Action的话则把控制权交给ActionProxy</span>
            dispatcher.serviceAction(request, response, servletContext, mapping);
        } <span class="hljs-keyword">finally</span> {
            dispatcher.cleanUpRequest(request);
            <span class="hljs-keyword">try</span> {
                ActionContextCleanUp.cleanUp(req);
            } <span class="hljs-keyword">finally</span> {
                UtilTimerStack.pop(timerKey);
            }
            devModeOverride.remove();
        }
    }</code><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li><li>20</li><li>21</li><li>22</li><li>23</li><li>24</li><li>25</li><li>26</li><li>27</li><li>28</li><li>29</li><li>30</li><li>31</li><li>32</li><li>33</li><li>34</li><li>35</li><li>36</li><li>37</li><li>38</li><li>39</li><li>40</li><li>41</li><li>42</li><li>43</li><li>44</li><li>45</li><li>46</li><li>47</li><li>48</li><li>49</li><li>50</li><li>51</li><li>52</li><li>53</li><li>54</li><li>55</li><li>56</li><li>57</li><li>58</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li><li>20</li><li>21</li><li>22</li><li>23</li><li>24</li><li>25</li><li>26</li><li>27</li><li>28</li><li>29</li><li>30</li><li>31</li><li>32</li><li>33</li><li>34</li><li>35</li><li>36</li><li>37</li><li>38</li><li>39</li><li>40</li><li>41</li><li>42</li><li>43</li><li>44</li><li>45</li><li>46</li><li>47</li><li>48</li><li>49</li><li>50</li><li>51</li><li>52</li><li>53</li><li>54</li><li>55</li><li>56</li><li>57</li><li>58</li></ul>
4、如果ActionMapper决定需要调用某个Action，FilterDispatcher把请求的处理交给ActionProxy
<code class="hljs lasso has-numbering">    <span class="hljs-keyword">public</span> <span class="hljs-literal">void</span> serviceAction(HttpServletRequest request, HttpServletResponse response, ServletContext context,
                              ActionMapping mapping) throws ServletException {
        <span class="hljs-built_in">Map</span><span class="hljs-subst"><</span><span class="hljs-built_in">String</span>, Object<span class="hljs-subst">></span> extraContext <span class="hljs-subst">=</span> createContextMap(request, response, mapping, context);
        <span class="hljs-comment">// If there was a previous value stack, then create a new copy and pass it in to be used by the new Action</span>
        ValueStack <span class="hljs-built_in">stack</span> <span class="hljs-subst">=</span> (ValueStack) request<span class="hljs-built_in">.</span>getAttribute(ServletActionContext<span class="hljs-built_in">.</span>STRUTS_VALUESTACK_KEY);
        boolean nullStack <span class="hljs-subst">=</span> <span class="hljs-built_in">stack</span> <span class="hljs-subst">==</span> <span class="hljs-built_in">null</span>;
        <span class="hljs-keyword">if</span> (nullStack) {
            ActionContext ctx <span class="hljs-subst">=</span> ActionContext<span class="hljs-built_in">.</span>getContext();
            <span class="hljs-keyword">if</span> (ctx <span class="hljs-subst">!=</span> <span class="hljs-built_in">null</span>) {
                <span class="hljs-built_in">stack</span> <span class="hljs-subst">=</span> ctx<span class="hljs-built_in">.</span>getValueStack();
            }
        }
        <span class="hljs-keyword">if</span> (<span class="hljs-built_in">stack</span> <span class="hljs-subst">!=</span> <span class="hljs-built_in">null</span>) {
            extraContext<span class="hljs-built_in">.</span>put(ActionContext<span class="hljs-built_in">.</span>VALUE_STACK, valueStackFactory<span class="hljs-built_in">.</span>createValueStack(<span class="hljs-built_in">stack</span>));
        }
        <span class="hljs-built_in">String</span> timerKey <span class="hljs-subst">=</span> <span class="hljs-string">"Handling request from Dispatcher"</span>;
        try {
            UtilTimerStack<span class="hljs-built_in">.</span>push(timerKey);
            <span class="hljs-built_in">String</span> namespace <span class="hljs-subst">=</span> mapping<span class="hljs-built_in">.</span>getNamespace();
            <span class="hljs-built_in">String</span> name <span class="hljs-subst">=</span> mapping<span class="hljs-built_in">.</span>getName();
            <span class="hljs-built_in">String</span> method <span class="hljs-subst">=</span> mapping<span class="hljs-built_in">.</span>getMethod();
            <span class="hljs-comment">//获取配置文件</span>
            Configuration config <span class="hljs-subst">=</span> configurationManager<span class="hljs-built_in">.</span>getConfiguration();
            <span class="hljs-comment">//根据配置文件找到此Action并生成ActionProxy</span>
            ActionProxy proxy <span class="hljs-subst">=</span> config<span class="hljs-built_in">.</span>getContainer()<span class="hljs-built_in">.</span>getInstance(ActionProxyFactory<span class="hljs-built_in">.</span>class)<span class="hljs-built_in">.</span>createActionProxy(
                    namespace, name, method, extraContext, <span class="hljs-literal">true</span>, <span class="hljs-literal">false</span>);
            request<span class="hljs-built_in">.</span>setAttribute(ServletActionContext<span class="hljs-built_in">.</span>STRUTS_VALUESTACK_KEY, proxy<span class="hljs-built_in">.</span>getInvocation()<span class="hljs-built_in">.</span>getStack());
            <span class="hljs-comment">// if the ActionMapping says to go straight to a result, do it!</span>
            <span class="hljs-keyword">if</span> (mapping<span class="hljs-built_in">.</span>getResult() <span class="hljs-subst">!=</span> <span class="hljs-built_in">null</span>) {
                Result result <span class="hljs-subst">=</span> mapping<span class="hljs-built_in">.</span>getResult();
                <span class="hljs-comment">//ActionProxy创建一个ActionInvocation的实例</span>
                result<span class="hljs-built_in">.</span>execute(proxy<span class="hljs-built_in">.</span>getInvocation());
            } <span class="hljs-keyword">else</span> {
                proxy<span class="hljs-built_in">.</span>execute();
            }
            <span class="hljs-comment">// If there was a previous value stack then set it back onto the request</span>
            <span class="hljs-keyword">if</span> (<span class="hljs-subst">!</span>nullStack) {
                request<span class="hljs-built_in">.</span>setAttribute(ServletActionContext<span class="hljs-built_in">.</span>STRUTS_VALUESTACK_KEY, <span class="hljs-built_in">stack</span>);
            }
        } catch (ConfigurationException e) {
            logConfigurationException(request, e);
            sendError(request, response, context, HttpServletResponse<span class="hljs-built_in">.</span>SC_NOT_FOUND, e);
        } catch (Exception e) {
            <span class="hljs-keyword">if</span> (handleException <span class="hljs-subst">||</span> devMode) {
                sendError(request, response, context, HttpServletResponse<span class="hljs-built_in">.</span>SC_INTERNAL_SERVER_ERROR, e);
            } <span class="hljs-keyword">else</span> {
                throw <span class="hljs-literal">new</span> ServletException(e);
            }
        } finally {
            UtilTimerStack<span class="hljs-built_in">.</span>pop(timerKey);
        }
    }</code><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li><li>20</li><li>21</li><li>22</li><li>23</li><li>24</li><li>25</li><li>26</li><li>27</li><li>28</li><li>29</li><li>30</li><li>31</li><li>32</li><li>33</li><li>34</li><li>35</li><li>36</li><li>37</li><li>38</li><li>39</li><li>40</li><li>41</li><li>42</li><li>43</li><li>44</li><li>45</li><li>46</li><li>47</li><li>48</li><li>49</li><li>50</li><li>51</li><li>52</li><li>53</li><li>54</li><li>55</li><li>56</li><li>57</li><li>58</li><li>59</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li><li>20</li><li>21</li><li>22</li><li>23</li><li>24</li><li>25</li><li>26</li><li>27</li><li>28</li><li>29</li><li>30</li><li>31</li><li>32</li><li>33</li><li>34</li><li>35</li><li>36</li><li>37</li><li>38</li><li>39</li><li>40</li><li>41</li><li>42</li><li>43</li><li>44</li><li>45</li><li>46</li><li>47</li><li>48</li><li>49</li><li>50</li><li>51</li><li>52</li><li>53</li><li>54</li><li>55</li><li>56</li><li>57</li><li>58</li><li>59</li></ul>
5、ActionProxy通过Configuration Manager询问框架的配置文件，找到需要调用的Action类
6、ActionProxy创建一个ActionInvocation的实例。
7、ActionInvocation实例使用命名模式来调用，在调用Action的过程前后，涉及到相关拦截器（Intercepter）的调用。
8、一旦Action执行完毕，ActionInvocation负责根据struts.xml中的配置找到对应的返回结果。返回结果通常是（但不总是，也可 能是另外的一个Action链）一个需要被表示的JSP或者FreeMarker的模版。
9、将处理结果返回给客户端
**(2) SpringMVC 的原理**
![这里写图片描述](https://img-blog.csdn.net/20161011155831879)
**执行步骤:**
**第一步：发起请求到前端控制器(DispatcherServlet)**
**第二步：前端控制器请求HandlerMapping查找 Handler**
可以根据xml配置、注解进行查找
**第三步：处理器映射器HandlerMapping向前端控制器返回Handler**
**第四步：前端控制器调用处理器适配器去执行Handler**
**第五步：处理器适配器去执行Handler**
**第六步：Handler执行完成给适配器返回ModelAndView**
**第七步：处理器适配器向前端控制器返回ModelAndView**
ModelAndView是SpringMVC框架的一个底层对象，包括 Model和view
**第八步：前端控制器请求视图解析器去进行视图解析**
根据逻辑视图名解析成真正的视图(jsp)
**第九步：视图解析器向前端控制器返回View**
**第十步：前端控制器进行视图渲染**
视图渲染将模型数据(在ModelAndView对象中)填充到request域
**第十一步：前端控制器向用户响应结果**
**(3) Hibernate 的原理**
1.通过Configuration().configure();读取并解析hibernate.cfg.xml配置文件
2.由hibernate.cfg.xml中的`<mapping resource="com/xx/User.hbm.xml"/>`读取并解析映射信息
3.通过config.buildSessionFactory();//创建SessionFactory
4.sessionFactory.openSession();//打开Sesssion
5.session.beginTransaction();//创建事务Transation
6.persistent operate持久化操作
7.session.getTransaction().commit();//提交事务
8.关闭Session
9.关闭SesstionFactory
**(4) MyBatis原理**
![这里写图片描述](https://img-blog.csdn.net/20161007161731534)
MyBatis框架执行过程：
1、配置MyBatis的配置文件，SqlMapConfig.xml（名称不固定）
2、通过配置文件，加载MyBatis运行环境，创建SqlSessionFactory会话工厂 
SqlSessionFactory 在实际使用时按单例方式。
3、通过SqlSessionFactory创建SqlSession 
SqlSession 是一个面向用户接口（提供操作[数据库](http://lib.csdn.net/base/mysql)方法），实现对象是线程不安全的，建议sqlSession应用场合在方法体内。
4、调用 sqlSession 的方法去操作数据。 
如果需要提交事务，需要执行 SqlSession 的 commit() 方法。
5、释放资源，关闭SqlSession
## **Struts2 和 SpringMVC 在 web.xml 中配置的不同**
(1) Struts2
<code class="hljs xml has-numbering"><span class="hljs-tag"><<span class="hljs-title">filter</span>></span>  
    <span class="hljs-tag"><<span class="hljs-title">filter-name</span>></span>struts2<span class="hljs-tag"></<span class="hljs-title">filter-name</span>></span>  
    <span class="hljs-tag"><<span class="hljs-title">filter-class</span>></span>org.apache.struts2.dispatcher.ng.filter.StrutsPrepareAndExecuteFilter<span class="hljs-tag"></<span class="hljs-title">filter-class</span>></span>  
    <span class="hljs-tag"><<span class="hljs-title">init-param</span>></span>  
        <span class="hljs-tag"><<span class="hljs-title">param-name</span>></span>filterConfig<span class="hljs-tag"></<span class="hljs-title">param-name</span>></span>  
        <span class="hljs-tag"><<span class="hljs-title">param-value</span>></span>classpath:struts2/struts.xml<span class="hljs-tag"></<span class="hljs-title">param-value</span>></span>  
    <span class="hljs-tag"></<span class="hljs-title">init-param</span>></span>  
<span class="hljs-tag"></<span class="hljs-title">filter</span>></span>  
<span class="hljs-tag"><<span class="hljs-title">filter-mapping</span>></span>  
    <span class="hljs-tag"><<span class="hljs-title">filter-name</span>></span>struts2<span class="hljs-tag"></<span class="hljs-title">filter-name</span>></span>  
    <span class="hljs-tag"><<span class="hljs-title">url-pattern</span>></span>/*<span class="hljs-tag"></<span class="hljs-title">url-pattern</span>></span>  
<span class="hljs-tag"></<span class="hljs-title">filter-mapping</span>></span>  </code><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li></ul>
Struts2使用Filter嵌入自己的框架。配置文件加载顺序为：default.properties -> struts-default.xml -> struts-plugins.xml -> struts.xml -> struts.locale。
加载顺序可以参考这篇文章的源码分析了解更多。[https://my.oschina.net/gschen/blog/121433](https://my.oschina.net/gschen/blog/121433)
(2) SpringMVC
<code class="hljs xml has-numbering">    <span class="hljs-comment"><!-- springmvc前端控制器，rest配置 --></span>
    <span class="hljs-tag"><<span class="hljs-title">servlet</span>></span>
        <span class="hljs-tag"><<span class="hljs-title">servlet-name</span>></span>springmvc_rest<span class="hljs-tag"></<span class="hljs-title">servlet-name</span>></span>
        <span class="hljs-tag"><<span class="hljs-title">servlet-class</span>></span>org.springframework.web.servlet.DispatcherServlet<span class="hljs-tag"></<span class="hljs-title">servlet-class</span>></span>
        <span class="hljs-comment"><!-- contextConfigLocation配置springmvc加载的配置文件（配置处理器映射器、适配器等等） 如果不配置contextConfigLocation，默认加载的是/WEB-INF/servlet名称-serlvet.xml（springmvc-servlet.xml） --></span>
        <span class="hljs-tag"><<span class="hljs-title">init-param</span>></span>
            <span class="hljs-tag"><<span class="hljs-title">param-name</span>></span>contextConfigLocation<span class="hljs-tag"></<span class="hljs-title">param-name</span>></span>
            <span class="hljs-tag"><<span class="hljs-title">param-value</span>></span>classpath:spring/springmvc.xml<span class="hljs-tag"></<span class="hljs-title">param-value</span>></span>
        <span class="hljs-tag"></<span class="hljs-title">init-param</span>></span>
    <span class="hljs-tag"></<span class="hljs-title">servlet</span>></span>
    <span class="hljs-tag"><<span class="hljs-title">servlet-mapping</span>></span>
        <span class="hljs-tag"><<span class="hljs-title">servlet-name</span>></span>springmvc_rest<span class="hljs-tag"></<span class="hljs-title">servlet-name</span>></span>
        <span class="hljs-tag"><<span class="hljs-title">url-pattern</span>></span>/<span class="hljs-tag"></<span class="hljs-title">url-pattern</span>></span>
    <span class="hljs-tag"></<span class="hljs-title">servlet-mapping</span>></span></code><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li></ul>
SpringMVC使用Servlet嵌入自己的框架。
（3）web.xml不同之处
**SpringMVC的入口是Servlet**，而**Struts2是Filter**（这里要指出，Filter和Servlet是不同的。以前认为filter是servlet的一种特殊），这就导致了二者的机制不同，这里就牵涉到Servlet和Filter的区别了。但是这只是接管用户请求的两种不同方式而已，控制权被Struts2和SpringMVC掌握之后，想做什么事都是可以做到的。
**Servlet**
servlet是一种运行服务器端的[Java](http://lib.csdn.net/base/javaee)应用程序，具有独立于平台和协议的特性，并且可以动态的生成web页面，它工作在客户端请求与服务器响应的中间层。最早支持 Servlet 技术的是
 JavaSoft 的 [Java](http://lib.csdn.net/base/java)Web Server。此后，一些其它的基于 [Java](http://lib.csdn.net/base/java) 的
 Web Server 开始支持标准的 Servlet API。Servlet 的主要功能在于交互式地浏览和修改数据，生成动态 Web 内容。这个过程为：
```
1) 客户端发送请求至服务器端；
2) 服务器将请求信息发送至 Servlet；
3) Servlet 生成响应内容并将其传给服务器。响应内容动态生成，通常取决于客户端的请求；
4) 服务器将响应返回给客户端。
在 Web 应用程序中，一个 Servlet 在一个时刻可能被多个用户同时访问。这时 Web 容器将为每个用户创建一个线程来执行 Servlet。如果 Servlet 不涉及共享资源的问题，不必关心多线程问题。但如果 Servlet 需要共享资源，需要保证 Servlet 是线程安全的。
为了简化开发流程，Servlet 3.0 引入了注解（annotation），这使得 web 部署描述符 web.xml 不再是必须的选择。
```
**Filter**：Filter是一个可以复用的代码片段，可以用来转换HTTP请求、响应和头信息。Filter不像Servlet，它不能产生一个请求或者响应，它只是修改对某一资源的请求，或者修改从某一的响应。Servlet中的过滤器Filter是实现了javax.servlet.Filter接口的服务器端程序，主要的用途是过滤字符编码、做一些业务逻辑判断等。其工作原理是，只要你在web.xml文件配置好要拦截的客户端请求，它都会帮你拦截到请求，此时你就可以对请求或响应(Request、Response)统一设置编码，简化操作；同时还可进行逻辑判断，如用户是否已经登陆、有没有权限访问该页面等等工作。它是随你的web应用启动而启动的，只初始化一次，以后就可以拦截相关请求，只有当你的web应用停止或重新部署的时候才销毁。Filter可认为是Servlet的一种“变种”，它主要用于对用户请求进行预处理，也可以对HttpServletResponse进行后处理，是个典型的处理链。它与Servlet的区别在于：它不能直接向用户生成响应。完整的流程是：Filter对用户请求进行预处理，接着将请求交给Servlet进行处理并生成响应，最后Filter再对服务器响应进行后处理。
Servlet与Filter的区别可以从这篇文章了解更多。[http://www.cnblogs.com/doit8791/p/4209442.html](http://www.cnblogs.com/doit8791/p/4209442.html)
## **Struts2 和 SpringMVC 处理用户请求的不同**
Struts2和SpringMVC的核心都是接管用户的请求，解决传统Servlet开发过于繁琐，重用性不高的问题。
Struts2和SpringMVC都有注解和配置文件两种匹配用户请求URL的方式。
**Struts2注解方式匹配URL**
参考网址：[http://struts.apache.org/docs/convention-plugin.html](http://struts.apache.org/docs/convention-plugin.html)
首先需要将架包(struts2-convention-plugin-xxx.jar)导入工程中
示例
<code class="hljs java has-numbering"><span class="hljs-keyword">package</span> com.example.actions;  
<span class="hljs-keyword">import</span> com.opensymphony.xwork2.ActionSupport;   
<span class="hljs-keyword">import</span> org.apache.struts2.convention.annotation.Action;  
<span class="hljs-keyword">import</span> org.apache.struts2.convention.annotation.Actions;  
<span class="hljs-keyword">import</span> org.apache.struts2.convention.annotation.Result;  
<span class="hljs-keyword">import</span> org.apache.struts2.convention.annotation.Results;  
<span class="hljs-annotation">@Results</span>({  
  <span class="hljs-annotation">@Result</span>(name=<span class="hljs-string">"failure"</span>, location=<span class="hljs-string">"fail.jsp"</span>)  
})  
<span class="hljs-keyword">public</span> <span class="hljs-class"><span class="hljs-keyword">class</span> <span class="hljs-title">HelloWorld</span> <span class="hljs-keyword">extends</span> <span class="hljs-title">ActionSupport</span> 
{</span>  
  <span class="hljs-annotation">@Action</span>(value=<span class="hljs-string">"/different/url"</span>,   
    results={<span class="hljs-annotation">@Result</span>(name=<span class="hljs-string">"success"</span>, location=<span class="hljs-string">"http://struts.apache.org"</span>, type=<span class="hljs-string">"redirect"</span>)}  
  )  
  <span class="hljs-keyword">public</span> String <span class="hljs-title">execute</span>() 
  {  
    <span class="hljs-keyword">return</span> SUCCESS;  
  }  
  <span class="hljs-annotation">@Action</span>(<span class="hljs-string">"/another/url"</span>)  
  <span class="hljs-keyword">public</span> String <span class="hljs-title">doSomething</span>()
  {  
    <span class="hljs-keyword">return</span> SUCCESS;  
  }  
}  </code><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li><li>20</li><li>21</li><li>22</li><li>23</li><li>24</li><li>25</li><li>26</li><li>27</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li><li>20</li><li>21</li><li>22</li><li>23</li><li>24</li><li>25</li><li>26</li><li>27</li></ul>
**Struts2配置方式匹配URL**
<code class="hljs xml has-numbering">    <span class="hljs-tag"><<span class="hljs-title">package</span> <span class="hljs-attribute">name</span>=<span class="hljs-value">"package"</span> <span class="hljs-attribute">namespace</span>=<span class="hljs-value">"/different"</span> <span class="hljs-attribute">extends</span>=<span class="hljs-value">"struts-default"</span>></span>
        <span class="hljs-tag"><<span class="hljs-title">global-results</span>></span>
            <span class="hljs-tag"><<span class="hljs-title">result</span> <span class="hljs-attribute">name</span>=<span class="hljs-value">"failure"</span>></span>/fail.jsp<span class="hljs-tag"></<span class="hljs-title">result</span>></span>
        <span class="hljs-tag"></<span class="hljs-title">global-results</span>></span>
        <span class="hljs-tag"><<span class="hljs-title">action</span> <span class="hljs-attribute">name</span>=<span class="hljs-value">"url"</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"com.example.actions.HelloWorld"</span> <span class="hljs-attribute">method</span>=<span class="hljs-value">"execute"</span>></span>
            <span class="hljs-tag"><<span class="hljs-title">result</span> <span class="hljs-attribute">name</span>=<span class="hljs-value">"success"</span> <span class="hljs-attribute">type</span>=<span class="hljs-value">"redirect"</span>></span>http://struts.apache.org<span class="hljs-tag"></<span class="hljs-title">result</span>></span>
        <span class="hljs-tag"></<span class="hljs-title">action</span>></span>
    <span class="hljs-tag"></<span class="hljs-title">package</span>></span>
    <span class="hljs-tag"><<span class="hljs-title">package</span> <span class="hljs-attribute">name</span>=<span class="hljs-value">"package2"</span> <span class="hljs-attribute">namespace</span>=<span class="hljs-value">"/another"</span> <span class="hljs-attribute">extends</span>=<span class="hljs-value">"struts-default"</span>></span>
        <span class="hljs-tag"><<span class="hljs-title">global-results</span>></span>
            <span class="hljs-tag"><<span class="hljs-title">result</span> <span class="hljs-attribute">name</span>=<span class="hljs-value">"failure"</span>></span>/fail.jsp<span class="hljs-tag"></<span class="hljs-title">result</span>></span>
        <span class="hljs-tag"></<span class="hljs-title">global-results</span>></span>
        <span class="hljs-tag"><<span class="hljs-title">action</span> <span class="hljs-attribute">name</span>=<span class="hljs-value">"url"</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"com.example.actions.HelloWorld"</span> <span class="hljs-attribute">method</span>=<span class="hljs-value">"doSomething"</span>></span>
        <span class="hljs-tag"></<span class="hljs-title">action</span>></span>
    <span class="hljs-tag"></<span class="hljs-title">package</span>></span></code><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li></ul>
**SpringMVC注解方式匹配URL**
<code class="hljs avrasm has-numbering">package <span class="hljs-keyword">com</span><span class="hljs-preprocessor">.jpkc</span><span class="hljs-preprocessor">.controller</span><span class="hljs-comment">;</span>
import javax<span class="hljs-preprocessor">.servlet</span><span class="hljs-preprocessor">.http</span><span class="hljs-preprocessor">.HttpServletRequest</span><span class="hljs-comment">;</span>
import javax<span class="hljs-preprocessor">.servlet</span><span class="hljs-preprocessor">.http</span><span class="hljs-preprocessor">.HttpServletResponse</span><span class="hljs-comment">;</span>
import javax<span class="hljs-preprocessor">.servlet</span><span class="hljs-preprocessor">.http</span><span class="hljs-preprocessor">.HttpSession</span><span class="hljs-comment">;</span>
import org<span class="hljs-preprocessor">.springframework</span><span class="hljs-preprocessor">.beans</span><span class="hljs-preprocessor">.factory</span><span class="hljs-preprocessor">.annotation</span><span class="hljs-preprocessor">.Autowired</span><span class="hljs-comment">;</span>
import org<span class="hljs-preprocessor">.springframework</span><span class="hljs-preprocessor">.stereotype</span><span class="hljs-preprocessor">.Controller</span><span class="hljs-comment">;</span>
import org<span class="hljs-preprocessor">.springframework</span><span class="hljs-preprocessor">.web</span><span class="hljs-preprocessor">.bind</span><span class="hljs-preprocessor">.annotation</span><span class="hljs-preprocessor">.RequestMapping</span><span class="hljs-comment">;</span>
import org<span class="hljs-preprocessor">.springframework</span><span class="hljs-preprocessor">.web</span><span class="hljs-preprocessor">.bind</span><span class="hljs-preprocessor">.annotation</span><span class="hljs-preprocessor">.RequestMethod</span><span class="hljs-comment">;</span>
import org<span class="hljs-preprocessor">.springframework</span><span class="hljs-preprocessor">.web</span><span class="hljs-preprocessor">.servlet</span><span class="hljs-preprocessor">.ModelAndView</span><span class="hljs-comment">;</span>
@RequestMapping(<span class="hljs-string">"/admin"</span>)
@Controller
public class LoginController
{   
    @RequestMapping(<span class="hljs-string">"/admin_home"</span>)
    public String admin_home() throws Exception
    {
        return <span class="hljs-string">"forward:/shop/index.jsp"</span><span class="hljs-comment">;</span>
    }
    @RequestMapping(<span class="hljs-string">"/exit"</span>)
    public String logout(ModelAndView model, HttpSession session) throws Exception
    {
        session<span class="hljs-preprocessor">.invalidate</span>()<span class="hljs-comment">;</span>
        return <span class="hljs-string">"redirect:/manager/login.jsp"</span><span class="hljs-comment">;</span>
    }
}
</code><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li><li>20</li><li>21</li><li>22</li><li>23</li><li>24</li><li>25</li><li>26</li><li>27</li><li>28</li><li>29</li><li>30</li><li>31</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li><li>20</li><li>21</li><li>22</li><li>23</li><li>24</li><li>25</li><li>26</li><li>27</li><li>28</li><li>29</li><li>30</li><li>31</li></ul>
**SpringMVC配置方式匹配URL**
<code class="hljs java has-numbering"><span class="hljs-keyword">public</span> <span class="hljs-class"><span class="hljs-keyword">class</span> <span class="hljs-title">ItemsController1</span> <span class="hljs-keyword">implements</span> <span class="hljs-title">Controller</span> 
{</span>
    <span class="hljs-annotation">@Override</span>
    <span class="hljs-keyword">public</span> ModelAndView <span class="hljs-title">handleRequest</span>(HttpServletRequest request,
            HttpServletResponse response) <span class="hljs-keyword">throws</span> Exception 
    {
        <span class="hljs-comment">//调用Service查找 数据库，查询商品列表，这里使用静态数据模拟</span>
        List<Items> itemsList = <span class="hljs-keyword">new</span> ArrayList<Items>();
        <span class="hljs-comment">//向list中填充静态数据</span>
        Items items_1 = <span class="hljs-keyword">new</span> Items();
        items_1.setName(<span class="hljs-string">"联想笔记本"</span>);
        items_1.setPrice(<span class="hljs-number">6000</span>f);
        items_1.setDetail(<span class="hljs-string">"ThinkPad T430 联想笔记本电脑！"</span>);
        Items items_2 = <span class="hljs-keyword">new</span> Items();
        items_2.setName(<span class="hljs-string">"苹果手机"</span>);
        items_2.setPrice(<span class="hljs-number">5000</span>f);
        items_2.setDetail(<span class="hljs-string">"iphone6苹果手机！"</span>);
        itemsList.add(items_1);
        itemsList.add(items_2);
        <span class="hljs-comment">//返回ModelAndView</span>
        ModelAndView modelAndView =  <span class="hljs-keyword">new</span> ModelAndView();
        <span class="hljs-comment">//相当 于request的setAttribut，在jsp页面中通过itemsList取数据</span>
        modelAndView.addObject(<span class="hljs-string">"itemsList"</span>, itemsList);
        <span class="hljs-comment">//指定视图</span>
        modelAndView.setViewName(<span class="hljs-string">"/WEB-INF/jsp/items/itemsList.jsp"</span>);
        <span class="hljs-keyword">return</span> modelAndView;
    }
}</code><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li><li>20</li><li>21</li><li>22</li><li>23</li><li>24</li><li>25</li><li>26</li><li>27</li><li>28</li><li>29</li><li>30</li><li>31</li><li>32</li><li>33</li><li>34</li><li>35</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li><li>20</li><li>21</li><li>22</li><li>23</li><li>24</li><li>25</li><li>26</li><li>27</li><li>28</li><li>29</li><li>30</li><li>31</li><li>32</li><li>33</li><li>34</li><li>35</li></ul><code class="hljs xml has-numbering"><span class="hljs-comment"><!-- 配置Handler --></span>
<span class="hljs-tag"><<span class="hljs-title">bean</span> <span class="hljs-attribute">id</span>=<span class="hljs-value">"itemsController1"</span> <span class="hljs-attribute">name</span>=<span class="hljs-value">"/queryItems.action"</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"cn.itcast.ssm.controller.ItemsController1"</span> /></span></code><ul style="" class="pre-numbering"><li>1</li><li>2</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li></ul>
1、Struts2是类级别的拦截， 一个类对应一个request上下文，SpringMVC是方法级别的拦截，一个方法对应一个request上下文，而方法同时又跟一个url对应,所以说从[架构](http://lib.csdn.net/base/architecture)本身上SpringMVC就容易实现restful
 url,而struts2的架构实现起来要费劲，因为Struts2中Action的一个方法可以对应一个url，而其类属性却被所有方法共享，这也就无法用注解或其他方式标识其所属方法了。
2、由上边原因，SpringMVC的方法之间基本上独立的，独享request response数据，请求数据通过参数获取，处理结果通过ModelMap交回给框架，方法之间不共享变量，而Struts2搞的就比较乱，虽然方法之间也是独立的，但其所有Action变量是共享的，这不会影响程序运行，却给我们编码 读程序时带来麻烦，每次来了请求就创建一个Action，一个Action对象对应一个request上下文。
3、由于Struts2需要针对每个request进行封装，把request，session等servlet生命周期的变量封装成一个一个Map，供给每个Action使用，并保证线程安全，所以在原则上，是比较耗费内存的。
## **Struts2 和 SpringMVC 实现 RESTful 的不同**
[http://localhost/jpkc/item/1609032329404095427579225](http://localhost/jpkc/item/1609032329404095427579225)
实现上面这个链接，其中localhost是域名，jpkc是项目名。
**Struts2实现方式**
<code class="hljs xml has-numbering">    <span class="hljs-tag"><<span class="hljs-title">package</span> <span class="hljs-attribute">name</span>=<span class="hljs-value">"course_info_package"</span> <span class="hljs-attribute">namespace</span>=<span class="hljs-value">"/item"</span> <span class="hljs-attribute">extends</span>=<span class="hljs-value">"struts-default"</span>></span>
        <span class="hljs-tag"><<span class="hljs-title">action</span> <span class="hljs-attribute">name</span>=<span class="hljs-value">"*"</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"com.jpkc.action.CourseAction"</span> <span class="hljs-attribute">method</span>=<span class="hljs-value">"get_course_info"</span>></span>
            <span class="hljs-tag"><<span class="hljs-title">result</span> <span class="hljs-attribute">name</span>=<span class="hljs-value">"success"</span>></span>/story/story_02.jsp<span class="hljs-tag"></<span class="hljs-title">result</span>></span>
        <span class="hljs-tag"></<span class="hljs-title">action</span>></span>
    <span class="hljs-tag"></<span class="hljs-title">package</span>></span></code><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li></ul><code class="hljs java has-numbering"><span class="hljs-keyword">public</span> <span class="hljs-class"><span class="hljs-keyword">class</span> <span class="hljs-title">CourseAction</span> <span class="hljs-keyword">extends</span> <span class="hljs-title">ActionSupport</span>
{</span>
    <span class="hljs-keyword">public</span> String <span class="hljs-title">get_course_info</span>()
    {
        String actionName = ServletActionContext.getActionMapping().getName();
        CourseInfo courseInfoFromDB = courseInfoDAO.findById(actionName);
        <span class="hljs-keyword">if</span> (courseInfoFromDB == <span class="hljs-keyword">null</span>)
        {
            <span class="hljs-keyword">return</span> <span class="hljs-string">"404"</span>;
        }
        Course courseFromDB = courseDAO.findById(actionName);
        <span class="hljs-keyword">if</span> (courseFromDB == <span class="hljs-keyword">null</span>)
        {
            <span class="hljs-keyword">return</span> <span class="hljs-string">"404"</span>;
        }
        setCourseInfo(courseInfoFromDB);
        setCourse(courseFromDB);
        <span class="hljs-keyword">return</span> SUCCESS;
    }
}</code><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li><li>20</li><li>21</li><li>22</li><li>23</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li><li>20</li><li>21</li><li>22</li><li>23</li></ul>
**SpringMVC实现方式**
<code class="hljs cs has-numbering">@Controller
<span class="hljs-keyword">public</span> <span class="hljs-keyword">class</span> CourseController
{
    @RequestMapping(<span class="hljs-string">"/item/{id}"</span>)
    <span class="hljs-keyword">public</span> ModelAndView <span class="hljs-title">get_course_info</span>(ModelAndView model, @<span class="hljs-title">PathVariable</span>("id") String id)
    {
        <span class="hljs-keyword">if</span> (CM.validIsEmptyWithTrim(id))
        {
            model.addObject(<span class="hljs-string">"message"</span>, <span class="hljs-string">"没有找到此视频页面"</span>);
            model.setViewName(<span class="hljs-string">"/WEB-INF/jsp/error"</span>);
            <span class="hljs-keyword">return</span> model;
        }
        CourseInfo courseInfoFromDB=<span class="hljs-keyword">null</span>;
        <span class="hljs-keyword">try</span>
        {
            courseInfoFromDB = courseInfoService.selectByPrimaryKey(id);
        }
        <span class="hljs-keyword">catch</span> (Exception e1)
        {
            System.<span class="hljs-keyword">out</span>.println(<span class="hljs-string">"没有找到课程信息"</span>);
        }
        <span class="hljs-keyword">if</span> (courseInfoFromDB == <span class="hljs-keyword">null</span>)
        {
            model.addObject(<span class="hljs-string">"message"</span>, <span class="hljs-string">"没有找到此视频页面"</span>);
            model.setViewName(<span class="hljs-string">"/WEB-INF/jsp/error"</span>);
            <span class="hljs-keyword">return</span> model;
        }
        Course courseFromDB = <span class="hljs-keyword">null</span>;
        <span class="hljs-keyword">try</span>
        {
            courseFromDB = courseService.selectByPrimaryKey(id);
        }
        <span class="hljs-keyword">catch</span> (Exception e)
        {
            System.<span class="hljs-keyword">out</span>.println(<span class="hljs-string">"没有查找到课程"</span>);
        }
        <span class="hljs-keyword">if</span> (courseFromDB == <span class="hljs-keyword">null</span>)
        {
            model.addObject(<span class="hljs-string">"message"</span>, <span class="hljs-string">"没有找到此视频页面"</span>);
            model.setViewName(<span class="hljs-string">"/WEB-INF/jsp/error"</span>);
            <span class="hljs-keyword">return</span> model;
        }
        model.addObject(<span class="hljs-string">"courseInfo"</span>, courseInfoFromDB);
        model.addObject(<span class="hljs-string">"course"</span>, courseFromDB);
        model.setViewName(<span class="hljs-string">"/story/story_02"</span>);
        <span class="hljs-keyword">return</span> model;
    }
}
</code><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li><li>20</li><li>21</li><li>22</li><li>23</li><li>24</li><li>25</li><li>26</li><li>27</li><li>28</li><li>29</li><li>30</li><li>31</li><li>32</li><li>33</li><li>34</li><li>35</li><li>36</li><li>37</li><li>38</li><li>39</li><li>40</li><li>41</li><li>42</li><li>43</li><li>44</li><li>45</li><li>46</li><li>47</li><li>48</li><li>49</li><li>50</li><li>51</li><li>52</li><li>53</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li><li>20</li><li>21</li><li>22</li><li>23</li><li>24</li><li>25</li><li>26</li><li>27</li><li>28</li><li>29</li><li>30</li><li>31</li><li>32</li><li>33</li><li>34</li><li>35</li><li>36</li><li>37</li><li>38</li><li>39</li><li>40</li><li>41</li><li>42</li><li>43</li><li>44</li><li>45</li><li>46</li><li>47</li><li>48</li><li>49</li><li>50</li><li>51</li><li>52</li><li>53</li></ul>
对于类似于[http://localhost/jpkc/item/id1](http://localhost/jpkc/item/id1)这种链接，Struts2实现RESTful风格需要在代码中调用ServletActionContext.getActionMapping().getName()获取ActionName。SpringMVC直接将链接映射到方法参数里去了。
如果类似于[http://localhost/jpkc/id2/id1](http://localhost/jpkc/id2/id1)这种链接，Struts2要进一步分析链接得到id1和id2。SpringMVC依然可以将id2映射到方法参数上。从调用的角度来看SpringMVC要方便一些。但是如果将Struts2获取方式封装一下，也可以得到同样的效果。
## **Struts2 和 SpringMVC 获取 request 参数的不同**
前台页面有一个表单需要提交。
**Struts2 接收 request 参数**
<code class="hljs xml has-numbering">        <span class="hljs-tag"><<span class="hljs-title">form</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"login-form"</span> <span class="hljs-attribute">action</span>=<span class="hljs-value">"/login_do"</span> <span class="hljs-attribute">method</span>=<span class="hljs-value">"post"</span>></span>
            <span class="hljs-tag"><<span class="hljs-title">h3</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"form-title"</span>></span>登录系统<span class="hljs-tag"></<span class="hljs-title">h3</span>></span>
            <span class="hljs-tag"><<span class="hljs-title">div</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"alert alert-danger display-hide"</span>></span>
                <span class="hljs-tag"><<span class="hljs-title">button</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"close"</span> <span class="hljs-attribute">data-close</span>=<span class="hljs-value">"alert"</span>></span><span class="hljs-tag"></<span class="hljs-title">button</span>></span>
                <span class="hljs-tag"><<span class="hljs-title">span</span>></span> 请输入用户名和密码 <span class="hljs-tag"></<span class="hljs-title">span</span>></span>
            <span class="hljs-tag"></<span class="hljs-title">div</span>></span>
            <span class="hljs-tag"><<span class="hljs-title">div</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"form-group"</span>></span>
                <span class="hljs-tag"><<span class="hljs-title">label</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"control-label visible-ie8 visible-ie9"</span>></span>用户名<span class="hljs-tag"></<span class="hljs-title">label</span>></span>
                <span class="hljs-tag"><<span class="hljs-title">div</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"input-icon"</span>></span>
                    <span class="hljs-tag"><<span class="hljs-title">i</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"fa fa-user"</span>></span><span class="hljs-tag"></<span class="hljs-title">i</span>></span>
                    <span class="hljs-tag"><<span class="hljs-title">input</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"form-control placeholder-no-fix"</span> <span class="hljs-attribute">type</span>=<span class="hljs-value">"text"</span> <span class="hljs-attribute">autocomplete</span>=<span class="hljs-value">"off"</span> <span class="hljs-attribute">placeholder</span>=<span class="hljs-value">"用户名"</span> <span class="hljs-attribute">name</span>=<span class="hljs-value">"account.id"</span> /></span>
                <span class="hljs-tag"></<span class="hljs-title">div</span>></span>
            <span class="hljs-tag"></<span class="hljs-title">div</span>></span>
            <span class="hljs-tag"><<span class="hljs-title">div</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"form-group"</span>></span>
                <span class="hljs-tag"><<span class="hljs-title">label</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"control-label visible-ie8 visible-ie9"</span>></span>密码<span class="hljs-tag"></<span class="hljs-title">label</span>></span>
                <span class="hljs-tag"><<span class="hljs-title">div</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"input-icon"</span>></span>
                    <span class="hljs-tag"><<span class="hljs-title">i</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"fa fa-lock"</span>></span><span class="hljs-tag"></<span class="hljs-title">i</span>></span>
                    <span class="hljs-tag"><<span class="hljs-title">input</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"form-control placeholder-no-fix"</span> <span class="hljs-attribute">type</span>=<span class="hljs-value">"password"</span> <span class="hljs-attribute">autocomplete</span>=<span class="hljs-value">"off"</span> <span class="hljs-attribute">placeholder</span>=<span class="hljs-value">"密码"</span> <span class="hljs-attribute">name</span>=<span class="hljs-value">"account.password"</span> /></span>
                <span class="hljs-tag"></<span class="hljs-title">div</span>></span>
            <span class="hljs-tag"></<span class="hljs-title">div</span>></span>
            <span class="hljs-tag"><<span class="hljs-title">div</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"form-actions"</span>></span>
                <span class="hljs-tag"><<span class="hljs-title">button</span> <span class="hljs-attribute">type</span>=<span class="hljs-value">"submit"</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"btn green pull-right"</span>></span>
                    登录 <span class="hljs-tag"><<span class="hljs-title">i</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"m-icon-swapright m-icon-white"</span>></span><span class="hljs-tag"></<span class="hljs-title">i</span>></span>
                <span class="hljs-tag"></<span class="hljs-title">button</span>></span>
            <span class="hljs-tag"></<span class="hljs-title">div</span>></span>
        <span class="hljs-tag"></<span class="hljs-title">form</span>></span></code><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li><li>20</li><li>21</li><li>22</li><li>23</li><li>24</li><li>25</li><li>26</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li><li>20</li><li>21</li><li>22</li><li>23</li><li>24</li><li>25</li><li>26</li></ul><code class="hljs java has-numbering"><span class="hljs-keyword">package</span> com.jpkc.pojo;
<span class="hljs-keyword">import</span> java.io.Serializable;
<span class="hljs-keyword">public</span> <span class="hljs-class"><span class="hljs-keyword">class</span> <span class="hljs-title">Account</span> <span class="hljs-keyword">implements</span> <span class="hljs-title">Serializable</span>
{</span>
    <span class="hljs-keyword">private</span> String id;
    <span class="hljs-keyword">private</span> String password;
    <span class="hljs-keyword">private</span> String name;
    <span class="hljs-keyword">public</span> <span class="hljs-title">Account</span>()
    {
        <span class="hljs-keyword">super</span>();
        <span class="hljs-comment">// TODO Auto-generated constructor stub</span>
    }
    <span class="hljs-keyword">public</span> String <span class="hljs-title">getId</span>()
    {
        <span class="hljs-keyword">return</span> id;
    }
    <span class="hljs-keyword">public</span> <span class="hljs-keyword">void</span> <span class="hljs-title">setId</span>(String id)
    {
        <span class="hljs-keyword">this</span>.id = id;
    }
    <span class="hljs-keyword">public</span> String <span class="hljs-title">getPassword</span>()
    {
        <span class="hljs-keyword">return</span> password;
    }
    <span class="hljs-keyword">public</span> <span class="hljs-keyword">void</span> <span class="hljs-title">setPassword</span>(String password)
    {
        <span class="hljs-keyword">this</span>.password = password;
    }
    <span class="hljs-keyword">public</span> String <span class="hljs-title">getName</span>()
    {
        <span class="hljs-keyword">return</span> name;
    }
    <span class="hljs-keyword">public</span> <span class="hljs-keyword">void</span> <span class="hljs-title">setName</span>(String name)
    {
        <span class="hljs-keyword">this</span>.name = name;
    }
}
</code><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li><li>20</li><li>21</li><li>22</li><li>23</li><li>24</li><li>25</li><li>26</li><li>27</li><li>28</li><li>29</li><li>30</li><li>31</li><li>32</li><li>33</li><li>34</li><li>35</li><li>36</li><li>37</li><li>38</li><li>39</li><li>40</li><li>41</li><li>42</li><li>43</li><li>44</li><li>45</li><li>46</li><li>47</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li><li>20</li><li>21</li><li>22</li><li>23</li><li>24</li><li>25</li><li>26</li><li>27</li><li>28</li><li>29</li><li>30</li><li>31</li><li>32</li><li>33</li><li>34</li><li>35</li><li>36</li><li>37</li><li>38</li><li>39</li><li>40</li><li>41</li><li>42</li><li>43</li><li>44</li><li>45</li><li>46</li><li>47</li></ul><code class="hljs java has-numbering"><span class="hljs-keyword">package</span> com.jpkc.action;
<span class="hljs-keyword">import</span> java.util.HashMap;
<span class="hljs-keyword">import</span> java.util.Map;
<span class="hljs-keyword">import</span> com.jpkc.common.CM;
<span class="hljs-keyword">import</span> com.jpkc.pojo.Account;
<span class="hljs-keyword">public</span> <span class="hljs-class"><span class="hljs-keyword">class</span> <span class="hljs-title">AccountAction</span> <span class="hljs-keyword">extends</span> <span class="hljs-title">BaseAction</span>
{</span>
    <span class="hljs-keyword">private</span> Account account;
    <span class="hljs-keyword">public</span> String <span class="hljs-title">login_do</span>()
    {
        String method = getRequest().getMethod();
        <span class="hljs-keyword">if</span> (method.toUpperCase().equals(<span class="hljs-string">"GET"</span>))
        {
            <span class="hljs-keyword">return</span> <span class="hljs-string">"404"</span>;
        }
        <span class="hljs-keyword">if</span> (account == <span class="hljs-keyword">null</span> || CM.validIsEmpty(account.getId()) || CM.validIsEmpty(account.getPassword()))
        {
            <span class="hljs-keyword">return</span> ERROR;
        }
        getSession().setAttribute(<span class="hljs-string">"accountSession"</span>, account);
        <span class="hljs-keyword">return</span> SUCCESS;
    }
    <span class="hljs-keyword">public</span> Account <span class="hljs-title">getAccount</span>()
    {
        <span class="hljs-keyword">return</span> account;
    }
    <span class="hljs-keyword">public</span> <span class="hljs-keyword">void</span> <span class="hljs-title">setAccount</span>(Account account)
    {
        <span class="hljs-keyword">this</span>.account = account;
    }
}</code><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li><li>20</li><li>21</li><li>22</li><li>23</li><li>24</li><li>25</li><li>26</li><li>27</li><li>28</li><li>29</li><li>30</li><li>31</li><li>32</li><li>33</li><li>34</li><li>35</li><li>36</li><li>37</li><li>38</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li><li>20</li><li>21</li><li>22</li><li>23</li><li>24</li><li>25</li><li>26</li><li>27</li><li>28</li><li>29</li><li>30</li><li>31</li><li>32</li><li>33</li><li>34</li><li>35</li><li>36</li><li>37</li><li>38</li></ul>
**SpringMVC 接收 request 参数**
<code class="hljs xml has-numbering">        <span class="hljs-tag"><<span class="hljs-title">form</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"login-form"</span> <span class="hljs-attribute">action</span>=<span class="hljs-value">"admin/login_do"</span> <span class="hljs-attribute">method</span>=<span class="hljs-value">"post"</span>></span>
            <span class="hljs-tag"><<span class="hljs-title">h3</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"form-title"</span>></span>登录系统<span class="hljs-tag"></<span class="hljs-title">h3</span>></span>
            <span class="hljs-tag"><<span class="hljs-title">div</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"alert alert-danger display-hide"</span>></span>
                <span class="hljs-tag"><<span class="hljs-title">button</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"close"</span> <span class="hljs-attribute">data-close</span>=<span class="hljs-value">"alert"</span>></span><span class="hljs-tag"></<span class="hljs-title">button</span>></span>
                <span class="hljs-tag"><<span class="hljs-title">span</span>></span> 请输入用户名和密码 <span class="hljs-tag"></<span class="hljs-title">span</span>></span>
            <span class="hljs-tag"></<span class="hljs-title">div</span>></span>
            <span class="hljs-tag"><<span class="hljs-title">div</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"form-group"</span>></span>
                <span class="hljs-tag"><<span class="hljs-title">label</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"control-label visible-ie8 visible-ie9"</span>></span>用户名<span class="hljs-tag"></<span class="hljs-title">label</span>></span>
                <span class="hljs-tag"><<span class="hljs-title">div</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"input-icon"</span>></span>
                    <span class="hljs-tag"><<span class="hljs-title">i</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"fa fa-user"</span>></span><span class="hljs-tag"></<span class="hljs-title">i</span>></span>
                    <span class="hljs-tag"><<span class="hljs-title">input</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"form-control placeholder-no-fix"</span> <span class="hljs-attribute">type</span>=<span class="hljs-value">"text"</span> <span class="hljs-attribute">autocomplete</span>=<span class="hljs-value">"off"</span> <span class="hljs-attribute">placeholder</span>=<span class="hljs-value">"用户名"</span> <span class="hljs-attribute">name</span>=<span class="hljs-value">"id"</span> /></span>
                <span class="hljs-tag"></<span class="hljs-title">div</span>></span>
            <span class="hljs-tag"></<span class="hljs-title">div</span>></span>
            <span class="hljs-tag"><<span class="hljs-title">div</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"form-group"</span>></span>
                <span class="hljs-tag"><<span class="hljs-title">label</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"control-label visible-ie8 visible-ie9"</span>></span>密码<span class="hljs-tag"></<span class="hljs-title">label</span>></span>
                <span class="hljs-tag"><<span class="hljs-title">div</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"input-icon"</span>></span>
                    <span class="hljs-tag"><<span class="hljs-title">i</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"fa fa-lock"</span>></span><span class="hljs-tag"></<span class="hljs-title">i</span>></span>
                    <span class="hljs-tag"><<span class="hljs-title">input</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"form-control placeholder-no-fix"</span> <span class="hljs-attribute">type</span>=<span class="hljs-value">"password"</span> <span class="hljs-attribute">autocomplete</span>=<span class="hljs-value">"off"</span> <span class="hljs-attribute">placeholder</span>=<span class="hljs-value">"密码"</span> <span class="hljs-attribute">name</span>=<span class="hljs-value">"password"</span> /></span>
                <span class="hljs-tag"></<span class="hljs-title">div</span>></span>
            <span class="hljs-tag"></<span class="hljs-title">div</span>></span>
            <span class="hljs-tag"><<span class="hljs-title">div</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"form-actions"</span>></span>
                <span class="hljs-tag"><<span class="hljs-title">button</span> <span class="hljs-attribute">type</span>=<span class="hljs-value">"submit"</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"btn green pull-right"</span>></span>
                    登录 <span class="hljs-tag"><<span class="hljs-title">i</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"m-icon-swapright m-icon-white"</span>></span><span class="hljs-tag"></<span class="hljs-title">i</span>></span>
                <span class="hljs-tag"></<span class="hljs-title">button</span>></span>
            <span class="hljs-tag"></<span class="hljs-title">div</span>></span>
        <span class="hljs-tag"></<span class="hljs-title">form</span>></span></code><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li><li>20</li><li>21</li><li>22</li><li>23</li><li>24</li><li>25</li><li>26</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li><li>20</li><li>21</li><li>22</li><li>23</li><li>24</li><li>25</li><li>26</li></ul><code class="hljs java has-numbering"><span class="hljs-keyword">package</span> com.jpkc.pojo;
<span class="hljs-keyword">import</span> java.io.Serializable;
<span class="hljs-keyword">public</span> <span class="hljs-class"><span class="hljs-keyword">class</span> <span class="hljs-title">Account</span> <span class="hljs-keyword">implements</span> <span class="hljs-title">Serializable</span>
{</span>
    <span class="hljs-keyword">private</span> String id;
    <span class="hljs-keyword">private</span> String password;
    <span class="hljs-keyword">private</span> String name;
    <span class="hljs-keyword">public</span> <span class="hljs-title">Account</span>()
    {
        <span class="hljs-keyword">super</span>();
        <span class="hljs-comment">// TODO Auto-generated constructor stub</span>
    }
    <span class="hljs-keyword">public</span> String <span class="hljs-title">getId</span>()
    {
        <span class="hljs-keyword">return</span> id;
    }
    <span class="hljs-keyword">public</span> <span class="hljs-keyword">void</span> <span class="hljs-title">setId</span>(String id)
    {
        <span class="hljs-keyword">this</span>.id = id;
    }
    <span class="hljs-keyword">public</span> String <span class="hljs-title">getPassword</span>()
    {
        <span class="hljs-keyword">return</span> password;
    }
    <span class="hljs-keyword">public</span> <span class="hljs-keyword">void</span> <span class="hljs-title">setPassword</span>(String password)
    {
        <span class="hljs-keyword">this</span>.password = password;
    }
    <span class="hljs-keyword">public</span> String <span class="hljs-title">getName</span>()
    {
        <span class="hljs-keyword">return</span> name;
    }
    <span class="hljs-keyword">public</span> <span class="hljs-keyword">void</span> <span class="hljs-title">setName</span>(String name)
    {
        <span class="hljs-keyword">this</span>.name = name;
    }
}
</code><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li><li>20</li><li>21</li><li>22</li><li>23</li><li>24</li><li>25</li><li>26</li><li>27</li><li>28</li><li>29</li><li>30</li><li>31</li><li>32</li><li>33</li><li>34</li><li>35</li><li>36</li><li>37</li><li>38</li><li>39</li><li>40</li><li>41</li><li>42</li><li>43</li><li>44</li><li>45</li><li>46</li><li>47</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li><li>20</li><li>21</li><li>22</li><li>23</li><li>24</li><li>25</li><li>26</li><li>27</li><li>28</li><li>29</li><li>30</li><li>31</li><li>32</li><li>33</li><li>34</li><li>35</li><li>36</li><li>37</li><li>38</li><li>39</li><li>40</li><li>41</li><li>42</li><li>43</li><li>44</li><li>45</li><li>46</li><li>47</li></ul><code class="hljs avrasm has-numbering">package <span class="hljs-keyword">com</span><span class="hljs-preprocessor">.jpkc</span><span class="hljs-preprocessor">.controller</span><span class="hljs-comment">;</span>
import java<span class="hljs-preprocessor">.util</span><span class="hljs-preprocessor">.HashMap</span><span class="hljs-comment">;</span>
import java<span class="hljs-preprocessor">.util</span><span class="hljs-preprocessor">.Map</span><span class="hljs-comment">;</span>
import javax<span class="hljs-preprocessor">.servlet</span><span class="hljs-preprocessor">.http</span><span class="hljs-preprocessor">.HttpServletRequest</span><span class="hljs-comment">;</span>
import javax<span class="hljs-preprocessor">.servlet</span><span class="hljs-preprocessor">.http</span><span class="hljs-preprocessor">.HttpServletResponse</span><span class="hljs-comment">;</span>
import javax<span class="hljs-preprocessor">.servlet</span><span class="hljs-preprocessor">.http</span><span class="hljs-preprocessor">.HttpSession</span><span class="hljs-comment">;</span>
import org<span class="hljs-preprocessor">.codehaus</span><span class="hljs-preprocessor">.jackson</span><span class="hljs-preprocessor">.map</span><span class="hljs-preprocessor">.ObjectMapper</span><span class="hljs-comment">;</span>
import org<span class="hljs-preprocessor">.springframework</span><span class="hljs-preprocessor">.beans</span><span class="hljs-preprocessor">.factory</span><span class="hljs-preprocessor">.annotation</span><span class="hljs-preprocessor">.Autowired</span><span class="hljs-comment">;</span>
import org<span class="hljs-preprocessor">.springframework</span><span class="hljs-preprocessor">.stereotype</span><span class="hljs-preprocessor">.Controller</span><span class="hljs-comment">;</span>
import org<span class="hljs-preprocessor">.springframework</span><span class="hljs-preprocessor">.web</span><span class="hljs-preprocessor">.bind</span><span class="hljs-preprocessor">.annotation</span><span class="hljs-preprocessor">.RequestMapping</span><span class="hljs-comment">;</span>
import org<span class="hljs-preprocessor">.springframework</span><span class="hljs-preprocessor">.web</span><span class="hljs-preprocessor">.bind</span><span class="hljs-preprocessor">.annotation</span><span class="hljs-preprocessor">.RequestMethod</span><span class="hljs-comment">;</span>
import org<span class="hljs-preprocessor">.springframework</span><span class="hljs-preprocessor">.web</span><span class="hljs-preprocessor">.servlet</span><span class="hljs-preprocessor">.ModelAndView</span><span class="hljs-comment">;</span>
import <span class="hljs-keyword">com</span><span class="hljs-preprocessor">.jpkc</span><span class="hljs-preprocessor">.common</span><span class="hljs-preprocessor">.CM</span><span class="hljs-comment">;</span>
import <span class="hljs-keyword">com</span><span class="hljs-preprocessor">.jpkc</span><span class="hljs-preprocessor">.exception</span><span class="hljs-preprocessor">.CustomException</span><span class="hljs-comment">;</span>
import <span class="hljs-keyword">com</span><span class="hljs-preprocessor">.jpkc</span><span class="hljs-preprocessor">.mapper</span><span class="hljs-preprocessor">.CourseInfoMapper</span><span class="hljs-comment">;</span>
import <span class="hljs-keyword">com</span><span class="hljs-preprocessor">.jpkc</span><span class="hljs-preprocessor">.pojo</span><span class="hljs-preprocessor">.Account</span><span class="hljs-comment">;</span>
import <span class="hljs-keyword">com</span><span class="hljs-preprocessor">.jpkc</span><span class="hljs-preprocessor">.pojo</span><span class="hljs-preprocessor">.CourseInfo</span><span class="hljs-comment">;</span>
import <span class="hljs-keyword">com</span><span class="hljs-preprocessor">.jpkc</span><span class="hljs-preprocessor">.service</span><span class="hljs-preprocessor">.LoginService</span><span class="hljs-comment">;</span>
@RequestMapping(<span class="hljs-string">"/admin"</span>)
@Controller
public class LoginController
{
    @Autowired
    LoginService loginService<span class="hljs-comment">;</span>
    @RequestMapping(value = <span class="hljs-string">"/login_do"</span>, method = { RequestMethod<span class="hljs-preprocessor">.POST</span> })
    public void login_do(ModelAndView model, HttpServletRequest request, HttpServletResponse response, HttpSession session, Account account) throws Exception
    {
        response<span class="hljs-preprocessor">.setCharacterEncoding</span>(<span class="hljs-string">"utf-8"</span>)<span class="hljs-comment">;</span>
        response<span class="hljs-preprocessor">.setContentType</span>(<span class="hljs-string">"application/json;charset=utf-8"</span>)<span class="hljs-comment">;</span>
        Map<String, Object> json = new HashMap<String, Object>()<span class="hljs-comment">;</span>
        String info<span class="hljs-comment">;</span>
        if (account == null || CM<span class="hljs-preprocessor">.validIsEmpty</span>(account<span class="hljs-preprocessor">.getId</span>()) || CM<span class="hljs-preprocessor">.validIsEmpty</span>(account<span class="hljs-preprocessor">.getPassword</span>()))
        {
            info = <span class="hljs-string">"用户名、密码都是必填项。"</span><span class="hljs-comment">;</span>
            json<span class="hljs-preprocessor">.put</span>(<span class="hljs-string">"success"</span>, false)<span class="hljs-comment">;</span>
            json<span class="hljs-preprocessor">.put</span>(<span class="hljs-string">"info"</span>, info)<span class="hljs-comment">;</span>
            response<span class="hljs-preprocessor">.getWriter</span>()<span class="hljs-preprocessor">.write</span>(new ObjectMapper()<span class="hljs-preprocessor">.writeValueAsString</span>(json))<span class="hljs-comment">;</span>
            return<span class="hljs-comment">;</span>
        }
        session<span class="hljs-preprocessor">.setAttribute</span>(<span class="hljs-string">"accountSession"</span>, account)<span class="hljs-comment">;</span>
        json<span class="hljs-preprocessor">.put</span>(<span class="hljs-string">"success"</span>, true)<span class="hljs-comment">;</span>
        response<span class="hljs-preprocessor">.getWriter</span>()<span class="hljs-preprocessor">.write</span>(new ObjectMapper()<span class="hljs-preprocessor">.writeValueAsString</span>(json))<span class="hljs-comment">;</span>
        return<span class="hljs-comment">;</span>
    }
}</code><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li><li>20</li><li>21</li><li>22</li><li>23</li><li>24</li><li>25</li><li>26</li><li>27</li><li>28</li><li>29</li><li>30</li><li>31</li><li>32</li><li>33</li><li>34</li><li>35</li><li>36</li><li>37</li><li>38</li><li>39</li><li>40</li><li>41</li><li>42</li><li>43</li><li>44</li><li>45</li><li>46</li><li>47</li><li>48</li><li>49</li><li>50</li><li>51</li><li>52</li><li>53</li><li>54</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li><li>20</li><li>21</li><li>22</li><li>23</li><li>24</li><li>25</li><li>26</li><li>27</li><li>28</li><li>29</li><li>30</li><li>31</li><li>32</li><li>33</li><li>34</li><li>35</li><li>36</li><li>37</li><li>38</li><li>39</li><li>40</li><li>41</li><li>42</li><li>43</li><li>44</li><li>45</li><li>46</li><li>47</li><li>48</li><li>49</li><li>50</li><li>51</li><li>52</li><li>53</li><li>54</li></ul>
Struts2单个方法可以处理一个request，接收参数Account需要定义一个成员变量，Struts2会自动将对应的参数调用成员变量的set方法设置进去。处理方法可以在方法内获取到。用完还存在request级别Map中。
SpringMVC的单个方法也对应于一个request，接收参数Account需要定义一个方法参数，SpringMVC会自动将对应的参数设置到方法参数中去。处理方法可以在方法内获取到。用完即销毁。
可以看出两种框架都可以实现参数的自动转换。Struts2定义一个成员变量，其他方法都是可以共享的，不用重新定义。SpringMVC每个方法都是独立的，方法参数是每一个方法独享的。
各有利弊。
成员变量共享可以避免重复定义，但是方法一多，用到的成员变量原来越多，整个Action类会惨不忍睹，因为你不知道其中一个方法具体会用到哪几个成员变量。而且用不到的成员变量也被存储到request级别Map中了。造成内存的浪费。
方法参数是方法独享的。则不能复用到其他方法，但是对于当前方法来说有哪些参数足够明确，而且不用和其他方法搅合，干脆利落。
从JVM角度来说，Struts2成员变量会被分配到堆中。SpringMVC方法参数则会存在于方法栈中，一般认为栈比堆更轻量一些，方法结束，用完参数即回收。堆需要垃圾回收触发时才能统一回收。
## **Struts2 和 SpringMVC 限制访问方式GET和POST的不同**
在上例中，表单提交有密码，需要指定只接受POST提交方式。
**Struts2指定POST方式**
<code class="hljs lasso has-numbering">    <span class="hljs-keyword">public</span> <span class="hljs-built_in">String</span> login_do()
    {
        <span class="hljs-built_in">String</span> method <span class="hljs-subst">=</span> getRequest()<span class="hljs-built_in">.</span>getMethod();
        <span class="hljs-keyword">if</span> (method<span class="hljs-built_in">.</span>toUpperCase()<span class="hljs-built_in">.</span><span class="hljs-keyword">equals</span>(<span class="hljs-string">"GET"</span>))
        {
            <span class="hljs-keyword">return</span> <span class="hljs-string">"404"</span>;
        }
    }</code><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li></ul>
**SpringMVC指定POST方式**
<code class="hljs avrasm has-numbering">    @RequestMapping(value = <span class="hljs-string">"/login_do"</span>, method = { RequestMethod<span class="hljs-preprocessor">.POST</span> })
    public void login_do(ModelAndView model, HttpServletRequest request, HttpServletResponse response, HttpSession session, Account account) throws Exception
    {
        response<span class="hljs-preprocessor">.setCharacterEncoding</span>(<span class="hljs-string">"utf-8"</span>)<span class="hljs-comment">;</span>
        response<span class="hljs-preprocessor">.setContentType</span>(<span class="hljs-string">"application/json;charset=utf-8"</span>)<span class="hljs-comment">;</span>
        Map<String, Object> json = new HashMap<String, Object>()<span class="hljs-comment">;</span>
        String info<span class="hljs-comment">;</span>
        if (account == null || CM<span class="hljs-preprocessor">.validIsEmpty</span>(account<span class="hljs-preprocessor">.getId</span>()) || CM<span class="hljs-preprocessor">.validIsEmpty</span>(account<span class="hljs-preprocessor">.getPassword</span>()))
        {
            info = <span class="hljs-string">"用户名、密码都是必填项。"</span><span class="hljs-comment">;</span>
            json<span class="hljs-preprocessor">.put</span>(<span class="hljs-string">"success"</span>, false)<span class="hljs-comment">;</span>
            json<span class="hljs-preprocessor">.put</span>(<span class="hljs-string">"info"</span>, info)<span class="hljs-comment">;</span>
            response<span class="hljs-preprocessor">.getWriter</span>()<span class="hljs-preprocessor">.write</span>(new ObjectMapper()<span class="hljs-preprocessor">.writeValueAsString</span>(json))<span class="hljs-comment">;</span>
            return<span class="hljs-comment">;</span>
        }
        session<span class="hljs-preprocessor">.setAttribute</span>(<span class="hljs-string">"accountSession"</span>, account)<span class="hljs-comment">;</span>
        json<span class="hljs-preprocessor">.put</span>(<span class="hljs-string">"success"</span>, true)<span class="hljs-comment">;</span>
        response<span class="hljs-preprocessor">.getWriter</span>()<span class="hljs-preprocessor">.write</span>(new ObjectMapper()<span class="hljs-preprocessor">.writeValueAsString</span>(json))<span class="hljs-comment">;</span>
        return<span class="hljs-comment">;</span>
    }</code><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li><li>20</li><li>21</li><li>22</li><li>23</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li><li>20</li><li>21</li><li>22</li><li>23</li></ul>
Struts2限制只能通过POST方式访问，是通过调用request的getMethod方法来得到当前访问方式。然后手工的去判断。
SpringMVC也可以调用request的getMethod方法来判断，但是框架本身提供了方便的内置判断。使用注解即可。
Struts2通过拦截器设置好访问方式的代码后，也可以通过注解的方式指定拦截器得到同样的效果。本身不是太难的事情，两个框架都可以实现，Struts2需要手工实现，SpringMVC默认提供了。即使SpringMVC不提供，调用SpringMVC的拦截器也能和Struts2的拦截器的效果一样。在GET和POST访问限制方面，并没有谁优谁劣，都可以实现。只是SpringMVC愿意往前多走一小步。
## **Struts2 和 SpringMVC 拦截器的不同**
后台页面需要登录，我们可以使用拦截器限制未登录的用户访问。
**Struts2实现拦截器的方式**
<code class="hljs java has-numbering"><span class="hljs-keyword">public</span> <span class="hljs-class"><span class="hljs-keyword">class</span> <span class="hljs-title">ManagerLoginInterceptor</span> <span class="hljs-keyword">extends</span> <span class="hljs-title">AbstractInterceptor</span>
{</span>
    <span class="hljs-annotation">@Override</span>
    <span class="hljs-keyword">public</span> String <span class="hljs-title">intercept</span>(ActionInvocation invocation) <span class="hljs-keyword">throws</span> Exception
    {
        String actionName = ServletActionContext.getActionMapping().getName();
        <span class="hljs-comment">// 如果是登录、注册、退出的话就不要拦截了</span>
        <span class="hljs-keyword">if</span> (actionName.equals(<span class="hljs-string">"exit"</span>) || actionName.equals(<span class="hljs-string">"login"</span>) || actionName.equals(<span class="hljs-string">"login_do"</span>) || actionName.equals(<span class="hljs-string">"regist"</span>)
                || actionName.equals(<span class="hljs-string">"regist_do"</span>))
        {
            <span class="hljs-keyword">return</span> invocation.invoke();
        }
        <span class="hljs-comment">// 如果不是管理员就不能进入</span>
        Manager managerTemp = (Manager) ServletActionContext.getRequest().getSession().getAttribute(<span class="hljs-string">"managerSession"</span>);
        <span class="hljs-keyword">if</span> (managerTemp == <span class="hljs-keyword">null</span>)
        {
            <span class="hljs-keyword">return</span> <span class="hljs-string">"manager_login"</span>;
        }
        <span class="hljs-comment">//验证成功，放行。</span>
        <span class="hljs-keyword">return</span> invocation.invoke();
    }
}</code><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li><li>20</li><li>21</li><li>22</li><li>23</li><li>24</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li><li>20</li><li>21</li><li>22</li><li>23</li><li>24</li></ul><code class="hljs xml has-numbering">    <span class="hljs-tag"><<span class="hljs-title">package</span> <span class="hljs-attribute">name</span>=<span class="hljs-value">"admin_package"</span> <span class="hljs-attribute">namespace</span>=<span class="hljs-value">"/admin"</span> <span class="hljs-attribute">extends</span>=<span class="hljs-value">"ssh-default"</span>></span>
        <span class="hljs-tag"><<span class="hljs-title">interceptors</span>></span>
            <span class="hljs-tag"><<span class="hljs-title">interceptor</span> <span class="hljs-attribute">name</span>=<span class="hljs-value">"LoginManagerValidate"</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"com.example.interceptor.ManagerLoginInterceptor"</span>></span>
            <span class="hljs-tag"></<span class="hljs-title">interceptor</span>></span>
            <span class="hljs-comment"><!-- 自定义拦截器栈-拦截未登录的管理员- --></span>
            <span class="hljs-tag"><<span class="hljs-title">interceptor-stack</span> <span class="hljs-attribute">name</span>=<span class="hljs-value">"LoginManagerValidateStack"</span>></span>
                <span class="hljs-tag"><<span class="hljs-title">interceptor-ref</span> <span class="hljs-attribute">name</span>=<span class="hljs-value">"LoginManagerValidate"</span>></span><span class="hljs-tag"></<span class="hljs-title">interceptor-ref</span>></span>
                <span class="hljs-tag"><<span class="hljs-title">interceptor-ref</span> <span class="hljs-attribute">name</span>=<span class="hljs-value">"defaultStack"</span>></span><span class="hljs-tag"></<span class="hljs-title">interceptor-ref</span>></span>
            <span class="hljs-tag"></<span class="hljs-title">interceptor-stack</span>></span>
        <span class="hljs-tag"></<span class="hljs-title">interceptors</span>></span>
        <span class="hljs-tag"><<span class="hljs-title">action</span> <span class="hljs-attribute">name</span>=<span class="hljs-value">"m_*"</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"com.example.action.ManagerAction"</span> <span class="hljs-attribute">method</span>=<span class="hljs-value">"m_{1}"</span>></span>
            <span class="hljs-tag"><<span class="hljs-title">interceptor-ref</span> <span class="hljs-attribute">name</span>=<span class="hljs-value">"LoginManagerValidateStack"</span>></span><span class="hljs-tag"></<span class="hljs-title">interceptor-ref</span>></span>
            <span class="hljs-tag"><<span class="hljs-title">result</span> <span class="hljs-attribute">name</span>=<span class="hljs-value">"success"</span> <span class="hljs-attribute">type</span>=<span class="hljs-value">"json"</span>></span>
                <span class="hljs-tag"><<span class="hljs-title">param</span> <span class="hljs-attribute">name</span>=<span class="hljs-value">"root"</span>></span>json<span class="hljs-tag"></<span class="hljs-title">param</span>></span>
            <span class="hljs-tag"></<span class="hljs-title">result</span>></span>
        <span class="hljs-tag"></<span class="hljs-title">action</span>></span>
    <span class="hljs-tag"></<span class="hljs-title">package</span>></span></code><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li></ul>
Struts2还提供了很多默认的拦截器供用户调用。
<code class="hljs applescript has-numbering"><interceptors>  
           <interceptor <span class="hljs-property">name</span>=<span class="hljs-string">"alias"</span><span class="hljs-type">class</span>=<span class="hljs-string">"com.opensymphony.xwork2.interceptor.AliasInterceptor"</span>/>  
           <interceptor <span class="hljs-property">name</span>=<span class="hljs-string">"autowiring"</span><span class="hljs-type">class</span>=<span class="hljs-string">"com.opensymphony.xwork2.spring.interceptor.ActionAutowiringInterceptor"</span>/>  
           <interceptor <span class="hljs-property">name</span>=<span class="hljs-string">"chain"</span><span class="hljs-type">class</span>=<span class="hljs-string">"com.opensymphony.xwork2.interceptor.ChainingInterceptor"</span>/>  
           <interceptor <span class="hljs-property">name</span>=<span class="hljs-string">"conversionError"</span><span class="hljs-type">class</span>=<span class="hljs-string">"org.apache.struts2.interceptor.StrutsConversionErrorInterceptor"</span>/>  
           <interceptor <span class="hljs-property">name</span>=<span class="hljs-string">"clearSession"</span><span class="hljs-type">class</span>=<span class="hljs-string">"org.apache.struts2.interceptor.ClearSessionInterceptor"</span>/>  
           <interceptor <span class="hljs-property">name</span>=<span class="hljs-string">"createSession"</span><span class="hljs-type">class</span>=<span class="hljs-string">"org.apache.struts2.interceptor.CreateSessionInterceptor"</span>/>  
           <interceptor <span class="hljs-property">name</span>=<span class="hljs-string">"debugging"</span><span class="hljs-type">class</span>=<span class="hljs-string">"org.apache.struts2.interceptor.debugging.DebuggingInterceptor"</span>/>  
           <interceptor <span class="hljs-property">name</span>=<span class="hljs-string">"externalRef"</span><span class="hljs-type">class</span>=<span class="hljs-string">"com.opensymphony.xwork2.interceptor.ExternalReferencesInterceptor"</span>/>  
           <interceptor <span class="hljs-property">name</span>=<span class="hljs-string">"execAndWait"</span><span class="hljs-type">class</span>=<span class="hljs-string">"org.apache.struts2.interceptor.ExecuteAndWaitInterceptor"</span>/>  
           <interceptor <span class="hljs-property">name</span>=<span class="hljs-string">"exception"</span><span class="hljs-type">class</span>=<span class="hljs-string">"com.opensymphony.xwork2.interceptor.ExceptionMappingInterceptor"</span>/>  
           <interceptor <span class="hljs-property">name</span>=<span class="hljs-string">"fileUpload"</span><span class="hljs-type">class</span>=<span class="hljs-string">"org.apache.struts2.interceptor.FileUploadInterceptor"</span>/>  
           <interceptor <span class="hljs-property">name</span>=<span class="hljs-string">"i18n"</span><span class="hljs-type">class</span>=<span class="hljs-string">"com.opensymphony.xwork2.interceptor.I18nInterceptor"</span>/>  
           <interceptor <span class="hljs-property">name</span>=<span class="hljs-string">"logger"</span><span class="hljs-type">class</span>=<span class="hljs-string">"com.opensymphony.xwork2.interceptor.LoggingInterceptor"</span>/>  
           <interceptor <span class="hljs-property">name</span>=<span class="hljs-string">"modelDriven"</span><span class="hljs-type">class</span>=<span class="hljs-string">"com.opensymphony.xwork2.interceptor.ModelDrivenInterceptor"</span>/>  
           <interceptor <span class="hljs-property">name</span>=<span class="hljs-string">"scopedModelDriven"</span><span class="hljs-type">class</span>=<span class="hljs-string">"com.opensymphony.xwork2.interceptor.ScopedModelDrivenInterceptor"</span>/>  
           <interceptor <span class="hljs-property">name</span>=<span class="hljs-string">"params"</span><span class="hljs-type">class</span>=<span class="hljs-string">"com.opensymphony.xwork2.interceptor.ParametersInterceptor"</span>/>  
           <interceptor <span class="hljs-property">name</span>=<span class="hljs-string">"actionMappingParams"</span><span class="hljs-type">class</span>=<span class="hljs-string">"org.apache.struts2.interceptor.ActionMappingParametersInteceptor"</span>/>  
           <interceptor <span class="hljs-property">name</span>=<span class="hljs-string">"prepare"</span><span class="hljs-type">class</span>=<span class="hljs-string">"com.opensymphony.xwork2.interceptor.PrepareInterceptor"</span>/>  
           <interceptor <span class="hljs-property">name</span>=<span class="hljs-string">"staticParams"</span><span class="hljs-type">class</span>=<span class="hljs-string">"com.opensymphony.xwork2.interceptor.StaticParametersInterceptor"</span>/>  
           <interceptor <span class="hljs-property">name</span>=<span class="hljs-string">"scope"</span><span class="hljs-type">class</span>=<span class="hljs-string">"org.apache.struts2.interceptor.ScopeInterceptor"</span>/>  
           <interceptor <span class="hljs-property">name</span>=<span class="hljs-string">"servletConfig"</span><span class="hljs-type">class</span>=<span class="hljs-string">"org.apache.struts2.interceptor.ServletConfigInterceptor"</span>/>  
           <interceptor <span class="hljs-property">name</span>=<span class="hljs-string">"sessionAutowiring"</span><span class="hljs-type">class</span>=<span class="hljs-string">"org.apache.struts2.spring.interceptor.SessionContextAutowiringInterceptor"</span>/>  
           <interceptor <span class="hljs-property">name</span>=<span class="hljs-string">"timer"</span><span class="hljs-type">class</span>=<span class="hljs-string">"com.opensymphony.xwork2.interceptor.TimerInterceptor"</span>/>  
           <interceptor <span class="hljs-property">name</span>=<span class="hljs-string">"token"</span><span class="hljs-type">class</span>=<span class="hljs-string">"org.apache.struts2.interceptor.TokenInterceptor"</span>/>  
           <interceptor <span class="hljs-property">name</span>=<span class="hljs-string">"tokenSession"</span><span class="hljs-type">class</span>=<span class="hljs-string">"org.apache.struts2.interceptor.TokenSessionStoreInterceptor"</span>/>  
           <interceptor <span class="hljs-property">name</span>=<span class="hljs-string">"validation"</span><span class="hljs-type">class</span>=<span class="hljs-string">"org.apache.struts2.interceptor.validation.AnnotationValidationInterceptor"</span>/>  
           <interceptor <span class="hljs-property">name</span>=<span class="hljs-string">"workflow"</span><span class="hljs-type">class</span>=<span class="hljs-string">"com.opensymphony.xwork2.interceptor.DefaultWorkflowInterceptor"</span>/>  
           <interceptor <span class="hljs-property">name</span>=<span class="hljs-string">"store"</span><span class="hljs-type">class</span>=<span class="hljs-string">"org.apache.struts2.interceptor.MessageStoreInterceptor"</span>/>  
           <interceptor <span class="hljs-property">name</span>=<span class="hljs-string">"checkbox"</span><span class="hljs-type">class</span>=<span class="hljs-string">"org.apache.struts2.interceptor.CheckboxInterceptor"</span>/>  
           <interceptor <span class="hljs-property">name</span>=<span class="hljs-string">"profiling"</span><span class="hljs-type">class</span>=<span class="hljs-string">"org.apache.struts2.interceptor.ProfilingActivationInterceptor"</span>/>  
           <interceptor <span class="hljs-property">name</span>=<span class="hljs-string">"roles"</span><span class="hljs-type">class</span>=<span class="hljs-string">"org.apache.struts2.interceptor.RolesInterceptor"</span>/>  
           <interceptor <span class="hljs-property">name</span>=<span class="hljs-string">"jsonValidation"</span><span class="hljs-type">class</span>=<span class="hljs-string">"org.apache.struts2.interceptor.validation.JSONValidationInterceptor"</span>/>  
           <interceptornameinterceptorname=<span class="hljs-string">"annotationWorkflow"</span><span class="hljs-type">class</span>=<span class="hljs-string">"com.opensymphony.xwork2.interceptor.annotations.AnnotationWorkflowInterceptor"</span>/>  </code><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li><li>20</li><li>21</li><li>22</li><li>23</li><li>24</li><li>25</li><li>26</li><li>27</li><li>28</li><li>29</li><li>30</li><li>31</li><li>32</li><li>33</li><li>34</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li><li>20</li><li>21</li><li>22</li><li>23</li><li>24</li><li>25</li><li>26</li><li>27</li><li>28</li><li>29</li><li>30</li><li>31</li><li>32</li><li>33</li><li>34</li></ul>
**SpringMVC实现拦截器的方式**
<code class="hljs java has-numbering"><span class="hljs-keyword">public</span> <span class="hljs-class"><span class="hljs-keyword">class</span> <span class="hljs-title">LoginInterceptor</span> <span class="hljs-keyword">implements</span> <span class="hljs-title">HandlerInterceptor</span>
{</span>
    <span class="hljs-comment">// 进入 Handler方法之前执行</span>
    <span class="hljs-comment">// 用于身份认证、身份授权</span>
    <span class="hljs-comment">// 比如身份认证，如果认证通过表示当前用户没有登陆，需要此方法拦截不再向下执行</span>
    <span class="hljs-annotation">@Override</span>
    <span class="hljs-keyword">public</span> <span class="hljs-keyword">boolean</span> <span class="hljs-title">preHandle</span>(HttpServletRequest request, HttpServletResponse response, Object handler) <span class="hljs-keyword">throws</span> Exception
    {
        <span class="hljs-comment">// 获取请求的url</span>
        String url = request.getRequestURI();
        <span class="hljs-comment">// 判断url是否是公开 地址（实际使用时将公开 地址配置配置文件中）</span>
        <span class="hljs-comment">// 这里公开地址是登陆提交的地址</span>
        <span class="hljs-keyword">if</span> (url.indexOf(<span class="hljs-string">"login"</span>) >= <span class="hljs-number">0</span> || url.indexOf(<span class="hljs-string">"exit"</span>) >= <span class="hljs-number">0</span>)
        {
            <span class="hljs-comment">// 如果进行登陆提交，放行</span>
            <span class="hljs-keyword">return</span> <span class="hljs-keyword">true</span>;
        }
        <span class="hljs-comment">// 判断session</span>
        HttpSession session = request.getSession();
        <span class="hljs-comment">// 从session中取出用户身份信息</span>
        Account account = (Account) session.getAttribute(<span class="hljs-string">"accountSession"</span>);
        <span class="hljs-keyword">if</span> (account != <span class="hljs-keyword">null</span>)
        {
            <span class="hljs-comment">// 身份存在，放行</span>
            <span class="hljs-keyword">return</span> <span class="hljs-keyword">true</span>;
        }
        <span class="hljs-comment">// 执行这里表示用户身份需要认证，跳转登陆页面</span>
        request.getRequestDispatcher(<span class="hljs-string">"/manager/login.jsp"</span>).forward(request, response);
        <span class="hljs-comment">// return false表示拦截，不向下执行</span>
        <span class="hljs-comment">// return true表示放行</span>
        <span class="hljs-keyword">return</span> <span class="hljs-keyword">false</span>;
    }
    <span class="hljs-comment">// 进入Handler方法之后，返回modelAndView之前执行</span>
    <span class="hljs-comment">// 应用场景从modelAndView出发：将公用的模型数据(比如菜单导航)在这里传到视图，也可以在这里统一指定视图</span>
    <span class="hljs-annotation">@Override</span>
    <span class="hljs-keyword">public</span> <span class="hljs-keyword">void</span> <span class="hljs-title">postHandle</span>(HttpServletRequest request, HttpServletResponse response, Object handler, ModelAndView modelAndView) <span class="hljs-keyword">throws</span> Exception
    {
        System.out.println(<span class="hljs-string">"HandlerInterceptor1...postHandle"</span>);
    }
    <span class="hljs-comment">// 执行Handler完成执行此方法</span>
    <span class="hljs-comment">// 应用场景：统一异常处理，统一日志处理</span>
    <span class="hljs-annotation">@Override</span>
    <span class="hljs-keyword">public</span> <span class="hljs-keyword">void</span> <span class="hljs-title">afterCompletion</span>(HttpServletRequest request, HttpServletResponse response, Object handler, Exception ex) <span class="hljs-keyword">throws</span> Exception
    {
        System.out.println(<span class="hljs-string">"HandlerInterceptor1...afterCompletion"</span>);
    }
}
</code><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li><li>20</li><li>21</li><li>22</li><li>23</li><li>24</li><li>25</li><li>26</li><li>27</li><li>28</li><li>29</li><li>30</li><li>31</li><li>32</li><li>33</li><li>34</li><li>35</li><li>36</li><li>37</li><li>38</li><li>39</li><li>40</li><li>41</li><li>42</li><li>43</li><li>44</li><li>45</li><li>46</li><li>47</li><li>48</li><li>49</li><li>50</li><li>51</li><li>52</li><li>53</li><li>54</li><li>55</li><li>56</li><li>57</li><li>58</li><li>59</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li><li>20</li><li>21</li><li>22</li><li>23</li><li>24</li><li>25</li><li>26</li><li>27</li><li>28</li><li>29</li><li>30</li><li>31</li><li>32</li><li>33</li><li>34</li><li>35</li><li>36</li><li>37</li><li>38</li><li>39</li><li>40</li><li>41</li><li>42</li><li>43</li><li>44</li><li>45</li><li>46</li><li>47</li><li>48</li><li>49</li><li>50</li><li>51</li><li>52</li><li>53</li><li>54</li><li>55</li><li>56</li><li>57</li><li>58</li><li>59</li></ul><code class="hljs xml has-numbering">    <span class="hljs-comment"><!--拦截器 --></span>
    <span class="hljs-tag"><<span class="hljs-title">mvc:interceptors</span>></span>
        <span class="hljs-comment"><!--多个拦截器,顺序执行 --></span>
        <span class="hljs-comment"><!-- 登录认证拦截器 --></span>
        <span class="hljs-tag"><<span class="hljs-title">mvc:interceptor</span>></span>
            <span class="hljs-tag"><<span class="hljs-title">mvc:mapping</span> <span class="hljs-attribute">path</span>=<span class="hljs-value">"/admin/**"</span> /></span>
            <span class="hljs-tag"><<span class="hljs-title">bean</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"com.jpkc.interceptor.LoginInterceptor"</span>></span><span class="hljs-tag"></<span class="hljs-title">bean</span>></span>
        <span class="hljs-tag"></<span class="hljs-title">mvc:interceptor</span>></span>
    <span class="hljs-tag"></<span class="hljs-title">mvc:interceptors</span>></span></code><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li></ul>
拦截器实现机制上，Struts2有自己的interceptor机制，SpringMVC用的是独立的AOP方式。都可以实现在前后进行拦截。
## **Struts2 和 SpringMVC 支持 JSON 的不同**
有时我们界面的一些操作，是通过 Ajax 调用后台的服务，获取服务器返回的 json 数据，进行后续的操作。
**Struts2 实现JSON数据返回的方式**
<code class="hljs xml has-numbering">        <span class="hljs-tag"><<span class="hljs-title">action</span> <span class="hljs-attribute">name</span>=<span class="hljs-value">"login_do"</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"com.jpkc.action.AccountAction"</span> <span class="hljs-attribute">method</span>=<span class="hljs-value">"login_do"</span>></span>
            <span class="hljs-tag"><<span class="hljs-title">result</span> <span class="hljs-attribute">name</span>=<span class="hljs-value">"success"</span> <span class="hljs-attribute">type</span>=<span class="hljs-value">"json"</span>></span>
                <span class="hljs-comment"><!-- 这里指定将被Struts2序列化的属性，该属性在action中必须有对应的getter方法 --></span>
                <span class="hljs-tag"><<span class="hljs-title">param</span> <span class="hljs-attribute">name</span>=<span class="hljs-value">"root"</span>></span>json<span class="hljs-tag"></<span class="hljs-title">param</span>></span>
            <span class="hljs-tag"></<span class="hljs-title">result</span>></span>
        <span class="hljs-tag"></<span class="hljs-title">action</span>></span></code><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li></ul><code class="hljs java has-numbering">
<span class="hljs-keyword">public</span> <span class="hljs-class"><span class="hljs-keyword">class</span> <span class="hljs-title">AccountAction</span> <span class="hljs-keyword">extends</span> <span class="hljs-title">BaseAction</span>
{</span>
    <span class="hljs-comment">// 常用变量</span>
    <span class="hljs-keyword">private</span> Map<String, Object> json;<span class="hljs-comment">// 返回到前台的map对象</span>
    <span class="hljs-keyword">private</span> Account account;
    <span class="hljs-keyword">public</span> <span class="hljs-title">AccountAction</span>()
    {
        json = <span class="hljs-keyword">new</span> HashMap<String, Object>();
    }
    <span class="hljs-keyword">public</span> String <span class="hljs-title">login_do</span>()
    {
        <span class="hljs-keyword">if</span> (account == <span class="hljs-keyword">null</span> || CM.validIsEmpty(account.getId()) || CM.validIsEmpty(account.getPassword()))
        {
            info = <span class="hljs-string">"用户名、密码都是必填项。"</span>;
            json.put(<span class="hljs-string">"success"</span>, <span class="hljs-keyword">false</span>);
            json.put(<span class="hljs-string">"info"</span>, info);
            <span class="hljs-keyword">return</span> SUCCESS;
        }
        getSession().setAttribute(<span class="hljs-string">"accountSession"</span>, account);
        json.put(<span class="hljs-string">"success"</span>, <span class="hljs-keyword">true</span>);
        <span class="hljs-keyword">return</span> SUCCESS;
    }
}</code><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li><li>20</li><li>21</li><li>22</li><li>23</li><li>24</li><li>25</li><li>26</li><li>27</li><li>28</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li><li>20</li><li>21</li><li>22</li><li>23</li><li>24</li><li>25</li><li>26</li><li>27</li><li>28</li></ul><code class="hljs javascript has-numbering">$.post(<span class="hljs-string">"login_do"</span>, $(<span class="hljs-string">".login-form"</span>).serialize(), <span class="hljs-function"><span class="hljs-keyword">function</span><span class="hljs-params">(json)</span>
{</span>
    <span class="hljs-keyword">if</span> (json.success == <span class="hljs-literal">true</span>)
    {
        window.location.href=<span class="hljs-string">"shop/index.jsp"</span>;
    }
    <span class="hljs-keyword">else</span>
    {
        alert(<span class="hljs-string">"操作失败："</span> + json.info);
    }
}, <span class="hljs-string">"json"</span>);</code><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li></ul>
**SpringMVC 实现JSON数据返回的方式**
<code class="hljs xml has-numbering">    <span class="hljs-comment"><!--注解适配器 --></span>
    <span class="hljs-tag"><<span class="hljs-title">bean</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"org.springframework.web.servlet.mvc.method.annotation.RequestMappingHandlerAdapter"</span>></span>
        <span class="hljs-tag"><<span class="hljs-title">property</span> <span class="hljs-attribute">name</span>=<span class="hljs-value">"messageConverters"</span>></span>
        <span class="hljs-tag"><<span class="hljs-title">list</span>></span>
        <span class="hljs-tag"><<span class="hljs-title">bean</span> <span class="hljs-attribute">class</span>=<span class="hljs-value">"org.springframework.http.converter.json.MappingJackson2HttpMessageConverter"</span>></span><span class="hljs-tag"></<span class="hljs-title">bean</span>></span>
        <span class="hljs-tag"></<span class="hljs-title">list</span>></span>
        <span class="hljs-tag"></<span class="hljs-title">property</span>></span>
    <span class="hljs-tag"></<span class="hljs-title">bean</span>></span>
</code><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li></ul><code class="hljs javascript has-numbering">$.post(<span class="hljs-string">"login_do"</span>, $(<span class="hljs-string">".login-form"</span>).serialize(), <span class="hljs-function"><span class="hljs-keyword">function</span><span class="hljs-params">(json)</span>
{</span>
    <span class="hljs-keyword">if</span> (json.success == <span class="hljs-literal">true</span>)
    {
        window.location.href=<span class="hljs-string">"shop/index.jsp"</span>;
    }
    <span class="hljs-keyword">else</span>
    {
        alert(<span class="hljs-string">"操作失败："</span> + json.info);
    }
}, <span class="hljs-string">"json"</span>);</code><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li></ul>
SpringMVC在控制器中返回json有两种方式。
一种是使用response返回json。
<code class="hljs avrasm has-numbering">    @RequestMapping(value = <span class="hljs-string">"/login_do"</span>, method = { RequestMethod<span class="hljs-preprocessor">.POST</span> })
    public void login_do(ModelAndView model, HttpServletRequest request, HttpServletResponse response, HttpSession session, Account account) throws Exception
    {
        response<span class="hljs-preprocessor">.setCharacterEncoding</span>(<span class="hljs-string">"utf-8"</span>)<span class="hljs-comment">;</span>
        response<span class="hljs-preprocessor">.setContentType</span>(<span class="hljs-string">"application/json;charset=utf-8"</span>)<span class="hljs-comment">;</span>
        Map<String, Object> json = new HashMap<String, Object>()<span class="hljs-comment">;</span>
        String info<span class="hljs-comment">;</span>
        if (account == null || CM<span class="hljs-preprocessor">.validIsEmpty</span>(account<span class="hljs-preprocessor">.getId</span>()) || CM<span class="hljs-preprocessor">.validIsEmpty</span>(account<span class="hljs-preprocessor">.getPassword</span>()))
        {
            info = <span class="hljs-string">"用户名、密码都是必填项。"</span><span class="hljs-comment">;</span>
            json<span class="hljs-preprocessor">.put</span>(<span class="hljs-string">"success"</span>, false)<span class="hljs-comment">;</span>
            json<span class="hljs-preprocessor">.put</span>(<span class="hljs-string">"info"</span>, info)<span class="hljs-comment">;</span>
            response<span class="hljs-preprocessor">.getWriter</span>()<span class="hljs-preprocessor">.write</span>(new ObjectMapper()<span class="hljs-preprocessor">.writeValueAsString</span>(json))<span class="hljs-comment">;</span>
            return<span class="hljs-comment">;</span>
        }
        session<span class="hljs-preprocessor">.setAttribute</span>(<span class="hljs-string">"accountSession"</span>, account)<span class="hljs-comment">;</span>
        json<span class="hljs-preprocessor">.put</span>(<span class="hljs-string">"success"</span>, true)<span class="hljs-comment">;</span>
        response<span class="hljs-preprocessor">.getWriter</span>()<span class="hljs-preprocessor">.write</span>(new ObjectMapper()<span class="hljs-preprocessor">.writeValueAsString</span>(json))<span class="hljs-comment">;</span>
        return<span class="hljs-comment">;</span>
    }</code><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li><li>20</li><li>21</li><li>22</li><li>23</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li><li>20</li><li>21</li><li>22</li><li>23</li></ul>
另一种是使用@ResponseBody注解方式。
<code class="hljs javascript has-numbering">    @RequestMapping(value = <span class="hljs-string">"/login_do"</span>, method = { RequestMethod.POST })
    public @ResponseBody Map<<span class="hljs-built_in">String</span>, <span class="hljs-built_in">Object</span>> login_do(ModelAndView model, HttpServletRequest request, HttpServletResponse response, HttpSession session, Account account) throws Exception
    {
        Map<<span class="hljs-built_in">String</span>, <span class="hljs-built_in">Object</span>> json = <span class="hljs-keyword">new</span> HashMap<<span class="hljs-built_in">String</span>, <span class="hljs-built_in">Object</span>>();
        <span class="hljs-built_in">String</span> info;
        <span class="hljs-keyword">if</span> (account == <span class="hljs-literal">null</span> || CM.validIsEmpty(account.getId()) || CM.validIsEmpty(account.getPassword()))
        {
            info = <span class="hljs-string">"用户名、密码都是必填项。"</span>;
            json.put(<span class="hljs-string">"success"</span>, <span class="hljs-literal">false</span>);
            json.put(<span class="hljs-string">"info"</span>, info);
            <span class="hljs-keyword">return</span> json;
        }
        session.setAttribute(<span class="hljs-string">"accountSession"</span>, account);
        json.put(<span class="hljs-string">"success"</span>, <span class="hljs-literal">true</span>);
        <span class="hljs-keyword">return</span> json;
    }</code><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li><li>15</li><li>16</li><li>17</li><li>18</li><li>19</li></ul>
可以看出，Struts2 和 SpringMVC 都可以实现 Ajax 请求返回 JSON。实现方式上，Struts2在配置文件配置返回类型为JSON。SpringMVC在方法上加一个@ResponseBody注解即可返回对应类型转成的JSON字符串。都是对返回数据转成JSON，但是不得不说SpringMVC的写法方便太多了。
## **Hibernate 和 MyBatis 在 ORM 侧重点的不同**
Hibernate对[数据库](http://lib.csdn.net/base/mysql)结构提供了较为完整的封装，Hibernate的O/R Mapping实现了POJO 和数据库表之间的映射，以及SQL 的自动生成和执行。程序员往往只需定义好了POJO
 到数据库表的映射关系，即可通过Hibernate 提供的方法完成持久层操作。程序员甚至不需要对SQL 的熟练掌握， **Hibernate/OJB 会根据指定的存储逻辑，自动生成对应的SQL** 并调用JDBC 接口加以执行。
**MyBatis 的着力点，则在于POJO 与SQL之间的映射关系**。然后通过映射配置文件，将SQL所需的参数，以及返回的结果字段映射到指定POJO。 相对Hibernate“O/R”而言，MyBatis 是一种“Sql Mapping”的ORM实现。
**SQL语句支持**：Hibernate可以完全不用手写SQL语句，MyBatis手动维护SQL语句。Hibernate修改优化SQL语句困难，MyBatis由于SQL语句自己控制，优化非常方便。
**开发速度**：Hibernate的真正掌握要比Mybatis来得难些。Mybatis框架相对简单很容易上手，但也相对简陋些。
**开发社区**：Hibernate 与Mybatis都是流行的持久层开发框架，但Hibernate开发社区相对多热闹些，支持的工具也多，更新也快。而Mybatis相对平静，工具较少。
**开发工作量**：Hibernate和MyBatis都有相应的代码生成工具。可以生成简单基本的DAO层方法。
针对高级查询，Mybatis需要手动编写SQL语句，以及ResultMap。而Hibernate有良好的映射机制，开发者无需关心SQL的生成与结果映射，可以更专注于业务流程。
## **Hibernate 和 MyBatis 在调优方面的不同**
- 制定合理的缓存策略；
- 尽量使用延迟加载特性；
- 采用合理的Session管理机制；
**SQL优化方面**
Hibernate的查询会将表中的所有字段查询出来，这一点会有性能消耗。Hibernate也可以自己写SQL来指定需要查询的字段，但这样就破坏了Hibernate开发的简洁性。而Mybatis的SQL是手动编写的，所以可以按需求指定查询的字段。
Hibernate HQL语句的调优需要将SQL打印出来，而Hibernate的SQL被很多人嫌弃因为太丑了。MyBatis的SQL是自己手动写的所以调整方便。但Hibernate具有自己的日志统计。Mybatis本身不带日志统计，使用Log4j进行日志记录。
**扩展性方面**
Hibernate与具体数据库的关联只需在XML文件中配置即可，所有的HQL语句与具体使用的数据库无关，移植性很好。MyBatis项目中所有的SQL语句都是依赖所用的数据库的，所以不同数据库类型的支持不好。
## **Hibernate 和 MyBatis 在对象管理与抓取策略的不同**
**对象管理**
Hibernate 是完整的对象/关系映射解决方案，它提供了对象状态管理（state management）的功能，使开发者不再需要理会底层数据库系统的细节。也就是说，相对于常见的 JDBC/SQL 持久层方案中需要管理 SQL 语句，Hibernate采用了更自然的面向对象的视角来持久化 Java 应用中的数据。
换句话说，使用 Hibernate 的开发者应该总是关注对象的状态（state），不必考虑 SQL 语句的执行。这部分细节已经由 Hibernate 掌管妥当，只有开发者在进行系统性能调优的时候才需要进行了解。
而MyBatis在这一块没有文档说明，用户需要对对象自己进行详细的管理。当调用sqlSession.commit()方法时才会进行真正的提交。
**抓取策略**
Hibernate对实体关联对象的抓取有着良好的机制。对于每一个关联关系都可以详细地设置是否延迟加载，并且提供关联抓取、查询抓取、子查询抓取、批量抓取四种模式。 它是详细配置和处理的。
而Mybatis的延迟加载是全局配置的，在resultMap中使用association中的select指定延迟加载去执行的statement的id。
<code class="hljs xml has-numbering">    <span class="hljs-comment"><!-- 延迟加载的resultMap --></span>
    <span class="hljs-tag"><<span class="hljs-title">resultMap</span> <span class="hljs-attribute">type</span>=<span class="hljs-value">"cn.itcast.mybatis.po.Orders"</span> <span class="hljs-attribute">id</span>=<span class="hljs-value">"OrdersUserLazyLoadingResultMap"</span>></span>
            <span class="hljs-comment"><!--对订单信息进行映射配置  --></span>
            <span class="hljs-tag"><<span class="hljs-title">id</span> <span class="hljs-attribute">column</span>=<span class="hljs-value">"id"</span> <span class="hljs-attribute">property</span>=<span class="hljs-value">"id"</span>/></span>
            <span class="hljs-tag"><<span class="hljs-title">result</span> <span class="hljs-attribute">column</span>=<span class="hljs-value">"user_id"</span> <span class="hljs-attribute">property</span>=<span class="hljs-value">"userId"</span>/></span>
            <span class="hljs-tag"><<span class="hljs-title">result</span> <span class="hljs-attribute">column</span>=<span class="hljs-value">"number"</span> <span class="hljs-attribute">property</span>=<span class="hljs-value">"number"</span>/></span>
            <span class="hljs-tag"><<span class="hljs-title">result</span> <span class="hljs-attribute">column</span>=<span class="hljs-value">"createtime"</span> <span class="hljs-attribute">property</span>=<span class="hljs-value">"createtime"</span>/></span>
            <span class="hljs-tag"><<span class="hljs-title">result</span> <span class="hljs-attribute">column</span>=<span class="hljs-value">"note"</span> <span class="hljs-attribute">property</span>=<span class="hljs-value">"note"</span>/></span>
            <span class="hljs-tag"><<span class="hljs-title">association</span> <span class="hljs-attribute">property</span>=<span class="hljs-value">"user"</span>  <span class="hljs-attribute">javaType</span>=<span class="hljs-value">"cn.itcast.mybatis.po.User"</span>
             <span class="hljs-attribute">select</span>=<span class="hljs-value">"cn.itcast.mybatis.mapper.UserMapper.findUserById"</span> <span class="hljs-attribute">column</span>=<span class="hljs-value">"user_id"</span>></span>
            <span class="hljs-comment"><!-- 实现对用户信息进行延迟加载 --></span>
            <span class="hljs-tag"></<span class="hljs-title">association</span>></span>
    <span class="hljs-tag"></<span class="hljs-title">resultMap</span>></span></code><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li></ul><ul style="" class="pre-numbering"><li>1</li><li>2</li><li>3</li><li>4</li><li>5</li><li>6</li><li>7</li><li>8</li><li>9</li><li>10</li><li>11</li><li>12</li><li>13</li><li>14</li></ul>
## **Hibernate 和 MyBatis 在缓存机制的不同**
**Hibernate缓存**
Hibernate一级缓存是Session缓存，利用好一级缓存就需要对Session的生命周期进行管理好。建议在一个Action操作中使用一个Session。一级缓存需要对Session进行严格管理。
Hibernate二级缓存是SessionFactory级的缓存。 SessionFactory的缓存分为内置缓存和外置缓存。内置缓存中存放的是SessionFactory对象的一些集合属性包含的数据(映射元素据及预定SQL语句等),对于应用程序来说,它是只读的。外置缓存中存放的是数据库数据的副本,其作用和一级缓存类似.二级缓存除了以内存作为存储介质外,还可以选用硬盘等外部存储设备。二级缓存称为进程级缓存或SessionFactory级缓存，它可以被所有session共享，它的生命周期伴随着SessionFactory的生命周期存在和消亡。
**MyBatis缓存**
MyBatis 包含一个非常强大的查询缓存特性,它可以非常方便地配置和定制。MyBatis 3 中的缓存实现的很多改进都已经实现了,使得它更加强大而且易于配置。
一级缓存是SqlSession级别的缓存，二级缓存是mapper(命名空间)级别的缓存，默认情况下是没有开启二级缓存的。
要开启二级缓存,你需要在你的 SQL 映射文件中添加一行: `<cache/>`
字面上看就是这样。这个简单语句的效果如下:
映射语句文件中的所有 select 语句将会被缓存。 
映射语句文件中的所有 insert,update 和 delete 语句会刷新缓存。 
缓存会使用 Least Recently Used(LRU,最近最少使用的)[算法](http://lib.csdn.net/base/datastructure)来收回。 
根据时间表(比如 no Flush Interval,没有刷新间隔), 缓存不会以任何时间顺序 来刷新。 
缓存会存储列表集合或对象(无论查询方法返回什么)的 1024 个引用。 
缓存会被视为是 read/write(可读/可写)的缓存,意味着对象检索不是共享的,而 且可以安全地被调用者修改,而不干扰其他调用者或线程所做的潜在修改。 
所有的这些属性都可以通过缓存元素的属性来修改。
比如: `<cache eviction="FIFO" flushInterval="60000" size="512" readOnly="true"/>`
这个更高级的配置创建了一个 FIFO 缓存,并每隔 60 秒刷新,存数结果对象或列表的 512 个引用,而且返回的对象被认为是只读的,因此在不同线程中的调用者之间修改它们会 导致冲突。可用的收回策略有, 默认的是 LRU:
LRU – 最近最少使用的:移除最长时间不被使用的对象。 
FIFO – 先进先出:按对象进入缓存的顺序来移除它们。 
SOFT – 软引用:移除基于垃圾回收器状态和软引用规则的对象。 
WEAK – 弱引用:更积极地移除基于垃圾收集器状态和弱引用规则的对象。 
flushInterval(刷新间隔)可以被设置为任意的正整数,而且它们代表一个合理的毫秒 形式的时间段。默认情况是不设置,也就是没有刷新间隔,缓存仅仅调用语句时刷新。
size(引用数目)可以被设置为任意正整数,要记住你缓存的对象数目和你运行环境的 可用内存资源数目。默认值是1024。
readOnly(只读)属性可以被设置为 true 或 false。只读的缓存会给所有调用者返回缓 存对象的相同实例。因此这些对象不能被修改。这提供了很重要的性能优势。可读写的缓存 会返回缓存对象的拷贝(通过序列化) 。这会慢一些,但是安全,因此默认是 false。
**相同点**
Hibernate和Mybatis的二级缓存除了采用系统默认的缓存机制外，都可以通过实现你自己的缓存或为其他第三方缓存方案，创建适配器来完全覆盖缓存行为。
**不同点**
Hibernate的二级缓存配置在SessionFactory生成的配置文件中进行详细配置，然后再在具体的表-对象映射中配置是那种缓存。
MyBatis的二级缓存配置都是在每个具体的表-对象映射中进行详细配置，这样针对不同的表可以自定义不同的缓存机制。并且Mybatis可以在命名空间中共享相同的缓存配置和实例，通过Cache-ref来实现。
**两者比较**
因为Hibernate对查询对象有着良好的管理机制，用户无需关心SQL。所以在使用二级缓存时如果出现脏数据，系统会报出错误并提示。
而MyBatis在这一方面，使用二级缓存时需要特别小心。如果不能完全确定数据更新操作的波及范围，避免Cache的盲目使用。否则，脏数据的出现会给系统的正常运行带来很大的隐患。
## **Hibernate 和 MyBatis 对比总结**
**两者相同点**
Hibernate与MyBatis都可以是通过SessionFactoryBuider由XML配置文件生成SessionFactory，然后由SessionFactory 生成Session，最后由Session来开启执行事务和SQL语句。其中SessionFactoryBuider，SessionFactory，Session的生命周期都是差不多的。
Hibernate和MyBatis都支持JDBC和JTA事务处理。
**Mybatis优势**
MyBatis可以进行更为细致的SQL优化，可以减少查询字段。
MyBatis容易掌握，而Hibernate门槛较高。
**Hibernate优势**
Hibernate的DAO层开发比MyBatis简单，Mybatis需要维护SQL和结果映射。
Hibernate对对象的维护和缓存要比MyBatis好，对增删改查的对象的维护要方便。
Hibernate数据库移植性很好，MyBatis的数据库移植性不好，不同的数据库需要写不同SQL。
Hibernate有更好的二级缓存机制，可以使用第三方缓存。MyBatis本身提供的缓存机制不佳，更新操作不能指定刷新指定记录，会清空整个表，但是也可以使用第三方缓存。
Hibernate 封装性好，屏蔽了数据库差异，自动生成SQL语句，应对数据库变化能力较弱，SQL语句优化困难。
MyBatis仅实现了SQL语句和对象的映射，需要针对具体的数据库写SQL语句，应对数据库变化能力较强，SQL语句优化较为方便。
## **SSH 和 SSM 对比总结**
SSH 和 SSM 的技术框架的不同只需要比较Struts2和SpringMVC的不同，以及Hibernate和MyBatis的不同。
对于不同的功能，两大技术阵营均有对应的解决方案。SSH将配置文件开发用到极致。SSM将注解开发用到极致。
企业进行技术选型，以低成本高回报作为技术选型的原则，根据项目组的技术力量来进行选择。
小弟水平有限，只能总结到这里。更进一步的底层代码级别的对比，才是本质的区别。用法上的区别只是表象而已，但是对于广大开发者来说，谁的开发者用户体验好，显然更能赢得开发者的青睐。
这里有一份Java工具和技术的调查，可以参考一下，[http://f.dataguru.cn/article-9978-1.html](http://f.dataguru.cn/article-9978-1.html)
## **参考文献**
[http://blog.csdn.net/firejuly/article/details/8190229](http://blog.csdn.net/firejuly/article/details/8190229)
[http://blog.csdn.net/chenyi0834/article/details/7334963](http://blog.csdn.net/chenyi0834/article/details/7334963)
[http://blog.csdn.net/gane_cheng/article/details/52787040](http://blog.csdn.net/gane_cheng/article/details/52787040)
[http://blog.csdn.net/gane_cheng/article/details/52759099](http://blog.csdn.net/gane_cheng/article/details/52759099)
[http://blog.csdn.net/gane_cheng/article/details/52751206](http://blog.csdn.net/gane_cheng/article/details/52751206)
[http://blog.csdn.net/chenleixing/article/details/44570681](http://blog.csdn.net/chenleixing/article/details/44570681)
