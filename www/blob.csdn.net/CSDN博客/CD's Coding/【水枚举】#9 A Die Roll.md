# 【水枚举】#9 A. Die Roll - CD's Coding - CSDN博客





2014年04月10日 16:08:13[糖果天王](https://me.csdn.net/okcd00)阅读数：997
所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)











Yakko, Wakko and Dot, world-famous animaniacs, decided to rest from acting in cartoons, and take a leave to travel a bit. Yakko dreamt to go to Pennsylvania, his Motherland and the Motherland of his ancestors. Wakko thought about Tasmania, its beaches, sun
 and sea. Dot chose Transylvania as the most mysterious and unpredictable place.


But to their great regret, the leave turned to be very short, so it will be enough to visit one of the three above named places. That's why Yakko, as the cleverest, came up with a truly genius idea: let each of the three roll an ordinary six-sided die, and
 the one with the highest amount of points will be the winner, and will take the other two to the place of his/her dreams.


Yakko thrown a die and got Y points, Wakko — W points. It was
 Dot's turn. But she didn't hurry. Dot wanted to know for sure what were her chances to visit Transylvania.


It is known that Yakko and Wakko are true gentlemen, that's why if they have the same amount of points with Dot, they will let Dot win.




Input


The only line of the input file contains two natural numbers Y and W —
 the results of Yakko's and Wakko's die rolls.




Output


Output the required probability in the form of irreducible fraction in format «A/B», where A —
 the numerator, and B — the denominator. If the required probability equals to zero, output «0/1».
 If the required probability equals to 1, output «1/1».




Sample test(s)




input
4 2




output
1/2






Note


Dot will go to Transylvania, if she is lucky to roll 4, 5 or 6 points.





有3个人掷骰子玩，A、B都扔过了，问C不小于这两个的概率，水题啦水题~不说啥啦~

C++：



```cpp
#include<iostream>
using namespace std;

int main(){
	int n,m;
	cin>>n>>m;// max one
	if(n<m) n=m;
	if(n==1) cout<<"1/1"<<endl;
	else if(n==2) cout<<"5/6"<<endl;
	else if(n==3) cout<<"2/3"<<endl;
	else if(n==4) cout<<"1/2"<<endl;
	else if(n==5) cout<<"1/3"<<endl;
	else cout<<"1/6"<<endl;
	return 0;
}
```







Python：



```python
# input
list = raw_input().split()
x = max(int(list[0]) , int(list[1]))

def gcd(a , b):
    if b == 0:
       return a
    return gcd(b , a%b)

a = 6-x+1
b = 6

print "%d/%d" % (a/gcd(a,b) , b/gcd(a,b))
```





