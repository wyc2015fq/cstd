# shell - sxf_123456的博客 - CSDN博客
2017年04月24日 20:16:44[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：129
个人分类：[shell](https://blog.csdn.net/sxf_123456/article/category/6881619)
                
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
typedef struct _link //定义结构体统计单词、和该单词个数
{ 
    char *data; 
    int num; 
    struct _link *next; 
}link; 
void sort(link *phead)
{
    link *p;//i 
    link *q;//j
    link *r;
    link temp; //temp
    for(p=phead;p;p=p->next)
        for(q=p->next;q;q=q->next)
        {
            if(strcmp(p->data,q->data)>0)
            {
                temp=*p;
                *p=*q;
                *q=temp;
                r=p->next;
                p->next=q->next;
                q->next=r;
            }
        }
}
void wordcount(char *input,char *output)
{
    FILE *fp; 
    char word[1025]; 
    size_t pos=0; 
    int c; 
    link *head, *pnow, *ptmp; 
    ptmp=pnow=head=NULL; 
    input[strlen(input)]='\0';
    output[strlen(output)]='\0';
    if((fp=fopen(input, "r"))==NULL) //接收文件路径
    { 
        printf("Can't open %s! Please check it!!", input); 
        //return -1; 
        exit(1);
    } 
    while(!feof(fp)) 
    { 
        c=fgetc(fp); 
        if((c>='a' && c<='z')|| (c>='A' && c<='Z')) 
        { 
            word[pos++]=c; //统计个数、识别单词
        } 
        else if(pos>0) 
        { 
            printf("pos=%d\n",pos);
            printf("word=%s\n",word);
            word[pos]=0; //将空格或者其他符号设置为0
            ptmp=head; 
            while(ptmp) //统计单词
            { 
                if(strcasecmp(word, ptmp->data)==0) 
                { 
                    ptmp->num++; 
                    break; 
                } 
                ptmp=ptmp->next; 
            } 
            if(ptmp==NULL) 
            { 
                ptmp=(link *)malloc(sizeof(link)); //申请一个结构体空间
       ptmp->data=(char *)malloc(pos); //申请单词存放空间
                strcpy(ptmp->data, word); 
                ptmp->num=1; 
                ptmp->next=NULL; 
                if(pnow) 
                { 
                    pnow->next=ptmp; 
                    pnow=ptmp; 
                } 
                else 
                { 
                    head=pnow=ptmp; 
                } 
            } 
            pos=0; 
        } 
    } 
    fclose(fp); 
    //link *phead;
    //phead=head;
    sort(head);
    if((fp=fopen(output, "w"))==NULL) 
    { 
        printf("Can't open %s to write. Please check your permission.\n", output); 
        //return -2; 
        exit(1);
    } 
    ptmp=head; 
    while(ptmp) 
    { 
        fprintf(fp, "%s\t:%d\n", ptmp->data, ptmp->num); 
        ptmp=ptmp->next; 
    } 
    //link phead;
    //phead=head;
    //sort(phead);
    fclose(fp); 
}
int main(int argc, char* argv[]) 
{ 
    if(argc !=3) //编译参数
    { 
        printf("words inputfile outputfile\n"); 
        //return 1; 
        exit(1);
    } 
    wordcount(argv[1],argv[2]);
    return 0; 
}
