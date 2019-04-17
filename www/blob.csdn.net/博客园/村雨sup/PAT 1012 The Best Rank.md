# PAT 1012 The Best Rank - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)







1012 The Best Rank （25 分)








To evaluate the performance of our first year CS majored students, we consider their grades of three courses only: `C` - C Programming Language, `M` - Mathematics (Calculus or Linear Algrbra), and `E` - English. At the mean time, we encourage students by emphasizing on their best ranks -- that is, among the four ranks with respect to the three courses and the average grade, we print the best rank for each student.

For example, The grades of `C`, `M`, `E` and `A` - Average of 4 students are given as the following:

```
StudentID  C  M  E  A
310101     98 85 88 90
310102     70 95 88 84
310103     82 87 94 88
310104     91 91 91 91
```

Then the best ranks for all the students are No.1 since the 1st one has done the best in C Programming Language, while the 2nd one in Mathematics, the 3rd one in English, and the last one in average.

### Input Specification:

Each input file contains one test case. Each case starts with a line containing 2 numbers 

### Output Specification:

For each of the 

The priorities of the ranking methods are ordered as `A`> `C`> `M`> `E`. Hence if there are two or more ways for a student to obtain the same best rank, output the one with the highest priority.

If a student is not on the grading list, simply output `N/A`.

### Sample Input:

```
5 6
310101 98 85 88
310102 70 95 88
310103 82 87 94
310104 91 91 91
310105 85 90 90
310101
310102
310103
310104
310105
999999
```

### Sample Output:

```
1 C
1 M
1 E
1 A
3 A
N/A
```

```
#include<bits/stdc++.h>
using namespace std;
typedef long long ll;
#define maxnum 100005
int n,m;
struct node{
    string s;
    int a,b,c,d;
};


struct cnm{
    int x;
    char q;
};

bool comp(const cnm a, const cnm b){
    if(a.x == b.x){
        if(a.q=='A') return 1;
        else if(a.q == 'C'&&b.q != 'A') return 1;
        else if(a.q == 'M'&&b.q != 'A'&&b.q != 'C') return 1;
        else return 0;
    }
    return a.x < b.x;

}


int main(){

    cin >> n >> m;
    node nod[n];
    for(int i=0;i < n;i++){
        cin >> nod[i].s >> nod[i].a >> nod[i].b >> nod[i].c;
        nod[i].d = (nod[i].a+nod[i].b+nod[i].c+0.5)/3;
    }

    map<int,int>mp;
    vector<int>temp;
    int cnt,beforenum;
    for(int i=0;i < n;i++){
        temp.push_back(nod[i].a);
    }
    sort(temp.begin(),temp.end());
    cnt = 1;
    beforenum = temp[temp.size()-1];
    for(int i=temp.size()-1;i>=0;i--){
        if(beforenum == temp[i])mp[temp[i]] = cnt;
        else{
            cnt = temp.size()-i;
            mp[temp[i]] = cnt;
        }
        beforenum = temp[i];
    }
    for(int i=0;i < n;i++){
        nod[i].a = mp[nod[i].a];
    }
    temp.clear();mp.clear();





    for(int i=0;i < n;i++){
        temp.push_back(nod[i].b);
    }
    sort(temp.begin(),temp.end());
    cnt = 1;
    beforenum = temp[temp.size()-1];
    for(int i=temp.size()-1;i>=0;i--){
        if(beforenum == temp[i])mp[temp[i]] = cnt;
        else{
            cnt = temp.size()-i;
            mp[temp[i]] = cnt;
        }
        beforenum = temp[i];
    }
    for(int i=0;i < n;i++){
        nod[i].b = mp[nod[i].b];
    }
    temp.clear();mp.clear();





    for(int i=0;i < n;i++){
        temp.push_back(nod[i].c);
    }
    sort(temp.begin(),temp.end());
    cnt = 1;
    beforenum = temp[temp.size()-1];
    for(int i=temp.size()-1;i>=0;i--){
        if(beforenum == temp[i])mp[temp[i]] = cnt;
        else{
            cnt = temp.size()-i;
            mp[temp[i]] = cnt;
        }
        beforenum = temp[i];
    }
    for(int i=0;i < n;i++){
        nod[i].c = mp[nod[i].c];
    }
    temp.clear();mp.clear();





    for(int i=0;i < n;i++){
        temp.push_back(nod[i].d);
    }
    sort(temp.begin(),temp.end());
    cnt = 1;
    beforenum = temp[temp.size()-1];
    for(int i=temp.size()-1;i>=0;i--){
        if(beforenum == temp[i])mp[temp[i]] = cnt;
        else{
            cnt = temp.size()-i;
            mp[temp[i]] = cnt;
        }
        beforenum = temp[i];
    }
    for(int i=0;i < n;i++){
        nod[i].d = mp[nod[i].d];
    }
    temp.clear();mp.clear();



    while(m--){
        string s1; cin >> s1;
        int pos = -1;
        for(int i=0;i < n;i++){
            if(nod[i].s == s1){pos = i;}
        }
        if(pos == -1){
            cout << "N/A" << endl;
        }
        else{
           cnm sb[4];
           sb[0].x = nod[pos].a;sb[0].q = 'C';
           sb[1].x = nod[pos].b;sb[1].q = 'M';
           sb[2].x = nod[pos].c;sb[2].q = 'E';
           sb[3].x = nod[pos].d;sb[3].q = 'A';
           sort(sb,sb+4,comp);
           cout << sb[0].x << " " << sb[0].q << endl;
        }

    }











    return 0;
}

//    for(auto num:temp)cout << num << " ";

//    map<int,int>::iterator it;
//    for(it=mp.begin();it!=mp.end();it++){
//        cout << it->first << " " << it->second << " ";
//    }
```

_写的超多。。这种题真的恶心。

复习了优先排序

排名并列应该1、1、3、4、5，而不是1、1、2、3、4，否则会有一个测试点不过

平均分是四舍五入的，所以需要按照+0.5后取整，保证是四舍五入的
` `













