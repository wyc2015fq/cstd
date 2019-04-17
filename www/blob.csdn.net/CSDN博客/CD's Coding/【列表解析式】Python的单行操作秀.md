# 【列表解析式】Python的单行操作秀 - CD's Coding - CSDN博客





2017年08月11日 11:08:41[糖果天王](https://me.csdn.net/okcd00)阅读数：1658








## 0x00 前言

```python
items = [line for line in open('test.txt', 'rb')]
```

通常，我们会在代码中看到一些形似上面的炫目操作（什么你们觉得很正常？好嘛是我菜了QvQ）

我一头猛扎进了python的怀抱，正是因为“人生苦短，我用python”，可能是相似的原因，我第一眼就看上了这种表达方式，并且乐于在自己的代码中使用这种方式来更加精简与清晰地生成所需要的列表。

由于在项目实现过程中产生过大量的问题与需求，在无数次尝试之后决定总结一下如何使用列表解析式（也可称作链表推导式，本文中使用前者，不为什么，因为觉得这很Coooooool呀~）来解决这些类型的问题。

## 0x01 两层列表解析式

个人觉得吧，循序渐进是个必要的过程，啊啊啊我好懒啊，好想直接贴代码就算了啊…… 

不行，这么干的话以后我自己也看不懂的…… 

那么稍微解释一下：
```python
# 写法如下
ret = [x for j in L for x in j]

# 这个是不是看起来很头疼？
# 但是实际上我们本来就是为了简短这种写法会很常见的
# 为了方便理解大概是下面这种感觉：
ret = [items for sub_list in L for items in sub_list]

# 其含义是：
ret = []
for sub_list in L:
    for items in sub_list:
        ret.append(items)

# 但是明显解析式会快得多，
# 想要深究原因的话可以试着从append()的实现，
# 与解析式如何生成list的区别这个方向来进行思考。
```

对于形似如下这种讨厌的参差不齐的二维列表，想对其进行Flatten操作的时候可以使用简单的两层列表解析式搞定（但是对于形似 `L = [1, [2,3]]` 这种的是不行的哦）

```python
L = [
    [1,3],
    [3],
    [1,2,3],
    [1,2],
]
```

## 0x02 两层列表解析式·进阶

特别的，实际上本身不是个两层，但是也有类似的需求该如何是好呢？ 

其实我们可以在解析式中对相关元素进行操作的，先直接上代码：

```python
# 举个栗子：
delim = u'；'
L = [u'今天是个好天气！', u'对呀对呀；我也这么想~']
ret = [x for j in L for x in j.split(delim)]

# 其含义是：
ret = []
delim = u'；'
for sentence in L:
    for items in sentence.split(delim):
        ret.append(items)

# 应用场景实例如下，需求为将传入句子按分号隔开获得新的句子列表：
def split_list(L, delim='；'):
    if L.__len__() == 0:
        return []
    if isinstance(L[0], unicode) and isinstance(delim, str):
        delim = delim.decode('utf-8')
    ret = [x for j in L for x in j.split(delim)]
    return filter(lambda x: x != '', ret)
```

在上述例子中，其实只是一个一维列表，但是我们使用 `split(delim)` 将一维列表中的每个字符串变成了一个列表，此处不仅仅是split，你们还可以自行设置各种更复杂的函数，只不过记得返回值是可迭代的就可以啦~

## 0x03 带判断的列表解析式

```python
# 举个栗子：
dic = {
    1:{'index':2, 'data':'klm'},
    2:{'index':7, 'data':'cde'},
}
ran = xrange(1,5)
ret = [(k, v) for (k, v) in dic.items() if v['index'] in ran]

# 其含义是：
ret = []
for (k,v) in dic.items():
    idx = v['index']
    if idx in ran:
        ret.append( (k,v) )

# 应用实例如下，需求为每次生成满足条件的子dict，
# 并更新元素中几个字段表示的在新dict中的位置：
def dict_select(self, dic, lef, rig=None, pos_lef=None):
    """
    return a dict which satisfied the range of current segment.
    :param dic: a dict
    :param lef: left index of word_index
    :param rig: right index of word_index
    :param pos_lef: left index of position
    """
    def change_pos(inp):
        cp = inp.copy()  # MUST BE A COPY for no-modify
        cp[u'word_index'] = inp[u'word_index'] - lef
        cp[u'position'] = inp[u'position'] - pos_lef
        return cp

    if rig:
        ran = xrange(lef, rig)
        ret = [(k, v) for (k, v) in dic.items() if v[u'word_index'] in ran]
    else:
        ret = [(k, v) for (k, v) in dic.items() if v[u'word_index'] >= lef]
    return dict([(k, change_pos(v)) for (k, v) in ret])
```

## 0x04 常用而实用的小函数

```python
a = [1,2,3]
b = [4,5,6,7]

def func(x): return x+1

# lambda 为匿名函数，此处的f和func是一个效果
f = lambda x: x+1
j = lambda x: x%2==1

map(f, a) # 对于list中的每一个元素做函数f操作
# => [2, 3, 4]

zip(a, b) # 按顺序组成pair的list，长度为两者较短的那个
# => [(1, 4), (2, 5), (3, 6)]

filter(j, a) # 判断是否满足条件，输出满足条件的list
# => [1, 3]

a.__len__() # 输出长度，这个要比len()用起来要好些
# => 3
```

## 0x05 综合应用

也就是说，在两层列表解析式中，x, j, L 你们都可以想想怎么瞎改， 

这里我只试着对x和j改来改去，因为我觉得吧—— 

你要改传入的L的话……你为什么不在外头改好了再扔进来呢？
```python
# 栗子快举完了
L = [
    (u'A', u'这个句子大概是A类，我也是A类，还有我A类'),
    (u'B', u'这个句子大概是B类，我才不告诉你我也是B类'),
    (u'A', u'A类吧大概，哼我是A类'),
]

ret = [x.strip() for j in L for x in zip(
    [j[0]] * len(j[1].split(delim)),
    filter(lambda x: x != '', j[1].split(delim))
)]

# 其含义是：
ret = []
delim = u'，'
for pair in L:
    label, data = pair
    items = data.split(delim)
    for it in zip([label] * len(items), items):
        ret.append(it)

# 应用场景实例如下，需求为将传入的(label, data)中的data按逗号隔开
# 并使用共同的label，以获得新的句子列表：
def split_pair(self, L, delim='，'):
    if L.__len__() == 0:
        return []
    if isinstance(L[0][1], unicode) and isinstance(delim, str):
        delim = delim.decode('utf-8')
    return [x for j in L for x in zip(
        [j[0]] * len(j[1].split(delim)),
        filter(lambda x: x != '', j[1].split(delim))
    )]
```

## 0xFF

忙忙碌碌到现在终于有假期了哎~ 

想着要好好休息休息但是觉得不行趁着余热写点东西要不然我这脑子过两天就忘干净了~

BTW. 啊夏活开始了我去看看twitch休息一下下~ 
![@Kancolle Summer17 | center](https://img-blog.csdn.net/20170811102052626?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





