
# Python数据结构与算法之常见的分配排序法示例【桶排序与基数排序】 - jiahaowanhao的博客 - CSDN博客


2018年04月16日 21:26:08[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：56


**Python数据结构与算法之常见的分配排序法示例【桶排序与基数排序】**
本文实例讲述了Python数据结构与算法之常见的分配排序法。分享给大家供大家参考，具体如下：
箱排序（桶排序）
箱排序是根据关键字的取值范围1~m，预先建立m个箱子，箱排序要求关键字类型为有限类型，可能会有无限个箱子，实用价值不大，一般用于基数排序的中间过程。
桶排序是箱排序的实用化变种，其对数据集的范围，如[0,1) 进行划分为n个大小相同的子区间，每一个子区间为一个桶，然后将n非记录分配到各桶中。因为关键字序列是均匀分布在[0,1)上的，所以一般不会有很多记录落入同一个桶中。
以下的桶排序方法采用字典实现，所以对于整数类型，并不需要建立多余空间
def BuckSort(A):
bucks = dict()  \# 桶
for i in A:
bucks.setdefault(i,[]) \# 每个桶默认为空列表
bucks[i].append(i)  \# 往对应的桶中添加元素
A_sort = []
for i in range(min(A), max(A)+1):
if i in bucks:     \# 检查是否存在对应数字的桶
A_sort.extend(bucks[i])  \# 合并桶中数据
return A_sort
基数排序
\# 基数排序
\# 输入：待排序数组s, keysize关键字位数, 亦即装箱次数, radix基数
def RadixSort(s, keysize=4, radix=10):
\# 按关键字的第k分量进行分配 k = 4,3,2,1
def distribute(s,k):
box = {r:[] for r in range(radix)}  \# 分配用的空箱子
for item in s:   \# 依次扫描s[]，将其装箱
t = item
t /= 10**(k-1)
t %= 10    \# 去关键字第k位
box[t].append(item)
return box
\# 按分配结果重新排列数据
def collect(s,box):
a = 0
for i in range(radix):
s[a:a + len(box[i])] = box[i][:] \# 将箱子中元素的合并，覆盖到原来的数组中
a += len(box[i])     \# 增加偏移值
\# 核心算法
for k in range(1,keysize+1):
box = distribute(s,k)   \# 按基数分配
collect(s,box)     \# 按分配结果拼合
以下摘自：《数据结构与算法——理论与实践》
基数排序可以拓展为按多关键字排序，如对扑克牌按花色、按点数排序。
一般地，设线性表有那个待排序元素，每个元素包含d个关键字{k1,k2,...,kd}，则该线性表对关键字有序指，对于线性表中任意两个元素r[i]和r[j]，1<=i<=j<=n，都满足下列有序关系：
{k1i,k2i,...,kdi} < {k1j,k2j,...,kdj}
其中k1称为最主位关键字，kd称为最次位关键字
其排序方法分两种：最高位优先MSD（most significant digit frist）与最低位优先LSD（least significant digit first）
MSD： 先按k1排序分组，同一组的个元素，若关键字k1相等，再对各组按k2排序分成子组，依次类推，直到最次位kd对各子组排序后，再将各组链接起来。
LSD： 与MSD相反，先按kd排序，再对kd-1排序，依次类推。

