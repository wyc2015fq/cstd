# CAS5.1.8添加验证码 - 左直拳的马桶_日用桶 - CSDN博客
2018年12月05日 10:04:39[左直拳](https://me.csdn.net/leftfist)阅读数：153
老生常谈的题目了吧。但是，要在网上找出一篇能跑又看得明白的相关教程，还真不容易。
我费了九牛二虎之力，终于添加了验证码功能，记录如下。
**一、运行结果**
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181204162811156.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0,size_16,color_FFFFFF,t_70)
**二、代码结构**
代码是纯新增的，不想修改现有的代码。CAS实在是太庞大了，idea加载都要几分钟。作为一个java小白，基本看不懂里面的代码，搞不清里面的结构，怎么改？能不动就不动，可以动也不动，打死都不动。
新加的代码，就在“lt”这个目录里，如图所示。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181204163440559.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0,size_16,color_FFFFFF,t_70)
**三、代码讲解**
代码呢，基本也是抄过来的（源码好像是在[这里](https://blog.csdn.net/yelllowcong/article/details/79250841)），我也不大懂，所以只能按照自己的理解说一下，说错勿怪。
代码文件职责以及调用关系。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181205091830282.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2xlZnRmaXN0,size_16,color_FFFFFF,t_70)
梳理一下，从上到下：
**1、在spring.factories里注册自定义配置类**
```
org.springframework.boot.autoconfigure.EnableAutoConfiguration=\
  com.landtool.sso.support.auth.config.CustomAuthenticationEventExecutionPlanConfiguration
```
**2、自定义配置类里负责将生成验证码、验证流程、验证码比较类注入**
```java
@Bean
    public CaptchaController captchaController(){
        return new CaptchaController();
    }
    @Bean
    public CaptchaAction captchaAction(){
        return new CaptchaAction();
    }
    @Bean
    @Order(1)
    public CasWebflowConfigurer defaultWebflowConfigurer() {
        final CustDefaultWebflowConfigurer c = new CustDefaultWebflowConfigurer(flowBuilderServices, loginFlowDefinitionRegistry);
        return c;
    }
```
**3、验证码比较和流程，需要用到自定义凭据**
凭据里存储了验证码，然后验证的时候，读取这个凭据。凭据一直都有，默认的，但没有验证码。这里重载了凭据类，将验证码加了进去。然后又重载了流程，流程里引用了凭据。
自定义凭据CustUsernamePasswordCredential
```java
public class CustUsernamePasswordCredential extends UsernamePasswordCredential {
    private static final long serialVersionUID = 1767227441947916650L;
    private String captcha;
    public CustUsernamePasswordCredential() {
    }
    public CustUsernamePasswordCredential(String userName, String password, String captcha) {
        super(userName, password);
        this.captcha = captcha;
    }
    public String getCaptcha() {
        return captcha;
    }
    public void setCaptcha(String captcha) {
        this.captcha = captcha;
    }
    @Override
    public boolean equals(Object o) {
        if (this == o) return true;
        if (!(o instanceof CustUsernamePasswordCredential)) return false;
        if (!super.equals(o)) return false;
        CustUsernamePasswordCredential that = (CustUsernamePasswordCredential) o;
        return Objects.equals(getCaptcha(), that.getCaptcha());
    }
    @Override
    public int hashCode() {
        return Objects.hash(super.hashCode(), getCaptcha());
    }
}
```
自定义流程
```java
/**
 * 参考 {@link org.apereo.cas.web.flow.DefaultWebflowConfigurer}
 */
public class CustDefaultWebflowConfigurer extends DefaultWebflowConfigurer {
    public CustDefaultWebflowConfigurer(FlowBuilderServices flowBuilderServices,
                                        FlowDefinitionRegistry flowDefinitionRegistry) {
        super(flowBuilderServices, flowDefinitionRegistry);
    }
    /**
     * 重写 {@link org.apereo.cas.web.flow.DefaultWebflowConfigurer} 的方法
     *
     * Create remember me authn webflow config.
     *
     * @param flow the flow
     */
    protected void createRememberMeAuthnWebflowConfig(final Flow flow) {
        /**
         *用我们拓展CustUsernamePasswordCredential来替换原来的
         * {@link org.apereo.cas.authentication.UsernamePasswordCredential}
         */
        super.createFlowVariable(flow, CasWebflowConstants.VAR_ID_CREDENTIAL, CustUsernamePasswordCredential.class);
    }
}
```
验证“验证码“CaptchaAction.java
```java
@Override
    protected Event doExecute(RequestContext context) throws Exception {
        HttpServletResponse response = (HttpServletResponse) context.getExternalContext().getNativeResponse();
        HttpServletRequest request = (HttpServletRequest) context.getExternalContext().getNativeRequest();
        HttpSession session = request.getSession();
        String captchaKey = (String) session.getAttribute(CAPTCHA_KEY);
        if(null == captchaKey){
            LOGGER.error("验证码验证异常");
            return this.getError(context,CUSTOM_CAPTCHA_FAIL_EXCEPTION);
        }
		//凭据--------------------
        CustUsernamePasswordCredential credential = (CustUsernamePasswordCredential) WebUtils.getCredential(context);
        if(null == credential || (null == credential.getCaptcha() || "".equals(credential.getCaptcha().trim()))){
            LOGGER.error("请输入验证码");
            return this.getError(context,CUSTOM_CAPTCHA_FAIL_ERROR);
        }
        String requestCaptcha = credential.getCaptcha();
        if(!captchaKey.toLowerCase().equals(requestCaptcha.toLowerCase())){
            LOGGER.error("验证码错误");
            return this.getError(context,CUSTOM_CAPTCHA_FAIL_ERROR);
        }
        return super.success();
    }
```
**4、前端访问的是控制器"生成验证码"**
\webapp\resources\templates\fragments/loginform.html
```
<section class="row">
            <label for="captcha" th:utext="#{screen.welcome.label.captcha}"/>
            <div>
                <input class="required"
                       type="text"
                       id="captcha"
                       name="captcha"
                       size="25"
                       tabindex="2"
                       autocomplete="off"/>
                <img th:src="@{/captcha/get}"/>
            </div>
        </section>
```
可以发现，其中的**核心是配置类**：`CustomAuthenticationEventExecutionPlanConfiguration`（为什么要这么长，装逼吧），它负责组织、注入各种实例。里面的这些神秘字眼“`@Bean`”呀，“`@Autowired`”呀，称之为注解。感觉跟C#里的Attitude（特性）差不多，声明一下，Spring容器就知道该干什么了。小字眼，做大事。
注入，这里面屡屡提到注入，很黄很暴力。其实，注入（DI）与依赖倒置、控制反转（IoC）都是面向接口编程的一种表述（见[拙作](https://blog.csdn.net/leftfist/article/details/41960689)）。何谓注入？编码的时候面向接口编程，比如A代码里使用了对象B，但它不是直接构造一个对象B，而是使用了接口IB，类B实现了接口IB，在运行过程中，将B的实例赋给接口IB，此为注入。在这里，这个配置类就是配置一下，用哪些实例来进行注入。
附录：
[完整的LT代码](https://download.csdn.net/download/leftfist/10829083)
