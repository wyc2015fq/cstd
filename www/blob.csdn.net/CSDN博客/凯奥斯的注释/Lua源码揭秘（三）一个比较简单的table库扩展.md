# Lua源码揭秘（三）一个比较简单的table库扩展 - 凯奥斯的注释 - CSDN博客





2017年10月30日 21:12:39[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：831
所属专栏：[Lua源码揭秘](https://blog.csdn.net/column/details/17856.html)









有了上一篇 [Lua源码揭秘（二）一个简单的数学扩展库函数](http://blog.csdn.net/ecidevilin/article/details/78377365)的经验之后，我们尝试写稍微复杂一点的库函数。 

我们直接打开ltablib.c（再次强调：在实际工作中的标准做法是在宿主语言中实现）。  

添加以下代码：
```
static in values(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    lua_newtable(L);
    lua_pushnil(L);
    int i = 0;
    while (lua_next(L, 1))
    {
        lua_rawseti(L, 2, ++i);
    }
    return 1;
}
static in keys(lua_State *L)
{
    luaL_checktype(L, 1, LUA_TTABLE);
    lua_newtable(L);
    lua_pushnil(L);
    int i = 0;
    while (lua_next(L, 1))
    {
        lua_pop(L, 1); /* remove value */
        lua_pushvalue(L, -1);
        lua_rawseti(L, 2, ++i);
    }
    return 1;
}
```

然后按照惯例在tab_funcs数组里的`{NULL, NULL}`之前添加：

```
{"keys", keys},
{"values", values},
```

然后编译后，lua里便多了两个方法table.keys和table.values。 

注意，很多工程里其实已经有了这两个函数，只不过是Lua版本：

```
function table.keys(t)
    local keys = {}
    for k, v in pairs(t) do
        keys[#keys + 1] = k
    end
    return keys
end
function table.values(t)
    local values = {}
    for k, v in pairs(t) do
        values[#values + 1] = v
    end
    return values
end
```

差不多是一个意思。 

ltablib.c里的库函数并不多，其中foreachi和foreach是仿函数式编程的写法，都是传入一个table和一个function，遍历table，table里的所有k, v座位参数传递个function并执行，如果函数没有返回便继续执行，否则中断遍历。 

其中luaL_checktype（lauxlib.c）是执行了一次类型检查，对于第一个参数t相当于：
```
if (type(t) ~= "table") then
        error("table value expected", 2)
    end
```

对第二个参数f的类型检查与之类似。 

二者的遍历方式有些区别，对于“数组”结构的table，先是通过luaL_getn（lapi.c）获取数组长度，然后一个for循环执行下来，lua_pushvalue（lapi.c）是将当前函数栈上对应位置的值重新压入栈中（原来位置不是受影响），lua_pushinteger是压入一个整形值，lua_rawgeti其实与lua里的rawget有些类似，都是无视元方法对table中的元素进行赋值，只不过lua_rawgeti（lapi.c）针对的是“数组”结构的table。lua_call（lapi.c）是调用函数，第二个参数表示f有多少个参数，第三个参数表示f有多少个返回值，会根据有多少个参数（2个）找到函数（栈下第3位）并执行。最后因为有一个返回值，所以要用lua_pop（lapi.c）把它“弹出”后才能继续后面的逻辑。 

foreach是针对“散列”结构的table（当然对“数组”结构的也适用）。lua_pushnil（lapi.c）先压入一个nil值作为next的第二个参数，next会返回两个值，即当前的key, value。我们注意到在lua_call之前再次将value压入了栈中，所以当前函数栈中有两份value，分别在-1和-4的位置，lua_call执行之后会执行n次“出栈”，n对应于参数个数，所以最后lua_pop要“弹出”两个值。 

maxn是求“散列”结构的table的最大整型下表，这个没什么好说的，一个遍历求最大值。 

getn是获取“数组”结构的table，直接调用了API，我们会在后续文章中做详细介绍。 

setn是个废弃的方法，略过不提。 

tinsert可以有2或3个参数，针对的是“数组”结构的table，如果只有两个参数，便将第二个参数加入table尾部。如果是3个参数，第二个参数作为位置，第三个参数是待插入的值，会稍微麻烦一点，因为涉及到“数组”的移动。 

tremove先是取出要删除的值，然后移动“数组”，将最后一个位置置为nil，然后你会惊奇的发现要删除的值正巧在栈顶，然后`return 1;`

tconcat是将一个“数组”结构的table连接成一个字符串，luaL_optlstring（lauxlib.c）是如果没有或为nil就给一个默认值，相当于：`local sep = sep or ""`
luaL_optint（lauxlib.c）与之类似。 

addfield里用到了lua_isstring（lapi.c）用来判断当前位置的值是字符串（或number）。 

这里用到了luaL_Buffer，这里不做深究，大抵上可以认为是一个用来拼字符串的缓冲器。 

最后用luaL_pushresult（lauxlib.c）将buffer里的字符拼起来。 

最后是sort函数，这里就略过了，有兴趣的可以自己研究。






