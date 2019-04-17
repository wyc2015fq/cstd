# dancing links 算法 解 Sudoku - 囧囧有神的专栏 - CSDN博客





2010年10月12日 00:02:00[囧囧有神](https://me.csdn.net/liujiyong7)阅读数：5856








** 1．Danclink links ****算法解Sudoku的基本思想：**



将Sudoku问题转化为等价的01矩阵问题，然后用dlx算法求解。从一个由0
 1组成的矩阵中找到一个行的集合，使得集合中每列恰好包含一个1，称为01矩阵问题。解决它的dancling
 links 算法利用双向链表的一个性质，巧妙的剪枝，获得了较好的性能



**A）构造01矩阵**

01矩阵问题是什么？

01矩阵是这样一个问题：：给定一个由0和1组成的矩阵，是否能找到一个行的集合，使得集合中每一列都恰好包含一个1？

例如，下面这个矩阵


|**![](http://hi.csdn.net/attachment/201010/12/0_12868564198BvU.gif)**|(3)|
|----|----|

包含了这样一个集合（第1，4，5行）。

第1行：0
 0 1 0 1 1 0

第4行：1
 0 0 1 0 0 0

第5行：0
 1 0 0 0 0 1

每1列恰1个1，符合条件。

如何用Sudoku构造01矩阵

Sudoku问题可以转化为01矩阵问题。可以知道，Sudoku有如下四个约束条件

1)每一个数（1到9）在每一行出现且仅出现一次。

2)每一个数（1到9）在每一列出现且仅出现一次。

3)每一个数（1到9）在每一个九宫格出现且仅出现一次。

4)每一个格子出现且仅出现一个数。

注意：第四个条件很容易漏掉，但这是必不可少的。



下面是Sudoku转化为01矩阵的方法

1）令r表示行，c表示列，b表示九宫格，k表示值，i表示一个格子，那么rk，ck，bk,
 i

即可表示4个约束条件，（例如rk=15，表示第一行只有1个5，bk=32，表示第3个九宫格只有1个2，i=32表示第32个格子只有一个数。因为r,
 c , b, k 范围为1到9，i范围为1到81，所以总共有9*9*3+81=324种情形，即表示01矩阵中有324列）



2）考虑每一个格子的情况，如果第2行第4列是5（rck=245）那么可知rk=25，ck=45，
 bk=25， i=13这四列下的1找到了。（为什么bk=25，因为按从左到右，从上到下的顺序，第2行第四列恰为第2个九宫格，同理i=13也是这个道理），那么可以生成这样一行01序列

0 0 0 0……1…0
 0 0…1…0 0 0…1…0
 0 0 0 0 0 0 0…1……0 0 0 0

这一行总共324个数，表示324行，其中4个1分别位于rk=25，ck=45，
 bk=25， i=13这四列。

假如第4行第2列是未知的。那么可知第4行第2列可能取值为1到9，共九种可能。（即rck=421，
 422，423， 424，……429）这9种可能可以生成01矩阵中的9行。例如rck=422可以生成这样一行0
 0 0 0 ……1…0 0 0…1…0
 0 0…1…0 0 0 0 0 0 0 0…1……0
 0 0 0。其中，四个一位于rk=42，ck=22，
 bk=42， i=29这四列下。



根据以上可构造出1个01矩阵，可知每一列下只可恰好包含一个1。如果我们选出一些行（确切来说是81行）使符合这个条件，那么就可以得到Sudoku的结果。那么danclink
 link X 算法是如何求解的呢？



**B****）dancing link X 解01矩阵**

**考虑下列的求解过程：**

如果01矩阵A是空的，问题解决；成功终止。
否则，选择一个列c（确定的）。
选择一个行r，满足 A[r,
 c]=1 （不确定的。注意这里的r c是矩阵A的行与列，与上面不同）。
把r包含进部分解。（假设这第r行是所求行集合的一部分）
对于所有满足 A[r,j]=1的j，
　　从矩阵A中删除第j列；
　　对于所有满足 A[i,j]=1的i，
　　　　从矩阵A中删除第i行。
在不断减少的矩阵A上递归地重复上述算法。



如果我们删到最后得到A是空的。那么我们就得到了01矩阵问题的一个解。问题是当我们得不到A是空的时候，我们必须回溯，我们必须把已经错误删掉的一些行与列恢复。这就会用到双向链表的一个有用性质。



**以下详细算法**：

一个实现X算法的好方法就是将矩阵A中的每个1用一个有5个域L[x]、R[x]、U[x]、D[x]、C[x]的*数据对象*（*data object*）x来表示。矩阵的每行都是一个经由域L和R(“左”和“右”)双向连接的环状链表；矩阵的每列是一个经由域U和D(“上”和“下”)双向连接的环状链表。每个列链表还包含一个特殊的数据对象，称作它的*表头*（*list header*）。
这些表头是一个称作*列对象*（*column object*）的大型对象的一部分。每个列对象y包含一个普通数据对象的5个域L[y]、R[y]、U[y]、D[y]和C[y]，外加两个域S[y]（大小）和N[y]（名字）；这里“大小”是一个列中1的个数，而“名字”则是用来标识输出答案的符号。每个数据对象的C域指向相应列头的列对象。
表头的L和R连接着所有需要被覆盖的列。这个环状链表也包含一个特殊的列对象称作“根”，h，它相当于所有活动表头的主人。而且它不需要U[h]、D[h]、C[h]、S[h]和N[h]这几个域。
举个例子，（3）中的0-1矩阵将用这些数据对象来表示，就像图2展示的那样，我们给这些列命名为A、B、C、D、E、F和G（这个图表在上下左右处“环绕扭曲”。C的连线没有画出，因为他们会把图形弄乱；每个C域指向每列最顶端的元素）。


|**![](http://hi.csdn.net/attachment/201010/12/0_1286856432Dv3z.gif)**|
|----|
|图2 完全覆盖问题（3）的四方向连接表示法|

我们寻找所有精确覆盖的不确定性算法现在可以定型为下面这个明析、确定的形式，即一个递归过程search(k)，它一开始被调用时k=0：

如果 R[h]=h，打印当前的解（见下）并且返回。
否则选择一个列对象c（见下）。
覆盖列c（见下）。
对于每个r←D[c]，D[D[c]]，……，当 r!=c，
　　设置 Ok<-r；
　　对于每个j←R[r]，R[R[r]]，……，当 j!=r，
　　　　覆盖列j（见下）；
search(k+1)；
　　设置 r←Ok且 c←C[r]；
　　对于每个j←L[r]，L[L[r]]，……，当 j!=r，
　　　　取消列j的覆盖（见下）。
取消列c的覆盖（见下）并且返回。

输出当前解的操作很简单：我们连续输出包含O0、O1、……、Ok-1的行,这里包含数据对象O的行可以通过输出N[C[O]]、N[C[R[O]]]、N[C[R[R[O]]]]……来输出。

为了选择一个列对象c,我们可以简单地设置c<-R[h]；这是最左边没有覆盖的列。或者如果我们希望使分支因数达到最小，我们可以设置s<-无穷大，那么接下来：

对于每个j←R[h]，R[R[h]]，……，当
 j!=h，
　　如果 S[j]<s 设置 c←j且 s←S[h]。

那么c就是包含1的序数最小的列(如果不用这种方法减少分支的话，S域就没什么用了)。
覆盖列c的操作则更加有趣：把c从表头删除并且从其他列链表中去除c链表的所有行。

设置 L[R[c]]←L[c]且 R[L[c]]←R[c]。
对于每个i←D[c]，D[D[c]]，……，当 i!=c，
　　对于每个j←R[i]，R[R{i]]，……，当 j!=i，
　　　　设置 U[D[j]]←U[j]，D[U[j]]←D[j]，
　　　　并且设置 S[C[j]]←S[C[j]]-1。

操作（1），就是我在本文一开始提到的，在这里他被用来除去水平、竖直方向上的数据对象。
最后，我们到达了整个算法的尖端，即还原给定的列c的操作。这里就是链表舞蹈的过程：

对于每个i←U[c]，U[U[c]]，……，当
 j!=i，
　　对于每个j←L[i]，L[L[i]]，……，当 j!=i，
　　　　设置 S[C[j]]←S[C[j]]+1，
　　　　并且设置 U[D[j]]←j，D[U[j]]←j。
设置 L[R[c]]←c且 R[L[c]]←c。

注意到还原操作正好与覆盖操作执行的顺序相反，我们利用操作（2）来取消操作（1）。（其实没必要严格限制“后执行的先取消”，由于j可以以任何顺序穿过第i行；但是从下往上取消对行的移除操作是非常重要的，因为我们是从上往下把这些行移除的。相似的，对于第r行从右往左取消列的移除操作也是十分重要的，因为我们是从左往右覆盖的。）


|![](http://hi.csdn.net/attachment/201010/12/0_12868564447Vhm.gif)|
|----|
|图3 图2中第A列后面的链表被覆盖|

考虑一下，例如，对图2表示的数据（3）执行search(0)会发生什么。通过从其他列移除A的行来将其覆盖；那么现在整个结构就成了图3的样子。注意现在D列出现了不对称的链接：上面的元素首先被删除，所以它仍然指向初始的邻居，但是另一个被删除的元素指向了列头。
继续search(0)，当r指向（A，D，G）这一行的A元素时，我们也覆盖D列和G列。图4展示了我们进入search(1)时的状态，这个数据结构代表削减后的矩阵


|![](http://hi.csdn.net/attachment/201010/12/0_12868564531Uu6.gif)|（4）|
|----|----|

现在search(1)将覆盖B列，而且C列将没有“1”。因此search(2)将什么也找不到。接着search(1)会找不到解并返回，图4的状态会恢复。外部的过程，search(0)，将把图4变回图3，而且它会让r前进到（A，D）行的A元素处。


|![](http://hi.csdn.net/attachment/201010/12/0_1286856510gN5M.gif)|
|----|
|图4 图3中D列和G列后的链被覆盖|

很快就能找到解，并输出

|A|D||
|----|----|----|
|B|G||
|C|E|F|


如果在选择c的时候无视S域，会输出

|A|D||
|----|----|----|
|E|F|C|
|B|G||


以下是C++源码    输入为长度为81的字符串， 点.表示未知位置，数字表示已知的值

**#include<iostream>using namespace std;#define RR 729                 // 9*9*9  表示rck总数目 r代表行，c代表列 k代表值 比如123 代表一行二列 值是三#define CC 324                 // 9*9*4  表示 rk，ck，bk，rc 总个数 分别表示四个约束条件，每个数每行只能有一个 每列只能一个 每个九宫格只能一个  每个格子只能有一个数  #define INF 1000000000         // 假定最大值  int mem[RR+9];int ans[RR+9];                 // 解char ch[RR+9];                 // 谜题数组  记录题目的  由人输入int cnt[CC+9];                 // 计数数组struct node{                           int r,c;            //位置    node *up;           // 上下左右四个指针    node *down;    node *left;    node *right;}head,all[RR*CC+99],row[RR],col[CC]; //头指针， 所有节点，行数组 列数组int all_t;    // inline void link(int r,int c){	//将坐标值为 rc 的节点 链接到 第r 行  第 c列	//    cnt[c]++;    node *t=&all[all_t++];    t->r=r;    t->c=c;// 构造一个节点，位置rc      t->left=&row[r];    t->right=row[r].right;    t->left->right=t;    t->right->left=t;// 将t 插入到第r行这个环里    t->up=&col[c];    t->down=col[c].down;    t->up->down=t;    t->down->up=t; // 将t 插入到第c列 这个环里}inline void remove(int c){	// 覆盖第c列	// 覆盖列c的操作则更加有趣：把c从表头删除并且从其他列链表中去除c链表的所有行。	//设置 L[R[c]]←L[c] 且 R[L[c]]←R[c]。	//对于每个i←D[c]，D[D[c]]，……，当 i!=c，	//对于每个j←R[i]，R[R{i]]，……，当 j!=i，	//设置 U[D[j]]←U[j]，D[U[j]]←D[j]，	//并且设置 S[C[j]]←S[C[j]]-1。*/    node *t,*tt;    col[c].right->left=col[c].left;    col[c].left->right=col[c].right; // 将c 从链表col 中删除    for(t=col[c].down;t!=&col[c];t=t->down)    {		// 从 c列 依次往下走 遍历每个节点t 删掉每个节点t 所在的行        for(tt=t->right;tt!=t;tt=tt->right)        {			//从t 往右走  ，从上下方向删掉每个节点tt            cnt[tt->c]--; // 该列计数减一            tt->up->down=tt->down;            tt->down->up=tt->up; //  将tt从上下方向删掉        }        t->left->right=t->right;        t->right->left=t->left; // 删掉t所在的行后 将t从左右方向删掉    }}inline void resume(int c){	// remove 的逆过程     node *t,*tt;    for(t=col[c].down;t!=&col[c];t=t->down)    {                t->right->left=t;        t->left->right=t;        for(tt=t->left;tt!=t;tt=tt->left)        {            cnt[tt->c]++;            tt->down->up=tt;            tt->up->down=tt;        }    }        col[c].left->right=&col[c];    col[c].right->left=&col[c];}int solve(int k){	// 搜索解路径	// 请仔细阅读 dancing links 的论文    if(head.right==&head)        return 1; // 递归终止条件    node*t,*tt;    int min=INF,tc;    for(t=head.right;t!=&head;t=t->right)    {		//从头结点开始依次往右走  找到一个 cnt 最小的列 记录为tc        if(cnt[t->c]<min)        {            min=cnt[t->c];            tc=t->c;            if(min<=1)break;        }    }    remove(tc); // 覆盖掉列tc   删掉tc列 及tc列中每个节点所在的行	int scnt=0; // 解计数 初始化为0    for(t=col[tc].down;t!=&col[tc];t=t->down)    {        mem[k]=t->r; // 将 tc列 中一个节点t 所在的行 记录在 men[k]中 ，到时候 答案从这里计算得到        t->left->right=t;// 将t 与原左边 的节点又连接上        for(tt=t->right;tt!=t;tt=tt->right)         {			// t往右走，将t 所在行所有节点所在的列覆盖掉            remove(tt->c);        }        t->left->right=t->right;// 将t 与左边的节点又断开        scnt+=solve(k+1); // 递归调用 solve 寻找下一个节点        t->right->left=t; // t 与右边节点又链接上        for(tt=t->left;tt!=t;tt=tt->left)        {			//t 往左走，将t所在行所有节点所在列恢复            resume(tt->c);        }        t->right->left=t->left;// t与右边节点又断开    }    resume(tc); // 恢复列tc    return scnt;// 返回 解计数}int main(){    double ss=0;	/*FILE *fp,*fs;	fp = fopen("puzzle.txt","r");	fs = fopen("solution.txt","a");	if (NULL == fp || NULL == fs) 		printf("file open error!/n");	else*/    while(gets(ch))    {	//while(fgets(ch,82,fp)!=NULL) {		// 得到谜题 ch        int i;//,j,k;        if(ch[0]=='e')break;        all_t=1;        memset(cnt,0,sizeof(cnt)); // 给cnt 初始化 为0        head.left=&head;        head.right=&head;        head.up=&head;        head.down=&head;        head.r=RR;        head.c=CC; // 初始化头结点        for(i=0;i<CC;i++)        {			// 初始化 列数组， 将每个节点与左右节点链接，  上下方向自己指向自己            col[i].c=i;            col[i].r=RR;            col[i].up=&col[i];            col[i].down=&col[i];            col[i].left=&head;            col[i].right=head.right;            col[i].left->right=&col[i];            col[i].right->left=&col[i];        }        for(i=0;i<RR;i++)        {			// 初始化行数组  每个节点 与上下节点 链接  左右方向指向自己            row[i].r=i;            row[i].c=CC;            row[i].left=&row[i];            row[i].right=&row[i];            row[i].up=&head;            row[i].down=head.down;            row[i].up->down=&row[i];            row[i].down->up=&row[i];        }        for(i=0;i<RR;i++)        {			// 根据谜题 构造一个双十字链表。            int r=i/9/9%9;// 这个r 是在 9*9格中的 行            int c=i/9%9; //  列            int val=i%9+1;// 值             if(ch[r*9+c]=='.'||ch[r*9+c]=='0'||ch[r*9+c]==val+'0')            {				//如果Sudoku相应格子处的值 未知， 或者刚好就是符合 就连上相应节点                link(i,r*9+val-1);  // 连上 i rk 处的节点  rk 范围 0 到80 9*9                link(i,81+c*9+val-1);//  i 1ck 处的节点    1ck 范围 81到161                 int tr=r/3;                int tc=c/3; // tr tc 九宫格位置                link(i,162+(tr*3+tc)*9+val-1); // i 2bk 处的节点  2bk 范围 162到241                link(i,243+r*9+c);  //  i rc 处的节点             }        }        for(i=0;i<RR;i++)        {			//构造完成后 将 row数组 删除            row[i].left->right=row[i].right;            row[i].right->left=row[i].left;        }        int scnt=solve(1); // 搜索解路径        for(i=1;i<=81;i++)        {            int t=mem[i]/9%81; // 计算t=rc  在九宫格中的位置            int val=mem[i]%9+1;// rc位置处的值            ans[t]=val; // 保存在ans数组中        }	//	for(i=0;i<81;i++)	//		printf("%d",ans[i]); // 将答案打印出来	//	printf("/n");	//	printf("scnt=%d/n",scnt);  // 打印 解的个数				for (i=0;i<81;i++) {			fprintf(fs,"%d",ans[i]);		}		fprintf(fs,"/n");			fgets(ch,80,fp);   //取空行    }	fclose(fp);	fclose(fs);	return 0;}**

**以上内容参考了网上很多资料，在此表示谢意**



