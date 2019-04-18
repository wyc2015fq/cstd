# Sweet Snippet系列之 Print Lua Table - tkokof1的专栏 - CSDN博客

2017年07月12日 20:42:47[tkokof1](https://me.csdn.net/tkokof1)阅读数：166标签：[lua																[函数																[数据结构																[调试																[数据](https://so.csdn.net/so/search/s.do?q=数据&t=blog)](https://so.csdn.net/so/search/s.do?q=调试&t=blog)](https://so.csdn.net/so/search/s.do?q=数据结构&t=blog)](https://so.csdn.net/so/search/s.do?q=函数&t=blog)](https://so.csdn.net/so/search/s.do?q=lua&t=blog)
个人分类：[游戏																[随性																[算法](https://blog.csdn.net/tkokof1/article/category/642709)](https://blog.csdn.net/tkokof1/article/category/642708)](https://blog.csdn.net/tkokof1/article/category/642710)


## 引子

Lua中唯一的内建数据结构就是关联数组(table),平时我们使用Lua时自然也一直在和table打交道,由于Lua中table的灵活性,使其拥有了强大的数据表达能力,但同时也让Lua的调试变的困难. 

很多时候在运行时打印Lua table的内容会非常有助于我们纠错或者检查程序的正确性,不过Lua内建的函数库并没有提供相关的print table函数,不过我们自然可以自己实现一个~

一般的print table实现方式都是使用递归,这便需要考虑table中可能存在循环引用的问题,例如:

```
local t1 = {}
local t2 = { t = t1 }
t1.t = t2
```

此时t1和t2便存在了循环引用,简单的递归打印会导致堆栈溢出或者内存耗尽,有不少方法可以解决这个问题,一种方式便是使用簿记的方式记录下所有访问过的table,对于已经访问过的table做特殊处理; 

另一种较简单的方式是限制递归长度,直接规避掉循环引用所产生的问题,但是对于较深层级的table便无法完整打印table内容了,但在平时的程序开发中我们一般也不会使用如此深层次的table,如果确实用到了,尝试打印该table前请首先思考下程序设计 :)

另一个需要考虑的问题是table中string类型的处理,考虑如下table:
`local t = { [1] = 1, ["1"] = "1" }`
如果打印时不做特殊处理, 可能会出现这种结果:

```
{
    [1] = 1
    [1] = 1
}
```

重复键值的输出结果可能让人感到困惑,也许你可以猜出其中某个键值可能是string类型,但是你仍然不能确定键值的数据类型,既然打印table是为了更方便的调试或者查证,那打印table本身便不应该再无端的增添问题. 

解决方法也很简单:使用引号或者双引号来标记string类型的数据.

## 代码

```
local function simple_print_table_recur(tbl, depth, print_func, indent)
    for k, v in pairs(tbl) do
        local indent_str = string.rep("  ", indent)
        local format_str = indent_str
        if type(k) == "string" then
            format_str = format_str .. "[\"" .. k .. "\"] = "
        else
            format_str = format_str .. "[" .. tostring(k) .. "] = "
        end

        if type(v) == "table" then
            if depth > 0 then
                print_func(format_str)
                print_func(indent_str .. "{")
                simple_print_table_recur(v, depth - 1, print_func, indent + 2)
                print_func(indent_str .. "}")
            else
                if next(v) then
                    print_func(format_str .. "{ ... }")
                else
                    print_func(format_str .. "{}")
                end
            end
        else
            if type(v) == "string" then
                print_func(format_str .. "\"" .. tostring(v) .. "\"")
            else
                print_func(format_str .. tostring(v))
            end
        end
    end
end

function SimplePrintTable(tbl, depth, print_func)
    depth = depth or math.huge
    print_func = print_func or print

    if type(tbl) ~= "table" then
        print_func(tostring(tbl))
    else
        print_func("{")
        simple_print_table_recur(tbl, depth, print_func, 2)
        print_func("}")
    end
end

function SimplePrint(tbl, depth, print_func)
    depth = depth or 32 -- simple assume depth here
    print_func = print_func or print

    SimplePrintTable(tbl, depth, print_func)
end

function SimplePrintToString(tbl, depth)
    depth = depth or 32 -- simple assume depth here
    local str_table = {}
    local function print_to_table(str)
        table.insert(str_table, str .. "\n")
    end

    SimplePrintTable(tbl, depth, print_to_table)

    return table.concat(str_table)
end
```

测试代码如下:

```
local t1 = nil
local t2 = 16
local t3 = "name"
local t4 = {}
local t5 = { 1, 2, "name" }
local t6 = { 1, 2, t1 = t5, t2 = t4 }
local t7 = { t2, t4, t1 = { { { t5, t4 } } } }
local t8 = {}
local t9 = { t = t8 }
t8.t = t9

SimplePrint(t1)
print "--------"
SimplePrint(t2)
print "--------"
SimplePrint(t3)
print "--------"
SimplePrint(t4)
print "--------"
SimplePrint(t5)
print "--------"
SimplePrint(t6)
print "--------"
SimplePrint(t7)
print "--------"
SimplePrint(t8)
print "--------"
SimplePrint(t9)
```

## 结语

Lua官网的[wiki](http://lua-users.org/wiki/TableSerialization)中有一个较完整的相关讨论,有兴趣的朋友可以仔细看看~

OK,下次再见吧~


