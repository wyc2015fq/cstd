# ASP.NET Core 开发 - Entity Framework (EF) - 文章 - 伯乐在线
原文出处： [LineZero](http://www.cnblogs.com/linezero/p/EntityFrameworkCore.html)
ASP.NET Core 开发 – Entity Framework (EF) Core,ASP.NET Core 操作数据库。
Entity Framework (EF) Core RC2 也发布了，可以适用于 .NET Core 及ASP.NET Core 。
EntityFrameworkCore SQLite 本篇文章以SQLite 数据库作为介绍。
目前 EF Core 支持的数据库：
Microsoft SQL Server
SQLite
Postgres (Npgsql)
SQL Server Compact Edition
InMemory (for testing purposes)
后面将会增加：
MySQL
IBM DB2
介绍完了，现在正式开始。
新建项目
这里我们选择 ASP.NET Core Web Application (.NET Core)
![1](http://jbcdn2.b0.upaiyun.com/2016/05/28c8edde3d61a0411511d3b1866f063629.png)
这里选择web 应用程序，然后更改身份验证 改为 不进行身份验证![2](http://jbcdn2.b0.upaiyun.com/2016/05/665f644e43731ff9db3d341da5c827e127.png)
**引用Entity Framework (EF) Core**
NuGet官方源已经支持 .NET Core RC2 的相关引用。
然后在 NuGet命令行下安装 ,我们也可以使用NuGet包管理器安装。

Shell
```
Install-Package Microsoft.EntityFrameworkCore.Sqlite –Pre
```
**创建实体**
我们在项目添加一个 Models 文件夹。
新建一个User.cs

C#
```
public class User
    {
        public int Id { get; set; }
        public string UserName { get; set; }
        public string Password { get; set; }
    }
```
这里我为了方便，继续新建 DataContext.cs

C#
```
public class DataContext : DbContext
    {
        public DataContext(DbContextOptions<DataContext> options)
            : base(options)
        {
        }
        public DbSet<User> Users { get; set; }
    }
```
**创建数据库**
打开 Startup.cs 在 ConfigureServices 下添加如下代码：

C#
```
public void ConfigureServices(IServiceCollection services)
        {
            var connection = "Filename=./efcoredemo.db";
            services.AddDbContext<DataContext>(options => options.UseSqlite(connection));
            // Add framework services.
            services.AddMvc();
        }
```
添加好以后，我们来安装 Microsoft.EntityFrameworkCore.Tools

Shell
```
Install-Package Microsoft.EntityFrameworkCore.Tools –Pre
```
安装好以后，我们在 project.json tools 节点下

C#
```
"tools": {
    "Microsoft.EntityFrameworkCore.Tools": {
      "version": "1.0.0-preview1-final",
      "imports": [
        "portable-net45+win8+dnxcore50",
        "portable-net45+win8"
      ]
    },
```
开始创建数据库 使用 dotnet ef
打开文件夹的命令行，
输入

Shell
```
dotnet ef migrations add MyFirstMigration
dotnet ef database update
```
这样我们就创建好了数据库。更多命令请 dotnet ef -h![3](http://jbcdn2.b0.upaiyun.com/2016/05/38026ed22fc1a91d92b5d2ef93540f2024.png)
**项目使用**
新建一个 UserController
然后 在Views 添加一个 User 文件，然后添加对应的视图。
添加一个Register Action，再添加一个 Register 视图

C#
```
@model EFCoreDemo.Models.User
@{ 
    ViewBag.Title = "用户添加";
}
<form asp-controller="User" asp-action="Register" method="post">
    <div class="form-group">
        <label asp-for="UserName" class="col-md-2 control-label">用户名：</label>
        <div class="col-md-10">
            <input class="form-control" asp-for="UserName" />
            <span asp-validation-for="UserName" class="text-danger"></span>
        </div>
        <label asp-for="Password" class="col-md-2 control-label">密码：</label>
        <div class="col-md-10">
            <input class="form-control" asp-for="Password" />
            <span asp-validation-for="Password" class="text-danger"></span>
        </div>
        <div class="col-md-offset-2 col-md-10">
            <input type="submit" value="保存" class="btn btn-default" />
        </div>
    </div>
</form>
```
UserController.cs

C#
```
public class UserController : Controller
    {
        private DataContext  _context;
        public UserController(DataContext context)
        {
            _context = context;
        }
        // GET: /<controller>/
        public IActionResult Index()
        {
            return View(_context.Users.ToList());
        }
        public IActionResult Register()
        {
            return View();
        }
        [HttpPost]
        [ValidateAntiForgeryToken]
        public IActionResult Register(User registeruser)
        {
            if (ModelState.IsValid)
            {
                _context.Users.Add(registeruser);
                _context.SaveChanges();
                return RedirectToAction("Index");
            }
            return View(registeruser);
        }
    }
```
程序运行起来：
http://localhost:5000/User/Register![4](http://jbcdn2.b0.upaiyun.com/2016/05/011ecee7d295c066ae68d4396215c3d020.png)
列表展示
Index.cshtml

C#
```
@model IEnumerable<EFCoreDemo.Models.User>
@{
    ViewBag.Title = "用户";
}
<table class="table">
    <tr>
        <th>Id</th>
        <th>用户名</th>
    </tr>
    @foreach (var item in Model)
    {
        <tr>
            <td>
                @Html.DisplayFor(modelItem => item.Id)
            </td>
            <td>
                @Html.DisplayFor(modelItem => item.UserName)
            </td>
        </tr>
    }
</table>
```
http://localhost:5000/User![5](http://jbcdn2.b0.upaiyun.com/2016/05/4e44f1ac85cd60e3caa56bfd4afb675e20.png)
