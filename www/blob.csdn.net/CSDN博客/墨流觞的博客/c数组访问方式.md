# c数组访问方式 - 墨流觞的博客 - CSDN博客





2018年12月05日 13:20:40[墨氲](https://me.csdn.net/dss_dssssd)阅读数：36








```
int arr[5] = { 0,1,2,3,4};
	int len = sizeof(arr) / sizeof(int);
	int *p=arr;
	//指针遍历1
	printf("指针遍历1\n");
	for (; p < arr + len; ++p)
	{
		
		printf("%#x,%d\n", p, *p);
	}
	//指针遍历2
	p = arr;
	printf("指针遍历2\n");
	for (int i=0; i < len; ++i)
	{
		
		printf("%#x,%d\n", p + i, *(p + i));
	}
	//下标遍历
	p = arr;
	printf("下标遍历\n");
	for (int i = 0; i < len; ++i)
	{
		 //等价于printf("%#x,%d\n", a + i, a[i]);
		printf("%#x,%d\n", p + i, p[i]);
	}
	//逆向遍历，反向输出
	printf("反向遍历\n");
	for (p = arr + len - 1; p >= arr; --p)
	{

		printf("%#x,%d\n", p, *p);
	}
```

out:

> 
指针遍历1

0x133fbc4,0

0x133fbc8,1

0x133fbcc,2

0x133fbd0,3

0x133fbd4,4

指针遍历2

0x133fbc4,0

0x133fbc8,1

0x133fbcc,2

0x133fbd0,3

0x133fbd4,4

下标遍历

0x133fbc4,0

0x133fbc8,1

0x133fbcc,2

0x133fbd0,3

0x133fbd4,4

反向遍历

0x133fbd4,4

0x133fbd0,3

0x133fbcc,2

0x133fbc8,1

0x133fbc4,0




