# C++ 在堆上开辟与释放二维、三维指针 - L_Andy的专栏 - CSDN博客

2013年08月14日 15:51:43[卡哥](https://me.csdn.net/L_Andy)阅读数：1505


                
学习C++新手通常会对指针的使用比较头疼，其实指针的概念很简单，只要能悟清楚这样一个简单的道理就能对指针有一定的认识了： 例如 int *a = new int[10]; 一维指针其实就相当于一维数组，不用去看书上所说的数组在内存中的首地址这些晦涩的话，以此类推 二维指针就相当于二维数组，新手对一维数组的开辟与释放比较容易熟悉，例如上面的a 释放就是 delete []a; a = NULL; 注意a = NULL; 必须要加上，这样是为了避免这个指针会变成“野指针”。写程序时一定要注意规范性和严谨性，避免可能会出现的错误。

//二维指针的开辟与释放

int **b = NULL;

b = new int*[10];

for(int i = 0; i != 10; ++i)

{

     b[i] = new int[50];

     memset(b[i], 0, sizeof(int)*50);

}

//这样就在堆上开辟了int类型的二维指针，大小为10*50，相当于在堆上一个二栈数组int b[10][50];

for(int i = 0; i != 10; ++i)

{

     delete []b[i];

     b[i] = NULL;

}

delete []b;

b = NULL;

//三维指针的开辟与释放

int ***a = NULL;

a = new int**[10];

for(int i = 0; i != 10; ++i)

{

    a[i] = new int*[50];

    for(int j = 0; j != 50; ++j)

    {

        a[i][j] = new int[30];

        memset(a[i][j], 0, sizeof(int)*30);

    }

}

//这样就在堆上开辟了int类型的三维指针，大小为10*50*30，相当于在栈上一个二维数组int a[10][50][30];

for(int i = 0; i != 10; ++i)

{

    for(int j = 0; j != 50; ++j)

    {

       delete []a[i][j];

       a[i][j] = NULL;           

    }

    delete []a[i];

    a[i] = NULL;

}

delete []a;

a = NULL;
            

