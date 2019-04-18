# WF 4.0中如何实现xaml工作流的动态加载 - weixin_33985507的博客 - CSDN博客
2010年10月01日 22:01:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8
[上一篇](http://www.cnblogs.com/chenxizhang/archive/2010/10/01/1840629.html)，我用实例讲解了在.NET Framework 3.0(3.5)中如何动态加载xoml创建和运行流程的做法。这一篇谈一下在WF 4.0中的情况，首先介绍一下WF 4的一些重要变化
- WF 4中，默认就是用xaml（注意，不是xoml)，同时不允许包含c#代码
- WF 4中，不再区分顺序工作流和状态机工作流
- WF 4中，不再能直接使用Code Activity，如果希望写代码，则需要编写一个自定义的Activity，继承Code Activity
我接下来还是用一个例子讲解一下如何在WF 4中动态加载xaml工作流的做法吧
**1. 创建自定义的Activity**
![image](https://images.cnblogs.com/cnblogs_com/chenxizhang/Windows-Live-Writer/WF-4.0xaml_130A6/image_thumb.png)
```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Activities;
namespace WorkflowConsoleApplication1
{
    public sealed class MyActivity : CodeActivity
    {
        // Define an activity input argument of type string
        public InArgument<string> Text { get; set; }
        // If your activity returns a value, derive from CodeActivity<TResult>
        // and return the value from the Execute method.
        protected override void Execute(CodeActivityContext context)
        {
            // Obtain the runtime value of the Text input argument
            string text = context.GetValue(this.Text);
            Console.WriteLine(text);
        }
    }
}
```
**2.将这个自定义的Activity添加到流程中**
![image](https://images.cnblogs.com/cnblogs_com/chenxizhang/Windows-Live-Writer/WF-4.0xaml_130A6/image_thumb_1.png)
设置它的Text属性
![image](https://images.cnblogs.com/cnblogs_com/chenxizhang/Windows-Live-Writer/WF-4.0xaml_130A6/image_thumb_2.png)
**3.将工作流的属性进行一些修改**
![image](https://images.cnblogs.com/cnblogs_com/chenxizhang/Windows-Live-Writer/WF-4.0xaml_130A6/image_thumb_3.png)
注意，将BuildAction设置为Content，同时Copy to Output Directory 设置为Copy always， 并且将Custom Tool设置为空白
完成操作之后，得到的xaml文件如下
```
<Activity mc:Ignorable="sap" x:Class="WorkflowConsoleApplication1.Workflow1" sap:VirtualizedContainerService.HintSize="240,240" mva:VisualBasic.Settings="Assembly references and imported namespaces for internal implementation" xmlns="http://schemas.microsoft.com/netfx/2009/xaml/activities" xmlns:local="clr-namespace:WorkflowConsoleApplication1" xmlns:mc="http://schemas.openxmlformats.org/markup-compatibility/2006" xmlns:mv="clr-namespace:Microsoft.VisualBasic;assembly=System" xmlns:mva="clr-namespace:Microsoft.VisualBasic.Activities;assembly=System.Activities" xmlns:s="clr-namespace:System;assembly=mscorlib" xmlns:s1="clr-namespace:System;assembly=System" xmlns:s2="clr-namespace:System;assembly=System.Xml" xmlns:s3="clr-namespace:System;assembly=System.Core" xmlns:sad="clr-namespace:System.Activities.Debugger;assembly=System.Activities" xmlns:sap="http://schemas.microsoft.com/netfx/2009/xaml/activities/presentation" xmlns:scg="clr-namespace:System.Collections.Generic;assembly=System" xmlns:scg1="clr-namespace:System.Collections.Generic;assembly=System.ServiceModel" xmlns:scg2="clr-namespace:System.Collections.Generic;assembly=System.Core" xmlns:scg3="clr-namespace:System.Collections.Generic;assembly=mscorlib" xmlns:sd="clr-namespace:System.Data;assembly=System.Data" xmlns:sl="clr-namespace:System.Linq;assembly=System.Core" xmlns:st="clr-namespace:System.Text;assembly=mscorlib" xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml">
  <local:MyActivity sad:XamlDebuggerXmlReader.FileName="D:\temp\WorkflowConsoleApplication1\WorkflowConsoleApplication1\Workflow1.xaml" sap:VirtualizedContainerService.HintSize="200,200" Text="Hello,World" />
</Activity>
```
**4. 通过下面的代码创建并且运行流程**
```
using System;
using System.Linq;
using System.Activities;
using System.Activities.Statements;
using System.Activities.XamlIntegration;
namespace WorkflowConsoleApplication1
{
    class Program
    {
        static void Main(string[] args)
        {
            WorkflowInvoker.Invoke(ActivityXamlServices.Load("workflow1.xaml"));
        }
    }
}
```
运行上述代码，我们会遇到一个错误
![image](https://images.cnblogs.com/cnblogs_com/chenxizhang/Windows-Live-Writer/WF-4.0xaml_130A6/image_thumb_4.png)
这是为什么呢？MyActivity找不到？
我们应该手工将xaml文件成下面这样。请注意粗体的部分，我添加了assembly的设置
```
<Activity mc:Ignorable="sap" x:Class="WorkflowConsoleApplication1.Workflow1" sap:VirtualizedContainerService.HintSize="240,240" mva:VisualBasic.Settings="Assembly references and imported namespaces for internal implementation" xmlns="http://schemas.microsoft.com/netfx/2009/xaml/activities"  xmlns:mc="http://schemas.openxmlformats.org/markup-compatibility/2006" xmlns:mv="clr-namespace:Microsoft.VisualBasic;assembly=System" xmlns:mva="clr-namespace:Microsoft.VisualBasic.Activities;assembly=System.Activities" xmlns:s="clr-namespace:System;assembly=mscorlib" xmlns:s1="clr-namespace:System;assembly=System" xmlns:s2="clr-namespace:System;assembly=System.Xml" xmlns:s3="clr-namespace:System;assembly=System.Core" xmlns:sad="clr-namespace:System.Activities.Debugger;assembly=System.Activities" xmlns:sap="http://schemas.microsoft.com/netfx/2009/xaml/activities/presentation" xmlns:scg="clr-namespace:System.Collections.Generic;assembly=System" xmlns:scg1="clr-namespace:System.Collections.Generic;assembly=System.ServiceModel" xmlns:scg2="clr-namespace:System.Collections.Generic;assembly=System.Core" xmlns:scg3="clr-namespace:System.Collections.Generic;assembly=mscorlib" xmlns:sd="clr-namespace:System.Data;assembly=System.Data" xmlns:sl="clr-namespace:System.Linq;assembly=System.Core" xmlns:st="clr-namespace:System.Text;assembly=mscorlib" xmlns:x="http://schemas.microsoft.com/winfx/2006/xaml">
  <local:MyActivity sad:XamlDebuggerXmlReader.FileName="D:\temp\WorkflowConsoleApplication1\WorkflowConsoleApplication1\Workflow1.xaml" sap:VirtualizedContainerService.HintSize="200,200" Text="Hello,World" />
</Activity>
```
![image](https://images.cnblogs.com/cnblogs_com/chenxizhang/Windows-Live-Writer/WF-4.0xaml_130A6/image_thumb_5.png)
我个人认为这应该算是一个bug。但目前的情况就是这样，如果你的自定义Activity是在当前应用程序里面，则也是需要设置Assembly的信息的。
当然，如果自定义Activity是单独的Assembly,则应该默认就会写上Assembly信息，那种情况反而是没有问题的。
