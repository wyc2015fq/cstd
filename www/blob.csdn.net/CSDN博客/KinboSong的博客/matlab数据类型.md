# matlab数据类型 - KinboSong的博客 - CSDN博客
2018年03月29日 08:24:40[songjinbo3](https://me.csdn.net/KinboSong)阅读数：115
# Matlab的数据类型
                原创                
2016年01月25日 16:31:27
```
<ul class="article_tags clearfix csdn-tracking-statistics tracking-click" data-mod="popu_377">
            <li class="tit">标签：</li>
```
[matlab](http://so.csdn.net/so/search/s.do?q=matlab&t=blog)/[编程语言](http://so.csdn.net/so/search/s.do?q=%E7%BC%96%E7%A8%8B%E8%AF%AD%E8%A8%80&t=blog)/
- 
- 7471
- 
[编辑](https://mp.csdn.net/postedit/50574869)
- 
删除
Matlab在工程中有着非常重要的作用，好多复杂的计算和变换都能通过matlab解决，所以作为一名计算机软件的从业者，我们应该对Matlab有所了解，这是我学习Matlab时所整理的知识点，跟大家分享一下。
Matlab的数据类型与我们平常用的编程语言稍有不同，但是语言都是想通的，有了其他语言的编程基础，Matlab上手会很快。
一、数值类型
**1、实数**
Matlab的数值类型分为三种，整形，单精度浮点型和双精度浮点型，具体的情况如下。
![这里写图片描述](https://img-blog.csdn.net/20160124193537652)
有几个地方需要注意一下：
- 系统默认的数值类型是double的，所以使用double时可以省略double符号
- 
double类型的数值与其他类型的数值运算时，结果为其他类型的数值，single类型的数值不能和整型直接运算，各种不同位数的整形之间也不能直接运算。
因为系统默认的数据类型是双精度浮点，所以在将变量设置为整数时需要进行转换，matlab提供了如下的转化函数。
|函数|运算法则|例子|
|----|----|----|
|floor|向下取整|floor(1.4)=1;floor(3.5)=3;floor(-3.5)=-4|
|ceil|向上取整|ceil(1.4)=2;ceil(3.5)=3;ceil(-3.5)=-3|
|round|取最接近的整数，如果小数部分是0.5，则向绝对值大的方向取整|round(1.4)=1;round(3.5)=4;round(-3.5)=-4|
|fix|向0取整|fix(1.4)=1;fix(3.5)=3;fix(-3.5)=-3|
**2、复数**
复数由实部和虚部两部分构成，在Matlab中，字符i或j默认作为虚部标志，关于复数的相关函数如下：
|函数|说明|函数|说明|
|----|----|----|----|
|complex(a,b)|构造以a为实部，以b为虚部的复数|i,j|虚部标识|
|real(z)|返回复数z的实部|image(z)|返回复数z的虚部|
|abs(z)|返回复数z的模|angle(z)|返回复数z的辅角|
|conj(z)|返回复数z的共轭复数|||
**3、无穷量（Inf）和非数值量（NaN）**
Matlab中使用Inf和-Inf分别表示正无穷大和负无穷大，NaN表示非数值量。正负无穷大一般是由于运算溢出产生，非数值量则是由于类似0/0或Inf/Inf类型的非正常运算产生。
二、字符与字符串
Matlab将文本作为特征字符串或简单地当做字符串，但是实际存储的是字符串的ASCII码。abs函数可以获得字符串的具体值，使用方法如下：
```
>> a='abc'
a =
abc
>> abs(a)
ans =
    97    98    99
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
三、结构
Matlab中也存在与C语言类似的结构这种类型，一个结构可以通过字段存储多个不同类型的数据。结构中的各个字段可以是不同的类型。
**1、创建结构对象**
有两种创建结构的方法，一种是逐一输入一个结构的各个属性，例如下面创建stu1的过程；另一种是使用struct函数，例如创建stu2的过程。
```
>> stu1.name='xing';
>> stu1.sno='123';
>> stu1
stu1 = 
    name: 'xing'
     sno: '123'
>> stu2=struct('name','sno','xiao','234')
stu2 = 
    name: 'sno'
    xiao: '234'
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
**2、访问结构对象**
使用结构名.属性名的方式可以获取结构的属性。
```
>> stu1.name
ans =
xing
```
- 1
- 2
- 3
- 4
- 5
**3、连接结构对象**
两个结构可以连接起来，但是要连接的这个两个结构的对应属性的类型应该相同。
```
>> stu1.name='xing';
>> stu1.sno=int16(123);
>> stu2.name='xiao';
>> stu2.sno=int16(456);
>> stu=[stu1,stu2]
stu = 
1x2 struct array with fields:
    name
    sno
>> stu(1)
ans = 
    name: 'xing'
     sno: 123
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
四、单元数组
单元数组是一种广义矩阵。每一个单元可以包括一个任意数组，如数值数组、 
字符数组、结构体数组或另外一个单元数组，因而每一个单元可以具有不同的尺寸和内存。
**1、创建单元数组**
有两种创建单元数组的方法：通过赋值语句或cell函数创建。
（a）使用赋值语句创建单元数组：单元数组使用花括号”{} ”来创建，使用“，”或空格来分隔单元，使用“；”来分行。
```
>> A={'x',[2;3;6];10,2*pi}
A = 
    'x'     [3x1 double]
    [10]    [    6.2832]
```
- 1
- 2
- 3
- 4
- 5
- 6
（b）使用cell函数创建空单元数组
```
>> B=cell(2,2)
B = 
    []    []
    []    []
```
- 1
- 2
- 3
- 4
- 5
- 6
**2、访问单元数组**
在单元数组中，单元和单元中的内容属于不同范畴，这意味着访问单元和访问单元中的内容是两个不同的操作。对于单元数组C，C（m，n）指的是单元数组中第m行第n列的单元，而C{m,n}则是指的是第m行第n列的单元中的内容。
```matlab
>> A(1,1)
ans = 
    'x'
>> A(1,2)
ans = 
    [3x1 double]
>> A{1,2}
ans =
     2
     3
     6
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
**3、单元数组的合并**
```
>> C={'a','b';'c','d'}
C = 
    'a'    'b'
    'c'    'd'
>> D={'1','2'}
D = 
    '1'    '2'
>> E={A;B}
E = 
    {2x2 cell}
    {2x2 cell}
>> E{1,1}
ans = 
    'x'     [3x1 double]
    [10]    [    6.2832
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
五、映射容器
Matlab中的映射容器就是Map类，这里的Map类与java的Map类类似，也是一个key对应着一个value，Map类的所有对象都有三个属性：
- Count：表示Map中存储的键值对的数量
- KeyType：表示Map对象中包括的key的类型
- ValueType：表示Map中的数据类型
**1、创建Map对象**
```
>> mapObj=containers.Map({1,2},{'xing','xiao'})
mapObj = 
  Map with properties:
        Count: 2
      KeyType: double
    ValueType: char
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
**2、查看和读取Map对象**
可以通过使用keys函数查看Map对象中包含的所有键；通过values函数查看所有的值。
```matlab
%获取所有的键
>> keys(mapObj)
ans = 
    [1]    [2]
%获取所有的值
>> values(mapObj)
ans = 
    'xing'    'xiao'
%获取指定键的值
>> mapObj(1)
ans =
xing
%获取多个值
>> values(mapObj,{1,2})
ans = 
    'xing'    'xiao'
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
**3、编辑Map对象**
（1）添加键值对
```
>> mapObj(3)='ming'
mapObj = 
  Map with properties:
        Count: 3
      KeyType: double
    ValueType: char
>> values(mapObj)
ans = 
    'xing'    'xiao'    'ming'
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
（2）删除键值对
```matlab
>> remove(mapObj,2)
ans = 
  Map with properties:
        Count: 2
      KeyType: double
    ValueType: char
>> values(mapObj)
ans = 
    'xing'    'ming'
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
（3）修改键值对
如果需要在值不变的情况下对键名进行修改，首先要删除键名和对应的值，然后再添加一个有正确键名的新条目。通过覆盖原有的值，可以在键名不变的情况下对值进行修改。
```
>> remove(mapObj,1)
ans = 
  Map with properties:
        Count: 1
      KeyType: double
    ValueType: char
>> mapObj(11)='xing'
mapObj = 
  Map with properties:
        Count: 2
      KeyType: double
    ValueType: char
>> mapObj(3)='aaa'
mapObj = 
  Map with properties:
        Count: 2
      KeyType: double
    ValueType: char
>> keys(mapObj)
ans = 
    [3]    [11]
>> values(mapObj)
ans = 
    'aaa'    'xing'
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
六、数据类型之间的转换
类型识别函数：
|函数|功能|函数|功能|
|----|----|----|----|
|iscell|判断是否为单元数组|isnumeric|判断是否为数值数组|
|isreal|判断是否为实数|ischar|判断是否为字符数组|
|isstr|判断是否为字符串|isfloat|判断是否为浮点数|
|isstruct|判断是否为结构数组|whos|列举变量类型及相关数据|
类型转换函数：
|函数|功能|函数|功能|
|----|----|----|----|
|dec2bin|十进制转为二进制|int2str|整数转为字符串|
|num2str|数值转为字符串|str2double|字符串转为double|
还有其他很多的转换函数，基本上都是类似的语法。
        $(".MathJax").remove(); 
