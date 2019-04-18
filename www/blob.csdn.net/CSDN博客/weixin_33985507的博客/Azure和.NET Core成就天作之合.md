# Azure和.NET Core成就天作之合 - weixin_33985507的博客 - CSDN博客
2018年05月09日 18:19:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
> \
## 本文要点
\\- ASP.NET Core支持跨平台（window/macOS/Linux），跟Azure托管的Windows和Linux虚拟机非常匹配。\\t
- ASP.NET Core包括一个内置的容器，它支持构造函数注入。\\t
- 在ASP.NET Core应用程序的启动类的ConfigureServices中配置该容器的服务\\t
- Azure资源管理器模板允许脚本化配置Azure虚拟机及其所需的软件\\t
- Azure应用程序服务将服务器维护的底层细节抽象出来，并让开发人员直接部署ASP.NET项目到Azure。\
\\
随着.NET Core 2.0的发布，微软拥有了通用、模块化、跨平台和开源的平台的下一个主要版本，该平台最初发布于2016年。.NET Core已经被创建为拥有很多在目前发布的.NET框架中可用的API。最初创建它的目的是为下一代[ASP.NET](%5C)解决方案提供支持，但是现在成为许多其他场景的基础，包括IoT、云以及下一代移动解决方案。在本系列文章中，我们将探讨.NET Core的优点以及它如何不仅仅有益于传统的.NET开发人员，还有益于所有需要把稳健、高性能和经济的解决方案带入市场的技术人员。
\\
本文是《[.NET Core](%5C)》系列文章的其中之一。您可以通过RSS订阅以接收相关通知。
\\
我最感兴趣的.NET Core的特性之一是跨平台支持，无论是在开发期还是在运行期。对于.NET，您不再被限制于Windows。如今，您可以使用Linux和macOS进行开发和运行应用程序。还有，不再要求开发和运行平台相同，因此，您可以在Mac上开发您的.NET Core应用程序，然后部署到Windows和Linux服务器。
\\
## 微软的云平台Azure
\\
Azure是微软的云平台，能出色地匹配.NET Core应用程序，因为其拥有大量用于托管这些应用程序的基础设施和平台服务，以及广泛的跨平台支持。Azure有一套基础架构服务，提供计算、存储和网络功能，使客户能够像管理传统数据中心的基础架构一样部署虚拟服务器。 这种方法为客户提供对托管应用程序的基础架构和操作系统配置的强大控制。 Azure虚拟机支持多种版本的Windows Server和多种Linux版本，包括Red Hat、CentOS、Suse等。
\\
在把您的.NET Core应用程序部署到Azure之前，您需要在Azure中设置应用程序主机或运行。在Azure中，您可以有很多方法来部署基础架构和服务。最简单容易的入门方法是使用Azure门户网站。从该门户网站，您可以在市场上找到所需的服务，并通过一系列询问向导来配置和部署这些服务。如果您正在配置虚拟机，只要虚拟机处于运行状态了，就可以进行远程管理和配置了，如果运行在Windows中，就使用远程桌面（Remote Desktop）；而如果运行在Linux中，则使用SSH。
\\
如果您跟我一样，是个DevOps的粉丝，也许喜欢自己尽可能地编写脚本，这样就可以做到可重复和精简的。Azure资源管理（Azure Resource Manager，简称ARM）模板允许您在Azure中自动部署服务。ARM模板只是简单的JSON文件，定义了您希望部署的资源以及它们之间的关系。这些ARM模板很受欢迎，并且有个GitHub repo，它含有数百个为许多服务、平台和配置准备的预置模板。您可以找到这些模板和这个GitHub repo。
\\
除了部署和配置Azure服务，您还可以使用ARM模板来配置操作系统，以及使用虚拟机扩展来安装其他依赖项。例如，如果您在Ubuntu Linux上设置一个web服务器，您就需要部署Ubuntu Linux虚拟机，然后部署一个像Apache的web服务器。利用自定义脚本虚拟机扩展，您能够在虚拟机完成部署后执行自定义脚本。利用这些自定义脚本，您可以做很多事，像安装其他服务以及类似Apache及PHP的应用程序服务器。您可以在Azure Quickstart模板GitHub repo中看到一个例子，它是用Apache部署Ubuntu Linux服务器ARM模板。在GitHub上呈现的README.md文件中，您可以点击“Deploy to Azure”按钮，如图1所示，以开始把选定模板部署到您的Azure订阅的过程。一旦您拥有web服务器，就可以在Azure中部署和运行ASP.NET Core应用程序了。
\\
﻿![61d81bd7f4b11ebaeda0a39e427c4257.png](https://static001.infoq.cn/resource/image/61/57/61d81bd7f4b11ebaeda0a39e427c4257.png)﻿
\\
**图1 – 拥有Apache GitHub README.md的Ubuntu**
\\
## 创建一个ASP.NET Core应用程序
\\
现在是时候创建一个我们可以部署到Azure的.NET Core应用程序了。利用Visual Studio 2017，我使用ASP.NET Core创建了一个简单的web API。由于那个新的Hello World!网络应用程序看起来是个待办事项列表，我就创建了一个待办事项列表API。
\\
首先，我在Visual Studio中创建了一个新项目，选择了web类别和ASP.NET Core Web Application模板，如图2所示。
\\
﻿![201528fee14fbdf20f969ad65ec77688.png](https://static001.infoq.cn/resource/image/20/88/201528fee14fbdf20f969ad65ec77688.png)﻿
\\
**图2 – Visual Studio 2017 新ASP.NET Core Web Application**
\\
创建项目之后，我添加了一个定义属性为待办事项列表的模型类，代码如图3所示。我让它很简单，只创建了id的属性和待办事项列表项的名称，以及一个用于跟踪该项是否完成的布尔量。
\\\`public class TodoItem\{\\    public string Id { get; set; }\    public string Name { get; set; }\    public bool IsComplete { get; set; }\}`\\
**图3 – TodoItem.cs模型类**
\\
在创建数据访问类时，我喜欢使用仓储模式，因此，我为待办事项列表存储库创建了一个接口，如图4所示。这定义了所有我需要用于访问数据的方法，包括一个读取单个待办事项的get方法、一个返回所有待办事项列表项的get方法，还有用于添加、更新和删除待办事项列表项的方法。
\\
\\`public interface ITodoItemRepository\{\    TodoItem Get(string id);\    IList\u0026lt;TodoItem\u0026gt; Get();\    void Add(TodoItem item);\    void Update(TodoItem item);\    void Delete(string id);\}\`\\
**图4 – ITodoItemRepository.cs 待办事项列表仓储模式接口**
\\
然后，我用实体框架（Entity Framework，简称EF）创建了待办事项列表项目仓储接口的实现，如图5所示。这包括EF上下文类和使用该EF上下文的仓储类。
\
\\
\\`public class TodoContext : DbContext\{\    public TodoContext(DbContextOptions\u0026lt;TodoContext\u0026gt; options)\        : base(options)\    {\    }\\    public DbSet\u0026lt;TodoItem\u0026gt; TodoItems { get; set; }\\}\public class TodoItemRepository : ITodoItemRepository\{\    private readonly TodoContext _context;\\    public TodoItemRepository(TodoContext context)\    {\        _context = context;\\        if (!_context.TodoItems.Any())\        {\            _context.TodoItems.Add(new TodoItem { Name = \"Item1\" });\            _context.SaveChanges();\        }\    }\\    public TodoItem Get(string id)\    {\        return _context.TodoItems.FirstOrDefault(t =\u0026gt; t.Id == id);\    }\\    public IList\u0026lt;TodoItem\u0026gt; Get()\    {\        return _context.TodoItems.ToList();\    }\\    public void Add(TodoItem item)\    {\        _context.TodoItems.Add(item);\        _context.SaveChanges();\`\\
**图5 – TodoContext.cs和TodoListRepository.cs**
\\
最后，我用图6中所示的代码为待办事项列表web API创建了控制器。该控制器只是使用ITodoItemRepository并执行恰当的数据访问方法。
\
\\
\\`[Produces(\"application/json\")]\[Route(\"api/Todo\")]\public class TodoController : Controller\{\    private ITodoItemRepository _repository;\\    public TodoController(ITodoItemRepository repository)\    {\        _repository = repository;\    }\        \    [HttpGet]\    public IEnumerable\u0026lt;TodoItem\u0026gt; Get()\    {\        return _repository.Get();\    }\        \    [HttpGet(\"{id}\`
