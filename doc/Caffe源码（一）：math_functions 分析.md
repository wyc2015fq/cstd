# Caffe源码（一）：math_functions 分析

2015年08月09日 19:15:44

 

seven_first

 

阅读数：70387

 

标签： [caffe源码](https://so.csdn.net/so/search/s.do?q=caffe%E6%BA%90%E7%A0%81&t=blog)[math](https://so.csdn.net/so/search/s.do?q=math&t=blog)[function](https://so.csdn.net/so/search/s.do?q=function&t=blog) 更多

个人分类： [caffe源码解析](https://blog.csdn.net/seven_first/article/category/5721883)



# 目录



- 目录
  - 主要函数
    - [caffe_cpu_gemm 函数](https://blog.csdn.net/seven_first/article/details/47378697#1-caffecpugemm-%E5%87%BD%E6%95%B0)
    - [caffe_cpu_gemv 函数](https://blog.csdn.net/seven_first/article/details/47378697#2-caffecpugemv-%E5%87%BD%E6%95%B0)
    - [caffe_axpy 函数](https://blog.csdn.net/seven_first/article/details/47378697#3caffeaxpy-%E5%87%BD%E6%95%B0)
    - [caffe_set 函数](https://blog.csdn.net/seven_first/article/details/47378697#4caffeset-%E5%87%BD%E6%95%B0)
    - [caffe_add_scalar 函数](https://blog.csdn.net/seven_first/article/details/47378697#5caffeaddscalar-%E5%87%BD%E6%95%B0)
    - [caffe_copy 函数](https://blog.csdn.net/seven_first/article/details/47378697#6caffecopy-%E5%87%BD%E6%95%B0)
    - [caffe_scal 函数](https://blog.csdn.net/seven_first/article/details/47378697#7caffescal-%E5%87%BD%E6%95%B0)
    - [caffeine_cup_axpby 函数](https://blog.csdn.net/seven_first/article/details/47378697#8caffeinecupaxpby-%E5%87%BD%E6%95%B0)
    - [caffe_add caffe_sub caffe_mul caffe_div 函数](https://blog.csdn.net/seven_first/article/details/47378697#9caffeadd-caffesub-caffemul-caffediv-%E5%87%BD%E6%95%B0)
    - [caffe_powx caffe_sqr caffe_exp caffe_abs 函数](https://blog.csdn.net/seven_first/article/details/47378697#10caffepowx-caffesqr-caffeexp-caffeabs-%E5%87%BD%E6%95%B0)
    - [int caffe_rng_rand 函数](https://blog.csdn.net/seven_first/article/details/47378697#11int-cafferngrand-%E5%87%BD%E6%95%B0)
    - [caffe_nextafer 函数](https://blog.csdn.net/seven_first/article/details/47378697#12caffenextafer-%E5%87%BD%E6%95%B0)
    - [caffe_cpu_strided_dot 函数](https://blog.csdn.net/seven_first/article/details/47378697#13caffecpustrideddot-%E5%87%BD%E6%95%B0)
    - [caffe_cpu_hamming_distance 函数](https://blog.csdn.net/seven_first/article/details/47378697#14caffecpuhammingdistance-%E5%87%BD%E6%95%B0)
    - [caffe_cpu_asum 函数](https://blog.csdn.net/seven_first/article/details/47378697#15-caffecpuasum-%E5%87%BD%E6%95%B0)
    - [caffe_cpu_scale 函数](https://blog.csdn.net/seven_first/article/details/47378697#16caffecpuscale-%E5%87%BD%E6%95%B0)



## 主要函数

math_function 定义了caffe 中用到的一些矩阵操作和数值计算的一些函数，这里以float类型为例做简单的分析

### 1. caffe_cpu_gemm 函数：

```
template<>
void caffe_cpu_gemm<float>(const CBLAS_TRANSPOSE TransA,
    const CBLAS_TRANSPOSE TransB, const int M, const int N, const int K,
    const float alpha, const float* A, const float* B, const float beta,
    float* C) {
  int lda = (TransA == CblasNoTrans) ? K : M;
  int ldb = (TransB == CblasNoTrans) ? N : K;
  cblas_sgemm(CblasRowMajor, TransA, TransB, M, N, K, alpha, A, lda, B,
      ldb, beta, C, N);
}12345678910
```

功能： C=alpha*A*B+beta*C 
A,B,C 是输入矩阵（一维数组格式） 
CblasRowMajor :数据是行主序的（二维数据也是用一维数组储存的） 
TransA, TransB：是否要对A和B做转置操作（CblasTrans CblasNoTrans） 
M： A、C 的行数 
N： B、C 的列数 
K： A 的列数， B 的行数 
lda ： A的列数（不做转置）行数（做转置） 
ldb： B的列数（不做转置）行数（做转置）

### 2. caffe_cpu_gemv 函数：

```
template <>
void caffe_cpu_gemv<float>(const CBLAS_TRANSPOSE TransA, const int M,
    const int N, const float alpha, const float* A, const float* x,
    const float beta, float* y) {
  cblas_sgemv(CblasRowMajor, TransA, M, N, alpha, A, N, x, 1, beta, y, 1);
}123456
```

功能： y=alpha*A*x+beta*y 
其中X和Y是向量，A 是矩阵 
M：A 的行数 
N：A 的列数 
cblas_sgemv 中的 参数1 表示对X和Y的每个元素都进行操作

### 3.caffe_axpy 函数：

```
template <>
void caffe_axpy<float>(const int N, const float alpha, const float* X,
    float* Y) { cblas_saxpy(N, alpha, X, 1, Y, 1); }123
```

功能： Y=alpha*X+Y 
N：为X和Y中element的个数

### 4.caffe_set 函数：

```
template <typename Dtype>
void caffe_set(const int N, const Dtype alpha, Dtype* Y) {
  if (alpha == 0) {
    memset(Y, 0, sizeof(Dtype) * N);  // NOLINT(caffe/alt_fn)
    return;
  }
  for (int i = 0; i < N; ++i) {
    Y[i] = alpha; 
  }
}12345678910
```

功能：用常数 alpha 对 Y 进行初始化 
函数 void *memset(void *buffer, char c, unsigned count) 一般为新申请的内存做初始化，功能是将buffer所指向内存中的每个字节的内容全部设置为c指定的ASCII值, count为块的大小

### 5.caffe_add_scalar 函数：

```
template <>
void caffe_add_scalar(const int N, const float alpha, float* Y) {
  for (int i = 0; i < N; ++i) {
    Y[i] += alpha;
  }
}123456
```

功能： 给 Y 的每个 element 加上常数 alpha

### 6.caffe_copy 函数：

```
template <typename Dtype>
void caffe_copy(const int N, const Dtype* X, Dtype* Y) {
  if (X != Y) {
    if (Caffe::mode() == Caffe::GPU) {
#ifndef CPU_ONLY
      // NOLINT_NEXT_LINE(caffe/alt_fn)
      CUDA_CHECK(cudaMemcpy(Y, X, sizeof(Dtype) * N, cudaMemcpyDefault));
#else
      NO_GPU;
#endif
    } else {
      memcpy(Y, X, sizeof(Dtype) * N);  // NOLINT(caffe/alt_fn)
    }
  }
}123456789101112131415
```

函数 void *memcpy(void *dest, void *src, unsigned int count) 把src所指向的内存区域 copy到dest所指向的内存区域, count为块的大小

### 7.caffe_scal 函数：

```
template <>
void caffe_scal<float>(const int N, const float alpha, float *X) {
  cblas_sscal(N, alpha, X, 1);
}
12345
```

功能：X = alpha*X 
N： X中element的个数

### 8.caffeine_cup_axpby 函数：

```
template <>
void caffe_cpu_axpby<float>(const int N, const float alpha, const float* X,
                            const float beta, float* Y) {
  cblas_saxpby(N, alpha, X, 1, beta, Y, 1);
}12345
```

功能：Y= alpha*X+beta*Y

### 9.caffe_add、 caffe_sub、 caffe_mul、 caffe_div 函数：

```
template <>
void caffe_add<float>(const int n, const float* a, const float* b,
    float* y) {
  vsAdd(n, a, b, y);
}
template <>
void caffe_sub<float>(const int n, const float* a, const float* b,
    float* y) {
  vsSub(n, a, b, y);
}

template <>
void caffe_mul<float>(const int n, const float* a, const float* b,
    float* y) {
  vsMul(n, a, b, y);
}

template <>
void caffe_div<float>(const int n, const float* a, const float* b,
    float* y) {
  vsDiv(n, a, b, y);
}

123456789101112131415161718192021222324
```

功能：这四个函数分别实现element-wise的加减乘除（y[i] = a[i] + - * \ b[i]）

### 10.caffe_powx、 caffe_sqr、 caffe_exp、 caffe_abs 函数：

```
template <>
void caffe_powx<float>(const int n, const float* a, const float b,
    float* y) {
  vsPowx(n, a, b, y);
}

template <>
void caffe_sqr<float>(const int n, const float* a, float* y) {
  vsSqr(n, a, y);
}


template <>
void caffe_exp<float>(const int n, const float* a, float* y) {
  vsExp(n, a, y);
}

template <>
void caffe_abs<float>(const int n, const float* a, float* y) {
    vsAbs(n, a, y);
}
12345678910111213141516171819202122
```

功能 : 同样是element-wise操作，分别是y[i] = a[i] ^ b， y[i] = a[i]^2，y[i] = exp(a[i] )，y[i] = |a[i] |

### 11.int caffe_rng_rand 函数：

```
unsigned int caffe_rng_rand() {
  return (*caffe_rng())();
}123
```

功能：返回一个随机数

### 12.caffe_nextafer 函数：

```
template <typename Dtype>
Dtype caffe_nextafter(const Dtype b) {
  return boost::math::nextafter<Dtype>(
      b, std::numeric_limits<Dtype>::max());
}12345
```

功能 ： 返回 b 最大方向上可以表示的最接近的数值。

### 13.caffe_cpu_strided_dot 函数：

```
template <>
double caffe_cpu_strided_dot<double>(const int n, const double* x,
    const int incx, const double* y, const int incy) {
  return cblas_ddot(n, x, incx, y, incy);
}12345
```

功能： 返回 vector X 和 vector Y 的内积。 
incx， incy ： 步长，即每隔incx 或 incy 个element 进行操作。

### 14.caffe_cpu_hamming_distance 函数：

```
template <>
int caffe_cpu_hamming_distance<float>(const int n, const float* x,
                                  const float* y) {
  int dist = 0;
  for (int i = 0; i < n; ++i) {
    dist += __builtin_popcount(static_cast<uint32_t>(x[i]) ^
                               static_cast<uint32_t>(y[i]));
  }
  return dist;
}
1234567891011
```

功能：返回 x 和 y 之间的海明距离。（两个等长字符串之间的海明距离是两个字符串对应位置的不同字符的个数。）

### 15. caffe_cpu_asum 函数：

```
template <>
float caffe_cpu_asum<float>(const int n, const float* x) {
  return cblas_sasum(n, x, 1);
}
12345
```

功能：计算 vector x 的所有element的绝对值之和。

### 16.caffe_cpu_scale 函数：

```
template <>
void caffe_cpu_scale<float>(const int n, const float alpha, const float *x,
                            float* y) {
  cblas_scopy(n, x, 1, y, 1);
  cblas_sscal(n, alpha, y, 1);
}123456
```

功能：y = alpha*x