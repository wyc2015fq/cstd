# momentum_code - wsp_1138886114的博客 - CSDN博客





2018年08月05日 21:28:01[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：101








##### momentum代码示例

当 momentum 动量越大时，其转换为势能的能量也就越大，就越有可能摆脱局部凹域的束缚，进入全局凹域。避免了不恰当的初始权值可能使得网络的损失函数在训练过程中陷入局部最小值，达不到全局最优的状态。 
- 一般，神经网络在更新权值时，采用如下公式: 
`w = w - learning_rate * dw`
- 引入momentum后，采用如下公式： 
`v = mu * v - learning_rate * dw`
`w = w + v`

其中：v初始化为0，mu是设定的一个超变量，最常见的设定值是0.9。
```python
import numpy as np 
import matplotlib.pyplot as plt 
# 目标函数:y=x^2 
def func(x): 
    return np.square(x) 
    # 目标函数一阶导数:dy/dx=2*x def dfunc(x): 
    return 2 * x 

def GD_momentum(x_start, df, epochs, lr, momentum): 
    """ 
    带有冲量的梯度下降法。 
    :param x_start: x的起始点 
    :param df:目标函数的一阶导函数 
    :param epochs: 迭代周期 
    :param lr: 学习率 
    :param momentum: 冲量 
    :return: x在每次迭代后的位置（包括起始点），长度为epochs+1 
    """
    xs = np.zeros(epochs+1) 
    x = x_start xs[0] = x
    v = 0 
    for i in range(epochs): 
        dx = df(x) # v表示x要改变的幅度 
        v = - dx * lr + momentum * v 
        x += v xs[i+1] = x 
    return xs


为了查看momentum大小对不同学习率的影响，
此处设置学习率为lr = [0.01, 0.1, 0.6, 0.9]，
冲量依次为momentum = [0.0, 0.1, 0.5, 0.9]，
起始位置为x_start = -5，迭代周期为6。测试以及绘图代码如下： 

def demo2_GD_momentum(): 
    line_x = np.linspace(-5, 5, 100) 
    line_y = func(line_x) 
    plt.figure('Gradient Desent: Learning Rate, Momentum') 
    x_start = -5 
    epochs = 6 

    lr = [0.01, 0.1, 0.6, 0.9] 
    momentum = [0.0, 0.1, 0.5, 0.9] 
    color = ['k', 'r', 'g', 'y'] 
    row = len(lr) 
    col = len(momentum) 
    size = np.ones(epochs+1) * 10 
    size[-1] = 70 
    for i in range(row): 
        for j in range(col): 
            x = GD_momentum(x_start, dfunc, epochs, lr=lr[i], momentum=momentum[j]) 
            plt.subplot(row, col, i * col + j + 1) 
            plt.plot(line_x, line_y, c='b') 
            plt.plot(x, func(x), c=color[i], 
            label='lr={}, mo={}'.format(lr[i], momentum[j])) 
            plt.scatter(x, func(x), c=color[i], s=size) 
            plt.legend(loc=0) 
    plt.show()
```

```python
import numpy as np
import matplotlib.pyplot as plt
from mpl_toolkits.mplot3d import Axes3D
# 本代码是一个最简单的线形回归问题，优化函数为 momentum
rate = 0.1 # learning rate
def da(y,y_p,x):
    return (y-y_p)*(-x)

def db(y,y_p):
    return (y-y_p)*(-1)
def calc_loss(a,b,x,y):
    tmp = y - (a * x + b)
    tmp = tmp ** 2  # 对矩阵内的每一个元素平方
    SSE = sum(tmp) / (2 * len(x))
    return SSE
def draw_hill(x,y):
    a = np.linspace(-20,20,100)
    print(a)
    b = np.linspace(-20,20,100)
    x = np.array(x)
    y = np.array(y)

    allSSE = np.zeros(shape=(len(a),len(b)))
    for ai in range(0,len(a)):
        for bi in range(0,len(b)):
            a0 = a[ai]
            b0 = b[bi]
            SSE = calc_loss(a=a0,b=b0,x=x,y=y)
            allSSE[ai][bi] = SSE

    a,b = np.meshgrid(a, b)

    return [a,b,allSSE]
#  模拟数据
x = [30 ,35,37, 59, 70, 76, 88, 100]
y = [1100,  1423,   1377,   1800,   2304,   2588,   3495,   4839]

# 数据归一化
x_max = max(x)
x_min = min(x)
y_max = max(y)
y_min = min(y)

for i in range(0,len(x)):
    x[i] = (x[i] - x_min)/(x_max - x_min)
    y[i] = (y[i] - y_min)/(y_max - y_min)

[ha,hb,hallSSE] = draw_hill(x,y)
hallSSE = hallSSE.T# 重要，将所有的losses做一个转置。原因是矩阵是以左上角至右下角顺序排列元素，而绘图是以左下角为原点。
# 初始化a,b值
a = 10.0
b = -20.0
fig = plt.figure(1, figsize=(12, 8))
fig.suptitle('learning rate: %.2f method:momentum'%(rate), fontsize=15)


# 绘制图1的曲面
ax = fig.add_subplot(2, 2, 1, projection='3d')
ax.set_top_view()
ax.plot_surface(ha, hb, hallSSE, rstride=2, cstride=2, cmap='rainbow')

# 绘制图2的等高线图
plt.subplot(2,2,2)
ta = np.linspace(-20, 20, 100)
tb = np.linspace(-20, 20, 100)
plt.contourf(ha,hb,hallSSE,15,alpha=0.5,cmap=plt.cm.hot)
C = plt.contour(ha,hb,hallSSE,15,colors='black')
plt.clabel(C,inline=True)
plt.xlabel('a')
plt.ylabel('b')

plt.ion() # iteration on

all_loss = []
all_step = []
last_a = a
last_b = b
va = 0
vb = 0
gamma = 0.9
for step in range(1,100):
    loss = 0
    all_da = 0
    all_db = 0
    for i in range(0,len(x)):
        y_p = a*x[i] + b
        loss = loss + (y[i] - y_p)*(y[i] - y_p)/2
        all_da = all_da + da(y[i],y_p,x[i])
        all_db = all_db + db(y[i],y_p)
    #loss_ = calc_loss(a = a,b=b,x=np.array(x),y=np.array(y))
    loss = loss/len(x)

    # 绘制图1中的loss点
    ax.scatter(a, b, loss, color='black')
    # 绘制图2中的loss点
    plt.subplot(2,2,2)
    plt.scatter(a,b,s=5,color='blue')
    plt.plot([last_a,a],[last_b,b],color='aqua')
    # 绘制图3中的回归直线
    plt.subplot(2, 2, 3)
    plt.plot(x, y)
    plt.plot(x, y, 'o')
    x_ = np.linspace(0, 1, 2)
    y_draw = a * x_ + b
    plt.plot(x_, y_draw)
    # 绘制图4的loss更新曲线
    all_loss.append(loss)
    all_step.append(step)
    plt.subplot(2,2,4)
    plt.plot(all_step,all_loss,color='orange')
    plt.xlabel("step")
    plt.ylabel("loss")


    # print('a = %.3f,b = %.3f' % (a,b))
    last_a = a
    last_b = b
    va = gamma * va+ rate*all_da
    vb = gamma * vb+ rate*all_db
    a = a - va
    b = b - vb

    if step%1 == 0:
        print("step: ", step, " loss: ", loss)
        plt.show()
        plt.pause(0.01)
plt.show()
plt.pause(99999999999)
```




