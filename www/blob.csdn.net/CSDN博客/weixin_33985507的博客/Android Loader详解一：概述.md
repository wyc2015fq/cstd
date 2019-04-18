# Android Loader详解一：概述 - weixin_33985507的博客 - CSDN博客
2012年05月05日 21:47:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
From：[http://blog.csdn.net/nkmnkm/article/details/7244117](http://blog.csdn.net/nkmnkm/article/details/7244117)
装载器从android3.0开始引进。它使得在activity或fragment中异步加载数据变得简单。装载器具有如下特性：
- 
它们对每个Activity和Fragment都有效。
- 
他们提供了异步加载数据的能力。
- 
它们监视数据源的一将一动并在内容改变时传送新的结果。
- 
当由于配置改变而被重新创建后，它们自动重连到上一个加载器的游标，所以不必重新查询数据。
## 装载器API概述
在使用装载器时，会涉及很多类和接口们，我们在下表中对它们总结一下：
|Class/Interface|说明|
|----|----|
|LoaderManager|一个抽像类，关联到一个Activity或Fragment，管理一个或多个装载器的实例。这帮助一个应用管理那些与Activity或Fragment的生命周期相关的长时间运行的的操作。最常见的方式是与一个CursorLoader一起使用，然而应用是可以随便写它们自己的装载器以加载其它类型的数据。每个activity或fragment只有一个LoaderManager。但是一个LoaderManager可以拥有多个装载器。|
|LoaderManager.LoaderCallbacks|一个用于客户端与LoaderManager交互的回调接口。例如，你使用回调方法onCreateLoader()来创建一个新的装载器。|
|Loader（装载器）|一个执行异步数据加载的抽象类。它是加载器的基类。你可以使用典型的CursorLoader，但是你也可以实现你自己的子类。一旦装载器被激活，它们将监视它们的数据源并且在数据改变时发送新的结果。|
|AsyncTaskLoader|提供一个AsyncTask来执行异步加载工作的抽象类。|
|CursorLoader|AsyncTaskLoader的子类，它查询ContentResolver然后返回一个Cursor。这个类为查询cursor以标准的方式实现了装载器的协议，它的游标查询是通过AsyncTaskLoader在后台线程中执行，从而不会阻塞界面。使用这个装载器是从一个ContentProvider异步加载数据的最好方式。相比之下，通过fragment或activity的API来执行一个被管理的查询就不行了。|
上面所列的类和接口们是你在你的应用中要实现装载器时的核心组件。你的每个装载器并不一定需要所有的组件，但是你总是需要引用LoaderManager来初始化一个装载器。后面的章节将向你展示如何使用这些类和接口们。
