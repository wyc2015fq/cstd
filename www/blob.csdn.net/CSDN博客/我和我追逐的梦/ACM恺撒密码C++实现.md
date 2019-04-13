
# ACM恺撒密码C++实现 - 我和我追逐的梦~~~ - CSDN博客


置顶2011年10月31日 22:21:18[一只鸟的天空](https://me.csdn.net/heyongluoyao8)阅读数：3419


恺撒的密码
Description
恺撒时代充满了动荡和危险，恺撒为了保证在战争中传递秘密消息，发明了一种密码。他在所有的信件中将所有的字符按字母顺序向后移动了5个位置（比如说，原文中是A，那么密信中就为F），密信中字母和原文中字母的对应关系如下：
密文：A B C D E F G H I J K L M N O P Q R S T U V W X Y Z
原文：V W X Y Z A B C D E F G H I J K L M N O P Q R S T U
只有字母被替换，而且所有字母都是大写的。
输入：
第一行是一个整数K，表示有多少个测试用例，以后每行一个测试用例。每行为一个字符串（长度不超过1000个字符）。
输出：
每行输出一个测试用例的结果。
Sample Input
3
NS BFW, JAJSYX TK NRUTWYFSHJ FWJ YMJ WJXZQY TK YWNANFQ HFZXJX
N BTZQI WFYMJW GJ KNWXY NS F QNYYQJ NGJWNFS ANQQFLJ YMFS XJHTSI NS WTRJ
IFSLJW PSTBX KZQQ BJQQ YMFY HFJXFW NX RTWJ IFSLJWTZX YMFS MJ
Sample Output
IN WAR, EVENTS OF IMPORTANCE ARE THE RESULT OF TRIVIAL CAUSES
I WOULD RATHER BE FIRST IN A LITTLE IBERIAN VILLAGE THAN SECOND IN ROME
DANGER KNOWS FULL WELL THAT CAESAR IS MORE DANGEROUS THAN HE
\#include<iostream>
using namespace std;
char str[1000];
int main()
{
int k;
cin>>k;
while(k--)
{
getchar();
for(int j=0; str[j-1]!='\n';j++)
str[j]=getchar();
int len = strlen(str);
int i = 0;
for( i=0 ; i<len; i++)
{
if('A' <= str[i]&&str[i]<='E')
{
str[i]=(char)(str[i]+21);
}
else if('F'<=str[i]&&str[i]<='Z')
{
str[i]=(char)(str[i]-5);
}
else
str[i]=str[i];
}
for(i = 0;i<len;i++)
cout<<str[i];
}
return 0;
}


