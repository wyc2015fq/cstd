
# Windows WorkFlow Foundation入门（六）- 编译工作流 - 西西吹雪 - 博客园
# [编程　&　生活      --       西西吹雪](https://www.cnblogs.com/watsonyin/)
开心工作，快乐生活  (原创文章，转载请注明出处)

### 导航
[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/watsonyin/)
[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)
[联系](https://msg.cnblogs.com/send/%E8%A5%BF%E8%A5%BF%E5%90%B9%E9%9B%AA)[订阅](https://www.cnblogs.com/watsonyin/rss)![订阅](//www.cnblogs.com/images/xml.gif)[管理](https://i.cnblogs.com/)
统计
随笔 -		88文章 -		10评论 -		1793引用 -		18
公告

[Windows WorkFlow Foundation入门（六）- 编译工作流](https://www.cnblogs.com/watsonyin/archive/2007/01/23/627951.html)
编译工作流
工作流可以通过以下三种方式被创建：
通过工作流markup文件，使用工作流markup，该文件的后缀为.xoml。通过宿主程序，把该文件传入给工作流运行时引擎，这种方式被称为“无代码工作流创建”，这种方式下，工作流不需要编译。被创建的文件名WorkflowSample.xoml（举例）
另一种方式是，markup文件用来声明工作流，然后与包含逻辑实现的代码一起被编译。这种方式被称为“代码分离工作流创建”。被创建的文件名为：WorkflowSample.xoml 和WorkflowSample.xoml.cs
在代码文件中使用WF对象模型。这种方式称为“仅代码工作流创建”。被创建的文件名为：WorkflowSample.cs 和 WorkflowSample.Designer.cs，这些文件会被编译。
当工作流被编译时，会执行以下过程：
验证工作流活动是否符合为该活动设置的规则，如果验证产生错误，编译器会返回一个错误列表；据传入到编译器中的标记（markup）定义，创建一个partial类；从标记文件产生的partial类和人代码文件产生的partial类，被送入到.net编译器，这个过程的输出结果就是一个.net 程序集。文件名如：WorkflowSample.dll，可以被部署运行工作流了。
在命令行中使用wfc.exe
WF提供了命令行方式的工作流编译器，即wfc.exe。WF同样提供一系列的支持工作流编译公共类型，可以用来开发自定义的工作流编译器。这些公共类型也是wfc.exe所用来的类型。你可以通过WorkflowCompiler类来创建一个自定义的编译器。
你还可以使用WorkflowCompiler类来编译工作流，例如：
WorkflowCompiler compiler=newWorkflowCompiler();
WorkflowCompilerParameters param=newWorkflowCompilerParameters();
compiler.Compile(param,newstring[] {"MainWorkflow.xoml"});
wfc.exe编译选项
wfc命令行的选项如下
Microsoft (R) Windows Workflow Compiler version 3.0.0.0
Copyright (C) Microsoft Corporation 2005. All rights reserved.
Windows Workflow Compiler Options
wfc.exe <XAML file> /target:codegen [/language:...]
wfc.exe <XAML file list> /target:assembly [<vb/cs file list>] [/language:...]
[/out:...] [/reference:...] [/library:...] [/debug:...] [/nocode:...] [/checktypes:...]
- OUTPUT FILE -
/out:<file>             Output file name
/target:assembly        Build a Windows Workflow assembly (default).
Short form: /t:assembly
/target:exe             Build a Windows Workflow application.
Short form: /t:exe
/target:codegen         Generate partial class definition.
Short form: /t:codegen
/delaysign[+|-]         Delay-sign the assembly using only the public portion
of the strong name key.
/keyfile:<file>         Specifies a strong name key file.
/keycontainer:<string>  Specifies a strong name key container.
- INPUT FILES -
<XAML file list>        XAML source file name(s).
<vb/cs file list>       Code file name(s).
/reference:<file list>  Reference metadata from the specified assembly file(s).
Short form is '/r:'.
/library:<path list>    Set of directories where to lookup for the references.
Short form is '/lib:'.
- CODE GENERATION -
/debug[+|-]             Emit full debugging information. The default is '+'.
Short form is '/d:'.
/nocode[+|-]            Disallow code-separation and code-within models.
The default is '-'. Short form is '/nc:'.
/checktypes[+|-]        Check for permitted types in wfc.exe.config file.
The default is '-'. Short form is '/ct:'.
- LANGUAGE -
/language:[cs|vb]       The language to use for the generated class.
The default is 'CS' (C\#). Short form is '/l:'.
/rootnamespace:<string> Specifies the root Namespace for all type declarations.
Valid only for 'VB' (Visual Basic) language.
Short form is '/rns:'.
- MISCELLANEOUS -
/help                   Display this usage message. Short form is '/?'.
/nologo                 Suppress compiler copyright message. Short form is '/n'.
/nowarn                 Ignore compiler warnings. Short form is '/w'.





posted on2007-01-23 14:41[西西吹雪](https://www.cnblogs.com/watsonyin/)阅读(...) 评论(...)[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=627951)[收藏](#)


[刷新评论](javascript:void(0);)[刷新页面](#)[返回顶部](#top)






Powered by:
博客园
Copyright © 西西吹雪
