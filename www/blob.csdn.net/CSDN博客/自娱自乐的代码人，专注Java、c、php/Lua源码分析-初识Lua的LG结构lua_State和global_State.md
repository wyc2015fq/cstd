# Lua源码分析 - 初识Lua的LG结构lua_State和global_State - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2019年03月11日 15:06:51[initphp](https://me.csdn.net/initphp)阅读数：75








Lua在执行main函数第2行代码，会调用luaL_newstate方法，luaL_newstate主要用来为每一个LUA线程创建独立的函数栈和线程栈，以及线程执行过程中需要用到的内存管理、字符串管理、gc等信息。

我们看main函数：

```
int main (int argc, char **argv) {
  int status, result;
  /* 第一步：创建一个主线程栈数据结构 */
  lua_State *L = luaL_newstate();  /* create state */
  if (L == NULL) {
    l_message(argv[0], "cannot create state: not enough memory");
    return EXIT_FAILURE;
  }
  lua_pushcfunction(L, &pmain);  /* to call 'pmain' in protected mode */
  lua_pushinteger(L, argc);  /* 1st argument */
  lua_pushlightuserdata(L, argv); /* 2nd argument */
  status = lua_pcall(L, 2, 1, 0);  /* do the call */
  result = lua_toboolean(L, -1);  /* get result */
  report(L, status);
  lua_close(L);
  return (result && status == LUA_OK) ? EXIT_SUCCESS : EXIT_FAILURE;
}
```

lua_newstate函数主要用于创建一个lua_State和global_State的数据结构

lua_State：主线程栈结构

global_State：全局状态机，维护全局字符串表、内存管理函数、gc等信息

lua_State和global_State结构，通过LG的方式进行链接，将全局状态机global_State挂载到lua_State数据结构上

```
/**
 * 分配lua_State和global_State
 * 通过LG结构方式，每个线程会独立维护自己的线程栈和函数栈
 * 对外通过lua_State结构暴露给用户，而global_State挂载在lua_State结构上
 * 主要管理管理全局数据，全局字符串表、内存管理函数、 GC 把所有对象串联起来的信息、内存等
 * global_State：全局状态机
 * lua_State：主线程栈结构
 */
LUA_API lua_State *lua_newstate (lua_Alloc f, void *ud) {
  int i;
  lua_State *L;
  global_State *g;
  /* 分配一块lua_State结构的内容块 */
  LG *l = cast(LG *, (*f)(ud, NULL, LUA_TTHREAD, sizeof(LG)));
  if (l == NULL) return NULL;
  L = &l->l.l;
  g = &l->g;
  L->next = NULL;
  L->tt = LUA_TTHREAD;
  g->currentwhite = bitmask(WHITE0BIT);
  L->marked = luaC_white(g);
  /* 初始化一个线程的栈结构数据 */
  preinit_thread(L, g);
  g->frealloc = f;
  g->ud = ud;
  g->mainthread = L;
  g->seed = makeseed(L);
  g->gcrunning = 0;  /* no GC while building state */
  g->GCestimate = 0;
  g->strt.size = g->strt.nuse = 0;
  g->strt.hash = NULL;
  setnilvalue(&g->l_registry);
  g->panic = NULL;
  g->version = NULL;
  g->gcstate = GCSpause;
  g->gckind = KGC_NORMAL;
  g->allgc = g->finobj = g->tobefnz = g->fixedgc = NULL;
  g->sweepgc = NULL;
  g->gray = g->grayagain = NULL;
  g->weak = g->ephemeron = g->allweak = NULL;
  g->twups = NULL;
  g->totalbytes = sizeof(LG);
  g->GCdebt = 0;
  g->gcfinnum = 0;
  g->gcpause = LUAI_GCPAUSE;
  g->gcstepmul = LUAI_GCMUL;
  for (i=0; i < LUA_NUMTAGS; i++) g->mt[i] = NULL;
  if (luaD_rawrunprotected(L, f_luaopen, NULL) != LUA_OK) {
    /* memory allocation error: free partial state */
    close_state(L);
    L = NULL;
  }
  return L;
}
```

preinit_thread函数主要用户初始化线程栈结构。

```
/*
 * 初始化线程结构栈数据
** preinitialize a thread with consistent values without allocating
** any memory (to avoid errors)
*/
static void preinit_thread (lua_State *L, global_State *g) {
  G(L) = g; /* 将global_State结构挂载到lua_State结构上 */
  L->stack = NULL;
  L->ci = NULL;
  L->nci = 0;
  L->stacksize = 0;
  L->twups = L;  /* thread has no upvalues */
  L->errorJmp = NULL;
  L->nCcalls = 0;
  L->hook = NULL;
  L->hookmask = 0;
  L->basehookcount = 0;
  L->allowhook = 1;
  resethookcount(L);
  L->openupval = NULL;
  L->nny = 1;
  L->status = LUA_OK;
  L->errfunc = 0;
}
```

lua_State主线程栈结构

```
/*
** 'per thread' state
** Lua 主线程 栈 数据结构
** 作用：管理整个栈和当前函数使用的栈的情况，最主要的功能就是函数调用以及和c的通信
*/
struct lua_State {
  CommonHeader;
  lu_byte status; /* 解析容器的用于记录中间状态*/

  /* 全局状态机 */
  global_State *l_G;

  /* 函数 - 调用栈信息管理（CallInfo 为双向链表结构） */
  unsigned short nci;  /* number of items in 'ci' list - 存储一共多少个CallInfo */
  CallInfo base_ci;  /* CallInfo for first level (C calling Lua) - 调用栈的头部指针 */
  CallInfo *ci;  /* call info for current function - 当前运行函数信息 */

  /* 栈指针地址管理  StkId = TValue 的数组 */
  StkId top;  /* first free slot in the stack - 线程栈的栈顶指针 */
  StkId stack_last;  /* last free slot in the stack  - 线程栈的最后一个位置 */
  StkId stack;  /* stack base - 栈的指针，当前执行的位置*/

  const Instruction *oldpc;  /* last pc traced 在当前thread 的解释执行指令的过程中，指向最后一次执行的指令的指针 */
  UpVal *openupval;  /* list of open upvalues in this stack */
  GCObject *gclist; /* GC列表 */
  struct lua_State *twups;  /* list of threads with open upvalues */
  struct lua_longjmp *errorJmp;  /* current error recover point */

  /* Hook 相关管理 - 服务于debug模块 */
  volatile lua_Hook hook;
  ptrdiff_t errfunc;  /* current error handling function (stack index) */
  int stacksize;
  int basehookcount;
  int hookcount;
  l_signalT hookmask;
  lu_byte allowhook;

  /* 跟C语言通信 管理*/
  unsigned short nCcalls;  /* number of nested C calls */
  unsigned short nny;  /* number of non-yieldable calls in stack */
};
```

global_State全局状态机

```
/*
** 'global state', shared by all threads of this state
** lua 全局状态机
** 作用：管理全局数据，全局字符串表、内存管理函数、 GC 把所有对象串联起来的信息、内存等
*/
typedef struct global_State {

  /* 版本号  */
  const lua_Number *version;  /* pointer to version number */

  /* 内存管理 */
  lua_Alloc frealloc;  /* Lua的全局内存分配器，用户可以替换成自己的 - function to reallocate memory */
  void *ud;         /* 分配器的userdata - auxiliary data to 'frealloc' */

  /* 线程管理 */
  struct lua_State *mainthread; /* 主线程 */
  struct lua_State *twups;  /* 闭包了当前线程变量的其他线程列表 - list of threads with open upvalues */

  /* 字符串管理 */
  stringtable strt;  /* 字符串table Lua的字符串分短字符串和长字符串 - hash table for strings */
  TString *strcache[STRCACHE_N][STRCACHE_M];  /* 字符串缓存 - cache for strings in API */

  /* 虚函数表 */
  TString *tmname[TM_N];  /* 预定义方法名字数组 -  array with tag-method names */
  struct Table *mt[LUA_NUMTAGS];  /* 每个基本类型一个metatable（整个Lua最重要的Hook机制） - metatables for basic types */

  /* 错误处理 */
  lua_CFunction panic;  /* to be called in unprotected errors */
  TString *memerrmsg;  /* memory-error message */

  /* GC管理 */
  unsigned int gcfinnum;  /* number of finalizers to call in each GC step */
  int gcpause;  /* size of pause between successive GCs */
  int gcstepmul;  /* GC 'granularity' */

  l_mem totalbytes;  /* number of bytes currently allocated - GCdebt */
  l_mem GCdebt;  /* bytes allocated not yet compensated by the collector */
  lu_mem GCmemtrav;  /* memory traversed by the GC */
  lu_mem GCestimate;  /* an estimate of the non-garbage memory in use */

  TValue l_registry;
  unsigned int seed;  /* randomized seed for hashes */
  lu_byte currentwhite;
  lu_byte gcstate;  /* state of garbage collector */
  lu_byte gckind;  /* kind of GC running */
  lu_byte gcrunning;  /* true if GC is running */
  GCObject *allgc;  /* list of all collectable objects */
  GCObject **sweepgc;  /* current position of sweep in list */
  GCObject *finobj;  /* list of collectable objects with finalizers */
  GCObject *gray;  /* list of gray objects */
  GCObject *grayagain;  /* list of objects to be traversed atomically */
  GCObject *weak;  /* list of tables with weak values */
  GCObject *ephemeron;  /* list of ephemeron tables (weak keys) */
  GCObject *allweak;  /* list of all-weak tables */
  GCObject *tobefnz;  /* list of userdata to be GC */
  GCObject *fixedgc;  /* list of objects not to be collected */

} global_State;
```







