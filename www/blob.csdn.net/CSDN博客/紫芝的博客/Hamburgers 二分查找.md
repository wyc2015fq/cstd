# Hamburgers 二分查找 - 紫芝的博客 - CSDN博客





2018年04月05日 22:38:13[紫芝](https://me.csdn.net/qq_40507857)阅读数：59








Polycarpus loves hamburgers very much. He especially adores the hamburgers he makes with his own hands. Polycarpus thinks that there are only three decent ingredients to make hamburgers from: a bread, sausage and cheese. He writes down the recipe of his favorite "Le Hamburger de Polycarpus" as a string of letters 'B' (bread), 'S' (sausage) и 'C' (cheese). The ingredients in the recipe go from bottom to top, for example, recipe "ВSCBS" represents the hamburger where the ingredients go from bottom to top as bread, sausage, cheese, bread and sausage again.

Polycarpus has *n**b* pieces of bread, *n**s* pieces of sausage and *n**c* pieces of cheese in the kitchen. Besides, the shop nearby has all three ingredients, the prices are *p**b*rubles for a piece of bread, *p**s* for a piece of sausage and *p**c* for a piece of cheese.

Polycarpus has *r* rubles and he is ready to shop on them. What maximum number of hamburgers can he cook? You can assume that Polycarpus cannot break or slice any of the pieces of bread, sausage or cheese. Besides, the shop has an unlimited number of pieces of each ingredient.
Input


The first line of the input contains a non-empty string that describes the recipe of "Le Hamburger de Polycarpus". The length of the string doesn't exceed 100, the string contains only letters 'B' (uppercase English B), 'S' (uppercase English S) and 'C' (uppercase English C).

The second line contains three integers *n**b*, *n**s*, *n**c* (1 ≤ *n**b*, *n**s*, *n**c* ≤ 100) — the number of the pieces of bread, sausage and cheese on Polycarpus' kitchen. The third line contains three integers *p**b*, *p**s*, *p**c* (1 ≤ *p**b*, *p**s*, *p**c* ≤ 100) — the price of one piece of bread, sausage and cheese in the shop. Finally, the fourth line contains integer *r* (1 ≤ *r* ≤ 1012) — the number of rubles Polycarpus has.

Please, do not write the %lld specifier to read or write 64-bit integers in С++. It is preferred to use the cin, cout streams or the %I64d specifier.


Output


Print the maximum number of hamburgers Polycarpus can make. If he can't make any hamburger, print 0.


Example




Input
BBBSSC
6 4 1
1 2 3
4



Output
2



Input
BBC
1 10 1
1 10 1
21



Output
7



Input
BSC
1 1 1
1 1 3
1000000000000



Output
200000000001






```cpp
//找到最大可能值，最小可能值，二分查找可能答案 
#include<bits/stdc++.h>
using namespace std;
#define MAX 1e13
	string ham;	
	long long r;
	long long nb,ns,nc,pb,ps,pc;
	long long s=0,b=0,c=0;
bool judge(long long mid)
{
	long long NS=s*mid,NB=b*mid,NC=mid*c;
	long long temp=r;
	temp-=ps*max(0ll,NS-ns);
	temp-=pb*max(0ll,NB-nb);
	temp-=pc*max(0ll,NC-nc);
	return temp>=0;
	}	
int main()
{	
	cin>>ham;
	cin>>nb>>ns>>nc>>pb>>ps>>pc>>r;
	
	for(int i=0;i<ham.length();i++)
	{
		if(ham[i]=='B')
		b++;
		if(ham[i]=='S')
		s++;
		if(ham[i]=='C')
		c++;
		}
	long long l=0,r=MAX,ans;
	while(l<=r){
		long long mid=(l+r)/2;
		if(judge(mid)){
			ans=mid;
			l=mid+1;
		}else{
			r=mid-1;
		}
	}
	cout<<ans<<endl;
	//cout<<0ll<<endl; 
	return 0;
}
```




