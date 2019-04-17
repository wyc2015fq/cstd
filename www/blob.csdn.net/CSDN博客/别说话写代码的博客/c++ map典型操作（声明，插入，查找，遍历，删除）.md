# c++ map典型操作（声明，插入，查找，遍历，删除） - 别说话写代码的博客 - CSDN博客





2019年01月02日 21:18:30[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：82








**声明：**

```cpp
map<string, string> mapStudent;
```

**插入：**

1、在map中，由key查找value时，首先要判断map中是否包含key。

2、如果不检查，直接返回map[key]，可能会出现意想不到的行为。如果map包含key，没有问题，如果map不包含key，使用下标有一个危险的副作用，会在map中插入一个key的元素，value取默认值，返回value。也就是说，map[key]不可能返回null。

```cpp
//用insert函數插入pair
    mapStudent.insert(pair<string, string>("r000", "student_zero"));
 
//用"array"方式插入
    mapStudent["r123"] = "student_first";
    mapStudent["r456"] = "student_second";
```

**查找：**

3、map提供了两种方式，查看是否包含key，m.count(key)，m.find(key)。

4、m.count(key)：由于map不包含重复的key，因此m.count(key)取值为0，或者1，表示是否包含。

5、m.find(key)：返回迭代器，判断是否存在。

6、对于下面的场景，存在key就使用，否则返回null，有下面两种写法：

```cpp
if(m.count(key)>0)
{
    return m[key];
}
return null;
```

```cpp
iter = m.find(key);
if(iter!=m.end())
{
    return iter->second;
}
return null;
```

这里需要注意：前一种方法很直观，但是效率差很多。因为前面的方法，需要执行两次查找。因此，推荐使用后一种方法。

7、对于STL中的容器，有泛型算法find(begin，end，target)查找目标，map还提供了一个成员方法find(key)

**遍历：**

使用迭代器迭代所有key和 value也很简单

```cpp
map<string,int> m;

map<string,int>::iterator it;

it = m.begin();

while(it != m.end())
{
    //it->first;
    //it->second;
    it ++;         
}
```

**删除与清空**：

清空可以 用clear(),删除使用erase()

```cpp
//迭代器刪除
iter = mapStudent.find("r123");
mapStudent.erase(iter);
 
//用關鍵字刪除
int n = mapStudent.erase("r123");//如果刪除了會返回1，否則返回0
 
//用迭代器範圍刪除 : 把整個map清空
mapStudent.erase(mapStudent.begin(), mapStudent.end());
//等同於mapStudent.clear()
```

参考：

[https://www.cnblogs.com/empty16/p/6395813.html](https://www.cnblogs.com/empty16/p/6395813.html)

[https://www.cnblogs.com/nzbbody/p/3409298.html](https://www.cnblogs.com/nzbbody/p/3409298.html)



