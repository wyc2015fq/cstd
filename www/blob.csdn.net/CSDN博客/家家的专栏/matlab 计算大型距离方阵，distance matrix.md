# matlab 计算大型距离方阵，distance matrix - 家家的专栏 - CSDN博客





2014年02月18日 15:07:02[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：2465








利用vlfeat工具包可行。

网址见：http://www.vlfeat.org/matlab/vl_alldist2.html

返回： pairwise distance matrix D of the columns of S1 and S2, yielding




D = [VL_ALLDIST2](http://www.vlfeat.org/matlab/vl_alldist2.html)(X,Y) returns the pairwise distance matrix D of the columns of S1 and S2, yielding
  D(i,j) = sum (X(:,i) - Y(:,j)).^2

[VL_ALLDIST2](http://www.vlfeat.org/matlab/vl_alldist2.html)(X) returns the pairwise distance matrix fo the columns of S, yielding
  D(i,j) = sum (X(:,i) - X(:,j)).^2

[VL_ALLDIST2](http://www.vlfeat.org/matlab/vl_alldist2.html)(...,'METRIC') changes the computed distance. Supported values for METRIC are
 METRIC  D(i,j)
 --------------------------------------------------------
  LINF   max |X  - Y|
  L2     sum (X  - Y).^2
  L1     sum |X  - Y|
  L0     sum (X ~= Y)
  CHI2   sum (X  - Y).^2 ./ (X + Y)
  HELL   sum (X^.5 - Y^.5) .^ 2


(Notice that the standard definition of chi2 is half of what is computed here).

[VL_ALLDIST2](http://www.vlfeat.org/matlab/vl_alldist2.html)(...,'KERNEL') computes the following 'kernels' K:
 KERNEL  K(i,j)
 ---------------------------------------------------------
  KL2    sum X .* Y
  KL1    sum min (X, Y)
  KCHI2  2 * sum (X .* Y) ./ (X + Y)
  KHELL  (X .* Y) .^ 0.5


The constant are chosen so that D(i,j) = K(i,i) + K(j,j) - 2 K(i,j) where D is the metric corresponding to the kenrel (if the arguments are non-negative vectors). Each kernel can be interpreted as the inner product inducing the corresponding metric in an embedding
 of the real space into an approrpiate reproducing Kenrel Hilbert space.

[VL_ALLDIST2](http://www.vlfeat.org/matlab/vl_alldist2.html)() supports several storage classes. X and Y must have the same storage class. The sotrage class of D is promoted to reduce the chance of overvlow,
 but this is not checked.

转自：http://www.vlfeat.org/matlab/vl_alldist2.html

          http://www.vlfeat.org/matlab/matlab.html



