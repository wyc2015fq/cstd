# Codeforces1009B——Minimum Ternary String - westbrook1998的博客 - CSDN博客





2018年08月19日 19:19:33[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：27标签：[构造字符串](https://so.csdn.net/so/search/s.do?q=构造字符串&t=blog)
个人分类：[ACM练习题](https://blog.csdn.net/westbrook1998/article/category/7652684)









> 
You are given a ternary string (it is a string which consists only of characters ‘0’, ‘1’ and ‘2’). 

  You can swap any two adjacent (consecutive) characters ‘0’ and ‘1’ (i.e. replace “01” with “10” or vice versa) or any two adjacent (consecutive) characters ‘1’ and ‘2’ (i.e. replace “12” with “21” or vice versa). 

  For example, for string “010210” we can perform the following moves: 

  “010210” → “100210”; 

  “010210” → “001210”; 

  “010210” → “010120”; 

  “010210” → “010201”. 

  Note than you cannot swap “02” → “20” and vice versa. You cannot perform any other operations with the given string excluding described above. 

  You task is to obtain the minimum possible (lexicographically) string by using these swaps arbitrary number of times (possibly, zero). 

  String a is lexicographically less than string b (if strings a and b have the same length) if there exists some position i (1≤i≤|a|, where |s| is the length of the string s) such that for every j
```cpp
#include <cstdio>
#include <algorithm>
#include <cstring>
const int N=1e5+20;
char a[N];
int ze;
int zy;
int er[N];
int idx;
bool flag;
bool has;
int main(void){
    scanf("%s",a);
    int l=strlen(a);
    for(int i=0;i<l;i++){
        if(a[i]=='1'){
            zy++;
        }
        else if(a[i]=='0'){
            if(flag){
                er[idx]++;
                //printf("%d\n",er[idx]);
            }
            else{
                ze++;
            }
        }
        else if(a[i]=='2'){
            has=true;
            if(flag){
                idx++;
            }
            else{
                flag=true;
            }
        }
    }
    for(int i=0;i<ze;i++){
        printf("0");
    }
    for(int i=0;i<zy;i++){
        printf("1");
    }
    for(int i=0;i<=idx;i++){
        if(has){
            printf("2");
        }
        for(int j=0;j<er[i];j++){
            printf("0");
        }
    }
    printf("\n");
    return 0;
}
```





