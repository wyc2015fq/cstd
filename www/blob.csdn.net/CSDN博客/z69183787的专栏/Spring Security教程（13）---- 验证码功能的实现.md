# Spring Security教程（13）---- 验证码功能的实现 - z69183787的专栏 - CSDN博客
2014年03月16日 08:16:32[OkidoGreen](https://me.csdn.net/z69183787)阅读数：4576
有三中方法可以实现验证码的功能
第一种是自定义一个filter，放在SpringSecurity过滤器之前，在用户登录的时候会先经过这个filter，然后在这个filter中实现对验证码进行验证的功能，这种方法不推荐，因为它已经脱离了SpringSecurity
第二种是自定义一个filter让它继承自UsernamePasswordAuthenticationFilter，然后重写attemptAuthentication方法在这个方法中实现验证码的功能，如果验证码错误就抛出一个继承自AuthenticationException的验证吗错误的异常比如（CaptchaException），然后这个异常就会被SpringSecurity捕获到并将异常信息返回到前台，这种实现起来比较简单
**[java]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18502265#)[copy](http://blog.csdn.net/jaune161/article/details/18502265#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/162872/fork)
- @Override
- public Authentication attemptAuthentication(HttpServletRequest request,  
-         HttpServletResponse response) throws AuthenticationException {  
- 
-     String requestCaptcha = request.getParameter(this.getCaptchaFieldName());  
-     String genCaptcha = (String)request.getSession().getAttribute("code");  
- 
-     logger.info("开始校验验证码，生成的验证码为："+genCaptcha+" ，输入的验证码为："+requestCaptcha);  
- 
- if( !genCaptcha.equals(requestCaptcha)){  
- thrownew CaptchaException(  
- this.messageSource.getMessage("AbstractUserDetailsAuthenticationProvider.badCaptcha",null,"Default",null));  
-     }  
- returnsuper.attemptAuthentication(request, response);  
- }  
然后在配置文件中配置下
**[html]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18502265#)[copy](http://blog.csdn.net/jaune161/article/details/18502265#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/162872/fork)
- <beanid="loginFilter"class="com.zrhis.system.security.DefaultUsernamePasswordAuthenticationFilter">
- <propertyname="authenticationManager"ref="authenticationManager"></property>
- <propertyname="authenticationSuccessHandler">
- <beanclass="org.springframework.security.web.authentication.SavedRequestAwareAuthenticationSuccessHandler">
- <propertyname="defaultTargetUrl"value="/index.jsp"></property>
- </bean>
- </property>
- <propertyname="authenticationFailureHandler">
- <beanclass="org.springframework.security.web.authentication.SimpleUrlAuthenticationFailureHandler">
- <propertyname="defaultFailureUrl"value="/login.jsp"></property>
- </bean>
- </property>
- </bean>
最后在http中加入custom-filter配置，将这个filter放在SpringSecurity的FORM_LOGIN_FILTER之前
**[html]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18502265#)[copy](http://blog.csdn.net/jaune161/article/details/18502265#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/162872/fork)
- <custom-filterref="loginFilter"before="FORM_LOGIN_FILTER"/>
最后一种是直接替换掉SpringSecurity的UsernamePasswordAuthenticationFilter，这种比较复杂，但是更为合理，也是我现在正在用的。
如果用这种方法那么http 中的auto-config就必须去掉，而form-login配置也必须去掉，因为这个不需要了，里面的属性都需要我们自行注入。
首先需要创建一个EntryPoint
**[html]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18502265#)[copy](http://blog.csdn.net/jaune161/article/details/18502265#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/162872/fork)
- <beanid="authenticationEntryPoint"class="org.springframework.security.web.authentication.LoginUrlAuthenticationEntryPoint">
- <propertyname="loginFormUrl"value="/login.jsp"/>
- </bean>
然后在http中配置下
**[html]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18502265#)[copy](http://blog.csdn.net/jaune161/article/details/18502265#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/162872/fork)
- <sec:httpaccess-decision-manager-ref="accessDecisionManager"
- entry-point-ref="authenticationEntryPoint">
然后我们来写CaptchaAuthenticationFilter，同样需要继承自UsernamePasswordAuthenticationFilter
**[java]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18502265#)[copy](http://blog.csdn.net/jaune161/article/details/18502265#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/162872/fork)
- publicclass CaptchaAuthenticationFilter extends UsernamePasswordAuthenticationFilter{  
- 
- publicstaticfinal String SPRING_SECURITY_FORM_CAPTCHA_KEY = "j_captcha";  
- publicstaticfinal String SESSION_GENERATED_CAPTCHA_KEY = Constant.SESSION_GENERATED_CAPTCHA_KEY;  
- 
- private String captchaParameter = SPRING_SECURITY_FORM_CAPTCHA_KEY;  
- 
- public Authentication attemptAuthentication(HttpServletRequest request,  
-             HttpServletResponse response) throws AuthenticationException {  
- 
-         String genCode = this.obtainGeneratedCaptcha(request);  
-         String inputCode = this.obtainCaptcha(request);  
- if(genCode == null)  
- thrownew CaptchaException(this.messages.getMessage("LoginAuthentication.captchaInvalid"));  
- if(!genCode.equalsIgnoreCase(inputCode)){  
- thrownew CaptchaException(this.messages.getMessage("LoginAuthentication.captchaNotEquals"));  
-         }  
- 
- returnsuper.attemptAuthentication(request, response);  
-     }  
- 
- protected String obtainCaptcha(HttpServletRequest request){  
- return request.getParameter(this.captchaParameter);  
-     }  
- 
- protected String obtainGeneratedCaptcha (HttpServletRequest request){  
- return (String)request.getSession().getAttribute(SESSION_GENERATED_CAPTCHA_KEY);  
-     }  
- 
- }  
在配置文件中配置CaptchaAuthenticationFilter
**[html]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18502265#)[copy](http://blog.csdn.net/jaune161/article/details/18502265#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/162872/fork)
- <beanid="captchaAuthenticaionFilter"class="com.zrhis.system.security.CaptchaAuthenticationFilter">
- <propertyname="authenticationManager"ref="authenticationManager"/>
- <propertyname="authenticationFailureHandler"ref="authenticationFailureHandler"/>
- <propertyname="authenticationSuccessHandler"ref="authenticationSuccessHandler"/>
- <propertyname="filterProcessesUrl"value="/login.do"/>
- </bean>
- 
- <beanid="authenticationSuccessHandler"class="com.zrhis.system.security.SimpleLoginSuccessHandler">
- <propertyname="defaultTargetUrl"value="/WEB-INF/app.jsp"></property>
- <propertyname="forwardToDestination"value="true"></property>
- </bean>
- <beanid="authenticationFailureHandler"class="org.springframework.security.web.authentication.SimpleUrlAuthenticationFailureHandler">
- <propertyname="defaultFailureUrl"value="/login.jsp"/>
- </bean>
从配置文件中就可以看出来authenticationManager、authenticationFailureHandler、authenticationSuccessHandler、filterProcessesUrl等都需要我们自行注入了。
filterProcessesUrl定义的是登录验证的地址，默认的是j_spring_security_check这里我们改成login.do
authenticationSuccessHandler中的defaultTargetUrl定义的是登录成功后跳转到的页面
authenticationFailureHandler中的defaultTargetUrl定义的是登录失败后跳转到的页面
我们的首页app.jsp在/WEB-INF下所以需要使用服务器跳转，所以需要将forwardToDestination设为true，因为客户端跳转是不能直接访问WEB-INF下的内容的。
最后在http中将FORM_LOGIN_FILTER替换掉，最终http中完整的配置就变成了下面的内容
**[html]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18502265#)[copy](http://blog.csdn.net/jaune161/article/details/18502265#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/162872/fork)
- <sec:httpaccess-decision-manager-ref="accessDecisionManager"
- entry-point-ref="authenticationEntryPoint">
- 
- <sec:access-denied-handlerref="accessDeniedHandler"/>
- 
- <sec:session-managementinvalid-session-url="/login.jsp"/>
- 
- <sec:custom-filterref="filterSecurityInterceptor"before="FILTER_SECURITY_INTERCEPTOR"/>
- <sec:custom-filterref="captchaAuthenticaionFilter"position="FORM_LOGIN_FILTER"/>
- </sec:http>
custom-filter中before是在这个filter之前，after是之后，position是替换。
这样就可以实现对验证码的验证了，效果如下
![](https://img-blog.csdn.net/20140119135725484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamF1bmUxNjE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
