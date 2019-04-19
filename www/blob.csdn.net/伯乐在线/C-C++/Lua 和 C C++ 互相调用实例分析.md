# Lua 和 C/C++ 互相调用实例分析 - 文章 - 伯乐在线
原文出处： [lxyfirst](http://www.cppblog.com/lxyfirst/archive/2008/10/29/65447.aspx)
lua作为小巧精悍的脚本语言，易于嵌入c/c++中 ， 广泛应用于游戏AI ，实际上在任何经常变化的逻辑上都可以使用lua实现，配合c/c++实现的底层接口服务，能够大大降低系统的维护成本。下面对lua和c/c++的交互调用做一个实例分析：
lua提供了API用于在c/c++中构造lua的运行环境，相关接口如下：

C++
```
//创建lua运行上下文
lua_State* 
luaL_newstate(void) ;
//加载lua脚本文件
int luaL_loadfile(lua_State *L, const 
char *filename);
```
lua和c/c++的数据交互通过”栈”进行
,操作数据时，首先将数据拷贝到”栈”上，然后获取数据，栈中的每个数据通过索引值进行定位，索引值为正时表示相对于栈底的偏移索引，索引值为负时表示相对于栈顶的偏移索引，索引值以1或-1为起始值，因此栈顶索引值永远为-1
,栈底索引值永远为1 。 “栈”相当于数据在lua和c/c++之间的中转地。每种数据都有相应的存取接口 。
数据入”栈”接口：

C++
```
void  (lua_pushnil) (lua_State *L);
void  (lua_pushnumber) (lua_State *L, lua_Number n);
void  (lua_pushinteger) (lua_State *L, lua_Integer n);
void  (lua_pushlstring) (lua_State *L, const char *s, size_t l);
void  (lua_pushstring) (lua_State *L, const char *s);
void  (lua_pushboolean) (lua_State *L, int b);
void  (lua_pushcclosure) (lua_State *L, lua_CFunction fn, int n);
```
数据获取接口：

C++
```
lua_Number (lua_tonumber) (lua_State *L, int idx);
lua_Integer (lua_tointeger) (lua_State *L, int idx);
int  (lua_toboolean) (lua_State *L, int idx);
const  char  *(lua_tolstring) (lua_State *L, int idx, size_t *len);
lua_CFunction (lua_tocfunction) (lua_State *L, int idx);
```
“栈”操作接口：

C++
```
int (lua_gettop) (lua_State *L);
void (lua_settop) (lua_State *L, int idx);
void (lua_pushvalue) (lua_State *L, int idx);
void (lua_remove) (lua_State *L, int idx);
void (lua_insert) (lua_State *L, int idx);
void (lua_replace) (lua_State *L, int idx);
int (lua_checkstack) (lua_State *L, int sz);
```
lua中定义的变量和函数存放在一个全局table中，索引值为LUA_GLOBALSINDEX
，table相关操作接口：

C++
```
void (lua_gettable) (lua_State *L, int idx);
void (lua_getfield) (lua_State *L, int idx, const char *k);
void (lua_settable) (lua_State *L, int idx);
void (lua_setfield) (lua_State *L, int idx, const char *k);
```
当”栈”中包含执行脚本需要的所有要素(函数名和参数)后，调用lua_pcall执行脚本：

C++
```
int (lua_pcall) (lua_State *L, int nargs, int nresults, int errfunc);
```
下面进行实例说明：
func.lua

C++
```
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
//lua头文件
#include <lua.h>
#include <lualib.h>
#include <lauxlib.h>
#define err_exit(num,fmt,args)  \
    do{printf("[%s:%d]"fmt"\n",__FILE__,__LINE__,##args);exit(num);} while(0)
#define err_return(num,fmt,args)  \
    do{printf("[%s:%d]"fmt"\n",__FILE__,__LINE__,##args);return(num);} while(0)
//lua中调用的c函数定义,实现加法
int csum(lua_State* l)
{
    int a = lua_tointeger(l,1) ;
    int b = lua_tointeger(l,2) ;
    lua_pushinteger(l,a+b) ;
    return 1 ;
}
int main(int argc,char** argv)
{
    lua_State * l = luaL_newstate() ;        //创建lua运行环境
    if ( l == NULL ) err_return(-1,"luaL_newstat() failed"); 
    int ret = 0 ;
    ret = luaL_loadfile(l,"func.lua") ;      //加载lua脚本文件
    if ( ret != 0 ) err_return(-1,"luaL_loadfile failed") ;
    ret = lua_pcall(l,0,0,0) ;
    if ( ret != 0 ) err_return(-1,"lua_pcall failed:%s",lua_tostring(l,-1)) ;
    lua_getglobal(l,"width");              //获取lua中定义的变量
    lua_getglobal(l,"height");
    printf("height:%ld width:%ld\n",lua_tointeger(l,-1),lua_tointeger(l,-2)) ;
    lua_pop(l,1) ;                        //恢复lua的栈
    int a = 11 ;
    int b = 12 ;
    lua_getglobal(l,"sum");               //调用lua中的函数sum
    lua_pushinteger(l,a) ;
    lua_pushinteger(l,b) ;
    ret = lua_pcall(l,2,1,0) ;
    if ( ret != 0 ) err_return(-1,"lua_pcall failed:%s",lua_tostring(l,-1)) ;
    printf("sum:%d + %d = %ld\n",a,b,lua_tointeger(l,-1)) ;
    lua_pop(l,1) ;
    const char str1[] = "hello" ;
    const char str2[] = "world" ;
    lua_getglobal(l,"mystrcat");          //调用lua中的函数mystrcat
    lua_pushstring(l,str1) ;
    lua_pushstring(l,str2) ;
    ret = lua_pcall(l,2,1,0) ;
    if ( ret != 0 ) err_return(-1,"lua_pcall failed:%s",lua_tostring(l,-1)) ;
    printf("mystrcat:%s%s = %s\n",str1,str2,lua_tostring(l,-1)) ;
    lua_pop(l,1) ;
    lua_pushcfunction(l,csum) ;         //注册在lua中使用的c函数
    lua_setglobal(l,"csum") ;           //绑定到lua中的名字csum
    lua_getglobal(l,"mysum");           //调用lua中的mysum函数，该函数调用本程序中定义的csum函数实现加法
    lua_pushinteger(l,a) ;
    lua_pushinteger(l,b) ;
    ret = lua_pcall(l,2,1,0) ;
    if ( ret != 0 ) err_return(-1,"lua_pcall failed:%s",lua_tostring(l,-1)) ;
    printf("mysum:%d + %d = %ld\n",a,b,lua_tointeger(l,-1)) ;
    lua_pop(l,1) ;
    lua_close(l) ;                     //释放lua运行环境
    return 0 ;
}
```
