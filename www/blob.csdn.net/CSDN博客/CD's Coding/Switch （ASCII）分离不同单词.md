# Switch （ASCII）分离不同单词 - CD's Coding - CSDN博客





2013年09月24日 20:54:25[糖果天王](https://me.csdn.net/okcd00)阅读数：714
个人分类：[技巧记忆](https://blog.csdn.net/okcd00/article/category/1650755)









                
转自CSDN ζёСяêτ - 小優YoU 的文章

 http://blog.csdn.net/lyy289065406/article/details/6645413






**解题思路：**


模拟题，细心点就好了，没难度。


Habb历一年365天


Tzolkin历一年260天


先计算Habb历从第0天到输入日期的总天数sumday


Sumday/day就是Tzolkin历的年份


Tzolkin历的天数Name每20一循环，先建立Tzolkin历天数Name与1~20的映射，


因此Sumday %20+1就是Tzolkin历的天数Name


Tzolkin历的天数ID每13一循环，且从1开始，则Sumday %13+1就是Tzolkin历的天数ID




```cpp
//POJ1008-Maya Calendar
```


```cpp
//Memory Time  
//264K   0MS 

#include<iostream>
using namespace std;

/*得到Haab历月份对应的数字*/
int GetMonth(char* month)
{
	int ASCII=0;
	for(int i=0;month[i];i++)
		ASCII+=month[i];

	switch(ASCII)
	{
	    case 335:return 1;  //pop
		case 221:return 2;  //no
		case 339:return 3;  //zip
		case 471:return 4;  //zotz
		case 438:return 5;  //tzec
		case 345:return 6;  //xul
		case 674:return 7;  //yoxkin
		case 328:return 8;  //mol
		case 414:return 9;  //chen
		case 338:return 10;  //yax
		case 318:return 11;  //zac
		case 304:return 12;  //ceh
		case 305:return 13;  //mac
		case 636:return 14;  //kankin
		case 433:return 15;  //muan
		case 329:return 16;  //pax
		case 534:return 17;  //koyab
		case 546:return 18;  //cumhu
		case 552:return 19;  //uayet
	}
}
/*计算Haab历从第0天到现在的天数*/
int HaabDay(int day,int month,int year)
{
	int sumday=0;
	for(int i=0;i<year;i++) //Haab历法一年有365天
		sumday+=365;

	for(int j=1;j<month;j++)
		sumday+=20;

	return sumday+day;
}

int main(void)
{
	char TzolkinDayName_Str[21][10]={"0","imix","ik","akbal","kan","chicchan","cimi","manik","lamat","muluk","ok","chuen","eb","ben","ix","mem","cib","caban","eznab","canac","ahau"};
	int day;
	char doc;  //注意输入格式有 "."
	char month[10];
	int year;

	int test;
	cin>>test;
	cout<<test<<endl;
	while(test-- && (cin>>day>>doc>>month>>year))  //日期. 月份 年数
	{
		int sumday=HaabDay(day,GetMonth(month),year);

		int TzolkinYear=sumday/260;   //Tzolkin历法一年有260天
		int TzolkinDayName=sumday%20+1;
		int TzolkinDayId=sumday%13+1;

		cout<<TzolkinDayId<<' '<<TzolkinDayName_Str[TzolkinDayName]<<' '<<TzolkinYear<<endl;
	}
	return 0;
}
```







