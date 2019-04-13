
# Python实现读取字符串按列分配后按行输出示例 - jiahaowanhao的博客 - CSDN博客


2018年05月22日 16:03:34[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：932


[Python实现读取字符串按列分配后按行输出示例](http://cda.pinggu.org/view/25610.html)
本文实例讲述了Python实现读取字符串按列分配后按行输出。分享给大家供大家参考，具体如下：
问题：
输入一个字符串和一个数字，数字代表分为几行，需要按照给定的列存储方法存储下来之后按行拼接读出，如：
输入：TNGDWXAZQSCVBK，3
输出：TWQBNDXZSVKGAC
中间转化的时候会形成这样的图形：
T   W   Q   K
N D X Z S V B
G   A   C
化为矩阵可能看得更清晰一点：
T 0 W 0 Q 0 B
N D X Z S V K
G 0 A 0 C 0 0
0是作为空白填充进去的，下面看具体的实现：
\#!usr/bin/env python
\#encoding:utf-8
'''''
__Author__:沂水寒城
功能：zigzag_pattern
T  W  Q  K
N D X Z S V B
G  A  C
convert("TNGDWXAZQSCVBK", 3) should return "TWQBNDXZSVKGAC".
'''
def convert_test(one_str,num):
'''''
'''
mod=len(one_str)%(num+1)
if mod==0:
cols=(len(one_str)/(num+1))*2
else:
cols=(len(one_str)/(num+1))*2+1
rows=num
\#print 'rows, cols', rows, cols
matrix=[]
for i in range(rows):
matrix.append([0]*cols)
one_str_list=list(one_str)
for j in range(cols):
if j%2==0:
for i in range(rows):
if len(one_str_list):
matrix[i][j]=one_str_list.pop(0)
else:
matrix[1][j]=one_str_list.pop(0)
\#print matrix
\# for one in matrix:
\#   one_list=[str(onec) for onec in one]
\#   print ' '.join(one_list)
res=''
for one_list in matrix:
one_list=[str(one) for one in one_list]
res+=''.join(one_list)
return ''.join(res.split('0'))
if __name__ == '__main__':
print convert_test("TNGDWXAZQSCVBK", 3)
print convert_test("PAYPALISHIRING", 2)
print convert_test("PAYPALISHIRING", 4)
结果如下：
TWQBNDXZSVKGAC
PPIINAYALSHRIG
PLRAAIIIYSNPHG
[Finished in 0.3s]

