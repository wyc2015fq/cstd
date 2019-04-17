# Maya Calendar（POJ-1008） - Alex_McAvoy的博客 - CSDN博客





2018年08月09日 13:31:36[Alex_McAvoy](https://me.csdn.net/u011815404)阅读数：63








> 
# Problem Description

During his last sabbatical, professor M. A. Ya made a surprising discovery about the old Maya calendar. From an old knotted message, professor discovered that the Maya civilization used a 365 day long year, called Haab, which had 19 months. Each of the first 18 months was 20 days long, and the names of the months were pop, no, zip, zotz, tzec, xul, yoxkin, mol, chen, yax, zac, ceh, mac, kankin, muan, pax, koyab, cumhu. Instead of having names, the days of the months were denoted by numbers starting from 0 to 19. The last month of Haab was called uayet and had 5 days denoted by numbers 0, 1, 2, 3, 4. The Maya believed that this month was unlucky, the court of justice was not in session, the trade stopped, people did not even sweep the floor. 

For religious purposes, the Maya used another calendar in which the year was called Tzolkin (holly year). The year was divided into thirteen periods, each 20 days long. Each day was denoted by a pair consisting of a number and the name of the day. They used 20 names: imix, ik, akbal, kan, chicchan, cimi, manik, lamat, muluk, ok, chuen, eb, ben, ix, mem, cib, caban, eznab, canac, ahau and 13 numbers; both in cycles. 

Notice that each day has an unambiguous description. For example, at the beginning of the year the days were described as follows: 

1 imix, 2 ik, 3 akbal, 4 kan, 5 chicchan, 6 cimi, 7 manik, 8 lamat, 9 muluk, 10 ok, 11 chuen, 12 eb, 13 ben, 1 ix, 2 mem, 3 cib, 4 caban, 5 eznab, 6 canac, 7 ahau, and again in the next period 8 imix, 9 ik, 10 akbal . . . 

Years (both Haab and Tzolkin) were denoted by numbers 0, 1, : : : , where the number 0 was the beginning of the world. Thus, the first day was: 

Haab: 0. pop 0 

Tzolkin: 1 imix 0 

Help professor M. A. Ya and write a program for him to convert the dates from the Haab calendar to the Tzolkin calendar. 

# Input

The date in Haab is given in the following format:

NumberOfTheDay. Month Year

The first line of the input file contains the number of the input dates in the file. The next n lines contain n dates in the Haab calendar format, each in separate line. The year is smaller then 5000. 

# Output

The date in Tzolkin should be in the following format:

Number NameOfTheDay Year

The first line of the output file contains the number of the output dates. In the next n lines, there are dates in the Tzolkin calendar format, in the order corresponding to the input dates. 

# Sample Input

**310. zac 00. pop 010. zac 1995**

# Sample Output

**33 chuen 01 imix 09 cimi 2801**


题意：

给出两种历法，要求输入后转换日期。

对于 Habb 日历，一共有 19 个月，前 18 个月，每月 20 天，用 0-19 表示，最后一个月只有5天，用 0-4 表示。

对于 Tzolkin 日历，一年有 260 天，它属于一个月份的日子以周期出现，月份 20 天一轮回，数字 13 天一轮回。

思路：

先把所给的 Habb 日历转换为天，即记录总天数，然后根据 Tzolkin 日历的计数方式，转换为对应的日期。

注意输入和输出问题，WA 了好多次，查了好久。。。

# Source Program

```cpp
#include<iostream>
#include<cstdio>
#include<cstring>
#include<cmath>
#include<algorithm>
#include<string>
#include<cstdlib>
#include<queue>
#include<set>
#include<map>
#include<stack>
#include<ctime>
#include<vector>
#define INF 0x3f3f3f3f
#define PI acos(-1.0)
#define N 1005
#define MOD 1e9+7
#define E 1e-6
using namespace std;
string Habb[20]={"pop","no","zip","zotz","tzec","xul","yoxkin","mol","chen","yax","zac","ceh","mac","kankin","muan","pax","koyab","cumhu","uayet"};
string Tzolikin[20]={"imix","ik","akbal","kan","chicchan","cimi","manik","lamat","muluk","ok","chuen","eb","ben","ix","mem","cib","caban","eznab","canac","ahau"};
int main()
{
    int n;
    cin>>n;

    cout<<n<<endl;

    for(int i=0;i<n;i++)
    {
        int day,year;
        string month;

        scanf("%d.",&day);
        cin>>month>>year;

        int sum=0;
        for(int j=0;j<19;j++)//不用考虑最后一个月的情况，最后五天过完了直接向年进位
        {
            if(month==Habb[j])
            {
                sum=365*year+j*20+day;
                break;
            }
        }

        day=sum%13+1;
        month=Tzolikin[sum%20];
        year=sum/260;

        cout<<day<<" "<<month<<" "<<year<<endl;

    }
    return 0;
}
```





