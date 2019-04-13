
# c语言柔性数组 - 嵌入式Linux - CSDN博客

2017年02月09日 17:10:16[写代码的篮球球痴](https://me.csdn.net/weiqifa0)阅读数：346


**[http://www.2cto.com/kf/201404/293519.html](http://www.2cto.com/kf/201404/293519.html)**


\#include "stdio.h"

```python
struct st_type
{
    int i;
	int a[];
};
void main(void)
{
	struct st_type *st_p =(struct st_type *)malloc(sizeof(struct st_type)+100*sizeof(int));
	st_p->i=10;
	memcpy(st_p->a,"i love you!!!!!!",20);
	printf("%d %s\r\n",st_p->i,st_p->a);
}
```
10 i love you!!!!!!
Press any key to continue

