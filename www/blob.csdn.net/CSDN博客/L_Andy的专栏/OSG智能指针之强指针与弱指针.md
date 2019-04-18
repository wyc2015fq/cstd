# OSG智能指针之强指针与弱指针 - L_Andy的专栏 - CSDN博客

2015年08月04日 15:10:57[卡哥](https://me.csdn.net/L_Andy)阅读数：991


使用OSG的人都知道OSG的内存管理方式采用了智能指针，通过智能指针的方式让OSG自己处理对象的销毁工作。在OSG中有两个智能指针类型，一个就是我们再熟悉不过的ref_ptr，另外一个可能很多人不一定经常用，但确实非常好的observer_ptr。这两种类型都是作为智能指针而出现的，那他们到底有什么区别？为什么会出现两个类似的东西？下面我们慢慢揭开它们的面纱。
    其实它们是两种智能指针类型，一种是我们熟悉的强指针类型(strong pointer)，一种是弱指针类型(weak pointer)，ref_ptr就是强指针类型，observer_ptr是弱指针类型，强指针类型ref_ptr和一般意义的智能指针类型相同，通过引用计数来记录有多少对象在使用该对象，当没有任何对象使用该对象时该对象自动销毁，而observer_ptr也指向一个对象，但它没有引用计数功能，它仅仅记录该对象的地址，当该对象在其它地方被销毁后，通过observer_ptr获得的对象地址将为空。为什么称之为弱指针，本人不知道这个概念的提出者是如何想的，个人感觉可能就是因为它仅仅是对对象地址的一个记录，不参与对象的生存期管理，所以称之为弱。不知道您是否这样认为，反正我这样认为了，哈哈。
    说到这里我们应该会有疑问，为什么还要多个弱智能指针呢？这不成了多个弱智吗？一个ref_ptr不就够了吗？我说说我的看法，不知道是不是准确，仅仅代表个人的观点，如果我们只用ref_ptr，那就意味着在任何情况下只要我们需要引用某个对象，就要增加它的引用计数，不用了就减少引用计数，我们要明确的第一个问题就是，如果对象数量特别大，频繁的增加引用计数和减少引用计数本身会降低一定的新能，这一点应该勿容置疑，另外，在很多情况下我们只是需要对对象进行引用以便在对象生存期内方便访问，而如果使用ref_ptr增加它的引用计数后也就改变了该对象的生存期，对象的生存期需要我们自己来通过额外的手段进行控制，这样必然增加系统的复杂性，比如我们在系统中创建了若干飞机，同时系统中有一个对所有飞机进行统一管理的飞机管理器，正常的逻辑是当飞机爆炸后我们要销毁该飞机对象，如果我们在飞机管理器中使用ref_ptr，飞机在别的地方全部解除引用后飞机对象也不会自动销毁，因为飞机管理器中还存在对其的引用，必须借助一定判断手段人为将该飞机对象从飞机管理器解除引用才将飞机对象引用计数降为0进而销毁该对象。这样以来我们是不是做了一件出力不讨好的事？如果用observer_ptr，我们将不受这种困扰，因为它不干涉对象的生存期，而且不会因为对引用计数的操作降低效率。这就是个人理解的为什么会出现弱智能指针的原因，可能不准确或不够全面，欢迎高人修正补充，这里权当抛砖引玉。
    接下来我们就看看observer_ptr如何实现的对对象的引用，如何做到当该对象被销毁后observer_ptr会知道该情况。其实很简单，通过名字我们就能猜出来这里肯定使用了观察者模式，当对象销毁后自动通知该观察者。下面我们通过代码来看看是不是这样。
    首先我们要看的一定是该智能指针的构造函数，它有若干构造函数，我们就用最常用也最典型的来看，其它的大同小异而已。
    observer_ptr(T* rp)
    {
        _reference = rp ? rp->getOrCreateObserverSet() : 0;
        _ptr = (_reference.valid() && _reference->getObserverdObject()!=0) ? rp : 0;
    }
    这和ref_ptr一样通过一个原始对象进行构造，在构造函数里做了一件事，就是对成员变量的初始化，成员变量一共两个，一个就是称之为观察者集合的对象，另外一个和ref_ptr一样就是该智能指针引用的原始对象的指针。这里的重点是那个观察者集合的成员，它其实就是对对象的一个观察者，当对象在别的地方被销毁后，该观察者会被通知到。
其实上面的rp、_ptr和_reference->getObserverdObject()是一个东西，我们想想，如果不一样是不是就完蛋了？因为我们要引用和观察的必须是一个东西！任何一个从Referenced类继承下来的子类都可以拥有一个观察者集合，一个观察者集合观察一个对象，一个集合可以拥有若干观察者，这里不涉及到观察者集合中的观察者，只是涉及到被观察的对象，所以我们为了防止干扰就不说它了，有兴趣的可以看代码。rp->getOrCreateObserverSet()就是给该智能指针引用的对象创建一个观察者集合，在创建观察者集合时，该对象就自然告诉该观察者集合观察自己了，不信？代码为证：

ObserverSet* Referenced::getOrCreateObserverSet() const

{

    ......

    ObserverSet* newObserverSet = new ObserverSet(this);

    ......

}

到这你应该明白刚才说的rp、_ptr和_reference->getObserverdObject()怎么是一个东西了吧。

好了，我们已经明白observer_ptr是如何观察其引用的对象了，那对象销毁的时候它是怎么知道的呢？什么？你不信它能知道？不管你信不信，我反正信了。看代码：

void Referenced::signalObserversAndDelete(bool signalDelete, bool doDelete) const

{

    ......

    if (observerSet && signalDelete)

    {

        observerSet->signalObjectDeleted(const_cast<Referenced*>(this));

    }

    ......

}

void ObserverSet::signalObjectDeleted(void* ptr)

{

    ......

    _observedObject = 0;

}

看到了吧，当它引用的对象销毁时会告诉这个观察者集合：你观察的对象没有啦:_observedObject = 0;
到这里，一去一返就接上了。快完了，别着急，我们再看一行代码：

inline T* get() const { return (_reference.valid() && _reference->getObserverdObject()!=0) ? _ptr : 0; }

看到什么了？如果对象销毁了它返回的是什么？空！你答对了，别忘了它不增加引用对象的引用计数，不修改引用的对象的生存期，这就意味着你引用的对象在你使用时可能已经在别的地方销毁了，所以用的时候一定别忘了判断一下，要不很容易“嘣！”的一声。

    完了吧？哦，不行，还有一点，如果我使用的时候，在使用过程中，对象在别的线程被销毁了怎么办？哎呀，确实是啊，这该怎么办？岂不是还得用ref_ptr，没错，这时候就要用ref_ptr了，否则你正在房顶上揭瓦呢，下面那王八蛋把梯子给你搬走了你摔死了。用ref_ptr！但不是让你舍弃observer_ptr不用只用ref_ptr哦，看代码我告诉你用什么：

    bool lock(ref_ptr<T>& rptr) const

    {

        if (!_reference)

        {

            rptr = 0;

            return false;

        }

        Referenced* obj = _reference->addRefLock();

        if (!obj)

        {

            rptr = 0;

            return false;

        }

        rptr = _ptr;

        obj->unref_nodelete();

        return rptr.valid();

    }

        {

            rptr = 0;

            return false;

        }

        Referenced* obj = _reference->addRefLock();

        if (!obj)

        {

            rptr = 0;

            return false;

        }

        rptr = _ptr;

        obj->unref_nodelete();

        return rptr.valid();

       }

这次你明白如果你意识到会有这种隐患的时候该怎么做了？我就不翻译了，看两个官方注释：

该函数的注释是：

 /**

     * Assign the observer_ptr to a ref_ptr. The ref_ptr will be valid if the

     * referenced object hasn't been deleted and has a ref count > 0.

     */

还有对observer_ptr的注释：

* If you are in any doubt about whether it is safe to access the object safe then use the

  * ref_ptr<> observer_ptr<>.lock() combination. */

全部完毕!

