# CodeForces 957A Tritonic Iridescence - 紫芝的博客 - CSDN博客





2018年06月01日 21:58:11[紫芝](https://me.csdn.net/qq_40507857)阅读数：48








Overlooking the captivating blend of myriads of vernal hues, Arkady the painter lays out a long, long canvas.

Arkady has a sufficiently large amount of paint of three colours: cyan, magenta, and yellow. On the one-dimensional canvas split into *n* consecutive segments, each segment needs to be painted in one of the colours.

Arkady has already painted some (possibly none or all) segments and passes the paintbrush to you. You are to determine whether there are at least two ways of colouring all the unpainted segments so that no two adjacent segments are of the same colour. Two ways are considered different if and only if a segment is painted in different colours in them.


Input        
The first line contains a single positive integer *n* (1 ≤ *n* ≤ 100) — the length of the canvas.

The second line contains a string *s* of *n* characters, the *i*-th of which is either

 'C' (denoting a segment painted in cyan), 'M' (denoting one painted in magenta),

 'Y' (one painted in yellow), or '?' (an unpainted one).
            Output        
If there are at least two different ways of painting, output "Yes"; otherwise output "No" (both without quotes).

You can print each character in any case (upper or lower).
            Examples        


   Input  
5
CY??Y



   Output  
Yes



   Input  
5
C?C?Y



   Output  
Yes



   Input  
5
?CYC?



   Output  
Yes



   Input  
5
C??MM



   Output  
No



   Input  
3
MMY



   Output  
No


            Note        
For the first example, there are exactly two different ways of colouring: CYCMY and CYMCY.

For the second example, there are also exactly two different ways of colouring: CMCMY and CYCMY.

For the third example, there are four ways of colouring: MCYCM, MCYCY, YCYCM, and YCYCY.

For the fourth example, no matter how the unpainted segments are coloured, the existing

 magenta segments will prevent the painting from satisfying the requirements.

 The similar is true for the fifth example.


题意：
给一个长度为n的字符串，如果有两个相邻的字符相等，就输出NO，
把字符串中的？改成C,M,Y，如果有两种或者两种以上的改法，就输出YES




```cpp
#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;
char str[105];
int n;
int main()
{
	scanf("%d",&n);
	scanf("%s",str);
	for(int i=0;i<n;i++){
		if(str[i] == str[i+1] && str[i] != '?'){
			printf("No\n");
			return 0;
		}
	}
	for(int i=1;i<n-1;i++){
		if(str[i] == '?'){
			if(str[i-1] == str[i+1] || str[i+1] == '?'){
				printf("Yes\n");
				return 0;
			}
		}
	}
	if(str[0] == '?' || str[n-1] == '?'){
		printf("Yes\n");
		return 0;
	}
	printf("No\n");
	return 0;
}
```




