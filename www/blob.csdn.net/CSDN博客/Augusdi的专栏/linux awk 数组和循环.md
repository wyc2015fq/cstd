
# linux awk 数组和循环 - Augusdi的专栏 - CSDN博客


2015年04月22日 17:54:06[Augusdi](https://me.csdn.net/Augusdi)阅读数：4786


[linux awk 数组和循环](http://blog.csdn.net/ithomer/article/details/8478716)
**awk**作为强大的文本处理工具，少不了数组处理。
awk 中数组叫做关联数组(associative arrays)，下标可以是数字也可以是字符串。awk 中的数组不必提前声明，也不必声明大小，初始化数组元素用 0 或空串，这根据上下文而定。

**一 语法**
语法：awk '{pattern + action}'    或   awk 'pattern {action}'
其中 pattern 表示 AWK 在数据中查找的内容， action 是在找到匹配内容时所执行的一系列命令。花括号 {} 不需要在程序中始终出现，但它们用于根据特定的模式对一系列指令进行分组（作用域）。


**二 数组定义**
**1 一维数组**
a） 数字下标
array[1]="it"
array[2]="homer"
array[3]="sunboy"
array[4]=2050

b） 字符下标
array["first"]="yang"
array["second"]="gang"
array["third"]="sunboy"

**示例 1：**

```python
#!/bin/bash
awk 'BEGIN{
array[1]="it"
array[2]="homer"
array[3]="sunboy"
array[4]=2050

array["first"]="yang"
array["second"]="gang"
array["third"]="sunboy"

print array[1], array[4]
print array[3], array["third"]}'
```
结果：
it    2050
sunboy    sunboy

**示例 2：**

```python
#!/bin/bash
awk 'BEGIN{
   for(i=1; i<=5; i++){
       array[i] = i*2 - 1;
   }
   for(i in array){
       print i" = " array[i];
   }
}'
```
结果：
4 = 7
5 = 9
1 = 1
2 = 3
3 = 5
注： for in 输出数组元素顺序是不定的，下面介绍对数组如何排序


**2 二维数组**
awk 多维数组在本质上是一维数组，因awk在存储上并不支持多维数组，awk提供了逻辑上模拟二维数组的访问方式。例如，array[2,3] = 1这样的访问是允许的。
awk使用一个特殊的字符串SUBSEP (\034)作为分割字段，在上面的例子array[2,3] = 1中，关联数组array存储的键值实际上是2\0343，2和3分别为下标（2，3），\034为SUBSEP分隔符
类似一维数组的成员测试，多维数组可以使用 if ( (i,j) in array) 语法，但是下标必须放置在圆括号中。
类似一维数组的循环访问，多维数组使用 for ( item in array ) 语法遍历数组。与一维数组不同的是，多维数组必须使用split()函数来访问单独的下标分量，格式： split ( item, subscr, SUBSEP)，例如： split (item, array2, SUBSEP); 后，array2[1]为下标“2”， array2[2]为下标“3”
**示例：**

```python
#!/bin/bash
awk 'BEGIN{
   for(i=1; i<=3; i++){
       for(j=1; j<=3; j++){
           array[i, j] = i * j;
           print i" * "j" = "array[i,j];
       }
   }
   print
   for(i in array){
       split(i, array2, SUBSEP);
       print array2[1]" * "array2[2]" = " array[i];
   }
}'
```
结果：
1 * 1 = 1
1 * 2 = 2
1 * 3 = 3
2 * 1 = 2
2 * 2 = 4
2 * 3 = 6
3 * 1 = 3
3 * 2 = 6
3 * 3 = 9
2 * 1 = 2
2 * 2 = 4
2 * 3 = 6
3 * 1 = 3
3 * 2 = 6
3 * 3 = 9
1 * 1 = 1
1 * 2 = 2
1 * 3 = 3
注： 示例中 split(i, array2, SUBSEP); 即是把二维数组作为一维数组处理，同样数组元素顺序不确定，下面将介绍数组排序


**三 数组函数**
**1） 数组长度**（length）
length(array) 获取数组长度， split 分割数组也返回数组长度，示例：

```python
#!/bin/bash
awk 'BEGIN{
    info="it is  a   test"; 
    len = split(info, array, " "); 
    
    print len, length(array);
    print
    for(i in array){
        print i" = " array[i];
    }
}'
```
结果：
4    4
4 = test
1 = it
2 = is
3 = a

**2） 数组排序**（asort）
asort对数组array按照首字母进行排序，返回数组长度；
如果要得到数组原本顺序，需要使用数组下标依次访问；
for...in 输出关联数组的顺序是无序的，所以通过for…in 得到是无序的数组。如果需要得到有序数组，需要通过下标获得
**示例：**

```python
#!/bin/bash
awk 'BEGIN{
    info="it is  a   test"; 
    len = split(info, array, " "); 
    
    print len, length(array);
    print "--- for in ---"
    for(i in array){
        print i" = " array[i];
    }
    print "--- for ---"
    for(i=1; i<=len; i++){
        print i" = "array[i];
    }

    print
    print "--- asort ---"
    print "asort(array) = ", asort(array);
    
    print
    print "--- for in ---"
    for(i in array){
        print i" = " array[i];
    }
    print "--- for ---"
    for(i=1; i<=len; i++){
        print i" = "array[i];
    }
}'
```
结果：
4 4
--- for in ---
4 = test
1 = it
2 = is
3 = a
--- for ---
1 = it
2 = is
3 = a
4 = test
--- asort ---
asort(array) =  4
--- for in ---
4 = test
1 = a
2 = is
3 = it
--- for ---
1 = a
2 = is
3 = it
4 = test

**3） 键值操作**
a 查找键值（in）
awk 'BEGIN{array["a"]="aaa"; array["b"]="bbb"; if(array["c"]!="ccc"){print "no found";}; for(k in array){print k, array[k];}}'
结果：
no found
a aaa
b bbb
c
注： array[“c”]没有定义，但是循环时存在该键值，它的值为空。这是因为awk数组是关联数组，只要通过数组引用它的key，就会自动创建改序列

正确做法是用： in
awk 'BEGIN{array["a"]="aaa"; array["b"]="bbb"; if("c"inarray){print "found";}else{print "not found"}; for(k in array){print k, array[k];}}'
结果：
not found
a aaa
b bbb
注： 没有引用array下标“c”，因此没有添加到数组中

b 删除键值（delete）
awk 'BEGIN{array["a"]="aaa"; array["b"]="bbb";deletearray["a"]; for(k in array){print k, array[k];}}'
结果：b bbb



**四 循环控制语句**
linuxawk中的流程控制语句和语法结构，与c语言类型。
awk 的 while、do-while、for语句中允许使用break、continue语句来控制流程走向，也允许使用exit这样的语句来退出，其中break中断当前正在执行的循环并跳到循环外执行下一条语句；if 是流程选择用法。
**1） if-else if 语句**

```python
#!/bin/bash
awk 'BEGIN{
    test = 80;
    if(test >= 90){
        print "good";
    }else if(test >= 80){
        print "soso";
    }else{
        print "fail";
    }
}'
```
结果：soso
**2） for 语句**

```python
#!/bin/bash
awk 'BEGIN{
    for(i=1; i<=3; i++){
        array[i] = i*i;
        print i" = "array[i];
    }
    print
    for(i=1; i<=length(array); i++){
        if(array[i] > 5){  # larger 5 then break
            break;
        }
        print i" = "array[i];
    }
}'
```
结果：
1 = 1
2 = 4
3 = 9
1 = 1
2 = 4

**3） while 语句**

```python
#!/bin/bash
awk 'BEGIN{
    test = 100;
    total=0;
    while(i<=test){
        total+=i;
        i++;
    }
    print "total = ", total;

    print
    test=100;
    total=0;
    i=0;
    do{
        total+=i;
        i++;
    }while(i<=test);
    print "total = ", total;
}'
```
结果：
total =  5050
total =  5050

以上为awk流程控制语句，从语法上与c语言是一样的。有了这些语句，其实很多shell程序都可以交给awk，而且性能是非常快

**跳转关键字**
|break|当|break|语句用于|while|或|for|语句时，导致退出程序循环。|
|continue|当|continue|语句用于|while|或|for|语句时，使程序循环移动到下一个迭代。|
|next|能能够导致读入下一个输入行，并返回到脚本的顶部。这可以避免对当前输入行执行其他的操作过程。|
|exit|语句使主输入循环退出并将控制转移到END,如果END存在的话。如果没有定义END规则，或在END中应用exit语句，则终止脚本的执行。|


**性能比较**
1） awk
time (awk 'BEGIN{ total=0; for(i=0; i<=100000; i++){total+=i;} print total;}')
结果：
5000050000
real0m0.035s
user0m0.020s
sys0m0.016s

2） sed
time(total=0; for i in $(seq 100000); do total=$(($total+i)); done; echo $total;)
结果：
5000050000
real0m0.976s
user0m0.672s
sys0m0.292s

测试100000累加，实现相同功能，awk实现的性能是shell 的约**30**倍



**参考推荐：**
[awk 实例](http://www.ibm.com/developerworks/cn/linux/shell/awk/awk-1/index.html)（IBM）
[沉于思考，默默学习](http://www.cnblogs.com/chengmo/tag/awk/)
[linux awk 内置函数实例](http://blog.csdn.net/sunboy_2050/article/details/8466007)
[linux awk 内置变量实例](http://blog.csdn.net/sunboy_2050/article/details/8477842)
[http://blog.csdn.net/ithomer/article/details/8478716](http://blog.csdn.net/ithomer/article/details/8478716)
﻿﻿

