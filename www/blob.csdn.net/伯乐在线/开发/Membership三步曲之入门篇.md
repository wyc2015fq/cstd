# Membership三步曲之入门篇 - 文章 - 伯乐在线
原文出处： [腾飞（Jesse)（博客园）](http://www.cnblogs.com/jesse2013/p/membership.html)
　　Membership 三步曲之入门篇 – Membership基础示例
Membership是ASP.NET为我们提供的一套验证和存储用户凭证的框架。它可以帮助我们快速的开发用户登录、管理以及权限验证相关的模块，很多人可能会说Membership开发起来确实很快，但是总是和需求有那么一点不匹配，那我们就要用到Membership的自定义扩展功能了，如果真正掌握了Membership的话，对于我们快速开发还是有很大帮助的，更重要的是Membership是一套可灵活扩展的框架，包括里面的Provider模型对于想学习架构的同学来说是一个不错的案例，那么接下来我们就来一步一步的看看Membership是如何做到的。
入门篇会从0开始将Membership集成到一个空的MVC站点中，并且与ASP.NET的权限管理体系相结合。进阶篇将剖析Membership的设计理念以及它的结构。高级篇我们将会扩展自己的MembershipProvider和RolesProvider，目地是直接利用我们老系统中已经存在的用户表和角色表，也就是用我们已经存在的数据库去集成Membership。
*注意：使用VS2012建立一个Internet的MVC项目，会直接帮助我们生成一个已经集成好Membership的一个项目。但是我们会从头开始自己把Membership加入到一个空的项目中，这样更有利于我们理解它的工作原理 :)。*
# 目录
- [Membership类成员介绍](#introduction)
- [集成Membership到ASP.NET MVC网站中](#integrate-into-mvc)
- [为Membership启用角色管理](#integrate-with-roles)
# Membership类成员介绍
一般来讲我们的网站要实现的与用户相关的最基本功能包括：注册，登录，修改用户资料和密码。Membership为我们提供了以下几个类来帮助我们完成这些功能。
![](http://jbcdn2.b0.upaiyun.com/2016/05/809a7d508d54595c62e0192ddda46481.png)
在System.Web.Security的命名空间包含了上图中的8个类，但是实际上我们只需要Membership一个类就可以帮助我们完成大部份的功能。接下来我们就来一步一步的将Membership集成到我们的MVC站点中去。
# 集成Membership到ASP.NET MVC网站中
这里我们的需求很简单，只是要实现一个包含注册、登录、以及修改密码功能的网站就可以了。我们接下来要做的就是建立一个基本的MVC站点，然后再配置Membership，加上我们的功能代码。
## 2.1 新建一个基本的MVC站点
![](http://jbcdn2.b0.upaiyun.com/2016/05/dbac76dbeada11d619ba212fd6010d59.png)
![](http://jbcdn2.b0.upaiyun.com/2016/05/5adfd3ea26aac39391a1d01b873318a3.png)
我们的站点默认已经启用[Forms](http://www.cnblogs.com/fish-li/archive/2012/04/15/2450571.html)认证，如果不了解Foms的话，园子里面有一篇比较好的文章，我已经加上链接了。但是Forms有安全方面的隐患，这个系列的后面我们会对比各种认证方式，有兴趣的同学请继续关注。
同时，还为我们默认配置了一个MembershipProvider(我们下面会说这个东西，这里暂时忽略也没有关系）
![](http://jbcdn2.b0.upaiyun.com/2016/05/1718aa488d9285fafa70c8d7db8ce54c.png)
## 2.2 添加功能代码
虽然说我们建立的这个项目已经配置好了Membership，但是并没有地方去使用它。我们没有Controller也没有View。所以接下来我们就要开始加入调用Membership的代码了。
首先我们在Layout里面加上三个链接：Register, Login, 如果已经登录的话我们就显示用户名和LogOff。　![](http://jbcdn2.b0.upaiyun.com/2016/05/d3fa160383eb93f45a0f21ab60611e0b.png)
当然我们还得加上HomeController并且添加一个默认的Index Action。然后我们再加上AccountController并且添加4个Action（Login和Register的Get、Post版本），View的代码我就不贴了，大家可以到未尾下载整个项目的源码。
*登录代码*

```
public ActionResult Login()
{
    return View();
}
[HttpPost]
[AllowAnonymous]
[ValidateAntiForgeryToken]
public ActionResult Login(LoginModel model, string returnUrl)
{
    // Membership.ValidateUser 判断用户名和密码是否正确
    if (ModelState.IsValid & Membership.ValidateUser(model.UserName, model.Password))
    {
        // 调用Forms 的登录 User.Identity.IsAuthenticated 将设置为True
        // User.Identity.Name 会设置成我们下面的UserName
        FormsAuthentication.SetAuthCookie(model.UserName, false);
        return RedirectToLocal(returnUrl);
    }
    ModelState.AddModelError("", "The user name or password provided is incorrect.");
    return View(model);
}
```
*注册相关代码*

```
public ActionResult Register()
{
    return View();
}
[HttpPost]
[AllowAnonymous]
[ValidateAntiForgeryToken]
public ActionResult Register(RegisterModel model)
{
    if (ModelState.IsValid)
    {
        try
        {   
            // Membership.CreateUser去创建用户
            var user = Membership.CreateUser(model.UserName, model.Password);
            if (user != null)
            {
                //注册完成之后直接登录用户
                FormsAuthentication.SetAuthCookie(user.UserName, false);
            }
            return RedirectToAction("Index", "Home");
        }
        catch (MembershipCreateUserException e)
        {
            ModelState.AddModelError("", ErrorCodeToString(e.StatusCode));
        }
    }
    return View(model);
}
```
*登出相关代码*

```
[HttpPost]
[ValidateAntiForgeryToken]
public ActionResult LogOff()
{
    FormsAuthentication.SignOut();
    return RedirectToAction("Index", "Home");
}
```
*修改密码相关代码*

```
[HttpPost]
[ValidateAntiForgeryToken]
public ActionResult LogOff()
{
    FormsAuthentication.SignOut();
    return RedirectToAction("Index", "Home");
}
[Authorize]
public ActionResult Manage()
{
    return View();
}
/**************************************************/
/*注意我们为Manage这两个Action加上了Authorize标签，只有登录过的用户才能访问
  如果用户没有登录直接访问/Account/Manage会被跳到/Account/Login要求登录*/
[Authorize]
[HttpPost]
[ValidateAntiForgeryToken]
public ActionResult Manage(LocalPasswordModel model)
{
    if (ModelState.IsValid)
    {
        bool changePasswordSucceeded;
        try
        {
            var user = Membership.GetUser(User.Identity.Name);
            changePasswordSucceeded = user.ChangePassword(model.OldPassword,
                model.NewPassword);
        }
        catch (Exception)
        {
            changePasswordSucceeded = false;
        }
        if (changePasswordSucceeded)
        {
            return RedirectToAction("Manage", new { Message = "修改密码成功！"});
        }
        else
        {
            ModelState.AddModelError("", "老密码不正确或新密码不符合要求");
        }
    }
    return View(model);
}
```
就是这么简单，我们的注册，登录，以及修改密码功能就完成了。
![](http://jbcdn2.b0.upaiyun.com/2016/05/9ae2e0786093a88d62b9a3b65a163a88.png)
来看一下我们用到了哪些Membership的功能：
- Membership.ValidateUser 验证用户名和密码实现登录
- Membership.CreateUser 创建新用户
- Membership.GetUser 获取用户
- MembershipUser.ChangePassword 修改密码
还有更多的方法我们可以使用，在最上面的表中我们已经列出来了，大家可以自行尝试。这些功能都出来以后，我们还有一个疑问没有解答，那就是我们的用户保存到哪里去了？　![](http://jbcdn2.b0.upaiyun.com/2016/05/bf40c258018902eec61055c13957ae4c.png)
Membership的配置节点中有一个connctionStringName的属性，可以指定我们的数据库连接字符串。那我们就来看一下它都帮我们搞了一个什么样的数据库。**注：**这个数据库是Membership帮我们生成的，我之前并没有创建它，也就是我们只需要写好连接字符串就可以了。
而我们的用户信息就保存在Users表和Memberships表中。
![](http://jbcdn2.b0.upaiyun.com/2016/05/64f880cfe8be651d6d07a458b01c6c5c.png)
# 为Membership启用角色管理
我们已经完成了登录、注册和修改密码的功能。登录属性认证的范畴，而与认证如影随形的还是授权。通俗的讲，认证是看这个用户是不是合法的，像一扇门，你放不放穿上用户进来。而授权是在他进来之后能够干什么，他可以坐在椅子上，但绝对不能坐在你头上，又或者你只允许你爸或者你老婆坐在你头上，这就是授权了。
相信很多人都自己写过权限相关的代码，也有人会写出一套权限管理的框架来。其实ASP.NET已经有一套比较成熟的权限体系，加上它的自定义功能基本可以满足我们大多数的需求了。
在我们创建这个MVC站点的时候，VS也为我们配置了权限模块。就在挨着membership结点的下方：
![](http://jbcdn2.b0.upaiyun.com/2016/05/b84973075ce08247b03a7fb6ace41194.png)
因为我们这里面的连接字符串与我们的membersihp中用到的是同一个，所以和权限相关的表Roles、UserInRoles也创建到了同一个数据库中。不一样的是我们不需要写一行代码就可以完成角色管理的功能，包括创建/修改/删除角色，以及给用户赋予角色。
通过项目->ASP.NET 配置就可以打开这个小网站去管理我们的角色（抱歉，我用的英文版 – -! )。
![](http://jbcdn2.b0.upaiyun.com/2016/05/21dcb5241f87c7fc7e57bfaa9f76121d.png)
![](http://jbcdn2.b0.upaiyun.com/2016/05/e2e773fc2191cc30edaefcebb8e62ec3.png)
我们可以点击Security（安全）到管理用户和角色的地页面。
![](http://jbcdn2.b0.upaiyun.com/2016/05/ab20bc18d551b0420f05a8258d075756.png)
我们先添加3个角色：Admin, Manager, User
![](http://jbcdn2.b0.upaiyun.com/2016/05/f868ce6369bdf350e35ee6e3c428526d.png)
然后我们就可以给我们的用户赋予角色了。![](http://jbcdn2.b0.upaiyun.com/2016/05/64a3d1b97f0e6c4ba017e55cdd20eef7.png)
接下来我们就可以在代码中使用User.IsInRole方法是看用户是否具有某一个角色的权限了。在MVC的Action上我们可以直接在Authorize标签中指定某个角色，那么这个Action就只有这个角色下的用户才能够访问了。

```
[Authorize(Roles="Admin")]
public ActionResult Manage()
{
    return View();
}
```
我们还可以通过调用Roles.GetRolesForUser将这个用户相关的角色的都取出来。
![](http://jbcdn2.b0.upaiyun.com/2016/05/e462c2778522a180386263d4741f7a13.png)
![](http://jbcdn2.b0.upaiyun.com/2016/05/e9f9ce819982e79581502c8f381175be.png)
现在我们就已经为我们的网站添加认证和授权模块了，分分搞定啊！功能是完成了，但是还留下了不少的问题需要解决，因为大多数情况下我们的User表都不会这么简单，要多加一些字段怎么办？ 我们使用了EF需要使用User entity怎么办？又或者角色是有父子关系的又该如何做呢？我们后面的进阶篇将会告诉你答案。
Demo下载：[http://pan.baidu.com/s/1gduY5CR](http://pan.baidu.com/s/1gduY5CR)（你点或不点，推荐按钮就在右下角 ！）
