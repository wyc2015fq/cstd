# 【水枚举】#12 A. Super Agent - CD's Coding - CSDN博客





2014年04月26日 00:19:25[糖果天王](https://me.csdn.net/okcd00)阅读数：670标签：[c++																[codeforces																[水题																[python](https://so.csdn.net/so/search/s.do?q=python&t=blog)
个人分类：[CF-A题终结者](https://blog.csdn.net/okcd00/article/category/2156945)

所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)](https://so.csdn.net/so/search/s.do?q=水题&t=blog)








There is a very secret base in Potatoland where potato mash is made according to a special recipe. The neighbours from Porridgia decided to seize this recipe and to sell it to Pilauland. For this mission they have been preparing special agent Pearlo for many
 years. When, finally, Pearlo learned all secrets of espionage, he penetrated into the Potatoland territory and reached the secret base.


Now he is standing at the entrance, but to get inside he need to pass combination lock. Minute ago one of the workers entered the password on the terminal and opened the door. The terminal is a square digital keyboard 3 × 3with
 digits from 1 to 9.


Pearlo knows that the password consists from distinct digits and is probably symmetric with respect to the central button of the terminal. He has heat sensor which allowed him to detect the digits which the worker pressed. Now he wants to check whether the
 password entered by the worker is symmetric with respect to the central button of the terminal. This fact can Help Pearlo to reduce the number of different possible password combinations.




Input


Input contains the matrix of three rows of three symbols each. Symbol «X» means that the corresponding button was pressed, and «.»
 means that is was not pressed. The matrix may contain no «X», also it may contain no «.».




Output


Print YES if the password is symmetric with respect to the central button of the terminal and NO otherwise.




Sample test(s)




input
XX.
...
.XX




output
YES




input
X.X
X..
...




output
NO



一个判断图形是否对称的题目，鉴于限定了3X3，所以连剪枝啥的都不要想了，n=3的时候感觉就算n^5、n^10都没啥了……暴力枚举咯~






Python:



```python
# input  
matrix = []  
for i in range(3):  
    matrix.append(raw_input())  
# solve  
def isOk():  
    for i in range(3):  
        for j in range(3):  
            x = 2-i  
            y = 2-j  
            if matrix[i][j] != matrix[x][y]:  
               return False  
    return True  
# ouput  
if isOk():  
    print "YES"  
else:  
    print "NO"
```


C++:



```cpp
#include <cstdio>
using namespace std;

int main()
{
	char s[3][3];
	char a;
	for(int i=0;i<3;i++)
	{ 
		for(int j=0;j<3;j++)
			scanf("%c",&s[i][j]);
		scanf("%c",&a);
	} 
	printf((s[0][0]==s[2][2]&&s[0][1]==s[2][1]&&s[0][2]==s[2][0]&&s[1][0]==s[1][2])?"YES":"NO");
	return 0; 
}
```](https://so.csdn.net/so/search/s.do?q=codeforces&t=blog)](https://so.csdn.net/so/search/s.do?q=c++&t=blog)




