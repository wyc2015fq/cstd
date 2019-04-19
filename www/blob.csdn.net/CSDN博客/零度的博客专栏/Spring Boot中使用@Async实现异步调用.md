# Spring Boot中使用@Async实现异步调用 - 零度的博客专栏 - CSDN博客
2018年12月19日 14:36:08[零度anngle](https://me.csdn.net/zmx729618)阅读数：327
什么是“异步调用”？
“异步调用”对应的是“同步调用”，同步调用指程序按照定义顺序依次执行，每一行程序都必须等待上一行程序执行完成之后才能执行；异步调用指程序在顺序执行时，不等待异步调用的语句返回结果就执行后面的程序。
同步调用
下面通过一个简单示例来直观的理解什么是同步调用：
- 定义Task类，创建三个处理函数分别模拟三个执行任务的操作，操作消耗时间随机取（10秒内）
|123456789101112131415161718192021222324252627282930|@Component**public****class****Task** {**public****static** Random random =**new** Random();**public****void****doTaskOne**() **throws** Exception {System.out.println("开始做任务一");**long** start = System.currentTimeMillis();Thread.sleep(random.nextInt(10000));**long** end = System.currentTimeMillis();System.out.println("完成任务一，耗时：" + (end - start) + "毫秒");}**public****void****doTaskTwo**() **throws** Exception {System.out.println("开始做任务二");**long** start = System.currentTimeMillis();Thread.sleep(random.nextInt(10000));**long** end = System.currentTimeMillis();System.out.println("完成任务二，耗时：" + (end - start) + "毫秒");}**public****void****doTaskThree**() **throws** Exception {System.out.println("开始做任务三");**long** start = System.currentTimeMillis();Thread.sleep(random.nextInt(10000));**long** end = System.currentTimeMillis();System.out.println("完成任务三，耗时：" + (end - start) + "毫秒");}}|
- 在单元测试用例中，注入Task对象，并在测试用例中执行`doTaskOne`、`doTaskTwo`、`doTaskThree`三个函数。
|123456789101112131415|@RunWith(SpringJUnit4ClassRunner.class)@SpringApplicationConfiguration(classes = Application.class)**public****class****ApplicationTests** {@Autowired**private** Task task;@Test**public****void****test**() **throws** Exception {task.doTaskOne();task.doTaskTwo();task.doTaskThree();}}|
- 执行单元测试，可以看到类似如下输出：
|123456|开始做任务一完成任务一，耗时：4256毫秒开始做任务二完成任务二，耗时：4957毫秒开始做任务三完成任务三，耗时：7173毫秒|
任务一、任务二、任务三顺序的执行完了，换言之`doTaskOne`、`doTaskTwo`、`doTaskThree`三个函数顺序的执行完成。
异步调用
上述的同步调用虽然顺利的执行完了三个任务，但是可以看到执行时间比较长，若这三个任务本身之间不存在依赖关系，可以并发执行的话，同步调用在执行效率方面就比较差，可以考虑通过异步调用的方式来并发执行。
在Spring Boot中，我们只需要通过使用`@Async`注解就能简单的将原来的同步函数变为异步函数，Task类改在为如下模式：
|12345678910111213141516171819|@Component**public****class****Task** {@Async**public****void****doTaskOne**() **throws** Exception {// 同上内容，省略}@Async**public****void****doTaskTwo**() **throws** Exception {// 同上内容，省略}@Async**public****void****doTaskThree**() **throws** Exception {// 同上内容，省略}}|
为了让@Async注解能够生效，还需要在Spring Boot的主程序中配置@EnableAsync，如下所示：
|123456789|@SpringBootApplication@EnableAsync**public****class****Application** {**public****static****void****main**(String[] args) {SpringApplication.run(Application.class, args);}}|
此时可以反复执行单元测试，您可能会遇到各种不同的结果，比如：
- 没有任何任务相关的输出
- 有部分任务相关的输出
- 乱序的任务相关的输出
原因是目前`doTaskOne`、`doTaskTwo`、`doTaskThree`三个函数的时候已经是异步执行了。主程序在异步调用之后，主程序并不会理会这三个函数是否执行完成了，由于没有其他需要执行的内容，所以程序就自动结束了，导致了不完整或是没有输出任务相关内容的情况。
注： @Async所修饰的函数不要定义为static类型，这样异步调用不会生效
异步回调
为了让`doTaskOne`、`doTaskTwo`、`doTaskThree`能正常结束，假设我们需要统计一下三个任务并发执行共耗时多少，这就需要等到上述三个函数都完成调动之后记录时间，并计算结果。
那么我们如何判断上述三个异步调用是否已经执行完成呢？我们需要使用`Future<T>`来返回异步调用的结果，就像如下方式改造`doTaskOne`函数：
|123456789|@Async**public** Future<String> **doTaskOne**() **throws** Exception {System.out.println("开始做任务一");**long** start = System.currentTimeMillis();Thread.sleep(random.nextInt(10000));**long** end = System.currentTimeMillis();System.out.println("完成任务一，耗时：" + (end - start) + "毫秒");**return****new** AsyncResult<>("任务一完成");}|
按照如上方式改造一下其他两个异步函数之后，下面我们改造一下测试用例，让测试在等待完成三个异步调用之后来做一些其他事情。
|12345678910111213141516171819202122|@Test**public****void****test**() **throws** Exception {**long** start = System.currentTimeMillis();Future<String> task1 = task.doTaskOne();Future<String> task2 = task.doTaskTwo();Future<String> task3 = task.doTaskThree();**while**(**true**) {**if**(task1.isDone() && task2.isDone() && task3.isDone()) {// 三个任务都调用完成，退出循环等待**break**;}Thread.sleep(1000);}**long** end = System.currentTimeMillis();System.out.println("任务全部完成，总耗时：" + (end - start) + "毫秒");}|
看看我们做了哪些改变：
- 在测试用例一开始记录开始时间
- 在调用三个异步函数的时候，返回`Future<String>`类型的结果对象
- 在调用完三个异步函数之后，开启一个循环，根据返回的`Future<String>`对象来判断三个异步函数是否都结束了。若都结束，就结束循环；若没有都结束，就等1秒后再判断。
- 跳出循环之后，根据结束时间 - 开始时间，计算出三个任务并发执行的总耗时。
执行一下上述的单元测试，可以看到如下结果：
|1234567|开始做任务一开始做任务二开始做任务三完成任务三，耗时：37毫秒完成任务二，耗时：3661毫秒完成任务一，耗时：7149毫秒任务全部完成，总耗时：8025毫秒|
可以看到，通过异步调用，让任务一、二、三并发执行，有效的减少了程序的总运行时间。
#  Spring Boot中使用@Scheduled创建定时任务
我们在编写Spring Boot应用中经常会遇到这样的场景，比如：我需要定时地发送一些短信、邮件之类的操作，也可能会定时地检查和监控一些标志、参数等。
##  创建定时任务
在Spring Boot中编写定时任务是非常简单的事，下面通过实例介绍如何在Spring Boot中创建定时任务，实现每过5秒输出一下当前时间。
- 在Spring Boot的主类中加入`@EnableScheduling`注解，启用定时任务的配置
|12345678910|@SpringBootApplication@EnableScheduling**public****class****Application** {**public****static****void****main**(String[] args) {SpringApplication.run(Application.class, args);}}|
- 创建定时任务实现类
|1234567891011|@Component**public****class****ScheduledTasks** {**private****static****final** SimpleDateFormat dateFormat = **new** SimpleDateFormat("HH:mm:ss");@Scheduled(fixedRate = 5000)**public****void****reportCurrentTime**() {System.out.println("现在时间：" + dateFormat.format(**new** Date()));}}|
- 运行程序，控制台中可以看到类似如下输出，定时任务开始正常运作了。
- 
`2016-05-15 10:40:04.073 INFO 1688 --- [ main] com.didispace.Application : Started Application in 1.433 seconds (JVM running for 1.967)`
- 
`现在时间：10:40:09`
- 
`现在时间：10:40:14`
- 
`现在时间：10:40:19`
- 
`现在时间：10:40:24`
- 
`现在时间：10:40:29522`
- 
`现在时间：10:40:34`
关于上述的简单入门示例也可以参见官方的[Scheduling Tasks](http://spring.io/guides/gs/scheduling-tasks/)
##  @Scheduled详解
在上面的入门例子中，使用了`@Scheduled(fixedRate = 5000)` 注解来定义每过5秒执行的任务，对于`@Scheduled`的使用可以总结如下几种方式：
- `@Scheduled(fixedRate = 5000)` ：上一次开始执行时间点之后5秒再执行
- `@Scheduled(fixedDelay = 5000)` ：上一次执行完毕时间点之后5秒再执行
- `@Scheduled(initialDelay=1000, fixedRate=5000)` ：第一次延迟1秒后执行，之后按fixedRate的规则每5秒执行一次
- `@Scheduled(cron="*/5 * * * * *")` ：通过cron表达式定义规则
#  Spring Boot属性配置文件详解
相信很多人选择Spring Boot主要是考虑到它既能兼顾Spring的强大功能，还能实现快速开发的便捷。我们在Spring Boot使用过程中，最直观的感受就是没有了原来自己整合Spring应用时繁多的XML配置内容，替代它的是在`pom.xml`中引入模块化的`Starter POMs`，其中各个模块都有自己的默认配置，所以如果不是特殊应用场景，就只需要在`application.properties`中完成一些属性配置就能开启各模块的应用。
在之前的各篇文章中都有提及关于`application.properties`的使用，主要用来配置数据库连接、日志相关配置等。除了这些配置内容之外，本文将具体介绍一些在`application.properties`配置中的其他特性和使用方法。
##  自定义属性与加载
我们在使用Spring Boot的时候，通常也需要定义一些自己使用的属性，我们可以如下方式直接定义：
|12|com.didispace.blog.name=程序猿DDcom.didispace.blog.title=Spring Boot教程|
然后通过`@Value("${属性名}")`注解来加载对应的配置属性，具体如下：
|1234567891011|@Component**public****class****BlogProperties** {@Value("${com.didispace.blog.name}")**private** String name;@Value("${com.didispace.blog.title}")**private** String title;// 省略getter和setter}|
按照惯例，通过单元测试来验证BlogProperties中的属性是否已经根据配置文件加载了。
|123456789101112131415|@RunWith(SpringJUnit4ClassRunner.class)@SpringApplicationConfiguration(Application.class)**public****class****ApplicationTests** {@Autowired**private** BlogProperties blogProperties;@Test**public****void****getHello**() **throws** Exception {Assert.assertEquals(blogProperties.getName(), "程序猿DD");Assert.assertEquals(blogProperties.getTitle(), "Spring Boot教程");}}|
##  参数间的引用
在`application.properties`中的各个参数之间也可以直接引用来使用，就像下面的设置：
|123|com.didispace.blog.name=程序猿DDcom.didispace.blog.title=Spring Boot教程com.didispace.blog.desc=${com.didispace.blog.name}正在努力写《${com.didispace.blog.title}》|
`com.didispace.blog.desc`参数引用了上文中定义的`name`和`title`属性，最后该属性的值就是`程序猿DD正在努力写《Spring Boot教程》`。
##  使用随机数
在一些情况下，有些参数我们需要希望它不是一个固定的值，比如密钥、服务端口等。Spring Boot的属性配置文件中可以通过`${random}`来产生int值、long值或者string字符串，来支持属性的随机值。
|12345678910|# 随机字符串com.didispace.blog.value=${random.value}# 随机intcom.didispace.blog.number=${random.int}# 随机longcom.didispace.blog.bignumber=${random.long}# 10以内的随机数com.didispace.blog.test1=${random.int(10)}# 10-20的随机数com.didispace.blog.test2=${random.int[10,20]}|
##  通过命令行设置属性值
相信使用过一段时间Spring Boot的用户，一定知道这条命令：`java -jar xxx.jar --server.port=8888`，通过使用–server.port属性来设置xxx.jar应用的端口为8888。
在命令行运行时，连续的两个减号`--`就是对`application.properties`中的属性值进行赋值的标识。所以，`java -jar xxx.jar --server.port=8888`命令，等价于我们在`application.properties`中添加属性`server.port=8888`，该设置在样例工程中可见，读者可通过删除该值或使用命令行来设置该值来验证。
通过命令行来修改属性值固然提供了不错的便利性，但是通过命令行就能更改应用运行的参数，那岂不是很不安全？是的，所以Spring Boot也贴心的提供了屏蔽命令行访问属性的设置，只需要这句设置就能屏蔽：`SpringApplication.setAddCommandLineProperties(false)`。
##  多环境配置
我们在开发Spring Boot应用时，通常同一套程序会被应用和安装到几个不同的环境，比如：开发、测试、生产等。其中每个环境的数据库地址、服务器端口等等配置都会不同，如果在为不同环境打包时都要频繁修改配置文件的话，那必将是个非常繁琐且容易发生错误的事。
对于多环境的配置，各种项目构建工具或是框架的基本思路是一致的，通过配置多份不同环境的配置文件，再通过打包命令指定需要打包的内容之后进行区分打包，Spring Boot也不例外，或者说更加简单。
在Spring Boot中多环境配置文件名需要满足`application-{profile}.properties`的格式，其中`{profile}`对应你的环境标识，比如：
- `application-dev.properties`：开发环境
- `application-test.properties`：测试环境
- `application-prod.properties`：生产环境
至于哪个具体的配置文件会被加载，需要在`application.properties`文件中通过`spring.profiles.active`属性来设置，其值对应`{profile}`值。
如：`spring.profiles.active=test`就会加载`application-test.properties`配置文件内容
下面，以不同环境配置不同的服务端口为例，进行样例实验。
- 
针对各环境新建不同的配置文件`application-dev.properties`、`application-test.properties`、`application-prod.properties`
- 
在这三个文件均都设置不同的`server.port`属性，如：dev环境设置为1111，test环境设置为2222，prod环境设置为3333
- 
application.properties中设置`spring.profiles.active=dev`，就是说默认以dev环境设置
- 
测试不同配置的加载
- 执行`java -jar xxx.jar`，可以观察到服务端口被设置为`1111`，也就是默认的开发环境（dev）
- 执行`java -jar xxx.jar --spring.profiles.active=test`，可以观察到服务端口被设置为`2222`，也就是测试环境的配置（test）
- 执行`java -jar xxx.jar --spring.profiles.active=prod`，可以观察到服务端口被设置为`3333`，也就是生产环境的配置（prod）
按照上面的实验，可以如下总结多环境的配置思路：
- `application.properties`中配置通用内容，并设置`spring.profiles.active=dev`，以开发环境为默认配置
- `application-{profile}.properties`中配置各个环境不同的内容
- 通过命令行方式去激活不同环境的配置
#  Spring Boot中Web应用的统一异常处理
我们在做Web应用的时候，请求处理过程中发生错误是非常常见的情况。Spring Boot提供了一个默认的映射：`/error`，当处理中抛出异常之后，会转到该请求中处理，并且该请求有一个全局的错误页面用来展示异常内容。
选择一个之前实现过的Web应用（[Chapter3-1-2](http://git.oschina.net/didispace/SpringBoot-Learning/tree/master/Chapter3-1-2)）为基础，启动该应用，访问一个不存在的URL，或是修改处理内容，直接抛出异常，如：
|1234|@RequestMapping("/hello")**public** String **hello**() **throws** Exception {**throw****new** Exception("发生错误");}|
此时，可以看到类似下面的报错页面，该页面就是Spring Boot提供的默认error映射页面。
![alt=默认的错误页面](https://img-blog.csdnimg.cn/2018121914360939)alt=默认的错误页面
##  统一异常处理
虽然，Spring Boot中实现了默认的error映射，但是在实际应用中，上面你的错误页面对用户来说并不够友好，我们通常需要去实现我们自己的异常提示。
下面我们以之前的Web应用例子为基础（[Chapter3-1-2](http://git.oschina.net/didispace/SpringBoot-Learning/tree/master/Chapter3-1-2)），进行统一异常处理的改造。
- 创建全局异常处理类：通过使用`@ControllerAdvice`定义统一的异常处理类，而不是在每个Controller中逐个定义。`@ExceptionHandler`用来定义函数针对的异常类型，最后将Exception对象和请求URL映射到`error.html`中
|123456789101112131415|@ControllerAdvice**class****GlobalExceptionHandler** {**public****static****final** String DEFAULT_ERROR_VIEW = "error";@ExceptionHandler(value = Exception.class)**public** ModelAndView **defaultErrorHandler**(HttpServletRequest req, Exception e) **throws** Exception {ModelAndView mav = **new** ModelAndView();mav.addObject("exception", e);mav.addObject("url", req.getRequestURL());mav.setViewName(DEFAULT_ERROR_VIEW);**return** mav;}}|
- 实现`error.html`页面展示：在`templates`目录下创建`error.html`，将请求的URL和Exception对象的message输出。
|123456789101112|<!DOCTYPE html><html><head lang="en"><meta charset="UTF-8" /><title>统一异常处理</title></head><body><h1>Error Handler</h1><div th:text="${url}"></div><div th:text="${exception.message}"></div></body></html>|
启动该应用，访问：`http://localhost:8080/hello`，可以看到如下错误提示页面。
![alt=自定义的错误页面](https://img-blog.csdnimg.cn/2018121914360968)alt=自定义的错误页面
通过实现上述内容之后，我们只需要在`Controller`中抛出`Exception`，当然我们可能会有多种不同的`Exception`。然后在`@ControllerAdvice`类中，根据抛出的具体`Exception`类型匹配`@ExceptionHandler`中配置的异常类型来匹配错误映射和处理。
##  返回JSON格式
在上述例子中，通过`@ControllerAdvice`统一定义不同Exception映射到不同错误处理页面。而当我们要实现RESTful API时，返回的错误是JSON格式的数据，而不是HTML页面，这时候我们也能轻松支持。
本质上，只需在`@ExceptionHandler`之后加入`@ResponseBody`，就能让处理函数return的内容转换为JSON格式。
下面以一个具体示例来实现返回JSON格式的异常处理。
- 创建统一的JSON返回对象，code：消息类型，message：消息内容，url：请求的url，data：请求返回的数据
|12345678910111213|**public****class****ErrorInfo**<**T**> {**public****static****final** Integer OK = 0;**public****static****final** Integer ERROR = 100;**private** Integer code;**private** String message;**private** String url;**private** T data;// 省略getter和setter}|
- 创建一个自定义异常，用来实验捕获该异常，并返回json
|1234567|**public****class****MyException****extends****Exception** {**public****MyException**(String message) {**super**(message);}}|
- `Controller`中增加json映射，抛出`MyException`异常
|123456789|@Controller**public****class****HelloController** {@RequestMapping("/json")**public** String **json**() **throws** MyException {**throw****new** MyException("发生错误2");}}|
- 为`MyException`异常创建对应的处理
|123456789101112131415|@ControllerAdvice**public****class****GlobalExceptionHandler** {@ExceptionHandler(value = MyException.class)@ResponseBody**public** ErrorInfo<String> **jsonErrorHandler**(HttpServletRequest req, MyException e) **throws** Exception {ErrorInfo<String> r = **new** ErrorInfo<>();r.setMessage(e.getMessage());r.setCode(ErrorInfo.ERROR);r.setData("Some Data");r.setUrl(req.getRequestURL().toString());**return** r;}}|
- 启动应用，访问：[http://localhost:8080/json，可以得到如下返回内容：](http://localhost:8080/json%EF%BC%8C%E5%8F%AF%E4%BB%A5%E5%BE%97%E5%88%B0%E5%A6%82%E4%B8%8B%E8%BF%94%E5%9B%9E%E5%86%85%E5%AE%B9%EF%BC%9A)
|123456|{code: 100，data: "Some Data"，message: "发生错误2"，url: "http://localhost:8080/json"}|
至此，已完成在Spring Boot中创建统一的异常处理，实际实现还是依靠Spring MVC的注解，更多更深入的使用可参考Spring MVC的文档。
