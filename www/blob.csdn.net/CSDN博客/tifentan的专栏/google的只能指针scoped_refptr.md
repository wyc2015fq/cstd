# google的只能指针scoped_refptr - tifentan的专栏 - CSDN博客

2017年01月06日 19:04:37[露蛇](https://me.csdn.net/tifentan)阅读数：1570



转自： [http://www.cnblogs.com/marco/archive/2010/09/09/1820724.html](http://www.cnblogs.com/marco/archive/2010/09/09/1820724.html)

　　很多人喜欢Chrome，喜欢它的简洁，喜欢它的“快”。 简洁大家一目了然，无需多言，这里重点要说的是它的“快”。什么是“快”呢？大概不少人第一反应就是cnBeta上的日经[JavaScript](http://lib.csdn.net/base/javascript)跑分贴，Chrome那叫一个快啊。（其实每次点开这类文章时，我都是想去同情一下我们可怜的IE同学的，不过最近IE9着实又让人充满了期待。）但Javascript跑分真的这么重要吗，我们真的能体会到很大的差距吗？反正我是感觉不太出来。那凭啥还说Chrome快呢？Chrome快就快在UI
 Responsive（UI响应性）上。说白了就是你双击一下桌面图标，它能很快启动起来（Firefox用户应该深有感触吧）；点击某个超链接，它能瞬间给你弹出个新页面来（用IE的朋友也没少遇过假死的现象吧）。因此，Chrome并不只是个跑分的工具。那Chrome良好的UI Responsive是如何做到的呢？这就要得益于它的线程模型了。

　　为了避免重复制造轮子，这里我并不打算阐述Chrome线程模型的总体框架，而是将更多的精力放在具体实现上。对Chrome线程模型框架不熟悉的朋友可以阅读一下duguguiyu的[这篇文章](http://www.cnblogs.com/duguguiyu/archive/2008/10/02/1303095.html)。有条件的朋友可以到[这里](http://chromium-browser-source.commondatastorage.googleapis.com/chromium_tarball.html)去下载一份Chromium的源码，或者到[这里](http://src.chromium.org/svn/trunk/src/)在线查看。闲话少说,
 Let's [Go](http://lib.csdn.net/base/go)!

NO.1 Smart Pointers

整个Chromium源码应该有十几万个文件吧，面对如此浩瀚的code海，我们该从哪下手呢？古话说，工欲善其事，必先利其器。我们就从Chromium中用得最多的几个智能指针着手吧。

先看scoped_ptr(可参见base/scoped_ptr.h)。这是一个在析构时将自动删除所持有对象的智能指针。其行为和std::tr1::scoped_ptr基本一致，不予多说。

Chromium中一个比较有意思的常用指针是scoped_refptr(可参见base/ref_counted.h)。和scoped_ptr稍微有点区别，scoped_refptr只能引用显式实现了引用记数接口的类型：

```
![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)代码

template <class T>
class scoped_refptr {

 public:

  scoped_refptr(T* p) : ptr_(p) {

    if (ptr_)

      ptr_->AddRef();

  }

  ~scoped_refptr() {

    if (ptr_)

      ptr_->Release();

  }

 protected:

  T* ptr_;

};
```

如上所示，class T必须显式实现AddRef和Release接口，而scoped_refptr在构造和析构的时候将调用它们来增加/减少引用记数。当一个对象引用记数为0时，class T的Release方法将负责将该对象自动删除。 很多人禁不住要问了，那要写一个class T岂不是很麻烦。别担心，Google已经为我们提供了两个默认的实现：RefCounted和RefCountedThreadSafe（均在base/ref_counted.h中），我们可以通过继承它们来让我们的类实现引用记数功能。

RefCounted比较简单，但值得注意的一点，它并不保证线程安全性，因此只能应用于单线程持有的对象。如果需要在多个线程中都可以引用的对象时，该怎么办呢？这时RefCountedThreadSafe就派上用场了（个人并不喜欢这个名字，或许叫ThreadSafeRefCounted会更合适）。

```
![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)代码

template <class T, typename Traits = DefaultRefCountedThreadSafeTraits<T> >
class RefCountedThreadSafe : public subtle::RefCountedThreadSafeBase {

 public:

  void AddRef() {

    subtle::RefCountedThreadSafeBase::AddRef();

  }

  void Release() {

    if (subtle::RefCountedThreadSafeBase::Release()) {

      Traits::Destruct(static_cast<T*>(this));

    }

  }

};
void RefCountedThreadSafeBase::AddRef() {

  AtomicRefCountInc(&ref_count_);

}
```

我们可以看到，RefCountedThreadSafe在增加和删除引用技术时，都采用了原子操作，保证了线程安全性。

当然，如果你认为RefCountedThreadSafe就这么简单的话，你就低估它了。仔细看，在类型实例化的时候，除了传入了一个class T，还有个Traits。那这个Traits是派什么用场的呢？注意Release方法，当引用记数为0时，正是使用了Traits来负责删除该对象。这就给了我们一个自定义删除对象的机会，这在很多情况下是非常有用的。比如说，一般来说，很多UI对象是不能在主线程之外析构的。一旦我们把它交给了scoped_refptr来管理，我们就无法保证它会在哪个线程中引用记数被置0，而这时需要做的只是使用自定义的Traits来给主线程发一个消息，让主线程负责去删除该对象，这就避免了很多问题的产生。

写到这里，不禁想稍微比较一下scoped_refptr和std::tr1::shared_ptr。同样使用引用记数来实现对象的自动删除，scoped_refptr却要求所引用的对象都显式实现AddRef和Release接口，还需要该对象的析构函数为私有，并把scoped_refptr设为友元；而shared_ptr只要求该对象提供一个public的析构函数就可以了。一眼看上去，似乎shared_ptr优雅很多。但正是这种表面上的优雅，让很多人在有意和无意间很容易忽略该对象可能会在多线程的环境中被使用，从而给我们带来了无数的烦恼。相比较这种优雅，我个人还是比较欣赏scoped_refptr这种有着明确语义，而且不容易引起误用的设计。另外还有一点就是上文所说的Traits，这是个非常实用的功能。可惜的是，如果我们把对象交付给了shared_ptr，就没办法再去控制它的析构了。如果不是看到这段代码，或许我根本就不会想到，原来shared_ptr还有这么多不尽如人意的地方，所以也强烈推荐大家多去读读这样高质量的代码。

另外还有一个指针是WeakPtr（base/weak_ptr.h），这里就不多做描述了，需要了解的朋友可以自行阅读它的代码。 

No.2 Tasks 

有了智能指针的知识做基础，我们接下来就来看看Chrome中Task的实现了。

Task类（base/task.h）的封装很简单，如下：

```
class Task : public tracked_objects::Tracked {

 public:

  virtual ~Task();

  // Tasks are automatically deleted after Run is called.
  virtual void Run() = 0;

};
```

稍微可以说下的是它所继承的Tracked类，这是用来做调试和性能跟踪用的。它会记录Task生成和销毁的时间，以及在何处被Post到线程的消息循环中去的（通过__LINE__, __FILE__等编译器定义的宏）。更多细节可以参阅base/tracked.h及相关文件。另外一些诸如CancelableTask、DeleteTask等子接口和子类也都是小角色，没什么可多说的。

看了上面的实现，很多人可能会问：如果每次我想往其他线程分发一个任务，都要写一个类来实现Task接口，岂不是很烦？如果你也有一样的忧虑，那么恭喜你，你和Chromium的设计者想到一块去了。task.h中还提供了很多NewRunnableMethod方法来让我们可以轻松的把用一个对象和它的方法来构造Task。

```
![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)代码

template <class T, class Method, class A>

inline CancelableTask* NewRunnableMethod(T* object, Method method, const A& a) {

  return new RunnableMethod<T, Method, Tuple1<A> >(object, method, MakeTuple(a));

}

template <class T, class Method, class Params>
class RunnableMethod : public CancelableTask {

 public:

  RunnableMethod(T* obj, Method meth, const Params& params)

      : obj_(obj), meth_(meth), params_(params) {

    traits_.RetainCallee(obj_);

    COMPILE_ASSERT((MethodUsesScopedRefptrCorrectly<Method, Params>::value),

                   badrunnablemethodparams);

  }

 private:

  T* obj_;

  Method meth_;

  Params params_;

  RunnableMethodTraits<T> traits_;

};
```

注意这句：COMPILE_ASSERT((MethodUsesScopedRefptrCorrectly<Method, Params>::value), badrunnablemethodparams)。这句的意思是保证所有传入Method的指针类型的参数都是scoped_refptr，否则编译器将报错。（关于如何实现编译期Assert，可以自行查阅源码或参考《Modern C++
 Design》。）细心的朋友可能还会发现：这里怎么也有个Traits？正像它名字描述的那样，所有Traits其实都只是一些小把戏。RunnableMethodTraits无非是在涉及多线程时，对Class T是否实现了RefCountedThreadSafe接口等做一些检查（只在Debug版本中检查）。另外，传入Method的方法参数将被拷贝（指针拷贝的是值，而不是指向的对象），如果你有一些自定义类型要使用特殊的拷贝语义，可以自己去对TrupleTraits做template specialization。

有了这些，我们的Task实现是不是就完备了呢？答案显然是否定的。我们很可能会用NewRunnableMethod(new T, &T::SomeMethod)来构建了一个Task并把它分发到指定线程。但问题是，如果在该Task执行之前，new T所创建出来的对象已经被销毁了呢？那Task所持有的不就成了一个Pending Pointer，这不就会引发Access Violation这样严重的问题了？所以Chromium给我们提供了另一套解决方案：Scoped
 factories。本着不重复制造轮子的原则，愿意做详细了解的朋友请阅读Optman的[这篇文章](http://optman.javaeye.com/blog/545036)。

No.3 Cross-Thread Request

Chrome中所有线程都有着明确的分工，比如UI线程专门负责响应用户请求，File线程专门负责访问文件系统。

一种很自然的情况是，用户可能会想查看某个文件的内容。在Chrome中，这项任务的完成需要UI线程先往File线程的消息队列中添加一个读取文件的Task，然后该Task才会在File线程中执行（如果File线程中还有其他Task，可能还需要等待）。等这个Task执行完之后，再往UI线程的消息队列里添加另一个Task用于显示文件的具体内容。等到显示文件内容的Task执行完后，该文件才被显示出来。看起来很复杂吧？而且在这些过程当中，也没有办法保证所有线程都没有被析构。当然，设计者是不可能把这么复杂的功能直接暴露给使用者的。CancelableRequest类就是用来封装这种复杂性的。再次感谢Optman，他的[这篇文章](http://optman.javaeye.com/blog/548352)为我们详细阐述了CancelableRequest类的实现。

但是，如果我们只是知道如何去实现，而不去关心为什么要这样做的话，也就成了圣人所说的学而不思者，后果必然就是“罔”了。不觉得相对于在UI线程中直接去读取文件，Chrome折腾这么一大堆东西之后，反而要多出很多线程调度和Task排队等待的时间来吗？那它是不是在瞎折腾呢？细细品味，这完全是有道理的。正是因为这个设计，才使得Chrome拥有了优秀的UI
 Responsive。我们知道，UI无响应的罪魁祸首就是在于在UI线程中执行耗时的操作。在UI线程中读取文件，带来的最直接后果就是用户容易觉得卡。也许你会质疑，如果我读的只是一个很小的文件呢，这样的话用户应该完全感觉不到啊？但是，如果这个时候你的卡巴斯基在“喀喀喀”地扫描你的硬盘呢，你的程序在这种极端的硬盘IO下面还能保持良好的响应性吗？再看Chrome，无论你硬盘怎么“喀”，UI线程只要在往File线程添加一个Task后就返回去响应用户操作了，而File线程读好之后，也能很快地进行更新。或许从用户操作到真正显示出文件内容来的时间上，Chrome还是要长些，但在这段时间中，用户还是能够流畅地切换标签页、打开新页面等。但如果使用UI线程直接访问文件，这就不可能做到了。

No.4 Sample

虽然还有MessageLoop、Thread等类没说，但已经不妨碍我们来看懂实际的例子了。可能很多人还是有疑问，在Chrome的线程模型中，到底是怎样规避使用锁的？这就要得益于它在对象设计时地规定对象只能生存在一个线程中。可能这么说还是不好理解。有句老话说，依葫芦画瓢。现在我们就把一个葫芦拿出来瞅瞅到底长啥样。
 接下来将以Chrome的历史记录管理做为例子来具体描述。（具体的代码请参见chrome/browser/history目录下的history.h和history.cc。）

history.h中有一个重要的类HistoryService，它是运行在UI线程中并给UI提供服务的。它有两个重要的成员thread_和history_backend_。thread_是用来初始化一个后台访问历史记录的线程（History线程），而history_backend_则在这个线程上勤勤恳恳地工作，帮HistoryService完成真正的数据访问。这就是所谓的对象只能生存在一个线程中：HistoryService生存在UI线程中，而history_backend_则生存在History线程中。请注意history_backend_是如何初始化和删除的。

```
![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)代码

class HistoryService : public CancelableRequestProvider,

                       public NotificationObserver,

                       public base::RefCountedThreadSafe<HistoryService> {

  .........

 private:

  scoped_refptr<history::HistoryBackend> history_backend_;

  base::Thread* thread_;

};

HistoryService::HistoryService()

    : thread_(new base::Thread(kHistoryThreadName)) ... {

  ......

}

bool HistoryService::Init(...) {

  if (!thread_->Start()) {

    Cleanup();

    return false;

  }

  ......

  // Create the history backend.
  LoadBackendIfNecessary();

  return true;

}

void HistoryService::LoadBackendIfNecessary() {

  if (!thread_ || history_backend_)

    return;  // Failed to init, or already started loading.

  scoped_refptr<HistoryBackend> backend(new HistoryBackend(...));

  history_backend_.swap(backend);

  ScheduleAndForget(PRIORITY_UI, &HistoryBackend::Init, no_db_);

}

HistoryService::~HistoryService() {

  Cleanup();

}

void HistoryService::Cleanup() {

  if (!thread_) {

    // We've already cleaned up.
    return;

  }

  // Unload the backend.
  UnloadBackend();

  base::Thread* thread = thread_;

  thread_ = NULL;

  delete thread;

}

void HistoryService::UnloadBackend() {

  if (!history_backend_)

    return;  // Already unloaded.
  Task* closing_task = NewRunnableMethod(history_backend_.get(), &HistoryBackend::Closing);

  history_backend_ = NULL;

  ScheduleTask(PRIORITY_NORMAL, closing_task);

}
```

 　　需要说明的是，history_backend_构造时只是简单地把所有指针成员置NULL而没有具体的初始化动作（HistoryBackend::Init方法），这既保证了UI线程在构造HistoryService时也可以快速完成而不需要等待，同时也是因为它需要在thread_所代表的线程中来完成初始化。当HistoryService需要调用HistoryBackend的方法时，也是通过往thread_的消息循环上分发Task来异步完成的。关于HistoryService是如何往thread_上分发任务和发起Cross-Thread
 Request的请自行参阅代码，基本都是使用了上面提到的方法，比较好懂。

      扯了这么多，该小结一下了。好像也没什么特别的感慨要发，只是觉得这些代码确实质量很高而且很精妙，稍加整理也可以用于我们自己的工程中。当然，请记得一定要遵循开源的游戏规则。想进一步了解的朋友可以自己去研读代码。本人水平有限，上面描述中有不妥当的地方，也请不吝指出。

