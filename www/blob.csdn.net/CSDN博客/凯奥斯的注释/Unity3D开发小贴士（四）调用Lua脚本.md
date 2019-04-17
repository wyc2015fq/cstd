# Unity3D开发小贴士（四）调用Lua脚本 - 凯奥斯的注释 - CSDN博客





2016年11月06日 20:29:35[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：2460
所属专栏：[Unity3D开发小贴士](https://blog.csdn.net/column/details/12907.html)









# [Unity3D开发小贴士（三）愉快的使用Lua开发](http://blog.csdn.net/ecidevilin/article/details/53025444)中介绍了如何在Unity上利用ToLua插件来进行lua脚本开发。那么本文就介绍一下如何在C#脚本中调用Lua。

首先一个最简单的调用：



```
using UnityEngine;
using LuaInterface;

public class Test : MonoBehaviour {
	public void Start()
	{
		LuaState state = new LuaState ();
		state.Start ();
		state.DoString ("print(\"Hello lua\")");

		state.Dispose();
	}
}
```





将Test脚本挂在到场景中的对象上，点击运行，就可以看到打印出了“Hello lua”。

这里需要用到LuaInterface命名空间，需要new一个LuaState，并执行Start()方法，使用DoString来运行Lua脚本，最后不要忘了Dispose。
理论上来讲，可以使用DoString执行任何Lua脚本，只要传入的字符串足够长。不过这样也就没有使用Lua脚本的意义了。

我们可以使用DoFile方法来执行lua文件。

例如一个名为Test.lua的文件中包含下列Lua代码：


`print("Hello lua in file")`
我们在state.Start()和state.Dispose()之间添加：



`state.DoFile ("Test1.lua");`
就可以打印出“Hello lua in file”。


但是只是执行文件中的代码意义并不是很大，我们希望可以有选择的执行文件中的某个函数。

例如lua文件中添加函数：



```
function testToLua1(t)
    print(t)
    return {t + 1 , t - 2 , t * 3, t / 4}
end
```
传入一个数字t，然后对它分别进行四则运算，返回一个数组形式的table。


那么在Test.cs中添加：



```
LuaFunction func1 = state.GetFunction ("testToLua1");
		func1.BeginPCall ();
		func1.Push (1024);
		func1.PCall ();
		if (state.lua_istable (-1)) {
			int n = state.LuaGetN (-1);
			for (int i = 0; i < n; i++) {
				state.LuaRawGetI (-1, i + 1);
				Debug.Log (state.LuaToNumber (-1));
				state.LuaPop (1);
			}
		}
		func1.EndPCall ();
```

就可以将四则运算的结果打印出来了。


首先获取testToLua1函数，然后执行BeginPCall，将1024这个参数Push到Lua栈里，执行PCall，就会在Lua打印出这个数字，然后会将返回值加入Lua栈中。

lua栈中，最先入栈的的下标为1，然后递增。也可以用负数表示，-1表示栈顶，然后递减。

可以通过lua_istable判断栈顶是否是table，然后使用LuaGetN方法获取数组table的长度，然后遍历数组元素。

遍历的时候使用LuaRawGetI获取table中对应下标的元素，会将这个元素入栈，接着使用LuaToNumber将栈顶元素转换成数（double）并打印出来，然后需要LuaPop将这个数出栈，后面的循环才成立（否则需要修改LuaRawGetI和LuaToNumber，将第一个参数改为对应的栈位置）。

最后EndPCall会将执行函数用到的栈位置清空（还原回执行之前的状态）。




再例如lua文件中再添加一个函数：



```
testToLua2 = function(t)
    print(t)
    return {a=t + 1 , b=t - 2 , c=t * 3, d=t / 4}
end
```

跟之前的函数相同，只是作为返回值是字典形式的table。





调用它的C#代码：



```
LuaFunction fun2 = state.GetFunction ("testToLua2");
		fun2.BeginPCall ();
		fun2.Push (1024);
		fun2.PCall ();
		if (state.lua_istable (-1)) {
			state.LuaGetField (-1, "a");
			Debug.Log (state.LuaToNumber (-1));
			state.LuaPop (1);
			state.LuaGetField (-1, "b");
			Debug.Log (state.LuaToNumber (-1));
			state.LuaPop (1);
			state.Push ("c");
			state.LuaRawGet (-2);
			Debug.Log (state.LuaToNumber (-1));
			state.LuaPop (1);
			state.Push ("d");
			state.LuaRawGet (-2);
			Debug.Log (state.LuaToNumber (-1));
			state.LuaPop (1);
		}
		fun2.EndPCall ();
```

这里使用了两种方式获取key对应的value。


一种是LuaGetField，只需要传入table所在的栈位置和key，就会将value入栈。

另一种是先将key入栈，然后使用LuaRawGet方法（关于rawget参考[Lua语法小贴士（五）元表与元方法](http://blog.csdn.net/ecidevilin/article/details/52858422)）获取value（这时候会将Key出栈）。

（数组形式的table也可以用LuaRawGet方法获取value，只需要先把对应的下标用state.Push入栈）




最后我们看一下可变参数和多个返回值。

Lua代码：



```
testToLua3 = function(...)
    print(...)
    return ...
end
```

没有什么逻辑，只是把传入的数值打印一下然后返回。





C#代码：



```
LuaFunction fun3 = state.GetFunction ("testToLua3");
		fun3.BeginPCall ();
		int oldTop = state.LuaGetTop ();
		for (int i = 0; i < 10; i++) {
			fun3.Push (i);
		}
		fun3.PCall ();
		for (int i = 0, imax = state.LuaGetTop () - oldTop + 1; i < imax; i++) {
			Debug.Log (state.LuaToNumber (-1));
			state.LuaPop (1);
		}
		fun3.EndPCall ();
```

将参数列表的值依次入栈，执行PCall之后，将返回值依次出栈，这里打印出来的值是反序的，毕竟是栈嘛。循环部分可以改成：




```
for (int i = 0, imax = state.LuaGetTop () - oldTop + 1; i < imax; i++) {
			Debug.Log (state.LuaToNumber (-imax+i));
		}
```

这样就变成正序了。


我们可以看到imax = state.LuaGetTop () - oldTop + 1，这是为什么呢？

我们可以在fun3.PCall ();上下分别添加Debug.Log (state.LuaGetTop ());

可以看到PCall之前栈深度是12，PCall之后是11。也就是说PCall这个方法从栈中弹出了一个元素。

我们可以添加一个静态方法用来遍历Lua栈：



```
static public void DumpLuaStack(LuaState luaState)
	{
		for (int i = 1, imax = luaState.LuaGetTop (); i <= imax; i++) {
			LuaTypes luaType = luaState.LuaType (i);
			switch (luaType) {
			case LuaTypes.LUA_TBOOLEAN:
				Debug.Log (luaState.LuaToBoolean (i));
				break;
			case LuaTypes.LUA_TFUNCTION:
				Debug.Log (luaState.GetFunction (i).ToString());
				break;
			case LuaTypes.LUA_TNIL:
				Debug.Log ("NIL");
				break;
			case LuaTypes.LUA_TNUMBER:
				Debug.Log (luaState.LuaToNumber (i));
				break;
			case LuaTypes.LUA_TSTRING:
				Debug.Log (luaState.LuaToString (i));
				break;
			case LuaTypes.LUA_TTABLE:
				Debug.Log (luaState.GetTable (i).ToString ());
				break;
			}
		}
	}
```

注：其实还有别的LuaType，但是本文示例中用不到它们，所以暂且省略掉了。


在PCall之前添加DumpLuaStack(state);

这样既可以看到栈中除了10个参数之外，还有两个LuaFunction，也就是两个testToLua3。




了解这些方法之后，我们可以给C#脚本中通过变量来指定调用哪个Lua文件的哪个函数了。






