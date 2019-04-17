# hdu1022——Train Problem I - westbrook1998的博客 - CSDN博客





2018年05月23日 21:56:29[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：35








> 
As the new term comes, the Ignatius Train Station is very busy nowadays. A lot of student want to get back to school by train(because the trains in the Ignatius Train Station is the fastest all over the world ^v^). But here comes a problem, there is only one railway where all the trains stop. So all the trains come in from one side and get out from the other side. For this problem, if train A gets into the railway first, and then train B gets into the railway before train A leaves, train A can’t leave until train B leaves. The pictures below figure out the problem. Now the problem for you is, there are at most 9 trains in the station, all the trains has an ID(numbered from 1 to n), the trains get into the railway in an order O1, your task is to determine whether the trains can get out in an order O2.  

  Input 

  The input contains several test cases. Each test case consists of an integer, the number of trains, and two strings, the order of the trains come in:O1, and the order of the trains leave:O2. The input is terminated by the end of file. More details in the Sample Input.  

  Output 

  The output contains a string “No.” if you can’t exchange O2 to O1, or you should output a line contains “Yes.”, and then output your way in exchanging the order(you should output “in” for a train getting into the railway, and “out” for a train getting out of the railway). Print a line contains “FINISH” after each test case. More details in the Sample Output.  

  Sample Input 

  3 123 321 

  3 123 312 

  Sample Output 

  Yes. 

  in 

  in 

  in 

  out 

  out 

  out 

  FINISH 

  No. 

  FINISH 

  For the first Sample Input, we let train 1 get in, then train 2 and train 3. 

  So now train 3 is at the top of the railway, so train 3 can leave first, then train 2 and train 1. 

  In the second Sample input, we should let train 3 leave first, so we have to let train 1 get in, then train 2 and train 3. 

  Now we can let train 3 leave. 

  But after that we can’t let train 1 leave before train 2, because train 2 is at the top of the railway at the moment. 

  So we output “No.”.
模拟列车出入站，栈的模拟，刚开始想遍模拟边输出，但后来觉得不行，因为开始要输出yes或no，但却只要到最后才能判断yes或no，所以用一个t数组来保存这些出入 

代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <stack>
#include <cstring>
using namespace std;
const int MAXN=1050;
char in[MAXN];
char out[MAXN];
// in 0 out 1
// t数组用来记录列车进出站的记录，最后再统一输出
char t[MAXN];
int n,j,k;
int main(void){
    stack<char> s;
    while(~scanf("%d%s%s",&n,in,out)){
        //注意多组样例，变量数组都要初始化
        j=0;
        k=0;
        while(!s.empty()){
            s.pop();
        }
        memset(t,'0',sizeof(t));
        //入栈
        for(int i=0;i<n;i++){
            s.push(in[i]);
            t[k++]='0';
            //这里一定要注意j<n 和!s.empty()这两个判断！！
            //例子 3 123 213
            //如果栈顶元素和out数组当前元素相同，说明该出栈
            while(j<n && !s.empty() && s.top()==out[j]){
                s.pop();
                j++;
                t[k++]='1';
            }
        }
        //这一遍是防止前面循环只入不出
        while(j<n && s.top()==out[j]){
            s.pop();
            j++;
            t[k++]='1';
        }
        if(k==2*n){
            printf("Yes.\n");
            for(int i=0;i<k;i++){
                if(t[i]=='0'){
                    printf("in\n");
                }
                else{
                    printf("out\n");
                }
            }
            printf("FINISH\n");
        }
        else{
            printf("No.\nFINISH\n");
        }
    }
    return 0;
}
```





