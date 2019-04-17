# Python读书笔记：70个注意的小Notes - zhusongziye的博客 - CSDN博客





2018年11月26日 19:28:59[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：78标签：[Python读书笔记：70个注意的小Notes](https://so.csdn.net/so/search/s.do?q=Python读书笔记：70个注意的小Notes&t=blog)
个人分类：[Python语言](https://blog.csdn.net/zhusongziye/article/category/7131144)









**摘要：**在阅读python相关书籍中，对其进行简单的笔记纪要。旨在注意一些细节问题，在今后项目中灵活运用，并对部分小notes进行代码标注。



1 python始终记录变量最新值。

2 变量应简短且具有描述性，如student_name等。

3 变量名推荐小写。

4 单双引号括起来的，字符串可以包含引号和撇号。用法："this's a cup"

5 title()将每个单词的首字母都改为大写。用法：str.title()

6 upper()将字符串转化为大写。用法：str.upper()

7 lower()将字符串转化为小写。用法：str.lower()

8 空白泛指任何非打印字符。如空格、制表符和换行符。

9 rstrip()剔除字符串末尾空白。用法：str.rstrip()

10 lstrip()剔除字符串开头空白。用法：str.lstrip()

11 strip()剔除字符串两端空白。用法：str.strip()

12 Python使用两个称号表示乘方。用法：3 ** 2

13 编程理念。Python之禅：import this

14 list中使用逗号来分割其中的元素。

15 list索引-1返回最后一个元素列表，-2以此类推。用法：list[-3:]

16 list[0] = 'update' 修改列表元素

17 list.append('add') 列表中添加元素

18 list.insert(0.'insert') 列表中指定位置插入元素

19 del list[0] del元素删除list元素

20 newlist = list.pop()方法pop()删除元素

21 从列表中删除元素且不再使用用del方法，删除元素后还有可能用选择pop()

22 list.remove('element') 根据值移除第一个指定的元素，可接着使用。

23 sort()列表按照字母永久性排序。如：list.sort()

24 sort()列表按照字母相反的排序。如：list.sort(reverse=True)

25 reverse() 反转列表元素排序。用法：list.reverse()

26 for循环遍历时候选择有意义的名称。用法： for cat in cats:

27 range() 生成一系列数字。用法： numbers= list(range(1,11,2))

28 list的内建统计函数。用法：min(list)/max(list)/sum(list)

29 python的切片功能。用法： list[0:3]/list[:]/list[-3:]/list[:9]

30 list复制。用法：new_foods = old_food[:]

31 元组包括一些列不可修改的元素且用圆括号标识。用法：tulple = (2,3)

32 检查是否相等时不考虑大小写。用法：str.lower() == 'somestr'

33 使用and检查多个条件。用法：condition1>=1 and condition2>=2 and …

34 使用or检查多个条件。用法：condition1>=1 or condition2>=2 or …

35 使用多个列表。用法：

```
list1 = ['1','2','3','4']
list2 = ['1','4']
for l2 in list2:
    if l2 in list1:
        go()
    else:
        pass
```

36 比较运算符两边各添加空格，便于可读性。用法：if age > 40：

37 dict修改值，用法：dict['key'] = value

38 dict删除键值对，用法： del dict['key']

39 字典的遍历，用法：

```
for key,value in dict.items():
for key in dict:
for key in dict.keys():
for value in dict.values():
for value in set(dict.values()): # 遍历字典的无重复值
```

40 字典列表，用法：

```
dict1 = ['key1':'values1','key2':'values2']
dict2 = ['key1':'values3','key2':'values4']
dict3 = ['key1':'values5','key2':'values6']

dicts = [dict1,dict2,dict3]

for dict in dicts:
    pass
```

41 字典中存储列表，用法：

```
dict1 = {'key1':'values1','key2':['values1','values2']}
for dict in dict1['key2']:
```

42 字典中存储字典，用法：

```
dicts = {
'keys1':{'key1':'values1'，'key1':'values2''key1':'values3'},
'keys2':{'key2':'values2'，'key2':'values2''key2':'values3'}
}
```

43 input接收用户输入，用法：message = input('user input some values!')

44 %取模运算判断奇偶，用法：

```
if (4 % 3) == 0:
    print('偶数')：
else:
    print('奇数')
```

45 while循环的常规用法：

```
current_number = 1
while current_number <= 10:
    print('current_number')
    current_number += 1
```

46 while循环使用标志的用法：

```
flag = True
while flag:
    message = input(prompt)
```

47 列表之间移动元素，用法：

```
while list[]:
    newlist.append(list[].pop())
```

48 删除特定的元素，用法：

```
while element in list:
   list.remove(element)
```

49 形参与实参的理解，用法：

```
def method(username): # username形参
method('zhangsan') # zhangsan实参
```

50 位置参数，用法：

```
def describe(name,age):
describe('zhangsan',22) # 参数位置对应传递
```

51 关键字实参是传递函数的名称-值对，用法：

```
def describe(name,age):
describe(name='zhangsan',age=22) # 关键字实参
describe(age=22,name='zhangsan') # 关键字实参,位置不重要
```

52 形参设置默认值，用法：def describe(name='lisi',age):

53 返回值，用法：

```
def describe(name='lisi',age):
    des = name + str(age)
return des # 可以返回字典、列表等形式
```

54 列表参数，用法：

```
lists = ['huangsan','lisi','wangjun','denghui']
def cats_name(lists):
    for list in lists:
        print("'my love is :\t'+list".title())
```

55 传递任意参数，用法：def cats_name(*cats): # 可以传递多个形参

56 位置实参和任意数量实参：

```
def cats_name(parament1,parament2,*cats): # 可以传递多个形参
cats_name(para1,para2,para3,para4,...)
```

57 任意实参和关键字实参，用法：(cats.py)

```
def cats_name(parament1,parament2,**cats): # 可以传递多个形参
cats_name(para1,para2,para3,newname=para4,...)
```

58 导入整个模块，用法：

```
import cats
cats.cats_name(para1,para2,para3,newname=para4,...)
```

59 导入特定的函数,用法：from nltk import map_tag as mt

60 导入模块所有函数,用法：from nltk import *

61 形参默认时，两边不能为空，用法：def function_name(parament_0,parament_1='default')

62 类的命名是驼峰型即首字母大写。

63 **init**(self,papa1,para2):避免python默认方法跟普通方法名称冲突,self必不可少，必须位于其他形参的前面，指向实例本身。

64 类的继承，用法：

```
# 父类
class Animal():
    def __init__(self,name,age):
        self.name = name
        self.age = age

    def animal_call(self):
        print('this is '+self.name.title()+' call.')


# 子类
class Cat(Animal):
    def __init__(self,name,age,color):
        super().__init__(name,age)
        self.color =color

    def cat_color(self):
        my_color = 'the cat is '+self.color
        print(my_color)
        return my_color


if __name__ == '__main__':
    cat = Cat('tom',22,'blue')
    cat.animal_call()
    strs=cat.cat_color()
```

65 几种类的导入方式，用法：

```
from cat import Cat # 导入单个类
from cat import Animal,Cat # 导入多个类
from cat # 导入整个模块
from cat import * # 导入所有类
```

66 读取文本文件，并删除字符串始末空白，用法：my_str = line.strip()

67 opem()自动创建文件路径，若路径不存在时候。

68 异常代码块：try-except

69 split()创建单词列表

```
str = 'this is a string'
str.split()
['this','is','a','string']
```

70 存储数据json.dump()和json.load()

```
import json

# 父类
class Animal():
    def __init__(self,name,age):
        self.name = name
        self.age = age

    def animal_call(self):
        print('this is '+self.name.title()+' call.')


# 子类
class Cat(Animal):
    def __init__(self,name,age,color):
        super().__init__(name,age)
        self.color =color

    def cat_color(self):
        my_color = 'the cat is '+self.color
        print(my_color)
        return my_color


if __name__ == '__main__':
    cat = Cat('tom',22,'blue')
    cat.animal_call()
    strs=cat.cat_color()

    filename = r'../AllProject/V4.0EngInfoExtract/Document/EnPapers_single/test.json'
    with open(filename,'w') as f_obj:
        json.dump(strs,f_obj)

    with open(filename,'r') as f_obj:
        strs = json.load(f_obj)
    print(strs)
```

### 附加matplotlib相关操作：

71 matplotlib绘制图表，plot绘制折线图

```
import matplotlib
import matplotlib.pyplot as plt
#加入中文显示
import  matplotlib.font_manager as fm

# 解决中文乱码，本案例使用宋体字
myfont=fm.FontProperties(fname=r"C:\\Windows\\Fonts\\simsun.ttc")

def line_chart(xvalues,yvalues):
    # 绘制折线图,c颜色设置，alpha透明度
    plt.plot(xvalues,yvalues,linewidth=10,alpha=0.5,c='red') # num_squares数据值，linewidth设置线条粗细

    # 设置折线图标题和横纵坐标标题
    plt.title("Python绘制折线图",fontsize=30,fontname='宋体',fontproperties=myfont)
    plt.xlabel('横坐标',fontsize=20,fontname='宋体',fontproperties=myfont)
    plt.ylabel('纵坐标',fontsize=20,fontname='宋体',fontproperties=myfont)

    # 设置刻度标记大小,axis='both'参数影响横纵坐标，labelsize刻度大小
    plt.tick_params(axis='both',labelsize=14)

    # 显示图形
    plt.show()
```

72  matplotlib绘制图表，scatter绘制散点图

```
def scatter_chart(xvalues,yvalues):
    # 绘制散点图，s设置点的大小,c数据点的颜色，edgecolors数据点的轮廓
    plt.scatter(xvalues,yvalues,c='green',edgecolors='none',s=40)

    # 设置散点图标题和横纵坐标标题
    plt.title("Python绘制折线图",fontsize=30,fontname='宋体',fontproperties=myfont)
    plt.xlabel('横坐标',fontsize=20,fontname='宋体',fontproperties=myfont)
    plt.ylabel('纵坐标',fontsize=20,fontname='宋体',fontproperties=myfont)

    # 设置刻度标记大小,axis='both'参数影响横纵坐标，labelsize刻度大小
    plt.tick_params(axis='both',which='major',labelsize=10)

    # 设置每个坐标轴取值范围
    plt.axis([80,100,6400,10000])

    # 显示图形
    plt.show()

    # 自动保存图表,bbox_inches剪除图片空白区
    # plt.savefig('squares_plot.png',bbox_inches='tight')
```

73 Pygal生成可缩略的矢量图文件

```
def histogram(xvalues,yvalues):
    # 绘制直方图
    hist = pygal.Bar()

    # 设置散点图标题和横纵坐标标题
    hist.title = '事件频率的直方图'
    hist.x_title = '事件的结果'
    hist.y_title = '事件的频率'

    # 绘制气温图,设置图形大小
    fig = plt.figure(dpi=128,figsize=(10,6))

    # 事件的结果
    hist.x_labels = xvalues

    # 事件的统计频率
    hist.add('事件',yvalues)

    # 保存文件路径
    hist.render_to_file('die_visual.svg')
```

74 读取csv文件显示折线图

```
def temper_char():
    dates,highs,lows = [],[],[]
    with open(r'../../../AllProject/PyProject/weather07.csv') as f:
        reader = csv.reader(f)
        header_row = next(reader) # 返回文件第一行
        # enumerate 获取元素的索引及其值
        # for index,column_header in enumerate(header_row):
        #     print(index,column_header)
        for row in reader:
            current_date = datetime.strptime(row[0],"%Y-%m-%d")
            dates.append(current_date)
            highs.append(int(row[1]))
            lows.append((int(row[3])))


    # 接收数据并绘制图形,facecolor填充区域颜色
    plt.plot(dates,highs,c='red',linewidth=4,alpha=0.5)
    plt.plot(dates,lows,c='green',linewidth=4,alpha=0.5)
    plt.fill_between(dates,highs,lows,facecolor='blue',alpha=0.2)

    # 设置散点图标题和横纵坐标标题
    plt.title("日常最高气温，2018年7月",fontsize=24,fontname='宋体',fontproperties=myfont)
    plt.xlabel('横坐标',fontsize=20,fontname='宋体',fontproperties=myfont)
    plt.ylabel('温度',fontsize=20,fontname='宋体',fontproperties=myfont)

    # 绘制斜的日期
    fig.autofmt_xdate()

    # 设置刻度标记大小,axis='both'参数影响横纵坐标，labelsize刻度大小
    plt.tick_params(axis='both',which='major',labelsize=15)

    # 显示图形
    plt.show()
```

75 Github最受欢迎的星标项目可视化

```
def repos_hist():
    #查看API速率限制
    # url = https://api.github.com/rate_limit
    # 执行github API调用并存储响应
    url = 'https://api.github.com/search/repositories?q=language:python&sort=stars'
    r = requests.get(url)
    print("Status code:",r.status_code) # 状态码200表示成功

    # 将API响应存储在一个变量里面
    response_dict = r.json()
    print("Hithub总的Python仓库数：",response_dict['total_count'])

    # 探索有关仓库的信息
    repo_dicts = response_dict['items']

    names,stars = [],[]
    for repo_dict in repo_dicts:
        names.append(repo_dict['name'])
        stars.append(repo_dict['stargazers_count'])

    # 可视化,x_label_rotation围绕x轴旋转45度，show_legend图例隐藏与否
    my_style = LS(base_style=LCS)

    my_config = pygal.Config()
    my_config.x_label_rotation=45 # 横坐标字体旋转角度
    my_config.show_legend=False
    my_config.title_font_size=24 # 标题大小
    my_config.label_font_size=14 # 副标题大小，纵横坐标数据
    my_config.major_label_font_size = 18 # 主标签大小，纵坐标5000整数倍
    my_config.truncate_label=15  # 项目名称显示前15个字
    my_config.show_y_guides=False # 隐藏水平线
    my_config.width=1200 # 自定义宽度
    # chart = pygal.Bar(style=my_style,x_label_rotation=45,show_legend=False)
    chart = pygal.Bar(my_config,style=my_style)
    chart.title = 'Github最受欢迎的星标项目'
    chart.x_labels = names
    chart.add('星标',stars)
    chart.render_to_file('python_repos.svg')
```

###完整的matplotlib可视化

```
import matplotlib
import matplotlib.pyplot as plt

import pygal
from pygal.style import LightColorizedStyle as LCS, LightStyle as LS

import csv
from datetime import datetime

import requests

#加入中文显示
import  matplotlib.font_manager as fm
# 解决中文乱码，本案例使用宋体字
myfont=fm.FontProperties(fname=r"C:\\Windows\\Fonts\\simsun.ttc")

def line_chart(xvalues,yvalues):
    # 绘制折线图,c颜色设置，alpha透明度
    plt.plot(xvalues,yvalues,linewidth=10,alpha=0.5,c='red') # num_squares数据值，linewidth设置线条粗细

    # 设置折线图标题和横纵坐标标题
    plt.title("Python绘制折线图",fontsize=30,fontname='宋体',fontproperties=myfont)
    plt.xlabel('横坐标',fontsize=20,fontname='宋体',fontproperties=myfont)
    plt.ylabel('纵坐标',fontsize=20,fontname='宋体',fontproperties=myfont)

    # 设置刻度标记大小,axis='both'参数影响横纵坐标，labelsize刻度大小
    plt.tick_params(axis='both',labelsize=14)

    # 显示图形
    plt.show()


def scatter_chart(xvalues,yvalues):
    # 绘制散点图，s设置点的大小,c数据点的颜色，edgecolors数据点的轮廓
    plt.scatter(xvalues,yvalues,c='green',edgecolors='none',s=40)

    # 设置散点图标题和横纵坐标标题
    plt.title("Python绘制折线图",fontsize=30,fontname='宋体',fontproperties=myfont)
    plt.xlabel('横坐标',fontsize=20,fontname='宋体',fontproperties=myfont)
    plt.ylabel('纵坐标',fontsize=20,fontname='宋体',fontproperties=myfont)

    # 设置刻度标记大小,axis='both'参数影响横纵坐标，labelsize刻度大小
    plt.tick_params(axis='both',which='major',labelsize=10)

    # 设置每个坐标轴取值范围
    plt.axis([80,100,6400,10000])

    # 显示图形
    plt.show()

    # 自动保存图表,bbox_inches剪除图片空白区
    # plt.savefig('squares_plot.png',bbox_inches='tight')


def histogram(xvalues,yvalues):
    # 绘制直方图
    hist = pygal.Bar()

    # 设置散点图标题和横纵坐标标题
    hist.title = '事件频率的直方图'
    hist.x_title = '事件的结果'
    hist.y_title = '事件的频率'

    # 绘制气温图,设置图形大小
    fig = plt.figure(dpi=128,figsize=(10,6))

    # 事件的结果
    hist.x_labels = xvalues

    # 事件的统计频率
    hist.add('事件',yvalues)

    # 保存文件路径
    hist.render_to_file('die_visual.svg')

def temper_char():
    dates,highs,lows = [],[],[]
    with open(r'../../../AllProject/PyProject/weather07.csv') as f:
        reader = csv.reader(f)
        header_row = next(reader) # 返回文件第一行
        # enumerate 获取元素的索引及其值
        # for index,column_header in enumerate(header_row):
        #     print(index,column_header)
        for row in reader:
            current_date = datetime.strptime(row[0],"%Y-%m-%d")
            dates.append(current_date)
            highs.append(int(row[1]))
            lows.append((int(row[3])))


    # 接收数据并绘制图形,facecolor填充区域颜色
    plt.plot(dates,highs,c='red',linewidth=4,alpha=0.5)
    plt.plot(dates,lows,c='green',linewidth=4,alpha=0.5)
    plt.fill_between(dates,highs,lows,facecolor='blue',alpha=0.2)

    # 设置散点图标题和横纵坐标标题
    plt.title("日常最高气温，2018年7月",fontsize=24,fontname='宋体',fontproperties=myfont)
    plt.xlabel('横坐标',fontsize=20,fontname='宋体',fontproperties=myfont)
    plt.ylabel('温度',fontsize=20,fontname='宋体',fontproperties=myfont)

    # 绘制斜的日期
    fig.autofmt_xdate()

    # 设置刻度标记大小,axis='both'参数影响横纵坐标，labelsize刻度大小
    plt.tick_params(axis='both',which='major',labelsize=15)

    # 显示图形
    plt.show()


def repos_hist():
    #查看API速率限制
    # url = https://api.github.com/rate_limit
    # 执行github API调用并存储响应
    url = 'https://api.github.com/search/repositories?q=language:python&sort=stars'
    r = requests.get(url)
    print("Status code:",r.status_code) # 状态码200表示成功

    # 将API响应存储在一个变量里面
    response_dict = r.json()
    print("Hithub总的Python仓库数：",response_dict['total_count'])

    # 探索有关仓库的信息
    repo_dicts = response_dict['items']

    names,stars = [],[]
    for repo_dict in repo_dicts:
        names.append(repo_dict['name'])
        stars.append(repo_dict['stargazers_count'])

    # 可视化,x_label_rotation围绕x轴旋转45度，show_legend图例隐藏与否
    my_style = LS(base_style=LCS)

    my_config = pygal.Config()
    my_config.x_label_rotation=45 # 横坐标字体旋转角度
    my_config.show_legend=False
    my_config.title_font_size=24 # 标题大小
    my_config.label_font_size=14 # 副标题大小，纵横坐标数据
    my_config.major_label_font_size = 18 # 主标签大小，纵坐标5000整数倍
    my_config.truncate_label=15  # 项目名称显示前15个字
    my_config.show_y_guides=False # 隐藏水平线
    my_config.width=1200 # 自定义宽度
    # chart = pygal.Bar(style=my_style,x_label_rotation=45,show_legend=False)
    chart = pygal.Bar(my_config,style=my_style)
    chart.title = 'Github最受欢迎的星标项目'
    chart.x_labels = names
    chart.add('星标',stars)
    chart.render_to_file('python_repos.svg')

    # print('查看每个python仓库的信息：\n')
    # for repo_dict in repo_dicts:
    #     print('项目名称：',repo_dict['name'])
    #     print('所有者：',repo_dict['owner']['login'])
    #     print('星级评分：',repo_dict['stargazers_count'])
    #     print('项目URL：',repo_dict['html_url'])
    #     print('仓库描述：',repo_dict['description'])
    #     print('\n')

    # 研究第一个仓库
    # repo_dict = repo_dicts[0]
    # print('\nKey:',len(repo_dict))
    # for key in sorted(repo_dict.keys()):
    #     print(key)
    # 处理结果
    # print(response_dict.keys())



if __name__ == '__main__':
    xvalues = list(range(1,100)) #校正坐标点，即横坐标值列表
    yvalues = [x**2 for x in xvalues] # 纵坐标值列表

    x_result = [1,2,3,4,5,6]
    y_frequencies = [152,171,175,168,150,179]

    # line_chart(xvalues,yvalues)
    # scatter_chart(xvalues,yvalues)
    # histogram(x_result,y_frequencies)
    # temper_char()
    repos_hist()
```

### Numpy主要操作　

```
import numpy
from numpy import array
from numpy import mat,matrix
from numpy import shape  # 查看矩阵或数组的方法
from numpy import multiply # 元素相乘
import random


def nu_add():
    mm = array((1,1,1))
    pp = array((2,2,2))
    rr = mm + pp**3     # 数组的和运算
    rr1 = mm * pp       # 数组相乘
    print(rr)
    print(rr1)


def nu_matrix():
    ss = mat([1,2,3])     # 矩阵
    mm = matrix([1,2,3])
    print('an element: '.title()+str(mm[0,0]))   # 访问矩阵中的单个元素
    print('Number of dimensions of mm '.title()+str(shape(mm)))
    print('mat is equal matrix: '.title()+str(ss==mm))
    print('Matrix multiplication: '.title()+str(ss*mm.T))   # 矩阵相乘需要进行转置
    print('Multiplication of elements： '.title()+str(multiply(mm,ss))) # mm每个元素和ss每个元素相乘


def nu_list_mat():
    pylist = [1,2,3]
    rr = mat(pylist) # 列表转化成矩阵
    print('list values: '.title()+str(pylist))
    print('rr type: '.title()+str(type(rr)))
    print('mat values: '.title()+str(rr))


def nu_mean():
    dd = mat([4,5,1])
    rr = dd.mean()           # 矩阵的均值
    print('mean of dd: '.title()+ str(rr))


def nu_mul_array():
    jj = mat([[1,2,3],[8,8,8]])
    print('Number of dimensions of jj '.title()+str(shape(jj)))
    one_row = jj[1,0:2]
    print(one_row)

def nu_tran_mat():    # 矩阵转置
    radMat = numpy.random.random((3,3))
    print('Before matrix transposition:\n '+str(radMat))
    print('After matrix transposition:\n '+str(radMat.T))


def nu_inverse_mat():    # 矩阵的逆
    radMat = numpy.random.random((3,3))
    print('Before matrix inverse:\n '+str(radMat))
    print('After matrix inverse:\n '+str(mat(radMat).I))


def nu_mat_mul_imat(): # 矩阵与其逆矩阵相乘
    bmat = mat(numpy.random.random((3,3)))
    imat = bmat.I
    rus  = bmat * imat
    print(rus) # 结果是3*3的单位矩阵，其位置原则应该都是0，实际中是非常小的数，这个计算机处理的问题



if __name__ == '__main__':
    # nu_add()
    # nu_matrix()
    # nu_list_mat()
    # nu_mean()
    # nu_mul_array()
    # nu_tran_mat()
    # nu_inverse_mat()
    nu_mat_mul_imat()
```



作者：白宁超

源自：http://www.cnblogs.com/baiboy/p/pythonnotes1.html



