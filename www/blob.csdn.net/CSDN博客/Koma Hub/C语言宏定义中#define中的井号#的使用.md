# C语言宏定义中#define中的井号#的使用 - Koma Hub - CSDN博客
2018年09月15日 10:26:36[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：76
```cpp
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
typedef enum
{
	E_1,
	E_2,
	E_3,
}ENUM;
//保持原有状态
#define x1(state) \
		f1(state, #state)
//衔接
#define y1(state) \
		f1(state##_1, #state)
int f1(ENUM state, char* s_state)
{
	printf("%d:%s\n", state, s_state);
	return 0;
}
int main(int argc, char *argv[])
{
	x1(E_2);
	y1(E);
	
	return -1;
}
```
结果：
```
$ gcc fns.c
$ ./a.exe
1:E_2
0:E
```
