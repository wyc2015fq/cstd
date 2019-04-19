# cas5.1.8 自定义验证 - 左直拳的马桶_日用桶 - CSDN博客
2018年02月13日 10:51:11[左直拳](https://me.csdn.net/leftfist)阅读数：1551
如前所述，cas虽然已经编译部署可运行，但只有一个默认账号casuser。这并没有什么意义。
可以改成支持现有数据库。
**1、修改配置文件**
改下配置文件（源码：webapp/resources/application.properties，部署之后是/WEB-INF/classes/application.properties）即可。以oracle为例：
```
#注释默认账号
#cas.authn.accept.users=casuser::Mellon 
#添加以下配置
cas.authn.jdbc.query[0].sql=SELECT REVERSE(u_password) as psw FROM org_user WHERE u_loginname=?
cas.authn.jdbc.query[0].healthQuery=SELECT 1 from dual
cas.authn.jdbc.query[0].url=jdbc:oracle:thin:@192.168.0.22:1521/pdbhnjczs #oracle数据库版本为12C，pdbhnjczs为PDB，所以连接串写为"IP:端口/PDB"的格式，而不是"IP:端口:DB"
cas.authn.jdbc.query[0].user=数据库登录账号
cas.authn.jdbc.query[0].password=数据库登录密码
cas.authn.jdbc.query[0].dialect=org.hibernate.dialect.Oracle10gDialect
cas.authn.jdbc.query[0].driverClass=oracle.jdbc.OracleDriver
cas.authn.jdbc.query[0].isolateInternalQueries=false
cas.authn.jdbc.query[0].failFast=true
cas.authn.jdbc.query[0].isolationLevelName=ISOLATION_READ_COMMITTED
cas.authn.jdbc.query[0].leakThreshold=10
cas.authn.jdbc.query[0].propagationBehaviorName=PROPAGATION_REQUIRED
cas.authn.jdbc.query[0].batchSize=1
cas.authn.jdbc.query[0].ddlAuto=create-drop
cas.authn.jdbc.query[0].maxAgeDays=180
cas.authn.jdbc.query[0].autocommit=false
cas.authn.jdbc.query[0].idleTimeout=5000
```
据说这样就可以支持数据库方式，从数据库里读取账号了。前提是jdk要支持oracle。比如说，我是jdk8，要将ojdbc7.jar拷贝到 %JAVA_HOME%\jre\lib\ext\。
假如账号的密码是明文的话，其实就可以直接使用了。但是！现实总是很复杂，通常密码都是加密的，前端提交过来的密码，往往也要经过一系列处理后才能比较；或者要添加验证码，或者添加所属部门等信息，这就要进行自定义验证。
2018-05-07 
上面这段话不对。其实账号加密的话，一般也无须自己编写代码进行校验。通过修改配置文件即可。 
比如说，我们项目的账号密码用MD5加密，则配置文件可以这样设置：
```
#MD5加密策略
cas.authn.jdbc.query[0].passwordEncoder.encodingAlgorithm=MD5
cas.authn.jdbc.query[0].passwordEncoder.strength=32 #MD5加密有16，32两种长度
```
以下编写自定义验证模块，与数据库是否加密无关，纯粹是为了说明这么个步骤。
**2、编写自定义验证模块**
这个验证模块完全是新增的，不修改现有的代码，通过在系统中注册，并设置验证顺序，从而让我们自定义的验证器生效。 
代码结构如图： 
![这里写图片描述](https://img-blog.csdn.net/20180213101752174?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
我刚用intellij idea不久，很不熟悉，不知道在上面如何新增一个模块，只好在文件夹里手动创建。我这个模块，姑且叫“lt”:
```
mkdir lt\src\main\java\com\landtool\sso\support\auth\config
mkdir lt\src\main\java\com\landtool\sso\support\auth\handler
```
分别在这两个文件夹里添加代码：
**1）自定义验证器**
UsernamePasswordSystemAuthenticationHandler.java
```java
package com.landtool.sso.support.auth.handler;
import org.apereo.cas.authentication.Credential;
import org.apereo.cas.authentication.HandlerResult;
import org.apereo.cas.authentication.PreventedException;
import org.apereo.cas.authentication.UsernamePasswordCredential;
import org.apereo.cas.authentication.handler.support.AbstractPreAndPostProcessingAuthenticationHandler;
import org.apereo.cas.authentication.principal.PrincipalFactory;
import org.apereo.cas.services.ServicesManager;
import javax.security.auth.login.AccountNotFoundException;
import java.security.GeneralSecurityException;
import java.util.Collections;
/**
 * 用户名系统认证，只要是admin用户就允许通过
 *
 * @author chenqu
 * @date 2018/2/12
 *
 */
public class UsernamePasswordSystemAuthenticationHandler extends AbstractPreAndPostProcessingAuthenticationHandler {
    public UsernamePasswordSystemAuthenticationHandler(String name, ServicesManager servicesManager, PrincipalFactory principalFactory, Integer order) {
        super(name, servicesManager, principalFactory, order);
    }
    @Override
    protected HandlerResult doAuthentication(Credential credential) throws GeneralSecurityException, PreventedException {
        //当用户名为admin即允许通过
        //UsernamePasswordSysCredential sysCredential = (UsernamePasswordSysCredential) credential;
        UsernamePasswordCredential ltCredential = (UsernamePasswordCredential)credential;
        if ("admin".equals(ltCredential.getUsername())) {
            //这里可以自定义属性数据
            return createHandlerResult(credential,
                    this.principalFactory.createPrincipal(ltCredential.getUsername(),
                            Collections.emptyMap()),
                    null);
        } else {
            throw new AccountNotFoundException("必须是admin用户才允许通过");
        }
    }
    @Override
    public boolean supports(Credential credential) {
        return true;
    }
}
```
**2）注册验证器**
CustomAuthenticationEventExecutionPlanConfiguration.java
```
package com.landtool.sso.support.auth.config;
import com.landtool.sso.support.auth.handler.UsernamePasswordSystemAuthenticationHandler;
import org.apereo.cas.authentication.AuthenticationEventExecutionPlan;
import org.apereo.cas.authentication.AuthenticationEventExecutionPlanConfigurer;
import org.apereo.cas.authentication.AuthenticationHandler;
import org.apereo.cas.authentication.principal.PrincipalFactory;
import org.apereo.cas.configuration.CasConfigurationProperties;
import org.apereo.cas.services.ServicesManager;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.boot.context.properties.EnableConfigurationProperties;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
@Configuration("customAuthenticationEventExecutionPlanConfiguration")
@EnableConfigurationProperties(CasConfigurationProperties.class)
public class CustomAuthenticationEventExecutionPlanConfiguration implements AuthenticationEventExecutionPlanConfigurer {
    @Autowired
    @Qualifier("servicesManager")
    private ServicesManager servicesManager;
    @Autowired
    @Qualifier("jdbcPrincipalFactory")
    public PrincipalFactory jdbcPrincipalFactory;
    /**
     * 注册验证器
     *
     * @return
     */
    @Bean
    public AuthenticationHandler customAuthenticationHandler() {
        //优先验证
        return new UsernamePasswordSystemAuthenticationHandler("customAuthenticationHandler",
                servicesManager, jdbcPrincipalFactory, 1);
    }
    //注册自定义认证器
    @Override
    public void configureAuthenticationExecutionPlan(final AuthenticationEventExecutionPlan plan) {
        plan.registerAuthenticationHandler(customAuthenticationHandler());
    }
}
```
**3）该模块下添加gradle文件**
lt/build.gradle
```
dependencies {
    compile project(":core:cas-server-core")
    compile project(":core:cas-server-core-configuration")
    compile project(":core:cas-server-core-authentication")
    compile project(":core:cas-server-core-services")
}
```
**4）该模块下添加spring配置文件**
lt\src\main\resources\META-INF\spring.factories（全都手动创建，shit！）
`org.springframework.boot.autoconfigure.EnableAutoConfiguration=\  com.landtool.sso.support.auth.config.CustomAuthenticationEventExecutionPlanConfiguration`
然后编译。 
参考文章： 
[CAS单点登录-自定义认证之重写Credential （十五）](http://blog.csdn.net/u010475041/article/details/78420322)
**5）将该模块加到war包**
![这里写图片描述](https://img-blog.csdn.net/20180213104644104?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
如此，将war包扔到tomcat8下就可以了： 
![这里写图片描述](https://img-blog.csdn.net/20180213104917090?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20180213104930172?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
