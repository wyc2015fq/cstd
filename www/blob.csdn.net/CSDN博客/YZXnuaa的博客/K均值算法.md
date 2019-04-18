# K均值算法 - YZXnuaa的博客 - CSDN博客
2019年04月14日 23:54:51[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：14
一、原理
二、Python实现
> 
import matplotlib
import matplotlib.pyplot as plt
import numpy as np
# Data for plotting
def createdata():
    np.random.seed(0)
    data0 = np.random.rand(2, 100)
    data0[0] = data0[0]+1
    data0[1] = data0[1]+1
    np.random.seed(1)
    data1 = np.random.rand(2, 100)
    data1[0] = data1[0]+1
    data1[1] = data1[1]-1
    np.random.seed(2)
    data2 = np.random.rand(2, 100)
    data2[0] = data2[0]-1
    data = np.concatenate((data0, data1, data2), axis=1)
    dataT = data.T
    np.random.shuffle(dataT)
    data = dataT.T
    return data
    # data.shape = (2, 300)
def dis(x, y):
    return np.sqrt((x[0] - y[0]) ** 2 + (x[1] - y[1]) ** 2)
def maxi(a):
    numm1 = 0
    max1 = a[0]
    for i in range(len(a)):
        if a[i] >= max1:
            numm1 = i
            max1 = a[i]
    return numm1
def main():
    fig, ax = plt.subplots()
    data = createdata()
    dataT = data.T
    # looking for beginning point0
    centroid = [sum(data[0]) / len(data[0]), sum(data[1]) / len(data[1])]
    # looking for beginning point1
    DistanceArray2centopoint0 = []
    for i in range(len(dataT)):
        DistanceArray2centopoint0.append(dis(dataT[i], centroid))
    centpoint1i = maxi(DistanceArray2centopoint0)
    centpoint1 = dataT[centpoint1i]
    # looking for beginning point2
    DistanceArray2centopoint1 = []
    for i in range(len(dataT)):
        DistanceArray2centopoint1.append(dis(dataT[i], centpoint1))
    centpoint2i = maxi(DistanceArray2centopoint1)
    centpoint2 = dataT[centpoint2i]
    # 记录质心变化
    centroidhistory = np.array(centroid).reshape((2, 1))
    centpoint1history = np.array(centpoint1).reshape((2, 1))
    centpoint2history = np.array(centpoint2).reshape((2, 1))
    # 算法迭代2次
    for n in range(2):
        # sample belong to which point
        def belong2point(x):
            minn = 0
            mind = dis(x, centroid)
            if dis(dataT[i], centpoint1) <= mind:
                minn = 1
            if dis(dataT[i], centpoint2) <= mind:
                minn = 2
            return minn
        # 确定属于某个簇的i
        cluster0 = []
        cluster1 = []
        cluster2 = []
        for i in range(len(dataT)):
            if belong2point(dataT[i]) == 0:
                cluster0.append(i)
            elif belong2point(dataT[i]) == 1:
                cluster1.append(i)
            elif belong2point(dataT[i]) == 2:
                cluster2.append(i)
        # 重新定义质心
        cluster0dataT = dataT[cluster0, :]
        cluster0data = cluster0dataT.T
        centroid = [sum(cluster0data[0]) / len(cluster0data[0]), sum(cluster0data[1]) / len(cluster0data[1])]
        cluster1dataT = dataT[cluster1, :]
        cluster1data = cluster1dataT.T
        centpoint1 = [sum(cluster1data[0]) / len(cluster1data[0]), sum(cluster1data[1]) / len(cluster1data[1])]
        cluster2dataT = dataT[cluster2, :]
        cluster2data = cluster2dataT.T
        centpoint2 = [sum(cluster2data[0]) / len(cluster2data[0]), sum(cluster2data[1]) / len(cluster2data[1])]
        # 测试集合
        ax.scatter(cluster0data[0], cluster0data[1])
        ax.scatter(cluster1data[0], cluster1data[1])
        ax.scatter(cluster2data[0], cluster2data[1])
        fig.savefig("第"+str(n)+"次")
        plt.cla()
        # 记录质心变化
        centroidhistory = np.concatenate((centroidhistory, np.array(centroid).reshape((2, 1))), axis=1)
        centpoint1history = np.concatenate((centpoint1history, np.array(centpoint1).reshape((2, 1))), axis=1)
        centpoint2history = np.concatenate((centpoint2history, np.array(centpoint2).reshape((2, 1))), axis=1)
    # pic
    ax.scatter(data[0], data[1])
    ax.plot(centroidhistory[0], centroidhistory[1], 'go-')
    ax.plot(centpoint1history[0], centpoint1history[1], 'm>-')
    ax.plot(centpoint2history[0], centpoint2history[1], 'kx-')
    fig.savefig("test.png")
    plt.show()
if __name__ == '__main__':
    main()
![](https://img-blog.csdnimg.cn/20190414235208407.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1laWG51YWE=,size_16,color_FFFFFF,t_70)
![](https://img-blog.csdnimg.cn/20190414235358198.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L1laWG51YWE=,size_16,color_FFFFFF,t_70)
**迭代两次就可以正确聚类**
