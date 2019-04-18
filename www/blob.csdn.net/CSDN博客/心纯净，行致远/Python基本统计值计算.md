# Python基本统计值计算 - 心纯净，行致远 - CSDN博客





2019年02月24日 22:10:44[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：35








```python
def GetNum():   #获得用户的不定长输入
    nums=[]
    iNum=input("请输入数字（以回车退出）：")
    while iNum!="":
        nums.append(eval(iNum))
        iNum=input("请输入数字（以回车退出）：")
    return nums

def Mean(nums): #calculae mean
    s=0.0
    for i in nums:
        s+=i
    return s/len(nums)

def Dev(nums,mean): #calculate dev
    dev=0.0
    for i in nums:
        dev+=(i-mean)**2
    return pow(dev/(len(nums)-1),0.5)

def median(nums): #calculate median num
    sorted(nums)
    if len(nums)%2==0:
        median=(nums[len(nums)//2-1]+nums[len(nums)//2])/2
    else:
        median=nums[len(nums)//2]
    return median

n=GetNum()
m=Mean(n)
print("平均值：{}，方差：{:.2}，中位数：{}".format(m,Dev(n,m),median(n)))
```

res:

RESTART: C:/Users/QinJX/AppData/Local/Programs/Python/Python37-32/python编程学习/9CalStatisticsV1.py

请输入数字（以回车退出）：1

请输入数字（以回车退出）：2

请输入数字（以回车退出）：3

请输入数字（以回车退出）：4

请输入数字（以回车退出）：5

请输入数字（以回车退出）：6

请输入数字（以回车退出）：7

请输入数字（以回车退出）：8

请输入数字（以回车退出）：9

请输入数字（以回车退出）：

平均值：5.0，方差：2.7，中位数：5



