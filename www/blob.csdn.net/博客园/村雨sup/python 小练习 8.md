# python 小练习 8 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)




砝码问题1
有一组砝码，重量互不相等，分别为m1、m2、m3……mn；它们可取的最大数量分别为x1、x2、x3……xn。 
现要用这些砝码去称物体的重量,问能称出多少种不同的重量。 
现在给你两个正整数列表w和n， 列表w中的第i个元素w[i]表示第i个砝码的重量，列表n的第
i个元素n[i]表示砝码i的最大数量。i从0开始，请你输出不同重量的种数。
如：w=[1,2], n=[2,1], 则输出5（分析：共有五种重量：0,1,2,3,4）


```
import copy

rsl = set()
rsl.add(0)                        #这里有个初始值0，很重要
size = len(n)

for i in range(size):
    temp = copy.deepcopy(rsl)     #复制一个集合
    for j in range(n[i]+1):
        v = w[i]*j                #j个重量
        for value in temp:        #不停用基础数据加上增加的数据
            rsl.add(value+v)      #用集合的好处就是能不重复的加上数
print(len(rsl))
```

 砝码问题2：
有一组砝码，重量互不相等，分别为m1、m2、m3……mn；每种砝码的数量有无限个。 
现要用这些砝码去称物体的重量,给你一个重量n,请你判断有给定的砝码能否称出重量n。 
现在给你一个正整数列表w和一个正整数n，列表w中的第i个元素w[i]表示第i种砝码的重量，
n表示要你判断的重量。如果给定砝码能称出重量n，输出Yes，否则输出No。
例如，w=[2,5,11], n=9,则输出Yes（取两个2，一个5）。
AC：解法背包法
```
L=len(w)
s=[-1 for i in range(n+1)]
s[0]=0
for i in range(L):
    for j in range(1,n+1):
        if j-w[i]>=0 and s[j-w[i]]!=-1:  #第一个是取到s[0]=0
            s[j]=1       #从最小的加起，如果能取到就换为1
if s[n]!=-1:
    print 'yes'
else:
    print "no"
```













