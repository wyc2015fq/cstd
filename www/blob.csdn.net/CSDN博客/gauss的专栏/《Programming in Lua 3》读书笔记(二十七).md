# 《Programming in Lua 3》读书笔记(二十七) - gauss的专栏 - CSDN博客
2014年11月14日 13:23:11[gauss](https://me.csdn.net/mathlmx)阅读数：242
个人分类：[脚本](https://blog.csdn.net/mathlmx/article/category/949452)

PartⅣ The C API 
31 Threads and States
     Lua实际上是不支持真正的多线程的，即preemptive threads 共享内存的情况。有两个原因：第一个，是ANSI C不支持，所以Lua中就没有合适的方法来实现这个机制；第二个原因，最重要的是作者认为Lua中支持真正的多线程不是个好主意。
     多线程是给低级开发语言而发展出的机制。多线程概念中的信号和监控等同步机制机制都是依赖与操作系统而不是依赖于应用程序的，因此很难去发现和修正由多线程造成的bug，而且这些bug中的一些可能会造成安全问题。不仅如此，多线程也可能会对程序运行过程由于需要做信息同步而造成性能影响，如影响程序的内存分配。
     由多线程带来的问题主要是从preemptive thread 和 内存共享结合 中产生的，因此Lua中通过使用 non-preemptive thread 和不共享内存来避免这些问题。Lua的线程(也叫协同程序)是协作的，因此避免了由不可预期的线程切换所带来的问题，且Lua的State是不共享内存的，因此在为并发性提供了一个良好基础。这一章主要是讲这个两点。

**31.1 Multiple Threads**
     Lua中的线程本质上是一个协同程序。我们可以认为一个协同程序就是一个线程加上一个良好的接口，或者我们可以认为一个线程就是带一个低等级API的协同程序。
     从C API角度来看，认为一个线程就是一个栈是挺有用的。而从实现过程角度来看，线程确实是一个栈。每个栈保存所有未执行的调用、每次调用的参数和局部变量等信息，换句话来说一个栈拥有一个线程要准备再次运行的所有信息。因此，多线程就是意味着多个独立的栈。
     实际上看，我们之前讨论的各个Lua-C 的API函数，都是在一个特定的栈上操作的，这个从一开始就知道了。但是我们又是如何知道我们要操作具体的哪个栈呢？会不会操作错了？这里关键点就在于我们每次调用函数时的第一个参数类型lua_State ,这个参数不仅代表一个lua state，也同时包括在这个state中的thread。
     无论何时创建了一个Lua state，Lua都会自动在这个state中创建一个新的线程，然后返回一个代表这个线程的lua_State。这个主线程是从不会被回收的(这个？？是否指的除去调用lua_newthread额外创建的这些线程，即创建lua state自动创建的这个线程)。当你用lua_close 关闭了这个state的时候，线程会随同state一起释放。
     可以在一个state中通过调用lua_newthread 来创建另外的thread：
**[plain]**[view
 plain](http://blog.csdn.net/zh379835552/article/details/39161217#)[copy](http://blog.csdn.net/zh379835552/article/details/39161217#)[print](http://blog.csdn.net/zh379835552/article/details/39161217#)[?](http://blog.csdn.net/zh379835552/article/details/39161217#)
- lua_State *lua_newthread(lua_State *L);  
     上面这个函数返回一个代表这个新线程的lua_State 型指针，并且同时将这个新的线程作为thread类型推进栈中，如：
**[plain]**[view
 plain](http://blog.csdn.net/zh379835552/article/details/39161217#)[copy](http://blog.csdn.net/zh379835552/article/details/39161217#)[print](http://blog.csdn.net/zh379835552/article/details/39161217#)[?](http://blog.csdn.net/zh379835552/article/details/39161217#)
- L1 = lua_newthread(L);  
     经过上面这步，我们就有了两个线程了，且两个线程都实际上在同一个lua state内部。每个线程都有自己独立的栈，新的线程L1有个空的栈；而L线程则将新的线程存储在其自身栈的栈顶。如：
**[plain]**[view
 plain](http://blog.csdn.net/zh379835552/article/details/39161217#)[copy](http://blog.csdn.net/zh379835552/article/details/39161217#)[print](http://blog.csdn.net/zh379835552/article/details/39161217#)[?](http://blog.csdn.net/zh379835552/article/details/39161217#)
- e.g.  
- printf("%d\n",lua_gettop(L1));                    --0 新的线程的栈是一个空的栈  
- printf("%s\n",luaL_typename(L,-1));           --thread 代表在栈顶是一个线程，即新的那个线程  
     除了主线程之外，其余的这些线程都是受garbage collection管理的对象，和Lua其余的这些对象一样。当创建了一个新的线程，以thread类型将这个线程推进至栈中，保证了这个线程不会被回收掉。我们应该确保不使用那些不确定还是否在栈中的线程。每次调用Lua的API都可能会触发回收那些不在栈中的线程，即便这个线程还在使用中，如：
**[plain]**[view
 plain](http://blog.csdn.net/zh379835552/article/details/39161217#)[copy](http://blog.csdn.net/zh379835552/article/details/39161217#)[print](http://blog.csdn.net/zh379835552/article/details/39161217#)[?](http://blog.csdn.net/zh379835552/article/details/39161217#)
- lua_State *L1 = lua_newthread(L);  
- lua_pop(L,1);     /* L1 is garbage*/  
- lua_pushstring(L1,"helloe");     /* danger */  
     调用lua_pushstring的时候可能会触发garbage collector回收掉L1，即便此时仍然在使用这个线程。为了避免遇到不可预期的错误，应该始终对我们在使用中的线程保持引用，for instance in the stack of an anchored thread or in the registry.
     新创建的线程，可以和使用主线程一样使用。可以从该线程的栈中推进和推出元素，可以使用栈来调用函数等等。这里介绍一个函数lua_xmove(L1,L,1),这个函数的作用是做同一个state中的栈的数据移动，如lua_xmove(F，T，n)，就是从栈F中推出一个 元素，然后推进至栈T中。做这个操作我们甚至都不需要创建额外新的线程，只需要在主线程中处理就可以了。在这里使用多线程的前提是实现协同程序，以便我们可以延缓或者恢复某个线程的执行。恢复执行某个函数,在这里就需要函数:lua_resume 来支持：
int lua_resume(lua_State *L,lua_State *from ,int nary);
     如我们使用lua_pcall 一样，如果我们想运行一个协同程序，我们可以使用lua_resume函数：将要调用的函数,参数推进至栈中，函数参数narg代表传递的参数的数量。(from 参数指的是执行这个调用的线程所在的state)。主要的行为类似于lua_pcall,但还是有三个不同的地方：1、lua_resume没有代表返回值数量的参数，函数总是会返回所有的返回结果；2、没有给消息句柄用的参数，错误不会在栈中触发，因此可以在错误之后检测栈(~~~没看懂)；3、如果在运行的这个程序被挂起了，lua_resume
 返回一个特殊代码：LUA_YIELD ，且将在state中的线程保留下来留给之后重新调用。
     协同程序也能调用C函数，这些C函数也能回调其他的Lua函数。

**31.2 Lua State**
     每次调用luaL_newstate函数将会创建一个新的lua state.不同的Lua state相互之间是独立的，这就意味着一个state内部的事件是不会对其他的state产生影响的，也意味着不能在state间直接进行数据交互，而是需要借助C代码。如：
lua_pushstring(L2,lua_tostring(L1,-1);
     上述代码将L1栈顶的元素借助C API传递到了L2中。因为我们是需要借助C API来实现数据交互的，所以传递的数据类型必须是C所支持的类型，像table之类的数据需要借助合适的方式首先进行转换才可以传递。
     在支持多线程的系统中，为每个线程创建独立的Lua state这个结构值得去探讨一番。这个结构的结果类似与UNIX中的processes，在这里我们可以不shared memory就可以得到并发性。
     这个小节剩下的部分是作者介绍他用POSIX 线程实现上述提到的为每个线程创建独立的Lua state，感觉有点难懂，就不再笔记了。留给以后再深入理解一番。
[http://blog.csdn.net/zh379835552/article/details/39161327](http://blog.csdn.net/zh379835552/article/details/39161327)
