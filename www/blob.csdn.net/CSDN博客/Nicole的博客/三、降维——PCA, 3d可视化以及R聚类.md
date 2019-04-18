# 三、降维——PCA, 3d可视化以及R聚类 - Nicole的博客 - CSDN博客
2018年04月23日 18:16:13[Nicole_Liang](https://me.csdn.net/weixin_39541558)阅读数：598

导入数据
数据使用的是iris数据包：
```
data(iris)
iris$Species <- factor(iris$Species, levels = c("Versicolor", "virginica", "setosa"))
```
可以使用head(iris)来查看数据情况。
可以简单看看数据的拟合情况：
`round(cor(iris[,1:4]),2`
# 主成分分析PCA
```
pca <- princomp(iris[,1:4], cor=TRUE, scores=TRUE)//选择研究correlation
```
2
通过summary(pc)可以查看计算结果
绘制结果：
```
plot(pc, type="lines")
biplot(pc)
```
# 3D可视化
要想进行3D的研究，可以选择使用rgl包，如果没有的话可以使用install.packages(“rgl”)进行安装
具体实现：
```
library(rgl)
plot3d(pc$scores[,1:3],col=iris$Species)
text3d(pc$scores[,1:3],texts=rownames(iris))
text3d(pc$loadings[,1:3], texts=rownames(pc$loadings), col="red")
coords <- NULL
for (i in 1:nrow(pc$loadings)) {
  coords <- rbind(coords, rbind(c(0,0,0),pc$loadings[i,1:3]))
}
lines3d(coords, col="red", lwd=4)
```
然后就大功告成了~！！！
