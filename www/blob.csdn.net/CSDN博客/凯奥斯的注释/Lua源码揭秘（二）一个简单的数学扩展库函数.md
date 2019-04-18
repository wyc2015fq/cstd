# Lua源码揭秘（二）一个简单的数学扩展库函数 - 凯奥斯的注释 - CSDN博客





2017年10月28日 17:05:21[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：491
所属专栏：[Lua源码揭秘](https://blog.csdn.net/column/details/17856.html)









之前遇到过一个问题，在我们的游戏中大量使用了atan方法，但是atan的方法太费了，所以我们使用了泰勒展开式来求一个近似值。 

arctan的泰勒展开式为： 


$arctan(x)=x-\frac{1}{3}x^3+\frac{1}{5}x^5-\frac{1}{7}x^7\cdots+\frac{{(-1)}^{n-1}}{2n-1}x^{2n-1}$

当时我是用lua实现的，不过这次，不妨考虑写成lua扩展库函数。 

我们直接打开lmathlib.c（在实际工作中的标准做法是在宿主语言中实现）。 

添加以下代码，这里为求简便我们只使用7阶泰勒公式：
```
static const double tCoef[4] = {1.0, -1.0/3.0, 1.0/5.0, -1.0/7.0};
static double tatan(double x)//first 't' for Taylor's formula
{
    double x2 = x * x;
    double x3 = x * x2;
    double x5 = x3 * x2;
    double x7 = x5 * x2;
    return tCoef[0] * x + tCoef[1] * x3 + tCoef[2] * x5 + tCoef[3] * x7;
}
```

这只是实现了具体的算法，还不没有编写lua库函数，我们直接把math_atan函数复制下来，修改一下：

```
static int math_tatan(lua_State *L)
{
    lua_pushnumber(L, tatan(luaL_checknumber(L, 1)));
    return 1;
}
```

编写了库函数还没完，还需要导出。 

在mathlib数组里的
`{NULL, NULL}`
之前添加
`{"tatan", math_tatan},`
然后编译后，lua里便多了一个math.tatan的方法。

因为lmathlib本身并没有什么可讲的，所以本文才举了一下上面的例子。 

lmathlib中的函数大抵上与math_tatan的内容相似。 

luaL_checknumber是定义在lauxlib.c中的C库函数，第一个参数L是状态机（lua_State*），第二个参数narg是当前函数栈的位置，这个函数的作用就是取出栈中的变量，把它转换成number并检查类型（类型检查失败会报错），然后返回。lua_pushnumber是将我们计算后的结果压入栈中，相当于lua的function里面的return xxx。我们注意到函数的返回值，math_modf和math_frexp是`return 2;`而math_randomseed是`return 0;`，其他都是`return 1;`。这是因为这里的返回值是代表lua中返回值的数量。相应的，lua_pushnumber也调用了同等的次数。 

此外我们注意到math_atan2、math_fmod、math_pow、math_ldexp有两个参数，获取第二个参数他们调用了`luaL_checknumber(L, 2)`或者`luaL_checkint(L, 2)`。 

math_min和math_max可以有多个参数，这里使用了lua_gettop来获取当前函数栈上有几个参数，然后跑一个循环得到最小或最大值。 

math_random可以由0或1或2个参数，所以对lua_gettop得到的参数进行switch，针对不同的参数个数做不同的计算。 

看最后一个函数：
```
/*
** Open math library
*/
LUALIB_API int luaopen_math (lua_State *L) {
  luaL_register(L, LUA_MATHLIBNAME, mathlib);
  lua_pushnumber(L, PI);
  lua_setfield(L, -2, "pi");
  lua_pushnumber(L, HUGE_VAL);
  lua_setfield(L, -2, "huge");
#if defined(LUA_COMPAT_MOD)
  lua_getfield(L, -1, "fmod");
  lua_setfield(L, -2, "mod");
#endif
  return 1;
}
```

这里调用了lauxlib.c里的luaL_register方法，将mathlib里的所有方法注册到math里面。 

这里math实际上是一个table，且现在刚刚压入栈中，在栈顶的下一个位置。然后我们把PI值压入栈中，调用lapi.c中的lua_setfield将这个值赋给math.pi。这里操作就等同于：
`math["pi"] = 3.14159265358979323846`
我们注意到，setfield的第二个参数是-2，这里负数表示从栈顶还是往下的计数，-1表示最新压入栈中的值。 

然后lua_setfield会“弹出”一个栈，所以当将HUGE_VAL压入栈后，math还是在-2的位置。 

最后是对mod方法的兼容处理，如果LUA_COMPAT_MOD定义了，那边便调用lua_getfield将fmod从math中取出来便压入栈中，然后赋值给math.mod。等同于：`math["mod"] = math["fmod"]`
最后luaopen_math实际上返回的是一个全局table变量math，所以`return 1;`

这个方法是在linit.c里调用，我们会在后续文章中讲解。






