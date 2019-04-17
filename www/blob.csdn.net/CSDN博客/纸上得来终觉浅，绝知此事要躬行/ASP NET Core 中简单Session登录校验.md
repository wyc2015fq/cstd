# ASP.NET Core 中简单Session登录校验 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年11月27日 15:13:45[boonya](https://me.csdn.net/boonya)阅读数：613








ASP.NET Core 中简单Session登录校验：从Session的配置添加、到请求过滤、再到页面操作。推荐相关阅读：[ASP.NET 会话状态概述](https://msdn.microsoft.com/zh-cn/library/ms178581(v=vs.100).aspx)[ASP.NET Cookie 概述](https://msdn.microsoft.com/zh-cn/library/ms178194(v=vs.100).aspx)[ASP.NET 状态管理建议](https://msdn.microsoft.com/zh-cn/library/z1hkazw7(v=vs.100).aspx)[ASP.NET Core 中的会话和应用状态](https://docs.microsoft.com/zh-cn/aspnet/core/fundamentals/app-state?view=aspnetcore-2.1)

**目录**

[添加Session配置服务](#%E6%B7%BB%E5%8A%A0Session%E9%85%8D%E7%BD%AE%E6%9C%8D%E5%8A%A1)

[启用Session配置](#%E5%90%AF%E7%94%A8Session%E9%85%8D%E7%BD%AE)

[添加用户模型](#%E6%B7%BB%E5%8A%A0%E7%94%A8%E6%88%B7%E6%A8%A1%E5%9E%8B)

[添加登录控制器](#%E6%B7%BB%E5%8A%A0%E7%99%BB%E5%BD%95%E6%8E%A7%E5%88%B6%E5%99%A8)

[控制器基础类](#%E6%8E%A7%E5%88%B6%E5%99%A8%E5%9F%BA%E7%A1%80%E7%B1%BB)

[登录页面视图](#%E7%99%BB%E5%BD%95%E9%A1%B5%E9%9D%A2%E8%A7%86%E5%9B%BE)

[项目结构与测试](#%E9%A1%B9%E7%9B%AE%E7%BB%93%E6%9E%84%E4%B8%8E%E6%B5%8B%E8%AF%95)

## 添加Session配置服务

配置session超时时间30分钟。

```
// This method gets called by the runtime. Use this method to add services to the container.
        public void ConfigureServices(IServiceCollection services)
        {
            services.Configure<CookiePolicyOptions>(options =>
            {
                // This lambda determines whether user consent for non-essential cookies is needed for a given request.
                options.CheckConsentNeeded = context => true;
                options.MinimumSameSitePolicy = SameSiteMode.None;
            });


            services.AddMvc().SetCompatibilityVersion(CompatibilityVersion.Version_2_1);

            // 添加用户Session服务
            //services.AddSession();
            services.AddSession(options =>
            {
                options.IdleTimeout = TimeSpan.FromMinutes(30);
                options.Cookie.HttpOnly = true;
            });
            // 指定Session保存方式:分发内存缓存
            services.AddDistributedMemoryCache();
        }
```

## 启用Session配置

注意放置代码的顺序，Session必须在MVC之前。

```
// This method gets called by the runtime. Use this method to configure the HTTP request pipeline.
        public void Configure(IApplicationBuilder app, IHostingEnvironment env)
        {
            if (env.IsDevelopment())
            {
                app.UseDeveloperExceptionPage();
            }
            else
            {
                app.UseExceptionHandler("/Home/Error");
                app.UseHsts();
            }

            app.UseHttpsRedirection();
            //使用静态文件
            app.UseStaticFiles();
            //Cookie策略
            //app.UseCookiePolicy();
            //Session
            app.UseSession();

            app.UseMvc(routes =>
            {
                routes.MapRoute(
                    name: "default",
                   // template: "{controller=Home}/{action=Index}/{id?}");
                   //template: "{controller=Home}/{action=Server}/{id?}");
                   template: "{controller=Login}/{action=SignIn}/{id?}");
            });
        }
```

## 添加用户模型

```
using System;
using System.Collections.Generic;
using System.ComponentModel.DataAnnotations;
using System.Linq;
using System.Threading.Tasks;

namespace RTVSWeb.Models
{
    public class UserModel
    {
        [Required(ErrorMessage = "用户名不能为空")]
        public string Username { get; set; }

        [Required(ErrorMessage = "密码不能为空")]
        [DataType(DataType.Password)]
        public string Password { get; set; }

        public bool RememberMe { get; set; }
    }
}
```

## 添加登录控制器

此类提供登录校验和退出。

```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using RTVSWeb.Models;
using RTVSWeb.Utils;

// For more information on enabling MVC for empty projects, visit https://go.microsoft.com/fwlink/?LinkID=397860

namespace RTVSWeb.Controllers
{
    public class LoginController : Controller
    {
        // GET: /<controller>/
        public IActionResult SignIn(UserModel userModel)
        {
            if (ModelState.IsValid)
            {
                //检查用户信息
                if (userModel.Username.Equals("rtvsweb") && userModel.Password.Equals("cvnavi2018"))
                {
                    //记录Session
                    HttpContext.Session.Set("User", ByteConvertHelper.Object2Bytes(userModel));
                    //跳转到系统首页
                    return RedirectToAction("Server", "Home");
                }
                ViewBag.ErrorInfo = "用户名或密码错误";
                return View(userModel);
            }
            ViewBag.ErrorInfo = ModelState.Values.First().Errors[0].ErrorMessage;
            return View(userModel);
        }

        public IActionResult SignOut()
        {
            //清除Session
            HttpContext.Session.Clear();
            //跳转到系统登录界面
            return RedirectToAction("SignIn", "Login");
        }
      
    }
}
```

## 控制器基础类

此类是提供给其他需要登录验证的Controller进行继承。

```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using Microsoft.AspNetCore.Mvc;
using Microsoft.AspNetCore.Mvc.Filters;

// For more information on enabling MVC for empty projects, visit https://go.microsoft.com/fwlink/?LinkID=397860

namespace RTVSWeb.Controllers
{
    public class BaseController : Controller
    {
        /// <summary>
        /// 请求过滤处理
        /// </summary>
        /// <param name="filterContext"></param>
        public override void OnActionExecuting(ActionExecutingContext filterContext)
        {
            byte[] result;
            filterContext.HttpContext.Session.TryGetValue("User", out result);
            if (result == null)
            {
                filterContext.Result = new RedirectResult("/Login/SignIn");
                return;
            }
            base.OnActionExecuting(filterContext);
        }
    } 
}
```

## 登录页面视图

/Login/SignIn.cshtml

```
@{
    Layout = null;
}
@model UserModel
<!DOCTYPE html>
<html>
<head>
    <title>系统登录</title>
    <!-- Tell the browser to be responsive to screen width -->
    <meta content="width=device-width, initial-scale=1, maximum-scale=1, user-scalable=no" name="viewport">
    <link rel="stylesheet" href="~/lib/bootstrap/dist/css/bootstrap.css">
    <link rel="stylesheet" href="~/lib/font-awesome/css/font-awesome.css">
    <link rel="stylesheet" href="~/css/AdminLTE.css">
    <link rel="stylesheet" href="~/lib/icheck/skins/square/blue.css">
</head>
<body class="hold-transition login-page">
    <div class="login-box">
        <div class="login-logo">
            <b>RTVS Web服务管理</b>
        </div>
        <!-- /.login-logo -->
        <div class="login-box-body">
            <p class="login-box-msg">系统登录校验</p>
            <!-- <div asp-validation-summary="All" class="text-danger"></div> -->
            <form asp-controller="Login" asp-action="SignIn" method="post">
                <span class="text-danger">@ViewBag.ErrorInfo</span>
                <div class="form-group has-feedback">
                    <input asp-for="Username" type="text" class="form-control" placeholder="用户名">
                    <span class="glyphicon glyphicon-user form-control-feedback"></span>
                    <span asp-validation-for="Username" class="text-danger"></span>
                </div>
                <div class="form-group has-feedback">
                    <input asp-for="Password" type="password" class="form-control" placeholder="密码">
                    <span class="glyphicon glyphicon-lock form-control-feedback"></span>
                </div>
                <div class="row">
                    <div class="col-xs-8">
                        <div class="checkbox icheck">
                            <label>
                                <input asp-for="RememberMe" type="checkbox"> 记住我
                            </label>
                        </div>
                    </div>
                    <!-- /.col -->
                    <div class="col-xs-4">
                        <button type="submit" class="btn btn-primary btn-block btn-flat">登录</button>
                    </div>
                    <!-- /.col -->
                </div>
            </form>
        </div>
        <!-- /.login-box-body -->
    </div>
    <!-- /.login-box -->
    <script src="~/lib/jquery/dist/jquery.js"></script>
    <script src="~/lib/bootstrap/dist/js/bootstrap.js"></script>
    <script src="~/lib/icheck/icheck.js"></script>
    <script>
        $(function () {
            $('input').iCheck({
                checkboxClass: 'icheckbox_square-blue',
                radioClass: 'iradio_square-blue',
                increaseArea: '20%' // optional
            });
        });
    </script>
</body>
</html>
```

## 项目结构与测试

项目结构如下：

![](https://img-blog.csdnimg.cn/20181127151045294.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

 测试效果：

![](https://img-blog.csdnimg.cn/20181127151133781.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181127151210455.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_16,color_FFFFFF,t_70)

参考文章：[http://www.cnblogs.com/fonour/p/5943401.html](http://www.cnblogs.com/fonour/p/5943401.html)



