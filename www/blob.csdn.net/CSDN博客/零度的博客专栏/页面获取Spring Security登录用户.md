# 页面获取Spring Security登录用户 - 零度的博客专栏 - CSDN博客
2016年07月15日 09:46:47[零度anngle](https://me.csdn.net/zmx729618)阅读数：18509标签：[spring security																[session](https://so.csdn.net/so/search/s.do?q=session&t=blog)](https://so.csdn.net/so/search/s.do?q=spring security&t=blog)
个人分类：[SpringSecurity](https://blog.csdn.net/zmx729618/article/category/6173384)
1.在session中取得spring security的登录用户名如下：
`${session.SPRING_SECURITY_CONTEXT.authentication.principal.username}`
       spring security 把SPRING_SECURITY_CONTEXT 放入了session 没有直接把username 放进去。下面一段[代码](http://www.07net01.com/program/)主要描述的是session中的存的变量，
存跳转时候的URLsession {SPRING_SECURITY_SAVED_REQUEST_KEY=SavedRequest[http://[localhost](http://www.07net01.com/tags-localhost-0.html):8080/AVerPortal/resourceAction/resourceIndex.action]}
存的是登录成功时候session中存的信息：
        session {SPRING_SECURITY_CONTEXT=org.springframework.security.context.SecurityContextImpl@87b16984: Authentication: org.springframework.security.providers.cas.CasAuthenticationToken@87b16984:
 Principal: com.avi.casExtends.UserInfo@ff631d80: Username: test; Password: [PROTECTED]; Enabled: true; AccountNonExpired: true; credentialsNonExpired: true; AccountNonLocked: true; Granted Authorities: ROLE_ADMIN; Password: [PROTECTED]; Authenticated: true;
 Details: org.springframework.security.ui.WebAuthenticationDetails@12afc: RemoteIpAddress: 127.0.0.1; SessionId: AE56E8925195DFF4C50ABD384574CCEA; Granted Authorities: ROLE_ADMIN Assertion: org.jasig.cas.client.validation.AssertionImpl@661a11 Credentials (Service/Proxy
 Ticket): ST-3-1lX3acgZ6HNgmhvjXuxB-cas, userId=2, userName=test}
2.在页面端用tag获取：
```
<%@ taglib prefix='security' uri='http://www.springframework.org/security/tags'%>
<security:authentication property="principal.username"></security:authentication>
```
或者
```
<security:authorize ifAllGranted="ROLE_ADMIN">
 <security:authentication property="principal.username"></security:authentication> 
</security:authorize>
```
或者取session中的值：
`${session.SPRING_SECURITY_CONTEXT.authentication.principal.username}`
3.在后台获取
```
UserDetails userDetails = (UserDetails) SecurityContextHolder.getContext()
    .getAuthentication()
    .getPrincipal();
userDetails.getUsername()
```
      如果想要获取更多的信息：得扩展userDetails的默认实现类user类和UserDetailsService[接口](http://www.07net01.com/tags-%E6%8E%A5%E5%8F%A3-0.html)
     由于springsecurity是把整个user信息放入session中的即：session.SPRING_SECURITY_CONTEXT.authentication.principal。这个就是代表着user对象。
