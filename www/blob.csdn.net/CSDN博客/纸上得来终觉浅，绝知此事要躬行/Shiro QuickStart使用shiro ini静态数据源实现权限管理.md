# Shiro QuickStart使用shiro.ini静态数据源实现权限管理 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年07月17日 14:52:22[boonya](https://me.csdn.net/boonya)阅读数：3761








      前言：Shiro可以通过使用shiro.ini静态数据源实现权限管理，实现用户登录、授权、权限验证、异常处理和登出等操作。这里以shiro的quickstart项目为例进行学习解读，shiro SVN地址：[http://svn.apache.org/repos/asf/shiro](http://svn.apache.org/repos/asf/shiro)，QuickStart
 demo 小巧易学，不妨下来看看。

#### 1.Shiro QuickStart提供的功能

##### 1.1.获取当前用户

   通过SecurityUtils.getSubject()，就可以获取当前Subject。Subject是应用中用户的一个特定安全的缩影，虽然感觉上直接使用User会更贴切，但是实际上它的意义远远超过了User。在单应用系统中，调用getSubject()会返回一个Subject，它是位于应用程序中特定位置的用户信息；在服务器中运行的情况下(比如web应用)，getSubject会返回一个位于当前线程或请求中的用户信息。




```java
// get the currently executing user:
  Subject currentUser = SecurityUtils.getSubject();
```



##### 1.2.共享session


   Session对象是Shiro中特有的对象，它和我们经常使用的HttpSession非常相似，但还提供了额外的东西，其中与HttpSession最大的不同就是Shiro中的Session不依赖HTTP环境(换句话说，可以在非HTTP 容器下运行)。

   如果将Shiro部署在web应用程序中，那么这个Session就是基于HttpSession的。但是像QuickStart示例那样，在非web环境下使用，Shiro则默认使用EnterpriseSessionManagment。也就是说，不论在应用中的任何一层使用同样的API，却不需要考虑部署环境，这一优点为应用打开一个全新的世界，因为应用中要获取Session对象再也不用依赖于HttpSession或者EJB的会话Bean。而且任何客户端技术都可以共享session
 数据。

```java
// Do some stuff with a Session (no need for a web or EJB container!!!)
 Session session = currentUser.getSession();
 session.setAttribute("someKey", "aValue");
 String value = (String) session.getAttribute("someKey");
 if (value.equals("aValue")) {
      log.info("Retrieved the correct value! [" + value + "]");
 }
```

##### 1.3.用户验证
    可以通过已得到的currentUser对象进行验证。Subject对象代表当前用户，但是，谁才是当前用户呢？他们可是匿名用户啊。也就是说，必须登录才能获取到当前用户。如果用户验证失败则需要通过捕获各类异常，根据不同类型的异常做出不同的处理：



```java
if (!currentUser.isAuthenticated()) {
      UsernamePasswordToken token = new UsernamePasswordToken("lonestarr", "vespa");
      token.setRememberMe(true);
      try {
           currentUser.login(token);
      } catch (UnknownAccountException uae) {
           log.info("There is no user with username of " + token.getPrincipal());
      } catch (IncorrectCredentialsException ice) {
           log.info("Password for account " + token.getPrincipal() + " was incorrect!");
      } catch (LockedAccountException lae) {
           log.info("The account for username " + token.getPrincipal() + " is locked.  " +
                        "Please contact your administrator to unlock it.");
      }
      // ... catch more exceptions here (maybe custom ones specific to your application?
      catch (AuthenticationException ae) {
           //unexpected condition?  error?
      }
  }
```
用户登录成功后可以进行信息核对，打印出当前登录用户的识别码信息：


```java
//print their identifying principal (in this case, a username):
 log.info("User [" + currentUser.getPrincipal() + "] logged in successfully.");
```

##### 1.4.角色判定


判断用户是否拥有特定的角色：



```java
//test a role:
  if (currentUser.hasRole("schwartz")) {
      log.info("May the Schwartz be with you!");
  } else {
       log.info("Hello, mere mortal.");
  }
```


##### 1.5.权限判定

判断用户是否对特定某实体有操作权限：

```java
//test a typed permission (not instance-level)
 if (currentUser.isPermitted("lightsaber:weild")) {
    log.info("You may use a lightsaber ring.  Use it wisely.");
 } else {
    log.info("Sorry, lightsaber rings are for schwartz masters only.");
 }
```
另外还可以判断用户是否有某个级别的操作权限：


```java
//a (very powerful) Instance Level permission:
if (currentUser.isPermitted("winnebago:drive:eagle5")) {
   log.info("You are permitted to 'drive' the winnebago with license plate (id) 'eagle5'.  " +
                    "Here are the keys - have fun!");
} else {
   log.info("Sorry, you aren't allowed to drive the 'eagle5' winnebago!");
}
```

##### 1.6.用户登出


最后，当用户使用系统完毕，退出应用：



```java
//all done - log out!
  currentUser.logout();
```




#### 2.Quickstart.java流程组合示例

Quakstart.java 完整代码如下：



```java
/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied.  See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

import org.apache.shiro.SecurityUtils;
import org.apache.shiro.authc.*;
import org.apache.shiro.config.IniSecurityManagerFactory;
import org.apache.shiro.mgt.SecurityManager;
import org.apache.shiro.session.Session;
import org.apache.shiro.subject.Subject;
import org.apache.shiro.util.Factory;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;


/**
 * Simple Quickstart application showing how to use Shiro's API.
 *
 * @since 0.9 RC2
 */
public class Quickstart {

    private static final transient Logger log = LoggerFactory.getLogger(Quickstart.class);


    public static void main(String[] args) {

        // The easiest way to create a Shiro SecurityManager with configured
        // realms, users, roles and permissions is to use the simple INI config.
        // We'll do that by using a factory that can ingest a .ini file and
        // return a SecurityManager instance:

        // Use the shiro.ini file at the root of the classpath
        // (file: and url: prefixes load from files and urls respectively):
        Factory<SecurityManager> factory = new IniSecurityManagerFactory("classpath:shiro.ini");
        SecurityManager securityManager = factory.getInstance();

        // for this simple example quickstart, make the SecurityManager
        // accessible as a JVM singleton.  Most applications wouldn't do this
        // and instead rely on their container configuration or web.xml for
        // webapps.  That is outside the scope of this simple quickstart, so
        // we'll just do the bare minimum so you can continue to get a feel
        // for things.
        SecurityUtils.setSecurityManager(securityManager);

        // Now that a simple Shiro environment is set up, let's see what you can do:

        // get the currently executing user:
        Subject currentUser = SecurityUtils.getSubject();

        // Do some stuff with a Session (no need for a web or EJB container!!!)
        Session session = currentUser.getSession();
        session.setAttribute("someKey", "aValue");
        String value = (String) session.getAttribute("someKey");
        if (value.equals("aValue")) {
            log.info("Retrieved the correct value! [" + value + "]");
        }

        // let's login the current user so we can check against roles and permissions:
        if (!currentUser.isAuthenticated()) {
            UsernamePasswordToken token = new UsernamePasswordToken("lonestarr", "vespa");
            token.setRememberMe(true);
            try {
                currentUser.login(token);
            } catch (UnknownAccountException uae) {
                log.info("There is no user with username of " + token.getPrincipal());
            } catch (IncorrectCredentialsException ice) {
                log.info("Password for account " + token.getPrincipal() + " was incorrect!");
            } catch (LockedAccountException lae) {
                log.info("The account for username " + token.getPrincipal() + " is locked.  " +
                        "Please contact your administrator to unlock it.");
            }
            // ... catch more exceptions here (maybe custom ones specific to your application?
            catch (AuthenticationException ae) {
                //unexpected condition?  error?
            }
        }

        //say who they are:
        //print their identifying principal (in this case, a username):
        log.info("User [" + currentUser.getPrincipal() + "] logged in successfully.");

        //test a role:
        if (currentUser.hasRole("schwartz")) {
            log.info("May the Schwartz be with you!");
        } else {
            log.info("Hello, mere mortal.");
        }

        //test a typed permission (not instance-level)
        if (currentUser.isPermitted("lightsaber:weild")) {
            log.info("You may use a lightsaber ring.  Use it wisely.");
        } else {
            log.info("Sorry, lightsaber rings are for schwartz masters only.");
        }

        //a (very powerful) Instance Level permission:
        if (currentUser.isPermitted("winnebago:drive:eagle5")) {
            log.info("You are permitted to 'drive' the winnebago with license plate (id) 'eagle5'.  " +
                    "Here are the keys - have fun!");
        } else {
            log.info("Sorry, you aren't allowed to drive the 'eagle5' winnebago!");
        }

        //all done - log out!
        currentUser.logout();

        System.exit(0);
    }
}
```

#### 3.默认shiro.ini文件内容配置



```
[users]
# user 'root' with password 'secret' and the 'admin' role
root = secret, admin

# user 'guest' with the password 'guest' and the 'guest' role
guest = guest, guest

# user 'presidentskroob' with password '12345' ("That's the same combination on
# my luggage!!!" ;)), and role 'president'
presidentskroob = 12345, president

# user 'darkhelmet' with password 'ludicrousspeed' and roles 'darklord' and 'schwartz'
darkhelmet = ludicrousspeed, darklord, schwartz

# user 'lonestarr' with password 'vespa' and roles 'goodguy' and 'schwartz'
lonestarr = vespa, goodguy, schwartz



[roles]
# 'admin' role has all permissions, indicated by the wildcard '*'
admin = *

# The 'schwartz' role can do anything (*) with any lightsaber:
schwartz = lightsaber:*

# The 'goodguy' role is allowed to 'drive' (action) the winnebago (type) with
# license plate 'eagle5' (instance specific id)
goodguy = winnebago:drive:eagle5
```


参考文章：[http://www.turingbook.com/article/details/163](http://www.turingbook.com/article/details/163)








