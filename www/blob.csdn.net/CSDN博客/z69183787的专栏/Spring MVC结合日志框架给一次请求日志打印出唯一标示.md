# Spring MVC结合日志框架给一次请求日志打印出唯一标示 - z69183787的专栏 - CSDN博客
2019年01月26日 20:30:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：302
[https://blog.csdn.net/helloworldwt/article/details/51818309](https://blog.csdn.net/helloworldwt/article/details/51818309)
**1.简介**
         在tomcat web项目中， 一次用户请求在同步情况下都是交给同一个线程去处理，如果能知道这个线程的处理路径，可以方便线上问题的排查、代码的调试。因此可以让同一次请求的线程日志带上同一个唯一的token，在查询日志时，grep token可以把相关的日志都查询出来。这时，如果已经在代码逻辑关键部分已经打印了日志，那么可以准确知道这次请求执行了哪些代码。
**2.日志框架原理简析**
         日志框架通常提供了一定的灵活性，以log4j和log4j2为例。
**3.实现**
**3.1 对日志框架进行拓展**
继承log4j的PatternParser类，并重写finalizeConverter方法，实现对自定义字符的解析，并ThreadTokenHelper.getThreadToken()返回自定义的token信息。
```java
public class Log4jExPatternParser extends PatternParser {
    public Log4jExPatternParser(String pattern) {
        super(pattern);
    }
 
    @Override
    protected void finalizeConverter(char c) {
        if (c == 'T') {
            this.addConverter(new ExPatternConverter(this.formattingInfo));
        } else {
            super.finalizeConverter(c);
        }
    }
 
    private static class ExPatternConverter extends PatternConverter {
        public ExPatternConverter(FormattingInfo fi) {
            super(fi);
        }
 
        @Override
        protected String convert(LoggingEvent event) {
            return String.valueOf(ThreadTokenHelper.getThreadToken());
        }
 
    }
}
```
 继承log4j的PatternLayout类，并重写createPatternParser方法。
```java
public class Log4jExPatternLayout extends PatternLayout {
    public Log4jExPatternLayout(String pattern){
        super(pattern);
    }
 
    public Log4jExPatternLayout(){
        super();
    }
 
    @Override
    protected PatternParser createPatternParser(String pattern) {
        return new Log4jExPatternParser(pattern);
    }
}
```
log4j配置文件的配置，PatternLayout使用自己定义的Log4jExPatternLayout。[%T]就是我们token信息的位置，通过上面的重写log解析，可以解析'T'这个字符。
```
<!-- console log-->
	<appender name="stdout" class="org.apache.log4j.ConsoleAppender">
		<param name="Target" value="System.out" />
		<layout class="com.my.extend.log.Log4jExPatternLayout">
			<param name="ConversionPattern" value="%d %p [%c] [%T] - (%m)%n" />
		</layout>
	</appender>
       <!-- root logger -->
	<root>
		<priority value="info" />
		<appender-ref ref="stdout" />
	</root>
```
**3.2 生成唯一token**
生成token算法，head与时间相关，精确到毫秒，body是线程号，foot是随机数。head模一天的毫秒数(86400000)，这样生成的token在某一天内，可以保证唯一性。
```java
private static String genThreadToken(){
        long head = System.currentTimeMillis()%86400000;
        long body = Thread.currentThread().getId();
        int foot = ThreadLocalRandom.current().nextInt(1000);
        String token = head + "_" + body + "_" + foot;
        ThreadContext.getContext().put("token", token);
        return token;
    }
```
   token的存储与清除，token存储在线程的threadlocal中，每一次请求结束清除线程当前的token值，下一次请求就会重新获取最新的token。如果请求发生异常，也需要清除token。
```java
public class ThreadContext {
 
    private static ThreadContext CONTEXT = new ThreadContext();
 
    private static ThreadLocal<Map<String, Object>> CONTEXT_LOCAL = new ThreadLocal<Map<String, Object>>() {
        @Override
        protected Map<String, Object> initialValue() {
            return new HashMap<String, Object>();
        }
    };
 
    private ThreadContext() {
    }
 
    public static ThreadContext getContext() {
        return CONTEXT;
    }
 
    public boolean put(String key, Object value) {
        CONTEXT_LOCAL.get().put(key, value);
        return true;
    }
 
    public Object get(String key) {
        return CONTEXT_LOCAL.get().get(key);
    }
 
    public Object remove(String key) {
        return CONTEXT_LOCAL.get().remove(key);
    }
 
    public void clear() {
        CONTEXT_LOCAL.get().clear();
    }
}
```
```java
public static String getThreadToken(){
        String token = (String) ThreadContext.getContext().get("token");
        if(token == null){
            return genThreadToken();
        }
        return token;
    }
 
    public static void clear(){
        ThreadContext.getContext().remove("token");
    }
```
**3.3 请求的处理**
通过Spring MVC拦截器intercepter，可以在一个请求前后做一些处理，通过把唯一token存储在线程中的ThreadLocal中，token能在同一个线程中进行传递。在这里主要需要，当一个请求结束后，把存储在当前ThreadLocal中的token清除，那么当下一个请求处理时，会重新获取新的token存储在ThreadLocal中。遇到异常抛出时，代码不会执行到这里，这时需要在异常处理的地方对线程ThreadLocal中的token进行清除。
         intercepter清除token，同时可以打印出请求访问的相关信息，访问用户， 参数，执行时间
```java
public class RequestLoggerHandler extends HandlerInterceptorAdapter {
    private static Logger LOGGER = LogManager.getLogger(RequestLoggerHandler.class);
 
    private ThreadLocal<Long> startTime = new ThreadLocal<>();
 
    @Override
    public boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object handler) throws Exception {
        String url = request.getRequestURI();
        if (DefaultConstant.DEFAULT_WELCOME_URL.equals(url)){
            return true;
        }
        startTime.set(System.currentTimeMillis());
        OpWorker user = new OpWorker(); //TODO
        AjaxRequest ajaxRequest = new AjaxRequest();
        LOGGER.info("[http]preHandle url: {"+request.getRequestURL()+"}, user:{"+JSON.toJSONString(user)+"}, httpBodyParams: {"+toJSONString(ajaxRequest.getParams())+"}, urlParams: {"+toJSONString(request.getQueryString())+"}" );
        return true;
    }
 
    @Override
    public void postHandle(HttpServletRequest request, HttpServletResponse response,
                           Object handler, ModelAndView modelAndView) throws Exception {
        String url = request.getRequestURI();
        if (DefaultConstant.DEFAULT_WELCOME_URL.equals(url)){
            return;
        }
 
        Long causeTime = System.currentTimeMillis() - startTime.get();
        LOGGER.info("[http]postHandle url: {"+request.getRequestURL()+"}, cause\t"+causeTime+"\tms");
        ThreadTokenHelper.clear();
    }
 
    private String toJSONString(Object object){
        try {
            if (object == null){
                return "";
            }
            return JSON.toJSONString(object);
        }catch (Exception e){
            LOGGER.warn("request请求参数序列化{}异常......");
            e.printStackTrace();
        }
        return null;
    }
}
```
 在Spring配置文件中的配置
```java
<mvc:interceptor>
    		<mvc:mapping path="/**/*" />
			<bean class="com.ttyc.ebike.op.backend.interceptor.RequestLoggerHandler" />
		</mvc:interceptor>
```
**3.4 日志示例**
下图红框内，就是一次请求唯一token信息，可以看到，两个token是相同的。通过grep 'token' log就可以把这次请求的所有日志都查询出来，可以追踪程序的运行，方便排查问题。
![](https://img-blog.csdn.net/20170301103807657?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaGVsbG93b3JsZHd0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**4.注意**
因为每一个请求都是绑定一个线程，并生成唯一token与此线程绑定，通过spring inteceptor在请求后清除此请求的token信息。如果发生异常时，会导致inteceptor不能在请求后清除token信息，会导致请求的token信息重复。这个时候需要在异常拦截的地方，手动清除下存储在线程token信息。
**4.1 异常情况**
因为每一个请求都是绑定一个线程，并生成唯一token与此线程绑定，通过spring inteceptor在请求后清除此请求的token信息。如果发生异常时，会导致inteceptor不能在请求后清除token信息，会导致请求的token信息重复。这个时候需要在异常拦截的地方，手动清除下存储在线程token信息。
**4.2 多线程处理**
有这样的场景，一个API在处理过程中，需要调用异步方法发送消息，在调用异步方法时就会有第二条线程来处理，这个时候，一个请求就会有两个token信息。通过查询一个token，只能得到部分日志。
**4.3 单线程处理多任务**
在使用消息队列的业务里，消费者可用使用单线程来处理多条消息，每一条消息是一个任务，应该在每一条消息处理后，手动清除下线程中的token信息，这样在处理下一条消息时，日志信息中会是新的token，从而不会与上一条消息的处理日志产生混淆。
