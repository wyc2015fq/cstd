
# C语言产随机数 - 嵌入式Linux - CSDN博客

2014年06月03日 09:26:20[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：659个人分类：[C																](https://blog.csdn.net/weiqifa0/article/category/1389385)



```python
#include "stdio.h"
#include "stdlib.h"
#include "time.h"
void main(void)
{
	int i;
	srand((unsigned)time(NULL));
	while(1){
		i=rand()%100;
		printf("%d\n",i);
	}
}
```


