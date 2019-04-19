# Lua中的模块与module函数巧用 - fanyun的博客 - CSDN博客
2017年04月04日 18:55:35[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：349
所属专栏：[巧用脚本](https://blog.csdn.net/column/details/15130.html)
1.Lua模块
什么是Lua的模块？通常我们可以理解为是一个table，这个table里有一些变量、一些函数…等等，说白了就跟我们C++，java中的类类似。
我们来看看一个简单的模块，新建一个文件，命名为test.lua，代码如下：
work = {}
function work.do()
   print("开始工作！");
end
function work.quit()
   print("完成工作，下班！");
end
return work;
我们定义了一个table，并且给这个table加了两个字段，只不过这两个字段的值是函数而已。
至于如何使用模块，那就要用到我们之前介绍过的require了。
我们在main函数里这么使用：
代码如下:
local function main()
   cc.FileUtils:getInstance():addSearchPath("src")
    work= require("work");
    work.do();
end
注意，我们要require其他文件的时候，要把文件路径给设置好，否则会找不到文件。因为我使用的是Cocos Code IDE，直接调用addSearchPath函数就可以了，我的work.lua文件是在src目录下的。
好了，运行代码，结果如下：
[LUA-print] 开始工作！
这就是一个很简单的模块，如果我们习惯了Java、C++等面向对象语言，那也可以简单地把模块理解为类。
2.为避免修改每个函数中的模块名
假设我们想把刚刚的work模块改个名字，改成eatDaddyWork，那么，我们需要做以下两件事情：
1).修改work.lua的文件名
2).修改work.lua的内容，把所有的work改成eatDaddyWork
目前的work.lua函数还算少，就两个，实际上一个模块的函数肯定不会少的，那么，要这么去改这些函数，太烦了。如果批量修改，又怕有哪个地方改错。
代码如下:
work = {}
local M = work;
function M.do()
   print("开始工作！");
end
function M.quit()
   print("完成工作，下班！");
end
return M;
我们用一个局部变量M来代替了work，于是，以后我们只需要修改前面两个的work就可以了，函数部分的内容完全不需要去修改。这种方式其实挺有用的，某些情况下，修改越少，越安全。
3．模块名参数
实际上，以后修改模块名，只需要修改模块的文件名就可以了，文件内容可以不管，具体怎么实现？
看代码：
代码如下:
local M = {};
local modelName = ...;
_G[modelName] = M;
function M.do()
   print("开始工作！");
end
function M.quit()
   print("完成工作，下班！");
end
return M;
留意一下，这里有一个 local modelName = …
“…”就是传递给模块的模块名，在这里其实就是“work”这个字符串。
接着，有点微妙了，还记得之前介绍的全局环境_G吗？我们以”work”作为字段名，添加到_G这个table里。
于是，当我们直接调用work的时候，其实就是在调用_G["work"]的内容了，而这个内容就是这里的M。
4.利用非全局环境制作更简洁和安全的模块
还记得我们的非全局环境吗？就是那个setfenv函数，他带给我们飞一般的感受。
我们来看看下面的代码：
代码如下:
local M = {};
local modelName = ...;
_G[modelName] = M;
setfenv(1, M);
function do()
   print("开始工作！");
end
function quit()
   print("完成工作，下班！");
end
return M;
我们把work.lua这个模块里的全局环境设置为M，于是，我们直接定义函数的时候，不需要再带M前缀。因为此时的全局环境就是M，不带前缀去定义变量，就是全局变量，这时的全局变量是保存在M里。
所以，实际上，do和quit函数仍然是在M这个table里。
于是，我们连前缀都不用写了，这真是到了一个极致，简直就是艺术！
另外，由于当前的全局环境是M，所以，在这里不需要担心重新定义了已存在的函数名，因为外部的全局变量与这里无关了。当然，如果大家现在就运行代码，肯定会报错了。
因为我们的全局环境改变了，所以print函数也找不到了。为了解决这个问题，我们看看第5条内容吧！
5.解决原全局变量的无法找到的问题——方案1
第一个方法，就是我们之前介绍过的，使用继承，如下代码：
复制代码代码如下:
local M = {};
local modelName = ...;
_G[modelName] = M;
-- 方法1：使用继承
setmetatable(M, {__index = _G});
setfenv(1, M);
function do()
   print("开始工作！");
end
function quit()
   print("完成工作，下班！");
end
return M;
没错，使用__index元方法就能解决这个问题了，当找不到print等函数时，就会去原来的_G里查找。
6.解决原全局变量的无法找到的问题——方案2
第二个方法更简单，使用一个局部变量把原来的_G保存起来，如下代码：
复制代码代码如下:
local M = {};
local modelName = ...;
_G[modelName] = M;
-- 方法2：使用局部变量保存_G
local _G = _G;
setfenv(1, M);
function do()
   _G.print("开始工作！");
end
function quit()
   _G.print("完成工作，下班！");
end
return M;
这种方法的缺点比较明显，那就是，每次调用print等函数时，都要使用_G前缀。
7.解决原全局变量的无法找到的问题——方案3
第三个方法比较繁琐，使用局部变量把需要用到的其他模块保存起来，如下代码：
复制代码代码如下:
local M = {};
local modelName = ...;
_G[modelName] = M;
-- 方法3：保存需要使用到的模块
local print = print;
setfenv(1, M);
function do()
   print("开始工作！");
end
function quit()
   print("完成工作，下班！");
end
return M;
这种方法的缺点更明显了，所有用到的模块都要用局部变量声明一次。
但，就速度而言，第三种方案比第二种方案快，第二种方法又比第一种快。
8. module函数
通过module，Lua它竟然把我们把这一切都自动完成了。
代码如下:
local M = {};
local modelName = ...;
_G[modelName] = M;
setmetatable(M, {__index = _G});
setfenv(1, M);
就这几句代码，其实我们可以忽略不写，因为，我们有module函数，它的功能就相当于写了这些代码。
我们修改一下work.lua的内容，如下代码：
module(..., package.seeall);
function do()
   print("开始工作！");
end
function quit()
   print("完成工作，下班！");
end
注意，前面的几行代码都没了，只留下了一个module函数的调用。
module函数的调用已经相当于之前的那些代码了。
而package.seeall参数的作用就是让原来的_G依然生效，相当于调用了：setmetatable(M, {__index = _G});
再次留意一下，代码末尾的return M也不见了，因为module函数的存在，已经不需要我们主动去返回这个模块的table了。
最后，Cocos Code IDE，或者说是Cocos2d-x lua的使用，可以帮我们更好的package.loaded，lua路径查找变量和模块，不妨试试！
