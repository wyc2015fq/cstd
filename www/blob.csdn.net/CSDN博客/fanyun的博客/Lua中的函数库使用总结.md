# Lua中的函数库使用总结 - fanyun的博客 - CSDN博客
2017年04月04日 18:58:15[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：590
所属专栏：[巧用脚本](https://blog.csdn.net/column/details/15130.html)
**Lua中的常用函数库汇总：**
**assert(v [, message])：**
功能：相当于C的断言，检查一个值是否为非nil, 若不是则(如果在wow.exe打开调试命令)显示对话框以及输出错误调试信息
参数：
v：当表达式v为nil或false将触发错误,
message：发生错误时返回的信息，默认为"assertionfailed!"
**date(format,time) ：**
功能：返回当前用户机器上的时间.
**collectgarbage(opt [, arg])：**
功能：是垃圾收集器的通用接口，用于操作垃圾收集器，垃圾收集器. (新增于1.10.1)
参数：
opt：操作方法标志
"Stop": 停止垃圾收集器 
"Restart": 重启垃圾收集器 
"Collect": 执行一次全垃圾收集循环
"Count": 返回当前Lua中使用的内存量(以KB为单位)
"Step": 单步执行一个垃圾收集. 步长"Size" 由参数arg指定　(大型的值需要多步才能完成)，如果要准确指定步长，需要多次实验以达最优效果。如果步长完成一次收集循环，将返回True
"Setpause": 设置 arg/100 的值作为暂定收集的时长 
"Setstepmul": 设置 arg/100 的值，作为步长的增幅(即新步长＝旧步长*arg/100)
dofile (filename)
功能：打开并且执行一个lua块,当忽略参数filename时，将执行标准输入设备(stdin)的内容。返回所有块的返回值。当发生错误时，dofile将错误反射给调用者
注：dofile不能在保护模式下运行
**error(message [, level])：**
功能：终止正在执行的函数，并返回message的内容作为错误信息(error函数永远都不会返回) ，发生错误时,输出一条定义的错误信息.使用pcall() (见下面)捕捉错误.
通常情况下，error会附加一些错误位置的信息到message头部.
Level参数指示获得错误的位置,
Level=1[默认]：为调用error位置(文件+行号)
Level=2：指出哪个调用error的函数的函数
Level=0:不添加错误位置信息
_G全局环境表(全局变量)
功能：记录全局环境的变量值的表 _G._G = _G
**getfenv(f)：**
功能：返回函数f的当前环境表
参数：f可以为函数或调用栈的级别，级别1[默认]为当前的函数,级别0或其它值将返回全局环境_G
**getmetatable(object)：**
功能：返回指定对象的元表(若object的元表.__metatable项有值，则返回object的元表.__metatable的值)，当object没有元表时将返回nil
ipairs (t)
功能：返回三个值迭代函数、表、0
多用于穷举表的键名和键值对
如：for i,v in ipairs(t) do 
end
每次循环将索引赋级i，键值赋给v
注：本函数只能用于以数字索引访问的表如：t={"1","cash"}
**load(func [, chunkname])：**
功能：装载一个块中的函数，每次调用func将返回一个连接前一结的字串，在块结尾处将返回nil
当没有发生错误时，将返回一个编译完成的块作为函数,否则返回nil加上错误信息，此函数的环境为全局环境
chunkname用于错误和调试信息
**loadfile([filename])：**
功能：与load类似，但装载的是文件或当没有指定filename时装载标准输入(stdin)的内容
loadstring (string [, chunkname])
功能：与load类似，但装载的内容是一个字串
如：assert(loadstring(s))()
**next(table [, index])：**
功能：允许程序遍历表中的每一个字段，返回下一索引和该索引的值。
参数：table：要遍历的表
　　　index：要返回的索引的前一索中的号，当index为nil[]时，将返回第一个索引的值，当索引号为最后一个索引或表为空时将返回nil
注：可以用next(t)来检测表是否为空(此函数只能用于以数字索引的表与ipairs相类似)
**ipairs(t)：**
功能：返回三个值 next函数、表、0
多用于穷举表的键名和键值对
如：for n,v in pairs(t) do 
end
每次循环将索引赋级i，键值赋给v
注：本函数只能用于以键名索引访问的表如：t={id="1",name="cash"}
**pcall(f, arg1, ···)：**
功能：在保护模式下调用函数(即发生的错误将不会反射给调用者)
当调用函数成功能返回true,失败时将返回false加错误信息
**print(···)：**
功能：简单的以tostring方式格式化输出参数的内容
**rawequal(v1, v2)：**
功能：检测v1是否等于v2，此函数不会调用任何元表的方法
**rawget(table, index)：**
功能：获取表中指定索引的值，此函数不会调用任何元表的方法，成功返回相应的值，当索引不存在时返回nil
注：本函数只能用于以数字索引访问的表如：t={"1","cash"}
**rawset(table, index, value)：**
功能：设置表中指定索引的值，此函数不会调用任何元表的方法，此函数将返回table
**select(index, ···)：**
功能：当index为数字将返回所有index大于index的参数:如：select(2,"a","b") 返回 "b"
当index为"#"，则返回参数的总个数(不包括index)
**setfenv(f, table)：**
功能：设置函数f的环境表为table
参数：f可以为函数或调用栈的级别，级别1[默认]为当前的函数,级别0将设置当前线程的环境表
**setmetatable(table, metatable)：**
功能：为指定的table设置元表metatable，如果metatable为nil则取消table的元表，当metatable有__metatable字段时，将触发错误
注：只能为LUA_TTABLE 表类型指定元表
**tonumber(e [, base])：**
功能：尝试将参数e转换为数字，当不能转换时返回nil
base(2~36)指出参数e当前使用的进制，默认为10进制，如tonumber(11,2)=3
**tostirng(e)：**
功能：将参数e转换为字符串，此函数将会触发元表的__tostring事件
**type(v)：**
功能：返回参数的类型名("nil"，"number", "string", "boolean","table", "function", "thread", "userdata")：
**unpack(list [, i [, j]])：**
功能：返回指定表的索引的值,i为起始索引，j为结束索引
注：本函数只能用于以数字索引访问的表,否则只会返回nil 如：t={"1","cash"}
**_VERSION：**
功能：返回当前Lua的版本号"Lua 5.1". 
**xpcall(f, err)：**
功能：与pcall类似，在保护模式下调用函数(即发生的错误将不会反射给调用者)
但可指定一个新的错误处理函数句柄
当调用函数成功能返回true,失败时将返回false加err返回的结果
**其它lua库函数：**
gcinfo() - 返回使用中插件内存的占用量(kb)以及当前垃圾收集器的使用量(kB).
getfenv(function or integer) - 返回此表已获取函数的堆栈结构或者堆栈等级
getmetatable(obj, mtable) - 获取当前的元表或者用户数据对象.
loadstring("Lua code") - 分析字符串中的lua代码块并且将结果作为一个函数返回
next(table, index) - 返回下一个key,一对表值.允许遍历整个表
pcall(func, arg1, arg2, ...) - 受保护调用. 执行函数内容，同时捕获所有的异常和错误.
select(index, list) - 返回选择此列表中的商品数值.或者是此件物品在列表中的索引值
setfenv(function or integer, table) - 设置此表已获取函数的堆栈结构或者堆栈等级
setmetatable(obj, mtable) - 设置当前表的元表或者用户数据对象
time(table) - 返回从一个unix时间值
type(var) - 判断当前变量的类型,"number", "string", "table", "function"或者 "userdata".
unpack(table) - 解压一个表.返回当前表中的所有值.
xpcall(func, err) - 返回一个布尔值指示成功执行的函数以及调用失败的错误信息.另外运行函数或者错误的返回值
数学函数
这些表函数同样可以参考Lua函数库(他们都是以math.开头, 详情参阅数学库了解更多信息).
三角函数是没有参考的, 包括角度->弧度的转换.暴雪的版本使用角度.而Lua的标准数学库则使用的弧度.
abs(value) - 返回当前数值的绝对值
acos(value) - 返回此角度值的弧余弦值.
asin(value) - 返回此角度值的弧正弦值
atan(value) - 返回此角度值的弧正切值
atan2(y, x) - 在角度中, 返回Y/X的弧正切值.
ceil(value) - 返回一个整数, 不管小数点后面是多少, 整数部分都进1
cos(degrees) - 返回角度的余弦值
deg(radians) - 弧度转换角度
exp(value) - 返回此值的指数值
floor(value) - 返回此值的整数值
frexp(num) - 返回当前数字小数点后的数值以及小数点后的位数
ldexp(value, 倍数) - 输出此数值的倍数
log(value) - 返回此值的自然对数 (以e为底数)
log10(value) - 返回以10为底数的值
max(value[, values...]) - 在一堆值中查找最大值
min(value[,values...]) - 在一堆值中查找最小值
mod(value,modulus) - 返回此值的余数
rad(degrees) - 角度转换弧度
random([ [lower,] upper]) - 返回一个随机数字 (可选界限为整数值)
randomseed(seed) - 为伪随机数字生成器设定种子
sin(degrees) - 返回当前角度的正弦值
sqrt(value) - 返回数值的平方根值(比如100的平方根为10)
tan(degrees) - 返回当前角度的正切值
字符串库
这些表函数同样可以参考Lua字符串库(他们都是以string.开头, 参考字符串库了解更多信息)
format(formatstring[, value[, ...]]) - 格式化字符串
gsub(string,pattern,replacement[,limitCount]) - 全局替换
strbyte(string[, index]) - 转换字符串为整数值(可以指定某个字符).
strchar(asciiCode[, ...]) - 转换整数为相对应的字符
strfind(string, pattern[, initpos[,plain]]) - 在一个指定的目标字符串中搜索指定的内容(第三个参数为索引),返回其具体位置.
strlen(string) - 返回当前字符串的字符数
strlower(string) - 将字符串的字母转换为小写格式
strmatch(string, pattern[, initpos]) - 与strfind不同的是,macth返回的是具体值,而find返回的是此值的位置
strrep(seed,count) - 返回一个字符串种子副本的个数
strsub(string, index[, endIndex]) - 返回字符串指定位置的值.
strupper(string) - 将字符串的字母转为大写格式
tonumber(arg[, base]) - 若参数能转为数字则返回一个数值.可以指定转换的类型.默认为十进制整数
tostring(arg) - 转换参数为字符串
下面的字符串函数是wow独有的
strtrim(string) - 去除字符串前后空格
strsplit(delimiter, string) - 分割字符串
strjoin(delimiter, string, string[, ...]) -根据定界符将字符串连接起来
 表函数
这些表函数同样可以参考Lua表库 (他们都是以"table.开头", 查阅Table库 f了解更多信息).
也知道, 很多表都被设计为只有数字索引, 开始于1并且没有结尾(如like {[1] = "foo", [3] = "bar"} --- [2]是不存在的所以为nil值).工作时，与任何其他表并没有差别，但很可能会产生意想不到的结果. 在编写lua中, 这个也就是通常所说的bug
foreach(table,function) - 为表中的每个元素执行函数
foreachi(table,function) - 为表中的每个元素执行函数,按顺序访问(不推荐, 可以使用ipairs代替)
getn(table) - 当作为一个聊表时, 返回当前表的大小. 现在已经废弃了, 你可以直接使用#符号.使用#(table)代替table.getn(table)
ipairs(table) - 返回一个迭代型整数遍历表。
pairs(table) - 返回一个迭代遍历表
sort(table[, comp]) - 对一个数组进行排序, 还可以指定一个可选的次序函数
tinsert(table[, pos], value) - 将一个元素插入指定的位置(默认在表的最后)
tremove(table[, pos]) - 删除指定位置的元素(默认表最后一个)
Bit Functions
魔兽世界包含了Lua BitLib库(他们都是以 "bit."开头). 与C的位运算相同.位运算库并非Lua标准函数库的一部分, 但是在WoW 1.9中得到了应用
bit.bnot(a) - 返回一个a的补充
bit.band(w1,...) - 返回w的位与
bit.bor(w1,...) - 返回w的位或
bit.bxor(w1,...) - 返回w的位异或
bit.lshift(a,b) - 返回a向左偏移到b位
bit.rshift(a,b) - 返回a逻辑右偏移到b位
bit.arshift(a,b) - 返回a算术偏移到b位
bit.mod(a,b) - 返回a除以b的整数余数
