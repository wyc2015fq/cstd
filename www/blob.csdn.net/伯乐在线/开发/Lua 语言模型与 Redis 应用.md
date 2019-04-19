# Lua 语言模型与 Redis 应用 - 文章 - 伯乐在线
原文出处： [朱吉芳](http://blog.csdn.net/zjf280441589/article/details/52716720)
从 2.6 版本起，Redis 开始支持 Lua 脚本，可以让开发者自己扩展 Redis。本文主要介绍了 Lua 语言不一样的设计模型(相比于Java/C/C++、JS、PHP)，以及 Redis 对 Lua 的扩展，最后结合 Lua 与 Redis 实现了一个支持过期时间的分布式锁。希望读者可以在读完后, 体会到 Lua 这门语言不一样的设计哲学，可以更加得心应手地使用/扩展 Redis。
## 引
案例-实现访问频率限制: 实现访问者 $ip 在一定的时间 $time 内只能访问 $limit 次.
- 非脚本实现


```
private boolean accessLimit(String ip, int limit, int time, Jedis jedis) {
    boolean result = true;
    String key = "rate.limit:" + ip;
    if (jedis.exists(key)) {
        long afterValue = jedis.incr(key);
        if (afterValue > limit) {
            result = false;
        }
    } else {
        Transaction transaction = jedis.multi();
        transaction.incr(key);
        transaction.expire(key, time);
        transaction.exec();
    }
    return result;
}
```
- 以上代码有两点缺陷
- 可能会出现竞态条件: 解决方法是用 `WATCH` 监控 `rate.limit:$IP` 的变动, 但较为麻烦;
- 以上代码在不使用 `pipeline` 的情况下最多需要向Redis请求5条指令, 传输过多.
- Lua脚本实现
Redis 允许将 Lua 脚本传到 Redis 服务器中执行, 脚本内可以调用大部分 Redis 命令, 且 Redis 保证脚本的**原子性**:- 首先需要准备Lua代码: script.lua

```
--
-- Created by IntelliJ IDEA.
-- User: jifang
-- Date: 16/8/24
-- Time: 下午6:11
--
local key = "rate.limit:" .. KEYS[1]
local limit = tonumber(ARGV[1])
local expire_time = ARGV[2]
local is_exists = redis.call("EXISTS", key)
if is_exists == 1 then
    if redis.call("INCR", key) > limit then
        return 0
    else
        return 1
    end
else
    redis.call("SET", key, 1)
    redis.call("EXPIRE", key, expire_time)
    return 1
end
```
- Java


```
private boolean accessLimit(String ip, int limit, int timeout, Jedis connection) throws IOException {
    List<String> keys = Collections.singletonList(ip);
    List<String> argv = Arrays.asList(String.valueOf(limit), String.valueOf(timeout));
    return 1 == (long) connection.eval(loadScriptString("script.lua"), keys, argv);
}
// 加载Lua代码
private String loadScriptString(String fileName) throws IOException {
    Reader reader = new InputStreamReader(Client.class.getClassLoader().getResourceAsStream(fileName));
    return CharStreams.toString(reader);
}
```
> - Lua 嵌入 Redis 优势:
- 减少网络开销: 不使用 Lua 的代码需要向 Redis 发送多次请求, 而脚本只需一次即可, 减少网络传输;
- 原子操作: Redis 将整个脚本作为一个原子执行, 无需担心并发, 也就无需事务;
- 复用: 脚本会永久保存 Redis 中, 其他客户端可继续使用.
## Lua语言模型
Lua是一种 ***便于嵌入应用程序*** 的脚本语言, 具备了作为通用脚本语言的所有功能. 其高速虚拟机实现非常有名(Lua的垃圾回收很有讲究- *增量垃圾回收* ), 在很多虚拟机系性能评分中都取得了优异的成绩. **Home**[lua.org](https://www.lua.org/home.html).
> 
以**嵌入式**为方针设计的Lua, 在默认状态下简洁得吓人. 除了基本的数据类型外, 其他一概没有. 标注库也就 *Coroutine*、*String*、*Table*、*Math*、 *I/O*、*OS*, 再加上*Modules包加载*而已. 参考: [Lua 5.1 Reference Manual – *Standard Libraries*](https://www.lua.org/manual/5.1/manual.html#5)([中文版: Lua 5.1 参考手册](http://www.codingnow.com/2000/download/lua_manual.html)).
注: 本文仅介绍 Lua 与众不同的**设计模型**(对比 **Java/C/C++**、**JavaScript**、**Python** 与 **Go**), 语言细节可参考文内和附录推荐的文章以及Lua之父**Roberto Ierusalimschy**的《Programming in Lua》(中文版: LUA程序设计(第2版)>)
### 基础
#### 1. 数据类型
- 作为通用脚本语言, Lua的数据类型如下:
- 数值型:
全部为浮点数型, 没有整型;
只有 `nil` 和 `false` 作为布尔值的 `false` , 数字 `0` 和空串（`‘’`/`‘’`）都是 `true`;
- 字符串
- 用户自定义类型
- 函数(function)
- 表(table)
> 
变量如果没有特殊说明为全局变量(那怕是语句块 or 函数内), 局部变量前需加`local`关键字.
#### 2. 关键字
![](http://jbcdn2.b0.upaiyun.com/2016/10/fe33e61ca081f36909b9f2d16b5c9d4b.png)
#### 3. 操作符
![](http://jbcdn2.b0.upaiyun.com/2016/10/84414e900ee77fc8fd1baf9fc9c7a7d7.png)
- Tips:
- 数学操作符的操作数如果是字符串会自动转换成数字;
- 连接 `..` 自动将数值转换成字符串;
- 比较操作符的结果一定是布尔类型, 且会严格判断数据类型(`'1' != 1`);
### 函数(function)
在 Lua 中, 函数是和字符串、数值和表并列的基本数据结构, 属于**第一类对象**( *first-class-object* /一等公民), 可以和数值等其他类型一样***赋给变量***、***作为参数传递***, 以及作为***返回值接收(闭包)***:
- 使用方式类似JavaScript:


```
-- 全局函数: 求阶乘
function fact(n)
    if n == 1 then
        return 1
    else
        return n * fact(n - 1)
    end
end
-- 1. 赋给变量
local func = fact
print("func type: " .. type(func), "fact type: " .. type(fact), "result: " .. func(4))
-- 2. 闭包
local function new_counter()
    local value = 0;
    return function()
        value = value + 1
        return value
    end
end
local counter = new_counter()
print(counter(), counter(), counter())
-- 3. 返回值类似Go/Python
local random_func = function(param)
    return 9, 'a', true, "ƒ∂π", param
end
local var1, var2, var3, var4, var5 = random_func("no param is nil")
print(var1, var2, var3, var4, var5)
-- 4. 变数形参
local function square(...)
    local argv = { ... }
    for i = 1, #argv do
        argv[i] = argv[i] * argv[i]
    end
    return table.unpack(argv)
end
print(square(1, 2, 3))
```
### 表(table)
Lua最具特色的数据类型就是**表(Table)**, 可以实现**数组**、`Hash`、**对象**所有功能的万能数据类型:


```
-- array
local array = { 1, 2, 3 }
print(array[1], #array)
-- hash
local hash = { x = 1, y = 2, z = 3 }
print(hash.x, hash['y'], hash["z"], #hash)
-- array & hash
array['x'] = 8
print(array.x, #array)
```
- Tips:
- 数组索引从`1`开始;
- 获取数组长度操作符`#`其’长度’只包括以**(正)整数**为索引的数组元素.
- Lua用**表管理全局变量**, 将其放入一个叫`_G`的table内:

```
-- pairs会遍历所有值不为nil的索引, 与此类似的ipairs只会从索引1开始递遍历到最后一个值不为nil的整数索引.
for k, v in pairs(_G) do
    print(k, " -> ", v, " type: " .. type(v))
end
```
> 
用`Hash`实现对象的还有**JavaScript**, 将数组和`Hash`合二为一的还有**PHP**.
#### 元表
> 
Every value in Lua can have a **metatable/元表**. This metatable is an ordinary Lua table that defines **the behavior of the original value under certain special operations**. You can **change several aspects of the behavior of operations over a value by setting specific fields in its metatable**. For instance, when a non-numeric value is the operand of an *addition*, Lua checks for a function in the field *“__add”* of the value’s metatable. If it finds one, Lua calls this function to perform the addition.
The key for each event in a metatable is a string with the event name prefixed by two underscores`__`; the corresponding values are called metamethods. In the previous example, the key is “__add” and the metamethod is the function that performs the addition.
**metatable**中的键名称为**事件/event**, 值称为**元方法/metamethod**, 我们可通过`getmetatable()`来获取任一值的**metatable**, 也可通过`setmetatable()`来替换**table**的**metatable**. Lua 事件一览表:
![](http://jbcdn2.b0.upaiyun.com/2016/10/32c66edba2fe0ea40e7d8a25b826677b.png)
对于这些操作, Lua 都将其关联到 metatable 的事件Key, 当 Lua 需要对一个值发起这些操作时, 首先会去检查其**metatable**中是否有对应的事件Key, 如果有则调用之以**控制Lua解释器作出响应**.
#### MetaMethods
MetaMethods主要用作一些类似C++中的**运算符重载**操作, 如重载`+`运算符:


```
local frac_a = { numerator = 2, denominator = 3 }
local frac_b = { numerator = 4, denominator = 8 }
local operator = {
    __add = function(f1, f2)
        local ret = {}
        ret.numerator = f1.numerator * f2.denominator + f1.denominator * f2.numerator
        ret.denominator = f1.denominator * f2.denominator
        return ret
    end,
    __tostring = function(self)
        return "{ " .. self.numerator .. " ," .. self.denominator .. " }"
    end
}
setmetatable(frac_a, operator)
setmetatable(frac_b, operator)
local frac_res = frac_a + frac_b
setmetatable(frac_res, operator) -- 使tostring()方法生效
print(tostring(frac_res))
```
> 
关于更多Lua事件处理可参考文档: [Metamethods](http://wiki.roblox.com/index.php?title=Metamethods).
#### MetaTables 与 面向对象
> 
Lua本来就不是设计为一种**面向对象**语言, 因此其面向对象功能需要通过**元表(metatable)**这种非常怪异的方式实现, Lua并不直接支持面向对象语言中常见的类、对象和方法: 其`对象`和`类`通过`表`实现, 而`方法`是通过`函数`来实现.
上面的**Event一览表**内我们看到有`__index`这个事件重载,这个东西主要是重载了`find key`操作, 该操作可以让Lua变得有点面向对象的感觉(类似JavaScript中的*prototype*). 通过Lua代码模拟:


```
local function gettable_event(t, key)
    local h
    if type(t) == "table" then
        local value = rawget(t, key)
        if value ~= nil then
            return value
        end
        h = getmetatable(t).__index
        if h == nil then
            return nil
        end
    else
        h = getmetatable(t).__index
        if h == nil then
            error("error")
        end
    end
    if type(h) == "function" then
        -- call the handler
        return (h(t, key))
    else
        -- or repeat opration on it
        return h[key]
    end
end
-- 测试
obj = { 1, 2, 3 }
op = {
    x = function()
        return "xx"
    end
}
setmetatable(obj, { __index = op['x'] })
print(gettable_event(obj, x))
```
- 对于任何事件, Lua的处理都可以归结为以下逻辑:
- 如果存在规定的操作则执行它;
- 否则从元表中取出各事件对应的`__`开头的元素, 如果该元素为函数, 则调用;
- 如果该元素不为函数, 则用该元素代替`table`来执行事件所对应的处理逻辑.
> 
这里的代码仅作模拟, 实际的行为已经嵌入Lua解释器, 执行效率要远高于这些模拟代码.
#### 方法调用的实现
面向对象的基础是创建对象和调用方法. Lua中, 表作为对象使用, 因此创建对象没有问题, 关于调用方法, 如果表元素为函数的话, 则可直接调用:


```
-- 从obj取键为x的值, 将之视为function进行调用
obj.x(foo)
```
不过这种实现方法调用的方式, 从面向对象角度来说还有2个问题:
- 首先: `obj.x`这种调用方式, 只是将表`obj`的属性`x`这个**函数对象**取出而已, 而在大多数面向对象语言中, **方法的实体位于类中, 而非单独的对象中**. 在JavaScript等**基于原型**的语言中, 是**以原型对象来代替类进行方法的搜索**, 因此**每个单独的对象也并不拥有方法实体**. 在Lua中, 为了实现基于原型的方法搜索, 需要使用元表的`__index`事件:
如果我们有两个对象`a`和`b`,想让`b`作为`a`的**prototype**需要`setmetatable(a, {__index = b})`, 如下例: 为`obj`设置`__index`加上`proto`模板来创建另一个实例:


```
proto = {
    x = function()
        print("x")
    end
}
local obj = {}
setmetatable(obj, { __index = proto })
obj.x()
```
> 
`proto`变成了原型对象, 当`obj`中不存在的属性被引用时, 就会去搜索`proto`.
- 其次: 通过方法搜索得到的函数对象只是单纯的函数, 而无法获得最初调用方法的表(**接收器**)相关信息. 于是, **过程和数据就发生了分离**.JavaScript中, 关于接收器的信息可由关键字`this`获得, 而在Python中通过方法调用形式获得的**并非单纯的函数对象**, 而是一个**“方法对象”** –其接收器会在内部**作为第一参数附在函数的调用过程中**.
而Lua准备了支持方法调用的**语法糖**:`obj:x()`. 表示`obj.x(obj)`, 也就是: **通过冒号记法调用的函数, 其接收器会被作为第一参数添加进来**(`obj`的求值只会进行一次, 即使有副作用也只生效一次).


```
-- 这个语法糖对定义也有效
function proto:y(param)
    print(self, param)
end
- Tips: 用冒号记法定义的方法, 调用时最好也用冒号记法, 避免参数错乱
obj:y("parameter")
```
更多MetaTable介绍可参考文档[Metatable](http://wiki.roblox.com/index.php?title=Metatable)与博客[metatable和metamethod](https://github.com/abbshr/Lua-newbie/wiki/metatable%E5%92%8Cmetamethod).
#### 基于原型的编程
> 
Lua虽然能够进行面向对象编程, 但用元表来实现, 仿佛把对象剖开看到五脏六腑一样.
《代码的未来》中松本行弘老师向我们展示了一个基于原型编程的Lua库, 通过该库, 即使没有深入解Lua原始机制, 也可以实现面向对象:


```
--
-- Author: Matz
-- Date: 16/9/24
-- Time: 下午5:13
--
-- Object为所有对象的上级
Object = {}
-- 创建现有对象副本
function Object:clone()
    local object = {}
    -- 复制表元素
    for k, v in pairs(self) do
        object[k] = v
    end
    -- 设定元表: 指定向自身`转发`
    setmetatable(object, { __index = self })
    return object
end
-- 基于类的编程
function Object:new(...)
    local object = {}
    -- 设定元表: 指定向自身`转发`
    setmetatable(object, { __index = self })
    -- 初始化
    object:init(...)
    return object
end
-- 初始化实例
function Object:init(...)
    -- 默认不进行任何操作
end
Class = Object:new()
```
另存为**prototype.lua**, 使用时只需`require()`引入即可:


```
require("prototype")
-- Point类定义
Point = Class:new()
function Point:init(x, y)
    self.x = x
    self.y = y
end
function Point:magnitude()
    return math.sqrt(self.x ^ 2 + self.y ^ 2)
end
-- 对象定义
point = Point:new(3, 4)
print(point:magnitude())
-- 继承: Point3D定义
Point3D = Point:clone()
function Point3D:init(x, y, z)
    self.x = x
    self.y = y
    self.z = z
end
function Point3D:magnitude()
    return math.sqrt(self.x ^ 2 + self.y ^ 2 + self.z ^ 2)
end
p3 = Point3D:new(1, 2, 3)
print(p3:magnitude())
-- 创建p3副本
ap3 = p3:clone()
print(ap3.x, ap3.y, ap3.z)
```
## Redis – Lua
在传入到Redis的Lua脚本中可使用`redis.call()`/`redis.pcall()`函数调用Reids命令:


```
redis.call("set", "foo", "bar")
local value = redis.call("get", "foo")
```
`redis.call()`返回值就是Reids命令的执行结果, Redis回复与Lua数据类型的对应关系如下:
|Reids返回值类型|Lua数据类型|
|----|----|
|整数|数值|
|字符串|字符串|
|多行字符串|表(数组)|
|状态回复|表(只有一个`ok`字段存储状态信息)|
|错误回复|表(只有一个`err`字段存储错误信息)|
> 
注: Lua 的 `false` 会转化为空结果.
redis-cli提供了`EVAL`与`EVALSHA`命令执行Lua脚本:
- EVAL
`EVAL script numkeys key [key ...] arg [arg ...]`
*key*和*arg*两类参数用于向脚本传递数据, 他们的值可在脚本中使用`KEYS`和`ARGV`两个table访问: `KEYS`表示要操作的键名, `ARGV`表示非键名参数(并非强制).
- EVALSHA
`EVALSHA`命令允许通过脚本的**SHA1**来执行(节省带宽), Redis在执行`EVAL`/`SCRIPT LOAD`后会计算脚本**SHA1**缓存, `EVALSHA`根据**SHA1**取出缓存脚本执行.
### 创建Lua环境
为了在 Redis 服务器中执行 Lua 脚本, Redis 内嵌了一个 Lua 环境, 并对该环境进行了一系列修改, 从而确保满足 Redis 的需要. 其创建步骤如下:
- 创建基础 Lua 环境, 之后所有的修改都基于该环境进行;
- 载入函数库到 Lua 环境, 使 Lua 脚本可以使用这些函数库进行数据操作: 如基础库(删除了`loadfile()`函数)、Table、String、Math、Debug等标准库, 以及CJSON、 Struct(用于Lua值与C结构体转换)、 cmsgpack等扩展库(Redis 禁用Lua标准库中与文件或系统调用相关函数, 只允许对 Redis 数据处理).
- 创建全局表`redis`, 其包含了对 Redis 操作的函数, 如`redis.call()`、 `redis.pcall()` 等;
- 替换随机函数: 为了确保相同脚本可在不同机器上产生相同结果, Redis 要求所有传入服务器的 Lua 脚本, 以及 Lua 环境中的所有函数, 都必须是无副作用的**纯函数**, 因此Redis使用自制函数替换了 Math 库中原有的 `math.random()`和 `math.randomseed()` .
- 创建辅助排序函数: 对于 Lua 脚本来说, 另一个可能产生数据不一致的地方是那些**带有不确定性质的命令**(如: 由于`set`集合无序, 因此即使两个集合内元素相同, 其输出结果也并不一样), 这类命令包括***SINTER***、***SUNION***、***SDIFF***、***SMEMBERS***、***HKEYS***、***HVALS***、***KEYS*** 等.
Redis 会创建一个辅助排序函数`__redis__compare_helper`, 当执行完以上命令后, Redis会调用`table.sort()`以`__redis__compare_helper`作为辅助函数对命令返回值排序.
- 创建错误处理函数: Redis创建一个 `__redis__err__handler` 错误处理函数, 当调用 `redis.pcall()` 执行 Redis 命令出错时, 该函数将打印异常详细信息.
- Lua全局环境保护: 确保传入脚本内不会将额外的全局变量导入到 Lua 环境内.
> 
小心: Redis 并未禁止用户修改已存在的全局变量.
- 完成Redis的`lua`属性与Lua环境的关联:
![](http://jbcdn2.b0.upaiyun.com/2016/10/3916d13312c22d84d29d3860b59544a9.png)
整个 Redis 服务器只需创建一个 Lua 环境.
### Lua环境协作组件
- Redis创建两个用于与Lua环境协作的组件: **伪客户端**– 负责执行 Lua 脚本中的 Redis 命令, `lua_scripts`**字典**– 保存 Lua 脚本:
- 伪客户端
执行Reids命令必须有对应的客户端状态, 因此执行 **Lua 脚本内的 Redis 命令**必须为 Lua 环境专门创建一个伪客户端, 由该客户端处理 Lua 内所有命令: `redis.call()`/`redis.pcall()`执行一个Redis命令步骤如下:
![](http://jbcdn2.b0.upaiyun.com/2016/10/ae7223b50754e37b7cd89cfe24fc13dd.png)
- `lua_scripts`字典
字典key为脚本 **SHA1** 校验和, value为 **SHA1** 对应脚本内容, 所有被`EVAL`和`SCRIPT LOAD`载入过的脚本都被记录到 `lua_scripts` 中, 便于实现 `SCRIPT EXISTS` 命令和脚本复制功能.
### EVAL命令原理
`EVAL`命令执行分为以下三个步骤:
- 定义Lua函数:
在 Lua 环境内定义 **Lua函数** : 名为`f_`前缀+脚本 **SHA1** 校验和, 体为**脚本内容本身**. 优势:- 执行脚本步骤简单, 调用函数即可;
- 函数的局部性可保持 Lua 环境清洁, 减少垃圾回收工作量, 且避免使用全局变量;
- 只要记住 **SHA1** 校验和, 即可在不知脚本内容的情况下, 直接调用 Lua 函数执行脚本(`EVALSHA`命令实现).
- 将脚本保存到`lua_scripts`字典;
- 执行脚本函数:
执行刚刚在定义的函数, 间接执行 Lua 脚本, 其准备和执行过程如下:
1). 将`EVAL`传入的键名和参数分别保存到`KEYS`和`ARGV`, 然后将这两个数组作为全局变量传入到Lua环境;
2). 为Lua环境装载超时处理`hook`(`handler`), 可在脚本出现运行超时时让通过`SCRIPT KILL`停止脚本, 或`SHUTDOWN`关闭Redis;
3). 执行脚本函数;
4). 移除超时`hook`;
5). 将执行结果保存到客户端输出缓冲区, 等待将结果返回客户端;
6). 对Lua环境执行垃圾回收.
> 
对于**会产生随机结果但无法排序的命令**(如只产生一个元素, 如 ***SPOP***、***SRANDMEMBER***、***RANDOMKEY***、***TIME***), Redis在这类命令执行后将脚本状态置为`lua_random_dirty`, 此后只允许脚本调用只读命令, 不允许修改数据库值.
## 实践
使用Lua脚本重新构建带有过期时间的分布式锁.
案例来源: Redis实战> 第6、11章, 构建步骤:
- 锁申请
- 首先尝试加锁:
- 成功则为锁设定过期时间; 返回;
- 失败检测锁是否添加了过期时间;
- wait.
- 锁释放
- 检查当前线程是否真的持有了该锁:
- 持有: 则释放; 返回成功;
- 失败: 返回失败.
### 非Lua实现


```
String acquireLockWithTimeOut(Jedis connection, String lockName, long acquireTimeOut, int lockTimeOut) {
    String identifier = UUID.randomUUID().toString();
    String key = "lock:" + lockName;
    long acquireTimeEnd = System.currentTimeMillis() + acquireTimeOut;
    while (System.currentTimeMillis() < acquireTimeEnd) {
        // 获取锁并设置过期时间
        if (connection.setnx(key, identifier) != 0) {
            connection.expire(key, lockTimeOut);
            return identifier;
        }
        // 检查过期时间, 并在必要时对其更新
        else if (connection.ttl(key) == -1) {
            connection.expire(key, lockTimeOut);
        }
        try {
            Thread.sleep(10);
        } catch (InterruptedException ignored) {
        }
    }
    return null;
}
boolean releaseLock(Jedis connection, String lockName, String identifier) {
    String key = "lock:" + lockName;
    connection.watch(key);
    // 确保当前线程还持有锁
    if (identifier.equals(connection.get(key))) {
        Transaction transaction = connection.multi();
        transaction.del(key);
        return transaction.exec().isEmpty();
    }
    connection.unwatch();
    return false;
}
```
### Lua脚本实现
- Lua脚本: acquire


```
local key = KEYS[1]
local identifier = ARGV[1]
local lockTimeOut = ARGV[2]
-- 锁定成功
if redis.call("SETNX", key, identifier) == 1 then
    redis.call("EXPIRE", key, lockTimeOut)
    return 1
elseif redis.call("TTL", key) == -1 then
    redis.call("EXPIRE", key, lockTimeOut)
end
return 0
```
- Lua脚本: release


```
local key = KEYS[1]
local identifier = ARGV[1]
if redis.call("GET", key) == identifier then
    redis.call("DEL", key)
    return 1
end
return 0
```
- Pre工具: 脚本执行器


```
/**
 * @author jifang
 * <a href='http://www.jobbole.com/members/chchxinxinjun'>@since</a> 16/8/25 下午3:35.
 */
public class ScriptCaller {
    private static final ConcurrentMap<String, String> SHA_CACHE = new ConcurrentHashMap<>();
    private String script;
    private ScriptCaller(String script) {
        this.script = script;
    }
    public static ScriptCaller getInstance(String script) {
        return new ScriptCaller(script);
    }
    public Object call(Jedis connection, List<String> keys, List<String> argv, boolean forceEval) {
        if (!forceEval) {
            String sha = SHA_CACHE.get(this.script);
            if (Strings.isNullOrEmpty(sha)) {
                // load 脚本得到 sha1 缓存
                sha = connection.scriptLoad(this.script);
                SHA_CACHE.put(this.script, sha);
            }
            return connection.evalsha(sha, keys, argv);
        }
        return connection.eval(script, keys, argv);
    }
}
```
- Client


```
public class Client {
    private ScriptCaller acquireCaller = ScriptCaller.getInstance(
            "local key = KEYS[1]\n" +
            "local identifier = ARGV[1]\n" +
            "local lockTimeOut = ARGV[2]\n" +
            "\n" +
            "if redis.call(\"SETNX\", key, identifier) == 1 then\n" +
            "    redis.call(\"EXPIRE\", key, lockTimeOut)\n" +
            "    return 1\n" +
            "elseif redis.call(\"TTL\", key) == -1 then\n" +
            "    redis.call(\"EXPIRE\", key, lockTimeOut)\n" +
            "end\n" +
            "return 0"
    );
    private ScriptCaller releaseCaller = ScriptCaller.getInstance(
            "local key = KEYS[1]\n" +
            "local identifier = ARGV[1]\n" +
            "\n" +
            "if redis.call(\"GET\", key) == identifier then\n" +
            "    redis.call(\"DEL\", key)\n" +
            "    return 1\n" +
            "end\n" +
            "return 0"
    );
    @Test
    public void client() {
        Jedis jedis = new Jedis("127.0.0.1", 9736);
        String identifier = acquireLockWithTimeOut(jedis, "ret1", 200 * 1000, 300);
        System.out.println(releaseLock(jedis, "ret1", identifier));
    }
    String acquireLockWithTimeOut(Jedis connection, String lockName, long acquireTimeOut, int lockTimeOut) {
        String identifier = UUID.randomUUID().toString();
        List<String> keys = Collections.singletonList("lock:" + lockName);
        List<String> argv = Arrays.asList(identifier,
                String.valueOf(lockTimeOut));
        long acquireTimeEnd = System.currentTimeMillis() + acquireTimeOut;
        boolean acquired = false;
        while (!acquired && (System.currentTimeMillis() < acquireTimeEnd)) {
            if (1 == (long) acquireCaller.call(connection, keys, argv, false)) {
                acquired = true;
            } else {
                try {
                    Thread.sleep(10);
                } catch (InterruptedException ignored) {
                }
            }
        }
        return acquired ? identifier : null;
    }
    boolean releaseLock(Jedis connection, String lockName, String identifier) {
        List<String> keys = Collections.singletonList("lock:" + lockName);
        List<String> argv = Collections.singletonList(identifier);
        return 1 == (long) releaseCaller.call(connection, keys, argv, true);
    }
}
```
- 参考 & 推荐
[代码的未来](https://www.douban.com/link2/?url=https://book.douban.com/subject/24536403/&query=%E4%BB%A3%E7%A0%81%E7%9A%84%E6%9C%AA%E6%9D%A5&cat_id=1001&type=search&pos=0)[Redis入门指南](https://www.douban.com/link2/?url=https://book.douban.com/subject/26419240/&query=Redis%E5%85%A5%E9%97%A8%E6%8C%87%E5%8D%97&cat_id=1001&type=search&pos=1)[Redis实战](https://www.douban.com/link2/?url=https://book.douban.com/subject/26612779/&query=Redis%E5%AE%9E%E6%88%98&cat_id=1001&type=search&pos=0)[Redis设计与实现](https://www.douban.com/link2/?url=https://book.douban.com/subject/25900156/&query=Redis%E8%AE%BE%E8%AE%A1%E4%B8%8E%E5%AE%9E%E7%8E%B0&cat_id=1001&type=search&pos=0)[云风的Blog: Lua与虚拟机](http://blog.codingnow.com/eo/luaoeeeaeau/)[Lua简明教程- CoolShell](http://coolshell.cn/articles/10739.html)[Lua-newbie](https://github.com/abbshr/Lua-newbie)[Lua-Users](http://lua-users.org/)[redis.io](http://redis.io/)
