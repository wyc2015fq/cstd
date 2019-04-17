# kaldi-matrix - xmdxcsj的专栏 - CSDN博客





2016年03月02日 21:05:33[xmucas](https://me.csdn.net/xmdxcsj)阅读数：1883








kaldi的矩阵运算是对BLAS和BAPACK进行的封装，封装后的类列表（可以查看每一个类型的详细定义）参考：

[](http://kaldi.sourceforge.net/group__matrix__group.html)[http://kaldi.sourceforge.net/group__matrix__group.html](http://kaldi.sourceforge.net/group__matrix__group.html)

一、分类

（一）、matrix和vector

Matrix矩阵，Vector向量，父类分别是MatrixBase和VectorBase

1.初始化

Vector<float> v(10), w(9);

for(int[i](http://kaldi.sourceforge.net/namespacernnlm.html#acb559820d9ca11295b4500f179ef6392)=0; [i](http://kaldi.sourceforge.net/namespacernnlm.html#acb559820d9ca11295b4500f179ef6392) < 9; [i](http://kaldi.sourceforge.net/namespacernnlm.html#acb559820d9ca11295b4500f179ef6392)++) {

v([i](http://kaldi.sourceforge.net/namespacernnlm.html#acb559820d9ca11295b4500f179ef6392)) =
[i](http://kaldi.sourceforge.net/namespacernnlm.html#acb559820d9ca11295b4500f179ef6392);

w([i](http://kaldi.sourceforge.net/namespacernnlm.html#acb559820d9ca11295b4500f179ef6392)) =
[i](http://kaldi.sourceforge.net/namespacernnlm.html#acb559820d9ca11295b4500f179ef6392)+1;

}

Matrix<float> M(10,9);

2.运算

（1）![\[ M \leftarrow M + \alpha v w^T \]](http://kaldi.sourceforge.net/form_23.png)

M.AddVecVec(1.0, v, w);

（2） ![\[ A = \alpha B C^T + \beta A \]](http://kaldi.sourceforge.net/form_24.png)

A.AddMatMat(alpha, B,[kNoTrans](http://kaldi.sourceforge.net/namespacekaldi.html#a610f4f6c3c8fe58b76713f593b9d724fa6decd3b781a809d748e5221d49f0ab5d), C,[kTrans](http://kaldi.sourceforge.net/namespacekaldi.html#a610f4f6c3c8fe58b76713f593b9d724fa538d6ef272b2ad113f1860fccbc2c3e2), beta);

kNoTrans和kTrans表示是否对矩阵进行转置

（3）求秩

Matrix<float> M(5, 10), N(5, 10), P(5, 5);

// initialize M and N somehow...

// next line: P := 1.0 * M * N^T + 0.0 * P.

P.AddMatMat(1.0, M, [kNoTrans](http://kaldi.sourceforge.net/namespacekaldi.html#a610f4f6c3c8fe58b76713f593b9d724fa6decd3b781a809d748e5221d49f0ab5d), N, [kTrans](http://kaldi.sourceforge.net/namespacekaldi.html#a610f4f6c3c8fe58b76713f593b9d724fa538d6ef272b2ad113f1860fccbc2c3e2), 0.0);

// tr(M N^T)

float f = [TraceMatMat](http://kaldi.sourceforge.net/namespacekaldi.html#a94b9f349754706b9d6f38bb5ccc11c77)(M, N, [kTrans](http://kaldi.sourceforge.net/namespacekaldi.html#a610f4f6c3c8fe58b76713f593b9d724fa538d6ef272b2ad113f1860fccbc2c3e2)),

g = P.Trace();

[KALDI_ASSERT](http://kaldi.sourceforge.net/group__error__group.html#gad5710173d69cddcda4fa21ded3c77f16)(f == g);
// we use this macro for asserts in Kaldi

// code (prints stack trace and throws exception).




（二）、对称矩阵和三角矩阵

SpMatrix 对称矩阵，TpMatrix 三角矩阵，共同的父类是PackedMatrix

Matrix<float> feats(1000, 39);

// ... initialize feats somehow ...

SpMatrix<float> scatter(39);

// next line does scatter = 0.001 * feats' * feats.

scatter.AddMat2(1.0/1000, feats, [kTrans](http://kaldi.sourceforge.net/namespacekaldi.html#a610f4f6c3c8fe58b76713f593b9d724fa538d6ef272b2ad113f1860fccbc2c3e2), 0.0);

TpMatrix<float> cholesky(39);

cholesky.Cholesky(scatter);

cholesky.Invert();

Matrix<float> whitened_feats(1000, 39);

// If scatter = C C^T, next line does:

// whitened_feats = feats * C^{-T}

whitened_feats.AddMatTp(1.0, feats, [kNoTrans](http://kaldi.sourceforge.net/namespacekaldi.html#a610f4f6c3c8fe58b76713f593b9d724fa6decd3b781a809d748e5221d49f0ab5d), cholesky, [kTrans](http://kaldi.sourceforge.net/namespacekaldi.html#a610f4f6c3c8fe58b76713f593b9d724fa538d6ef272b2ad113f1860fccbc2c3e2), 0.0);




（三）、子矩阵和子向量

SubMatrix子矩阵，SubVector子向量，父类分别是MatrixBase和VectorBase。两个没有resize函数，因为是指向原来矩阵的指针，没有单独的存储空间。而且使用的时候，如果原始的矩阵或者向量空间被释放，将会导致子矩阵和子向量指向freed memory；由于SubVector不是const类型，所以如果原始的Vector是const类型的话，有可能被SubVector改变，这算是kadli不合理的地方。

1.初始化

Vector<float> v(10), w(10);

Matrix<float> M(10, 10);

SubVector<float> vs(v, 1, 9), ws(w, 1, 9);

SubMatrix<float> Ms(M, 1, 9, 1, 9);

2.运算

vs.AddMatVec(1.0, Ms,[kNoTrans](http://kaldi.sourceforge.net/namespacekaldi.html#a610f4f6c3c8fe58b76713f593b9d724fa6decd3b781a809d748e5221d49f0ab5d), ws);

3.转化

M是矩阵，那么M.Row(3)将返回第三行作为SubVector，但是不支持列column，因为vector要求在内存的存储上面是连续。还可以使用range进行转化

// get a sub-vector of length 5 starting from position 0; zero it.

v.Range(0, 5).SetZero();

// get a sub-matrix of size 2x2 starting from position (5,5); zero it.

M.Range(5, 2, 5, 2).SetZero();




二、函数调用

当函数需要vector和matrix类型的时候，一般使用VectorBase和MatrixBase，因为这样我们可以传入Vector和Matrix，也可以传入SubVector和SubMatrix。

例外情况是，如果是需要resize矩阵和向量的话，需要传入Vector<BaseFloat>*和Matrix<BaseFloat>*。

三、其他运算

1.复制

具体查看Matrix和Vector的copy函数，可以实现矩阵和向量之间的相互转化。

2.内积

对于输出是标量的运算，是通过单独的函数定义的，没有通过类实现，具体参考：

[http://kaldi.sourceforge.net/group__matrix__funcs__scalar.html](http://kaldi.sourceforge.net/group__matrix__funcs__scalar.html)

3.fft或者矩阵求幂

[http://kaldi.sourceforge.net/group__matrix__funcs__misc.html](http://kaldi.sourceforge.net/group__matrix__funcs__misc.html)

4.resize

除了SubMatrix和SubVector以外的矩阵和向量，都可以进行resize操作。

Vector<float> v;

Matrix<float> M;

SpMatrix<float> S;

v.Resize(10);

M.Resize(5, 10);

S.Resize(10);

Resize函数会把多出来的部分默认设为0，可以通过以下三个参数控制：

- [kSetZero](http://kaldi.sourceforge.net/namespacekaldi.html#ab4ace1fea44c7724ab9a76d70abad80bae9d0df3a51f0394dd8e041960265c112) (the default): sets the data
 to zero
- [kUndefined](http://kaldi.sourceforge.net/namespacekaldi.html#ab4ace1fea44c7724ab9a76d70abad80baf2cd3231820315500697550960956126) : leaves the data undefined
- [kCopyData](http://kaldi.sourceforge.net/namespacekaldi.html#ab4ace1fea44c7724ab9a76d70abad80ba6ccfb1220c1b49764f14603227edd6e0) : Copies any old data that
 shared the same index as the new data, leaving new indices at zero.


四、矩阵I/O

bool binary = 
false;

std::ofstream os( ... );

Vector<float> v;

v.Write(os, binary);

...

std::ifstream is( ... );

Vector<float> v;

v.Read(is, binary);

也可以使用<<或者>>来完成，不过这种方式只支持text，不支持binary。



