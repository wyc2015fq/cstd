# 使用Spring MVC的@ControllerAdvice注解做Json的异常处理 - z69183787的专栏 - CSDN博客
2016年08月23日 13:21:25[OkidoGreen](https://me.csdn.net/z69183787)阅读数：6743
一，本文介绍Spring MVC的自定义异常处理，即在Controller中抛出自定义的异常时，客户端收到更友好的JSON格式的提示。而不是常见的报错页面。
二，场景描述：实现公用API，验证API key的逻辑，放在拦截器中判断（等同于在Controller中）并抛出异常，用户收到类似下图的提示：
其中，Http状态Code也能自由控制。
三，解决方案：
1，在***RateLimitInterceptor.java***拦截器中抛出异常：
**[java]**[view
 plain](http://blog.csdn.net/cesul/article/details/38539343#)[copy](http://blog.csdn.net/cesul/article/details/38539343#)
- publicclass RateLimitInterceptor extends HandlerInterceptorAdapter{  
- 
- @Autowiredprivate RedisService rs;  
- 
- /**
-      * 流量控制检查入口
-      */
- @Override
- publicboolean preHandle(HttpServletRequest request,  
-             HttpServletResponse response, Object handler) throws RequiredParameterException, SignException, RateLimitException,Exception {  
- super.preHandle(request, response, handler);  
-         String appKey = request.getParameter("appKey");  
- //判断appKey是否为空或是否合法
- if(appKey == null){  
- thrownew RequiredParameterException("");  
-         }elseif(AppKeyUtils.isFormatCorrect(appKey) || !rs.isExist(appKey)){  
- 
- thrownew SignException();  
- 
-         }else {  
- try {  
-                 AppCall appCall = AppCall.create(appKey, AppKeyUtils.getPlanDetails(appKey));  
-                 appCall.decrease();  
-                 rs.save(appCall);  
-                 System.out.println("RateLimitInterceptor pass.");  
-             } catch (RateLimitException e) {  
- //抛出超限异常
- thrownew RateLimitException();  
-             }  
-         }  
- returntrue;  
-     }  
- 
- }  
当代码走到（具体怎样走到，需考虑具体业务逻辑，上述代码使用AppCall类来封装，这是后话）
**[java]**[view
 plain](http://blog.csdn.net/cesul/article/details/38539343#)[copy](http://blog.csdn.net/cesul/article/details/38539343#)
- thrownew SignException();  
时，Spring将自动捕获这个异常。然后做一些处理。这是正常的流程。那么Spring如何自动不火
2，使用Spring MVC的@ControllerAdvice，在***GlobalExceptionHandler.java***类中实现全局的异常处理类：
**[java]**[view
 plain](http://blog.csdn.net/cesul/article/details/38539343#)[copy](http://blog.csdn.net/cesul/article/details/38539343#)
- @ControllerAdvice
- publicclass GlobalExceptionHandler {  
- 
- @ExceptionHandler(SQLException.class)  
- @ResponseStatus(HttpStatus.INTERNAL_SERVER_ERROR)  
- @ResponseBody
- public ExceptionResponse handleSQLException(HttpServletRequest request, Exception ex) {  
-         String message = ex.getMessage();  
- return ExceptionResponse.create(HttpStatus.INTERNAL_SERVER_ERROR.value(), message);  
-     }  
- 
- @ResponseStatus(value=HttpStatus.NOT_FOUND, reason="IOException occured")  
- @ExceptionHandler(IOException.class)  
- @ResponseBody
- publicvoid handleIOException(){  
- //returning 404 error code
-     }  
- 
- @ResponseStatus(HttpStatus.BAD_REQUEST)  
- @ResponseBody
- @ExceptionHandler(SignException.class)  
- public ExceptionResponse signException(SignException ex) {  
- return ex.getEr();  
-     }  
- 
- }  
在方法的头上注解：
**[java]**[view
 plain](http://blog.csdn.net/cesul/article/details/38539343#)[copy](http://blog.csdn.net/cesul/article/details/38539343#)
- @ExceptionHandler(SignException.class)  
即表示让Spring捕获到所有抛出的SignException异常，并交由这个被注解的方法处理。
注解：
**[java]**[view
 plain](http://blog.csdn.net/cesul/article/details/38539343#)[copy](http://blog.csdn.net/cesul/article/details/38539343#)
- @ResponseBody
即表示返回的对象，Spring会自动把该对象进行json转化，最后写入到Response中。
注解：
**[java]**[view
 plain](http://blog.csdn.net/cesul/article/details/38539343#)[copy](http://blog.csdn.net/cesul/article/details/38539343#)
- @ResponseStatus(HttpStatus.BAD_REQUEST)  
表示设置状态码。如果应用级别的错误，此处其实永远返回200也是可以接受的，但是要在你自定义的异常串和异常码中做好交代。
本文的方案自定义了一个*ExceptionResponse.java*类，如下：
**[java]**[view
 plain](http://blog.csdn.net/cesul/article/details/38539343#)[copy](http://blog.csdn.net/cesul/article/details/38539343#)
- /**
-  * 返回的json数据
-  * @author craig
-  *
-  */
- publicclass ExceptionResponse {  
- 
- private String message;  
- private Integer code;  
- 
- /**
-      * Construction Method
-      * @param code
-      * @param message
-      */
- public ExceptionResponse(Integer code, String message){  
- this.message = message;  
- this.code = code;  
-     }  
- 
- publicstatic ExceptionResponse create(Integer code, String message){  
- returnnew ExceptionResponse(code, message);  
-     }  
- 
- public Integer getCode() {  
- return code;  
-     }  
- public String getMessage() {  
- return message;  
-     }  
- 
- }  
如你所知，这个类就是最后传到用户面前的一个异常类，code和message根据业务定义并让用户知晓。而自定义的*SignException.java*实际上起到了一个桥梁的作用。Spring把**SignException**对象捕获到，转成相应的*ExceptionResponse*对象，剩下的就是如何优雅实现的问题了。 如下是***SignException.java***的实现:
**[java]**[view
 plain](http://blog.csdn.net/cesul/article/details/38539343#)[copy](http://blog.csdn.net/cesul/article/details/38539343#)
- /**
-  * 签名异常
-  * @author tuxiao.czz
-  *
-  */
- publicclass SignException extends Exception {  
- 
- privatestaticfinallong serialVersionUID = 4714113994147018010L;  
- private String message = "AppKey is not correct, please check.";  
- private Integer code = 10002;  
- 
- private ExceptionResponse er;  
- 
- public SignException() {  
-         er = ExceptionResponse.create(code, message);  
-     }  
- 
- public ExceptionResponse getEr() {  
- return er;  
-     }  
- 
- }  
所有关于这个异常的code和message都写在这个类里，个人感觉还是可以接受。当然还有另外一种实现，就是只拦截、定义一种Exception类，然后传不同的code和message进去，然后做相应的处理。这些都是比较灵活的。
以上便是实现“自定义异常json化处理”的相关代码和说明。
