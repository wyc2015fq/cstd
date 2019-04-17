# REST框架SerfJ - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年10月07日 21:51:09[boonya](https://me.csdn.net/boonya)阅读数：1128标签：[java																[rest																[servlet																[url																[mvc](https://so.csdn.net/so/search/s.do?q=mvc&t=blog)
个人分类：[APIService](https://blog.csdn.net/boonya/article/category/6806709)






From: [http://www.oschina.net/code/snippet_12_1948](http://www.oschina.net/code/snippet_12_1948)


SerfJ 是一个比较简单的框架用来开发Java的REST的Web应用。可帮助你开发优雅的MVC架构的应用，使用惯例重于配置的思路，无需配置文件和注解。


SerfJ 框架的示例代码请看下面（代码分享）：






### 1. [代码]web.xml     [](http://www.oschina.net/code/snippet_12_1948#2846)[](http://www.oschina.net/code/snippet_12_1948#2847)[](http://www.oschina.net/code/snippet_12_1948#2848)[](http://www.oschina.net/code/snippet_12_1948#2849)[](http://www.oschina.net/code/piece_full?code=1948&piece=2846#2846)



[?](http://www.oschina.net/code/snippet_12_1948#)

```
```xml
<
```

```xml
servlet
```

```xml
>
```



```xml

```

```xml
<
```

```xml
servlet-name
```

```xml
>RestServlet</
```

```xml
servlet-name
```

```xml
>
```



```xml

```

```xml
<
```

```xml
servlet-class
```

```xml
>net.sf.serfj.RestServlet</
```

```xml
servlet-class
```

```xml
>
```



```xml

```

```xml
<
```

```xml
load-on-startup
```

```xml
>5</
```

```xml
load-on-startup
```

```xml
>
```



```xml
</
```

```xml
servlet
```

```xml
>
```



```xml

```



```xml
<
```

```xml
servlet-mapping
```

```xml
>
```



```xml

```

```xml
<
```

```xml
servlet-name
```

```xml
>RestServlet</
```

```xml
servlet-name
```

```xml
>
```



```xml

```

```xml
<
```

```xml
url-pattern
```

```xml
>/banks/*</
```

```xml
url-pattern
```

```xml
>
```



```xml
</
```

```xml
servlet-mapping
```

```xml
>
```



```xml

```



```xml
<
```

```xml
servlet-mapping
```

```xml
>
```



```xml

```

```xml
<
```

```xml
servlet-name
```

```xml
>RestServlet</
```

```xml
servlet-name
```

```xml
>
```



```xml

```

```xml
<
```

```xml
url-pattern
```

```xml
>/accounts/*</
```

```xml
url-pattern
```

```xml
>
```



```xml
</
```

```xml
servlet-mapping
```

```xml
>
```
```





### 2. [代码]serfj.properties     [](http://www.oschina.net/code/snippet_12_1948#2846)[](http://www.oschina.net/code/snippet_12_1948#2847)[](http://www.oschina.net/code/snippet_12_1948#2848)[](http://www.oschina.net/code/snippet_12_1948#2849)[](http://www.oschina.net/code/piece_full?code=1948&piece=2847#2847)



[?](http://www.oschina.net/code/snippet_12_1948#)

```
```bash
#
 Main package where looking for classes (controllers, serializers)
```



```bash
main.package=net.sf.serfj.
```

```bash
test
```
```





### 3. [代码]Bank.java     [](http://www.oschina.net/code/snippet_12_1948#2846)[](http://www.oschina.net/code/snippet_12_1948#2847)[](http://www.oschina.net/code/snippet_12_1948#2848)[](http://www.oschina.net/code/snippet_12_1948#2849)[](http://www.oschina.net/code/piece_full?code=1948&piece=2848#2848)



[?](http://www.oschina.net/code/snippet_12_1948#)

```
```java
public
```

```java
class
```

```java
Bank
```

```java
extends
```

```java
RestController
 {
```



```java

```

```java
@GET
```



```java

```

```java
public
```

```java
void
```

```java
index()
 {
```



```java

```

```java
//
 By default, this action redirects to index.jsp (or index.html or index.htm)
```



```java

```

```java
}
```



```java

```



```java

```

```java
@GET
```



```java

```

```java
public
```

```java
void
```

```java
show()
 {
```



```java

```

```java
//
 Gets ID from URL /banks/1
```



```java

```

```java
String
 id =
```

```java
this
```

```java
.getId(
```

```java
"bank"
```

```java
);
```



```java

```



```java

```

```java
//
 Gets account's ID from URL /banks/1/accounts/2
```



```java

```

```java
String
 accountId =
```

```java
this
```

```java
.getId(
```

```java
"account"
```

```java
);
```



```java

```



```java

```

```java
//
 Gets the account
```



```java

```

```java
Account
 account =
```

```java
//
 Code that gets the account 2 from bank 1
```



```java

```



```java

```

```java
//
 Put account into the request so the page will be able to use it
```



```java

```

```java
this
```

```java
.addObject2Request(
```

```java
"account"
```

```java
,
 account);
```



```java

```



```java

```

```java
//
 By default, this action redirects to show.jsp (or show.html or show.htm)
```



```java

```

```java
}
```



```java

```



```java

```

```java
@GET
```



```java

```

```java
public
```

```java
void
```

```java
newResource()
 {
```



```java

```

```java
//
 By default, this action redirects to new.jsp (or new.html or new.htm)
```



```java

```

```java
}
```



```java

```



```java

```

```java
@GET
```



```java

```

```java
public
```

```java
void
```

```java
edit()
 {
```



```java

```

```java
//
 By default, this action redirects to edit.jsp (or edit.html or edit.htm)
```



```java

```

```java
}
```



```java

```



```java

```

```java
@POST
```



```java

```

```java
public
```

```java
void
```

```java
create()
 {
```



```java

```

```java
//
 By default, this action redirects to create.jsp (or create.html or create.htm)
```



```java

```

```java
}
```



```java

```



```java

```

```java
@PUT
```



```java

```

```java
public
```

```java
void
```

```java
update()
 {
```



```java

```

```java
//
 Gets bank's ID
```



```java

```

```java
String
 id =
```

```java
this
```

```java
.getId(
```

```java
"bank"
```

```java
);
```



```java

```



```java

```

```java
Bank
 bank =
```

```java
//
 Code that gets the bank object
```



```java

```



```java

```

```java
//
 Gets new name for the bank
```



```java

```

```java
String
 name =
```

```java
this
```

```java
.getStringParam(
```

```java
"name"
```

```java
);
```



```java

```



```java

```

```java
//
 Updating the bank
```



```java

```

```java
//
 ... Code that updates the bank's information
```



```java

```



```java

```

```java
//
 By default, this action redirects to update.jsp (or update.html or update.htm)
```



```java

```

```java
}
```



```java

```



```java

```

```java
@DELETE
```



```java

```

```java
public
```

```java
void
```

```java
delete()
 {
```



```java

```

```java
//
 By default, this action redirects to delete.jsp (or delete.html or delete.htm)
```



```java

```

```java
}
```



```java

```



```java

```

```java
@GET
```



```java

```

```java
public
```

```java
void
```

```java
someAction()
 {
```



```java

```

```java
//
 By default, this action redirects to someAction.jsp (or someAction.html or someAction.htm)
```



```java

```

```java
}
```



```java
}
```
```





### 4. [代码]Account.java     跳至[[1]](http://www.oschina.net/code/snippet_12_1948#2846)[[2]](http://www.oschina.net/code/snippet_12_1948#2847)[[3]](http://www.oschina.net/code/snippet_12_1948#2848)[[4]](http://www.oschina.net/code/snippet_12_1948#2849)[[全屏预览]](http://www.oschina.net/code/piece_full?code=1948&piece=2849#2849)



[?](http://www.oschina.net/code/snippet_12_1948#)

```
```java
public
```

```java
class
```

```java
Account
```

```java
extends
```

```java
RestController
 {
```



```java

```

```java
@GET
```



```java

```

```java
public
```

```java
void
```

```java
index()
 {
```



```java

```

```java
//
 By default, this action redirects to index.jsp (or index.html or index.htm)
```



```java

```

```java
}
```



```java

```



```java

```

```java
@GET
```



```java

```

```java
public
```

```java
void
```

```java
show()
 {
```



```java

```

```java
//
 By default, this action redirects to show.jsp (or show.html or show.htm)
```



```java

```

```java
}
```



```java

```



```java

```

```java
@GET
```



```java

```

```java
public
```

```java
void
```

```java
newResource()
 {
```



```java

```

```java
//
 By default, this action redirects to new.jsp (or new.html or new.htm)
```



```java

```

```java
}
```



```java

```



```java

```

```java
@GET
```



```java

```

```java
public
```

```java
void
```

```java
edit()
 {
```



```java

```

```java
//
 By default, this action redirects to edit.jsp (or edit.html or edit.htm)
```



```java

```

```java
}
```



```java

```



```java

```

```java
@POST
```



```java

```

```java
public
```

```java
void
```

```java
create()
 {
```



```java

```

```java
//
 By default, this action redirects to create.jsp (or create.html or create.htm)
```



```java

```

```java
//
 But I want to render another page!... easy
```



```java

```

```java
this
```

```java
.renderPage(
```

```java
"mypage.jsp"
```

```java
);
```



```java

```

```java
}
```



```java

```



```java

```

```java
@PUT
```



```java

```

```java
public
```

```java
void
```

```java
update()
 {
```



```java

```

```java
//
 By default, this action redirects to update.jsp (or update.html or update.htm)
```



```java

```

```java
//
 But I want to render another page... from another controller!... easy
```



```java

```

```java
this
```

```java
.renderPage(
```

```java
"bank"
```

```java
,
```

```java
"another_page.jsp"
```

```java
);
```



```java

```

```java
}
```



```java

```



```java

```

```java
@DELETE
```



```java

```

```java
public
```

```java
void
```

```java
delete()
 {
```



```java

```

```java
//
 By default, this action redirects to delete.jsp (or delete.html or delete.htm)
```



```java

```

```java
//
 Well, if something happens, I want to redirect to mypage.jsp
```



```java

```

```java
if
```

```java
(somethingHappens)
 {
```



```java

```

```java
this
```

```java
.renderPage(
```

```java
"mypage.jsp"
```

```java
);
```



```java

```

```java
}
```

```java
else
```

```java
{
```



```java

```

```java
//
 Default page
```



```java

```

```java
this
```

```java
.renderPage();
```



```java

```

```java
}
```



```java

```

```java
}
```



```java

```



```java

```

```java
/**
```



```java

```

```java
*
 If this method is called as /accounts/1/accountBalance.xml, then the balance object will
```



```java

```

```java
*
 be serialized as an XML, whereas if it's called as /accounts/1/accountBalance.json, the
```



```java

```

```java
*
 object will be serialized as a JSON object.
```



```java

```

```java
*/
```



```java

```

```java
@POST
```



```java

```

```java
public
```

```java
Balance
 accountBalance() {
```



```java

```

```java
//
 Gets account's Id
```



```java

```

```java
String
 id =
```

```java
this
```

```java
.getId(
```

```java
"account"
```

```java
);
```



```java

```



```java

```

```java
//
 Calculate balance
```



```java

```

```java
BalanceManager
 manager =
```

```java
new
```

```java
BalanceManager();
```



```java

```

```java
Balance
 balance = manager.getBalance(id);
```



```java

```

```java
this
```

```java
.serialize(balance);
```



```java

```

```java
}
```



```java
}
```
```






举报](https://so.csdn.net/so/search/s.do?q=url&t=blog)](https://so.csdn.net/so/search/s.do?q=servlet&t=blog)](https://so.csdn.net/so/search/s.do?q=rest&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)




