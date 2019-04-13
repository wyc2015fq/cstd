
# python嵌套字典比较值与取值的实现示例 - jiahaowanhao的博客 - CSDN博客


2018年04月24日 21:29:55[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：634


[python嵌套字典比较值与取值的实现示例](http://cda.pinggu.org/view/25360.html)
本文通过示例给大家介绍了python嵌套字典比较值，取值，分享出来供大家参考学习，下面话不多说了，来一起看看详细的介绍吧。
示例代码
\#取值import types
allGuests = {'Alice': {'apples': 5, 'pretzels': {'12':{'beijing':456}}},
'Bob': {'ham sandwiches': 3, 'apple': 2},
'Carol': {'cups': 3, 'apple pies': 1}}
def dictget(dict1,obj,default=None):
for k,v in dict1.items():
if k == obj:
print(v)
else:
if type(v) is dict:
re=dictget(v,obj)
if re is not default:
print(re)
dictget(allGuests,'beijing')

结果：
![](http://files.jb51.net/file_images/article/201711/201711395521776.png?201710395728)
![](http://files.jb51.net/file_images/article/201711/201711395741261.png?201710395750)
比较大小
defbijiaodict(dict1,dict2):
fork,vindict1.items():
fork2,v2indict2.items():
ifk==k2andv==v2:
print('dict1=dict2')
else:
print('dict1!=dict2')
dict1={'2':'6'}
dict2={2:{1:{1:8}}}
bijiaodict(dict1,dict2)
结果：
![](http://files.jb51.net/file_images/article/201711/201711395804324.png?201710395811)
python字典嵌套字典的情况下获取某个key的value
正好最近在用python写接口的测试程序，期间用到解析字典获取某个key的value，由于多个接口返回的字典格式不是固定的并存在多层嵌套的情况。在字典的方法中也没有找到可直接达到目的的方法，于是自己写了个程序。分享给大家：
\#coding: utf-8
import types
\#获取字典中的objkey对应的值，适用于字典嵌套
\#dict:字典
\#objkey:目标key
\#default:找不到时返回的默认值
def dict_get(dict, objkey, default):
tmp = dict
for k,v in tmp.items():
if k == objkey:
return v
else:
if type(v) is types.DictType:
ret = dict_get(v, objkey, default)
if ret is not default:
return ret
return default
\#如
dicttest={"result":{"code":"110002","msg":"设备设备序列号或验证码错误"}}
ret=dict_get(dicttest, 'msg', None)
print(ret)
总结
以上就是这篇文章的全部内容了，希望本文的内容对大家的学习或者工作具有一定的参考学习价值

