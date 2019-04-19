# Introduction to Model View Presenter on Andriod - 定义域 - CSDN博客
置顶2016年02月24日 15:10:44[定义域](https://me.csdn.net/u010278882)阅读数：525标签：[mvp																[android](https://so.csdn.net/so/search/s.do?q=android&t=blog)](https://so.csdn.net/so/search/s.do?q=mvp&t=blog)
个人分类：[架构](https://blog.csdn.net/u010278882/article/category/5804917)
# Android中的MVP简介
> 
FROM: [https://github.com/konmik/konmik.github.io/wiki/Introduction-to-Model-View-Presenter-on-Android](https://github.com/konmik/konmik.github.io/wiki/Introduction-to-Model-View-Presenter-on-Android)
  AUTHOR: Konstantin Mikheev
这篇文章将一步一步的介绍Android中的MVP模式，从最简单的示例到最佳时间。本文还引入了一个新的库，通过使用该库让 Android 中应用MVP模式更加简单。
## MVP 模式简单吗？运用MVP模式我能获得哪些利益？
### 什么是 MVP 模式？
- 视图是一个显示数据并对用户动作进行响应的层。Android中，视图可能是Activity、Fragment、View 或 Dialog。
- 模型是一个类似数据库API或者远程服务器API的数据访问层。
- 表示器是一个为视图提供来自模型的数据的层。表示器也处理后台任务。
在 Android 中 MVP 是一种将后台任务从 Activity/Fragment/View 中分离出来，让其不受大多数的生命周期事件的影响。通过使用该模式，整个应用的理解和设计变得更加简单，应用的可靠性也由很大提升，编写的代码更少，代码的可维护性更好，最终开发者的生活也变得更加开心。
### 为什么要在 Android 上使用 MVP 模式
#### 原因1：让应用变得简单和容易理解
如果你不知道KISS原则，可以[了解一下](https://people.apache.org/~fhanik/kiss.html)。
- 大多数现在Android应用都只使用视图-模型架构。
- Android 开发者更倾向于对视图复杂性的辩论而不是商业逻辑。
如果应用使用视图-模型架构的话，通常最终的结果就是“所有事物都是互相关联的”。
![这里写图片描述](https://img-blog.csdn.net/20160224151112444)
如果上面的图表还不够复杂，那么你可以考虑一下视图随机的显示和消息的情况吧。别忘了，还有视图状态的保存和取回呢。为临时使用的视图添加一系列后台任务，然后“你的蛋糕就好了”。
“所有事物都是互相关联的”的另一种说法是“上帝对象”。
![这里写图片描述](https://img-blog.csdn.net/20160224151123367)
“上帝对象”是超级复杂的；不能对其中的一部分进行重用、测试、调试和重构。
使用 MVP
- 复杂任务被划分为简单任务，而简单的任务更容易被解决
- 对象更小（代码更少），更少的bug，更易于调试
- 更易于测试
![这里写图片描述](https://img-blog.csdn.net/20160224151138538)
因为应用了MVP模式的视图层更加简单，所以在请求数据的时候甚至不需要回调。视图逻辑会变得非常线性化。
#### 原因2：后台任务
无论何时编写Activity、Fragment或者自定义视图时，你可以将所有与后台任务相关的方法放到另一个外部类或静态类中。拿出来后，后台任务不再与 Activity 关联，也就不会有内存泄露的问题，也不会受到Activity重新创建的影响。我们将这种对象叫做”表示器”。
虽然存在多种不同的方法来处理后台任务，但是他们中没有一个有 MVP 可靠。
##### Why this works
下图展示了当设备配置发生改变或发生内存溢出事件时，不同的应用组件会发生什么(行为)。每个 Android 开发者都知道下面的这份数据，但是很好有开发者对其整理。
![这里写图片描述](https://img-blog.csdn.net/20160224151151616)
- 情况1：当用户旋转屏幕、改变语言设置、附加外部显示器时，通常会发生配置改变。配置发生改变的详细情况[介绍](http://developer.android.com/reference/android/R.attr.html#configChanges)。
- 情况2：当用户在设置的开发者设置中设置了”不要保持Activity“时或者另一个Activty抵达顶端时，Activity都会重启。
- 情况3：如果没有足够的内存并且应用运行在后台时，进程重启就会发生。
**结论**
现在可以看出，设置了setRetainInstance(true)的Fragment起不到任何作用 - 在上面的情况中我们还是需要自行保存 Fragment 的状态。所以设置了setRetainInstance的Fragment只能应对少数几个问题。下面给出简化的法则：
![这里写图片描述](https://img-blog.csdn.net/20160224151204491)
现在看起来好多了。现在我们仅需要编写两类代码就能完整的处理任何原因导致的应用程序的状态恢复：
- 保存/恢复 Activity、View、Fragment、DialogFragment
- restart background requests in case of a process restart
第一部分可通过 Android API 实现。第二部分可通过”表示器“实现。表示器会记住执行了哪个请求，如果在执行期间发生了进程重启，”表示器”会再次执行该请求。
### 简单例子
示例会从远程服务加载数据并将其显示出来。如果发生错误将弹出toast。
我推荐使用RxJava来创建”表示器”，因为使用它来控制数据流会非常简单。
我还要感谢一些创建了我的示例使用的这个API的伙计：[The Internet Chuck Norris Database](http://www.icndb.com/)。
#### 不使用 MVP 的情况: [example 00](https://github.com/konmik/MVPExamples/tree/master/example00)
有经验的开发者可能会注意到这个简单例子中有一些严重缺陷：
- 每当用户旋转屏幕时就会发起一个数据请求 - 应用的请求次数要比他需要的多，并且在每次旋转屏幕的过程中，用户会看到屏幕空空的
- 如果用户频繁旋转屏幕，会引发内存泄露 - 因为每个回调都持有一个对 MainActivity 的引用，这样，当请求正在被执行的过程中，回调会驻留在内存中。实际上，非常有可能导致应用的崩溃，由于内存溢出错误或者严重的应用关闭。
#### 使用 MVP 的情况：[example 01](https://github.com/konmik/MVPExamples/tree/master/example01)
请别在主页尝试! :) 例子仅用于演示目的。实际中，并不需要静态变量来引用”表示器”。
理论上来说，MainPresenter has three threads of events：onNext、onError、onTakeView。onTaskView提供 MainActivity 实例，onNext 和 onError 向实例发布数据。
MainActivity 创建了 MainPresenter，并将其放在了 onCreate/onDestroy 的外部。MainActivity 使用静态变量引用 MainPresenter，这样每次由于内存溢出事件的发生导致的进程重启时，MainActivity 就需要检查”表示器”是否存在，并在需要时创建。
是的，这样处理（对”表示器“的存在性进行检查并使用静态变量）起来看起来确实有些代码臃肿，但是稍后会演示如何简化。
总之，核心思想是：
- 示例应用没有在每次用户旋转屏幕时发起一个数据请求
- 如果进程重启则数据会被重新加载
- 如果 MainActivity 被销毁，那么MainPresenter 并没有保留 MainActivity 实例的引用，所以在屏幕旋转过程中不会发生内存泄露，也不需要取消注册的请求。
### [Nucleus](https://github.com/konmik/nucleus)
“原子核”是一个库，该库是受到 [Mortar](https://github.com/square/mortar) 库和 [KISS](https://people.apache.org/~fhanik/kiss.html) 文章的影响而创建的。
特性列表如下：
- 可以保存/还原 ”表示器“ 状态到 View/Fragment/Activity 的状态 Bundle 中。”表示器”能够保存请求参数到bundle中以便之后重新发起请求。
- 提供工具方法来检测请求结果和错误，让视图显示正确的信息。仅需一行代码，这样就不用写那么多 !=null 检查啦
- 可以让一个”表示器”被多个视图实例引用。如果使用传统方式（使用Dagger）来实例化”表示器“是不能做到这一点的。
- 提供了将”表示器”绑定到”视图”的快捷方式，仅需一行代码
- 提供了基础的视图类：`NucleusView`、`NucleusFragment`、`NucleusSupportFragment`、`NucleusActivity`。当然你也可以粘贴复制其中的代码来创建自己的”表示器”工具类。
- 进程重启后能自动重发请求，在`onDestory`期间自动取消RxJava的订阅
- 最后一点，因为简单所以开发者都能理解。只需180行代码就能使用”表示器“，只需230行代码就能获取RxJava支持。
#### 使用 Nucleus:[example 02](https://github.com/konmik/MVPExamples/tree/master/example02)
如你所见，这里的代码更加简短，也更加清晰了。”原子核”自动的创建/销毁/保存”表示器”，附加和移除视图到表示器，发送正确的请求到附加的视图中。
`MainPresenter`的代码之所以简短是因为其使用了`deliverLateestCache()`操作，该操作将延迟所有数据源提交的数据和错误直到视图可用。此外，还将数据缓存在内存中，这样可以在配置改变时重用数据。
`MainActivity`的代码之所以简短是因为表示器的创建是由`NucleusActivity`管理的。绑定表示器所需的全部操作就是加上`@RequiresPresenter(MainPresenter.class)`注解。
注意，在Android中，如果使用注解，最好检查一下其是否会降低性能。我在 Galaxy S 上的进行的基准测试，注解处理大概需要 0.3 毫秒。注解的处理仅会在视图实例化的过程中进行，所以可以考虑使用注解。
#### 更多示例
[Nucleus Example](https://github.com/konmik/nucleus/tree/master/nucleus-example)
[Nucleus Example With Tests](https://github.com/konmik/nucleus/tree/master/nucleus-example-with-tests)
##### deliverLatestCache() 方法
RxPresenter 工具方法有三个变体：
- deliver()
- deliverLatest()
- deliverLatestCache()
待续…
