
# Python中的字典 - 喜欢打酱油的老鸟 - CSDN博客


2018年08月14日 15:31:15[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：54


[http://blog.itpub.net/31543790/viewspace-2157895/](http://blog.itpub.net/31543790/viewspace-2157895/)
**字典(dict)**
dic是映射类型,由{}括起来的键值对组成,在dict中key是唯一的.在保存的时候,根据key来计算出唯一个内存地址. 然后将key-value保存在这个地址中.这种算法被称为hash算法,所以,在dict中存储的key-value中的key必须是可hash的,如果你搞不懂什什么是可哈希, 暂时可以这样记, 可以改变的都是不可哈希的,可哈希就意味着不可变. 这个是为了能准确的计算内存地址而规定的。
![Python教程](http://www.ujiuye.com/uploadfile/2018/0712/20180712112500231.jpg)
已知的可哈希(不可变)的数据类型: int, str, tuple, bool
不可哈希(可变)的数据类型: list, dict, set
**语法:**{key1: value1, key2: value2....}
dict保存的数据不是按照我们添加进去的顺序保存的. 是按照hash表的顺序保存的. 而hash表不是连续的. 所以不能进行切片工作. 它只能通过key来获取dict中的数据,在3.6之前.打印一个字典的结果中键值对的顺序都是错乱的,3.6之后,键值对的顺序与输入的顺序一致,但是保存的时候还是无序的
**增加:**
1.dict[一个字典中不存在的key0 = 值, 就会新增一个键值对
2.setdefalt():设置默认值,当没有为一个key设置value时,key等于这个默认value,通过方法1设置之后,value就会是设定的那个value
**删除:**
1.pop(key):类似于list的pop,不过list使用的是下标,而dict用的是key
2.del 关键字 用法同list
3.popitem():随机删除一个键值对
4.clear():清空,一个不剩
**修改:**
1.dict[一个已经存在的key] = value 会对已有的键值对重新赋值,覆盖原有的的value
2.dict1.update(dict2):把dict2中的键值对更新到dict1中,key一样的会覆盖,没有的键值对会新增
**查询:**
1..dict[一个已经存在的key] 当key在dict中不存在的时候,会报错
2.get(key,defalt=None):与方法1不同的是.key不存在的时候返回None,这个None可以通过参数defalt修改
**其他相关操作:**
例:dic = {"id": 123, "name": 'sylar', "age": 18, "ok": "科比"}
1.print(dic.keys) \# dict_keys(['id', 'name', 'age', 'ok']) 它的形式类似于list,但是并不是list,print(type(dic.keys))的结果是 ,但是可以当做list来用,可以进行for循环.
for key in dic.keys:
print(key)
2.print(dic.values()) \#同keys基本一样,用法也相同
for value in dic.values:
print(value)
3.print(dic.items()) \#dict_items([('id', 123), ('name', 'sylar'), ('age',18), ('ok', '科比')])
for key, value in dic.items:
print(key, value)
*一个特殊情况:直接遍历dic,打印的也是key.
for i in dic:
print(i)
http://shenzhen.offcn.com/

