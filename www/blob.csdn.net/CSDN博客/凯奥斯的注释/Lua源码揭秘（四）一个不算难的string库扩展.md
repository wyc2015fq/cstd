# Lua源码揭秘（四）一个不算难的string库扩展 - 凯奥斯的注释 - CSDN博客





2017年11月06日 18:48:12[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：335
所属专栏：[Lua源码揭秘](https://blog.csdn.net/column/details/17856.html)









前两篇[Lua源码揭秘（二）一个简单的数学扩展库函数](http://blog.csdn.net/ecidevilin/article/details/78377365)[Lua源码揭秘（三）一个比较简单的table库扩展](http://blog.csdn.net/ecidevilin/article/details/78397419)分别对math库和table库进行了扩展，本篇就继续扩展一下string库。 

我们直接打开lstrlib.c（再次强调：在实际工作中的标准做法是在宿主语言中实现）。  

添加以下代码：
```
static int str_split(lua_State *L)
{
    size_t l1 = 0;
    size_t l2 = 0;
    const char *str = luaL_checklstring(L, 1, &l1);
    const char *sep = luaL_checklstring(L, 2, &l2);
    if (strlen(sep) == 0)
    {
        return 0;
    }
    lua_newtable(L);
    const char *s2 = NULL;
    int pos = 0;
    int idx = 0;
    while (1)
    {
        s2 = lmemfind(str + pos, l1 - pos, sep, l2);
        if (s2)
        {
            if (s2 - str - pos)
            {
                lua_pushlstring(L, str + pos, s2 - str - pos);
                lua_rawseti(L, -2, ++idx);
            }
            pos = s2 - str + l2 + 1;
            if (pos >= l1)
            {
                break;
            }
        }
        else
        {
            if (l1 - pos)
            {
                lua_pushlstring(L, str + pos, l1 - pos);
                lua_rawseti(L, -2, ++idx);
            }
            break;
        }
    }
    return 1;
}
```

然后按照惯例在strlib数组里的{NULL, NULL}之前添加：
`{"splitby", str_split},`
然后编译后，lua里便多了一个方法string.splitby。 

当然这也是一个很常见的函数。 

lstrlib.c里的方法也不算太多，相对于其他脚本语言的字符串库，可以说相当寒酸了。 

具体用法，在[Lua语法小贴士（二）string库](http://blog.csdn.net/ecidevilin/article/details/52786404)及其相关文章中已经提到过，这里就简单说几个点。 

str_len中用到了luaL_checklstring（lauxlib.c）除了检查对应位置的值是否是string类型外，还可以通过传入一个size_t的指针给第三个参数来获取字符串的长度。 

str_sub用luaL_checkinteger（lauxlib.c）检查第二个参数，luaL_optinteger（lauxlib.c）来获取第三个参数（如果没有则使用默认值）。然后根据首尾值，将子串的开始处指针和长度用lua_pushlstring（lapi.c）加入函数栈中（在地址上已经脱离了跟原字符串的关系）。如果首尾参数值不对，则将空串入栈。 

str_reverse，str_lower，str_upper和都很简单，值得一提的是这里用到了luaL_Buffer（上篇已经提过[http://blog.csdn.net/ecidevilin/article/details/78397419](http://blog.csdn.net/ecidevilin/article/details/78397419)）。 

str_byte中用到了luaL_checkstack（lauxlib.c），用来检查和扩展堆栈。 

str_dump是个神奇的方法，这里用到了lua_dump（lapi.c）这个方法，涉及到lua的字节码编码，这里我们我们先挖个坑，用空再聊。这里用到了luaL_checktype（lauxlib.c）用来检查类型。 

捕获相关的函数（match、find、gsub、gmatch）用到的算法有兴趣可以看一下，核心就是match这个函数，这里不做讲解。 

其中gmatch实际上一个[迭代器生成器](http://blog.csdn.net/ecidevilin/article/details/52944974)，用lua_pushcclosure（lapi.c）返回了一个c闭包作为迭代函数。 

最后，在打开string库之后，调用了createmetatable，将为string类型添加[元表](http://blog.csdn.net/ecidevilin/article/details/52858422)，这样就可以索引到上述所有库函数。其中lua_setmetatable和lua里的setmetatable类似，不过lua里的setmetatable在此之前进行了类型判断（只有table才能被设置元表，并且如果有__metatable字段，也是不能添加元表），而lua_setmetatable里除了table和userdata之外，为其他类型设置元表实际上都是为该类型整体设置元表。




