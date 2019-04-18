# 关于动态重载Lua脚本的一些思考 - tkokof1的专栏 - CSDN博客

2019年02月21日 15:41:53[tkokof1](https://me.csdn.net/tkokof1)阅读数：99


> 
本文简述了一些自己关于动态重载Lua脚本的思考

平时工作中自己多使用 Lua 脚本,过程中常常会遇到一个痛点:如何动态重载Lua脚本以加快开发的迭代速度.

对于不存在 **外部引用** 和 **内部数据** 的脚本,直接清理重载(譬如使用**require**)即可,非常清爽~

但现实开发中这种理想的情况并不多见,下面就是我对解决这两个问题的一些思考:

#### 外部引用

一般来讲,没有银弹可以解决外部引用问题,我们需要遵守一定的编程范式,以下是我想到的一些方法:
- 不要存在外部引用,统一使用类似 Hub 的机制进行获取,重载脚本后仅需更新 Hub 中的引用即可

```
function func()
    -- get "module" by require or 
    -- you can just export "module" to global env
    local module = require("module")
    if module then
        module.method()
    end
end
```
- 对于一些难以避免的外部引用(譬如注册回调),考虑实现 init/release 机制

```
function module.init()
    register_external()
end

function module.release()
    unregister_external()
end

-- usage
local old_module = require("module")
old_module.release()

-- clear old module here
-- e.g. package.loaded["module"] = nil

local new_module = require("module")
new_module.init()
```
- 另外的一些外部引用(譬如为了编码方便等原因进行的本地缓存),考虑实现 on_reload 机制

```
-- buffer module
local module = require("module")

-- use module

function on_reload(modname)
    if modname == "module" then
        -- re-buffer module
        module = require("module")
    end
end
```

#### 内部数据

内部数据的重载处理可以分情况讨论:

##### 不包含内部数据

如最开始提到的那样,脚本不包含内部数据是"解决"内部数据重载最简洁的解决方案(***解决某个问题的方法之一便是回避这个问题~***)

##### 包含可重建的数据

如果脚本包含的内部数据可以重建,则使用简单的重建方法应该就足够了

```
function module.rebuild_data()
    module.data_1 = 1
    module.data_2 = {}
    ......
end
```

##### 包含不可重建的数据

如果脚本包含不可重建的数据,则需要考虑合并新脚本与旧脚本的内部数据了,这里涉及到 upvalue 等数据的合并,你可能需要实现一个复杂的 reload 方法来进行这项操作,并且这个方法往往缺乏通用性

```
function module.reload(old_module)
    -- merge old module data to new module data ...
end
```

#### 相对靠谱的一种方案

拆分脚本为数据脚本和方法脚本是一种相对靠谱的脚本重载方案

由于方法脚本不存在内部数据,重载起来自然非常简单;

但数据脚本的重载仍然会比较麻烦,尤其是包含不可重建数据时,好在一般来讲,方法脚本的变更比较多,数据脚本的变更比较少,而且数据脚本的变更幅度一般也不会太大,此时实现相应的 reload 函数往往并不困难,相对谨慎的设计基本可以规避数据脚本的重载问题(可以把数据脚本类比于一个数据库).

```
-- module_data.lua
local module_data = {}
module_data.data_1 = 1
module_data.data_2 = {}
......
return module_data

-- module.lua
local module = {}
local module_data = require("module_data")

function module.method()
    module_data.data_1 = module_data.data_1 + 1
    ......
end
```

这种方式有点类似于[ECS](https://en.wikipedia.org/wiki/Entity_component_system).

[这里](https://gist.github.com/tkokof/ed2e69b309a42c37607ea03cdbdb3327)有份简单的gist实现.

#### 更多资料
- [如何让 lua 做尽量正确的热更新](https://blog.codingnow.com/2016/11/lua_update.html)
- [重载一个 skynet 中的 lua 服务](https://blog.codingnow.com/2016/03/skynet_reload.html)
- [lua reload原理](https://blog.csdn.net/liuhangang/article/details/79525727)

