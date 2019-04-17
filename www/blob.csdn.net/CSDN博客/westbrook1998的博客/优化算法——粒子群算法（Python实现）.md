# 优化算法——粒子群算法（Python实现） - westbrook1998的博客 - CSDN博客





2018年07月30日 21:26:06[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：523








```python
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt

class Position:
    def __init__(self,x,y,f):
        # x y为自变量 可以多个
        self.x=x
        self.y=y
        # 该点的适应度 即要求的函数值
        self.f=f

    def __str__(self):
        return "x:"+str(self.x)+" y:"+str(self.y)+" f:"+str(self.f)

# 粒子数
n=100
# 粒子集合
p=[]
v=[]
pBest=[]
gBest=Position(0,0,0)
# 学习因子
c1=2.0
c2=2.0
# 惯性权重
w=0.2
# 更新边界
vmax=0.05
# 迭代次数
_max=10000

# 适应函数（主要修改的地方）
def fitnessFunction(x,y):
    return x**2+y**3-5*x*y

def pso():
    # 创建点集
    p = [Position(xi*10, yi*10, fitnessFunction(xi, yi)) for xi, yi in zip(np.random.rand(n), np.random.rand(n))]
    v = [Position(xi * vmax, yi * vmax, fitnessFunction(xi, yi)) for xi, yi in zip(np.random.rand(n), np.random.rand(n))]
    pBest = p.copy()
    # 当前个体极值
    gBest = max(p, key=lambda pi: pi.f)
    for i in range(_max):
        for j in range(n):
            # 更新速度和位置
            vx=w*v[j].x+c1*np.random.random()*(pBest[j].x-p[j].x)+c2*np.random.random()*(gBest.x-p[j].x)
            vy=w*v[j].y+c1*np.random.random()*(pBest[j].y-p[j].y)+c2*np.random.random()*(gBest.y-p[j].y)
            if vx>vmax:
                vx=vmax
            if vy>vmax:
                vy=vmax
            v[j]=Position(vx,vy,fitnessFunction(vx,vy))
            p[j].x+=v[j].x
            p[j].y+=v[j].y
            p[j].f=fitnessFunction(p[j].x,p[j].y)

            # 越界判断
            if p[j].x>=10:
                p[j].x=9.9
            if p[j].x<=0:
                p[j].x=0.1
            if p[j].y>=10:
                p[j].y=9.9
            if p[j].y<=0:
                p[j].y=0.1

        # 更新个体极值和群体极值
        for j in range(n):
            if pBest[j].f<p[j].f:
                pBest[j]=p[j]
            if p[j].f>gBest.f:
                gBest=p[j]

        print("====="+str(i+1)+"=====gBest:"+gBest.__str__())

if __name__=='__main__':
    pso()
```



