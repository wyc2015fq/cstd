# springMVC之mvc:interceptors拦截器的用法 - z69183787的专栏 - CSDN博客
2018年01月31日 11:47:53[OkidoGreen](https://me.csdn.net/z69183787)阅读数：346
[http://blog.csdn.net/tong_xinglong/article/details/52035131](http://blog.csdn.net/tong_xinglong/article/details/52035131)
1.配置拦截器
在springMVC.xml配置文件增加：
    <mvc:interceptors>
   <!-- 日志拦截器 -->
   <mvc:interceptor>
    <mvc:mapping path="/**" />
    <mvc:exclude-mapping path="/static/**" />
    <bean class="拦截器java代码路径" />
   </mvc:interceptor>
 </mvc:interceptors>
说明：
1）mvc:mapping 拦截器路径配置
2）mvc:exclude-mapping 拦截器不需要拦截的路径
2.参考代码
public class LogsInterceptor extends HandlerInterceptorAdapter {
    private static final Logger logger = LoggerFactory.getLogger(LogsInterceptor.class);
    
    private  NamedThreadLocal<String> logContext = new NamedThreadLocal<String>("log-id");
    @Autowired
    private TLogDao logDao;
    /**
     * preHandle方法是进行处理器拦截用的，顾名思义，该方法将在Controller处理之前进行调用，
     * SpringMVC中的Interceptor拦截器是链式的，可以同时存在多个Interceptor，
     * 然后SpringMVC会根据声明的前后顺序一个接一个的执行，
     * 而且所有的Interceptor中的preHandle方法都会在Controller方法调用之前调用。
     * SpringMVC的这种Interceptor链式结构也是可以进行中断的，
     * 这种中断方式是令preHandle的返回值为false，当preHandle的返回值为false的时候整个请求就结束了。
     */
    @Override
    public boolean preHandle(HttpServletRequest request, HttpServletResponse response, Object handler) throws Exception {
        String host = request.getRemoteHost();
        String url = request.getRequestURI();
        TLogEntity entity = new TLogEntity();
        entity.setCreateTime(new Timestamp(System.currentTimeMillis()));
        entity.setCreateUser("admin");
        entity.setIpAddress(host);
        entity.setLogUrl(url);
        entity.setIsSuccess("N");
        logDao.save(entity);
        logContext.set(entity.getLogId());
        logger.debug("IP为---->>> " + host + " <<<-----访问了系统");
        return true;
    }
    /**
     * 这个方法只会在当前这个Interceptor的preHandle方法返回值为true的时候才会执行。
     * postHandle是进行处理器拦截用的，它的执行时间是在处理器进行处理之 后， 也就是在Controller的方法调用之后执行，
     * 但是它会在DispatcherServlet进行视图的渲染之前执行，也就是说在这个方法中你可以对ModelAndView进行操作。
     * 这个方法的链式结构跟正常访问的方向是相反的，也就是说先声明的Interceptor拦截器该方法反而会后调用，
     * 这跟Struts2里面的拦截器的执行过程有点像，
     * 只是Struts2里面的intercept方法中要手动的调用ActionInvocation的invoke方法，
     * Struts2中调用ActionInvocation的invoke方法就是调用下一个Interceptor或者是调用action，
     * 然后要在Interceptor之前调用的内容都写在调用invoke之前，要在Interceptor之后调用的内容都写在调用invoke方法之后。
     */
    @Override
    public void postHandle(HttpServletRequest request, HttpServletResponse response, Object handler, ModelAndView modelAndView) throws Exception {
    }
    /**
     * 该方法也是需要当前对应的Interceptor的preHandle方法的返回值为true时才会执行。
     * 该方法将在整个请求完成之后，也就是DispatcherServlet渲染了视图执行， 这个方法的主要作用是用于清理资源的，
     */
    @Override
    public void afterCompletion(HttpServletRequest request, HttpServletResponse response, Object handler, Exception ex) {
        String host = request.getRemoteHost();
        String logId = logContext.get();
        TLogEntity entity = logDao.findOne(logId);
        entity.setIsSuccess("Y");
        logDao.save(entity);
        logger.debug("IP为---->>> " + host + " <<<-----访问成功");
    }
}
