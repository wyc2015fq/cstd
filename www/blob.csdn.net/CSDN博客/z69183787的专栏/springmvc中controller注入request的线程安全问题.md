# springmvc中controller注入request的线程安全问题 - z69183787的专栏 - CSDN博客
2017年12月20日 11:45:22[OkidoGreen](https://me.csdn.net/z69183787)阅读数：537
[http://m.blog.csdn.net/forlovedoit/article/details/53204667](http://m.blog.csdn.net/forlovedoit/article/details/53204667)
# 本文全部转载于http://www.cnblogs.com/developer-ios/p/5918999.html
# [SpringMvc学习心得（四）springmvc中request的线程安全问题](http://blog.csdn.net/csluanbin/article/details/50930138)
    servlet是单例的，而tomcat则是在多个线程中调用servlet的处理方法。因此如果servlet存在实例对象，那么就会引出线程安全的问题。而springmvc允许在controller类中通过@Autowired配置request、response以及requestcontext等实例对象。这种配置方法是否线程安全？答案是——这种配置方法是线程安全的，request、response以及requestcontext在使用时不需要进行同步。而根据[spring](http://lib.csdn.net/base/17)的默认规则，controller对于beanfactory而言是单例的。即controller只有一个，controller中的request等实例对象也只有一个。然而tomcat依旧会以多线程的方式访问controller。这种做法似乎并不能保证线程安全。我们如何理解这一矛盾？
   在解释controller线程安全这一问题之前需要首先了解如下的一些问题和概念：
   1.servlet的request域的问题：request域是javaweb的基础概念，他指的是从发起http请求到返回响应的这一段时间内，存在一个httprequest对象对应于http请求。以上的表述是没有问题的，然而有些人“自作主张”的将之前的表述换成了其他的描述方式：(1)：request对象的生命周期以发起http请求开始，当http请求返回时结束；(2)：用户发送一个请求的时候，request被创建，当用户关闭请求的时候，request会消亡。以上两种表述的主要错误在于混淆了http请求和request对象这两个概念。tomcat在接收到http请求的时候并不会创建一个request对象，即request对象并不是一个http请求的实例。只是request对象“恰巧”拥有了http请求中的所有参数而已。request对象在tomcat发起处理线程的时候就被创建，只有当处理线程终止的时候request才会被销毁。我们可以创建一个servlet类，并在doget和dopost方法上面打上断点。你会发现如果是同一个进程，即便发起多次访问，request对象的id始终不变。读者可以亲自尝试，用以验证本人说法的真伪。
   2.Threadlocal类：该对象包含两个关键函数：set(Object obj)和get()。这两个函数与调用该函数的线程相关，set方法将某一对象“注入”到当前线程中，而get方法则是从当前线程中获取对象。
   3.InvocationHandler接口：这是springmvc保证request对象线程安全的核心。通过实现该接口，开发者能够在[Java](http://lib.csdn.net/base/17)对象方法执行时进行干预，搭配Threadlocal就能够实现线程安全。
  下面将通过例子介绍springmvc如何保证request对象线程安全：
   Httprequest接口：
- public interface HttpRequest {  
- public void service();  
- 
}  
   HttpRequestImpl类：对httprequest接口的具体实现，为了区别不同的HttpRequestImpl对象，本人为HttpRequestImpl设置了一个Double对象，如果不设置该对象，其默认为null
- public class HttpRequestImpl implements HttpRequest{  
- public Double d;  
- @Override  
- public void service() {  
- 
        System.out.println("do some serivce, random value is "+d);  
- 
    }  
- 
- 
}  
   ThreadLocalTest类：负责向ThreadLocal设置对象和获取对象，本人设置ThreadLocal对象为static，因此ThreadLocalTest类中只能有一个ThreadLocal对象。
- public class ThreadLocalTest {  
- public static ThreadLocal<HttpRequest> local=new ThreadLocal<HttpRequest>();  
- public static void set(HttpRequest f){  
- if(get()==null){  
- 
            System.out.println("ThreadLocal is null");  
- 
            local.set(f);  
- 
        }  
- 
    }  
- public static HttpRequest get(){  
- return local.get();  
- 
    }  
- 
}  
   Factory类：该类是一个工厂类并且是单例模式，主要负责向ThreadLocalTest对象中设置和获取对象
- public class Factory{  
- private static Factory factory=new Factory();  
- private Factory(){  
- 
- 
    }  
- public static Factory getInstance(){  
- return factory;  
- 
    }  
- public HttpRequest getObject(){  
- return (HttpRequest)ThreadLocalTest.get();  
- 
    }  
- public void setObject(HttpRequest request){  
- 
        ThreadLocalTest.set(request);  
- 
    }  
- 
}  
  Delegate类：该类实现了InvocationHandler接口，并实现了invoke方法
- import java.lang.reflect.InvocationHandler;  
- import java.lang.reflect.Method;  
- import java.lang.reflect.Proxy;  
- 
- 
- public class Delegate implements InvocationHandler{  
- private Factory factory;  
- 
- public Factory getFactory() {  
- return factory;  
- 
    }  
- 
- public void setFactory(Factory factory) {  
- this.factory = factory;  
- 
    }  
- 
- @Override  
- public Object invoke(Object proxy, Method method, Object[] args)  
- throws Throwable {  
- return method.invoke(this.factory.getObject(), args);  
- 
    }  
- 
- 
}  
   ProxyUtils类：该类是一个工具类，负责生成一个httprequest对象的代理
- import java.lang.reflect.Proxy;  
- 
- 
- public class ProxyUtils {  
- public static HttpRequest getRequest(){  
- 
        HttpRequest request=new HttpRequestImpl();  
- 
        Delegate delegate=new Delegate();  
- 
        delegate.setFactory(Factory.getInstance());  
- 
        HttpRequest proxy=(HttpRequest) Proxy.newProxyInstance(request.getClass().getClassLoader(), request.getClass().getInterfaces(), delegate);  
- return proxy;  
- 
    }  
- 
}  
   TestThread类：该类用来模拟多线程调用controller的情况，类中拥有一个静态对象request。
- public class TestThread implements Runnable{  
- private static HttpRequest request;  
- public void init(){  
- 
        HttpRequestImpl requestimpl=new HttpRequestImpl();  
- 
        requestimpl.d=Math.random();  
- 
        Factory.getInstance().setObject(requestimpl);  
- 
    }  
- @Override  
- public void run() {  
- 
        System.out.println("*********************");  
- 
        init();  
- 
        request.service();  
- 
        System.out.println("*********************");  
- 
    }  
- public static HttpRequest getRequest() {  
- return request;  
- 
    }  
- public static void setRequest(HttpRequest request) {  
- 
        TestThread.request = request;  
- 
    }  
- 
- 
}  
   main：测试类
- public class main {  
- 
- /** 
-      * @param args 
-      */  
- public static void main(String[] args) {  
- 
        HttpRequest request=ProxyUtils.getRequest();  
- 
- 
        TestThread thread1=new TestThread();  
- 
        thread1.setRequest(request);  
- 
- 
        TestThread thread2=new TestThread();  
- 
        thread2.setRequest(request);  
- 
- 
        Thread t1=new Thread(thread1);  
- 
        Thread t2=new Thread(thread2);  
- 
- 
        t1.start();  
- 
        t2.start();  
- 
    }  
- 
}  
   thread1和thread2设置了同一个request对象，正常来说这两个对象调用run方法时输出的随机值应该为null(因为设置给这两个对象的request并没有设置d的值)。然而事实上这两个线程在调用时不但输出了随机值而且随机值还各不相同。这是因为request对象设置了代理，当调用request对象的service方法时，代理对象会从Threadlocal中获取实际的request对象以替代调用当前的request对象。由于httprequest对象在处理线程中保持不变，因此controller通过调用httprequest对象的方法能够获取当前请求的参数。
   以上都是一家之言，下面将通过展现springmvc源码的形式证明以上的说法：
   ObjectFactoryDelegatingInvocationHandler类：该类是AutowireUtils的一个私有类，该类拦截了除了equals、hashcode以及toString以外的其他方法，其中的objectFactory是RequestObjectFactory实例。
- private static class ObjectFactoryDelegatingInvocationHandler implements InvocationHandler, Serializable {  
- 
- private final ObjectFactory objectFactory;  
- 
- public ObjectFactoryDelegatingInvocationHandler(ObjectFactory objectFactory) {  
- this.objectFactory = objectFactory;  
- 
        }  
- 
- public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {  
- 
            String methodName = method.getName();  
- if (methodName.equals("equals")) {  
- // Only consider equal when proxies are identical.  
- return (proxy == args[0]);  
- 
            }  
- else if (methodName.equals("hashCode")) {  
- // Use hashCode of proxy.  
- return System.identityHashCode(proxy);  
- 
            }  
- else if (methodName.equals("toString")) {  
- return this.objectFactory.toString();  
- 
            }  
- try {  
- return method.invoke(this.objectFactory.getObject(), args);  
- 
            }  
- catch (InvocationTargetException ex) {  
- throw ex.getTargetException();  
- 
            }  
- 
        }  
- 
    }  
   RequestObjectFactory类：其中currentReuqestAttributes负责从Threadlocal中获取对象
- private static class RequestObjectFactory implements ObjectFactory<ServletRequest>, Serializable {  
- 
- public ServletRequest getObject() {  
- return currentRequestAttributes().getRequest();  
- 
        }  
- 
- @Override  
- public String toString() {  
- return "Current HttpServletRequest";  
- 
        }  
- 
    }  
  既然需要从Threadlocal中获取对象，那springmvc在何时向Threadlocal设置了该对象呢？分别在如下两个类中完成：RequestContextListener和FrameworkServlet。RequestContextListener负责监听servletcontext，当servletcontext启动时，RequestContextListener向Threadlocal设置了httprequest对象。FrameworkServlet是DispatchServlet的基类，tomcat会在运行过程中启动新的线程，而该线程中并没有httprequest对象。因此servlet会在每次处理http请求的时候检验当前的Threadlocal中是否有httprequest对象，如果没有则设置该对象。
   FrameworkServlet通过布尔值previousRequestAttributes检验httprequest是否存在的代码：
- protected final void processRequest(HttpServletRequest request, HttpServletResponse response)  
- throws ServletException, IOException {  
- 
- long startTime = System.currentTimeMillis();  
- 
        Throwable failureCause = null;  
- 
- // Expose current LocaleResolver and request as LocaleContext.  
- 
        LocaleContext previousLocaleContext = LocaleContextHolder.getLocaleContext();  
- 
        LocaleContextHolder.setLocaleContext(buildLocaleContext(request), this.threadContextInheritable);  
- 
- // Expose current RequestAttributes to current thread.  
- 
        RequestAttributes previousRequestAttributes = RequestContextHolder.getRequestAttributes();  
- 
        ServletRequestAttributes requestAttributes = null;  
- if (previousRequestAttributes == null || previousRequestAttributes.getClass().equals(ServletRequestAttributes.class)) {  
- 
            requestAttributes = new ServletRequestAttributes(request);  
- 
            RequestContextHolder.setRequestAttributes(requestAttributes, this.threadContextInheritable);  
- 
        }  
- 
- if (logger.isTraceEnabled()) {  
- 
            logger.trace("Bound request context to thread: " + request);  
- 
        }  
- 
- try {  
- 
            doService(request, response);  
- 
        }  
- catch (ServletException ex) {  
- 
            failureCause = ex;  
- throw ex;  
- 
        }  
- catch (IOException ex) {  
- 
            failureCause = ex;  
- throw ex;  
- 
        }  
- catch (Throwable ex) {  
- 
            failureCause = ex;  
- throw new NestedServletException("Request processing failed", ex);  
- 
        }  
- 
- finally {  
- // Clear request attributes and reset thread-bound context.  
- 
            LocaleContextHolder.setLocaleContext(previousLocaleContext, this.threadContextInheritable);  
- if (requestAttributes != null) {  
- 
                RequestContextHolder.setRequestAttributes(previousRequestAttributes, this.threadContextInheritable);  
- 
                requestAttributes.requestCompleted();  
- 
            }  
- if (logger.isTraceEnabled()) {  
- 
                logger.trace("Cleared thread-bound request context: " + request);  
- 
            }  
- 
- if (logger.isDebugEnabled()) {  
- if (failureCause != null) {  
- this.logger.debug("Could not complete request", failureCause);  
- 
                }  
- else {  
- this.logger.debug("Successfully completed request");  
- 
                }  
- 
            }  
- if (this.publishEvents) {  
- // Whether or not we succeeded, publish an event.  
- long processingTime = System.currentTimeMillis() - startTime;  
- this.webApplicationContext.publishEvent(  
- new ServletRequestHandledEvent(this,  
- 
                                request.getRequestURI(), request.getRemoteAddr(),  
- 
                                request.getMethod(), getServletConfig().getServletName(),  
- 
                                WebUtils.getSessionId(request), getUsernameForRequest(request),  
- 
                                processingTime, failureCause));  
- 
            }  
- 
        }  
- 
    }  
   RequestContextListener在context初始化时通过requestInitialized函数向Threadlocal设置httprequest对象的代码：
- public void requestInitialized(ServletRequestEvent requestEvent) {  
- if (!(requestEvent.getServletRequest() instanceof HttpServletRequest)) {  
- throw new IllegalArgumentException(  
- "Request is not an HttpServletRequest: " + requestEvent.getServletRequest());  
- 
        }  
- 
        HttpServletRequest request = (HttpServletRequest) requestEvent.getServletRequest();  
- 
        ServletRequestAttributes attributes = new ServletRequestAttributes(request);  
- 
        request.setAttribute(REQUEST_ATTRIBUTES_ATTRIBUTE, attributes);  
- 
        LocaleContextHolder.setLocale(request.getLocale());  
- 
        RequestContextHolder.setRequestAttributes(attributes);  
- 
    }  
