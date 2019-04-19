# python字典（一） - Koma Hub - CSDN博客
2018年01月16日 13:35:00[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：168
个人分类：[Python](https://blog.csdn.net/Rong_Toa/article/category/7257892)
**删除字典元素del**
```python
#python 2017.10 rong tao 
name = ['rong','tao']
age = [35]
person = {'name':name, 'age':age}
print person['name']
print person["age"]
# add element
person['hobit'] = 'gym'
print person
rong = {}
rong['name'] = 'tao'
rong['age'] = 27
if rong['age'] < 18:
    print "rong tao is young!"
elif rong['age'] >= 18 and rong['age'] <= 30:
    print "rong tao is eldly"
else:
    print "rong tao is very old"
#delete element
del rong['age']
print rong
favorite_languages = {
    'jen': 'python',
    'sarah': 'c',
    'edward': 'ruby',
    'phil': 'python',
    }
print favorite_languages['jen'].title()
```
**字典项items()**
```python
#python 2017.10 rong tao 
msgs = {'name':'rongtao','age':'170','hobit':'gym'}
print msgs
msgs['tall'] = '170'
print msgs
del msgs['hobit']
print msgs
#items()
for key,value in msgs.items():
    print key + ":" + value
```
**字典关键字keys()**
```python
#python 2017.10 rong tao 
msgs = {'name':'rongtao','age':'170','hobit':'gym'}
print msgs
msgs['tall'] = '170'
print msgs
del msgs['hobit']
print msgs
#items()
for key,value in msgs.items():
    print key + ":" + value
#keys()
msg = ['rongtao','age','name']
for key in msgs.keys():
    print key
    if key in msg:
        print key + ".eq." 
    if 'shabi' not in msgs.keys():
        print "shabi not in msgs.keys()"
#sorted
for a in sorted(msgs.keys()):
    print a
```
**字典值values()**
```python
#python 2017.10 rong tao 
msgs = {'name':'rongtao','age':'170','hobit':'gym'}
print msgs
msgs['tall'] = '170'
print msgs
del msgs['hobit']
print msgs
#items()
for key,value in msgs.items():
    print key + ":" + value
#keys()
msg = ['rongtao','age','name']
for key in msgs.keys():
    print key
    if key in msg:
        print key + ".eq." 
    if 'shabi' not in msgs.keys():
        print "shabi not in msgs.keys()"
#sorted
for a in sorted(msgs.keys()):
    print a
#values()
print msgs
for value in sorted(msgs.values()):
    print value
for value in set(msgs.values()):
    print value
```
**字典键值对**
```python
#python 2017.10 rong tao 
rivers = {'changjiang1':'1','changjiang2':'2','huanghe1':'1','huanghe2':'2','changjiang3':'3'}
print rivers
for river,num in rivers.items():
    print river + ":" + num
print rivers
for river in rivers.keys():
    print river
print rivers
for num in rivers.values():
    print num
print rivers
for river in set(sorted(rivers.keys())):
    print river
for num in sorted(set(rivers.values())):
    print num
```
