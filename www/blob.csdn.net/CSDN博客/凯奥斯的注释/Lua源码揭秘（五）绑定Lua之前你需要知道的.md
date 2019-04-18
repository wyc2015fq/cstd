# Lua源码揭秘（五）绑定Lua之前你需要知道的 - 凯奥斯的注释 - CSDN博客





2017年12月02日 13:21:48[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：1561
所属专栏：[Lua源码揭秘](https://blog.csdn.net/column/details/17856.html)









lapi.c和lauxlib.c里包含了大部分的lua的capi。所谓capi是指在c形式的api接口，可以在c/c++/c#/…中可以调用到的api，但是lua中并不一定有对应的方法。ldebug.c里有一些调试用的capi（后续文章会介绍）。另外一些capi，便是打开lua库函数的capi，在lualib.h里集中声明。

# **lapi.c**

基础的capi。

## *state相关*

### lua_checkstatck

检查栈的深度，如果没有超过最大深度便会扩充栈，否则返回0（交给上层处理）。

### lua_xmove

将n个对象从一个状态（协程）移动到另一个状态（协程）上。

### lua_setlevel

设置状态（协程）的调用层次，用于resume。

### lua_atpanic

设置状态（协程）的内部错误处理方法。

### lua_newthread

新建一个状态（协程）。

## *stack相关*

### lua_gettop

获取栈顶，一般用于检查参数个数。

### lua_settop

设置栈顶，一般用于设置返回值个数。之前的文章在编写库函数的时候，会返回一个返回值数量，这个数量要与当前函数栈的深度一致，否则会出问题。

### lua_remove

在函数栈的某个位置上移除对象。

### lua_insert

在函数栈的某个位置上插入对象。

### lua_replace

在函数栈的某个位置上替换成当前栈顶的对象。

### lua_pushvalue

最常用的，在栈顶上压入对象，与之对应的是lua_pop，实际上就是lua_settop（宏定义）。

## *type相关*

### lua_type

类型：0-nil，1-bool，2-lightuserdata，3-number，4-string，5-table，6-function，7-userdata，8-thread。-1代表啥也没有（不是nil）。

### lua_typename

类型名，lua里的type方法实际上调用了上面二者。

### lua_iscfunction

是否是c方法。

### lua_isnumber

是否是number，或者可以转换成number。

### lua_isstring

是否是string或者number。

### lua_isuserdata

是否是userdata或lightuserdata。

### lua_rawequal

绕过元方法判断是否相等。

### lua_equal

（如果有，便使用元方法）判断是否相等。

### lua_lessthan

（如果有，便使用元方法）判断是否小于。

### lua_tonumber

转换成number（失败返回0）。

### lua_tointeger

转换成number并向下取整（失败返回0），64位下返回值类型为int64，只有52-53位精度（有机会再细说）。32位下返回值为int，没有损失精度。

### lua_toboolean

如果是false或nil或0返回false，否则返回true。

### lua_tolstring

转换成字符串并获取长度。

### lua_objlen

获取对象的长度，暂时可以认为与#操作符类似。

### lua_tocfunction

转换成c方法，失败返nil。

### lua_touserdata

转换成userdata或lightuserdata，失败返nil。

### lua_tothread

转换成状态（协程），失败返nil。

### lua_topointer

转换成指针。

## *push相关*

### lua_pushnil

压入nil。

### lua_pushnumber

压入number。

### lua_pushinteger

压入向下取整的number。

### lua_pushlstring

压入string，附带长度信息。

### lua_pushstring

压入string，长度信息自己算。

### lua_pushvfstring

压入格式化变参的string。

### lua_pushfstring

压入格式化变参的string，与上面的并没什么区别，只不过上面的是为了兼用上层调用的变参（详情请搜索va_list）。

### lua_pushcclosure

压入c方法，并根据栈上的参数创建闭包。

### lua_pushboolean

压入bool。

### lua_pushlightuserdata

压入lightuserdata。

### lua_pushthread

压入状态（协程）。

## *获取Lua->stack*

### lua_gettable

获取栈上的table的某个字段，键为栈顶对象。

### lua_getfield

获取栈上的table的某个字段。

### lua_rawget

绕过元表获取table的元素，键为栈顶的对象。

### lua_rawgeti

绕过元表获取数组类型table的元素。

### lua_createtable

新建一个table

### lua_getmetatable

获取元表，如果不是table或userdata，那么获取该类型的元表。

### lua_getfenv

获取环境变量table。

## *设置stack->Lua*

### lua_settable

设置栈上的table的某个字段，键和值为栈顶前两个对象。

### lua_setfield

设置栈上的table的某个字段，值为栈顶对象。

### lua_rawset

绕过元表设置table的元素，键和值为栈顶前两个对象。

### lua_rawseti

绕过元表设置table的元素，值为栈顶对象。

### lua_setmetatable

设置元表，如果不是table或userdata，那么为该类型的设置元表。

### lua_setfenv

设置环境变量table。

## *load & call*

### lua_call

调用方法。

### lua_pcall

类似lua的方法xpcall，保护调用。

### lua_cpcall

保护调用c方法，可传入一个指针作为参数。

### lua_load

加载lua代码。

### lua_dump

加载lua字节码。

### lua_status

返回状态（协程）的状态。

## *其他*

### lua_gc

垃圾回收。

### lua_error

报错

### lua_next

获取table的下一对键值。从nil开始。

### lua_concat

拼接字符串。

### lua_getallocf

获取内存分配函数。

### lua_setallocf

设置内存分配函数。

### lua_newuserdata

新建userdata。

### lua_getupvalue

获取upvalue。

### lua_setupvalue

设置upvalue。

# **lauxlib.c**

扩展的capi

## *error相关*

### luaL_argerror

参数错误，将一些字符串的拼接起来组成错误信息，最后调用luaL_error。

### luaL_typerror

类型错误，同上。

### luaL_where

获取并拼接方法的信息。

### luaL_error

缺省错误，提供格式化变参。

## *type相关*

### luaL_checkoption

检查参数列表，如失败便报错。

### luaL_newmetatable

为全局对象新建元表。

### luaL_checkudata

检查全局对象是否与传入的userdata相同，如失败便报错。

### luaL_checkstack

检查栈空间，如失败便报错。

### luaL_checktype

检查类型，如失败便报错。

### luaL_checkany

检查是否有参数，如失败便报错。

### luaL_checklstring

检查string，如失败便报错。

### luaL_optlstring

检查string，如失败使用缺省值。

### luaL_checknumber

检查number，如失败便报错。

### luaL_optnumber

检查number，如失败使用缺省值。

### luaL_checkinteger

检查“integer”，如失败便报错。

### luaL_optinteger

检查“integer”，如失败使用缺省值。

## *meta相关*

### luaL_getmetafield

获取元表字段。

### luaL_callmeta

调用元方法。

## *lib相关*

### luaL_register

注册库函数。

### luaL_gsub

替换字符串。

### luaL_findtable

根据路径查询table，路径形式可能为aa.bb.cc，一次查找aa、bb、cc，如果没有则新建。

## *buffer相关*

### luaL_prepbuffer

准备缓冲区。

### luaL_addlstring

将string加入缓冲区，附带长度信息。

### luaL_addstring

将string加入缓冲区，长度自己算。

### luaL_pushresult

将buffer里的字符串拼接起来，压入栈中。

### luaL_addvalue

将栈顶对象加入缓冲区。

### luaL_buffinit

初始化缓冲区。

## *ref相关*

### luaL_ref

添加对某个对象的引用，方便获取某个对象。

### luaL_unref

取消对某个对象的引用。

## *load相关*

### luaL_loadfile

加载文件，并解析。

### luaL_loadbuffer

加载源码/字节码字符串，并解析。

### luaL_loadstring

同上。

## *state相关*

### luaL_newstate

新建一个状态（协程）。

# 参考文献

[https://www.codingnow.com/2000/download/lua_manual.html](https://www.codingnow.com/2000/download/lua_manual.html)



