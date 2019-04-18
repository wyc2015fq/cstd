# 学生信息管理系统（C实现） - Soul Joy Hub - CSDN博客

2016年06月13日 09:56:01[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：8214


http://blog.csdn.net/u011239443/article/details/51655217

![](http://images.cnitblog.com/blog/709693/201501/212318590638696.png)![](http://images.cnitblog.com/blog/709693/201501/212319411721428.png)

![](http://images.cnitblog.com/blog/709693/201501/212320157036909.png)

头文件

```
![](http://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)

  1 #ifndef __SIMS__
  2 #define __SIMS__
  3 #include<stdio.h>
  4 #include<stdlib.h>
  5 #include<string.h>
  6 #include<ctype.h>
  7 #include<conio.h> 
  8 #include<Windows.h>
  9 #define PWD_LEN 10
 10 #define NAME_LEN 20
 11 #define GENDER_LEN 5
 12 #define LINE_LEN 512
 13 #define CONFIRM_CNT 3
 14 #define M1 "1.search stu"
 15 #define M2 "2.update stu"
 16 #define M3 "3.delete stu"
 17 #define M4 "4.add stu"
 18 #define M5 "5.search acc"
 19 #define M6 "6.update acc"
 20 #define M7 "7.delete acc"
 21 #define M8 "8.add acc"
 22 #define M9 "9.exit"
 23 
 24 
 25 struct stu_accout
 26 {
 27     char passwd[PWD_LEN];
 28     char name[NAME_LEN];
 29     int  role ;
 30     stu_accout* next ;
 31 };
 32 
 33 struct student            //定义一个结构体作为全局变量，用于存储学生信息，和作为结点
 34 {
 35     long int num;            //学号
 36     char name[NAME_LEN];            //姓名
 37     char gender [GENDER_LEN];
 38     int age;                //年龄
 39     float math;                //数学
 40     float phy;                    
 41     float eng;                
 42     float cpu;            
 43     float sum;                
 44     student *next;    //下一个结点地址
 45 };
 46 
 47 
 48 //登入函数
 49 int login(int,int&,stu_accout*);
 50 
 51 
 52 //主目录函数
 53 int menu(int role);
 54 
 55 //3.查询信息菜单
 56  student *menu1(student*);
 57 
 58 
 59 //7.排序信息菜单
 60 student *menu7(student*);
 61 
 62 
 63 //交换两个结点信息的函数，用于排序函数调用
 64 void swap(student *, student *);                    
 65 
 66 
 67 //输出信息的函数
 68 void print(student *);
 69 void Account_print(stu_accout *head);
 70 
 71 //添加学生信息 
 72  student *add(student *, student *);
 73 
 74 //读取学生文件
 75 student *Student_File_Load(student* head);
 76 student *Other_Student_File_Load( student *head);
 77 
 78 //读取账户文件
 79 stu_accout *Acount_Load(stu_accout* head);        //定义函数
 80 
 81 int Save_Stu(student *p1);
 82 int Other_Save_Stu(student *p1);
 83 int Save_Account(student *p1);
 84 
 85 //查询模块
 86 //1.按学号查询
 87  student *search1(student *head);
 88 
 89 //2.按姓名查询
 90  student *search2(student *head);
 91 
 92 student *search_By_Gender(student *head);
 93 
 94 //3.按年龄查询
 95  student *search3(student *head);
 96 
 97 //4.按数学成绩查询
 98  student *search4(student *head);
 99 
100 //5.按物理成绩查询
101  student *search5(student *head);
102 
103 //6.按英语成绩查询
104  student *search6(student *head);
105 
106 //7.按计算机成绩查询
107  student *search7(student *head);
108 
109 //8.按总分成绩查询
110  student *search8(student *head);
111 
112 
113 //删除模块
114  student *del(student *head);
115 
116  //修改
117  student *cha(student *head);
118  stu_accout *Password_cha(stu_accout *head);
119 
120 //排序模块
121 //1.按学号排序
122  student *sort1(student *head);
123 
124 //2.按年龄排序
125  student *sort2(student *head);
126 
127 //3.按数学成绩排序
128  student *sort3(student *head);
129 
130 //4.按物理成绩排序
131  student *sort4(student *head);
132 
133 //5.按英语成绩排序
134  student *sort5(student *head);
135 
136 //6.按计算机成绩排序
137  student *sort6(student *head);
138 
139 //7.按总分成绩排序
140  student *sort7(student *head);
141 
142 #endif

View Code
```

实现文件

```
![](http://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)

   1 #include "My_header.h"
   2 
   3 int n;                        //定义一个整形变量计数学生人数
   4 int x;                      //定义一个整形变量计数账户人数
   5 char UserName[NAME_LEN];
   6 
   7 
   8 //***************************************************读取文件模块
   9 
  10 //读取学生文件
  11 student *Student_File_Load(student* head)        //定义函数
  12 {
  13     student *p1, *p2 ;
  14     int m=0;
  15     FILE *fp;
  16 
  17     if((fp=fopen("F:\\Student.txt", "rb"))==NULL)        //地址为空，则输出提示
  18     {
  19         printf("不能打开文件");
  20         return 0;
  21     }
  22     head=NULL;
  23     do
  24     {
  25         p1=(student *)malloc(sizeof(student));//每读取一次创建一个新的结点
  26         fscanf(fp, "%ld\t%s\t%s\t%d\t%f\t%f\t%f\t%f\t%f\n", &p1->num, p1->name,p1->gender, &p1->age, &p1->math, &p1->phy, &p1->eng, &p1->cpu, &p1->sum);   //读入内存
  27         m=m+1;
  28         if(m==1)
  29             head=p2=p1;        //判断结点个数以添加首地址
  30         else
  31         {
  32             p2->next=p1;
  33             p2=p1;
  34         }
  35     }while(!feof(fp));
  36 
  37     p2->next=NULL;
  38     fclose(fp);
  39     n=m;
  40     return(head);        //返回读取信息的首地址供其他函数使用
  41 }
  42 
  43 //另外读取学生文件
  44 student *Other_Student_File_Load( student *head)        //定义函数
  45 {
  46     struct student *p1, *p2;
  47     int m=0;
  48     char fileopen[10];
  49     FILE *fp;
  50     printf("请输入文件路径及文件名:");            //输入读取文件的路径和文件名
  51     scanf("%s", fileopen);
  52     if((fp=fopen(fileopen, "rb"))==NULL)        //地址为空，则输出提示
  53     {
  54         printf("不能打开文件");
  55         return 0;
  56     }
  57     head=NULL;
  58     do
  59     {
  60         p1=(struct student *)malloc(sizeof(struct student));//每读取一次创建一个新的结点
  61         fscanf(fp, "%ld\t%s\t%s\t%d\t%f\t%f\t%f\t%f\t%f\n", &p1->num, p1->name,p1->gender, &p1->age, &p1->math, &p1->phy, &p1->eng, &p1->cpu, &p1->sum);   //读入内存
  62         m=m+1;
  63         if(m==1)
  64             head=p2=p1;        //判断结点个数以添加首地址
  65         else
  66         {
  67             p2->next=p1;
  68             p2=p1;
  69         }
  70     }while(!feof(fp));
  71     p2->next=NULL;
  72     fclose(fp);
  73     n=m;
  74     return(head);        //返回读取信息的首地址供其他函数使用
  75 }
  76 
  77 //读取账户文件
  78 stu_accout *Acount_Load(stu_accout* head)        //定义函数
  79 {
  80     stu_accout *p1, *p2 ;
  81     int m=0;
  82     FILE *fp;
  83 
  84     if((fp=fopen("F:\\Account.txt", "rb"))==NULL)        //地址为空，则输出提示
  85     {
  86         printf("不能打开文件");
  87         return 0;
  88     }
  89     head=NULL;
  90     do
  91     {
  92         p1=(stu_accout *)malloc(sizeof(stu_accout));//每读取一次创建一个新的结点
  93         fscanf(fp, "%s\t%s\t%d\n", p1->name, p1->passwd,&p1->role);   //读入内存
  94         m=m+1;
  95         if(m==1)
  96             head=p2=p1;        //判断结点个数以添加首地址
  97         else
  98         {
  99             p2->next=p1;
 100             p2=p1;
 101         }
 102     }while(!feof(fp));
 103     x=m;
 104     p2->next=NULL;
 105     fclose(fp);
 106     return(head);        //返回读取信息的首地址供其他函数使用
 107 }
 108 
 109 
 110 
 111 //******************************************************读取文件模块
 112 
 113 
 114 
 115 
 116 //******************************************************保存文件模块
 117 //存 学生
 118 int Save_Stu(student *p1)            //定义保存函数
 119 {
 120     FILE *fp;                            //定义fp指针
 121     if((fp=fopen("F:\\Student.txt", "wb"))==NULL)
 122     {
 123         printf("不能打开文件\n");
 124         return 0;
 125     }
 126     while(p1!=NULL)
 127     {
 128         fprintf(fp, "%ld\t%s\t%s\t%d\t%f\t%f\t%f\t%f\t%f\n",p1->num, p1->name,p1->gender, p1->age, p1->math, p1->phy, p1->eng, p1->cpu, p1->sum);//把内存中的文件输入到硬盘中
 129         p1=p1->next;
 130     }
 131     fclose(fp);                //关闭文件
 132     printf("文件已保存\n"); //成功保存，显示提示
 133     return 1;
 134 }
 135 
 136 
 137 //另存为 学生
 138 int Other_Save_Stu(student *p1)            //定义保存函数
 139 {
 140     FILE *fp;                            //定义fp指针
 141     char fileopen[20];
 142     printf("请输入保存的文件路径及文件名:");    //输入文件名和路径
 143     scanf("%s", fileopen);
 144     if((fp=fopen(fileopen, "wb"))==NULL)
 145     {
 146         printf("不能打开文件\n");
 147         return 0;
 148     }
 149     while(p1!=NULL)
 150     {
 151         fprintf(fp, "%ld\t%s\t%s\t%d\t%f\t%f\t%f\t%f\t%f\n",p1->num, p1->name,p1->gender, p1->age, p1->math, p1->phy, p1->eng, p1->cpu, p1->sum);//把内存中的文件输入到硬盘中
 152         p1=p1->next;
 153     }
 154     fclose(fp);                //关闭文件
 155     printf("文件已保存\n"); //成功保存，显示提示
 156     return 1;
 157 }
 158 
 159 
 160 //保存账户
 161 int Save_Account(stu_accout *p1)            //定义保存函数
 162 {
 163     FILE *fp;                            //定义fp指针
 164     if((fp=fopen("F:\\Account.txt", "wb"))==NULL)
 165     {
 166         printf("不能打开文件\n");
 167         return 0;
 168     }
 169     while(p1!=NULL)
 170     {
 171         fprintf(fp, "%s\t%s\t%d\n",p1->name, p1->passwd, p1->role);//把内存中的文件输入到硬盘中
 172         p1=p1->next;
 173     }
 174     fclose(fp);                //关闭文件
 175     printf("文件已保存\n"); //成功保存，显示提示
 176     return 1;
 177 }
 178 
 179 
 180 
 181 //******************************************************保存文件模块
 182 
 183 
 184 
 185 
 186 
 187 
 188 
 189 
 190 //***************************************************登入模块
 191 //登入函数
 192 int login(int Login_Change,int& type,stu_accout* Account)
 193 {
 194 
 195     system("cls");
 196 
 197     stu_accout *p=Account;
 198     printf("\n\n\n");
 199     printf("请输入用户名：");
 200 
 201     scanf("%s",UserName);
 202 
 203     while(p!=NULL)
 204     {
 205         if(strcmp(p->name,UserName)==0)
 206             break;
 207         p=p->next;
 208     }
 209 
 210 
 211     if(p==NULL) 
 212     {
 213         printf("无该用户名！！\n");
 214         printf("您还有%d次机会\n",Login_Change);
 215         getchar();
 216         getchar();
 217         return 0;
 218     }
 219     else
 220     {
 221         printf("请输入密码：");
 222         char c=0;   
 223         int i=0;
 224         char password[PWD_LEN];
 225         while(i<PWD_LEN-1&&c!='\r')   
 226         {       c=getch();   
 227 
 228         if(c!='\r')
 229         {
 230             putchar('*');
 231             password[i]=c;
 232             ++i;   
 233         }
 234         }
 235         if(i==PWD_LEN-1&&c!='\n') 
 236         {
 237             printf("\n密码错误！！\n");
 238             printf("您还有%d次机会\n",Login_Change);
 239             getchar();
 240             getchar();
 241             return 0;
 242         }
 243         password[i]='\0';
 244         if(strcmp(password,p->passwd)!=0) 
 245         {
 246             printf("\n密码错误！！\n");
 247             printf("您还有%d次机会\n",Login_Change);
 248             getchar();
 249             getchar();
 250             return 0;
 251         }
 252     }
 253     system("cls");
 254     printf("登入成功\n");
 255     type=p->role;
 256     int tem=2;
 257     while(tem--)
 258     {
 259         system("cls");
 260         
 261 printf("                        i8S,         \n");
 262 printf("            S&X         5#@9         \n");
 263 printf("           :@@B;        .:53sr:      \n");
 264 printf("         :8BMh            i&&XG91.   \n");
 265 printf("        ;M@#; ;;,    13S5,.hGHAHMHs  \n");
 266 printf("        X@@r.G3HA9. S#Xh3Mh,:S#@@@@s \n");
 267 printf("        H@A.&MsSG@,.rH&XB@@M1:S@@@@#.\n");
 268 printf("        X@X8@@@@@1 . ,3H#@@M1,;@@@@@s\n");
 269 printf("        s@@AHMBGr     .;iri, ,8@@@@@S\n");
 270 printf("         A@#Gh;,.,:.;:.,.,:s8M@@@@@@1\n");
 271 printf("         :#@@@B&85SS959XAB@@@@@@@@@@r\n");
 272 printf("          rsiiirri:iih9XAM@@@@@@@@@@;\n");
 273 printf("      ,hs:,....           ,rSXM@@@@# \n");
 274 printf("     ;AHr;;:::,,,,,,,, ,iri:..;h&@@3 \n");
 275 printf("  .,.3@Miii;;;::::::,,9BBHA&Gh:,;SG  \n");
 276 printf(" 1&&&A@@9,iiiiii;;;;,3@@@@@@@@9GAX8i \n");
 277 printf(".GB95SGM#8;;iiiiiiii;9@@@@@@@@@GSSHA \n");
 278 printf(" :993398&H,.,::;;;;;;;8#@@@@@#X533Xs \n");
 279 printf("      ...              ,;;,:GXS5s:   \n");
 280         printf("载入中\n");
 281         Sleep(500);
 282         system("cls");
 283 printf("                          i8S,         \n");
 284 printf("              S&X         5#@9         \n");
 285 printf("             :@@B;        .:53sr:      \n");
 286 printf("           :8BMh            i&&XG91.   \n");
 287 printf("          ;M@#; ;;,    13S5,.hGHAHMHs  \n");
 288 printf("          X@@r.G3HA9. S#Xh3Mh,:S#@@@@s \n");
 289 printf("          H@A.&MsSG@,.rH&XB@@M1:S@@@@#.\n");
 290 printf("          X@X8@@@@@1 . ,3H#@@M1,;@@@@@s\n");
 291 printf("          s@@AHMBGr     .;iri, ,8@@@@@S\n");
 292 printf("           A@#Gh;,.,:.;:.,.,:s8M@@@@@@1\n");
 293 printf("           :#@@@B&85SS959XAB@@@@@@@@@@r\n");
 294 printf("            rsiiirri:iih9XAM@@@@@@@@@@;\n");
 295 printf("        ,hs:,....           ,rSXM@@@@# \n");
 296 printf("       ;AHr;;:::,,,,,,,, ,iri:..;h&@@3 \n");
 297 printf("    .,.3@Miii;;;::::::,,9BBHA&Gh:,;SG  \n");
 298 printf("   1&&&A@@9,iiiiii;;;;,3@@@@@@@@9GAX8i \n");
 299 printf("  .GB95SGM#8;;iiiiiiii;9@@@@@@@@@GSSHA \n");
 300 printf("   :993398&H,.,::;;;;;;;8#@@@@@#X533Xs \n");
 301 printf("        ...              ,;;,:GXS5s:   \n");
 302         printf("载入中…\n");
 303         Sleep(500);
 304         system("cls");
 305 printf("                        i8S,         \n");
 306 printf("            S&X         5#@9         \n");
 307 printf("           :@@B;        .:53sr:      \n");
 308 printf("         :8BMh            i&&XG91.   \n");
 309 printf("        ;M@#; ;;,    13S5,.hGHAHMHs  \n");
 310 printf("        X@@r.G3HA9. S#Xh3Mh,:S#@@@@s \n");
 311 printf("        H@A.&MsSG@,.rH&XB@@M1:S@@@@#.\n");
 312 printf("        X@X8@@@@@1 . ,3H#@@M1,;@@@@@s\n");
 313 printf("        s@@AHMBGr     .;iri, ,8@@@@@S\n");
 314 printf("         A@#Gh;,.,:.;:.,.,:s8M@@@@@@1\n");
 315 printf("         :#@@@B&85SS959XAB@@@@@@@@@@r\n");
 316 printf("          rsiiirri:iih9XAM@@@@@@@@@@;\n");
 317 printf("      ,hs:,....           ,rSXM@@@@# \n");
 318 printf("     ;AHr;;:::,,,,,,,, ,iri:..;h&@@3 \n");
 319 printf("  .,.3@Miii;;;::::::,,9BBHA&Gh:,;SG  \n");
 320 printf(" 1&&&A@@9,iiiiii;;;;,3@@@@@@@@9GAX8i \n");
 321 printf(".GB95SGM#8;;iiiiiiii;9@@@@@@@@@GSSHA \n");
 322 printf(" :993398&H,.,::;;;;;;;8#@@@@@#X533Xs \n");
 323 printf("      ...              ,;;,:GXS5s:   \n");
 324         printf("载入中……\n");
 325         Sleep(500);
 326         system("cls");
 327 printf("                          i8S,         \n");
 328 printf("              S&X         5#@9         \n");
 329 printf("             :@@B;        .:53sr:      \n");
 330 printf("           :8BMh            i&&XG91.   \n");
 331 printf("          ;M@#; ;;,    13S5,.hGHAHMHs  \n");
 332 printf("          X@@r.G3HA9. S#Xh3Mh,:S#@@@@s \n");
 333 printf("          H@A.&MsSG@,.rH&XB@@M1:S@@@@#.\n");
 334 printf("          X@X8@@@@@1 . ,3H#@@M1,;@@@@@s\n");
 335 printf("          s@@AHMBGr     .;iri, ,8@@@@@S\n");
 336 printf("           A@#Gh;,.,:.;:.,.,:s8M@@@@@@1\n");
 337 printf("           :#@@@B&85SS959XAB@@@@@@@@@@r\n");
 338 printf("            rsiiirri:iih9XAM@@@@@@@@@@;\n");
 339 printf("        ,hs:,....           ,rSXM@@@@# \n");
 340 printf("       ;AHr;;:::,,,,,,,, ,iri:..;h&@@3 \n");
 341 printf("    .,.3@Miii;;;::::::,,9BBHA&Gh:,;SG  \n");
 342 printf("   1&&&A@@9,iiiiii;;;;,3@@@@@@@@9GAX8i \n");
 343 printf("  .GB95SGM#8;;iiiiiiii;9@@@@@@@@@GSSHA \n");
 344 printf("   :993398&H,.,::;;;;;;;8#@@@@@#X533Xs \n");
 345 printf("        ...              ,;;,:GXS5s:   \n");
 346         printf("载入中………\n");
 347         }
 348     
 349 
 350     return 1;
 351 }
 352 //***************************************************登入模块
 353 
 354 
 355 
 356 
 357 
 358 
 359 
 360 
 361 
 362 //*****************************************************主目录模块
 363 //主目录函数
 364 int menu(int role)
 365 {
 366     system("cls");
 367     int k;//定义一个整形变量用于输入选择
 368 
 369 
 370 
 371     printf("\n");
 372     printf("****************************************************************\n");
 373     printf("*                                                              *\n");
 374     printf("*                   1.查询学生的信息                           *\n");
 375     printf("*                   2.阅读学生的信息                           *\n");
 376     printf("*                   3.排序学生的信息                           *\n");
 377     printf("*                   4.查询本账户的信息                         *\n");
 378     printf("*                   5.修改密码                                 *\n");
 379     printf("*                   6.退出                                     *\n");
 380     printf("*                                                              *\n");
 381     if(role)
 382     {
 383         printf("*   *****       ********      *****   *******    ***  ***      *\n");
 384         printf("*                                                              *\n");
 385         printf("*              sS8Si  r9G1  1SS89SS   S88SSS31                 *\n"); 
 386         printf("*                #G    G5      #8      MX   i83                *\n");
 387         printf("*                3@   rG       @9      #G    r@                *\n");
 388         printf("*                 #X  Gs       @8      #G iisXG                *\n");
 389         printf("*                 h@ir8        M9      #Xrs11r                 *\n");
 390         printf("*                  HA8i        #9      @3                      *\n");
 391         printf("*                   @S      1SS89SS    @3                      *\n");
 392         printf("*                                                              *\n");
 393         printf("*                   7.输入学生的信息                           *\n");
 394         printf("*                   8.删除学生的信息                           *\n");
 395         printf("*                   9.修改学生的信息                           *\n");
 396         printf("*                   10.打开新文件…                            *\n");
 397         printf("*                   11.文件另存为…                            *\n");
 398     }
 399     printf("****************************************************************\n\n");
 400     printf("我的选择:");            //在显示器上显示菜单页面
 401     scanf("%d", &k);                //输入选择
 402     getchar();
 403     return(k);                        //返回值进入switch函数判断，以调用不同功能的函数
 404 }
 405 //*****************************************************主目录模块
 406 
 407 
 408 
 409 
 410 
 411 
 412 //******************************************************查询模块
 413 
 414 //3.查询信息菜单
 415 student *menu1(student *head)
 416 {
 417     while(1)
 418     {
 419         system("cls");
 420         int k;                            //定义一个整形变量输入switch进行判断        
 421 
 422         printf("\n\n\n");
 423         printf("*****************************************************************\n");
 424         printf("*                      查询学生信息目录                         *\n");
 425         printf("*                      1.按学生学号查询                         *\n");
 426         printf("*                      2.按学生姓名查询                         *\n");
 427         printf("*                      3.按学生性别查询                         *\n");
 428         printf("*                      4.按学生年龄查询                         *\n");
 429         printf("*                      5.按数学成绩查询                         *\n");
 430         printf("*                      6.按物理成绩查询                         *\n");
 431         printf("*                      7.按英语成绩查询                         *\n");
 432         printf("*                      8.按计算机成绩查询                       *\n");
 433         printf("*                      9.按总分成绩查询                         *\n");
 434         printf("*                      10.返回主菜单                             *\n");
 435         printf("******************************************************** ********\n\n");
 436         printf("我的选择:");            //输出菜单界面
 437 
 438         scanf("%d", &k);                //输入选择
 439         switch(k)
 440         {
 441         case 1:                            //调用学号查询函数
 442             {
 443                 head=search1(head);        //返回首地址head用于其他函数的处理
 444                 break;                    //处理完毕，结束语句
 445             }
 446         case 2:                            //调用姓名查询函数
 447             {
 448                 head=search2(head);        //返回首地址head用于其他函数的处理
 449                 break;                    //处理完毕，结束语句
 450             }
 451         case 3:                            //调用年龄查询函数
 452             {
 453                 head=search_By_Gender(head);
 454                 break;
 455             }
 456         case 4:                            //调用数学成绩查询函数
 457             {
 458                 head=search3(head);        //返回首地址head用于其他函数的处理
 459                 break;                    //处理完毕，结束语句
 460 
 461             }
 462         case 5:                            //调用物理成绩查询函数
 463             {
 464                 head=search4(head);        //返回首地址head用于其他函数的处理
 465                 break;                    //处理完毕，结束语句
 466 
 467             }
 468         case 6:                            //调用英语成绩查询函数
 469             {
 470                 head=search5(head);        //返回首地址head用于其他函数的处理
 471                 break;                    //处理完毕，结束语句
 472 
 473             }
 474         case 7:                            //调用计算机成绩查询函数
 475             {
 476                 head=search6(head);
 477                 break;
 478 
 479             }
 480         case 8:                            //调用总分成绩查询函数
 481             {
 482                 head=search7(head);
 483                 break;
 484 
 485             }
 486         case 9:{  head=search8(head);
 487             break;
 488 
 489                }    
 490         case 10: break;//退出查询功能，返回主菜单
 491         default:printf("输入错误请重试\n");
 492         }
 493         if(k==10)break;
 494     }
 495     return head;
 496 }
 497 
 498 //1.按学号查询
 499 student *search1(student *head)//定义一个函数，赋给他查找链表的首地址
 500 {
 501 
 502     system("cls");
 503     int num;
 504     student *p1;
 505     printf("请输入要查询学生的学号(输入0退出):");
 506     scanf("%ld",&num);                         //输入查找学号
 507     while(num!=0)
 508     {
 509         system("cls");
 510         if(head==NULL)
 511         {
 512             printf("\n没有任何学生的资料，请输入\n");
 513             return(head);
 514         }
 515         printf("******************************************************************\n");
 516         printf("学号\t姓名\t性别\t年龄\t数学\t物理\t英语\t计算机\t总分\n");
 517         printf("__________________________________________________________________\n");
 518         p1=head;
 519         while (num!=p1->num&&p1->next!=NULL)     //开始扫描每个结点以查找与要求吻合的结点
 520             p1=p1->next;//如果不相同就扫描下一个
 521         if(num==p1->num)//查找到相同的信息开始输出
 522         {
 523             printf("%ld\t%s\t%s\t%d\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n", p1->num, p1->name,p1->gender, p1->age,p1->math, p1->phy,p1->eng, p1->cpu, p1->sum);
 524         }
 525         else printf("不存在学号为%ld的学生\n", num); //没有查找到满足要求的结点
 526         printf("输入要查询学生的学号(输入0退出):");
 527         scanf("%ld", &num);
 528     }
 529     return(head);
 530 }
 531 
 532 //2.按姓名查询
 533 student *search2(student *head)  //定义一个函数，赋给他查找链表的首地址
 534 {
 535     system("cls");
 536     char name[NAME_LEN];
 537     student *p1;
 538     printf("请输入要查询学生的姓名:");    //输入查找姓名    
 539     scanf("%s",&name);
 540     printf("******************************************************************\n");
 541     printf("学号\t姓名\t性别\t年龄\t数学\t物理\t英语\t计算机\t总分\n");
 542     printf("__________________________________________________________________\n");
 543     p1=head;
 544     while (strcmp(name, p1->name)!=0&&p1->next!=NULL) //开始扫描每个结点以查找与要求吻合的结点
 545         p1=p1->next;                              //如果不相同就扫描下一个
 546     if(strcmp(name, p1->name)==0)            //查找到相同的信息开始输出
 547     {
 548         printf("%ld\t%s\t%s\t%d\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n", p1->num, p1->name,p1->gender, p1->age,p1->math, p1->phy,p1->eng, p1->cpu, p1->sum);
 549         getchar();
 550         getchar();
 551     }
 552     else
 553         printf("不存在姓名为%s学生\n", name);//没有查找到满足要求的结点
 554     return(head);
 555 }        
 556 
 557 
 558 //2.按性别查询
 559 student *search_By_Gender(student *head)  //定义一个函数，赋给他查找链表的首地址
 560 {
 561     system("cls");
 562     char gender[GENDER_LEN];
 563     student *p1;
 564     printf("请输入要查询学生的性别:");    //输入查找性别    
 565     scanf("%s",&gender);
 566     printf("******************************************************************\n");
 567     printf("学号\t姓名\t性别\t年龄\t数学\t物理\t英语\t计算机\t总分\n");
 568     printf("__________________________________________________________________\n");
 569     p1=head;
 570     while (strcmp(gender, p1->gender)!=0&&p1->next!=NULL) //开始扫描每个结点以查找与要求吻合的结点
 571         p1=p1->next;                              //如果不相同就扫描下一个
 572     if(strcmp(gender, p1->gender)==0)            //查找到相同的信息开始输出
 573     {
 574         printf("%ld\t%s\t%s\t%d\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n", p1->num, p1->name,p1->gender, p1->age,p1->math, p1->phy,p1->eng, p1->cpu, p1->sum);
 575         getchar();
 576         getchar();
 577     }
 578     else
 579         printf("不存在姓名为%s学生\n", gender);//没有查找到满足要求的结点
 580     return(head);
 581 }        
 582 
 583 //3.按年龄查询
 584 student *search3(student *head)                //同以上查找函数
 585 {
 586     system("cls");
 587     int age;
 588     student *p1;
 589     printf("请输入要查询学生的年龄(输入0退出):");
 590     scanf("%d",&age);
 591     while(age!=0)
 592     {
 593         system("cls");
 594         if(head==NULL)
 595         {
 596             printf("\n没有任何学生的信息，请输入\n");
 597             return(head);
 598         }
 599         printf("******************************************************************\n");
 600         printf("学号\t姓名\t性别\t年龄\t数学\t物理\t英语\t计算机\t总分\n");
 601         printf("__________________________________________________________________\n");
 602         p1=head;
 603         while (age!=p1->age&&p1->next!=NULL)
 604             p1=p1->next;
 605         if(age==p1->age)
 606         {
 607             printf("%ld\t%s\t%s\t%d\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n", p1->num, p1->name,p1->gender, p1->age,p1->math, p1->phy,p1->eng, p1->cpu, p1->sum);
 608 
 609         }
 610         else
 611             printf("不存在年龄为%d学生\n", age);
 612         printf("输入要查询学生的年龄(输入0退出):");
 613         scanf("%d", &age);
 614     }
 615     printf("退出查询\n");
 616     return(head);
 617 }
 618 
 619 //4.按数学成绩查询
 620 student *search4(student *head)                //同以上查找函数
 621 {
 622     system("cls");
 623     float math;
 624     student *p1;
 625     printf("请输入要查询学生的数学成绩(输入0退出):");
 626     scanf("%f",&math);
 627     while(math!=0)
 628     {
 629         system("cls");
 630         if(head==NULL)
 631         {
 632             printf("\n没有任何学生的资料，请输入\n");
 633             return(head);
 634         }
 635         printf("******************************************************************\n");
 636         printf("学号\t姓名\t性别\t年龄\t数学\t物理\t英语\t计算机\t总分\n");
 637         printf("__________________________________________________________________\n");
 638         p1=head;
 639         while (math!=p1->math&&p1->next!=NULL)
 640             p1=p1->next;
 641         if(math==p1->math)
 642         {
 643             printf("%ld\t%s\t%s\t%d\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n", p1->num, p1->name,p1->gender, p1->age,p1->math, p1->phy,p1->eng, p1->cpu, p1->sum);
 644 
 645         }
 646         else
 647             printf("不存在数学成绩为%.2f学生\n", math);
 648         printf("输入要查询学生的数学成绩(输入0退出):");
 649         scanf("%f", &math);
 650     }
 651     printf("退出查询\n");
 652     return(head);
 653 }        
 654 
 655 //5.按物理成绩查询
 656 student *search5(student *head)                //同以上查找函数
 657 {
 658     system("cls");
 659     float phy;
 660     student *p1;
 661     printf("请输入要查询学生的物理成绩(输入0退出):");
 662     scanf("%f",&phy);
 663 
 664     while(phy!=0)
 665     {
 666         system("cls");
 667         if(head==NULL)
 668         {
 669             printf("\n没有任何学生的资料，请输入\n");
 670             return(head);
 671         }
 672         printf("******************************************************************\n");
 673         printf("学号\t姓名\t性别\t年龄\t数学\t物理\t英语\t计算机\t总分\n");
 674         printf("__________________________________________________________________\n");
 675         p1=head;
 676         while (phy!=p1->phy&&p1->next!=NULL)
 677             p1=p1->next;
 678         if(phy==p1->phy)
 679         {
 680             printf("%ld\t%s\t%s\t%d\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n", p1->num, p1->name,p1->gender, p1->age,p1->math, p1->phy,p1->eng, p1->cpu, p1->sum);
 681 
 682         }
 683         else
 684             printf("不存在物理成绩为%.2f学生\n", phy);
 685         printf("输入要查询学生的物理成绩(输入0退出):");
 686         scanf("%f", &phy);
 687     }
 688     printf("退出查询\n");
 689     return(head);
 690 }        
 691 
 692 //6.按英语成绩查询
 693 student *search6(student *head)                //同以上查找函数
 694 {
 695     system("cls");
 696     float eng;
 697     student *p1;
 698     printf("请输入要查询学生的英语成绩(输入0退出):");
 699     scanf("%f",&eng);
 700     while(eng!=0)
 701     {
 702         system("cls");
 703         if(head==NULL)
 704         {
 705             printf("\n没有任何学生的资料，请输入\n");
 706             return(head);
 707         }
 708         printf("******************************************************************\n");
 709         printf("学号\t姓名\t性别\t年龄\t数学\t物理\t英语\t计算机\t总分\n");
 710         printf("__________________________________________________________________\n");
 711         p1=head;
 712         while (eng!=p1->eng&&p1->next!=NULL)
 713             p1=p1->next;
 714         if(eng==p1->eng)
 715         {
 716             printf("%ld\t%s\t%s\t%d\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n", p1->num, p1->name,p1->gender, p1->age,p1->math, p1->phy,p1->eng, p1->cpu, p1->sum);
 717 
 718         }
 719         else
 720             printf("不存在英语成绩为%.2f学生\n", eng);
 721         printf("输入要查询学生的英语成绩(输入0退出):");
 722         scanf("%f", &eng);
 723     }
 724     printf("退出查询\n");
 725     return(head);
 726 }
 727 
 728 //7.按计算机成绩查询
 729 student *search7(student *head)                //同以上查找函数
 730 {
 731     system("cls");
 732     system("cls");
 733     float cpu;
 734     student *p1;
 735     printf("请输入要查询学生的计算机成绩(输入0退出):");
 736     scanf("%f",&cpu);
 737     while(cpu!=0)
 738     {
 739         if(head==NULL)
 740         {
 741             printf("\n没有任何学生的资料，请输入\n");
 742             return(head);
 743         }
 744         printf("******************************************************************\n");
 745         printf("学号\t姓名\t性别\t年龄\t数学\t物理\t英语\t计算机\t总分\n");
 746         printf("__________________________________________________________________\n");
 747         p1=head;
 748         while (cpu!=p1->cpu&&p1->next!=NULL)
 749             p1=p1->next;
 750         if(cpu==p1->cpu)
 751         {
 752             printf("%ld\t%s\t%s\t%d\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n", p1->num, p1->name,p1->gender, p1->age,p1->math, p1->phy,p1->eng, p1->cpu, p1->sum);
 753         }
 754         else
 755             printf("不存在计算机成绩为%.2f学生\n", cpu);
 756         printf("输入要查询学生的计算机成绩(输入0退出):");
 757         scanf("%f", &cpu);
 758     }
 759     printf("退出查询\n");
 760     return(head);
 761 }
 762 
 763 //8.按总分成绩查询
 764 student *search8(student *head)                //同以上查找函数
 765 {
 766     system("cls");
 767     float sum;
 768     student *p1;
 769     printf("请输入要查询学生的总分(输入0退出):");
 770     scanf("%f",&sum);
 771     while(sum!=0)
 772     {
 773         system("cls");
 774         if(head==NULL)
 775         {
 776             printf("\n没有任何学生的资料，请输入\n");
 777             return(head);
 778         }
 779         printf("******************************************************************\n");
 780         printf("学号\t姓名\t性别\t年龄\t数学\t物理\t英语\t计算机\t总分\n");
 781         printf("__________________________________________________________________\n");
 782         p1=head;
 783         while (sum!=p1->sum&&p1->next!=NULL)
 784             p1=p1->next;
 785         if(sum==p1->sum)
 786         {
 787             printf("%ld\t%s\t%s\t%d\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n", p1->num, p1->name,p1->gender, p1->age,p1->math, p1->phy,p1->eng, p1->cpu, p1->sum);
 788 
 789         }
 790         else
 791             printf("不存在总分成绩为%.2f学生\n", sum);
 792         printf("输入要查询学生的总分成绩(输入0退出):");
 793         scanf("%f", &sum);
 794     }
 795     printf("退出查询\n");
 796     return(head);
 797 }
 798 
 799 //******************************************************查询模块
 800 
 801 
 802 
 803 
 804 
 805 
 806 //**********************************************   阅读模块
 807 
 808 //输出
 809 void print(student *head)                    //赋给函数首地址，根据地址输出链表
 810 {
 811     system("cls");
 812     student *p;
 813     printf("已录入%d个学生的记录是:\n", n);
 814     printf("******************************************************************************\n");
 815     printf("学号\t姓名\t性别\t年龄\t数学\t物理\t英语\t计算机\t总分\n");
 816     printf("______________________________________________________________________________\n");    //输出格式
 817     p=head;
 818     if(head!=NULL)
 819         do                        //输出信息
 820         {
 821             printf("%ld\t%s\t%s\t%d\t%.2f\t%.2f\t%.2f\t%.2f\t%.2f\n", p->num, p->name,p->gender, p->age,p->math, p->phy,p->eng, p->cpu, p->sum);
 822             p=p->next;
 823             printf("______________________________________________________________________________\n");
 824         }while(p!=NULL);        //当到达最后一个结点停止
 825     else
 826         printf("\n没有任何学生的信息\n");        //链表为空输出
 827     getchar();
 828 }
 829 
 830 
 831 void Account_print(stu_accout *head)                    //赋给函数首地址，根据地址输出链表
 832 {
 833     system("cls");
 834     stu_accout *p;
 835     printf("******************************************************************************\n");
 836     printf("账户名\t密码\t权限\n");
 837     printf("______________________________________________________________________________\n");    //输出格式
 838     p=head;
 839     if(head!=NULL)
 840         do                    //输出信息
 841         {
 842             if(strcmp(p->name,UserName)==0)
 843             {
 844                 printf("%s\t%s\t%d\n",p->name,p->passwd,p->role);
 845                 printf("______________________________________________________________________________\n");
 846                 break;
 847             }
 848             p=p->next;    
 849         }while(p!=NULL);        //当到达最后一个结点停止
 850     else
 851         printf("\n没有任何学生的信息\n");        //链表为空输出
 852     getchar();
 853 }
 854 
 855 //**********************************************   阅读模块
 856 
 857 
 858 
 859 
 860 
 861 
 862 //******************************************************排序模块
 863 
 864 
 865 //7.排序信息菜单
 866 student *menu7(student *head)
 867 {
 868     while(1)
 869     {
 870         system("cls");
 871         int k;                                    //定义一个整形变量输入switch进行判断
 872         student *stu=0;
 873         printf("\n\n\n");
 874         printf("******************************************************************\n");
 875         printf("*                       排序学生信息目录                         *\n");
 876         printf("*                       1.按学生学号排序                         *\n");
 877         printf("*                       2.按学生年龄排序                         *\n");
 878         printf("*                       3.按数学成绩排序                         *\n");
 879         printf("*                       4.按物理成绩排序                         *\n");
 880         printf("*                       5.按英语成绩排序                         *\n");
 881         printf("*                       6.按计算机成绩排序                       *\n");
 882         printf("*                       7.按总分成绩排序                         *\n");
 883         printf("*                       8.返回主菜单                             *\n");
 884         printf("******************************************************************\n\n");
 885         printf("我的选择:");                    //输出菜单界面
 886 
 887         scanf("%d", &k);//输入选择
 888         getchar();
 889         switch(k)                                //判断
 890         {
 891         case 1:
 892             {
 893                 head=sort1(head);                //按学号排序，调用学号排序函数
 894                 break;                            //跳出
 895             }
 896         case 2:
 897             {
 898                 head=sort2(head);                //按年龄排序，调用年龄排序函数
 899                 break;
 900             }
 901         case 3:
 902             {
 903                 head=sort3(head);                //按数学成绩排序，调用数学成绩排序函数
 904                 break;
 905             }
 906         case 4:
 907             {
 908                 head=sort4(head);                //按物理成绩排序，调用物理成绩排序函数
 909                 break;
 910             }
 911         case 5:
 912             {
 913                 head=sort5(head);
 914                 break;
 915             }
 916         case 6:
 917             {
 918                 head=sort6(head);
 919                 break;
 920             }
 921         case 7:
 922             {
 923                 head=sort7(head);
 924                 break;
 925             }
 926         case 8:break;                            //返回主菜单
 927         default:printf("输入错误请重试\n");
 928         }
 929         if(k==8) break;
 930     }
 931     return head;
 932 }
 933 
 934 //交换
 935 void swap(student *p1, student *p2)     //交换函数，用于冒泡法的排序函数中
 936 {
 937     int k, d;                                    //定义各种交换的中间变量
 938     char a[10];
 939     float temp;                                    
 940     k=p1->num;                                //把新定义的num变量作为中间变量交换swap函数中的前后两个变量的学号信息
 941     p1->num=p2->num;
 942     p2->num=k;
 943 
 944     strcpy(a, p1->name);            //交换姓名信息，采用一个同类型中间变量，过程同上
 945     strcpy(p1->name, p2->name);
 946     strcpy(p2->name,a);
 947 
 948     strcpy(a, p1->gender);            //交换性别信息，采用一个同类型中间变量，过程同上
 949     strcpy(p1->gender, p2->gender);
 950     strcpy(p2->gender,a);
 951 
 952     d=p1->age;                   //交换年龄信息，采用一个同类型中间变量，过程同上
 953     p1->age=p2->age;
 954     p2->age=d;
 955 
 956     temp=p1->math;               //交换成绩信息，采用一个同类型中间变量，过程同上
 957     p1->math=p2->math;
 958     p2->math=temp;
 959 
 960     temp=p1->phy;              //交换成绩信息，采用一个同类型中间变量，过程同上
 961     p1->phy=p2->phy;
 962     p2->phy=temp;
 963 
 964     temp=p1->eng;             //交换成绩信息，采用一个同类型中间变量，过程同上
 965     p1->eng=p1->eng;
 966     p2->eng=temp;
 967 
 968     temp=p1->cpu;             //交换成绩信息，采用一个同类型中间变量，过程同上
 969     p1->cpu=p2->cpu;
 970     p2->cpu=temp;
 971 
 972     temp=p1->sum;                 //交换成绩信息，采用一个同类型中间变量，过程同上
 973     p1->sum=p2->sum;
 974     p2->sum=temp;
 975 }    
 976 
 977 //1.按学号排序
 978 student *sort1(student *head)            //定义排序函数，赋值给所需删除操作链表的首地址
 979 {
 980     system("cls");
 981     student *p1, *p2;
 982     int i, j;            
 983     p2=head;
 984     p1=head;
 985     if(head==NULL)                                    //进行一次判断，链表为空时，提示
 986     {
 987         printf("没有任何学生的信息，请先输入\n");
 988     }
 989     for(i=1; i<n; i++)
 990     {
 991         for(j=i+1;j<=n; j++)
 992         {
 993             p2=p1;
 994             p1=p1->next;
 995             if(p2->num<p1->num)                    //按冒泡法如果相邻两个结点不符合顺序要求，交换其值
 996                 swap(p2,p1);                    //因为是结构体，有多个元素，因此先前定义了一个swap函数，此时调用
 997         }
 998         p2=head;                                //第一次排序完成，令其返回首结点进行循环
 999         p1=head;
1000     }
1001     print(head);//调用print函数输出排序结果
1002     return head;                                    //返回首地址，供其他函数处理该链表
1003 }
1004 
1005 //2.按年龄排序            
1006 student *sort2(student *head)            //同以上排序函数
1007 {
1008     system("cls");
1009     student *p1, *p2;
1010     int i, j;            
1011     p2=head;
1012     p1=head;
1013     if(head==NULL)
1014     {
1015         printf("没有任何学生的信息，请先输入\n");
1016     }
1017     for(i=1; i<n; i++)
1018     {
1019         for(j=i+1;j<=n; j++)
1020         {
1021             p2=p1;
1022             p1=p1->next;
1023             if(p2->age<p1->age)
1024                 swap(p2,p1);
1025         }
1026         p2=head;
1027         p1=head;
1028     }
1029     print(head);
1030     return head;
1031 }
1032 
1033 //3.按数学成绩排序
1034 student *sort3(student *head)            //同以上排序函数
1035 {
1036     system("cls");
1037     student *p1, *p2;
1038     int i, j;            
1039     p2=head;
1040     p1=head;
1041     if(head==NULL)
1042     {
1043         printf("没有任何学生的信息，请先输入\n");
1044     }
1045     for(i=1; i<n; i++)
1046     {
1047         for(j=i+1;j<=n; j++)
1048         {
1049             p2=p1;
1050             p1=p1->next;
1051             if(p2->math<p1->math)
1052                 swap(p2,p1);
1053         }
1054         p2=head;
1055         p1=head;
1056     }
1057     print(head);
1058     return head;
1059 }
1060 
1061 //4.按物理成绩排序
1062 student *sort4(student *head)            //同以上排序函数
1063 {
1064     system("cls");
1065     student *p1, *p2;
1066     int i, j;            
1067     p2=head;
1068     p1=head;
1069     if(head==NULL)
1070     {
1071         printf("没有任何学生的信息，请先输入\n");
1072     }
1073     for(i=1; i<n; i++)
1074     {
1075         for(j=i+1;j<=n; j++)
1076         {
1077             p2=p1;
1078             p1=p1->next;
1079             if(p2->phy<p1->phy)
1080                 swap(p2,p1);
1081         }
1082         p2=head;
1083         p1=head;
1084     }
1085     print(head);
1086     return head;
1087 }
1088 
1089 //5.按英语成绩排序
1090 student *sort5(student *head)            //同以上排序函数
1091 {
1092     system("cls");
1093     student *p1, *p2;
1094     int i, j;            
1095     p2=head;
1096     p1=head;
1097     if(head==NULL)
1098     {
1099         printf("没有任何学生的信息，请先输入\n");
1100     }
1101     for(i=1; i<n; i++)
1102     {
1103         for(j=i+1;j<=n; j++)
1104         {
1105             p2=p1;
1106             p1=p1->next;
1107             if(p2->eng<p1->eng)
1108                 swap(p2,p1);
1109         }
1110         p2=head;
1111         p1=head;
1112     }
1113     print(head);
1114     return head;
1115 }
1116 
1117 //6.按计算机成绩排序
1118 student *sort6(student *head)            //同以上排序函数
1119 {
1120     system("cls");
1121     student *p1, *p2;
1122     int i, j;            
1123     p2=head;
1124     p1=head;
1125     if(head==NULL)
1126     {
1127         printf("没有任何学生的信息，请先输入\n");
1128     }
1129     for(i=1; i<n; i++)
1130     {
1131         for(j=i+1;j<=n; j++)
1132         {
1133             p2=p1;
1134             p1=p1->next;
1135             if(p2->cpu<p1->cpu)
1136                 swap(p2,p1);
1137         }
1138         p2=head;
1139         p1=head;
1140     }
1141     print(head);
1142     return head;
1143 }
1144 
1145 //7.按总分成绩排序
1146 student *sort7(student *head)            //同以上排序函数
1147 {
1148     system("cls");
1149     student *p1, *p2;
1150     int i, j;
1151     p2=head;
1152     p1=head;
1153     if(head==NULL)
1154     {
1155         printf("没有任何学生的信息，请先输入\n");
1156     }
1157     for(i=1; i<n; i++)
1158     {
1159         for(j=i+1;j<=n; j++)
1160         {
1161             p2=p1;
1162             p1=p1->next;
1163             if(p2->sum<p1->sum)
1164                 swap(p2,p1);
1165         }
1166         p2=head;
1167         p1=head;
1168     }
1169     print(head);
1170     return head;
1171 }
1172 
1173 //******************************************************排序模块
1174 
1175 
1176 
1177 
1178 
1179 
1180 //******************************************************增删修模块
1181 
1182 //添加学生信息 
1183 student *add(student *head, student *stu)        //定义学生函数和首地址及添加信息地址
1184 {
1185     student *p0, *p1, *p3;
1186     int i;
1187     p3=stu=(student *)malloc(sizeof(student));    //把新建节点的地址赋给指针
1188     printf("\n请输入要增加的学生的信息:");
1189     printf("请输入学生学号(学号大于0):");
1190     scanf("%ld", &stu->num);
1191     //getchar();
1192     p3=head;
1193     if(n>0)
1194     {
1195         for(i=0; i<n; i++)                        //循环，让指针指向最后一个结点
1196         {
1197             p3=p3->next;
1198         }
1199     }
1200     printf("请输入学生姓名:");                                        //输入插入学生的
1201     scanf("%s", stu->name);
1202     //getchar();
1203     printf("请输入学生性别:");                                        //输入插入学生的
1204     scanf("%s", stu->gender);
1205     //getchar();
1206     printf("请输入年龄、数学、物理、英语、计算机成绩并以空格隔开(0-100):");
1207     scanf("%d%f%f%f%f", &stu->age, &stu->math, &stu->phy, &stu->eng, &stu->cpu);
1208     getchar();
1209     stu->sum=stu->math+stu->phy+stu->eng+stu->cpu;
1210     p1=head;
1211     p0=stu;
1212     if(head!=NULL)                                                
1213     {
1214         while(p1->next!=NULL)
1215         {
1216             p1=p1->next;
1217         }
1218         p1->next=p0;                    //把保存有添加学生信息的结点连接到链表最后
1219         p0->next=NULL;
1220     }
1221     else
1222     {
1223         head=p0;
1224         p0->next=NULL;
1225     }
1226 
1227     int Save=Save_Stu(head);
1228     if(Save)
1229         n=n+1;                                //插入一个学生，n自增一次
1230     printf("现在的学生数为:%d\n", n);            
1231     getchar();
1232     return(head);                                //返回改变链表的首地址
1233 }
1234 
1235 //删除学生信息 
1236 student *del(student *head)        //定义删除函数，赋值给所需删除操作链表的首地址
1237 {
1238     student *p1, *p2;
1239     long int num;                                    //定义一个与结构体重学号元素同类型的变量输入所需删除的学号
1240     printf("输入要删除的学生的学号(输入0时退出):");
1241     scanf("%ld", &num);
1242     getchar();
1243     p1=head;
1244     while(num!=p1->num&&p1->next!=NULL)            //在连表中查找满足要求的结点
1245     {
1246         p2=p1;
1247         p1=p1->next;
1248     }
1249     if(num==p1->num)
1250     {
1251         if(p1!=head)
1252         {
1253             p2->next=p1->next;                    //让该结点的上下两个结点相连，即可“删除”该结点
1254 
1255         }
1256         else
1257         {
1258             head=p1->next;
1259 
1260         }
1261         printf("成功删除%d\n", num);
1262 
1263         int Save=Save_Stu(head);
1264         if(Save)
1265             n=n-1;                                //插入一个学生，n自增一次
1266         printf("现在的学生数为:%d\n", n);            
1267     }
1268     else
1269         printf("没有检索到%ld学生\n", num);        //没有扫描到符合要求的结点，提示
1270     getchar();
1271     return(head);                                    //返回修改过的链表的首地址以供其他函数使用
1272 }
1273 
1274 
1275 //修改学生信息
1276 student *cha(student *head)
1277 {
1278     while(1)
1279     {
1280         student *p1;
1281         long int num;
1282         int k;
1283         printf("输入要修改的学生的学号(输入0时退出):");
1284         scanf("%ld", &num);
1285         if(num==0) break;
1286         getchar();
1287         p1=head;
1288         while(num!=p1->num&&p1->next!=NULL)            //在连表中查找满足要求的结点
1289             p1=p1->next;
1290         if(p1!=NULL)
1291         {
1292             printf("请选择你要修改的内容：\n  姓名:1   性别:2   年龄:3   数学成绩:4  \n  物理成绩:5    英语成绩:6    计算机成绩:7\n");
1293 
1294             scanf("%d",&k);
1295             getchar();
1296             switch (k)
1297             {
1298             case 1:
1299                 {
1300                     printf("请输入姓名：");
1301                     scanf("%s",p1->name);
1302                     break;
1303                 }
1304             case 2:
1305                 {
1306                     printf("请输入性别：");
1307                     scanf("%s",p1->gender);
1308                     break;
1309                 }
1310             case 3:
1311                 {
1312                     printf("请输入年龄：");
1313                     scanf("%d",&p1->age);
1314                     break;
1315                 }
1316             case 4:
1317                 {
1318                     printf("请输入数学成绩：");
1319                     scanf("%f",&p1->math);
1320                     p1->sum=p1->math+p1->phy+p1->eng+p1->cpu;
1321                     break;
1322                 }
1323             case 5:
1324                 {
1325                     printf("请输入物理成绩：");
1326                     scanf("%f",&p1->phy);
1327                     p1->sum=p1->math+p1->phy+p1->eng+p1->cpu;
1328                     break;
1329                 }
1330             case 6:
1331                 {
1332                     printf("请输入英语成绩：");
1333                     scanf("%f",&p1->eng);
1334                     p1->sum=p1->math+p1->phy+p1->eng+p1->cpu;
1335                     break;
1336                 }
1337             case 7:
1338                 {
1339                     printf("请输入计算机成绩：");
1340                     scanf("%f",&p1->cpu);
1341                     p1->sum=p1->math+p1->phy+p1->eng+p1->cpu;
1342                     break;
1343                 }
1344             default:printf("输入错误请重试\n");
1345                       break;
1346             }
1347             printf("成功修改%d\n", num);
1348             int Save=Save_Stu(head);
1349                     
1350         }
1351         else
1352             printf("没有检索到%ld学生\n", num);        //没有扫描到符合要求的结点，提示
1353         getchar();
1354         getchar();                                //返回修改过的链表的首地址以供其他函数使用
1355     }
1356         return(head);
1357 }
1358 
1359 //修改学生信息
1360 stu_accout *Password_cha(stu_accout *head)
1361 {
1362     
1363         stu_accout *p1;
1364         int k;
1365         p1=head;
1366         while(strcmp(UserName,p1->name)!=0&&p1->next!=NULL)            //在连表中查找满足要求的结点
1367             p1=p1->next;
1368             printf("请原来的密码\n");
1369             char OrPassword[PWD_LEN];
1370             scanf("%s",OrPassword);
1371             if(strcmp(OrPassword,p1->passwd)!=0)
1372                  printf("密码错误！！\n");
1373             else
1374             {
1375                 char NewPass1[PWD_LEN];
1376                 char NewPass2[PWD_LEN];
1377                 printf("请输入新密码:");
1378                     scanf("%s",NewPass1);
1379                 getchar();
1380                 printf("请确认新密码:");
1381                     scanf("%s",NewPass2);
1382                 if(strcmp(NewPass1,NewPass2)!=0)
1383                    printf("密码不一致！！！");
1384                 else
1385                 {
1386                     strcpy(p1->passwd,NewPass1);
1387                     int Save=Save_Account(head);
1388                      printf("成功修改\n");
1389                    
1390                 }
1391              
1392             }
1393             
1394                     
1395     
1396         getchar();
1397         getchar();                                //返回修改过的链表的首地址以供其他函数使用
1398     
1399         return(head);
1400 }
1401 
1402 //******************************************************增删修模块

View Code
```

主函数

```
![](http://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)![](http://images.cnblogs.com/OutliningIndicators/ExpandedBlockStart.gif)

  1 #include "My_header.h"
  2 
  3 //主函数
  4 int main()
  5 {
  6     student *Student_head=0, *stu=0;
  7     stu_accout *Account_Head=0;
  8     int k, a;                        //定义录入选择的整形变量
  9     while(1)
 10     {
 11         system("cls");
 12 
 13         printf(",.,,..... ...:,,,:,...   ....,,,.   ,,,,,,:.  ..:,,,..... ....\n");
 14         printf(":  ,s1hhhhiii ... :111h55h11s...sh51...... rhhhi .  r1hhhhs:i \n");
 15         printf(",.9Ahrrrs5&@A.,,,,;rr;3@#riri,,:;9@@s.:,,,:#@@1;,.1H9srir19M@r\n");
 16         printf(".8@; ,,,,..3M:,,,:,,,.1@H.,,,::,.hM@X.:,:.3M@#:,,s@8 ,,,,..iM5\n");
 17         printf(".A@i..,,,:,.i:,,,,,,,.1@H,,,,,,,.S8A@i,,,:H5@#:,.5@8...,,::.;;\n");
 18         printf(".rM@X5r,..,,,,:,,,,,,.1@H,,,,,,:.S9h@9.:.3Xi@#:,,:X@B9s:...,, \n");
 19         printf(": :h9B#MA9s,.,:,,,,,,.1@B,,,,,::.S8.##:,,Bri@#:,:,.i9H##H81,..\n");
 20         printf(",.,..,ih8B@MS,,:,,,,,.1@B,,,,,,:.S8 9@5 S& s@#:,,,,,..;19H@#3.\n");
 21         printf(": ,:,,,...i&@G,,,,,,,.1@B,,,,,,:.S8 i@H.H1 s@M:,:,,::,,...iX@8\n");
 22         printf(",9s.::,,:,..#@:,,,,,,.1@B,,,,,,,.S8..X@X&..s@M:,,Gh ,:,,:,..B@\n");
 23         printf(" 9#r..,,,. s@G,,,:,,, s@B..,,,:, S8 .s@@s..r@M,,.8@1..,,,. r@G\n");
 24         printf(".,@B31riir5&5 ....,rriS@Msrri...;G&i. A& .iS@M1: i#H31riir5&S \n\n\n");
 25         printf("                                               开发人员:卓寿杰   \n");
 26         printf("                                                                 \n");
 27         printf("  1.进入                                                         \n");
 28         printf("  2.退出                                                         \n");
 29         printf("*****************************************************************\n\n");
 30         printf("我的选择:");
 31         scanf("%d", &a);                    //输入选择
 32         if(a==1)                            //进入系统
 33         {
 34             stu_accout *Account=NULL;
 35             Account=Acount_Load(Account);
 36             int Login_Change=3;  //三次登入机会
 37             int tem;
 38             int type;
 39             while(Login_Change--)
 40             {
 41                 tem=login(Login_Change,type,Account);
 42                 if(tem) break;
 43             }
 44             if(!tem) break;
 45             while(1)                        //使用循环，在完成一个
 46             {
 47 
 48                 Student_head= Student_File_Load(Student_head);   
 49                 k=menu(type); //调用主目录函数                
 50                 switch(k)
 51                 {
 52                 case 1:// 1.查询学生的信息
 53                     {            
 54                         Student_head=menu1(Student_head);   
 55                         break;
 56                     }
 57                 case 2:print(Student_head);break;    //输出函数，显示学生信息
 58                 case 3:
 59                     {              //调用信息排序菜单函数，并进一步调用各种排序函数
 60                         Student_head=menu7(Student_head);                    
 61                         break;
 62                     }
 63                 case 7:
 64                     {   student*  tem=NULL;
 65                     Student_head=add(Student_head, tem);                    
 66                     break;
 67                     }
 68                 case 8:
 69                     {
 70                         print(Student_head);
 71                         Student_head=del(Student_head);           //调用删除函数返回首地址
 72                         print(Student_head);        //根据删除函数返回的地址输出结果
 73                         break;
 74                     }
 75                 case 9:
 76                     {
 77                         print(Student_head);
 78                         Student_head=cha(Student_head);
 79                         getchar();
 80                         print(Student_head);
 81                         break;
 82                     }
 83                 case 4:
 84                     {
 85                         Account_print(Account);
 86                         break;
 87                     }
 88                 case 5:
 89                     {
 90                         Account=Password_cha(Account);
 91                         break;
 92                     }
 93                 case 6:break;     //返回登录界面
 94                 case 10:
 95                     {
 96                         Student_head=Other_Student_File_Load(Student_head);
 97                         break;
 98                     }
 99                 case 11:
100                     {
101                         Other_Save_Stu(Student_head);
102                         break;
103                     }
104                 case 0:exit(0);                 //退出
105                 default:printf("输入错误请重试\n");
106                     
107                 }
108                 if(k==6)break;
109             }
110         }
111         else
112         {
113             printf("您是否确认退出?(y/n)");  //确认退出程序
114             getchar();
115             if(getchar()=='y')
116                 break;
117         }
118     }
119 }

View Code
```


![](https://img-blog.csdn.net/20170502101051517?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMTIzOTQ0Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

