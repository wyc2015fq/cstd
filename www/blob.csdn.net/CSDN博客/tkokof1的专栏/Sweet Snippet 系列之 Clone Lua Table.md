# Sweet Snippet 系列之 Clone Lua Table - tkokof1的专栏 - CSDN博客

2018年03月20日 19:59:28[tkokof1](https://me.csdn.net/tkokof1)阅读数：74


## 引子

参加工作好多年了,却一直对程序中的clone(拷贝)语义比较陌生,印象中很多开发书籍对此都有很大篇幅的讨论,但自己平日里用到的情况却很少. 

细想一下原因,可能还是由于拷贝的定义不明引起的:**深拷贝**和**浅拷贝**几乎是开发人员的常识,概念上似乎很简单,但是真正联系到实际项目,那就需要仔细思忖了. 

就拿Lua中table举例,我们现在想要执行一次深拷贝操作,简单来想似乎写个递归就可以了,代码大概是这个样子:

```
function clone(tbl)
  if type(tbl) ~= "table" then 
      return tbl 
  end
  local clone_tbl = {}
  for k, v in pairs(tbl) do
      clone_tbl[clone(k)] = clone(v)
  end
  return clone_tbl
end
```

但是这个实现有几个问题: 

1. 没有处理metatable(元表) 

2. 不能处理recursive table(table中存在递归的引用,最简单的情况便是table中存在对自身的引用) 

3. table可能定义了__pairs 元方法,代码直接使用pairs遍历table执行拷贝可能存在问题

对于recursive table的问题我们可以通过缓存记录来解决,但是对于metatable相关的两个问题却没有简单答案了,网上很多同学提供的方案都是使用setmetatable(clone_tbl, getmetatable(tbl))的方式来进行设置,但实际上,哪怕你确实需要保持元表关系,也仍然需要根据实际的项目情况来决定是否还需要**深拷贝**元表(setmetatable(clone_tbl, clone(getmetatable(tbl))))

## 代码

这里简单列个参考实现(细节大部分还是根据自己的项目需求所定)

```
local function clone_table_shallow(tbl)
    if type(tbl) ~= "table" then
        return tbl
    else
        local clone_table = {}
        for k, v in pairs(tbl) do
            clone_table[k] = v
        end
        return clone_table
    end
end

local function clone_table_recur(tbl, lookup_table)
    if type(tbl) ~= "table" then
        return tbl
    elseif lookup_table[tbl] then
        return lookup_table[tbl]
    end

    local new_table = {}
    lookup_table[tbl] = new_table
    for key, value in pairs(tbl) do
        new_table[clone_table_recur(key, lookup_table)] = clone_table_recur(value, lookup_table)
    end
    return new_table
end

local function clone_table_deep(tbl)
    local lookup_table = {}
    return clone_table_recur(tbl, lookup_table)
end

-- NOTE : 
-- 1. do not handle "metatable" now
-- 2. only support "table" clone
-- 3. it will influence by the "__pairs" metamethod
function clone(tbl, shallow_clone)
    if shallow_clone then
        return clone_table_shallow(tbl)
    else
        return clone_table_deep(tbl)
    end
end
```

## 更多

关于这个话题的更多讨论可以看[这里](http://lua-users.org/wiki/CopyTable)和[这里](https://stackoverflow.com/questions/640642/how-do-you-copy-a-lua-table-by-value),参考的gist代码可以在[这里](https://gist.github.com/tylerneylon/81333721109155b2d244)和[这里](https://gist.github.com/Deco/3985043)找到

