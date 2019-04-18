# Python3算法实例 1.2：动态规划 之 换零钱 - weixin_33985507的博客 - CSDN博客
2018年03月22日 09:55:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：31
![4734220-f715f631815f8a6f.jpg](https://upload-images.jianshu.io/upload_images/4734220-f715f631815f8a6f.jpg)
money.jpg
##### 问题(基础版)：
把100元兑换成1元，2元，5元，10元，20元，50元的零钱，共有多少种不同换法。
##### 动态规划思想解析：
- 拆解子问题
下面以5元换成1,2,3元的零钱为例。**T[(change),target]**表示用零钱序列change组成target的所有方法。见下图：
![4734220-24afb25a0cdc28b6.png](https://upload-images.jianshu.io/upload_images/4734220-24afb25a0cdc28b6.png)
example.png
其中：
**T1[(1,2)，5-0*3]**可看作**只用1和2元**、**不用3元**组成5元的方法数；
**T2[(1,2)，5-1*3]**可看作**用1和2元**、并且**只用1个3元**组成5元的方法数；
**T6[(1)，3]**可看作**用1元**，**只用1个2元**、并且**不用3元**组成5元的方法数；
**T9[(1)，0]**可看作**用1元**，**只用1个2元**、**只用1个3元**组成5元的方法数。
- 
状态转移表达式：
![4734220-bb05af6049f50c53.png](https://upload-images.jianshu.io/upload_images/4734220-bb05af6049f50c53.png)
problem.png
- 边界条件：
- M-nd不得小于0，并且**当M-nd等于0时**，也就是说之前的选择已经组成了目标，所以表达式结果为**1**。
- 如果表达式为T[(a),c]，只有**a可以被c整除，此表达式才为1**，否则为0。例如T[(2),5]=0,T[(2),6]=1。
###### Python3程序实现
```
#递归实现
#根据边界条件编写程序
def Solve(exlist,target):
    #判断问题解
    if target==0:
        return 1
    else:
        if len(exlist)==1 and target%exlist[0]==0:
            return 1
        else:
            return 0
#递归的方式，先将问题拆解为子问题的集合 
def Dismantle(exlist):
    if len(exlist[0][0])>1:
        fu=[]
        for ie in exlist:
            for ji in range(int(ie[1]/ie[0][-1])+1):
                fu.append([ie[0][0:-1],ie[1]-ji*ie[0][-1]])
        return Dismantle(fu)
    else:
        return exlist
#计算结果
def Recursion(exlist,target):
    structer=[[exlist,target]]
    count=0
    for jj in Dismantle(structer):#所有子问题的集合
        if Solve(jj[0],jj[1])==1:#计算每一个子问题的结果
            count+=Solve(jj[0],jj[1])
    return count
print(Recursion([1,2,5,10,20,50],100))
结果：4562
```
上述的递归程序编写较为复杂，但是容易理解，下面给出基于动态规划的程序。
```
#动态规划实现
def DP_Com(exlist,num):
    an=[1]+[0]*num
    for i in exlist :
        for j in range(i,num+1):
            an[j]+=an[j-i]
    return an[num]
print(DP_Com([1,2,5,10,20,50],100))
结果：4562
```
##### 问题(进阶版)：
有面值为1元、3元和5元的钱币若干张，如何用最少的钱币凑够11元，并列出组合方法。
###### 拆解子问题：
和基础版问题相比，此题复杂之处在于最小值的计算。详细见下图：
![4734220-223c5062e56a99cc.png](https://upload-images.jianshu.io/upload_images/4734220-223c5062e56a99cc.png)
min.png
###### Python3程序实现
```
#记录
def DP_Min(exlist,target):
    fan=[0]+[target]*target
    record=['']*(target+1)
    #存储最后一位
    lic=[]
    for i in exlist:
        for j in range(i,target+1):
            if fan[j-i]!=target:#如果等于target，说明之前没有数的结合可以达到这个数
                fan[j]=min(fan[j-i]+1,fan[j])
                #记录
                if fan[j-i]+1<=fan[j]:
                    record[j]=record[j-i]+'%d*'%i
                else:
                    record[j]=record[j]
        if record[-1]!='':
            lic.append(record[-1])
    if len(lic)==0:
        return '无解'
    else:
        return lic,fan[target]
#处理字符串
def Handle(exstr,num):
    for i in set(exstr):
        hu=i.split('*')
        hu=list(filter(lambda x:x,hu))#去除掉split留下的空格
        if len(hu)==num:
            shuoming=''
            for j in set(hu):
                shuoming+='%s个%s '%(hu.count(j),j)
            print(shuoming)
    return 'Done'
ggg=DP_Min([1,3,5],11)
print(Handle(ggg[0],ggg[1]))
#结果：最少为3。组合方法：2个5 1个1
```
**不定期更新，欢迎留言，敬请关注!!!**
