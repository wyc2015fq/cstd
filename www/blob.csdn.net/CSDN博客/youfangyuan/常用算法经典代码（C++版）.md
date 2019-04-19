# 常用算法经典代码（C++版） - youfangyuan - CSDN博客
2012年09月14日 01:05:51[youfangyuan](https://me.csdn.net/youfangyuan)阅读数：759标签：[算法																[c++																[string																[merge																[c](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=merge&t=blog)](https://so.csdn.net/so/search/s.do?q=string&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)](https://so.csdn.net/so/search/s.do?q=算法&t=blog)
个人分类：[C++](https://blog.csdn.net/youfangyuan/article/category/891239)
```cpp
一、快速排序
void qsort(int x,int y) //待排序的数据存放在a[1]..a[n]数组中
  {int h=x,r=y;
   int m=a[(x+y)>>1]; //取中间的那个位置的值
   while(h<r)
{while (a[h]<m) h++; //比中间那个位置的值小，循环直到找一个比中间那个值大的
      while (a[r]>m) r--; //比中间那个位置的值大，循环直到找一个比中间那个值小的
      if(h<=r)
{int temp=a[h];//如果此时h<=r，交换a[h]和a[r]
         a[h]=a[r];
         a[r]=temp;
         h++;r--; //这两句必不可少哦
}
}
     if(r>x) qsort(x,r);//注意此处，尾指针跑到前半部分了
     if(h<y) qsort(h,y); //注意此处，头指针跑到后半部分了
}
调用：qsort(1,n)即可实现数组a中元素有序。适用于n比较大的排序
 
二、冒泡排序
void paopao(void) //待排序的数据存放在a[1]..a[n]数组中
{for(int i=1;i<n;i++)  //控制循环（冒泡）的次数，n个数，需要n-1次冒泡
  for(int j=1;j<=n-i;j++) //相邻的两两比较
    if(a[j]<a[j+1]) {int temp=a[j];a[j]=a[j+1];a[j+1]=temp;}
}
或者
void paopao(void) //待排序的数据存放在a[1]..a[n]数组中
{for(int i=1;i<n;i++)  //控制循环（冒泡）的次数，n个数，需要n-1次冒泡
  for(int j=n-i;j>=1;j--) //相邻的两两比较
    if(a[j]<a[j+1]) {int temp=a[j];a[j]=a[j+1];a[j+1]=temp;}
}
 
调用：paopao()，适用于n比较小的排序
 
三、桶排序
void bucketsort(void)//a的取值范围已知。如a<=cmax。
 {memset(tong,0,sizeof(tong));//桶初始化
for(int i=1;i<=n;i++)//读入n个数
    {int a
cin>>a;
tong[a]++;}//相应的桶号计数器加1
  for(int i=1;i<=cmax;i++)
  {if(tong[i]>0) //当桶中装的树大于0，说明i出现过tong[i]次，否则没出现过i
     while (tong[i]!=0)
       {tong[i]--;
cout<<i<<’ ‘;}
}
}
 
桶排序适用于那些待排序的关键字的值在已知范围的排序。
 
四、合（归）并排序
void merge(int l,int m,int r)//合并[l,m]和[m+1,r]两个已经有序的区间
{ int b[101];//借助一个新的数组B，使两个有序的子区间合并成一个有序的区间，b数组的大小要注意
  int h,t,k;
  k=0;//用于新数组B的指针
  h=l;t=m+1;//让h指向第一个区间的第一个元素，t指向第二个区间的第一个元素。
  while((h<=m)&&(t<=r))//在指针h和t没有到区间尾时，把两个区间的元素抄在新数组中
    {k++;       //新数组指针加1
     if (a[h]<a[t]){b[k]=a[h];h++;}       //抄第一个区间元素到新数组
     else{b[k]=a[t];t++;}   //抄第二个区间元素到新数组
    }
  while(h<=m){k++;b[k]=a[h];h++;}  //如果第一个区间没有抄结束，把剩下的抄在新数组中
  while(t<=r){k++;b[k]=a[t];t++;}   //如果第二个区间没有抄结束，把剩下的抄在新数组中
  for(int o=1;o<=k;o++)//把新数组中的元素，再抄回原来的区间，这两个连续的区间变为有序的区间。
a[l+o-1]=b[o];
}
void mergesort(int x,int y)//对区间[x,y]进行二路归并排序
{
  int mid;
  if(x>=y) return;
  mid=(x+y)/2;//求[x,y]区间，中间的那个点mid,mid把x,y区间一分为二
  mergesort(x,mid);//对前一段进行二路归并
  mergesort(mid+1,y);//对后一段进行二路归并
  merge(x,mid,y);//把已经有序的前后两段进行合并
}
 
归并排序应用了分治思想，把一个大问题，变成两个小问题。二分是分治的思想。
 
五、二分查找
int find(int x,int y,int m) //在[x,y]区间查找关键字等于m的元素下标
{ int head,tail,mid;
  head=x;tail=y;mid=((x+y)/2);//取中间元素下标
  if(a[mid]==m) return mid;//如果中间元素值为m返回中间元素下标mid
  if(head>tail) return 0;//如果x>y，查找失败，返回0
  if(m>a[mid])  //如果m比中间元素大，在后半区间查找，返回后半区间查找结果
    return find(mid+1,tail);
  else //如果m比中间元素小，在前半区间查找，返回后前区间查找结果
    return find(head,mid-1);
}
六、高精度加法
#include<iostream>
#include<cstring>
using namespace std;
int main()
{
  string str1,str2;
  int a[250],b[250],len;   //数组的大小决定了计算的高精度最大位数
  int i;
  memset(a,0,sizeof(a));
  memset(b,0,sizeof(b));
  cin>>str1>>str2;   //输入两个字符串
  a[0]=str1.length();  //取得第一个字符串的长度
  for(i=1;i<=a[0];i++)  //把第一个字符串转换为整数，存放在数组a中
    a[i]=str1[a[0]-i]-'0';
  b[0]=str2.length();   //取得第二个字符串长度
  for(i=1;i<=b[0];i++)   //把第二个字符串中的每一位转换为整数，存放在数组B中
    b[i]=str2[b[0]-i]-'0';
  len=(a[0]>b[0]?a[0]:b[0]);   //取两个字符串最大的长度
  for(i=1;i<=len;i++)   //做按位加法，同时处理进位
  {
    a[i]+=b[i];
    a[i+1]+=a[i]/10;
    a[i]%=10;   
  }
  len++;    //下面是去掉最高位的0，然后输出。
  while((a[len]==0)&&(len>1)) len--;
  for(i=len;i>=1;i--)
    cout<<a[i];
  return 0; 
}
 
注意：两个数相加，结果的位数，应该比两个数中大的那个数多一位。
 
七、高精度减法
#include<iostream>
using namespace std;
int compare(string s1,string s2);
int main()
{
  string str1,str2;
  int a[250],b[250],len;
  int i;
  memset(a,0,sizeof(a));
  memset(b,0,sizeof(b));
  cin>>str1>>str2;
  a[0]=str1.length();
  for(i=1;i<=a[0];i++)
    a[i]=str1[a[0]-i]-'0';
  b[0]=str2.length();
  for(i=1;i<=b[0];i++)
    b[i]=str2[b[0]-i]-'0';
  if((compare(str1,str2))==0)  //大于等于，做按位减，并处理借位。
  {
    for(i=1;i<=a[0];i++)
      {a[i]-=b[i];
       if (a[i]<0) {a[i+1]--;a[i]+=10;}
      }
    a[0]++;
    while((a[a[0]]==0)&&(a[0]>1)) a[0]--;
    for(i=a[0];i>=1;i--)
      cout<<a[i];
    cout<<endl; 
  }                          
  else
  {
    cout<<'-';  //小于就输出负号
    for(i=1;i<=b[0];i++)  //做按位减，大的减小的
      {b[i]-=a[i];
       if (b[i]<0) {b[i+1]--;b[i]+=10;}
      }
    b[0]++;
    while((b[b[0]]==0)&&(b[0]>1)) b[0]--;
    for(i=b[0];i>=1;i--)
      cout<<b[i];
    cout<<endl;        
  }
  return 0; 
}
int compare(string s1,string s2)  //比较字符串（两个数）数字的大小，大于等于返回0，小于返回1。
{
  if(s1.length()>s2.length()) return 0;  //先比较长度，哪个字符串长，对应的那个数就大
  if(s1.length()<s2.length()) return 1;
  for(int i=0;i<=s1.length();i++)  //长度相同时，就一位一位比较。
  {
    if(s1[i]>s2[i]) return 0;
    if(s1[i]<s2[i]) return 1;                          
  }
  return 0;   //如果长度相同，每一位也一样，就返回0，说明相等
}
 
做减法时，首先要判断两个字符串的大小，决定是否输出负号，然后就是按位减法，注意处理借位。
 
八、高精度乘法
#include<iostream>
#include<cstring>
using namespace std;
int main()
{
  string str1,str2;
  int a[250],b[250],c[500],len;    //250位以内的两个数相乘
  int i,j;
  memset(a,0,sizeof(a));
  memset(b,0,sizeof(b));
  cin>>str1>>str2;
  a[0]=str1.length();
  for(i=1;i<=a[0];i++)
    a[i]=str1[a[0]-i]-'0';
  b[0]=str2.length();
  for(i=1;i<=b[0];i++)
    b[i]=str2[b[0]-i]-'0';
  memset(c,0,sizeof(c));
  for(i=1;i<=a[0];i++)   //做按位乘法同时处理进位，注意循环内语句的写法。
    for(j=1;j<=b[0];j++)
    {
    c[i+j-1]+=a[i]*b[j];
    c[i+j]+=c[i+j-1]/10;
    c[i+j-1]%=10;   
    }
  len=a[0]+b[0]+1;  //去掉最高位的0，然后输出
  while((c[len]==0)&&(len>1)) len--;   //为什么此处要len>1??
  for(i=len;i>=1;i--)
    cout<<c[i];
  return 0; 
}
 
注意：两个数相乘，结果的位数应该是这两个数的位数和减1。
优化：万进制
#include<iostream>
#include<cstring>
using namespace std;
void num1(int s[],string st1);
int a[2501],b[2501],c[5002];//此处可以进行2500位万进制乘法，即10000位十进制乘法。
Int main()
{  
    string str1,str2;
    int len;
    cin>>str1>>str2;
    memset(a,0,sizeof(a));
    memset(b,0,sizeof(b));
    memset(c,0,sizeof(c));
    num1(a,str1); //把str1从最低位开始，每4位存放在数组a中
    num1(b,str2); //把str2从最低位开始，每4位存放在数组b中
    for(int i=1;i<=a[0];i++) //作按位乘法并处理进位，此处是万进制进位
      for(int j=1;j<=b[0];j++)
        {
          c[i+j-1]+=a[i]*b[j];
          c[i+j]+=c[i+j-1]/10000;
          c[i+j-1]%=10000;
        }
    len=a[0]+b[0];//a[0]和b[0]存放的是每个数按4位处理的位数
    while ((c[len]==0)&&(len>1)) len--;//去掉高位的0，并输出最高位
      cout<<c[len];
    for(int i=len-1;i>=1;i--)//把剩下来的每一位还原成4位输出
      {
        if (c[i]<1000) cout<<’0’;
        if (c[i]<100) cout<<’0’;
        if (c[i]<10) cout<<’0’;        
        cout<<c[i];
      }
    cout<<endl;
    return 0;
}
void num1(int s[],string st1)//此函数的作用就是把字符串st1，按4位一组存放在数组s中
{   int k=1,count=1;
    s[0]=st1.length();//存放st1的长度，省去一长度变量
    for(int i=s[0]-1;i>=0;i--) //从最低位开始，处理每一位
    { if (count%4==0) {s[k]+=(st1[i]-‘0’)*1000; if(i!=0) k++;}
      if (count%4==1) s[k]=(st1[i]-‘0’);
      if (count%4==2) s[k]+=(st1[i]-‘0’)*10;
      if (count%4==3) s[k]+=(st1[i]-‘0’)*100;
      count++;
    }
    s[0]=k; //存放数组的位数，就是按4位处理后的万进制数的位数。  
    Return;
}
 
九、高精度除法（没讲）
 
十、筛选法建立素数表
void maketable(int x)//建立X以内的素数表prim，prim[i]为0，表示i为素数，为1表示不是质数
{
 memset(prim,0,sizeof(prim));//初始化质数表
 prim[0]=1;prim[1]=1;prim[2]=0;//用筛选法求X以内的质数表
 for(int i=2;i<=x;i++)
    if (prim[i]==0)
     {int j=2*i;
      while(j<=x)
       {prim[j]=1;j=j+i;}
}
}
 
对于那些算法中，经常要判断素数的问题，建立一个素数表，可以达到一劳永逸的目的。
 
十一、深度优先搜索
void dfs(int x)  \\以图的深度优先遍历为例。
    { 
      cout<<x<<‘ ‘; \\访问x顶点
      visited[x]=1; \\作已访问的标记
      for(int k=1;k<=n;k++) \\对与顶点x相邻而又没访问过的结点k进行深度优先搜索。
        if((a[x][k]==1)&&(visited[k]==0))
         dfs(k);
    }
十二、广度优先搜索
void  bfs(void) //按广度优先非递归遍历图G，n个顶点，编号为1..n。注：图不一定是连通的
{//使用辅助队列Q和访问标记数组visited。
    for(v=1;v<=n;v++)  visited[v]=0；//标记数组初始化
    for(v=1; v<=n; v++)
      if(visited[v]==0 ) {        //v尚未访问
         int h=1,r=1;    //置空的辅助队列q
         visited[v]=1；//顶点v,作访问标记
         cout<<v<<‘ ‘; //访问顶点v
         q[r]=v；    //v入队列
         while(h<=r) //当队列非空时循环
 {
             int tmp=q[h];  //队头元素出队，并赋值给tmp
             for(int j=1;j<=n;j++)
               if((visited[j]==0)&&(a[tmp][j]==1))
{//j为tmp的尚未访问的邻接顶点
                    visited[j]=1;  对j作访问标记
                    cout<<j<<‘ ‘; 访问j
                    r++; //队尾指针加1
q[r]=j; //j入队
}  //end-if
             h++;
           }//end -while
}
十三、二叉树的前序、中序和后序遍历
void preorder(int x)//二叉树的先序遍历
 {
   if(x==0) return;
   cout<<x;//先访问根
   preorder(a[x].ld);//再先序遍历根的左子树
   preorder(a[x].rd);//最后先序遍历根的右子树
}
 
void inorder(int x)//二叉树的中序遍历
 {
   if(x==0) return;
   preorder(a[x].ld);//先中序遍历根的左子树
   cout<<x;//再访问根
   preorder(a[x].rd);//最后中序遍历根的右子树
}
 
void reorder(int x)//二叉树的后序遍历
 {
   if(x==0) return;
   preorder(a[x].ld);//先后序遍历根的左子树
   preorder(a[x].rd);//再后序遍历根的右子树
   cout<<x;//最后访问根
}
 
 
十四、树转换为二叉树算法
 
十五、二叉排序树
 
十六、哈夫曼树
void haff(void) //构建哈夫曼树
{
   for(int i=n+1;i<=2*n-1;i++) //依次生成n-1个结点
     {int l=fmin(i-1); //查找权值最小的结点的编号l
       a[i].lchild=l; //把l作为结点i的左孩子
       a[l].father=i; //把l的父结点修改为i
       int r=fmin(i-1); //查找次小权值的编号r
       a[i].rchild=r; //把l作为结点i的右孩子
       a[r].father=i; //把r的父结点修改为i
       a[i].da=a[l].da+a[r].da; //合并l,j结点，生成新结点i
     }
}
int fmin(int k)//在1到K中寻找最小的权值的编号
       {
         int mins=0;
         for(int s=1;s<=k;s++)
           if((a[mins].da>a[s].da)&&(a[s].father==0)) //a[s].father=0,说明这个结点还不是别个结点
mins=s;                           //的孩子，不等于0说明这个结点已经用过。
         return mins;
       }
void inorder(int x)//递归生成哈夫曼编码
{
  if(a[x].father==0) {a[x].code=”“;}//根结点
  if(a[a[x].father].lchild==x)  a[x].code=a[a[x].father].code+'0';
  if(a[a[x].father].rchild==x)  a[x].code=a[a[x].father].code+'1';
  if(a[x].lchild!=0) inorder(a[x].lchild);//递归生成左子树
  if((a[x].lchild==0)&&(a[x].rchild==0))//输出叶子结点
     cout<<a[x].da<<':'<<a[x].code<<endl;
  if(a[x].rchild!=0) inorder(a[x].rchild);//递归生成右子树
}
十七、并查集
int getfather(int x)//非递归求X结点的根结点的编号
{while(x!=father[x])
  x=father[x];
 return x;
}
 
int getfather(int x)//递归求X结点的根结点的编号
{if(x==father[x]) return x;
 else return getfather(father[x]); 
}
 
int getfather(int x)//非递归求X结点的根结点编号同时进行路径压缩
{int p=x;
while(p!=father[p])//循环结束后，P即为根结点
   p=father[p];
 while(x!=father[x])//从X结点沿X的父结点进行路径压缩
   {int temp=father[x];//暂存X没有修改前的父结点
father[x]=p;//把X的父结点指向P
x=temp;
   }
 return p;
}
 
int getfather(int x)//递归求X结点的根结点编号同时进行路径压缩
{if(x==father[x]) return x;
 else {
       int temp=getfather(father[x]);
       father[x]=temp;
       return temp;
}
}
 
void merge(int x,int y)//合并x,y两个结点
 {int x1,x2;
  x1=getfather(x);//取得X的父结点
  x2=getfather(y);//取得Y的父结点
  if(x1!=x2) father[x1]=x2; //两个父结点不同的话就合并，注意：合并的是X，Y两个结点的根。
}
 
十八、Prime算法
void prime(void) //prim算法求最小生成树，elist[i]是边集数组，a[i][j]为<I,j>的权值。edge为结构体类型。
{for (int i=1;i<=n-1;i++)//初始化结点1到其它n-1个结点形成的边集
   {elist[i].from=1;
elist[i].to=i+1;
elist[i].w=a[1][i+1];
   }
 for (int i=1;i<=n-1;i++)//依次确定n-1条边
  {int m=i;
   for(int j=i+1;j<=n-1;j++)//确定第i条边时，依次在i+1至n-1条边中找最小的那条边
     if(elist[j].w<elist[m].w) m=j;
   if(m!=i) //如果最小的边不是第i条边就交换
{edge tmp=elist[i];elist[i]=elist[m];elist[m]=tmp;}
   for(int j=i+1;j<=n-1;j++)//更新第i+1至n-1条边的最小距离。
     {if(elist[j].w>a[elist[i].to][elist[j].to]) elist[j].w=a[elist[i].to][elist[j].to];}
}                            
for(int i=1;i<=n-1;i++)//求最小生成树的值
ans=ans+elist[i].w;
}                       
 
如果要求出哪些边构成最小生成树，在更新第i+1至n-1条边到已经生成的树中最小距离时(上面代码中加粗的部分)，还要加上elist[j].from=elist[i].to;语句，即在更新权值时，还应该更新起点。
Prime算法适用于顶点不是太多的稠密图，如果对于顶点数较多的稀疏图，就不太适用了。
 
十九、Dijkstra算法
void dijkstra(int x)  //求结点x到各个结点的最短路径
{
memset(vis,0,sizeof(vis)); //初始化，vis[i]＝0表示源点到结点i未求，否则已求
vis[x]=1;pre[x]=0; //初始化源点。
for(int i=1;i<=n;i++)   //对其它各点初始化。
    if(i!=x)
{
dis[i]=g[x][i];
pre[i]=x;
}
for(int i=1;i<=n-1;i++)   //对于n个结点的图，要求x到其它n-1个结点的最短距离
    {
int m=big; //虚拟一个最大的数big=99999999;
int k=x;
      for(int j=1;j<=n;j++)   //在未求出的结点中找一个源点到其距离最小的点
        if(vis[j]==0&&m>dis[j])
{
m=dis[j];
k=j;
}
      vis[k]=1;   //思考：如果k=X说明什么？说明后面的点，无解。
      for(int j=1;j<=n;j++)   //用当前找的结点更新未求结点到X的最短路径
      if((vis[j]==0)&&(dis[k]+g[k][j]<dis[j]))
        {   
         dis[j]=dis[k]+g[k][j];  //更新
         pre[j]=k;  //保存前趋结点，以便后面求路径
        }
    }
}
说明：dis[i]表示x到i的最短距离，pre[i]表示i结点的前趋结点。
二十、Kruscal算法
void qsort(int x,int y)//对边集数组进行快速排序
{int h=x,r=y,m=elist[(h+r)>>1].w;
 while(h<r)
  {while(elist[h].w<m) h++;
   while(elist[r].w>m) r--;
   if(h<=r)
    {edge tmp=elist[h];elist[h]=elist[r];elist[r]=tmp;h++;r--;}
  }
 if(x<r) qsort(x,r);
 if(h<y) qsort(h,y);
}
 
int getfather(int x)//找根结点，并压缩路径，此处用递归实现的。
{if(x==father[x]) return x;
 else {
        int f=getfather(father[x]);
        father[x]=f;
        return f;
      }
}
 
void merge(int x,int y)//合并x,y结点，在此题中的x,y为两个根结点。
{father[x]=y;}
 
void kruscal(void)
{int sum=0,ans=0;
qsort(1,t);//对t条边按权值大小按从小到大的次序进行快速排序
   for(int i=1;i<=t;i++)
     {int x1=getfather(elist[i].from);//取第i条边的起点所在的树的根
int x2=getfather(elist[i].to);// 取第i条边的终点所在的树的根
if(x1!=x2)
{sum++;merge(x1,x2);ans+=elist[i].w;}//不在同一个集合，合并，即第i条边可以选取。
if(sum>n-1)
break;//已经确定了n-1条边了，最小生成树已经生成了，可以提前退出循环了
}
   if(sum<n-1)
cout<<"Impossible"<<endl; //从t条边中无法确定n-1条边，说明无法生成最小生成树
   else  
cout<<ans<<endl;
  }
 
克鲁斯卡尔算法，只用了边集数组，没有用到图的邻接矩阵，因此当图的结点数比较多的时候，输入数据又是边的信息时，就要考虑用Kruscal算法。对于岛国问题，我们就要选择此算法，如果用Prim算法，还要开一个二维的数组来表示图的邻接矩阵，对于10000个点的数据，显然在空间上是无法容忍的。
 
二十一、Floyed算法
void floyed(void)// a[i][j]表示结点i到结点j的最短路径长度，初始时值为<I,J>的权值。
{for(int k=1;k<=n;k++) //枚举中间加入的结点不超过K时f[i][j]最短路径长度，K相当DP中的阶段
        for(int i=1;i<=n;i++) //i，j是结点i到结点J，相当于DP中的状态
for(int j=1;j<=n;j++)
      if (a[i][j]>a[i][k]+a[k][j]) a[i][j]=a[i][k]+a[k][j];//这是决策，加和不加中间点，取最小的值
}
 
弗洛伊德算法适合于求没有负权回路的图的最短路径长度，利用FLOYED算法，可写出判断结点i和结点J是否连通的算法。
 
二十二、01背包问题
n为物品的数量，w[i]表示第i个物品的重量，c[i]表示第i个物品的价值，v为背包的最大重量。
有状态转移方程f[i][j]=max{f[i-1][j],f[i-1][j-w[i]]+c[i]}。f[i][j]表示前i个物品，在背包载重为j时获得的最大价值。显然f[n][v]即为所求。边界条件为f[0][s]=0,s=0,1,…,v。
for(int i=1;i<=n;i++)//枚举阶段
 for(int j=0;j<=v;j++)//枚举状态，当然此处也可写成:for(int j=v;j>=0;j--)
   {
f[i][j]=f[i-1][j];//不选第i个物品
if(f[i][j]<f[i-1][j-w[i]]+c[i]) f[i][j]=f[i-1][j-w[i]]+c[i];//选第i个物品
}
cout<<f[n][v]<<endl;//输出结果。
 
优化：用一维数组实现，把第i-1阶段和第i阶段数据存在一块。
for(int i=1;i<=n;i++)//枚举阶段
 for(int j=v;j>=0;j--)//枚举状态，当然此处也可写成:for(int j=v;j>=0;j--)
   {
f[j]=f[j];//不选第i个物品，可省略此语句。
     if((j>w[i])&&(f[j]<f[j-w[i]]+c[i])) f[j]=f[j-w[i]]+c[i];//选第i个物品
}
cout<<f[v]<<endl;//输出结果。
 
对比优化前后，我们不难发现，优化后的代码实际上就是在原来基本的代码基础上，减少了阶段这一维，同时在枚举状态时，为了保证结果的正确性，枚举的顺序只能是v到0，而不能是0到v。大家细想一下为什么？就是保证在求第i阶段j状态时，f[j-w[i]]为第i-1阶段的值。
 
进一步优化，在上面代码中，枚举状态时，还可以写成for(int j=v;j>=w[i];j--)，此时下面的判断条件j>=w[i]就可以省略了。
 
二十三、完全背包问题
和01背包问题不同的是，完全背包，对于任何一个物品i，只要背包重量允许，可以多次选取，也就是在决策上，可以选0个，1个，2个，…，v/w[i]个。
状态转移方程f[i][j]=max{f[i-1][j],f[i-1][j-w[i]]+c[i]，f[i-1][j-2*w[i]]+2*c[i]，…，f[i-1][j-k*w[i]]+k*c[i]}。k=0，1，2，…，v/w[i]。f[i][j]表示前i个物品，在背包载重为j时获得的最大价值。显然f[n][v]即为所求。边界条件为f[0][s]=0,s=0,1,…,v。
for(int i=1;i<=n;i++)//枚举阶段
 for(int j=0;j<=v;j++)//枚举状态，当然此处也可写成:for(int j=v;j>=0;j--)
    {f[i][j]=f[i-1][j];//k=0的情况作为f[i][j]的初始值，然后在k=1,2,…,v/w[i]中找最大值
    for(int k=1;k<=v/w[i];k++)  
if(f[i][j]<f[i-1][j-k*w[i]]+k*c[i]) f[i][j]=f[i-1][j-k*w[i]]+k*c[i];//选第i个物品
}
cout<<f[n][v]<<endl;//输出结果。
 
二十四、多属性背包问题
 
二十五、多背包问题
 
二十六、最长不降（上升）子序列问题
 
   f[i]表示从第1个数开始，以第i个数结尾的最长递增子序列。
 
状态转移方程：f[i]=max{f[j]}+1 （1≤j≤i-1，1≤i≤n，a[i]≥a[j]）
临界状态：f[1]=1;
 
二十七、最长公共子序列问题
 
   f[i][j]表示第一个串前i个字符和第二个串前j个字符的最长公共子序列数。
   状态转移方程： 
       f[i-1][j-1]                 (若a[i]==b[j])
f[i][j]=
                          max{f[i-1][j],f[i][j-1]}+1     (若a[i]≠b[j])
 
   临界状态:f[0][j]=0，f[i][0]=0
```
