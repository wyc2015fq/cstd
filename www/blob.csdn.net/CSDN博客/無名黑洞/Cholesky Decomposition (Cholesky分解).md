# Cholesky Decomposition (Cholesky分解) - 無名黑洞 - CSDN博客
2011年12月01日 14:42:42[wangxiaojun911](https://me.csdn.net/wangxiaojun911)阅读数：37703
**简介**
Cholesky分解是一种分解矩阵的方法, 在线形代数中有重要的应用。Cholesky分解把矩阵分解为一个下三角矩阵以及它的共轭转置矩阵的乘积（那实数界来类比的话，此分解就好像求平方根）。与一般的矩阵分解求解方程的方法比较，Cholesky分解效率很高。
Cholesky是生于19世纪末的法国数学家，曾就读于巴黎综合理工学院。Cholesky分解是他在学术界最重要的贡献。后来，Cholesky参加了法国军队，不久在一战初始阵亡。
**Cholesky分解的条件**
一、Hermitianmatrix：矩阵中的元素共轭对称（复数域的定义，类比于实数对称矩阵）。Hermitiank意味着对于任意向量x和y，(x*)Ay共轭相等
二、Positive-definite：正定（矩阵域，类比于正实数的一种定义）。正定矩阵A意味着，对于任何向量x，(x^T)Ax总是大于零(复数域是(x*)Ax>0)
**Cholesky分解的形式**
可记作A = L L*。其中L是下三角矩阵。L*是L的共轭转置矩阵。
可以证明，只要A满足以上两个条件，L是唯一确定的，而且L的对角元素肯定是正数。反过来也对，即存在L把A分解的话，A满足以上两个条件。
如果A是半正定的（semi-definite），也可以分解，不过这时候L就不唯一了。
特别的，如果A是实数对称矩阵，那么L的元素肯定也是实数。
另外，满足以上两个条件意味着A矩阵的特征值都为正实数，因为Ax = lamda * x,
(x*)Ax = lamda * (x*)x > 0, lamda > 0
**Cholesky分解的方式**
.
可以使用高斯消元法分解矩阵。过程如下：
设A = | a11    w*|
         | w      K|
= (R1*) * | 1                 0 | * R1
| 0  K – w(w*)/a11|
其中，
R1* = | 1                   0| 
| w/sqrt(a11)         I |
R1 = | 1   w/sqrt(a11)| 
| 0              I |
如果K –w(w*)/a11大于零，那么就可以一直分解下去。因为它是一个正定矩阵的子矩阵，所以肯定可以满足。
最后：
R* = (R1*)(R2*)…(Rm*)
R  =(Rm)…(R2)(R1)
因为矩阵的一半元素很相似，所以算法只需要实现一半就可以了。数据也可以只用一半，这样就节约了很多时间。同样，输出只需要一个上三角矩阵就可以了。
**Cholesky分解的算法实现**
R = A
For (k = 0, k < m; k++){
       For(j = k; j < m; j++){
Rj,j:m = Rj,j:m – Rk,j:m Rkj / Rkk            -(1)
}
Rk,k:m = Rk,k:m/ sqrt(Rkk)
}
为了节约数据空间，其中，A仅初始化为上三角矩阵。Cholesky分解的效率分析, 由于(1) 式占用了O（m）的时间，所以总计O（m^3 / 3）。 (参考文献一)
在GPU上可实现为：
```cpp
__global__ void Core(float *R, int k, int j, int m){//Parallel portion of Cholesky Algorithm
	int tid = j + threadIdx.x + blockIdx.x * blockDim.x;
	while(tid < m){ 
		float Rkj = R[k * m + j];
		float Rkk = R[k * m + k];
		R[j*m + tid] = R[j*m + tid] - R[k*m + tid] * Rkj / (Rkk);
		tid += blockDim.x * gridDim.x;
	}
}
__global__ void Core2(float *R, int k, int m){//Parallel portion of Cholesky Algorithm
	int tid = k + threadIdx.x + blockIdx.x * blockDim.x;
	while(tid < m){ 
		float Rkk = R[k * m + k];
		R[k*m + tid] = R[k*m + tid] / sqrt(Rkk);
		tid += blockDim.x * gridDim.x;
	}
}
void CholeskyAlgorithm(float *R, int m){
	for(int k = 0; k < m-1; k++){ 
		for(int j = k+1; j < m; j++) 
			Core<<<blocksPerGrid, threadsPerBlock>>>(R, k, j, m);	
		Core2<<<blocksPerGrid, threadsPerBlock>>>(R, k, m);
	}
}
```
此方法把效率提高到O(N^2)。更进一步地，可以在GPU上把运算时间提高到线性，如下：
```cpp
__global__ void Core(float *R, int k, int m){
	int tid = threadIdx.x + blockIdx.x * blockDim.x;
	while( tid < (m-k-1)*(m-k-1) ){ 
		int j = tid / (m-k-1);
		int l = tid % (m-k-1);
		if(j <= l ){
			j += (k+1);
			l += (k+1);
			float Rkj = R[k * m + j];
			float Rkk = R[k * m + k];
			R[j*m + l] = R[j*m + l] - R[k*m + l] * Rkj / (Rkk);
		}
		tid += blockDim.x * gridDim.x;
	}
}
__global__ void Core2(float *R, int k, int m){
	int tid = k + threadIdx.x + blockIdx.x * blockDim.x;
	while(tid < m){ 
		float Rkk = R[k * m + k];
		R[k*m + tid] = R[k*m + tid] / sqrt(Rkk);
		tid += blockDim.x * gridDim.x;
	}
}
void CholeskyAlgorithm(float *R, int m){
	for(int k = 0; k < m-1; k++){ 
		Core<<<blocksPerGrid, threadsPerBlock>>>(R, k, m);	
		Core2<<<blocksPerGrid, threadsPerBlock>>>(R, k, m);
	}
}
```
**参考文献：**
一. Trefethen，David.Bau，Lloyd，Numerical.Linear.Algebra..
二. http://www.wikipedia.org/
