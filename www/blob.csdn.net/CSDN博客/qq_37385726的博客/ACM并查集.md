# ACM并查集 - qq_37385726的博客 - CSDN博客





2018年05月23日 21:28:02[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：34








![](https://img-blog.csdn.net/20180523212431149?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


```cpp
#include<iostream>
using namespace std;


class Union_Find
{
	private:
		int *array;
		int size;
		int Find(int curr) const;  //返回结点num的根节点
	public:
		Union_Find(int size); 
		void Union(int num1,int num2);
		bool differ(int num1,int num2); //num1和num2在一棵树中，返回0；不在一棵树中，返回1 
};


Union_Find::Union_Find(int size)
{
	array=new int[size+1];
	for(int i=1;i<=size;i++)
	{
		array[i]=i;
	 } 
	 this->size=size;
}

//路径压缩算法 
int Union_Find::Find(int curr) const
{
	if(array[curr]==curr)
	{
		return curr;
	}
	array[curr]=Find(array[curr]);
	return array[curr];
	
}

void Union_Find::Union(int num1,int num2)
{
	int rootnum1=Find(num1);
	int rootnum2=Find(num2);
	if(rootnum1!=rootnum2)
	{
		array[rootnum2]=rootnum1;
	} 
}

bool Union_Find::differ(int num1,int num2)
{
	int rootnum1=Find(num1);
	int rootnum2=Find(num2);
	if(rootnum1!=rootnum2)
	{
		return 1; 
	} 
	else
	{
		return 0;
	}
}



int main(void)
{
	int n,m;
	while(~scanf("%d %d",&n,&m))
	{
		Union_Find tree(n);
		char op;
		int head,tail;
		for(int i=0;i<m;i++)
		{
			getchar();
			scanf("%c %d %d",&op,&head,&tail);
			if(op=='Q')
			{
				bool flag=tree.differ(head,tail);
				if(flag)
				{
					printf("N\n");
				}
				else
				{
					printf("Y\n");
				}
			}
			else
			{
				tree.Union(head,tail);
			}
		}
	}
	return 0;
}
```

并查集的实现是维护一个数组，每一个数组元素对应的值为其父节点的index.

![](https://img-blog.csdn.net/20180523212741767?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM3Mzg1NzI2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)













