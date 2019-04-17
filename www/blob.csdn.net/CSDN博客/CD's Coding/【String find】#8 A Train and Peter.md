# 【String::find】#8 A. Train and Peter - CD's Coding - CSDN博客





2014年04月09日 22:12:28[糖果天王](https://me.csdn.net/okcd00)阅读数：1248
所属专栏：[CF-A Terminator](https://blog.csdn.net/column/details/cf-amaster.html)











Peter likes to travel by train. He likes it so much that on the train he falls asleep.


Once in summer Peter was going by train from city A to city B, and as usual, was sleeping. Then he woke up, started to look through the window and noticed that every railway station has a flag of a particular colour.


The boy started to memorize the order of the flags' colours that he had seen. But soon he fell asleep again. Unfortunately, he didn't sleep long, he woke up and went on memorizing the colours. Then he fell asleep again, and that time he slept till the end of
 the journey.


At the station he told his parents about what he was doing, and wrote two sequences of the colours that he had seen before and after his sleep, respectively.


Peter's parents know that their son likes to fantasize. They give you the list of the flags' colours at the stations that the train passes sequentially on the way from A to B, and ask you to find out if Peter could see those sequences on the way from A to B,
 or from B to A. Remember, please, that Peter had two periods of wakefulness.


Peter's parents put lowercase Latin letters for colours. The same letter stands for the same colour, different letters — for different colours.




Input


The input data contains three lines. The first line contains a non-empty string, whose length does not exceed 105,
 the string consists of lowercase Latin letters — the flags' colours at the stations on the way from A to B. On the way from B to A the train passes the same stations, but in reverse order.


The second line contains the sequence, written by Peter during the first period of wakefulness. The third line contains the sequence, written during the second period of wakefulness. Both sequences are non-empty, consist of lowercase Latin letters, and the
 length of each does not exceed 100 letters. Each of the sequences is written in chronological order.




Output


Output one of the four words without inverted commas:
- 
«forward» — if Peter could see such sequences only on the way from A to B;
- 
«backward» — if Peter could see such sequences on the way from B to A;
- 
«both» — if Peter could see such sequences both on the way from A to B, and on the way from B to A;
- 
«fantasy» — if Peter could not see such sequences.




Sample test(s)




input
atob
a
b




output
forward




input
aaacaaa
aca
aa




output
both






Note


It is assumed that the train moves all the time, so one flag cannot be seen twice. There are no flags at stations A and B.







C++：


这道题大概的意思是说给一个母串，问从左往右看的时候字串a是否在字串b前面，同时母串从右往左看的时候字串a是否在字串b前面。


这个如果慢悠悠的匹配略麻烦（见解法一By ZoeCUR），此处可以使用C++String类里的find函数，判断pos位置来打到效果（见解法二 By okcd00）


//Code 1st By ZoeCUR



```cpp
#include<iostream>
#include<cstdio>
#include<string.h>
using namespace std;
char sqc[100000],fsts[100000],scds[100000];
int main()
{
    
    int rcd_ff_e=-1,rcd_fs_b=-1,rcd_bf_e=-1,rcd_bs_b=-1;
    scanf("%s %s %s",sqc,fsts,scds);
    int sqc_len = strlen(sqc);
    int fsts_len = strlen(fsts);
    int scds_len = strlen(scds);  
    for(int i=0;i<sqc_len;i++)
    {
           if(sqc[i]==fsts[0])
           {
                              int j;
                              bool flagf=1,flagb=1;
                              if(sqc_len-i<fsts_len)flagf=0;
                              if(i<fsts_len-1)flagb=0;
                              if(rcd_ff_e!=-1)flagf=0;
                              for(j=1;j<fsts_len;j++)
                              {
                                                    if(i+j<sqc_len&&sqc[i+j]!=fsts[j])flagf=0;
                                                    if(i-j>=0&&sqc[i-j]!=fsts[j])flagb=0;
                              }
                              if(flagf==1)rcd_ff_e=i+j-1;
                              if(flagb==1)rcd_bf_e=i-j+1;
                             
           }
           if(sqc[i]==scds[0])
           {
                              int j;
                              bool flagf=1,flagb=1;
                              if(sqc_len-i<scds_len)flagf=0;
                              if(i<scds_len-1)flagb=0;
                              if(rcd_bs_b!=-1)flagb=0;
                              for(j=1;j<scds_len;j++)
                              {
                                                     if(i+j<sqc_len&&sqc[i+j]!=scds[j])flagf=0;
                                                     if(i-j>=0&&sqc[i-j]!=scds[j])flagb=0;
                              }  
                              if(flagf==1)rcd_fs_b=i;
                              if(flagb==1)rcd_bs_b=i;
                             
           }
           
    
		   

    }
    int f=0,b=0;
    if(rcd_ff_e!=-1&&rcd_fs_b!=-1&&rcd_ff_e<rcd_fs_b)f=1;
    if(rcd_bf_e!=-1&&rcd_bs_b!=-1&&rcd_bf_e>rcd_bs_b)b=1;
    if(f&&(!b))printf("forward");
    if((!f)&&b)printf("backward");
    if(f&&b)printf("both");
    if((!f)&&(!b))printf("fantasy");
    
    return 0;
}
```


//Code 2nd By okcd00


需要注意的是Str1和Str2不重叠，所以find()的第二个参数需要后移str1的长度







```cpp
#include <iostream>
#include <algorithm>
#include <cstdio>
#include <cstring>
#include <string>
using namespace std;

string sqc,str1,str2;
//unsigned pos1[100001],pos2[100001];
int main()
{
    bool AB=false,BA=false;
    
    cin>>sqc>>str1>>str2;
    
    size_t found1 = sqc.find(str1);//1st position of str1 in (string)sqc 
    //if(found1!=string::npos)cout<< "str1 found at: " << found1 << endl;
    size_t found2 = sqc.find(str2,found1+str1.length());//search 1st position of str2 behind found
    //if(found2!=string::npos)cout<< "str2 found at: " << found2 + found1 << endl;
    if(found1!=string::npos&&found2!=string::npos)AB=true;
    
    reverse(sqc.begin(),sqc.end()); //reverse function exist!!!
    //cout<<sqc<<endl;
    
    size_t found3 = sqc.find(str1);
    //if(found3!=string::npos)cout<< "str1 found at: " << found3 << endl;
    size_t found4 = sqc.find(str2,found3+str1.length());
    //if(found4!=string::npos)cout<< "str2 found at: " << found4 + found3 << endl;
    if(found3!=string::npos&&found4!=string::npos)BA=true;                       
    
    if(AB==true)
                if(BA==true)cout<<"both";
                else cout<<"forward";
                //{ cout<<(BA==true)? "both" : "forward" ;}
    else
                if(BA==true)cout<<"backward";
                else cout<<"fantasy";
                //{ cout<<(BA==true)? "backward" : "fantasy" ;}
    //cin>>sqc;    
    return 0;
}
```











ByTheWay 最近看了编程之美的2013传话游戏，看到了find之后顺便看了rfind，是个好方法的样子，贴一下：




```cpp
// string::rfind
#include <iostream>
#include <string>

int main ()
{
  std::string str ("The sixth sick sheik's sixth sheep's sick.");
  std::string key ("sixth");

  unsigned found = str.rfind(key);
  if (found!=std::string::npos)
    str.replace (found,key.length(),"seventh");

  std::cout << str << '\n';

  return 0;
}

//output:The sixth sick sheik's seventh sheep's sick.
```









这道题其实是个简单模拟，当年那这道题来练习string的stl函数罢了，不要被那么多没见过的函数吓到了呢，一般模拟题的代码看起来都长一些，但其实看懂了之后就是照着题意把他说的用代码模拟出来而已，要加油^_^


```cpp
#include<cstdio>
#include<cstring>
#include<algorithm>
using namespace std;
char s[100005],sa[105],sb[105];
int main()
{
    while(scanf("%s%s%s",s,sa,sb)!=EOF)
    {
        int l=strlen(s),la=strlen(sa),lb=strlen(sb);
        int flag1=0,pos=0,flag2=0,flag3=0,flag4=0;
        for(int i=0;i<l;i++)
        {
            flag1=1;
            for(int j=0;j<la;j++)
                if(s[i+j]!=sa[j]){flag1=0;break;}
            if(flag1==1){pos=i+la;break;}
        }
        if(flag1==1)
        {
            for(int i=pos;i<l;i++)
            {
                flag2=1;
                for(int j=0;j<lb;j++)
                    if(s[i+j]!=sb[j]){flag2=0;break;}
                if(flag2==1)	break;
            }
        }
        for(int i=l-1;i>=0;i--)
        {
            flag3=1;
            for(int j=0;j<la;j++)
                if(s[i-j]!=sa[j]){flag3=0;break;}
            if(flag3==1){pos=i-la;break;}
        }
        for(int i=pos;i>=0;i--)
        {
            flag4=1;
            for(int j=0;j<lb;j++)
                if(s[i-j]!=sb[j]){flag4=0;break;}
            if(flag4==1)	break;
        }
        if(flag2==1 && flag4==1)	printf("both\n");
        else if(flag2==1)	printf("forward\n");
        else if(flag4==1)	printf("backward\n");
        else	printf("fantasy\n");
    }
    return 0;
}
```


------


Update 2014/10/10 艾玛双十萌节我又回来卖萌了……


熟练了string的我回来用string来解这道题，不需要傻乎乎的自己写查找函数，直接调用find就好~


```cpp
#include <cstdio>
#include <string>
#include <cstring> 
#include <iostream>
using namespace std;

int main()
{
	string s,a,b;
	cin>>s>>a>>b;
	string sr="";
	for(int i=s.length()-1;i>=0;i--) sr+=s[i];
	int pa=s.find(a,0),pb=s.find(b,pa+a.length());
	int pra=sr.find(a,0),prb=sr.find(b,pra+a.length());
	//这里的 +xx.length()是为了不重叠哦~ find后的参数是(要找的子串，从第几个位置开始找) 
	bool f1=(pa==string::npos || pb==string::npos),f2=(pra==string::npos || prb==string::npos);
	if(f1 && f2) cout<<"fantasy";
	else if(!f1 && f2) cout<<"forward";
	else if(f1 && !f2) cout<<"backward";
	else cout<<"both";
	return 0;
}
```








