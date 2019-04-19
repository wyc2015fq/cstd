# kmeans算法并行化的mpi程序 - sxf_123456的博客 - CSDN博客
2018年01月07日 13:29:38[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：193
个人分类：[毕业设计](https://blog.csdn.net/sxf_123456/article/category/7384850)
[kmeans算法并行化的mpi程序](http://www.cnblogs.com/LCcnblogs/p/6013961.html)
https://www.cnblogs.com/LCcnblogs/archive/2016/10/30/6013961.html
　　用c语言写了kmeans算法的串行程序，再用mpi来写并行版的，貌似参照着串行版来写并行版，效果不是很赏心悦目~
　　并行化思路：
　　使用主从模式。由一个节点充当主节点负责数据的划分与分配，其他节点完成本地数据的计算，并将结果返回给主节点。大致过程如下：
　　1、进程0为主节点，先从文件中读取数据集，然后将数据集划分并传给其他进程；
　　2、进程0选择每个聚类的中心点，并发送给其他进程；
　　3、其他进程计算数据块中每个点到中心点的距离，然后标出每个点所属的聚类，并计算每个聚类所有点到其中心点的距离之和，最后将这些结果返回给进程0；
　　4、进程0计算出新的中心点并发送给其他进程，并计算其他进程传来的聚类所有点到其中心点的距离总和；
　　5、重复3和4直到，直到步骤4中的所有聚类的距离之和不变（即收敛）。
　　code:
```
1 #include <stdio.h>
  2 #include <stdlib.h>
  3 #include <math.h>
  4 #include <time.h>
  5 #include "mpi.h"
  6 
  7 int  main(int argc,char *argv[])
  8 {
  9     int i,j;
 10     MPI_Status status;
 11     float temp1,temp2;
 12     int K,N,D;  //聚类的数目，数据量，数据的维数
 13     float **data;  //存放数据
 14     int *all_in_cluster;  //进程0标记每个点属于哪个聚类
 15     int *local_in_cluster;  //其他进程标记每个点属于哪个聚类
 16     int *in_cluster;  //进程0标记每个点属于哪个聚类
 17     int count=0;
 18     float *sum_diff;
 19     float *global_sum_diff;
 20     float **cluster_center;  //存放每个聚类的中心点
 21     int rank,size;
 22     float **array(int m,int n);
 23     float **loadData(int *k,int *d,int *n);
 24     float getDistance(float avector[],float bvector[],int n);
 25     void cluster(int n,int k,int d,float **data,float **cluster_center,int *local_in_cluster);
 26     float getDifference(int k,int n,int d,int *in_cluster,float **data,float **cluster_center,float *sum);
 27     void getCenter(int k,int d,int n,int *in_cluster,float **data,float **cluster_center);
 28 
 29     MPI_Init(&argc,&argv);
 30     MPI_Comm_rank(MPI_COMM_WORLD,&rank);
 31     MPI_Comm_size(MPI_COMM_WORLD,&size);
 32     if(!rank){
 33         data=loadData(&K,&D,&N);  //进程0读入数据
 34         if(size==1||size>N||N%(size-1))    MPI_Abort(MPI_COMM_WORLD,1);  //若不满足条件则退出
 35     }
 36     MPI_Bcast(&K,1,MPI_INT,0,MPI_COMM_WORLD);  //进程0广播
 37     MPI_Bcast(&N,1,MPI_INT,0,MPI_COMM_WORLD);  
 38     MPI_Bcast(&D,1,MPI_INT,0,MPI_COMM_WORLD);  
 39     if(rank)    data=array(N/(size-1),D);  //其他进程分配存储数据集的空间
 40     all_in_cluster=(int *)malloc(N/(size-1)*size*sizeof(int));  //用于进程0
 41     local_in_cluster=(int *)malloc(N/(size-1)*sizeof(int));  //用于每个进程
 42     in_cluster=(int *)malloc(N*sizeof(int));  //用于进程0
 43     sum_diff=(float *)malloc(K*sizeof(float));  //进程中每个聚类的数据点与其中心点的距离之和
 44     global_sum_diff=(float *)malloc(K*sizeof(float));
 45     for(i=0;i<K;i++)    sum_diff[i]=0.0;  //初始化
 46 
 47     if(!rank){  //进程0向其他进程分配数据集
 48         for(i=0;i<N;i+=(N/(size-1)))
 49             for(j=0;j<(N/(size-1));j++)
 50                 MPI_Send(data[i+j],D,MPI_FLOAT,(i+j)/(N/(size-1))+1,99,MPI_COMM_WORLD);  
 51         printf("Data sets:\n");
 52         for(i=0;i<N;i++)
 53             for(j=0;j<D;j++){
 54                 printf("%-8.2f",data[i][j]);
 55                 if((j+1)%D==0)    putchar('\n');
 56             }
 57            printf("-----------------------------\n");
 58     }else{  //其他进程接收进程0数据
 59         for(i=0;i<(N/(size-1));i++)
 60             MPI_Recv(data[i],D,MPI_FLOAT,0,99,MPI_COMM_WORLD,&status);
 61     }
 62     MPI_Barrier(MPI_COMM_WORLD);  //同步一下
 63     cluster_center=array(K,D);  //中心点 
 64     if(!rank){  //进程0产生随机中心点
 65         srand((unsigned int)(time(NULL)));  //随机初始化k个中心点
 66         for(i=0;i<K;i++)
 67             for(j=0;j<D;j++)
 68                 cluster_center[i][j]=data[(int)((double)N*rand()/(RAND_MAX+1.0))][j];
 69     }
 70     for(i=0;i<K;i++)    MPI_Bcast(cluster_center[i],D,MPI_FLOAT,0,MPI_COMM_WORLD);  //进程0向其他进程广播中心点
 71     if(rank){
 72         cluster(N/(size-1),K,D,data,cluster_center,local_in_cluster);  //其他进程进行聚类
 73         getDifference(K,N/(size-1),D,local_in_cluster,data,cluster_center,sum_diff);
 74         for(i=0;i<N/(size-1);i++)
 75             printf("data[%d] in cluster-%d\n",(rank-1)*(N/(size-1))+i,local_in_cluster[i]+1);
 76     }
 77     MPI_Gather(local_in_cluster,N/(size-1),MPI_INT,all_in_cluster,N/(size-1),MPI_INT,0,MPI_COMM_WORLD);  //全收集于进程0
 78     MPI_Reduce(sum_diff,global_sum_diff,K,MPI_FLOAT,MPI_SUM,0,MPI_COMM_WORLD);  //归约至进程0,进程中每个聚类的数据点与其中心点的距离之和
 79     if(!rank){  
 80         for(i=N/(size-1);i<N+N/(size-1);i++) 
 81             in_cluster[i-N/(size-1)]=all_in_cluster[i];  //处理收集的标记数组
 82         temp1=0.0;
 83         for(i=0;i<K;i++) temp1+=global_sum_diff[i];
 84         printf("The difference between data and center is: %.2f\n\n", temp1);
 85         count++;
 86     }
 87     MPI_Bcast(&temp1,1,MPI_FLOAT,0,MPI_COMM_WORLD);
 88     MPI_Barrier(MPI_COMM_WORLD);
 89 
 90     do{   //比较前后两次迭代，若不相等继续迭代
 91         temp1=temp2;
 92         if(!rank)    getCenter(K,D,N,in_cluster,data,cluster_center);  //更新中心点
 93         for(i=0;i<K;i++)    MPI_Bcast(cluster_center[i],D,MPI_FLOAT,0,MPI_COMM_WORLD);  //广播中心点    
 94         if(rank){
 95             cluster(N/(size-1),K,D,data,cluster_center,local_in_cluster);  //其他进程进行聚类
 96             for(i=0;i<K;i++)    sum_diff[i]=0.0;
 97             getDifference(K,N/(size-1),D,local_in_cluster,data,cluster_center,sum_diff);
 98             for(i=0;i<N/(size-1);i++)
 99                 printf("data[%d] in cluster-%d\n",(rank-1)*(N/(size-1))+i,local_in_cluster[i]+1);
100         }
101         MPI_Gather(local_in_cluster,N/(size-1),MPI_INT,all_in_cluster,N/(size-1),MPI_INT,0,MPI_COMM_WORLD);
102         if(!rank)
103             for(i=0;i<K;i++)    global_sum_diff[i]=0.0;
104         MPI_Reduce(sum_diff,global_sum_diff,K,MPI_FLOAT,MPI_SUM,0,MPI_COMM_WORLD);
105         if(!rank){
106             for(i=N/(size-1);i<N+N/(size-1);i++) 
107                 in_cluster[i-N/(size-1)]=all_in_cluster[i];
108             temp2=0.0;
109             for(i=0;i<K;i++) temp2+=global_sum_diff[i];
110             printf("The difference between data and center is: %.2f\n\n", temp2);
111             count++;
112         }
113         MPI_Bcast(&temp2,1,MPI_FLOAT,0,MPI_COMM_WORLD);
114         MPI_Barrier(MPI_COMM_WORLD);
115     }while(fabs(temp2-temp1)!=0.0);
116     if(!rank)    printf("The total number of cluster is: %d\n\n",count);
117     MPI_Finalize();
118 }
119 
120 
121 //动态创建二维数组
122 float **array(int m,int n)
123 {
124     int i;
125     float **p;
126     p=(float **)malloc(m*sizeof(float *));
127     p[0]=(float *)malloc(m*n*sizeof(float));
128     for(i=1;i<m;i++)    p[i]=p[i-1]+n;
129     return p;
130 }
131 
132 //从data.txt导入数据，要求首行格式：K=聚类数目,D=数据维度,N=数据量
133 float **loadData(int *k,int *d,int *n)
134 {
135     float **array(int m,int n);
136     int i,j;
137     float **arraydata;
138     FILE *fp;
139     if((fp=fopen("data.txt","r"))==NULL)    fprintf(stderr,"cannot open data.txt!\n");
140     if(fscanf(fp,"K=%d,D=%d,N=%d\n",k,d,n)!=3)    fprintf(stderr,"load error!\n");
141     arraydata=array(*n,*d);  //生成数据数组
142     for(i=0;i<*n;i++)
143         for(j=0;j<*d;j++)
144             fscanf(fp,"%f",&arraydata[i][j]);  //读取数据点
145     return arraydata;
146 }
147 
148 //计算欧几里得距离
149 float getDistance(float avector[],float bvector[],int n)
150 {
151     int i;
152     float sum=0.0;
153     for(i=0;i<n;i++)
154         sum+=pow(avector[i]-bvector[i],2);
155     return sqrt(sum);
156 }
157 
158 //把N个数据点聚类，标出每个点属于哪个聚类
159 void cluster(int n,int k,int d,float **data,float **cluster_center,int *local_in_cluster)
160 {
161     int i,j;
162     float min;
163     float **distance=array(n,k);  //存放每个数据点到每个中心点的距离
164     for(i=0;i<n;++i){
165         min=9999.0;
166         for(j=0;j<k;++j){
167             distance[i][j] = getDistance(data[i],cluster_center[j],d);
168             if(distance[i][j]<min){
169             min=distance[i][j];
170             local_in_cluster[i]=j;
171         }
172        }
173     }
174     printf("-----------------------------\n");
175     free(distance);
176 }
177 
178 //计算所有聚类的中心点与其数据点的距离之和
179 float getDifference(int k,int n,int d,int *in_cluster,float **data,float **cluster_center,float *sum)
180 {
181     int i,j;
182     for(i=0;i<k;++i)
183         for(j=0;j<n;++j)
184             if(i==in_cluster[j])
185                 sum[i]+=getDistance(data[j],cluster_center[i],d);
186 }
187 
188 //计算每个聚类的中心点
189 void getCenter(int k,int d,int n,int *in_cluster,float **data,float **cluster_center)
190 {
191     float **sum=array(k,d);  //存放每个聚类中心
192     int i,j,q,count;
193     for(i=0;i<k;i++)
194         for(j=0;j<d;j++)
195             sum[i][j]=0.0;
196     for(i=0;i<k;i++){
197         count=0;  //统计属于某个聚类内的所有数据点
198         for(j=0;j<n;j++){
199             if(i==in_cluster[j]){
200                 for(q=0;q<d;q++)
201                     sum[i][q]+=data[j][q];  //计算所属聚类的所有数据点的相应维数之和
202                 count++;
203             }
204         }
205         for(q=0;q<d;q++)
206             cluster_center[i][q]=sum[i][q]/count;
207     }
208     printf("The new center of cluster is:\n");
209         for(i = 0; i < k; i++)
210             for(q=0;q<d;q++){
211                 printf("%-8.2f",cluster_center[i][q]);
212                 if((q+1)%d==0)    putchar('\n');
213     }
214     free(sum);
215 }
```
```
1 //生成测试数据
 2 #include<stdio.h>
 3 #include<stdlib.h>
 4 #include<time.h>
 5 #define N 1000
 6 
 7 int main()
 8 {
 9     int i;
10     float a;
11     int k,d,n;
12     FILE *fp;
13     fprintf(stdout,"input(k d n):");
14     scanf("%d%d%d",&k,&d,&n);
15     if((fp=fopen("data.txt","w"))==NULL)    exit(1);
16     fprintf(fp,"K=%d,D=%d,N=%d\n",k,d,n);
17     srand((unsigned int)(time(NULL)));
18     for(i=1;i<=d*n;i++){
19         a=(int)(1.0+(double)N*rand()/(RAND_MAX+1.0));
20         fprintf(fp,"%.2f ",a);
21         if(i%d==0) putc('\n',fp);
22     }
23     if(fclose(fp)) exit(2);
24 }
```
　　实验：
　　聚类数K=10，数据的维度D=2，单位(秒)：
|数据量N|10000|100000|500000|
|----|----|----|----|
|串行|1|21|109|
|并行(2个进程)|2|25|101|
|并行(3个进程)|3|26|101|
　　分析：电脑配置是奔腾双核，按照该并行程序，一个核心用作主节点以分配数据集，另一个核心作为承担了大多数计算任务的节点。当数据量较小时，并行程序花在进程间数据通信的时间占了总体时间的很大比重，所以并行程序耗时要多于串行程序。在本电脑CPU为两个核心的环境下，当数据量较大时，并行程序与串行程序耗时相当或者稍微偏小。在CPU核心数在3个以上时，该并行程序的优势才突显出来。
