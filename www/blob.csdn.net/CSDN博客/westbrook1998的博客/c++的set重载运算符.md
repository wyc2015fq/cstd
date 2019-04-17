# c++的set重载运算符 - westbrook1998的博客 - CSDN博客





2018年08月18日 23:35:02[westbrook1998](https://me.csdn.net/westbrook1998)阅读数：738








set 容器模版需要3个泛型参数，如下： 
`template<class T, class C, class A> class set;`

第一个T 是元素类型，必选； 

第二个C 指定元素比较方式，缺省为 Less, 即使用 < 符号比较； 

第三个A 指定空间分配对象，一般使用默认类型。 

因此: 

(1) 如果第2个泛型参数你使用默认值的话，你的自定义元素类型需要重载 < 运算操作； 

（2）如果你第2个泛型参数不使用默认值的话，则比较对象必须具有 () 操作，即： 
`bool operator()(const T &a, const T &b)`
例子：

```cpp
#include <cstdio>
#include <algorithm>
#include <set>
using namespace std;
int m,k;
struct cmp{
    bool operator() (int a,int b){
        if(abs(a-b)<=k){
            return false;
        }
        else{
            return a<b;
        }
    }
};
set<int,cmp> s;
char op[10];
int x;
int main(void){
    scanf("%d%d",&m,&k);
    while(m--){
        scanf("%s%d",op,&x);
        if(op[0]=='a'){
            if(s.find(x)==s.end()){
                s.insert(x);
            }
        }
        else if(op[0]=='d'){
            s.erase(x);
        }
        else{
            if(s.find(x)!=s.end()){
                puts("Yes");
            }
            else{
                puts("No");
            }
        }
    }
    return 0;
}
```




