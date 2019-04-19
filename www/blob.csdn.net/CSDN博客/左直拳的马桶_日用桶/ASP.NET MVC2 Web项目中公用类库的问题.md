# ASP.NET MVC2 Web项目中公用类库的问题 - 左直拳的马桶_日用桶 - CSDN博客
2013年11月23日 17:43:02[左直拳](https://me.csdn.net/leftfist)阅读数：5069
ASP.NET WEB窗体 网站中，加入公用类文件的话，系统会很自动并殷勤的问你，说要不要把它存放在文件夹 App_Code 里。一旦加入，全站都可以很方便地加以使用，一点问题没有。
这种习以为常的方式，在MVC2模式下，却不存在。在MVC2（其他版本的我尚未知道）里，加入一个类文件，系统一生不吭，不会提醒你要将这么个东西放到app_code里，并且在添加ASP.NET文件的菜单命令里，只有App_Data，App_GlobalResources，App_Browsers几个，并没有所谓App_Code。
![](https://img-blog.csdn.net/20131123172339484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
没有不要紧，我们可以自己添加这个文件夹。
但是运行的时候，问题就出来了。
比如说，我们要在控制器传递一个变量给视图，可以采用
ViewData[key]的方式。这个key，必须在控制器，视图里保持一致，否则会搞错；万一要修改，就要两个地方都改一遍。傻瓜都能想到，可以将这个key，定义在公用类里，大家都引用即可。
现在
/app_code/common.cs有代码如下
```
namespace mvc.App_Code
{
    public class Common
    {
        public static readonly string KeyTeacherList = "TeacherList";
    }
}
```
//控制器
```
public ActionResult Teacher()
        {
            College.Bll.Teacher bll = new College.Bll.Teacher();
            DataTable dt  = bll.GetData();
            ViewData[common.KeyTeacherList] = dt;
            return View(dt);
        }
```
//视图
![](https://img-blog.csdn.net/20131123173820796?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
结果一运行就出问题了：
![](https://img-blog.csdn.net/20131123173951218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVmdGZpc3Q=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
究其原因，应该是MVC项目编译之后，这个类存在于统一的项目dll里，然后代码这里又有源代码，因此冲突了。
解决的办法是将公用的类放在项目外，编译成独立的DLL，然后项目加以引用，问题解决。
==============================================================
这似乎是App_Code这个文件夹名字导致的问题。只要改成别的名字，就可以？
==============================================================
有关MVC中，app_code里的类无法找到，无法引用的问题，解决之道是
选中你的类文件，在属性中”高级“中的”生成操作“默认”内容“要改为编译就可以了。
![](https://img-blog.csdn.net/20151016113851836?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
参考文章：
http://www.cnblogs.com/netlover/archive/2011/04/17/AspNet_App_Code.html
