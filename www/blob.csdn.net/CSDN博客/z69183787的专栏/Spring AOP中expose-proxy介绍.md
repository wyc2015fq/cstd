# Spring AOP中expose-proxy介绍 - z69183787的专栏 - CSDN博客
2018年10月19日 19:07:20[OkidoGreen](https://me.csdn.net/z69183787)阅读数：746
[https://www.cnblogs.com/chihirotan/p/7356683.html](https://www.cnblogs.com/chihirotan/p/7356683.html)
### 写在前面
　　expose-proxy。为是否暴露当前代理对象为ThreadLocal模式。
　　SpringAOP对于最外层的函数只拦截public方法，不拦截protected和private方法（后续讲解），另外不会对最外层的public方法内部调用的其他方法也进行拦截，即只停留于代理对象所调用的方法。
### 案例分析
![复制代码](https://common.cnblogs.com/images/copycode.gif)
public class AServiceImpl implements AService{
    @Override
    public void barA() {
        System.out.println("AServiceImpl.barA()");  
        barB();
    }
    @Override
    public void barB() {
         System.out.println("AServiceImpl.barB()");  
    }
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
控制台的输出结果：
run my before advice
AServiceImpl.barA()
AServiceImpl.barB()
分析：
　　发现aop并没有对barB方法进行增强，只是增强了barA方法。
　　判断上述this.barB()方法是否被拦截的最本质的东西是看this到底是谁？有如下对象B类的对象b,和cglib生成的代理对象bProxy，代理对象bProxy内部拥有b。如果调用b对象的任何方法，肯定不会发生任何拦截，当调用bProxy的方法则都会进入拦截函数。
　　当我们调用bProxy对象的barA()方法时，先执行cglib之前设置的callback对象的intercept拦截函数，如下： （之前的文章已经分析过代码）
![复制代码](https://common.cnblogs.com/images/copycode.gif)
public Object intercept(Object proxy, Method method, Object[] args, MethodProxy methodProxy) throws Throwable {
            Object oldProxy = null;
            boolean setProxyContext = false;
            Class<?> targetClass = null;
            Object target = null;
            try {
                if (this.advised.exposeProxy) {
                    oldProxy = AopContext.setCurrentProxy(proxy);
                    setProxyContext = true;
                }
                target = getTarget();
                if (target != null) {
                    targetClass = target.getClass();
                }
                List<Object> chain = this.advised.getInterceptorsAndDynamicInterceptionAdvice(method, targetClass);
                Object retVal;
                if (chain.isEmpty() && Modifier.isPublic(method.getModifiers())) {
                    retVal = methodProxy.invoke(target, args);
                }
                else {
                    retVal = new CglibMethodInvocation(proxy, target, method, args, targetClass, chain, methodProxy).proceed();
                }
                retVal = processReturnType(proxy, target, method, retVal);
                return retVal;
            }
            finally {
                if (target != null) {
                    releaseTarget(target);
                }
                if (setProxyContext) {
                    AopContext.setCurrentProxy(oldProxy);
                }
            }
        }
![复制代码](https://common.cnblogs.com/images/copycode.gif)
　　这个过程之前的文章已经分析过，这里就是首先取出拦截器链List<Object> chain，当barA方法不符合我们所配置的pointcut时，拦截器链必然为空,然后就是直接执行目标对象的方法。 
当barA方法符合所配置的pointcut时，拦截器链不为空，执行相应的通知advice，currentInterceptorIndex 从-1开始，如下（之前的文章已经分析过代码）
![复制代码](https://common.cnblogs.com/images/copycode.gif)
public Object proceed() throws Throwable {
        if (this.currentInterceptorIndex == this.interceptorsAndDynamicMethodMatchers.size() - 1) {
            return invokeJoinpoint();
        }
        Object interceptorOrInterceptionAdvice =
                this.interceptorsAndDynamicMethodMatchers.get(++this.currentInterceptorIndex);
        if (interceptorOrInterceptionAdvice instanceof InterceptorAndDynamicMethodMatcher) {
            InterceptorAndDynamicMethodMatcher dm =
                    (InterceptorAndDynamicMethodMatcher) interceptorOrInterceptionAdvice;
            if (dm.methodMatcher.matches(this.method, this.targetClass, this.arguments)) {
                return dm.interceptor.invoke(this);
            }
            else {
                return proceed();
            }
        }
        else {
            return ((MethodInterceptor) interceptorOrInterceptionAdvice).invoke(this);
        }
    }
![复制代码](https://common.cnblogs.com/images/copycode.gif)
　　随着通知不断的传递执行，最终this.currentInterceptorIndex == this.interceptorsAndDynamicMethodMatchers.size() - 1将会满足条件，将会来到执行目标对象的方法invokeJoinpoint()： 
![复制代码](https://common.cnblogs.com/images/copycode.gif)
protected Object invokeJoinpoint() throws Throwable {
            if (this.publicMethod) {
                return this.methodProxy.invoke(this.target, this.arguments);
            }
            else {
                return super.invokeJoinpoint();
            }
        }
![复制代码](https://common.cnblogs.com/images/copycode.gif)
　　在这里不管要拦截的目标方法是不是public方法，最终所传递的对象都是this.target，他是目标对象而不是代理对象，即执行上述barA()函数的对象是目标对象而不是代理对象，所以它内部所调用的this.barB()也是目标对象,因此不会发生拦截，如果是执行的是代理对象.barB（）则必然会进入intercept拦截过程。所以上述调用barA函数，其内部调用的barB函数是不会发生拦截的，因为this指的是目标对象，不是代理对象。 
### 实现BarB拦截
　　如果你想实现barA调用时内部的BarB也进行拦截，就必须把this换成代理对象。这时就要用到了，xml配置中的expose-proxy="true",即暴露出代理对象，它使用的是ThreadLocal设计模式，我们可以这样获取代理对象,AopContext.currentProxy()就是代理对象，然后转换成目标对象或者目标接口，执行相应的方法：
![复制代码](https://common.cnblogs.com/images/copycode.gif)
public class AServiceImpl implements AService{
    @Override
    public void barA() {
        System.out.println("AServiceImpl.barA()");  
        AService proxy=(AService) AopContext.currentProxy();
        proxy.barB();
    }
    @Override
    public void barB() {
         System.out.println("AServiceImpl.barB()");  
    }
}
![复制代码](https://common.cnblogs.com/images/copycode.gif)
![复制代码](https://common.cnblogs.com/images/copycode.gif)
    <bean id="aServiceImplProxy" class="org.springframework.aop.framework.ProxyFactoryBean">
        <property name="exposeProxy">
            <value>true</value>
        </property>
        <property name="interfaces" value="com.xxx.plus.aop.demo.AService"/>
        <property name="target">
            <ref bean="aServiceImpl"/>
        </property>
         <property name="interceptorNames">  
            <list>  
                <value>myBeforAdvice</value>  
            </list>  
        </property>  
    </bean>
![复制代码](https://common.cnblogs.com/images/copycode.gif)
执行结果：
run my before advice
AServiceImpl.barA()
run my before advice
AServiceImpl.barB()
分析：
　　barA()和barB都被aop拦截实现了增强
最后再给出Spring的文档说明：
　　Due to the proxy-based nature of Spring’s AOP framework, protected methods are by definition not intercepted, neither for JDK proxies (where this isn’t applicable) nor for CGLIB proxies (where this is technically possible but not recommendable for AOP purposes). As a consequence, any given pointcut will be matched against public methods only! 
　　If your interception needs include protected/private methods or even constructors, consider the use of Spring-driven native AspectJ weaving instead of Spring’s proxy-based AOP framework. This constitutes a different mode of AOP usage with different characteristics, so be sure to make yourself familiar with weaving first before making a decision.
