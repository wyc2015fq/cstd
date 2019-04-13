
# lua的VS或者VC环境的搭建调试 - 高科的专栏 - CSDN博客

2012年11月16日 18:37:58[高科](https://me.csdn.net/pbymw8iwm)阅读数：8145标签：[lua																](https://so.csdn.net/so/search/s.do?q=lua&t=blog)[Lua																](https://so.csdn.net/so/search/s.do?q=Lua&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=lua&t=blog)个人分类：[开发环境的配置																](https://blog.csdn.net/pbymw8iwm/article/category/1219848)[lua																](https://blog.csdn.net/pbymw8iwm/article/category/1219039)[
							](https://blog.csdn.net/pbymw8iwm/article/category/1219848)


安装完LuaForWindows_v5.1.4
打开vs
tools->options->projects->directories
executable files 选项添加lua安装以后的路径,我的是 C:\Program Files\Lua\5.1
include files选项添加lua include路径,我的是 C:\Program Files\Lua\5.1include
library files 选项添加lua lib路径,我的是 C:\Program Files\Lua\5.1\lib
然后
加入lua静态库链接
Project->setings->link->input
Additional Dependencies 选项加入 lua5.1.lib lua51.lib
其他一些问题及解决办法
1.编译报错一
Linking...
test.obj : error LNK2001: unresolved external symbol _lua_close
test.obj : error LNK2001: unresolved external symbol _lua_pcall
test.obj : error LNK2001: unresolved external symbol _luaL_loadstring
test.obj : error LNK2001: unresolved external symbol _luaopen_io
test.obj : error LNK2001: unresolved external symbol _luaopen_base
test.obj : error LNK2001: unresolved external symbol _luaL_newstate
Debug/test.exe : fatal error LNK1120: 6 unresolved externals
原因
加入
\#pragma comment(lib, "lua5.1.lib")
如果你是CPP文件，那么我建议你最好使用
extern "C" {
\#include <lua.h>
\#include <lauxlib.h>
\#include <lualib.h>
}
写段code看看吧：
\#include "lua.h"
\#include "lualib.h"
\#include "lauxlib.h"
\#pragma comment(lib, "lua5.1.lib")
const char *buf = "print('hello, world!')";
int main(int argc, char* argv[]) {
lua_State *L = lua_open();     //创建一个指向lua解释器的指针
luaopen_base(L);  //
luaL_dostring(L,buf);
lua_close(L);
return 0;
}


