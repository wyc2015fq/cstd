# Spring Security教程外篇（1）---- AuthenticationException异常详解 - z69183787的专栏 - CSDN博客
2014年03月13日 21:24:09[OkidoGreen](https://me.csdn.net/z69183787)阅读数：8807
这个异常是在登录的时候出现错误时抛出的异常，比如账户锁定，证书失效等，先来看下AuthenticationException常用的的子类：
`UsernameNotFoundException 用户找不到`
`BadCredentialsException 坏的凭据`
AccountStatusException 用户状态异常它包含如下子类
AccountExpiredException 账户过期
LockedException账户锁定
DisabledException 账户不可用
CredentialsExpiredException 证书过期
常见的异常就这几个，还有很多不再一一赘述，仅仅为了展示，对后续的阅读没有什么影响
用户登录验证的过滤器是UsernamePasswordAuthenticationFilter，它继承自AbstractAuthenticationProcessingFilter。
今天研究的是看Spring如何处理AuthenticationException这个异常的，异常的处理一般都是doFilter中处理的，所以首先看AbstractAuthenticationProcessingFilter的doFilter中的代码
**[java]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18361421#)[copy](http://blog.csdn.net/jaune161/article/details/18361421#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/159430/fork)
- publicvoid doFilter(ServletRequest req, ServletResponse res, FilterChain chain)  
- throws IOException, ServletException {  
- 
- .......  
- 
- try {  
-            authResult = attemptAuthentication(request, response);  
- if (authResult == null) {  
- // return immediately as subclass has indicated that it hasn't completed authentication
- return;  
-            }  
-            sessionStrategy.onAuthentication(authResult, request, response);  
-        } catch(InternalAuthenticationServiceException failed) {  
-            logger.error("An internal error occurred while trying to authenticate the user.", failed);  
-            unsuccessfulAuthentication(request, response, failed);  
- 
- return;  
-        }catch (AuthenticationException failed) {  
- // Authentication failed
-            unsuccessfulAuthentication(request, response, failed);  
- 
- return;  
-        }  
- 
-        ......  
-    }  
从这段代码中我们看到Spring将异常捕获后交给了unsuccessfulAuthentication这个方法来处理（InternalAuthenticationServiceException也是AuthenticationException异常的子类，这是一个内部认证服务异常）
**[java]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18361421#)[copy](http://blog.csdn.net/jaune161/article/details/18361421#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/159430/fork)
- protectedvoid unsuccessfulAuthentication(HttpServletRequest request, HttpServletResponse response,  
-         AuthenticationException failed) throws IOException, ServletException {  
-     SecurityContextHolder.clearContext();  
- 
- if (logger.isDebugEnabled()) {  
-         logger.debug("Authentication request failed: " + failed.toString());  
-         logger.debug("Updated SecurityContextHolder to contain null Authentication");  
-         logger.debug("Delegating to authentication failure handler " + failureHandler);  
-     }  
- 
-     rememberMeServices.loginFail(request, response);  
- 
-     failureHandler.onAuthenticationFailure(request, response, failed);  
- }  
unsuccessfulAuthentication又交给了failureHandler(AuthenticationFailureHandler)来处理，然后追踪failureHandler
**[java]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18361421#)[copy](http://blog.csdn.net/jaune161/article/details/18361421#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/159430/fork)
- private AuthenticationFailureHandler failureHandler =   
- new SimpleUrlAuthenticationFailureHandler();  
发现最终是由SimpleUrlAuthenticationFailureHandler这个类的onAuthenticationFailure方法来处理的，打开这个类，有两个核心方法
**[java]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18361421#)[copy](http://blog.csdn.net/jaune161/article/details/18361421#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/159430/fork)
- publicvoid onAuthenticationFailure(HttpServletRequest request, HttpServletResponse response,  
-          AuthenticationException exception) throws IOException, ServletException {  
- 
- if (defaultFailureUrl == null) {  
-          logger.debug("No failure URL set, sending 401 Unauthorized error");  
- 
-          response.sendError(HttpServletResponse.SC_UNAUTHORIZED, "Authentication Failed: " + exception.getMessage());  
-      } else {  
-          saveException(request, exception);  
- 
- if (forwardToDestination) {  
-              logger.debug("Forwarding to " + defaultFailureUrl);  
- 
-              request.getRequestDispatcher(defaultFailureUrl).forward(request, response);  
-          } else {  
-              logger.debug("Redirecting to " + defaultFailureUrl);  
-              redirectStrategy.sendRedirect(request, response, defaultFailureUrl);  
-          }  
-      }  
-  }  
- 
- /**
-   * Caches the {@code AuthenticationException} for use in view rendering.
-   * <p>
-   * If {@code forwardToDestination} is set to true, request scope will be used, otherwise it will attempt to store
-   * the exception in the session. If there is no session and {@code allowSessionCreation} is {@code true} a session
-   * will be created. Otherwise the exception will not be stored.
-   */
- protectedfinalvoid saveException(HttpServletRequest request, AuthenticationException exception) {  
- if (forwardToDestination) {  
-          request.setAttribute(WebAttributes.AUTHENTICATION_EXCEPTION, exception);  
-      } else {  
-          HttpSession session = request.getSession(false);  
- 
- if (session != null || allowSessionCreation) {  
-              request.getSession().setAttribute(WebAttributes.AUTHENTICATION_EXCEPTION, exception);  
-          }  
-      }  
-  }  
从onAuthenticationFailure中我们可以看出，如果没有设置defaultFailureUrl就发出401错误。（SC_UNAUTHORIZED是一个常量值是401），如果设置了defaultFailureUrl，调用saveException方法后跳转到defaultFailureUrl页面。
forwardToDestination是是否是服务器跳转，默认使用重定向即客户端跳转。
saveException字面意思是保存异常信息，其实就是把异常信息写入request或者Session中。
如果使用服务器跳转则写入request，客户端跳转则写入Session。
而key是WebAttributes.AUTHENTICATION_EXCEPTION打开WebAttributes找到这个常量
**[java]**[view
 plain](http://blog.csdn.net/jaune161/article/details/18361421#)[copy](http://blog.csdn.net/jaune161/article/details/18361421#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/159430/fork)
- publicfinalclass WebAttributes {  
- 
- publicstaticfinal String ACCESS_DENIED_403 = "SPRING_SECURITY_403_EXCEPTION";  
- 
- publicstaticfinal String AUTHENTICATION_EXCEPTION = "SPRING_SECURITY_LAST_EXCEPTION";  
- 
- publicstaticfinal String WEB_INVOCATION_PRIVILEGE_EVALUATOR_ATTRIBUTE =   
-     WebAttributes.class.getName() + ".WEB_INVOCATION_PRIVILEGE_EVALUATOR_ATTRIBUTE";  
- }  
发现其值为SPRING_SECURITY_LAST_EXCEPTION，这样的话我们就可以在页面上通过el表达式来获取到这个异常了
注意：saveException保存的是Session对象所以直接使用 ${SPRING_SECURITY_LAST_EXCEPTION}是获取不到异常信息的，需要使用${SPRING_SECURITY_LAST_EXCEPTION.message}
