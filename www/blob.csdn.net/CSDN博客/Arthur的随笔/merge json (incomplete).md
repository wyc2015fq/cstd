# merge json (incomplete) - Arthur的随笔 - CSDN博客
2013年04月27日 14:47:30[largetalk](https://me.csdn.net/largetalk)阅读数：1095
个人分类：[杂七杂八																[python](https://blog.csdn.net/largetalk/article/category/715661)](https://blog.csdn.net/largetalk/article/category/823511)
```python
from pprint import pprint
a = {
        "_types" : [
            "InterimResponseHistory"
            ],
        "uptime" : "2013-04-26T06:16:06.638Z",
        "permission_id" : "w131",
        "_cls" : "InterimResponseHistory",
        "response" : {
            "current_item_id" : "2013-T5-LJTL-002",
            "items" : [
                {
                    "value" : {
                        "real" : "D",
                        "form" : "2,"
                        },
                    "completed" : True,
                    "marked" : False,
                    "time_spent" : 1,
                    "retry_count" : 0,
                    "submitted" : False,
                    "type" : "sc",
                    "id" : "2013-T5-LJTL-002"
                    }
                ],
            "current_group_id" : "3",
            "current_section_id" : "1",
            "time_spent" : 0,
            "permit" : "w131",
            "sections" : [
                {
                    "time_spent" : 332,
                    "status" : "",
                    "id" : "1"
                    },
                {
                    "time_spent" : 0,
                    "status" : "",
                    "id" : "2"
                    }
                ],
            "id" : "F05"
            }
        }
b = {
        "_types" : [
            "InterimResponseHistory"
            ],
        "uptime" : "2013-04-26T06:16:28.639Z",
        "permission_id" : "w131",
        "_cls" : "InterimResponseHistory",
        "response" : {
            "current_item_id" : "2013-T2-SWCL-003",
            "items" : [
                {
                    "value" : {
                        "real" : "D",
                        "form" : "2,"
                        },
                    "completed" : True,
                    "marked" : False,
                    "time_spent" : 2,
                    "retry_count" : 0,
                    "submitted" : False,
                    "type" : "sc",
                    "id" : "2013-T2-SWCL-003"
                    }
                ],
            "current_group_id" : "4",
            "current_section_id" : "1",
            "time_spent" : 0,
            "permit" : "w131",
            "sections" : [
                {
                    "time_spent" : 354,
                    "status" : "",
                    "id" : "1"
                    },
                {
                    "time_spent" : 0,
                    "status" : "",
                    "id" : "2"
                    }
                ],
            "id" : "F05"
            }
        }
def merge(former, latter):
    if type(former) != type(latter):
        return latter
    if type(former) not in [dict, list]:
        return latter
    if type(latter) == dict:
        for key in latter.keys():
            if key not in former:
                former[key] = latter[key]
            else:
                former[key] = merge(former[key], latter[key])
        return former
    if type(latter) == list:
        for item in latter:
            if type(item) not in [dict, list]:
                try:
                    former.index(item)
                except:
                    former.append(item)
            else:
                if type(item) == dict:
                    iid = item.get('id', None)
                    flag = True
                    if iid:
                        for fitem in former:
                            if type(fitem) == dict and fitem.get('id', None) == iid:
                                merge(fitem, item)
                                flag = False
                                break
                    if flag:
                        former.append(item)
                if type(item) == list:
                    index = latter.index(item)
                    try:
                        former.index(index)
                    except:
                        if former[index] == list:
                            merge(former[index], item)
                        else:
                            former.append(item)
        return former
print 'before:'
print 'a ---> ',
pprint(a)
print 'b ---> ',
pprint(b)
c = merge(a, b)
print '-------------'
print 'after:'
print 'c ---> ', 
pprint(c)
```
```python
$ python mergejson.py 
before:
a ---> {'_cls': 'InterimResponseHistory',
 '_types': ['InterimResponseHistory'],
 'permission_id': 'w131',
 'response': {'current_group_id': '3',
              'current_item_id': '2013-T5-LJTL-002',
              'current_section_id': '1',
              'id': 'F05',
              'items': [{'completed': True,
                         'id': '2013-T5-LJTL-002',
                         'marked': False,
                         'retry_count': 0,
                         'submitted': False,
                         'time_spent': 1,
                         'type': 'sc',
                         'value': {'form': '2,', 'real': 'D'}}],
              'permit': 'w131',
              'sections': [{'id': '1', 'status': '', 'time_spent': 332},
                           {'id': '2', 'status': '', 'time_spent': 0}],
              'time_spent': 0},
 'uptime': '2013-04-26T06:16:06.638Z'}
b ---> {'_cls': 'InterimResponseHistory',
 '_types': ['InterimResponseHistory'],
 'permission_id': 'w131',
 'response': {'current_group_id': '4',
              'current_item_id': '2013-T2-SWCL-003',
              'current_section_id': '1',
              'id': 'F05',
              'items': [{'completed': True,
                         'id': '2013-T2-SWCL-003',
                         'marked': False,
                         'retry_count': 0,
                         'submitted': False,
                         'time_spent': 2,
                         'type': 'sc',
                         'value': {'form': '2,', 'real': 'D'}}],
              'permit': 'w131',
              'sections': [{'id': '1', 'status': '', 'time_spent': 354},
                           {'id': '2', 'status': '', 'time_spent': 0}],
              'time_spent': 0},
 'uptime': '2013-04-26T06:16:28.639Z'}
-------------
after:
c ---> {'_cls': 'InterimResponseHistory',
 '_types': ['InterimResponseHistory'],
 'permission_id': 'w131',
 'response': {'current_group_id': '4',
              'current_item_id': '2013-T2-SWCL-003',
              'current_section_id': '1',
              'id': 'F05',
              'items': [{'completed': True,
                         'id': '2013-T5-LJTL-002',
                         'marked': False,
                         'retry_count': 0,
                         'submitted': False,
                         'time_spent': 1,
                         'type': 'sc',
                         'value': {'form': '2,', 'real': 'D'}},
                        {'completed': True,
                         'id': '2013-T2-SWCL-003',
                         'marked': False,
                         'retry_count': 0,
                         'submitted': False,
                         'time_spent': 2,
                         'type': 'sc',
                         'value': {'form': '2,', 'real': 'D'}}],
              'permit': 'w131',
              'sections': [{'id': '1', 'status': '', 'time_spent': 354},
                           {'id': '2', 'status': '', 'time_spent': 0}],
              'time_spent': 0},
 'uptime': '2013-04-26T06:16:28.639Z'}
```
###################################################################################
找零： 现在有1分、2分、5分的硬币各无限枚，要凑成1元有多少种凑法?
```python
#coding: utf8
def charge(a, coins, i):
    #将总数为a的现金换成n种硬币的不同方式的数目等于
    #1, 将现金数a换成除第一种硬币之外所有其他硬币的不同方式数目， 加上
    #2, 将现金数a-d换成所有种类的硬币的不同方式数目， d是第一种硬币的面值
    if a == 0: return 1
    if a < 0 or i >= len(coins): return 0
    used = charge(a - coins[i], coins, i)
    notUsed = charge(a, coins, i+1)
    return used + notUsed
print charge(100, [1, 2, 5], 0)
def charge2(a, coins):
    opt = [0] * (a + 1)
    opt[0] = 1
    #第一层循环计算包含第i种零钱时j元钱的组合数目
    #第二层循环j(j>coins[i]）元钱的组合数目
    for i in range(len(coins)):
        for j in range(coins[i], a + 1):
            opt[j] += opt[j - coins[i]]
    return opt[a]
print charge2(100, [1,2,5])
```
==============================================================================
很多时候，我们需要将一些大的文件进行跨网传输，一个文件动不动几十G,当你的带宽资源紧张的时候，传输这样几个文件会花10小时，甚至几天。
nc传输文件，通过以下方法，将加快文件传输，希望对大家有所帮助。
环境： 
server1        源地址
server2        目标地址
1.安装nc，多数linux发行版都默认安装了 
apt-get install netcat
2.server2上操作 
进入data目录，确保有足够的空间，然后启动监听 
cd /data 
nc -l 23456 | tar xvzf –
3.server1上操作 
tar cvzf – /var/lib/mysql | nc server2 23456
