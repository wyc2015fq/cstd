# 【简单数论】#60 A. Harry Potter and Three Spells - CD's Coding - CSDN博客





2014年08月20日 12:09:24[糖果天王](https://me.csdn.net/okcd00)阅读数：556标签：[codeforces](https://so.csdn.net/so/search/s.do?q=codeforces&t=blog)
个人分类：[CF-A题终结者](https://blog.csdn.net/okcd00/article/category/2156945)

所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)


















A long time ago (probably even in the first book), Nicholas Flamel, a great alchemist and the creator of the Philosopher's Stone, taught Harry Potter three useful spells. The first one allows you to convert *a*grams
 of sand into *b* grams of lead, the second one allows you to convert *c* grams
 of lead into *d* grams of gold and third one allows you to convert *e* grams
 of gold into *f* grams of sand. When Harry told his friends about these spells, Ron Weasley was amazed. After all, if they succeed in turning sand into lead,
 lead into gold, and then turning part of the gold into sand again and so on, then it will be possible to start with a small amount of sand and get huge amounts of gold! Even an infinite amount of gold! Hermione Granger, by contrast, was skeptical about that
 idea. She argues that according to the law of conservation of matter getting an infinite amount of matter, even using magic, is impossible. On the contrary, the amount of matter may even decrease during transformation, being converted to magical energy. Though
 Hermione's theory seems convincing, Ron won't believe her. As far as Ron is concerned, Hermione made up her law of conservation of matter to stop Harry and Ron wasting their time with this nonsense, and to make them go and do homework instead. That's why Ron
 has already collected a certain amount of sand for the experiments. A quarrel between the friends seems unavoidable...


Help Harry to determine which one of his friends is right, and avoid the quarrel after all. To do this you have to figure out whether it is possible to get the amount of gold greater than any preassigned number from some finite amount of sand.




Input


The first line contains 6 integers *a*, *b*, *c*, *d*, *e*, *f* (0 ≤ *a*, *b*, *c*, *d*, *e*, *f* ≤ 1000).




Output


Print "Ron", if it is possible to get an infinitely large amount of gold having a certain finite amount of sand (and not having any gold and lead at all), i.e., Ron is right. Otherwise, print "Hermione".




Sample test(s)




input
100 200 250 150 200 250




output
Ron




input
100 50 50 200 200 100




output
Hermione




input
100 10 200 20 300 30




output
Hermione




input
0 0 0 0 0 0




output
Hermione




input
1 1 0 1 1 1




output
Ron




input
1 0 1 2 1 2




output
Hermione




input
100 1 100 1 0 1




output
Ron






Note


Consider the first sample. Let's start with the 500 grams of sand. Apply the first spell 5 times
 and turn the sand into 1000 grams of lead. Then apply the second spell 4 times
 to get 600 grams of gold. Let’s take 400 grams from the resulting
 amount of gold turn them back into sand. We get 500 grams of sand and 200 grams
 of gold. If we apply the same operations to 500 grams of sand again, we can get extra 200 grams
 of gold every time. Thus, you can get 200, 400, 600 etc.
 grams of gold, i.e., starting with a finite amount of sand (500 grams), you can get the amount of gold which is greater than any preassigned number.


In the forth sample it is impossible to get sand, or lead, or gold, applying the spells.


In the fifth sample an infinitely large amount of gold can be obtained by using only the second spell, which allows you to receive 1 gram of gold out of nothing.
 Note that if such a second spell is available, then the first and the third one do not affect the answer at all.


The seventh sample is more interesting. We can also start with a zero amount of sand there. With the aid of the third spell you can get sand out of nothing. We get 10000 grams
 of sand in this manner. Let's get 100 grams of lead using the first spell 100 times.
 Then make 1 gram of gold from them. We managed to receive 1 gram of
 gold, starting with a zero amount of sand! Clearly, in this manner you can get an infinitely large amount of gold.







有三种物质，罗恩说我有三个法术可以让它们按顺序转化，所以可以获得无限金，赫敏说不可能

法术1： a克沙->b克铅

法术2： c克铅->d克金

法术3： e克金->f克沙




简单数论，但是神烦……很多东西都可能考虑不到

1）大家肯定立马想到的就是绕一圈回来有没有多，变换率>1.0肯定就Ron了对不对？

但是还有 2）如果0克铅可以换非0克金，也是无限

以及必须需要 3）如果d为0，打死都得不到一点金

然后我没想到的是4）ab都是0的时候无法获得铅，就算铅换金很划算也没有意义







```cpp
#include <cstdio>
using namespace std;

int main()
{
	int a,b,c,d,e,f;
	while (scanf("%d%d%d%d%d%d",&a,&b,&c,&d,&e,&f)!=EOF) 
	{
		if(d && (!c || !a&&b) || a*c*e < b*d*f)	printf("Ron\n");
        else	printf("Hermione\n");
	}
	return 0;
}
```
















