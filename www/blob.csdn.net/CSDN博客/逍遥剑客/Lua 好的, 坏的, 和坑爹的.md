# Lua: 好的,  坏的, 和坑爹的 - 逍遥剑客 - CSDN博客
2013年10月16日 00:42:55[xoyojank](https://me.csdn.net/xoyojank)阅读数：18176
在我使用Lua编程整整9个月后, 是时候停下来反省一下这段经历了. 过去了几年里, 我使用了各式各样的语言:Perl ([soaplite.com](http://soaplite.com/), 还有其它的项目, 包括我现在的咨询工作), C ([DHCPLite](http://notebook.kulchenko.com/dhcplite/dhcp-server-with-arduino-v012) 和 [ping-pong juggling robot](http://notebook.kulchenko.com/juggling/robot-juggles-two-ping-pong-balls)), JavaScript ([Google Maps相关经验 和 canvas](http://notebook.kulchenko.com/maps/google-maps-using-multiple-api-versions)), MATLAB ([ping-pong juggling robot](http://notebook.kulchenko.com/juggling/robot-juggles-two-ping-pong-balls)), 等等, 从Turbo Pascal 到 F# -- 比较一下Lua和我接触过的其它语言是一件非常好玩的事情. 我已经使用Lua完成了一些不同类型的项目: 一个远程调试器([MobDebug](https://github.com/pkulchenko/MobDebug)), 扩展了一个LuaIDE ([ZeroBrane Studio](http://studio.zerobrane.com/)), 一个移动应用程序 ([LuaRemote](https://github.com/pkulchenko/LuaRemote)), 一些教育性的脚本 ([EduPack](https://github.com/pkulchenko/ZeroBraneEduPack)), 还有一个[使用Lua在浏览器画板上进行绘图](http://notebook.kulchenko.com/programming/drawing-on-browser-canvas-with-lua)的demo.
虽然我已经见过很多提到Lua的好和坏的列表 (例如, [Lua的优势](http://lua-users.org/lists/lua-l/2007-11/msg00248.html), [为什么使用Lua](http://blog.datamules.com/blog/2012/01/30/why-lua/), [为什么Lua没有得到广泛地应用](http://lua-users.org/lists/lua-l/2012-01/msg00731.html), [Lua的优点](http://stackoverflow.com/questions/564665/advantages-of-lua),[Lua的好和坏](http://gergely.imreh.net/blog/2011/07/language-of-the-month-lua-part-2/), [Lua对比JavaScript](http://stackoverflow.com/questions/1022560/subtle-differences-between-javascript-and-lua), 还有[Lua的陷阱](http://www.luafaq.org/gotchas.html)), 但是有些特性坑死爹了, 还有些他们忘了提, 所以我就自己搞了个列表. 虽然这说不上非常专业, 也没有覆盖到语言的每个方面 (如 math 和string 库), 但这是根据我的编程语言经历得出来的.
## 好的
- 小巧: [20000行C代码](https://github.com/LuaDist/lua) 可以编译进182K的可执行文件 (Linux下).
- 可移植: 只要是有ANSI C 编译器的平台都可以编译. 你可以看到它可以在几乎所有的平台上运行:从 [microcontrollers](http://www.eluaproject.net/)[Lego Minstorms NXT](http://hempeldesigngroup.com/lego/pblua/), 到 [移动平台](https://github.com/divineprog/mobilelua), 到 [游戏主机](http://blog.brokenfunction.com/2011/11/how-to-make-a-massively-cross-platform-game/),甚至 [浏览器](http://notebook.kulchenko.com/programming/drawing-on-browser-canvas-with-lua) (翻译成JavaScript).
- 作为一个[嵌入式可扩展语言](http://www.lua.org/pil/24.html) 提供了简单直接的 C/C++交互接口.
- 足够快:见 [与其它语言的性能比较](http://benchmarksgame.alioth.debian.org/), 还有一个 [JIT编译器](http://luajit.org/performance.html)可以显著地提高多数任务的性能; 对于那些仍然对性能不满意的人, 可以把关键部分使用C实现, 然后与其集成, 这样还可以享受其它方面的好处. [3/9/2013更新]替换已经消失的结果为 [benchmarksgame](http://benchmarksgame.alioth.debian.org/).
- 文档完善: [参考手册](http://www.lua.org/manual/), [书籍](http://lua.org/pil/), [wiki](http://lua-users.org/wiki/), [6页的简短参考](http://www.capgo.com/Resources/SoftwareDev/LuaShortRef51.pdf) 等.
- 友好和热情的[社区](http://lua-users.org/). 在杰出的文档, wiki, [邮件列表](http://www.lua.org/lua-l.html), 和 [StackOverflow](http://stackoverflow.com/questions/tagged/lua)中,没有什么问题没有我找不到答案的.
- 适合初学者和非程序员的简洁语法. Lua 从 [Modula](http://en.wikipedia.org/wiki/Modula) ([Pascal](http://en.wikipedia.org/wiki/Pascal_%28programming_language%29)的分支, 已经广泛应用于教育做为教学语言)借鉴了多数的控制语法. 我现在仍然记得早期使用过Philippe Kahn的快速而优雅的 [Turbo Pascal](http://en.wikipedia.org/wiki/Turbo_Pascal)IDE.
- 集成的解释器:只需要在命令行下运行 `lua`.
- 先天的[协程](http://www.lua.org/pil/9.html)支持, 用于实现 [迭代器](http://www.lua.org/pil/9.3.html) 和[非抢占式多线程](http://www.lua.org/pil/9.4.html).
- 低延迟的增量[垃圾回收](http://www.lua.org/manual/5.1/manual.html#2.10), 没有额外的内存开销, 低实现复杂度, 并且支持 [weak tables](http://www.lua.org/pil/17.html).
- 强大并多样化的[表](http://www.lua.org/pil/2.5.html) 可以保存任意类型的数据 (除了 `nil`) , 还可以使用任意类型的值进行索引 (除了 `nil`): `{1, 2, 5, foo = "bar", [func] = "something", ["some spaces"] = value()}`.
- 词法作用域.
- [一流的函数](http://www.lua.org/pil/5.html) 和 [闭包](http://www.lua.org/pil/6.1.html) 支持的 [函数式编程](http://www.lua.org/pil/6.html).
- [尾调用](http://www.lua.org/pil/6.3.html): `return functioncall()`.
- 递归函数不需要事先声明: `local function foo() ... foo() ... end`; 注意这样不行 `local foo = function() ... foo() ... end`.
- 函数返回 [多个值](http://www.lua.org/pil/5.1.html): `return 1, 2, 3`. 调用者可以认为返回值是任意个数的: 如果多于3个, 其余会被丢弃; 如果少于3个, 那其它的会是未初始化的 `nil`.
- 函数允许变化的变量个数, `function foo(...) local args = {...}; bar(param, ...) end`.
- Table可以 "拆包" 成参数列表,`unpack` (或 Lua 5.2的 `table.unpack`): `print(unpack({1, 2, 3}))` 打印`1 2 3`.
- 操作环境变量 (`Lua 5.1中的getfenv` 和`setfenv` 和`Lua 5.2中的_ENV` 操作), 此外还可以构造 [沙盒](http://lua-users.org/wiki/SandBoxes) .
- 同时赋值多个变量: `local a, b, c = 1, 2`, `x, y = y, x`, or `a, b = foo()`.
- 多行字符串 (using `[[...]]`; 可以使用 `[[...[=[...]=]...]]`)包含和注释 (`--[[...]]`).
- 可选的分号语句分隔符 (多数用于解决模棱两可的的情况`a = f; (g).x(a)`).
- 重载使用 [metatables](http://www.lua.org/manual/5.1/manual.html#2.8).
- [元编程](http://metalua.luaforge.net/) 可以根据你的 [DSL](http://en.wikipedia.org/wiki/Domain-specific_language)修改抽象语法树来创造新的语法.
- `for` 语句有两种形式: [generic](http://www.lua.org/pil/4.3.5.html) (使用迭代器: `for a in iter() do ... end`) 和 [numeric](http://www.lua.org/pil/4.3.4.html) (使用数字: `for a = 1, 5, 0.1 do ... end`); 数字的这个支持各种类型的步进 (不仅仅是整数).
- 函数调用的语法糖 (`f'string'`, `f"string"`, `f[[string]]`, and`f{table}`)和方法调用(`obj:m()`).
- 简单而强大的 [调试](http://www.lua.org/pil/23.html) 库.
## 与众不同的
- 表和字符串索引从1而不是0开始.
- 对一个表中的值赋 `nil` 会从表中删除它. 这就是说对于不存在的值返回 `nil` , 所以元素存不存在跟它是不是 `nil`是同一个问题. `a = {b = nil}` 产生一个空表.
- 没有独立的整数类型; [数字类型](http://www.lua.org/pil/2.3.html) 表示的是实数.
- 没有类; [面向对象](http://www.lua.org/pil/16.html) 使用 [表](http://www.lua.org/pil/2.5.html) 和 [函数](http://www.lua.org/pil/2.6.html)实现; 继承使用 [metatable](http://www.lua.org/manual/5.1/manual.html#2.8) 机制实现.
- 方法调用使用 `object:method(args)` 的写法, 与 `object.method(object, args)` 的写法是等价的, 但 `object` 只取值一次.
- `nil` 和`false` 是仅有的表示假的值; 0, 0.0, "0" 等其它的一切值都是`true`.
- 不等于是 ~= (例如, `if a ~= 1 then ... end`).
- `not, or, and` 操作符是逻辑运算符.
- [赋值是语句](http://www.lua.org/pil/4.1.html), 这就意味着没有 `a=b=1` 或`if (a=1) then ... end`的写法.
- 没有 `a+=1`, `a++`, 或其它简写形式.
- 没有 `continue` 语句, 尽管有一个 [解释](http://www.luafaq.org/#T1.26) 和一堆的替代品, 如在循环中使用 `repeat break until true` 跳出 或者使用一个Lua 5.2中的`goto` 语句.
- 没有 `switch` 语句.
- 某些上下文可能会用到括号; 例如, `a = {}; a.field` 正常, 但`{}.field` 不行; 后者需要这样写 `({}).field`.
- 循环的控制变量默认是局部的,  循环完了就没了.
- `for` 循环中的极限和步进值是 [缓存](http://www.lua.org/pil/4.3.4.html)过的; 这意味着 `for i = init(), limit(), step() do ... end` 中的三个函数 `init`, `limit`, 和`step` 只在循环开前调用过一次.
- [条件](http://www.lua.org/pil/4.3.1.html) 和其它控制语言不需要括号.
- 字符串和数字会自动转换 (需要一个数字时提供一个字符串, 反之亦然), 除了相等比较: `0 == "0"` 为`false`, `{} ~= 1` 为 `true`, 还有`foo["0"]` 和`foo[0]` 引用的是表中不同的值; 其它关系运算符会在比较不同类型的值时产生错误.
- 逗号和[分号](http://www.lua.org/pil/3.6.html) 都可以作为表中的元素分隔符; 也同样都可以作为 [可选的分隔符](http://www.lua.org/pil/3.6.html) 放在结束括号前: `a = {a = 1, b = 2, }`.
- 比想像中还要少的内部组件; 可能一些人觉得这就像 "电池没有包含在内"一样. 从另一个角度来看, 这成就了它的紧凑而又可移植的核心, 不过同时有一些库可以进行补偿, 如 [LuaRocks](http://luarocks.org/) 和[Penlight](http://stevedonovan.github.com/Penlight/).
## 坏的
- [有限的错误处理支持](http://www.lua.org/pil/8.4.html) (使用[pcall](http://www.lua.org/manual/5.1/manual.html#pdf-pcall) 和[xpcall](http://www.lua.org/manual/5.1/manual.html#pdf-xpcall)),尽管有些人 [争论这已经够用了](http://lua-users.org/lists/lua-l/2010-09/msg00364.html) , 只需要加一些语法糖和特性支持 (如确定性的finalizer).  `pcall` 和`error` 的组合十分强大, 特别是 `error` 可以返回任何东西 (例如一个表)而不是仅仅是一个字符串, 但是 `catch ... finally` 结构在多数情况下可能更加清晰直观.
- 默认是全局的作用域 (这么说对 [Lua 5.2不公平](http://lua-users.org/lists/lua-l/2010-02/msg00753.html), 它已经没有全局了). 有一个 [strict](http://metalua.luaforge.net/src/lib/strict.lua.html) 模块要求所有全局变量都需要初始化. 虽然我并没有很多问题是由未初始化的全局变量引起的, 但还是把它放到"坏的"分类, 因为有一次我犯了一个错误, 在调用一个"next"变量时没有局部化它, [引起一个问题](https://github.com/pkulchenko/ZeroBraneStudio/commit/867de8f3e46ea0ff868a9ae78c632a03fdf7747c) 就是迭代器覆盖了另一个模块的[next](http://www.lua.org/pil/7.3.html) 函数.
- 没有[Unicode 支持](http://lua-users.org/wiki/LuaUnicode) (最起码`string.len` 和模式识别函数需要识别 Unicode 字符); 不过有一个[ICU](http://en.wikipedia.org/wiki/International_Components_for_Unicode)库的[绑定](https://github.com/duncanc/icu4lua) 实现了Unicode支持. 可以看一下这条 [消息](http://lua-users.org/lists/lua-l/2012-02/msg00241.html) 和后续总结的的关于现有的支持和`string.*` 需要什么样的修改.
- 有限的[模式匹配支持](http://www.lua.org/pil/20.1.html), 尽管已有的也十分强大.在使用了15 年 Perl后, 我非常想念其中的一些正则表达式特性(多数是前向搜索, 可选组 `(group )?`, 还有组内组), 没有任何一个都是会增加实现复杂度的. 对于需要更强大的正则表达式的人可以使用 [LPeg](http://www.inf.puc-rio.br/~roberto/lpeg/lpeg.html) 和它的 [re](http://www.inf.puc-rio.br/~roberto/lpeg/re.html) 模块.
- 没有三目运算符; [有一些替代品](http://lua-users.org/wiki/TernaryOperator). 一般我使用 `foo = test and value1 or value2` 形式,   `value2` 在`test` 和`value1` 都为 `false`时可以赋值.
- 没有内置POSIX函数. 虽然有 [luaposix](https://github.com/rrthomas/luaposix) 模块, 但是它需要编译, 这并不是一个好的选择. 尽管对于这个我并没有很强的需求, 但是每当我需要获取/设置一个环境变量时总会直观想到去访问 `getenv` 和`setenv`[6/1/2012更新]  miko 在评论中提到, 有 [os.getenv](http://www.lua.org/manual/5.1/manual.html#pdf-os.getenv), 但是没有相应的 `os.setenv`.
- 没有类/对象 finalizer. Lua 通过 [__gc metamethod](http://www.lua.org/pil/29.html)提供finalizer 的功能 , 但它只能用于自定义类型 (不是表), 并且不能跟其它语言的相应功能匹配, 举例来说, Perl中的 [DESTROY](http://perldoc.perl.org/perltoot.html#Destructors) 和 [END](http://perldoc.perl.org/perltoot.html#Class-Destructors)方法. [05/27/2012更新]  Lua 5.1中有一个没有文档说明的 [newproxy](http://lua-users.org/wiki/HiddenFeatures)特性, 它实现了表的 finalizers; Lua 5.2 移除了这个特性的同时增加了 [表的__gc元方法](http://www.luafaq.org/#T8.1.6).
- 没有Lua和C代码之间的yielding: `coroutine.yield` 在跨越 Lua/C 边界调用时会失败 `attempt to yield across metamethod/C-call boundary`. 我在使用 [luasocket](http://w3.impa.br/~diego/software/luasocket/)和协程进行异步编辑时多次遇到过这个错误, 最后使用 [copas](http://keplerproject.github.com/copas/) 模块解决. 在Lua 5.2中这个问题得到解决.
## 坑爹的
- 表中元素的个数并不是很容易获取, 结果取决于你怎么做 (或你怎么定义"长度"). 这可能不是个意外, 因为Lua提供了强大的表并支持灵活的索引方式 (数字或其它Lua类型, 除了 `nil`). Lua中的表有两部分:  "数组" 部分(使用 `t = {1, 2, 3}`生成) 和 "哈希" 部分(使用`t = {a = "foo", ["b"] = 2}`生成); 这两者可以灵活地结合在一起. `#table` 返回最短的"数组"部分长度(没有任何缺口) 而`table.maxn(t)` 返回最长的 "数组" 部分(Lua 5.2移除了这个函数). "哈希" 部分没有定义长度. 两者都可以使用 `pairs` 方法进行遍历, 同时允许你对其中的元素进行计数. 然而, `print(#{1, 2, nil, 3})` 打印4 却不是想像中的 2 ,`print(#{1, 2, nil, 3, nil})` 打印的则是2. 我确信有一个合理的理由解释它, 但是现在说是就是"坑爹"的地方. [11/17/2012更新] FireFly 在评论中提到, Lua 5.2 中表的长度 [只定义成](http://www.lua.org/manual/5.2/manual.html#3.4.6) 没有洞的.
- `return` 必须是语句块中的最后一句; 也就是说,`function foo() print(1); return; print(2) end` 会触发一个错误 `'end' expected...` 或 `unexpected symbol near <whatever statement you have after 'return'>` (这取决于在`return` 之后有没有分号). 没有人会这样写, 除非你在调试, 但我却被它坑了好几次. 原本我想把它放进"与众不同的"分类, 但是我发现它前后矛盾. 在一个不能使用 `return`的地方却能使用 `do return end` . [5/19/2012更新] 这同样出现在 `break` 语句上, 虽然在Lua 5.2中 `break`[不再必须是语句块的最后一句了](http://lua-list.2524044.n2.nabble.com/patch-to-goto-optional-semicolon-changing-opcodes-td7566529.html).
- 函数只返回一个值但它并不是列表中的最后一个; 如:
```
function f123() return 1, 2, 3 end
  function f456() return 4, 5, 6 end
  print(f123(), f456()) -- prints 1, 4, 5, 6
  print(f456(), f123()) -- prints 4, 1, 2, 3
```
这个 `return` 的行为也受到这条规则约束: `return f456()` 返回3个值, 但`return (f456())` 只返回一个值 (注意多出的括号). 关于这个语言特性有 [很好的文档](http://www.lua.org/pil/5.1.html), 但我仍然认为它太坑爹了 (或许在旁人看来它是优点).
总的来说, 到目前为止我很享受这个语言带来的简洁和便利, 尽管有些东西跟我之前的做法有点不一样. 特别是在8岁的儿子很快地学会了Lua的语法后, 我觉得自己的那些关于 Turbo Pascal 的经历已经过时了.
[Posted by Paul Kulchenko on Sunday, March 25, 2012 at 5:22 PM](http://notebook.kulchenko.com/programming/lua-good-different-bad-and-ugly-parts)
