# 说说 Python 的嵌套式数据结构 - weixin_33985507的博客 - CSDN博客
2019年03月02日 20:30:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：1
![3386108-5850bc02a843d255.png](https://upload-images.jianshu.io/upload_images/3386108-5850bc02a843d255.png)
嵌套式数据结构指的是：字典存储在列表中， 或者列表作为值存储在字典中。甚至还可以在字典中嵌套字典。
## 1 字典列表
列表中包含字典。比如花名册：
```
people1 = {'name': '林旭恒', 'school': '安徽师范大学附属中学'}
people2 = {'name': '吴作同', 'school': '中山市第一中学'}
people_list = [people1, people2]
for people in people_list:
    print(people)
```
运行结果：
> 
{'name': '林旭恒', 'school': '安徽师范大学附属中学'}
{'name': '吴作同', 'school': '中山市第一中学'}
有的应用场景，会在列表中包含大量的字典， 而且其中的每个字典都会包含拥有众多属性的大对象。
## 2 列表字典
字典包含列表。比如图书的标签，一本书会被标注多个标签：
```
book = {'title': '现代艺术150年',
        'tags': ['艺术', '艺术史']}
for tags in book['tags']:
    print(tags)
```
运行结果：
> 
艺术
艺术史
在字典中，需要一个键关联多个值时，我们就可以在字典中嵌套一个列表。
> 
**注意：** 列表和字典的嵌套层级不宜太多。 如果嵌套层级多于 2 级， 那么可能有更好的解决方案。
## 3 字典中嵌套字典
比如说真实的英文单词字典：
```
dict = {'evaporation': {'释义': '蒸发', '词性': 'n'},
        'carpenter': {'释义': '木匠', '词性': 'n'}}
for word, info in dict.items():
    print('单词 ' + word + ' 释义为：' + info['释义'] + '，词性为：' + info['词性'])
```
运行结果：
> 
单词 evaporation 释义为：蒸发，词性为：n
单词 carpenter 释义为：木匠，词性为：n
