# matlab 矩阵分解 - 家家的专栏 - CSDN博客





2012年02月15日 10:36:50[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：10771








矩阵分解是指根据一定的原理用某种算法将一个矩阵分解成若干个矩阵的乘积。常见的矩阵分解有LU分解、QR分解、Cholesky分解，以及Schur分解、Hessenberg分解、奇异分解等。


(1) LU分解

矩阵的LU分解就是将一个矩阵表示为一个交换下三角矩阵和一个上三角矩阵的乘积形式。线性代数中已经证明，只要方阵A是非奇异的，LU分解总是可以进行的。

MATLAB提供的lu函数用于对矩阵进行LU分解，其调用格式为：

[L,U]=lu(X)：产生一个上三角阵U和一个变换形式的下三角阵L(行交换)，使之满足X=LU。注意，这里的矩阵X必须是方阵。

[L,U,P]=lu(X)：产生一个上三角阵U和一个下三角阵L以及一个置换矩阵P，使之满足PX=LU。当然矩阵X同样必须是方阵。

实现LU分解后，线性方程组Ax=b的解x=U\(L\b)或x=U\(L\Pb)，这样可以大大提高运算速度。


例7-2  用LU分解求解例7-1中的线性方程组。

命令如下：

A=[2,1,-5,1;1,-5,0,7;0,2,1,-1;1,6,-1,-4];

b=[13,-9,6,0]';

[L,U]=lu(A);

x=U\(L\b)

或采用LU分解的第2种格式，命令如下：

[L,U ,P]=lu(A);

x=U\(L\P*b)


(2) QR分解

对矩阵X进行QR分解，就是把X分解为一个正交矩阵Q和一个上三角矩阵R的乘积形式。QR分解只能对方阵进行。MATLAB的函数qr可用于对矩阵进行QR分解，其调用格式为：

[Q,R]=qr(X)：产生一个一个正交矩阵Q和一个上三角矩阵R，使之满足X=QR。

[Q,R,E]=qr(X)：产生一个一个正交矩阵Q、一个上三角矩阵R以及一个置换矩阵E，使之满足XE=QR。

实现QR分解后，线性方程组Ax=b的解x=R\(Q\b)或x=E(R\(Q\b))。


例7-3  用QR分解求解例7-1中的线性方程组。

命令如下：

A=[2,1,-5,1;1,-5,0,7;0,2,1,-1;1,6,-1,-4];

b=[13,-9,6,0]';

[Q,R]=qr(A);

x=R\(Q\b)

或采用QR分解的第2种格式，命令如下：

[Q,R,E]=qr(A);

x=E*(R\(Q\b))


(3) Cholesky分解

如果矩阵X是对称正定的，则Cholesky分解将矩阵X分解成一个下三角矩阵和上三角矩阵的乘积。设上三角矩阵为R，则下三角矩阵为其转置，即X=R'R。MATLAB函数chol(X)用于对矩阵X进行Cholesky分解，其调用格式为：

R=chol(X)：产生一个上三角阵R，使R'R=X。若X为非对称正定，则输出一个出错信息。

[R,p]=chol(X)：这个命令格式将不输出出错信息。当X为对称正定的，则p=0，R与上述格式得到的结果相同；否则p为一个正整数。如果X为满秩矩阵，则R为一个阶数为q=p-1的上三角阵，且满足R'R=X(1:q,1:q)。

实现Cholesky分解后，线性方程组Ax=b变成R‘Rx=b，所以x=R\(R’\b)。


例7-4  用Cholesky分解求解例7-1中的线性方程组。

命令如下：

A=[2,1,-5,1;1,-5,0,7;0,2,1,-1;1,6,-1,-4];

b=[13,-9,6,0]';

R=chol(A)

??? Error using ==> chol

Matrix must be positive definite

命令执行时，出现错误信息，说明A为非正定矩阵。

转自：http://203.208.37.104/search?q=cache:EfIWKymoWB8J:www.math.org.cn/forums/index.php%3Fact%3DAttach%26type%3Dpost%26id%3D213920+matlab+%E7%9F%A9%E9%98%B5LU%E5%88%86%E8%A7%A3&hl=zh-CN&ct=clnk&cd=15&gl=cn&client=firefox-a&st_usg=ALhdy2-W-UGiapmEd7-JkiCNACw5NK2Gew






