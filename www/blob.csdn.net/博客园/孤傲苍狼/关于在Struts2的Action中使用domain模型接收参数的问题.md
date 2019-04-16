# 关于在Struts2的Action中使用domain模型接收参数的问题 - 孤傲苍狼 - 博客园



# [孤傲苍狼](https://www.cnblogs.com/xdp-gacl/)

只为成功找方法，不为失败找借口！




## [关于在Struts2的Action中使用domain模型接收参数的问题](https://www.cnblogs.com/xdp-gacl/p/3490201.html)



最近在搭建一个最新的ssh2框架，今天在调试的时候，发现了一个以前一直没有注意过的问题，我在Action中使用域模型的方式去接收jsp画面中的参数的时候，发现参数总是接收不完，头一次遇到这种问题，现在把解决办法写下来

Jsp中的代码如下：

```
1 <form action="TestAction" method="post">
2     用户名：<input type="text" name="user.userName"><br/>
3     密    码：<input type="text" name="user.userPwd"><br/>
4     <input type="submit" value="提交">
5 </form>
```

然后我在Action中定义一个user属性，user中包含了userName和userPwd属性，代码如下：

```
1 /**
2  * 定义UserInfo的域模型，用于接收Form表单中的参数
3  */
4 private UserInfo user;
5 public void setUser(UserInfo user) {
6     this.user = user;
7 }
```



因为我在Action只写了setUser方法，没有写相应的getUser方法，所以在接收参数的时候就出现了如下问题

![](https://images0.cnblogs.com/blog/289233/201312/25104323-02f09b3b66434e24adf525e617f209e8.png)

在调试过程中发现，user域模型只接收到了一个参数，userName的却为空了

![](https://images0.cnblogs.com/blog/289233/201312/25104358-7a11243254684d788c36f5949663bda7.png)

使用了Struts2框架那么久了，还是头一次遇到这个问题，百思不得其解。后来上网查了原因发现，原来是少了对应的getUser方法才导致出现了这个问题，因为struts2对这种混合类型的注入不是一次性全部注入其属性，而是每注入一次下一次就在前台获取obj然后再注入。我们编程时习惯getter/setter 都写，因此很少碰到这个问题。 

在Action中加上对应的getUser方法，代码如下：

```
1 private UserInfo user;
2 **public UserInfo getUser() {3     return user;4 }**5 public void setUser(UserInfo user) {
6     this.user = user;
7 }
```

再次进行接收参数测试

![](https://images0.cnblogs.com/blog/289233/201312/25104756-510612241fba4c5c8bc098bc40cae25c.png)

这次就可以把JSP中填写的用户名和密码都获取下来了，如下图所示：

![](https://images0.cnblogs.com/blog/289233/201312/25104850-5638a344162c44a6b06f7443ff27884b.png)

总结：在Struts2中使用domain模型接收参数时，一定要写上该模型对应的get和set方法，否则就会出现参数接收不完的情况。

在Action中使用域模型接收参数的正确写法范例：

```
1 /**
 2  * 定义UserInfo的域模型，用于接收Form表单中的参数
 3  */
 4 private UserInfo user;
 5 /**
 6  * 对应的get方法
 7  */
 8 public UserInfo getUser() {
 9     return user;
10 }
11 /**
12  * 对应的set方法
13  */
14 public void setUser(UserInfo user) {
15     this.user = user;
16 }
```











