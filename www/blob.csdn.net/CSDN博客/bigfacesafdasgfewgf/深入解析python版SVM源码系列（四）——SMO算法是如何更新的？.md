# 深入解析python版SVM源码系列（四）——SMO算法是如何更新的？ - bigfacesafdasgfewgf - CSDN博客





2015年03月24日 10:50:41[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：1897








SMO算法在SVM源码中的更新步骤是最为重要的，如下：

```
#是否可以继续优化
            if ((labelMat[i]*Ei < -toler) and (alphas[i] < C)) or ((labelMat[i]*Ei > toler) and (alphas[i] > 0)):
                j = selectJrand(i,m)  # 随机选择第j个样本
                fXj = float(multiply(alphas,labelMat).T*(dataMatrix*dataMatrix[j,:].T)) + b  # 样本j的预测类别
                Ej = fXj - float(labelMat[j])  # 误差

                alphaIold = alphas[i].copy();  # 拷贝，分配新的内存
                alphaJold = alphas[j].copy();

                if (labelMat[i] != labelMat[j]):
                    L = max(0, alphas[j] - alphas[i])
                    H = min(C, C + alphas[j] - alphas[i])
                else:
                    L = max(0, alphas[j] + alphas[i] - C)
                    H = min(C, alphas[j] + alphas[i])

                if L==H: print "L==H"; continue

                eta = 2.0 * dataMatrix[i,:]*dataMatrix[j,:].T - dataMatrix[i,:]*dataMatrix[i,:].T - dataMatrix[j,:]*dataMatrix[j,:].T

                if eta >= 0: print "eta>=0"; continue

                alphas[j] -= labelMat[j]*(Ei - Ej)/eta
                alphas[j] = clipAlpha(alphas[j],H,L)  # 门限函数阻止alpha_j的修改量过大

                #如果修改量很微小
                if (abs(alphas[j] - alphaJold) < 0.00001): print "j not moving enough"; continue

                # alpha_i的修改方向相反
                alphas[i] += labelMat[j]*labelMat[i]*(alphaJold - alphas[j])#update i by the same amount as j
                                                                        #the update is in the oppostie direction
                # 为两个alpha设置常数项b
                b1 = b - Ei- labelMat[i]*(alphas[i]-alphaIold)*dataMatrix[i,:]*dataMatrix[i,:].T - labelMat[j]*(alphas[j]-alphaJold)*dataMatrix[i,:]*dataMatrix[j,:].T
                b2 = b - Ej- labelMat[i]*(alphas[i]-alphaIold)*dataMatrix[i,:]*dataMatrix[j,:].T - labelMat[j]*(alphas[j]-alphaJold)*dataMatrix[j,:]*dataMatrix[j,:].T
                if (0 < alphas[i]) and (C > alphas[i]): b = b1
                elif (0 < alphas[j]) and (C > alphas[j]): b = b2
                else: b = (b1 + b2)/2.0

                # 说明alpha已经发生改变
                alphaPairsChanged += 1
                print "iter: %d i:%d, pairs changed %d" % (iter,i,alphaPairsChanged)
```

这个算法步骤是这样子的：

Step 1：首先随机选择两个alpha_i和alpha_j，如果它们满足： 
![这里写图片描述](https://img-blog.csdn.net/20150324104553569)

说明它们可以进行优化，也就是不满足KKT条件。
对应的源码部分是：
`if ((labelMat[i]*Ei < -toler) and (alphas[i] < C)) or ((labelMat[i]*Ei > toler) and (alphas[i] > 0)):`
Step 2：如果选择的两个alpha是可以更新优化的，那么我们使用下面的算法（SMO的核心）来学习两个新的alpha：

![这里写图片描述](https://img-blog.csdn.net/20150324104932048)

参考：

[http://blog.csdn.net/v_july_v/article/details/7624837](http://blog.csdn.net/v_july_v/article/details/7624837)




