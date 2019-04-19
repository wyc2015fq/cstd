# fstream从txt文件中读取指定一行 - sinat_31425585的博客 - CSDN博客
2016年09月17日 11:50:09[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：2824
记忆力差的孩纸得勤做笔记！
最近写一段程序需要从txt文件中读取指定行，现在将demo共享出来！，好了，多的不说，直接上demo！
```cpp
#include <string>
#include <fstream>
#include <iostream>
using namespace std;
char readline(int idx);
int main()
{
	char str1[60];
	int i = 1;
	str1[60] = readline(i);
	return 0;
}
char readline(int idx)
{
	ifstream in("data//1.txt");
	char str[60];
	int icount = 0;
	while(in.getline(str,60))
	{
		icount++;
		if(icount == idx)
		{
			cout << str << endl;
			return str[60];
			break;
		}
		
	}
}
```
需要注意的一点是，存储一行的数组str的大小需要指定，太小将读取失败，太大处理数据速度又太慢！好了，打完收工~
            
