# POJ 3620 Avoid The Lakes - weixin_33985507的博客 - CSDN博客
2011年08月20日 00:53:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
**POJ_3620**
只要顺序遍历每个格子，找到一个有水的格子后就以这个格子为起点进行深搜或者广搜即可，直到把连成片的有水的格子搜完即可，然后将搜到的有水的格子数与max进行比较，如果比max大则更新max的值。
```
#include<stdio.h>
#include<string.h>
int a[110][110],sx[1010],sy[1010];
int dx[]={-1,1,0,0},dy[]={0,0,-1,1};
int main()
{
int i,j,k,N,M,K,top,x,y,newx,newy,max,count;
while(scanf("%d%d%d",&N,&M,&K)==3)
    {
        memset(a,0,sizeof(a));
for(i=0;i<K;i++)
        {
            scanf("%d%d",&x,&y);
            a[x][y]=1;
        }
        max=0;
for(i=1;i<=N;i++)
for(j=1;j<=M;j++)
if(a[i][j])
                {
                    top=0;
                    count=0;
                    a[i][j]=0;
                    sx[top]=i;
                    sy[top]=j;
                    top++;
                    count++;
while(top>0)
                    {
                        top--;
                        x=sx[top];
                        y=sy[top];
for(k=0;k<4;k++)
                        {
                            newx=x+dx[k];
                            newy=y+dy[k];
if(a[newx][newy])
                            {
                                a[newx][newy]=0;
                                sx[top]=newx;
                                sy[top]=newy;
                                top++;
                                count++;
                            }
                        }
                    }
if(count>max)
                        max=count;
                }
        printf("%d\n",max);
    }
return 0;    
}
```
