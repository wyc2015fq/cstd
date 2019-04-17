# 最优化算法（三）：PSO - qq_38593211的博客 - CSDN博客





2018年08月05日 18:40:46[小屁猴多看书](https://me.csdn.net/qq_38593211)阅读数：292








## 1.介绍

     我这里就不去生动形象的举什么鸟类捕食的例子了，直接给出流程，基本的粒子群算法很简单，只用明确它每次迭代优化的方法就好了。同样PSO也是一个无约束的迭代求解的最优化算法，我们的目标是(这里的min只是举例子，真实情况不一定是求最小值，可能是求一个特定的f(x))：

![min_{x\in R^{(N)}}f(x)](https://private.codecogs.com/gif.latex?min_%7Bx%5Cin%20R%5E%7B%28N%29%7D%7Df%28x%29)

     N就是x所在的搜索空间的维度，我们的目标是要找到一个![x^{*}={x_{1}.......x_{N}}](https://private.codecogs.com/gif.latex?x%5E%7B*%7D%3D%7Bx_%7B1%7D.......x_%7BN%7D%7D)使得我们能达到目标，一般f(x)被称为适应函数，初始化的时候我们是要随机生成M（自己定）个x值（即种群），每个x值会初始化一个变化速度v，这个变化速度是一个向量，它的维度和x是一样的，因为你肯定在不同的维度有不同的变化，之后就是随机初始化全局最优（什么叫全局最优？就是这M个x中令适应函数更接近目标的x就是全局最优值）和个体最优（什么叫个体最优？我么的算法在不断的迭代的，对于种群中的每个x我们都会记录他的适应函数值，最好的那个值对应的x就是个体最优），然后我们在每一次迭代中都会向全局最优值和个体最优值学习，这里的学习会有一个学习权重，代表着它们是更相信种群还是更愿意保持自己的优势，那学习它们的什么呢？当然是学习它们的位置和变化速度了，这就是我最开始说的你需要明白的每次迭代优化的方法，下面详细介绍流程和更新公式。

## 2.流程

     输入：目标求解函数f(x)，以及种群大小M，目标求解值K，c1，c2，w

     输出：当f(x)=K时x的取值 

     step1：初始化M个x（种群）的x值（N维）和速度v（N维）

     step2：对于每个x计算适应度值f(x)，并得到每个粒子的个体最优pbest和种群**当代**的最强粒子gbest     

     step3：对于每个粒子，将本次迭代的适应度值和pbest比较，如果比它大就更新pbest，同理更新gbest

     step4：按下列公式更新每个x的值和它的变化速度（w是惯性指数，即粒子保持自身的系数）：

**![v[i]=wv[i]+c_{1}rand()pbest+c_{2}rand()gbest](https://private.codecogs.com/gif.latex?v%5Bi%5D%3Dwv%5Bi%5D&plus;c_%7B1%7Drand%28%29pbest&plus;c_%7B2%7Drand%28%29gbest)**

**![x[i]=x[i]+v[i]](https://private.codecogs.com/gif.latex?x%5Bi%5D%3Dx%5Bi%5D&plus;v%5Bi%5D)**

     step5：当迭代次数达到最大或达到停止条件就结束，否则返回步骤2

## 3.特点

     粒子群算法适合求解实数问题，算法简单，计算方便，求解速度快，但是存在着陷入局部收敛和后期收敛过慢的缺点。

## 4.参数调节

    PSO其实是一个很老但是很经典的算法，现在有非常多的改进和融合方法，但我觉得由于它本身的特性决定很多东西是不能从根本上去解决的，所以我们要掌握它的基本思想，把这种思想能用到以后解决问题中就可以了，至于参数的调节也有一些方法。粒子数一般取 20 – 40，其实对于大部分的问题10个粒子已经足够可以取得好的结果, 不过对于比较难的问题或者特定类别的问题, 粒子数可以取到100 或 200 。还有就是学习因子的调节，一般最开始c2大，后期c1大，学习率先大后小，之所以这样做也是为了后期更精确的收敛。

## 5.实现        

```python
import numpy as np
w = 0.6  # 惯性权重
c1 = c2 = 0.5 #学习率
DIM = 2  # 搜索空间的维度
population_size =30  # 粒子群数量
MAX_STEPS = 100  # 迭代次数
def init_pso(population_size, dim):
        x_bound = [-10, 10]  # 解空间范围
        x = np.random.uniform(x_bound[0], x_bound[1],(population_size, dim))  # 随机初始化粒子群位置
        v = np.random.rand(population_size, dim)  # 初随机始化粒子群速度
        fitness = np.sum(np.square(x), axis=1)#计算适应度值
        pbest = x  # 个体的最佳位置
        gbest = x[np.argmin(fitness)]  # 全局最佳位置
        individual_best_fitness = fitness  # 个体的最优适应度
        global_best_fitness = np.max(fitness)  # 全局最佳适应度
        return x,v,pbest,gbest,individual_best_fitness,global_best_fitness

def evolve(max_steps,dim,c1,c2,w,population_size):
        x,v,pbest,gbest,individual_best_fitness,global_best_fitness=init_pso(population_size,dim)
        for step in range(max_steps):
            #随机数
            r1 = np.random.rand(population_size, dim)
            r2 = np.random.rand(population_size, dim)
            # 更新速度和权重
            v = w*v+c1*r1*(pbest-x)+c2*r2*(gbest-x)
            x = v + x
            fitness = np.sum(np.square(x), axis=1)
            # 需要更新的个体
            update_id = np.greater(individual_best_fitness, fitness)
            pbest[update_id] = x[update_id]
            individual_best_fitness[update_id] = fitness[update_id]
            # 新一代出现了更小的fitness，所以更新全局最优fitness和位置
            if np.min(fitness) < global_best_fitness:
                gbest = x[np.argmin(fitness)]
                global_best_fitness = np.min(fitness)
            print('best fitness: %.5f, mean fitness: %.5f' % (global_best_fitness, np.mean(fitness)))
```

```python
evolve(MAX_STEPS,DIM,c1,c2,w,population_size)



best fitness: 0.06337, mean fitness: 32.54324
best fitness: 0.06337, mean fitness: 10.85758
best fitness: 0.06337, mean fitness: 4.66646
best fitness: 0.06168, mean fitness: 4.87920
best fitness: 0.01544, mean fitness: 3.95356
best fitness: 0.00392, mean fitness: 1.69832
best fitness: 0.00392, mean fitness: 0.72295
best fitness: 0.00069, mean fitness: 0.57723
best fitness: 0.00069, mean fitness: 0.41466
best fitness: 0.00057, mean fitness: 0.24419
best fitness: 0.00027, mean fitness: 0.10731
best fitness: 0.00014, mean fitness: 0.04552
best fitness: 0.00008, mean fitness: 0.03174
best fitness: 0.00006, mean fitness: 0.02867
best fitness: 0.00004, mean fitness: 0.01872
best fitness: 0.00002, mean fitness: 0.00969
best fitness: 0.00002, mean fitness: 0.00655
best fitness: 0.00002, mean fitness: 0.00485
best fitness: 0.00002, mean fitness: 0.00339
best fitness: 0.00000, mean fitness: 0.00170
best fitness: 0.00000, mean fitness: 0.00086
best fitness: 0.00000, mean fitness: 0.00050
best fitness: 0.00000, mean fitness: 0.00031
best fitness: 0.00000, mean fitness: 0.00024
best fitness: 0.00000, mean fitness: 0.00018
best fitness: 0.00000, mean fitness: 0.00009
best fitness: 0.00000, mean fitness: 0.00004
best fitness: 0.00000, mean fitness: 0.00003
best fitness: 0.00000, mean fitness: 0.00002
best fitness: 0.00000, mean fitness: 0.00001
best fitness: 0.00000, mean fitness: 0.00001
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
best fitness: 0.00000, mean fitness: 0.00000
```





