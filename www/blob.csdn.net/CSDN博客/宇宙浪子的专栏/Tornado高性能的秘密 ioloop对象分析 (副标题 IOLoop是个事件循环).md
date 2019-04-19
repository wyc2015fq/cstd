# Tornado高性能的秘密:ioloop对象分析 (副标题:IOLoop是个事件循环) - 宇宙浪子的专栏 - CSDN博客
2017年08月15日 18:00:43[_宇宙浪子_](https://me.csdn.net/bluehawksky)阅读数：401
转自：[http://blog.csdn.net/u014252563/article/details/51532284](http://blog.csdn.net/u014252563/article/details/51532284)
网上都说nginx和lighthttpd是高性能web服务器，而tornado也是著名的高抗负载应用，它们间有什么相似处呢？上节提到的ioloop对象是如何循环的呢？往下看。
首先关于TCP服务器的开发上节已经提过，很明显那个三段式的示例是个效率很低的（因为只有一个连接被端开新连接才能被接受）。要想开发高性能的服务器，就得在这accept上下功夫。
首先，新连接的到来一般是经典的三次握手，只有当服务器收到一个SYN时才说明有一个新连接（还没建立），这时监听fd是可读的可以调用accept，此前服务器可以干点别的，这就是SELECT/POLL/EPOLL的思路。而只有三次握手成功后，accept才会返回，此时监听fd是读完成状态，似乎服务器在此之前可以转身去干别的，等到读完成再调用accept就不会有延迟了，这就是AIO的思路，不过在*nix平台上好像支持不是很广。。。再有，accept得到的新fd，不一定是可读的（客户端请求还没到达），所以可以等新fd可读时在read()（可能会有一点延迟），也可以用AIO等读完后再read就不会延迟了。同样类似，对于write，close也有类似的事件。
总的思路就是，在我们关心的fd上注册关心的多个事件，事件发生了就启动回调，没发生就看点别的。这是单线程的，多线程的复杂一点，但差不多。nginx和lightttpd以及tornado都是类似的方式，只不过是多进程和多线程或单线程的区别而已。为简便，我们只分析tornado单线程的情况。
关于ioloop.py的代码，主要有两个要点。一个是configurable机制，一个就是epoll循环。先看epoll循环吧。IOLoop 类的start是循环所在，但它必须被子类覆盖实现，因此它的start在PollIOLoop里。略过循环外部的多线程上下文环境的保存与恢复，单看循环：
|`01`|`while``True``:`|
|`02`|`   ``poll_timeout ``=``3600.0`|
|`03`||
|`04`|`   ````# Prevent IO event starvation by delaying new callbacks```|
|`05`|`   ````# to the next iteration of the event loop.```|
|`06`|`   ``with ``self``._callback_lock:`|
|`07`|`       ``callbacks ``=``self``._callbacks`|
|`08`|`       ``self``._callbacks ``=``[]`|
|`09`|`   ``for``callback ``in``callbacks:`|
|`10`|`       ``self``._run_callback(callback)`|
|`11`||
|`12`|`   ``if``self``._timeouts:`|
|`13`|`       ``now ``=``self``.time()`|
|`14`|`       ``while``self``._timeouts:`|
|`15`|`           ``if``self``._timeouts[``0``].callback ``is``None``:`|
|`16`|`               ````# the timeout was cancelled```|
|`17`|`               ``heapq.heappop(``self``._timeouts)`|
|`18`|`           ``elif``self``._timeouts[``0````].deadline <````=``now:`|
|`19`|`               ``timeout ``=``heapq.heappop(``self``._timeouts)`|
|`20`|`               ``self``._run_callback(timeout.callback)`|
|`21`|`           ``else``:`|
|`22`|`               ``seconds ``=``self``._timeouts[``0``].deadline ``-``now`|
|`23`|`               ``poll_timeout ``=``min````(seconds, poll_timeout)```|
|`24`|`               ``break`|
|`25`||
|`26`|`   ``if``self``._callbacks:`|
|`27`|`       ````# If any callbacks or timeouts called add_callback,```|
|`28`|`       ````# we don't want to wait in poll() before we run them.```|
|`29`|`       ``poll_timeout ``=``0.0`|
|`30`||
|`31`|`   ``if``not``self``._running:`|
|`32`|`       ``break`|
|`33`||
|`34`|`   ``if``self``._blocking_signal_threshold ``is``not``None``:`|
|`35`|`       ````# clear alarm so it doesn't fire while poll is waiting for```|
|`36`|`       ````# events.```|
|`37`|`       ``signal.setitimer(signal.ITIMER_REAL, ``0``, ``0``)`|
|`38`||
|`39`|`   ``try``:`|
|`40`|`       ``event_pairs ``=``self``._impl.poll(poll_timeout)`|
|`41`|`   ``except````Exception as e:```|
|`42`|`       ````# Depending on python version and IOLoop implementation,```|
|`43`|`       ````# different exception types may be thrown and there are```|
|`44`|`       ````# two ways EINTR might be signaled:```|
|`45`|`       ````# * e.errno == errno.EINTR```|
|`46`|`       ````# * e.args is like (errno.EINTR, 'Interrupted system call')```|
|`47`|`       ``if``(``getattr``(e, ``'errno'``, ``None``) ``=``=``errno.EINTR ``or`|
|`48`|`           ``(``isinstance``(``getattr``(e, ``'args'``, ``None``), ``tuple``) ``and`|
|`49`|`            ``len``(e.args) ``=``=``2``and``e.args[``0``] ``=``=``errno.EINTR)):`|
|`50`|`           ``continue`|
|`51`|`       ``else``:`|
|`52`|`           ``raise`|
|`53`||
|`54`|`   ``if``self``._blocking_signal_threshold ``is``not``None``:`|
|`55`|`       ``signal.setitimer(signal.ITIMER_REAL,`|
|`56`|`                        ``self``._blocking_signal_threshold, ``0``)`|
|`57`||
|`58`|`   ````# Pop one fd at a time from the set of pending fds and run```|
|`59`|`   ````# its handler. Since that handler may perform actions on```|
|`60`|`   ````# other file descriptors, there may be reentrant calls to```|
|`61`|`   ````# this IOLoop that update self._events```|
|`62`|`   ``self``._events.update(event_pairs)`|
|`63`|`   ``while``self``._events:`|
|`64`|`       ````fd, events````=``self``._events.popitem()`|
|`65`|`       ``try``:`|
|`66`|`           ``self````._handlers[fd](fd, events)```|
|`67`|`       ``except````(OSError, IOError) as e:```|
|`68`|`           ``if``e.args[``0``] ``=``=``errno.EPIPE:`|
|`69`|`               ````# Happens when the client closes the connection```|
|`70`|`               ``pass`|
|`71`|`           ``else``:`|
|`72`|`               ``app_log.error(````"Exception in I/O handler for fd %s"````,`|
|`73`|`                             ````fd, exc_info````=``True``)`|
|`74`|`       ``except``Exception:`|
|`75`|`           ``app_log.error(````"Exception in I/O handler for fd %s"````,`|
|`76`|`                         ````fd, exc_info````=``True``)`|
首先是设定超时时间。然后在互斥锁下取出上次循环遗留下的回调列表（在add_callback添加对象），把这次列表置空，然后依次执行列表里的回调。这里的_run_callback就没什么好分析的了。紧接着是检查上次循环遗留的超时列表，如果列表里的项目有回调而且过了截止时间，那肯定超时了，就执行对应的超时回调。然后检查是否又有了事件回调（因为很多回调函数里可能会再添加回调），如果是，则不在poll循环里等待，如注释所述。接下来最关键的一句是event_pairs = self._impl.poll(poll_timeout)，这句里的_impl是epoll，在platform/epoll.py里定义，总之就是一个等待函数，当有事件（超时也算）发生就返回。然后把事件集保存下来，对于每个事件，self._handlers[fd](fd,
 events)根据fd找到回调，并把fd和事件做参数回传。如果fd是监听的fd，那么这个回调handler就是accept_handler函数，详见上节代码。如果是新fd可读，一般就是_on_headers 或者 _on_requet_body了，详见前几节。我好像没看到可写时的回调？以上，就是循环的流程了。可能还是看的糊里糊涂的，因为很多对象怎么来的都不清楚，configurable也还没有看。看完下面的分析，应该就可以了。
[Configurable](http://www.nowamagic.net/academy/tag/Configurable)类在util.py里被定义。类里有一段注释，已经很明确的说明了它的设计意图和用法。它是可配置接口的父类，可配置接口对外提供一致的接口标识，但它的子类实现可以在运行时进行configure。一般在跨平台时由于子类实现有多种选择，这时候就可以使用可配置接口，例如select和epoll。首先注意
 Configurable 的两个函数： configurable_base 和 configurable_default， 两函数都需要被子类（即可配置接口类）覆盖重写。其中，base函数一般返回接口类自身，default返回接口的默认子类实现，除非接口指定了__impl_class。IOLoop及其子类实现都没有初始化函数也没有构造函数，其构造函数继承于Configurable，如下：
|`01`|`def``__new__(``cls``, ``*``*``kwargs):`|
|`02`|`    ``base ``=``cls``.configurable_base()`|
|`03`|`    ``args ``=``{}`|
|`04`|`    ``if``cls``is``base:`|
|`05`|`        ``impl ``=``cls``.configured_class()`|
|`06`|`        ``if``base.__impl_kwargs:`|
|`07`|`            ``args.update(base.__impl_kwargs)`|
|`08`|`    ``else``:`|
|`09`|`        ``impl ``=``cls`|
|`10`|`    ``args.update(kwargs)`|
|`11`|`    ``instance ``=``super``(Configurable, ``cls``).__new__(impl)`|
|`12`|`    ````# initialize vs __init__ chosen for compatiblity with AsyncHTTPClient```|
|`13`|`    ````# singleton magic.  If we get rid of that we can switch to __init__```|
|`14`|`    ````# here too.```|
|`15`|`    ``instance.initialize(``*``*``args)`|
|`16`|`    ``return``instance`|
当子类对象被构造时，子类__new__被调用，因此参数里的cls指的是Configurabel的子类（可配置接口类，如IOLoop）。先是得到base，查看IOLoop的代码发现它返回的是自身类。由于base和cls是一样的，所以调用configured_class()得到接口的子类实现，其实就是调用base（现在是IOLoop）的configurable_default，总之就是返回了一个子类实现（epoll/kqueue/select之一），顺便把__impl_kwargs合并到args里。接着把kwargs并到args里。然后调用Configurable的父类（Object）的__new__方法，生成了一个impl的对象，紧接着把args当参数调用该对象的initialize（继承自PollIOloop，其initialize下段进行分析），返回该对象。
所以，当构造IOLoop对象时，实际得到的是EPollIOLoop或其它相似子类。另外，Configurable 还提供configure方法来给接口指定实现子类和参数。可以看的出来，Configurable类主要提供构造方法，相当于对象工厂根据配置来生产对象，同时开放configure接口以供配置。而子类按照约定调整配置即可得到不同对象，代码得到了复用。
解决了构造，来看看IOLoop的instance方法。先检查类是否有成员_instance，一开始肯定没有，于是就构造了一个IOLoop对象（即EPollIOLoop对象）。以后如果再调用instance，得到的则是已有的对象，这样就确保了ioloop在全局是单例。再看epoll循环时注意到self._impl，Configurable 和 IOLoop 里都没有， 这是在哪儿定义的呢？ 为什么IOLoop的start跑到PollIOLoop里，应该是EPollIOLoop才对啊。 对，应该看出来了，EPollIOLoop
 就是PollIOLoop的子类，所以方法被继承了是很常见的哈。
从上一段的构造流程里可以看到，EPollIOLoop对象的initialize方法被调用了，看其代码发现它调用了其父类（PollIOLoop）的它方法, 并指定了impl=select.epoll(), 然后在父类的方法里就把它保存了下来，所以self._impl.poll就等效于select.epoll().poll().PollIOLoop里还有一些注册，修改，删除监听事件的方法，其实就是对self._impl的封装调用。就如上节的 add_accept_handler 就是调用ioloop的add_handler方法把监听fd和accept_handler方法进行关联。
IOLoop基本是个事件循环，因此它总是被其它模块所调用。而且为了足够通用，基本上对回调没多大限制，一个可执行对象即可。事件分发就到此结束了，和IO事件密切相关的另一个部分是IOStream，看看它是如何读写的。
