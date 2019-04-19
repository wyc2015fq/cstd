# 算法笔记：简单的字符串模式匹配-KMP算法（与BF算法对比时间复杂度） - Koma Hub - CSDN博客
2018年04月21日 10:29:14[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：550
简单的讲就是字符串不回溯。
```cpp
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int countBF = 0;
int countKMP = 0;
int BF(char *A, char *a)
{
	printf("\n-------BF-------\n");
	int j = 0, i = 0, ispace;
	printf("%s\n",A);
	while(A[i] != '\0')
	{
		if(j<strlen(a))//a[j] != '\0')
		{
			if(A[i] == a[j])
			{
				printf("%c",a[j]);
				j ++;
				i ++;
				countBF ++;
			}
			else 
			{
				printf("%c\n",a[j]);
				i = i - j +1;
				j = 0;
				for(ispace = 0; ispace < i; ispace++)
					printf("%c",' ');
				countBF ++;
			}
		}
		if(j == strlen(a))
		{
			printf("\n-----------------\n");
			return i - j;
		}
	}
	printf("\n-----------------\n");
	return -1;
}
int *next;
void KMP_NEXT(char *a)
{
	next = malloc(sizeof(int)*(int)strlen(a));
	int j = 0, i;
	next[0] = next[1] = 0;
	
	for(i = 2; i < (int)strlen(a); i++)
	{
		while(j > 0 && a[i-1] != a[j])
		{
			countKMP ++;
			j = next[j];
		}
		if( a[j] == a[i-1] )
		{
			countKMP ++;
			j ++;
		}
		next[i] = j;
	}
}
int KMP(char *A, char *a)
{
	printf("\n-------KMP-------\n");
	printf("%s\n",A);
	int i, j = 0, ispace;
	for(i=0;i<strlen(A);i++)
	{
		while(j>0 && a[j] != A[i])
		{
			printf("%c\n",a[j]);
			for(ispace = 0; ispace < i; ispace++)
					printf("%c",' ');
			j = next[j];
			countKMP ++;
		}
		if(A[i] == a[j])
		{
			printf("%c",a[j]);
			j++;
			countKMP ++;
		}
		if(j == strlen(a))
		{
			printf("\n-----------------\n");
			return i-j+1;
		}
	}
	printf("\n-----------------\n");
	return -1;
}
int main(int argc, char **argv)
{
	char *A, *a;
	A = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabc";
	a = "aaaaaaaaaaaaabc";
	printf("A(%2d): %s\n",strlen(A),A);
	printf("a(%2d): %s\n",strlen(a),a);
	int bf = BF(A,a);
	KMP_NEXT(a);
	int kmp = KMP(A,a);
	
	printf("next(%2d):",strlen(a));
	int i;
	for (i=0;i<strlen(a);i++)
		printf("%d,",next[i]);
	printf("\n");
	
	printf("BF  pos: %d, O(%d)\n",bf,countBF);
	printf("KMP pos: %d, O(%d)\n",kmp,countKMP);
	
	return 0;
}
```
测试字符串：
```cpp
A = "aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabc";
a = "aaaaaaaaaaaaabc";
```
结果：
```
-------BF-------
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabc
aaaaaaaaaaaaab
 aaaaaaaaaaaaab
  aaaaaaaaaaaaab
   aaaaaaaaaaaaab
    aaaaaaaaaaaaab
     aaaaaaaaaaaaab
      aaaaaaaaaaaaab
       aaaaaaaaaaaaab
        aaaaaaaaaaaaab
         aaaaaaaaaaaaab
          aaaaaaaaaaaaab
           aaaaaaaaaaaaab
            aaaaaaaaaaaaab
             aaaaaaaaaaaaab
              aaaaaaaaaaaaab
               aaaaaaaaaaaaab
                aaaaaaaaaaaaab
                 aaaaaaaaaaaaab
                  aaaaaaaaaaaaab
                   aaaaaaaaaaaaab
                    aaaaaaaaaaaaab
                     aaaaaaaaaaaaab
                      aaaaaaaaaaaaab
                       aaaaaaaaaaaaab
                        aaaaaaaaaaaaab
                         aaaaaaaaaaaaab
                          aaaaaaaaaaaaab
                           aaaaaaaaaaaaab
                            aaaaaaaaaaaaab
                             aaaaaaaaaaaaab
                              aaaaaaaaaaaaabc
-----------------
-------KMP-------
aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaabc
aaaaaaaaaaaaab
             ab
              ab
               ab
                ab
                 ab
                  ab
                   ab
                    ab
                     ab
                      ab
                       ab
                        ab
                         ab
                          ab
                           ab
                            ab
                             ab
                              ab
                               ab
                                ab
                                 ab
                                  ab
                                   ab
                                    ab
                                     ab
                                      ab
                                       ab
                                        ab
                                         ab
                                          abc
-----------------
next(15):0,0,1,2,3,4,5,6,7,8,9,10,11,12,0,
BF  pos: 30, O(435)
KMP pos: 30, O(99)
```
测试字符串：
```cpp
A = "aaaacaaaaaabaaacaaaaabcaaaaacaaa";
a = "aaaabcaaa";
```
结果：
```
-------BF-------
aaaacaaaaaabaaacaaaaabcaaaaacaaa
aaaab
 aaaa
  aaa
   aa
    a
     aaaab
      aaaab
       aaaabc
        aaaa
         aaa
          aa
           a
            aaaa
             aaa
              aa
               a
                aaaab
                 aaaabcaaa
-----------------
-------KMP-------
aaaacaaaaaabaaacaaaaabcaaaaacaaa
aaaab
    a
    a
    a
    aaaab
         ab
          abc
            aaaa
               a
               a
               aaaab
                    abcaaa
-----------------
next( 9):0,0,1,2,3,0,0,1,2,
BF  pos: 17, O(65)
KMP pos: 17, O(43)
```
参考：
[https://blog.csdn.net/rrrfff/article/details/6539598](https://blog.csdn.net/rrrfff/article/details/6539598)
[http://www.matrix67.com/blog/archives/115](http://www.matrix67.com/blog/archives/115)
[https://blog.csdn.net/zhang0558/article/details/50157903](https://blog.csdn.net/zhang0558/article/details/50157903)
