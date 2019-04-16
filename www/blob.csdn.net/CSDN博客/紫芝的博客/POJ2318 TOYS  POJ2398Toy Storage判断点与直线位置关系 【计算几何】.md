# POJ2318 TOYS / POJ2398Toy Storage判断点与直线位置关系 【计算几何】 - 紫芝的博客 - CSDN博客





2018年10月24日 17:13:20[紫芝](https://me.csdn.net/qq_40507857)阅读数：77








Calculate the number of toys that land in each bin of a partitioned toy box. 

**计算每一个玩具箱里面玩具的数量**



Mom and dad have a problem - their child John never puts his toys away when he is finished playing with them. They gave John a rectangular box to put his toys in, but John is rebellious and obeys his parents by simply throwing his toys into the box. All the toys get mixed up, and it is impossible for John to find his favorite toys. 
**妈妈和爸爸有一个问题，他们的孩子约翰从来没有在玩完玩具后把玩具都放好，他们有了约翰一个矩形的箱子用来放他的玩具，但是约翰很叛逆，他服从了他父母，不过只是简单地把玩具扔进了箱子里。所有的玩具都搞混了，并且约翰不可能找到他最喜欢的玩具。**



John's parents came up with the following idea. They put cardboard partitions into the box. Even if John keeps throwing his toys into the box, at least toys that get thrown into different bins stay separated. The following diagram shows a top view of an example toy box. 

**约翰的父母想出了如下的注意，他们在箱子里放了纸板进行分区，即使约翰不停地把玩具扔进箱子里，至少玩具被扔进不同的垃圾箱并保持分开，下图展现了样例玩具箱的俯视图。**

![](https://vj.e949.cn/b881c911dee4ea005b86bfade3fc9423?v=1540089839)

For this problem, you are asked to determine how many toys fall into each partition as John throws them into the toy box.

**对于这个问题，当约翰把玩具扔进箱子里时，你被要求确定有多少个玩具被扔进了分区。**

Input

The input file contains one or more problems. The first line of a problem consists of six integers, n m x1 y1 x2 y2. The number of cardboard partitions is n (0 < n <= 5000) and the number of toys is m (0 < m <= 5000). The coordinates of the upper-left corner and the lower-right corner of the box are (x1,y1) and (x2,y2), respectively. The following n lines contain two integers per line, Ui Li, indicating that the ends of the i-th cardboard partition is at the coordinates (Ui,y1) and (Li,y2). You may assume that the cardboard partitions do not intersect each other and that they are specified in sorted order from left to right. The next m lines contain two integers per line, Xj Yj specifying where the j-th toy has landed in the box. The order of the toy locations is random. You may assume that no toy will land exactly on a cardboard partition or outside the boundary of the box. The input is terminated by a line consisting of a single 0.

**  输入文件包含一个或多个问题，第一行是第一个问题包含6个数，n,m,x1,y1,x2,y2,纸板分区的数目为n（0 < n <= 5000），玩具的数量是m（0＜m＜5000），箱子左上角的坐标为（x1,y1），右下角的坐标为（x2,y2）。以下N行，每行包含两个整数，Ui Li，表示第i个纸板分区的结束是在坐标（UI，y1）和（Li，y2）。你可以假设纸板分区彼此不相交，它们是从左到右按排序顺序指定的。接下来的m行每行包含两个整数，Xj Yj指定该位置被第j个玩具落在了箱子里，玩具位置的顺序是随机的。你可以假设没有玩具会准确地落在硬纸板的隔板上或盒子的外面（就是所有玩具肯定都会掉进箱子里面），输入由一个单0组成的行终止。**



Output

The output for each problem will be one line for each separate bin in the toy box. For each bin, print its bin number, followed by a colon and one space, followed by the number of toys thrown into that bin. Bins are numbered from 0 (the leftmost bin) to n (the rightmost bin). Separate the output of different problems by a single blank line.

**每个问题的输出将是玩具箱中每个独立的箱子的一行。对于每一个箱子，打印它的箱子的号码，后面是冒号和一个空格，后面跟着丢进箱子的玩具数量。箱编号从0（最左边箱子）N（最右边的箱子）。用一条空行分隔不同问题的输出。**



Sample Input

```
5 6 0 10 60 0
3 1
4 3
6 8
10 10
15 30
1 5
2 1
2 8
5 5
40 10
7 9
4 10 0 10 100 0
20 20
40 40
60 60
80 80
5 10
15 10
25 10
35 10
45 10
55 10
65 10
75 10
85 10
95 10
0
```

Sample Output

```
0: 2
1: 1
2: 1
3: 1
4: 0
5: 1

0: 2
1: 2
2: 2
3: 2
4: 2
```

Hint

As the example illustrates, toys that fall on the boundary of the box are "in" the box.
**如图所示，落在盒子边界上的玩具在盒子里**

### 分析：

**给n条线段，构成n+1个区间，然后是m个点的坐标，问0-n个区间，每个区间有多少个点？**

**我们先来看一下只有1条线段的情况，一条线段把平面分成两个区间，如果点在线段的左侧，那么这个点在左区间（0号），否则在右区间（1号）；**

![](https://img-blog.csdn.net/20181024190042209?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzQwNTA3ODU3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

点(u,y1)和点(x,y)连线的斜率为k1= ![\frac{y1-y}{u-x1}](https://private.codecogs.com/gif.latex?%5Cfrac%7By1-y%7D%7Bu-x1%7D)，点(l,y2)和点(x,y)连线的斜率为k2= ![\frac{y2-y}{l-x2}](https://private.codecogs.com/gif.latex?%5Cfrac%7By2-y%7D%7Bl-x2%7D)
- k1=k2,三点共线
- k1>k2,点在直线的右侧
- k1<k2,点在直线的左侧

**POJ2318代码：**

```java
import java.util.*;
import java.math.*;
class point{//记录坐标
	int x;
	int y;
	public void point(int _x,int _y){
		this.x=_x;
		this.y=_y;
	}
}

class node{//隔板上下的点
	point a,b;
	public void node(point a,point b) {
		this.a=a;
		this.b=b;
	}
}

public class Main {
	static int maxn=(int)5e3+10;
	static int n,m,x1,y1,x2,y2;
	static node[] A=new node[maxn];
	static int[] ans=new int[maxn];
	
	//判断点(x,y)与线段A[mid]的关系
	static boolean judge(int x,int y,int mid) {
		point a=A[mid].a;	point b=A[mid].b;
		if((a.x-x)*(b.y-y)-(b.x-x)*(a.y-y)>0)
			return true;//点在线段右边
			return false;//点在线段左边
	}
	//二分查找点(x,y)最近的线段
	static void search(int x,int y,int n) {
		int left=0,right=n-1;
		while(left<=right) {
			int mid=(left+right)>>1;
			if(judge(x,y,mid)) //点在mid所在那条线段的右侧
				left=mid+1;
			else			   //点在mid所在那条线段的左侧
				right=mid-1;
		}
		ans[left]++;//记录不同箱子所得到的玩具的数量
	}
	
	public static void main(String[] args) {
		Scanner cin=new Scanner(System.in);
		int ca=1;
		while(cin.hasNext()) {
			if(ca!=1)	System.out.println();
			ca++;
			n=cin.nextInt();
			if(n==0)	break;
			m=cin.nextInt();
			x1=cin.nextInt();
			y1=cin.nextInt();
			x2=cin.nextInt();
			y2=cin.nextInt();
			
			for(int i=0;i<n;i++) {
				int u,l;
				u=cin.nextInt();
				l=cin.nextInt();
				node tmp=new node();
				point a=new point();
				point b=new point();
				a.point(u, y1);
				b.point(l, y2);
				tmp.node(a, b);
				A[i]=tmp;
			}//存储隔板的位置
			
			for(int i=0;i<=n;i++) {//初始化
				ans[i]=0;
			}
			
			for(int i=0;i<m;i++) {
				int x=cin.nextInt();
				int y=cin.nextInt();
				search(x,y,n);//寻找这个玩具在那个区间
			}
			
			for(int i=0;i<=n;i++) {
				System.out.println(i+": "+ans[i]);
			}
		}
		cin.close();
	}
}
```

```cpp
#include<cstdio>
#include<cstdlib>
#include<cstring>
using namespace std;
struct point{
	int x,y;
};//记录坐标
struct Node{
	point a,b;
}A[5010];//隔板上下的点
int pos[5010];
//判断点(x,y)与线段A[mid]的关系
bool judge(int xx,int yy,int mid){
	int ans=(A[mid].a.x-xx)*(A[mid].b.y-yy)-(A[mid].a.y-yy)*(A[mid].b.x-xx);
	if(ans<0)return false;//点在线段左边
	return true;//点在线段右边
}
//二分查找点(x,y)最近的线段
void search(int xx,int yy,int n){
	int left=0,right=n-1;
	while(left<=right){
		int mid=(left+right)>>1;
		if(judge(xx,yy,mid)){//点在mid所在那条线段的右侧
			left=mid+1;
		}
		else { //点在mid所在那条线段的左侧
			right=mid-1;
		}
	}
	pos[left]++;//记录不同箱子所得到的玩具的数量
}
int main()
{
	int n,m,i,j,x1,x2,y1,y2;
	while(scanf("%d",&n),n){
		scanf("%d%d%d%d%d",&m,&x1,&y1,&x2,&y2);
		for(i=0;i<n;++i){
			int xd,xu;
			scanf("%d%d",&xu,&xd);
			A[i].a.x=xu;
			A[i].a.y=y1;
			A[i].b.x=xd;
			A[i].b.y=y2;
		}//存储隔板的位置
		memset(pos,0,sizeof(pos));
		for(i=0;i<m;++i){
			int xx,yy;
			scanf("%d%d",&xx,&yy);
			search(xx,yy,n);
		}//寻找这个玩具在那个区间

		for(i=0;i<=n;++i){
			printf("%d: %d\n",i,pos[i]);
		}
		printf("\n");
	}
	return 0;
}
```

**POJ 2398：先给N条线段从小到大排序，然后二分查找**

```java
import java.util.*;
import java.math.*;
class point{//记录坐标
	int x;
	int y;
	public void point(int _x,int _y){
		this.x=_x;
		this.y=_y;
	}
}

class node{//隔板上下的点
	point a,b;
	public void node(point a,point b) {
		this.a=a;
		this.b=b;
	}
}

//比较器，x坐标从小到大排序；x相同时，按照y从小到大排序
/*
class MyComparator implements Comparator{
	public int compare(Object arg0,Object arg1) {
		point a=(point)arg0;
		point b=(point)arg1;
		if(a.x!=b.x)
			return a.x>b.x?1:-1;
			return a.y>b.y?1:-1;
	}
}
*/
class MyComparator implements Comparator<point>{
	public int compare(point a,point b) {
		if(a.x!=b.x)
			return a.x>b.x?1:-1;
			return a.y>b.y?1:-1;
	}
}
public class Main {
	static int maxn=(int)5e3+10;
	static int n,m,x1,y1,x2,y2;
	static node[] A=new node[maxn];
	static int[] ans=new int[maxn];
	
	//判断点(x,y)与线段A[mid]的关系
	static boolean judge(int x,int y,int mid) {
		point a=A[mid].a;	point b=A[mid].b;
		if((a.x-x)*(b.y-y)-(b.x-x)*(a.y-y)>0)
			return true;//点在线段右边
			return false;//点在线段左边
	}
	//二分查找点(x,y)最近的线段
	static void search(int x,int y,int n) {
		int left=0,right=n-1;
		while(left<=right) {
			int mid=(left+right)>>1;
			if(judge(x,y,mid)) //点在mid所在那条线段的右侧
				left=mid+1;
			else			   //点在mid所在那条线段的左侧
				right=mid-1;
		}
		ans[left]++;//记录不同箱子所得到的玩具的数量
	}
	
	public static void main(String[] args) {
		Scanner cin=new Scanner(System.in);
		int ca=1;
		while(cin.hasNext()) {
			//if(ca!=1)	System.out.println();
			ca++;
			n=cin.nextInt();
			if(n==0)	break;
			m=cin.nextInt();
			x1=cin.nextInt();
			y1=cin.nextInt();
			x2=cin.nextInt();
			y2=cin.nextInt();
			
			//List<point> list=new ArrayList<point>();
			point[] arr=new point[n];
			for(int i=0;i<n;i++) {
				int u,l;
				u=cin.nextInt();
				l=cin.nextInt();
				point p=new point();
				p.point(u, l);
				//list.add(p);
				arr[i]=p;
			}//存储隔板的位置
			Comparator cmp=new MyComparator();
			Arrays.sort(arr, cmp);
			//list.sort(cmp);
			for(int i=0;i<n;i++) {//初始化
				//point p=list.get(i);
				point p=arr[i];
				node tmp=new node();
				point a=new point();
				point b=new point();
				a.point(p.x, y1);
				b.point(p.y, y2);
				tmp.node(a, b);
				A[i]=tmp;
				ans[i]=0;
			}
			ans[n]=0;
			
			for(int i=0;i<m;i++) {
				int x=cin.nextInt();
				int y=cin.nextInt();
				search(x,y,n);//寻找这个玩具在那个区间
			}
			int[] cnt=new int[1010];
			for(int i=1;i<1010;i++)
				cnt[i]=0;
			int max=-1;
			for(int i=0;i<=n;i++) {
				if(ans[i]!=0) {
					cnt[ans[i]]++;
					max=ans[i]>max?ans[i]:max;
				}
			}
			System.out.println("Box");
			for(int i=1;i<=max;i++)
				if(cnt[i]!=0)
			System.out.println(i+": "+cnt[i]);
		}
		cin.close();
	}
}
```





