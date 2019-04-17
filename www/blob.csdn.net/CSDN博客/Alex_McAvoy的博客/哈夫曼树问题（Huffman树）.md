# 哈夫曼树问题（Huffman树） - Alex_McAvoy的博客 - CSDN博客





2018年06月17日 15:09:19[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：489








### 【问题描述】

给定 n 个叶结点，每个结点有一个权值 w[i] ，将它们中两个合并为树，假设每个结点从根到它的距离是 d[i] ，使得最终 ∑（ wi * di ）最小。

### 【构造方法】


- 从森林里取两个权和最小的结点；
- 将它们的权和相加，得到新的结点，并且把原结点删除，将新结点插入到森林中；
- 重复 1~2，直到整个森林里只有一棵树。

### 【数据结构】



显然，问题当中需要执行的操作是：


- 从一个表中取出最小的数；
- 插入一个数字到这个表中。

支持动态查找最小数和动态插入操作的数据结构，选择用堆来实现，因为取的是最小元素，所以要用小根堆实现。

### 【操作实现】

整个程序开始时通过 n 次 put 操作建立一个小根堆，然后不断重复如下操作：两次 get 操作取出两个最小数累加起来，并且形成一个新的结点，再插入到堆中。

如：1+1=2，再把 2 插入到堆的后面一个位置，然后从下往上调整，使得包括 2 在内的数组满足堆的性质。

![](https://img-blog.csdn.net/20180617145823802?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

![](https://img-blog.csdn.net/20180617145935420?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTE4MTU0MDQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

### 【源代码】

一般实现：

```cpp
#define N 30001
int heap_size,n;
int heap[N];

void put(int d)//堆的put操作
{
	int now, next;
	heap[++heap_size] = d;
	now = heap_size;
	while(now > 1)
	{
		next = now >> 1;
		if(heap[now] >= heap[next])
            return;
		swap(heap[now], heap[next]);
		now = next;
	}
}

int get()//堆的get操作
{
	int now, next, res;
	res = heap[1];
	heap[1] = heap[heap_size--];
	now = 1;
	while(now * 2 <= heap_size)
	{
		next = now * 2;
		if(next < heap_size && heap[next + 1] < heap[next])next++;
		if(heap[now] <= heap[next])return res;
		swap(heap[now], heap[next]);
		now = next;
	}
	return res;
}

void work()
{
	int i, x, y, ans = 0;
	cin >> n;
	for(i = 1 ; i <= n ; i++)//建堆
	{
		cin >> x;
		put(x);
	}
	for(i = 1 ; i < n ; i++) //取、统计、插入
	{
		x = get();
		y = get();                
		ans += x + y;
		put(x + y);
	}
	cout << ans << endl;
}

int main()
{
	ios::sync_with_stdio(false);//优化。打消iostream的输入输出缓存，使得cin cout 时间和printf scanf 相差无几
	work();
	return 0;
}
```



使用 STL 实现：



```cpp
int n;
priority_queue<int,vector<int>,greater<int> > h;//优先队列 

void work()
{
	int i, x, y, ans = 0;
	cin >> n;
	for(i = 1 ; i <= n ; i++)//建堆
	{
		cin >> x;
		h.push(x);
	}
	for(i = 1 ; i < n ; i++)//取、统计、插入
	{
		x = h.top();h.pop();
		y = h.top();h.pop();
		ans += x + y;
		h.push(x + y);
	}
	cout << ans << endl;
}

int main()
{
	work();
	return 0;
}
```

例题：[点击这里](https://blog.csdn.net/u011815404/article/details/80657564)



