# 文本搜索（C实现） - Soul Joy Hub - CSDN博客

2016年06月20日 11:55:29[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：1398


```
#include <stdio.h>
#include <string.h>
using namespace std;

struct cha
{
    char aa[50];//存储单词（不重复）
    int  num;//该单词出现的次数
    int  Mark[100];//标记该单词出现过的行
};

cha CHA[1000]; 

struct Mystr
{
    char bb[50];//存储 ：字符串
    int rol;//存储：字符串 所在行
};

void Standard_String(char cans[]) //标准化字符串
{
    int i=0;
    while(cans[i]!='\0')
    {
        if(cans[i]>='A'&&cans[i]<='Z')//字母全小写
            cans[i]+='a'-'A';
        else if((! ((cans[i]>='A'&&cans[i]<='Z')||(cans[i]>='a'&&cans[i]<='z')))&&cans[i]!=' ')
            cans[i]=' ';              //符号全变为空格
        ++i;
    }

    char standard[501];   //存放标准字符串
    i=0;int j=0;
    while(cans[i]==' ')//去开头空格
        ++i;
    while(cans[i]!='\0')
    {
        while(cans[i] != ' ' && cans[i]!='\0')
        {
            char ctem=cans[i];
            standard[j++]=ctem;
            ++i;
        }
        standard[j++]=' ';
        while(cans[i] == ' '&&cans[i]!='\0')
            ++i;    
    }
    standard[j]='\0';
    strcpy(cans,standard);
}

int main()
{
    char cc[500][501];  //存储 原版文本
    char Standard_File[500][501];  //存储标准化后的文本，用于查找字符串
    Mystr ans[1000];//存储字符串
    char tem[50];
    int i=0,m;

   /***************************读取文件*******************************/
    FILE* fp;
    if((fp=fopen("E:\\cc.txt","r"))==NULL) 
    {
        printf("cannot open file\n");
        return 0;
    }
    int fcount=0;//记录文本的行数
    while(!feof(fp))
    {
        i=0;
        while(!feof(fp))//100个字符为一行存入cc中
        {
            fgets(cc[fcount],500,fp);
            char ctem[501];
            strcpy(ctem,cc[fcount]);
            Standard_String(ctem);//字符串标准化
            strcpy(Standard_File[fcount++],ctem);
        }
    }
    fclose(fp);
  /***************************************************************************/


 /**************************扣取单词，统计单词个数*******************************/

    int count=0;//单词数
    for(m=0;m<fcount;m++)
    {
        i=0;
        while(cc[m][i]==' ')//去开头空格
            ++i;
        while(cc[m][i]!='\0')
        {
            ++count;
            int j=0;
            //printf("%d : ",count);
            while( ( (cc[m][i]>='a'&&cc[m][i]<='z')||(cc[m][i]>='A'&&cc[m][i]<='Z')||cc[m][i]=='\'')&&cc[m][i]!='\0')
            {
                char ctem=cc[m][i];
                if(cc[m][i]>='A'&&cc[m][i]<='Z') ctem+='a'-'A';
                printf("%c",ctem);
                ans[count].bb[j++]=ctem;
                ++i;
            }
            ans[count].bb[j]='\0';
            ans[count].rol=m;//记录每个单词所在的行号
            printf("\n");
            while( (!( (cc[m][i]>='a'&&cc[m][i]<='z')||(cc[m][i]>='A'&&cc[m][i]<='Z')||cc[m][i]=='\'' ))&&cc[m][i]!='\0' )
                 ++i;    
        }
    }
    printf("一共 %d 个单词\n",count);

/***************************************************************************/



    /****************************按照字符顺 冒泡排序*********************/
    int is=0;//是否排序完成
    int Tct=count;
    while(is==0)
    {
        is=1;
        for(i=1;i<Tct;i++)
        {
            if(strcmp(ans[i].bb,ans[i+1].bb)>0)
            {
                is=0;
                Mystr Mtem=ans[i];
                ans[i]=ans[i+1];
                ans[i+1]=Mtem;
            }
        }
        --Tct;
    }
    /*********************************************************************************/



/***************************记录单词出现的次数和所在的行号**************************/

    printf("\n");
    printf("\n");
    int count2=-1;//不同的单词的个数
    for(i=1;i<=count;i++)
    {
        if(strcmp(ans[i].bb,ans[i+1].bb)==0)
        {
            strcpy(CHA[++count2].aa,ans[i].bb);

            if(CHA[count2].Mark[ans[i].rol]!=1)//标记该单词出现过的行号
                CHA[count2].Mark[ans[i].rol]=1;
            if(CHA[count2].Mark[ans[i+1].rol]!=1)//标记该单词出现过的行号
                CHA[count2].Mark[ans[i+1].rol]=1;

            strcpy(tem,ans[i].bb);
            CHA[count2].num+=2;
            i+=2;
            while(strcmp(tem,ans[i].bb)==0)
            {
                ++CHA[count2].num;
                if(CHA[count2].Mark[ans[i].rol]!=1)//标记该单词出现过的行号
                    CHA[count2].Mark[ans[i].rol]=1;
                ++i;
            }
            --i;
        }
        else if (strcmp(ans[i].bb,ans[i+1].bb)!=0)
        {
            strcpy(CHA[++count2].aa,ans[i].bb);

            if(CHA[count2].Mark[ans[i].rol]!=1)//标记该单词出现过的行号
                CHA[count2].Mark[ans[i].rol]=1;
            CHA[count2].num++;
        }
    } 

/*********************************************************************************/

/******************************按照次数排序*************************************/

    printf("\n");
    printf("\n");
    is=0;//是否排序完成
    Tct=count2;
    while(is==0)
    {
        is=1;
        for(i=0;i<Tct;i++)
        {
            if(CHA[i].num<CHA[i+1].num)
            {
                is=0;
                cha ctem=CHA[i];
                CHA[i]=CHA[i+1];
                CHA[i+1]=ctem;
            }
        }
        --Tct;
    }
    if(count2!=0)
    {
        printf("先按次数再按字符序的排序如下：\n\n");
        for(i=0;i<=count2;i++)
            printf("第%d个： 出现次数为：%d次    %s\n",i+1,CHA[i].num,CHA[i].aa);
    }

    /*********************************************************************************/


/******************************输出 次数 频率*************************************/

    printf("\n");
    printf("\n");
    for(i=0;i<=count2;i++)
        printf("字符：%s  次数为:%d 频率：%0.3lf\n",CHA[i].aa,CHA[i].num,(double)CHA[i].num/count);
    printf("\n");
    printf("\n");
/*********************************************************************************/


/***********************************查找***************************************/

    printf("查找单词请输入‘1’，查找字符串请输入‘2’：");
    int mark1;
    while(scanf("%d",&mark1)!=EOF)
    {
        int find=0;
     /***********************查找单词************************/
        if(mark1==1) 
        {
            char word[50];
            printf("请输入要查找的单词：\n");
            scanf("%s",word);
            printf("\n");

            for(i=0;i<=count2;i++)
            {
                if(strcmp(word,CHA[i].aa)==0)
                {
                    find=1;
                    for(m=0;m<fcount;m++)
                    {
                        if(CHA[i].Mark[m]==1)
                            printf("找到第%d行：%s\n",m+1,cc[m]);
                    }

                    break;
                }
            }

            if(find==0) printf("该单词不存在！！\n");
        }
      /***************************查找字符串************************/
        else
        {
            char word2[50];
            printf("请输入要查找的字符串：\n");
            getchar();
            gets(word2);
            Standard_String(word2);//字符串标准化
            int len2 = strlen(word2);
            printf("\n");
            for(m=0;m<fcount;m++)
            {
                int len1=strlen(Standard_File[m]);

                if(len1<len2) continue;

                for(i=0;i+len2<=len1;i++)
                {
                    while((i+len2<=len1)&&word2[0]!=Standard_File[m][i])
                        ++i;

                    if(i+len2>len1) continue;

                    char ctem[50];
                    strncpy(ctem,Standard_File[m]+i,len2);
                    ctem[len2]='\0';
                    if(strcmp(ctem,word2)==0)
                    {
                        printf("找到第%d行：%s\n",m+1,cc[m]);
                        break;
                    }
                }
            }
        }
        printf("\n查找单词请输入‘1’，查找字符串请输入‘2’：");
    }

 /*********************************************************************************/

    return 1;
}
```

