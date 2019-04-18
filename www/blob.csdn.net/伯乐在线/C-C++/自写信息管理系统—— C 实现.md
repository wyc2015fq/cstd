# 自写信息管理系统—— C 实现 - 文章 - 伯乐在线
原文出处： [chudongfang2015，2016-06-29](http://blog.csdn.net/chudongfang2015/article/details/51785937)
**信息管理系统**
简介：
该管理系统分为三个权限：学生，老师，管理员
用双向链表实现，文件储存，有账号密码功能。
密码实现MD5加密，较难破解
实现了链表添加，查找，排序，删除等基本操作
管理员能够管理学生和老师的密码和账号，进行一些简单的添加，删除，更改操作。老师具有信息录入 ,查看，修改，查询，删除，排序权限。而学生有信息查看，查询和排序权限。管理员具有信息录入 ,查看，修改，查询，删除，排序和老师学生信息管理权限。对于密码，这里采用及其难以破解的MD5密码加密，根据MD5源码进行改编，最终得到MD5的函数，对录入的密码进行加密操作，然后储存到文件中，这样就保证了即使知道文件并打开文件，也无法破解其密码！
共分为6个文件
5个.cpp文件  1个.h文件
下面上代码 ：
mian.cpp:


```
#include<stdio.h>  
#include<math.h>  
#include<string.h>  
#include<conio.h>  
#include<stdlib.h>  
#include<ctype.h>  
#include<io.h>  
   
#define LEN  100  
#define PAGE 3  //分页输出时，每页的学生个数   
  
  
void admin();  
int menu_admin();  
void change_change_order_tea();  
int  menu_change_order_tea();  
void change_delete_order_tea();  
  
void teacher();  
int menu_tea();  
bool judge_tea();  
int read_file_order_tea();  
  
  
  
  
void change_change_order_stu();  
int  menu_change_order_stu();  
void change_delete_order_stu();  
  
  
void student();  
int menu_stu();  
bool judge_stu();  
int read_file_order_stu();  
  
bool judge_admin();  
void change_order_admin();  
  
  
  
  
  
int  menu();  
int  pmenu_search();  
int  read_file();  
  
void pdisplay();  
void pdisplay_sort();  
  
int  psearch_mname();  
int  psearch_schoolnum();  
int  psearch_phone();  
int  psearch_sex();  
int  psearch_age();  
  
int  delete_pmenu_search();  
int  delete_psearch_schoolnum();  
int  delete_psearch_mname();  
int  delete_psearch_phone();  
int  delete_psearch_age();  
int  delete_psearch_sex();  
  
int  change_pmenu_search();  
int  change_psearch_schoolnum();  
int  change_psearch_mname();  
int  change_psearch_phone();  
int  change_psearch_age();  
int  change_psearch_sex();  
  
int  psort_menu();  
void psort_age();  
void psort_schoolnum();  
  
void save_order_tea();  
void save();  
void save_order_stu();  
  
typedef struct {  
        char mname[9];  
        char schoolnum[15];  
        char sex[3];  
        int  age;  
        char address[20];  
        char phone[15];       
    }Node;  
      
typedef struct student  
{  
    Node information;   
    struct student *next;  
    struct student *last;  
}STUDENT;  
  
  
typedef struct  
{  
    char username[100];  
    char password[100];  
}Node_order_tea;  
  
  
typedef struct order_tea  
{   
    Node_order_tea information;  
    struct order_tea *next;  
    struct order_tea *last;  
}ORDER_TEA;  
  
typedef struct  
{  
    char username[100];  
    char password[100];  
}Node_order_stu;  
  
  
typedef struct order_stu  
{   
    Node_order_stu information;  
    struct order_stu *next;  
    struct order_stu *last;  
}ORDER_STU;  
  
  
  
int num=0;                   //文件中总的学生信息个数   
STUDENT *head=NULL,*iend,*inew;  
FILE *fp;                    //文件指针   
  
int num_order_tea=0;                   //文件中总的学生信息个数   
ORDER_TEA *head_order_tea=NULL,*iend_order_tea,*inew_order_tea;  
FILE *fp_order_tea;   
  
int num_order_stu=0;                   //文件中总的学生信息个数   
ORDER_STU *head_order_stu=NULL,*iend_order_stu,*inew_order_stu;  
FILE *fp_order_stu;   
  
unsigned char PADDING[]={0x80,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};    
  
  
  
/**********************新加入*****************************/  
int main()  
{  
    int chioce;  
    do  
    {  
        printf("\n\t\t********************************\n");  
        printf("\t\t*        1.老师                 *\n");  
        printf("\t\t*        2.学生                 *\n");  
        printf("\t\t*        3.管理员               *\n");           
        printf("\t\t*        0.退出                 *\n");  
        printf("\t\t*********************************\n");  
        printf("\t\t请选择登陆身份：");  
        scanf("%d",&chioce);  
        switch(chioce)  
        {    
            case 1:  
                teacher();  
                break;  
            case 2:  
                student();  
                break;  
            case 3:  
                admin();  
            default:  
                break;  
        }  
    }while(chioce!=0);  
    return 0;  
}
```
feature.cpp


```
#include"head.h"  
  
/********************获取文件名；读取内容；给num，fp，head,flieopenname赋值；创建链表***********************/  
//打开文件，并读取文件   
int read_file()  
{  
    int i;  
    int fno,fsize;  
    if((fp=fopen("student_system.txt","rt"))==NULL)//判断文件是否存在，若存在打开   
    {                                  //否则询问是否继续打开   
        printf("\n库文件不存在！\n");  
        return 0;  
    }  
  
    rewind(fp);                            //使文件内部指针移动到文件的开始位置   
    fno=fileno(fp);              //获取文件描述字                         
    fsize=filelength(fno);      //计算文件大小，以字节为单位   
    num=fsize/sizeof(Node);     //计算文件包含的学生信息数目   
    iend=inew=(STUDENT *)malloc(sizeof(STUDENT));//申请动态空间   
    fread(inew,sizeof(Node),1,fp);   //以Node（结构体）的空间大小读数据   
    for(i=1;i<=num;i++)   //利用for循环创建链表，并把数据储存到其中   
    {  
        if(i==1)          //第一个节点的创建   
        {  
            inew->next=head;  
            inew->last=NULL; //双向链表   
            iend=inew;  
            head=inew;  
        }  
        else  
        {  
            inew->next=NULL;  
            inew->last=iend;  
            iend->next=inew;  
            iend      =inew;  
        }  
        inew=(STUDENT *)malloc(sizeof(STUDENT));  
        fread(inew,sizeof(Node),1,fp);  
    }  
    free(inew);//释放最后一个没有储存数据的节点   
    fclose(fp);//关闭文件   
    return num;  
}  
  
  
  
  
  
  
  
  
  
  
  
/****************************输入函数*******************/   
  
STUDENT *pinput()  
{  
    char ch;  
    if(num==0)//判断是否是第一次录入   
    {  
        iend=inew=(STUDENT *)malloc(sizeof(STUDENT));//申请空间   
        inputone(inew);                              //录入信息   
        while(1)                                        
        {  
            num++;             
            if(num==1)       //录入第一个学生信息时，第一个节点的指针指向             
            {  
                inew->next=NULL;  
                inew->last=NULL;   
                iend      =inew;  
                head      =inew;  
            }  
            else             //在第一个节点的存在下，进行其他节点的录入                 
            {  
                inew->next=NULL;     //新建立节点next指向NULL           
                inew->last=iend;     //新建立节点last指向上一个节点   
                iend->next=inew;     //第一个next->下一个节点  
                iend      =inew;     //让iend指向最后一个节点   
            }  
            inew=(STUDENT *)malloc(sizeof(STUDENT));  
            printf("信息输入完毕，是否继续？(是-Y  否-N)：");   
            scanf("\n%c",&ch);  
            if(ch=='N'||ch=='n')     break;//判断是否跳出循环   
            inputone(inew);   
        }  
        free(inew);                  //释放未存储数据的节点   
        return head;  
    }  
    else//非第一次录入时   
    {  
        while(1)//进行循环输入，并进行判断，是否跳出循环   
        {  
              
            inew=(STUDENT *)malloc(sizeof(STUDENT));  
            getch();  
            inputone(inew);  
            num++;  
            inew->next=NULL;   //新建立节点next指向NULL      
            inew->last=iend;   //新建立节点last指向上一个节点  
            iend->next=inew;   //第一个next->下一个节点     
            iend      =inew;   //让iend指向最后一个节点  
            printf("信息输入完毕，是否继续？(是-Y  否-N)：");  
            scanf("\n%c",&ch);  
            if(ch=='N'||ch=='n')  
             break;  
        }  
        return head;  
    }  
}  
  
void inputone(STUDENT *temp)  
{  
    printf("************************\n");  
    printf("请输入学生的姓名：");  
    scanf("%s",inew->information.mname);  
    printf("请输入学生的学号：");  
    scanf("%s",inew->information.schoolnum);  
    printf("请输入学生的性别（男-M  女-F）：");  
    scanf("%s",inew->information.sex);  
    printf("请输入学生的年龄：");  
    scanf("%d",&inew->information.age);  
    printf("请输入学生的家庭住址：");  
    scanf("%s",inew->information.address);  
    printf("请输入学生的电话号码：");  
    scanf("%s",inew->information.phone);  
}  
  
  
  
  
  
  
/****************************输出函数*******************/   
void displayone(STUDENT *temp) //为简化程序-  单个输出函数，用于调用！   
{  
    printf("**********************************\n");   
    printf("姓名：%s\n",temp->information.mname);  
    printf("学号：%s\n",temp->information.schoolnum);   
    printf("性别：%s\n",temp->information.sex);  
    printf("年龄：%d\n",temp->information.age);  
    printf("家庭住址：%s\n",temp->information.address);  
    printf("电话号码：%s\n",temp->information.phone);  
    printf("===========================\n");  
}  
  
/**************************以head为起始输出链表******************************/  
void pdisplay()//核心思想：对链表进行操作，然后利用while循环输出   
{  
    STUDENT *temp,*temp1;  
    int page=1,lastnum=num%PAGE,stu=0,i,page1;  
    char ch;  
    int choice;  
    if(head==NULL)//判断文件中是否有数据   
    {  
        printf("文件中没有数据！！\n");  
        getch();  
        return;  
    }  
    temp=head;  
    page1=num/PAGE+1;//进行页数的初始化   
    if(lastnum==0)//判断最后一页的数据数   
    {  
        lastnum=PAGE;  
        page1=page1-1;  
    }     
    while(1)//保留最后节点的指针   
    {  
        if(temp->next==NULL)  
        {  
            temp1=temp;  
            break;  
        }  
        temp=temp->next;  
    }   
    temp=head;  
    rewind(fp);       
loop:   printf("----------------------------第%d页-----------------------------\n",page);  
    while(temp!=NULL)//利用while 结构进行输出 知道到最后节点   
    {  
        displayone(temp);   
        temp=temp->next;  
        stu++;  
        if(stu==PAGE)//一页输出完后进行选择，对其链表进行操作   
        {  
            printf("4---首页  <--上一页--1                   3--下一页-->  尾页---6  \n");  
            scanf("%d",&choice);  
            switch(choice)  
            {  
                case 1://上一页，清屏 ，对其链表进行操作   
                    for(i=1;i<=PAGE+stu;i++)  
                    {  
                        temp=temp->last;  
                        if(temp==head)  
                        {  
                            temp=head;  
                            page=1;  
                            break;  
                        }  
                    }  
                    if(temp!=head)  
                    page--;  
                    stu=0;  
                    system("cls");  
                    goto loop;  
                      
                    break;  
                case 4://第一页，清屏 ，把head值赋给temp   
                    system("cls");  
                    temp=head;  
                    stu=0;  
                    page=1;  
                    goto loop;   
                    break;  
                case 6://最后一页，清屏 ，对链表进行操作，找到相应链表值赋给temp   
                    system("cls");  
                    temp=temp1;  
                    for(i=1;i<=lastnum-1;i++)  
                    {  
                        temp=temp->last;  
                        if(temp==head)  
                        {  
                            temp=head;  
                            page=1;  
                            break;  
                        }  
                    }  
                    if(temp!=head)  
                    page=page1;   
                    goto loop;    
                    break;  
                case 3://下一页，清屏 继续输出，  
                    system("cls");  
                    stu=0;  
                    page++;  
                    goto loop;  
                    break;                
            }  
            stu=0;  
        }     
    }             
    if(temp==NULL)  
    {  
        printf("\n-------------已到尾页，是否返回首页？(是-Y 否-N)：---------------");  
        scanf("\n%c",&ch);  
        if(toupper(ch)=='Y')  
        {  
            temp= head;  
            page=1;   
            stu=0;  
            system("cls");  
            goto loop;  
        }  
    }      
    getch();  
}  
  
/***************************加上提示语句*以head为起始输出链表**********************************/   
void pdisplay_sort()  
{                     //提示语句，表示已经进行排序，然后调用输出函数   
    printf("\n信息已经进行了排序，排序后信息如下：\n");  
    pdisplay();  
}  
  
  
  
  
  
  
/****************************搜索函数***head****************/  
//基本思路：从第一个数据节点开始进行遍历，知道找到相应信息为止  
//遍历： while循环        
//信息比较：if()条件判断语句        及用break;跳出循环   
//进行显示   
//若未找到,输出提示！   
int pmenu_search()  
{  
    int chioce;  
    do//利用switch选择结构和do-while循环，创建子菜单！   
    {  
        printf("\n");  
        printf("\t\t********************************\n");  
        printf("\t\t*        1.按姓名查询          *\n");  
        printf("\t\t*        2.按学号查询          *\n");  
        printf("\t\t*        3.按电话号码查询      *\n");            
        printf("\t\t*        4.按性别查询          *\n");  
        printf("\t\t*        5.按年龄查询          *\n");  
        printf("\t\t*        0.退出                *\n");  
        printf("\t\t********************************\n");  
        printf("\t\t请选择：");  
        scanf("%d",&chioce);  
        switch(chioce)  
        {    
        case 1:  
         psearch_mname();        //调用按姓名查询函数    
         getch();  
        break;  
        case 2:  
         psearch_schoolnum();     //调用按学号查询函数    
         getch();  
        break;  
        case 3:  
         psearch_phone();         //调用按电话号码查询函数    
         getch();  
        break;  
        case 4:  
         psearch_sex();          //调用按性别查询函数    
         getch();  
        break;  
        case 5:  
         psearch_age();          //调用按年龄查询函数    
         getch();  
        break;  
        default:  
        break;  
    }  
    }while(chioce!=0);  
    return 0;  
}  
  
int psearch_schoolnum()  
{  
    char schoolnum1[15];  
    int x=0;  
    STUDENT *temp;  
    temp=head;  
    printf("请输入学生学号：");  
    scanf("%s",schoolnum1);  
    while(temp!=NULL)       //利用while 函数对链表进行遍历，并查询   
    {  
        if(!strcmp(schoolnum1,temp->information.schoolnum))//利用字符串处理函数进行比较   
        {         
            printf("查询结果如下：\n");  
            printf("=============================\n");  
            printf("\t姓名：%s\n",temp->information.mname);  
            printf("\t学号：%s\n",temp->information.schoolnum);  
            printf("\t性别：%s\n",temp->information.sex);  
            printf("\t年龄：%d\n",temp->information.age);  
            printf("\t家庭住址：%s\n",temp->information.address);    
            printf("\t电话号码：%s\n",temp->information.phone);  
            printf("=============================\n");  
            break;           //输出并跳出循环   
        }  
        x++;  
        temp=temp->next;  
    }  
    if(x==num)//判断是否找到 ，如果未找到，输出提示！   
    {  
        printf("未能找到符合条件的结果！！");  
        return -1;  
    }  
    else  
        return x;  
}  
  
int psearch_mname()  
{  
    char mname1[9];  
    int x=0;  
    STUDENT *temp;  
    temp=head;  
    printf("请输入学生姓名：");  
    scanf("%s",mname1);  
    while(temp!=NULL)  
    {  
        if(!strcmp(mname1,temp->information.mname))  
        {         
            printf("查询结果如下：\n");  
            printf("=============================\n");  
            printf("\t姓名：%s\n",temp->information.mname);  
            printf("\t学号：%s\n",temp->information.schoolnum);  
            printf("\t性别：%s\n",temp->information.sex);  
            printf("\t年龄：%d\n",temp->information.age);  
            printf("\t家庭住址：%s\n",temp->information.address);    
            printf("\t电话号码：%s\n",temp->information.phone);  
            printf("=============================\n");  
            break;  
        }  
        x++;  
        temp=temp->next;  
    }  
    if(x==num)  
    {  
        printf("未能找到符合条件的结果！！");  
        return -1;  
    }  
    else  
        return x;  
  
}  
  
int psearch_phone()  
{  
    char phone1[15];  
    int x=0;  
    STUDENT *temp;  
    temp=head;  
    printf("请输入学生电话：");  
    scanf("%s",phone1);  
    while(temp!=NULL)  
    {  
        if(!strcmp(phone1,temp->information.phone))  
        {         
            printf("查询结果如下：\n");  
            printf("=============================\n");  
            printf("\t姓名：%s\n",temp->information.mname);  
            printf("\t学号：%s\n",temp->information.schoolnum);  
            printf("\t性别：%s\n",temp->information.sex);  
            printf("\t年龄：%d\n",temp->information.age);  
            printf("\t家庭住址：%s\n",temp->information.address);    
            printf("\t电话号码：%s\n",temp->information.phone);  
            printf("=============================\n");  
            break;  
        }  
        x++;  
        temp=temp->next;  
    }  
    if(x==num)  
    {  
        printf("未能找到符合条件的结果！！");  
        return -1;  
    }  
    else  
        return x;  
}  
  
int psearch_sex()  
{  
    char sex1[15];  
    int x=0;  
    STUDENT *temp;  
    temp=head;  
    printf("请输入学生性别：");  
    scanf("%s",sex1);  
    while(temp!=NULL)  
    {  
        if(!strcmp(sex1,temp->information.sex))  
        {         
            printf("查询结果如下：\n");  
            printf("=============================\n");  
            printf("\t姓名：%s\n",temp->information.mname);  
            printf("\t学号：%s\n",temp->information.schoolnum);  
            printf("\t性别：%s\n",temp->information.sex);  
            printf("\t年龄：%d\n",temp->information.age);  
            printf("\t家庭住址：%s\n",temp->information.address);    
            printf("\t电话号码：%s\n",temp->information.phone);  
            printf("=============================\n");  
        }  
        x++;  
        temp=temp->next;  
    }  
    if(x==num)  
    {  
        printf("未能找到符合条件的结果！！");  
        return -1;  
    }  
    else  
        return x;  
  
}  
  
int psearch_age()  
{  
    STUDENT *temp;  
    temp=head;  
    int  age,x=0;  
    printf("请输入学生年龄：");  
    scanf("%d",&age);  
    while(temp!=NULL)  
    {  
        if(age==temp->information.age)  
        {         
            printf("姓名：%s\n",temp->information.mname);  
            printf("学号：%s\n",temp->information.schoolnum);  
            printf("性别：%s\n",temp->information.sex);  
            printf("年龄：%d\n",temp->information.age);  
            printf("家庭住址：%s\n",temp->information.address);    
            printf("电话号码：%s\n",temp->information.phone);  
            printf("===========================\n");  
            x++;  
        }  
        temp=temp->next;  
    }  
    if(x==num)  
    {  
        printf("未能找到符合条件的结果！！");  
        return -1;  
    }  
    else  
        return x;  
}  
  
  
  
/****************************删除函数  head*******************/   
//基本思路：从第一个数据节点开始进行遍历，知道找到相应信息为止  
//而后把其节点删除，把指针连接到下一节点上   
//若未找到,输出提示！   
int delete_pmenu_search()  
{  
    int chioce;  
    do  
    {  
        printf("\n");  
        printf("\t\t********************************\n");  
        printf("\t\t*        1.按姓名检索          *\n");  
        printf("\t\t*        2.按学号检索          *\n");  
        printf("\t\t*        3.按电话号码检索      *\n");            
        printf("\t\t*        4.按性别检索          *\n");  
        printf("\t\t*        5.按年龄检索          *\n");  
        printf("\t\t*        0.退出                *\n");  
        printf("\t\t********************************\n");  
        printf("\t\t请选择：");  
        scanf("%d",&chioce);  
        switch(chioce)  
        {    
        case 1:  
         delete_psearch_mname();  
         getch();  
        break;  
        case 2:  
         delete_psearch_schoolnum();  
         getch();  
        break;  
        case 3:  
         delete_psearch_phone();  
         getch();  
        break;  
        case 4:  
         delete_psearch_sex();  
         getch();  
        break;  
        case 5:  
         delete_psearch_age();  
         getch();  
        break;  
        default:  
        break;  
    }  
    }while(chioce!=0);  
    return 0;  
}  
                  
int delete_psearch_schoolnum()  
{  
    char schoolnum1[15];  
    char ch;  
    int i;  
    int x=0,live=0;  
    STUDENT *temp,*pPre,*pTemp;  
    temp =head;  
    pTemp=head;  
    pPre =head;  
    printf("请输入学生学号：");  
    scanf("%s",schoolnum1);  
    while(temp!=NULL)//利用while先遍历搜索   
    {  
        x++;  
        if(!strcmp(schoolnum1,temp->information.schoolnum))  
        {         
            printf("查询结果如下：\n");  
            printf("=============================\n");  
            printf("\t姓名：%s\n",temp->information.mname);  
            printf("\t学号：%s\n",temp->information.schoolnum);  
            printf("\t性别：%s\n",temp->information.sex);  
            printf("\t年龄：%d\n",temp->information.age);  
            printf("\t家庭住址：%s\n",temp->information.address);    
            printf("\t电话号码：%s\n",temp->information.phone);  
            printf("=============================\n");  
            live=1;  
            break;  
        }     
        temp=temp->next;  
    }  
    if(live==0)  
    {  
        printf("未能找到符合条件的结果！！");  
        return -1;  
    }  
    else  
    {  
        printf("是否删除数据？（删除-Y，放弃删除-N）");  
        scanf("\n%c",&ch);  
        if(ch=='y'||ch=='Y')  
        {  
            if(x==1)//判断是否是第一个数据   
            {  
                head=pTemp->next;  
            }   
            else       //x为计数器，找出其节点的上一个和下一个节点   
            {         //并连接两个节点 ，也可以利用双向链表进行连接   
                for(i=1;i<x;i++)    //若建立双向链表   
                {                   //temp->last->next=temp->next;   
                    pPre=pTemp;            
                    pTemp=pTemp->next;    
                }                           
                pPre->next=pTemp->next;  
             }  
             free(pTemp);     //释放删除了的节点空间   
             num--;           //num代表总的学生数据数，数据数减一   
             printf("数据已删除！");  
        }  
        return x;  
    }  
    x=0;          
}  
  
int delete_psearch_mname()  
{  
    char mname1[9];  
    char ch;  
    int x=0,i,live=0;  
    STUDENT *temp,*pPre,*pTemp;  
    temp =head;  
    pTemp=head;  
    pPre =pTemp;  
    printf("请输入学生姓名：");  
    scanf("%s",mname1);  
    while(temp!=NULL)  
    {  
        x++;  
        if(!strcmp(mname1,temp->information.mname))  
        {         
            printf("查询结果如下：\n");  
            printf("=============================\n");  
            printf("\t姓名：%s\n",temp->information.mname);  
            printf("\t学号：%s\n",temp->information.schoolnum);  
            printf("\t性别：%s\n",temp->information.sex);  
            printf("\t年龄：%d\n",temp->information.age);  
            printf("\t家庭住址：%s\n",temp->information.address);    
            printf("\t电话号码：%s\n",temp->information.phone);  
            printf("=============================\n");  
            live=1;  
            break;  
        }  
        temp=temp->next;  
    }  
    if(live==0)  
    {  
        printf("未能找到符合条件的结果！！");  
        return -1;  
    }  
    else  
    {  
        printf("是否删除数据？（删除-Y，放弃删除-N）");  
        scanf("\n%c",&ch);  
        if(ch=='y'||ch=='Y')  
        {  
            if(x==1)  
            {  
                head=pTemp->next;  
            }   
             else  
             {  
               
               for(i=1;i<x;i++)  
                {  
                    pPre=pTemp;  
                    pTemp=pTemp->next;  
                }  
                pPre->next=pTemp->next;  
             }  
            free(pTemp);  
            num--;  
            printf("数据已删除！");  
        }  
  
        return x;  
    }  
    x=0;  
}  
  
int delete_psearch_phone()  
{  
    char phone1[15];  
    char ch;  
    int x=0,i,live=0;  
    STUDENT *temp,*pPre,*pTemp;  
    temp =head;  
    pTemp=head;  
    pPre =pTemp;  
    printf("请输入学生电话：");  
    scanf("%s",phone1);  
    while(temp!=NULL)  
    {  
        x++;  
        if(!strcmp(phone1,temp->information.phone))  
        {         
            printf("查询结果如下：\n");  
            printf("=============================\n");  
            printf("\t姓名：%s\n",temp->information.mname);  
            printf("\t学号：%s\n",temp->information.schoolnum);  
            printf("\t性别：%s\n",temp->information.sex);  
            printf("\t年龄：%d\n",temp->information.age);  
            printf("\t家庭住址：%s\n",temp->information.address);    
            printf("\t电话号码：%s\n",temp->information.phone);  
            printf("=============================\n");  
            live=1;  
            break;  
        }  
        temp=temp->next;  
    }  
    if(live==0)  
    {  
        printf("未能找到符合条件的结果！！");  
        return -1;  
    }  
    else  
    {  
        printf("是否删除数据？（删除-Y，放弃删除-N）");  
        scanf("\n%c",&ch);  
        if(ch=='y'||ch=='Y')  
        {  
            if(x==1)  
            {  
                head=pTemp->next;  
            }   
             else  
             {  
               
               for(i=1;i<x;i++)  
                {  
                    pPre=pTemp;  
                    pTemp=pTemp->next;  
                }  
                pPre->next=pTemp->next;  
             }  
            free(pTemp);  
            num--;  
            printf("数据已删除！");  
        }  
  
        return x;  
    }  
    x=0;  
}  
  
int delete_psearch_sex()  
{  
    char sex1[15];  
    int x=0,live=0;  
    STUDENT *temp;  
    temp=head;  
    printf("请输入学生性别：");  
    scanf("%s",sex1);  
    while(temp!=NULL)  
    {  
        if(!strcmp(sex1,temp->information.sex))  
        {         
            printf("查询结果如下：\n");  
            printf("=============================\n");  
            printf("\t姓名：%s\n",temp->information.mname);  
            printf("\t学号：%s\n",temp->information.schoolnum);  
            printf("\t性别：%s\n",temp->information.sex);  
            printf("\t年龄：%d\n",temp->information.age);  
            printf("\t家庭住址：%s\n",temp->information.address);    
            printf("\t电话号码：%s\n",temp->information.phone);  
            printf("=============================\n");  
            live=1;  
        }  
        x++;  
        temp=temp->next;  
    }  
    if(live==0)  
    {  
        printf("未能找到符合条件的结果！！");  
        return -1;  
    }  
    else  
    {  
        printf("请确定您所需要删除的学生信息!");  
        getch();  
        delete_pmenu_search();  
        return x;  
    }  
}  
  
int delete_psearch_age()  
{  
    STUDENT *temp;  
    temp=head;  
    int  age,x=0,live=0;  
    printf("请输入学生年龄：");  
    scanf("%d",&age);  
    while(temp!=NULL)  
    {  
        if(age==temp->information.age)  
        {         
            printf("姓名：%s\n",temp->information.mname);  
            printf("学号：%s\n",temp->information.schoolnum);  
            printf("性别：%s\n",temp->information.sex);  
            printf("年龄：%d\n",temp->information.age);  
            printf("家庭住址：%s\n",temp->information.address);    
            printf("电话号码：%s\n",temp->information.phone);  
            printf("===========================\n");  
            live=1;  
            x++;  
        }  
        temp=temp->next;  
    }  
    if(live==0)  
    {  
        printf("未能找到符合条件的结果！！");  
        return -1;  
    }  
    else  
    {  
        printf("请确定您所需要删除的学生信息!");  
        getch();  
        delete_pmenu_search();  
        return x;  
    }  
}  
  
  
  
  
/***************************修改函数* head******************/   
//基本思路：从第一个数据节点开始进行遍历，知道找到相应信息为止  
//而后把其节点重新录入，覆盖原节点！   
//若未找到,输出提示！   
int change_one(STUDENT *pTemp)//利用单个函数简化函数   
{  
    printf("请重新录入！");  
    printf("************************\n");  
    printf("请输入学生的姓名：");  
    scanf("%s",pTemp->information.mname);  
    printf("请输入学生的学号：");  
    scanf("%s",pTemp->information.schoolnum);  
    printf("请输入学生的性别（男-M  女-F）：");  
    scanf("%s",pTemp->information.sex);  
    printf("请输入学生的年龄：");  
    scanf("%d",&pTemp->information.age);  
    printf("请输入学生的家庭住址：");  
    scanf("%s",pTemp->information.address);  
    printf("请输入学生的电话号码：");  
    scanf("%s",pTemp->information.phone);      
    return 0;  
}   
  
int change_pmenu_search()  
{  
    int chioce;  
    do  
    {  
        printf("\n");  
        printf("\t\t********************************\n");  
        printf("\t\t*        1.按姓名检索          *\n");  
        printf("\t\t*        2.按学号检索          *\n");  
        printf("\t\t*        3.按电话号码检索      *\n");            
        printf("\t\t*        4.按性别检索          *\n");  
        printf("\t\t*        5.按年龄检索          *\n");  
        printf("\t\t*        0.退出                *\n");  
        printf("\t\t********************************\n");  
        printf("\t\t请选择：");  
        scanf("%d",&chioce);  
        switch(chioce)  
        {    
        case 1:  
        change_psearch_mname();  
         getch();  
        break;  
        case 2:  
        change_psearch_schoolnum();  
         getch();  
        break;  
        case 3:  
        change_psearch_phone();  
         getch();  
        break;  
        case 4:  
        change_psearch_sex();  
         getch();  
        break;  
        case 5:  
        change_psearch_age();  
         getch();  
        break;  
        default:  
        break;  
    }  
    }while(chioce!=0);  
    return 0;  
}  
  
int change_psearch_schoolnum()  
{  
    char schoolnum1[15];  
    char ch;  
    int live=0;  
    STUDENT *temp;  
    temp=head;  
    printf("请输入学生学号：");  
    scanf("%s",schoolnum1);  
    while(temp!=NULL)  
    {  
        if(!strcmp(schoolnum1,temp->information.schoolnum))  
        {         
            printf("查询结果如下：\n");  
            printf("=============================\n");  
            printf("\t姓名：%s\n",temp->information.mname);  
            printf("\t学号：%s\n",temp->information.schoolnum);  
            printf("\t性别：%s\n",temp->information.sex);  
            printf("\t年龄：%d\n",temp->information.age);  
            printf("\t家庭住址：%s\n",temp->information.address);    
            printf("\t电话号码：%s\n",temp->information.phone);  
            printf("=============================\n");  
            live=1;  
            break;  
        }     
        temp=temp->next;  
    }  
    if(live==0)  
    {  
        printf("未能找到符合条件的结果！！");  
        return -1;  
    }  
    else  
    {  
        printf("是否修改数据？（修改-Y，放弃修改-N）");  
        scanf("\n%c",&ch);  
        if(ch=='y'||ch=='Y')  
        {  
            change_one(temp);  
            printf("数据修改完成！");  
        }  
        return 0;  
    }         
}  
    
int change_psearch_mname()  
{  
    char mname1[9];  
    char ch;  
    int live=0;  
    STUDENT *temp;  
    temp=head;  
    printf("请输入学生姓名：");  
    scanf("%s",mname1);  
    while(temp!=NULL)  
    {  
        if(!strcmp(mname1,temp->information.mname))  
        {         
            printf("查询结果如下：\n");  
            printf("=============================\n");  
            printf("\t姓名：%s\n",temp->information.mname);  
            printf("\t学号：%s\n",temp->information.schoolnum);  
            printf("\t性别：%s\n",temp->information.sex);  
            printf("\t年龄：%d\n",temp->information.age);  
            printf("\t家庭住址：%s\n",temp->information.address);    
            printf("\t电话号码：%s\n",temp->information.phone);  
            printf("=============================\n");  
            live=1;  
            break;  
        }  
        temp=temp->next;  
    }  
    if(live==0)  
    {  
        printf("未能找到符合条件的结果！！");  
        return -1;  
    }  
    else  
    {  
        printf("是否修改数据？（修改-Y，放弃修改-N）");  
        scanf("\n%c",&ch);  
        if(ch=='y'||ch=='Y')  
        {  
            change_one(temp);  
            printf("数据修改完成！");  
        }  
        return 0;  
    }  
}  
  
int change_psearch_phone()  
{  
    char phone1[15];  
    char ch;  
    int live=0;  
    STUDENT *temp;  
    temp =head;  
    printf("请输入学生电话：");  
    scanf("%s",phone1);  
    while(temp!=NULL)  
    {  
        if(!strcmp(phone1,temp->information.phone))  
        {         
            printf("查询结果如下：\n");  
            printf("=============================\n");  
            printf("\t姓名：%s\n",temp->information.mname);  
            printf("\t学号：%s\n",temp->information.schoolnum);  
            printf("\t性别：%s\n",temp->information.sex);  
            printf("\t年龄：%d\n",temp->information.age);  
            printf("\t家庭住址：%s\n",temp->information.address);    
            printf("\t电话号码：%s\n",temp->information.phone);  
            printf("=============================\n");  
            live=1;  
            break;  
        }  
        temp=temp->next;  
    }  
    if(live==0)  
    {  
        printf("未能找到符合条件的结果！！");  
        return -1;  
    }  
    else  
    {  
        printf("是否修改数据？（修改-Y，放弃修改-N）");  
        scanf("\n%c",&ch);  
        if(ch=='y'||ch=='Y')  
        {  
            change_one(temp);  
            printf("数据修改完成！");  
        }  
        return 0;  
    }  
}  
  
int change_psearch_sex()  
{  
    char sex1[15];  
    int x=0,live=0;  
    STUDENT *temp;  
    temp=head;  
    printf("请输入学生性别：");  
    scanf("%s",sex1);  
    while(temp!=NULL)  
    {  
        if(!strcmp(sex1,temp->information.sex))  
        {         
            printf("查询结果如下：\n");  
            printf("=============================\n");  
            printf("\t姓名：%s\n",temp->information.mname);  
            printf("\t学号：%s\n",temp->information.schoolnum);  
            printf("\t性别：%s\n",temp->information.sex);  
            printf("\t年龄：%d\n",temp->information.age);  
            printf("\t家庭住址：%s\n",temp->information.address);    
            printf("\t电话号码：%s\n",temp->information.phone);  
            printf("=============================\n");  
            live=1;  
        }  
        x++;  
        temp=temp->next;  
    }  
    if(live==0)  
    {  
        printf("未能找到符合条件的结果！！");  
        return -1;  
    }  
    else  
    {  
        printf("请确定您所需要修改的学生信息!");  
        getch();  
        change_pmenu_search();  
        return x;  
    }  
}  
  
int change_psearch_age()  
{  
    STUDENT *temp;  
    temp=head;  
    int  age,x=0,live=0;  
    printf("请输入学生年龄：");  
    scanf("%d",&age);  
    while(temp!=NULL)  
    {  
        if(age==temp->information.age)  
        {         
            printf("姓名：%s\n",temp->information.mname);  
            printf("学号：%s\n",temp->information.schoolnum);  
            printf("性别：%s\n",temp->information.sex);  
            printf("年龄：%d\n",temp->information.age);  
            printf("家庭住址：%s\n",temp->information.address);    
            printf("电话号码：%s\n",temp->information.phone);  
            printf("===========================\n");  
            live=1;  
            x++;  
        }  
        temp=temp->next;  
    }  
    if(live==0)  
    {  
        printf("未能找到符合条件的结果！！");  
        return -1;  
    }  
    else  
    {  
        printf("请确定您所需要修改的学生信息!");  
        getch();  
        change_pmenu_search();  
        return x;  
    }  
}  
  
  
  
/****************************排序函数，  head*******************/   
//基本思路：选择要进行排序的元素（年龄or学号） 选择排序方法 ：冒泡排序！    
//注意使用链表排序，交换问题！   
//进行输出，调用输出函数！   
int psort_menu()  
{  
    int chioce;  
    do  
    {  
        printf("\n");  
        printf("\t\t********************************\n");  
        printf("\t\t*        1.按学号排序          *\n");              
        printf("\t\t*        2.按年龄年龄          *\n");  
        printf("\t\t*        0.退出                *\n");  
        printf("\t\t********************************\n");  
        printf("\t\t请选择：");  
        scanf("%d",&chioce);  
        switch(chioce)  
        {    
        case 1:  
            psort_schoolnum();  
            break;  
        case 2:  
            psort_age();  
            break;  
        default:  
        break;  
    }  
    }while(chioce!=0);  
    return 0;  
}  
  
void psort_schoolnum()//利用冒泡排序-----下节李珍同学讲   
{  
    STUDENT *p1,*p2,*ptemp,*pfinished=NULL;  
    for(p1=head;p1->next!=pfinished;)  
    {  
        for(p2=p1;p2->next!=pfinished;)  
        {  
            if(atof(p2->information.schoolnum)>atof(p2->next->information.schoolnum))  
            {  
                if(p2==p1)  
                {  
                    p1=p2->next;  
                    p2->next=p1->next;  
                    p1->next=p2;  
                    ptemp=p1;  
                }  
                else  
                {  
                    ptemp->next=p2->next;  
                    ptemp=p2->next;  
                    p2->next=ptemp->next;  
                    ptemp->next=p2;        
                }  
            }  
            else  
            {  
                ptemp=p2;  
                p2=p2->next;  
            }  
        }  
        pfinished=p2;  
    }         
    head=p1;  
    pdisplay_sort();  
}  
  
void psort_age()  
{  
    STUDENT *p1,*p2,*ptemp,*pfinished=NULL;  
    for(p1=head;p1->next!=pfinished;)  
    {  
        for(p2=p1;p2->next!=pfinished;)  
        {  
            if(p2->information.age>p2->next->information.age)  
            {  
                if(p2==p1)  
                {  
                    p1=p2->next;  
                    p2->next=p1->next;  
                    p1->next=p2;  
                    ptemp=p1;  
                }  
                else  
                {  
                    ptemp->next=p2->next;  
                    ptemp=p2->next;  
                    p2->next=ptemp->next;  
                    ptemp->next=p2;  
                      
                }  
            }  
            else  
            {  
                ptemp=p2;  
                p2=p2->next;  
            }  
        }  
        pfinished=p2;  
    }  
    head=p1;          
    pdisplay_sort();  
}  
  
  
  
  
//保存链表，把链表保存到相应文件中   
void save()  
{     
    STUDENT *temp;  
    char ch;      
    temp=head;  
    char filename[LEN];   
    getch();  
    printf("是否把信息存储到文件中？(是-Y  否-N)：");   
    scanf("\n%c",&ch);            
    if(toupper(ch)=='Y')  
    {  
        printf("是否储存到原文件中？(是-Y  否-N)：");  
        scanf("\n%c",&ch);  
        /*******************创建新文件，并把链表写入*************************/   
        if(toupper(ch)=='N')  
        {  
            printf("\n请输入要保存的文件的完整路径及文件名：");  
            scanf("\n");  
            gets(filename);   
            fp=fopen(filename,"wt");  
            while(temp!=NULL)  
            {  
                fwrite(temp,sizeof(Node),1,fp);  
                temp=temp->next;       
            }                     
            fclose(fp);   
        }  
        else /****************把数据写入原文件fileopenname*********************/   
        {  
            fp=fopen("student_system.txt","wt");  
            while(temp!=NULL)  
            {  
                fwrite(temp,sizeof(Node),1,fp);  
                temp=temp->next;       
            }                     
            fclose(fp);       
        }  
    }  
}
```
admin.cpp


```
#include"head.h"  
  
  
  
void admin()  
{  
    if(judge_admin())  
    {  
        read_file();   
        menu_admin();  
    }  
    else   
        return ;  
}  
  
  
bool judge_admin()  
{  
    int i=0,j=0;  
    FILE *pf;  
    char password[100];  
    char password1[100];  
    pf=fopen("order_admin.txt","rt");  
    fscanf(pf,"%s",password);  
    fclose(pf);  
    printf("\n请输入密码：");  
    do  
    {  
        password1[i]=getch();  
        if(password1[i]==0x1B)  
        {  
            return false;  
        }  
        if(password1[i]==13)  
        {  
            password1[i]='\0';  
            break;  
        }  
        i++;  
        printf("*");  
    }while(1);  
    if(!strcmp(MD5(password1),password))  
            return true;  
    else  
    {  
         printf("\n密码错误！");  
         return false;  
    }  
}  
  
  
  
int menu_admin()  
{  
    int chioce;  
    do  
    {  
        printf("\n\t\t*******************************\n");  
        printf("\t\t*        1.信息录入           *\n");  
        printf("\t\t*        2.信息查看           *\n");  
        printf("\t\t*        3.信息修改           *\n");              
        printf("\t\t*        4.信息查询           *\n");  
        printf("\t\t*        5.信息删除           *\n");  
        printf("\t\t*        6.信息排序           *\n");  
        printf("\t\t*        7.学生密码管理       *\n");  
        printf("\t\t*        8.老师密码管理       *\n");  
        printf("\t\t*        9.更改管理员密码     *\n");  
        printf("\t\t*        0.退出               *\n");  
        printf("\t\t*******************************\n");  
        printf("\t\t请选择：");  
        scanf("%d",&chioce);  
        switch(chioce)  
        {    
            case 1:  
                pinput();  
                break;  
            case 2:  
                pdisplay();  
                break;  
            case 3:  
                change_pmenu_search();  
                break;  
            case 4:  
                pmenu_search();  
                break;  
            case 5:  
                delete_pmenu_search();  
                break;  
            case 6:  
                psort_menu();   
                break;  
            case 7:  
                read_file_order_stu();  
                menu_change_order_stu();  
                break;  
            case 8:  
                read_file_order_tea();  
                menu_change_order_tea();  
                break;  
            case 9:  
                change_order_admin();  
                break;  
            default:  
                break;  
        }  
    }while(chioce!=0);  
    return 0;  
}  
  
  
  
  
void change_order_admin()  
{  
    int i=0;  
    FILE *fp;  
    FILE *fp1;  
    char password[100];  
    char oldpassword[100];  
    char newpassword[100];  
    char newpassword1[100];  
    fp=fopen("order_admin.txt","rt");  
    fscanf(fp,"%s",password);  
    fclose(fp);  
  
    printf("请输入原来密码：");  
    scanf("%s",oldpassword);  
    if(strcmp(password,MD5(oldpassword)))     
    {  
        printf("\n密码错误！");  
        return ;  
    }  
    printf("\n请输入新密码：");  
    scanf("%s",newpassword);  
      
    strcpy(newpassword1,MD5(newpassword));  
    fp1=fopen("order_admin.txt","wt");  
    fprintf(fp1,"%s",newpassword1);  
    fclose(fp1);  
    printf("\n密码修改完毕！");  
}  
  
  
/**************************admin_tea***************************/  
int  menu_change_order_tea()  
{  
    int chioce;  
    do  
    {  
        printf("\n\t\t********************************\n");  
        printf("\t\t*        1.添加信息             *\n");  
        printf("\t\t*        2.更改信息             *\n");  
        printf("\t\t*        3.删除信息             *\n");  
        printf("\t\t*        0.退出                 *\n");  
        printf("\t\t*********************************\n");  
        printf("\t\t请选择：");  
        scanf("%d",&chioce);  
        switch(chioce)  
        {    
            case 0:  
                save_order_tea();  
                break;  
            case 1:  
                change_add_order_tea();  
                break;  
            case 2:  
                change_change_order_tea();  
                break;  
            case 3:  
                change_delete_order_tea();  
                break;  
            default:  
                break;  
        }  
    }while(chioce!=0);  
    return 0;  
}  
  
void change_delete_order_tea()//管理老师密码  
{  
    int x;  
    ORDER_TEA *temp=head_order_tea;  
    char username1[20];  
    printf("\n请输入用户名：");  
    scanf("%s",username1);  
    while(temp!=NULL)  
    {  
        if(!strcmp(temp->information.username,username1))  
        {  
            temp->last->next=temp->next;  
            printf("\n删除完毕！");  
            return ;  
        }  
        temp=temp->next;  
        x++;  
    }  
    printf("不存在此用户名！");  
    return ;  
}  
  
  
  
  
void change_change_order_tea()  
{  
    int x,i=0;  
    ORDER_TEA *temp=head_order_tea;  
    char username1[100];  
    char password1[100];  
    printf("\n请输入用户名：");  
    scanf("%s",username1);  
    while(temp!=NULL)  
    {  
        if(!strcmp(temp->information.username,username1))  
        {         
            printf("\n请重新输入密码：");  
            scanf("%s",password1);  
            /*do 
            { 
                if(password1[i]=='\0')   break; 
                password1[i]=password1[i]+i; 
                i++; 
            }while(1);*/  
            strcpy(temp->information.password,MD5(password1));  
            printf("修改完毕！");  
            return ;  
        }  
        temp=temp->next;  
        x++;  
    }  
    printf("不存在此用户名！");  
    return ;  
}  
  
  
  
  
  
  
ORDER_TEA *change_add_order_tea()  
{  
    int i=0;  
    char username2[100];  
    char password2[100];  
    char ch;  
    if(num_order_tea==0)//判断是否是第一次录入   
    {  
        iend_order_tea=inew_order_tea=(ORDER_TEA *)malloc(sizeof(ORDER_TEA));//申请空间   
        printf("\n请输入用户名：");  
        scanf("%s",username2);  
        strcpy(inew_order_tea->information.username,username2);  
        printf("\n请输入密码：");  
        scanf("%s",password2);  
        /*do 
        { 
            if(password2[i]=='\0')   break; 
            password2[i]=password2[i]+i; 
            i++; 
        }while(1);*/  
        strcpy(inew_order_tea->information.password,MD5(password2));  
        //录入信息   
        while(1)                                        
        {  
            num_order_tea++;             
            if(num_order_tea==1)       //录入第一个学生信息时，第一个节点的指针指向             
            {  
                inew_order_tea->next=NULL;  
                inew_order_tea->last=NULL;   
                iend_order_tea      =inew_order_tea;  
                head_order_tea      =inew_order_tea;  
            }  
            else             //在第一个节点的存在下，进行其他节点的录入                 
            {  
                inew_order_tea->next=NULL;     //新建立节点next指向NULL           
                inew_order_tea->last=iend_order_tea;     //新建立节点last指向上一个节点   
                iend_order_tea->next=inew_order_tea;     //第一个next->下一个节点  
                iend_order_tea      =inew_order_tea;     //让iend指向最后一个节点   
            }  
            inew_order_tea=(ORDER_TEA *)malloc(sizeof(ORDER_TEA));  
            printf("信息输入完毕，是否继续？(是-Y  否-N)：");   
            scanf("\n%c",&ch);  
            if(ch=='N'||ch=='n')     break;//判断是否跳出循环   
            printf("\n请输入用户名：");  
            scanf("%s",username2);  
            strcpy(inew_order_tea->information.username,username2);  
            printf("\n请输入密码：");  
            scanf("%s",password2);  
            /*do 
            { 
                if(password2[i]=='\0')   break; 
                password2[i]=password2[i]+i; 
                i++; 
            }while(1);*/  
            strcpy(inew_order_tea->information.password,MD5(password2));   
        }  
        free(inew_order_tea);                  //释放未存储数据的节点   
        return head_order_tea;  
    }  
    else//非第一次录入时   
    {  
        while(1)//进行循环输入，并进行判断，是否跳出循环   
        {  
              
            inew_order_tea=(ORDER_TEA *)malloc(sizeof(ORDER_TEA));  
            getch();  
              
            printf("\n请输入用户名：");  
            scanf("%s",username2);  
            strcpy(inew_order_tea->information.username,username2);  
            printf("\n请输入密码：");  
            scanf("%s",password2);  
            do  
            {  
                if(password2[i]=='\0')   break;  
                password2[i]=password2[i]+i;  
                i++;  
            }while(1);  
            strcpy(inew_order_tea->information.password,password2);  
              
            num_order_tea++;  
            inew_order_tea->next=NULL;   //新建立节点next指向NULL      
            inew_order_tea->last=iend_order_tea;   //新建立节点last指向上一个节点  
            iend_order_tea->next=inew_order_tea;   //第一个next->下一个节点     
            iend_order_tea      =inew_order_tea;   //让iend指向最后一个节点  
            printf("信息输入完毕，是否继续？(是-Y  否-N)：");  
            scanf("\n%c",&ch);  
            if(ch=='N'||ch=='n')  
             break;  
        }  
        return head_order_tea;  
    }  
}  
  
void save_order_tea()  
{  
    int i=0;  
    ORDER_TEA *temp=head_order_tea;  
    fp_order_tea=fopen("order_tea.txt","wt");  
    while(temp!=NULL)  
    {  
        fwrite(temp,sizeof(Node_order_tea),1,fp_order_tea);  
        temp=temp->next;       
    }                     
    fclose(fp_order_tea);     
}  
  
  
/*************************admin_student***********************************/  
int  menu_change_order_stu()  
{  
    int chioce;  
    do  
    {  
        printf("\n\t\t********************************\n");  
        printf("\t\t*        1.添加信息             *\n");  
        printf("\t\t*        2.更改信息             *\n");  
        printf("\t\t*        3.删除信息             *\n");  
        printf("\t\t*        0.退出                 *\n");  
        printf("\t\t*********************************\n");  
        printf("\t\t请选择：");  
        scanf("%d",&chioce);  
        switch(chioce)  
        {    
            case 0:  
                save_order_stu();  
                break;  
            case 1:  
                change_add_order_stu();  
                break;  
            case 2:  
                change_change_order_stu();  
                break;  
            case 3:  
                change_delete_order_stu();  
                break;  
            default:  
                break;  
        }  
    }while(chioce!=0);  
    return 0;  
}  
  
void change_delete_order_stu()//管理学生密码  
{  
    int x;  
    ORDER_STU *temp=head_order_stu;  
    char username1[20];  
    printf("\n请输入用户名：");  
    scanf("%s",username1);  
    while(temp!=NULL)  
    {  
        if(!strcmp(temp->information.username,username1))  
        {  
            temp->last->next=temp->next;  
            printf("\n删除完毕！");  
            return ;  
        }  
        temp=temp->next;  
        x++;  
    }  
    printf("不存在此用户名！");  
    return ;  
}  
  
  
  
  
void change_change_order_stu()  
{  
    int x,i=0;  
    ORDER_STU *temp=head_order_stu;  
    char username1[100];  
    char password1[100];  
    printf("\n请输入用户名：");  
    scanf("%s",username1);  
    while(temp!=NULL)  
    {  
        if(!strcmp(temp->information.username,username1))  
        {         
            printf("\n请重新输入密码：");  
            scanf("%s",password1);  
            /*do 
            { 
                if(password1[i]=='\0')   break; 
                password1[i]=password1[i]+i; 
                i++; 
            }while(1);*/  
            strcpy(temp->information.password,MD5(password1));  
            printf("修改完毕！");  
            return ;  
        }  
        temp=temp->next;  
        x++;  
    }  
    printf("不存在此用户名！");  
    return ;  
}  
  
  
  
  
  
  
ORDER_STU *change_add_order_stu()  
{  
    int i=0;  
    char username2[100];  
    char password2[100];  
    char ch;  
    if(num_order_stu==0)//判断是否是第一次录入   
    {  
        iend_order_stu=inew_order_stu=(ORDER_STU *)malloc(sizeof(ORDER_STU));//申请空间   
        printf("\n请输入用户名：");  
        scanf("%s",username2);  
        strcpy(inew_order_stu->information.username,username2);  
        printf("\n请输入密码：");  
        scanf("%s",password2);  
        strcpy(inew_order_stu->information.password,MD5(password2));  
        //录入信息   
        while(1)                                        
        {  
            num_order_stu++;             
            if(num_order_stu==1)       //录入第一个学生信息时，第一个节点的指针指向             
            {  
                inew_order_stu->next=NULL;  
                inew_order_stu->last=NULL;   
                iend_order_stu      =inew_order_stu;  
                head_order_stu      =inew_order_stu;  
            }  
            else             //在第一个节点的存在下，进行其他节点的录入                 
            {  
                inew_order_stu->next=NULL;     //新建立节点next指向NULL           
                inew_order_stu->last=iend_order_stu;     //新建立节点last指向上一个节点   
                iend_order_stu->next=inew_order_stu;     //第一个next->下一个节点  
                iend_order_stu      =inew_order_stu;     //让iend指向最后一个节点   
            }  
            inew_order_stu=(ORDER_STU *)malloc(sizeof(ORDER_STU));  
            printf("信息输入完毕，是否继续？(是-Y  否-N)：");   
            scanf("\n%c",&ch);  
            if(ch=='N'||ch=='n')     break;//判断是否跳出循环   
            printf("\n请输入用户名：");  
            scanf("%s",username2);  
            strcpy(inew_order_stu->information.username,MD5(password2));  
            printf("\n请输入密码：");  
            scanf("%s",password2);  
            do  
            {  
                if(password2[i]=='\0')   break;  
                password2[i]=password2[i]+i;  
                i++;  
            }while(1);  
            strcpy(inew_order_stu->information.password,password2);    
        }  
        free(inew_order_stu);                  //释放未存储数据的节点   
        return head_order_stu;  
    }  
    else//非第一次录入时   
    {  
        while(1)//进行循环输入，并进行判断，是否跳出循环   
        {  
              
            inew_order_stu=(ORDER_STU *)malloc(sizeof(ORDER_STU));  
            getch();  
              
            printf("\n请输入用户名：");  
            scanf("%s",username2);  
            strcpy(inew_order_stu->information.username,username2);  
            printf("\n请输入密码：");  
            scanf("%s",password2);  
            do  
            {  
                if(password2[i]=='\0')   break;  
                password2[i]=password2[i]+i;  
                i++;  
            }while(1);  
            strcpy(inew_order_stu->information.password,password2);  
              
            num_order_stu++;  
            inew_order_stu->next=NULL;   //新建立节点next指向NULL      
            inew_order_stu->last=iend_order_stu;   //新建立节点last指向上一个节点  
            iend_order_stu->next=inew_order_stu;   //第一个next->下一个节点     
            iend_order_stu      =inew_order_stu;   //让iend指向最后一个节点  
            printf("信息输入完毕，是否继续？(是-Y  否-N)：");  
            scanf("\n%c",&ch);  
            if(ch=='N'||ch=='n')  
             break;  
        }  
        return head_order_stu;  
    }  
}  
  
void save_order_stu()  
{  
    int i=0;  
    ORDER_STU *temp=head_order_stu;  
    fp_order_stu=fopen("order_stu.txt","wt");  
    while(temp!=NULL)  
    {  
        fwrite(temp,sizeof(Node_order_stu),1,fp_order_stu);  
        temp=temp->next;       
    }                     
    fclose(fp_order_stu);     
}  
  
  
  
  
  
  
  
/***************************************************************************/
```
student.cpp


```
#include"head.h"  
  
  
/*****************************student**********************************/  
  
  
void student()  
{  
    read_file_order_stu();  
    if(judge_stu())     
    {  
        read_file();  
        menu_stu();  
    }  
    else   return ;  
}  
  
  
  
int menu_stu()  
{  
    int chioce;  
    do  
    {  
        printf("\n\t\t*******************************\n");  
        printf("\t\t*        1.信息查看           *\n");              
        printf("\t\t*        2.信息查询           *\n");  
        printf("\t\t*        3.信息排序           *\n");  
        printf("\t\t*        0.退出               *\n");  
        printf("\t\t*******************************\n");  
        printf("\t\t请选择：");  
        scanf("%d",&chioce);  
        switch(chioce)  
        {    
            case 1:  
                pdisplay();  
                break;  
            case 2:  
                pmenu_search();  
                break;  
            case 3:  
                psort_menu();   
                break;  
            default:  
                break;  
        }  
    }while(chioce!=0);  
    return 0;  
}  
  
  
  
  
  
bool judge_stu()  
{  
    int x,i=0,j=0;  
    ORDER_STU *temp=head_order_stu;  
    char username1[100];  
    char password1[100];  
    char password2[100];  
      
    printf("\n请输入用户名：");  
    scanf("%s",username1);  
  
    while(temp!=NULL)  
    {  
        if(!strcmp(temp->information.username,username1))  
        {         
            printf("\n请输入密码：");  
            do  
            {  
                password1[i]=getch();  
                if(password1[i]==13)  
                {  
                    password1[i]='\0';  
                    break;  
                }  
                i++;  
                printf("*");      
            }while(1);  
              
            /*do{ 
                if(temp->information.password[j]=='\0')  
                { 
                    password2[j]='\0'; 
                    break; 
                } 
                password2[j]=temp->information.password[j]-j; 
                j++; 
            }while(1);*/  
            strcpy(password2,temp->information.password);  
            if(!strcmp(MD5(password1),password2))  
                return true;  
            else  
            {  
                    printf("\n密码错误！");  
                    return false;  
            }  
          
        }  
        temp=temp->next;  
        x++;  
    }  
    printf("不存在此用户名！");  
    return false;  
}  
  
  
int read_file_order_stu()  
{  
    int i;  
    int fno,fsize;  
    if((fp_order_stu=fopen("order_stu.txt","rt"))==NULL)   
    {  
        printf("\n库文件不存在！\n");  
        return 0;  
    }  
    rewind(fp_order_stu);                            //使文件内部指针移动到文件的开始位置   
    fno=fileno(fp_order_stu);              //获取文件描述字                         
    fsize=filelength(fno);      //计算文件大小，以字节为单位   
    num_order_stu=fsize/sizeof(Node_order_stu);     //计算文件包含的学生信息数目   
    iend_order_stu=inew_order_stu=(ORDER_STU *)malloc(sizeof(ORDER_STU));//申请动态空间   
    fread(inew_order_stu,sizeof(Node_order_stu),1,fp_order_stu);   //以Node（结构体）的空间大小读数据   
    for(i=1;i<=num_order_stu;i++)   //利用for循环创建链表，并把数据储存到其中   
    {  
        if(i==1)          //第一个节点的创建   
        {  
            inew_order_stu->next=head_order_stu;  
            inew_order_stu->last=NULL; //双向链表   
            iend_order_stu=inew_order_stu;  
            head_order_stu=inew_order_stu;  
        }  
        else  
        {  
            inew_order_stu->next=NULL;  
            inew_order_stu->last=iend_order_stu;  
            iend_order_stu->next=inew_order_stu;  
            iend_order_stu      =inew_order_stu;  
        }  
        inew_order_stu=(ORDER_STU *)malloc(sizeof(ORDER_STU));  
        fread(inew_order_stu,sizeof(Node_order_stu),1,fp_order_stu);  
    }  
    free(inew_order_stu);//释放最后一个没有储存数据的节点   
    fclose(fp_order_stu);//关闭文件   
    return num_order_stu;  
}  
  
  
  
  
  
  
/****************************************************/
```
teacher.cpp


```
#include"head.h"  
  
  
  
/************************teacher************************************/  
void teacher()  
{  
    read_file_order_tea();  
    if(judge_tea())   
    {  
        read_file();  
        menu_tea();  
    }  
    else   return ;  
}  
  
  
  
int menu_tea()  
{  
    int chioce;  
    do  
    {  
        printf("\n\t\t*******************************\n");  
        printf("\t\t*        1.信息录入           *\n");  
        printf("\t\t*        2.信息查看           *\n");  
        printf("\t\t*        3.信息修改           *\n");              
        printf("\t\t*        4.信息查询           *\n");  
        printf("\t\t*        5.信息删除           *\n");  
        printf("\t\t*        6.信息排序           *\n");  
        printf("\t\t*        0.退出               *\n");  
        printf("\t\t*******************************\n");  
        printf("\t\t请选择：");  
        scanf("%d",&chioce);  
        switch(chioce)  
        {    
            case 1:  
                pinput();  
                break;  
            case 2:  
                pdisplay();  
                break;  
            case 3:  
                change_pmenu_search();  
                break;  
            case 4:  
                pmenu_search();  
                break;  
            case 5:  
                delete_pmenu_search();  
                break;  
            case 6:  
                psort_menu();   
                break;  
            default:  
                break;  
        }  
    }while(chioce!=0);  
    return 0;  
}  
  
  
  
  
  
bool judge_tea()  
{  
    int x,i=0,j=0;  
    ORDER_TEA *temp=head_order_tea;  
    char username1[100];  
    char password1[100];  
    char password2[100];  
      
    printf("\n请输入用户名：");  
    scanf("%s",username1);  
  
    while(temp!=NULL)  
    {  
        if(!strcmp(temp->information.username,username1))  
        {         
            printf("\n请输入密码：");  
            do  
            {  
                password1[i]=getch();  
                if(password1[i]==13)  
                {  
                    password1[i]='\0';  
                    break;  
                }  
                i++;  
                printf("*");  
                  
            }while(1);  
            /*do{ 
                if(temp->information.password[j]=='\0')  
                { 
                    password2[j]='\0'; 
                    break; 
                } 
                password2[j]=(temp->information.password[j])-j; 
                j++; 
            }while(1);*/  
            strcpy(password2,temp->information.password);  
            if(!strcmp(MD5(password1),password2))  
                return true;  
            else  
            {  
                    printf("\n密码错误！");  
                    return false;  
            }  
          
        }  
        temp=temp->next;  
        x++;  
    }  
    printf("不存在此用户名！");  
    return false;  
}  
  
  
int read_file_order_tea()  
{  
    int i;  
    int fno,fsize;  
    if((fp_order_tea=fopen("order_tea.txt","rt"))==NULL)   
    {  
        printf("\n库文件不存在！\n");  
        return 0;  
    }  
    rewind(fp_order_tea);                            //使文件内部指针移动到文件的开始位置   
    fno=fileno(fp_order_tea);              //获取文件描述字                         
    fsize=filelength(fno);      //计算文件大小，以字节为单位   
    num_order_tea=fsize/sizeof(Node_order_tea);     //计算文件包含的学生信息数目   
    iend_order_tea=inew_order_tea=(ORDER_TEA *)malloc(sizeof(ORDER_TEA));//申请动态空间   
    fread(inew_order_tea,sizeof(Node_order_tea),1,fp_order_tea);   //以Node（结构体）的空间大小读数据   
    for(i=1;i<=num_order_tea;i++)   //利用for循环创建链表，并把数据储存到其中   
    {  
        if(i==1)          //第一个节点的创建   
        {  
            inew_order_tea->next=head_order_tea;  
            inew_order_tea->last=NULL; //双向链表   
            iend_order_tea=inew_order_tea;  
            head_order_tea=inew_order_tea;  
        }  
        else  
        {  
            inew_order_tea->next=NULL;  
            inew_order_tea->last=iend_order_tea;  
            iend_order_tea->next=inew_order_tea;  
            iend_order_tea      =inew_order_tea;  
        }  
        inew_order_tea=(ORDER_TEA *)malloc(sizeof(ORDER_TEA));  
        fread(inew_order_tea,sizeof(Node_order_tea),1,fp_order_tea);  
    }  
    free(inew_order_tea);//释放最后一个没有储存数据的节点   
    fclose(fp_order_tea);//关闭文件   
    return num_order_tea;  
}
```
MD5.cpp


```
/************************************************************************* 
    > File Name: md5_1.cpp 
    > Author:chudongfang  
    > Mail:1149669942@qq.com  
    > Created Time: 2016年06月22日 星期三 16时12分30秒 
 ************************************************************************/  
 #include"head.h"  
//第一位1 其后若干个0,用于MD5Final函数时的补足  
  
/************************ 
 * 函数功能：初始化一个MD5 text 
 * 函数参数：MD5 text 指针 
 * ***********************/  
//初始化  
void MD5Init(MD5_CTX *context)    
{    
    context->count[0] = 0;    
    context->count[1] = 0;     
    //分别赋固定值    
    context->state[0] = 0x67452301;  
    context->state[1] = 0xEFCDAB89;    
    context->state[2] = 0x98BADCFE;    
    context->state[3] = 0x10325476;    
}    
  
  
/************************************************ 
 * 函数功能：对一个MD5 text,把输入的数据进行分组，并进行加密 
 * 未用到的数据把其储存在MD5 text中。 
 * 
 * 参数分析： 
 * MD5_CTX *context       ：一个MD5 text    
 * unsigned char *input   ：新添加的数据   
 * unsigned int inputlen  ：新添加数据的长度(字节) 
 * 
 ***********************************************/  
  
void MD5Update(MD5_CTX *context,unsigned char *input,unsigned int inputlen)    
{    
    unsigned int i = 0,index = 0,partlen = 0;    
      
    //index：当前状态的位数对64取余，其单位是字节  
    //也可以写作：  index=(context->count[0]/8)%64  
    index = (context->count[0] >> 3) & 0x3F;    
      
    //partlen:可以补齐64字节的字节数  
    partlen = 64 - index;    
      
    //下面代码是解决一个unsignde int 无法储存极大数据导致溢出的问题  
    //当前位数加上新添加的位数，由于inputlen是以字节为单位，所以其转换为位数  
    //相当于context->count[0] += inputlen*8;    
    context->count[0] += inputlen << 3;    
     
    //当其出现溢出的情况时，通过以下操作把两个16位的数连在一块，生成一个  
    //32位的二进制数串，从而扩大其储存范围  
    if(context->count[0] < (inputlen << 3))    
        context->count[1]++;  
      
    //该语句可替换为 context->count[1]+=(inputlen<<3)>>32;  
    //便于理解  
    context->count[1] += inputlen >> 29;    
      
    //当其输入字节数的大于其可以补足64字节的字节数，进行补足  
    if(inputlen >= partlen)    
    {    
        //向buffer中补足partlen个字节，使其到达64字节  
        memcpy(&context->buffer[index],input,partlen);  
          
        //buffer达到64字节512位，则把其作为一组进行运算  
        MD5Transform(context->state,context->buffer);    
          
        //如果输入的数据还可以组成多个64字节，则把其可以组成  
        //的作为若干组进行运算  
        for(i = partlen;i+64 <= inputlen;i+=64)    
            MD5Transform(context->state,&input[i]);    
          
        //恢复0值，照应 下面 把输入 剩余字节(不能组成64字节组) 储存的操作  
        index = 0;            
    }     
    //否则，把输入的数据按顺序放在原来数据后面  
    else    
    {    
        i = 0;    
    }    
  
    //放置剩余数据  
    memcpy(&context->buffer[index],&input[i],inputlen-i);    
}    
  
  
  
  
/************************************************* 
 * 函数功能：对数据进行补足，并加入数据位数信息，并进一步加密 
 *  
 * 参数分析： 
 * MD5_CTX *context          :一个MD5 text 
 * unsigned char digest[16]  :储存加密结果的数组 
 *************************************************/  
  
void MD5Final(MD5_CTX *context,unsigned char digest[16])    
{    
    unsigned int index = 0,padlen = 0;    
      
    //bits： 8个字节，64位  
    unsigned char bits[8];    
      
    //index:对64取余结果  
    index = (context->count[0] >> 3) & 0x3F;    
    //因为要填充满足使其位长对512求余的结果等于448（56位）  
    //所以当其所剩余的数小于56字节，则填充56-index字节，  
    //否则填充120-index字节  
    //这里padlen代表其所需填充的字节  
    padlen = (index < 56)?(56-index):(120-index);    
      
    //然后，在这个结果后面附加一个以64位二进制表示的填充前数据长度。  
    //把填充前数据数据长度转换后放到bit字符数组中  
    MD5Encode(bits,context->count,8);  
      
    //根据已经存储好的数组PADDING，在信息的后面填充一个1和无数个0，  
    //直到满足上面的条件时才停止用0对信息的填充  
    //其填充后进行了一系列的加密操作，其定剩余48个字节  
    MD5Update(context,PADDING,padlen);    
  
    //在最后添加进8个字节的数据长度信息，最后凑成一组，进行一次加密处理  
    MD5Update(context,bits,8);    
      
    //把最终得到的加密信息变成字符输出，共16字节  
    MD5Encode(digest,context->state,16);    
}    
  
  
  
  
/********************************************************** 
 * 函数功能：利用位操作，按1->4方式把数字分解成字符 
 * 
 * 参数分析： 
 * unsigned char  *output ：输出的字符的数组 
 * unsigned int   *input  ：输入数字的数组 
 * unsigned int   len     : 输入数字数组的长度（单位：位）  
 * *********************************************************/  
  
void MD5Encode(unsigned char *output,unsigned int *input,unsigned int len)    
{    
    unsigned int i = 0,j = 0;    
    while(j < len)    
    {    
        //这里& 0xFF为取后8位  
        //i代表数字数组下标  
        //j代表字符数组下标  
        //把数字的8、8-16、16-24、24-32分别赋值给字符  
        output[j] = input[i] & 0xFF;      
        output[j+1] = (input[i] >> 8) & 0xFF;    
        output[j+2] = (input[i] >> 16) & 0xFF;    
        output[j+3] = (input[i] >> 24) & 0xFF;    
        i++;    
        j+=4;    
    }    
}    
  
  
  
  
/********************************************************** 
 * 函数功能：利用位操作，按4->1方式把字符合成数字 
 * 
 * 参数分析： 
 * unsigned int  *output ：输出的数字的数组 
 * unsigned char *input  ：输入字符的数组 
 * unsigned int  len     : 输入字符的长度 （单位：位） 
 * *********************************************************/  
  
void MD5Decode(unsigned int *output,unsigned char *input,unsigned int len)    
{    
    unsigned int i = 0,j = 0;    
    while(j < len)    
    {    
        //利用位操作，把四个单位为1字节的字符，合成一个单位为4字节的数字  
        //因为FF GG HH II和非线性函数都只能对数字进行处理  
        //第一个字符占前8位，第二个占8-16位，第三个占16-24位，第四个占  
        //24-32位。  
        //i代表数字数组下标  
        //j代表字符数组下标  
        output[i] = (input[j]) |    
            (input[j+1] << 8) |    
            (input[j+2] << 16) |    
            (input[j+3] << 24);    
        i++;    
        j+=4;     
    }    
}    
  
/************************************************************** 
* 函数功能：对512位的block数据进行加密，并把加密结果存入state数组中 
* 对512位信息(即block字符数组)进行一次处理，每次处理包括四轮 
*state[4]：md5结构中的state[4]，用于保存对512bits信息加密的中间结果或者最终结果 
* block[64]：欲加密的512bits信息或其中间数据 
***************************************************************/  
void MD5Transform(unsigned int state[4],unsigned char block[64])    
{    
    //a b c d继承上一个加密的结果，所以其具有继承性  
    unsigned int a = state[0];    
    unsigned int b = state[1];    
    unsigned int c = state[2];    
    unsigned int d = state[3];    
      
    //这里只需用到16个，我把原来的unsiged int x[64]  改为了 x[16]  
    unsigned int x[16];    
      
    //把字符转化成数字，便于运算  
    MD5Decode(x,block,64);    
      
  
    //具体函数方式固定，不再赘述  
  
    /*************第一轮******************/  
    FF(a, b, c, d, x[ 0], 7, 0xd76aa478);     
    FF(d, a, b, c, x[ 1], 12, 0xe8c7b756);     
    FF(c, d, a, b, x[ 2], 17, 0x242070db);     
    FF(b, c, d, a, x[ 3], 22, 0xc1bdceee);     
      
    FF(a, b, c, d, x[ 4], 7, 0xf57c0faf);     
    FF(d, a, b, c, x[ 5], 12, 0x4787c62a);     
    FF(c, d, a, b, x[ 6], 17, 0xa8304613);     
    FF(b, c, d, a, x[ 7], 22, 0xfd469501);     
      
    FF(a, b, c, d, x[ 8], 7, 0x698098d8);     
    FF(d, a, b, c, x[ 9], 12, 0x8b44f7af);     
    FF(c, d, a, b, x[10], 17, 0xffff5bb1);     
    FF(b, c, d, a, x[11], 22, 0x895cd7be);     
      
    FF(a, b, c, d, x[12], 7, 0x6b901122);     
    FF(d, a, b, c, x[13], 12, 0xfd987193);     
    FF(c, d, a, b, x[14], 17, 0xa679438e);     
    FF(b, c, d, a, x[15], 22, 0x49b40821);     
    
        
    /*************第二轮*****************/  
    GG(a, b, c, d, x[ 1], 5, 0xf61e2562);     
    GG(d, a, b, c, x[ 6], 9, 0xc040b340);     
    GG(c, d, a, b, x[11], 14, 0x265e5a51);     
    GG(b, c, d, a, x[ 0], 20, 0xe9b6c7aa);     
      
    GG(a, b, c, d, x[ 5], 5, 0xd62f105d);     
    GG(d, a, b, c, x[10], 9,  0x2441453);     
    GG(c, d, a, b, x[15], 14, 0xd8a1e681);     
    GG(b, c, d, a, x[ 4], 20, 0xe7d3fbc8);     
      
    GG(a, b, c, d, x[ 9], 5, 0x21e1cde6);     
    GG(d, a, b, c, x[14], 9, 0xc33707d6);     
    GG(c, d, a, b, x[ 3], 14, 0xf4d50d87);     
    GG(b, c, d, a, x[ 8], 20, 0x455a14ed);     
      
    GG(a, b, c, d, x[13], 5, 0xa9e3e905);     
    GG(d, a, b, c, x[ 2], 9, 0xfcefa3f8);     
    GG(c, d, a, b, x[ 7], 14, 0x676f02d9);     
    GG(b, c, d, a, x[12], 20, 0x8d2a4c8a);     
    
        
    /*************第三轮*****************/  
    HH(a, b, c, d, x[ 5], 4, 0xfffa3942);     
    HH(d, a, b, c, x[ 8], 11, 0x8771f681);     
    HH(c, d, a, b, x[11], 16, 0x6d9d6122);     
    HH(b, c, d, a, x[14], 23, 0xfde5380c);     
      
    HH(a, b, c, d, x[ 1], 4, 0xa4beea44);     
    HH(d, a, b, c, x[ 4], 11, 0x4bdecfa9);     
    HH(c, d, a, b, x[ 7], 16, 0xf6bb4b60);     
    HH(b, c, d, a, x[10], 23, 0xbebfbc70);     
      
    HH(a, b, c, d, x[13], 4, 0x289b7ec6);     
    HH(d, a, b, c, x[ 0], 11, 0xeaa127fa);     
    HH(c, d, a, b, x[ 3], 16, 0xd4ef3085);     
    HH(b, c, d, a, x[ 6], 23,  0x4881d05);     
      
    HH(a, b, c, d, x[ 9], 4, 0xd9d4d039);     
    HH(d, a, b, c, x[12], 11, 0xe6db99e5);     
    HH(c, d, a, b, x[15], 16, 0x1fa27cf8);     
    HH(b, c, d, a, x[ 2], 23, 0xc4ac5665);     
    
        
      
    /*************第四轮******************/  
    II(a, b, c, d, x[ 0], 6, 0xf4292244);     
    II(d, a, b, c, x[ 7], 10, 0x432aff97);     
    II(c, d, a, b, x[14], 15, 0xab9423a7);     
    II(b, c, d, a, x[ 5], 21, 0xfc93a039);     
      
    II(a, b, c, d, x[12], 6, 0x655b59c3);     
    II(d, a, b, c, x[ 3], 10, 0x8f0ccc92);     
    II(c, d, a, b, x[10], 15, 0xffeff47d);     
    II(b, c, d, a, x[ 1], 21, 0x85845dd1);     
      
    II(a, b, c, d, x[ 8], 6, 0x6fa87e4f);     
    II(d, a, b, c, x[15], 10, 0xfe2ce6e0);     
    II(c, d, a, b, x[ 6], 15, 0xa3014314);     
    II(b, c, d, a, x[13], 21, 0x4e0811a1);     
      
    II(a, b, c, d, x[ 4], 6, 0xf7537e82);     
    II(d, a, b, c, x[11], 10, 0xbd3af235);     
    II(c, d, a, b, x[ 2], 15, 0x2ad7d2bb);     
    II(b, c, d, a, x[ 9], 21, 0xeb86d391);     
      
  
    //更换原来的结果  
    state[0] += a;    
    state[1] += b;    
    state[2] += c;    
    state[3] += d;    
}    
  
  
char * MD5(char *encrypt)  
{  
    MD5_CTX md5;  //定义一个MD5 text  
    MD5Init(&md5);//初始化           
    unsigned char decrypt[16]; //加密结果     
    
    MD5Update(&md5,(unsigned char*)encrypt,strlen(encrypt));//进行初步分组加密    
    MD5Final(&md5,decrypt);   //进行后序的补足，并加密   
  
    return (char *)decrypt;  
}
```
head.h


```
#include<stdio.h>  
#include<math.h>  
#include<string.h>  
#include<conio.h>  
#include<stdlib.h>  
#include<ctype.h>  
#include<io.h>  
   
#define LEN  100  
#define PAGE 3  //分页输出时，每页的学生个数   
  
  
  
  
  
void admin();  
int menu_admin();  
void change_change_order_tea();  
int  menu_change_order_tea();  
void change_delete_order_tea();  
  
void teacher();  
int menu_tea();  
bool judge_tea();  
int read_file_order_tea();  
  
  
  
  
void change_change_order_stu();  
int  menu_change_order_stu();  
void change_delete_order_stu();  
  
  
void student();  
int menu_stu();  
bool judge_stu();  
int read_file_order_stu();  
  
bool judge_admin();  
void change_order_admin();  
  
  
  
  
  
int  menu();  
int  pmenu_search();  
int  read_file();  
  
void pdisplay();  
void pdisplay_sort();  
  
int  psearch_mname();  
int  psearch_schoolnum();  
int  psearch_phone();  
int  psearch_sex();  
int  psearch_age();  
  
int  delete_pmenu_search();  
int  delete_psearch_schoolnum();  
int  delete_psearch_mname();  
int  delete_psearch_phone();  
int  delete_psearch_age();  
int  delete_psearch_sex();  
  
int  change_pmenu_search();  
int  change_psearch_schoolnum();  
int  change_psearch_mname();  
int  change_psearch_phone();  
int  change_psearch_age();  
int  change_psearch_sex();  
  
int  psort_menu();  
void psort_age();  
void psort_schoolnum();  
  
void save_order_tea();  
void save();  
void save_order_stu();  
  
typedef struct {  
        char mname[9];  
        char schoolnum[15];  
        char sex[3];  
        int  age;  
        char address[20];  
        char phone[15];       
    }Node;  
      
typedef struct student  
{  
    Node information;   
    struct student *next;  
    struct student *last;  
}STUDENT;  
  
  
  
  
  
  
  
/*******************md5*********************/  
/*********************************** 
 * 非线性函数 
 * (&是与,|是或,~是非,^是异或)  
 *  
 * 这些函数是这样设计的： 
 *   如果X、Y和Z的对应位是独立和均匀的， 
 *   那么结果的每一位也应是独立和均匀的。  
 *  
 * 函数F是按逐位方式操作：如果X，那么Y，否则Z。 
 * 函数H是逐位奇偶操作符 
 **********************************/  
#define F(x,y,z) ((x & y) | (~x & z))    
#define G(x,y,z) ((x & z) | (y & ~z))    
#define H(x,y,z) (x^y^z)    
#define I(x,y,z) (y ^ (x | ~z))    
  
  
/************************************** 
 *向右环移n个单位 
 * ************************************/  
#define ROTATE_LEFT(x,n) ((x << n) | (x >> (32-n)))    
  
  
  
  
/**************************************************** 
 * 每次操作对a，b，c和d中的其中三个作一次非线性函数运算 
 *  F(b,c,d)   G(b,c,d)   H(b,c,d)   I(b,c,d) 
 * 
 * 然后将所得结果加上 第四个变量(a)， 
 * F(b,c,d)+a 
 * 
 * 文本的一个子分组(x) 
 * F(b,c,d)+a+x 
 *  
 * 和一个常数(ac)。 
 * F(b,c,d)+a+x+ac 
 * 
 * 再将所得结果向右环移一个不定的数(s)， 
 * ROTATE_LEFT( F(b,c,d)+a+x+ac , s ) 
 *  
 * 并加上a，b，c或d中之一(b)。 
 * ROTATE_LEFT( F(b,c,d)+a+x+ac , s )+b 
 *  
 * 最后用该结果取代a，b，c或d中之一(a)。 
 * a=ROTATE_LEFT( F(b,c,d)+a+x+ac , s )+b 
 *  
 * ***************************************************/  
  
  
#define FF(a,b,c,d,x,s,ac) { a += F(b,c,d) + x + ac;  a = ROTATE_LEFT(a,s); a += b; }  
#define GG(a,b,c,d,x,s,ac) { a += G(b,c,d) + x + ac;  a = ROTATE_LEFT(a,s); a += b; }  
#define HH(a,b,c,d,x,s,ac) { a += H(b,c,d) + x + ac;  a = ROTATE_LEFT(a,s); a += b; }  
#define II(a,b,c,d,x,s,ac) { a += I(b,c,d) + x + ac;  a = ROTATE_LEFT(a,s); a += b; }  
  
  
  
  
  
//储存一个MD5 text信息   
typedef struct    
{    
    unsigned int count[2];      
    //记录当前状态，其数据位数     
      
    unsigned int state[4];      
    //4个数，一共32位 记录用于保存对512bits信息加密的中间结果或者最终结果    
      
    unsigned char buffer[64];  
    //一共64字节，512位        
}MD5_CTX;    
    
  
  
  
  
//函数声明区，每个函数在下面都有较详细说明，这里不再赘述  
  
void MD5Init(MD5_CTX *context);    
  
void MD5Update(MD5_CTX *context,unsigned char *input,unsigned int inputlen);    
  
void MD5Final(MD5_CTX *context,unsigned char digest[16]);    
  
void MD5Transform(unsigned int state[4],unsigned char block[64]);    
  
void MD5Encode(unsigned char *output,unsigned int *input,unsigned int len);    
  
void MD5Decode(unsigned int *output,unsigned char *input,unsigned int len);    
    
char * MD5(char *encrypt);  
  
  
  
  
  
  
  
  
  
  
typedef struct  
{  
    char username[100];  
    char password[100];  
}Node_order_tea;  
  
  
typedef struct order_tea  
{   
    Node_order_tea information;  
    struct order_tea *next;  
    struct order_tea *last;  
}ORDER_TEA;  
  
typedef struct  
{  
    char username[100];  
    char password[100];  
}Node_order_stu;  
  
  
typedef struct order_stu  
{   
    Node_order_stu information;  
    struct order_stu *next;  
    struct order_stu *last;  
}ORDER_STU;  
  
  
  
  
  
  
  
//全局变量声明    
extern int num;                   //文件中总的学生信息个数   
extern STUDENT *head,*iend,*inew;  
extern FILE *fp;                    //文件指针   
  
extern int num_order_tea;                   //文件中总的学生信息个数   
extern ORDER_TEA *head_order_tea,*iend_order_tea,*inew_order_tea;  
extern FILE *fp_order_tea;   
   
extern int num_order_stu;                   //文件中总的学生信息个数   
extern ORDER_STU *head_order_stu,*iend_order_stu,*inew_order_stu;  
extern FILE *fp_order_stu;   
  
extern unsigned char PADDING[]={0x80,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,    
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};    
  
void inputone(STUDENT *temp);  
STUDENT *pinput();  
int  change_one(STUDENT *pTemp);  
void displayone(STUDENT *temp);  
   
  
ORDER_TEA *change_add_order_tea();  
ORDER_STU *change_add_order_stu();
```
注：登陆密码为：1234567
具体文件在我上传的资源中，名字为信息管理系统
