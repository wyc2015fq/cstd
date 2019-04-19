# python 中map函数使用 - sxf_123456的博客 - CSDN博客
2017年12月19日 16:16:57[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：164
python 中map函数第一个作为函数，第二个作为迭代对象
ls = [1,2,3]
rs=map(str,ls)
for num in rs:
print(num)
将ls列表转化为str类型
则 rs=["1","2","3"]
list_num=[1,2,4,5,6]
def add(num):
return num+1
rs=map(add,list_num)
for i in rs:
print(i)
输出
