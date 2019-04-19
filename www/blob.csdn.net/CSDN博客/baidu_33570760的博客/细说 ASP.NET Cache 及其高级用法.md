# 细说 ASP.NET Cache 及其高级用法 - baidu_33570760的博客 - CSDN博客
2017年08月29日 14:27:08[carman_风](https://me.csdn.net/baidu_33570760)阅读数：274
个人分类：[c#																[前端](https://blog.csdn.net/baidu_33570760/article/category/6898158)](https://blog.csdn.net/baidu_33570760/article/category/6898157)
**原文连接：http://www.cnblogs.com/fish-li/archive/2011/12/27/2304063.html**
**阅读目录**
- [开始](http://www.cnblogs.com/fish-li/archive/2011/12/27/2304063.html#_labelStart)
- [Cache的基本用途](http://www.cnblogs.com/fish-li/archive/2011/12/27/2304063.html#_label0)
- [Cache的定义](http://www.cnblogs.com/fish-li/archive/2011/12/27/2304063.html#_label1)
- [Cache常见用法](http://www.cnblogs.com/fish-li/archive/2011/12/27/2304063.html#_label2)
- [Cache类的特点](http://www.cnblogs.com/fish-li/archive/2011/12/27/2304063.html#_label3)
- [缓存项的过期时间](http://www.cnblogs.com/fish-li/archive/2011/12/27/2304063.html#_label4)
- [缓存项的依赖关系 - 依赖其它缓存项](http://www.cnblogs.com/fish-li/archive/2011/12/27/2304063.html#_label5)
- [缓存项的依赖关系 - 文件依赖](http://www.cnblogs.com/fish-li/archive/2011/12/27/2304063.html#_label6)
- [缓存项的移除优先级](http://www.cnblogs.com/fish-li/archive/2011/12/27/2304063.html#_label7)
- [缓存项的移除通知](http://www.cnblogs.com/fish-li/archive/2011/12/27/2304063.html#_label8)
- [巧用缓存项的移除通知 实现【延迟操作】](http://www.cnblogs.com/fish-li/archive/2011/12/27/2304063.html#_label9)
- [巧用缓存项的移除通知 实现【自动加载配置文件】](http://www.cnblogs.com/fish-li/archive/2011/12/27/2304063.html#_label10)
- [文件监视技术的选择](http://www.cnblogs.com/fish-li/archive/2011/12/27/2304063.html#_label11)
- [各种缓存方案的共存](http://www.cnblogs.com/fish-li/archive/2011/12/27/2304063.html#_label12)
许多做过程序性能优化的人，或者关注过程程序性能的人，应该都使用过各类缓存技术。而我今天所说的Cache是专指ASP.NET的Cache，我们可以使用HttpRuntime.Cache访问到的那个Cache，而不是其它的缓存技术。
以前我在[【我心目中的Asp.net核心对象】](http://www.cnblogs.com/fish-li/archive/2011/08/21/2148640.html)这篇博客中简单地提过它，今天我打算为它写篇专题博客，专门来谈谈它，因为它实在是太重要了。在这篇博客中，我不仅要介绍它的一些常见用法，还将介绍它的一些高级用法。在上篇博客[【在.net中读写config文件的各种方法】](http://www.cnblogs.com/fish-li/archive/2011/12/18/2292037.html)的结尾处，我给大家留了一个问题，今天，我将在这篇博客中给出一个我认为较为完美的答案。
本文提到的【延迟操作】方法（如：延迟合并写入数据库）属于我的经验总结，希望大家能喜欢这个思路。
[回到顶部](http://www.cnblogs.com/fish-li/archive/2011/12/27/2304063.html#_labelTop)
# Cache的基本用途
提到Cache，不得不说说它的主要功能：改善程序性能。
ASP.NET是一种动态页面技术，用ASP.NET技术做出来的网页几乎都是动态的，所谓动态是指：页面的内容会随着不同的用户或者持续更新的数据，而呈现出不同的显示结果。既然是动态的，那么这些动态的内容是从哪里来的呢？我想绝大多数网站都有自己的数据源，程序通过访问数据源获取页面所需的数据，然后根据一些业务规则的计算处理，最后变成适合页面展示的内容。
由于这种动态页面技术通常需要从数据源获取数据，并经过一些计算逻辑，最终变成一些HTML代码发给客户端显示。而这些计算过程显然也是有成本的。这些处理成本最直接可表现为影响服务器的响应速度，尤其是当数据的处理过程变得复杂以及访问量变大时，会变得比较明显。另一方面，有些数据并非时刻在发生变化，如果我们可以将一些变化不频繁的数据的最终计算结果（包括页面输出）缓存起来，就可以非常明显地提升程序的性能，缓存的最常见且最重要的用途就体现在这个方面。这也是为什么一说到性能优化时，一般都将缓存摆在第一位的原因。我今天要说到的ASP.NET
 Cache也是可以实现这种缓存的一种技术。不过，它还有其它的一些功能，有些是其它缓存技术所没有的。
[回到顶部](http://www.cnblogs.com/fish-li/archive/2011/12/27/2304063.html#_labelTop)
# Cache的定义
在介绍Cache的用法前，我们先来看一下Cache的定义：（说明：我忽略了一些意义不大的成员）![](http://images.cnblogs.com/cnblogs_com/fish-li/297352/r_expand.png)
ASP.NET为了方便我们访问Cache，在HttpRuntime类中加了一个静态属性Cache，这样，我们就可以在任意地方使用Cache的功能。而且，ASP.NET还给它增加了二个“快捷方式”：Page.Cache, HttpContext.Cache，我们通过这二个对象也可以访问到HttpRuntime.Cache，注意：这三者是在访问同一个对象。Page.Cache访问了HttpContext.Cache，而HttpContext.Cache又直接访问HttpRuntime.Cache
[回到顶部](http://www.cnblogs.com/fish-li/archive/2011/12/27/2304063.html#_labelTop)
# Cache常见用法
通常，我们使用Cache时，一般只有二个操作：读，写。
要从Cache中获取一个缓存项，我们可以调用Cache.Get(key)方法，要将一个对象放入缓存，我们可以调用Add, Insert方法。然而，Add, Insert方法都有许多参数，有时我们或许只是想简单地放入缓存，一切接受默认值，那么还可以调用它的默认索引器，我们来看一下这个索引器是如何工作的：
public object this[string key]
{
    get
    {
        return this.Get(key);
    }
    set
    {
        this.Insert(key, value);
    }
}
可以看到：读缓存，其实是在调用Get方法，而写缓存则是在调用Insert方法的最简单的那个重载版本。
注意了：Add方法也可以将一个对象放入缓存，这个方法有7个参数，而Insert也有一个签名类似的重载版本，它们有着类似的功能：**将指定项添加到 System.Web.Caching.Cache 对象，该对象具有依赖项、过期和优先级策略以及一个委托（可用于在从 Cache 移除插入项时通知应用程序）。**然而，它们有一点小的区别：**当要加入的缓存项已经在Cache中存在时，Insert将会覆盖原有的缓存项目，而Add则不会修改原有缓存项。**
也就是说：如果您希望某个缓存项目一旦放入缓存后，就不要再被修改，那么调用Add确实可以防止后来的修改操作。而调用Insert方法，则永远会覆盖已存在项（哪怕以前是调用Add加入的）。
从另一个角度看，Add的效果更像是 static readonly 的行为，而Insert的效果则像 static 的行为。
注意：我只是说【像】，事实上它们比一般的static成员有着更灵活的用法。
由于缓存项可以让我们随时访问，看起来确实有点static成员的味道，但它们有着更高级的特性，比如：缓存过期（绝对过期，滑动过期），缓存依赖（依赖文件，依赖其它缓存项），移除优先级，缓存移除前后的通知等等。后面我将会分别介绍这四大类特性。
[回到顶部](http://www.cnblogs.com/fish-li/archive/2011/12/27/2304063.html#_labelTop)
# Cache类的特点
Cache类有一个很难得的优点，用MSDN上的说话就是：
此类型是线程安全的。
为什么这是个难得的优点呢？因为在.net中，绝大多数类在实现时，都只是保证静态类型的方法是线程安全，而不考虑实例方法是线程安全。这也算是一条基本的.NET设计规范原则。
对于那些类型，MSDN通常会用这样的话来描述：
此类型的公共静态（在 Visual Basic 中为 Shared）成员是线程安全的。但不能保证任何实例成员是线程安全的。 
所以，这就意味着我们可以在任何地方读写Cache都不用担心Cache的数据在多线程环境下的数据同步问题。多线程编程中，最复杂的问题就是数据的同步问题，而Cache已经为我们解决了这些问题。
不过我要提醒您：ASP.NET本身就是一个多线程的编程模型，所有的请求是由线程池的线程来处理的。通常，我们在多线程环境中为了解决数据同步问题，一般是采用锁来保证数据同步，自然地，ASP.NET也不例外，它为了解决数据的同步问题，内部也是采用了锁。
说到这里，或许有些人会想：既然只一个Cache的静态实例，那么这种锁会不会影响并发？
答案是肯定的，有锁肯定会在一定程度上影响并发，这是没有办法的事情。
然而，ASP.NET在实现Cache时，会根据CPU的个数创建多个缓存容器，尽量可能地减小冲突，以下就是Cache创建的核心过程：![](http://images.cnblogs.com/cnblogs_com/fish-li/297352/r_expand.png)
说明：CacheInternal是个内部用的包装类，Cache的许多操作都要由它来完成。
在上面的代码中，numSingleCaches的计算过程很重要，如果上面代码不容易理解，那么请看我下面的示例代码：![](http://images.cnblogs.com/cnblogs_com/fish-li/297352/r_expand.png)
程序将会输出：
1,2,4,4,8,8,8,8,16,16,16,16,16,16,16,16,32,32,32,32
CacheMultiple的构造函数如下：![](http://images.cnblogs.com/cnblogs_com/fish-li/297352/r_expand.png)
现在您应该明白了吧：CacheSingle其实是ASP.NET内部使用的缓存容器，多个CPU时，它会创建多个缓存容器。
在写入时，它是如何定位这些容器的呢？请继续看代码：![](http://images.cnblogs.com/cnblogs_com/fish-li/297352/r_expand.png)
说明：参数中的hashCode是直接调用我们传的key.GetHashCode() ，GetHashCode是由Object类定义的。
所以，从这个角度看，虽然ASP.NET的Cache只有一个HttpRuntime.Cache静态成员，但它的内部却可能会包含多个缓存容器，这种设计可以在一定程度上减少并发的影响。
不管如何设计，在多线程环境下，共用一个容器，冲突是免不了的。如果您只是希望简单的缓存一些数据，不需要Cache的许多高级特性，那么，可以考虑不用Cache 。比如：可以创建一个Dictionary或者Hashtable的静态实例，它也可以完成一些基本的缓存工作，不过，我要提醒您：您要自己处理多线程访问数据时的数据同步问题。
顺便说一句：Hashtable.Synchronized(new Hashtable())也是一个线程安全的集合，如果想简单点，可以考虑它。
接下来，我们来看一下Cache的高级特性，这些都是Dictionary或者Hashtable不能完成的。
[回到顶部](http://www.cnblogs.com/fish-li/archive/2011/12/27/2304063.html#_labelTop)
# 缓存项的过期时间
ASP.NET支持二种缓存项的过期策略：绝对过期和滑动过期。
1. 绝对过期，这个容易理解：就是在缓存放入Cache时，指定一个具体的时间。当时间到达指定的时间的时，缓存项自动从Cache中移除。
2. 滑动过期：某些缓存项，我们可能只希望在有用户在访问时，就尽量保留在缓存中，只有当一段时间内用户不再访问该缓存项时，才移除它，这样可以优化内存的使用，因为这种策略可以保证缓存的内容都是【很热门】的。操作系统的内存以及磁盘的缓存不都是这样设计的吗？而这一非常有用的特性，Cache也为我们准备好了，只要在将缓存项放入缓存时，指定一个滑动过期时间就可以实现了。
以上二个选项分别对应Add, Insert方法中的DateTime absoluteExpiration, TimeSpan slidingExpiration这二个参数。
注意：这二个参数都是成对使用的，但不能同时指定它们为一个【有效】值，最多只能一个参数值有效。当不使用另一个参数项时，请用Cache类定义二个static readonly字段赋值。
这二个参数比较简单，我就不多说了，只说一句：如果都使用Noxxxxx这二个选项，那么缓存项就一直保存在缓存中。（或许也会被移除）
[回到顶部](http://www.cnblogs.com/fish-li/archive/2011/12/27/2304063.html#_labelTop)
# 缓存项的依赖关系 - 依赖其它缓存项
ASP.NET Cache有个很强大的功能，那就是缓存依赖。一个缓存项可以依赖于另一个缓存项。以下示例代码创建了二个缓存项，且它们间有依赖关系。首先请看页面代码：![](http://images.cnblogs.com/cnblogs_com/fish-li/297352/r_expand.png)
页面后台代码：![](http://images.cnblogs.com/cnblogs_com/fish-li/297352/r_expand.png)
当运行这个示例页面时，运行结果如下图所示，点击按钮【设置Key1的值】时，将会出现缓存项的内容（左图）。点击按钮【设置Key2的值】时，此时将获取不到缓存项的内容（右图）。
![](http://pic002.cnblogs.com/images/2011/281816/2011122722172062.gif)
根据结果并分析代码，我们可以看出，在创建Key1的缓存项时，我们使用了这种缓存依赖关系：
CacheDependency dep = new CacheDependency(null, new string[] { "key2" });
所以，当我们更新Key2的缓存项时，Key1的缓存就失效了（不存在）。
不要小看了这个示例。的确，仅看这几行示例代码，或许它们实在是没有什么意义。那么，我就举个实际的使用场景来说明它的使用价值。
![](http://pic002.cnblogs.com/images/2011/281816/2011123011080183.gif)
上面这幅图是我写的一个小工具。在示意图中，左下角是一个缓存表CacheTable，它由一个叫Table1BLL的类来维护。CacheTable的数据来源于Table1，由Table1.aspx页面显示出来。同时，ReportA, ReportB的数据也主要来源于Table1，由于Table1的访问几乎绝大多数都是读多写少，所以，我将Table1的数据缓存起来了。而且，ReportA, ReportB这二个报表采用GDI直接画出（由报表模块生成，可认是Table1BLL的上层类），鉴于这二个报表的浏览次数较多且数据源是读多写少，因此，这二个报表的输出结果，我也将它们缓存起来。
在这个场景中，我们可以想像一下：如果希望在Table1的数据发生修改后，如何让二个报表的缓存结果失效？
让Table1BLL去通知那二个报表模块，还是Table1BLL去直接删除二个报表的缓存？
其实，不管是选择前者还是后者，当以后还需要在Table1的CacheTable上做其它的缓存实现时（可能是其它的新报表），那么，势必都要修改Table1BLL，那绝对是个失败的设计。这也算是模块间耦合的所带来的恶果。
幸好，ASP.NET Cache支持一种叫做缓存依赖的特性，我们只需要让Table1BLL公开它缓存CacheTable的KEY就可以了（假设KEY为 CacheTableKey），然后，其它的缓存结果如果要基于CacheTable，设置一下对【**CacheTableKey**】的依赖就可以实现这样的效果：**当CacheTable更新后，被依赖的缓存结果将会自动清除。**这样就彻底地解决了模块间的缓存数据依赖问题。
[回到顶部](http://www.cnblogs.com/fish-li/archive/2011/12/27/2304063.html#_labelTop)
# 缓存项的依赖关系 - 文件依赖
在上篇博客[【在.net中读写config文件的各种方法】](http://www.cnblogs.com/fish-li/archive/2011/12/18/2292037.html)的结尾，我给大家留了一个问题：
**我希望在用户修改了配置文件后，程序能立刻以最新的参数运行，而且不用重启网站。**
今天我就来回答这个问题，并给出所需的全部实现代码。
首先，我要说明一点：上次博客的问题，虽然解决方案与Cache的文件依赖有关，但还需与缓存的移除通知配合使用才能完美的解决问题。为了便于内容的安排，我先使用Cache的文件依赖来简单的实现一个粗糙的版本，在本文的后续部分再来完善这个实现。
先来看个粗糙的版本。假如我的网站中有这样一个配置参数类型：![](http://images.cnblogs.com/cnblogs_com/fish-li/297352/r_expand.png)
我可以将它配置在这样一个XML文件中：
<?xml version="1.0" encoding="utf-8"?>
<RunOptions xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
            xmlns:xsd="http://www.w3.org/2001/XMLSchema">
  <WebSiteUrl>http://www.cnblogs.com/fish-li</WebSiteUrl>
  <UserName>fish li</UserName>
</RunOptions>
再来一个用于显示运行参数的页面：![](http://images.cnblogs.com/cnblogs_com/fish-li/297352/r_expand.png)
下面的代码就可以实现：**在XML修改后，浏览页面就能立即看到最新的参数值**：![](http://images.cnblogs.com/cnblogs_com/fish-li/297352/r_expand.png)
注意：这里仍然是在使用CacheDependency，只是我们现在是给它的构造函数的第一个参数传递要依赖的文件名。
在即将结束对缓存的依赖介绍之前，还要补充二点：
1. CacheDependency还支持【嵌套】，即：CacheDependency的构造函数中支持传入其它的CacheDependency实例，这样可以构成一种非常复杂的树状依赖关系。
2. 缓存依赖的对象还可以是SQL SERVER，具体可参考SqlCacheDependency
[回到顶部](http://www.cnblogs.com/fish-li/archive/2011/12/27/2304063.html#_labelTop)
# 缓存项的移除优先级
缓存的做法有很多种，一个静态变量也可以称为是一个缓存。一个静态的集合就是一个缓存的容器了。我想很多人都用Dictionary，List，或者Hashtable做过缓存容器，我们可以使用它们来保存各种数据，改善程序的性能。一般情况下，如果我们直接使用这类集合去缓存各类数据，那么，那些数据所占用的内存将不会被回收，哪怕它们的使用机会并不是很多。当缓存数据越来越多时，它们所消耗的内存自然也会越来越多。那么，能不能在内存不充足时，释放掉一些访问不频繁的缓存项呢？
这个问题也确实是个较现实的问题。虽然，使用缓存会使用程序运行更快，但是，我们数据会无限大，不可能统统缓存起来，毕竟，内存空间是有限的。因此，我们可以使用前面所说的**基于一段时间内不再访问**就删除的策略来解决这个问题。然而，在我们编码时，根本不知道我们的程序会运行在什么配置标准的计算机上，因此，根本不可能会对内存的大小作出任何假设，此时，我们可能会希望当缓存占用过多的内存时，且当内存不够时，能自动移除一些不太重要的缓存项，这或许也比较有意义。
对于这个需求，在.net framework提供了二种解决办法，一种是使用WeakReference类，另一种是使用Cache 。不过，既然我们是在使用ASP.NET，选择Cache当然会更方便。在Cache的Add, Insert方法的某些重载版本中，可以指定缓存项的保存优先级策略，由参数CacheItemPriority priority来传入。其中，CacheItemPriority是一个枚举类型，它包含了如下枚举值：![](http://images.cnblogs.com/cnblogs_com/fish-li/297352/r_expand.png)
说明：当我们调用Cache的Add, Insert方法时，如果不指定CacheItemPriority选项，最终使用Normal所代表的优先级。如果我们希望将某个可能不太重要的数据放入缓存时，可以指定优先级为Low或者BelowNormal。如果想让缓存项在内存不足时，也不会被移除（除非到期或者依赖项有改变），可使用NotRemovable。
显然，我们可以使用这个特性来控制缓存对内存压力的影响。其它的缓存方案，如static Collection + WeakReference也较难实现这样灵活的控制。
[回到顶部](http://www.cnblogs.com/fish-li/archive/2011/12/27/2304063.html#_labelTop)
# 缓存项的移除通知
ASP.NET Cache与一些static变量所实现的缓存效果并不相同，它的缓存项是可以根据一些特定的条件失效的，那些失效的缓存将会从内存中移除。虽然，某些移除条件并不是由我们的代码直接解发的，但ASP.NET还是提供一种方法让我们可以在缓存项在移除时，能通知我们的代码。
注意哦：ASP.NET Cache支持移除【前】通知 和 移除【后】通知二种通知方式。
我们可以在调用Add, Insert方法时，通过参数onRemoveCallback传递一个CacheItemRemovedCallback类型的委托，以便在移除指定的缓存项时，能够通知我们。这个委托的定义如下：![](http://images.cnblogs.com/cnblogs_com/fish-li/297352/r_expand.png)
委托的各个参数的含义以及移除原因，在注释中都有明确的解释，我也不再重复了。
我想：有很多人知道Cache的Add, Insert方法有这个参数，也知道有这个委托，但是，它们有什么用呢？ 在后面的二个小节中，我将提供二个示例来演示这一强大的功能。
通常，我们会以下面这种方式从Cache中获取结果：
RunOptions options = HttpRuntime.Cache[RunOptionsCacheKey] as RunOptions;
if( options == null ) {
    // 缓存中没有，则从文件中加载
    // ..................................
    HttpRuntime.Cache.Insert(RunOptionsCacheKey, options, dep);
}
return options;
这其实也是一个惯用法了：先尝试从缓存中获取，如果没有，则从数据源中加载，并再次放入缓存。
为什么会在访问Cache时返回null呢？答案无非就是二种原因：1. 根本没有放入Cache，2. 缓存项失效被移除了。
这种写法本身是没有问题，可是，如果从数据源中加载数据的时间较长，情况会怎样呢？
显然，会影响后面第一次的访问请求。您有没有想过，如果缓存项能一直放在Cache中，那不就可以了嘛。是的，通常来说，只要您在将一个对象放入Cache时，不指定过期时间，不指定缓存依赖，且设置为永不移除，那么对象确实会一直在Cache中，可是，过期时间和缓存依赖也很有用哦。如何能二者兼得呢？
为了解决这个问题，微软在.net framework的3.5 SP1、3.0 SP1、2.0 SP1版本中，加入了【移除前通知】功能，不过，这个方法仅受Insert支持，随之而来的还有一个委托和一个移除原因的枚举定义：![](http://images.cnblogs.com/cnblogs_com/fish-li/297352/r_expand.png)
注意：CacheItemUpdateReason这个枚举只有二项。原因请看MSDN的解释：
与 CacheItemRemovedReason 枚举不同，此枚举不包含 Removed 或 Underused 值。可更新的缓存项是不可移除的，因而绝不会被 ASP.NET 自动移除，即使需要释放内存也是如此。
**再一次提醒：**有时我们确实需要缓存失效这个特性，但是，缓存失效后会被移除。虽然我们可以让后续的请求在获取不到缓存数据时，从数据源中加载，也可以在CacheItemRemovedCallback回调委托中，重新加载缓存数据到Cache中，**但是在数据的加载过程中，Cache并不包含我们所期望的缓存数据，如果加载时间越长，这种【空缺】效果也会越明显。**这样会影响（后续的）其它请求的访问。为了保证让我们所期望的缓存数据能够一直存在于Cahce中，且仍有失效机制，我们可以使用【移除前通知】功能。
[回到顶部](http://www.cnblogs.com/fish-li/archive/2011/12/27/2304063.html#_labelTop)
# 巧用缓存项的移除通知 实现【延迟操作】
我看过一些ASP.NET的书，也看过一些人写的关于Cache方面的文章，基本上，要么是一带而过，要么只是举个毫无实际意义的示例。可惜啊，这么强大的特性，我很少见到有人把它用起来。
今天，我就举个有实际意义的示例，再现Cache的强大功能！
我有这样一个页面，可以让用户调整（上下移动）某个项目分支记录的上线顺序：
![](http://pic002.cnblogs.com/images/2011/281816/2011123011083424.gif)
当用户需要调整某条记录的位置时，页面会弹出一个对话框，要求输入一个调整原因，并会发邮件通知所有相关人员。
![](http://pic002.cnblogs.com/images/2011/281816/2011123011085942.gif)
由于界面的限制，一次操作（点击上下键头）只是将一条记录移动一个位置，当要对某条记录执行跨越多行移动时，必须进行多次移动。考虑到操作的方便性以及不受重复邮件的影响，程序需要实现这样一个需求：页面只要求输入一次原因便可以对一条记录执行多次移动操作，并且不要多次发重复邮件，而且要求将最后的移动结果在邮件中发出来。
这个需求很合理，毕竟谁都希望操作简单。
那么如何实现这个需求呢？这里要从二个方面来实现，首先，在页面上我们应该要完成这个功能，对一条记录只弹一次对话框。由于页面与服务端的交互全部采用Ajax方式进行（不刷新），状态可以采用JS变量来维持，所以这个功能在页面中是很容易实现。再来看一下服务端，由于服务端并没有任何状态，当然也可以由页面把它的状态传给服务端，但是，哪次操作是最后一次呢？显然，这是无法知道的，最后只能修改需求，如果用户在2分钟之内不再操作某条记录时，便将最近一次操作视为最后一次操作。
基于新的需求，程序必须记录用户的最近一次操作，以便在2分钟不操作后，发出一次邮件，但要包含第一次输入的原因，还应包含最后的修改结果哦。
该怎么实现这个需求呢？ 我立即就想到了ASP.NET Cache，因为我了解它，知道它能帮我完成这个功能。下面我来说说在服务端是如何实现的。
**整个实现的思路是：**
1. 客户端页面还是每次将记录的RowGuid, 调整方向，调整原因，这三个参数发到服务端。
2. 服务端在处理完顺序调整操作后，将要发送的邮件信息Insert到Cache中，同时提供slidingExpiration和onRemoveCallback参数。
3. 在CacheItemRemovedCallback回调委托中，忽略CacheItemRemovedReason.Removed的通知，如果是其它的通知，则发邮件。
为了便于理解，我特意为大家准备了一个示例。整个示例由三部分组成：一个页面，一个JS文件，服务端代码。先来看页面代码：![](http://images.cnblogs.com/cnblogs_com/fish-li/297352/r_expand.png)
页面的显示效果如下：
![](http://pic002.cnblogs.com/images/2011/281816/2011122722184225.gif)
处理页面中二个按钮的JS代码如下：![](http://images.cnblogs.com/cnblogs_com/fish-li/297352/r_expand.png)
说明：在服务端，我使用了我在[【用Asp.net写自己的服务框架】](http://www.cnblogs.com/fish-li/archive/2011/09/05/2168073.html)那篇博客中提供的服务框架，服务端的全部代码是这个样子的：**（注意代码中的注释）**![](http://images.cnblogs.com/cnblogs_com/fish-li/297352/r_expand.png)
为了能让JavaScript能直接调用C#中的方法，还需要在web.config中加入如下配置：
<httpHandlers>
    <add path="*.fish" verb="*" validate="false" type="MySimpleServiceFramework.AjaxServiceHandler"/>
</httpHandlers>
好了，示例代码就是这些。如果您有兴趣，可以在本文的结尾处下载这些示例代码，自己亲自感受一下利用Cache实现的【延迟处理】的功能。
其实这种【延迟处理】的功能是很有用的，比如还有一种适用场景：有些数据记录可能需要频繁更新，如果每次更新都去写数据库，肯定会对数据库造成一定的压力，**但由于这些数据也不是特别重要**，因此，我们可以利用这种【延迟处理】来将写数据库的时机进行**合并处理**，最终我们可以实现：将多次的写入变成一次或者少量的写入操作，我称这样效果为：**延迟合并写入**
这里我就对数据库的**延迟合并写入**提供一个思路：将需要写入的数据记录放入Cache，调用Insert方法并提供slidingExpiration和onRemoveCallback参数，然后在CacheItemRemovedCallback回调委托中，模仿我前面的示例代码，将多次变成一次。不过，这样可能会有一个问题：如果数据是一直在修改，那么就一直不会写入数据库。最后如果网站重启了，数据可能会丢失。如果担心这个问题，那么，可以在回调委托中，遇到CacheItemRemovedReason.Removed时，使用计数累加的方式，当到达一定数量后，再写入数据库。比如：遇到10次CacheItemRemovedReason.Removed我就写一次数据库，这样就会将原来需要写10次的数据库操作变成一次了。当然了，如果是其它移除原因，写数据库总是必要的。注意：**对于金额这类敏感的数据，绝对不要使用这种方法。**
再补充二点：
1. 当CacheItemRemovedCallback回调委托被调用时，缓存项已经不在Cache中了。
2. 在CacheItemRemovedCallback回调委托中，我们还可以将缓存项重新放入缓存。
有没有想过：这种设计可以构成一个循环？如果再结合参数slidingExpiration便可实现一个定时器的效果。
关于缓存的失效时间，我要再提醒一点：通过absoluteExpiration, slidingExpiration参数所传入的时间，当缓存时间生效时，缓存对象并不会立即移除，**ASP.NET Cache大约以20秒的频率去检查这些已过时的缓存项。**
[回到顶部](http://www.cnblogs.com/fish-li/archive/2011/12/27/2304063.html#_labelTop)
# 巧用缓存项的移除通知 实现【自动加载配置文件】
在本文的前部分的【文件依赖】小节中，有一个示例演示了：当配置文件更新后，页面可以显示最新的修改结果。在那个示例中，为了简单，我直接将配置参数放在Cache中，每次使用时再从Cache中获取。如果配置参数较多，这种做法或许也会影响性能，毕竟配置参数并不会经常修改，如果能直接访问一个静态变量就能获取到，应该会更快。通常，我们可能会这样做：![](http://images.cnblogs.com/cnblogs_com/fish-li/297352/r_expand.png)
但是，这种做法有一缺点就是：不能在配置文件更新后，自动加载最新的配置结果。
为了解决这个问题，我们可以使用Cache提供的文件依赖以及移除通知功能。前面的示例演示了移除后通知功能，这里我再演示一下移除前通知功能。
**说明：事实上，完成这个功能，可以仍然使用移除后通知，只是移除前通知我还没有演示，然而，这里使用移除前通知并没有显示它的独有的功能。**
下面的代码演示了在配置文件修改后，自动更新运行参数的实现方式：**（注意代码中的注释）**![](http://images.cnblogs.com/cnblogs_com/fish-li/297352/r_expand.png)
改动很小，只是LoadRunOptions方法做了修改了而已，但是效果却很酷。
还记得我在上篇博客[【在.net中读写config文件的各种方法】](http://www.cnblogs.com/fish-li/archive/2011/12/18/2292037.html)的结尾处留下来的问题吗？这个示例就是我的解决方案。
[回到顶部](http://www.cnblogs.com/fish-li/archive/2011/12/27/2304063.html#_labelTop)
# 文件监视技术的选择
对于文件监视，我想有人或许会想到FileSystemWatcher。正好我就来说说关于【文件监视技术】的选择问题。
说明，本文所有结论均为我个人的观点，仅供参考。
这个组件，早在做WinForm开发时就用过了，对它也是印象比较深的。
它有一个包装不好的地方是：**事件会重复发出。**比如：一次文件的保存操作，它却引发了二次事件。
什么，你不信？ 正好，我还准备了一个示例程序。
![](http://pic002.cnblogs.com/images/2011/281816/2011122722185820.gif)
说明：图片中显示了发生过二次事件，但我只是在修改了文件后，做了一次保存操作而已。本文的结尾处有我的示例程序，您可以自己去试一下。这里为了方便，还是贴出相关代码：![](http://images.cnblogs.com/cnblogs_com/fish-li/297352/r_expand.png)
对于这个类的使用，只想说一点：会引发的事件很多，因此一定要注意过滤。以下引用MSDN的一段说明：
Windows 操作系统在 FileSystemWatcher 创建的缓冲区中通知组件文件发生更改。如果短时间内有很多更改，则缓冲区可能会溢出。这将导致组件失去对目录更改的跟踪，并且它将只提供一般性通知。使用 InternalBufferSize 属性来增加缓冲区大小的开销较大，因为它来自无法换出到磁盘的非页面内存，所以应确保缓冲区大小适中（尽量小，但也要有足够大小以便不会丢失任何文件更改事件）。若要避免缓冲区溢出，请使用 NotifyFilter 和 IncludeSubdirectories
 属性，以便可以筛选掉不想要的更改通知。
幸运的是，ASP.NET Cache并没有使用这个组件，我们不用担心文件依赖而引发的重复操作问题。它直接依赖于webengine.dll所提供的API，因此，建议在ASP.NET应用程序中，优先使用Cache所提供的文件依赖功能。
[回到顶部](http://www.cnblogs.com/fish-li/archive/2011/12/27/2304063.html#_labelTop)
# 各种缓存方案的共存
ASP.NET Cache是一种缓存技术，然而，我们在ASP.NET程序中还可以使用其它的缓存技术，这些不同的缓存也各有各自的长处。由于ASP.NET Cache不能提供对外访问能力，因此，它不可能取代以memcached为代表的分布式缓存技术，但它由于是不需要跨进程访问，效率也比分布式缓存的速度更快。如果将ASP.NET Cache设计成【一级缓存】，分布式缓存设计成【二级缓存】，就像CPU的缓存那样，那么将能同时利用二者的所有的优点，实现更完美的功能以及速度。
其实缓存是没有一个明确定义的技术，一个static变量也是一个缓存，一个static集合就是一个缓存容器了。这种缓存与ASP.NET Cache相比起来，显然static变量的访问速度会更快，如果static集合不是设计得很差的话，并发的冲突也可能会比ASP.NET Cache小，也正是因为这一点，static集合也有着广泛的使用。然而，ASP.NET Cache的一些高级功能，如：过期时间，缓存依赖（包含文件依赖），移除通知，也是static集合不具备的。因此，合理地同时使用它们，会让程序有着最好的性能，也同时拥有更强大的功能。
[点击此处下载示例代码](http://files.cnblogs.com/fish-li/AspnetCache.cab.7z)
