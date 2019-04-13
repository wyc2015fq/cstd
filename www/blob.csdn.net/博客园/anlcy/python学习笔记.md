
# python学习笔记 - anlcy - 博客园






# [python学习笔记](https://www.cnblogs.com/camilla/p/7198764.html)
来自：https://www.runoob.com/python3/python3-tutorial.html
1.使用循环嵌套来实现99乘法法则：
for i in range(1,10):
for j in range(1,i+1):
sun=i*j
print('%d*%d=%d'%(j,i,sun),end=' ')
print('\r')

2.生成斐波纳契数列：
a,b=0,1
>>> while b<1000:
print(b,end=' ')
a,b=b,a+b
输出：1 1 2 3 5 8 13 21 34 55 89 144 233 377 610 987
使用了 yield 的函数被称为生成器（generator），生成器是一个返回迭代器的函数，只能用于迭代操作，更简单点理解生成器就是一个迭代器。
在调用生成器运行的过程中，每次遇到 yield 时函数会暂停并保存当前所有的运行信息，返回yield的值。并在下一次执行 next()方法时从当前位置继续运行。
以下实例使用 yield 实现斐波那契数列：
importsysdeffibonacci(n):\# 生成器函数 - 斐波那契a,b,counter=0,1,0whileTrue:if(counter>n):returnyielda
        a,b=b,a+b
        counter+=1f=fibonacci(10)\# f 是一个迭代器，由生成器返回生成whileTrue:try:print(next(f),end=" ")exceptStopIteration:sys.exit()结果：011235813213455
3.列表推导式，对序列里每一个元素逐个调用某方法：
vec1 = [2, 4, 6]
vec2 = [4, 3, -9]
[x*y for x in vec1 for y in vec2]
结果：[8, 6, -18, 16, 12, -36, 24, 18, -54]
4.嵌套列表解析：
展示3*4的矩形列表：
matrix = [[1,2,3,4],[5,6,7,8],[9,10,11,12]]
将3X4的矩阵列表转换为4X3列表：
[[row[i] for row in matrix]for i in range(4)]
或者
transposed=[]
foriinrange(4):
transposed.append([row[i]forrowinmatrix])
transposed
或者
transposed = []
for i in range(4):
transposed_row = []
for row in matrix:
transposed_row.append(row[i])
transposed.append(transposed_row)
transposed
结果：[[1, 5, 9], [2, 6, 10], [3, 7, 11], [4, 8, 12]]





