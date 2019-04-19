# 您必须先调用“WebSecurity.InitializeDatabaseConnection”方法。。。 - 左直拳的马桶_日用桶 - CSDN博客
2014年01月25日 17:08:59[左直拳](https://me.csdn.net/leftfist)阅读数：5333
有如下代码：
```
[Authorize]
        public ActionResult Index()
        {
            ViewBag.ShowList = ShowList.GetShowList(WebSecurity.CurrentUserId);
            return View();
        }
```
一运行，报错：
## *您必须先调用“WebSecurity.InitializeDatabaseConnection”方法，然后再调用"WebSecurity"类的任何其他方法。此调用应当放在站点根目录的 _AppStart.cshtml 文件中。*
原来要加上特性
```
[Authorize]
        [InitializeSimpleMembership]
        public ActionResult Index()
        {
            ViewBag.ShowList = ShowList.GetShowList(WebSecurity.CurrentUserId);
            return View();
        }
```
如果一个个ACTION地加，也可以加在Controller上
```
[InitializeSimpleMembership]
    public class UserController : Controller
    {
        //
        // GET: /User/
        
        [Authorize]
        public ActionResult Index()
        {
            ViewBag.ShowList = ShowList.GetShowList(WebSecurity.CurrentUserId);
            return View();
        }
}
```
#### 解决方法 1:
您可以添加 `[InitializeSimpleMembership]` 到您的控制器而不是为每个操作的顶部，可以使用`WebSecurity.GetUserId(User.Identity.Name)`
或者，您可以使用
`var userId = Membership.GetUser(User.Identity.Name).ProviderUserKey;`
参考文章：
[http://www.itstrike.cn/Question/6a7fbe3b-f44e-430c-94c6-f760c713063](http://www.itstrike.cn/Question/6a7fbe3b-f44e-430c-94c6-f760c713063)

