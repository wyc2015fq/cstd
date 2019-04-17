# Python小知识：else除了跟if很搭，你还知道其他的关键词吗？ - zhusongziye的博客 - CSDN博客





2018年03月15日 20:44:45[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：199








用Python这么久了，只知道else与if很搭，其实跟else关系很好的关键词还有while、for。


## 一、while

### 1.1 最基本的while循环

```
while condition:
    statements
```

只要condition条件是True，程序就执行while代码块中的statements。例如：

```
restrict = 5
index = 1

while index<restrict:
    print('{}<{}'.format(index,restrict))
    index+=1
```

```
1<5
2<5
3<5
4<5
```

### 1.2 带else的while循环

大家都知道if_else，我一直以为python的世界里只有if才和else很配，没想到while也可以这么写。貌似很少看到这种写法

```
while conditon:
    statement1
else:
    statement2
```

例如

```
restrict = 5
index = 1

while index<restrict:
    print('{}<{}'.format(index,restrict))
    index+=1
else:
    print('程序结束')
```

```
1<5
2<5
3<5
4<5
程序结束
```

需要注意一点，当while中break之后，else里的指令就不会再执行。

## 二、for循环

### 2.1 最简单的for

```
for item in iterable_obj:
    statements
```

### 2.2 for_else

真没想到，if、while和for都能跟else搭配使用！

```
for item in iterable_obj:
    statement1
else:
    statement2
```

例如

```
fruits = ["banana", "apple", "orange", "kiwi"]

for food in fruits:
    print(food)
else:
    print("reached end of list")
```

```
banana
apple
orange
kiwi
reached end of list
```





