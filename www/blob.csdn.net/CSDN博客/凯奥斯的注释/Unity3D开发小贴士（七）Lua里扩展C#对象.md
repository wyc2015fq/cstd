# Unity3D开发小贴士（七）Lua里扩展C#对象 - 凯奥斯的注释 - CSDN博客





2016年11月11日 23:08:18[凯奥斯](https://me.csdn.net/ecidevilin)阅读数：2018
所属专栏：[Unity3D开发小贴士](https://blog.csdn.net/column/details/12907.html)









# [Unity3D开发小贴士（六）Lua里调用C#扩展](http://blog.csdn.net/ecidevilin/article/details/53121730)里介绍了如何在Lua里调用C#扩展，但是如果不希望使用C#的扩展，而是希望使用Lua扩展C#对象，那么就得使用到tolua.setpeer。

这个方法的第一个参数为userdata，第二个参数为table。

示例：



```
local function ExtendGameObject(go)
    local tbl = {}
    tolua.setpeer(go,tbl)
    tbl.__index = tbl

    local get = tolua.initget(tbl)
    local set = tolua.initset(tbl)

    local _base = go.base            

    local _name = go.name
    --重写同名属性获取        
    get.name = function(self)                              
        return _base.name                
    end            

    --重写同名属性设置
    set.name = function(self, v)                                                                
        if _name ~= v then         
            _name = v                    
            _base.name = v                                                                                      
        end
    end

    function tbl:AddComponentIfNil(t)
        if nil == _base:GetComponent(t) then
            _base:AddComponent(t)
        end
    end
end
```

tolua.initget和tolua.initset是初始化tbl的属性（property）访问器。
例如：



```
local go = UnityEngine.GameObject.New("ext")
ExtendGameObject(go)
go.name = "extended in lua"
print(go.name)
```
访问和设置name的时候就会调用到get.name和set.name。
我们还可以修改GameObject的同名方法，例如我们可以讲上面的AddComponentIfNil修改为AddComponent。

一般情况下，我们为一个对象添加两个Rigidbody时：



```
go:AddComponentIfNil(typeof(UnityEngine.Rigidbody))
go:AddComponentIfNil(typeof(UnityEngine.Rigidbody))
```


会提示：

Can't add component 'Rigidbody' to extended in lua because such a component is already added to the game object!
但是扩展后，就只会对没有Rigidbody的对象添加Rigidbody了。

当然，这是不是一种科学的办法，因为会有一些组件可以在对象上添加多次，所以最好将这个方法独立出来，使用AddComponentIfNil这样的方法名。




在没有扩展之前，当我们访问或设置userdata不存在的成员的时候，程序就会出错，但是在扩展之后，我们便可以扩展对象的成员。

例如：



```
go.luaName = "test ext"
print(go.luaName)
```

（相关原理，我们有机会再聊。）


