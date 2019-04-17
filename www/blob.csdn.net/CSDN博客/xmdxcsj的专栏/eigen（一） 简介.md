# eigen（一） 简介 - xmdxcsj的专栏 - CSDN博客





2016年09月23日 21:52:00[xmucas](https://me.csdn.net/xmdxcsj)阅读数：9113








## 一、概况

[Eigen](http://eigen.tuxfamily.org/index.php?title=Main_Page)是有关线性代数（矩阵、向量等）的c++模板库。支持SSE2/3/4, ARM NEON (32-bit and 64-bit), PowerPC AltiVec/VSX (32-bit and 64-bit) instruction sets, S390x SIMD (ZVector)

### （一）安装

所有的源码在头文件，无需编译

### （二）优化
- 使用[vectorization](http://eigen.tuxfamily.org/index.php?title=FAQ#Vectorization)

x86使用-msse2编译选项 

x86-64默认支持SSE2 

32-bit ARM NEON使用-mfpu=neon -mfloat-abi=softfp|hard 

64-bit ARM SIMD默认支持- 小矩阵（维度为2-4）使用fixed-size
- 注意temporary objects的创建
- [矩阵表达式的写法](http://eigen.tuxfamily.org/dox/TopicWritingEfficientProductExpression.html)

## 二、使用流程

1.包含头文件 
[头文件模块](http://eigen.tuxfamily.org/dox/group__QuickRefPage.html#QuickRef_Types)，简单的矩阵乘法使用#include 

```cpp
#include <iostream>
#include <Eigen/Dense>
using Eigen::MatrixXd;
int main()
{
  MatrixXd m(2,2);
  m(0,0) = 3;
  m(1,0) = 2.5;
  m(0,1) = -1;
  m(1,1) = m(1,0) + m(0,1);
  std::cout << m << std::endl;
}
```

2.编译指定eigen在本机的安装路径
`g++ -I /path/to/eigen/ my_program.cpp -o my_program`
## 三、数据类型

[数据类型](http://eigen.tuxfamily.org/dox/group__QuickRefPage.html#QuickRef_Types)包括两类Matrix和Array。

```cpp
typedef Matrix<Scalar, RowsAtCompileTime, ColsAtCompileTime, Options> MyMatrixType;
typedef Array<Scalar, RowsAtCompileTime, ColsAtCompileTime, Options> MyArrayType;
```

[存储顺序](http://eigen.tuxfamily.org/dox/group__TopicStorageOrders.html)默认使用列存储，可以使用data()函数获得存储数据的首地址.

### 1. Matrix

Matrix

```cpp
//使用case
Matrix<double, 6, Dynamic>                  // Dynamic number of columns (heap allocation)
Matrix<double, Dynamic, 2>                  // Dynamic number of rows (heap allocation)
Matrix<double, Dynamic, Dynamic, RowMajor>  // Fully dynamic, row major (heap allocation)
Matrix<double, 13, 3>                       // Fully fixed (usually allocated on stack)
//typedef简化
Matrix<float,Dynamic,Dynamic>   <=>   MatrixXf
Matrix<double,Dynamic,1>        <=>   VectorXd
Matrix<int,1,Dynamic>           <=>   RowVectorXi
Matrix<float,3,3>               <=>   Matrix3f
Matrix<float,4,1>               <=>   Vector4f
```

### 2.Array

定义类似于Matrix，支持coefficient-wise运算，比如每个元素加一个常数；两个矩阵对应的元素相乘。

```cpp
//typdef 简化
Array<float,Dynamic,Dynamic>    <=>   ArrayXXf
Array<double,Dynamic,1>         <=>   ArrayXd
Array<int,1,Dynamic>            <=>   RowArrayXi
Array<float,3,3>                <=>   Array33f
Array<float,4,1>                <=>   Array4f
```

### 3.Matrix和Array之间相互转化

matrix=>array: matrix的.array()函数 

array=>matrix: array的.matrix()函数 

eigen不允许在表达式中混合使用matrix和array，但是允许使用=进行隐式转化，比如：
```cpp
MatrixXf m(2,2);
MatrixXf n(2,2);
MatrixXf result(2,2);
result = m.array() * n.array();
```

matrix自身有成员函数.cwiseProduct()可以执行coefficient-wise product。

## 四、初始化

### 1.构造函数

```cpp
Vector4d  v4;
Vector2f  v1(x, y);
Array3i   v2(x, y, z);
Vector4d  v3(x, y, z, w);
MatrixXf  m5; // empty object
MatrixXf  m6(nb_rows, nb_columns);
```

### 2.comma

```cpp
Vector3f  v1;     v1 << x, y, z;
ArrayXf   v2(4);  v2 << 1, 2, 3, 4;
Matrix3f  m1;   m1 << 1, 2, 3,
                      4, 5, 6,
                      7, 8, 9;
```

### 3. map

对于原始的数据，可以使用Map复用原来的数据地址。

```cpp
//pf为float*
Map<MatrixXf> mf(pf,rows,columns);
//pi为int*
Map<const Vector4i> mi(pi);
```

使用map

```cpp
float data[] = {1,2,3,4};
Map<Vector3f> v1(data);       // uses v1 as a Vector3f object
Map<ArrayXf>  v2(data,3);     // uses v2 as a ArrayXf object
Map<Array22f> m1(data);       // uses m1 as a Array22f object
Map<MatrixXf> m2(data,2,2);   // uses m2 as a MatrixXf object

float data[] = {1,2,3,4,5,6,7,8,9};
Map<VectorXf,0,InnerStride<2> >  v1(data,3);                      // = [1,3,5]
Map<VectorXf,0,InnerStride<> >   v2(data,3,InnerStride<>(3));     // = [1,4,7]
Map<MatrixXf,0,OuterStride<3> >  m2(data,2,3);                    // both lines     |1,4,7|
Map<MatrixXf,0,OuterStride<> >   m1(data,2,3,OuterStride<>(3));   // are equal to:  |2,5,8
```

map和原始数组之间的相互转化

```cpp
//array=>Matrix
double *x;
MatrixXd m = Map<MatrixXd> (x, rows, cols);
//Matrix=>array
double *y;
MatrixXd n;
Map<MatrixXd>(y, n.rows(), n.cols())=n;
```

## 五、运算

### 1.aliasing

aliasing：如果等号的两边出现同一个matrix或者array，就会引起结果异常。 

1.对于标量乘法、加减法，没有影响

```cpp
mat = 2 * mat;      //no aliasing
arr = arr.square(); //no aliasing
```

2.只有matrix乘法，eigen认定肯定会出现aliasing 

默认引入临时matrix来存放中间结果

```cpp
//1.aliasing
matA=matA*matA
//eigen默认将上式分解成
temp=matA*matA
matA=temp

//2.no aliasing
//使用noalias()来避免上面的分解
matB=matA*matA //仍然会分解，虽然没有必要
matB.noalias()=matA*matA //不会分解
```

3.除了矩阵乘法以外的运算如果会出现aliasing问题，可以使用eval()函数或者xxxInPlace()函数避免

```cpp
Matrix2i a; a << 1, 2, 3, 4;
a = a.transpose(); //error: aliasing
a = a.transpose().eval(); //ok
a.transposeInPlace(); //ok
```

### 2.尽量使用复杂表达式，eigen可以更好地优化

## 六、多线程

### 1.支持多线程

eigen的部分算法（矩阵乘法、PartialPivLU）支持多线程，编译的时候增加-fopenmp，可以通过以下方式设置使用的线程数：

```cpp
OMP_NUM_THREADS=n ./my_program
omp_set_num_threads(n);
Eigen::setNbThreads(n);
```

也可以通过EIGEN_DONT_PARALLELIZE宏定义关闭eigen的多线程。 

如果程序本身使用了多线程，需要使用以下方式初始化eigen

```cpp
#include <Eigen/Core>
int main(int argc, char** argv)
{
  Eigen::initParallel();

  ...
}
```

### 2.使用mkl库

Intel mkl(Math Kernal Library)可以支持更高程度优化的多线程数学计算。eigen使用MKL流程： 

- define the EIGEN_USE_MKL_ALL macro before including any Eigen’s header 

- link your program to MKL libraries (see the MKL linking advisor) 

- on a 64bits system, you must use the LP64 interface (not the ILP64 one) 

MKL的优化适用于Dynamic的matrix，数据类型仅限于float/double/complex/complex，其他类型的数据或者real和complex混用的数据不会使用mkl优化，而是使用内置的算法优化。





