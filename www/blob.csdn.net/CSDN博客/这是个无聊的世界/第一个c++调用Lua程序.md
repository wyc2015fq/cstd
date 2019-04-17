# 第一个c++调用Lua程序 - 这是个无聊的世界 - CSDN博客





2016年05月11日 16:31:04[lancelot_vim](https://me.csdn.net/lancelot_vim)阅读数：1177








# 第一个c++调用Lua程序

**main.cpp**

```cpp
#include <lua.hpp>
#include <lualib.h>
#include <stdio.h>
#include <string.h>

int main(){
    char buf[256];
    int error;
    lua_State *L = luaL_newstate();
    luaL_openlibs(L);
    while(fgets(buf, sizeof(buf), stdin) != NULL ) {
        error = luaL_loadbuffer(L, buf, strlen(buf), "line");
        lua_pcall(L, 0, 0, 0);
        if(error) {
            size_t len;
            fprintf(stderr, "%s", lua_tolstring(L, -1, &len));
            lua_pop(L, 1);
        }
    }
    lua_close(L);
    return 0;
}
```

**.pro**

```
TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp

LIBS += /usr/lib/x86_64-linux-gnu/liblua5.1-c++.so.0
```

注意，你需要链接动态链接库liblua5.1-c++.so.0，windows下可自行查询需要链接的库

### 编译效果

```
~/workCode/cpp_torch/build $ make
g++ -c -m64 -pipe -O2 -Wall -W  -I/usr/share/qt4/mkspecs/linux-g++-64 -I../../cpp_torch -I../../cpp_torch -I. -o main.o ../main.cpp
g++ -m64 -Wl,-O1 -o cpp_torch main.o     /usr/lib/x86_64-linux-gnu/liblua5.1-c++.so.0
```

### 运行效果

```
~/workCode/cpp_torch/build $ ./cpp_torch
require 'torch'
require 'nn'

x = torch.randn(10)
print(x)
-0.5072
-0.6467
 1.0296
 0.8567
-1.7439
-1.1684
-1.0907
 0.4219
-1.0408
-1.7348
[torch.DoubleTensor of size 10]

net = nn.Sequential()
net:add(nn.Linear(10,20))
y = net:forward(x)
print(y)
 0.0463
 0.2011
 0.1265
 0.1126
 0.8329
-0.0219
 0.7075
-0.0519
 0.0226
 0.8394
 1.0159
-0.5954
-0.0089
-1.3776
 0.4461
-0.3691
 0.2173
-0.6644
-0.2885
 0.0942
[torch.DoubleTensor of size 20]
```

### 头文件以及API意义

lua.h定义了Lua提供得基础函数，包括创建Lua环境、调用Lua函数，读写Lua环境中得全局变量，以及注册供Lua调用的新函数等。 

lualib.h定义了各种辅助库提供的函数，它所有得定义都已LuaL_开头，比如LuaL_loadbuffer 

Lua库中没有定义任何全局变量，它所有的状态都保存在动态结构lua_State中，所有的c/c++API，都要求传入一个指向该结构的指针 

这个程序中，用LuaL_newstate新建了一个Lua运行环境，然后调用LuaL_loadbuffer来解释用户每行输入的内容，如果正确，返回0，并压栈，然后调用lua_pcall，这个函数会弹栈，并在保护模式中运行，成功也返回0 

如果发生错误，那么将错误消息压栈，用lua_tostring获取消息，然后调用lua_pop把它从栈中删除



