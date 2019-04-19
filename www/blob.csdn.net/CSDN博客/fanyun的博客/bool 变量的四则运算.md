# bool 变量的四则运算 - fanyun的博客 - CSDN博客
2017年11月30日 20:55:37[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：488
所属专栏：[C/C++/C#开发实战365](https://blog.csdn.net/column/details/c-plus-plus-01.html)
                
测试项目：
1.bool 变量的 四则运算
2.bool 变量所在内存大小
测试代码：
#include <iostream> 
using namespace std; 
int main() 
 { 
   int iSizeCouner = 0; 
    bool bTestBool = false; 
    bool a = 3; 
    bool b = 1; 
     bool c = a + b; 
    bool d = a - b; 
     // test  
 cout << "a = " << a <<endl; 
  cout << "b = " << b<<endl; 
   cout << "c = " << c<<endl; 
   cout << "d = " << d <<endl;   
iSizeCouner = sizeof(bTestBool); 
   cout << iSizeCouner <<endl; 
   cout << bTestBool <<endl; 
   cout << bTestBool <<endl; 
    bTestBool = true; 
    cout <<boolalpha<<bTestBool <<endl; 
    while(1); 
} 
打印输出：
a = 1
b = 1
c = 1
d = 0
iSizeCouner = 1
bTestBool     = 0
结论：
1.在四则运算中始终都是 0，1 参与运算
2.bool 变量 所占字节大小 为1 字节
            
