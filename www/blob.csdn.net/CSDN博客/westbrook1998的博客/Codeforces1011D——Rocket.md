# Codeforces1011D——Rocket - westbrook1998的博客 - CSDN博客





2018年08月24日 13:51:29[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：62








> 
This is an interactive problem. 

  Natasha is going to fly to Mars. Finally, Natasha sat in the rocket. She flies, flies… but gets bored. She wishes to arrive to Mars already! So she decides to find something to occupy herself. She couldn’t think of anything better to do than to calculate the distance to the red planet. 

  Let’s define x as the distance to Mars. Unfortunately, Natasha does not know x. But it is known that 1≤x≤m, where Natasha knows the number m. Besides, x and m are positive integers. 

  Natasha can ask the rocket questions. Every question is an integer y (1≤y≤m). The correct answer to the question is −1, `if x<y, 0, if x=y, and 1, if x>y.` But the rocket is broken — it does not always answer correctly. Precisely: let the correct answer to the current question be equal to t, then, if the rocket answers this question correctly, then it will answer t, otherwise it will answer −t. 

  In addition, the rocket has a sequence p of length n. Each element of the sequence is either 0 or 1. The rocket processes this sequence in the cyclic order, that is 1-st element, 2-nd, 3-rd, …, (n−1)-th, n-th, 1-st, 2-nd, 3-rd, …, (n−1)-th, n-th, …. If the current element is 1, the rocket answers correctly, if 0 — lies. Natasha doesn’t know the sequence p, but she knows its length — n. 

  You can ask the rocket no more than 60 questions. 

  Help Natasha find the distance to Mars. Assume, that the distance to Mars does not change while Natasha is asking questions. 

  Your solution will not be accepted, if it does not receive an answer 0 from the rocket (even if the distance to Mars is uniquely determined by the already received rocket’s answers). 

  Input 

  The first line contains two integers m and n (1≤m≤109, 1≤n≤30) — the maximum distance to Mars and the number of elements in the sequence p. 

  Interaction 

  You can ask the rocket no more than 60 questions. 

  To ask a question, print a number y (1≤y≤m) and an end-of-line character, then do the operation flush and read the answer to the question. 

  If the program reads 0, then the distance is correct and you must immediately terminate the program (for example, by calling exit(0)). If you ignore this, you can get any verdict, since your program will continue to read from the closed input stream. 

  If at some point your program reads −2 as an answer, it must immediately end (for example, by calling exit(0)). You will receive the “Wrong answer” verdict, and this will mean that the request is incorrect or the number of requests exceeds 60. If you ignore this, you can get any verdict, since your program will continue to read from the closed input stream. 

  If your program’s request is not a valid integer between −231 and 231−1 (inclusive) without leading zeros, then you can get any verdict. 

  You can get “Idleness limit exceeded” if you don’t print anything or if you forget to flush the output. 

  To flush the output buffer you can use (after printing a query and end-of-line): 

  fflush(stdout) in C++; 

  System.out.flush() in Java; 

  stdout.flush() in Python; 

  flush(output) in Pascal; 

  See the documentation for other languages. 

  Hacking 

  Use the following format for hacking: 

  In the first line, print 3 integers m,n,x (1≤x≤m≤109, 1≤n≤30) — the maximum distance to Mars, the number of elements in the sequence p and the current distance to Mars. 

  In the second line, enter n numbers, each of which is equal to 0 or 1 — sequence p. 

  The hacked solution will not have access to the number x and sequence p. 

  Example 

  Input 

  5 2 

  1 

  -1 

  -1 

  1 

  0 

  Output 

  1 

  2 

  4 

  5 

  3 

  Note 

  In the example, hacking would look like this: 

  5 2 3 

  1 0 

  This means that the current distance to Mars is equal to 3, Natasha knows that it does not exceed 5, and the rocket answers in order: correctly, incorrectly, correctly, incorrectly … 

  Really: 

  on the first query (1) the correct answer is 1, the rocket answered correctly: 1; 

  on the second query (2) the correct answer is 1, the rocket answered incorrectly: −1; 

  on the third query (4) the correct answer is −1, the rocket answered correctly: −1; 

  on the fourth query (5) the correct answer is −1, the rocket answered incorrectly: 1; 

  on the fifth query (3) the correct and incorrect answer is 0.
这是一到交互题，什么是交互题呢  其实我也不知道，应该就是你的输入同时就会有输出，然后你还得根据这些输出来改变你程序的输入 

这题其实就是猜数字，但是有一个难点是计算机可能会说谎，但还好他的说慌是有规律的，而且猜的这个数字有下界是1，那么我们刚开始全部猜1 计算机要不说谎的话肯定得返回大于 所以就可以记录下说谎的位置了  

然后再二分数字进行询问就可以了
代码：

```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
using namespace std;
int query(int y){
    printf("%d\n",y);
    //关闭输出流
    fflush(stdout);
    int t;
    //读取反馈的答案
    scanf("%d",&t);
    if(t==0 || t==-2){
        exit(0);
    }
    return t;
}
int main(void){
    int m,n;
    scanf("%d%d",&m,&n);
    int p[35];
    for(int i=0;i<n;i++){
        //n次询问1
        int t=query(1);
        //因为x必须大于1 返回值如果不是1说明说谎
        p[i]=t==1;
    }
    int l=2,r=m;
    for(int q=0;;q++){
        int y=(l+r)/2;
        //获取反馈
        int t=query(y);
        //看是否说谎，如果说谎则反馈要反过来看
        if(!p[q%n]){
            t*=-1;
        }
        if(t==1){
            l=y+1;
        }
        else{
            r=y-1;
        }
    }
    return 0;
}
```






