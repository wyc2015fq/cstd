# c++使用二重指针实现矩阵的储存释放和乘法 - 彩虹糖的博客 - CSDN博客





2017年05月28日 21:41:55[彩虹糖梦](https://me.csdn.net/caozixuan98724)阅读数：411








# c++使用二重指针实现矩阵的储存释放和乘法

```
#include<iostream>
using namespace std;
//新建一个矩阵
double **allocMatrix(int n) {
    double **matrix;
    matrix = new double *[n];
    for (int i = 0; i < n; i++) {
        matrix[i] = new double[n];
    }
    for (int i = 0; i<n; i++)
        for (int j = 0; j<n; j++)
        {
            cin >> matrix[i][j];
        }
    return matrix;
}
//释放一个矩阵
void releaseMatrix(double **m, int n) {
    for (int i = 0; i<n; i++)
        delete[] m[i];
    delete[] m;
}
//输出一个矩阵
void printMatrix(double **m, int n) {
    for (int x = 0; x < n; x++) {
        for (int y = 0; y < n; y++)
        {
            cout << m[x][y]<<" ";
        }
        cout << endl;
    }
}
//实现两个矩阵的乘法
double **multiple(double **m1, double**m2, int n) {
    double **mul;
    mul = new double *[n];
    for (int i = 0; i < n; i++) {
        mul[i] = new double[n];
    }
    for (int x = 0; x<n; x++)
        for (int y = 0; y<n; y++)
        {
            mul[x][y]=0;
        }
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                mul[i][j] = mul[i][j]+m1[i][k]*m2[k][j];
            }
        }
    }
    return mul;
}
int main() {
    cout << "请输入矩阵的阶数" << endl;
    int n;
    cin >> n;
    cout << "请输入第一个矩阵的参数" << endl;
    double **m1 = allocMatrix(n);
    cout << "请输入第二个矩阵的参数" << endl;
    double **m2 = allocMatrix(n);
    cout << "第一个矩阵为" << endl;
    printMatrix(m1, n);
    cout << "第二个矩阵为" << endl;
    printMatrix(m2, n);
    double **mul =multiple(m1, m2, n);
    printMatrix(mul, n);
    releaseMatrix(m1, n);
    releaseMatrix(m2, n);
    system("pause");
    return 0;
}
```



