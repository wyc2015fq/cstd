# Python 数据类型的内置函数与方法 - Alex_McAvoy的博客 - CSDN博客





2018年07月23日 21:31:45[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：174








# 【内置函数】

## 1.字符串
|cmp(str1, str2)|比较两个字符串的元素|
|----|----|
|len(str)|返回字符串 str 长度|
|max(str)|返回字符串 str 中最大的字母|
|min(str)|返回字符串 str 中最小的字母|

## 2.列表
|cmp(list1, list2)|比较两个列表的元素|
|----|----|
|len(list)|计算列表元素个数|
|max(list)|返回列表元素最大值|
|min(list)|返回列表元素最小值|
|list(seq)|将元组转换为列表|

## 3.元组
|cmp(tuple1, tuple2)|比较两个元组元素|
|----|----|
|len(tuple)|计算元组元素个数|
|max(tuple)|返回元组中元素最大值|
|min(tuple)|返回元组中元素最小值|
|tuple(seq)|将列表转换为元组|

## 4.字典
|cmp(dict1, dict2)|比较两个字典元素|
|----|----|
|len(dict)|计算字典元素个数，即键的总数|
|str(dict)|输出字典可打印的字符串表示|
|type(variable)|返回输入的变量类型，如果变量是字典就返回字典类型|

# 【方法】

## 1.字符串
|string.find(str, beg=0, end=len(string))|检测 str 是否包含在 string 中 beg 至 end 的指定范围内，若是返回开始的索引值，否则返回-1|
|----|----|
|string.rfind(str, beg=0,end=len(string) )|类似 find()函数，只是从右边开始查找|
|string.count(str, beg=0, end=len(string))|返回 str 在 string 中 beg 至 end 的指定范围内 str 出现的次数|
|string.replace(str1, str2, num=string.count(str1))|把 string 中的 str1 替换成 str2，替换不超过 num 次|
|string.swapcase()|翻转 string 中的大小写|
|string.lower()|转换 string 中的大写字母为小写|
|string.upper()|转换 string 中的小写字母为大写|
|string.isdecimal()|如果 string 只包含十进制数字，则返回 True，否则返回 False.|
|string.isalpha()|如果 string 至少有一个字符并且所有字符都是字母，则返回 True，否则返回 False|
|string.isalnum()|如果 string 至少有一个字符并且所有字符都是字母或数字，则返回 True，否则返回 False|
|string.title()|将 string 的首字母大写|
|string.rstrip()|删除 string 末尾空白|
|string.lstrip()|删除 string 开头空白|
|string.strip()|删除 string 两端空白|

## 2.列表
|list.append(obj)|在列表末尾添加新的对象|
|----|----|
|list.count(obj)|统计某个元素在列表中出现的次数|
|list.extend(seq)|在列表末尾一次性追加另一个序列中的多个值（用新列表扩展原来的列表）|
|list.index(obj)|从列表中找出某个值第一个匹配项的索引位置|
|list.insert(index, obj)|将对象插入列表|
|list.pop([index=-1])|移除列表中的一个元素（默认最后一个元素），并且返回该元素的值|
|list.remove(obj)|移除列表中某个值的第一个匹配项|
|list.reverse()|反向列表中元素|
|list.sort(cmp=None, key=None, reverse=False)|对原列表进行排序|
|list.sorted(cmp=None, key=None, reverse=False)|返回一个新的列表，对其进行排序（不改变原列表顺序）|
|list.title()|将 list 的首字母大写|

## 3.字典
|dict.clear()|删除字典内所有元素|
|----|----|
|dict.copy()|返回一个字典的浅复制|
|dict.fromkeys(seq[, val])|创建新字典，以 seq 中元素做字典的键，val 为字典所有键对应的始值|
|dict.get(key, default=None)|返回指定键的值，如果值不在字典中返回default值|
|dict.has_key(key)|如果键在字典dict里返回true，否则返回false|
|dict.items()|以列表返回可遍历的(键, 值) 元组数组|
|dict.keys()|以列表返回一个字典所有的键|
|dict.setdefault(key, default=None)|和get()类似, 但如果键不存在于字典中，将会添加键并将值设为default|
|dict.update(dict2)|把字典dict2的键/值对更新到dict里|
|dict.values()|以列表返回字典中的所有值|
|pop(key[,default])|删除字典给定键 key 所对应的值，返回值为被删除的值。key值必须给出。 否则，返回default值|
|popitem()|随机返回并删除字典中的一对键和值|

## 4.集合
|set.clear()|删除集合内所有元素|
|----|----|
|set.copy()|返回一个集合的浅复制|
|set.add(x)|向集合内添加一个元素 x|
|set1.different(set2)|返回两组集合的差异|
|set.discard(x)|删除集合指定元素 x|
|set1.isdisjoint(set2)|若两集合是不相交集合则返回 true，否则返回 false|
|set1.issubset(set2)|若 set1 属于 set2 则返回 true，否则返回 false|
|set.pop()|从集合中随机删除一个元素并返回这个元素|
|set.remove(x)|在集合中搜寻给定的元素 x 并删除，若不存在，则会引发keyError异常|
|set1.union(set2)|返回一个新集合，其中包含两集合中不同的元素。|



