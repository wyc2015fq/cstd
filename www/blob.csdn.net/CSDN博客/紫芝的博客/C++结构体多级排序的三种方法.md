# C++结构体多级排序的三种方法 - 紫芝的博客 - CSDN博客





2018年11月03日 11:12:51[紫芝](https://me.csdn.net/qq_40507857)阅读数：484








### C++结构体多级排序的三种方法

```cpp
struct node{
	int chinese,math;
	char name[15];
};
```

**需求：按数学成绩从大到小排序 **

### 1.自定义比较器

```cpp
//自定义比较函数
bool cmp(node a,node b){
    return a.math>b.math;
}
```

### 2.定义友元函数

```cpp
struct node{
	int chinese,math;
	char name[15];
	//友元函数
	friend bool operator<(node a,node b){
        return a.math>b.math;
	}
};
```

### 3.重载小于运算符

```cpp
struct node{
	int chinese,math;
	char name[15];
	
	//重载小于运算符
	bool operator <(const node&b)const{
        return math>b.math;
	}
};
```

### 完整实例程序

```cpp
#include <iostream>
#include <cstdio>
#include<algorithm>
using namespace std;
#define INF 0x3f3f3f3f
const int maxn = (1e5+10);

struct node{
	int chinese,math;
	char name[15];
	/*
	//重载小于运算符
	bool operator <(const node&b)const{
        return math>b.math;
	}
	*/
	//友元函数
	friend bool operator<(node a,node b){
        return a.math>b.math;
	}
};
node arr[maxn];
//自定义比较函数
bool cmp(node a,node b){
    return a.math>b.math;
}
int gcd(int a,int b)
{
    return b==0?a:gcd(b,a%b);
}
int lcm(int a,int b)
{
    return a/gcd(a,b)*b;
}
int main() {
    int n;
    scanf("%d",&n);
    for(int i=0;i<n;i++){
        scanf("%d%d%s",&arr[i].chinese,&arr[i].math,arr[i].name);
    }
    sort(arr,arr+n);
    printf("%d\n%s\n",lcm(arr[0].chinese,arr[1].chinese),arr[0].name);
}
/*
3
50 60 张三
40 80 李四
45 100 王五
*/
```





