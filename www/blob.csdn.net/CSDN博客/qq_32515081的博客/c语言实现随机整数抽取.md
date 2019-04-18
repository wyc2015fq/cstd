# c语言实现随机整数抽取 - qq_32515081的博客 - CSDN博客
2019年01月08日 23:27:41[TengAdventure](https://me.csdn.net/qq_32515081)阅读数：63
        用c语言实现以下功能：例如给定1000，从其中随机抽取10个数（不重复），本程序可实时输入总个数和抽样个数，最终抽取的数字保存在‘D:\sampling_number.txt’中。
```
#include <stdio.h>
#include <stdlib.h>
#include<time.h>
#include<math.h>
void swap2(int *,int *);
void bubble(int storenumber[ ],int n);
int storenumber[100000]={0};  /*the max number of sampling is 100000*/
int main()
{
	FILE *f1;
	double p;
	int h,k,i;
	int N,Nsample,number;
	char anything;
    srand((unsigned)time(NULL));
    printf("Enter the total number:\n");
    scanf("%d",&N);
    printf("Enter the sampling size:\n");
    scanf("%d",&Nsample);
    for(h=1;h<=Nsample;h++){
        p=rand()/(double)(RAND_MAX);
        number=(int)(p*N)+1;
        for(k=1;k<=h;k++){         /*remove the duplicate number*/
            if(number==storenumber[k]){
                p=rand()/(double)(RAND_MAX);
                number=(int)(p*N)+1;
                k=0;
            }
        }
        storenumber[h]=number;
    }
    printf("\n MAKE A FORTUNE, LEO!!!\n\n");
    bubble(storenumber,Nsample);   /*for the numeric sorting*/
    if((f1=fopen("d:\\sampling_number.txt","w"))==NULL){
        printf("file open error!\n");
        exit(0);
    }
    for(i=1;i<=Nsample;i++){
        fprintf(f1,"%d\n",storenumber[i]);
        printf("The %d sample is %d\n",i,storenumber[i]);
    }
    if(fclose(f1)){
        printf("can not close the file!\n");
        exit(0);
    }
    printf("\n ATTENTION:\n The file is saved in 'D:\\sampling_number.txt' \n");
    printf("\n Type any character on keyboard,  then 'Enter'");
    scanf("%s",&anything);
    return 0;
}
void bubble(int storenumber[ ],int n)         /*for the numeric sorting*/
{
    int i,j;
    for(i=1;i<=n;i++){
        for(j=1;j<=n-i;j++){
            if(storenumber[j]>storenumber[j+1]){
                swap2(&storenumber[j],&storenumber[j+1]);
            }
        }
    }
}
void swap2(int *px,int *py)
{
    int t;
    t=*px;
    *px=*py;
    *py=t;
}
```
