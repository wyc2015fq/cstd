# Unity3D开发小贴士（六）Lua里调用C#扩展 - 凯奥斯的注释 - CSDN博客





2016年11月10日 21:50:00[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：2548
所属专栏：[Unity3D开发小贴士](https://blog.csdn.net/column/details/12907.html)









# [C#语法小知识（八）扩展](http://blog.csdn.net/ecidevilin/article/details/52641213)介绍过如何扩展C#的类型，[Unity3D开发小贴士（五）Lua调用C#](http://blog.csdn.net/ecidevilin/article/details/53072207)里介绍了如何使用ToLua插件在Lua脚本里调用C#代码。那么本文介绍一下如何在Lua里调用C#类型的扩展方法。

首先我们为GameObject创建一个扩展：



```
using UnityEngine;
using System.Collections;

public static class GameObjectExt {

	public static Component GetOrAddComponent(this GameObject obj, System.Type t)
	{
		Component cpt = obj.GetComponent (t);
		if (null == cpt) {
			cpt = obj.AddComponent (t);
		}
		return cpt;
	}
}
```

然后我们在CustomSetting.cs文件里找到GameObject，把它修改为：

`_GT(typeof(GameObject)).AddExtendType(typeof(GameObjectExt)),`
然后在编辑器里，点击Lua->Gen LuaWrap + Binder。然后我们会发现在UnityEngine_GameObjectWrap.cs文件的Register方法里多了一行：

`L.RegFunction("GetOrAddComponent", GetOrAddComponent);`
为GameObject在Lua状态机里注册GetOrAddComponent方法。
接着文件中还多了一个方法，也就是GetOrAddComponent：



```
[MonoPInvokeCallbackAttribute(typeof(LuaCSFunction))]
	static int GetOrAddComponent(IntPtr L)
	{
		try
		{
			ToLua.CheckArgsCount(L, 2);
			UnityEngine.GameObject obj = (UnityEngine.GameObject)ToLua.CheckObject(L, 1, typeof(UnityEngine.GameObject));
			System.Type arg0 = (System.Type)ToLua.CheckObject(L, 2, typeof(System.Type));
			UnityEngine.Component o = obj.GetOrAddComponent(arg0);
			ToLua.Push(L, o);
			return 1;
		}
		catch(Exception e)
		{
			return LuaDLL.toluaL_exception(L, e);
		}
	}
```

这样，我们就可以在Lua里面调用这个扩展方法了。
我们在Main.Lua里添加以下代码：



```
local go = UnityEngine.GameObject.New("ext")
local bc = go:GetOrAddComponent(typeof(UnityEngine.BoxCollider))
print(bc:ToString())
```

然后在Unity场景中为一个对象添加LuaClient脚本，点击运行，就可以看到输出结果：
ext (UnityEngine.BoxCollider)




