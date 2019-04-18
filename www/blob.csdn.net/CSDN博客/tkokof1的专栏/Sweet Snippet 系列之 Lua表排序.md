# Sweet Snippet 系列之 Lua表排序 - tkokof1的专栏 - CSDN博客

2015年03月08日 21:02:33[tkokof1](https://me.csdn.net/tkokof1)阅读数：828


﻿﻿

  作为Lua中实现各类数据结构的基石，表的使用想必是贯穿于整个项目的开发过程之中，其中对表内容的排序想必亦是常见的需求之一，Lua内置的Table函数库便提供了sort函数来实现这项功能，但是仅能支持表中数组部分内容的排序，而想要排序表中哈希部分的内容，简单的一个方法就是另写一个迭代器来支持：


```
function order_pairs(tbl)
    local names_buffer = {}
    for name, _ in pairs(tbl) do
        table.insert(names_buffer, name)
    end
    table.sort(names_buffer, function(a, b) return tostring(a) < tostring(b) end)
    
    local table_index = 0
    local function iterator()
        table_index = table_index + 1
        local key = names_buffer[table_index]
        return key, tbl[key]
    end
    
    return iterator
end
```

写个简单的测试：


```
local function print_table(tbl)
    for k, v in pairs(tbl) do
        print(k .. " = " .. v)
    end
    print "========"
end

local function print_table_order(tbl)
    for k, v in order_pairs(tbl) do
        print(k .. " = " .. v)
    end
    print "========"
end

local t = { [4] = "test", "order", b = "pairs", [2] = "another", a = "string" }

print_table(t)

print_table_order(t)
```

  基本就是这么简单～

**更新:**

  其实之前的实现有两个问题,使用默认的comp实现(简单的基于字符串比较),在某些情况下会得到非期望的排序结果(依赖于字符串比较的实现方式),再者也缺少扩展性,无法定制comp,更好的实现方式还是开放comp的设置,并提供默认实现: 


```
-- order pairs implementation :
-- order is number first, then sort by tostring

local function default_sort_comp(k1, k2)
    if type(k1) == "number" and type(k2) == "number" then
        return k1 < k2
    elseif type(k1) == "number" then
        return true
    elseif type(k2) == "number" then
        return false
    else
        return tostring(k1) < tostring(k2)
    end
end

function order_pairs(t, sort_comp)
    -- get original keys and do sort
    local keys_buffer = {}
    for k, v in pairs(t) do
        table.insert(keys_buffer, k)
    end
    table.sort(keys_buffer, sort_comp or default_sort_comp)
    
    -- gen iterator
    local current_index = 0
    local function iterator()
        current_index = current_index + 1
        local cur_key = keys_buffer[current_index]
        return cur_key, t[cur_key]
    end
    
    return iterator
end
```

