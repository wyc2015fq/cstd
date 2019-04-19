# c/c++处理命令行参数int argc, char **argv的方法 - Koma Hub - CSDN博客
2018年04月13日 13:38:38[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：87
***第一种：switch***
```cpp
#include<iostream>
#include<string>
#include<cstring>
#include<cmath>
#include<complex>
#include<algorithm>
#include<vector>
#include<utility>
#include<cstddef> /*size_t*/
#include<bitset>
#include<assert.h>
#include<cstdlib>//abort()
#include<list>
#include<map>
#include<deque>
#include<set>
#include<iterator>
#include<exception>
#include<stack>
#include<queue>
#include<ctype.h>//atoi()
using namespace std;
void usage(){
    cout<<"usage"<<endl;
}
int main(int argc, char*argv[])
/* Rong Tao 2018.03.17 */
{
    string version("");
    bool debug = false;
    for (int i=1;i<argc;i++)
    {
        char *pchar = argv[i];
        switch(pchar[0]){
            case '-': {
                switch(pchar[1]){
                    case'd':debug = true;
                            break;
                    case'v':version = string("gcc-4.8.5");
                            break;
                    case'h':usage();
                            break;
                    default:break;
                }
            }
            default: cout<<""<<endl;
        }
    }
    if(debug)
        cout<<version<<endl;
    return 0;
}
```
**第二种：unistd.h：getopt **（参见：[http://c.biancheng.net/cpp/u/hs12/](http://c.biancheng.net/cpp/u/hs12/)）
```cpp
#include <stdio.h>
#include <unistd.h>
int main(int argc, char **argv)
{
   int ch;
   opterr = 0;
   
   /*
   定义函数：int getopt(int argc, char * const argv[], const char * optstring);
	函数说明：getopt()用来分析命令行参数。
	1、参数argc 和argv 是由main()传递的参数个数和内容。
	2、参数optstring 则代表欲处理的选项字符串
   */
   while((ch = getopt(argc, argv, "a:bcde")) != -1)
   switch(ch)
   {
      case 'a':
         printf("option a:'%s'\n", optarg);  break;
      case 'b':
         printf("option b:'%s'\n", optarg);  break;
      default:
         printf("other option :%c\n", ch);
   }
   printf("optopt +%c\n", optopt);
}
```
