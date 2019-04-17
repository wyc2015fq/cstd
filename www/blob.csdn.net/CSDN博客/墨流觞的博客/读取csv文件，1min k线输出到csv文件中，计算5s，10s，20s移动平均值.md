# 读取csv文件，1min k线输出到csv文件中，计算5s，10s，20s移动平均值 - 墨流觞的博客 - CSDN博客





2018年12月06日 22:51:48[墨氲](https://me.csdn.net/dss_dssssd)阅读数：48标签：[C																[K线](https://so.csdn.net/so/search/s.do?q=K线&t=blog)](https://so.csdn.net/so/search/s.do?q=C&t=blog)
个人分类：[c](https://blog.csdn.net/dss_dssssd/article/category/8499802)








### 参考：
- 
C字符串转换为int，float
[https://blog.csdn.net/li6727975/article/details/42875641](https://blog.csdn.net/li6727975/article/details/42875641)

- 
结构体
[http://www.runoob.com/cprogramming/c-structures.html](http://www.runoob.com/cprogramming/c-structures.html)

- 
strncat
[http://c.biancheng.net/cpp/html/169.html](http://c.biancheng.net/cpp/html/169.html)

- 
字符串操作
[https://www.kancloud.cn/digest/clearning/149703](https://www.kancloud.cn/digest/clearning/149703)

- 
bool
[https://blog.csdn.net/qq_31407743/article/details/52196012](https://blog.csdn.net/qq_31407743/article/details/52196012)

- 
循环队列
[https://blog.csdn.net/lpp0900320123/article/details/20694409](https://blog.csdn.net/lpp0900320123/article/details/20694409)

本文主要添加了求平均值函数和修改了代码中的小问题- 
读取和写入csv问价
[http://zhangrui111.github.io/2016/04/09/C语言读取CSV文件/](http://zhangrui111.github.io/2016/04/09/C%E8%AF%AD%E8%A8%80%E8%AF%BB%E5%8F%96CSV%E6%96%87%E4%BB%B6/)


以下为代码：

### queue.h

```
#pragma once
#include <stdbool.h>
typedef struct queue
{
	int *pBase;
	int front;    //指向队列第一个元素
	int rear;    //指向队列最后一个元素的下一个元素
	int maxsize; //循环队列的最大存储空间
}QUEUE, *PQUEUE;

void CreateQueue(PQUEUE Q, int maxsize);
void TraverseQueue(PQUEUE Q);
bool FullQueue(PQUEUE Q);
bool EmptyQueue(PQUEUE Q);
bool Enqueue(PQUEUE Q, int val);
bool Dequeue(PQUEUE Q);
float average_queue(PQUEUE Q);
void FreeQueue(PQUEUE queue);
```

### queue.c

```
#include<stdio.h>
#include<stdlib.h>
#include <stdbool.h>
#include"malloc.h"
#include"queue.h"



void CreateQueue(PQUEUE Q, int maxsize)
{
	Q->pBase = (int *)malloc(sizeof(int)*maxsize);
	if (NULL == Q->pBase)
	{
		printf("Memory allocation failure");
		exit(-1);         //退出程序
	}
	Q->front = 0;          //初始化参数
	Q->rear = 0;
	Q->maxsize = maxsize;
}

void TraverseQueue(PQUEUE Q)
{
	int i = Q->front;
	printf("队列中的元素是:\n");
	while (i%Q->maxsize != Q->rear)
	{
		printf("%d ", Q->pBase[i]);
		i = (i + 1) % Q->maxsize;
	}
	printf("\n");
}
bool FullQueue(PQUEUE Q)
{
	if (Q->front == (Q->rear + 1) % Q->maxsize)    //判断循环链表是否满，留一个预留空间不用
		return true;
	else
		return false;
}
bool EmptyQueue(PQUEUE Q)
{
	if (Q->front == Q->rear)    //判断是否为空
		return true;
	else
		return false;
}
bool Enqueue(PQUEUE Q, int val)
{
	if (FullQueue(Q))
		return false;
	else
	{
		Q->pBase[Q->rear] = val;
		Q->rear = (Q->rear + 1) % Q->maxsize;
		return true;
	}
}

bool Dequeue(PQUEUE Q)
{
	if (EmptyQueue(Q))
	{
		return false;
	}
	else
	{
		Q->front = (Q->front + 1) % Q->maxsize;
		return true;
	}
}

float average_queue(PQUEUE Q) {
	if (FullQueue(Q)) {
		int ai = Q->front;
		float sum = 0.0;
		while (ai%Q->maxsize != Q->rear)
		{
			//printf("%d ", Q->pBase[i]);
			sum += Q->pBase[ai];
			ai = (ai + 1) % Q->maxsize;

			//printf("%f ", sum);
		}
		return sum / (Q->maxsize - 1);
	}
	else
	{
		printf("not Full!!!");
		return -1;
	}

}
void FreeQueue(PQUEUE queue) {
	free(queue->pBase);
}
```

### main.c

```
#define _CRT_SECURE_NO_DEPRECATE
/*
solution 1: #define _CRT_SECURE_NO_DEPRECATE
solution 2 : 使用*_s形式
*/


#include<stdio.h>
#include <string.h>
#include <stdlib.h>
#include "queue.h"

//按收盘价平均值算MA的，准备是用队列数据结构

#define MAX_LINE_SIZE 40
//调试函数
void print_Info_per_min(struct Info_per_min *info);


bool one_min_read_and_write(FILE *rf, FILE *wf, char line[], size_t total, int share_price[], double trading_volume[], struct Info_per_min* info_per_min);
void average_trading_volumn(struct Info_per_min* info_per_min, double trading_volumn[], int length);
void shared_price_processed(struct Info_per_min* info_per_min, int share_price[], int length);
void kLineXEnqueue(PQUEUE kLinex, int closing_price);
void kLineInCsvFile(PQUEUE kLinex, FILE *wf);
struct Info_per_min
{
	//  时间 "年/月/日 时：分"
	char time_min[20];
	// 开盘股价
	int opening_price;
	//收盘价格
	int closing_price;
	//最高股价
	int max_share_price;
	//最低股价
	int min_share_price;
	//平均成交量
	double average_trading_volumn;
};
int main() {
	char file_name[] = "in_Arr.csv";
	char file_write[] = "out_arr.csv";
	//char test[] = "out_Arr_1min.csv";
	//文件开始要删除的行数
	int deletenumbers = 4;
	
	// 测试处理行数
	int totalnum = 600, process_cir=0;
	
	//每次要处理的行数
	size_t total = 120;

	/* 定义每1min内要存储的数据 */
	
	// 1min内股价
	int share_price[120] = { 0 };
	// 1min中的成交量
	double trading_volume[120] = { 0 };
	
	struct Info_per_min info_per_min;

	//Ns内K线计算
	QUEUE kLine5, kLine10, kLine20;
	CreateQueue(&kLine5, 6);
	CreateQueue(&kLine10, 11);
	CreateQueue(&kLine20, 21);

	//缓冲文件中每一行
	char line[MAX_LINE_SIZE];





	FILE *rf, *wf;
	rf = fopen(file_name, "r");
	wf = fopen(file_write, "w+");
	fprintf(wf, "时间, 开盘, 最高, 最低, 收盘, 成交量, MA_5, MA_10, MA_20");


	if (!rf) {
		fprintf(stderr, "failed to open file for reading\n");
		return 1;
	}

	
	
	//将前四行缓冲掉
	for (char line_num = 0; line_num < deletenumbers; ++line_num)
		fgets(line, MAX_LINE_SIZE, rf);

	while (1) {
		if (one_min_read_and_write(rf, wf, line, total, share_price, trading_volume, &info_per_min)){
			average_trading_volumn(&info_per_min, trading_volume, 120);
			shared_price_processed(&info_per_min, share_price, 120);


			fprintf(wf, "\n%s", info_per_min.time_min);
			fprintf(wf, ",%d", info_per_min.opening_price);
			fprintf(wf, ",%d", info_per_min.max_share_price);
			fprintf(wf, ",%d", info_per_min.min_share_price);
			fprintf(wf, ",%d", info_per_min.closing_price);
			fprintf(wf, ",%f", info_per_min.average_trading_volumn);

			//计算k线, 
			//收盘股价入队列
			kLineXEnqueue(&kLine5, info_per_min.closing_price);
			kLineXEnqueue(&kLine10, info_per_min.closing_price);
			kLineXEnqueue(&kLine20, info_per_min.closing_price);

			//计算收盘股价平均值，写入csv文件
			kLineInCsvFile(&kLine5, wf);
			kLineInCsvFile(&kLine10, wf);
			kLineInCsvFile(&kLine20, wf);
		}
		else break;
		
		
		

		//print_Info_per_min(&info_per_min);
		
		/*printf( "%s\n",info_per_min.time_min);
		printf("shared_price:\n");
		for (size_t i = 0; i < 120; ++i)
			printf("%d： %d\n", i, share_price[i]);
		printf("trading_volume\n");
		for (size_t i = 0; i < 120; ++i)
			printf("%d： %f\n", i, trading_volume[i]);*/
		
		
	}
	fclose(rf);
	fclose(wf);
	FreeQueue(&kLine5);
	FreeQueue(&kLine10);
	FreeQueue(&kLine20);


	system("pause");


}

//读出120行数据，读入和写出到文件中
bool one_min_read_and_write(FILE *rf, FILE *wf, char line[], size_t total, int share_price[], double trading_volume[],struct Info_per_min* info_per_min) {
	
	/*
	rf: 读入文件
	wf:写出文件
	line：存储一行数据的变量
	total:一次读取的行数
	*/
	if (fgets(line, MAX_LINE_SIZE, rf) == NULL)
		return false;
	char *result = NULL;
	size_t num_line = 0;
	while (fgets(line, MAX_LINE_SIZE, rf) != NULL) {
		
		result = strtok(line, ",");
		short int times_into = 0;
		while (result != NULL) {
			// printf("%d:, %s\n", times_into, result);
			if (num_line == 0) {
				if (times_into == 0) {
					strcpy(info_per_min->time_min, result);
					strcat(info_per_min->time_min, " ");
					//printf("%d:, %s\n", times_into, info_per_min->time_min);
				}
				else if (times_into == 1) {
					strncat(info_per_min->time_min, result, strlen(result) - 3);
					//printf("%d:, %s\n", times_into, info_per_min->time_min);
				}

				
			}
			if (times_into == 3)
				share_price[num_line] = atoi(result);
			else if (times_into == 4)
				trading_volume[num_line] = atof(result);



			times_into++;
			
			result = strtok(NULL, ",");
			
			
		}

		++num_line;
		//printf("\n");
		if (num_line >= total) break;
	}
	return true;
}


void average_trading_volumn(struct Info_per_min* info_per_min, double trading_volumn[], int length) {
	/*
	info_per_min: 存储一分钟内要获取的信息
	trading_volumn： 一分钟内成交量
	length： 数组trading_volumn长度
	*/
	double sum = 0.0;
	for (int i = 0; i < length; ++i) {
		sum += trading_volumn[i];
	}
	info_per_min->average_trading_volumn = sum / 120;
}

void shared_price_processed(struct Info_per_min* info_per_min, int share_price[], int length) {
	/*
	info_per_min: 存储一分钟内要获取的信息
	share_price: 一分钟内股价变化
	length： 数组share_price的长度
	*/
	int min=share_price[0], max = share_price[0];
	info_per_min->opening_price = share_price[0];
	info_per_min->closing_price = share_price[length - 1];
	for (int i = 1; i < length; ++i) {
		if (max < share_price[i])
			max = share_price[i];
		else if (min > share_price[i])
			min = share_price[i];
	}
	info_per_min->min_share_price = min;
	info_per_min->max_share_price = max;
}

void print_Info_per_min(struct Info_per_min *info) {
	printf("时间: %s\n", info->time_min);
	printf("开盘股价: %d\n", info->opening_price);
	printf("收盘价格: %d\n", info->closing_price);
	printf("最高股价: %d\n", info->max_share_price);
	printf("最低股价: %d\n", info->min_share_price);
	printf("平均成交量: %f\n", info->average_trading_volumn);

}

void kLineXEnqueue(PQUEUE kLinex,int closing_price) {
	if (!Enqueue(kLinex, closing_price)) {
		Dequeue(kLinex);
		Enqueue(kLinex, closing_price);
	}
}

void kLineInCsvFile(PQUEUE kLinex,FILE *wf) {
	float average = 0.0;
	if (FullQueue(kLinex)) {
		average = average_queue(kLinex);
		fprintf(wf, ",%f", average);
	}
		
}
```






