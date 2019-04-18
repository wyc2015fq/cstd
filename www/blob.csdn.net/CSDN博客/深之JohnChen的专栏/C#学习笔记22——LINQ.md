# C#学习笔记22——LINQ - 深之JohnChen的专栏 - CSDN博客

2011年09月03日 11:50:28[byxdaz](https://me.csdn.net/byxdaz)阅读数：1165标签：[linq																[c#																[语言																[编程																[.net																[xml](https://so.csdn.net/so/search/s.do?q=xml&t=blog)](https://so.csdn.net/so/search/s.do?q=.net&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=c#&t=blog)](https://so.csdn.net/so/search/s.do?q=linq&t=blog)
个人分类：[.Net & C#](https://blog.csdn.net/byxdaz/article/category/628880)


**查询表达式(LINQ)简介**

LINQ是Language IntegratedQuery的简称，它是集成在.NET编程语言中的一种特性。已成为编程语言的一个组成部分，在编写程序时可以得到很好的编译时语法检查，丰富的元数据，智能感知、静态类型等强类型语言的好处。并且它同时还使得查询可以方便地对内存中的信息进行查询而不仅仅只是外部数据源。

LINQ定义了一组标准查询操作符用于在所有基于.NET平台的编程语言中更加直接地声明跨越、过滤和投射操作的统一方式，标准查询操作符允许查询作用于所有基于IEnumerable<T>接口的源，并且它还允许适合于目标域或技术的第三方特定域操作符来扩大标准查询操作符集，更重要的是，第三方操作符可以用它们自己的提供附加服务的实现来自由地替换标准查询操作符，根据LINQ模式的习俗，这些查询喜欢采用与标准查询操作符相同的语言集成和工具支持。

我们来总体看看LINQ架构

![](http://hi.csdn.net/attachment/201109/3/0_1315021022S77x.gif)

在.NET3.5下，微软为我们提供了一些命名空间

![](http://hi.csdn.net/attachment/201109/3/0_1315021068QoFO.gif)

LINQ包括五个部分：LINQ to Objects、LINQ to DataSets、LINQ to SQL、LINQ to Entities、LINQ to XML。

LINQ to SQL全称基于关系数据的.NET语言集成查询，用于以对象形式管理关系数据，并提供了丰富的查询功能。其建立于公共语言类型系统中的基于SQL的模式定义的集成之上，当保持关系型模型表达能力和对底层存储的直接查询评测的性能时，这个集成在关系型数据之上提供强类型。

LINQ to XML在System.Xml.LINQ命名空间下实现对XML的操作。采用高效、易用、内存中的XML工具在宿主编程语言中提供XPath/XQuery功能等。

LINQ基本查询操作

**获取数据源 **

在 LINQ 查询中，第一步是指定数据源。像在大多数编程语言中一样，在 C# 中，必须先声明变量，才能使用它。在 LINQ 查询中，最先使用 from 子句的目的是引入数据源 (customers) 和范围变量 (cust)。

//queryAllCustomers is an IEnumerable<Customer>
varqueryAllCustomers = from cust in customers
                        select cust;

范围变量类似于 foreach 循环中的迭代变量，但在查询表达式中，实际上不发生迭代。执行查询时，范围变量将用作对 customers 中的每个后续元素的引用。因为编译器可以推断 cust 的类型，所以您不必显式指定此类型。其他范围变量可由 let 子句引入

**筛选 **

也许最常用的查询操作是应用布尔表达式形式的筛选器。此筛选器使查询只返回那些表达式结果为 true 的元素。使用 where 子句生成结果。实际上，筛选器指定从源序列中排除哪些元素。在下面的示例中，只返回那些地址位于伦敦的 customers。

var queryLondonCustomers = from cust in customers
                           where cust.City == "London"
                           select cust;

您可以使用熟悉的 C# 逻辑 AND 和 OR 运算符来根据需要在 where 子句中应用任意数量的筛选表达式。例如，若要只返回位于“伦敦”AND 姓名为“Devon”的客户，您应编写下面的代码：

where cust.City=="London" && cust.Name== "Devon"

若要返回位于伦敦或巴黎的客户，您应编写下面的代码：

where cust.City == "London" || cust.City == "Paris"

**排序 **

通常可以很方便地将返回的数据进行排序。orderby 子句将使返回的序列中的元素按照被排序的类型的默认比较器进行排序。例如，下面的查询可以扩展为按 Name 属性对结果进行排序。因为 Name 是一个字符串，所以默认比较器执行从 A 到 Z 的字母排序。

var queryLondonCustomers3 = 
    from cust in customers
    where cust.City == "London"
    orderby cust.Name ascending
    select cust;

若要按相反顺序（从 Z 到 A）对结果进行排序，请使用 orderby…descending子句。

**分组 **

使用 group 子句，您可以按指定的键分组结果。例如，您可以指定结果应按 City 分组，以便位于伦敦或巴黎的所有客户位于各自组中。在本例中，cust.City 是键。

// queryCustomersByCity is anIEnumerable<IGrouping<string, Customer>>
  var queryCustomersByCity =
      from cust in customers
      group cust by cust.City;

  // customerGroup is anIGrouping<string, Customer>
  foreach (var customerGroup in queryCustomersByCity)
  {
     Console.WriteLine(customerGroup.Key);
      foreach (Customer customer in customerGroup)
      {
          Console.WriteLine("    {0}", customer.Name);
      }
  }

在使用 group 子句结束查询时，结果采用列表的列表形式。列表中的每个元素是一个具有 Key 成员及根据该键分组的元素列表的对象。在循环访问生成组序列的查询时，您必须使用嵌套的 foreach 循环。外部循环用于循环访问每个组，内部循环用于循环访问每个组的成员。

如果您必须引用组操作的结果，可以使用 into 关键字来创建可进一步查询的标识符。下面的查询只返回那些包含两个以上的客户的组：

// custQuery is an IEnumerable<IGrouping<string,Customer>>
var custQuery=
    from cust in customers
    group cust by cust.City intocustGroup
    where custGroup.Count() > 2
    orderby custGroup.Key
    select custGroup;

**联接 **

联接运算创建数据源中没有显式建模的序列之间的关联。例如，您可以执行联接来查找符合以下条件的所有客户：位于巴黎，且从位于伦敦的供应商处订购产品。在 LINQ 中，join 子句始终针对对象集合而非直接针对数据库表运行。在 LINQ 中，您不必像在 SQL 中那样频繁使用 join，因为 LINQ 中的外键在对象模型中表示为包含项集合的属性。例如，Customer 对象包含 Order 对象的集合。不必执行联接，只需使用点表示法访问订单：

**选择（投影） **

select 子句生成查询结果并指定每个返回的元素的“形状”或类型。例如，您可以指定结果包含的是整个 Customer 对象、仅一个成员、成员的子集，还是某个基于计算或新对象创建的完全不同的结果类型。当 select 子句生成除源元素副本以外的内容时，该操作称为“投影”。使用投影转换数据是 LINQ 查询表达式的一种强大功能。

LinQ to Object 
[http://blog.163.com/qinshan_xy/blog/static/609177292010410103536571/](http://blog.163.com/qinshan_xy/blog/static/609177292010410103536571/Linq)

Linq to sql
[http://www.cnblogs.com/lovecherry/archive/2007/08/13/853754.html](http://www.cnblogs.com/lovecherry/archive/2007/08/13/853754.htmlLINQ)

LINQ to DataSet
[http://developer.51cto.com/art/200908/143770.htm](http://developer.51cto.com/art/200908/143770.htm)
[http://www.cnblogs.com/lincats/archive/2009/05/05/1450169.html](http://www.cnblogs.com/lincats/archive/2009/05/05/1450169.htmlLINQ)

LINQ to XML
[http://jonsion.iteye.com/blog/702489](http://jonsion.iteye.com/blog/702489LINQ)

LINQ to Entity
[http://www.cnblogs.com/Abbey/archive/2011/07/31/2122780.html](http://www.cnblogs.com/Abbey/archive/2011/07/31/2122780.html)


