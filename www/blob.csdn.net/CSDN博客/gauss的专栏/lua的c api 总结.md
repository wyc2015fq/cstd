# lua的c api 总结 - gauss的专栏 - CSDN博客
2014年10月19日 13:17:03[gauss](https://me.csdn.net/mathlmx)阅读数：1280
                
lua和c的交互入门例子,功能是把字符串中的key=value字符串全部转换成XML格式<key>value</key>
代码如下：
使用lua的string.gsub 函数转换字符串（作为例子）:
**[cpp]**[view plain](http://blog.csdn.net/chenjiayi_yun/article/details/18893789#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/18893789#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/455748/fork)
- int main()  
- {  
- //Lua示例代码
- char *szLua_code =  
- "r = string.gsub(c_Str, c_Mode, c_Tag) --宿主给的变量 "
- "u = string.upper(r)";  
- //Lua的字符串模式
- char *szMode = "(%w+)%s*=%s*(%w+)";  
- //要处理的字符串
- char *szStr = "key1 = value1 key2 = value2";  
- //目标字符串模式
- char *szTag = "<%1>%2</%1>";  
- 
-     lua_State *L = luaL_newstate();  
-     luaL_openlibs(L);  
- 
- //把3个变量（c_Str, c_Mode, c_Tag）发送到Lua虚拟机
-     lua_pushstring(L, szMode);//把C字符串压入栈顶
-     lua_setglobal(L, "c_Mode");//lua_setglobal的作用是把栈顶的数据传到Lua环境中作为全局变量
-     lua_pushstring(L, szTag);  
-     lua_setglobal(L, "c_Tag");  
-     lua_pushstring(L, szStr);  
-     lua_setglobal(L, "c_Str");  
- 
- //执行
- bool err = luaL_loadbuffer(L, szLua_code, strlen(szLua_code),  
- "demo") || lua_pcall(L, 0, 0, 0);  
- if(err)  
-     {  
- //如果错误，显示
-        cerr << lua_tostring(L, -1);  
- //弹出栈顶的这个错误信息
-        lua_pop(L, 1);  
-     }  
- else
-     {  
- //Lua执行后取得全局变量的值
-        lua_getglobal(L, "r");//从Lua环境中取得全局变量压入栈顶
-        cout << "r = " << lua_tostring(L,-1) << endl;//把栈顶的数据转成字符串
-        lua_pop(L, 1);//调用前与调用后栈里的数据量不变
-        lua_getglobal(L, "u");  
-        cout << "u = " << lua_tostring(L,-1) << endl;      
-        lua_pop(L, 1);  
-     }  
-     lua_close(L);  
- return 0;  
- }  
C++和Lua之间交互接口一直是依赖着栈。Lua C API就是用于操作栈的，操作列举如下：
# 1、虚拟机维护
lua_State *lua_newstate (lua_Alloc f, void *ud);
创建的一个新的独立的状态机。如果创建不了（因为内存问题）返回 NULL 。参数 f 是一个分配器函数； Lua 将通过这个函数做状态机内所有的内存分配操作。第二个参数 ud ，这个指针将在每次调用分配器时被直接传入。 
________________________________________
void lua_close (lua_State *L);
销毁指定 Lua 状态机中的所有对象（如果有垃圾收集相关的元方法的话，会调用它们），并且释放状态机中使用的所有动态内存。在一些平台上，你可以不必调用这个函数，因为当宿主程序结束的时候，所有的资源就自然被释放掉了。另一方面，长期运行的程序，比如一个后台程序或是一个 web 服务器，当不再需要它们的时候就应该释放掉相关状态机。这样可以避免状态机扩张的过大。 
________________________________________
lua_Alloc lua_getallocf (lua_State *L, void **ud);
返回给定状态机的内存分配器函数。如果 ud 不是 NULL ，Lua 把调用 lua_newstate 时传入的那个指针放入 *ud 。 
________________________________________
lua_setallocf
void lua_setallocf (lua_State *L, lua_Alloc f, void *ud);
把指定状态机的分配器函数换成带上指针 ud 的 f 。 
# 2、存取lua全局变量
void lua_setglobal (lua_State *L, const char *name);
作用是把栈顶的数据传到Lua环境中作为全局变量name 
由一个宏定义出来：
#define lua_setglobal(L,s) lua_setfield(L, LUA_GLOBALSINDEX, s)
void lua_getglobal (lua_State *L, const char *name);
把lua全局变量 name 里的值压入堆栈。 这个是用一个宏定义出来的：
#define lua_getglobal(L,s) lua_getfield(L, LUA_GLOBALSINDEX, s)
# 3、压入元素到栈里
void lua_pushboolean (lua_State *L, int b);
把 b 作为一个 boolean 值压入堆栈。 
________________________________________
void lua_pushcclosure (lua_State *L, lua_CFunction fn, int n);
把一个新的 C closure 压入堆栈。 
当创建了一个 C 函数后，你可以给它关联一些值，这样就是在创建一个 C closure （参见 §3.4）；接下来无论函数何时被调用，这些值都可以被这个函数访问到。为了将一些值关联到一个 C 函数上，首先这些值需要先被压入堆栈（如果有多个值，第一个先压）。接下来调用 lua_pushcclosure 来创建出 closure 并把这个 C 函数压到堆栈上。参数 n 告之函数有多少个值需要关联到函数上。
 lua_pushcclosure 也会把这些值从栈上弹出。 
________________________________________
void lua_pushcfunction (lua_State *L, lua_CFunction f);
将一个 C 函数压入堆栈。这个函数接收一个 C 函数指针，并将一个类型为 function 的 Lua 值压入堆栈。当这个栈定的值被调用时，将触发对应的 C 函数。 
注册到 Lua 中的任何函数都必须遵循正确的协议来接收参数和返回值（参见 lua_CFunction）。 
lua_pushcfunction 是作为一个宏定义出现的： 
     #define lua_pushcfunction(L,f)  lua_pushcclosure(L,f,0)
________________________________________
const char *lua_pushfstring (lua_State *L, const char *fmt, ...);
把一个格式化过的字符串压入堆栈，然后返回这个字符串的指针。它和 C 函数 sprintf 比较像，不过有一些重要的区别： 
*        摸你需要为结果分配空间：其结果是一个 Lua 字符串，由 Lua 来关心其内存分配（同时通过垃圾收集来释放内存）。 
*        这个转换非常的受限。不支持 flag ，宽度，或是指定精度。它只支持下面这些： '%%' （插入一个 '%'）， '%s' （插入一个带零终止符的字符串，没有长度限制）， '%f' （插入一个 lua_Number）， '%p' （插入一个指针或是一个十六进制数）， '%d' （插入一个 int)， '%c' （把一个 int 作为一个字符插入）。 
________________________________________
void lua_pushinteger (lua_State *L, lua_Integer n);
把 n 作为一个整形数压栈。 
________________________________________
void lua_pushlightuserdata (lua_State *L, void *p);
把一个 light userdata 压栈。 
userdata 在 Lua 中表示一个 C 值。 light userdata 表示一个指针。它是一个像数字一样的值：你不需要专门创建它，它也没有独立的 metatable ，而且也不会被收集（因为从来不需要创建）。只要表示的 C 地址相同，两个 light userdata 就相等。 
________________________________________
void lua_pushlstring (lua_State *L, const char *s, size_t len);
把指针 s 指向的长度为 len 的字符串压栈。 Lua 对这个字符串做一次内存拷贝（或是复用一个拷贝），因此 s 处的内存在函数返回后，可以释放掉或是重用于其它用途。字符串内可以保存有零字符。 
________________________________________
void lua_pushnil (lua_State *L);
把一个 nil 压栈。 
________________________________________
void lua_pushnumber (lua_State *L, lua_Number n);
把一个浮点数 n 压栈。 
________________________________________
void lua_pushstring (lua_State *L, const char *s);
把指针 s 指向的以零结尾的字符串压栈。 Lua 对这个字符串做一次内存拷贝（或是复用一个拷贝），因此 s 处的内存在函数返回后，可以释放掉或是重用于其它用途。字符串中不能包含有零字符；第一个碰到的零字符会认为是字符串的结束。 
________________________________________
int lua_pushthread (lua_State *L);
把 L 中提供的线程压栈。如果这个线程是当前状态机的主线程的话，返回 1 。 
________________________________________
void lua_pushvalue (lua_State *L, int index);
把堆栈上给定有效处索引处的元素作一个拷贝压栈。 
________________________________________
const char *lua_pushvfstring (lua_State *L,
                              const char *fmt,
                              va_list argp);
等价于 lua_pushfstring，不过是用 va_list 接收参数，而不是用可变数量的实际参数。 
________________________________________
void *lua_newuserdata (lua_State *L, size_t size)
创建C值到栈
该函数分配一块由size指定大小的内存块, 并放在栈顶
返回值是新分配的块的地址
栈+1， 栈顶是userdata
userdata用来在lua中表示c中的值. 一个完整的userdata有自己的元表, 在垃圾回收时, 可以调用它的元表的__gc方法
________________________________________
void lua_pushcclosure (lua_State *L, lua_CFunction fn, int n)
向栈上压一个C闭包
注册c函数到lua中, 其实没有这回事, lua中只有c闭包
当一个c函数被创建时, 可以绑定几个值在它上面, 从而形成一个闭包.  在任何时刻调用这个c函数时, 都可以访问这几个绑定值. 
绑定的方法: 先一次压入要绑定的n个值到栈上, 然后调用lua_pushcclosure(L, fn, n)这样就形成的一个c闭包
无返回值
栈 –(n - 1) , 一共弹出n个元素(及那些绑定的值), 压入一个cclosure
#define lua_pushcfunction(L, f) lua_pushcclosure(L, f, 0)
#define lua_register(L, n, f) (lua_pushcfunction(L, f), lua_setglobal(L, n))
没有返回值
栈不变化
这个是比较常用的, 以n为lua中的key压入一个0个绑定值的cclosure.
________________________________________
int lua_load (lua_State *L,
              lua_Reader reader,
              void *data,
              const char *chunkname);
加载一个 Lua chunk 。如果没有错误， lua_load 把一个编译好的 chunk 作为一个 Lua 函数压入堆栈。否则，压入出错信息。 lua_load 的返回值可以是： 
*        0: 没有错误； 
*        LUA_ERRSYNTAX: 在预编译时碰到语法错误； 
*        LUA_ERRMEM: 内存分配错误。 
这个函数仅仅加栽 chunk ；而不会去运行它。 
lua_load 会自动检测 chunk 是文本的还是二进制的，然后做对应的加载操作（参见程序 luac）。 
lua_load 函数使用一个用户提供的 reader 函数来读取 chunk （参见 lua_Reader）。 data 参数会被传入读取器函数。 
chunkname 这个参数可以赋予 chunk 一个名字，这个名字被用于出错信息和调试信息（参见 §3.8）。 
________________________________________
lua_State *lua_newthread (lua_State *L);
创建一个新线程，并将其压入堆栈，并返回维护这个线程的 lua_State 指针。这个函数返回的新状态机共享原有状态机中的所有对象（比如一些 table），但是它有独立的执行堆栈。 
没有显式的函数可以用来关闭或销毁掉一个线程。线程跟其它 Lua 对象一样是垃圾收集的条目之一。 
________________________________________
void *lua_newuserdata (lua_State *L, size_t size);
这个函数分配分配一块指定大小的内存块，把内存块地址作为一个完整的 userdata 压入堆栈，并返回这个地址。 
userdata 代表 Lua 中的 C 值。完整的 userdata 代表一块内存。它是一个对象（就像 table 那样的对象）：你必须创建它，它有着自己的元表，而且它在被回收时，可以被监测到。一个完整的 userdata 只和它自己相等（在等于的原生作用下）。 
当 Lua 通过 gc 元方法回收一个完整的 userdata 时， Lua 调用这个元方法并把 userdata 标记为已终止。等到这个 userdata 再次被收集的时候，Lua 会释放掉相关的内存。 
________________________________________
lua_register
void lua_register (lua_State *L,
                   const char *name,
                   lua_CFunction f);
把 C 函数 f 设到全局变量 name 中。它通过一个宏定义： 
     #define lua_register(L,n,f) /
            (lua_pushcfunction(L, f), lua_setglobal(L, n))
# 4、查询栈里的元素
lua_isboolean
int lua_isboolean (lua_State *L, int index);
当给定索引的值类型为 boolean 时，返回 1 ，否则返回 0 。 
________________________________________
lua_iscfunction
int lua_iscfunction (lua_State *L, int index);
当给定索引的值是一个 C 函数时，返回 1 ，否则返回 0 。 
________________________________________
lua_isfunction
int lua_isfunction (lua_State *L, int index);
当给定索引的值是一个函数（ C 或 Lua 函数均可）时，返回 1 ，否则返回 0 。 
________________________________________
lua_islightuserdata
int lua_islightuserdata (lua_State *L, int index);
当给定索引的值是一个 light userdata 时，返回 1 ，否则返回 0 。 
________________________________________
lua_isnil
int lua_isnil (lua_State *L, int index);
当给定索引的值是 nil 时，返回 1 ，否则返回 0 。 
________________________________________
lua_isnumber
int lua_isnumber (lua_State *L, int index);
当给定索引的值是一个数字，或是一个可转换为数字的字符串时，返回 1 ，否则返回 0 。 
________________________________________
lua_isstring
int lua_isstring (lua_State *L, int index);
当给定索引的值是一个字符串或是一个数字（数字总能转换成字符串）时，返回 1 ，否则返回 0 。 
________________________________________
lua_istable
int lua_istable (lua_State *L, int index);
当给定索引的值是一个 table 时，返回 1 ，否则返回 0 。 
________________________________________
lua_isthread
int lua_isthread (lua_State *L, int index);
当给定索引的值是一个 thread 时，返回 1 ，否则返回 0 。 
________________________________________
lua_isuserdata
int lua_isuserdata (lua_State *L, int index);
当给定索引的值是一个 userdata （无论是完整的 userdata 还是 light userdata ）时，返回 1 ，否则返回 0 。 
________________________________________
int lua_type (lua_State *L, int index);
返回给定索引处的值的类型，当索引无效时则返回 LUA_TNONE （那是指一个指向堆栈上的空位置的索引）。 lua_type 返回的类型是一些个在 lua.h 中定义的常量： LUA_TNIL ， LUA_TNUMBER ， LUA_TBOOLEAN ， LUA_TSTRING ， LUA_TTABLE ， LUA_TFUNCTION ， LUA_TUSERDATA ， LUA_TTHREAD
 ， LUA_TLIGHTUSERDATA 。 
________________________________________
lua_typename
const char *lua_typename  (lua_State *L, int tp);
返回 tp 表示的类型名，这个 tp 必须是 lua_type 可能返回的值中之一。 
# 5、转换栈里的元素
int lua_toboolean (lua_State *L, int index);
把指定的索引处的的 Lua 值转换为一个 C 中的 boolean 值（ 0 或是 1 ）。和 Lua 中做的所有测试一样， lua_toboolean 会把任何不同于 false 和 nil 的值当作 1 返回；否则就返回 0 。如果用一个无效索引去调用也会返回 0 。（如果你想只接收真正的 boolean 值，就需要使用 lua_isboolean 来测试值的类型。） 
________________________________________
lua_CFunction lua_tocfunction (lua_State *L, int index);
把给定索引处的 Lua 值转换为一个 C 函数。这个值必须是一个 C 函数；如果不是就返回 NULL 。 
________________________________________
lua_Integer lua_tointeger (lua_State *L, int idx);
把给定索引处的 Lua 值转换为 lua_Integer 这样一个有符号整数类型。这个 Lua 值必须是一个数字或是一个可以转换为数字的字符串（参见 §2.2.1）；否则，lua_tointeger 返回 0 。 
如果数字不是一个整数，截断小数部分的方式没有被明确定义。 
________________________________________
const char *lua_tolstring (lua_State *L, int index, size_t *len);
把给定索引处的 Lua 值转换为一个 C 字符串。如果 len 不为 NULL ，它还把字符串长度设到 *len 中。这个 Lua 值必须是一个字符串或是一个数字；否则返回返回 NULL 。如果值是一个数字，lua_tolstring 还会把堆栈中的那个值的实际类型转换为一个字符串。（当遍历一个表的时候，把 lua_tolstring 作用在键上，这个转换有可能导致 lua_next
 弄错。） 
lua_tolstring 返回 Lua 状态机中字符串的以对齐指针。这个字符串总能保证 （ C 要求的）最后一个字符为零 ('/0') ，而且它允许在字符串内包含多个这样的零。因为 Lua 中可能发生垃圾收集，所以不保证 lua_tolstring 返回的指针，在对应的值从堆栈中移除后依然有效。 
________________________________________
const char *lua_tostring (lua_State *L, int index);
等价于 lua_tolstring ，而参数 len 设为 NULL 。 
________________________________________
lua_Number lua_tonumber (lua_State *L, int index);
把给定索引处的 Lua 值转换为 lua_Number 这样一个 C 类型（参见 lua_Number ）。这个 Lua 值必须是一个数字或是一个可转换为数字的字符串；否则，lua_tonumber 返回 0 。 
________________________________________
const void *lua_topointer (lua_State *L, int index);
把给定索引处的值转换为一般的 C 指针 (void*) 。这个值可以是一个 userdata ，table ，thread 或是一个 function ；否则，lua_topointer 返回 NULL 。不同的对象有不同的指针。不存在把指针再转回原有类型的方法。 
这个函数通常只为产生 debug 信息用。 
________________________________________
lua_State *lua_tothread (lua_State *L, int index);
把给定索引处的值转换为一个 Lua 线程（由 lua_State* 代表）。这个值必须是一个线程；否则函数返回 NULL 。 
________________________________________
void *lua_touserdata (lua_State *L, int index);
如果给定索引处的值是一个完整的 userdata ，函数返回内存块的地址。如果值是一个 light userdata ，那么就返回它表示的指针。否则，返回 NULL
________________________________________
double          lua_tonumber (lua_State *L, int index);
size_t          lua_strlen (lua_State *L, int index);
# 6、Lua栈的维护
int lua_checkstack (lua_State *L, int extra);
确保堆栈上至少有 extra 个空位。如果不能把堆栈扩展到相应的尺寸，函数返回 false 。这个函数永远不会缩小堆栈；如果堆栈已经比需要的大了，那么就放在那里不会产生变化。 
________________________________________
int lua_dump (lua_State *L, lua_Writer writer, void *data);
把函数 dump 成二进制 chunk 。函数接收栈顶的 Lua 函数做参数，然后生成它的二进制 chunk 。若被 dump 出来的东西被再次加载，加载的结果就相当于原来的函数。当它在产生 chunk 的时候，lua_dump 通过调用函数 writer （参见 lua_Writer）来写入数据，后面的 data 参数会被传入 writer 。
最后一次由写入器 (writer) 返回值将作为这个函数的返回值返回； 0 表示没有错误。 
这个函数不会把 Lua 返回弹出堆栈。 
________________________________________
int lua_error (lua_State *L);
产生一个 Lua 错误。错误信息（实际上可以是任何类型的 Lua 值）必须被置入栈顶。这个函数会做一次长跳转，因此它不会再返回。（参见 luaL_error）。 
________________________________________
int lua_gc (lua_State *L, int what, int data);
控制垃圾收集器。 
这个函数根据其参数 what 发起几种不同的任务： 
*        LUA_GCSTOP: 停止垃圾收集器。 
*        LUA_GCRESTART: 重启垃圾收集器。 
*        LUA_GCCOLLECT: 发起一次完整的垃圾收集循环。 
*        LUA_GCCOUNT: 返回 Lua 使用的内存总量（以 K 字节为单位）。 
*        LUA_GCCOUNTB: 返回当前内存使用量除以 1024 的余数。 
*        LUA_GCSTEP: 发起一步增量垃圾收集。步数由 data 控制（越大的值意味着越多步），而其具体含义（具体数字表示了多少）并未标准化。如果你想控制这个步数，必须实验性的测试 data 的值。如果这一步结束了一个垃圾收集周期，返回返回 1 。 
*        LUA_GCSETPAUSE: 把 data/100 设置为 garbage-collector pause 的新值。函数返回以前的值。 
*        LUA_GCSETSTEPMUL: 把 arg/100 设置成 step multiplier。函数返回以前的值。 
________________________________________
int lua_gettop (lua_State *L);
返回栈顶元素的索引。因为索引是从 1 开始编号的，所以这个结果等于堆栈上的元素个数（因此返回 0 表示堆栈为空）。 
________________________________________
lua_insert
void lua_insert (lua_State *L, int index);
把栈顶元素插入指定的有效索引处，并依次移动这个索引之上的元素。栈中数目没有改变。不要用伪索引来调用这个函数，因为伪索引不是真正指向堆栈上的位置。 
________________________________________
int  lua_gettop (lua_State *L);
    取得栈顶元素的索引，即栈中元素的个数
________________________________________
void lua_settop (lua_State *L, int index);
    设置栈顶索引，即设置栈中元素的个数，如果index<0，则从栈顶往下数，下同
________________________________________
void lua_pushvalue (lua_State *L, int index);
    把栈中指定索引的元素复制一份到栈顶
________________________________________
void lua_remove (lua_State *L, int index);
从给定有效索引处移除一个元素，把这个索引之上的所有元素移下来填补上这个空隙。不能用伪索引来调用这个函数，因为伪索引并不指向真实的栈上的位置。 
________________________________________
void lua_replace (lua_State *L, int index);
从栈顶弹出元素值并将其设置到指定索引位置，栈中的数目减一。不移动任何元素（因此在那个位置处的值被覆盖掉）。 
________________________________________
int  lua_pop(L,n)
    从栈顶弹出n个元素，它是一个lua_settop的包装：#define lua_pop(L,n)  lua_settop(L, -(n)-1)
________________________________________
lua_Reader
typedef const char * (*lua_Reader) (lua_State *L,
                                    void *data,
                                    size_t *size);
lua_load 用到的读取器函数，每次它需要一块新的 chunk 的时候， lua_load 就调用读取器，每次都会传入一个参数 data 。读取器需要返回含有新的 chunk 的一块内存的指针，并把 size 设为这块内存的大小。内存块必须在下一次函数被调用之前一直存在。读取器可以通过返回一个 NULL 来指示 chunk 结束。读取器可能返回多个块，每个块可以有任意的大于零的尺寸。 
________________________________________
lua_Writer
typedef int (*lua_Writer) (lua_State *L,
                           const void* p,
                           size_t sz,
                           void* ud);
由 lua_dump 用到的写入器函数。每次 lua_dump 产生了一块新的 chunk ，它都会调用写入器。传入要写入的缓存 (p) 和它的尺寸 (sz) ，还有 lua_dump 的参数 data 。 
写入器会返回一个错误码： 0 表示没有错误；别的值均表示一个错误，并且会让 lua_dump 停止再次调用写入器。 
# 7、表的操作
在Lua中，table是一个很重要的数据类型，但是没有直接压入table和转换为table的接口。
## (1)lua表访问方式
lua table 的存储和访问如：
可以象C中的数据一样放一组数据，如：
tb = {[1]=1, [2]=2, [3] = 3, [4]=4}
可以象map一样以key=value的方式存放数据，如：
tb = {"abc",12,true,x=10,y=20,z=30}  前三个数据可以用tb[1]~tb[3]取得， 后三个数据通过tb.x, tb.y, tb.z访问
实际上Lua的table中，所有的数据都是以key=value的形式存放的，这句Lua代码也可以写成：
tb = {[1]="abc", [2]=12, [3] = true, ["x"]=10, ["y"]=20, ["z"]=30}
形式就是[key]=value，所谓的tb.x只是tb["x"]的语法糖而已，tb.x和tb["x"]是等价的，数值都是10。
## (2)表相关lua C API
建一个新表
void lua_createtable (lua_State *L, int narr, int nrec)
创建一个新的table, 并把它放在栈顶. narr和nrec分别指定该table的array部分和hash部分的预分配元素数量
无返回值
栈高度+1, 栈顶元素是新table
#define lua_newtable(L) lua_createtable(L, 0, 0) 常用这个
void lua_newtable (lua_State *L);    新建一个空的table并压入栈顶。
________________________________________
 取表中的元素
void lua_getfield (lua_State *L, int index, const char *k)
操作:   arr = Stack[index]    // arr肯定是表
        Stack.push( arr[k] )
取表中键为k的元素, 这里的表是由index指向的栈上的一个表
栈高度+1, 栈顶元素是(Stack[index])[k]
注意, 该操作将触发 __index 元方法
取得table.k并放到栈顶，如：lua_pushstring(L,"key");lua_gettable(L,-2);可以替换成lua_getfield(L,-1,"key")
________________________________________
给表中键为k的元素赋值
void lua_setfield (lua_State *L, int index, const char *k)
操作:   arr = Stack[index]
        arr[k] = Stack.top()
        Stack.pop()
给表中键为k的元素赋值value(value就是栈顶元素), 这里的表是由index指向的栈上的一个表
栈高度-1, 被弹出的是value
注意, 该操作将触发 __newindex 元方法
把栈顶的数据作为value放入table.k中，形如lua_pushstring(L, "key");lua_pushstring(L, value);lua_settable(L, -3);
可以改成lua_pushstring(L, value);lua_setfield(L,-2,"key");的形式。
________________________________________
取表元素到栈顶
void lua_gettable(lua_State *L, int index)
操作:     ele  = Stack[index]
            key = Stack.top()
            Stack.pop()
            value = ele[key]
            Stack.push(value)
根据index指定取到相应的表; 取栈顶元素为key, 并弹出栈; 获取表中key的值压入栈顶.
栈高度不变, 但是发生了一次弹出和压入的操作, 弹出的是key, 压入的是value
注意, 该操作将触发 __index 元方法
________________________________________
设置栈顶元素到表元素
void lua_settable (lua_State *L, int index)
操作:   ele    = Stack[index]
        value  = Stack.top()
        Stack.pop()
        key    = Stack.top()
        Stack.pop()
        ele[key] = value
根据index指定取到相应的表; 取栈顶元素做value, 弹出之; 再取当前栈顶元素做key, 亦弹出之; 然后将表的键为key的元素赋值为value
栈高度-2, 第一次弹出value, 第二次弹出key
注意, 该操作将触发 __newindex 元方法
________________________________________
 获取表元素到栈顶
void lua_rawget (lua_State *L, int index)
和lua_gettable操作一样
但是不触发相应的元方法
________________________________________
获取表中元素到栈顶
void lua_rawgeti(lua_State *L, int index, int n)
操作:   ele = Stack[index]
    value = ele[n]
    Stack.push(value)
    栈+1， 栈顶新增元素就是 value
    不触发相应的元方法
取得table[n]并放到栈顶.如：lua_pushnumber(L,i);lua_gettable(L,-2);可以用lua_rawgeti(L,-1,i)代替。
________________________________________
设置栈顶元素到表元素
void lua_rawset (lua_State *L, int index) 
和lua_settable操作一样
但是不触发相应的原方法
________________________________________
设置栈顶元素到表元素
void lua_rawseti (lua_State *L, int index, int n) 
操作:   ele = Stack[index]
    value = Stack.top()
    Stack.pop()
    ele[n] = value
栈-1, 栈顶将value弹出
不触发相应的元方法
________________________________________
复制栈上元素并压入栈</span></span>
void lua_pushvalue (lua_State *L, int index)
操作:   value = Stack[index]      
           Stack.push(value)
           栈+1
________________________________________
创建一个元表
int luaL_newmetatable (lua_State *L, const char *tname)
操作:  //在注册表中查找tname, 如果已经注册, 就返回0, 否者继续, 并平栈
        lua_getfield(L, LUA_REGISTRYINDEX, tname)
        if (!lua_isnil(L, -1))
            return 0;
        lua_pop(L, 1);
        // 创建一个表, 并注册, 返回1
        lua_newtable(L)
        lua_pushvalue(L, -1)
        lua_setfield(L, LUA_REGISTRYINDEX, tname)
        return 1
有返回值
栈+1, 栈顶元素是在注册表中注册过的新表
________________________________________
int lua_next (lua_State *L, int index);
从栈上弹出一个 key（键），然后把索引指定的表中 key-value（健值）对压入堆栈（指定 key 后面的下一 (next) 对）。如果表中以无更多元素，那么 lua_next 将返回 0 （什么也不压入堆栈）。 
典型的遍历方法是这样的： 
     /* table 放在索引 't' 处 */
     lua_pushnil(L);  /* 第一个 key */
     while (lua_next(L, t) != 0) {
       /* 用一下 'key' （在索引 -2 处） 和 'value' （在索引 -1 处） */
       printf("%s - %s/n",
              lua_typename(L, lua_type(L, -2)),
              lua_typename(L, lua_type(L, -1)));
       /* 移除 'value' ；保留 'key' 做下一次叠代 */
       lua_pop(L, 1);
     }
在遍历一张表的时候，不要直接对 key 调用 lua_tolstring ，除非你知道这个 key 一定是一个字符串。调用 lua_tolstring 有可能改变给定索引位置的值；这会对下一次调用 lua_next 造成影响。
________________________________________
int lua_getmetatable (lua_State *L, int index);
把给定索引指向的位置的元表压入堆栈。如果索引无效，或是这个位置没有元表，函数将返回 0 并且不会向栈上压任何东西。 
________________________________________
void lua_getfenv (lua_State *L, int index);
把索引处值的环境表压入堆栈。 
________________________________________
size_t lua_objlen (lua_State *L, int index);
返回指定的索引处的值的长度。对于 string ，那就是字符串的长度；对于 table ，是取长度操作符 ('#') 的结果；对于 userdata ，就是为其分配的内存块的尺寸；对于其它值，为 0 。 
________________________________________
lua_setfenv
int lua_setfenv (lua_State *L, int index);
从堆栈上弹出一个 table 并把它设为指定索引处值的新环境。如果指定索引处的值即不是函数又不是线程或是 userdata ， lua_setfenv 会返回 0 ，否则返回 1 。 
________________________________________
lua_setmetatable
int lua_setmetatable (lua_State *L, int index);
把一个 table 弹出堆栈，并将其设为给定索引处的值的 metatable 。 
________________________________________
lua_settable
void lua_settable (lua_State *L, int index);
作一个等价于 t[k] = v 的操作，这里 t 是一个给定有效索引 index 处的值， v 指栈顶的值，而 k 是栈顶之下的那个值。 
这个函数会把键和值都从堆栈中弹出。和在 Lua 中一样，这个函数可能触发 "newindex" 事件的元方法（参见 §2.8）。 
________________________________________
## (3)c访问 lua表实例
例子代码如下：
**[cpp]**[view plain](http://blog.csdn.net/chenjiayi_yun/article/details/18893789#)[copy](http://blog.csdn.net/chenjiayi_yun/article/details/18893789#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/455748/fork)
- int main()  
- {  
- //Lua示例代码，使用table
- char *szLua_code =  
- "x = {} --用于存放结果的table "
- "x[1],x[2] = string.gsub(c.Str, c.Mode, c.Tag) --x[1]里是结果，x[2]里是替换次数 "
- "x.u = string.upper(x[1])";  
- //Lua的字符串模式
- char *szMode = "(%w+)%s*=%s*(%w+)";  
- //要处理的字符串
- char *szStr = "key1 = value1 key2 = value2";  
- //目标字符串模式
- char *szTag = "<%1>%2</%1>";  
- 
-     lua_State *L = luaL_newstate();  
-     luaL_openlibs(L);  
- 
- //把一个tabele送给Lua
-     lua_newtable(L);    //新建一个table并压入栈顶
-     lua_pushstring(L, "Mode");// key
-     lua_pushstring(L, szMode);// value
- //由于上面两次压栈，现在table元素排在栈顶往下数第三的位置
- //lua_settable会自己弹出上面压入的key和value
-     lua_settable(L, -3);//设置newtable[Mode]=szMode
- 
-     lua_pushstring(L, "Tag");// key
-     lua_pushstring(L, szTag);// value
-     lua_settable(L, -3);    //设置newtable[Tag]=szTag
- 
-     lua_pushstring(L, "Str");// key
-     lua_pushstring(L, szStr);// value
-     lua_settable(L, -3);    //设置newtable[Str]=szStr
- 
-     lua_setglobal(L,"c"); //将栈顶元素（newtable）置为Lua中的全局变量c
- 
- //执行
- bool err = luaL_loadbuffer(L, szLua_code, strlen(szLua_code),  
- "demo") || lua_pcall(L, 0, 0, 0);  
- if(err)  
-     {  
- //如果错误，显示
-         cerr << lua_tostring(L, -1);  
- //弹出栈顶的这个错误信息
-         lua_pop(L, 1);  
-     }  
- else
-     {  
- //Lua执行后取得全局变量的值
-         lua_getglobal(L, "x");  
- 
- //如果x是个table，从表中取出数据
- if(lua_istable(L,-1))  
-         {  
- //取得x.u,即x["u"]
-             lua_pushstring(L,"u");    //压入key
- //由于这次压栈，x处于栈顶第二位置
-             lua_gettable(L,-2);//lua_gettable会弹出上面压入的key，然后把对应的value压入
-             cout << "x.u = " << lua_tostring(L,-1) << endl;//取得数据
-             lua_pop(L, 1);//从栈中弹出这个value
- 
- //取得x[1]和x[2]
- for(int i=1; i<=2; i++)  
-             {  
- //压入key
-                 lua_pushnumber(L,i);  
-                 lua_gettable(L,-2);  
-                 cout << "x[" << i <<"] = " << lua_tostring(L,-1) << endl;  
-                 lua_pop(L, 1);  
-             }  
-         }  
- 
- //弹出栈顶的x
-         lua_pop(L, 1);  
-     }  
-     lua_close(L);  
- return 0;  
- }  
# 8、lua函数调用
无保护的lua函数调用
void lua_call(lua_State* L, int nargs, int nresults)
调用它之前, 需要布局一下栈, 第一, 要把要call的函数压入栈; 第二, call要用的参数正序压入栈中; 然后才能调用lua_call, 调用完了, 自己去取返回值, 它都给你压栈上了.
栈操作:
      argn = Stack.pop()
      ... // 一共压入nargs个参数
      arg2 = Stack.pop()
      arg3 = Stack.pop()
       func = Stack.pop() // 函数本身也弹出
       res1， res2， ..., resj = func(arg1, arg2, ..., argn)
       Stack.push(res1)
       Stack.push(res2)
       … // 压入nresults个返回值
       Stack.push(resj)
调用结束后, 栈高度增加 nresults – (1 + nargs), 如果将nresults参数设置为LUA_MULTRET, 那么lua返回几个值, 栈上就压入几个值, 否者强制压入nresults个值, 不足的是空值, 多余的抛弃掉
注意, 这个函数是有危险的, 如果在其中发生了错误, 会直接退出程序（实际是被调用函数内发生的错误将（通过 longjmp）一直上抛。 ）
这个函数的用途: 尚未发现, 除非你能接受出错立马退出, lauxlib.h中的luaL_check*一族函数也是这样的, 不符合预期的话, 直接退出, 这些函数都要小心
代码：
要实现a = f("how", t.x, 14)
这里是 C 里的代码： 
     lua_getfield(L, LUA_GLOBALSINDEX, "f");          /* 将调用的函数 */
     lua_pushstring(L, "how");                          /* 第一个参数 */
     lua_getfield(L, LUA_GLOBALSINDEX, "t");          /* table 的索引 */
     lua_getfield(L, -1, "x");         /* 压入 t.x 的值（第 2 个参数）*/
     lua_remove(L, -2);                           /* 从堆栈中移去 't' */
     lua_pushinteger(L, 14);                           /* 第 3 个参数 */
     lua_call(L, 3, 1); /* 调用 'f'，传入 3 个参数，并索取 1 个返回值 */
     lua_setfield(L, LUA_GLOBALSINDEX, "a");      /* 设置全局变量 'a' */
注意上面这段代码是“平衡”的：到了最后，堆栈恢复成原来的。这是一种良好的编程习惯。 
________________________________________
保护下调用一个lua函数
int lua_pcall(lua_State* L, int nargs, int nresults, int errfunc)
参数, 行为和lua_call都一样, 如果在调用中没有发生任何错误, lua_pcall == lua_call; 但是如果有错误发生时, lua_pcall会捕获它
errfunc指出了Stack上的一个元素, 这个元素应该是一个函数, 当发生错误的时候
    ef = Stack[errfunc]
    value = ef(errmsg)
    Stack.push(value)
也就是说, 在错误的时候, errfunc指定的是栈上位置的错误处理函数，其将会被调用, 该处理函数的返回值被压到栈上（错误信息）.
默认情况下, 可以给errfunc传值0, 实际的效果是指定了这样一个函数做出错处理 function defaulterr(errmsg) return errmsg end.（错误信息和原始错误信息一致）
本函数有返回值 LUA_ERRRUN运行时错误  LUA_ERRMEM内存分配错误[注意, 这种错会导致lua调用不了错误处理函数]  LUA_ERRERR运行错误处理函数时出错了, 写程序的时候必须检查返回值
lua_pcall 函数在调用成功时返回 0 ，否则返回以下（定义在 lua.h 中的）错误代码中的一个： 
      LUA_ERRRUN: 运行时错误。 
      LUA_ERRMEM: 内存分配错误。对于这种错，Lua 调用不了错误处理函数。 
      LUA_ERRERR: 在运行错误处理函数时发生的错误。
推荐该函数 :典型的用法中，错误处理函数被用来在出错信息上加上更多的调试信息，比如栈跟踪信息 (stack traceback) 。这些信息在 lua_pcall 返回后，因为栈已经展开 (unwound) ，所以收集不到了。 
________________________________________
保护下调用一个c函数
int lua_cpcall (lua_State *L, lua_CFunction func, void *ud)
以保护模式调用c函数, func中可以且只能从堆栈上拿到一个参数, 就是ud, 当有错误时, 和lua_pcall返回相同的错误代码, 并在堆栈顶部留下errmsg字符串, 调用成功的话它返回零, 并且不会修改堆栈, 所有从func中返回的值都被扔掉.
这里注意的问题是:
1. "当有错误时", 这个错误的意思是lua的错误, 而不是c/c++的错误. 在func中使用lua_call和lua_check*族函数, 并不会导致程序退出了, 而是表现的像lua_pcall那样.
2. 调用成功的时候func中的返回值都被扔掉了.
# 9、类型
lua_CFunction
typedef int (*lua_CFunction) (lua_State *L);
C 函数的类型。 
为了正确的和 Lua 通讯，C 函数必须使用下列定义了参数以及返回值传递方法的协议：
C 函数通过 Lua 中的堆栈来接受参数，参数以正序入栈（第一个参数首先入栈）。
因此，当函数开始的时候， lua_gettop(L) 可以返回函数收到的参数个数。第一个参数（如果有的话）在索引 1 的地方，而最后一个参数在索引 lua_gettop(L) 处。当需要向 Lua 返回值的时候，C 函数只需要把它们以正序压到堆栈上（第一个返回值最先压入），然后返回这些返回值的个数。在这些返回值之下的，堆栈上的东西都会被 Lua 丢掉。和 Lua 函数一样，从 Lua 中调用 C 函数也可以有很多返回值。 
下面这个例子中的函数将接收若干数字参数，并返回它们的平均数与和： 
     static int foo (lua_State *L) {
       int n = lua_gettop(L);    /* 参数的个数 */
       lua_Number sum = 0;
       int i;
       for (i = 1; i <= n; i++) {
         if (!lua_isnumber(L, i)) {
           lua_pushstring(L, "incorrect argument");
           lua_error(L);
         }
         sum += lua_tonumber(L, i);
       }
       lua_pushnumber(L, sum/n);   /* 第一个返回值 */
       lua_pushnumber(L, sum);     /* 第二个返回值 */
       return 2;                   /* 返回值的个数 */
     }
________________________________________
lua_Number
typedef double lua_Number;
Lua 中数字的类型。确省是 double ，但是你可以在 luaconf.h 中修改它。 
通过修改配置文件你可以改变 Lua 让它操作其它数字类型（例如：float 或是 long ）。 
________________________________________
lua_Integer
typedef ptrdiff_t lua_Integer;
这个类型被用于 Lua API 接收整数值。 
缺省时这个被定义为 ptrdiff_t ，这个东西通常是机器能处理的最大整数类型。 
# 10、栈上值操作
void lua_concat (lua_State *L, int n);
连接栈顶的 n 个值，然后将这些值出栈，并把结果放在栈顶。如果 n 为 1 ，结果就是一个字符串放在栈上（即，函数什么都不做）；如果 n 为 0 ，结果是一个空串。 连接依照 Lua 中创建语义完成（参见 §2.5.4 ）。 
________________________________________
int lua_equal (lua_State *L, int index1, int index2);
如果依照 Lua 中 == 操作符语义，索引 index1 和 index2 中的值相同的话，返回 1 。否则返回 0 。如果任何一个索引无效也会返回 0。 
________________________________________
int lua_rawequal (lua_State *L, int index1, int index2);
如果两个索引 index1 和 index2 处的值简单地相等（不调用元方法）则返回 1 。否则返回 0 。如果任何一个索引无效也返回 0 。 
________________________________________
int lua_lessthan (lua_State *L, int index1, int index2);
如果索引 index1 处的值小于索引 index2 处的值时，返回 1 ；否则返回 0 。其语义遵循 Lua 中的 < 操作符（就是说，有可能调用元方法）。如果任何一个索引无效，也会返回 0 。 
________________________________________
lua_settop
void lua_settop (lua_State *L, int index);
参数允许传入任何可接受的索引以及 0 。它将把堆栈的栈顶设为这个索引。如果新的栈顶比原来的大，超出部分的新元素将被填为 nil 。如果 index 为 0 ，把栈上所有元素移除。 
# 11、多线程
lua_State
typedef struct lua_State lua_State;
一个不透明的结构，它保存了整个 Lua 解释器的状态。 Lua 库是完全可重入的：它没有任何全局变量。（译注：从 C 语法上来说，也不尽然。例如，在 table 的实现中用了一个静态全局变量 dummynode_ ，但这在正确使用时并不影响可重入性。只是万一你错误链接了 lua 库，不小心在同一进程空间中存在两份 lua 库实现的代码的话，多份 dummynode_ 不同的地址会导致一些问题。）所有的信息都保存在这个结构中。 
这个状态机的指针必须作为第一个参数传递给每一个库函数。 lua_newstate 是一个例外，这个函数会从头创建一个 Lua 状态机。 
________________________________________
lua_status
int lua_status (lua_State *L);
返回线程 L 的状态。 
正常的线程状态是 0 。当线程执行完毕或发生一个错误时，状态值是错误码。如果线程被挂起，状态为 LUA_YIELD 。 
________________________________________
void lua_xmove (lua_State *from, lua_State *to, int n);
传递 同一个 全局状态机下不同线程中的值。 
这个函数会从 from 的堆栈中弹出 n 个值，然后把它们压入 to 的堆栈中。 
________________________________________
int lua_resume (lua_State *L, int narg);
在给定线程中启动或继续一个 coroutine 。 
要启动一个 coroutine 的话，首先你要创建一个新线程（参见 lua_newthread ）；然后把主函数和若干参数压到新线程的堆栈上；最后调用 lua_resume ，把 narg 设为参数的个数。这次调用会在 coroutine 挂起时或是结束运行后返回。当函数返回时，堆栈中会有传给 lua_yield 的所有值，或是主函数的所有返回值。如果 coroutine 切换时，lua_resume
 返回 LUA_YIELD ，而当 coroutine 结束运行且没有任何错误时，返回 0 。如果有错则返回错误代码（参见 lua_pcall）。在发生错误的情况下，堆栈没有展开，因此你可以使用 debug API 来处理它。出错信息放在栈顶。要继续运行一个 coroutine 的话，你把需要传给 yield 作结果的返回值压入堆栈，然后调用 lua_resume 。 
________________________________________
int lua_yield  (lua_State *L, int nresults);
切出一个 coroutine 。 
这个函数只能在一个 C 函数的返回表达式中调用。如下： 
     return lua_yield (L, nresults);
当一个 C 函数这样调用 lua_yield ，正在运行中的 coroutine 将从运行中挂起，然后启动这个 coroutine 用的那次对 lua_resume 的调用就返回了。参数 nresults 指的是堆栈中需要返回的结果个数，这些返回值将被传递给 lua_resume 。 
# 12、调试接口
Lua 没有内建的调试设施。取而代之的是提供了一些函数接口和钩子。利用这些接口，可以做出一些不同类型的调试器，性能分析器，或是其它一些需要从解释器中取到“内部信息”的工具。 
________________________________________
lua_Debug
typedef struct lua_Debug {
  int event;
  const char *name;           /* (n) */
  const char *namewhat;       /* (n) */
  const char *what;           /* (S) */
  const char *source;         /* (S) */
  int currentline;            /* (l) */
  int nups;                   /* (u) upvalue 个数 */
  int linedefined;            /* (S) */
  int lastlinedefined;        /* (S) */
  char short_src[LUA_IDSIZE]; /* (S) */
  /* 私有部分 */
  其它域
} lua_Debug;
一个用来携带活动中函数的各种信息的结构。 lua_getstack 仅填写这个结构中的私有部分，这些部分以后会用到。调用 lua_getinfo 则可以填上 lua_Debug 中有用信息的那些域。 
lua_Debug 中的各个域有下列含义： 
 *       source: 如果函数是定义在一个字符串中，source 就是这个字符串。如果函数定义在一个文件中， source 是一个以 ['@'](mailto:%27@%27) 开头的文件名。 
 *       short_src: 一个“可打印版本”的 source，用于出错信息。 
 *       linedefined: 函数定义开始处的行号。 
 *       lastlinedefined: 函数定义结束处的行号。 
 *       what: 如果函数是一个 Lua 函数，则为一个字符串 "Lua" ；如果是一个 C 函数，则为 "C"；如果它是一个 chunk 的主体部分，则为 "main"；如果是一个作了尾调用的函数，则为 "tail" 。别的情况下，Lua 没有关于函数的别的信息。 
 *       currentline: 给定函数正在执行的那一行。当提供不了行号信息的时候，currentline 被设为 -1 。 
 *       name: 给定函数的一个合理的名字。因为 Lua 中的函数也是一个值，所以它们没有固定的名字：一些函数可能是全局复合变量的值，另一些可能仅仅只是被保存在一个 table 中。 lua_getinfo 函数会检查函数是这样被调用的，以此来找到一个适合的名字。如果它找不到名字，name 就被设置为 NULL 。 
  *      namewhat: 结实 name 域。 namewhat 的值可以是 "global", "local", "method", "field", "upvalue", 或是 "" （空串）。这取决于函数怎样被调用。（Lua 用空串表示其它选项都不符合） 
  *      nups: 函数的 upvalue 的个数。 
________________________________________
lua_gethook
lua_Hook lua_gethook (lua_State *L);
返回当前的钩子函数。 
________________________________________
lua_gethookcount
int lua_gethookcount (lua_State *L);
返回当前钩子记数。 
________________________________________
lua_gethookmask
int lua_gethookmask (lua_State *L);
返回当前的钩子掩码 (mask) 。 
________________________________________
lua_getinfo
int lua_getinfo (lua_State *L, const char *what, lua_Debug *ar);
返回一个指定的函数或函数调用的信息。 
当用于取得一次函数调用的信息时，参数 ar 必须是一个有效的活动的记录。这条记录可以是前一次调用 lua_getstack 得到的，或是一个钩子 （参见 lua_Hook）得到的参数。 
用于获取一个函数的信息时，可以把这个函数压入堆栈，然后把 what 字符串以字符 '>' 起头。（这个情况下，lua_getinfo 从栈顶上弹出函数。） 例如，想知道函数 f 在哪一行定义的，你可以下下列代码： 
     lua_Debug ar;
     lua_getfield(L, LUA_GLOBALSINDEX, "f");  /* 取到全局变量 'f' */
     lua_getinfo(L, ">S", &ar);
     printf("%d/n", ar.linedefined);
what 字符串中的每个字符都筛选出结构 ar 结构中一些域用于填充，或是把一个值压入堆栈： 
*        'n': 填充 name 及 namewhat 域； 
*        'S': 填充 source， short_src， linedefined， lastlinedefined，以及 what 域； 
*        'l': 填充 currentline 域； 
*        'u': 填充 nups 域； 
*        'f': 把正在运行中指定级别处函数压入堆栈；（译注：一般用于获取函数调用中的信息，级别是由 ar 中的私有部分来提供。如果用于获取静态函数，那么就直接把指定函数重新压回堆栈，但这样做通常无甚意义。） 
*        'L': 压一个 table 入栈，这个 table 中的整数索引用于描述函数中哪些行是有效行。（有效行指有实际代码的行，即你可以置入断点的行。无效行包括空行和只有注释的行。） 
这个函数出错会返回 0 （例如，what 中有一个无效选项）。 
________________________________________
lua_getlocal
const char *lua_getlocal (lua_State *L, lua_Debug *ar, int n);
从给定活动记录中获取一个局部变量的信息。参数 ar 必须是一个有效的活动的记录。这条记录可以是前一次调用 lua_getstack 得到的，或是一个钩子 （参见 lua_Hook）得到的参数。索引 n 用于选择要检阅哪个局部变量（ 1 表示第一个参数或是激活的第一个局部变量，以此类推，直到最后一个局部变量）。 lua_getlocal 把变量的值压入堆栈并返回它的名字。 
以 '(' （正小括号）开始的变量指内部变量（循环控制变量，临时变量，C 函数局部变量）。 
当索引大于局部变量的个数时，返回 NULL （什么也不压入）。 
________________________________________
lua_getstack
int lua_getstack (lua_State *L, int level, lua_Debug *ar);
获取解释器的运行时栈的信息。 
这个函数用正在运行中的给定级别处的函数的活动记录来填写 lua_Debug 结构的一部分。 0 级表示当前运行的函数，而 n+1 级处的函数就是调用第 n 级函数的那一个。如果没有错误，lua_getstack 返回 1 ；当调用传入的级别大于堆栈深度的时候，返回 0 。 
________________________________________
lua_getupvalue
const char *lua_getupvalue (lua_State *L, int funcindex, int n);
获取一个 closure 的 upvalue 信息。（对于 Lua 函数，upvalue 是函数需要使用的外部局部变量，因此这些变量被包含在 closure 中。） lua_getupvalue 获取第 n 个 upvalue ，把这个 upvalue 的值压入堆栈，并且返回它的名字。 funcindex 指向堆栈上 closure 的位置。（ 因为 upvalue 在整个函数中都有效，所以它们没有特别的次序。因此，它们以字母次序来编号。） 
当索引号比 upvalue 数量大的时候，返回 NULL （而且不会压入任何东西）对于 C 函数，这个函数用空串 "" 表示所有 upvalue 的名字。 
________________________________________
lua_Hook
typedef void (*lua_Hook) (lua_State *L, lua_Debug *ar);
用于调试的钩子函数类型。 
无论何时钩子被调用，它的参数 ar 中的 event 域都被设为触发钩子的事件。 Lua 把这些事件定义为以下常量： LUA_HOOKCALL， LUA_HOOKRET, LUA_HOOKTAILRET， LUA_HOOKLINE， and LUA_HOOKCOUNT。除此之外，对于 line 事件，currentline 域也被设置。要想获得 ar 中的其他域，钩子必须调用 lua_getinfo。对于返回事件，event 的正常值可能是 LUA_HOOKRET，或者是 LUA_HOOKTAILRET 。对于后一种情况，Lua
 会对一个函数做的尾调用也模拟出一个返回事件出来；对于这个模拟的返回事件，调用 lua_getinfo 没有什么作用。 
当 Lua 运行在一个钩子内部时，它将屏蔽掉其它对钩子的调用。也就是说，如果一个钩子函数内再调回 Lua 来执行一个函数或是一个 chunk ，这个执行操作不会触发任何的钩子。 
________________________________________
lua_sethook
int lua_sethook (lua_State *L, lua_Hook f, int mask, int count);
设置一个调试用钩子函数。 
参数 f 是钩子函数。 mask 指定在哪些事件时会调用：它由下列一组位常量构成 LUA_MASKCALL， LUA_MASKRET， LUA_MASKLINE，以及 LUA_MASKCOUNT。参数 count 只在 mask 中包含有 LUA_MASKCOUNT 才有意义。对于每个事件，钩子被调用的情况解释如下：
*        call hook: 在解释器调用一个函数时被调用。钩子将于 Lua 进入一个新函数后，函数获取参数前被调用。 
*        return hook: 在解释器从一个函数中返回时调用。钩子将于 Lua 离开函数之前的那一刻被调用。你无权访问被函数返回出去的那些值。 （译注：原文 (You have no access to the values to be returned by the function) 如此。但“无权访问”一词值得商榷。某些情况下你可以访问到一些被命名为 (*temporary) 的局部变量，那些索引被排在最后的 (*temporary) 变量指的就是返回值。但是由于 Lua 对特殊情况做了一些优化，比如直接返回一个被命名的局部变量，那么就找不到对应的
 (*temporary) 变量了。本质上，返回值一定存在于此刻的局部变量中，并且可以访问它，只是无法确定是哪些罢了。至于这个时候函数体内的其它局部变量，是不保证有效的。进入 return hook 的那一刻起，实际已经退出函数内部的运行环节，返回值占用的局部变量空间以后的部分，都有可能因 hook 本身复用它们而改变。） 
?        line hook: 在解释器准备开始执行新的一行代码时，或是跳转到这行代码中时（即使在同一行内跳转）被调用。（这个事件仅仅在 Lua 执行一个 Lua 函数时发生。） 
?        count hook: 在解释器每执行 count 条指令后被调用。（这个事件仅仅在 Lua 执行一个 Lua 函数时发生。） 
钩子可以通过设置 mask 为零屏蔽。 
________________________________________
lua_setlocal
const char *lua_setlocal (lua_State *L, lua_Debug *ar, int n);
设置给定活动记录中的局部变量的值。参数 ar 与 n 和 lua_getlocal 中的一样（参见 lua_getlocal）。 lua_setlocal 把栈顶的值赋给变量然后返回变量的名字。它会将值从栈顶弹出。 
当索引大于局部变量的个数时，返回 NULL （什么也不弹出）。
int lua_type (lua_State *L, int index);
返回给定索引处的值的类型，当索引无效时则返回 LUA_TNONE （那是指一个指向堆栈上的空位置的索引）。 lua_type 返回的类型是一些个在 lua.h 中定义的常量： LUA_TNIL ， LUA_TNUMBER ， LUA_TBOOLEAN ， LUA_TSTRING ， LUA_TTABLE ， LUA_TFUNCTION ， LUA_TUSERDATA ， LUA_TTHREAD
 ， LUA_TLIGHTUSERDATA 。 
________________________________________
lua_typename
const char *lua_typename  (lua_State *L, int tp);
返回 tp 表示的类型名，这个 tp 必须是 lua_type 可能返回的值中之一。 
________________________________________
