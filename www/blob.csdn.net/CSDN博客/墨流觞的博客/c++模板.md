# c++模板 - 墨流觞的博客 - CSDN博客





2018年12月30日 17:49:53[墨氲](https://me.csdn.net/dss_dssssd)阅读数：18








主要涉及函数模板和类模板

### 函数模板：

`template <typename T1[, typename T2, ...]> 返回类型 函数名(参数列表)`

其中，参数列表可以不同，此时为重载

示例：

```
#include <iostream>
#include<stdlib.h>
#include <string>
using namespace std;

template <typename T> T Max(T a, T b) {
	return a < b ? b : a;
}

int main()
{
	

	int aa = 6, bb = 7;
	double da = 6.0, db = 7.0;
	cout << Max<int>(aa, bb) << endl;
	cout << Max(aa, bb) << endl;
	cout << Max(da, db) << endl;


	system("pause");
	return 0;
}
```

```
#include <iostream>
#include<stdlib.h>
#include <string>
using namespace std;


template<typename T1, typename T2> T1 add(T1 a, T2 b) {
	return a + b;
}
int main()
{
	

	int aa = 6, bb = 7;
	double da = 6.1, db = 7.0;
	

	cout << add(aa, bb) << endl;
	cout << add<double, int>(da, aa) << endl;
	cout << add(da, aa) << endl;
	cout << add(aa, da) << endl;
	system("pause");
	return 0;
}
```

out:

> 
13

12.1

12.1

12

请按任意键继续. . .
**warning C4244: “return”: 从“T2”转换到“T1”，可能丢失数据**,按位置指定的。

```
#include <iostream>
#include<stdlib.h>
#include <string>
#include<iomanip>
using namespace std;


template <typename T1, typename T2> void inverse(T1 *mat1, T2 *mat2, int a, int b) {
	int i, j;
	for(i=0;i<a;i++)
		for (j = 0;j < b; j++)
			mat2[j][i] = mat1[i][j];
	
}

/*template <typename T> void inverse_(T ** mat1, T **mat2, int a, int b) {
	int i, j;
	for (i = 0; i <a; i++)
		for (j = 0;j <b; j++)
			mat2[j][i] = mat1[i][j];
		
}*/

template <typename T> void output(T *mat, int a,int b) {
	int i, j;
	for (i = 0; i < a; i++) {	
		for (j = 0; j < b; j++) {
			cout <<setw(6)<< mat[i][j];
		}
		cout << endl;
	}
	cout << endl;

}

int main()
{
	

	int middle[6][3];
	int middle_[6][3];
	int matrix[3][6] = { 1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18 };

	output(matrix, 3, 6);

	inverse(matrix, middle,  3, 6);//显示： inverse<int[6], int[3]>(matrix, middle,  3, 6)
	output(middle, 6,3);
	
	//inverse_(matrix, middle_, 3, 6); 这是错误定义
	//output(middle_, 6, 3);
	system("pause");
	return 0;
}
```

### 类模板

```
template <typename T1[, typename T2, ...]> class 类名{
//类定义

//成员函数
返回类型 函数名(参数列表);
};

//类成员函数
template<模板参数列表> 返回类型 类名<模板参数列表> :: 成员函数1(参数列表){
}
```

```
#include <iostream>
#include<stdlib.h>
#include <string>
#include<iomanip>
using namespace std;

//可以增加默认值的
template <typename T=int, size_t s = 5> class Array {
private:
	T arr[s];
public:
	T& operator[](const size_t &num) {
		if (num >= s) {
			cout << "访问越界！！返回数组第一个值" << endl;
			return arr[0];

		}
		return arr[num];
	}

	



};



int main()
{
	
	const int row = 3, col = 4;
	Array<Array<int, col>, row> arr;
	
	/*数组赋值 和 打印数组*/
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			arr[i][j] = i + j;
		}
	}
	for (int i = 0; i < row; i++) {
		for (int j = 0; j < col; j++) {
			cout <<setw(3)<< arr[i][j];
		}
		cout << endl;
	}
	 

	cout << arr[4][1];//输出1  注意一下
	cout << arr[2][6];// 输出2  注意一下

	
	system("pause");
	return 0;
}
```

```
#include <iostream>
#include<stdlib.h>
#include <string>
#include<iomanip>
using namespace std;

//可以增加默认值的
template <typename T=int, size_t s = 5> class Array {
private:
	T arr[s];
public:
	T& operator[](const size_t &num) {
		if (num >= s) {
			cout << "访问越界！！返回数组第一个值" << endl;
			return arr[0];

		}
		return arr[num];
	}

	T max();

	



};

template <typename T, size_t s> T Array<T, s>::max() {
	T maxv = arr[0];
	int i = 1;
	for (; i < s; i++) {
		if (maxv < arr[i])
			maxv = arr[i];
	}
	return maxv;
}


int main()
{
	
	Array<int, 5> arri;
	Array<double, 7> arrd;

	for (int i = 0; i < 5; i++) {
		arri[i] = i;
	}

	for (int i = 0; i < 7; i++) {
		arrd[i] = (i + 0.1);
	}

	cout << endl << arri.max() << endl;
	for (int i = 0; i < 5; i++) {
		cout<<arri[i]<<' ';
	}

	cout << endl << arrd.max() << endl;
	for (int i = 0; i < 7; i++) {
		cout << arrd[i]<<' ';
	}

	
	

	
	system("pause");
	return 0;
}
```

out:

> 
4

0 1 2 3 4

6.1

0.1 1.1 2.1 3.1 4.1 5.1 6.1
**之后看的：STL中的vectorpairmap需要注意： iterator以及最常用的string还有就是C++提供的算法库。**







