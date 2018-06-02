
#include <omp.h> 
#include <stdio.h> 

//循环测试函数  
int test()
{
  int sum=0;
	for (int i = 0; i < 10000000; i++)
	{
		//printf("%d\n", i);
    sum += i;
	}
  return sum;
}

int test_openmp()
{
	printf("test_openmp\n");
	int times = 8;
	if (1) {
		double start = omp_get_wtime();//获取起始时间  
		for (int i = 0; i < times; i++) {
			int sum = test();
			printf("%d - %d (%d) %d\n", omp_get_num_threads(), omp_get_thread_num(), omp_in_parallel(), sum);
		}
		double end = omp_get_wtime();
		printf("time: %f\n", end - start);
	}
	omp_set_num_threads(8);
	{
		double start = omp_get_wtime();//获取起始时间
#pragma omp parallel for
		for (int i = 0; i < times; i++) {
			int sum = test();
			printf("%d - %d (%d) %d\n", omp_get_num_threads(), omp_get_thread_num(), omp_in_parallel(), sum);
		}
		double end = omp_get_wtime();
		printf("openmp time: %f\n", end - start);
	}

	return 0;
}
