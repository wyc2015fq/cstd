# spring boot security自定义配置授权服务器实现Oauth2授权（支持密码和授权码两种模式） - 零度的博客专栏 - CSDN博客
2018年08月23日 18:44:33[零度anngle](https://me.csdn.net/zmx729618)阅读数：6619
1、application.yml
```
server:
  port: 8888
  
security:
  oauth2:
    client:
      client-id: test_client_id
      client-secret: test_client_secret
      scope: read,write     
      auto-approve-scopes: '.*'
logging:
  level:
    org.springframework.security: info
```
2、pom.xml
```
<dependency>
			<groupId>org.springframework.boot</groupId>
			<artifactId>spring-boot-starter-actuator</artifactId>
		</dependency>
		<dependency>
			<groupId>org.springframework.boot</groupId>
			<artifactId>spring-boot-starter-security</artifactId>
		</dependency>
		<dependency>
			<groupId>org.springframework.boot</groupId>
			<artifactId>spring-boot-starter-web</artifactId>
		</dependency>
		<dependency>
			<groupId>org.springframework.security.oauth</groupId>
			<artifactId>spring-security-oauth2</artifactId>
		</dependency>
```
3、自定义授权服务器配置CustomAuthorizationServerConfiguration.java
```
package com.zmx.springcloud.config.authorization_server;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.beans.factory.annotation.Qualifier;
import org.springframework.boot.bind.RelaxedPropertyResolver;
import org.springframework.context.EnvironmentAware;
import org.springframework.context.annotation.Bean;
import org.springframework.context.annotation.Configuration;
import org.springframework.core.env.Environment;
import org.springframework.security.authentication.AuthenticationManager;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.security.oauth2.config.annotation.configurers.ClientDetailsServiceConfigurer;
import org.springframework.security.oauth2.config.annotation.web.configuration.AuthorizationServerConfigurerAdapter;
import org.springframework.security.oauth2.config.annotation.web.configuration.EnableAuthorizationServer;
import org.springframework.security.oauth2.config.annotation.web.configurers.AuthorizationServerEndpointsConfigurer;
import org.springframework.security.oauth2.provider.token.TokenStore;
import org.springframework.security.oauth2.provider.token.store.InMemoryTokenStore;
/**
 * 自定义配置授权服务器
 * 配置授权的相关信息，配置的核心都在这里 在这里进行配置客户端，配置token存储方式等
 * @author zhangwenchao
 *
 */
@Configuration
//认证服务器
@EnableAuthorizationServer
public class CustomAuthorizationServerConfiguration extends AuthorizationServerConfigurerAdapter implements EnvironmentAware {
	
	
    private Logger logger = LoggerFactory.getLogger(CustomAuthorizationServerConfiguration.class);
    
    private static final String ENV_OAUTH = "security.oauth2.client.";
    private static final String PROP_CLIENTID = "client-id";
    private static final String PROP_SECRET = "client-secret";
    
    /**
     * 使用EnvironmentAware的setEnvironment方法初始化
     */
    private RelaxedPropertyResolver propertyResolver;
    
    
    /**
     * 认证管理器
     */
    @Autowired
    @Qualifier("authenticationManagerBean")
    private AuthenticationManager authenticationManager;
    
    /**
     * UserDetailsService
     */
    @Autowired
    private UserDetailsService userDetailsService;
   /*
    @Autowired
    private DataSource dataSource;
    @Bean
    public TokenStore tokenStoreForJDBC(){
        //这个是基于JDBC的实现tokenStore，令牌（Access Token）会保存到数据库
        return new JdbcTokenStore(dataSource);
    }
    */
    
    @Bean("tokenStore")
    public TokenStore tokenStore() {
        //使用内存的tokenStore,令牌（Access Token）会保存到内存  	
        return new InMemoryTokenStore();
    }
    
    @Override
    public void configure(AuthorizationServerEndpointsConfigurer endpoints) throws Exception {
        // endpoints.tokenStore(tokenStoreForJDBC()).authenticationManager(authenticationManager).userDetailsService(customUserDetailsService);
        
        //配置userDetailsService这样每次认证的时候会去检验用户是否锁定，有效等
        endpoints.tokenStore(tokenStore()).authenticationManager(authenticationManager).userDetailsService(userDetailsService); 
    }
    
    /**
     * 声明ClientDetailService--验证客户端id与secret
     */
    /*
    @Bean    
    public ClientDetailsService  getClientDetails() {
        return new ClientDetailsService() {
			
			@Override
			public ClientDetails loadClientByClientId(String clientId) throws ClientRegistrationException {
		       
		        ClientDetails clientDetails = new BaseClientDetails();
		        return clientDetails;
			}
		};
    }
    */
    
    @Override
    public void configure(ClientDetailsServiceConfigurer clients) throws Exception {
    	
    	    logger.info("===============配置授权服务器开始...=========");   	    
	        //clients.withClientDetails(getClientDetails());  //数据库验证方式
    	    
	        /**
	         * 配置客户端clientid secret等信息
	         */
	        clients.inMemory() // 使用in-memory存储	        
		           .withClient(propertyResolver.getProperty(PROP_CLIENTID))//client_id用来标识客户的Id
		           .scopes("read", "write") //允许授权范围--这一项用于服务提供商区分提供哪些服务数据
		         //.authorities("ROLE_ADMIN","ROLE_USER")//客户端可以使用的权限		       
		           .authorizedGrantTypes("authorization_code","password", "refresh_token")//允许授权类型		       		        
		           .secret(propertyResolver.getProperty(PROP_SECRET))//secret客户端安全码
		           .accessTokenValiditySeconds(10000) //token过期时间
		           .refreshTokenValiditySeconds(100000); //refresh过期时间 
	        logger.info("===============配置授权服务器完成=========");
    }
    @Override
    public void setEnvironment(Environment environment) {
        //获取到前缀是"authentication.oauth." 的属性列表值.
        this.propertyResolver = new RelaxedPropertyResolver(environment, ENV_OAUTH);
    }
}
```
4、自定义登录验证，CustomUserDetailsService.java
```
package com.zmx.springcloud.config.authorization_server;
import java.util.ArrayList;
import java.util.Collection;
import java.util.HashSet;
import java.util.Optional;
import java.util.Set;
import org.springframework.context.annotation.Primary;
import org.springframework.security.core.GrantedAuthority;
import org.springframework.security.core.authority.SimpleGrantedAuthority;
import org.springframework.security.core.userdetails.UserDetails;
import org.springframework.security.core.userdetails.UserDetailsService;
import org.springframework.security.core.userdetails.UsernameNotFoundException;
import org.springframework.stereotype.Service;
import com.zmx.springcloud.entity.MyUserDetails;
import com.zmx.springcloud.entity.User;
@Primary
@Service("userDetailsService")
public class CustomUserDetailsService implements UserDetailsService{
	
	//@Autowired  //业务服务类
	//private UserService userService;
    private final static Set<User> users = new HashSet<>();
    static {
        users.add(new User(1, "test-user1", "123456"));
        users.add(new User(2, "test-user2", "123456"));
        users.add(new User(3, "test-user3", "123456"));
        users.add(new User(4, "test-user4", "123456"));
    }
    /**
     * spring会将MyUserDetails中的密码与session中的密码比较，否是验证通过
     */
	@Override
	public UserDetails loadUserByUsername(String userName) throws UsernameNotFoundException {	
		
        //User对应数据库中的用户表，是最终存储用户和密码的表
		//User user = userService.findByName(userName);
		//if (user == null) {
		//	throw new UsernameNotFoundException("UserName " + userName + " not found");
		//}
		
		//grantedAuthorities对应数据库中权限的表
		Collection<GrantedAuthority> grantedAuthorities = new ArrayList<GrantedAuthority>();
		grantedAuthorities.add(new SimpleGrantedAuthority("ROLE_ADMIN"));
		grantedAuthorities.add(new SimpleGrantedAuthority("ROLE_USER"));
		
        Optional<User> user = users.stream()
        		              .filter((u) -> u.getUsername().equals(userName))
                              .findFirst();
        if (user.isPresent()){
        	return new MyUserDetails(user.get(),grantedAuthorities);  
        }else{      	
        	throw new UsernameNotFoundException("there's no user founded!");
        }
     
		
	}
	
	
	
	
}
```
5、测试一个需授权的资源并测试
```
package com.zmx.springcloud.controller;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.oauth2.provider.token.TokenStore;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestHeader;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;
import com.zmx.springcloud.entity.MyUserDetails;
import com.zmx.springcloud.entity.User;
@RestController
@RequestMapping("/secure")
public class UserController {
    @Autowired
    private TokenStore tokenStore;
 
    @PostMapping("/user")
    public String user(@RequestHeader("Authorization") String auth) { //需要提供访问的token才能访问，资源服务也需要验证token是否有效
    	String token = auth.split(" ")[1];   //获取token（验证）
        MyUserDetails userDetails = (MyUserDetails) tokenStore.readAuthentication(token).getPrincipal();
        User user = userDetails.getUser();
        return user.getUsername() + ":" + user.getPassword();
    }
}
```
6、测试密码模式
    6.1获取token：访问：[http://localhost:8888/oauth/token](http://localhost:8888/oauth/token)，需要client-id与secret生成header（base64编码）
![](https://img-blog.csdn.net/20180823181249351?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3pteDcyOTYxOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180823181353954?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3pteDcyOTYxOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![](https://img-blog.csdn.net/20180823181509525?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3pteDcyOTYxOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
6.2使用token访问授权资源：
![](https://img-blog.csdn.net/2018082318191576?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3pteDcyOTYxOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
7、使用授权码模式访问受限资源：
    7.1 获取code，访问：
```
http://localhost:8888/oauth/authorize?
response_type=code&client_id=test_client_id&redirect_uri=http://www.baidu.com&scope=read write
```
![](https://img-blog.csdn.net/20180823182706887?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3pteDcyOTYxOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
会跳转到授权页面，收入用户名和密码：
![](https://img-blog.csdn.net/20180823182545417?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3pteDcyOTYxOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
登录成功后，跳转到下面页面：
![](https://img-blog.csdn.net/20180823183251435?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3pteDcyOTYxOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
同意并授权后跳转到redirect-uri页面并带上code，如下页面从而能获取到code：
![](https://img-blog.csdn.net/20180823183625297?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3pteDcyOTYxOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
7.2根据code获取token：
![](https://img-blog.csdn.net/20180823183913822?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3pteDcyOTYxOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
同样的也需要client-id与secret组成Authorization头部信息：
7.3使用token访问授权资源
![](https://img-blog.csdn.net/20180823184303185?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3pteDcyOTYxOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
至此，两种模式均已验证完毕！
