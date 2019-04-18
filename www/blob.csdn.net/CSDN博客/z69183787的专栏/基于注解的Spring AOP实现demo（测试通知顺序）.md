# 基于注解的Spring AOP实现demo（测试通知顺序） - z69183787的专栏 - CSDN博客
2017年06月23日 11:15:47[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1802
[http://www.jianshu.com/p/66d21dae6a68](http://www.jianshu.com/p/66d21dae6a68)
为了理解AOP，我们必须先了解AOP的相关术语，很简单不难:
通知（Advice）：在AOP中，切面的工作被称为通知。通知定义了切面“是什么”以及“何时”使用。除了描述切面要完成的工作，通知还解决了何时执行这个工作的问题。
Spring切面可以应用5种类型的通知：
- 前置通知（Before）：在目标方法被调用之前调用通知功能
- 后置通知（After）：在目标方法完成之后调用通知，此时不会关心方法的输出是什么
- 返回通知（After-returning）：在目标方法成功执行之后调用通知
- 异常通知（After-throwing）：在目标方法抛出异常后调用通知
- 环绕通知（Around）：通知包裹了被通知的方法，在被通知的方法调用之前和调用之后执行自定义的行为
运行顺序:前置通知--目标方法执行--返回通知/异常通知--后置通知
PS: 环绕通知也可以称作自定义通知,这是最强大的一种通知类型，环绕通知可以在方法调用前后完成自定义的行为，它也会选择是否继续执行连接点或直接返回它们自己的返回值或抛出异常来结束执行
连接点（Join point）：连接点是在应用执行过程中能够插入切面的一个点。这个点可以是调用方法时、抛出异常时、甚至修改一个字段时。切面代码可以利用这些点插入到应用的正常流程之中，并添加行为。
切点（Pointcut）:如果说通知定义了切面“是什么”和“何时”的话，那么切点就定义了“何处”。比如我想把日志引入到某个具体的方法中，这个方法就是所谓的切点。
切面（Aspect）：切面是通知和切点的结合。通知和切点共同定义了切面的全部内容———他是什么，在何时和何处完成其功能。
- 准备工作 : 该demo运行在 [http://www.jianshu.com/p/8fcefc4d724b](http://www.jianshu.com/p/8fcefc4d724b) 环境中
- 开启AspectJ自动扫描,因为spring默认是关闭的
![](http://upload-images.jianshu.io/upload_images/1723854-dde90e26240238c5.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
- 
定义切面类 AspectjLog
```
package com.sam.aop.aspectj;
import org.aspectj.lang.JoinPoint;
import org.aspectj.lang.ProceedingJoinPoint;
import org.aspectj.lang.annotation.*;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.core.annotation.Order;
import org.springframework.stereotype.Component;
/**
* 切面类
* Created by sam on 16/6/1.
*/
@Order(1)
@Aspect
@Component
public class AspectjLog {
  private Logger logger = LoggerFactory.getLogger(AspectjLog.class);
  //此处定义一个通用的切点,以便下方4个通知使用
  @Pointcut("execution(* com.sam.aop.service.AspectService.*(..))")
  public void logAop() {
  }
  // 如果没有定义公用的切点或者需要单独指定其他切点,可以使用表达式指定
  // @Before("execution(* com.sam.aop.service.AspectService.*(..)) && args(email)")
  @Before("logAop() && args(email)")
  public void logBefore(JoinPoint joinPoint,String email) {
      logger.debug("前置通知Before>>{}", email);
  }
  @AfterReturning("logAop() && args(email)")
  public void logAfterReturning(String email) {
      logger.debug("返回通知AfterReturning>>{}", email);
  }
  @After("logAop() && args(email)")
  public void logAfter(String email) {
      logger.debug("后置通知After>>{}", email);
  }
  @AfterThrowing("logAop() && args(email)")
  public void logAfterThrow(String email) {
      logger.debug("异常通知AfterThrowing>>{}", email);
  }
  //环绕通知功能很强,可以替代上面的所有通知
  @Around("logAop() && args(email)")
  public void logAround(ProceedingJoinPoint jp, String email) {
      try {
          logger.debug("自定义前置通知Before>>>{}", email);
          jp.proceed();//将控制权交给被通知的方法，也就是执行sayHello方法
          logger.debug("自定义返回通知AfterReturning>>>{}", email);
      } catch (Throwable throwable) {
          logger.debug("异常处理>>>>{}", email);
          throwable.printStackTrace();
      }
       logger.debug("自定义后置通知After>>>{}", email);
  }
}
```
- 
当调用AspectService的方法时,就会触发通知
```
/**
* Created by sam on 16/6/1.
*/
@Service
public class AspectService implements IAspectService {
  private Logger logger = LoggerFactory.getLogger(AspectService.class);
  @Override
  public void sayHello(String email) {
      logger.debug("sayHello to >>>>{}", email);
  }
}
```
```
public interface IAspectService {  
   void sayHello(String email);
}
```
- 
controller中
```
@Autowired
AspectService aspectService;
@RequestMapping("/aop")public String aop(Model model) {    
    aspectService.sayHello("sam@balabala.com");      
    model.addAttribute("hello", "您好! AOP");      
    model.addAttribute("world", "everything is AOP");   
    return "sam";
}
```
- console中的执行顺序如图
![](http://upload-images.jianshu.io/upload_images/1723854-8fb46ae8e13245ed.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Paste_Image.png
- 
关于执行顺序的相关说明 : 你会发现这里的环绕通知先于前置通知执行了,而实际上,如果两个 advice 位于同一 aspect 内,且执行顺序有先后,通过 advice 的声明顺序是无法确定其执行顺序的，因为 advice 方法的声明顺序无法通过反射获取.而如果两个advice位于不同的aspect的话,可以通过如下两种方式控制 Aspect 的施加顺序：
> 
1.Aspect 类添加注解：org.springframework.core.annotation.Order
顺序值：使用注解属性指定
2.Aspect 类实现接口：org.springframework.core.Ordered
顺序值：实现 Ordered 接口的 getOrder() 方法即可
- 
针对上述第一种情况,只能采取如下变通方式，二选一：
将两个 advice 合并为一个 advice，那么执行顺序就可以通过代码控制了
将两个 advice 分别抽离到各自的 aspect 内，然后为 aspect 指定执行顺序
也就是说,还是不要给一个切点既设置环绕又设置其他通知了
- 
通知顺序Order 顺序可参考 [http://blog.csdn.net/z69183787/article/details/73614254](http://blog.csdn.net/z69183787/article/details/73614254)
