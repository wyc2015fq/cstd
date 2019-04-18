# Lucene中的VInt(可变长度整形) - Orisun - 博客园







# [Lucene中的VInt(可变长度整形)](https://www.cnblogs.com/zhangchaoyang/articles/2670928.html)





可变长度的整型定义：最高位表示是否还有字节要读取，低七位就是就是具体的有效位，添加到

结果数据中。比如00000001 最高位表示0，那么说明这个数就是一个字节表示，有效位是后面的七位0000001，值为1。10000010 00000001 第一个字节最高位为1，表示后面还有字节，第二个字节最高位0表示到此为止了，即就是两个字节。

注意低位在前，高位在后。所以“10000010 00000001”去年符号位后是“00000010 00000001”。高位是“00000001”，表示128，低位是“00000010”，表示2。所以整个数表示130。

int num=0xffffffff;		//num=-1


**VInt Encoding Example**
|Value|First byte|Second byte|Third byte|
|----|----|----|----|
|0|00000000|||
|1|00000001|||
|2|00000010|||
|...||||
|127|01111111|||
|128|10000000|00000001||
|129|10000001|00000001||
|130|10000010|00000001||
|...||||
|16,383|11111111|01111111||
|16,384|10000000|10000000|00000001|
|16,385|10000001|10000000|00000001|
|...||||

下面写个程序，给定无符号整数，输出它的VInt格式

#include<stdlib.h>
#include<stdio.h>
#include<string.h>

//把一个char输出成二进制
char *toBi(char ch){
	char *rect=(char*)calloc(8,sizeof(char));
	int i;
	for(i=0;i<8;++i){
		if(ch & 1U)
			rect[7-i]='1';
		else
			rect[7-i]='0';
		ch >>= 1;		//注意ch>>1移位操作并不改变数据本身
	}
}

void VInt(unsigned int number){
	while(1){
		char ch=number & 0x7f;
		number >>= 7;
		if(number > 0)
			ch |= 0x80;
		char *str=toBi(ch);
		str[8]='\0';	//C语言是不检查数组越界的，给str加一个字符串结束符是因为下面要输出str
		printf("%s ",str);
		free(str);
		if(number <=0 )
			break;
	}
	printf("\n");
}

main(){
	int a;
	printf("Input a number.\n");
	while(1){
		scanf("%d",&a);
		VInt((unsigned)a);
	}
}















