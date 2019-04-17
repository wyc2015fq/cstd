# 【水值域】#58 A. A Student's Dream - CD's Coding - CSDN博客





2014年08月20日 11:51:13[糖果天王](https://me.csdn.net/okcd00)阅读数：726标签：[codeforces](https://so.csdn.net/so/search/s.do?q=codeforces&t=blog)
个人分类：[CF-A题终结者](https://blog.csdn.net/okcd00/article/category/2156945)

所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)













Statistics claims that students sleep no more than three hours a day. But even in the world of their dreams, while they are snoring peacefully, the sense of impending doom is still upon them.


A poor student is dreaming that he is sitting the mathematical analysis exam. And he is examined by the most formidable professor of all times, a three times Soviet Union Hero, a Noble Prize laureate in student expulsion, venerable Petr Palych.


The poor student couldn't answer a single question. Thus, instead of a large spacious office he is going to apply for a job to thorium mines. But wait a minute! Petr Palych decided to give the student the last chance! Yes, that is possible only in dreams.


So the professor began: "Once a Venusian girl and a Marsian boy met on the Earth and decided to take a walk holding hands. But the problem is the girl has *a**l* fingers
 on her left hand and *a**r* fingers
 on the right one. The boy correspondingly has *b**l* and *b**r* fingers.
 They can only feel comfortable when holding hands, when no pair of the girl's fingers will touch each other. That is, they are comfortable when between any two girl's fingers there is a boy's finger. And
 in addition, no three fingers of the boy should touch each other. Determine if they can hold hands so that the both were comfortable."


The boy any the girl don't care who goes to the left and who goes to the right. The difference is only that if the boy goes to the left of the girl, he will take her left hand with his right one, and if he goes to the right of the girl, then it is vice versa.




Input


The first line contains two positive integers not exceeding 100. They are the number of fingers on the Venusian girl's left and right hand correspondingly. The second
 line contains two integers not exceeding 100. They are the number of fingers on the Marsian boy's left and right hands correspondingly.




Output


Print YES or NO, that is, the answer to Petr Palych's question.




Sample test(s)




input
5 1
10 5




output
YES



input
4 5
3 3




output
YES



input
1 2
11 6




output
NO





Note


The boy and the girl don't really care who goes to the left.








有一个（让我哭会儿）少年，他梦见了和自己心爱的女孩牵手……为了让两人牵手的时候都觉得舒服，每两个女生的手指间都必须有一个男生的手指，但是每两个女生的手指间不得超过两个男生的手指…… 男生女生的左右手指数告诉你（但是为什么会有100根手指的定义域？！！）问这次牵手能不能舒服……

首先男左女右，男友女左都得考虑

接着，女生的手指数减一是舒服时所需男生手指数最小的时候，最大的时候呢是(女生手指数+1)X2,因为最左和左右的手指外头还能放手指：

所以值域是(x-1,2x+2)



```cpp
#include <cstdio>
#include <memory>
#include <string>
#include <cstring>
#include <iostream>
#include <algorithm>
using namespace std;
int vl,vr,ml,mr;	//girl L/R & boy L/R 

bool judge(int g, int b)//girl & boy
{
	if(b<g-1)	return false;
	else if(b>2*g+2) return false;
	else return true;
}

int main()
{
	cin>>vl>>vr>>ml>>mr;
	if(judge(vl,mr) || judge(vr,ml))	cout<<"YES"<<endl;
	else cout<<"NO"<<endl;
	return 0;
}
```





