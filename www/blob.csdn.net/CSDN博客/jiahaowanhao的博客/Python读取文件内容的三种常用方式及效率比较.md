
# Python读取文件内容的三种常用方式及效率比较 - jiahaowanhao的博客 - CSDN博客


2018年02月22日 20:48:56[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：5890


**Python读取文件内容的三种常用方式及效率比较**
本文实例讲述了Python读取文件内容的三种常用方式。分享给大家供大家参考，具体如下：
本次实验的文件是一个60M的文件，共计392660行内容。
程序一：
def one():
start = time.clock()
fo = open(file,'r')
fc = fo.readlines()
num = 0
for l in fc:
tup = l.rstrip('\n').rstrip().split('\t')
num = num+1
fo.close()
end = time.clock()
print end-start
print num
运行结果：0.812143868027s
程序二：
def two():
start = time.clock()
num = 0
with open(file, 'r') as f:
for l in f:
tup = l.rstrip('\n').rstrip().split('\t')
num = num+1
end = time.clock()
times = (end-start)
print times
print num
运行时间：0.74222778078
程序三：
def three():
start = time.clock()
fo = open(file,'r')
l = fo.readline()
num = 0
while l:
tup = l.rstrip('\n').rstrip().split('\t')
l = fo.readline()
num = num+1
end = time.clock()
print end-start
print num
运行时间：1.02316120797
由结果可得出，程序二的速度最快。
img

