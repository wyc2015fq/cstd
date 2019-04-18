# spring3.0事务的多数据源的annotation-driven用法 - z69183787的专栏 - CSDN博客
2012年10月25日 14:35:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1912

**一.**annotation-driven如何正确使用事务管理器
(本文仅基于3.0+版本作为测试)
假定spring 容器中定义了两个事务管理器:transactionManagerX,transactionManagerY,分管两个数据源datasourceX和datasourceY.
<tx:annotation-driven transaction-manager="transactionManagerX" />
<tx:annotation-driven transaction-manager="transactionManagerY" />
（spring容器中的定义顺序如上）
有如下应用代码：
public interface TestEntityService {
public void methodX();
public void methodY();
}
接口实现类1
public class TestEntityServiceImpl implements TestEntityService {
@Resource
private TestEntityDao testEntityDao;//实际操作的是datasourceX.
@Transactional
public void methodX() {
testEntityDao.xxx();
testEntityDao.zzz();
}
public void methodY() {
}
}
接口实现类2
public class AnotherTestEntityServiceImpl implements TestEntityService {
@Resource
private TestEntityDao anOtherTestEntityDao;//实际操作的是datasourceY.
@Transactional
public void methodX() {
testEntityDao.mmm();
testEntityDao.nnn();
}
public void methodY() {
}
}
假设方法methodX需要事务控制的，通常我们是直接在方法上添加@Transactional标注，
但是好像spring3.0（具体版本没弄清）之前的Transactional标注不支持区分使用哪个事务管理器。3.0之后的版本Transactional增加了个string类型的value属性来特殊指定加以区分。
例如@Transactional("aaaaa"),即显示的要求spring用id="aaaaa"的事务管理器来管理事务。该属性亦可省略（省略的话用容器中缺省的transactionManager）
对于该属性的用法做了如下测试来
|methodX()事务生效测试结果|@Transactional("transactionManagerX")|@Transactional("transactionManagerY")|@Transactional("transactionManagerZ") transactionManagerZ为未定义过的|@Transactional|
|----|----|----|----|----|
|TestEntityServiceImpl（实际使用datasourceX）|Y|N|Y|Y|
|AnotherTestEntityServiceImpl （实际使用datasourceY）|N|Y|N|N|
如果调换两个事务管理器在容器中的定义顺序，如
<tx:annotation-driven transaction-manager="transactionManagerY" />
<tx:annotation-driven transaction-manager="transactionManagerX" />
得到的结果
|methodX()事务生效测试结果|@Transactional("transactionManagerX")|@Transactional("transactionManagerY")|@Transactional("transactionManagerZ") transactionManagerZ为未定义过的|@Transactional|
|----|----|----|----|----|
|TestEntityServiceImpl（实际使用datasourceX）|Y|N|N|N|
|AnotherTestEntityServiceImpl （实际使用datasourceY）|N|Y|Y|Y|
分析结果（其实源码就可以反应出）：容器指定一个默认的事务管理器
1.当在@Transactional("xxx")中正确指定了需要使用的事务管理器时，事务控制正常。
2.如果@Transactional指定了未定义过的事务管理器，spring以缺省默认的事务管理器来处理。（如果程序正好使用的是缺省事务管理器同一个数据源，事务控制将生效）。
3.如果@Transactional不指定事务管理器，使用缺省。
4.如果@Transactional指定了不匹配的事务管理器（实际用到的数据源和指定的事务管理器控制的数据源不一致），事务控制将失效.
注：spring容器缺省事务管理器：以加载顺序，首先加载的作为缺省。例如
如果
<tx:annotation-driven transaction-manager="transactionManagerX" />
<tx:annotation-driven transaction-manager="transactionManagerY" />
定义在同一个文件中，则第一个transactionManagerX作为缺省。
定义在不同文件，则按文件的加载顺序，首先加载的作为缺省。
建议：实际代码中需要用到@Transactional时，即使默认只有一个transactionManager，@Transactional也将其标明。以提高新增数据源后代码可读性，另外防止定义多个数据源后，以前缺省的不被spring默认为缺省了（比如哪天上线新定义了一个数据源，刚好新定义的transactionManager被先加载了，那就悲剧了。）
二.bean的配置使用
容器中加了<tx:annotation-driven >(需要增加一些xsd)之后，需要事务控制的的service，不需要再具体的bean上做其他的配置，例如用代理包装。直接配置即可
<bean id="testEntityService" class="com.xxx.impl.TestEntityServiceImpl"/>
spring将由JdkDynamicAopProxy 生成代理过的类提供使用。
这种用法的效果和下面配置使用效果一样。都是由JdkDynamicAopProxy 生成代理对象提供使用。
我觉得区别是下面的方法在事务控制的代码可读性上不好，因为哪个方法需要事务控制和控制粒度都在配置文件中，和代码分开了。
<bean id="testEntityService3" class="org.springframework.transaction.interceptor.TransactionProxyFactoryBean">
      <property name="transactionManager" ref="transactionManagerX" />
      <property name="target">
         <bean class="com.xxxx.impl.TestEntityServiceImpl" />
      </property>
      <property name="proxyInterfaces" value="com.xxxx.TestEntityService"/>
      <property name="transactionAttributes">
         <props>
           <prop key="*">PROPAGATION_REQUIRED</prop>
         </props>
      </property>
</bean>
方法的可见度和 @Transactional
@Transactional 注解应该只被应用到 public 可见度的方法上。 如果你在 protected、private 或者 package-visible 的方法上使用 @Transactional 注解，它也不会报错， 但是这个被注解的方法将不会展示已配置的事务设置。
@Transactional 注解可以被应用于接口定义和接口方法、类定义和类的 public 方法上。然而，请注意仅仅 @Transactional 注解的出现不足于开启事务行为，它仅仅 是一种元数据，能够被可以识别 @Transactional 注解和上述的配置适当的具有事务行为的beans所使用。上面的例子中，其实正是 <tx:annotation-driven/>元素的出现 开启 了事务行为。
Spring团队的建议是你在具体的类（或类的方法）上使用 @Transactional 注解，而不要使用在类所要实现的任何接口上。你当然可以在接口上使用 @Transactional 注解，但是这将只能当你设置了基于接口的代理时它才生效。因为注解是 不能继承 的。
实际开发中，多半喜欢将持久化操作的代码集中抽出为另一个方法（因为不想事务被无关的业务代码托的持续太长），然后在抽取出来的方法上加上@Transactional，这样的结果是被抽离出的代码即使加了事务标记，也根本起不到事务控制的效果（不管是private和public）。
例如：
public class TestEntityServiceImpl implements TestEntityService {
@Resource
private TestEntityDao testEntityDao;//实际操作的是datasourceX.
@Transactional
public void methodX() {
testEntityDao.xxx();
testEntityDao.zzz();
}
public void methodY() {
methodX() 
}
}
如果执行TestEntityService.methodY();事务是不生效的。只有TestEntityService.methodX();才生效。
从spring实现这些的原理（动态代理和aop）上来看，只拦截外部调用，方法的内部调用通常是不被aop支持的。
从网上扒到一篇文章，可以解决这个问题。[http://blog.csdn.net/quzishen/archive/2010/08/11/5803721.aspx](http://blog.csdn.net/quzishen/archive/2010/08/11/5803721.aspx)
