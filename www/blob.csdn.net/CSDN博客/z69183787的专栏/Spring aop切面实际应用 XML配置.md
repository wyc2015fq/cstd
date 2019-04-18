# Spring aop切面实际应用 XML配置 - z69183787的专栏 - CSDN博客
2016年10月31日 20:39:07[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1747
Aspect：
```java
/**
 * Created by Administrator on 2016/8/15.
 */
@Component
public class LogAspect {
    private final static Logger logger = LogUtil.get("access");
    @Autowired
    private LogTask logTask;
    public void afterReturning(JoinPoint joinPoint,Object retVal) {
        logTask.log((PrizeInvokeBean)joinPoint.getArgs()[0],(ExternalResponse<String>) retVal);
    }
    public void accessAfterReturning(JoinPoint joinPoint,Object retVal) {
        logger.info("DrawPrizeRequest =: {} , DrawPrizeResponse =: {}", (DrawPrizeRequest) joinPoint.getArgs()[0], (DrawPrizeResponse)retVal);
    }
}
```
aop-xml：
```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
       xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
       xmlns:context="http://www.springframework.org/schema/context"
       xmlns:aop="http://www.springframework.org/schema/aop"
       xmlns:tx="http://www.springframework.org/schema/tx"
       xsi:schemaLocation="
    http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-4.1.xsd
    http://www.springframework.org/schema/aop http://www.springframework.org/schema/aop/spring-aop-4.1.xsd
       http://www.springframework.org/schema/tx http://www.springframework.org/schema/tx/spring-tx-4.1.xsd
    http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context-4.1.xsd">
    <aop:config>
        <aop:pointcut id="lotteryLog"
                      expression="execution(* com.xxxxxx.xxxxx.invoke(..))"/>
        <aop:aspect id="check" ref="logAspect">
            <aop:after-returning pointcut-ref="lotteryLog"
                                 method="afterReturning" arg-names="joinPoint,retVal"  returning="retVal"/>
        </aop:aspect>
    </aop:config>
    <aop:config>
        <aop:pointcut id="accessLog"
                      expression="execution(* com.xxxxx.xxxxxx.drawPrize(..))"/>
        <aop:aspect id="access" ref="logAspect">
            <aop:after-returning pointcut-ref="accessLog"
                                 method="accessAfterReturning" arg-names="joinPoint,retVal"  returning="retVal"/>
        </aop:aspect>
    </aop:config>
    <aop:aspectj-autoproxy/>
</beans>
```
注解：
```java
@Aspect
@Component
public class LogAspect {
    @Pointcut("execution(* com.xxxx.PrizeRemoteServiceImpl.create*(..))")
    public void create(){}
    @Pointcut("execution(* com.xxxxx.PrizeRemoteServiceImpl.update*(..))")
    public void update(){}
    @Before("create()")
    public void before(JoinPoint joinPoint){
        String appName = (String) ContextUtils.getLocalContext("CLIENT_APP");
        joinPoint.getTarget().getClass().getName();
        joinPoint.getArgs();
        joinPoint.getSignature().getName();
    }
    @AfterReturning(value="create()", argNames="rtv", returning="rtv")
    public void afterInsertMethod(JoinPoint jp, Object rtv) throws Throwable {
        Signature signature = jp.getSignature();
        log.debug("DeclaringType:" + signature.getDeclaringType());
        log.debug("DeclaringTypeName:" + signature.getDeclaringTypeName());
        log.debug("Modifiers:" + signature.getModifiers());
        log.debug("Name:" + signature.getName());
        log.debug("LongString:" + signature.toLongString());
        log.debug("ShortString:" + signature.toShortString());
        for (int i = 0; i < jp.getArgs().length; i++) {
            Object arg = jp.getArgs()[i];
            if(null != arg) {
                log.debug("Args:" + arg.toString());
            }
        }
        log.debug("Return:" + rtv);
    }
```
