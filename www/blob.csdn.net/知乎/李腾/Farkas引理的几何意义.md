# Farkas引理的几何意义 - 知乎
# 

> Farkas引理是最优化理论中重要的基石定理，与“对偶”这个概念有着深刻的联系。Farkas引理的证明常利用凸集分离定理（凸集分离定理本身也有比较直观的几何意义），过程比较抽象，然而实际上Farkas引理的几何意义相当清晰，从几何上理解很容易记忆，并大大增强使用时的“自然”的感觉

## **Farkas引理**

设A是一个mxn阶的实矩阵，b是一个n维实向量，则下述两组方程中仅有一组有解：

![Ax\leq 0,\quad b^Tx>0, \quad (1)](https://www.zhihu.com/equation?tex=Ax%5Cleq+0%2C%5Cquad+b%5ETx%3E0%2C+%5Cquad+%281%29)

![A^Ty=b, \quad y\geq 0, \quad (2)](https://www.zhihu.com/equation?tex=A%5ETy%3Db%2C+%5Cquad+y%5Cgeq+0%2C+%5Cquad+%282%29)

其中x是n维实向量，y是m维实向量

## **几何解释**
- 请见顶图，在空间 ![R^n](https://www.zhihu.com/equation?tex=R%5En) 中，用矩阵A的行张成一个锥（张成锥与张成线性空间的区别就在于前者只能用非负的组合系数），即图中的锥 ![OA_1A2](https://www.zhihu.com/equation?tex=OA_1A2) （阴影部分）
- 那么b的位置只可能有两种情形：（1）落在锥外；（2）落在锥内（含边界）
- 若b落在锥外：那么因为b点和A的行锥均为凸集，所以恰可利用凸集分离定理直接得到方程（1）
- 落b落在锥内：则b可以用A的行以非负系数线性表出，这恰是方程（2）
- 其实经典证明背后的思想就一句话：**b点要么落在锥外，要么落在锥内**


