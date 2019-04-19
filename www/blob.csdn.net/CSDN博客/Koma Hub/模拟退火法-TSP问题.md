# 模拟退火法-TSP问题 - Koma Hub - CSDN博客
2019年01月06日 16:04:00[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：29
模拟退火算法（Simulate Anneal Arithmetic，SAA）是一种通用概率演算法，用来在一个大的搜寻空间内找寻命题的最优解。模拟退火是S.Kirkpatrick, C.D.Gelatt和M.P.Vecchi在1983年所发明。而V.Černý在1985年也独立发明此演算法。模拟退火算法是解决TSP问题的有效方法之一。
[旅行商问题](https://baike.so.com/doc/6257276-6470691.html)，即TSP问题*(Travelling Salesman Problem)*又译为旅行推销员问题、货郎担问题，是数学领域中著名问题之一。假设有一个旅行商人要拜访n个城市，他必须选择所要走的路径，路径的限制是每个城市只能拜访一次，而且最后要回到原来出发的城市。路径的选择目标是要求得的路径路程为所有路径之中的最小值。
一个github代码：[https://github.com/Shelbin/simulated-annealing-parallelization](https://github.com/Shelbin/simulated-annealing-parallelization)
```cpp
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include <sys/time.h>
#define ZUIDADIANSHU 200	//最大的点数
#define QISHIDIAN 4			//起始点
#define CHUSHIWENDU 1400000	//初始温度
#define WENDUXISHU 0.99	    //降温系数
#define T_MIN 0.000001
/**
城市文件
100
0	0	0
1	1	0
2	2	0
3	3	0
4	4	0
5	5	0
6	6	0
7	7	0
8	8	0
9	9	0
10	0	1
11	1	1
12	2	1
13	3	1
14	4	1
15	5	1
16	6	1
17	7	1
18	8	1
19	9	1
20	0	2
21	1	2
22	2	2
23	3	2
24	4	2
25	5	2
26	6	2
27	7	2
28	8	2
29	9	2
30	0	3
31	1	3
32	2	3
33	3	3
34	4	3
35	5	3
36	6	3
37	7	3
38	8	3
39	9	3
40	0	4
41	1	4
42	2	4
43	3	4
44	4	4
45	5	4
46	6	4
47	7	4
48	8	4
49	9	4
50	0	5
51	1	5
52	2	5
53	3	5
54	4	5
55	5	5
56	6	5
57	7	5
58	8	5
59	9	5
60	0	6
61	1	6
62	2	6
63	3	6
64	4	6
65	5	6
66	6	6
67	7	6
68	8	6
69	9	6
70	0	7
71	1	7
72	2	7
73	3	7
74	4	7
75	5	7
76	6	7
77	7	7
78	8	7
79	9	7
80	0	8
81	1	8
82	2	8
83	3	8
84	4	8
85	5	8
86	6	8
87	7	8
88	8	8
89	9	8
90	0	9
91	1	9
92	2	9
93	3	9
94	4	9
95	5	9
96	6	9
97	7	9
98	8	9
99	9	9
*/
//全局变量
char point_list[ZUIDADIANSHU][20];				//存储点名称
double point_coordinate[ZUIDADIANSHU][2];		//存储点x y坐标
int *simulation(int *x,int point_number);
double P(int *i,int *j,double t,int point_number);
int *simulation(int *x,int point_number);
double distance_sum(int *x,int point_number);
void Neighbour(int *father,int *result,int point_number);
double distance(int x,int y);
double random0_1(void);
int *random2(int point_number);
void print_coordinate(int *list,int point_number);
double mytime();
int main()
{
    //记录时间
    double tstart,tstop;
	char filename[81];
	double x_position,y_position;
	FILE *pfile;
	int *sequence;			//初始序列
	int point_number;		//点数量  可以考虑用宏定义定义最大数量
	int i;					//for循环专用变量
	int *endlist;
	double sumlen;
    tstart=mytime();
	strcpy(filename,"100.txt");
	if((pfile = fopen(filename,"r"))==NULL)
	{
		printf("没有找到城市文件%s!\n",filename);
		return 0;
	}
	/*读取目标文件城市信息*/
	fscanf(pfile,"%d",&point_number);
	for(i=0;i<point_number;i++)
	{
		fscanf(pfile,"%s\t%lf\t%lf",&point_list[i],&x_position,&y_position);
		point_coordinate[i][0] = x_position;
		point_coordinate[i][1] = y_position;
	}
	fclose(pfile);
	srand( (unsigned)time( NULL ) );   //设置种子 初始化随机发生器
	sequence = random2(point_number);
	endlist = simulation(sequence,point_number);
	printf("last: ");
	for(i=0;i<point_number;i++)
	{
		printf("%d ",endlist[i]);
	}
	sumlen = distance_sum(endlist,point_number);
	printf("\n sumlen:%lf\n",sumlen);
	free(endlist);
	free(sequence);
    tstop=mytime();
    printf("spend time=%lf\n",tstop-tstart);
	return 0;
}
/*产生一个以0开头的 0-point_number的不重复序列*/
int *random2(int point_number)
{
	int temp = 0,signal = 1,k=0;
	int i,j;
	int *save = NULL;
	save =  (int *)malloc(point_number*sizeof(int));
	//数组初始化
	for(j = 0;j < point_number;j++)
		save[j] = -1;
	/*产生一个0-point_number的不重复序列*/
	do
	{
		signal = 1;
		temp = rand() % point_number;	//temp取0-point_number的值
		for(i=0;i<point_number;i++)
		{
			if(save[i] == temp)
			{
				signal = 0;
				break;
			}
		}
		if(signal != 0)
		{
			save[k] = temp;
			k++;
		}
	}while(signal == 0 || k != point_number);
	/*使得save[0]=0（save[0]为开始点）*/
	for(i=1;i<point_number;i++)
	{
		if(save[i]==QISHIDIAN)
		{
			temp = save[i];
			save[i] = save[0];
			save[0] = temp;
		}
	}
	return save;
}
/*产生随机数,范围在0-1之间*/
double random0_1(void)
{
	return (double)rand() / (double)RAND_MAX;
}
/*计算x y 2点之间的距离*/
double distance(int x,int y)
{
	double xy_distance = 0;
//	printf("x=%d y=%d\t",x,y);
	xy_distance = sqrt( (point_coordinate[x][0] - point_coordinate[y][0]) * (point_coordinate[x][0] - point_coordinate[y][0]) +
   						(point_coordinate[x][1] - point_coordinate[y][1]) * (point_coordinate[x][1] - point_coordinate[y][1]) );
	return xy_distance;
}
/* 产生新序列
father代表父序列，result代表子序列,point_number代表点个数 将交换结果存储在result中*/
void Neighbour(int *father,int *result,int point_number)
{
	int n = 0,m = 0,temp = 0;
	int k;
	for(k=0;k<point_number;k++)
		result[k] = father[k];
	do
	{
		n = rand() % (point_number - 1); //-1-point_number-1
		m = rand() % (point_number - 1);
	}while(n == m);		//获得随机的不相等的n，m
	n++;
	m++;//0-point_number
	//交换
	temp = result[n];
	result[n] = result[m];
	result[m] = temp;
}
/*计算某一个序列中城市之间的距离总和*/
double distance_sum(int *x,int point_number)
{
	double result = 0;
	int i;
	for(i=0;i<point_number-1;i++)
	{
		result += distance(x[i],x[i+1]);
	}
//	result += distance(x[point_number-1],x[0]);	//是否回到起始点
	return result;
}
/*温度的下降函数
初始温度为T=CHUSHIWENDU 温度下降系数为rate=WENDUXISHU
x为序列 point_number为点数*/
int *simulation(int *x,int point_number)
{
	int	*temp = NULL;
	double random = 0;
	int *i = malloc(point_number*sizeof(int));
	int *j = malloc(point_number*sizeof(int));
	int m;
	double t = CHUSHIWENDU, rate = WENDUXISHU;		//初始温度，降温系数
	int L = 200*point_number;			//每个温度的迭代次数,也就是每一个温度上的平衡条件
	int time = 0;						//记录某一温度下的迭代次数
	for(m=0;m<point_number;m++)
	{
		i[m] = x[m];
		j[m] = 0;
	}
    
    double nextlen=0.0,len=0.0,local_len=0.0,d_len=0.0;
	do
	{
		do
		{
            for(m=0;m<20000;m++)
            {
                Neighbour(i,j,point_number);
                random = P(i,j,t,point_number);
                if ( (random == 1.0) || (random > random0_1()) )
                {
                    temp = i;
                    i = j;
                    j = temp;//保证i序列始终是当前起始序列
                 }
            }
            local_len = distance_sum(i,point_number);
            nextlen = len;
            len = local_len;
            d_len = len-nextlen;
        }while(d_len>0.1);    //		f2 = distance_sum(i,point_number);
		t = rate * t;
	}while(t>T_MIN);//结束条件2：t>t_min
	free(j);
	return i;
}
//t代表当前温度,i,j分别代表不同的两个序列,返回对应的转移发生概率
double P(int *i,int *j,double t,int point_number)
{
	double fi = 0,fj = 0;
	double result = 0;
	fi = distance_sum(i,point_number);
	fj = distance_sum(j,point_number);
	if(fj < fi)
		result = 1.0;
	else
		result = exp( (fi - fj)/t );
	return result;
}
void print_coordinate(int *list,int point_number)
{
	int i;
	for(i=0;i<point_number;i++)
	{
		printf("G00 X%.3lf Y%.3lf\n",point_coordinate[list[i]][0],point_coordinate[list[i]][1]);
	}
}
double mytime()
{
    double ts = 0.0;
    struct timeval mt;
    gettimeofday(&mt,(struct timezone*)0);
    ts = (double)(mt.tv_sec+mt.tv_usec*1.0e-6);
    return (ts);
}
```
