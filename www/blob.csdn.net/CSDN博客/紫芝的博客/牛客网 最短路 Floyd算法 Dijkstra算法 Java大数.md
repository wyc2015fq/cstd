# 牛客网 最短路 Floyd算法 Dijkstra算法 Java大数 - 紫芝的博客 - CSDN博客





2018年11月09日 20:50:39[紫芝](https://me.csdn.net/qq_40507857)阅读数：2068
所属专栏：[牛客刷题机](https://blog.csdn.net/column/details/29630.html)









 链接：[https://www.nowcoder.com/questionTerminal/a29d0b5eb46b4b90bfa22aa98cf5ff17](https://www.nowcoder.com/questionTerminal/a29d0b5eb46b4b90bfa22aa98cf5ff17)

来源：牛客网

最短路径
- 热度指数：2992 时间限制：1秒 空间限制：65536K
- [算法知识视频讲解](/courses/semester/algorithm-senior)

N个城市，标号从0到N-1，M条道路，第K条道路（K从0开始）的长度为2^K，求编号为0的城市到其他城市的最短距离

**输入描述:**

```
第一行两个正整数N（2<=N<=100）M(M<=500),表示有N个城市，M条道路
接下来M行两个整数，表示相连的两个城市的编号
```



**输出描述:**
`N-1行，表示0号城市到其他城市的最短路，如果无法到达，输出-1，数值太大的以MOD 100000 的结果输出。`
示例1

## 输入

```
4 4
1 2
2 3
1 3
0 1
```

## 输出

```
8
9
11
```

### Floyd算法 

```java
import java.util.*;
import java.math.*;
public class Main{
	static int maxn=(int)(100+10);
	static BigInteger MOD =new BigInteger("100000");
	static String INF="";
	static BigInteger[][] map=new BigInteger[maxn][maxn];
	static int n,m;
	static BigInteger base=new BigInteger("2");
	static void init() {
		for(int i=0;i<=n;i++) {
			for(int j=0;j<=n;j++) {
				if(i!=j)
				map[i][j]=new BigInteger(INF);
				else
				map[i][j]=new BigInteger("0");
			}
		}
	}
	
	static void floyd() {
		for(int k=0;k<n;k++)
			for(int i=0;i<n;i++)
				for(int j=0;j<n;j++)
					if(map[i][j].compareTo(map[i][k].add(map[k][j]))>0)
						map[i][j]=map[i][k].add(map[k][j]);
	}
	
	public static void main(String[] args) {
		Scanner cin=new Scanner(System.in);
		for(int i=0;i<=160;i++)//2^500=10^155;
			INF+="9";
		while(cin.hasNext()) {
			n=cin.nextInt();
			m=cin.nextInt();
			init();		
			for(int k=0;k<m;k++) {
				int u=cin.nextInt();
				int v=cin.nextInt();
				if(!map[u][v].toString().equals(INF))	continue;
				map[u][v]=map[v][u]=new BigInteger( base.pow(k).toString());
			}
		
		floyd();
		for(int i=1;i<n;i++) {
			BigInteger d=map[0][i];
			if(d.toString().equals(INF))
				System.out.println("-1");
			else
				System.out.println(d.mod(MOD));
			}
		}
		cin.close();
	}
}
```

### **Dijkstra算法**

```cpp
import java.util.*;
import java.math.*;
public class Main{
	static int maxn=(int)(100+10);
	static int n,m;
	static BigInteger MOD =new BigInteger("100000");
	static String INF="";
	static BigInteger[][] map=new BigInteger[maxn][maxn];
	static BigInteger base=new BigInteger("2");
	
	static void init() {
		for(int i=0;i<=n;i++) {
			for(int j=0;j<=n;j++) {
				if(i!=j)
				map[i][j]=new BigInteger(INF);
				else
				map[i][j]=new BigInteger("0");
			}
		}
	}
	
	static BigInteger[] dis=new BigInteger[maxn];
	static boolean[] vis =new boolean[maxn];
	static void dijkstra(int start) {
		for(int i=0;i<n;i++) {
			dis[i]=map[start][i];
			vis[i]=false;
		}
		dis[start]=new BigInteger("0");
		vis[start]=true;
		for(int i=0;i<n-1;i++) {
			int k=-1;
			BigInteger minD=new BigInteger(INF);
			for(int j=0;j<n;j++) {
				if(!vis[j]&&dis[j].compareTo(minD)<0) {
					minD=dis[j];
					k=j;
				}
			}
			if(k==-1)	break;
			vis[k]=true;
			for(int j=0;j<n;j++) {
				if(!vis[j]&&dis[j].compareTo(dis[k].add(map[k][j]))>0)
					dis[j]=dis[k].add(map[k][j]);
			}
		}
	}
	public static void main(String[] args) {
		Scanner cin=new Scanner(System.in);
		for(int i=0;i<=160;i++)
			INF+="9";
	
		while(cin.hasNext()) {
			n=cin.nextInt();
			m=cin.nextInt();
			init();		
			for(int k=0;k<m;k++) {
				int u=cin.nextInt();
				int v=cin.nextInt();
				if(!map[u][v].toString().equals(INF))	continue;
				map[u][v]=map[v][u]=new BigInteger( base.pow(k).toString());
			}
            
			dijkstra(0);
		for(int i=1;i<n;i++) {
			BigInteger d=dis[i];
			if(d.toString().equals(INF))
				System.out.println("-1");
			else
				System.out.println(d.mod(MOD));
			}
		}
		cin.close();
	}
}
```





