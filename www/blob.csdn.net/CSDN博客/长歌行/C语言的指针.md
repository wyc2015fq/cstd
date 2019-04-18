# C语言的指针 - 长歌行 - CSDN博客





2013年01月03日 17:12:22[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：611
个人分类：[C、C++](https://blog.csdn.net/iuhsihsow/article/category/1072760)












二维数组



```cpp
void main()
 {
 	int num;
 	/* 声明子函数 */
 	void average(float *point, int n);
 	void search(float(*point)[4], int n);
 
 	/* 定义一个静态存储数组并赋初值 */
 	static float score[4][4] = {{76, 90, 92, 87}, {68, 78, 69, 94},  
 	                            {89, 82, 81, 60}, {81, 68, 60, 97}};
 
 	printf("班级的总平均分：");
 	average(*score, 16);    /* 调用函数average求12个分数的平均分 */
 	printf("请输入学生的学号(0-3)：");
 	scanf("%d", &num);
 	search(score, num);    /* 求出第四个学生的成绩 */
 }
 
 /* 子函数定义 */
 void average(float *point, int n)
 {
 	float *p_end;
 	float aver;
 	float sum = 0;
 	p_end = point + n -1;
 	for(; point<=p_end; point++)
 		sum = sum + (*point);
 	aver = sum/n;
 	printf("%5.2f\n", aver);
 }
 
 void search(float(*point)[4], int n)
 {
 	int i;
 	for(i=0; i<4; i++)
 		printf("%5.2f ", *(*(point+n)+i));
 	printf("\n");
 }
```

函数指针


```cpp
int(*p)(const char *, const char *);    /* 函数指针 */
 
 	p = strcmp;    /* 将函数strcmp的地址赋给函数指针p */
 	check(s1, s2, p);    /* 通过指针变量p传递函数strcmp的地址 */
 void check(char *a, char *b, int(*cmp)(const char *, const char *))
 {}
```








