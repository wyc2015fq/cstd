# python入门——练习题 - wsp_1138886114的博客 - CSDN博客





2018年05月20日 11:29:32[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：769
所属专栏：[python基础及相关](https://blog.csdn.net/column/details/23328.html)









注：部分 print 未带括号为 python 2.x 版本。

### 一、你好，世界

```python
print('--------------------你好，世界-----------------------')
    print '你好，世界'


    print('------------输出字典遇到的中文问题处理----------------')
    import json
    dict1 = {'School':'zhoubei','Name': '张三', 'Age': 7, 'Class': '第一班'}
    print dict1['School']
    print json.dumps(dict1).decode('unicode-escape')
```

### 二、if /while 循环

```python
print('-------输入三个数字，是否能组成三角形，并判断是什么三角形--------')
	n1 = int(raw_input('请输入第一个整数数字：'))
	n2 = int(raw_input('请输入第二个整数数字：'))
	n3 = int(raw_input('请输入第三个整数数字：'))

	if n1 + n2> n3 and n2 + n3> n1 and n1 + n3> n2:        #两边之和大于第三边
		if n1 == n2 and n2 == n3:
			print ('这是个等边三角形')
		elif n1 == n2 or n2 == n3 or n1 == n3:
			print ('这是个等腰三角形')
		elif n1**2+n2**2 == n3**2 or n2**2 + n3**2 == n1**2 or n1**2 + n3**2 == n2**2:    #符合勾股定理为直角三角形
			print ('这是个直角三角形')
		else:
			print ('这是个普通的三角形')
	else:
		print ('无法组成三角形：')
```

```python
print('-----------------与电脑猜拳-------------------')
	import random
	import json
	dict_c = {1:'石头',2:'剪刀',3:'布'}
	dict_u =  {'石头':1,'剪刀':2,'布':3}
	user = raw_input('请输入：(只能是石头/剪刀/布)')
	user1 = dict_u[user]
	computer = random.randint(1,3)
	c = dict_c[computer]

	if user != '石头' and user != '剪刀' and user != '布':
		print('你又调皮了，只能输入石头/剪刀/布')
	else:
		print('电脑出：')
		print json.dumps(c).decode('unicode-escape')

		print("输赢结果：")
		if user == c:
			print('平手')
		elif user1 == 1 and computer == 3:
			print('SB，你输啦')
		elif user1 == 2 and computer == 1:
			print('SB，你输啦')
		elif user1 == 3 and computer == 2:
			print('SB，你输啦')
		else:
			print('你走狗屎运了')
```

```python
print('-----------------输出100以内的素数-------------------')
	n=2
	while(n<100):
		i = 2                    #i从2开始
		while(i<=10):
			if n%i == 0:
				break
			i+=1
		if (i>n/i):
			print n
		n+=1
```

### 三、for 循环

```python
#公鸡3元/只，母鸡5元/只，小鸡每元3只
	print('-----------------100元买100只鸡-------------------')
	for x in range(33):                  #100元内，公鸡数不能超过33只
		for y in range(20):              #同理，母鸡不能超过20只
			for z in range(100):         #同理，小鸡不会超过100只
				if 3*x+5*y+(1.0/3)*z ==100 and x+y+z==100:
					print x,y,z
```

```python
print('--------------输出1-9,0-----------------')
	list1 = range(10)
	list1.append(list1.pop(0))
	print list1
```

```python
#一对兔子每月生一对兔子，刚出生的兔子两个月后才能生育
	print('--------------21个月兔子数量-----------------')
	num1 = 2
	num2 = 2
	sum = 0
	for i in range(1,21):
		print "第",i,"个月"        
		num1 = num2
		num2 = sum
		sum = num1+num2
		print "有", sum, "个兔子"      #后一个月兔子的数量为前两个月兔子数量之和
	print "一共有",sum,"个兔子"
```

```python
小明有一美元要买（1-100美分）东西，求出找零最少的情况
        print('--------------找零钱-----------------')
	buy = input('请输入买糖的价格1-100：')
	count = 0
	for a in range(0,5):                    #25美分面额的 不能超过4张  （25×5>100）        
		for b in range(0,3):                #10美分面额的 不能超过2张  （10×3>25）这时可以给一张25美分面额的替代了
			for c in range(0,2):            # 5美分面额的 不能超过1张  （5×2=10） 这时可以用一张10美分面额的替代了
				for d in range(0,5):        #1美分的 不能超过4张   （1×5=5） 这时可以给一张5 美分面额的替代
					if a*25+b*10+c*5+d*1+buy == 100 and 10*b+5*c!=25 and 5*d+c!=10:
						count = a + b + c + d
						print '找零%d个25美分 %d个10美分 %d个5美分 %d个1美分'%(a, b, c, d)
						print '一共', count, '个硬币'
```

```python
print('-------传入列表的长度大于2，保留前两个长度的内容--------')
	def myfunction_3(str2):
		if len(str2)>2:
			del str2[2:]            #删除第二个后面的所有元素
			return str2
		else:
			print('列表小于2')
	str2 = [3,4,5]
	myfunction_3(str2)
	print str2
```

```python
print('---------乘法口诀表-------------')
    for i in range(1,10):
        for j in range(1,i+1):
            if i*j<10:
                print i, '×', j, '=', i * j, ' ',
            else:
                print i,'×',j ,'=', i*j,'',
        print ''
```

### 四、函数

```python
print('-------输出不定个数的数字的和--------')
	sum_b = 0
	sum_c = 0
	sum_d = 0
	def functionC(x,y,*args,**kwargs):
		global sum_d
		sum_b = x+y
		sum_c= sum(args)        #求和函数sum()必须传入可迭代对象
		sum_d = sum_c+sum_b
		return sum_d
	functionC(1,2,3,4,5,6,7,8,9,10)
	print sum_d
```

```python
print('-------模拟range（）全功能--------')
	list_input = [2,9]                #手动调制输入的range()
	def range_1(*args):
		if len(list_input)==1:    #输入一个数字时
			x0 = list_input[0]
			list_output = []
			i = 0
			while x0>1 and i<x0:
				list_output.append(i)
				i+=1
			else:
				print '输入的数字必须大于1'
			return list_output
		elif len(list_input)==2:  #输入二个数字时
			x0  = list_input[0]
			x1 = list_input[1]
			list_output = []
			i = x0
			if x0<x1:
				while x1>0 and i<x1:
					list_output.append(i)
					i+=1
				return list_output
			else:
				print('输入错误')
		elif len(list_input)==3:  #输入三个数字时
			x0  = list_input[0]
			x1 = list_input[1]
			x3 = list_input[2]
			list_output = []
			i = x0
			if x0<x1 and x3<x1:
				while x1>1 and i<x1:
					list_output.append(i)
					i+=x3
				return list_output
			else:
				print('输入错误')
		else:
			print '你又调皮了，最多只能输入三个'

	print range_1(list_input)
```

```python
print('-------遍历计算字符串的长度--------')
	list_a = []
	j = 0
	def str_x(str_a):
		for i in str_a:
			global j            #使用全局变量计数
			list_a.append(i)
			j+=1
		return j
	str_a =list(raw_input('请输入字符串：'))
	str_x(str_a)
	print j
```

```python
print('-------计算传入数字的各个位数之和--------')
	def item_sum(x):
		if str1<0:                #输入的数小于零时
			abs(str1)         #取绝对值
			list_sum = []
			str2 = str(x)
			for item in str2:
				list_sum.append(int(item))
		elif str1>0:
			list_sum = []
			str2 = str(x)
			for item in str2:
				list_sum.append(int(item))
			return sum(list_sum)
		else:
			return 0
	str1 = 3418291283            #传入数字转成字符串 / 传入数字型字符串
	print item_sum(str1)
```

```python
print('-----------互不相同且无重复数字的三位数-------------')
	count = 0
	list_b = 0
	def third_num(list_a):
		list_len = []
		global count
		for i in list_a:
			for j in list_a:
				for k in list_a:
					if i!=j and j!=k and i!=k:
						list_b =str(i)+str(j)+str(k)
						list_len.append(list_b)
						count+=1
		return list_len

	list_a = [1,2,3,4]
	print third_num(list_a)
	print '三位数个数：',count
```

```python
print('-----------一个加上100和加上268后都是一个完全平方数的数-------------')
	def square_num(x):
		num_y = 0
		num_x = 0
		for num_2 in range(200):
			for i in range(2,500):                #随便设定一个阈值
				for j in range(2,500):
					if (num_2 + 100) == i ** 2:
						num_x = num_2
						if (num_2+268)==j**2:
							num_y = num_2
							if num_x==num_y:
								return num_2
	num_2 = range(100)
	print square_num(num_2)
```

```python
print('-----------判断这一天是这一年的第几天------------------')
	def day_in_year(year_n,month_n,day_n):
		run = [31,29,31,30,31,30,31,31,30,31,30,31]        #闰年，二月份天数不能大于29
		ping = [31,28,31,30,31,30,31,31,30,31,30,31]        #非闰年，二月份天数不能大于28
		if year_n>= 1 and 12>=month_n>= 1 and 32>day_n>= 1:
			if year_n % 4 == 0 and year_n % 100 != 0 or year_n % 400==0:
				day_n = sum(run[:month_n-1])+day_n
				return day_n
			else:
				day_n = sum(ping[:month_n-1])+day_n
				return day_n
		else:
			print '输入格式错误'
	year_n, month_n,day_n= int(raw_input('请输入年')),int(raw_input('请输入月')),int(raw_input('请输入日'))
	print day_in_year(year_n,month_n,day_n)
```

```python
print('---------不使用sort()完成对任意list的升序和降序的排列-------------')    #冒泡排序
	list7 = [2,5,6,1,4,9,]
	def sort_num(*args):
		list8 = []
		for i in range(len(list7)):
			for j in range(len(list7)-1):
				if list7[i]<list7[j]:
					list7[j],list7[i] = list7[i],list7[j]    #直接交换
		list8 = list7
		return list8
	print '升序结果：',sort_num(list7)
```

```python
print('---------对元组内数字型元素求和-------------')
	def fun(num1,num2,*agrs,**kwargs):
		sum1 = 0
		sum2 = 0
		sum0 = int(num1.strip())+int(num2)
		for i in agrs:
			if str(i).strip().isdigit():
				sum1 += int(i)
		for j in kwargs.values():
			if str(j).strip().isdigit():
				sum2 += int(j)
		num3 = sum0+sum1+sum2
		return num3
	a = fun('  1', 2, 3, '  4', 5, 'hello', a1=2, b1=3)
	print a
```

```python
print('---------字典合并-------------')
	def merge_two_dicts(x, y):
		dict3 = {}
		dict3 = dict1.copy()
		dict3.update(dict2)
		return dict3
	dict1 = {'a':1,'b':2,'c':3,'d':'hello'}
	dict2 = {'a':2,'b':'2','c':'she','e':'10','f':4}
	print(merge_two_dicts(dict1,dict2))
```























