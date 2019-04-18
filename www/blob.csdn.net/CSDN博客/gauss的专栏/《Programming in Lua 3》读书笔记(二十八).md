# 《Programming in Lua 3》读书笔记(二十八) - gauss的专栏 - CSDN博客
2014年11月14日 13:21:23[gauss](https://me.csdn.net/mathlmx)阅读数：251
个人分类：[脚本](https://blog.csdn.net/mathlmx/article/category/949452)

PartⅣ The C API
32 Memory Management
Lua是动态的对其数据结构进行管理的。所有数据结构按需求进行增长，最终进行释放。Lua对其内存的使用控制较为严格，当我们关闭了一个Lua state的时候，Lua会立即释放其内存占用。不仅如此，Lua中的所有对象都被垃圾回收器管理，包括tables，strings，和functions，threads，和modules。
Lua进行内存管理的方式对多数程序来说都是挺合适的。但是还是有一些特殊的程序需要做一定的适配，如运行在内存紧张的环境中，或者减少内存回收间隔到最小值。Lua允许从两个等级来对这些情况做适配:低等级的时候，设定Lua使用的内存管理函数进行控制；高等级的时候，可以设定一些参数来控制Lua的垃圾回收器，或者甚至我们可以全靠操控这个回收器。在这一节将会对Lua的内存管理进行介绍。
32.1 The Allocation Function
Lua进行内存分配的时候，既不调用malloc 也不调用 realloc函数，而是通过allocation function 进行内存分配和内存回收，这个函数用户必须在创建lua state的时候提供何时创建的。
之前提到的创建lua状态的函数luaL_newstate是一个带默认内存分配函数的辅助函数。其所带的内存分配函数使用C标准库中的标准malloc - realloc - free 函数，这对大多数程序来说是足够用的。当我们需要对Lua的内存管理进行全盘接管的时候，就需要使用另一个函数:lua_newstate:
lua_State *lua_newstate(lua_Alloc f, void *ud);
这个函数接受两个参数：第一个是一个内存分配函数，第二个参数是user data.通过这个函数创建的state总是通过调用f来实现内存的分配和回收。
lua_Alloc 的被定义为：
```cpp
typedef void * (*lua_Alloc) (void *ud,void *ptr,size_t osize,size_t nsize);
```
第一个参数是我们提供给lua_newstate 的user data；第二个参数是被分配或者被回收的内存块的地址；第三个参数是原有的内存块大小；最后一个参数为需要分配的大小。
Lua定义NULL为size为0的内存块。当 nsize 为0的时候，内存管理函数将会将释放 prt指向的内存块然后返回NULL，这样符合了分配的大小为0的要求；当ptr 为NULL的时候，函数将会分配并返回给定大小的内存块；当不能分配给定的大小的时候，将会返回NULL；如果ptr 为NULL且 nsize 为0，之前的两种情况都会发生：返回NULL，内存分配函数不做任何工作。
当ptr 不为 NULL且 nsize 不为0的时候，内存管理函数将会对内存块进行再分配，类似于realloc操作，然后返回新的地址。当发生错误的时候，将会返回NULL。 Lua假设内存管理过程中当新的分配大小不小于原有的大小时候是不会发生错误的 。
luaL_newstate所用的标准内存管理函数定义如下：
void *l_alloc (void *ud,void *ptr ,size_t osize,size_t nsize)
{
if (nsize == 0)
{
free(ptr);
return NULL;
}
elsereturn realloc(ptr,nsize);
}
函数假设free(NULL)不会做任何操作，而realloc(NULL,size) 等于 malloc(size)。ANSI C标准是支持这两个假设的。
可以使用函数lua_getallocf 得到一个Lua state的内存管理器：
lua_Alloc lua_getallocf(lua_State *L,void **ud);
当参数 ud为NULL的时候，函数将会设置 ud为这个state的user state的值。当然，可以通过函数lua_setallocf 来改变一个state的内存管理器：
void lua_setallocf(lua_State *L,lua_Alloc f, void *ud);
这里要记住：任何新的管理器都需要负责管理由原有管理器控制的内存管理工作。或者说，新的管理器是原有管理器的一个包装器。
32.2 The Garbage Collector
直到Lua5.0版本，Lua都是使用简单的mark-sweep 模式的garbage collector(GC).这个回收器有时也被称为"stop the world"回收器。这个意味着，Lua会时不时的停止解释主程序的来执行垃圾回收循环工作。每次循环经历三个步骤：mark，cleaning，sweep。
Lua从其根集合开始标记其对象的alive，包含那些能直接访问的对象：registry和主线程。任何存储在live object中的对象都是能被程序访问到的，也因此被标记为alive。当所有能访问到的对象都被标记为了alive后，mark步骤工作执行结束。
在开始sweep步骤之前，Lua先执行cleaning 步骤，这个步骤与finalizer和weak table有关。首先，先遍历所有由finalization标记的对象看是否有未标记的，被标记为alive的对象会被存储在一个单独的链表中，供finalization 阶段使用。然后，Lua遍历weak table 从中移除那些key或者value未被标记的条目。
sweep阶段将会遍历所有的lua 对象。如果一个对象未被标记为alive的，lua会回收掉。而标记为alive的，lua会清除其标记状态，为下一次回收循环准备。在sweep阶段，lua也会调用finalizer中在cleaning阶段单独存储的那些对象。
在Lua5.1版本中新加入了一个incremental collector，这个回收器与之前的那个回收器工作原理类似，只是不会在其运行过程中停止对主程序的解释。当这个回收器工作的时候，解释器可能会改变某个受回收器影响的对象的访问性，不影响主程序的运行。
Gargage-collector API
Lua提供了一些API接口，让我们对回收器做额外的一些控制：
在C中，使用lua_gc:
int lua_gc(lua_State *L,int what,int data);
在Lua中，使用collectgarbage函数：
`collectgarbage(what[ ,data])`
两个函数都提供了相同的功能：what 参数代表要干嘛，操作有：
LUA_GCSTOP("stop") ：停止回收器，直到再一次用参数"restart"调用collectgarbage(lua_gc);
LUA_GCRESTART("restart") :重启回收器；
LUA_GCCOLLECT("collect") : 执行一个完整的垃圾回收循环，保证所有未能被访问到的对象都被回收和finalized。这个是colectgarbage的默认操作；
LUA_GCSTEP("step") :执行一些垃圾回收工作，这些工作数量等于分配了data byte之后回收器将会做的工作的数量；
LUA_GCCOUNT("count") : 返回Lua当前使用的内存数量(千字节)。也包括没有被回收掉的那些内存数量。
LUA_GCCOUNTB(not available) ：返回Lua使用的内存数量(千字节的小数部分。
LUA_GCSETPAUSE("setpause") :设置回收器的pause 参数，使用百分比单位，即当设定的data值为100，将会设定1(100%).用来控制两次回收循环的间隔时间。
LUA_GCSETSTEPMUL("setstepmul"): 设置回收器的step multiplier 参数。也是百分比单位。控制每次回收工作要回收多少数量的内存。
算是马马虎虎，勉勉强强的把这本书看过一遍了吧，看纯英文版的技术类书籍还是有点难度，主要是一些技术概念还不懂，因为之前没有接触过Lua这门编程语言，直接上外文书籍有点吃力，想必经过看这本书的历程，之后再学习应该难度会降低不少吧。
