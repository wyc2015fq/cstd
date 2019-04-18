# 1043 输出PATest——c实现 - 心纯净，行致远 - CSDN博客





2018年11月03日 12:34:08[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：107








> 
1043 输出PATest （20 point(s)）

给定一个长度不超过 10​4​​ 的、仅由英文字母构成的字符串。请将字符重新调整顺序，按 `PATestPATest....` 这样的顺序输出，并忽略其它字符。当然，六种字符的个数不一定是一样多的，若某种字符已经输出完，则余下的字符仍按 PATest 的顺序打印，直到所有字符都被输出。

### 输入格式：

输入在一行中给出一个长度不超过 10​4​​ 的、仅由英文字母构成的非空字符串。

### 输出格式：

在一行中按题目要求输出排序后的字符串。题目保证输出非空。

### 输入样例：

```
redlesPayBestPATTopTeePHPereatitAPPT
```

### 输出样例：

```
PATestPATestPTetPTePePee
```


# 思路：

这道题分析起来比较容易。只需要用6个int变量存储即可，再依次判断是否非0，非0就打印相关字母。

# 代码：

```cpp
//1043 输出PATest V1
#include <stdio.h>

int main(){
	int P=0,A=0,T=0,e=0,s=0,t=0;
	char c;
	while((c=getchar())!='\n'){
		if(c=='P')	P++;
		if(c=='A')	A++;
		if(c=='T')	T++;
		if(c=='e')	e++;
		if(c=='s')	s++;
		if(c=='t')	t++;
	}
	while(P!=0||A!=0||T!=0||e!=0||s!=0||t!=0){
		if(P!=0){ P--; printf("P");	};
		if(A!=0){ A--; printf("A");	};
		if(T!=0){ T--; printf("T");	};
		if(e!=0){ e--; printf("e");	};
		if(s!=0){ s--; printf("s");	};
		if(t!=0){ t--; printf("t");	};
	}
	return 0;	
}
```





