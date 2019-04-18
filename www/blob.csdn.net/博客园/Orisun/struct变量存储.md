# struct变量存储 - Orisun - 博客园







# [struct变量存储](https://www.cnblogs.com/zhangchaoyang/articles/2713620.html)





在没有#pragma pack宏的情况下，结构体中的变量存储遵循以下对齐原则：

原则一：第一个*数据成员*放在offset为0的地方，以后每个*数据成员*存储的起始位置要从其自身大小的整数倍开始。

原则二：结构体的总大小，也就是sizeof的结果，必须是其内部最大*数据成员*的整数倍，不足的要补齐。

原则三：如果一个结构里有某些结构体成员，则结构体成员要从其内部最大*数据成员*大小的整数倍地址开始存储。

上面提到的“数据成员”均指基本数据类型。

#include<stdio.h>

struct A{
	char c;
	int i;
	short s;
	
};

struct B{
	int i;
	char c;
	short s;
	
};

struct C{
	char c;
	struct B b;
};

int main(){
	printf("sizeof(char)=%d\n",sizeof(char));
	printf("sizeof(short)=%d\n",sizeof(short));
	printf("sizeof(int)=%d\n",sizeof(int));
	printf("sizeof(long)=%d\n",sizeof(long));
	printf("sizeof(float)=%d\n",sizeof(float));
	printf("sizeof(double)=%d\n",sizeof(double));
	printf("sizeof(struct A)=%d\n",sizeof(struct A));
	printf("sizeof(struct B)=%d\n",sizeof(struct B));
	printf("sizeof(struct C)=%d\n",sizeof(struct C));
}


sizeof(char)=1
sizeof(short)=2
sizeof(int)=4
sizeof(long)=4
sizeof(float)=4
sizeof(double)=8
sizeof(struct A)=12
sizeof(struct B)=8
sizeof(struct C)=12

struct A中char c放在0号位置，int i放在从4（4是sizeof(int)的整倍数）开始的位置,short s放在从8（8是sizeof(short)的整倍数）开始的位置,再根据原则二可得sizeof(struct A)=12。

struct B中int i放在从0开始的位置，char c放在从4开始的位置，short s放在从6开始的位置，所以sizeof(struct B)=8。

struct C中char c放在0号位置，b放在从4开始的位置。












