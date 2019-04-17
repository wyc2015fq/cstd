# strlen()&sizeof在静态数组和动态数组中的应用~ - qq_37385726的博客 - CSDN博客





2017年03月25日 13:47:42[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：491








**一.strlen()&sizeof在静态数组中的应用**




```
#include
#include
#include
int main(void)
{
	char ch[20] = "hello";//可以在初始化的时候直接对数组赋一个字符串；
	//但是不能ch="hello"
	printf("strlen(ch)=%d\nsizeof(ch)=%d\n", strlen(ch), sizeof(ch));
	system("pause");
	return 0;
}
```



【结果为5，20】




strlen的结果是到遇到第一个‘\0’就结束。这个数组被初始化为字符串“hello”，‘\0’加在了字符串的后面，所以结果是5

sizeof返回的数组设定时本身的大小，即20






**二.strlen()&sizeof在动态数组中的应用**




```
#include
#include
#include
int main(void)
{
	char *p = (char*)malloc(sizeof(char)* 5);
	printf("strlen(ch)=%d\nsizeof(ch)=%d\n", strlen(p), sizeof(p));
	p = "hello";
	printf("strlen(ch)=%d\nsizeof(ch)=%d\n", strlen(p), sizeof(p));
	free(p);//记得释放哦~
	system("pause");
	return 0;
}
```




【结果为22，4，5，4】






**sizeof的返回结果是指针所占用的字节数，而不是指针所指向内存的宽度，而所有的指针占用的字节数都是4。**

**strlen的结果是到遇到第一个‘\0’就结束。**但是由于这个指针没有被初始化所以计算机随机的赋了一组垃圾，但是虽然我们动态分配的内存是5个字节的，但是，不等于我能占用的空间只有5。所以那一些垃圾可能不止占用了5个字节的空间。

**引用别人的一个例子：**


[分配内存](https://www.baidu.com/s?wd=%E5%88%86%E9%85%8D%E5%86%85%E5%AD%98&tn=44039180_cpr&fenlei=mv6quAkxTZn0IZRqIHckPjm4nH00T1YLuhF9nHwbuWTsnhRsmvRY0ZwV5Hcvrjm3rH6sPfKWUMw85HfYnjn4nH6sgvPsT6KdThsqpZwYTjCEQLGCpyw9Uz4Bmy-bIi4WUvYETgN-TLwGUv3EPHRsPHnzrjTv)，和分地差不多，
分给你一亩地，你种十亩，其实也可以，
不过你用了别人的土地，是要付出代价的。
打架，打官司，乃至被人害了，都是有可能的。
你合法的拥有的可以使用的土地，就只有分给你的那一亩地。
别的都是别人家的。收了别人的庄稼，或者种了别人的地，别人会不高兴的。
至于会出什么状况，看别人有什么反应就知道了。



也就是说如果我们malloc分配5个空间，但是我们输入6个字符的时候，内存会出现问题。虽然程序没有报错的话，但是有可能会影响到旁边的内存数据。如果旁边没有数据，是空内存，就不会有影响，但是如果旁边的内存有数据，就会修改原来的数据，而这个行为导致的影响将是不可估量的。所以一定要注意避免这种情况，并且记得释放内存。

我们还有一个函数是realloc




```
#include
#include
#include
int main(void)
{
	char *p = (char*)malloc(sizeof(char)* 5);
	printf("strlen(ch)=%d\nsizeof(ch)=%d\n", strlen(p), sizeof(p));
	p = "hello";
	printf("strlen(ch)=%d\nsizeof(ch)=%d\n", strlen(p), sizeof(p));
	p = (char*)realloc(p, sizeof(char)* 10);//realloc函数，重新分配内存空间
	free(p);//记得释放哦~
	system("pause");
	return 0;
}
```





**再举一个例子：**



```
#include
#include
#include
int main()
{
	int i;
	int*pn = (int*)malloc(5 * sizeof(int));
	printf("malloc%p\n", pn);
	for (i = 0; i<5; i++)
		pn[i] = i;
	pn = (int*)realloc(pn, 10 * sizeof(int));
	printf("realloc%p\n", pn);
	for (i = 5; i<10; i++)
		pn[i] = i;
	for (i = 0; i<10; i++)
		printf("%3d", pn[i]);
	free(pn);
	system("pause");
	return 0;
}
```




**[结果为  malloc004699C8   realloc004699C8    0 1 2 3 4 5 6 7 8 9]**





1.**当扩大一块内存空间时， realloc()试图直接从堆上现存的数据后面的那些字节中获得附加的字节，如果能够满足，自然天下太平。也就是说，如果原先的内存大小后面还有足够的空闲空间用来分配，加上原来的空间大小= newsize。那么就ok。得到的是一块连续的内存。**

2.**如果原先的内存大小后面没有足够的空闲空间用来分配，那么从堆中另外找一块newsize大小的内存。****并把原来大小内存空间中的内容复制到newsize中。返回新的mem_address指针。（数据被移动了）。老块被放回堆上。**












