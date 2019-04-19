# Spring与Struts - LC900730的博客 - CSDN博客
2017年08月08日 14:24:20[lc900730](https://me.csdn.net/LC900730)阅读数：117标签：[spring](https://so.csdn.net/so/search/s.do?q=spring&t=blog)
个人分类：[java](https://blog.csdn.net/LC900730/article/category/7027036)
## 整合
开发步骤： 
    1.引入struts.jar相关文件 
    2.spring-core相关jar文件 
    3.spring-web支持jar包 
配置： 
    struts.xml     struts与action的映射 
    bean.xml     Spring ioc容器配置 
    web.xml     核心过滤器，引入struts功能，初始化spring容器配置
### AOP
业务代码与关注点代码分离的好处： 
    关注点写一次即可 
    开发者只需要关注核心业务 
    运行期间，执行核心业务代码时候动态植入关注点代码【代理】
### 代理
```java
public class ProxyFactory{
        //维护目标对象
        private static Object target;
        private static Aop aop;
        //生成代理对象的方法
        public static Object getProxyInstance(Object target_,Aop aop_){
            target=target_;
            aop=aop_;
            return Proxy.newProxyInstance(
                target.getClass().getClassLoader(),
                target.getClass().getInterfaces(),
                new InvocationHandler(){
                    @Override
                    public Object invoke(Object proxy,Method method,Object[] args) throws Throwable{
                    //执行重复代码
                    aop.begin();
                    //执行目标对象的方法
                Object  returnvalue=method.invoke(target,args);
                    //执行重复代码
                    aop.commit();
                return returnvalue;
                        }
                });
        }
    }
```
调用工厂方法，返回userDao代理后的对象
```
<bean id="userDao_proxy" class="cn.xxx.ProxyFactory" factory-metho="getProxyInstance">
        <constructor-arg index="0" ref="userDao"><constructor-arg>
        <constructor-arg index="1" ref="aop"><constructor-arg>
    </bean>
```
### 注解方式实现AOP
```
spring-aop-3.2.5
aopalliance.jar
aspectjweaver.jar
aspectjrt.jar
如果是spring2.5的版本的jar，如果用jdk1.7可能会有问题
需要升级aspectj组件，即使用aspectj-1.8.2版本中提供jar支持
```
开启扫描注解： 
    开启aop注解 
指定切面类
```java
@Component
@Aspect
public class Aop{
    //指定切入点表达式，拦截哪些方法，即为哪些类生成代理对象
    @Pointcut()
    @Pointcut("execution(* cn.itcast.e_myaop1.UserDao.save(..))")
    public void begin(){
        System.out.println("开始事务"); 
    }
    @Pointcut("execution(* cn.itcast.e_myaop1.UserDao.save(..))")
    public void commit(){
        System.out.println("提交事务");
}
```
