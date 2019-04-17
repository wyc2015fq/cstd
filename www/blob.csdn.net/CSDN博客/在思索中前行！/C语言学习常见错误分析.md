# C语言学习常见错误分析 - 在思索中前行！ - CSDN博客





2014年11月05日 22:18:26[_Tham](https://me.csdn.net/txl16211)阅读数：724








错误分类

    语法错

    逻辑错

    运行错

0.忘记定义变量:

int main()

{

 x=3;y=6;

 printf(“%d/n”,x+y);

}

1.C语言的变量一定要先定义才能使用;

2.输入输出的数据的类型与所用格式说明符不一致

int a=3;float b=4.5;printf(“%f%d/n”,a,b);

它们并不是按照赋值的规则进行转换(如把4.5转换为4),而是将数据在存储单元中的形式按格式符的要求组织输出(如b占4个字节,只把最后两个字节的数据按%d,作为整数输出)

3.未注意int型数据的数值范围

int型数据的数值范围(-32768~32768)

                       int num=89101;

                       printf(“%d”,num);

会将超过低16位的数截去从而得到23563

注意:定义了long型,而在输出时仍用”%d”说明符,仍会出现以上错误

4.输入变量时忘记使用地址符

scanf(“%d%d”,a,b);

5.输入时数据的组织与要求不符

对scanf函数中格式字符串中除了格式说明符外,对其他字符必须按原样输入

6.误把”=“作为”等于”比较符

“=“为附值运算符

“==“为比较运算符



7.语句后面漏分号

{

   t=a;

   a=b;

   b=t

}

它是pascal的语法

8.不该加分号的地方加了分号

if(a>b);

       printf(“a is larger than b/n”);

for(i=0;i<10;i++);

{

     scanf(“%d”,&x);

     printf(“%d/n”,x*x);

}

9.对应该有花括弧的复合语句,忘记加花括弧

sum=0;

i=1;

while(i<=100)

 sum=sum+1;

 i++;

10.括弧不配对

while((c=getchar()!=‘#’)

 putchar(c);

11.在用标识时,忘记了大写字母和小写字母的区别

{

 int a,b,c;

 a=2;

 b=3;

 C=A+B;

 printf(“%d+%d=%D”,A,B,C);

}



12.引用数组元素时误用发圆括弧

{

 int i,a(10);

 for(i=0;i<10;i++)

  scanf(“%d”,&a(i));

}

13.在定义数组时,将定义的”元素个数”误认为是”可使用的最大下标值

{

 int a[10]={1,2,3,4,5,6,7,8,9,10};

 int i;

 for(i=1;i<=10;i++)

  printf(“%d”,a[i]);

}

14.对二维或多维数组的定义和引用的方法不对

{

 int a[5,4];

 …

 printf(“%d”,a[1+2,2+2]);

 …

}

15.误以为数组名代表数组中全部元素

{int a[4]={1,2,3,4};

  printf(“%d%d%d%d”,a);

}



16.混淆字符数组与字符指针的区别

main()

{

 char str[40];

 str=“Computer and c”;

 printf(“%s/n”,str);

}



17.在引用指针变量之前没有对它赋予确定的值

{

 char *p;

 scanf(“%s”,p);

}

{

 char *p,c[20];

 p=c;

 scanf(“%s”,p);

}

18.switch语句的各分支中漏写 break语句

混淆字符和字符串的表示形式

…

char sex;

sex=“M”;

…

19.使用自加（++）和自减（--）运算符时出的错误

{

 int *p,a[5]={1,3,5,7,9};

 p=a;

 printf(“%d”,*p++);

}

注意于*(++p)的区别；

20.所调用的函数在调用语句之后才定义,而又在调用前未加说明

main()

{float x,y,z;

 x=3.5;y=-7.6;

 z=max(x,y);

 printf(“%f”,z);

}

float max(float x,float y)

{

 return (x>y?x:y);

}

21.误认为形参值的改变会影响实参的值

swap(int x,int y)

{int t;

 t=x;x=y;y=t;

}

main()

{int a,b;

 a=3;b=4;

 swap(a,b);

 printf(“%d,%d/n”,a,b);

}

22.函数的实参和形参类型不一致

fun(float x,float y)

main()

{

 int a=3,b=4;

 c=fun(a,b);

 …

}

23.不同类的型指针混用

{

 int i=3,*p1;

 float a=1.5,*p2;

 p1=&i;p2=&a;

 p2=p1;

 printf(“%d,%d/n”,*p1,*p2);

}

24.没有注意函数参数的求值顺序

int i=3;

prinft(“%d,%d,%d/n”,i,++i,++i);

结果为5,5,4

因为VC++是采取自右至左的顺序求函数的值

C标准没有具体规定函数参数求值的顺序

25.混淆数组名与指针变量的区别

{ int i,a[5];

 for(i=0;i<5;i++)

  scanf(“%d”,a++);

}

{int a[5],*p;

 p=a;

 for(int i=0;i<5;i++)

  scanf(“%d”,p++)

}

{int a[5],*p;

 for(p=a;p<a+5;p++)

  scanf(“%d”,p);

}

26.混淆结构体类型与结构体变量的区别

struct worker

{long int num;

 char name[20];

 char sex;

 int age;

};

worker.num=187045;

strcpy(worker.name,”ZhangFun”);

worker.sex=‘M’;

worker.age=18;

27.使用文件时忘记打开,用只读方式打开,却企图向该文件输出数据

if(fp=fopen(“test”,”r”))==NULL)

 {printf(“cannot open this file/n”);

    exit(0);

 }

ch=fgetc(fp);

while(ch!=‘#’)

 {ch=ch+4;

 fputc(ch,fp);

 ch=fgetc(fp);

 }



