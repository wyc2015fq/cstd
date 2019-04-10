# Horn–Schunck光流算法

 																				2018年01月31日 14:16:19 					[hhyh612](https://me.csdn.net/hhyh612) 						阅读数：2321 										

 									

## 简介

Horn–Schunck光流算法用一种全局方法估计图像的稠密光流场（即对图像中的每个像素计算光流） 
 算法基于两个假设

1. 灰度不变假设 
    物体上同一个点在图像中的灰度是不变的，即使物体发生了运动。（这个假设在稳定光照的情况可以满足，但是对于存在高光反射的图像是不成立的）
2. 光流场平滑假设 
    场景中属于同一物体的像素形成光流场向量应当十分平滑，只有在物体边界的地方才会出现光流的突变，但这只占图像的一小部分。总体来看图像的光流场应当是平滑的。 
    算法构造了一个能量函数，求光流场的问题转化为求能量函数的最小值。

## 数学模型

给定图像序列$I(x,y,t)$，求光流场$\overrightarrow{V}(x,y)$，等价于求光流的两个分量$u(x,y)$和$v(x,y)$ 
 定义能量函数 
$$
E(u,v)=\iint{[(I_xu+I_yv+I_t)^2+\alpha^2({\left\| \nabla u \right\|}^2+{\left\| \nabla v \right\|}^2 )]dxdy}
$$
 其中$I_x,I_y,I_t$分别是图像对x,y,t的导数 

$(I_xu+I_yv+I_t)^2$是灰度变化因子($I_xu+I_yv=-I_t​$就是经典的灰度不变假设) 

$\alpha^2({\left\| \nabla u \right\|}^2+{\left\| \nabla v \right\|}^2 )$是平滑因子，可以理解为限制u,v分量变化的速度 

 一个合理的光流估计，应当是使上述两个因子都尽可能小的光流场 

 这是一个泛函的极值问题，可以用欧拉-拉格朗日方程求解。对应上式的是

[双变量双函数一阶导数的欧拉-拉格朗日方程组](https://en.wikipedia.org/wiki/Euler%E2%80%93Lagrange_equation#Several_functions_of_several_variables_with_single_derivative)
$$
\frac{\partial L}{\partial u}-\frac{\partial}{\partial x}\frac{\partial L}{\partial u_x}-\frac{\partial}{\partial y}\frac{\partial L}{\partial u_y}=0 \\
\frac{\partial L}{\partial v}-\frac{\partial}{\partial x}\frac{\partial L}{\partial v_x}-\frac{\partial}{\partial y}\frac{\partial L}{\partial v_y}=0
$$


∂L∂u−∂∂x∂L∂ux−∂∂y∂L∂uy=0∂L∂v−∂∂x∂L∂vx−∂∂y∂L∂vy=0

 其中$L=(I_xu+I_yv+I_t)^2+\alpha^2({\left\| \nabla u \right\|}^2+{\left\| \nabla v \right\|}^2 )$，求导后可得 
$$
I_x(I_xu+I_yv+I_t)-\alpha ^2 \Delta u=0 \\
I_y(I_xu+I_yv+I_t)-\alpha ^2 \Delta v=0
$$
Δ是拉普拉斯算子，定义为$\Delta=\frac{\partial^2}{\partial x^2}+\frac{\partial^2}{\partial y^2}$，实际运用中可以用近似算法$\Delta u(x,y)=\bar{u}(x,y)-u(x,y)$，论文[1]中使用了以下模板来计算Δu
$$
\frac{1}{12}\left( \begin{matrix}
   1 & 2 & 1  \\
   2 & -12 & 2  \\
   1 & 2 & 1  \\
\end{matrix} \right)
$$
用$\Delta u(x,y)=\bar{u}(x,y)-u(x,y)$替换后，方程组变化为 
$$
(I_x^2+\alpha ^2)u+I_xI_yv=\alpha^2 \bar u-I_xI_t \\
I_xI_yu+(I_y^2+\alpha ^2)v=\alpha^2 \bar v-I_yI_t \\
$$


 这是一个线性方程组，但是由于需要首先计算$\bar u$和$\bar v$，因此可以使用迭代法来求解，迭代公式为 
$$
u^{k+1}=\bar u^k-\frac{I_x(I_x\bar u^k+I_y\bar v^k+I_t)}{\alpha ^2+I_x^2+I_y^2} \\
v^{k+1}=\bar v^k-\frac{I_y(I_x\bar u^k+I_y\bar v^k+I_t)}{\alpha ^2+I_x^2+I_y^2} \\
$$


## 实现

## 参考资料

1. B.K.P. Horn and B.G. Schunck, “Determining optical flow.” 1981
2. <https://en.wikipedia.org/wiki/Horn%E2%80%93Schunck_method>