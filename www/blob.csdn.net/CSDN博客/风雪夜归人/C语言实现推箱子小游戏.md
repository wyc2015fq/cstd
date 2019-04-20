# C语言实现推箱子小游戏 - 风雪夜归人 - CSDN博客
2016年04月11日 22:38:06[cdjccio](https://me.csdn.net/qq_34624951)阅读数：3600
```cpp
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#define N 1000
/*
r目的地 
o箱子 
i人 
x墙 
 路（空格） 
w上 
a左 
d右 
s下 
*/
void menu();
int level1();
int level2();
void swap(char*,char*);
int opera(char *p,int *ren,int xzs);
void dituprint();
void menu()
{
     printf("推箱子\n");
     printf("1.新游戏          2.选关\n");
     printf("0.退出\n");
}
     
void xuanguan()
{
     int choice_level;
     while(1)
     {
     printf("请选择关卡：\n");
     scanf("%d",&choice_level);
     switch(choice_level)
     {
      case 1:
           level1();
           break;
      case 2:
           level2();
           break;
      default:
              printf("输入错误，请重新选择关卡！"); 
      
     }
     }
}
     
     void swap(char *a,char *b)
     {
         
          char ch;
          ch=*a;
          *a=*b;
          *b=ch;
          }
          
     void dituprint(char *p)
     {
        
          for(int i=0;i<9;i++)
          {
                  for(int j=0;j<9;j++)
                  printf("%3c",p[i*9+j]);
                  printf("\n");
                  }
          }
     int opera(char *p,int *ren,int xzs)
     {
         dituprint(p);
         while(1)
         {
           for(int i=1,panduan=0;i<=xzs;i++)
             {
                if(p[ren[i]]=='o')
                   {
                     panduan+=1;
                     printf("panduan=%d\n",panduan);
                   }
                if(panduan==xzs)
                   {
                    printf("恭喜你，过关啦！");
                    getchar();
                    return 0; 
                    }
              }
         char ch;
         printf("你的按键是：");
         scanf("%c",&ch);
         switch(ch)
         {
           case 'w':
              {
                if(p[ren[0]]=='r')
                  {
                    if(p[ren[0]-9]=='r')
                      {
                        ren[0]-=9;
                        p[ren[0]='i'];
                        p[ren[0]+9]='r';
                        break;
                      }
                    if(p[ren[0]-9]=='x')
                      {
                        printf("人碰到墙了  换按其他键1哇>>>>>");
                        break;
                      }
                    if(p[ren[0]-9]==' ')
                      {
                        ren[0]-=9;
                        p[ren[0]='i'];
                        p[ren[0]-9]='r';
                        break;
                       }
                     if(p[ren[0]-9]=='o')
                       {
                         if(p[ren[0]-18]=='x'||'o')
                            printf("箱子碰到了   换按其他键1哇>>>>>");break;
                              if(p[ren[0]-18]=='r')
                                 {
                                    p[ren[0]-18]='o';
                                    p[ren[0]-9]='i';
                                    p[ren[0]]='r';
                                    ren[0]-=9;
                                    break;                                                                                  
                                  }
                               if(p[ren[0]-18]=='o')
                                  printf("推不动诶   换按其他键1哇>>>>>");break;
                               if(p[ren[0]-18]==' ')
                                  {
                                    swap(&p[ren[0]-9],&p[ren[0]-18]);
                                    swap(&p[ren[0]],&p[ren[0]-9]);
                                    p[ren[0]]='r';
                                    ren[0]-=9;
                                    break;
                                  }                    
                        }                    
                    }
                        else
                          {
                            if(p[ren[0]-9]=='x')
                              {
                                 printf("人碰到墙了  换按其他键2>>>>>");
                                 break;
                              }
                            if(p[ren[0]-9]==' ')
                              {
                                 swap(&p[ren[0]],&p[ren[0]-9]);
                                 ren[0]-=9;
                                 break;                                                               
                              }
                             if(p[ren[0]-9]=='o')
                              {
                                 if(p[ren[0]-18]=='o'||p[ren[0]-18]=='x')
                                    {
                                      printf("走不动了  换按其他键哇。。。。\n");
                                      break;
                                    }
                                 if(p[ren[0]-18]=='r')
                                    {
                                       p[ren[0]-18]='o';
                                       p[ren[0]-9]='i';
                                       p[ren[0]]=' ';
                                       ren[0]-=9;
                                       break;
                                     }
                               }                    
                         }
                    }
                 break;
                case 'a':
                        {
                          if(p[ren[0]]=='r')
                          {
                          if(p[ren[0]-1]=='r')
                          {
                                              ren[0]-=1;
                                              p[ren[0]]='i';
                                              p[ren[0]+1]='r';
                                              break;
                                              }
                          if(p[ren[0]-1]=='x')
                          {
                                              printf("人碰到墙了   换按其他键1哇>>>>>");
                                              break;
                                              }
                          if(p[ren[0]-1]==' ')
                          {
                                              ren[0]-=1;
                                              p[ren[0]]='i';
                                              p[ren[0]+9]='r';
                                              break;
                                              }
                          if(p[ren[0]-1]=='o')
                          {
                                              if(p[ren[0]-2]=='o'||p[ren[0]-2]=='x')
                                              {
                                                 printf("走不动了  换按其他键娃娃。。。。。\n");
                                                 break;
                                              }
                                              if(p[ren[0]-2]=='r')
                                              {
                                                                  p[ren[0]-2]='o';
                                                                  p[ren[0]-1]='i';
                                                                  p[ren[0]]=' ';
                                                                  ren[0]-=1;
                                                                  break;
                                                                  }
                                              if(p[ren[0]-2]==' ')
                                              {
                                                                p[ren[0]-2]='o';
                                                                p[ren[0]-1]='i';
                                                                p[ren[0]]=' ';
                                                                ren[0]-=1;
                                                                break;
                                                                }
                                              } 
                                            }  
                            }
                            break;
                          case 's':
                               {
                                   if(p[ren[0]]=='r')
                                   {
                                               if(p[ren[0]+9]=='r')
                                               {
                                                                   ren[0]+=9;
                                                                   p[ren[0]]='i';
                                                                   p[ren[0]-9]='r';
                                                                   break;
                                                                   }
                                               if(p[ren[0]+9]=='x')
                                               {
                                                                   printf("人碰到墙了  换按其他键1哇>>>>>");
                                                                   break;
                                                                   }
                                               if(p[ren[0]+9]==' ')
                                               {
                                                                 ren[0]+=9;
                                                                 p[ren[0]]='i';
                                                                 p[ren[0]+9]='r';
                                                                 break;
                                                                 }
                                               if(p[ren[0]+9]=='o')
                                               {
                                                                   if(p[ren[0]+18]=='x'||'o')
                                                                   {
                                                                       printf("箱子碰到墙了  换按其他键1哇>>>>>");break;
                                                                   if(p[ren[0]+18]=='r')
                                                                   {
                                                                       p[ren[0]+18]='o';
                                                                       p[ren[0]+9]='i';
                                                                       p[ren[0]]='r';
                                                                       ren[0]+=9;
                                                                       break; 
                                                                       }
                                                                   if(p[ren[0]+18]=='o')
                                                                   printf("推不动哎   换按其他键1哇>>>>>");break;
                                                                   if(p[ren[0]+18]==' ')
                                                                   {
                                                                       swap(&p[ren[0]+9],&p[ren[0]+18]);
                                                                       swap(&p[ren[0]],&p[ren[0]-9]);
                                                                       p[ren[0]]='r';
                                                                       ren[0]+=9;
                                                                       break;
                                                                   }
                                                                   }
                                                                   }
                                               else
                                               {
                                                   if(p[ren[0]+9]=='x')
                                                   {
                                                                       printf("人碰到墙了，换按其他键哇2>>>>>");
                                                                       break;
                                                                       }
                                                   if(p[ren[0]+9]==' ')
                                                   {
                                                                       swap(&p[ren[0]],&p[ren[0]+9]);
                                                                       ren[0]+=9;
                                                                       break;
                                                                     }
                                                   if(p[ren[0]+9]=='o')
                                                   {
                                                                       if(p[ren[0]+18]=='o'||p[ren[0]+18]=='x')
                                                                       {
                                                                            printf("走不动了，换按其他键哇。。。。。\n");
                                                                            break;                                   
                                                                       }
                                                                       if(p[ren[0]+18]=='r')
                                                                       {
                                                                            p[ren[0]+18]='o';
                                                                            p[ren[0]+9]='i';
                                                                            p[ren[0]]=' ';
                                                                            ren[0]+=9;
                                                                            break;      
                                                                       }
                                                                       if(p[ren[0]+18]==' ')
                                                                       {
                                                                            p[ren[0]+18]='o';
                                                                            p[ren[0]+9]='i';
                                                                            p[ren[0]]=' ';
                                                                            ren[0]+=9;
                                                                            break;      
                                                                       }
                                                                       }
                                                   }
                                                     }
                                                     break;
                                                     case'd':
                                                             {
                                                                       if(p[ren[0]]=='r')
                                                                       {
                                                                            if(p[ren[0]+1]=='r')
                                                                            {
                                                                                 ren[0]+=1;
                                                                                 p[ren[0]]='i';
                                                                                 p[ren[0]-1]='r';
                                                                                 break;
                                                                            }
                                                                            if(p[ren[0]+1]=='x')
                                                                            {
                                                                                 printf("人碰到墙了  换按其他键1哇>>>>>");
                                                                                 break;
                                                                            }
                                                                            if(p[ren[0]+1]==' ')
                                                                            {
                                                                                 ren[0]+=1;
                                                                                 p[ren[0]]='i';
                                                                                 p[ren[0]-1]='r';
                                                                                 break;
                                                                            }
                                                                            if(p[ren[0]+1]=='o')
                                                                            {
                                                                                 if(p[ren[0]+2]=='x'||'o')
                                                                                 printf("箱子碰到墙了  换按其他键1哇>>>>>>");
                                                                                 break;
                                                                                 if(p[ren[0]+2]=='r')
                                                                                 {
                                                                                 p[ren[0]+2]='o';
                                                                                 p[ren[0]+1]='i';
                                                                                 p[ren[0]]='r';
                                                                                 p[0]+=1;
                                                                                 break;
                                                                                 }
                                                                                 if(p[ren[0]+2]=='o')
                                                                                 printf("推不动诶  换按其他键1哇>>>>>");break;
                                                                                 if(p[ren[0]+2]==' ')
                                                                                 {
                                                                                 swap(&p[ren[0]+1],&p[ren[0]+2]);
                                                                                 swap(&p[ren[0]],&p[ren[0]-1]);
                                                                                 p[ren[0]]='r';
                                                                                 ren[0]+=1;
                                                                                 break; 
                                                                                 }  
                                                                            }
                                                                       }
                                                                       else
                                                                       {
                                                                           if(p[ren[0]+1]=='x')
                                                                           {
                                                                                 printf("人碰到墙了，换按其他键哇2>>>>>");
                                                                                 break;
                                                                           }
                                                                           if(p[ren[0]+1]==' ')
                                                                           {
                                                                                 swap(&p[ren[0]],&p[ren[0]+1]);
                                                                                 ren[0]+=1;
                                                                                 break;
                                                                           }
                                                                           if(p[ren[0]+1]=='o')
                                                                           {
                                                                              if(p[ren[0]+2]=='o'||p[ren[0]+2]=='x')
                                                                              {
                                                                                  printf("走不动了！换按其他键哇。。。。。");
                                                                                  break; 
                                                                              }
                                                                              if(p[ren[0]+2]=='r')
                                                                              {
                                                                                  p[ren[0]+2]='o';
                                                                                  p[ren[0]+1]='i';
                                                                                  p[ren[0]]=' ';
                                                                                  ren[0]+=1;
                                                                                  break; 
                                                                              }
                                                                              if(p[ren[0]+2]==' ')
                                                                              {
                                                                                  p[ren[0]+2]='o';
                                                                                  p[ren[0]+1]='i';
                                                                                  p[ren[0]]=' ';
                                                                                  ren[0]+=1;
                                                                                  break; 
                                                                              }
                                                                                               }
                                                                           }
                                                             }
                                                             break;
                                                             default:printf("操作错误！请确认后再重新操作。。。。。"); 
                                   }
                                   dituprint(p);
                   } 
                   /*int panduan=0;
                   for(int i=1;i<xzs;i++)
                   {
                           if(p[ren[0]]=='o')panduan++;
                   }
                   if(panduan=(xzs-1)){getchar();break;}*/
                 }
                 return 0;
         }
         
         int level1()
         {
             printf("第一关\n");
             char ditu1[9*9]={
             '0','1','2','3','4','5','6','7','8',
             '1',' ',' ','x','x','x',' ',' ',' ',
             '2',' ',' ','x','r','x',' ',' ',' ',
             '3',' ',' ','x',' ','x','x','x','x',
             '4','x','x','x','o','x','o','r','x',
             '5','x','r',' ','o','i','x','x','x',
             '6','x','x','x','x','o','x',' ',' ',
             '7',' ',' ',' ','x','r','x',' ',' ',
             '8',' ',' ',' ','x','x','x',' ',' ', 
                             };
              int ren1[5]={50,22,47,43,68};
              opera(ditu1,ren1,4);
              level2();
              return 0;
         }
             
         int level2()
         {
             printf("欢迎来到第二关\n");
             char ditu1[9*9]={
             '0','1','2','3','4','5','6','7','8',
             '1',' ',' ','x','x','x',' ',' ',' ',
             '2',' ',' ','x','r','x',' ',' ',' ',
             '3',' ',' ','x',' ','x','x','x','x',
             '4','x','x','x','o','x','o','r','x',
             '5','x','r',' ','o','i','x','x','x',
             '6','x','x','x','x','o','x',' ',' ',
             '7',' ',' ',' ','x','r','x',' ',' ',
             '8',' ',' ',' ','x','x','x',' ',' ', 
                              };
              int  ren1[5]={50,22,47,43,68};
              opera(ditu1,ren1,4);
              level1();
              return 0;   
         }
          
int main()
          {
              int choice;
              menu();
              while(1)
              {
                      scanf("%d",&choice);
                      switch(choice)
                      {
                      case 0:
                           exit(0);
                      case 1:
                           {
                           level1();
                           break;
                           }
                      case 2:
                           {
                           xuanguan();
                           break;
                           }
                           default:
                           {
                           printf("操作错误！请确认正确后操作。。。。。\n");
                           } 
                      }
              }
                      getchar();
                      return 0;
          }
```
