# 1025 PAT Ranking——C/C++ - 心纯净，行致远 - CSDN博客





2018年12月05日 11:05:55[吉大秦少游](https://me.csdn.net/zhanshen112)阅读数：96








# Question

> 
1025 PAT Ranking （25 point(s)）

Programming Ability Test (PAT) is organized by the College of Computer Science and Technology of Zhejiang University. Each test is supposed to run simultaneously in several places, and the ranklists will be merged immediately after the test. Now it is your job to write a program to correctly merge all the ranklists and generate the final rank.

### Input Specification:

Each input file contains one test case. For each case, the first line contains a positive number N (≤100), the number of test locations. Then N ranklists follow, each starts with a line containing a positive integer K (≤300), the number of testees, and then K lines containing the registration number (a 13-digit number) and the total score of each testee. All the numbers in a line are separated by a space.

### Output Specification:

For each test case, first print in one line the total number of testees. Then print the final ranklist in the following format:

```
registration_number final_rank location_number local_rank
```

The locations are numbered from 1 to N. The output must be sorted in nondecreasing order of the final ranks. The testees with the same score must have the same rank, and the output must be sorted in nondecreasing order of their registration numbers.

### Sample Input:

```
2
5
1234567890001 95
1234567890005 100
1234567890003 95
1234567890002 77
1234567890004 85
4
1234567890013 65
1234567890011 25
1234567890014 100
1234567890012 85
```

### Sample Output:

```
9
1234567890005 1 1 1
1234567890014 1 2 1
1234567890001 3 1 2
1234567890003 3 1 2
1234567890004 5 1 4
1234567890012 5 2 2
1234567890002 7 1 5
1234567890013 8 2 3
1234567890011 9 2 4
```


# Algorithm

this question is very common . And it always be solved by struct and sort. In the process ,it must be logical and clear of our mind.

# code

1、final_rank and local_rank are in the struct node

```cpp
#include <iostream>
#include <algorithm>
using namespace std;
struct node{
      string id;
      int rank1,score,location_id,rank2;
};
bool cmp(node a, node b){
      return a.score!=b.score?a.score>b.score:a.id<b.id;
}
int main()
{
    int N,num=0;  cin>>N;
    node stu[30025];    //size need over 100*300
    for(int i=1;i<=N;i++){
      int cnt;    cin>>cnt;
      for(int j=0;j<cnt;j++){
            cin>>stu[num].id>>stu[num].score;
            stu[num].location_id=i;
            num++;
      }
      sort(stu+num-cnt,stu+num,cmp);
      stu[num-cnt].rank2=1;
      for(int i=num-cnt+1;i<num;i++){
            if(stu[i].score==stu[i-1].score)    stu[i].rank2=stu[i-1].rank2;
            else stu[i].rank2=i+1-(num-cnt);
      }
    }
    cout<<num<<endl;
    sort(stu,stu+num,cmp);
    stu[0].rank1=1;
    for(int i=1;i<num;i++){
          if(stu[i].score!=stu[i-1].score)   stu[i].rank1=i+1;
          else stu[i].rank1=stu[i-1].rank1;
    }
    for(int i=0;i<num;i++)    cout<<stu[i].id<<" "<<stu[i].rank1<<" "<<stu[i].location_id<<" "<<stu[i].rank2<<endl;
    return 0;
}
```

2、only local_rank in the struct node

```cpp
#include <iostream>
#include <algorithm>
using namespace std;
struct node{
      string id;
      int rank1,score,location_id,rank2;
};
bool cmp(node a, node b){
      return a.score!=b.score?a.score>b.score:a.id<b.id;
}
int main()
{
    int N,num=0;  cin>>N;
    node stu[30005];    //size need over 100*300
    for(int i=1;i<=N;i++){
      int cnt;    cin>>cnt;
      for(int j=0;j<cnt;j++){
            cin>>stu[num].id>>stu[num].score;
            stu[num].location_id=i;
            num++;
      }
      sort(stu+num-cnt,stu+num,cmp);
      stu[num-cnt].rank2=1;
      for(int i=num-cnt+1;i<num;i++){
            if(stu[i].score==stu[i-1].score)    stu[i].rank2=stu[i-1].rank2;
            else stu[i].rank2=i+1-(num-cnt);
      }
    }
    cout<<num<<endl;
    sort(stu,stu+num,cmp);
    int r=1;
    for(int i=0;i<num;i++){
          if(i>0&&stu[i].score!=stu[i-1].score)   r=i+1;
          cout<<stu[i].id<<" "<<r<<" "<<stu[i].location_id<<" "<<stu[i].rank2<<endl;
    }
    return 0;
}
```





