
# unity3d动态加载dll的API以及限制 - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[unity3d动态加载dll的API以及限制](https://www.cnblogs.com/eniac1946/p/7244400.html)
|Posted on|2017-07-27 13:37|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7244400)|[收藏](#)

|现在参与的项目是做MMO手游，目标平台是Android和iOS，iOS平台不能动态加载dll（什么原因找乔布斯去），可以直接忽略，而在Android平台是可以动态加载dll的，有了这个就可以实现代码更新，不过实际上，在unity里要用上动态加载dll是有很多限制的（不了解的话就是坑）。
|限制1：在|Android|手机里动态加载dll不能使用Assembly.LoadFile(string path)，|只能使用Assembly.Load|(byte[] rawAssembly)这个接口，所以要自己想办法先读出来。
|限制2：动态加载的脚本不能在编辑器里挂在prefab上。
|限制3：如果脚本在动态dll里，调用AddComponent()挂此脚本上prefab上时不能使用AddComponent(“SomeScript”)的方式调用，要用AddComponent(Type.GetType(“SomeScript”))。
|限制4：在动态dll里使用[RequireComponent(typeof(SomeScript))]无效，所以不能使用。
|我目前主要就是遇到这些坑，以后有什么新发现再进行补充。|WINDOWS下LoadFile()测试可行。
|修正&补充：在iOS下不是都不能动态加载dll，越狱的手机可以实现，具体可参考文章：http://blog.csdn.net/lucky_06/article/details/9093879，亲测没问题。
|再次修正：iOS实际上还是不能实现真正的代码热更新，使用上述方法只能调用外部dll的接口，但执行逻辑还是在native code里面。简单的说，之前我测试的时候是项目里面有a.dll的代码，然后再Assembly.Load a.dll进来，调用a.dll里面的方法是可以的，但实际是调用了项目里的代码，如果项目里没有a.dll的代码，调用会报错，以上。
|二、C\#反射知识及API
|一些关于C\#反射的知识，估计也就最多达到使用API的程度，至于要深入了解，以现在的水平估计很难做到，所以下面此篇文章，以作为一个阶段的总结。
|对于反射的总结，我想从以下几个方面展开，首先是反射程序集，模块，类的成员以及成员的一些信息；接下来就是动态调用类的成员方法；第三个方面就动态产生程序集，模块和类以及类的成员。好了，现在就让我们从反射各种信息开始吧
|在C\#中，我们要使用反射，首先要搞清楚以下命名空间中几个类的关系:
|System.Reflection命名空间
|(1)   AppDomain:应用程序域，可以将其理解为一组程序集的逻辑容器
|(2)   Assembly:程序集类
|(3)   Module:模块类
|(4)   Type:使用反射得到类型信息的最核心的类
|他们之间是一种从属关系，也就是说，一个AppDomain可以包含N个Assembly,一个Assembly可以包含N个Module,而一个Module可以包含N个Type.
|AppDomain这个类我们等下再来讲解。我们先关注Assembly个类
|在程序中，如果我们要动态加载一个程序集怎么办呢？有几种方式可以使用，分别是Load、LoadFrom和LoadWithPartialName三个Assembly的静态方法.
|先来讲解Assembly.Load方法，该方法会有多个重载版本，其中一个就是提供程序集的详细信息，即程序集的标识，包括程序集的名称，版本，区域信息，公有密钥标记,全部都是以一个字符串的形式提供，例如:"MyAssembly,Version=1.0.0.0,culture=zh-CN,PublicKeyToken=47887f89771bc57f”.
|那么,使用Assembly.Load加载程序集的顺序是怎样的呢？首先它会去全局程序集缓存查找，然后到应用程序的根目录查找，最后会到应用程序的私有路径查找。
|当然，如果你使用的是弱命名程序集，也即只给出程序集的名称，那么这个时候，CLR将不会在程序集上应用任何安全或者部署策略，而且Load也不会到全局缓存程序集中查找程序集。
|[测试](http://lib.csdn.net/base/softwaretest)|加载弱命名程序集的例子如下:
|(1)   新建一个控制台应用程序的工程，同时勾选创建解决方案
|(2)   在解决方案中新建一个类库的项目，随便写一个类和一个方法
|(3)   在控制台项目中，首先不添加引用，直接在Main方法中添加如下代码:
|Assembly assembly =|Assembly.Load|("MyAssembly");
|if (assembly != null)
|{ Console.WriteLine("加载成功"); }
|执行程序，会抛出异常，说找不到该程序集。什么原因呢？因为我们使用的是弱命名程序集，Load方法不会去全局程序集缓存中查找，而该应用程序目录下又没有该程序集，所以程序找不到。这个时候，我们把程序稍微改一下，不用添加代码，只需添加对MyAssembly的引用，重新运行程序，加载成功了。
|接下来，我们就要看看Load怎么加载强命名程序集了，这个步骤稍微有些复杂。还是刚才的项目，找到MyAssembly.dll程序集所在的目录，一般在bin"Debug目录下
|(1)生成密钥对文件   sn –k MyAssemblyKey.keys
|你也可以自己随便起一个密钥对文件名
|(2)生成公钥文件
|sn –p MyAssemblyKey.keys MyAssemblyPublicKey.PublicKey
|注：查看公钥命令:sn –tp MyAssemblyPublicKey.PublicKey
|(3)创建强命名程序集。
|很简单，只需要在声明命名空间的那句代码上加上如下特性:
|[assembly:AssemblyKeyFileAttribute(@”D:"Test"MyAssemblyKey.keys”)]
|(4)   编译项目
|(5)   将程序集添加到程序集全局缓存
|gacutil –i MyAssembly.dll
|这个时候，转到加载程序集的项目中，将Load方法中的参数改为”程序集名,Version=版本,culture=区域信息,PublicKeyToken=公钥“，然后再去掉对程序集的引用，我们会发现，程序运行成功。表明Load到全局缓存区查找到了该程序集。
|使用Load方法加载程序集，特别是强命名程序集，能在程序集上应用安全和部署策略，推荐使用该方法动态加载程序集，至于LoadFrom和LoadWithPartialName。
|首先我们还是来看看LoadFrom方法，这个方法的原理是这样的：我们如果要使用它来动态加载程序集，必须告诉它程序集的路径，也即在哪个目录下面，CLR会去加载与你指定的路径完全匹配的程序集。记住，当我们指定程序集路径时，不能包括任何关于程序集强命名的信息，所以，CLR不会在我们指定的程序集文件上应用任何策略，而且也不会去任何其他的地方搜索程序集，简言之，它就是指哪打哪，呵呵。
|例如:你有个程序集在D:/Test/MyAssembly.dll,你要用Assembly.LoadFrom加载该程序集，代码就如下:
|Assembly assembly = Assembly.LoadFrom(@”D:/Test/MyAssembly.dll”);
|对于,LoadWithParitalName方法，推荐大家最好不要使用它，因为程序无法确定最终要去加载哪个程序集的版本，所以我们这里只是简单的介绍一下它的工作原理：你可以传递一个程序集标识给它，包括程序集名称，至于其他信息是可选的（区域信息，公有密钥等），该方法执行时，会首先检查应用程序中配置文件的qualifyAssembly节点，如果存在，则把该部分名称的程序集替换成完全的程序集标识，如果不存在，则使用程序集名称先到应用程序根目录下查找，然后是私有目录，没有找到的话，就到程序集全局缓存中查找。简单过程如下:
|应用程序根目录 -> 应用程序私有目录 -> 程序集全局缓存.
|Assembly.Load()方法，Assembly.LoadFrom()方法，Assembly.LoadFile()方法的区别！
|1，Assembly.Load()
|这个方法通过程序集的长名称（包括程序集名，版本信息，语言文化，公钥标记）来加载程序集的，会加载此程序集引用的其他程序集，一般情况下都应该优先使用 这个方法，他的执行效率比LoadFrom要高很多，而且不会造成重复加载的问题（原因在第2点上说明）
|使用这个方法的时候， CLR会应用一定的策略来查找程序集，实际上CLR按如下的顺序来定位程序集：
|⑴如果程序集有强名称，在首先在全局程序集缓(GAC)中查找程序集。
|⑵如果程序集的强名称没有正确指定或GAC中找不到，那么通过配置文件中的<codebase>元素指定的URL来查找
|⑶如果没有指定强名称或是在GAC中找不到，CLR会探测特定的文件夹：
|假设你的应用程序目录是C:/AppDir,<probing>元素中的privatePath指定了一个路径Path1,你要定位的程序集是AssemblyName.dll则CLR将按照如下顺序定位程序集
|C:/AppDir/AssemblyName.dll
|C:/AppDir/AssemblyName/AssemblyName.dll
|C:/AppDir/Path1/AssemblyName.dll
|C:/AppDir/Path1/AssemblyName/AssemblyName.dll
|如果以上方法不能找到程序集，会发生编译错误，如果是动态加载程序集，会在运行时抛出异常！
|2，Assembly.LoadFrom()
|这个方法从指定的路径来加载程序集，实际上这个方法被调用的时候，CLR会打开这个文件，获取其中的程序集版本，语言文化，公钥标记等信息，把他们传递给 Load方法，接着，Load方法采用上面的策略来查找程序集。如果找到了程序集，会和LoadFrom方法中指定的路径做比较，如果路径相同，该程序集 会被认为是应用程序的一部分，如果路径不同或Load方法没有找到程序集，那该程序集只是被作为一个“数据文件”来加载，不会被认为是应用程序的一部分。 这就是在第1点中提到的Load方法比LoadFrom方法的执行效率高的原因。另外，由于可能把程序集作为“数据文件”来加载，所以使用 LoadFrom从不同路径加载相同程序集的时候会导致重复加载。当然这个方法会加载此程序集引用的其他程序集。
|3，Assembly.LoadFile()
|这个方法是从指定的文件来加载程序集，和上面方法的不同之处是这个方法不会加载此程序集引用的其他程序集！
|结论：一般大家应该优先选择Load方法来加载程序集，如果遇到需要使用LoadFrom方法的时候，最好改变设计而用Load方法来代替！
|另：Assembly.LoadFile 与 Assembly.LoadFrom的区别
|1、Assembly.LoadFile只载入相应的dll文件，比如Assembly.LoadFile("abc.dll")，则载入abc.dll，假如abc.dll中引用了def.dll的话，def.dll并不会被载入。
|Assembly.LoadFrom则不一样，它会载入dll文件及其引用的其他dll，比如上面的例子，def.dll也会被载入。
|2、用Assembly.LoadFrom载入一个Assembly时，会先检查前面是否已经载入过相同名字的Assembly，比如abc.dll有两个版本(版本1在目录1下，版本2放在目录2下)，程序一开始时载入了版本1，当使用Assembly.LoadFrom("2//abc.dll")载入版本2时，不能载入，而是返回版本1。Assembly.LoadFile的话则不会做这样的检查，比如上面的例子换成Assembly.LoadFile的话，则能正确载入版本2。
|LoadFile:加载指定路径上的程序集文件的内容。LoadFrom: 根据程序集的文件名加载程序集文件的内容。
|区别：
|LoadFile 方法用来来加载和检查具有相同标识但位于不同路径中的程序集.但不会加载程序的依赖项。
|LoadFrom 不能用于加载标识相同但路径不同的程序集。







|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
