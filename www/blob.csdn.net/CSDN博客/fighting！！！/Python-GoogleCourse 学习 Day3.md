# Python-GoogleCourse 学习 Day3 - fighting！！！ - CSDN博客
2019年02月23日 22:45:50[dujiahei](https://me.csdn.net/dujiahei)阅读数：15
## 参考：[http://www.runoob.com/python3/python3-set.html](http://www.runoob.com/python3/python3-set.html)
[https://www.liaoxuefeng.com/wiki/0014316089557264a6b348958f449949df42a6d3a2e542c000/001431676242561226b32a9ec624505bb8f723d0027b3e7000](https://www.liaoxuefeng.com/wiki/0014316089557264a6b348958f449949df42a6d3a2e542c000/001431676242561226b32a9ec624505bb8f723d0027b3e7000)
## 知识点
- dict 字典
	- 定义
		字典的元素是由一对对键值对组成，每一对之间用逗号隔开，将所有的键值对用花括号括起来就构成了字典。它是无序的，键和值之间用冒号隔开。
		语法格式：dic_name={key:value,key:value,key:value}
		简单来说：
		　　1.类似于json,也就是key-value键值对
		　　2.不允许同一个键值出现两次，所以一般对字典的操作都是对键进行操作。
		　　3.键必须不可变，所以可以用数字，字符串，元组充当，而不能用列表
		例如：1 dic={"key1":"value1","key2":"value2","key3":"value3"} #定义字典​​
- 创建
		d0=dict() #创建空字典
		d1=​​{“key1”:"value1", "key2":"value2", "key3":"value3"}
- 字典的方法
		- 复制
- 查找、更新
- set 集合
	set和dict类似，也是一组key的集合，但不存储value。由于key不能重复，所以，在set中，没有重复的key。
	集合（set）是一个无序的不重复元素序列。
	可以使用大括号 { } 或者 set() 函数创建集合，注意：创建一个空集合必须用 set() 而不是 { }，因为 { } 是用来创建一个空字典。​
	- 特性
		- 有的可变，有的不可变
- 无序：集合没有索引，它的元素无次序，不是序列。
- 不重复性：集合中的元素不能重复，可作为一种简单高效的元素去重方式。
- 创建
		parame = {value01,value02,...}
		或者
		set(value)
		- 不可变集合
			利用set()和{}建立集合时，要求集合中的元素必须是可哈希(hsshable)的，即在利用set()和{}创建集合的时候，集合中的元素必须是不可变的。
- 可变集合
			利用set()创建的集合是可变集合，它的类型是不可哈希(unhashable)的。对于这句话的理解是，set()创建的集合，整体上是可变的，可以增、删；但集合中的元素(个体)是不可变(hashable)的，不能被修改，且集合中的元素不能是列表、字典等可变类型的对象。
- 方法
		- 添加元素
			set1.add( x )
			set1.update( x )​
- 移除元素
			s.remove( x )---将元素 x 从集合 s 中移除，如果元素不存在，则会发生错误。
			​s.discard( x )---移除集合中的元素，且如果元素不存在，不会发生错误。
			​s.pop() ---随机删除集合中的一个元素
- 统计个数
			len(s)
- 清空集合
			s.clear()
- 判断元素是否在集合中存在
			x in s
- 集合内置方法完整列表
			
![](https://img.mubu.com/document_image/20fb61c1-d96d-4005-9ff6-02f90ef66e26-2329912.jpg)
- 判断语句（要求掌握多条件判断）
	python中没有switch case语句这种写法，使用if 条件判断。
	- if 语句
		- 一般形式
			if condition_1:
			statement_block_1
			elif condition_2:
			statement_block_2
			else:
			statement_block_3
			如果 "condition_1" 为 True 将执行 "statement_block_1" 块语句
			如果 "condition_1" 为False，将判断 "condition_2"
			如果"condition_2" 为 True 将执行 "statement_block_2" 块语句
			如果 "condition_2" 为False，将执行"statement_block_3"块语句
- 注意点
			Python 中用 elif 代替了 else if，所以if语句的关键字为：if – elif – else。
			​1、每个条件后面要使用冒号 :，表示接下来是满足条件后要执行的语句块。
			2、使用缩进来划分语句块，相同缩进数的语句在一起组成一个语句块。
			3、在Python中没有switch – case语句。
- if 嵌套
		在嵌套 if 语句中，可以把 if...elif...else 结构放在另外一个 if...elif...else 结构中。
		形式：
		if 表达式1:
		语句
		if 表达式2:
		语句
		elif 表达式3:
		语句
		else:
		语句
		elif 表达式4:
		语句
		else:
		语句​​
- 三目表达式
	一般支持三目运算符的语言（如C语言）的语法格式一般是这样的：判断条件（返回布尔值）？为真时的结果：为假时的结果，例如：x = x%2==1 ? x+1:x;
	python并不支持? :这种符号所表达的先判断再选择的含义，但仍然有相应的语法支持：为真时的结果 if 判断条件 else 为假时的结果（注意，没有冒号），例如：x = x+1 if x%2==1 else x​
- 循环语句
	- for...in 循环：for...in循环，依次把list或tuple中的每个元素迭代出来
		for x in ...循环就是把每个元素代入变量x，然后执行缩进块的语句。
		如果循环的变量是比较大的范围，可以借助range() 生成一个整数序列，再通过list（）函数转换为list。​
- while 循环
		只要条件满足，就不断循环，条件不满足时退出循环。
		在循环内部变量n不断自减，直到变为-1时，不再满足while条件，循环退出。​
- break
		在循环中，break语句可以提前退出循环。
		break的作用是提前结束循环。​
- continue
		在循环过程中，也可以通过continue语句，跳过当前的这次循环，直接开始下一次循环。
		continue的作用是提前结束本轮循环，并直接开始下一轮循环。​
- 小结
		循环是让计算机做重复任务的有效的方法。
		break语句可以在循环过程中直接退出循环，而continue语句可以提前结束本轮循环，并直接开始下一轮循环。这两个语句通常都必须配合if语句使用。
		要特别注意，不要滥用break和continue语句。break和continue会造成代码执行逻辑分叉过多，容易出错。大多数循环并不需要用到break和continue语句，上面的两个例子，都可以通过改写循环条件或者修改循环逻辑，去掉break和continue语句。
		有些时候，如果代码写得有问题，会让程序陷入“死循环”，也就是永远循环下去。这时可以用Ctrl+C退出程序，或者强制结束Python进程。
## 代码练习
1.创建一个2019年法定节假日字典，要求键为节日名称，值为放假天数，除“国庆节”，“春节”放假天数为7，其 余均默认为3。检查所有法定节假日放假天数，发现劳动节只放假1天，做出修改。遍历字典，将每个假期放假天数一一对应，打印出来。
```python
# for 循环建立 法定节假日字典 
import operator
valueList=["元旦","春节","清明节","劳动节","端午节","中秋节","国庆节"]
dictDay = dict() #创建空字典
for key in valueList:
    if (operator.eq(key,"春节") | operator.eq(key,"国庆节")):
        dictDay[key]=7
    else:
        dictDay[key]=3
print(dictDay)
# 但是劳动节只放1天。。。。修改
dictDay["劳动节"]=1
print(dictDay)
# 遍历字典，将每个假期放假天数一一对应，打印出来。
for key in dictDay:
    print(key,dictDay[key])
```
![](https://img-blog.csdnimg.cn/20190223224806605.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2R1amlhaGVp,size_16,color_FFFFFF,t_70)
