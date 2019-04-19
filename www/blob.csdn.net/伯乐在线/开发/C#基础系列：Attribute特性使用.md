# C#基础系列：Attribute特性使用 - 文章 - 伯乐在线
原文出处： [懒得安分](http://www.cnblogs.com/landeanfen/p/4642819.html)
前言：总结了下反射得基础用法，这章我们来看看C#的另一个基础技术——特性。
1、什么是特性：就博主的理解，特性就是在类的类名称、属性、方法等上面加一个标记，使这些类、属性、方法等具有某些统一的特征，从而达到某些特殊的需要。比如：方法的异常捕捉，你是否还在某些可能出现异常的地方（例如数据库的操作、文件的操作等）经常使用try…catch。这个时候如果使用特性，就可以大大减少方法里面的try…catch的使用。你只需要定义一个专门捕捉异常的特性类ExceptionExAttribute，然后给这个特性类做些特殊处理，比如给它增加一个AOP拦截的功能（AOP拦截的方式很多，有兴趣可以搜搜看，园子里面很多类似的文章）。那么在可能出现异常的方法名称上面加上一个[ExceptionEx]特性标签，这个方法就具有自动捕捉异常的能力。还是加上官方定义：
特性提供功能强大的方法，用以将元数据或声明信息与代码（程序集、类型、方法、属性等）相关联。 reflection.” data-guid=”716c0768f610f38427afe934e71f1d47″ style=”margin: 0px; padding: 0px;”>特性与程序实体关联后，即可在运行时使用名为“反射”的技术查询特性。
特性具有以下属性：
- 特性可向程序中添加元数据。 Metadata is information about the types defined in a program.” data-guid=”01e24c05eb8a815d9d031ae958a894ae” style=”margin: 0px; padding: 0px;”>元数据是有关在程序中定义的类型的信息。 所有的 .NET 程序集都包含指定的一组元数据，这些元数据描述在程序集中定义的类型和类型成员。 可以添加自定义特性，以指定所需的任何附加信息。
- 可以将一个或多个特性应用到整个程序集、模块或较小的程序元素（如类和属性）。
- 特性可以与方法和属性相同的方式接受参数。
- 程序可以使用反射检查自己的元数据或其他程序内的元数据。
（以上来自MSDN）
2、为什么需要特性：这个上面已经简单介绍过，特性能大大减少统一需求的代码量。其他不说，至少它能让我们的代码看上去更大气点吧~~
3、特性的使用：博主这次还是打算从三个方便分别介绍下特性的常规使用方法。当然这几种方式都是博主原来用过的，可能不是最好的举例场景，但是也算比较典型的特性用法吧。
**（1）类的属性上面特性的用法：**
之所以将这个放在最前面介绍是因为博主最近做的一个BS项目正好用到，并且使用场景也比较典型。首先介绍下使用场景：最近项目有一个需求，BS界面需要一个拖拽的功能。如下图
![c#基础系列](http://jbcdn2.b0.upaiyun.com/2016/02/976b922d96abd4798c49ea59af2aa537.png)
当将左边的3个div拖到右边来时，每个div都有自己的特有属性，比如2部门拖过来时，要显示如下属性：
![c#基础系列](http://jbcdn2.b0.upaiyun.com/2016/02/5af719b7a40894952b74c099b3f52424.png)
设计思路：每个div对应的Model，每个Model里面有自己特有的属性，然后属性上面加上特性显示属性的名称和默认值，以及界面应该呈现的html标签。
实现代码：
首先来看自定义的一个特性类：

C#
```
public class DetailAttribute : Attribute
 {
        public string AttrName { set; get; }
        public string Html { set; get; }
        public string DefaultValue { set; get; }
        public string DataSource { set; get; }
 }
```
对应的Model：

C#
```
public class Factory
    {
        [Detail(AttrName="宽度", Html="<input type='text' />", DefaultValue="50", DataSource=null)]
        public string Width { set; get; }
        [Detail(AttrName = "高度", Html = "<input type='text' />", DefaultValue = "50", DataSource = null)]
        public string Height { set; get; }
        [Detail(AttrName = "状态", Html = "<select></select>", DefaultValue = null, DataSource = "select text,value from status")]
        public string Status { set; get; }
        [Detail(AttrName = "Tag值", Html = "<input type='text' />", DefaultValue = "", DataSource = null)]
        public string Tag { set; get; }
    }
    public class FactoryDetail
    {
        [Detail(AttrName = "宽度", Html = "<input type='text' />", DefaultValue = "50", DataSource = null)]
        public string Width { set; get; }
        [Detail(AttrName = "高度", Html = "<input type='text' />", DefaultValue = "50", DataSource = null)]
        public string Height { set; get; }
        [Detail(AttrName = "状态", Html = "<select></select>", DefaultValue = null, DataSource = "select text,value from status")]
        public string Status { set; get; }
        [Detail(AttrName = "Tag值", Html = "<input type='text' />", DefaultValue = "", DataSource = null)]
        public string Tag { set; get; }
        [Detail(AttrName = "描述", Html = "<input type='text' />", DefaultValue = "", DataSource = null)]
        public string Desc { set; get; }
    }
```
然后在界面的拖放事件结束时通过js发送ajax请求来得到界面要呈现的html：

XHTML
```
$(".jq-draggable-outcontainer").draggable({
        helper: "clone",
        scroll: true,
        drag: function (event, ui) {
            //   debugger;
        }
    });
    $("#content").droppable({
        drop: function (event, ui) {
            //  debugger;
            if (ui.draggable[0].className.indexOf("jq-draggable-outcontainer") > 0) {
                var text = ui.draggable[0].innerText.trim();
                $(this).append('<div class="window jq-draggable-incontainer" onclick="GetPropertiesByType(\'1\',this)" style="position:absolute;left:' +(event.clientX-20) + 'px;top:' + (event.clientY-20) + 'px" id="window' + iIndex + '"><strong>' + text + '</strong></div>');
                $("#content2").html("");
                cur_selector = $("#window"+iIndex);
                $.Ewin.AjaxPost("/Home/GetModelByType", { strType: "Factory" }, function (data, status) {
                    var element = $.parseJSON(data.Json);
                    var arrProp = element.element.property;
                    //0.构造html
                    var strHtml = "<div style='float:right;padding-top:0px;width:300px;height:auto;'><table cellpadding='5' border='1'>";
                    //1.拼html构造属性
                    strHtml += "</table></div>";
                    $("#content2").append(strHtml);
                }, function () {
                }, null);
                iIndex++;
            }
        }
    });
```
对应的C#方法：

C#
```
public JsonResult GetModelByType(string strType)
        {
　　　　　　　//strType传过来的是Factory或者FactoryDetail
            var assembly = Assembly.Load("Ewin.Client.Web");//参数为程序集的名称
            var oType = assembly.GetType("Ewin.Client.Web.Controllers." + strType);
　　　　　　  //得到类的所有属性
            var lstProperties = oType.GetProperties();
            foreach (var oProperty in lstProperties)
            {
　　　　　　　　   //得到每一个属性的特性类集合
                IList<CustomAttributeData> lstAttr = oProperty.GetCustomAttributesData();
                foreach (var oAttr in lstAttr)
                {
　　　　　　　　　　　　//得到每一个特性类的全称
                    Console.WriteLine("特性类的名称" + oAttr.AttributeType.FullName);
                    Console.WriteLine("特性类成员如下：");
　　　　　　　　　　　　//得到特性类的所有参数
                    var lstAttrArgu = oAttr.NamedArguments;
                    foreach (var oAttrAru in lstAttrArgu)
                    {
　　　　　　　　　　　　　　//取每个特性类参数的键值对
                        Console.WriteLine(oAttrAru.MemberName + "=" + oAttrAru.TypedValue.Value);
                    }
                    //Console.WriteLine(oAttr.AttributeType+"——"+oAttr.NamedArguments);
                }
            }
            return Json(new { }, JsonRequestBehavior.AllowGet);
        }
```
GetModelByType方法结果简单构造下然后将属性的键值对返回给js方法，然后再由js追加到界面上面。这样通过特性和反射的结合能很快完成这个小功能的设计。
**（2）类的方法上面特性的用法：**
这个用法.Net framework里面就很多，如果MVC里面Filter过滤器的用法：

C#
```
public class SuperLogStat : ActionFilterAttribute
    {
        //模块名称
        private EnumModuleName moduleEnum = EnumModuleName.ModuleOther;
        //功能名称
        private string functionName = string.Empty;
        //用户Id
        private string userId = string.Empty;
        public string Version
        {
            get { return ConfigurationManager.AppSettings["UploatStatVersion"]; }
        }
        public EnumModuleName ModuleEnum
        {
            get { return this.moduleEnum; }
            set { this.moduleEnum = value; }
        }
        public string FunctionName
        {
            get { return this.functionName; }
            set { this.functionName = value; }
        }
　　   //这两个方法都是父类的virtual方法，一个再return View()之前执行，一个再之后执行
        //
        // 摘要:
        //     在执行操作方法之前由 MVC 框架调用。
        //
        // 参数:
        //   filterContext:
        //     筛选器上下文。
        public override void OnActionExecuting(ActionExecutingContext filterContext) 
        {
            try
            {
                string userName = filterContext.HttpContext.User.Identity.Name;
                this.userId = userName.Replace("china\\", "");
            }
            catch (Exception)
            {
                this.userId = string.Empty;
            }
        }
        //
        // 摘要:
        //     在执行操作结果之前由 MVC 框架调用。
        //
        // 参数:
        //   filterContext:
        //     筛选器上下文。
        public override void OnResultExecuting(ResultExecutingContext filterContext)
        {
            UserLogUtils.LogUserStatic(this.userId, this.Version,
                EnumUtils.getEnumDescByValue((int)this.moduleEnum, typeof(EnumModuleName)),
                this.functionName);
        }
    }
```
在Controller里面方法上面加上特性：

C#
```
//调用
[SuperLogStat(ModuleEnum = EnumModuleName.ModuleHome, FunctionName = "待审核")]
public ActionResult MyApplyToAuditing()
{
   return View();
}
```
这个ActionFilterAttribute这个特性用法里面就有异常的拦截机制，和前面说的自定义的异常拦截是相同的。
**（3）类上面特性的用法：**
类上面特性的用法其实.Net里面也很多。比如为了避免new一个对象而使用的MEF就是一个很有说服力的例子：
在定义个类实现一个接口时：

C#
```
[Export("Impc_TB_Test", typeof(Ifc_TB_Test))]
 public class Impc_TB_Test : Ifc_TB_Test
 {
    ......
 }
```
定义接口没有任何特殊：

C#
```
public interface Ifc_TB_Test
{
   ......
}
```
然后在使用时只需要加一个[Import]标签，这个变量就会在编译时自动new一个Impc_TB_Test变量：

C#
```
[Import("Impc_TB_Test")]
Ifc_TB_Test service { get; set; }
```
在使用service变量时，就可以直接把它当做一个Impc_TB_Test对象来使用。是不是很方便。
这几种常见用法都是博主用过的觉得比较好的场景，当然特性的用法肯定远不止如此，欢迎大侠们指正拍砖~~
### 本系列：
- [C#基础系列：再也不用担心面试官问我“事件”了](http://blog.jobbole.com/98200/)
- [C#基础系列：扩展方法的使用](http://blog.jobbole.com/98230/)
- [C#基础系列：Linq to Xml读写xml](http://blog.jobbole.com/98243/)
- [C#基础系列：反射笔记](http://blog.jobbole.com/98266/)
- [C#基础系列：小话泛型](http://blog.jobbole.com/98359/)
- [C#基础系列：多线程的常见用法详解](http://blog.jobbole.com/98370/)
- [C#基础系列：委托实现简单设计模式（1）](http://blog.jobbole.com/98378/)
- [C#基础系列：委托和设计模式（2）](http://blog.jobbole.com/98388/)
- [C#基础系列：序列化效率比拼](http://blog.jobbole.com/98397/)
- [C#基础系列：异步编程初探async和await](http://blog.jobbole.com/98405/)
