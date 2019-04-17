# 【rcmp】@CQU2014 校赛_B.Board 标程阅读 - CD's Coding - CSDN博客





2014年05月11日 15:44:58[糖果天王](https://me.csdn.net/okcd00)阅读数：610标签：[c++																[cqu																[rcmp](https://so.csdn.net/so/search/s.do?q=rcmp&t=blog)
个人分类：[Coding Competition](https://blog.csdn.net/okcd00/article/category/2133459)





                
Problem B.Board

input: board.inoutput: board.out



ACM国际大学生程序设计竞赛（英语：ACM International Collegiate Programming Contest, ICPC）是由美国计算机协会（ACM）主办的，一项旨在展示大学生创新能力、团队精神和在压力下编写程序、分析和解决问题能力的年度竞赛。经过30多年的发展，ACM国际大学生程序设计竞赛已经发展成为最具影响力的大学生计算机竞赛。赛事目前由IBM公司赞助。

ACM-ICPC以团队的形式代表各学校参赛，每队由3名队员组成。

比赛期间，每队使用1台电脑需要在5个小时内使用C、C++或Java中的一种编写程序解决7到10个问题。程序完成之后提交裁判运行，运行的结果会判定为"AC(正确)/WA（错误）/TLE（超时）/MLE（超出内存限制）/RE（运行错误）/PE（格式错误）"中的一种并及时通知参赛队。每队在正确完成一题后，组织者将在其位置上升起一只代表该题颜色的气球。

最后的获胜者为正确解答题目最多且总用时最少的队伍（正确解答题目更多的队伍排名靠前，若两个解答题目相同时总用时较少的队伍排名靠前）。每道试题用时将从竞赛开始到试题解答被判定为正确为止，其间每一次提交运行结果被判错误的话将被加罚20分钟时间，未正确解答的试题不记时，已经通过的题目再次被提交无论评判结果均不记时。

例如：A、B两队都正确完成两道题目，其中A队提交这两题的时间分别是比赛开始后60分钟和165分钟，B队为80分钟和120分钟，但B队有一题提交了2次。这样A队的总用时为60+165=225而B队为80+120+20=220，所以B队以总用时少而获胜。

在比赛期间以及赛后的成绩排名榜单通常被称为board，现在给出一场比赛的所有提交及其评判结果（这里简单的分为Yes代表正确，No代表错误），需要你编写程序输出该场比赛的board，即最终排名及每个队伍各自的解题数和总用时。



(Note: 以上部分描述摘自Wikipedia)



Input

输入第一行一个整数T(T<=10)代表输入数据组数

每组数据第一行两个整数N,M(N<=200 M<=3000)，分别代表这场比赛的参赛队伍数和提交总数

每组数据第二行起共M行，每行由三个整数t, id, p和一个字符串res组成，以描述每一次提交；其中t代表提交时间，id表示提交的队伍编号，p代表提交的题目编号，res代表提交的结果(0<=t<300, 1<=id<=N, 1001<=p<=1010, res只会是”Yes”或”No”)

保证同一只队伍在同一分钟内不会有超过一次的提交记录

如果出现多只队伍有完全相同的解题数和总用时，则先输出队伍编号较小的队伍



Output

对于每组输入输出一组结果：

每组输出第一行为Case #C:

(C代表数据编号，从1开始直至T，:后面没有空格，直接换行)

每组输出第二行起共N行，输出最终的比赛结果，按照排名从高至低输出，每行包含三个整数id, solve, time分别表示队伍编号，解题数和总用时。



Sample Input

2

2 6

299 2 1001 Yes

299 1 1003 No

6 1 1003 Yes

10 2 1010 No

20 2 1010 Yes

298 1 1006 No

4 9

299 2 1001 Yes

299 1 1003 No

6 1 1003 Yes

10 2 1010 No

20 2 1010 Yes

100 1 1006 No

274 1 1006 Yes

1 4 1002 No

299 3 1002 No



Sample Output

Case #1:

2 2 339

1 1 6

Case #2:

1 2 300

2 2 339

3 0 0

4 0 0




Source Code:

```cpp
#include<iostream>
#include<cstdio>
#include<cstdlib>
#include<algorithm>
using namespace std;

#define N 210
#define P 1200
#define M 3100
//以后再不开动态数组了…… Define+Max的方式定义数组比较无脑也不容易错

int n,m,id[M],rid[N];			//队伍数，提交总数，提交条目ID号（标记）
int tim[M],tid[M],prb[M],res[M];//评测时间 ,队伍编号 ,题号 ,结果Y/N
int slv[N],cst[N],f[N][P];		//解题数，最终罚时，此队N在此题P上WA的罚时，（-1为AC）

//loop variable
int _T,i,_i;

int cmp (const int &i,const int &j){ return tim[i]<tim[j]; }//按评测时间先把条目按照评测顺序排序

int rcmp(const int &i,const int &j){						//此处为解题数slv[i/j]优先，解题数一致的罚时cst[i/j]优先，罚时一致时队号优先
	return slv[i]!=slv[j]?slv[i]>slv[j]:					//解题数优先
					( cst[i]!=cst[j]?cst[i]<cst[j] 			//罚时优先
												: i<j ); 	//队伍号优先
}//按值主、次、辅排序 【Mark】

int main(){
    
    freopen("board.in","r",stdin);freopen("board.out","w",stdout);
    
    int T;scanf("%d",&T);//Case数
    for(_T=1;_T<=T;_T++){
        printf("Case #%d:\n",_T);
        
        scanf("%d%d",&n,&m);
        memset(slv,0,sizeof(slv));			//初始化 解题数_数组
        memset(cst,0,sizeof(cst));			//初始化 最终罚时_数组
        memset(f  ,0,sizeof(f  ));			//初始化 罚时_数组
        
        for(i=0;i<n;i++)rid[i]=i+1;
        for(i=0;i<m;i++){
            char c_str[10];
			id[i]=i;
            scanf("%d%d%d%s",&tim[i],&tid[i],&prb[i],c_str);	//当前时间，题目ID，题号，结果Y/N
            res[i]=c_str[0]=='Y'?1:0; 		// 1/0存储Y/N
        }
        
        sort(id,id+m,cmp);								//按评测时间先把条目按照评测顺序排序
        for(_i=0;_i<m;_i++){
			int i=id[_i];								//按条目顺序
            if(f[tid[i]][prb[i]]==-1)continue; 			//若此题此前已AC，此评测可无视
            if(res[i]){									//此题未AC，此次评测AC时YES
                slv[tid[i]]++;							//当前队伍解题数++
                cst[tid[i]]+=f[tid[i]][prb[i]]+tim[i];	//最终罚时+当前时间+此题上因WA而产生的罚时
                f[tid[i]][prb[i]]=-1;					//标记为已AC
            }else f[tid[i]][prb[i]]+=20;				//此题未AC，此次评测AC时NO，增加此队伍此题罚时20
        }
        
        sort(rid,rid+n,rcmp);	//rid- 将队伍号按照RANK顺序排序
        for(_i=0;_i<n;_i++){	
			int i=rid[_i];		//获取第i+1名次的队号
            printf("%d %d %d\n",i,slv[i],cst[i]); //输出队号，解题数，最终罚时
        }
    }
    
    fclose(stdin);fclose(stdout);
    
    return 0;
}
```](https://so.csdn.net/so/search/s.do?q=cqu&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)




