# eigen（二） 神经网络前向 - xmdxcsj的专栏 - CSDN博客





2016年09月23日 21:54:04[xmucas](https://me.csdn.net/xmdxcsj)阅读数：1525








## 一、 数组和Matrix转化

```cpp
void Array2Matrix()
{
    //假设array原始的矩阵是以下形式(ColMajor)
    //1 3 5
    //2 4 6
    float array1[] = {1,2,3,4,5,6};
    MatrixXf mat1 = Map<MatrixXf> (array1, 2, 3);
    cout << mat1 << endl;
    //默认是ColMajor,output:
    //1 3 5
    //2 4 6

    //假设array原始的矩阵是以下形式(RowMajor)
    //1 2 3
    //4 5 6
    float array2[] = {1,2,3,4,5,6};
    Matrix<float, Dynamic, Dynamic, RowMajor> mat2;
    mat2 = Map<Matrix<float, 2, 3, RowMajor> > (array2);
    cout << mat2 << endl;
    //Matrix的RowMajor/ColMajor，需要跟原始矩阵存储为raw-array对应上，output:
    //1 2 3
    //4 5 6
}

void ChangeElement()
{
    float array1[] = {1,2,3,4,5,6};
    Map<MatrixXf> map1(array1, 2, 3);
    printf("array1 start address=%x, map1 data start address=%x\n", array1, map1.data());
    //地址相同，所以Map和raw-array共享内存地址

    map1(0,0) = 11;
    for(int i = 0; i < 6; ++i)
    {
        cout << array1[i] << endl;
    }
    //修改Map的值，raw-array受影响，output:
    //11
    //2
    //3
    //4
    //5
    //6

    array1[5] = 11;
    cout << map1 << endl;
    //修改raw-array的值，Map受影响，output:
    //11 3  5
    //2  4  11

    float array2[] = {1,2,3,4,5,6};
    MatrixXf mat2 = Map<MatrixXf> (array2, 2, 3);
    printf("array2 start address=%x, mat2 data start address=%x\n", array2, mat2.data());
    //地址不同，Matrix复制构造了Map，此时地址并不一致

    mat2(0,0) = 11;
    for(int i = 0; i < 6; ++i)
    {
        cout << array2[i] << endl;
    }
    //修改Matrix的值，raw-array并不会受影响，output:
    //1
    //2
    //3
    //4
    //5
    //6

    array2[5] = 11;
    cout << mat2 << endl;
    //修改raw-array的值，Matrix并不受影响，output:
    //11 3  5
    //2  4  6
}
```

## 二、sigmoid函数

三种实现形式以及时间对比

```cpp
float SigmoidFunc(float x)
{
    return 1.0 / (1.0 + exp(-x));
}

VectorXf Sigmoid(const VectorXf& v)
{
    VectorXf res = (1.0 + (-v).array().exp()).inverse().matrix();
    return res;
}

void Sigmoid(VectorXf& src, VectorXf& dst)
{
    float *src_data = src.data();
    float *dst_data = dst.data();
    for(int i = 0; i < src.size(); ++i)
    {
        dst_data[i] = SigmoidFunc(src_data[i]);
    }
}

void SigmoidTest()
{
    int dim = 100000;
    VectorXf src(dim);
    VectorXf dst(dim);
    for(int i = 0; i < dim; ++i)
    {
        src(i) = i * 1.0 / dim;
    }
    //1. 使用unaryExpr,6ms
    dst = src.unaryExpr(&SigmoidFunc);
    //cout << dst << endl;
    //2. 使用array,10ms
    dst = Sigmoid(src);
    //cout << dst << endl;
    //3. 自己实现,4ms
    Sigmoid(src, dst);
}
```

## 三、eigen使用mkl加速

矩阵*矩阵，mkl会多线程加速;矩阵*向量，mkl没有多线程

```cpp
void MklTest1()
{
    MatrixXf a(100,200);
    MatrixXf b(200,100);
    MatrixXf c(100,100);
    for(int i = 0; i < 100; ++i)
    {
        for(int j = 0; j < 100; ++j)
        {
            a(i, j) = i+j;
            b(i, j) = i-j;
        }
    }
    while(1)
    {
        c = a*b;
    }
}

void MklTest2()
{
    MatrixXf a(100,100);
    VectorXf b(100);
    VectorXf c(100);
    for(int i = 0; i < 100; ++i)
    {   
        for(int j = 0; j < 100; ++j)
        {   
            a(i, j) = i+j;
        }   
        b(i) = i;
    }   
    while(1)
    {   
        c = a*b;
    }   
}
```



