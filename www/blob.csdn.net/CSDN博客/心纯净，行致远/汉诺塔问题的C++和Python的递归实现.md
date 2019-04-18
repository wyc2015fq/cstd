# 汉诺塔问题的C++和Python的递归实现 - 心纯净，行致远 - CSDN博客





2018年06月29日 23:40:03[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：333








 递归实现主要使用函数和分支语句，最重要的思想是区分清楚基例和链条两个部分，类似于数学里面的数学归纳法。

C++实现：

```cpp
#include <iostream>
using namespace std;

void hanoi(int N,char source,char relay,char destination){	//递归函数的定义 
	if(N==1)	//递归终结条件：N==1 
		cout<<source<<"->"<<destination<<endl;	//直接把这个盘子从source移动到destination 
	else{
		hanoi(N-1,source,destination,relay);	//先把上面N-1个盘子移到relay
		cout<<source<<"->"<<destination<<endl;
		hanoi(N-1,relay,source,destination); 
	}
}

int main(){
	int n;
	cout<<"请输入汉诺塔的层数： ";
	cin>>n;
	hanoi(n,'A','B','C');	//调用递归函数将n个盘子从A转移到C （借助B）
	return 0; 
}
```

Python实现：

```python
count = 0
def hanoi(n,src,dst,mid):   #将n个圆盘从src借助mid移动到dst
    global count    #将count声明为全局变量,记录移动的次数
    if n==1:
        print("{}:{}->{}".format(1,src,dst))    #当只有一个圆盘，从SRC->DST
        count+=1
    else:
        hanoi(n-1,src,mid,dst)  #将n-1个圆盘从src借助dst移动到mid
        print("{}:{}->{}".format(n,src,dst))  #将第n个圆盘从src移动dst
        count+=1
        hanoi(n-1,mid,dst,src)   #将n-1个圆盘从mid借助src移动到dst
hanoi(6,'A','B','C')
print(count)
```

实验了一下，当n=4，count=15;n=5,count=31;n=6,count=63……因此移动n个圆盘，需要的次数为2^n-1,即时间复杂度呈现指数式增长



