
# map类的erase方法的在Linux与Windows中的差异 - 高科的专栏 - CSDN博客

2014年01月20日 17:29:52[高科](https://me.csdn.net/pbymw8iwm)阅读数：7732


这次的代码是跨平台的，尼玛在win32上通过，但是在linux上不通过了，查找了一下原来是平台linux不支持。
有人举了例子：
```python
std::map<
```
```python
int
```
```python
,
```
```python
float
```
```python
> i_f_map;
```

```python
i_f_map[1] = 1.2f;
```

```python
i_f_map[23] = 1.4f;
```

```python
std::map<
```
```python
int
```
```python
,
```
```python
float
```
```python
>::iterator itr;
```

```python
for
```
```python
(itr = i_f_map.begin(); itr != i_f_map.end(); itr = i_f_map.erase(itr));
```
```python
// win32可用，linux 不可用
```

```python
for
```
```python
(itr = i_f_map.begin(); itr != i_f_map.end(); i_f_map.erase(itr++));
```
```python
// linux，win32可用
```
上边是很多人的做法，我也很赞同，但是为了代码的可读性，我一般会采取一下的措施，先用临时的一个 迭代器保存当前的迭代器，，并把当前的迭代器往后移。然后把这个临时的迭代器删除掉。这样下次循环就接着后移过的迭代器。

```python
for
```
```python
(itr = i_f_map.begin(); itr != i_f_map.end(); ){
```
if(itr->first == 12){
map<int,**float**>::iterator
 tempitr = itr++;
i_f_map.erase(tempitr);
}else{
itr++;
}
```python
}
```
尤其是在嵌套容器中，容器里再嵌套容器，就很有可能出现itr是非法的，导致itr != i_f_map.end()条件不能被判断出来。
所以我建议大家使用下边的。

