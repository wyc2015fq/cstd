# Science上发表的超赞聚类算法 - wishchinYang的专栏 - CSDN博客
2014年09月01日 19:55:54[wishchin](https://me.csdn.net/wishchin)阅读数：3473
论文. Clustering by fast search and find of density peak. Alex Rodriguez, Alessandro Laio
参考链接：[Science上发表的超赞聚类算法](https://www.cnblogs.com/jiahuaking/p/3942152.html)--
作者(Alex Rodriguez, Alessandro Laio)提出了一种很简洁优美的聚类算法, 可以识别各种形状的类簇, 并且其超参数很容易确定.
## 算法思想
该算法的假设是, 类簇的中心由一些局部密度比较低的点围绕, 并且这些点距离其他高局部密度的点的距离都比较大.
首先定义两个值:
      局部密度**![](https://images0.cnblogs.com/blog/650633/201408/281758406731109.png)**,  以及到高局部密度点的距离![](https://images0.cnblogs.com/blog/650633/201408/281758411733023.png):  ![](https://img-blog.csdnimg.cn/20181108102046460.png)
其中：
![](https://img-blog.csdnimg.cn/20181108102105979.png)![](https://img-blog.csdnimg.cn/20181108102136159.png)是一个截断距离, 是一个超参数.
所以![](https://images0.cnblogs.com/blog/650633/201408/281758411733023.png)相当于距离点i的距离小于![](https://img-blog.csdnimg.cn/20181108102136159.png)的点的个数. 由于该算法只对**![](https://images0.cnblogs.com/blog/650633/201408/281758406731109.png)**的相对值敏感, 所以对![](https://img-blog.csdnimg.cn/20181108102136159.png)的选择比较鲁棒, 一种推荐做法是选择![](https://img-blog.csdnimg.cn/20181108102136159.png)使得平均每个点的邻居数为所有点的1%-2%.
![](https://img-blog.csdnimg.cn/2018110810225768.png)
对于密度最大的点, 设置![](https://img-blog.csdnimg.cn/20181108102304896.png). 注意只有那些密度是局部或者全局最大的点才会有远大于正常的相邻点间距.
## 聚类过程
     那些有着比较大的局部密度**![](https://images0.cnblogs.com/blog/650633/201408/281758406731109.png)**和很大的![](https://images0.cnblogs.com/blog/650633/201408/281758411733023.png)的点被认为是类簇的中心. 局部密度较小但是![](https://images0.cnblogs.com/blog/650633/201408/281758411733023.png)较大的点是异常点.在确定了类簇中心之后, 所有其他点属于距离其最近的类簇中心所代表的类簇. 图例如下:
![](https://img-blog.csdnimg.cn/20181108100843925.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dpc2hjaGlu,size_16,color_FFFFFF,t_70)
       左图是所有点在二维空间的分布, 右图是以**![](https://images0.cnblogs.com/blog/650633/201408/281758406731109.png)**为横坐标, 以![](https://images0.cnblogs.com/blog/650633/201408/281758411733023.png)为纵坐标, 这种图称作决策图(decision tree). 可以看到, 1和10两个点的**![](https://images0.cnblogs.com/blog/650633/201408/281758406731109.png)**和![](https://images0.cnblogs.com/blog/650633/201408/281758411733023.png)都比较大, 作为类簇的中心点. 26, 27, 28三个点的![](https://images0.cnblogs.com/blog/650633/201408/281758411733023.png)也比较大, 但是**![](https://images0.cnblogs.com/blog/650633/201408/281758406731109.png)**较小, 所以是异常点.
## 聚类分析
        在聚类分析中, 通常需要确定每个点划分给某个类簇的可靠性. 在该算法中, 可以首先为每个类簇定义一个边界区域(border region), 亦即划分给该类簇但是距离其他类簇的点的距离小于![](https://img-blog.csdnimg.cn/20181108102136159.png)的点. 然后为每个类簇找到其边界区域的局部密度最大的点, 令其局部密度为**![](https://images0.cnblogs.com/blog/650633/201408/281758406731109.png)**. 该类簇中所有局部密度大于**![](https://images0.cnblogs.com/blog/650633/201408/281758406731109.png)**的点被认为是类簇核心的一部分(亦即将该点划分给该类簇的可靠性很大), 其余的点被认为是该类簇的光晕(halo), 亦即可以认为是噪音. 图例如下
![](https://img-blog.csdnimg.cn/20181108100956730.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dpc2hjaGlu,size_16,color_FFFFFF,t_70)
       A图为生成[数据](http://www.shellsec.com/)的概率分布, B, C二图为分别从该分布中生成了4000, 1000个点. D, E分别是B, C两组[数据](http://www.shellsec.com/)的决策图(decision tree), 可以看到两组[数据](http://www.shellsec.com/)都只有五个点有比较大的**![](https://images0.cnblogs.com/blog/650633/201408/281758406731109.png)**和很大的![](https://images0.cnblogs.com/blog/650633/201408/281758411733023.png). 这些点作为类簇的中心, 在确定了类簇的中心之后, 每个点被划分到各个类簇(彩色点), 或者是划分到类簇光晕(黑色点). F图展示的是随着抽样点数量的增多, 聚类的错误率在逐渐下降, 说明该算法是鲁棒的.
       最后展示一下该算法在各种数据分布上的聚类效果, 非常赞.
![](https://img-blog.csdnimg.cn/20181108101113658.jpg?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dpc2hjaGlu,size_16,color_FFFFFF,t_70)
数据集DATA来源:[http://cs.joensuu.fi/sipu/datasets/](http://cs.joensuu.fi/sipu/datasets/) ，测试数据集是几个文本文件，可以直接看着分析。
Python代码：原始链接：[http://www.chinakdd.com/article-Mut2iwV7S3i16cj.html](http://www.chinakdd.com/article-Mut2iwV7S3i16cj.html)
```python
#coding=utf-8
import math
import pylab as pl
def getDistance(pt1, pt2):
    tmp = pow(pt1[0]-pt2[0],2) + pow(pt1[1]-pt2[1],2)
    #dis = pow(tmp,0.5)
    dis = math.sqrt(math.fabs(tmp) )
    return dis
def ChooseDc(dc_percent,points,dis,distance):
    avgNeighbourNum = dc_percent*len(points)
    maxd =0
    for i in range(0,len(points)):
        for j in range(i ,len(points)):
           pt1 = points[i]
           pt2 = points[j]
           d = getDistance(pt1,pt2)
           dis.append(d)
           distance[i,j]= d
           dis.append(d)
           distance[j,i]= d
           if d>maxd:
               maxd = d
    #print dis
    dis.sort()
    #print dis
    print avgNeighbourNum
    print len(points)*2
    dc = dis[ int ( avgNeighbourNum* len(points)*2)]
    #print dc
    return dc
def drawOriginGraph(pl,points,cl,colorNum):
    x =[xx for(xx,yy)in points]
    y =[yy for(xx,yy)in points]
    cm = pl.get_cmap("RdYlGn")
    #print cl
    #for i in range(len(points)):
    #    if cl[i]==0:
    #        pl.plot(x[i],y[i],'o', color =cm(cl[i]*1.0/colorNum))
    #    else:
    #        pl.plot(x[i],y[i],'*', color =cm(cl[i]*1.0/colorNum))
    for i in range(len(points)):
        pl.plot(x[i],y[i],'o', color =cm( math.fabs(cl[i]) *1.0/colorNum))
        #pl.plot(x[i],y[i],'o', color =cm( cl[i] *1.0/colorNum))
def drawDecisionGraph(pl,rho, delta,cl,colorNum):
    cm = pl.get_cmap("RdYlGn")
    for i in range(len(rho)):
            pl.plot(delta[i],rho[i], 'o',color=cm( math.fabs(cl[i]) *1.0/colorNum ))
        
   #pl.xlabel(r'$rho$')
   #pl.ylabel(r'$delta$')
    pl.xlabel('wishchin')
    pl.ylabel('delta')
def Cluster(dc_percent):
#=========Load Data=========
    InputFileName = "flame"
    InputFileName = "Compound"
    InputFileName = "spiral"
    
    FolderName ="E:\Develope\EclipseWorks\MeachinLearning/Ch19_ScineceCluster/"
    
    InputFileName = FolderName + InputFileName
    OutputFileName= InputFileName +"_out"
    suffix =".txt"
    Fin= open(InputFileName+ suffix,"r")
    Fout= open(OutputFileName+ suffix,"w")
    points =[]
    for line in Fin.readlines():
        data = line.split()
        if len(data)==3:
            a =float(data[0])
            b =float(data[1])
            points.append((a,b))
#=========Calculating=========
#-----choose dc-----
    #dc_percent = 0.015 #0.5 #0.015
    dis =[]
    distance ={}
    dc =ChooseDc( dc_percent, points, dis, distance)
    print("dc:" ,str(dc))
#-----cal rho:"Cut off" kernel
#'''
#   rho = [0 for i in range(len(points))]
#   for i in range(0,len(points)):
#       for j in range(i 1,len(points)):
#           dij = getDistance(points[i],points[j])
#           if dij<dc:
#               rho[i] = 1
#               rho[j] = 1
#   '''
    
    #-----cal rho:"Gaussian Kernel"
    rho =[i for i in range(len(points))]
    for i in range(0,len(points)):
        for j in range(i ,len(points)):
            dij = getDistance(points[i],points[j])
            #print (dij, dc)
            #高斯核！！
            rho[i] = math.exp(-(dij/dc)*(dij/dc))
            rho[j] = math.exp(-(dij/dc)*(dij/dc))
    rho_list =[(rho[i],i)for i in range(len(rho))]
    rho_sorted = sorted(rho_list, reverse=1)
    print("Highest rho:",rho_sorted[0])
    maxd = dis[-1]
    delta =[maxd for i in range(len(points))]
    nneigh =[-1for i in range(len(points))]
    for ii in range(1,len(rho_sorted)):
        for jj in range(0,ii):
            id_p1 = rho_sorted[ii][1]#get point1's id
            id_p2 = rho_sorted[jj][1]#get point2's id
            if(distance[id_p1,id_p2]<delta[id_p1]):
                delta[id_p1]= distance[id_p1,id_p2]
                nneigh[id_p1]= id_p2
#assignment
    cl =[-1 for i in range(len(points))]
    colorNum =0
    for ii in range(len(rho_sorted)):
        id_p = rho_sorted[ii][1]
        if(cl[id_p]==-1 and delta[id_p]>7.0):
            cl[id_p]= colorNum
            colorNum =1
        else:
            if(cl[id_p]==-1 and cl[nneigh[id_p]!=-1]):
                cl[id_p]= cl[nneigh[id_p]]
    #print(colorNum)
    #import pylab as pl
    fig1 = pl.figure(1)
    pl.subplot(121)
    #pl.subplot(211)
    drawOriginGraph(pl,points,cl,colorNum)
    pl.subplot(122)
    drawDecisionGraph(pl,rho,delta,cl,colorNum)
    
    pl.show()
    for i in range(len(points)):
        Fout.write(str(i) +"," +str(rho[i])+ "," +str(delta[i])+ "\n")
    #Assign Cluster
    Fin.close()
    Fout.close()
    #if __name__=="__main__":
        #Cluster()
```
测试：
```python
import Cluster
Cluster.Cluster()
```
后记：[发表在 Science 上的一种新聚类算法](https://blog.csdn.net/itplus/article/details/38926837)-
