# Shiro使用教程 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年10月15日 16:25:23[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：198


**入门使用加完整源码**

链接: [点击进入](https://blog.csdn.net/u013142781/article/details/50629708).

该原文中最后面项目源码下载需要30积分，如果你有CSDN会员直接下载即可，还是要尊重原创。

如果真不舍得花钱开会员就从从github上下载吧
`git@github.com:zzh546934282/shiro.git`

我自己在这里说一些比较值得注意的地方

1： 访问的url所需权限的设置

2： 指定登录页面、登录成功页面、权限不够页面

其中 loginUrl 代表未登录时访问了需登录以后才能查看的页面，重定向的页面地址。

上面链接中作者示例，未登录状态访问index页面，会跳转到登录页面，原因就是下面 设置了/index.jhtml = authc 即访问这个页面需要对用户身份进行验证，再配合lginUrl 就跳转到了登录页面

```xml
<bean id="shiroFilter" class="org.apache.shiro.spring.web.ShiroFilterFactoryBean">  
        <property name="securityManager" ref="securityManager" />  
        <property name="loginUrl" value="/login.jhtml" />  
        <property name="successUrl" value="/loginsuccess.jhtml" />  
        <property name="unauthorizedUrl" value="/error.jhtml" />  
        <property name="filterChainDefinitions">  
            <value>  
                /index.jhtml = authc  
                /login.jhtml = anon
                /checkLogin.json = anon  
                /loginsuccess.jhtml = anon  
                /logout.json = anon  
                /** = authc  
            </value>  
        </property>  
    </bean>
```

还有一个核心的内容就是如下这个类，该类负责添加用户权限，以及用户信息的验证

```java
package com.luo.shiro.realm;

import java.util.HashSet;
import java.util.Set;

import org.apache.shiro.authc.AuthenticationException;
import org.apache.shiro.authc.AuthenticationInfo;
import org.apache.shiro.authc.AuthenticationToken;
import org.apache.shiro.authc.SimpleAuthenticationInfo;
import org.apache.shiro.authc.UsernamePasswordToken;
import org.apache.shiro.authz.AuthorizationInfo;
import org.apache.shiro.authz.SimpleAuthorizationInfo;
import org.apache.shiro.realm.AuthorizingRealm;
import org.apache.shiro.subject.PrincipalCollection;

import com.luo.util.DecriptUtil;

public class MyShiroRealm extends AuthorizingRealm {
	
	//这里因为没有调用后台，直接默认只有一个用户("luoguohui"，"123456")
	private static final String USER_NAME = "luoguohui";  
	private static final String PASSWORD = "123456";  
	
	/* 
	 * 授权
	 */
	@Override
	protected AuthorizationInfo doGetAuthorizationInfo(PrincipalCollection principals) { 
        Set<String> roleNames = new HashSet<String>();  
        Set<String> permissions = new HashSet<String>();  
        roleNames.add("administrator");//添加角色
        permissions.add("newPage.jhtml");  //添加权限
        SimpleAuthorizationInfo info = new SimpleAuthorizationInfo(roleNames);  
        info.setStringPermissions(permissions);  
        return info;  
	}
	
	
	/* 
	 * 登录验证
	 */
	@Override
	protected AuthenticationInfo doGetAuthenticationInfo(
			AuthenticationToken authcToken) throws AuthenticationException {
		UsernamePasswordToken token = (UsernamePasswordToken) authcToken;
		if(token.getUsername().equals(USER_NAME)){
			return new SimpleAuthenticationInfo(USER_NAME, DecriptUtil.MD5(PASSWORD), getName());  
		}else{
			throw new AuthenticationException();  
		}
	}

}
```

**关于何时执行shiro AuthorizingRealm 里的 doGetAuthenticationInfo与doGetAuthorizationInfo**

链接: [点击进入](https://blog.csdn.net/fj200821/article/details/54601629).

