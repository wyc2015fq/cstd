# 初次使用Windows下的OpenMP - 战斗蜗牛的专栏 - CSDN博客





2012年11月03日 19:26:01[vbskj](https://me.csdn.net/vbskj)阅读数：8932








一、开发环境配置




1）Windows下面比较容易，安装VS2008 or VS2010之后，新建一个工程并右键点击“属性”，依次进入“配置属性”--》“C/C++”--》“语言”，设置右侧的第五个选项，“OpenMP支持”设定为“是（/openmp）”，这样就完成了开发环境的设定；




![](https://img-my.csdn.net/uploads/201211/03/1351941709_9886.jpg)





2）设置系统环境变量

在环境变量中增加“OMP_NUM_THREADS”变量，数值自己根据你的CPU的性能来设置，一般2、4、8等。




![](https://img-my.csdn.net/uploads/201211/03/1351941959_7994.jpg)





二、OpenMP的helloworld

经过以上环境变量设置之后，可以再VS界面下编写OpenMP程序了，比如建立 一个控制台程序：




#include "stdafx.h"

#include "stdlib.h"

#include <omp.h>




int _tmain(int argc, _TCHAR* argv[])

{
omp_set_num_threads(4);
#pragma omp parallel
printf("hello world!No.%d\n",omp_get_thread_num());
return 0;



}


输出结果如下：




![](https://img-my.csdn.net/uploads/201211/03/1351942141_2511.jpg)





另一个稍微复杂一点的：

// firstMP.cpp : 定义控制台应用程序的入口点。

//



#include "stdafx.h"

#include "stdlib.h"

#include <omp.h>

#define TRUE  1

#define FALSE 0



int _tmain(int argc, _TCHAR* argv[])

{

 int n = 4;

  int *a, **b;



#ifdef _OPENMP

   (void) omp_set_dynamic(FALSE);

   if (omp_get_dynamic()) {printf("Warning: dynamic adjustment of threads has been set\n");}

   (void) omp_set_num_threads(4);

#endif



  if ( (a=(int *)malloc(n*sizeof(int))) == NULL ) {

      perror("array a"); exit(-1);

  }



  if ( (b=(int **)malloc(n*sizeof(int *))) == NULL ) {

      perror("array b"); exit(-1);

  } 

  else {

      for (int i=0; i<n; i++)

          if ( (b[i]=(int *)malloc(n*sizeof(int))) == NULL )

             {perror("array b"); exit(-1);}

  }


  #pragma omp parallel shared(n,a,b)

  {

    #pragma omp for

    for (int i=0; i<n; i++)

    {

        a[i] = i + 1;

        #pragma omp parallel for 

        for (int j=0; j<n; j++)

            b[i][j] = a[i];

    }

  } 



  for (int i=0; i<n; i++)

  {

      for (int j=0; j<n; j++)

         printf("b[%d][%d] = %d ",i,j,b[i][j]);

      printf("\n");

  }



  free(a);

  free(b);

  return 0;

}




输出结果为：




![](https://img-my.csdn.net/uploads/201211/03/1351942227_9052.jpg)






