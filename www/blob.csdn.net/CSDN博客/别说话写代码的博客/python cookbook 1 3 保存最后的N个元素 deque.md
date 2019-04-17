# python cookbook 1.3 保存最后的N个元素 deque - 别说话写代码的博客 - CSDN博客





2019年01月07日 18:58:14[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：25标签：[python cookbook																[保存最后的N个元素																[python deque](https://so.csdn.net/so/search/s.do?q=python deque&t=blog)
个人分类：[python](https://blog.csdn.net/qq_21997625/article/category/7160968)

所属专栏：[python cookbook](https://blog.csdn.net/column/details/32402.html)](https://so.csdn.net/so/search/s.do?q=保存最后的N个元素&t=blog)




from collections import deque

deque(maxlen=N)创建了一个固定 长度的队列。deque是队列，可以 再两端执行添加和弹出操作

```python
from collections import deque
q=deque(maxlen=3) #创建一个固定长度的队列
q.append(1)
q.append(2)
q.append(3)
print(q)
```

![](https://img-blog.csdnimg.cn/20190107185638137.png)

```python
q.append(4)
print(q)
```

![](https://img-blog.csdnimg.cn/20190107185701971.png)

```python
que = deque() #不指定队列大小，在两端都可以进行添加和弹出操作
que.append(1)
que.append(2)
que.append(3)
print(que)
que.appendleft(5)
print(que)
que.pop()
print(que)
que.popleft()
print(que)
#  从队列两端插入或删除元素复杂度是O（1），列表头插入或者删除元素复杂度为O(n)
```

![](https://img-blog.csdnimg.cn/20190107185725718.png)

```python
#下面代码做简单的文本匹配操作，当发现有匹配时就输出当前的匹配行以及最后检查过的N行文本
''' 文本内容为
i love python
i love c++
hello world
python is a good language
python is easy
python cookbook
print end
'''
from collections import deque
def search(lines,pattern,history=5):
    prelines = deque(maxlen=history)
    for line in lines:
        if pattern in line:
            yield line,prelines
        prelines.append(line)
if __name__=='__main__':
    with open('file.txt') as f:
        for line,prelines in search(f,'python',5):
            for pline in prelines:
                print(pline,end='')
            print(line,end='')
            print('-'*20)
```

![](https://img-blog.csdnimg.cn/20190107185750732.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzIxOTk3NjI1,size_16,color_FFFFFF,t_70)](https://so.csdn.net/so/search/s.do?q=python cookbook&t=blog)




