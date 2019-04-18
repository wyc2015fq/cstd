# 运用std::string 实现split功能 - writeeee的专栏 - CSDN博客
2016年05月17日 23:56:30[writeeee](https://me.csdn.net/writeeee)阅读数：739
```cpp
#include <iostream>
#include <string>
#include <vector>
using namespace std;
int main()
{
    string str,substr;    
    vector<string> vstr;
    char separator=',';
    int start=0,
    int index=0;
    cout<<"input some numbers";    
    cin>>str;
    do {
        index = str.find_first_of(&separator,start);
        if (index !=string::npos) {
           substr= str.substr(start,index-start);
           vstr.push_back(substr);
           start = str.find_first_not_of(&separator,index);
           if (start == string::npos) {
               break;
           }
        }
    }while(index !=string::npos);
    //当最后不是以‘，’　结尾的时候，获取最后一个数字
    if (start !=string::npos) {
        substr=str.substr(start);
        vstr.push_back(substr);
    }
     for (vector<string>::iterator iter = vstr.begin(); iter !=vstr.end(); iter++) {
        cout<<*iter<<endl;
    }
```
参考
> 
[http://blog.csdn.net/pj81102/article/details/5836066](http://blog.csdn.net/pj81102/article/details/5836066)
