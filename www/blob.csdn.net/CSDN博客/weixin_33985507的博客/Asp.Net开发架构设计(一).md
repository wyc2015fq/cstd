# Asp.Net开发架构设计(一) - weixin_33985507的博客 - CSDN博客
2012年08月07日 15:14:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：4
本篇我以理论和实践（源代码）两个方面和大家探讨一下我的方案，希望大家多提宝贵意见。
一、软件架构的概念问题，什么是软件的架构?我的理解是：软件的架构包括两个方面的内容，一个是软件的开发架构，一个是软件的部署架构，所谓部署架构就是指部署时的分布式，集群等设计问题；开发架构就是我们平常说的软件分层设计问题，也就是我们今天要谈的问题。
二、何谓分层？分层的方式有几种？分层也就是把一个大的软件解决方案分成多个项目进行开发，分为三种，一种是按照代码的功能层次进行分层，分为数据库访问层，业务逻辑层，UI层等，一种是按照要实现的功能模块进行分层，例如新闻管理层，博客管理层等，第三种就是把前两者结合起来进行分层：先按照代码功能划分好层次，然后再在每一层中分层成各个功能模块。
三、面向接口能够降低各层之间的依赖关系吗？还需要什么？面向接口只是把对对象的直接调用转到了对接口的调用身上，但对接口的调用也需要对象的实例化为前提，因为有对象实例化的存在，所以接口并不能屏蔽掉背后的对象来完成功能；在这种情况下，依赖注入/控制反转等概念应运而生，IOC容器出现了，它能够让对象自动实例化，并管理对象的生存空间。
四、说了这么多，到底应该怎样分？我们不妨从软件的开发过程上来考虑，做软件的都知道我们要进行需求分析，要有需求分析文档，需求分析的任务就是把客户的需求转变为对软件功能的需求，主要内容包括：软件要实现那些功能模块，每个功能模块下包含哪些业务操作，这些业务操作需要哪些页面（或者窗口）支持，这些页面大概都是什么样子的，上面都有什么？有了这些信息，我们就可以开始我们的系统设计了。
我的方案中各个项目的引用关系如下：
![](https://images.cnblogs.com/cnblogs_com/xiaozhuang/BlogsImages/Images20080815/1.JPG)
说明：
Web：UI层，Asp.net
 Web应用程序项目，提供用户界面。
UnityConfig：依赖注入配置层，类库项目，对Service层和Business层进行实现配置以提供给Web调用Service层。
Service：为Web层提供服务，类库项目，里面全是接口。
Business：商业逻辑层，通过调用DA数据访问层和Entities层来实现Service层的服务接口。类库项目。
Entities：实体层，类库项目，存放业务实体，贫血型实体。
请注意：在上面的引用关系中，Web层并没有直接引用Business层。
现在假设我们有这样一个需求文档：它只需要实现一个功能模块，就是查找并显示雇员列表。页面形式如图：
![](https://images.cnblogs.com/cnblogs_com/xiaozhuang/BlogsImages/Images20080815/2.JPG)
首先我们根据需求，先设计服务，因为只有一个功能模块，所以我们只需要一个接口类，再因为只有一个操作功能（查找），所以我们只需要一个服务方法，那么我们在Service层建立一个接口类：IQueryEmployeeService，并添加一个接口方法：QueryEmployee.
根据页面所示，我们这个服务方法需要两个对象参数，一个是查找的条件，一个是查找的结果；这样我们就得到了两个实体类。在Entities建立分别命名为QueryEntry和ListEntry。
代码如下：
namespace Xiaozhuang.Service
{
publicinterfaceIQueryEmployeeService
    {
///<summary>
///查询雇员信息
///</summary>
///<param
 name="queryentity"></param>
///<returns></returns>
List<ListEntry>
 QueryEmployee(QueryEntry queryentry);
    }
}
namespace Xiaozhuang.Entities
{
publicclassQueryEntry
    {
publicstring DepartmentID
 { get; set;
 }
publicstring EmployeeName
 { get; set;
 }
publicstring EmployeeAge
 { get; set;
 }
publicoverridestring ToString()
        {
return"DeaprtmentID:" +
 DepartmentID + "EmployeeName:" + EmployeeName + "EmployeeAge:" +
 EmployeeAge;
        }
    }
}
namespace Xiaozhuang.Entities
{
publicclassListEntry
    {
publicstring EmployeeID
 { get; set;
 }
publicstring EmployeeName
 { get; set;
 }
publicstring EmployeeSex
 { get; set;
 }
publicint EmployeeAge
 { get; set;
 }
publicstring DepartmentName
 { get; set;
 }
publicstring MobilePhone
 { get; set;
 }
publicoverridestring ToString()
        {
return"EmployeeID:" +
 EmployeeID + "EmployeeName:" + EmployeeName + "EmployeeSex:" +
 EmployeeSex;
        }
    }
}
请注意，业务实体的设计完全是参照页面而来，对实体字段类型的设计也是参照页面而来，多数都是String类型，因为这个时候并不知道打算在数据库中怎么存储这些数据。
当然在实际的项目中，因为那个部门列表还需要建立部门的业务实体，这里简单其间略去，有了这些业务实体和服务接口，我们就可以设计数据库了，数据库设计完成后，系统设计的工作就完成了。
接下来就是在Business层具体的实现这个查找的方法了：
namespace Xiaozhuang.Business
{
publicclassQueryEmployeeBusiness :IQueryEmployeeService
    {
        #region IQueryEmployeeService 成员
publicList<ListEntry>
 QueryEmployee(QueryEntry queryentry)
        {
List<ListEntry>
 listEntry = newList<ListEntry>();
ListEntry entry1
 = newListEntry()
 { EmployeeID = "1", EmployeeName = "雇员1",
 EmployeeSex = "男",
 DepartmentName = "部门1",
 EmployeeAge = 30, MobilePhone = "123546789" };
ListEntry entry2
 = newListEntry()
 { EmployeeID = "2", EmployeeName = "雇员2",
 EmployeeSex = "女",
 DepartmentName = "部门2",
 EmployeeAge = 29, MobilePhone = "123546789" };
            listEntry.Add(entry1);
            listEntry.Add(entry2);
return listEntry;
        }
        #endregion
    }
}
此处略去从数据库查询的方法，可以用Linq to sql 实现或者其他Orm工具。
接下来配置一下我们的UnityConfig层，就可以在Web层访问这个查找雇员的服务了。
namespace Xiaozhuang.UnityConfig
{
publicinterfaceIContainerAccessor
        {
IUnityContainer Container
 { get; }
        }
publicclassUnityContainerConfig
        {
publicIUnityContainer GetIUnityContainer()
            {
IUnityContainer container
 = newUnityContainer();
                container.RegisterType<IQueryEmployeeService, QueryEmployeeBusiness>();
return container;
            }
        }
}
当然你也可以把这个配置写到Web。Config中。
接下来的问题就是怎样在Web层调用这个服务的问题了，这个问题其实还是比较复杂的，文章太长了大家都看烦了，我将在下次再详细说说这个问题。
