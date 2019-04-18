# SpringMVC在Controller层中注入request的坑 - z69183787的专栏 - CSDN博客
2017年12月20日 10:34:18[OkidoGreen](https://me.csdn.net/z69183787)阅读数：204
个人分类：[Spring-注入&注解&扫描																[SpringMvc-原理分析](https://blog.csdn.net/z69183787/article/category/6456963)](https://blog.csdn.net/z69183787/article/category/7066579)
[https://my.oschina.net/sluggarddd/blog/678603?fromerr=XhvpvVTi](https://my.oschina.net/sluggarddd/blog/678603?fromerr=XhvpvVTi)
摘要: 记一次为了节省代码没有在方法体中声明HttpServletRequest，而用autowire直接注入所钻的坑
###结论 给心急的人。 直接在Controller的成员变量上使用@Autowire声明HttpServletRequest，这是线程安全的！
```java
```java
@Controller
public class TestController{
    @Autowire
    HttpServletRequest request;
    @RequestMapping("/")
    public void test(){
        request.getAttribute("uid");   
    }
}
```
```
结论如上。 ###背景
是这样的，由于项目中我在Request的头部加入身份验证信息，而我在拦截器截获信息并且验证通过后，会将当前用户的身份加到request的Attribute中，方便在Controller层拿出来复用。
疑问：为什么不直接在Controller上使用@RequestHeader取出来呢？ 因为header里面是加密后的数据，且要经过一些复杂的身份验证判断，所以直接将这一步直接丢在了拦截器执行。
所以当解密后，我将用户信息（如uid）用`request.setAttribute()`设入request中在Controller提取。
而如果需要使用request，一般需要在方法上声明，如:
```cpp
```cpp
public Result save(HttpServletRequest request){
   // dosomething();
}
```
```
那么我每个方法都要用到uid的岂不是每个方法都要声明一个request参数，为了节省着个冗余步骤。我写了一个基类。
```java
```java
public class CommonController{
    @Autowire
    HttpServletReqeust request;
    public String getUid(){
        return (String)request.getAttribute("uid");
    }
}
```
```
后来我就担心，因为controller是单例的，这么写会不会导致后面的reqeust覆盖前面的request，在并发条件下有线程安全问题。 于是我就到segmentFault上提问，大部分网友说到，确实有线程问题！[segmentFault问题地址](https://segmentfault.com/q/1010000005139036?_ea=803262) ###验证过程 因为网友大部分的观点是只能在方法上声明，我自然不想就此放弃多写那么多代码，于是开始我的验证过程。
 热心的程序员们给我提供了好几种解决方案，我既然花力气证明了，就把结果放在这里，分享给大家。
方法1
第一个方法就是在controller的方法中显示声明HttpServletReqeust,代码如下：
```
@RequestMapping("/test")
@RestController
public class CTest {
    Logger logger = LoggerFactory.getLogger(getClass());
    @RequestMapping("/iiii")
    public String test(HttpServletRequest request) {
        logger.info(request.hashCode() + "");
        return null;
    }
}
```
在浏览器狂按F5
输出
![reqeust的hashCode](https://static.oschina.net/uploads/img/201605/20151703_PslI.png)
当时我是懵逼的，**说好的线程安全呢！**这特么不是同一个request吗！特么的在逗我！ 为此我还找了很久request是不是重写了hashcode()!
啊，事实是这样的，因为我用浏览器狂按F5,再怎么按他也是模拟不了并发的。那么就相当于，服务器一直在用同一个线程处理我的请求就足够了，至于这个request的hashcode，按照jdk的说法是根据obj在jvm的虚拟地址计算的，后面的事情是我猜的，如果有知道真正真想的还望告知！
猜测
服务器中每个thread所申请的request的内存空间在这个服务器启动的时候就是固定的，那么我每次请求，他都会在他所申请到的内存空间（可能是类似数组这样的结构）中新建一个request，（类似于数组的起点总是同一个内存地址），那么我发起一个请求，他就会在起始位置新建一个Request传递给Servlet并开始处理，处理结束后就会销毁，那么他下一个请求所新建的Request，因为之前的request销毁了，所以又从起始地址开始创建，这样一切就解释得通了！
猜测完毕
验证猜想：
我不让他有销毁的时间不就可以了吗 测试代码
```
@RequestMapping("/test")
@RestController
public class CTest {
    Logger logger = LoggerFactory.getLogger(getClass());
    @RequestMapping("/oooo")
    public String testA(HttpServletRequest request) throws Exception {
        Thread.sleep(3000);
        logger.info(request.hashCode() + "");
        logger.info(reqeust.getHeader("uid");
        return null;
    }
    @RequestMapping("/iiii")
    public String test(HttpServletRequest request) {
        logger.info(request.hashCode() + "");
        logger.info(reqeust.getHeader("uid");
        return null;
    }
}
```
如上，我在接口`/oooo`中休眠3秒，如果他是共用一个reqeust的话，那么后面的请求将覆盖这个休眠中的reqeust,所传入的uid即为接口地址。先发起`/oooo`后发起`/iiii`
输出
```
```
controller.CTest:33 - 364716268
controller.CTest:34 - iiii
controller.CTest:26 - 1892130707
controller.CTest:27 - oooo
```
```
结论： 1、后发起的`/iiii`没有覆盖前面`/oooo`的数据，没有线程安全问题。 2、request的hashcode不一样，因为`/oooo`的阻塞，导致另一个线程需要去处理，所以他新建了request，而不是向之前一样全部hashcode相同。
二轮验证
```java
```java
public class HttpTest {
    public static void main(String[] args) throws Exception {
        for (int i = 300; i > 0; i--) {
            final int finalI = i;
            new Thread() {
                @Override
                public void run() {
                    System.out.println("v###" + finalI);
                    HttpRequest.get("http://localhost:8080/test/iiii?").header("uid", "v###" + finalI).send();
                }
            }.start();
        }
    }
}
```
```
在模拟并发条件下，header中的uid300个完全接受，没有覆盖
所以这种方式，没有线程安全问题。
方法2
在CommonController中，使用`@ModelAttribute`处理。
```java
```java
public class CommonController  {
//    @Autowired
    protected HttpServletRequest request;
    @ModelAttribute
    public void bindreq(HttpServletRequest request) {
        this.request = request;
    }
    protected String getUid() {
        System.out.println(request.toString());
        return request.getAttribute("uid") == null ? null : (String) request.getAttribute("uid");
    }
}
```
```
这样子是有线程安全问题的！后面的request有可能覆盖掉之前的！
验证代码
```
@RestController
@RequestMapping("/test")
public class CTest extends CommonController {
    Logger logger = LoggerFactory.getLogger(getClass());
    @RequestMapping("/iiii")
    public String test() {
        logger.info(request.getHeader("uid"));
        return null;
    }
}
```
```java
```java
public class HttpTest {
    public static void main(String[] args) throws Exception {
        for (int i = 100; i > 0; i--) {
            final int finalI = i;
            new Thread() {
                @Override
                public void run() {
                    System.out.println("v###" + finalI);
                    HttpRequest.get("http://localhost:8080/test/iiii").header("uid", "v###" + finalI).send();
                }
            }.start();
        }
    }
}
```
```
截取了部分输出结果
```
```
controller.CTest:26 - v###52
controller.CTest:26 - v###13
controller.CTest:26 - v###57
controller.CTest:26 - v###57
controller.CTest:26 - v###21
controller.CTest:26 - v###10
controller.CTest:26 - v###82
controller.CTest:26 - v###82
controller.CTest:26 - v###93
controller.CTest:26 - v###71
controller.CTest:26 - v###71
controller.CTest:26 - v###85
controller.CTest:26 - v###85
controller.CTest:26 - v###14
controller.CTest:26 - v###47
controller.CTest:26 - v###47
controller.CTest:26 - v###69
controller.CTest:26 - v###22
controller.CTest:26 - v###55
controller.CTest:26 - v###61
```
```
可以看到57、71、85、47被覆盖了，丢失了部分request！
这么做是线程不安全的！
方法3
使用CommonController作为基类，将request Autowire。
```
public class CommonController {
    @Autowired
    protected HttpServletRequest request;
    protected String getUid() {
        System.out.println(request.toString());
        return request.getAttribute("uid") == null ? null : (String) request.getAttribute("uid");
    }
}
```
测试接口同上，结果喜人！ 100个request没有任何覆盖，我加大范围测了五六次，上千次请求没一个覆盖，可以证明这种写法没有线程安全问题了！
另外还有一点有趣的是，无论使用多少并发，request的hashcode始终是相同的，而且，测试同一个Controller中不同的接口，他也相同，使用sleep强行阻塞，hashcode也是相同。但是访问不同的controller，hashcode却是不同的，具体里面如何实现我也就没有继续深挖了。
但是结论是出来的，就如文章最开始所说一样。
